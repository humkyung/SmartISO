// GetPdsLineID.cpp : implementation file
//

#include "stdafx.h"
#include "UserApp.h"
#include "GetPdsLineID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGetPdsLineID

IMPLEMENT_DYNCREATE(CGetPdsLineID, CFormView)

CGetPdsLineID::CGetPdsLineID()
	: CFormView(CGetPdsLineID::IDD), m_pBoldFont(NULL)
{
	//{{AFX_DATA_INIT(CGetPdsLineID)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CGetPdsLineID::~CGetPdsLineID()
{
	try
	{
		if(NULL != m_pBoldFont) delete m_pBoldFont;
	}
	catch(...)
	{
		
	}
}

void CGetPdsLineID::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGetPdsLineID)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGetPdsLineID, CFormView)
	//{{AFX_MSG_MAP(CGetPdsLineID)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGetPdsLineID diagnostics

#ifdef _DEBUG
void CGetPdsLineID::AssertValid() const
{
	CFormView::AssertValid();
}

void CGetPdsLineID::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGetPdsLineID message handlers

void CGetPdsLineID::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	const int gap = 3;
	
	if(m_viewLogFileStatic.m_hWnd)
	{
		CRect rect;
		m_viewLogFileStatic.GetWindowRect(rect);
		ScreenToClient(&rect);

		m_viewLogFileStatic.SetWindowPos(NULL , 0 , 0 , cx , rect.Height() , SWP_NOZORDER);
	}

	if(::IsWindow(m_wndScintilla.GetSafeHwnd()))
	{
		if(m_viewLogFileStatic.m_hWnd)
		{
			CRect rect;
			m_viewLogFileStatic.GetWindowRect(rect);
			ScreenToClient(&rect);

			m_wndScintilla.SetWindowPos(NULL , gap , rect.bottom + gap * 2 , cx - gap, cy - rect.bottom - gap * 2, SWP_NOZORDER);
		}
	}	
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-04
    @class      CGetPdsLineID
    @function   CreateScintillaWnd
    @return     int
    @brief
******************************************************************************/
int CGetPdsLineID::CreateScintillaWnd()
{
	if(NULL == m_wndScintilla.GetSafeHwnd())
	{
		const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;
		if (!m_wndScintilla.Create(_T("LogWnd"), WS_CHILD | WS_VISIBLE, CRect(0,0,0,0), this, 10000)) // hb - todo autogenerate id
		{   
			LPVOID lpMsgBuf;
			::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf,0,NULL);
			// Write to stderr
			TRACE (_T("%s\n"), (LPCTSTR)lpMsgBuf);
			// Free the buffer.
			LocalFree( lpMsgBuf );

			return -1;
		}

		m_wndScintilla.SetLexer(SCLEX_CPP);
		m_wndScintilla.SendMessage(SCI_SETSTYLEBITS, 5, 0);
		m_wndScintilla.SendMessage(SCE_B_NUMBER, RGB(0, 127, 127));
		m_wndScintilla.SendMessage(SCE_B_KEYWORD, RGB(0, 0, 127));
		m_wndScintilla.SendMessage(SCI_COLOURISE, 0, -1);
		m_wndScintilla.SetDisplayLinenumbers();
		m_wndScintilla.Init();

		// Set up the global default style. These attributes are used wherever no explicit choices are made.
		m_wndScintilla.SetAStyle( STYLE_DEFAULT, RGB(0 , 0 , 0) , RGB(255 , 255 , 255) , 10, _T("Courier New") );
		m_wndScintilla.SetCurrentLineHiLiting(true , RGB(232 , 232 , 255));

		// Set syntax colors
		for ( long i = 0; g_rgbSyntaxCpp[ i ].iItem != -1; i++ )
		{
			m_wndScintilla.SetAStyle( g_rgbSyntaxCpp[ i ].iItem, g_rgbSyntaxCpp[ i ].rgb , RGB(255 , 255 , 255) , 12 , _T("Courier New") );
		}
		m_wndScintilla.SendMessage(SCI_STYLESETBOLD, SCE_C_NUMBER , TRUE);
	}

	return ERROR_SUCCESS;
}

void CGetPdsLineID::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	static bool __init = false;

	if(false == __init)
	{
		m_pBoldFont = new CFont;
		m_pBoldFont->CreateFont(18,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"MS Sans Serif");
		
		m_viewLogFileStatic.SubclassWindow(GetDlgItem(IDC_STATIC_VIEW_LOG_FILE)->m_hWnd);
		m_viewLogFileStatic.SetGradientColor( GetSysColor(COLOR_BTNFACE) );
		m_viewLogFileStatic.SetColor( DarkenColor(GetSysColor(COLOR_BTNFACE),0.15) );
		m_viewLogFileStatic.SetVerticalGradient(); //set gradient to be vertical
		m_viewLogFileStatic.SetFont(m_pBoldFont);
		m_viewLogFileStatic.SetTextColor(RGB(0 , 0 , 0));
		
		CreateScintillaWnd();
		
		__init = true;
	}
	
}

/**
**/
void CGetPdsLineID::AddLog(const CString &rLog , const bool& bShow )
{
	CString rMsg(rLog);
	rMsg.Replace(_T("\n") , _T("\r\n"));
	if( true == bShow )
	{
		m_wndScintilla.AppendText(rMsg + _T("\r\n"));
	}
	m_LogTextEntry.push_back( rMsg.operator LPCSTR() );
}

/**
**/
void CGetPdsLineID::ClearAllLog()
{
	m_LogTextEntry.clear();
	m_wndScintilla.Clear();
}
