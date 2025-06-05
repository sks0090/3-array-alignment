//#include "stdafx.h"
#include "My_FFT.h"


void My_FFT::CrossPower(cplx* input0, cplx *input1, cplx *output, int N)
{
	cplx result;
	cplx resultmag;
	double mag;
	for (int x = 0; x < N; x++)
	{
		result.real = (input0[x].real*input1[x].real + input0[x].imag*input1[x].imag);
		result.imag = (input0[x].imag*input1[x].real - input0[x].real*input1[x].imag);

		resultmag.real = (input0[x].real*input1[x].real - input0[x].imag*input1[x].imag);
		resultmag.imag = (input0[x].imag*input1[x].real + input0[x].real*input1[x].imag);

	//	mag = sqrt(result.real*result.real + result.imag*result.imag);
		mag = sqrt(resultmag.real*resultmag.real + resultmag.imag*resultmag.imag);
		output[x].real = result.real/mag;
		output[x].imag = result.imag/mag;
	}
}

void My_FFT::CrossPowerSingle(cplx* input0, cplx *input1, cplx *output, int n)
{
	cplx result;
	cplx resultmag;
	double mag;
	result.real = (input0[n].real*input1[n].real + input0[n].imag*input1[n].imag);
	result.imag = (input0[n].imag*input1[n].real - input0[n].real*input1[n].imag);

	resultmag.real = (input0[n].real*input1[n].real - input0[n].imag*input1[n].imag);
	resultmag.imag = (input0[n].imag*input1[n].real + input0[n].real*input1[n].imag);

//	mag = sqrt(result.real*result.real + result.imag*result.imag);
	mag = sqrt(resultmag.real*resultmag.real + resultmag.imag*resultmag.imag);
	output[n].real = result.real / mag;
	output[n].imag = result.imag / mag;
}

cplx My_FFT::CalExp(int k, int N)
{
	cplx out;
	out.real = cos(2 * PI*k / N);
	out.imag = -sin(2 * PI*k / N);
	return out;
}

cplx My_FFT::SumCplx(cplx a, cplx b)
{
	cplx out;
	out.real = a.real + b.real;
	out.imag = a.imag + b.imag;
	return out;
}

cplx My_FFT::MulCplx(cplx a, cplx b)
{
	cplx out;
	out.real = (a.real * b.real) - (a.imag * b.imag);
	out.imag = (a.real * b.imag) + (a.imag * b.real);
	return out;
}

cplx My_FFT::DivCplx(cplx a, cplx b)
{
	cplx out;
	double cd2;
	cd2 = b.real*b.real + b.imag*b.imag;
	out.real = (a.real * b.real + a.imag * b.imag) / cd2;
	out.imag = (a.imag * b.real - a.real * b.imag) / cd2;
	return out;
}

void My_FFT::CopyCplx(cplx* dst, cplx* src)
{
	dst->real = src->real;
	dst->imag = src->imag;
	dst->mag = src->mag;
	//	dst = src;
}

int My_FFT::GetStage(int N)
{
	int stage = 0;
	while (N >= 2)
	{
		N = N / 2;
		stage++;
	}
	return stage;
}

void My_FFT::ReverseBit(int *table, int N)
{
	int bit = GetStage(N);
	int temp;
	int aa;
	int bb;
	for (int i = 0; i < N; i++)
	{
		temp = 0;
		for (int k = 0; k <bit; k++)
		{
			aa = i&(0x1 << k); //pick
			bb = aa >> k;
			temp += bb << (bit - 1 - k);
		}
		table[i] = temp;
	}
}

void My_FFT::ConjIFFT(cplx *data, int N)
{
	cplx *temp = (cplx*)calloc(N, sizeof(cplx));
	for (int i = 0; i < N; i++)
	{
		if (i == 0)
		{
			CopyCplx(&temp[i], &data[i]);
		}
		else
		{
			CopyCplx(&temp[i], &data[N - i]);
		}
		temp[i].real = temp[i].real / N;
		temp[i].imag = temp[i].imag / N;
	}
	for (int i = 0; i < N; i++)
	{
		CopyCplx(&data[i], &temp[i]);
	}
	free(temp);
}

