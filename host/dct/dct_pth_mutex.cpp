#include <pthread.h>
#include <semaphore.h>

#include "dct.h"

pthread_mutex_t thread_counter_mutex;
pthread_cond_t thread_counter_cv;
int thread_counter;

typedef struct {
	dct_data_t (*pin_block)[DCT_SIZE];
	dct_data_t (*pout_block)[DCT_SIZE];
	dct_data_t (*pnii_block)[DCT_SIZE];
} Params;

typedef struct {
	dct_data_t (*pin_block)[DCT_SIZE];
	dct_data_t (*pout_block)[DCT_SIZE];
	dct_data_t (*pnii_block)[DCT_SIZE];
	dct_data_t *input;
	dct_data_t *output;
} TP0Params;

static void sync_threads();

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

/*
void dct_2d(dct_data_t in_block[DCT_SIZE][DCT_SIZE],
			dct_data_t out_block[DCT_SIZE][DCT_SIZE])
{
	dct_data_t row_outbuf[DCT_SIZE][DCT_SIZE];
	dct_data_t col_outbuf[DCT_SIZE][DCT_SIZE], col_inbuf[DCT_SIZE][DCT_SIZE];
	unsigned i, j;

	// DCT rows
	for(i = 0; i < DCT_SIZE; i++) {
		dct_1d(in_block[i], row_outbuf[i]);
	}
	// Transpose data in order to re-use 1D DCT code
	for (j = 0; j < DCT_SIZE; j++)
		for(i = 0; i < DCT_SIZE; i++)
			col_inbuf[j][i] = row_outbuf[i][j];
	// DCT columns
	for (i = 0; i < DCT_SIZE; i++) {
		dct_1d(col_inbuf[i], col_outbuf[i]);
	}
	// Transpose data back into natural order
	for (j = 0; j < DCT_SIZE; j++)
		for(i = 0; i < DCT_SIZE; i++)
			out_block[j][i] = col_outbuf[i][j];
}
*/

void read_data(short input[N], short buf[DCT_SIZE][DCT_SIZE])
{
	int r, c;

	for (r = 0; r < DCT_SIZE; r++) {
		for (c = 0; c < DCT_SIZE; c++)
			buf[r][c] = input[r * DCT_SIZE + c];
	}
}

void write_data(short buf[DCT_SIZE][DCT_SIZE], short output[N])
{
	int r, c;

	for (r = 0; r < DCT_SIZE; r++) {
		for (c = 0; c < DCT_SIZE; c++)
			output[r * DCT_SIZE + c] = buf[r][c];
	}
}

