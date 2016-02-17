
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <ctime>
#include <complex>
#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
#include "gauss.h"

using namespace std;

extern int RANDOMSEED;


void ReadInputFromFiles(int *pIn, int Sz, const char *name)
{
	FILE *file;

	if((file=fopen(name,"r"))==NULL)
		throw std::runtime_error ("file open error\n");

	for(int i = 0;i < Sz; i++)
	{
		if(fscanf(file,"%d",(pIn+i)) == EOF)
		{
			if(ferror(file) != 0)
				throw std::runtime_error ("file read error\n");
		}

	}
	
	fclose(file);
}

void ReadInputFromFiles(float *pIn, int Sz, const char *name)
{
	FILE *file;

	if((file=fopen(name,"r"))==NULL)
		throw std::runtime_error ("file open error\n");

	for(int i=0; i < Sz; i++)
	{
		if(fscanf(file, "%f", (pIn+i)) == EOF)
		{
			if(ferror(file) != 0)
				throw std::runtime_error ("file read error\n");
		}

	}
	fclose(file);
}


void ReadInputFromFiles(float *pIn, int Sz, const char *nameReal, const char *nameImag)
{
	FILE *file;

	if((file=fopen(nameReal,"r"))==NULL)
		throw std::runtime_error ("file open error\n");

	float *pReadInReal=new float[Sz];
	
	for(int i=0;i<Sz;i++)
	{
		if(fscanf(file,"%f",(pReadInReal+i)) == EOF)
		{
			if(ferror(file) != 0)
				throw std::runtime_error ("file read error\n");
		}

	}
	fclose(file);

	if((file=fopen(nameImag,"r"))==NULL)
		throw std::runtime_error ("file open error\n");

	float *pReadInImag=new float[Sz];
	for(int i=0;i<Sz;i++)
	{
		if(fscanf(file,"%f",(pReadInImag+i)) == EOF)
		{
			if(ferror(file) != 0)
				throw std::runtime_error ("file read error\n");
		}

	}
	fclose(file);

	for(int i=0;i<Sz;i++)
	{
		pIn[i]=pReadInReal[i];
		pIn[i+Sz] = pReadInImag[i];
		/*
		pIn[2 * i + 0]=pReadInReal[i];
		pIn[2 * i + 1] = pReadInImag[i];
		*/
	}

	delete[] pReadInReal;
	delete[] pReadInImag;
}

void ReadInputFromFiles(float *pInReal, float *pInImag, int Sz, const char *nameReal, const char *nameImag)
{
	FILE *file;

	if((file=fopen(nameReal,"r"))==NULL)
	throw std::runtime_error ("file open error\n");

	float *pReadInReal=new float[Sz];

	for(int i=0;i<Sz;i++)
	{
		if(fscanf(file,"%f",(pReadInReal+i)) == EOF)
		{
			if(ferror(file) != 0)
				throw std::runtime_error ("file read error\n");
		}

	}
	fclose(file);

	if((file=fopen(nameImag,"r"))==NULL)
		throw std::runtime_error ("file open error\n");
	float *pReadInImag=new float[Sz];
	for(int i=0;i<Sz;i++)
	{
		if(fscanf(file,"%f",(pReadInImag+i)) == EOF)
		{
			if(ferror(file) != 0)
				throw std::runtime_error ("file read error\n");
		}

	}
	fclose(file);

	for(int i=0;i<Sz;i++)
	{
		pInReal[i]=pReadInReal[i];
		pInImag[i]=pReadInImag[i];
	}

	delete[] pReadInReal;
	delete[] pReadInImag;
}