void My_FFT::Real2Cplx(int* input, int N, cplx* out)
{
	for (int i = 0; i < N; i++)
	{
		out[i].real = input[i];
		out[i].imag = 0;
	}
}

void My_FFT::Real2Cplx(double* input, int N, cplx* out)
{
	for (int i = 0; i < N; i++)
	{
		out[i].real = input[i];
		out[i].imag = 0;
	}
}

void My_FFT::Real2Cplx(unsigned char* input, int N, cplx* out)
{
	for (int i = 0; i < N; i++)
	{
		out[i].real = input[i];
		out[i].imag = 0;
	}
}

void My_FFT::Real2Cplx(unsigned short* input, int N, cplx* out)
{
	for (int i = 0; i < N; i++)
	{
		out[i].real = input[i];
		out[i].imag = 0;
	}
}

void My_FFT::MakeReverse(cplx* data, int N)
{
	int *table = (int*)calloc(N, sizeof(int));
	ReverseBit(table, N);
	cplx* temp = (cplx*)calloc(N, sizeof(cplx));
	for (int i = 0; i < N; i++)
	{
		CopyCplx(&(temp[i]), &(data[table[i]]));
	}
	for (int i = 0; i < N; i++)
	{
		CopyCplx(&(data[i]), &(temp[i]));
	}
	free(table);
	free(temp);
}

void My_FFT::FFT(cplx* data, int N, int step)
{
	CString sText;
	int totstep = GetStage(N); //총 step 수 계산
	int powerstep;
	if (step <= totstep)
	{
		if (step == 1) //첫 스텝일 경우 reverse bit로 변환
		{
			MakeReverse(data, N);
		}
		cplx *temp = (cplx*)calloc(N, sizeof(cplx)); //target 버퍼 생성
		powerstep = pow(2, step);
		for (int i = 0; i < N / powerstep; i++)
		{
			for (int k = 0; k < powerstep; k++)
			{
				if (k < powerstep / 2)
				{
					temp[i*powerstep + k] = SumCplx(data[i * powerstep + k], MulCplx(CalExp(k, powerstep), data[i * powerstep + k + (powerstep / 2)]));
				}
				else
				{
					temp[i*powerstep + k] = SumCplx(MulCplx(CalExp(k, powerstep), data[i * powerstep + k]), data[i * powerstep + k - (powerstep / 2)]);
				}
			}
		}
		for (int i = 0; i < N; i++)
		{
			CopyCplx(&data[i], &temp[i]);
		}
		free(temp);
		FFT(data, N, step + 1);
	}

}

void My_FFT::FFT_Step(cplx* data, int N, int step)
{
	//CString sText;
	//int totstep = GetStage(N); //총 step 수 계산
	int powerstep;
	//if (step <= totstep)
	//{
	//	if (step == 1) //첫 스텝일 경우 reverse bit로 변환
	//	{
	//		MakeReverse(data, N);
	//	}
		cplx* temp = (cplx*)calloc(N, sizeof(cplx)); //target 버퍼 생성
		powerstep = pow(2, step);
		for (int i = 0; i < N / powerstep; i++)
		{
			for (int k = 0; k < powerstep; k++)
			{
				if (k < powerstep / 2)
				{
					temp[i * powerstep + k] = SumCplx(data[i * powerstep + k], MulCplx(CalExp(k, powerstep), data[i * powerstep + k + (powerstep / 2)]));
				}
				else
				{
					temp[i * powerstep + k] = SumCplx(MulCplx(CalExp(k, powerstep), data[i * powerstep + k]), data[i * powerstep + k - (powerstep / 2)]);
				}
			}
		}
		for (int i = 0; i < N; i++)
		{
			CopyCplx(&data[i], &temp[i]);
		}
		free(temp);
	//	FFT(data, N, step + 1);
	//}
}

void My_FFT::IFFT(cplx* data, int N)
{
	FFT(data, N, 1);
	ConjIFFT(data, N);
}

int My_FFT::Get2Pow(int in)
{
	int out = 1;
	while (out < in)
	{
		out = out * 2;
	}
	return out;
}


