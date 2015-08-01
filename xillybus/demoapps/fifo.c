#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/*********************************************************************
 *                                                                   *
 *                 D E C L A R A T I O N S                           *
 *                                                                   *
 *********************************************************************/

struct xillyfifo {
  unsigned long read_total;
  unsigned long write_total;
  unsigned int bytes_in_fifo;
  unsigned int read_position;
  unsigned int write_position;
  unsigned int size;
  unsigned int done;
  unsigned char *baseaddr;
  sem_t write_sem;
  sem_t read_sem;
};

struct xillyinfo {
  int slept;
  int bytes;
  int position;
  void *addr;
};

#define FIFO_BACKOFF 0
static int read_fd = 0;

/*********************************************************************
 *                                                                   *
 *                 A P I   F U N C T I O N S                         *
 *                                                                   *
 *********************************************************************/

// IMPORTANT:
// =========
//
// NEITHER of the fifo_* functions is reentrant. Only one thread should have
// access to any set of them. This is pretty straightforward when one thread
// writes and one thread reads from the FIFO.
//
// Also make sure that fifo_drained() and fifo_wrote() are NEVER called with
// req_bytes larger than what their request-counterparts RETURNED, or
// things will go crazy pretty soon.


int fifo_init(struct xillyfifo *fifo,
	      unsigned int size) {

  fifo->baseaddr = NULL;
  fifo->size = 0;
  fifo->bytes_in_fifo = 0;
  fifo->read_position = 0;
  fifo->write_position = 0;
  fifo->read_total = 0;
  fifo->write_total = 0;
  fifo->done = 0;

  if (sem_init(&fifo->read_sem, 0, 0) == -1)
    return -1; // Fail!

  if (sem_init(&fifo->write_sem, 0, 1) == -1)
    return -1;

  fifo->baseaddr = malloc(size);

  if (!fifo->baseaddr)
    return -1;

  if (mlock(fifo->baseaddr, size)) {
    unsigned int i;
    unsigned char *buf = fifo->baseaddr;

    fprintf(stderr, "Warning: Failed to lock RAM, so FIFO's memory may swap to disk.\n"
	    "(You may want to use ulimit -l)\n");

    // Write something every 1024 bytes (4096 should be OK, actually).
    // Hopefully all pages are in real RAM after this. Better than nothing.

    for (i=0; i<size; i+=1024)
      buf[i] = 0;
  }

  fifo->size = size;

  return 0; // Success
}

void fifo_done(struct xillyfifo *fifo) {
  fifo->done = 1;
  sem_post(&fifo->read_sem);
  sem_post(&fifo->write_sem);
}

void fifo_destroy(struct xillyfifo *fifo) {
  if (!fifo->baseaddr)
    return; // Better safe than SEGV

  munlock(fifo->baseaddr, fifo->size);
  free(fifo->baseaddr);

  sem_destroy(&fifo->read_sem);
  sem_destroy(&fifo->write_sem);

  fifo->baseaddr = NULL;
}

int fifo_request_drain(struct xillyfifo *fifo,
		       struct xillyinfo *info) {
  int taken = 0;
  unsigned int now_bytes, max_bytes;

  info->slept = 0;
  info->addr = NULL;

  now_bytes = __sync_add_and_fetch(&fifo->bytes_in_fifo, 0);

  while (now_bytes == 0) {
    if (fifo->done)
      goto fail; // FIFO will not be used by other side, and is empty

    // fifo_wrote() updates bytes_in_fifo and then increments semaphore,
    // so there's no chance for oversleeping. On the other hand, it's
    // possible that the data was drained between the bytes_in_fifo
    // update and the semaphore increment, leading to a false wakeup.
    // That's why we're in a while loop ( + other race conditions).

    info->slept = 1;

    if (sem_wait(&fifo->read_sem) && (errno != EINTR))
      goto fail;

    now_bytes = __sync_add_and_fetch(&fifo->bytes_in_fifo, 0);
  }

  max_bytes = fifo->size - fifo->read_position;
  taken = (now_bytes < max_bytes) ? now_bytes : max_bytes;
  info->addr = fifo->baseaddr + fifo->read_position;

 fail:
  info->bytes = taken;
  info->position = fifo->read_position;

  return taken;
}

void fifo_drained(struct xillyfifo *fifo,
		 unsigned int req_bytes) {

  int semval;

  if (req_bytes == 0)
    return;

  __sync_sub_and_fetch(&fifo->bytes_in_fifo, req_bytes);
  __sync_add_and_fetch(&fifo->read_total, req_bytes);

  fifo->read_position += req_bytes;

  if (fifo->read_position >= fifo->size)
    fifo->read_position -= fifo->size;

  if (sem_getvalue(&fifo->write_sem, &semval))
    semval = 1; // This fallback should never happen

  // Don't increment the semaphore if it's nonzero anyhow. The possible
  // race condition between reading and possibly incrementing has no effect.

  if (semval == 0)
    sem_post(&fifo->write_sem);
}

