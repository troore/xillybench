
#include "matrix.h"
#include <iostream>

using namespace std;

	template<class U, class T>
T InnerProd(U Len, T* v1,T* v2)
{
	T tmp=(T) 0;
	for(int k=0;k<Len;k++)
	{
		tmp+=(*(v1+k))*(*(v2+k));
	}
	return tmp;
}
	template<class U, class T>
void VectorPlus(U len, T* v1, T* v2, T* vout)
{
	for(int i=0;i<len;i++){*(vout+i)=(*(v1+i))+(*(v2+i));}
}

	template<class U, class T>
void VectorMinus(U len, T* v1, T* v2, T* vout)
{
	for(int i=0;i<len;i++){*(vout+i)=(*(v1+i))-(*(v2+i));}
}

	template<class U, class T>
double norm(U len, T* v)
{
	double normValue = 0.0;
	for(int i=0;i<len;i++)
	{
		normValue += pow(abs(*(v+i)),2.0);
	}
	normValue = sqrt(normValue);
	return normValue;
}

	template<class U, class T>
double p1_norm(U len, T* v)
{
	double p1_normValue = 0.0;
	for(int i=0;i<len;i++)
	{
		p1_normValue += abs(*(v+i));
	}
	return p1_normValue;
}

	template<class U,class T>
void MatrixProd(U d1, U d2, U d3, T** M1, T** M2, T** oM)
{
	for(int r=0;r<d1;r++)
	{
		T* pv1 = new T[d2];
		for(int i=0;i<d2;i++){*(pv1+i)=*(*(M1+r)+i);}
		for(int c=0;c<d3;c++)
		{
			T* pv2 = new T[d2];
			for(int i=0;i<d2;i++)
			{*(pv2+i)=*(*(M2+i)+c);}

			*(*(oM+r)+c)=InnerProd(d2,pv1,pv2);
			delete[] pv2;
		}
		delete[] pv1;
	}
}

	template<class U,class T>
void MatrixProd(U d1, U d2, U d3, T** M1, T* V2, T* oV)
{
	for(int i=0;i<d1;i++)
	{
		*(oV+i) = InnerProd(d2,*(M1+i),V2);
	}
}


	template<class U,class T>
void MatrixInv(U sz,T** pM,T** pInvM)
{
	T** pX=new T*[sz];
	for(int r=0;r<sz;r++){*(pX+r)=new T[2*sz];}

	for(int r=0;r<sz;r++)
	{
		for(int c=0;c<sz;c++)
		{*(*(pX+r)+c)=*(*(pM+r)+c);}
		for(int c=sz;c<2*sz;c++)
		{
			if(c==r+sz){*(*(pX+r)+c)=(T)1.0;}
			else{*(*(pX+r)+c)=(T)0.0;}
		}
	}




	T* pCurRow=new T[2*sz];
	for(int r=0;r<sz;r++)
	{

		for(int c=0;c<2*sz;c++){*(pCurRow+c)=(*(*(pX+r)+c))/(*(*(pX+r)+r));}

		for(int c=0;c<2*sz;c++){*(*(pX+r)+c)=*(pCurRow+c);}

		for(int er=r+1;er<sz;er++)
		{
			T curC=*(*(pX+er)+r);

			for(int c=0;c<2*sz;c++){*(*(pX+er)+c)-=(curC)*(*(pCurRow+c));}  
		}
	}



	for(int r=sz-1;r>=0;r--)
	{

		for(int c=0;c<2*sz;c++){*(pCurRow+c)=(*(*(pX+r)+c));}

		for(int er=r-1;er>=0;er--)
		{
			T curC = *(*(pX+er)+r);

			for(int c=0;c<2*sz;c++){*(*(pX+er)+c)-=(curC)*(*(pCurRow+c));}

		}
	}




	for(int r=0;r<sz;r++)
	{

		for(int c=0;c<sz;c++)
		{
			int col=c+sz;
			*(*(pInvM+r)+c)=*(*(pX+r)+col);
		}

	}

	delete[] pCurRow;

	for(int r=0;r<sz;r++){delete[] *(pX+r);}
	delete[] pX;
}

	template<class U,class T>