void My_FFT::FFT_2d_N(cplx* Data, int N)
{
//	int SizeXX = Get2Pow(SizeX);
//	int SizeYY = Get2Pow(SizeY);
	cplx *cplx_1d_hor = (cplx*)calloc(N, sizeof(cplx));
	cplx *cplx_1d_ver = (cplx*)calloc(N, sizeof(cplx));

	for (int y = 0; y < N; y++)
	{
		//for (int x = 0; x < SizeXX; x++)
		//{
		//	cplx_1d_hor[x].real = Data[x + SizeXX*y].real; 
		//	cplx_1d_hor[x].imag = Data[x + SizeXX*y].imag;
		//}
		//FFT(cplx_1d_hor, SizeXX, 1); //FFT
		//for (int x = 0; x < SizeXX; x++)
		//{
		//	CopyCplx(&Data[x + SizeXX*y], &cplx_1d_hor[x]); //FFT 한 값을 2d 버퍼에 복사
		//}

		FFT(&Data[0 + N*y], N, 1);

	}

	for (int x = 0; x < N; x++)
	{
		for (int y = 0; y < N; y++)
		{
			//if (y < SizeY)
			//{
			cplx_1d_ver[y].real = double(Data[x + y*N].real); //zero padding 더해서 cplx버퍼로 한줄씩 옮김
			cplx_1d_ver[y].imag = double(Data[x + y*N].imag); //zero padding 더해서 cplx버퍼로 한줄씩 옮김
																   //}
																   //else
																   //{
																   //	cplx_1d_ver[y].real = 0;
																   //	cplx_1d_ver[y].imag = 0;
																   //}
		}
		FFT(cplx_1d_ver, N, 1); //FFT
		for (int y = 0; y < N; y++)
		{
			CopyCplx(&Data[x + N*y], &cplx_1d_ver[y]); //FFT 한 값을 2d 버퍼에 복사
		}
	}

	free(cplx_1d_hor);
	free(cplx_1d_ver);

}

void My_FFT::FFT_2d(cplx* Data, int SizeX, int SizeY)
{
	int SizeXX = Get2Pow(SizeX);
	int SizeYY = Get2Pow(SizeY);
	cplx *cplx_1d_hor = (cplx*)calloc(SizeXX, sizeof(cplx));
	cplx *cplx_1d_ver = (cplx*)calloc(SizeYY, sizeof(cplx));

	for (int y = 0; y < SizeYY; y++)
	{
		//for (int x = 0; x < SizeXX; x++)
		//{
		//	cplx_1d_hor[x].real = Data[x + SizeXX*y].real; 
		//	cplx_1d_hor[x].imag = Data[x + SizeXX*y].imag;
		//}
		//FFT(cplx_1d_hor, SizeXX, 1); //FFT
		//for (int x = 0; x < SizeXX; x++)
		//{
		//	CopyCplx(&Data[x + SizeXX*y], &cplx_1d_hor[x]); //FFT 한 값을 2d 버퍼에 복사
		//}

		FFT(&Data[0 + SizeXX*y], SizeXX, 1);

	}

	for (int x = 0; x < SizeXX; x++)
	{
		for (int y = 0; y < SizeYY; y++)
		{
			//if (y < SizeY)
			//{
			cplx_1d_ver[y].real = double(Data[x + y*SizeXX].real); //zero padding 더해서 cplx버퍼로 한줄씩 옮김
			cplx_1d_ver[y].imag = double(Data[x + y*SizeXX].imag); //zero padding 더해서 cplx버퍼로 한줄씩 옮김
																   //}
																   //else
																   //{
																   //	cplx_1d_ver[y].real = 0;
																   //	cplx_1d_ver[y].imag = 0;
																   //}
		}
		FFT(cplx_1d_ver, SizeYY, 1); //FFT
		for (int y = 0; y < SizeYY; y++)
		{
			CopyCplx(&Data[x + SizeXX*y], &cplx_1d_ver[y]); //FFT 한 값을 2d 버퍼에 복사
		}
	}

	free(cplx_1d_hor);
	free(cplx_1d_ver);

}

void My_FFT::MyProcFFTHor(void* userdataptr, int threadnum, int controlnum, float* progress)
{
	FFT_MP_STRUCT* ptr = (FFT_MP_STRUCT*)userdataptr;

	int blocksize = ptr->sizey / ptr->threadnum;
	int starty = (threadnum == 0) ? 0 : threadnum * blocksize;
	int endy = ((threadnum == ptr->threadnum - 1) ? ptr->sizey : (threadnum + 1) * blocksize);

	for (int y = starty; y < endy; y++)
	{
		ptr->pMy_FFT->FFT(&(ptr->cplx_src[0+ptr->sizex*y]), ptr->sizex, 1);
	}

//	ptr->cplx_1d[threadnum][0];

}

