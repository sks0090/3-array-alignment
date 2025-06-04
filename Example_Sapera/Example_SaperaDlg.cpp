
// Example_SaperaDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Example_Sapera.h"
#include "Example_SaperaDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CExampleSaperaDlg 대화 상자



CExampleSaperaDlg::CExampleSaperaDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EXAMPLE_SAPERA_DIALOG, pParent)
	, m_nBuffer(2)
	, m_nGrabFrame(1)
	, m_nGrabCnt(0)
	, m_nTrashCnt(0)
	, m_nProcCnt(0)
	, m_nXferCycle(0)
	, m_nProCycle(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CExampleSaperaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CB_FG, m_cbFG);
	DDX_Text(pDX, IDC_EDIT_BUF_NUM, m_nBuffer);
	DDX_Text(pDX, IDC_EDIT_FRAME_NUM, m_nGrabFrame);
	DDX_Control(pDX, IDC_LIST_STATUS, m_listStatus);
	DDX_Control(pDX, IDC_CHECK_INIT_FREE, m_InitFree);
	DDX_Control(pDX, IDC_CHECK_SNAP, m_Snap);
	DDX_Control(pDX, IDC_CHECK_START_STOP, m_StartStop);
	DDX_Text(pDX, IDC_EDIT_GRAB_CNT, m_nGrabCnt);
	DDX_Control(pDX, IDC_CHECK_PROC, m_Processing);
	DDX_Text(pDX, IDC_EDIT_TRASH_CNT, m_nTrashCnt);
	DDX_Text(pDX, IDC_EDIT_PROC_CNT, m_nProcCnt);
	DDX_Text(pDX, IDC_EDIT_XFER_CYCLE, m_nXferCycle);
	DDX_Text(pDX, IDC_EDIT_PRO_CYCLE, m_nProCycle);
	DDX_Control(pDX, IDC_ST_DISPLAY, m_ViewWnd1);
	DDX_Control(pDX, IDC_ST_DISPLAY2, m_ViewWnd2);
	DDX_Control(pDX, IDC_ST_DISPLAY3, m_ViewWnd3);
	DDX_Control(pDX, IDC_HORZ_SCROLLBAR, m_ctrlScrollHor);
	DDX_Control(pDX, IDC_VERT_SCROLLBAR, m_ctrlScrollVer);
}

BEGIN_MESSAGE_MAP(CExampleSaperaDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_UPDATE_MSG, &CExampleSaperaDlg::OnUpdateMsg)
	ON_CBN_SELCHANGE(IDC_CB_FG, &CExampleSaperaDlg::OnCbnSelchangeCbFg)
	ON_BN_CLICKED(IDC_CHECK_INIT_FREE, &CExampleSaperaDlg::OnBnClickedCheckInitFree)
	ON_BN_CLICKED(IDC_CHECK_SNAP, &CExampleSaperaDlg::OnBnClickedCheckSnap)
	ON_BN_CLICKED(IDC_CHECK_START_STOP, &CExampleSaperaDlg::OnBnClickedCheckStartStop)
	ON_BN_CLICKED(IDC_BTN_SAVE_IMG, &CExampleSaperaDlg::OnBnClickedBtnSaveImg)
	ON_BN_CLICKED(IDC_CHECK_PROC, &CExampleSaperaDlg::OnBnClickedCheckProc)
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_FILE_3IMAGESLOAD, &CExampleSaperaDlg::OnFile3imagesload)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSELEAVE()
END_MESSAGE_MAP()


// CExampleSaperaDlg 메시지 처리기

BOOL CExampleSaperaDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	InitCtrl();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CExampleSaperaDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CExampleSaperaDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		//m_ImageWnd1->OnPaint();
		//m_ImageWnd2->OnPaint();
		//m_ImageWnd3->OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CExampleSaperaDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CExampleSaperaDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
}

