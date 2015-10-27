#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "dct_co.h"

pthread_mutex_t thread_counter_mutex;
pthread_cond_t thread_counter_cv;
int thread_counter;

typedef struct {
	dct_data_t *buf_2d_in;
	dct_data_t *row_outbuf;
	dct_data_t *col_inbuf;
	dct_data_t *nii_block;
} Params;

typedef struct {
	dct_data_t *col_inbuf;
	dct_data_t *col_outbuf;
	dct_data_t *buf_2d_out;
	dct_data_t *nii_block;
	dct_data_t *input;
	dct_data_t *output;
} TP0Params;

static void sync_threads();

/*
void dct_1d(dct_data_t src[DCT_SIZE], dct_data_t dst[DCT_SIZE])
{
	unsigned int k, n;
	int tmp;
	const dct_data_t dct_coeff_table[DCT_SIZE][DCT_SIZE] = {
#include "dct_coeff_table.txt"
	};

	for (k = 0; k < DCT_SIZE; k++) {
		for(n = 0, tmp = 0; n < DCT_SIZE; n++) {
			int coeff = (int)dct_coeff_table[k][n];
			tmp += src[n] * coeff;
		}
		dst[k] = DESCALE(tmp, CONST_BITS);
	}
}
*/
void dct_1d(dct_data_t *src, dct_data_t *dst)
{
	unsigned int k, n;
	int tmp;
	const dct_data_t dct_coeff_table[DCT_SIZE][DCT_SIZE] = {
#include "dct_coeff_table.txt"
	};

	for (k = 0; k < DCT_SIZE; k++) {
		for(n = 0, tmp = 0; n < DCT_SIZE; n++) {
			int coeff = (int)dct_coeff_table[k][n];
			tmp += src[n] * coeff;
		}
		dst[k] = DESCALE(tmp, CONST_BITS);
	}
}

/*
void read_data(int input[N], int buf[DCT_SIZE][DCT_SIZE])
{
	int r, c;

	for (r = 0; r < DCT_SIZE; r++) {
		for (c = 0; c < DCT_SIZE; c++)
			buf[r][c] = input[r * DCT_SIZE + c];
	}
}
*/
void read_data(dct_data_t *input, dct_data_t *buf)
{
	int k, r, c;

	for (k = 0; k < SFN; k++) {
	for (r = 0; r < DCT_SIZE; r++) {
		for (c = 0; c < DCT_SIZE; c++)
			buf[k * DCT_SIZE * DCT_SIZE + r * DCT_SIZE + c] = 
				input[k * DCT_SIZE * DCT_SIZE + r * DCT_SIZE + c];
	}
	}
}

/*
void write_data(int buf[DCT_SIZE][DCT_SIZE], int output[N])
{
	int r, c;

	for (r = 0; r < DCT_SIZE; r++) {
		for (c = 0; c < DCT_SIZE; c++)
			output[r * DCT_SIZE + c] = buf[r][c];
	}
}
*/
void write_data(dct_data_t *buf, dct_data_t *output)
{
	int k, r, c;

	for (k = 0; k < SFN; k++) {
	for (r = 0; r < DCT_SIZE; r++) {
		for (c = 0; c < DCT_SIZE; c++)
			output[k * DCT_SIZE * DCT_SIZE + r * DCT_SIZE + c] =
				buf[k * DCT_SIZE * DCT_SIZE + r * DCT_SIZE + c];
	}
	}
}

