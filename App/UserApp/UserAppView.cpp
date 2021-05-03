// UserAppView.cpp : implementation of the CUserAppView class
//

#include "stdafx.h"
#include "UserApp.h"
#include "UserAppDocData.h"
#include "UserAppDoc.h"
#include "UserAppView.h"
#include "Profile.h"
#include "Splash/SplashScreenFx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserAppView

IMPLEMENT_DYNCREATE(CUserAppView, CFormView)

BEGIN_MESSAGE_MAP(CUserAppView, CFormView)
	//{{AFX_MSG_MAP(CUserAppView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserAppView construction/destruction
static CUserAppView* __instance__ = NULL;

CUserAppView::CUserAppView() : CFormView(CUserAppView::IDD)/* , m_pSelUser(NULL)*/
{
	//{{AFX_DATA_INIT(CUserAppView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	__instance__ = this;
}

CUserAppView::~CUserAppView()
{
}

void CUserAppView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserAppView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BOOL CUserAppView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CUserAppView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	///ResizeParentToFit();
}

/////////////////////////////////////////////////////////////////////////////
// CUserAppView printing

BOOL CUserAppView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CUserAppView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CUserAppView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CUserAppView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
	UNUSED_ALWAYS( pDC );
}

/////////////////////////////////////////////////////////////////////////////
// CUserAppView diagnostics

#ifdef _DEBUG
void CUserAppView::AssertValid() const
{
	CFormView::AssertValid();
}

