
#ifndef _GENERALFUNC_H_
#define _GENERALFUNC_H_

#include <complex>

using namespace std;

void ReadInputFromFiles(int *pIn,int Sz,const char *name);

void ReadInputFromFiles(float *pIn, int Sz, const char *name);

void ReadInputFromFiles(complex<float> *pIn, int Sz, const char *nameReal, const char *nameImag);
void ReadInputFromFiles(float *pIn, int Sz, const char *nameReal, const char *nameImag);
void ReadInputFromFiles(float *pInReal, float *pInImag, int Sz, const char *nameReal, const char *nameImag);
void ReadInputFromFiles(complex<float> *pIn, int Sz, const char *nameReal, const char *nameImag);

void GeneRandomInput(complex<float> *pIn, int Sz, const char *nameReal, const char *nameImag);
void GeneRandomInput(float *pIn, int Sz, const char *nameReal, const char *nameImag);
void GeneRandomInput(float *pInReal, float *pInImag, int Sz, const char *nameReal, const char *nameImag);

void GeneRandomInput(int *pIn,int Sz,const char *name);
void GeneRandomInput(float *pIn,int Sz,const char *name);

void WriteOutputToFiles(int *pOut, int Sz, const char *name);

void WriteOutputToFiles(float *pOut, int Sz, const char *name);

void WriteOutputToFiles(complex<float> *pOut, int Sz, const char *nameReal, const char *nameImag);
void WriteOutputToFiles(float *pOut, int Sz, const char *nameReal, const char *nameImag);
void WriteOutputToFiles(float *pOutReal, float *pOutImag, int Sz, const char *nameReal, const char *nameImag);

void GenerateLTEChainInput(int *pDataSource, int DataK, int *pTxDS, int RANDOMSEED);
void ReadLTEChainOutput(int *pFileSink, int *pRxFS, int DataK);

#endif



