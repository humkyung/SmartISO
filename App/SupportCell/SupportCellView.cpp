// SupportCellView.cpp : implementation of the CSupportCellView class
//

#include "stdafx.h"
#include "SupportCell.h"

#include "SupportCellDoc.h"
#include "SupportCellView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSupportCellView

IMPLEMENT_DYNCREATE(CSupportCellView, CFormView)

BEGIN_MESSAGE_MAP(CSupportCellView, CFormView)
	//{{AFX_MSG_MAP(CSupportCellView)
	ON_WM_SIZE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSupportCellView construction/destruction

CSupportCellView::CSupportCellView()
	: CFormView(CSupportCellView::IDD)
{
	//{{AFX_DATA_INIT(CSupportCellView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// TODO: add construction code here

}

CSupportCellView::~CSupportCellView()
{
}

void CSupportCellView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSupportCellView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX , IDC_LIST_CELL_INFO , m_cellInfoListCtrl);
	//}}AFX_DATA_MAP
}

BOOL CSupportCellView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CSupportCellView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	CString rHeadings("Name , 200; File Pos , 200;Cell Size(Words) ,200; Origin X , 200;Origin Y , 200");
	m_cellInfoListCtrl.SetHeadings(rHeadings);
	m_cellInfoListCtrl.SetGridLines();
	
}

/////////////////////////////////////////////////////////////////////////////
// CSupportCellView diagnostics

#ifdef _DEBUG
void CSupportCellView::AssertValid() const
{
	CFormView::AssertValid();
}

void CSupportCellView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CSupportCellDoc* CSupportCellView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSupportCellDoc)));
	return (CSupportCellDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSupportCellView message handlers

void CSupportCellView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(m_cellInfoListCtrl.GetSafeHwnd())
	{
		m_cellInfoListCtrl.SetWindowPos(NULL , 0 , 0 , cx , cy , SWP_NOACTIVATE);
	}
}

int CSupportCellView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	return 0;
}

