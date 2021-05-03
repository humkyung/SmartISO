// ViewLogFileForm.cpp : implementation file
//

#include "stdafx.h"
#include "UserApp.h"
#include "ViewLogFileForm.h"

#include <fstream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewLogFileForm

IMPLEMENT_DYNCREATE(CViewLogFileForm, CFormView)

CViewLogFileForm::CViewLogFileForm() : CFormView(CViewLogFileForm::IDD)
{
	//{{AFX_DATA_INIT(CViewLogFileForm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CViewLogFileForm::~CViewLogFileForm()
{
	try
	{
	}
	catch(...)
	{

	}
}

void CViewLogFileForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewLogFileForm)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewLogFileForm, CFormView)
	//{{AFX_MSG_MAP(CViewLogFileForm)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewLogFileForm diagnostics

#ifdef _DEBUG
void CViewLogFileForm::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewLogFileForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewLogFileForm message handlers

BOOL CViewLogFileForm::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CViewLogFileForm::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
}

void CViewLogFileForm::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
}