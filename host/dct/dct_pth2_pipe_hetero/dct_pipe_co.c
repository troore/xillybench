#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <pthread.h>
#include <semaphore.h>

#include "dct.h"

pthread_mutex_t thread_counter_mutex;
pthread_cond_t thread_counter_cv;
int thread_counter;

typedef struct {
	dct_data_t *pin_block;
	dct_data_t *pout_block;
	dct_data_t *pnii_block;
} Params;

typedef struct {
	dct_data_t *pin_block;
	dct_data_t *pout_block;
	dct_data_t *pnii_block;
	dct_data_t *input;
	dct_data_t *output;
} TP0Params;

static void sync_threads();

void dct_1d(dct_data_t *src, dct_data_t *dst)
{
	unsigned int k, n;
	int tmp;
	const dct_data_t dct_coeff_table[DCT_SIZE][DCT_SIZE] = {
#include "../dct_coeff_table.txt"
	};

	for (k = 0; k < DCT_SIZE; k++) {
		for(n = 0, tmp = 0; n < DCT_SIZE; n++) {
			int coeff = (int)dct_coeff_table[k][n];
			tmp += src[n] * coeff;
		}
		dst[k] = DESCALE(tmp, CONST_BITS);
	}
}

void read_data(dct_data_t *input, dct_data_t *buf)
{
	int r, c;
//	int h;

//	for (h = 0; h < SFN; h++) {
	for (r = 0; r < DCT_SIZE; r++) {
		for (c = 0; c < DCT_SIZE; c++)
			buf[r * DCT_SIZE + c] = input[r * DCT_SIZE + c];
	}
//	}
}

void write_data(dct_data_t *buf, dct_data_t *output)
{
	int r, c;
//	int h;

//	for (h = 0; h < SFN; h++) {
	for (r = 0; r < DCT_SIZE; r++) {
		for (c = 0; c < DCT_SIZE; c++)
			output[r * DCT_SIZE + c] = buf[r * DCT_SIZE + c];
	}
//	}
}

void dct_2d(dct_data_t *in_block, dct_data_t *out_block)
{
	int i;
	
	for (i = 0; i < DCT_SIZE; i++) {
		dct_1d(in_block + i * DCT_SIZE, out_block + i * DCT_SIZE);
	}
}

static void sync_threads()
{
	pthread_mutex_lock(&thread_counter_mutex);
	thread_counter++;

	if (thread_counter < D)
		pthread_cond_wait(&thread_counter_cv, &thread_counter_mutex);

	pthread_mutex_unlock(&thread_counter_mutex);

	if (thread_counter == D) {
		thread_counter = 0;
		pthread_cond_broadcast(&thread_counter_cv);
	}
}

void *dct_2d_pth(void *params)
{
	int i, j;
	int h;
	Params *p = (Params *)params;

	/*
	int fdr, fdw;

	fdr = open("/dev/xillybus_read_32", O_RDONLY);
	fdw = open("/dev/xillybus_write_32", O_WRONLY);
	if ((fdr < 0) || (fdw < 0)) {
		perror("Failed to open Xillybus device file(s)");
		exit(1);
	}
	*/

	for (i = 1; i < FF; i++) {
		for (h = 0; h < SFN; h++) {
		for (j = 0; j < DCT_SIZE; j++) {
			dct_1d(p->pin_block + j * DCT_SIZE, p->pout_block + j * DCT_SIZE);
		}
		}
		/*
		write(fdw, (void *)p->pin_block , N * sizeof(dct_data_t));
		read(fdr, (void *)p->pout_block , N * sizeof(dct_data_t));
		*/
		
		sync_threads();
		
		if (i + 1 < FF) {
			block_memcpy(p->pout_block, p->pnii_block);
		}

		//	sync_threads();
	}

	/*
	close(fdr);
	close(fdw);
	*/

	return NULL;
}