void dct_2d(short in_block[DCT_SIZE][DCT_SIZE],
			short out_block[DCT_SIZE][DCT_SIZE])
{
	int i;
	
	for (i = 0; i < DCT_SIZE; i++) {
		dct_1d(in_block[i], out_block[i]);
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

void *dct_2d_pth_3(void *params)
{
	int i, j;
	Params *p = (Params *)params;

	for (i = 3; i < DW; i++) {
		for (j = 0; j < DCT_SIZE; j++) {
			dct_1d(p->pin_block[j], p->pout_block[j]);
		}
		
		sync_threads();
		
		if (i + 1 < DW) {
			block_memcpy(p->pout_block, p->pnii_block);
		}

		//	sync_threads();
	}

	return NULL;
}

void *dct_2d_pth_1(void *params)
{
	int i, j;
	Params *p = (Params *)params;

	for (i = 3; i < DW; i++) {
		for (j = 0; j < DCT_SIZE; j++) {
			dct_1d(p->pin_block[j], p->pout_block[j]);
		}
		
		sync_threads();
		
		if (i + 1 < DW) {
			block_memcpy(p->pout_block, p->pnii_block);
		}

		//	sync_threads();
	}

	return NULL;
}

void transpose(short in_block[DCT_SIZE][DCT_SIZE],
			   short out_block[DCT_SIZE][DCT_SIZE])
{
	int i, j;
	
	for (i = 0; i < DCT_SIZE; i++)
		for(j = 0; j < DCT_SIZE; j++)
			out_block[i][j] = in_block[j][i];
}

void *transpose_pth_2(void *params)
{
	int i, j, k;
	Params *p = (Params *)params;

	for (i = 3; i < DW; i++) {
		for (j = 0; j < DCT_SIZE; j++)
			for(k = 0; k < DCT_SIZE; k++)
				p->pout_block[j][k] = p->pin_block[k][j];

		sync_threads();
		
		if (i + 1 < DW) {
			block_memcpy(p->pout_block, p->pnii_block);
		}

		//	sync_threads();
	}

	return NULL;
}

void *transpose_pth_0(void *params)
{
	int i, j, k;
	TP0Params *p = (TP0Params *)params;

	for (i = 3; i < DW; i++) {
		for (j = 0; j < DCT_SIZE; j++)
			for(k = 0; k < DCT_SIZE; k++)
				p->pout_block[j][k] = p->pin_block[k][j];

		sync_threads();
		
		if (i + 1 < DW)
			read_data(p->input + (i + 1) * N, p->pnii_block);
		
		write_data(p->pout_block, p->output + (i - 3) * N);

		//	sync_threads();
	}

	return NULL;
}

void block_memcpy(short in_block[DCT_SIZE][DCT_SIZE],
			   short out_block[DCT_SIZE][DCT_SIZE])
{
	int i, j;
	
	for (i = 0; i < DCT_SIZE; i++)
		for(j = 0; j < DCT_SIZE; j++)
			out_block[i][j] = in_block[i][j];
}

void dct(short input[DW * N], short output[DW * N])
{
//	int i;
	pthread_t thread_id[D];
	Params thread_args[D];
	TP0Params tp0_thread_args;
   
	short buf_2d_in[D][DCT_SIZE][DCT_SIZE];
	short buf_2d_out[D][DCT_SIZE][DCT_SIZE];

	dct_data_t row_outbuf[D][DCT_SIZE][DCT_SIZE];
	dct_data_t col_outbuf[D][DCT_SIZE][DCT_SIZE], col_inbuf[D][DCT_SIZE][DCT_SIZE];

	thread_args[3].pin_block = buf_2d_in[3];
	thread_args[3].pout_block = row_outbuf[3];
	thread_args[3].pnii_block = row_outbuf[2];

	thread_args[2].pin_block = row_outbuf[2];
	thread_args[2].pout_block = col_inbuf[2];
	thread_args[2].pnii_block = col_inbuf[1];

	thread_args[1].pin_block = col_inbuf[1];
	thread_args[1].pout_block = col_outbuf[1];
	thread_args[1].pnii_block = col_outbuf[0];

	tp0_thread_args.pin_block = col_outbuf[0];
	tp0_thread_args.pout_block = buf_2d_out[0];
	tp0_thread_args.pnii_block = buf_2d_in[3];
	tp0_thread_args.input = input;
	tp0_thread_args.output = output;

	// pre operations
	read_data(input + 0 * N, buf_2d_in[0]);
	dct_2d(buf_2d_in[0], row_outbuf[0]);
	transpose(row_outbuf[0], col_inbuf[0]);
	dct_2d(col_inbuf[0], col_outbuf[0]);

	read_data(input + 1 * N, buf_2d_in[1]);
	dct_2d(buf_2d_in[1], row_outbuf[1]);
	transpose(row_outbuf[1], col_inbuf[1]);

	read_data(input + 2 * N, buf_2d_in[2]);
	dct_2d(buf_2d_in[2], row_outbuf[2]);

	read_data(input + 3 * N, buf_2d_in[3]);


	pthread_mutex_init(&thread_counter_mutex, NULL);
	pthread_cond_init(&thread_counter_cv, NULL);
	thread_counter = 0;

	pthread_create(&thread_id[3], NULL, &dct_2d_pth_3, &thread_args[3]);
	pthread_create(&thread_id[2], NULL, &transpose_pth_2, &thread_args[2]);
	pthread_create(&thread_id[1], NULL, &dct_2d_pth_1, &thread_args[1]);
	pthread_create(&thread_id[0], NULL, &transpose_pth_0, &tp0_thread_args);

	/*
	for (i = 3; i < DW; i++) {
		//	dct_2d(buf_2d_in[3], row_outbuf[3]);
		pthread_create(&thread_id[3], NULL, &dct_2d_pth, &thread_args[3]);
		
		//	transpose(row_outbuf[2], col_inbuf[2]);
		pthread_create(&thread_id[2], NULL, &transpose_pth, &thread_args[2]);
		
		//	dct_2d(col_inbuf[1], col_outbuf[1]);
		pthread_create(&thread_id[1], NULL, &dct_2d_pth, &thread_args[1]);
		
		//	transpose(col_outbuf[0], buf_2d_out[0]);
		pthread_create(&thread_id[0], NULL, &transpose_pth, &thread_args[0]);
		
		//	write_data(buf_2d_out[0], output + (i - 3) * N);
		
		//sync threads
		pthread_join(thread_id[3], NULL);
		pthread_join(thread_id[2], NULL);
		pthread_join(thread_id[1], NULL);
		pthread_join(thread_id[0], NULL);

		write_data(buf_2d_out[0], output + (i - 3) * N);
		if (i + 1 < DW) {
			read_data(input + (i + 1) * N, buf_2d_in[3]);
			block_memcpy(row_outbuf[3], row_outbuf[2]);
			block_memcpy(col_inbuf[2], col_inbuf[1]);
			block_memcpy(col_outbuf[1], col_outbuf[0]);
		}
	}
	*/

	pthread_join(thread_id[3], NULL);
	pthread_join(thread_id[2], NULL);
	pthread_join(thread_id[1], NULL);
	pthread_join(thread_id[0], NULL);
	
	//post operations
	transpose(col_outbuf[1], buf_2d_out[1]);
	write_data(buf_2d_out[1], output + ((DW - 1) - 2) * N);

	dct_2d(col_inbuf[2], col_outbuf[2]);
	transpose(col_outbuf[2], buf_2d_out[2]);
	write_data(buf_2d_out[2], output + ((DW - 1) - 1) * N);

	transpose(row_outbuf[3], col_inbuf[3]);
	dct_2d(col_inbuf[3], col_outbuf[3]);
	transpose(col_outbuf[3], buf_2d_out[3]);
	write_data(buf_2d_out[3], output + (DW - 1) * N);
}

