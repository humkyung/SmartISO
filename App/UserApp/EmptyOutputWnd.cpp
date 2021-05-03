// WritePds.cpp : implementation file
//

#include "stdafx.h"
#include "UserApp.h"
#include "MainFrm.h"
#include "UserAppView.h"
#include "UserAppDocData.h"
#include <BCMenu.h>

#include "tokenizer.h"
#include "EmptyOutputWnd.h"
#include "WorkStatusDlg.h"
#include <ado/SQLFile.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEmptyOutputWnd

IMPLEMENT_DYNCREATE(CEmptyOutputWnd, CFormView)

CEmptyOutputWnd::CEmptyOutputWnd()
	: CFormView(CEmptyOutputWnd::IDD),m_pUserAppDoc(NULL)
{
	//{{AFX_DATA_INIT(CEmptyOutputWnd)
	//}}AFX_DATA_INIT
}

CEmptyOutputWnd::~CEmptyOutputWnd()
{
	try
	{
	}
	catch(...)
	{
		
	}
}

void CEmptyOutputWnd::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CEmptyOutputWnd, CFormView)
	//{{AFX_MSG_MAP(CEmptyOutputWnd)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEmptyOutputWnd diagnostics

#ifdef _DEBUG
void CEmptyOutputWnd::AssertValid() const
{
	CFormView::AssertValid();
}

void CEmptyOutputWnd::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEmptyOutputWnd message handlers

BOOL CEmptyOutputWnd::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CEmptyOutputWnd::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
}

void CEmptyOutputWnd::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
}