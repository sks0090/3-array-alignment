#include "pch.h"
#include "My_GUI.h"

MyGUI::MyGUI() {

}
MyGUI::~MyGUI() {

}

// 8bit 이미지용 DC Initialize
bool MyGUI::Init(CStatic* picbox, unsigned char* data, int width, int height, int channel, bool bSlave, MyGUI* RefGui) {
	m_picbox = picbox;

	m_pDC = m_picbox->GetDC();
	m_memDC.CreateCompatibleDC(m_pDC);		// memory DC 생성
	m_pDC->SetStretchBltMode(COLORONCOLOR);		// 이미지 확대/축소 방식 설정

	m_bitDepth = 8;
	m_sizeImgX = width;
	m_sizeImgY = height;
	m_channel = channel;

	//m_dataView = (unsigned char*)calloc(m_sizeImgX * m_sizeImgY * 3, sizeof(unsigned char));
	//m_dataView = new BYTE[m_sizeImgX * m_sizeImgY * 3];
	m_dataView = (unsigned int*)calloc(m_sizeImgX * m_sizeImgY, sizeof(unsigned int));

	m_imgCoorViewCenterX = m_sizeImgX / 2;
	m_imgCoorViewCenterY = m_sizeImgY / 2;

	m_bitmap.CreateBitmap(m_sizeImgX, m_sizeImgY, 1, 32, m_dataView);
	m_memDC.SelectObject(m_bitmap);

	UpdateViewRect();

	m_fMagX = m_fMagFitMin;
	m_fMagY = m_fMagFitMin;
	m_fMagstep = 1.5;
	scrollStep = 10;

	m_bSlave = bSlave;
	m_RefGui = RefGui;

	// View Thread 생성
	m_bThreadView = true;
	m_bThreadViewProc = false;
	m_ThreadView = AfxBeginThread(ViewThread, this, THREAD_PRIORITY_NORMAL, 0);

	m_bDown = false;
	m_bInit = true;

	Sleep(100);
	Show(data, m_imgCoorViewCenterX, m_imgCoorViewCenterY);

	return m_bInit;
}

bool MyGUI::Init(CStatic* picbox, SapBuffer* buffer, SapView* view, bool bSlave, MyGUI* RefGui) {
	m_picbox = picbox;
	m_Buffer = buffer;
	m_View = view;
	m_RefGui = RefGui;
	m_bSlave = bSlave;

	m_sizeImgX = buffer->GetWidth();
	m_sizeImgY = buffer->GetHeight();
	m_channel = buffer->GetBytesPerPixel();;
	m_bitDepth = 8;

	m_imgCoorViewCenterX = m_sizeImgX / 2;
	m_imgCoorViewCenterY = m_sizeImgY / 2;

	UpdateViewRect();

	m_fMagX = m_fMagFitMin;
	m_fMagY = m_fMagFitMin;
	m_fMagstep = 1.5;
	scrollStep = 10;

	m_bDown = false;
	m_bInit = true;

	Show(m_imgCoorViewCenterX, m_imgCoorViewCenterY);

	return m_bInit;
}

void MyGUI::Free() {
	m_bThreadView = false;
	m_eventThreadView.SetEvent();
	if (m_bThreadViewProc) {
		Sleep(200);
	}

	free(m_dataView);

	//m_bitmap.DeleteObject();

	//m_memDC.DeleteDC();

	//m_pDC->ReleaseAttribDC();
	//m_picbox->ReleaseDC(m_pDC);

	m_bInit = false;
}

void MyGUI::UpdateViewRect(void)
{
	float magFitX, magFitY;

	m_picbox->GetClientRect(&m_viewCoorViewRect);				// View coordinate Rect for picture box

	m_picbox->GetWindowRect(m_clientCoordinateViewRect);				// window coordinate Rect for picture box
	m_picbox->GetParent()->ScreenToClient(m_clientCoordinateViewRect);	// client coordinate Rect for picture box
	m_sizeViewX = m_clientCoordinateViewRect.right - m_clientCoordinateViewRect.left;
	m_sizeViewY = m_clientCoordinateViewRect.bottom - m_clientCoordinateViewRect.top;

	magFitX = (float)m_sizeViewX / (float)m_sizeImgX;
	magFitY = (float)m_sizeViewY / (float)m_sizeImgY;
	m_fMagFitMin = min(magFitX, magFitY);
	m_fMagMax = (float)m_sizeViewY / (float)4;
}

