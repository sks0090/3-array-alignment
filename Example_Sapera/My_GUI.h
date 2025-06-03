#pragma once
#include "pch.h"

#include "SapClassBasic.h"

// image coordinate : 이미지 영역 왼쪽 위 모서리 (0,0)
// view coordinate : picture pox 영역 왼쪽 위 모서리 (0,0)
// client coordinate : 다이얼로그(클라이언트) 영역 왼쪽 위 모서리 (0,0)
// Window coordinate : 모니터 전체 화면 왼쪽 위 모서리 (0,0)

class MyGUI {
public:		// Variables
	CRect m_viewCoorViewRect;						// view coordinate Rect for picture box
	CRect m_clientCoordinateViewRect;				// client coordinate Rect for picture box
	
	CPoint m_clientCoordinateMouse;			// client coordinate for Mouse
	CPoint m_imageCoordinateMouse;			// image coordinate for Mouse
	CPoint m_viewCoorBtnDown;				// view coordinate for MOuse Button Down
	
	CDC* m_pDC;
	CDC m_memDC;

	bool m_bInit;
	bool m_bDown;
	bool m_bSlave;
	bool m_bThreadView, m_bThreadViewProc;

	int m_bitDepth;
	int m_sizeImgX, m_sizeImgY, m_channel;	// Image 크기
	int m_sizeViewX, m_sizeViewY;			// View 크기
	
	int m_imgCoorMouseDragX, m_imgCoorMouseDragY;		// 마우스 왼쪽 버튼 누르고 끈 위치
	
	int m_imgCoorViewX, m_imgCoorViewY;					// View의 왼쪽위 모서리 위치에 대한 이미지 좌표
	int m_imgCoorViewCenterX, m_imgCoorViewCenterY;		// VIew의 중앙 이미지 좌표
	int m_imgCoorMouseX, m_imgCoorMouseY;				// 마우스 위치의 이미지 좌표

	int row, col;

	float m_fMagFitMin;
	float m_fMagMax;
	float m_fMagX, m_fMagY;
	float m_fMagstep;
	float scrollStep;

	//unsigned char* m_dataView;		// View 이미지 data
	unsigned int* m_dataView;
	unsigned char* m_data8;			// 8bit data
	unsigned short* m_data16;		// 16bit data

	MyGUI *m_RefGui;

	CWinThread* m_ThreadView;

	CEvent m_eventThreadView;

	CBitmap m_bitmap;

	// GUI
	CStatic* m_picbox;

	// Sapera
	SapView* m_View;
	SapBuffer* m_Buffer;


public:		// Methods
	MyGUI();
	~MyGUI();
	// Thread
	static UINT ViewThread(LPVOID lParam);		// View Thread

	// Mouse Event
	bool OnMouseMove(UINT nFlags, CPoint windowCoorMouse);					// 아우스 움직임 이벤트 처리기
	void OnMouseWheel(UINT nFlags, short zDelta, CPoint windowCoorMouse);		// 마우스 휠 이벤트 처리기
	void OnMouseLeave(void);							// 마우스 위치가 현재 dialog를 벗어난 경우
	void OnLButtonDown(UINT nFlags, CPoint clientCoorMouse);		// 마우스 왼쪽 버튼 down
	void OnLButtonUp(UINT nFlags, CPoint clientCoorMouse);		// 마우스 왼쪽 버튼 up
	
	// General
	bool Init(CStatic* picbox, unsigned char* data, int width, int height, int channel, bool bSlave = false, MyGUI* RefGui = NULL);		// DC 이용
	bool Init(CStatic* picbox, SapBuffer* buffer, SapView* view, bool bSlave = false, MyGUI* RefGui = NULL);	// Sapera 이용
	void Free(void);
	void UpdateViewRect(void);		// Dialog 기준 picture box 좌표 및 magfit 도출

	void Show(unsigned char* data, int PosCenterX, int PosCenterY);
	void Show(int imgCoorViewCenterX, int imgCoorViewCenterY);
	void Show();

	void MoveWindow(int x, int y, int nWidth, int nHeight);		// View picture control 이동/크기변경

	bool isOnImg(CPoint clientCoorMouse);		// clientCoorMouse가 image 위에 있는지 여부 확인
	
	void FindImgCoorVIewFromImgCoorCenter(int imgCoorViewCenterX, int imgCoorViewCenterY);				// image coordinate Center로부터 image coordinate View 결정
	
	void ConvertClientCoorToImgCoor(CPoint clientCoorMouse, CPoint *imgCoorMouse);						// client coordinate -> image coordinate 변환
	void ConvertViewCoorToImgCoor(CPoint viewCoorMouse, int* imgCoorMouseX, int* imgCoorMouseY);		// viewr coordinate을 -> image coordiate 변환

	void ZoomInOut(int PosX, int PosY, CPoint point);
	
	void FitWindow(void);		// View 꽉 채우기

};