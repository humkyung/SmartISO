// PictureWnd.cpp : implementation file
//

#include "StdAfx.h"
#include <afxpriv.h>
#include "resource.h"
#include "PictureWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPictureWnd

CPictureWnd::CPictureWnd()
{
	m_pPicture = NULL;
}

CPictureWnd::~CPictureWnd()
{
	if (m_pPicture != NULL)
	{
		m_pPicture->Release();
		m_pPicture = NULL;
		
		TRACE(_T("CImgViewerDoc::~CImgViewerDoc - m_pPicture->Release()\n"));
	}
}


BEGIN_MESSAGE_MAP(CPictureWnd, CWnd)
	//{{AFX_MSG_MAP(CPictureWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPictureWnd message handlers

int CPictureWnd::Load(LPCTSTR lpszPathName)
{
	ASSERT(::AfxIsValidString(lpszPathName));
	
	IPicture *pPic = NULL;
	
	USES_CONVERSION;
	HRESULT hr = ::OleLoadPicturePath(const_cast<LPOLESTR>(T2COLE(lpszPathName)),
		NULL,
		0,
		0,
		IID_IPicture,
		reinterpret_cast<LPVOID *>(&pPic));
	
	TRACE(_T("CPictureWnd::Load - OleLoadPicturePath(\"%s\"): ")
		_T("hr = 0x%08X, m_pPicture = 0x%08X\n"), lpszPathName, hr, pPic);
	
	if (SUCCEEDED(hr) && pPic != NULL)
	{	
		this->DeleteContents();

		ASSERT(m_pPicture == NULL);
		
		m_pPicture = pPic;
		
		// get width and height of picture
		VERIFY(SUCCEEDED(m_pPicture->get_Width(&m_sizeInHiMetric.cx)));
		VERIFY(SUCCEEDED(m_pPicture->get_Height(&m_sizeInHiMetric.cy)));
		
		const int HIMETRIC_PER_INCH = 2540;
		
		const HDC hDCScreen = ::GetDC(NULL);
		ASSERT(hDCScreen != NULL);
		// Pixels per logical inch along width
		const int nPixelsPerInchX = ::GetDeviceCaps(hDCScreen, LOGPIXELSX);
		// Pixels per logical inch along height
		const int nPixelsPerInchY = ::GetDeviceCaps(hDCScreen, LOGPIXELSY);
		VERIFY(::ReleaseDC(NULL, hDCScreen) != 0);
		
		// convert himetric to pixels
		m_sizeInPix.cx = (nPixelsPerInchX * m_sizeInHiMetric.cx +
			HIMETRIC_PER_INCH / 2) / HIMETRIC_PER_INCH;
		m_sizeInPix.cy = (nPixelsPerInchY * m_sizeInHiMetric.cy +
			HIMETRIC_PER_INCH / 2) / HIMETRIC_PER_INCH;
		
		return TRUE;
	}
	else
	{
		// OleLoadPicturePath failed.
		CString strErrMsg;
		strErrMsg.Format("%s 파일을 로드할수 없습니다" , lpszPathName);
		::AfxMessageBox(strErrMsg, MB_OK | MB_ICONEXCLAMATION);
		
		return FALSE;
	}

	return FALSE;
}

void CPictureWnd::DeleteContents()
{
	if (m_pPicture != NULL)
	{
		m_pPicture->Release();
		m_pPicture = NULL;
		
		TRACE(_T("CImgViewerDoc::DeleteContents - m_pPicture->Release()\n"));
	}
}

void CPictureWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if (this->m_pPicture != NULL)
	{
		// get palette
		HPALETTE hPal = NULL;
		VERIFY(SUCCEEDED(this->m_pPicture->get_hPal(reinterpret_cast<OLE_HANDLE *>(&hPal))));
		
		CPalette *pPalOld = NULL;
		if (hPal != NULL)
		{
			TRACE(_T("CImgViewerView::OnDraw - RealizePalette\n"));
			pPalOld = dc.SelectPalette(CPalette::FromHandle(hPal), FALSE);
			dc.RealizePalette();
		}
		
		// transparent?
		DWORD dwAttr = 0;
		if (FAILED(this->m_pPicture->get_Attributes(&dwAttr)) ||
			(dwAttr & PICTURE_TRANSPARENT))
		{
			TRACE(_T("CImgViewerView::OnDraw - Transparent\n"));
			
			// use an off-screen DC to prevent flickering
			CDC MemDC;
			VERIFY(MemDC.CreateCompatibleDC(&dc));
			CBitmap Bmp;
			VERIFY(Bmp.CreateCompatibleBitmap(&dc,
				this->m_sizeInPix.cx,
				this->m_sizeInPix.cy));
			
			CBitmap *pBmpOld = MemDC.SelectObject(&Bmp);
			CPalette *pPalMemOld = NULL;
			if (hPal != NULL)
			{
				pPalMemOld = MemDC.SelectPalette(CPalette::FromHandle(hPal), FALSE);
				MemDC.RealizePalette();
			}
			
			const RECT rc = { 0, 0, this->m_sizeInPix.cx, this->m_sizeInPix.cy };
			
			MemDC.FillSolidRect(&rc, ::GetSysColor(COLOR_WINDOW));
			
			// display picture using IPicture::Render
			VERIFY(SUCCEEDED(this->m_pPicture->Render(MemDC.GetSafeHdc(),
				0,
				0,
				this->m_sizeInPix.cx,
				this->m_sizeInPix.cy,
				0,
				this->m_sizeInHiMetric.cy,
				this->m_sizeInHiMetric.cx,
				-this->m_sizeInHiMetric.cy,
				&rc)));
			
			VERIFY(dc.BitBlt(0, 0, this->m_sizeInPix.cx, this->m_sizeInPix.cy,
				&MemDC, 0, 0, SRCCOPY));
			
			MemDC.SelectObject(pBmpOld);
			
			if (pPalMemOld != NULL)
			{
				ASSERT(hPal != NULL);
				MemDC.SelectPalette(pPalMemOld, FALSE);
			}
		}
		else
		{
			RECT rc;
			this->GetClientRect(&rc);
			
			int left = (rc.right - rc.left - this->m_sizeInPix.cx ) * 0.5;
			if( left < 0) left = 0;
			int top = (rc.bottom - rc.top - this->m_sizeInPix.cy ) * 0.5;
			if( top < 0) top = 0;
			// display picture using IPicture::Render
			VERIFY(SUCCEEDED(this->m_pPicture->Render(dc.GetSafeHdc(),
				left ,
				top,
				this->m_sizeInPix.cx,
				this->m_sizeInPix.cy,
				0,
				this->m_sizeInHiMetric.cy,
				this->m_sizeInHiMetric.cx,
				-this->m_sizeInHiMetric.cy,
				&rc)));
		}
		
		if (pPalOld != NULL)
		{
			ASSERT(hPal != NULL);
			dc.SelectPalette(pPalOld, FALSE);
		}
	}
	
	// Do not call CWnd::OnPaint() for painting messages
}

BOOL CPictureWnd::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	CBrush br(::GetSysColor(COLOR_WINDOW));
	//this->FillOutsideRect(pDC, &br);
	
	return TRUE;
}

SIZE CPictureWnd::GetImgSize()
{
	return m_sizeInPix;
}
