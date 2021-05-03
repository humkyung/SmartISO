// EditListBox.cpp : implementation file
//

#include "stdafx.h"
#include "Admin.h"
#include "EditListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditListBox

CEditListBox::CEditListBox() : 
	m_atomHotkey(NULL), m_szDummy(NULL), m_bDummy(FALSE), m_bTipTimeOut(FALSE),
	m_hThemeDLL(NULL), m_hTheme(NULL), m_pfnDrawThemeBackground(NULL), m_bVisualStyleEnabled(FALSE)
{
	::memset(&m_captionInfo, 0, sizeof(CaptionInfo));
	m_captionInfo.iActiveButton = -1;
	m_captionInfo.iMouseOver = -1;

	::memset(&m_editInfo, 0, sizeof(EditInfo));
	m_editInfo.bShowButton = FALSE;

	const int cyCaption = ::GetSystemMetrics(SM_CYCAPTION);
	m_captionInfo.nHeight = max(cyCaption, BITMAP_SIZE);

	m_atomHotkey = ::GlobalAddAtom(_T("codeproject::CEditListBoxImplT"));

	SetBitmapButtonTipText(_T("New (Insert)\tDelete (Delete)\tMove Item Up (ALT+Up Arrow)\tMove Item Down (ALT+Down Arrow)"));

	SetLastDummyText(_T("Double click here to add a new item"));
}

CEditListBox::~CEditListBox()
{
	delete [] m_szDummy;
	m_szDummy = NULL;
	
#ifdef _DEBUG
	
	::SetLastError(ERROR_SUCCESS);
	
#endif	// #ifdef _DEBUG
	
	::GlobalDeleteAtom(m_atomHotkey);
	
#ifdef _DEBUG
	
	ATLASSERT(ERROR_SUCCESS == ::GetLastError());
	
#endif	// #ifdef _DEBUG
	
	Term();
	
	// deallocate the storage for bitmap button tips
	SetBitmapButtonTipText(NULL);
}


BEGIN_MESSAGE_MAP(CEditListBox, CListBox)
	//{{AFX_MSG_MAP(CEditListBox)
	ON_WM_NCCALCSIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditListBox message handlers
HWND CEditListBox::Create(HWND hWndParent, RECT& rcPos, LPCTSTR szWindowName = NULL,
			  DWORD dwStyle = 0, DWORD dwExStyle = 0,
			  UINT nID = 0, LPVOID lpCreateParam = NULL)
{
	CListBox wndTempListBox;
	HWND hWnd = wndTempListBox.Create(hWndParent, rcPos, szWindowName, dwStyle, dwExStyle, nID, lpCreateParam);
	wndTempListBox.Detach();
	
	if(hWnd)
		SubclassWindow(hWnd);
	
	return hWnd;
}


void CEditListBox::Init()
{
	if(!IsWindow(m_wndTip.m_hWnd))
		CreateTip();
	
	if(m_wndImages.IsNull())
	{
		m_wndImages.Create(__BMP_BTNWID, __BMP_HEIGHT, ILC_COLOR4 | ILC_MASK, 4, 0);
		CBitmap bmpButton = LoadBmpButtonImage();
		m_wndImages.Add(bmpButton, RGB(255, 0, 255));
	}
	
	if(!IsWindow(m_wndEdit.m_hWnd))
	{
		m_wndEdit.Create(m_hWnd, rcDefault, NULL, WS_BORDER | WS_CHILD | WS_CLIPCHILDREN | ES_AUTOHSCROLL, NULL, GetDlgCtrlID());
		ATLASSERT(m_wndEdit.IsWindow());
		m_wndEdit.SetFont(GetFont());
		m_wndEdit.LimitText(MAX_PATH);
		m_wndEdit.HideButton(!m_editInfo.bShowButton);
	}
	
	// set the minimum item height to fit In-Place Edit control
	RECT rcFmt;
	m_wndEdit.SendMessage(EM_GETRECT, 0, (LPARAM)&rcFmt);
	int nEditHeight = rcFmt.bottom - rcFmt.top + 2;
	int nItemHeight = max(nEditHeight, GetItemHeight(0));
	SetItemHeight(0, nItemHeight);
	
	AddButton(_ELBM_NEW, _BTN_NEW);
	AddButton(_ELBM_DELETE, _BTN_DELETE);
	EnableButton(_ELBM_DELETE, FALSE);
	if(!m_captionInfo.bHideMoveButtons)
	{
		AddButton(_ELBM_ITEMUP, _BTN_ITEMUP);
		EnableButton(_ELBM_ITEMUP, FALSE);
		AddButton(_ELBM_ITEMDOWN, _BTN_ITEMDOWN);
		EnableButton(_ELBM_ITEMDOWN, FALSE);
	}
	
	// insert the last dummy item
	InsertLastDummy();
	
	//	Doing this forces our ListBox window to pay attention to our change in it's client size
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	
	UpdateButtons();
	
	InitBorderCode();
}