void ReadInputFromFiles(std::complex<float> *pIn, int Sz, const char *nameReal, const char *nameImag)
{
	FILE *file;

	if((file=fopen(nameReal,"r"))==NULL)
	throw std::runtime_error ("file open error\n");

	float *pReadInReal=new float[Sz];

	for(int i=0;i<Sz;i++)
	{
		if(fscanf(file,"%f",(pReadInReal+i)) == EOF)
		{
			if(ferror(file) != 0)
				throw std::runtime_error ("file read error\n");
		}

	}
	fclose(file);

	if((file=fopen(nameImag,"r"))==NULL)
		throw std::runtime_error ("file open error\n");
	float *pReadInImag=new float[Sz];
	for(int i=0;i<Sz;i++)
	{
		if(fscanf(file,"%f",(pReadInImag+i)) == EOF)
		{
			if(ferror(file) != 0)
				throw std::runtime_error ("file read error\n");
		}

	}
	fclose(file);

	for(int i=0;i<Sz;i++)
	{
		pIn[i] = std::complex<float>(pReadInReal[i], pReadInImag[i]);
	}

	delete[] pReadInReal;
	delete[] pReadInImag;
}


/*void ReadInputFromFiles(float *pIn, int Sz, const char *nameReal, const char *nameImag)
{
	FILE *file;

	if((file=fopen(nameReal,"r"))==NULL)
		throw std::runtime_error ("file open error\n");

	float *pReadInReal=new float[Sz];
	
	for(int i=0;i<Sz;i++)
	{
		if(fscanf(file,"%f",(pReadInReal+i)) == EOF)
		{
			if(ferror(file) != 0)
				throw std::runtime_error ("file read error\n");
		}

	}
	fclose(file);

	if((file=fopen(nameImag,"r"))==NULL)
		throw std::runtime_error ("file open error\n");

	float *pReadInImag=new float[Sz];
	for(int i=0;i<Sz;i++)
	{
		if(fscanf(file,"%f",(pReadInImag+i)) == EOF)
		{
			if(ferror(file) != 0)
				throw std::runtime_error ("file read error\n");
		}

	}
	fclose(file);

	for(int i=0;i<Sz;i++)
	{
	//	pIn[i]=complex<float>(pReadInReal[i], pReadInImag[i]);
	//	pIn[2 * i + 0] = pReadInReal[i];
	//	pIn[2 * i + 1] = pReadInImag[i];
		pIn[i] = pReadInReal[i];
		pIn[Sz + i] = pReadInImag[i];
	}

	delete[] pReadInReal;
	delete[] pReadInImag;
}*/


/*
void ReadInputFromFiles(float *pIn, int Sz, const char *nameReal, const char *nameImag)
{
	FILE *real_file, *imag_file;
	int i;

	if ((real_file = fopen(nameReal, "r")) == NULL)
		throw std::runtime_error ("real file open error\n");
	if ((imag_file = fopen(nameImag, "r")) == NULL)
		throw std::runtime_error ("imag file open error\n");

	for (i = 0; i < Sz; i++)
	{
		if (fscanf(real_file, "%f", &(pIn[i*2+0])) == EOF)
		{
			if (ferror(real_file) != 0)
				throw std::runtime_error ("real file read error\n");
		}
		if (fscanf(imag_file, "%f", &(pIn[i*2+1])) == EOF)
		{
			if (ferror(imag_file) != 0)
				throw std::runtime_error ("imag file read error\n");
		}
	}
	
	fclose(real_file);
	fclose(imag_file);
}
*/

//////////////////// Random Generate Input and Write to Files ////////////////////////

void GeneRandomInput(int *pIn,int Sz,const char *name)
{
	FILE *fptr=NULL;
	int sd = -111;

	int *pRandom=new int[Sz];

	fptr = fopen(name,"w+");
	for(int i = 0; i < Sz; i++)
	{
		sd-=i;
		float v=(float)gauss1(&sd);
		if(v>0){*(pRandom+i)=1;}
		else{*(pRandom+i)=0;}
		fprintf(fptr,"%d\n",*(pRandom+i));
	}
	fclose(fptr);

	for (int i = 0; i < Sz; i++)
	{
		pIn[i] = pRandom[i];
	}

	delete[] pRandom;
}