void CUserAppView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CUserAppDoc* CUserAppView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUserAppDoc)));
	return (CUserAppDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CUserAppView message handlers

int CUserAppView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRuntimeClass* pLineListConvertFormClass = RUNTIME_CLASS(CBackupDataForm);
	m_pBackupDataForm = (CBackupDataForm*)pLineListConvertFormClass->CreateObject();
	if(NULL != m_pBackupDataForm)
	{
		CCreateContext context;
		context.m_pNewViewClass = pLineListConvertFormClass;
		context.m_pCurrentDoc = NULL;
		if(!m_pBackupDataForm->Create(NULL , NULL , WS_CHILD | WS_BORDER , CRect(0 , 0 , 0 , 0) , this , AFX_IDW_PANE_FIRST , &context))
		{
			delete m_pBackupDataForm;
			return -1;
		}
	}

	CRuntimeClass* pLineListFormClass = RUNTIME_CLASS(CEditLineDataForm);
	m_pLineDataForm = (CEditLineDataForm*)pLineListFormClass->CreateObject();
	if(NULL != m_pLineDataForm)
	{
		CCreateContext context;
		context.m_pNewViewClass = pLineListFormClass;
		context.m_pCurrentDoc = NULL;
		if(!m_pLineDataForm->Create(NULL , NULL , WS_CHILD | WS_BORDER | WS_VISIBLE , CRect(0 , 0 , 0 , 0) , this , AFX_IDW_PANE_FIRST , &context))
		{
			delete m_pLineDataForm;
			return -1;
		}
	}

	CRuntimeClass* pInsertToPDSFormClass = RUNTIME_CLASS(CInsertToPDSAttribute);
	m_pInsertToPDS = (CInsertToPDSAttribute*)pInsertToPDSFormClass->CreateObject();
	if(NULL != m_pInsertToPDS)
	{
		CCreateContext context;
		context.m_pNewViewClass = pInsertToPDSFormClass;
		context.m_pCurrentDoc = NULL;
		if(!m_pInsertToPDS->Create(NULL , NULL , WS_CHILD | WS_BORDER , CRect(0 , 0 , 0 , 0) , this , AFX_IDW_PANE_FIRST , &context))
		{
			delete m_pInsertToPDS;
			return -1;
		}
	}

	/*
	CRuntimeClass* pPDSLineDataFormClass = RUNTIME_CLASS(CPDSLineDataForm);
	m_pPDSLineDataForm = (CPDSLineDataForm*)pPDSLineDataFormClass->CreateObject();
	if(NULL != m_pPDSLineDataForm)
	{
		CCreateContext context;
		context.m_pNewViewClass = pPDSLineDataFormClass;
		context.m_pCurrentDoc = GetDocument();
		if(!m_pPDSLineDataForm->Create(NULL , NULL , WS_CHILD | WS_BORDER , CRect(0 , 0 , 0 , 0) , this , AFX_IDW_PANE_FIRST , &context))
		{
			delete m_pPDSLineDataForm;
			return -1;
		}
	}
	*/
	
	CRuntimeClass* pIsoEditFormHolderClass = RUNTIME_CLASS(CIsoEditFormHolder);
	m_pIsoEditFormHolder = (CIsoEditFormHolder*)pIsoEditFormHolderClass->CreateObject();
	if(NULL != m_pIsoEditFormHolder)
	{
		CCreateContext context;
		context.m_pNewViewClass = pIsoEditFormHolderClass;
		context.m_pCurrentDoc = GetDocument();
		if(!m_pIsoEditFormHolder->Create(NULL , NULL , WS_CHILD | WS_BORDER , CRect(0 , 0 , 0 , 0) , this , AFX_IDW_PANE_FIRST , &context))
		{
			SAFE_DELETE( m_pIsoEditFormHolder );
			return -1;
		}
	}

	/*CRuntimeClass* pIsoEditFormClass = RUNTIME_CLASS(CIsoEditForm);
	m_pIsoEditForm = (CIsoEditForm*)pIsoEditFormClass->CreateObject();
	if(NULL != m_pIsoEditForm)
	{
		CCreateContext context;
		context.m_pNewViewClass = pIsoEditFormClass;
		context.m_pCurrentDoc = NULL;
		if(!m_pIsoEditForm->Create(NULL , NULL , WS_CHILD | WS_BORDER , CRect(0 , 0 , 0 , 0) , this , AFX_IDW_PANE_FIRST , &context))
		{
			delete m_pIsoEditForm;
			return -1;
		}
	}*/

	CRuntimeClass* pDrawingReportFormClass = RUNTIME_CLASS(CDrawingReportForm);
	m_pDrawingReportForm = (CDrawingReportForm*)pDrawingReportFormClass->CreateObject();
	if(NULL != m_pDrawingReportForm)
	{
		CCreateContext context;
		context.m_pNewViewClass = pDrawingReportFormClass;
		context.m_pCurrentDoc = NULL;
		if(!m_pDrawingReportForm->Create(NULL , NULL , WS_CHILD | WS_BORDER , CRect(0 , 0 , 0 , 0) , this , AFX_IDW_PANE_FIRST , &context))
		{
			delete m_pDrawingReportForm;
			return -1;
		}
	}

	CRuntimeClass* pBMReportFormClass = RUNTIME_CLASS(CBMReportForm);
	m_pBMReportForm = (CBMReportForm*)pBMReportFormClass->CreateObject();
	if(NULL != m_pBMReportForm)
	{
		CCreateContext context;
		context.m_pNewViewClass = pBMReportFormClass;
		context.m_pCurrentDoc = NULL;
		if(!m_pBMReportForm->Create(NULL , NULL , WS_CHILD | WS_BORDER , CRect(0 , 0 , 0 , 0) , this , AFX_IDW_PANE_FIRST , &context))
		{
			delete m_pBMReportForm;
			return -1;
		}
	}

	CRuntimeClass* pDrawingPlottingForm = RUNTIME_CLASS(CDrawingPlottingForm);
	m_pDrawingPlottingForm = (CDrawingPlottingForm*)pDrawingPlottingForm->CreateObject();
	if(NULL != m_pDrawingPlottingForm)
	{
		CCreateContext context;
		context.m_pNewViewClass = pDrawingPlottingForm;
		context.m_pCurrentDoc = NULL;
		if(!m_pDrawingPlottingForm->Create(NULL , NULL , WS_CHILD | WS_BORDER , CRect(0 , 0 , 0 , 0) , this , AFX_IDW_PANE_FIRST , &context))
		{
			delete m_pDrawingPlottingForm;
			return -1;
		}
	}

	return 0;
}