void CEditListBox::Term()
{
	TermBorderCode();
	
	m_wndImages.Destroy();
	
	if(m_wndEdit.IsWindow())
		m_wndEdit.DestroyWindow();
	
	if(m_wndTip.IsWindow())
		m_wndTip.DestroyWindow();
}

LRESULT CEditListBox::SendNotifyMsg(UINT code, int iSource, int iDestination)
{
	NMEDITLIST nm = { 0 };
	nm.hdr.hwndFrom = m_hWnd;
	nm.hdr.idFrom = GetDlgCtrlID();
	nm.hdr.code = code;
	nm.iSrc = iSource;
	nm.iDst = iDestination;
	return ::SendMessage(GetParent(), WM_NOTIFY, nm.hdr.idFrom, (LPARAM)&nm);
}

HBITMAP CEditListBox::LoadBmpButtonImage()
{
	LPTSTR szFilePathTemp = ::_ttmpnam(NULL);
	// The pointer returned by tmpnam points to an internal static buffer.
	// free does not need to be called to deallocate this pointer.
	
	FILE *pfBmpButton = ::_tfopen(szFilePathTemp, _T("wb"));
	ATLASSERT(pfBmpButton);
	size_t written = ::fwrite(lbbuddy_data, sizeof(unsigned char), __bmp_size, pfBmpButton);
	written;
	ATLASSERT(sizeof(lbbuddy_data) == written);
	int nClosed = ::fclose(pfBmpButton);
	nClosed;
	ATLASSERT(0 == nClosed);
	
	HBITMAP hbmpButton = (HBITMAP)::LoadImage(
		NULL,
		szFilePathTemp,
		IMAGE_BITMAP,
		__BMP_WIDTH,
		__BMP_HEIGHT,
		LR_CREATEDIBSECTION | LR_LOADFROMFILE
		);
	
	int nDeleted = ::_tunlink(szFilePathTemp);
	nDeleted;
	ATLASSERT(0 == nDeleted);
	
	return hbmpButton;
}

void CEditListBox::AddButton(UINT uCmd, int iImage)
{
	m_captionInfo.buttonInfo[m_captionInfo.cnButton].uCmd = uCmd;
	m_captionInfo.buttonInfo[m_captionInfo.cnButton].iImage = iImage;
	
	m_captionInfo.cnButton++;
}

void CEditListBox::UpdateButtons()
{
	ATLASSERT(IsWindow());
	int iCurSel = GetCurSel();
	int iDummy = GetCount();
	int iLastItem = iDummy - 1;
	
	m_captionInfo.buttonInfo[0].bDisabled  = FALSE;
	m_captionInfo.buttonInfo[1].bDisabled  = LB_ERR == iCurSel || iDummy == iCurSel;
	m_captionInfo.buttonInfo[2].bDisabled  = LB_ERR == iCurSel || iDummy == iCurSel || 0 == iCurSel;
	m_captionInfo.buttonInfo[3].bDisabled  = LB_ERR == iCurSel || iDummy == iCurSel || iLastItem == iCurSel;
	
	RECT rcWindow;
	GetWindowRect(&rcWindow);
	
	CWindowDC dcWindow(m_hWnd);
	int nSaveDC = dcWindow.SaveDC();
	
	RECT rcClip;
	::CopyRect(&rcClip, &rcWindow);
	::OffsetRect(&rcClip, -rcWindow.left, -rcWindow.top);
	rcClip.bottom = rcClip.top + m_captionInfo.nHeight + 2 * ::GetSystemMetrics(SM_CYEDGE);
	dcWindow.IntersectClipRect(&rcClip);
	
	for(int i = 0; i < m_captionInfo.cnButton; i++)
	{
		m_captionInfo.buttonInfo[i].bValidated = FALSE;
		DrawBmpButton(i, dcWindow);
	}
	
	dcWindow.RestoreDC(nSaveDC);
}