void My_FFT::MyProcFFTVer(void* userdataptr, int threadnum, int controlnum, float* progress)
{
	FFT_MP_STRUCT* ptr = (FFT_MP_STRUCT*)userdataptr;

	int blocksize = ptr->sizex / ptr->threadnum;
	int startx = (threadnum == 0) ? 0 : threadnum * blocksize;
	int endx = ((threadnum == ptr->threadnum - 1) ? ptr->sizex : (threadnum + 1) * blocksize);

	for (int x = startx; x < endx; x++)
	{
		for (int y = 0; y < ptr->sizey; y++)
		{
			ptr->cplx_1d_ver[threadnum][y].real = double(ptr->cplx_src[x + y * ptr->sizex].real); //zero padding 더해서 cplx버퍼로 한줄씩 옮김
			ptr->cplx_1d_ver[threadnum][y].imag = double(ptr->cplx_src[x + y * ptr->sizex].imag);
		}
		ptr->pMy_FFT->FFT((ptr->cplx_1d_ver[threadnum]), ptr->sizey, 1);
		for (int y = 0; y < ptr->sizey; y++)
		{
			ptr->pMy_FFT->CopyCplx(&(ptr->cplx_src[x + ptr->sizex * y]), & (ptr->cplx_1d_ver[threadnum][y])); //FFT 한 값을 2d 버퍼에 복사
		}
	}

	//	ptr->cplx_1d[threadnum][0];

}




void My_FFT::FFT_2d(cplx* Data, int SizeX, int SizeY, int SizeXX, int SizeYY, int MultiThreadNum)
{
//	int SizeXX = Get2Pow(SizeX);
//	int SizeYY = Get2Pow(SizeY);
	cplx** cplx_1d_hor = (cplx**)calloc(MultiThreadNum, sizeof(cplx*));
	cplx** cplx_1d_ver = (cplx**)calloc(MultiThreadNum, sizeof(cplx*));

	for (int i = 0; i < MultiThreadNum; i++)
	{
		cplx_1d_hor[i] = (cplx*)calloc(SizeXX, sizeof(cplx));
		cplx_1d_ver[i] = (cplx*)calloc(SizeYY, sizeof(cplx));
	}
	m_MP = new Kyle_MultiProcessing();
	m_MP->Init(1, MultiThreadNum);

	FFT_MP_STRUCT userdata;
	userdata.sizex = SizeXX;
	userdata.sizey = SizeYY;
	userdata.threadnum = MultiThreadNum;
	userdata.cplx_1d_hor = cplx_1d_hor;
	userdata.cplx_1d_ver = cplx_1d_ver;
	userdata.cplx_src = Data;
	userdata.pMy_FFT = this;

	m_MP->InputProcess(MyProcFFTHor, NULL, &userdata, true);
	m_MP->InputProcess(MyProcFFTVer, NULL, &userdata, true);
	

	
	
	m_MP->Free();
	free(m_MP);
	for (int i = 0; i < MultiThreadNum; i++)
	{
		free(cplx_1d_hor[i]);
		free(cplx_1d_ver[i]);
	}
	free(cplx_1d_hor);
	free(cplx_1d_ver);
}







void My_FFT::ConjIFFT_2d_N(cplx *data, int N)
{
	cplx *temp = (cplx*)calloc(N*N, sizeof(cplx));
//	int N = N*N;
	for (int y = 0; y < N; y++)
	{
		for (int x = 0; x < N; x++)
		{
			if (x == 0)
			{
				CopyCplx(&temp[x + y*N], &data[x + y*N]);
			}
			else
			{
				CopyCplx(&temp[x + y*N], &data[N - x + y*N]);
			}
			temp[x + y*N].real = temp[x + y*N].real / N;
			temp[x + y*N].imag = temp[x + y*N].imag / N;
		}
	}


	for (int x = 0; x < N; x++)
	{
		for (int y = 0; y < N; y++)
		{
			if (y == 0)
			{
				CopyCplx(&data[x + y*N], &temp[x + y*N]);
			}
			else
			{
				CopyCplx(&data[x + y*N], &temp[x + (N - y)*N]);
			}
			data[x + y*N].real = data[x + y*N].real / N;
			data[x + y*N].imag = data[x + y*N].imag / N;
		}
	}

	free(temp);
}