/*
void dct_2d(int in_block[DCT_SIZE][DCT_SIZE],
			int out_block[DCT_SIZE][DCT_SIZE])
{
	int i;
	
	for (i = 0; i < DCT_SIZE; i++) {
		dct_1d(in_block[i], out_block[i]);
	}
}
*/
void dct_2d(dct_data_t *in_block, dct_data_t *out_block)
{
	int k, i;
	
	for (k = 0; k < SFN; k++) {
	for (i = 0; i < DCT_SIZE; i++) {
		dct_1d(in_block + k * DCT_SIZE * DCT_SIZE + i * DCT_SIZE,
				out_block + k * DCT_SIZE * DCT_SIZE + i * DCT_SIZE);
	}
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

void *half_dct_2d_pth_1(void *params)
{
	int i;
	Params *p = (Params *)params;

#ifdef FPGA
	int fdr, fdw;
	dct_data_t *in, *out;
	int len;

	fdr = open("/dev/xillybus_read_8", O_RDONLY);
	fdw = open("/dev/xillybus_write_8", O_WRONLY);
	if ((fdr < 0) || (fdw < 0)) {
		perror("Failed to open Xillybus device file(s)");
		exit(1);
	}
	len = SFN * DCT_SIZE * DCT_SIZE;
	in = (dct_data_t *)malloc((1 + len) * sizeof(dct_data_t));
	in[0] = 1;
	for (i = 1; i <= len; i++)
		in[i] = p->buf_2d_in[i - 1];
	out = p->row_outbuf;
#endif

	for (i = 1; i < FF; i++) {
#ifdef FPGA
		write(fdw, (void *)in, (len + 1) * sizeof(dct_data_t));
		read(fdr, (void *)out, len * sizeof(dct_data_t));
#else
		dct_2d(p->buf_2d_in, p->row_outbuf);
#endif
		transpose(p->row_outbuf, p->col_inbuf);
		
		sync_threads();
		
		if (i + 1 < FF) {
			block_memcpy(p->col_inbuf, p->nii_block);
		}

		//	sync_threads();
	}

#ifdef FPGA
	free(in);

	close(fdr);
	close(fdw);
#endif

	return NULL;
}

void *half_dct_2d_pth_0(void *params)
{
	int i;
	TP0Params *p = (TP0Params *)params;

#ifdef FPGA
	int fdr, fdw;
	dct_data_t *in, *out;
	int len;

	fdr = open("/dev/xillybus_read_32", O_RDONLY);
	fdw = open("/dev/xillybus_write_32", O_WRONLY);
	if ((fdr < 0) || (fdw < 0)) {
		perror("Failed to open Xillybus device file(s)");
		exit(1);
	}
	len = SFN * DCT_SIZE * DCT_SIZE;
	in = (dct_data_t *)malloc((1 + len) * sizeof(dct_data_t));
	in[0] = 0;
	for (i = 1; i <= len; i++)
		in[i] = p->col_inbuf[i - 1];
	out = p->col_outbuf;
#endif

	for (i = 1; i < FF; i++) {
#ifdef FPGA
		write(fdw, (void *)in, (len + 1) * sizeof(dct_data_t));
		read(fdr, (void *)out, len * sizeof(dct_data_t));
#else
		dct_2d(p->col_inbuf, p->col_outbuf);
#endif

		transpose(p->col_outbuf, p->buf_2d_out);
		
		sync_threads();
		
		if (i + 1 < FF) {
			read_data(p->input + (i + 1) * SFN * N, p->nii_block);
		}
		write_data(p->buf_2d_out, p->output + (i - 1) * SFN * N);

		//	sync_threads();
	}

#ifdef FPGA
	free(in);

	close(fdr);
	close(fdw);
#endif

	return NULL;
}

void transpose(dct_data_t *in_block, dct_data_t *out_block)
{
	int k, i, j;
	
	for (k = 0; k < SFN; k++) {
	for (i = 0; i < DCT_SIZE; i++)
		for(j = 0; j < DCT_SIZE; j++)
			out_block[k * DCT_SIZE * DCT_SIZE + i * DCT_SIZE + j] = 
				in_block[k * DCT_SIZE * DCT_SIZE + j * DCT_SIZE + i];
	}
}

/*
void block_memcpy(int in_block[DCT_SIZE][DCT_SIZE],
			   int out_block[DCT_SIZE][DCT_SIZE])
{
	int i, j;
	
	for (i = 0; i < DCT_SIZE; i++)
		for(j = 0; j < DCT_SIZE; j++)
			out_block[i][j] = in_block[i][j];
}
*/
void block_memcpy(dct_data_t *in_block, dct_data_t *out_block)
{
	int k, i, j;
	
	for (k = 0; k < SFN; k++) {
	for (i = 0; i < DCT_SIZE; i++)
		for(j = 0; j < DCT_SIZE; j++)
			out_block[k * DCT_SIZE * DCT_SIZE + i * DCT_SIZE + j] =
				in_block[k * DCT_SIZE * DCT_SIZE + i * DCT_SIZE + j];
	}
}

void dct(dct_data_t input[DW * N], dct_data_t output[DW * N])
{
	pthread_t thread_id[D];
	Params thread_args[D];
	TP0Params tp0_thread_args;
   
//	int buf_2d_in[D][DCT_SIZE][DCT_SIZE];
//	int buf_2d_out[D][DCT_SIZE][DCT_SIZE];
	dct_data_t *buf_2d_in0, *buf_2d_in1;
	dct_data_t *buf_2d_out0, *buf_2d_out1;

//	dct_data_t row_outbuf[D][DCT_SIZE][DCT_SIZE];
//	dct_data_t col_outbuf[D][DCT_SIZE][DCT_SIZE], col_inbuf[D][DCT_SIZE][DCT_SIZE];
	dct_data_t *row_outbuf0, *row_outbuf1;
	dct_data_t *col_outbuf0, *col_outbuf1;
	dct_data_t *col_inbuf0, *col_inbuf1;

	buf_2d_in0 = (dct_data_t *)malloc(SFN * DCT_SIZE * DCT_SIZE * sizeof(dct_data_t));
	buf_2d_in1 = (dct_data_t *)malloc(SFN * DCT_SIZE * DCT_SIZE * sizeof(dct_data_t));
	buf_2d_out0 = (dct_data_t *)malloc(SFN * DCT_SIZE * DCT_SIZE * sizeof(dct_data_t));
	buf_2d_out1 = (dct_data_t *)malloc(SFN * DCT_SIZE * DCT_SIZE * sizeof(dct_data_t));
	row_outbuf0 = (dct_data_t *)malloc(SFN * DCT_SIZE * DCT_SIZE * sizeof(dct_data_t));
	row_outbuf1 = (dct_data_t *)malloc(SFN * DCT_SIZE * DCT_SIZE * sizeof(dct_data_t));
	col_outbuf0 = (dct_data_t *)malloc(SFN * DCT_SIZE * DCT_SIZE * sizeof(dct_data_t));
	col_outbuf1 = (dct_data_t *)malloc(SFN * DCT_SIZE * DCT_SIZE * sizeof(dct_data_t));
	col_inbuf0 = (dct_data_t *)malloc(SFN * DCT_SIZE * DCT_SIZE * sizeof(dct_data_t));
	col_inbuf1 = (dct_data_t *)malloc(SFN * DCT_SIZE * DCT_SIZE * sizeof(dct_data_t));

	thread_args[1].buf_2d_in = buf_2d_in1;
	thread_args[1].row_outbuf = row_outbuf1;
	thread_args[1].col_inbuf = col_inbuf1;
	thread_args[1].nii_block = col_inbuf0;

	tp0_thread_args.col_inbuf = col_inbuf0;
	tp0_thread_args.col_outbuf = col_outbuf0;
	tp0_thread_args.buf_2d_out = buf_2d_out0;
	tp0_thread_args.nii_block = buf_2d_in1;
	tp0_thread_args.input = input;
	tp0_thread_args.output = output;

	// pre operations
	read_data(input + 0 * SFN * N, buf_2d_in0);
	dct_2d(buf_2d_in0, row_outbuf0);
	transpose(row_outbuf0, col_inbuf0);

	read_data(input + 1 * SFN * N, buf_2d_in1);

	pthread_mutex_init(&thread_counter_mutex, NULL);
	pthread_cond_init(&thread_counter_cv, NULL);
	thread_counter = 0;	// used in sync_threads()

	pthread_create(&thread_id[1], NULL, &half_dct_2d_pth_1, &thread_args[1]);
	pthread_create(&thread_id[0], NULL, &half_dct_2d_pth_0, &tp0_thread_args);

	pthread_join(thread_id[1], NULL);
	pthread_join(thread_id[0], NULL);
	
	//post operations
	dct_2d(col_inbuf1, col_outbuf1);
	transpose(col_outbuf1, buf_2d_out1);
	write_data(buf_2d_out1, output + (DW - 1 * SFN) * N);

	free(buf_2d_in0);
	free(buf_2d_in1);
	free(buf_2d_out0);
	free(buf_2d_out1);
	free(row_outbuf0);
	free(row_outbuf1);
	free(col_outbuf0);
	free(col_outbuf1);
	free(col_inbuf0);
	free(col_inbuf1);
}