BOOL CEditListBox::IsVisualStyleEnabled() const
{
	HINSTANCE hinstDll = ::LoadLibrary(_T("Comctl32.dll"));
	if(hinstDll)
	{
		DLLGETVERSIONPROC  pDllGetVersion = (DLLGETVERSIONPROC)::GetProcAddress(hinstDll, "DllGetVersion");
		
		if(pDllGetVersion)
		{
			DLLVERSIONINFO dvi;
			::memset(&dvi, 0, sizeof(dvi));
			dvi.cbSize = sizeof(dvi);
			
			HRESULT hr = (*pDllGetVersion)(&dvi);
			
			if(SUCCEEDED(hr))
				return (dvi.dwMajorVersion >= 6);
		}
		
		::FreeLibrary(hinstDll);
	}
	
	return FALSE;
}

void CEditListBox::InitBorderCode()
{
	m_hTheme = NULL;
	m_hThemeDLL = NULL;
	
	m_bVisualStyleEnabled = IsVisualStyleEnabled();
	
	if(m_bVisualStyleEnabled)
	{
		m_hThemeDLL = ::LoadLibrary(_T("uxtheme.dll"));
		if(NULL != m_hThemeDLL)
		{
			m_pfnDrawThemeBackground = (PFN_DrawThemeBackground)::GetProcAddress(m_hThemeDLL, "DrawThemeBackground");
			ATLASSERT(NULL != m_pfnDrawThemeBackground);
			if(NULL != m_pfnDrawThemeBackground)
				OpenThemeData(L"LISTVIEW");
			else
			{
				::FreeLibrary(m_hThemeDLL);
				m_hThemeDLL = NULL;
			}
		}
	}
}

void CEditListBox::TermBorderCode()
{
	if(NULL != m_hThemeDLL)
	{
		CloseThemeData();
		::FreeLibrary(m_hThemeDLL);
		m_hThemeDLL = NULL;
	}
}

UINT CEditListBox::GetThemeChangedMsg()
{
	
#ifndef WM_THEMECHANGED
	
	static const UINT WM_THEMECHANGED = 0x031A;
	
#endif //!WM_THEMECHANGED
	
	return WM_THEMECHANGED;
}

void CEditListBox::OpenThemeData(LPCWSTR pszClassList)
{
	ATLASSERT(NULL != m_hThemeDLL);
	
	PFNOpenThemeData pfnOpenThemeData = (PFNOpenThemeData)::GetProcAddress(m_hThemeDLL, "OpenThemeData");
	ATLASSERT(NULL != pfnOpenThemeData);
	if(NULL != pfnOpenThemeData)
		m_hTheme = pfnOpenThemeData(m_hWnd, pszClassList);
}

void CEditListBox::CloseThemeData()
{
	ATLASSERT(NULL != m_hThemeDLL);
	
	if(NULL == m_hTheme)
		return;   // nothing to do
	
	PFNCloseThemeData pfnCloseThemeData = (PFNCloseThemeData)::GetProcAddress(m_hThemeDLL, "CloseThemeData");
	ATLASSERT(NULL != pfnCloseThemeData);
	if(NULL != pfnCloseThemeData)
	{
		pfnCloseThemeData(m_hTheme);
		m_hTheme = NULL;
	}
}

void CEditListBox::CalcBorderOffsetFromStyle(LPSIZE lpszOffset)
{
	if(NULL == lpszOffset)
		return;
	
	lpszOffset->cx = 0;
	lpszOffset->cy = 0;
	
	const DWORD dwExStyle = GetExStyle(); 
	const DWORD dwStyle = GetStyle(); 
	
	if(WS_BORDER & dwStyle && !(WS_EX_STATICEDGE & dwExStyle) && !(WS_EX_DLGMODALFRAME & dwExStyle)) 
	{ 
		lpszOffset->cx = ::GetSystemMetrics(SM_CXBORDER); 
		lpszOffset->cy = ::GetSystemMetrics(SM_CYBORDER);
	} 
	
	if(WS_EX_CLIENTEDGE & dwExStyle) 
	{ 
		lpszOffset->cx += ::GetSystemMetrics(SM_CXEDGE); 
		lpszOffset->cy += ::GetSystemMetrics(SM_CYEDGE); 
	} 
	
	if(WS_EX_STATICEDGE & dwExStyle && !(WS_EX_DLGMODALFRAME & dwExStyle)) 
	{ 
		if(WS_EX_CLIENTEDGE & dwExStyle) 
		{
			lpszOffset->cx += 1;
			lpszOffset->cy += 1;
		}
		else
		{
			lpszOffset->cx += ::GetSystemMetrics(SM_CXEDGE);
			lpszOffset->cy += ::GetSystemMetrics(SM_CYEDGE);
		}
	}
	
	if(WS_EX_DLGMODALFRAME & dwExStyle) 
	{ 
		lpszOffset->cx += ::GetSystemMetrics(SM_CXDLGFRAME); 
		lpszOffset->cy += ::GetSystemMetrics(SM_CYDLGFRAME); 
	} 
}