void GeneRandomInput(float *pIn, int Sz, const char *name)
{
	FILE *fptr=NULL;
	int sd = -111;

	float *pRandom = new float[Sz];

	fptr = fopen(name,"w+");
	for(int i = 0; i < Sz; i++)
	{
		sd-=i;
		float v=(float)gauss1(&sd);
		if(v>0){*(pRandom+i)=1;}
		else{*(pRandom+i)=0;}
		fprintf(fptr,"%f\n",*(pRandom+i));
	}
	fclose(fptr);

	for (int i = 0; i < Sz; i++)
	{
		pIn[i] = pRandom[i];
	}

	delete[] pRandom;
}


void GeneRandomInput(complex<float> *pIn, int Sz, const char *nameReal, const char *nameImag)
{
    FILE *fptr_real=NULL;
    FILE *fptr_imag=NULL;
    int sd = -111;

	complex<float> *pRandom=new complex<float>[Sz];

	fptr_real = fopen(nameReal,"w+");
	fptr_imag = fopen(nameImag,"w+");
	for(int i=0;i<Sz;i++)
	{
		sd-=i;
		float vr = (float)gauss1(&sd);
		sd-=222;
		float vi = (float)gauss1(&sd);
		*(pRandom+i)=complex<float>(vr,vi);
		fprintf(fptr_real,"%f\n",(*(pRandom+i)).real());
		fprintf(fptr_imag,"%f\n",(*(pRandom+i)).imag());
	}
	fclose(fptr_real);
	fclose(fptr_imag);

	delete[] pRandom;
}

void GeneRandomInput(float *pIn, int Sz, const char *nameReal, const char *nameImag)
{
    FILE *fptr_real=NULL;
    FILE *fptr_imag=NULL;
    int sd = -111;

	fptr_real = fopen(nameReal, "w+");
	fptr_imag = fopen(nameImag, "w+");

	for(int i=0;i<Sz;i++)
	{
		sd-=i;
		float vr = (float)gauss1(&sd);
		sd-=222;
		float vi = (float)gauss1(&sd);
		/*
		pIn[i*2+0] = vr;
		pIn[i*2+1] = vi;
		fprintf(fptr_real, "%f\n", pIn[i*2+0]);
		fprintf(fptr_imag, "%f\n", pIn[i*2+1]);
		*/
		pIn[i] = vr;
		pIn[i + Sz] = vi;
		fprintf(fptr_real, "%f\n", pIn[i]);
		fprintf(fptr_imag, "%f\n", pIn[i + Sz]);
	}

	fclose(fptr_real);
	fclose(fptr_imag);
}

void GeneRandomInput(float *pInReal, float *pInImag, int Sz, const char *nameReal, const char *nameImag)
{
    FILE *fptr_real=NULL;
    FILE *fptr_imag=NULL;
    int sd = -111;

	fptr_real = fopen(nameReal, "w+");
	fptr_imag = fopen(nameImag, "w+");

	for(int i=0;i<Sz;i++)
	{
		sd-=i;
		float vr = (float)gauss1(&sd);
		sd-=222;
		float vi = (float)gauss1(&sd);

		pInReal[i] = vr;
		pInImag[i] = vi;
		fprintf(fptr_real, "%f\n", pInReal[i]);
		fprintf(fptr_imag, "%f\n", pInImag[i]);
	}

	fclose(fptr_real);
	fclose(fptr_imag);
}

void WriteOutputToFiles(int *pOut, int Sz, const char *name)
{
	FILE *fptr=NULL;

	fptr = fopen(name,"w+");
	for(int i=0;i<Sz;i++)
	{
		fprintf(fptr,"%d\n",*(pOut+i));
	}
	fclose(fptr);
}