void CircularConvolution(U Ld,U Sd,T* pVL,T* pVS,T* oV)
{
	T *ptmp=new T[Sd];
	for(int i=0;i<Ld;i++)
	{
		for(int s=0;s<Sd;s++)
		{*(ptmp+s)=*(pVL+((i-s+Ld)%Ld));}
		*(oV+i)=InnerProd(Sd,ptmp,pVS);
	}
	delete[] ptmp;
}

template int InnerProd(int Len, int* v1,int* v2);
template void VectorPlus(int len, int* v1, int* v2, int* vout);
template void VectorMinus(int len,int* v1, int* v2, int* vout);
template double norm(int len, int* v);
template double p1_norm(int len, int* v);
template void MatrixProd(int d1,int d2,int d3,int** M1, int** M2, int** oM);
template void MatrixProd(int d1,int d2,int d3,int** M1, int* V2, int* oV);
template void CircularConvolution(int Ld,int Sd,int* pVL,int* pVS,int* oV);
template void MatrixInv(int sz,int** pM,int** pInvM);

template float InnerProd(int Len, float* v1,float* v2);
template void VectorPlus(int len, float* v1, float* v2, float* vout);
template void VectorMinus(int len,float* v1, float* v2, float* vout);
template double norm(int len, float* v);
template double p1_norm(int len, float* v);
template void MatrixProd(int d1,int d2,int d3,float** M1, float** M2, float** oM);
template void MatrixProd(int d1,int d2,int d3,float** M1, float* V2, float* oV);
template void CircularConvolution(int Ld,int Sd,float* pVL,float* pVS,float* oV);
template void MatrixInv(int sz,float** pM,float** pInvM);

template double InnerProd(int Len, double* v1,double* v2);
template void VectorPlus(int len, double* v1, double* v2, double* vout);
template void VectorMinus(int len,double* v1, double* v2, double* vout);
template double norm(int len, double* v);
template double p1_norm(int len, double* v);
template void MatrixProd(int d1,int d2,int d3,double** M1, double** M2, double** oM);
template void MatrixProd(int d1,int d2,int d3,double** M1, double* V2, double* oV);
template void CircularConvolution(int Ld,int Sd,double* pVL,double* pVS,double* oV);
template void MatrixInv(int sz,double** pM,double** pInvM);

template complex<float> InnerProd(int Len, complex<float>* v1,complex<float>* v2);
template void VectorPlus(int len, complex<float>* v1, complex<float>* v2, complex<float>* vout);
template void VectorMinus(int len,complex<float>* v1, complex<float>* v2, complex<float>* vout);
template double norm(int len, complex<float>* v);
template double p1_norm(int len, complex<float>* v);
template void MatrixProd(int d1,int d2,int d3,complex<float>** M1, complex<float>** M2, complex<float>** oM);
template void MatrixProd(int d1,int d2,int d3,complex<float>** M1, complex<float>* V2, complex<float>* oV);
template void CircularConvolution(int Ld,int Sd,complex<float>* pVL,complex<float>* pVS,complex<float>* oV);
template void MatrixInv(int sz,complex<float>** pM,complex<float>** pInvM);

template complex<double> InnerProd(int Len, complex<double>* v1,complex<double>* v2);
template void VectorPlus(int len, complex<double>* v1, complex<double>* v2, complex<double>* vout);
template void VectorMinus(int len,complex<double>* v1, complex<double>* v2, complex<double>* vout);
template double norm(int len, complex<double>* v);
template double p1_norm(int len, complex<double>* v);
template void MatrixProd(int d1,int d2,int d3,complex<double>** M1, complex<double>** M2, complex<double>** oM);
template void MatrixProd(int d1,int d2,int d3,complex<double>** M1, complex<double>* V2, complex<double>* oV);
template void CircularConvolution(int Ld,int Sd,complex<double>* pVL,complex<double>* pVS,complex<double>* oV);
template void MatrixInv(int sz,complex<double>** pM,complex<double>** pInvM);