UINT MyGUI::ViewThread(LPVOID lParam)
{
	MyGUI* pDlg = (MyGUI*)lParam;
	int row, col, ch;

	WaitForSingleObject(pDlg->m_eventThreadView, INFINITE);
	while (pDlg->m_bThreadView)
	{
		// View 버퍼 업데이트
		if (pDlg->m_bitDepth == 8)	// 8 bit
		{
			for (int i = 0; i < pDlg->m_sizeImgX * pDlg->m_sizeImgY; i++)
			{
				pDlg->m_dataView[i] = (0xFF << 24) | (pDlg->m_data8[i] << 16) | (pDlg->m_data8[i] << 8) | (pDlg->m_data8[i]);	// ARGB 포맷으로 변환
			}
		}
		else
		{
			for (row = 0; row < pDlg->m_sizeImgY; row++) {
				for (col = 0; col < pDlg->m_sizeImgX; col++) {
					for (ch = 0; ch < pDlg->m_channel; ch++) {
						pDlg->m_dataView[(row * pDlg->m_sizeImgX + col) * pDlg->m_channel + ch] = pDlg->m_data16[row * pDlg->m_sizeImgX + col];
					}
				}
			}
		}
		SetBitmapBits(pDlg->m_bitmap, pDlg->m_sizeImgX * pDlg->m_sizeImgY * 4, pDlg->m_dataView);

		
		if (pDlg->m_bDown)	// 마우스 왼쪽 버튼으로 이미지 이동 시
			pDlg->FindImgCoorVIewFromImgCoorCenter(pDlg->m_imgCoorMouseDragX, pDlg->m_imgCoorMouseDragY);		// View 이동
		else
			pDlg->FindImgCoorVIewFromImgCoorCenter(pDlg->m_imgCoorViewCenterX, pDlg->m_imgCoorViewCenterY);

		// 이미지 그리기(Show)
		pDlg->m_pDC->StretchBlt(0, 0, pDlg->m_viewCoorViewRect.right, pDlg->m_viewCoorViewRect.bottom, &pDlg->m_memDC, pDlg->m_imgCoorViewX, pDlg->m_imgCoorViewY, pDlg->m_viewCoorViewRect.right / pDlg->m_fMagX, pDlg->m_viewCoorViewRect.bottom / pDlg->m_fMagY, SRCCOPY);	// 이미지 확대/축소 출력

		pDlg->m_bThreadViewProc = false;
		WaitForSingleObject(pDlg->m_eventThreadView, INFINITE);
	}
	return 0;
}

void MyGUI::Show(unsigned char* data, int PosCenterX, int PosCenterY)
{
	if (m_bInit)
	{
		// DC
		//CvtCenterPosToScrollPos(PosCenterX, PosCenterY);
		if (!m_bThreadViewProc)
		{
			m_data8 = data;
			m_bThreadViewProc = true;
			m_eventThreadView.SetEvent();
			//	(*ViewCNT)++;
		}

		// Sapera SDK
		
	}

}

void MyGUI::Show(int imgCoorViewCenterX, int imgCoorViewCenterY)
{
	FindImgCoorVIewFromImgCoorCenter(imgCoorViewCenterX, imgCoorViewCenterY);

	m_View->SetScalingMode(m_fMagX, m_fMagY, 0);
	//	m_View->getsc
	if (m_imgCoorViewX >= 0)
		m_View->OnHScroll(m_imgCoorViewX);
	if (m_imgCoorViewY >= 0)
		m_View->OnVScroll(m_imgCoorViewY);
	//TRACE("ScrollPos X%d Y%d\n", m_imgCoorViewX, m_imgCoorViewY);
	
	
	m_View->Show(0);
}

void MyGUI::Show()
{
	if (!m_bSlave) {
		//Show(m_imgCoorViewX, m_imgCoorViewY);
		Show(m_imgCoorViewCenterX, m_imgCoorViewCenterY);
	}
	else {
		Show(m_RefGui->m_imgCoorViewCenterX, m_RefGui->m_imgCoorViewCenterY);
	}
}