BOOL CExampleSaperaDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			// ESC 키 이벤트에 대한 처리 추가
			return TRUE;
		}
		else if (pMsg->wParam == VK_RETURN)
		{
			// Enter 키 이벤트에 대한 처리 추가
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CExampleSaperaDlg::DisplayStatus(const TCHAR* fmt, ...)
{
	CString strMsg;
	va_list args = NULL;
	va_start(args, fmt);
	strMsg.FormatV(fmt, args);
	va_end(args);

	CExampleSaperaDlg* pDlg = (CExampleSaperaDlg*)AfxGetApp()->GetMainWnd();
	if (pDlg && pDlg->GetSafeHwnd() && ::IsWindow(pDlg->GetSafeHwnd()))
	{
		pDlg->SendMessage(WM_UPDATE_MSG, (WPARAM)(LPSTR)(LPCTSTR)strMsg, 1);
	}
}

LRESULT CExampleSaperaDlg::OnUpdateMsg(WPARAM wParam, LPARAM lParam)
{
	LPCTSTR szMsg = (LPCTSTR)wParam;
	int messageType = int(lParam);

	CString strMsg;

	switch (messageType)
	{
	case 0:		
		SetDlgItemInt(IDC_EDIT_GRAB_CNT, m_nGrabCnt);
		SetDlgItemInt(IDC_EDIT_TRASH_CNT, m_nTrashCnt);
		//SetDlgItemInt(IDC_EDIT_PROC_CNT, m_nProcCnt);
		//SetDlgItemInt(IDC_EDIT_XFER_CYCLE, m_nXferCycle);
		//SetDlgItemInt(IDC_EDIT_PRO_CYCLE, m_nProCycle);
		
		break;
	case 1:
		SYSTEMTIME systemTime;
		::GetLocalTime(&systemTime);
		strMsg.Format(_T("%04d%02d%02d %02d:%02d:%02d:%03d> %s"), systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond, systemTime.wMilliseconds,
			szMsg);

		m_listStatus.InsertString(0, strMsg);

		if (m_listStatus.GetCount() > 1000)
		{
			m_listStatus.DeleteString(1000);
		}
		break;
	default:
		break;
	}

	return 0;
}

bool CExampleSaperaDlg::InitCtrl()
{
	int countFG;
	char nameFG[100];

	m_bInit = false;

	// Get Dalsa Frame Grabber List
	countFG = m_Manager->GetServerCount();
	for (int i = 0; i < countFG; i++) {
		m_Manager->GetServerName(i, nameFG, sizeof(nameFG));
		m_cbFG.AddString(CA2T(nameFG));
	}

	m_cbFG.SetCurSel(countFG-1);
	m_cbFG.GetWindowText(m_strBoardName);

	QueryPerformanceFrequency(&m_perfFrequency);
	xferStartTime.QuadPart = 0;
	xferEndTime.QuadPart = 0;
	proStartTime.QuadPart = 0;
	proEndTime.QuadPart = 0;

	m_nXferCycleSum = 0;
	m_bEnableDisplay = true;
	m_bProcessing = false;
	m_bProcessing2 = false;

	ShowWindow(SW_MAXIMIZE);

	return true;
}

bool CExampleSaperaDlg::InitSap()
{
	m_Acq = new SapAcquisition(SapLocation(m_strBoardName, 0), m_strCCFPath, SapAcquisition::EventEndOfFrame, NULL, NULL);
	if (!m_Acq->Create())
		return false;

	m_Acq->GetParameter(CORACQ_PRM_CROP_WIDTH, &m_nSizeX);
	m_Acq->GetParameter(CORACQ_PRM_CROP_HEIGHT, &m_nSizeY);
	m_Acq->GetParameter(CORACQ_PRM_PIXEL_DEPTH, &m_nBitDepth);
	m_Acq->GetParameter(CORACQ_PRM_OUTPUT_FORMAT, &m_OutputFormat);
	m_Acq->GetParameter(CORACQ_PRM_SCAN, &m_CameraType);	// 1: Area(CORACQ_VAL_SCAN_AREA), 2: Line(CORACQ_VAL_SCAN_LINE)
	if (m_OutputFormat != CORBUFFER_VAL_FORMAT_MONO8P3) {
		DisplayStatus(_T("3 plane mode를 선택해주세요."));
		return false;
	}

	m_fMagFitHor = (double)(m_nWidthPicCtrl) / (double)m_nSizeX;
	m_fMagFitVert = (double)(m_nHeightPicCtrl) / (double)m_nSizeY;
	m_fMag = m_fMagFitVert;

	if (m_CameraType == CORACQ_VAL_SCAN_AREA) {
		DisplayStatus(_T("Area Mode CCF 입니다."));
	}
	else if (m_CameraType == CORACQ_VAL_SCAN_LINE) {
		DisplayStatus(_T("TDI mode CCF 입니다."));
	}
	else {
		AfxMessageBox("알맞은 CCF를 선택해 주세요.");
	}

	m_Buffer = new SapBufferWithTrash(m_nBuffer, m_Acq);
	if (!m_Buffer->Create())
		return false;

	m_Buffer0 = new SapBuffer(2, m_nSizeX, m_nSizeY, SapFormatMono8, SapBuffer::TypeScatterGather);
	m_Buffer1 = new SapBuffer(2, m_nSizeX, m_nSizeY, SapFormatMono8, SapBuffer::TypeScatterGather);
	m_Buffer2 = new SapBuffer(2, m_nSizeX, m_nSizeY, SapFormatMono8, SapBuffer::TypeScatterGather);
	m_Buffer0->Create();
	m_Buffer1->Create();
	m_Buffer2->Create();

	SapData value = SapDataMono(128);

	m_Buffer0->Clear(value);
	m_Buffer1->Clear(value);
	m_Buffer2->Clear(value);

	m_Buffer0->GetParameter(0, CORBUFFER_PRM_ADDRESS, &m_Data0);
	m_Buffer1->GetParameter(0, CORBUFFER_PRM_ADDRESS, &m_Data1);
	m_Buffer2->GetParameter(0, CORBUFFER_PRM_ADDRESS, &m_Data2);

	m_Xfer = new SapAcqToBuf(m_Acq, m_Buffer, XferCallback, (void *)this);
	if (!m_Xfer->Create())
		return false;

	m_View1 = new SapView(m_Buffer0, m_ViewWnd1.GetSafeHwnd());
	if (!m_View1->Create())
		return false;
	m_View1->SetScalingMode(m_fMagFitVert, m_fMagFitVert);

	m_View2 = new SapView(m_Buffer1, m_ViewWnd2.GetSafeHwnd());
	if (!m_View2->Create())
		return false;
	m_View2->SetScalingMode(m_fMagFitVert, m_fMagFitVert);

	m_View3 = new SapView(m_Buffer2, m_ViewWnd3.GetSafeHwnd());
	if (!m_View3->Create())
		return false;
	m_View3->SetScalingMode(m_fMagFitVert, m_fMagFitVert);

	//m_ImageWnd1 = new CImageWnd(m_View1, &m_ViewWnd1, &m_ctrlScrollHor, &m_ctrlScrollVer, NULL);
	//m_ImageWnd2 = new CImageWnd(m_View2, &m_ViewWnd2, &m_ctrlScrollHor, &m_ctrlScrollVer, NULL);
	//m_ImageWnd3 = new CImageWnd(m_View3, &m_ViewWnd3, &m_ctrlScrollHor, &m_ctrlScrollVer, NULL);
	
	return true;
}

void CExampleSaperaDlg::FreeSap()
{
	// Delete all object pointer
	if (m_Xfer) {
		m_Xfer->Destroy();
		
		delete m_Xfer;
	}

	if (m_View1) {
		m_View1->Destroy();
		delete m_View1;
	}
	if (m_View2) {
		m_View2->Destroy();
		delete m_View2;
	}
	if (m_View3) {
		m_View3->Destroy();
		delete m_View3;
	}

	//if (m_ImageWnd1)   delete m_ImageWnd1;
	//if (m_ImageWnd2)   delete m_ImageWnd2;
	//if (m_ImageWnd3)   delete m_ImageWnd3;

	if (m_Buffer0) {
		m_Buffer0->Destroy();
		delete m_Buffer0;
	}
	if (m_Buffer1) {
		m_Buffer1->Destroy();
		delete m_Buffer1;
	}
	if (m_Buffer2) {
		m_Buffer2->Destroy();
		delete m_Buffer2;
	}

	if (m_Buffer) {
		m_Buffer->Destroy();
		delete m_Buffer;
	}
	if (m_Acq) {
		m_Acq->Destroy();
		delete m_Acq;
	}
}

void CExampleSaperaDlg::XferCallback(SapXferCallbackInfo* pInfo)
{
	CExampleSaperaDlg *pDlg = (CExampleSaperaDlg *)pInfo->GetContext();

	pDlg->Xfer_Callback(pInfo);
}

void CExampleSaperaDlg::Xfer_Callback(SapXferCallbackInfo* pInfo)
{
	QueryPerformanceCounter(&xferEndTime);
	if (m_nGrabCnt > 1) {
		m_nXferCycle = (xferEndTime.QuadPart - xferStartTime.QuadPart) * 1000.f / m_perfFrequency.QuadPart;
		m_nXferCycleSum += m_nXferCycle;

		if (m_nXferCycleSum > 30)
		{
			m_bEnableDisplay = true;
			m_nXferCycleSum = 0;
		}
		else {
			m_bEnableDisplay = false;
		}
	}
	else {
		m_bEnableDisplay = true;
	}



	if (pInfo->IsTrash())
	{
		++m_nTrashCnt;
	}

	// Refresh view
	else
	{
		++m_nGrabCnt;

		int bufIndex = m_Buffer->GetIndex();


		m_Buffer->GetParameter(bufIndex, CORBUFFER_PRM_ADDRESS, &m_DataGrab);
		memcpy(m_Data0, m_DataGrab, m_nSizeX * m_nSizeY);
		memcpy(m_Data1, m_DataGrab + (m_nSizeX * m_nSizeY) * 1, m_nSizeX * m_nSizeY);
		memcpy(m_Data2, m_DataGrab + (m_nSizeX * m_nSizeY) * 2, m_nSizeX * m_nSizeY);

		// Processing 수행
		if (m_bProcessing) {
			m_eventProcessing.SetEvent();
			m_eventProcessing2.SetEvent();
		}

		if (m_bEnableDisplay) {
			if (!m_pMyGui1->m_bDown)		// 마우스 왼쪽 버튼 Down 상태
			{
				m_pMyGui1->Show();
				m_pMyGui2->Show();
				m_pMyGui3->Show();
			}
		}
		//PostMessage(WM_UPDATE_MSG, 0, 0);
	}

	QueryPerformanceCounter(&xferStartTime);
}

UINT CExampleSaperaDlg::ProcessingThread(LPVOID lParam)
{
	CExampleSaperaDlg* pDlg = (CExampleSaperaDlg*)lParam;
	int count = 9;
	CString sText;

	double* retX = (double*)calloc(count, sizeof(double));
	double* retY = (double*)calloc(count, sizeof(double));
	double* retM = (double*)calloc(count, sizeof(double));

	while (pDlg->m_bProcessing)
	{
		sText.Format("Start(%d,%d) Size(%d,%d)", pDlg->m_pMyGui1->m_imgCoorViewX, pDlg->m_pMyGui1->m_imgCoorViewY, int(pDlg->m_pMyGui1->m_sizeViewX / pDlg->m_pMyGui1->m_fMagX), int(pDlg->m_pMyGui1->m_sizeViewY / pDlg->m_pMyGui1->m_fMagY));
		pDlg->DisplayStatus(_T(sText));

		//pDlg->m_pMyFft.CalCorrelation(pDlg->m_Data0, pDlg->m_Data1, pDlg->m_nSizeX, pDlg->m_nSizeY, pDlg->m_pMyGui1->m_imgCoorViewX, pDlg->m_pMyGui1->m_imgCoorViewY, int(pDlg->m_pMyGui1->m_sizeViewX / pDlg->m_pMyGui1->m_fMagX), int(pDlg->m_pMyGui1->m_sizeViewY / pDlg->m_pMyGui1->m_fMagY), 10, retX, retY);
		pDlg->m_pMyFft.CalCorrelation2(pDlg->m_Data0, pDlg->m_Data1, pDlg->m_nSizeX, pDlg->m_nSizeY, pDlg->m_pMyGui1->m_imgCoorViewX, pDlg->m_pMyGui1->m_imgCoorViewY, int(pDlg->m_pMyGui1->m_sizeViewX / pDlg->m_pMyGui1->m_fMagX), int(pDlg->m_pMyGui1->m_sizeViewY / pDlg->m_pMyGui1->m_fMagY), 10, retX, retY, retM, count);

		for (int i = 0; i < count; i++)
		{
			//sText.Format("rank:%d X:%f Y:%f M:%f TopRank:%f", i, retX[i], retY[i], retM[i], retM[i] / retM[0]);
			//pDlg->DisplayStatus(_T(sText));
		}

		if ((retM[1] / retM[0]) < 0.25)
		{
			count = 1;
		}
		else
		{
			count = 4;
		}

		double sumX = 0, sumY = 0, sumM = 0;// , sumMX = 0, sumMY = 0;
		for (int i = 0; i < count; i++)
		{
			//	if ((retM[i] / retM[0]) > 0.25)
			{
				sumX += retX[i] * retM[i];
				sumY += retY[i] * retM[i];
				sumM += retM[i];
			}
		}

		//sText.Format("TotalSum M:%f", sumM);
		//pDlg->DisplayStatus(_T(sText));

		if ((sumY / sumM) > 0)
		{
			sText.Format("Grav Y(Array 1 is behind of Array 0):%f Extend tube", sumY / sumM);
		}
		else
		{
			sText.Format("Grav Y(Array 1 is ahead of Array 0):%f Extend tube", sumY / sumM);
		}
		pDlg->DisplayStatus(_T(sText));

		if ((sumX / sumM) < 0)
		{
			sText.Format("Grav X(Array 1 is right side of Array 0):%f Rotate camera", sumX / sumM);
		}
		else
		{
			sText.Format("Grav X(Array 1 is left side of Array 0):%f Rotate camera", sumX / sumM);
		}
		pDlg->DisplayStatus(_T(sText));

		// 현재 쓰레드 카운터를 가져옴
		WaitForSingleObject(pDlg->m_eventProcessing, INFINITE);
	}

	free(retX);
	free(retY);
	free(retM);

	return 1;
}

UINT CExampleSaperaDlg::ProcessingThread2(LPVOID lParam)
{
	CExampleSaperaDlg* pDlg = (CExampleSaperaDlg*)lParam;
	int count = 9;
	CString sText;

	double* retX = (double*)calloc(count, sizeof(double));
	double* retY = (double*)calloc(count, sizeof(double));
	double* retM = (double*)calloc(count, sizeof(double));

	while (pDlg->m_bProcessing2)
	{
		//sText.Format("Start(%d,%d) Size(%d,%d)", pDlg->m_pMyGui1->m_imgCoorViewX, pDlg->m_pMyGui1->m_imgCoorViewY, int(pDlg->m_pMyGui1->m_sizeViewX / pDlg->m_pMyGui1->m_fMagX), int(pDlg->m_pMyGui1->m_sizeViewY / pDlg->m_pMyGui1->m_fMagY));
		//pDlg->DisplayStatus(_T(sText));

		//pDlg->m_pMyFft.CalCorrelation(pDlg->m_Data0, pDlg->m_Data1, pDlg->m_nSizeX, pDlg->m_nSizeY, pDlg->m_pMyGui1->m_imgCoorViewX, pDlg->m_pMyGui1->m_imgCoorViewY, int(pDlg->m_pMyGui1->m_sizeViewX / pDlg->m_pMyGui1->m_fMagX), int(pDlg->m_pMyGui1->m_sizeViewY / pDlg->m_pMyGui1->m_fMagY), 10, retX, retY);
		pDlg->m_pMyFft.CalCorrelation2(pDlg->m_Data1, pDlg->m_Data2, pDlg->m_nSizeX, pDlg->m_nSizeY, pDlg->m_pMyGui1->m_imgCoorViewX, pDlg->m_pMyGui1->m_imgCoorViewY, int(pDlg->m_pMyGui1->m_sizeViewX / pDlg->m_pMyGui1->m_fMagX), int(pDlg->m_pMyGui1->m_sizeViewY / pDlg->m_pMyGui1->m_fMagY), 10, retX, retY, retM, count);

		for (int i = 0; i < count; i++)
		{
			//sText.Format("rank:%d X:%f Y:%f M:%f TopRank:%f", i, retX[i], retY[i], retM[i], retM[i] / retM[0]);
			//pDlg->DisplayStatus(_T(sText));
		}

		if ((retM[1] / retM[0]) < 0.25)
		{
			count = 1;
		}
		else
		{
			count = 4;
		}

		double sumX = 0, sumY = 0, sumM = 0;// , sumMX = 0, sumMY = 0;
		for (int i = 0; i < count; i++)
		{
			//	if ((retM[i] / retM[0]) > 0.25)
			{
				sumX += retX[i] * retM[i];
				sumY += retY[i] * retM[i];
				sumM += retM[i];
			}
		}

		//sText.Format("TotalSum M:%f", sumM);
		//pDlg->DisplayStatus(_T(sText));

		if ((sumY / sumM) > 0)
		{
			sText.Format("Grav Y(Array 2 is behind of Array 1):%f Extend tube", sumY / sumM);
		}
		else
		{
			sText.Format("Grav Y(Array 2 is ahead of Array 1):%f Extend tube", sumY / sumM);
		}
		pDlg->DisplayStatus(_T(sText));

		if ((sumX / sumM) < 0)
		{
			sText.Format("Grav X(Array 2 is right side of Array 1):%f Rotate camera", sumX / sumM);
		}
		else
		{
			sText.Format("Grav X(Array 2 is left side of Array 1):%f Rotate camera", sumX / sumM);
		}
		pDlg->DisplayStatus(_T(sText));

		// 현재 쓰레드 카운터를 가져옴
		WaitForSingleObject(pDlg->m_eventProcessing2, INFINITE);
	}

	free(retX);
	free(retY);
	free(retM);

	return 1;
}

bool CExampleSaperaDlg::Snap(int num)
{
	if (m_Xfer->Snap(num))
	{
		//if (!m_Xfer->Wait(10000)) {		// 10초 동안 Snap 완료 안될 시, 강제 종료
		//	m_Xfer->Abort();
		//
		//	return false;
		//}
	}
	else {
		return false;
	}

	return true;
}

bool CExampleSaperaDlg::GrabStart()
{
	if (!m_Xfer->Grab())
	{
		return false;
	}

	return true;
}

bool CExampleSaperaDlg::GrabStop()
{
	if (m_Xfer->Freeze()) {
		if (!m_Xfer->Wait(1000))
		{
			m_Xfer->Abort();
			return false;
		}
	}
	else {
		return false;
	}

	return true;
}

void CExampleSaperaDlg::OnCbnSelchangeCbFg()
{
	m_cbFG.GetWindowText(m_strBoardName);
}


void CExampleSaperaDlg::OnBnClickedCheckInitFree()
{
	UpdateData(TRUE);

	if (m_InitFree.GetCheck())
	{
		// CCF 선택
		TCHAR szFilter[] = _T("ccf file(*.ccf) | *.ccf;|*.*||");
		CFileDialog dlg(TRUE, _T("*.ccf"), _T("ccf"), OFN_HIDEREADONLY, szFilter);
		if (IDOK != dlg.DoModal())
		{
			m_InitFree.SetCheck(BST_UNCHECKED);
			return;
		}

		m_strCCFPath = dlg.GetPathName();

		// Sapera Init
		if (!InitSap())
		{
			 m_InitFree.SetCheck(BST_UNCHECKED);
			 FreeSap();
			AfxMessageBox(_T("Failed in InitSapera"));
			return;
		}

		m_pMyGui1 = new MyGUI;
		m_pMyGui2 = new MyGUI;
		m_pMyGui3 = new MyGUI;
		m_pMyGui1->Init(&m_ViewWnd1, m_Buffer0, m_View1, false);
		m_pMyGui2->Init(&m_ViewWnd2, m_Buffer1, m_View2, true, m_pMyGui1);
		m_pMyGui3->Init(&m_ViewWnd3, m_Buffer2, m_View3, true, m_pMyGui1);

		m_bInit = true;

		m_InitFree.SetWindowTextA(_T("Free"));
		DisplayStatus(_T("Sapera 할당 완료 - 할당 버퍼 개수 : %d"), m_nBuffer);
	}
	else
	{
		if (m_bProcessing)
		{
			m_Processing.SetCheck(BST_UNCHECKED);
			OnBnClickedCheckProc();
		}

		FreeSap();

		m_bInit = false;

		m_InitFree.SetWindowTextA(_T("Init"));
		DisplayStatus(_T("Sapera 해제 완료"));
	}
}


void CExampleSaperaDlg::OnBnClickedCheckSnap()
{
	UpdateData(TRUE);

	if (m_Snap.GetCheck())
	{
		if (m_Xfer->IsGrabbing())
		{
			m_Snap.SetCheck(BST_UNCHECKED);
			return;
		}

		m_nGrabCnt = 0;
		m_nTrashCnt = 0;
		m_nProcCnt = 0;
		m_nXferCycle = 0;
		m_nProCycle = 0;
		m_nXferCycleSum = 0;
		m_Snap.SetWindowTextA(_T("Acquiring..."));
		if (Snap(m_nGrabFrame))
		{
			//DisplayStatus(_T("Snap 완료"));
			m_Snap.SetCheck(BST_UNCHECKED);
		}
		else
		{
			DisplayStatus(_T("Snap 실패"));
			m_Snap.SetCheck(BST_UNCHECKED);
		}

		m_Snap.SetWindowTextA(_T("Snap"));
	}
	else
	{
		m_Snap.SetWindowTextA(_T("Snap"));
	}
}


void CExampleSaperaDlg::OnBnClickedCheckStartStop()
{
	UpdateData(TRUE);

	if (m_StartStop.GetCheck())
	{
		if (m_Xfer->IsGrabbing())
		{
			m_StartStop.SetCheck(BST_UNCHECKED);
			return;
		}
		m_nGrabCnt = 0;
		m_nTrashCnt = 0;
		m_nProcCnt = 0;
		m_nXferCycle = 0;
		m_nProCycle = 0;
		GrabStart();

		m_StartStop.SetWindowTextA(_T("Grab Stop"));
		//DisplayStatus(_T("Grab Start"));
	}
	else
	{
		GrabStop();

		m_StartStop.SetWindowTextA(_T("Grab Start"));
		//DisplayStatus(_T("Grab Stop"));
	}
}


void CExampleSaperaDlg::OnBnClickedBtnSaveImg()
{
	SYSTEMTIME systemTime;
	::GetLocalTime(&systemTime);

	CString strMonthFolder;
	strMonthFolder.Format(_T("%04d_%02d_%02d"), systemTime.wYear, systemTime.wMonth, systemTime.wDay);

	CFileFind finder;
	if (!finder.FindFile(strMonthFolder))
	{
		CreateDirectory(strMonthFolder, NULL);

	}

	CString strImgFile;

	//strImgFile.Format(_T("%s\\Img_%02d%02d%02d.crc"), strMonthFolder, systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
	//m_Buffer->Save(strImgFile, "-format crc", -1, 0);

	strImgFile.Format(_T("%s\\Img_0_%d_%02d%02d%02d.tif"), strMonthFolder, systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
	m_Buffer0->Save(strImgFile, "-format tiff", -1, 0);
	strImgFile.Format(_T("%s\\Img_1_%d_%02d%02d%02d.tif"), strMonthFolder, systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
	m_Buffer1->Save(strImgFile, "-format tiff", -1, 0);
	strImgFile.Format(_T("%s\\Img_2_%d_%02d%02d%02d.tif"), strMonthFolder, systemTime.wHour, systemTime.wMinute, systemTime.wSecond);
	m_Buffer2->Save(strImgFile, "-format tiff", -1, 0);

	DisplayStatus(_T("이미지 저장 완료"));
}


void CExampleSaperaDlg::OnBnClickedCheckProc()
{
	UpdateData(TRUE);

	if (m_Processing.GetCheck())
	{
		if (m_StartStop.GetCheck())
		{
			GrabStop();
			m_StartStop.SetCheck(BST_UNCHECKED);
			m_StartStop.SetWindowTextA(_T("Grab Start"));
			
		}

		// Processing Thread 생성
		m_bProcessing = true;
		m_pThreadProcessing = AfxBeginThread(ProcessingThread, this, THREAD_PRIORITY_NORMAL, 0);

		// Processing Thread 생성
		m_bProcessing2 = true;
		m_pThreadProcessing2 = AfxBeginThread(ProcessingThread2, this, THREAD_PRIORITY_NORMAL, 0);
		
		m_nGrabFrame = 1;
		m_StartStop.EnableWindow(FALSE);
		m_Processing.SetWindowTextA(_T("Enable Correlation"));
		DisplayStatus(_T("Enable Correlation 계산"));
	}
	else
	{
		// Processing Thread 종료
		m_bProcessing = false;
		m_eventProcessing.SetEvent();

		m_bProcessing2 = false;
		m_eventProcessing2.SetEvent();

		m_StartStop.EnableWindow(TRUE);
		m_Processing.SetWindowTextA(_T("Disable Correlation"));
		DisplayStatus(_T("Disble Correlation 계산"));
	}

	UpdateData(FALSE);
}


void CExampleSaperaDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	int posDisplayX = 165, posDisplayY = 8;
	int heightListStatus = 100;
	int gap = 4;

	m_nWidthPicCtrl = cx - posDisplayX - (gap*2) ;
	m_nHeightPicCtrl = (cy - posDisplayY - heightListStatus - gap*2) / 3 - gap;

	if (m_bInit) {
		m_fMagFitHor = (double)(m_nWidthPicCtrl) / (double)m_nSizeX;
		m_fMagFitVert = (double)(m_nHeightPicCtrl) / (double)m_nSizeY;
	}

	// resize image viewer
	if (m_ViewWnd1.GetSafeHwnd())
	{
		if (m_bInit) {
			m_pMyGui1->MoveWindow(posDisplayX, posDisplayY, m_nWidthPicCtrl, m_nHeightPicCtrl);
		}
		else {
			m_ViewWnd1.MoveWindow(posDisplayX, posDisplayY, m_nWidthPicCtrl, m_nHeightPicCtrl);
		}
	}
	if (m_ViewWnd2.GetSafeHwnd())
	{
		if (m_bInit) {
			m_pMyGui2->MoveWindow(posDisplayX, posDisplayY + m_nHeightPicCtrl + gap, m_nWidthPicCtrl, m_nHeightPicCtrl);
		}
		else {
			m_ViewWnd2.MoveWindow(posDisplayX, posDisplayY + m_nHeightPicCtrl + gap, m_nWidthPicCtrl, m_nHeightPicCtrl);
		}
	}
	if (m_ViewWnd3.GetSafeHwnd())
	{
		if (m_bInit) {
			m_pMyGui3->MoveWindow(posDisplayX, posDisplayY + (m_nHeightPicCtrl + gap) * 2, m_nWidthPicCtrl, m_nHeightPicCtrl);
		}
		else {
			m_ViewWnd3.MoveWindow(posDisplayX, posDisplayY + (m_nHeightPicCtrl + gap) * 2, m_nWidthPicCtrl, m_nHeightPicCtrl);
		}
	}

	if (m_ctrlScrollHor.GetSafeHwnd()) {
		m_ctrlScrollHor.MoveWindow(posDisplayX, cy - heightListStatus - gap*2, m_nWidthPicCtrl, gap);
	}

	if (m_ctrlScrollVer.GetSafeHwnd()) {
		m_ctrlScrollVer.MoveWindow(cx - gap - 4, posDisplayY, gap, (m_nHeightPicCtrl + gap)*3);
	}

	// Status List
	if (m_listStatus.GetSafeHwnd()) {
		m_listStatus.MoveWindow(posDisplayX, cy - heightListStatus, m_nWidthPicCtrl, heightListStatus - gap);
	}

	if (m_bInit) {
		m_pMyGui1->UpdateViewRect();
		m_pMyGui2->UpdateViewRect();
		m_pMyGui3->UpdateViewRect();
		if (!m_Xfer->IsGrabbing()) {
			m_pMyGui1->Show();
			m_pMyGui2->Show();
			m_pMyGui3->Show();
		}
	}
}

void CExampleSaperaDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar->GetDlgCtrlID() == IDC_HORZ_SCROLLBAR)
	{
		if (m_bInit) {
			// Adjust source's horizontal origin
			//if (m_ImageWnd1) {
			//	m_ImageWnd1->OnHScroll(nSBCode, nPos);
			//	//if (!m_Xfer->IsGrabbing()) {
			//		m_ImageWnd1->OnPaint();
			//	//}
			//}
			//if (m_ImageWnd2) {
			//	m_ImageWnd2->OnHScroll(nSBCode, nPos);
			//	//if (!m_Xfer->IsGrabbing()) {
			//		m_ImageWnd2->OnPaint();
			//	//}
			//}
			//if (m_ImageWnd3) {
			//	m_ImageWnd3->OnHScroll(nSBCode, nPos);
			//	//if (!m_Xfer->IsGrabbing()) {
			//		m_ImageWnd3->OnPaint();
			//	//}
			//}
		}
	}

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CExampleSaperaDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar->GetDlgCtrlID() == IDC_VERT_SCROLLBAR)
	{
		if (m_bInit) {
			// Adjust source's horizontal origin
			//if (m_ImageWnd1) {
			//	m_ImageWnd1->OnVScroll(nSBCode, nPos);
			//	//if (!m_Xfer->IsGrabbing()) {
			//		m_ImageWnd1->OnPaint();
			//	//}
			//}
			//if (m_ImageWnd2) {
			//	m_ImageWnd2->OnVScroll(nSBCode, nPos);
			//	//if (!m_Xfer->IsGrabbing()) {
			//		m_ImageWnd2->OnPaint();
			//	//}
			//}
			//if (m_ImageWnd3) {
			//	m_ImageWnd3->OnVScroll(nSBCode, nPos);
			//	//if (!m_Xfer->IsGrabbing()) {
			//		m_ImageWnd3->OnPaint();
			//	//}
			//}
		}
	}

	CDialogEx::OnVScroll(nSBCode, nPos, pScrollBar);
}