int fifo_request_write(struct xillyfifo *fifo,
		       struct xillyinfo *info) {
  int taken = 0;
  unsigned int now_bytes, max_bytes;

  info->slept = 0;
  info->addr = NULL;

  now_bytes = __sync_add_and_fetch(&fifo->bytes_in_fifo, 0);

  if (fifo->done)
    goto fail; // No point filling an abandoned FIFO

  while (now_bytes >= (fifo->size - FIFO_BACKOFF)) {
    // fifo_drained() updates bytes_in_fifo and then increments semaphore,
    // so there's no chance for oversleeping. On the other hand, it's
    // possible that the data was drained between the bytes_in_fifo
    // update and the semaphore increment, leading to a false wakeup.
    // That's why we're in a while loop ( + other race conditions).

    info->slept = 1;

    if (sem_wait(&fifo->write_sem) && (errno != EINTR))
      goto fail;

    if (fifo->done)
      goto fail; // No point filling an abandoned FIFO

    now_bytes = __sync_add_and_fetch(&fifo->bytes_in_fifo, 0);
  }

  taken = fifo->size - (now_bytes + FIFO_BACKOFF);

  max_bytes = fifo->size - fifo->write_position;

  if (taken > max_bytes)
    taken = max_bytes;
  info->addr = fifo->baseaddr + fifo->write_position;

 fail:
  info->bytes = taken;
  info->position = fifo->write_position;

  return taken;
}

void fifo_wrote(struct xillyfifo *fifo,
		 unsigned int req_bytes) {

  int semval;

  if (req_bytes == 0)
    return;

  __sync_add_and_fetch(&fifo->bytes_in_fifo, req_bytes);
  __sync_add_and_fetch(&fifo->write_total, req_bytes);

  fifo->write_position += req_bytes;

  if (fifo->write_position >= fifo->size)
    fifo->write_position -= fifo->size;

  if (sem_getvalue(&fifo->read_sem, &semval))
    semval = 1; // This fallback should never happen

  // Don't increment the semaphore if it's nonzero anyhow. The possible
  // race condition between reading and possibly incrementing has no effect.

  if (semval == 0)
    sem_post(&fifo->read_sem);
}

/*********************************************************************
 *                                                                   *
 *                 A P P L I C A T I O N   C O D E                   *
 *                                                                   *
 *********************************************************************/

// Read from FIFO, write to standard output

void *write_thread(void *arg)
{
  struct xillyfifo *fifo = arg;
  int do_bytes, written_bytes;
  struct xillyinfo info;
  unsigned char *buf;

  while (1) {
    do_bytes = fifo_request_drain(fifo, &info);

    if (do_bytes == 0)
      return NULL;

    for (buf = info.addr; do_bytes > 0;
	 buf += written_bytes, do_bytes -= written_bytes) {

      written_bytes = write(1, buf, do_bytes);

      if ((written_bytes < 0) && (errno != EINTR)) {
	perror("write() failed");
	return NULL;
      }

      if (written_bytes == 0) {
	fprintf(stderr, "Reached write EOF (?!)\n");
	fifo_done(fifo);
	return NULL;
      }

      if (written_bytes < 0) { // errno is EINTR
	written_bytes = 0;
	continue;
      }

      fifo_drained(fifo, written_bytes);
    }
  }
}

// Write to FIFO, read from standard output

void *read_thread(void *arg)
{
  struct xillyfifo *fifo = arg;
  int do_bytes, read_bytes;
  struct xillyinfo info;
  unsigned char *buf;

  while (1) {
    do_bytes = fifo_request_write(fifo, &info);

    if (do_bytes == 0)
      return NULL;

    for (buf = info.addr; do_bytes > 0;
	 buf += read_bytes, do_bytes -= read_bytes) {

      read_bytes = read(read_fd, buf, do_bytes);

      if ((read_bytes < 0) && (errno != EINTR)) {
	perror("read() failed");
	return NULL;
      }

      if (read_bytes == 0) {
	// Reached EOF. Quit without complaining.
	fifo_done(fifo);
	return NULL;
      }

      if (read_bytes < 0) { // errno is EINTR
	read_bytes = 0;
	continue;
      }

      fifo_wrote(fifo, read_bytes);
    }
  }
}


void *status_thread(void *arg) {
  struct xillyfifo *fifo = arg;

  while (fifo->done < 2)
    fprintf(stderr, "%9d bytes in FIFO, %12ld read, %12ld written\r",
	    __sync_add_and_fetch(&fifo->bytes_in_fifo, 0),
	    __sync_add_and_fetch(&fifo->read_total, 0),
	    __sync_add_and_fetch(&fifo->write_total, 0)
	    );
  return NULL;
}

int main(int argc, char *argv[]) {
  pthread_t tid[3];
  struct xillyfifo fifo;
  unsigned int fifo_size;

  if ((argc != 2) && (argc != 3)) {
    fprintf(stderr, "Usage: %s fifo_size [read-file]\n", argv[0]);
    exit(1);
  }

  fifo_size = atoi(argv[1]);

  if (fifo_size == 0) {
    fprintf(stderr, "Bad fifo_size argument %s\n", argv[1]);
    exit(1);
  }

  if (fifo_init(&fifo, fifo_size)) {
    perror("Failed to init");
    exit(1);
  }

  if (argc > 2) {
    read_fd = open(argv[2], O_RDONLY);

    if (read_fd < 0) {
      perror("Failed to open read file");
      exit(1);
    }
  }

  if (pthread_create(&tid[0], NULL, read_thread, &fifo)) {
    perror("Failed to create thread");
    exit(1);
  }

  if (pthread_create(&tid[1], NULL, write_thread, &fifo)) {
    perror("Failed to create thread");
    exit(1);
  }

  if (pthread_create(&tid[2], NULL, status_thread, &fifo)) {
    perror("Failed to create thread");
    exit(1);
  }

  pthread_join(tid[0], NULL);
  pthread_join(tid[1], NULL);

  fifo.done = 2; // This is a hack for the status thread
  pthread_join(tid[2], NULL);

  fifo_destroy(&fifo);

  pthread_exit(NULL);

  return 0;
}