void My_FFT::ConjIFFT_2d(cplx *data, int SizeXX, int SizeYY)
{
	cplx *temp = (cplx*)calloc(SizeXX*SizeYY, sizeof(cplx));
	int N = SizeXX*SizeYY;
	for (int y = 0; y < SizeYY; y++)
	{
		for (int x = 0; x < SizeXX; x++)
		{
			if (x == 0)
			{
				CopyCplx(&temp[x + y*SizeXX], &data[x + y*SizeXX]);
			}
			else
			{
				CopyCplx(&temp[x + y*SizeXX], &data[SizeXX - x + y*SizeXX]);
			}
			temp[x + y*SizeXX].real = temp[x + y*SizeXX].real / SizeXX;
			temp[x + y*SizeXX].imag = temp[x + y*SizeXX].imag / SizeXX;
		}
	}


	for (int x = 0; x < SizeXX; x++)
	{
		for (int y = 0; y < SizeYY; y++)
		{
			if (y == 0)
			{
				CopyCplx(&data[x + y*SizeXX], &temp[x + y*SizeXX]);
			}
			else
			{
				CopyCplx(&data[x + y*SizeXX], &temp[x + (SizeYY - y)*SizeXX]);
			}
			data[x + y*SizeXX].real = data[x + y*SizeXX].real / SizeYY;
			data[x + y*SizeXX].imag = data[x + y*SizeXX].imag / SizeYY;
		}
	}

	free(temp);
}

void My_FFT::IFFT_2d(cplx* Data, int SizeX, int SizeY)
{
	FFT_2d(Data, SizeX, SizeY);
	ConjIFFT_2d(Data, SizeX, SizeY);
}

void My_FFT::IFFT_2d_N(cplx* Data, int N)
{
	FFT_2d_N(Data, N);
	ConjIFFT_2d_N(Data, N);
//	ConjIFFT_2d(Data, SizeX, SizeY);
}


void My_FFT::Real2Cplx_2d_N(unsigned char* input, int imageSizeX, int imageSizeY, int N, cplx* out)
{
//	int SizeXX = Get2Pow(SizeX);
//	int SizeYY = Get2Pow(SizeY);

	for (int y = 0; y <N ; y++)
	{
		for (int x = 0; x < N; x++)
		{
			if ((x < imageSizeX) && (y<imageSizeY))
			{
				out[x + y*N].real = double(input[x + y*imageSizeX]);
				out[x + y*N].imag = 0.0;

			}
			else
			{
				out[x + y*N].real = 0;
				out[x + y*N].imag = 0.0;
			}
		}
	}
}



void My_FFT::Real2Cplx_2d(unsigned char* input, int SizeX, int SizeY, cplx* out)
{
	int SizeXX = Get2Pow(SizeX);
	int SizeYY = Get2Pow(SizeY);

	for (int y = 0; y < SizeYY; y++)
	{
		for (int x = 0; x < SizeXX; x++)
		{
			if ((x < SizeX) && (y<SizeY))
			{
				out[x + y*SizeXX].real = double(input[x + y*SizeX]);
				out[x + y*SizeXX].imag = 0.0;

			}
			else
			{
				out[x + y*SizeXX].real = 0;
				out[x + y*SizeXX].imag = 0.0;
			}
		}
	}
}

void My_FFT::Real2Cplx_2d_SQ(unsigned short* input, int SizeX, int SizeY, int SizeXX, int SizeYY, cplx* out)
{
	//int SizeXX = Get2Pow(SizeX);
	//int SizeYY = Get2Pow(SizeY);

	for (int y = 0; y < SizeYY; y++)
	{
		for (int x = 0; x < SizeXX; x++)
		{
			if ((x < SizeX) && (y < SizeY))
			{
				out[x + y * SizeXX].real = double(input[x + y * SizeX]);
				out[x + y * SizeXX].imag = 0.0;

			}
			else
			{
				out[x + y * SizeXX].real = 0;
				out[x + y * SizeXX].imag = 0.0;
			}
		}
	}
}