void MyGUI::MoveWindow(int x, int y, int nWidth, int nHeight)
{
	m_picbox->MoveWindow(x, y, nWidth, nHeight);

	UpdateViewRect();
}

void MyGUI::FindImgCoorVIewFromImgCoorCenter(int imgCoorViewCenterX, int imgCoorViewCenterY)
{
	if (imgCoorViewCenterX < int(float(m_sizeViewX) / 2.0 / m_fMagX + 0.5))		// 이미지 크기 < picture box 크기
		m_imgCoorViewX = 0;
	else if (m_sizeImgX < imgCoorViewCenterX + int(float(m_sizeViewX) / 2.0 / m_fMagX + 0.5))		// 이미지 우측 끝이 view를 벗어난 경우
		m_imgCoorViewX = m_sizeImgX - int(float(m_sizeViewX) / m_fMagX + 0.5);
	else  // 이미지 크기 > picture box 크기
		m_imgCoorViewX = imgCoorViewCenterX - int(float(m_sizeViewX) / 2.0 / m_fMagX + 0.5);

	if (imgCoorViewCenterY < int(float(m_sizeViewY) / 2.0 / m_fMagY + 0.5))		// 이미지 크기 < picture box 크기
		m_imgCoorViewY = 0;
	else if (m_sizeImgY < imgCoorViewCenterY + int(float(m_sizeViewY) / 2.0 / m_fMagY + 0.5))		// 이미지 아래 끝이 view를 벗어난 경우
		m_imgCoorViewY = m_sizeImgY - int(float(m_sizeViewY) / m_fMagY + 0.5);
	else  // 이미지 크기 > picture box 크기
		m_imgCoorViewY = imgCoorViewCenterY - int(float(m_sizeViewY) / 2.0 / m_fMagY + 0.5);
	//TRACE("m_imgCoorViewX : %d, imgCoorViewCenterX : %d, m_sizeImgX : %d, m_sizeViewX : %d\n", m_imgCoorViewX, imgCoorViewCenterX, m_sizeImgX, m_sizeViewX);
}

///////////////////////////////////////////////////// Mouse Event /////////////////////////////////////////////////////////
// point : dialog 왼쪽 위가 (0,0)인 client 좌표 기준 마우스 위치
bool MyGUI::OnMouseMove(UINT nFlags, CPoint clientCoorMouse)
{
	int indexImg;

	if (m_bInit)
	{
		
		if (isOnImg(clientCoorMouse))
		{
			// Mouse가 위치한 곳의 image 좌표
			m_imgCoorMouseX = m_imgCoorViewCenterX + int(double((clientCoorMouse.x - m_clientCoordinateViewRect.left)) / double(m_fMagX));
			m_imgCoorMouseY = m_imgCoorViewCenterY + int(double((clientCoorMouse.y - m_clientCoordinateViewRect.top)) / double(m_fMagY));
			//TRACE("image 좌표 : %d, %d\r", m_imgCoorMouseX, m_imgCoorMouseY);

			if (m_bDown)		// 마우스 왼쪽 버튼 Down 상태
			{
				if (m_bSlave)
				{
					m_imgCoorMouseDragX = m_RefGui->m_imgCoorViewCenterX + int(float(m_viewCoorBtnDown.x - (clientCoorMouse.x - m_clientCoordinateViewRect.left)) / m_fMagX + 0.5);
					m_imgCoorMouseDragY = m_RefGui->m_imgCoorViewCenterY + int(float(m_viewCoorBtnDown.y - (clientCoorMouse.y - m_clientCoordinateViewRect.top)) / m_fMagY + 0.5);
				}
				else
				{
					m_imgCoorMouseDragX = m_imgCoorViewCenterX + int(float(m_viewCoorBtnDown.x - (clientCoorMouse.x - m_clientCoordinateViewRect.left)) / m_fMagX + 0.5);
					m_imgCoorMouseDragY = m_imgCoorViewCenterY + int(float(m_viewCoorBtnDown.y - (clientCoorMouse.y - m_clientCoordinateViewRect.top)) / m_fMagY + 0.5);
				}
				
				//Show(m_data8, m_imgCoorViewCenterX + int(float(m_viewCoorBtnDown.x - (clientCoorMouse.x - m_clientCoordinateViewRect.left)) / m_fMagX + 0.5), m_imgCoorViewCenterY + int(float(m_viewCoorBtnDown.y - (clientCoorMouse.y - m_clientCoordinateViewRect.top)) / m_fMagY + 0.5));
				Show(m_imgCoorMouseDragX, m_imgCoorMouseDragY);
			}
			m_clientCoordinateMouse = clientCoorMouse;
			ConvertClientCoorToImgCoor(clientCoorMouse, &m_imageCoordinateMouse);
			//TRACE("image 좌표2 : %d, %d\n", m_imageCoordinateMouse.x, m_imageCoordinateMouse.y);
			//if (m_imageCoordinateMouse.x < m_sizeImgX && m_imageCoordinateMouse.y < m_sizeImgY) {
			//	return true;
			//}

			return false;
		}
		else
		{
			m_clientCoordinateMouse = clientCoorMouse;
			return false;
		}
		

	}
	else {
		return false;
	}
}

