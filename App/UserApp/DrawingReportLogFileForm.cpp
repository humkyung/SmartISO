// DrawingReportLogFileForm.cpp : implementation file
//

#include "stdafx.h"
#include "UserApp.h"
#include "DrawingReportLogFileForm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDrawingReportLogFileForm

IMPLEMENT_DYNCREATE(CDrawingReportLogFileForm, CFormView)

CDrawingReportLogFileForm::CDrawingReportLogFileForm()
	: CFormView(CDrawingReportLogFileForm::IDD)
{
	//{{AFX_DATA_INIT(CDrawingReportLogFileForm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDrawingReportLogFileForm::~CDrawingReportLogFileForm()
{
	try
	{
		///if(NULL != m_pBoldFont) delete m_pBoldFont;
	}
	catch(...)
	{

	}
}

void CDrawingReportLogFileForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDrawingReportLogFileForm)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDrawingReportLogFileForm, CFormView)
	//{{AFX_MSG_MAP(CDrawingReportLogFileForm)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawingReportLogFileForm diagnostics

#ifdef _DEBUG
void CDrawingReportLogFileForm::AssertValid() const
{
	CFormView::AssertValid();
}

void CDrawingReportLogFileForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDrawingReportLogFileForm message handlers

void CDrawingReportLogFileForm::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
}

///******************************************************************************
//    @author     humkyung
//    @date       2011-11-04
//    @class      CDrawingReportLogFileForm
//    @function   CreateScintillaWnd
//    @return     int
//    @brief
//******************************************************************************/
//int CDrawingReportLogFileForm::CreateScintillaWnd()
//{
//	if(NULL == m_wndScintilla.GetSafeHwnd())
//	{
//		const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;
//		if (!m_wndScintilla.Create(_T("LogWnd"), WS_CHILD | WS_VISIBLE, CRect(0,0,0,0), &(this->m_wndTabCtrl) , 10000)) // hb - todo autogenerate id
//		{   
//			LPVOID lpMsgBuf;
//			::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
//				NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf,0,NULL);
//			// Write to stderr
//			TRACE (_T("%s\n"), (LPCTSTR)lpMsgBuf);
//			// Free the buffer.
//			LocalFree( lpMsgBuf );
//
//			return -1;
//		}
//		m_wndTabCtrl.AddTab(&m_wndScintilla , _T("LOG"));
//
//		m_wndScintilla.SetLexer(SCLEX_CPP);
//		m_wndScintilla.SendMessage(SCI_SETSTYLEBITS, 5, 0);
//		m_wndScintilla.SendMessage(SCE_B_NUMBER, RGB(0, 127, 127));
//		m_wndScintilla.SendMessage(SCE_B_KEYWORD, RGB(0, 0, 127));
//		m_wndScintilla.SendMessage(SCI_COLOURISE, 0, -1);
//		m_wndScintilla.SetDisplayLinenumbers();
//		m_wndScintilla.Init();
//
//		// Set up the global default style. These attributes are used wherever no explicit choices are made.
//		m_wndScintilla.SetAStyle( STYLE_DEFAULT, RGB(0 , 0 , 0) , RGB(255 , 255 , 255) , 10, _T("Courier New") );
//		m_wndScintilla.SetCurrentLineHiLiting(true , RGB(232 , 232 , 255));
//
//		// Set syntax colors
//		for ( long i = 0; g_rgbSyntaxCpp[ i ].iItem != -1; i++ )
//		{
//			m_wndScintilla.SetAStyle( g_rgbSyntaxCpp[ i ].iItem, g_rgbSyntaxCpp[ i ].rgb , RGB(255 , 255 , 255) , 12 , _T("Courier New") );
//		}
//		m_wndScintilla.SendMessage(SCI_STYLESETBOLD, SCE_C_NUMBER , TRUE);
//	}
//
//	return ERROR_SUCCESS;
//}

BOOL CDrawingReportLogFileForm::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CDrawingReportLogFileForm::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
}

//void CDrawingReportLogFileForm::AddLog(const CString &rLog)
//{
//	CString rMsg(rLog);
//	rMsg.Replace(_T("\n") , _T("\r\n"));
//	m_wndScintilla.AppendText(rMsg + _T("\r\n"));
//}