void CUserAppView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	if(m_pBackupDataForm->m_hWnd)
	{
		m_pBackupDataForm->SetWindowPos(NULL , 0 , 0 , cx , cy , SWP_NOMOVE);
	}

	if(m_pLineDataForm->m_hWnd)
	{
		m_pLineDataForm->SetWindowPos(NULL , 0 , 0 , cx , cy , SWP_NOMOVE);
	}

	if(m_pInsertToPDS->m_hWnd)
	{
		m_pInsertToPDS->SetWindowPos(NULL, 0 , 0 , cx , cy , SWP_NOMOVE);
	}

	if(m_pIsoEditFormHolder->GetSafeHwnd())
	{
		m_pIsoEditFormHolder->SetWindowPos(NULL , 0 , 0 , cx , cy , SWP_NOMOVE);
	}
	/*if(m_pIsoEditForm->m_hWnd)
	{
		m_pIsoEditForm->SetWindowPos(NULL , 0 , 0 , cx , cy , SWP_NOMOVE);
	}*/

	if(m_pDrawingReportForm->m_hWnd)
	{
		m_pDrawingReportForm->SetWindowPos(NULL , 0 , 0 , cx , cy , SWP_NOMOVE);
	}

	if(m_pBMReportForm->m_hWnd)
	{
		m_pBMReportForm->SetWindowPos(NULL , 0 , 0 , cx , cy , SWP_NOMOVE);
	}

	if(m_pDrawingPlottingForm->m_hWnd)
	{
		m_pDrawingPlottingForm->SetWindowPos(NULL, 0 , 0 , cx , cy , SWP_NOMOVE);
	}
}

/**	
	@brief	컨트롤들을 초기화시킨다.
	@return	void	
*/
void CUserAppView::InitContents()
{
	CAppDocData* pDocData = CAppDocData::GetInstance();

	//m_pSelUser = pDocData->m_pSelUser;

	if(m_pIsoEditFormHolder)
	{
		CSplashScreenFx* pSplash = CSplashScreenFx::GetInstance();
		if(pSplash) pSplash->m_ctrlProgress.SetPos(0);

		m_pBackupDataForm->InitContents();
		if(pSplash) pSplash->m_ctrlProgress.SetPos(15);

		m_pLineDataForm->InitContents();
		if(pSplash) pSplash->m_ctrlProgress.SetPos(30);

		m_pInsertToPDS->InitContents();
		if(pSplash) pSplash->m_ctrlProgress.SetPos(45);

		m_pIsoEditFormHolder->InitContents();
		if(pSplash) pSplash->m_ctrlProgress.SetPos(60);

		m_pDrawingReportForm->InitContents();
		if(pSplash) pSplash->m_ctrlProgress.SetPos(75);
		
		m_pBMReportForm->InitContents();
		if(pSplash) pSplash->m_ctrlProgress.SetPos(90);

		m_pDrawingPlottingForm->InitContents();

		if(pSplash) pSplash->m_ctrlProgress.SetPos(100);
	}
}

void CUserAppView::ShowBackupDataForm(const bool &show)
{
	if(NULL != m_pBackupDataForm)
	{
		if(show)
		{
			m_pBackupDataForm->ShowWindow(SW_SHOWNORMAL);
			m_pInsertToPDS->ShowWindow(SW_HIDE);
			m_pBMReportForm->ShowWindow(SW_HIDE);
			m_pDrawingReportForm->ShowWindow(SW_HIDE);
			m_pLineDataForm->ShowWindow(SW_HIDE);
			m_pIsoEditFormHolder->ShowWindow(SW_HIDE);
			m_pDrawingPlottingForm->ShowWindow(SW_HIDE);
		}
		else
		{
			m_pBackupDataForm->ShowWindow(SW_HIDE);
		}
	}
}

void CUserAppView::ShowLineDataForm(const bool &show)
{
	if(NULL != m_pLineDataForm)
	{
		if(show)
		{
			m_pLineDataForm->ShowWindow(SW_SHOWNORMAL);
			m_pInsertToPDS->ShowWindow(SW_HIDE);
			m_pBMReportForm->ShowWindow(SW_HIDE);
			m_pDrawingReportForm->ShowWindow(SW_HIDE);
			///m_pPDSLineDataForm->ShowWindow(SW_HIDE);
			m_pBackupDataForm->ShowWindow(SW_HIDE);
			m_pIsoEditFormHolder->ShowWindow(SW_HIDE);
			m_pDrawingPlottingForm->ShowWindow(SW_HIDE);
		}
		else
		{
			m_pLineDataForm->ShowWindow(SW_HIDE);
		}
	}
}