/// <summary>
/// nFlags : Ctrl 등의 기타 키 입력 여부
/// zDelta : 휠 변화
/// pt : 윌 사용 시, 마우스 포인터 위치
/// </summary>
/// <param name="nFlags"></param>
/// <param name="zDelta"></param>
/// <param name="pt"></param>
void MyGUI::OnMouseWheel(UINT nFlags, short zDelta, CPoint windowCoorMouse)
{
	bool bEnableZoom = false;

	if (m_bInit)
	{
		CPoint viewCoorMouse;
		CPoint pointClientRef;
		CRect rect;
		if (!m_bSlave) {	// Master
			m_picbox->GetWindowRect(rect);
			viewCoorMouse.x = windowCoorMouse.x - rect.left;		//View 기준 마우스 포인터
			viewCoorMouse.y = windowCoorMouse.y - rect.top;
			//TRACE(_T("windowCoorMouse(%d, %d), ViewRect(%d, %d), viewCoorMouse(%d, %d)\n"), windowCoorMouse.x, windowCoorMouse.y, rect.left, rect.top, viewCoorMouse.x, viewCoorMouse.y);
		}
		else {			// Slave
			m_picbox->GetParent()->ScreenToClient(&windowCoorMouse);
			viewCoorMouse.x = windowCoorMouse.x - m_RefGui->m_clientCoordinateViewRect.left;
			viewCoorMouse.y = windowCoorMouse.y - m_RefGui->m_clientCoordinateViewRect.top;
			//TRACE(_T("SLAVE:: windowCoorMouse(%d, %d), ViewRect(%d, %d), viewCoorMouse(%d, %d)\n"), windowCoorMouse.x, windowCoorMouse.y, m_RefGui->m_clientCoordinateViewRect.left, m_RefGui->m_clientCoordinateViewRect.top, viewCoorMouse.x, viewCoorMouse.y);
		}


		if (isOnImg(m_clientCoordinateMouse))
		{
			if (!m_bSlave) {	// Master
				if (m_imageCoordinateMouse.x < m_sizeImgX && m_imageCoordinateMouse.y < m_sizeImgY)
				{

					bEnableZoom = true;
				}
				//TRACE(_T("m_imageCoordinateMouse(%d,%d)\n"), m_imageCoordinateMouse.x, m_imageCoordinateMouse.y);
			}
			else {		// Slave
				if (m_RefGui->m_imageCoordinateMouse.x < m_sizeImgX && m_RefGui->m_imageCoordinateMouse.y < m_sizeImgY)
				{
					bEnableZoom = true;
				}
				//TRACE(_T("Slave::m_imageCoordinateMouse(%d,%d)\n"), m_imageCoordinateMouse.x, m_imageCoordinateMouse.y);
			}
			if (bEnableZoom) {
				int imgCoorMouseX, imgCoorMouseY;
				if (zDelta > 0)		// 스크롤 업(확대)
				{
					ConvertViewCoorToImgCoor(viewCoorMouse, &imgCoorMouseX, &imgCoorMouseY);
					m_fMagX = m_fMagX * m_fMagstep;
					m_fMagY = m_fMagX;
					if (m_fMagX > m_fMagMax) {
						m_fMagX = m_fMagMax;
						m_fMagY = m_fMagMax;
					}
					ZoomInOut(imgCoorMouseX, imgCoorMouseY, viewCoorMouse);
				}
				else {	// 스크롤 다운(축소)
					if ((m_sizeViewX > (m_sizeImgX * (m_fMagX / m_fMagstep))) && (m_sizeViewY > (m_sizeImgY * (m_fMagY / m_fMagstep))))
					{
						ConvertViewCoorToImgCoor(viewCoorMouse, &imgCoorMouseX, &imgCoorMouseY);
						FitWindow();
						ZoomInOut(imgCoorMouseX, imgCoorMouseY, viewCoorMouse);
					}

					else {
						ConvertViewCoorToImgCoor(viewCoorMouse, &imgCoorMouseX, &imgCoorMouseY);
						m_fMagX = m_fMagX / m_fMagstep;
						m_fMagY = m_fMagX;

						if (m_sizeViewX > m_sizeImgX * m_fMagX || m_sizeViewY > m_sizeImgY * m_fMagY) {
							//m_Buffer->Clear(1);
							m_View->Show(1);
						}

						//if(magx < magFitMin)
						//m_pDC->FillSolidRect(&m_viewCoorViewRect, RGB(192, 192, 192));		// 이미지 축소 시, 이미지 외부는 black으로 표시

						ZoomInOut(imgCoorMouseX, imgCoorMouseY, viewCoorMouse);
					}
				}
			}
		}
	}
}