void CEditListBox::DrawBorder(HDC hdcWindow, LPRECT lpRect, UINT grfFlags)
{
	if(NULL == hdcWindow || NULL == lpRect)
		return;
	
	const DWORD dwExStyle = GetExStyle(); 
	const DWORD dwStyle = GetStyle(); 
	
	if(m_bVisualStyleEnabled && m_hTheme)
	{
		if(WS_BORDER & dwStyle && !(WS_EX_CLIENTEDGE & dwExStyle) && !(WS_EX_STATICEDGE & dwExStyle) && !(WS_EX_DLGMODALFRAME & dwExStyle)) 
		{
			::MoveToEx(hdcWindow, lpRect->left, lpRect->top, NULL);
			
			if(BF_LEFT & grfFlags)
				::LineTo(hdcWindow, lpRect->left, lpRect->bottom - 1);
			else
				::MoveToEx(hdcWindow, lpRect->left, lpRect->bottom - 1, NULL);
			
			if(BF_BOTTOM & grfFlags)
				::LineTo(hdcWindow, lpRect->right - 1, lpRect->bottom - 1);
			else
				::MoveToEx(hdcWindow, lpRect->right - 1, lpRect->bottom - 1, NULL);
			
			if(BF_RIGHT & grfFlags)
				::LineTo(hdcWindow, lpRect->right - 1, lpRect->top);
			else
				::MoveToEx(hdcWindow, lpRect->right - 1, lpRect->top, NULL);
			
			if(BF_TOP & grfFlags)
				::LineTo(hdcWindow, lpRect->left, lpRect->top);
			else
				::MoveToEx(hdcWindow, lpRect->left, lpRect->top, NULL);
			
			::InflateRect(lpRect, -::GetSystemMetrics(SM_CXBORDER), -::GetSystemMetrics(SM_CXBORDER));
		} 
		
		if(WS_EX_CLIENTEDGE & dwExStyle) 
		{ 
			const int cxEdge = ::GetSystemMetrics(SM_CXEDGE);
			const int cyEdge = ::GetSystemMetrics(SM_CYEDGE);
			
			RECT rcClip;
			if(BF_LEFT & grfFlags)
			{
				rcClip = *lpRect;
				rcClip.right = rcClip.left + cxEdge;
				m_pfnDrawThemeBackground(m_hTheme, hdcWindow, LVP_LISTITEM, LIS_NORMAL, lpRect, &rcClip);
			}
			
			if(BF_TOP & grfFlags)
			{
				rcClip = *lpRect;
				rcClip.bottom = rcClip.top + cyEdge;
				m_pfnDrawThemeBackground(m_hTheme, hdcWindow, LVP_LISTITEM, LIS_NORMAL, lpRect, &rcClip);
			}
			
			if(BF_RIGHT & grfFlags)
			{
				rcClip = *lpRect;
				rcClip.left = rcClip.right - cxEdge;
				m_pfnDrawThemeBackground(m_hTheme, hdcWindow, LVP_LISTITEM, LIS_NORMAL, lpRect, &rcClip);
			}
			
			if(BF_BOTTOM & grfFlags)
			{
				rcClip = *lpRect;
				rcClip.top = rcClip.bottom - cyEdge;
				m_pfnDrawThemeBackground(m_hTheme, hdcWindow, LVP_LISTITEM, LIS_NORMAL, lpRect, &rcClip);
			}
			
			::InflateRect(lpRect, -cxEdge, -cyEdge);
			
			if((WS_BORDER & dwStyle) && !(WS_EX_STATICEDGE & dwExStyle) && !(WS_EX_DLGMODALFRAME & dwExStyle))
				::DrawEdge(hdcWindow, lpRect, BDR_SUNKENINNER, BF_RECT & grfFlags);
		} 
		
		if(WS_EX_STATICEDGE & dwExStyle && !(WS_EX_DLGMODALFRAME & dwExStyle)) 
		{ 
			if(WS_EX_CLIENTEDGE & dwExStyle)
			{
				::DrawEdge(hdcWindow, lpRect, BDR_SUNKENINNER, BF_RECT & grfFlags);
				::InflateRect(lpRect, -1, -1);
			}
			else
			{
				::DrawEdge(hdcWindow, lpRect, BDR_SUNKENOUTER, BF_RECT & grfFlags);
				::InflateRect(lpRect, -::GetSystemMetrics(SM_CXEDGE), -::GetSystemMetrics(SM_CYEDGE));
			}
			
		}
		
		if(WS_EX_DLGMODALFRAME & dwExStyle) 
		{
			if(WS_EX_CLIENTEDGE & dwExStyle)
			{
				::DrawEdge(hdcWindow, lpRect, BDR_SUNKENINNER, BF_BOTTOMRIGHT & grfFlags);
				::InflateRect(lpRect, -1, -1);
				::DrawEdge(hdcWindow, lpRect, EDGE_SUNKEN, BF_RECT & grfFlags);
				//	::InflateRect(lpRect, -::GetSystemMetrics(SM_CXDLGFRAME) + 1, -::GetSystemMetrics(SM_CXDLGFRAME) + 1);
			}
			else
			{
				::DrawEdge(hdcWindow, lpRect, EDGE_RAISED, BF_RECT & grfFlags);
				//	::InflateRect(lpRect, -cxDlgFrame, -cyDlgFrame);
			}
		} 
	}
	else
	{
		if(WS_BORDER & dwStyle && !(WS_EX_STATICEDGE & dwExStyle) && !(WS_EX_DLGMODALFRAME & dwExStyle)) 
		{
			if(!(WS_EX_CLIENTEDGE & dwExStyle))
			{
				::MoveToEx(hdcWindow, lpRect->left, lpRect->top, NULL);
				
				if(BF_LEFT & grfFlags)
					::LineTo(hdcWindow, lpRect->left, lpRect->bottom - 1);
				else
					::MoveToEx(hdcWindow, lpRect->left, lpRect->bottom - 1, NULL);
				
				if(BF_BOTTOM & grfFlags)
					::LineTo(hdcWindow, lpRect->right - 1, lpRect->bottom - 1);
				else
					::MoveToEx(hdcWindow, lpRect->right - 1, lpRect->bottom - 1, NULL);
				
				if(BF_RIGHT & grfFlags)
					::LineTo(hdcWindow, lpRect->right - 1, lpRect->top);
				else
					::MoveToEx(hdcWindow, lpRect->right - 1, lpRect->top, NULL);
				
				if(BF_TOP & grfFlags)
					::LineTo(hdcWindow, lpRect->left, lpRect->top);
				else
					::MoveToEx(hdcWindow, lpRect->left, lpRect->top, NULL);
			}
			
			::InflateRect(lpRect, -::GetSystemMetrics(SM_CXBORDER), -::GetSystemMetrics(SM_CXBORDER));
		} 
		
		if(WS_EX_STATICEDGE & dwExStyle && !(WS_EX_DLGMODALFRAME & dwExStyle)) 
		{ 
			if(WS_EX_CLIENTEDGE & dwExStyle)
			{
				::DrawEdge(hdcWindow, lpRect, BDR_SUNKENOUTER, BF_RECT & grfFlags);
				::InflateRect(lpRect, -1,  -1);
			}
			else
			{
				::DrawEdge(hdcWindow, lpRect, BDR_SUNKENOUTER, BF_RECT & grfFlags);
				::InflateRect(lpRect, -::GetSystemMetrics(SM_CXEDGE), -::GetSystemMetrics(SM_CYEDGE));
			}
		}
		
		if(WS_EX_DLGMODALFRAME & dwExStyle) 
		{ 
			::DrawEdge(hdcWindow, lpRect, EDGE_RAISED, BF_RECT & grfFlags);
			::InflateRect(lpRect, -::GetSystemMetrics(SM_CXDLGFRAME), -::GetSystemMetrics(SM_CXDLGFRAME));
		} 
		
		if(WS_EX_CLIENTEDGE & dwExStyle) 
		{ 
			::DrawEdge(hdcWindow, lpRect, EDGE_SUNKEN, BF_RECT & grfFlags);
			//	::InflateRect(lpRect, -::GetSystemMetrics(SM_CXEDGE), -::GetSystemMetrics(SM_CYEDGE));
		} 
	}
}

void CEditListBox::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	// TODO: Add your message handler code here and/or call default
	uMsg;
	ATLASSERT(WM_NCCALCSIZE == uMsg);
	
	LRESULT lRes = DefWindowProc(uMsg, wParam, lParam);
	
	LPNCCALCSIZE_PARAMS lpnccs = reinterpret_cast<LPNCCALCSIZE_PARAMS>(lParam);
	
	lpnccs->rgrc[0].top += m_captionInfo.nHeight + 2 * ::GetSystemMetrics(SM_CYEDGE);
	
	return lRes;

	CListBox::OnNcCalcSize(bCalcValidRects, lpncsp);
}
