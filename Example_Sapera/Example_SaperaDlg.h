
// Example_SaperaDlg.h: 헤더 파일
//

#pragma once
#include "SapClassBasic.h"		// Sapera Header
//#include "SapClassGui.h"
#include "SapMyProcessing.h"	// Sapera custom Processing Header

#include "My_GUI.h"

#define WM_UPDATE_MSG       WM_USER + 1				// UI 업데이트

// CExampleSaperaDlg 대화 상자
class CExampleSaperaDlg : public CDialogEx
{
// 생성입니다.
public:
	CExampleSaperaDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXAMPLE_SAPERA_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	///////////////////////////////////////// GUI 변수 /////////////////////////////////////////
	// Combo Box
	CComboBox m_cbFG;

	// Edit Control
	int m_nBuffer;
	int m_nGrabFrame;

	int m_nTrashCnt;
	int m_nGrabCnt;
	int m_nProcCnt;

	int m_nXferCycle;
	int m_nProCycle;

	// List Box
	CListBox m_listStatus;

	// Button
	CButton m_InitFree;
	CButton m_Snap;
	CButton m_StartStop;
	CButton m_Processing;

	int		m_nWidthPicCtrl, m_nHeightPicCtrl;		// Picture Control 가로/세로 크기

	///////////////////////////////////////// Sapera 변수 /////////////////////////////////////////
	SapManager* m_Manager;		// Manager
	SapAcquisition* m_Acq;		// Frame grabber
	SapBuffer* m_Buffer;		// Buffer
	SapBuffer* m_Buffer0, * m_Buffer1, * m_Buffer2;	// for Display each plane
	SapTransfer* m_Xfer;		// Transfer
	SapView *m_View1, *m_View2, *m_View3;			// View

	SapMyProcessing* m_Pro;		// Processing
	SapBuffer* m_BufferPro;		// Processing Buffer

	// Display
	CStatic m_ViewWnd1, m_ViewWnd2, m_ViewWnd3;
	//CImageWnd *m_ImageWnd1, *m_ImageWnd2, *m_ImageWnd3;

	CScrollBar m_ctrlScrollHor;
	CScrollBar m_ctrlScrollVer;

	///////////////////////////////////////// 일반 변수 /////////////////////////////////////////
	MyGUI* m_pMyGui1, * m_pMyGui2, *m_pMyGui3;
	
	CString			m_strBoardName;		// 프레임 그래버 이름
	CString			m_strCCFPath;		// CCF 경로

	int		m_nSizeX;					// 버퍼 width
	int		m_nSizeY;					// 버퍼 height
	int		m_nBitDepth;
	int		m_nPlanes;					// plane 수
	UINT32	m_OutputFormat;				// CORBUFFER_VAL_FORMAT_MONO8, CORBUFFER_VAL_FORMAT_MONO16, CORBUFFER_VAL_FORMAT_RGBP8
	UINT32 m_CameraType;				// CORACQ_VAL_SCAN_AREA, CORACQ_VAL_SCAN_LINE
	
	char m_charTriggerMode[50];

	bool			m_bProcessing;
	bool			m_bInit;

	unsigned char* m_DataGrab;
	unsigned char* m_Data0;
	unsigned char* m_Data1;
	unsigned char* m_Data2;

	// 시간 측정
	LARGE_INTEGER		m_perfFrequency;
	LARGE_INTEGER		xferStartTime, xferEndTime;
	LARGE_INTEGER		proStartTime, proEndTime;

	int m_nXferCycleSum;

	bool m_bEnableDisplay;

	double m_fMagFitHor, m_fMagFitVert, m_fMag;

public:		
	///////////////////////////////////////// 일반 Methods /////////////////////////////////////////
	bool	InitCtrl();

	bool	InitSap();
	void	FreeSap();
	bool	InitSapPro();
	void	FreeSapPro();

	bool	Snap(int num);
	bool	GrabStart();
	bool	GrabStop();

	// Event Callback Method
	static void XferCallback(SapXferCallbackInfo* pInfo);
	static void ProCallback(SapProCallbackInfo* pInfo);
	void Xfer_Callback(SapXferCallbackInfo* pInfo);
	void Pro_Callback(SapProCallbackInfo* pInfo);

	///////////////////////////////////////// GUI Event Methods /////////////////////////////////////////
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void DisplayStatus(const TCHAR* fmt, ...);
	afx_msg LRESULT OnUpdateMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnCbnSelchangeCbFg();
	afx_msg void OnBnClickedCheckInitFree();
	afx_msg void OnBnClickedCheckSnap();
	afx_msg void OnBnClickedCheckStartStop();
	afx_msg void OnBnClickedBtnSaveImg();
	afx_msg void OnBnClickedCheckProc();
	
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnFile3imagesload();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
};