void WriteOutputToFiles(float *pOut, int Sz, const char *name)
{
	FILE *fptr=NULL;
	
	fptr = fopen(name,"w+");
	for(int i=0;i<Sz;i++)
	{
		fprintf(fptr,"%f\t",*(pOut+i));
	}
	fclose(fptr);
}

void WriteOutputToFiles(float *pOut, int Sz, const char *nameReal, const char *nameImag)
{
	FILE *fptr_real=NULL;
	FILE *fptr_imag=NULL;
	
//	fptr_real = fopen(nameReal, "w+");
	if ((fptr_real = fopen(nameReal,"w+")) == NULL)
		throw std::runtime_error ("file open error\n");
//	fptr_imag = fopen(nameImag, "w+");
	if ((fptr_imag = fopen(nameImag,"w+")) == NULL)
		throw std::runtime_error ("file open error\n");
	
	for(int i=0;i<Sz;i++)
	{
		fprintf(fptr_real,"%f\n",pOut[i]);
		fprintf(fptr_imag,"%f\n",pOut[i+Sz]);
		/*
		fprintf(fptr_real, "%f\n", pOut[2 * i + 0]);
		fprintf(fptr_imag, "%f\n", pOut[2 * i + 1]);
		*/
	}
	
	fclose(fptr_real);
	fclose(fptr_imag);
}

void WriteOutputToFiles(float *pOutReal, float *pOutImag, int Sz, const char *nameReal, const char *nameImag)
{
	FILE *fptr_real=NULL;
	FILE *fptr_imag=NULL;
	
	fptr_real = fopen(nameReal, "w+");
	fptr_imag = fopen(nameImag, "w+");
	
	for(int i=0;i<Sz;i++)
	{
		fprintf(fptr_real,"%f\n",pOutReal[i]);
		fprintf(fptr_imag,"%f\n",pOutImag[i]);
	}
	
	fclose(fptr_real);
	fclose(fptr_imag);
}

void WriteOutputToFiles(std::complex<float> *pOut,  int Sz, const char *nameReal, const char *nameImag)
{
	FILE *fptr_real=NULL;
	FILE *fptr_imag=NULL;
	
	fptr_real = fopen(nameReal, "w+");
	fptr_imag = fopen(nameImag, "w+");
	
	for(int i=0;i<Sz;i++)
	{
		fprintf(fptr_real,"%f\n", std::real(pOut[i]));
		fprintf(fptr_imag,"%f\n", std::imag(pOut[i]));
	}
	
	fclose(fptr_real);
	fclose(fptr_imag);
}


/*void WriteOutputToFiles(float *pOut, int Sz, const char *nameReal, const char *nameImag)
{
	FILE *fptr_real = NULL;
	FILE *fptr_imag = NULL;
	
	fptr_real = fopen(nameReal, "w+");
	fptr_imag = fopen(nameImag, "w+");
	
	for(int i=0;i<Sz;i++)
	{
		fprintf(fptr_real, "%f\n", pOut[i*2+0]);
		fprintf(fptr_imag, "%f\n", pOut[i*2+1]);
	}
	
	fclose(fptr_real);
	fclose(fptr_imag);
}*/

//////////////////////END Read Output and Write Output to Files ////////////////////////

void GenerateLTEChainInput(int *pDataSource, int DataK, int *pTxDS, int RANDOMSEED)
{
	int sd = -RANDOMSEED;
	float v;

	for(int i = 0; i < DataK; i++)
	{
		sd -= i;
		v = (float)gauss1(&sd);
		
		pTxDS[i] = (v > 0) ? 1 : 0;
	}

//	(*pDataSource).Write(pTxDS);
	for (int i = 0; i < DataK; i++)
	{
		pDataSource[i] = pTxDS[i];
	}
}

void ReadLTEChainOutput(int *pFileSink, int *pRxFS, int DataK)
{
//	(*pFileSink).Read(pRxFS);
	for (int i = 0; i < DataK; i++)
	{
		pRxFS[i] = pFileSink[i];
	}
}


