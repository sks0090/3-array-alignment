#pragma once
#include "pch.h"

#include "SapClassBasic.h"

// image coordinate : �̹��� ���� ���� �� �𼭸� (0,0)
// view coordinate : picture pox ���� ���� �� �𼭸� (0,0)
// client coordinate : ���̾�α�(Ŭ���̾�Ʈ) ���� ���� �� �𼭸� (0,0)
// Window coordinate : ����� ��ü ȭ�� ���� �� �𼭸� (0,0)

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
	int m_sizeImgX, m_sizeImgY, m_channel;	// Image ũ��
	int m_sizeViewX, m_sizeViewY;			// View ũ��
	
	int m_imgCoorMouseDragX, m_imgCoorMouseDragY;		// ���콺 ���� ��ư ������ �� ��ġ
	
	int m_imgCoorViewX, m_imgCoorViewY;					// View�� ������ �𼭸� ��ġ�� ���� �̹��� ��ǥ
	int m_imgCoorViewCenterX, m_imgCoorViewCenterY;		// VIew�� �߾� �̹��� ��ǥ
	int m_imgCoorMouseX, m_imgCoorMouseY;				// ���콺 ��ġ�� �̹��� ��ǥ

	int row, col;

	float m_fMagFitMin;
	float m_fMagMax;
	float m_fMagX, m_fMagY;
	float m_fMagstep;
	float scrollStep;

	//unsigned char* m_dataView;		// View �̹��� data
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
	bool OnMouseMove(UINT nFlags, CPoint windowCoorMouse);					// �ƿ콺 ������ �̺�Ʈ ó����
	void OnMouseWheel(UINT nFlags, short zDelta, CPoint windowCoorMouse);		// ���콺 �� �̺�Ʈ ó����
	void OnMouseLeave(void);							// ���콺 ��ġ�� ���� dialog�� ��� ���
	void OnLButtonDown(UINT nFlags, CPoint clientCoorMouse);		// ���콺 ���� ��ư down
	void OnLButtonUp(UINT nFlags, CPoint clientCoorMouse);		// ���콺 ���� ��ư up
	
	// General
	bool Init(CStatic* picbox, unsigned char* data, int width, int height, int channel, bool bSlave = false, MyGUI* RefGui = NULL);		// DC �̿�
	bool Init(CStatic* picbox, SapBuffer* buffer, SapView* view, bool bSlave = false, MyGUI* RefGui = NULL);	// Sapera �̿�
	void Free(void);
	void UpdateViewRect(void);		// Dialog ���� picture box ��ǥ �� magfit ����

	void Show(unsigned char* data, int PosCenterX, int PosCenterY);
	void Show(int imgCoorViewCenterX, int imgCoorViewCenterY);
	void Show();

	void MoveWindow(int x, int y, int nWidth, int nHeight);		// View picture control �̵�/ũ�⺯��

	bool isOnImg(CPoint clientCoorMouse);		// clientCoorMouse�� image ���� �ִ��� ���� Ȯ��
	
	void FindImgCoorVIewFromImgCoorCenter(int imgCoorViewCenterX, int imgCoorViewCenterY);				// image coordinate Center�κ��� image coordinate View ����
	
	void ConvertClientCoorToImgCoor(CPoint clientCoorMouse, CPoint *imgCoorMouse);						// client coordinate -> image coordinate ��ȯ
	void ConvertViewCoorToImgCoor(CPoint viewCoorMouse, int* imgCoorMouseX, int* imgCoorMouseY);		// viewr coordinate�� -> image coordiate ��ȯ

	void ZoomInOut(int PosX, int PosY, CPoint point);
	
	void FitWindow(void);		// View �� ä���

};