void transpose(dct_data_t *in_block, dct_data_t *out_block)
{
	int i, j;
	
	for (i = 0; i < DCT_SIZE; i++)
		for(j = 0; j < DCT_SIZE; j++)
			out_block[i * DCT_SIZE + j] = in_block[j * DCT_SIZE + i];
}

void *transpose_pth(void *params)
{
	int i, j, k;
	int h;
	TP0Params *p = (TP0Params *)params;

	int fdr, fdw;

	fdr = open("/dev/xillybus_read_32", O_RDONLY);
	fdw = open("/dev/xillybus_write_32", O_WRONLY);
	if ((fdr < 0) || (fdw < 0)) {
		perror("Failed to open Xillybus device file(s)");
		exit(1);
	}

	for (i = 1; i < FF; i++) {
		/*
		for (h = 0; h < SFN; h++) {
		for (j = 0; j < DCT_SIZE; j++)
			for(k = 0; k < DCT_SIZE; k++)
				p->pout_block[j * DCT_SIZE + k] = p->pin_block[k * DCT_SIZE + j];
		}
		*/
		write(fdw, (void *)p->pin_block , N * sizeof(dct_data_t));
		read(fdr, (void *)p->pout_block , N * sizeof(dct_data_t));

		sync_threads();
		
		if (i + 1 < FF)
		{
			// get the next frame
			read_data(p->input + (i + 1) * N, p->pnii_block);
		}
		
		write_data(p->pout_block, p->output + i * N);

		//	sync_threads();
	}

	close(fdr);
	close(fdw);

	return NULL;
}

void block_memcpy(dct_data_t *in_block, dct_data_t *out_block)
{
	int i, j;
//	int h;
	
//	for (h = 0; h < SFN; h++) {
	for (i = 0; i < DCT_SIZE; i++)
		for(j = 0; j < DCT_SIZE; j++)
			out_block[i * DCT_SIZE + j] = in_block[i * DCT_SIZE + j];
//	}
}

void dct(dct_data_t *input, dct_data_t *output)
{
//	int i;
	pthread_t thread_id[D];
	Params thread_args[D];
	TP0Params tp0_thread_args;
   
	dct_data_t buf_2d_in[DCT_SIZE * DCT_SIZE];
	dct_data_t buf_2d_out[DCT_SIZE * DCT_SIZE];

	dct_data_t row_outbuf0[DCT_SIZE * DCT_SIZE];
	dct_data_t row_outbuf1[DCT_SIZE * DCT_SIZE];

	thread_args[1].pin_block = buf_2d_in;
	thread_args[1].pout_block = row_outbuf1;
	thread_args[1].pnii_block = row_outbuf0;

	tp0_thread_args.pin_block = row_outbuf0;
	tp0_thread_args.pout_block = buf_2d_out;
	tp0_thread_args.pnii_block = buf_2d_in;
	tp0_thread_args.input = input;
	tp0_thread_args.output = output;

	// pre operations
	read_data(input + 0 * N, buf_2d_in);
	dct_2d(buf_2d_in, row_outbuf0);

	read_data(input + 1 * N, buf_2d_in);

	pthread_mutex_init(&thread_counter_mutex, NULL);
	pthread_cond_init(&thread_counter_cv, NULL);
	thread_counter = 0;

//	pthread_create(&thread_id[3], NULL, &dct_2d_pth_3, &thread_args[3]);
//	pthread_create(&thread_id[2], NULL, &transpose_pth_2, &thread_args[2]);
	pthread_create(&thread_id[1], NULL, &dct_2d_pth, &thread_args[1]);
	pthread_create(&thread_id[0], NULL, &transpose_pth, &tp0_thread_args);

//	pthread_join(thread_id[3], NULL);
//	pthread_join(thread_id[2], NULL);
	pthread_join(thread_id[1], NULL);
	pthread_join(thread_id[0], NULL);
	
	//post operations
	transpose(row_outbuf1, buf_2d_out);
	write_data(buf_2d_out, output + (FF - 1) * N);
}

