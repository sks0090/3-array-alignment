#ifndef _SAPMYPROCESSING_H_
#define _SAPMYPROCESSING_H_

//	SapMyProcessing.h : header file
//

#include "SapClassBasic.h"

//
// SapMyProcessing class declaration
//
class SapMyProcessing : public SapProcessing
{
public:
	// Constructor/Destructor
	SapMyProcessing(SapBuffer *pBuffers, SapBuffer *pBuffersPro, SapProCallback pCallback, void *pContext);
	virtual ~SapMyProcessing();

protected:
	virtual BOOL Run();
	SapAcquisition *m_Acq;
	SapBuffer      *m_BuffersPro; 

public:
	bool ProcessImage(UINT8* pDst, UINT8* pSrc, int width, int height);
}; 

#endif	// _SAPMYPROCESSING_H_