// 
void MyGUI::OnLButtonDown(UINT nFlags, CPoint clientCoorMouse)
{
	if (m_bInit)
	{
		m_bDown = true;
		m_imgCoorMouseDragX = m_imgCoorViewCenterX;
		m_imgCoorMouseDragY = m_imgCoorViewCenterY;
		m_viewCoorBtnDown.x = clientCoorMouse.x - m_clientCoordinateViewRect.left;	// picbox client point
		m_viewCoorBtnDown.y = clientCoorMouse.y - m_clientCoordinateViewRect.top;		// picbox client point
	}
}

void MyGUI::OnLButtonUp(UINT nFlags, CPoint clientCoorMouse)
{
	int X;
	int Y;
	if (m_bInit)
	{
		if (m_bDown)
		{
			m_bDown = false;
			//UpdataViewMain(vpos_MainX + (oldPoint.x - point.x), vpos_MainY + (oldPoint.y - point.y));

			X = m_imgCoorViewCenterX + int(float(m_viewCoorBtnDown.x - (clientCoorMouse.x - m_clientCoordinateViewRect.left)) / m_fMagX + 0.5);
			Y = m_imgCoorViewCenterY + int(float(m_viewCoorBtnDown.y - (clientCoorMouse.y - m_clientCoordinateViewRect.top)) / m_fMagY + 0.5);

			if (X < int(float(m_sizeViewX) / 2.0 / m_fMagX + 0.5))		// 이미지 축소
				X = int(float(m_sizeViewX) / 2.0 / m_fMagX + 0.5);
			else if (m_sizeImgX < X + int(float(m_sizeViewX) / 2.0 / m_fMagX + 0.5))		// 이미지 확대
				X = m_sizeImgX - int(float(m_sizeViewX) / 2.0 / m_fMagX + 0.5);

			if (Y < int(float(m_sizeViewY) / 2.0 / m_fMagY + 0.5))		// 이미지 축소
				Y = int(float(m_sizeViewY) / 2.0 / m_fMagY + 0.5);
			else if (m_sizeImgY < Y + int(float(m_sizeViewY) / 2.0 / m_fMagY + 0.5))		// 이미지 확대
				Y = m_sizeImgY - int(float(m_sizeViewY) / 2.0 / m_fMagY + 0.5);

			m_imgCoorViewCenterX = X;
			m_imgCoorViewCenterY = Y;
		}
	}
}