void My_FFT::Real2Cplx_2d(unsigned short* input, int SizeX, int SizeY, cplx* out)
{
	int SizeXX = Get2Pow(SizeX);
	int SizeYY = Get2Pow(SizeY);

	for (int y = 0; y < SizeYY; y++)
	{
		for (int x = 0; x < SizeXX; x++)
		{
			if ((x < SizeX) && (y < SizeY))
			{
				out[x + y * SizeXX].real = double(input[x + y * SizeX]);
				out[x + y * SizeXX].imag = 0.0;

			}
			else
			{
				out[x + y * SizeXX].real = 0;
				out[x + y * SizeXX].imag = 0.0;
			}
		}
	}
}


void My_FFT::CalMag(cplx* Data, int N)
{
	for (int i = 0; i < N; i++)
	{
		Data[i].mag = sqrt(Data[i].real*Data[i].real + Data[i].imag*Data[i].imag);
	}
}


void My_FFT::CalCorrelation(unsigned char* data0, unsigned char* data1, int imageSizeX, int imageSizeY, int blockStartX, int blockStartY, int blockSizeX, int blockSizeY, int N, double* retX, double*retY)
{
	int SizeXX = Get2Pow(blockSizeX);
	int SizeYY = Get2Pow(blockSizeY);

	cplx* cplx_0 = (cplx*)calloc(SizeXX*SizeYY, sizeof(cplx));
	cplx* cplx_1 = (cplx*)calloc(SizeXX*SizeYY, sizeof(cplx));
	cplx* cplx_S = (cplx*)calloc(SizeXX*SizeYY, sizeof(cplx));


	for (int y = blockStartY; y < blockStartY + SizeYY; y++)
	{
		for (int x = blockStartX; x < blockStartX+SizeXX; x++)
		{
			if ((x < blockStartX + blockSizeX) && (y<blockStartY + blockSizeY))
			{
				cplx_0[x- blockStartX + (y - blockStartY)*SizeXX].real = double(data0[x + y*imageSizeX]);
				cplx_0[x - blockStartX + (y - blockStartY)*SizeXX].imag = 0.0;

				cplx_1[x - blockStartX + (y - blockStartY)*SizeXX].real = double(data1[x + y*imageSizeX]);
				cplx_1[x - blockStartX + (y - blockStartY)*SizeXX].imag = 0.0;
			}
			else
			{
				cplx_0[x - blockStartX + (y - blockStartY)*SizeXX].real = 0;
				cplx_0[x - blockStartX + (y - blockStartY)*SizeXX].imag = 0.0;

				cplx_1[x - blockStartX + (y - blockStartY)*SizeXX].real = 0;
				cplx_1[x - blockStartX + (y - blockStartY)*SizeXX].imag = 0.0;
			}
		}
	}
	FFT_2d(cplx_0, SizeXX, SizeYY);
	FFT_2d(cplx_1, SizeXX, SizeYY);

	CrossPower(cplx_0, cplx_1, cplx_S, SizeXX*SizeYY);

	IFFT_2d(cplx_S, SizeXX, SizeYY);

	CalMag(cplx_S, SizeXX*SizeYY);

	double *data_result = (double*)calloc(blockSizeX*blockSizeY, sizeof(double));

	for (int y = 0; y < blockSizeY / 2; y++)
	{
		for (int x = 0; x < blockSizeX / 2; x++)
		{
			data_result[blockSizeX / 2 + x + (blockSizeY / 2 + y)*blockSizeX] = cplx_S[x + y*SizeXX].mag;
			data_result[blockSizeX / 2 - 1 - x + (blockSizeY / 2 + y)*blockSizeX] = cplx_S[blockSizeX - 1 - x + y*SizeXX].mag;
			data_result[blockSizeX / 2 + x + (blockSizeY / 2 - 1 - y)*blockSizeX] = cplx_S[x + (blockSizeY - 1 - y)*SizeXX].mag;
			data_result[blockSizeX / 2 - 1 - x + (blockSizeY / 2 - 1 - y)*blockSizeX] = cplx_S[blockSizeX - 1 - x + (blockSizeY - 1 - y)*SizeXX].mag;
			//	data_result[SizeX / 2 - x + y*SizeX] = cplx_S[SizeX - 1 - x + y*CSizeX].mag;
		}
	}

	double GravX = 0;
	double GravY = 0;
	double sum = 0;
	for (int y = 0; y < blockSizeX; y++)
	{
		for (int x = 0; x < blockSizeY; x++)
		{
			sum += data_result[x + y*blockSizeX];
		}
	}
	for (int y = 0; y < blockSizeY / 2; y++)
	{
		for (int x = 0; x < blockSizeX / 2; x++)
		{
			//x>0 y>0
			GravX += data_result[blockSizeX / 2 + x + (blockSizeY / 2 + y)*blockSizeX] * x;
			GravY += data_result[blockSizeX / 2 + x + (blockSizeY / 2 + y)*blockSizeX] * y;
			//x<0 y>0
			GravX -= data_result[blockSizeX / 2 - 1 - x + (blockSizeY / 2 + y)*blockSizeX] * x;
			GravY += data_result[blockSizeX / 2 - 1 - x + (blockSizeY / 2 + y)*blockSizeX] * y;
			//x>0 y<0
			GravX += data_result[blockSizeX / 2 + x + (blockSizeY / 2 - 1 - y)*blockSizeX] * x;
			GravY -= data_result[blockSizeX / 2 + x + (blockSizeY / 2 - 1 - y)*blockSizeX] * y;
			//x<0 y<0
			GravX -= data_result[blockSizeX / 2 - 1 - x + (blockSizeY / 2 - 1 - y)*blockSizeX] * x;
			GravY -= data_result[blockSizeX / 2 - 1 - x + (blockSizeY / 2 - 1 - y)*blockSizeX] * y;
		}
	}
	(*retX) = GravX / sum;
	(*retY) = GravY / sum;
}