// point : dialog 왼쪽 위가 (0,0)인 client 좌표 기준 마우스 위치
void CExampleSaperaDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CString msg;

	if (m_bInit) {
		if (m_pMyGui1->OnMouseMove(nFlags, point))
		{
		
		}
		//msg = m_ImageWnd1->GetPixelString(m_Buffer0, point, TRUE, TRUE);
		
		//SetWindowText(msg);

		m_pMyGui2->OnMouseMove(nFlags, point);
		m_pMyGui3->OnMouseMove(nFlags, point);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}


BOOL CExampleSaperaDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (m_bInit) {
		m_pMyGui1->OnMouseWheel(nFlags, zDelta, pt);
		m_pMyGui2->OnMouseWheel(nFlags, zDelta, pt);
		m_pMyGui3->OnMouseWheel(nFlags, zDelta, pt);
	}
	//if (m_bInit)
	//{
	//	if (zDelta > 0)		// 스크롤 업(확대)
	//	{
	//
	//		m_fMag = m_fMag * 1.5;
	//		m_View1->SetScalingMode(m_fMag, m_fMag);
	//		m_View2->SetScalingMode(m_fMag, m_fMag);
	//		m_View3->SetScalingMode(m_fMag, m_fMag);
	//
	//		m_ImageWnd1->OnSize();
	//		m_ImageWnd2->OnSize();
	//		m_ImageWnd3->OnSize();
	//	}
	//	else {	// 스크롤 다운(축소)
	//		if (m_fMag > m_fMagFitHor)
	//		{
	//			m_fMag = m_fMag / 1.5;
	//			if (m_fMag < m_fMagFitHor)
	//			{
	//				m_fMag = m_fMagFitHor;
	//			}
	//			m_View1->SetScalingMode(m_fMag, m_fMag);
	//			m_View2->SetScalingMode(m_fMag, m_fMag);
	//			m_View3->SetScalingMode(m_fMag, m_fMag);
	//
	//			m_ImageWnd1->OnSize();
	//			m_ImageWnd2->OnSize();
	//			m_ImageWnd3->OnSize();
	//		}
	//	}
	//}

	

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


void CExampleSaperaDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_bInit)
	{
		m_pMyGui1->OnLButtonDown(nFlags, point);
		m_pMyGui2->OnLButtonDown(nFlags, point);
		m_pMyGui3->OnLButtonDown(nFlags, point);
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CExampleSaperaDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bInit)
	{
		m_pMyGui1->OnLButtonUp(nFlags, point);
		m_pMyGui2->OnLButtonUp(nFlags, point);
		m_pMyGui3->OnLButtonUp(nFlags, point);
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CExampleSaperaDlg::OnMouseLeave()
{
	if (m_bInit)
	{
		m_pMyGui1->OnMouseLeave();
		m_pMyGui2->OnMouseLeave();
		m_pMyGui3->OnMouseLeave();
	}

	CDialogEx::OnMouseLeave();
}

///////////////////////////////////////////////////////// <summary>			 /////////////////////////////////////////////////////////
///////////////////////////////////////////////////////// MENU 이벤트 처리기 /////////////////////////////////////////////////////////
///////////////////////////////////////////////////////// </summary>		 /////////////////////////////////////////////////////////

/// <summary>
/// File
/// </summary>
void CExampleSaperaDlg::OnFile3imagesload()
{
	// Image file 선택
	CString strFilePath[100];
	int fileCnt = 0;
	TCHAR szFilter[] = _T("image(*.bmp, *.tif, *.jpg, *.png, *.crc) | *.bmp; *.tif; *.jpg; *.png, *.crc | ALL Files(*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT, szFilter);
	if (IDOK != dlg.DoModal())
	{
		return;
	}

	POSITION pos = dlg.GetStartPosition();
	while (pos)
	{
		strFilePath[fileCnt] = dlg.GetNextPathName	(pos);
		fileCnt++;
	}

	if (fileCnt != 3) {
		AfxMessageBox("3개 이미지를 선택해 주세요.");
	}
}