bool MyGUI::isOnImg(CPoint clientCoorMouse)
{
	// Master View
	if (!m_bSlave) {
		//TRACE("ClientCoorMouse(%d,%d)\tClientCoorViewRect(%d,%d)(%d,%d)\r", clientCoorMouse.x, clientCoorMouse.y, m_clientCoordinateViewRect.left, m_clientCoordinateViewRect.top, m_clientCoordinateViewRect.right, m_clientCoordinateViewRect.bottom);
		if (clientCoorMouse.x > m_clientCoordinateViewRect.left && clientCoorMouse.x < m_clientCoordinateViewRect.right && clientCoorMouse.y > m_clientCoordinateViewRect.top && clientCoorMouse.y < m_clientCoordinateViewRect.bottom)
		{
			return true;
		}
		else
		{
			OnLButtonUp(0, clientCoorMouse);
			return false;
		}
	}
	else {	// Slave View
		//TRACE("ClientCoorMouse(%d,%d)\tClientCoorViewRect(%d,%d)(%d,%d)\r", clientCoorMouse.x, clientCoorMouse.y, m_RefGui->m_clientCoordinateViewRect.left, m_RefGui->m_clientCoordinateViewRect.top, m_RefGui->m_clientCoordinateViewRect.right, m_RefGui->m_clientCoordinateViewRect.bottom);
		if (clientCoorMouse.x > m_RefGui->m_clientCoordinateViewRect.left && clientCoorMouse.x < m_RefGui->m_clientCoordinateViewRect.right && clientCoorMouse.y > m_RefGui->m_clientCoordinateViewRect.top && clientCoorMouse.y < m_RefGui->m_clientCoordinateViewRect.bottom)
		{
			return true;
		}
		else
		{
			OnLButtonUp(0, clientCoorMouse);
			return false;
		}
	}
}

void MyGUI::OnMouseLeave(void)
{
	if (m_bInit)
	{
		OnLButtonUp(0, m_clientCoordinateMouse);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MyGUI::ConvertClientCoorToImgCoor(CPoint clientCoorMouse, CPoint * imgCoorMouse)
{
	if (m_bInit)
	{
		CPoint viewCoorMouse;

		viewCoorMouse.x = clientCoorMouse.x - m_clientCoordinateViewRect.left;
		viewCoorMouse.y = clientCoorMouse.y - m_clientCoordinateViewRect.top;
		imgCoorMouse->x = m_imgCoorViewX + int(float(viewCoorMouse.x) / m_fMagX);
		imgCoorMouse->y = m_imgCoorViewY + int(float(viewCoorMouse.y) / m_fMagY);
	}
}

void MyGUI::ConvertViewCoorToImgCoor(CPoint viewCoorMouse, int* imgCoorMouseX, int* imgCoorMouseY)
{
		*imgCoorMouseX = m_imgCoorViewX + int(float(viewCoorMouse.x) / m_fMagX + 0.5);
		*imgCoorMouseY = m_imgCoorViewY + int(float(viewCoorMouse.y) / m_fMagY + 0.5);
}

void MyGUI::ZoomInOut(int imgCoorMouseX, int imgCoorMouseY, CPoint viewCoorMouse)
{
	//find center

	int X = imgCoorMouseX - int(float((viewCoorMouse.x) - (m_sizeViewX + 1) / 2) / m_fMagX + 0.5); //이만큼 벗어남
	int Y = imgCoorMouseY - int(float((viewCoorMouse.y) - (m_sizeViewY + 1) / 2) / m_fMagY + 0.5);
	m_imgCoorViewCenterX = X;
	m_imgCoorViewCenterY = Y;

	if (m_bSlave)
	{
		//Show(m_data8, m_RefGui->m_imgCoorViewCenterX, m_RefGui->m_imgCoorViewCenterY);
		Show(m_RefGui->m_imgCoorViewCenterX, m_RefGui->m_imgCoorViewCenterY);
	}
	else {
		//Show(m_data8, m_imgCoorViewCenterX, m_imgCoorViewCenterY);
		Show(m_imgCoorViewCenterX, m_imgCoorViewCenterY);
	}
	

}

void MyGUI::FitWindow(void)
{
	m_imgCoorViewCenterX = m_sizeImgX / 2;
	m_imgCoorViewCenterY = m_sizeImgY / 2;

	m_fMagX = m_fMagFitMin;
	m_fMagY = m_fMagFitMin;

	//m_pDC->FillSolidRect(&m_viewCoorViewRect, RGB(192, 192, 192));		// 이미지 축소 시, 이미지 외부는 black으로 표시
	//Show(m_data8, m_imgCoorViewCenterX, m_imgCoorViewCenterY);

	Show(m_imgCoorViewCenterX, m_imgCoorViewCenterY);
}