void My_FFT::CalCorrelation2(unsigned char* data0, unsigned char* data1, int imageSizeX, int imageSizeY, int blockStartX, int blockStartY, int blockSizeX, int blockSizeY, int N, double* retX, double* retY, double* retM, int count)
{
	int SizeXX = Get2Pow(blockSizeX);
	int SizeYY = Get2Pow(blockSizeY);

	if (SizeXX<int(pow(2, N)))
		SizeXX = int(pow(2, N));
	if (SizeYY<int(pow(2, N)))
		SizeYY = int(pow(2, N));

	cplx* cplx_0 = (cplx*)calloc(SizeXX * SizeYY, sizeof(cplx));
	cplx* cplx_1 = (cplx*)calloc(SizeXX * SizeYY, sizeof(cplx));
	cplx* cplx_S = (cplx*)calloc(SizeXX * SizeYY, sizeof(cplx));


	for (int y = blockStartY; y < blockStartY + SizeYY; y++)
	{
		for (int x = blockStartX; x < blockStartX + SizeXX; x++)
		{
			cplx_0[x - blockStartX + (y - blockStartY) * SizeXX].real = double(data0[(x - blockStartX) % blockSizeX + blockStartX + ((y - blockStartY) % blockSizeY + blockStartY) * imageSizeX]);
			cplx_0[x - blockStartX + (y - blockStartY) * SizeXX].imag = 0.0;

			cplx_1[x - blockStartX + (y - blockStartY) * SizeXX].real = double(data1[(x - blockStartX) % blockSizeX + blockStartX + ((y - blockStartY) % blockSizeY + blockStartY) * imageSizeX]);
			cplx_1[x - blockStartX + (y - blockStartY) * SizeXX].imag = 0.0;
		}
	}

	//windowing

	//for (int y = 0; y < blockSizeY; y++)
	//{
	//	for (int x = 0; x < blockSizeX; x++)
	//	{
	//		cplx_0[x + y*SizeXX].real = cplx_0[x + y*SizeXX].real*double(abs((blockSizeX / 2 - x))) *double(abs((blockSizeY / 2 - y))) / double(blockSizeX / 2) / double(blockSizeY / 2);
	//		cplx_1[x + y*SizeXX].real = cplx_1[x + y*SizeXX].real*double(abs((blockSizeX / 2 - x))) *double(abs((blockSizeY / 2 - y))) / double(blockSizeX / 2) / double(blockSizeY / 2);
	//	}
	//}

	FFT_2d(cplx_0, SizeXX, SizeYY);
	FFT_2d(cplx_1, SizeXX, SizeYY);

	CrossPower(cplx_0, cplx_1, cplx_S, SizeXX * SizeYY);

	IFFT_2d(cplx_S, SizeXX, SizeYY);

	CalMag(cplx_S, SizeXX * SizeYY);

	double* data_result = (double*)calloc(SizeXX * SizeYY, sizeof(double));
	////double *debug = (double*)calloc(SizeXX*SizeYY, sizeof(double));
	////for (int i = 0; i < SizeXX*SizeYY; i++)
	////{
	//////	debug[i] = log(cplx_S[i].mag);
	////	debug[i] = (cplx_S[i].mag);
	////}
	////CFile file;
	////file.Open("debug.bin", CFile::modeCreate|CFile::modeReadWrite, NULL);
	////file.Write(debug, sizeof(double)*SizeXX*SizeYY);
	////file.Close();
	////free(debug);

	for (int y = 0; y < SizeYY / 2; y++)
	{
		for (int x = 0; x < SizeXX / 2; x++)
		{
			data_result[SizeXX / 2 + x + (SizeYY / 2 + y) * SizeXX] = cplx_S[x + y * SizeXX].mag;
			data_result[x + (SizeYY / 2 + y) * SizeXX] = cplx_S[x + SizeXX / 2 + y * SizeXX].mag;
			data_result[SizeXX / 2 + x + (y)*SizeXX] = cplx_S[x + (y + SizeYY / 2) * SizeXX].mag;
			data_result[x + (y)*SizeXX] = cplx_S[SizeXX / 2 + x + (y + SizeYY / 2) * SizeXX].mag;
		}
	}

	double* debug = (double*)calloc(SizeXX * SizeYY, sizeof(double));
	for (int i = 0; i < SizeXX * SizeYY; i++)
	{
		//	debug[i] = log(cplx_S[i].mag);
		//	debug[i] = (cplx_S[i].mag);
		debug[i] = log(data_result[i]);
	}
	CFile file;
	file.Open("debuglog.bin", CFile::modeCreate | CFile::modeReadWrite, NULL);
	file.Write(debug, sizeof(double) * SizeXX * SizeYY);
	file.Close();

	for (int i = 0; i < SizeXX * SizeYY; i++)
	{
		//	debug[i] = log(cplx_S[i].mag);
		//	debug[i] = (cplx_S[i].mag);
		debug[i] = (data_result[i]);
	}
	file.Open("debug.bin", CFile::modeCreate | CFile::modeReadWrite, NULL);
	file.Write(debug, sizeof(double) * SizeXX * SizeYY);
	file.Close();
	free(debug);

	//	double *max = (double *)calloc(count,sizeof(double));
	double max = 0;;
	int* list = (int*)calloc(count, sizeof(int));;
	for (int k = 0; k < count; k++)
	{
		max = 0;
		for (int y = SizeYY / 2 - blockSizeY / 2; y < SizeYY / 2 + blockSizeY / 2; y++)
		{
			for (int x = SizeXX / 2 - blockSizeX / 2; x < SizeXX / 2 + blockSizeX / 2; x++)
			{
				int i = x + y * SizeXX;
				//			for (int i = 0; i < SizeXX*SizeYY; i++)
				{
					bool exist = false;
					for (int j = 0; j < count; j++)
						if (i == list[j])
							exist = true;
					if ((max < data_result[i]) && (!exist))
						//	&& ((i%SizeXX - SizeXX / 2) < SizeXX / 4)
						//	&& ((i%SizeXX - SizeXX / 2) > -SizeXX / 4)
						//	&& ((i / SizeXX - SizeYY / 2) < SizeYY / 4)
						//	&& ((i / SizeXX - SizeYY / 2) > -SizeYY / 4)
						//	)
					{
						max = data_result[i];
						list[k] = i;
						(retX[k]) = double(i % SizeXX - SizeXX / 2);
						(retY[k]) = double(i / SizeXX - SizeYY / 2);
						(retM[k]) = double(max);
					}
				}
			}
		}
	}
}