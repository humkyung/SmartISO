// ogl2dTestView.cpp : implementation of the COgl2dTestView class
//

#include "stdafx.h"
#include "ogl2dTest.h"

#include "ogl2dTestDoc.h"
#include "ogl2dTestView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COgl2dTestView

IMPLEMENT_DYNCREATE(COgl2dTestView, CView)

BEGIN_MESSAGE_MAP(COgl2dTestView, CView)
	//{{AFX_MSG_MAP(COgl2dTestView)
	ON_WM_SIZE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COgl2dTestView construction/destruction

COgl2dTestView::COgl2dTestView()
{
	// TODO: add construction code here

}

COgl2dTestView::~COgl2dTestView()
{
}

BOOL COgl2dTestView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// COgl2dTestView drawing

void COgl2dTestView::OnDraw(CDC* pDC)
{
	COgl2dTestDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// COgl2dTestView printing

BOOL COgl2dTestView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void COgl2dTestView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void COgl2dTestView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// COgl2dTestView diagnostics

#ifdef _DEBUG
void COgl2dTestView::AssertValid() const
{
	CView::AssertValid();
}

void COgl2dTestView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COgl2dTestDoc* COgl2dTestView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COgl2dTestDoc)));
	return (COgl2dTestDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COgl2dTestView message handlers

void COgl2dTestView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	if(m_glCanvas.GetSafeHwnd())
	{
		m_glCanvas.SetWindowPos(NULL , 0 , 0 , cx , cy , SWP_NOZORDER);
	}
}

int COgl2dTestView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if(!m_glCanvas.GetSafeHwnd()) 
	{
		CRect rect;
		rect.SetRectEmpty();
		
		m_glCanvas.Create(TRUE, &rect, this);
		/*
		m_GLCanvas.SetScene(&m_GLScene);
		
		CIsVolume vol = m_GLScene.GetVolume();
		m_GLCanvas.m_dTrans[2] = (vol.maxx() - vol.minx())/200.f;
		m_GLCanvas.m_dTrans[0] = -(m_GLScene.m_ptCenter.x());
		m_GLCanvas.m_dTrans[1] = -(m_GLScene.m_ptCenter.y());
		*/
	}
	
	return 0;
}