/******************************************************************************
    @author     humkyung
    @date       ????-??-??
    @class      CUserAppView
    @function   ShowUpdatePDSForm
    @return     void
    @param      const   bool
    @param      &show
    @brief
******************************************************************************/
void CUserAppView::ShowUpdatePDSForm(const bool &show)
{
	if(NULL != m_pInsertToPDS)
	{
		if(show)
		{
			m_pInsertToPDS->ShowWindow(SW_SHOWNORMAL);
			m_pLineDataForm->ShowWindow(SW_HIDE);
			m_pBMReportForm->ShowWindow(SW_HIDE);
			m_pDrawingReportForm->ShowWindow(SW_HIDE);
			///m_pPDSLineDataForm->ShowWindow(SW_HIDE);
			m_pBackupDataForm->ShowWindow(SW_HIDE);
			m_pIsoEditFormHolder->ShowWindow(SW_HIDE);
			m_pDrawingPlottingForm->ShowWindow(SW_HIDE);
		}
		else
		{
			m_pInsertToPDS->ShowWindow(SW_HIDE);
		}
	}

}

void CUserAppView::ShowIsoEditForm(const bool &show)
{
	if(NULL != m_pIsoEditFormHolder)
	{
		if(show)
		{
			m_pIsoEditFormHolder->ShowWindow(SW_SHOWNORMAL);
			m_pInsertToPDS->ShowWindow(SW_HIDE);
			m_pBMReportForm->ShowWindow(SW_HIDE);
			m_pDrawingReportForm->ShowWindow(SW_HIDE);
			m_pBackupDataForm->ShowWindow(SW_HIDE);
			m_pLineDataForm->ShowWindow(SW_HIDE);
			m_pDrawingPlottingForm->ShowWindow(SW_HIDE);

			m_pIsoEditFormHolder->SetFocus();
		}
		else
		{
			m_pIsoEditFormHolder->ShowWindow(SW_HIDE);
		}
	}
}

void CUserAppView::ShowIndexReportForm(const bool &show)
{
	if(NULL != m_pDrawingReportForm)
	{
		if(show)
		{
			m_pDrawingReportForm->ShowWindow(SW_SHOWNORMAL);
			m_pInsertToPDS->ShowWindow(SW_HIDE);
			m_pBMReportForm->ShowWindow(SW_HIDE);
			m_pIsoEditFormHolder->ShowWindow(SW_HIDE);
			m_pBackupDataForm->ShowWindow(SW_HIDE);
			m_pLineDataForm->ShowWindow(SW_HIDE);
			m_pDrawingPlottingForm->ShowWindow(SW_HIDE);
		}
		else
		{
			m_pIsoEditFormHolder->ShowWindow(SW_HIDE);
		}
	}
}

void CUserAppView::ShowBMReportForm(const bool &show)
{
	if(NULL != m_pBMReportForm)
	{
		if(show)
		{
			m_pBMReportForm->ShowWindow(SW_SHOWNORMAL);
			m_pInsertToPDS->ShowWindow(SW_HIDE);
			m_pDrawingReportForm->ShowWindow(SW_HIDE);
			m_pIsoEditFormHolder->ShowWindow(SW_HIDE);
			///m_pPDSLineDataForm->ShowWindow(SW_HIDE);
			m_pBackupDataForm->ShowWindow(SW_HIDE);
			m_pLineDataForm->ShowWindow(SW_HIDE);
			m_pDrawingPlottingForm->ShowWindow(SW_HIDE);
		}
		else
		{
			m_pBMReportForm->ShowWindow(SW_HIDE);
		}
	}
}

/**
**/
void CUserAppView::ShowDrawingPlottingForm(const bool &show)
{
	if(NULL != m_pDrawingPlottingForm)
	{
		if(show)
		{
			m_pDrawingPlottingForm->ShowWindow(SW_SHOWNORMAL);
			m_pBMReportForm->ShowWindow(SW_HIDE);
			m_pInsertToPDS->ShowWindow(SW_HIDE);
			m_pDrawingReportForm->ShowWindow(SW_HIDE);
			m_pIsoEditFormHolder->ShowWindow(SW_HIDE);
			///m_pPDSLineDataForm->ShowWindow(SW_HIDE);
			m_pBackupDataForm->ShowWindow(SW_HIDE);
			m_pLineDataForm->ShowWindow(SW_HIDE);
		}
		else
		{
			m_pDrawingPlottingForm->ShowWindow(SW_HIDE);
		}
	}
}

void CUserAppView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CFormView::OnKeyDown(nChar, nRepCnt, nFlags);
}

CUserAppView* CUserAppView::GetInstance()
{
	return __instance__;
}
