#pragma once

#include "windows.h"
#include "stdio.h"
#include "math.h"
#include "afx.h"
#include "My_MultiProcessing.h"
#define PI (asin(1.0) * 2.0)

typedef struct {
	double real;
	double imag;
	double mag;
} cplx;


class My_FFT
{


public:


	cplx CalExp(int k, int N);
	cplx SumCplx(cplx a, cplx b);
	cplx MulCplx(cplx a, cplx b);
	cplx DivCplx(cplx a, cplx b);
	void CopyCplx(cplx* dst, cplx* src);
	int GetStage(int N);
	void ReverseBit(int* table, int N);
	void ConjIFFT(cplx* data, int N);
	void Real2Cplx(int* input, int N, cplx* out);
	void Real2Cplx(double* input, int N, cplx* out);
	void Real2Cplx(unsigned char* input, int N, cplx* out);
	void Real2Cplx(unsigned short* input, int N, cplx* out);
	void MakeReverse(cplx* data, int N);
	void FFT(cplx* data, int N, int step);
	void FFT_Step(cplx* data, int N, int step);
	void IFFT(cplx* data, int N);
	int Get2Pow(int in);
	void FFT_2d(cplx* Data, int SizeX, int SizeY);
	void FFT_2d(cplx* Data, int SizeX, int SizeY, int SizeXX, int SizeYY, int MultiThreadNum);
	void FFT_2d_N(cplx* Data, int N);
	void ConjIFFT_2d(cplx* data, int SizeXX, int SizeYY);
	void ConjIFFT_2d_N(cplx* data, int N);
	void IFFT_2d(cplx* Data, int SizeX, int SizeY);
	void IFFT_2d_N(cplx* Data, int N);
	void Real2Cplx_2d(unsigned char* input, int SizeX, int SizeY, cplx* out);
	void Real2Cplx_2d(unsigned short* input, int SizeX, int SizeY, cplx* out);
	void Real2Cplx_2d_SQ(unsigned short* input, int SizeX, int SizeY, int SizeXX, int SizeYY, cplx* out);
	void Real2Cplx_2d_N(unsigned char* input, int imageSizeX, int imageSizeY, int N, cplx* out);
	void CalMag(cplx* Data, int N);
	void CrossPower(cplx* input0, cplx* input1, cplx* output, int N);
	void CrossPowerSingle(cplx* input0, cplx* input1, cplx* output, int n);

	void CalCorrelation(unsigned char* data0, unsigned char* data1, int imageSizeX, int imageSizeY, int blockStartX, int blockStartY, int blockSizeX, int blockSizeY, int N, double* retX, double* retY);
	void CalCorrelation2(unsigned char* data0, unsigned char* data1, int imageSizeX, int imageSizeY, int blockStartX, int blockStartY, int blockSizeX, int blockSizeY, int N, double* retX, double* retY, double* retM, int count);

	Kyle_MultiProcessing* m_MP;

	static void MyProcFFTHor(void* userdataptr, int threadnum, int controlnum, float* progress);
	static void MyProcFFTVer(void* userdataptr, int threadnum, int controlnum, float* progress);

};

typedef struct {
	int sizex;
	int sizey;
	int threadnum;
	cplx* cplx_src;
	cplx** cplx_1d_hor;
	cplx** cplx_1d_ver;
	My_FFT* pMy_FFT;
} FFT_MP_STRUCT;
