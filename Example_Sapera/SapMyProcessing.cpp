//	SapMyProcessing.cpp : implementation file
//
#include "pch.h"
#include "SapMyProcessing.h"

#include <math.h>

//#include "GrabDemo.h"
//
// Constructor/Destructor
//
SapMyProcessing::SapMyProcessing(SapBuffer *pBuffers, SapBuffer *pBuffersPro, SapProCallback pCallback, void *pContext)
: SapProcessing(pBuffers, pCallback, pContext)
{ 
	m_BuffersPro  = pBuffersPro; 
}

SapMyProcessing::~SapMyProcessing()
{

   if (m_bInitOK) 
 		Destroy();
}
//
// Processing Control
//
// Do image inversion
BOOL SapMyProcessing::Run()
{
	int width = m_pBuffers->GetWidth();
	int height = m_pBuffers->GetHeight();
	int pixelDepth = m_pBuffers->GetPixelDepth();
	int bytePerPixel = m_pBuffers->GetBytesPerPixel();
	
	void* pSrcbuf, * pDestbuf;
	UINT8* pSrc, * pDst;

	// Get source and destination buffer address
	m_BuffersPro->Next();
	m_BuffersPro->GetAddress(m_BuffersPro->GetIndex(), &pDestbuf);

	// !!! This is INCORRECT,  the current source buffer index is NOT current buffer index
	//m_pBuffers  ->GetAddress( m_pBuffers->GetIndex(), &pSrcbuf);

	// This is CORRECT,  the current source buffer index is SapProcessing object buffer index
	m_pBuffers->GetAddress(GetIndex(), &pSrcbuf);

	pSrc = (UINT8*)pSrcbuf;
	pDst = (UINT8*)pDestbuf;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////// Processing ///////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	//memcpy(pDestbuf, pSrcbuf, width*height);
	ProcessImage(pDst, pSrc, width, height);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	return TRUE;
}

bool SapMyProcessing::ProcessImage(UINT8* pDst, UINT8* pSrc, int width, int height)
{
	for (int iByte = 0; iByte < width * height; iByte++)
	{
		//*pDst = maxValue - *pSrc ;
		*pDst = *pSrc + 100;
		pDst++;
		pSrc++;
	}

	return true;
}