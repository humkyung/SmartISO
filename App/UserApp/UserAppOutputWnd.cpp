// UserAppOutputWnd.cpp : implementation file
//

#include "stdafx.h"
#include "UserApp.h"
#include "MainFrm.h"
#include "UserAppView.h"
#include "UserAppOutputWnd.h"

// CUserAppOutputWnd

IMPLEMENT_DYNAMIC(CUserAppOutputWnd, CDockablePane)

CUserAppOutputWnd::CUserAppOutputWnd()
{
	m_pUserAppDoc = NULL;

	m_pQualifiedLineForm = NULL;
	m_pLineDataEditDlg = NULL;
	//m_pGetPdsLogFileForm = NULL;
	m_pDrawingReportLogFileForm = NULL;
	m_pIsoEditViewLogFileForm = NULL;
	m_pViewLogFileForm = NULL;
	m_pBackupDataOutputForm = NULL;
	m_pQualifiedLineForm = NULL;
	m_pBMLogFileForm = NULL;
	m_pEmptyOutputWnd = NULL;
}

CUserAppOutputWnd::~CUserAppOutputWnd()
{
}


BEGIN_MESSAGE_MAP(CUserAppOutputWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CUserAppOutputWnd message handlers

int CUserAppOutputWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CMainFrame* pMainFrame = GetUserAppMainWnd();
	CUserAppView* pUserAppView = (CUserAppView*)(CUserAppView::GetInstance());

	//////////////////////////////////////////////////////////////////////////
	CRuntimeClass* pBackupOuputFormClass = RUNTIME_CLASS(CBackupDataOutputForm);
	m_pBackupDataOutputForm = (CBackupDataOutputForm*)pBackupOuputFormClass->CreateObject();
	if(NULL != m_pBackupDataOutputForm)
	{
		CCreateContext context;
		context.m_pNewViewClass = pBackupOuputFormClass;
		context.m_pCurrentDoc = NULL;
		if(!m_pBackupDataOutputForm->Create(NULL , NULL , WS_CHILD | WS_BORDER , CRect(0 , 0 , 0 , 0) , this , AFX_IDW_PANE_FIRST , &context))
		{
			delete m_pBackupDataOutputForm;
			return -1;
		}
		pUserAppView->m_pBackupDataForm->SetOutputForm(m_pBackupDataOutputForm);
	}

	CRuntimeClass* pQualifiedLineFormClass = RUNTIME_CLASS(CQualifiedLineForm);
	m_pQualifiedLineForm = (CQualifiedLineForm*)pQualifiedLineFormClass->CreateObject();
	if(NULL != m_pQualifiedLineForm)
	{
		CCreateContext context;
		context.m_pNewViewClass = pQualifiedLineFormClass;
		context.m_pCurrentDoc = NULL;
		if(!m_pQualifiedLineForm->Create(NULL , NULL , WS_CHILD | WS_BORDER , CRect(0 , 0 , 0 , 0) , this , AFX_IDW_PANE_FIRST , &context))
		{
			delete m_pQualifiedLineForm;
			return -1;
		}
	}

	CRuntimeClass* pIsoEditViweLogFileFormClass = RUNTIME_CLASS(CIsoEditViewLogFileForm);
	m_pIsoEditViewLogFileForm = (CIsoEditViewLogFileForm*)pIsoEditViweLogFileFormClass->CreateObject();
	if(NULL != m_pIsoEditViewLogFileForm)
	{
		CCreateContext context;
		context.m_pNewViewClass = pIsoEditViweLogFileFormClass;
		context.m_pCurrentDoc   = NULL;
		if(!m_pIsoEditViewLogFileForm->Create(NULL , NULL , WS_CHILD | WS_BORDER , CRect(0 , 0 , 0 , 0) , this , AFX_IDW_PANE_FIRST , &context))
		{
			delete m_pIsoEditViewLogFileForm;
			return -1;
		}
		pUserAppView->m_pIsoEditFormHolder->SetViewLogFileForm(m_pIsoEditViewLogFileForm);
	}

	CRuntimeClass* pViweLogFileFormClass = RUNTIME_CLASS(CViewLogFileForm);
	m_pViewLogFileForm = (CViewLogFileForm*)pViweLogFileFormClass->CreateObject();
	if(NULL != m_pViewLogFileForm)
	{
		CCreateContext context;
		context.m_pNewViewClass = pViweLogFileFormClass;
		context.m_pCurrentDoc = NULL;
		if(!m_pViewLogFileForm->Create(NULL , NULL , WS_CHILD | WS_BORDER , CRect(0 , 0 , 0 , 0) , this , AFX_IDW_PANE_FIRST , &context))
		{
			delete m_pViewLogFileForm;
			return -1;
		}
		pUserAppView->m_pDrawingPlottingForm->SetViewLogFileForm(m_pViewLogFileForm);
	}

	CRuntimeClass* pDrawingReportLogFileFormClass = RUNTIME_CLASS(CDrawingReportLogFileForm);
	m_pDrawingReportLogFileForm = (CDrawingReportLogFileForm*)pDrawingReportLogFileFormClass->CreateObject();
	if(NULL != m_pDrawingReportLogFileForm)
	{
		CCreateContext context;
		context.m_pNewViewClass = pDrawingReportLogFileFormClass;
		context.m_pCurrentDoc = NULL;
		if(!m_pDrawingReportLogFileForm->Create(NULL , NULL , WS_CHILD | WS_BORDER , CRect(0 , 0 , 0 , 0) , this , AFX_IDW_PANE_FIRST , &context))
		{
			delete m_pDrawingReportLogFileForm;
			return -1;
		}
		pUserAppView->m_pDrawingReportForm->SetViewLogFileForm(m_pDrawingReportLogFileForm);
	}
	
	CRuntimeClass* pBMLogFileFormClass = RUNTIME_CLASS(CBMLogFileForm);
	m_pBMLogFileForm = (CBMLogFileForm*)pBMLogFileFormClass->CreateObject();
	if(NULL != m_pBMLogFileForm)
	{
		CCreateContext context;
		context.m_pNewViewClass = pBMLogFileFormClass;
		context.m_pCurrentDoc = NULL;
		if(!m_pBMLogFileForm->Create(NULL , NULL , WS_CHILD | WS_BORDER , CRect(0 , 0 , 0 , 0) , this , AFX_IDW_PANE_FIRST , &context))
		{
			delete m_pBMLogFileForm;
			return -1;
		}
		pUserAppView->m_pBMReportForm->SetViewLogFileForm(m_pBMLogFileForm);
	}

	
	CRuntimeClass* pWritePdsClass = RUNTIME_CLASS(CEmptyOutputWnd);
	m_pEmptyOutputWnd = (CEmptyOutputWnd*)pWritePdsClass->CreateObject();
	if(NULL != m_pEmptyOutputWnd)
	{
		CCreateContext context;
		context.m_pNewViewClass = pWritePdsClass;
		context.m_pCurrentDoc = NULL;
		if(!m_pEmptyOutputWnd->Create(NULL , NULL , WS_CHILD | WS_BORDER , CRect(0 , 0 , 0 , 0) , this , AFX_IDW_PANE_FIRST , &context))
		{
			delete m_pEmptyOutputWnd;
			return -1;
		}
		pUserAppView->m_pInsertToPDS->SetViewLogFileForm(m_pEmptyOutputWnd);
	}
	
	CRuntimeClass* pLineDataEditDlg = RUNTIME_CLASS(CLineDataEditDlg);
	m_pLineDataEditDlg = (CLineDataEditDlg*)pLineDataEditDlg->CreateObject();
	if(NULL != m_pLineDataEditDlg)
	{
		CCreateContext context;
		context.m_pNewViewClass = pLineDataEditDlg;
		context.m_pCurrentDoc = NULL;
		if(!m_pLineDataEditDlg->Create(NULL , NULL , WS_CHILD | WS_BORDER | WS_VISIBLE , CRect(0 , 0 , 0 , 0) , this , AFX_IDW_PANE_FIRST , &context))
		{
			delete m_pLineDataEditDlg;
			return -1;
		}
		pUserAppView->m_pLineDataForm->m_pQualifiedLineForm = m_pLineDataEditDlg;
	}

	return 0;
}

BOOL CUserAppOutputWnd::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CUserAppOutputWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	this->RecalcLayout();
	
	CRect rect;
	this->GetWindowRect(&rect);
	ScreenToClient(&rect);

	if(NULL != m_pBackupDataOutputForm)
	{
		if(m_pBackupDataOutputForm->m_hWnd)
		{
			m_pBackupDataOutputForm->SetWindowPos(NULL , rect.left , rect.top , rect.Width() , rect.Height() , SWP_NOMOVE);
		}

		if(m_pQualifiedLineForm->m_hWnd)
		{
			m_pQualifiedLineForm->SetWindowPos(NULL , rect.top , rect.top , rect.Width() , rect.Height() , SWP_NOMOVE);
		}

		if(m_pIsoEditViewLogFileForm->GetSafeHwnd())
		{
			m_pIsoEditViewLogFileForm->SetWindowPos(NULL , rect.left , rect.top , rect.Width() , rect.Height() , SWP_NOMOVE);
		}

		if(m_pViewLogFileForm->m_hWnd)
		{
			m_pViewLogFileForm->SetWindowPos(NULL , rect.left , rect.top , rect.Width() , rect.Height() , SWP_NOMOVE);
		}

		if(m_pDrawingReportLogFileForm->m_hWnd)
		{
			m_pDrawingReportLogFileForm->SetWindowPos(NULL , rect.left , rect.top , rect.Width() , rect.Height() , SWP_NOMOVE);
		}

		if(m_pBMLogFileForm->m_hWnd)
		{
			m_pBMLogFileForm->SetWindowPos(NULL , rect.left , rect.top , rect.Width() , rect.Height() , SWP_NOMOVE);
		}
		
		if(m_pEmptyOutputWnd->GetSafeHwnd())
		{
			m_pEmptyOutputWnd->SetWindowPos(NULL , rect.left , rect.top , rect.Width() , rect.Height() , SWP_NOMOVE);
		}

		if(m_pLineDataEditDlg->GetSafeHwnd())
		{
			m_pLineDataEditDlg->SetWindowPos(NULL, rect.left , rect.top , rect.Width() , rect.Height() , SWP_NOMOVE);
		}
	}
}

void CUserAppOutputWnd::ShowBackupDataOutputForm(const bool &show)
{
	if(NULL != m_pBackupDataOutputForm)
	{
		if(show)
		{
			m_pBackupDataOutputForm->ShowWindow(SW_SHOWNORMAL);

			m_pDrawingReportLogFileForm->ShowWindow(SW_HIDE);
			m_pQualifiedLineForm->ShowWindow(SW_HIDE);
			m_pIsoEditViewLogFileForm->ShowWindow(SW_HIDE);
			m_pViewLogFileForm->ShowWindow(SW_HIDE);
			m_pBMLogFileForm->ShowWindow(SW_HIDE);
			m_pEmptyOutputWnd->ShowWindow(SW_HIDE);
			m_pLineDataEditDlg->ShowWindow(SW_HIDE);
		}
		else
		{
			m_pBackupDataOutputForm->ShowWindow(SW_HIDE);
		}
	}
}

void CUserAppOutputWnd::ShowQualifiedLineForm(const bool &show)
{
	if(NULL != m_pQualifiedLineForm)
	{
		if(show)
		{
			m_pQualifiedLineForm->ShowWindow(SW_SHOWNORMAL);
			
			m_pDrawingReportLogFileForm->ShowWindow(SW_HIDE);
			m_pBackupDataOutputForm->ShowWindow(SW_HIDE);
			m_pIsoEditViewLogFileForm->ShowWindow(SW_HIDE);
			m_pViewLogFileForm->ShowWindow(SW_HIDE);
			m_pBMLogFileForm->ShowWindow(SW_HIDE);
			m_pEmptyOutputWnd->ShowWindow(SW_HIDE);
			m_pLineDataEditDlg->ShowWindow(SW_HIDE);
		}
		else
		{
			m_pQualifiedLineForm->ShowWindow(SW_HIDE);
		}
	}
}

/**
**/
void CUserAppOutputWnd::ShowIsoEditViewLogFileForm(const bool &show)
{
	if(NULL != m_pViewLogFileForm)
	{
		if(show)
		{
			m_pIsoEditViewLogFileForm->ShowWindow(SW_SHOWNORMAL);
			
			m_pDrawingReportLogFileForm->ShowWindow(SW_HIDE);
			m_pBackupDataOutputForm->ShowWindow(SW_HIDE);
			m_pViewLogFileForm->ShowWindow(SW_HIDE);
			m_pQualifiedLineForm->ShowWindow(SW_HIDE);
			m_pBMLogFileForm->ShowWindow(SW_HIDE);
			m_pEmptyOutputWnd->ShowWindow(SW_HIDE);
			m_pLineDataEditDlg->ShowWindow(SW_HIDE);
		}
		else
		{
			m_pViewLogFileForm->ShowWindow(SW_HIDE);
		}
	}
}

/**
**/
void CUserAppOutputWnd::ShowViewLogFileForm(const bool &show)
{
	if(NULL != m_pViewLogFileForm)
	{
		if(show)
		{
			m_pViewLogFileForm->ShowWindow(SW_SHOWNORMAL);
			
			m_pDrawingReportLogFileForm->ShowWindow(SW_HIDE);
			m_pBackupDataOutputForm->ShowWindow(SW_HIDE);
			m_pIsoEditViewLogFileForm->ShowWindow(SW_HIDE);
			m_pQualifiedLineForm->ShowWindow(SW_HIDE);
			m_pBMLogFileForm->ShowWindow(SW_HIDE);
			m_pEmptyOutputWnd->ShowWindow(SW_HIDE);
			m_pLineDataEditDlg->ShowWindow(SW_HIDE);
		}
		else
		{
			m_pViewLogFileForm->ShowWindow(SW_HIDE);
		}
	}
}

void CUserAppOutputWnd::ShowBMLogFileForm(const bool &show)
{
	if(NULL != m_pBMLogFileForm)
	{
		if(show)
		{
			m_pBMLogFileForm->ShowWindow(SW_SHOWNORMAL);
			
			m_pDrawingReportLogFileForm->ShowWindow(SW_HIDE);
			m_pBackupDataOutputForm->ShowWindow(SW_HIDE);
			m_pQualifiedLineForm->ShowWindow(SW_HIDE);
			m_pIsoEditViewLogFileForm->ShowWindow(SW_HIDE);
			m_pViewLogFileForm->ShowWindow(SW_HIDE);
			m_pEmptyOutputWnd->ShowWindow(SW_HIDE);
			m_pLineDataEditDlg->ShowWindow(SW_HIDE);
		}
		else
		{
			m_pBMLogFileForm->ShowWindow(SW_HIDE);
		}
	}
}

void CUserAppOutputWnd::ShowUpdatePds(const bool &show)
{
	if(NULL != m_pEmptyOutputWnd)
	{
		if(show)
		{
			m_pEmptyOutputWnd->ShowWindow(SW_SHOWNORMAL);

			m_pBMLogFileForm->ShowWindow(SW_HIDE);	
			m_pDrawingReportLogFileForm->ShowWindow(SW_HIDE);
			m_pBackupDataOutputForm->ShowWindow(SW_HIDE);
			m_pQualifiedLineForm->ShowWindow(SW_HIDE);
			m_pIsoEditViewLogFileForm->ShowWindow(SW_HIDE);
			m_pViewLogFileForm->ShowWindow(SW_HIDE);
			//m_pGetPdsLogFileForm->ShowWindow(SW_HIDE);
			m_pLineDataEditDlg->ShowWindow(SW_HIDE);
		}
		else
		{
			m_pEmptyOutputWnd->ShowWindow(SW_HIDE);
		}
	}
}

/**
	\brief	
	\author	¹éÈì°æ
**/
void CUserAppOutputWnd::HideAll()
{
	m_pIsoEditViewLogFileForm->ShowWindow(SW_HIDE);
	m_pViewLogFileForm->ShowWindow(SW_HIDE);
	m_pQualifiedLineForm->ShowWindow(SW_HIDE);
	m_pDrawingReportLogFileForm->ShowWindow(SW_HIDE);
	m_pEmptyOutputWnd->ShowWindow(SW_HIDE);
	if(m_pLineDataEditDlg) m_pLineDataEditDlg->ShowWindow(SW_HIDE);
}

void CUserAppOutputWnd::InitContents()
{
	if(NULL == m_pUserAppDoc)
	{
		CMainFrame* pMainFrame = GetUserAppMainWnd();
		//////////////////////////////////////////////////////////////////////////
		CUserAppView* pUserAppView = (CUserAppView*)(CUserAppView::GetInstance());

		m_pUserAppDoc = pUserAppView->GetDocument();
	}

	if(m_pQualifiedLineForm)
	{
		m_pBackupDataOutputForm->OnInitialUpdate();
		m_pBackupDataOutputForm->m_pUserAppDoc = m_pUserAppDoc;
		m_pBackupDataOutputForm->InitContents();

		m_pQualifiedLineForm->OnInitialUpdate();
		m_pQualifiedLineForm->m_pUserAppDoc = m_pUserAppDoc;
		m_pQualifiedLineForm->InitContents();

		m_pEmptyOutputWnd->OnInitialUpdate();
		m_pEmptyOutputWnd->m_pUserAppDoc = m_pUserAppDoc;

		m_pLineDataEditDlg->m_pUserAppDoc = m_pUserAppDoc;
		if(m_pLineDataEditDlg->GetSafeHwnd()) m_pLineDataEditDlg->InitContents();
	}
}

void CUserAppOutputWnd::ShowDrawingReportOutputForm(const bool &show)
{
	if(NULL != m_pDrawingReportLogFileForm)
	{
		if(show)
		{
			m_pDrawingReportLogFileForm->ShowWindow(SW_SHOWNORMAL);
			
			m_pIsoEditViewLogFileForm->ShowWindow(SW_HIDE);
			m_pViewLogFileForm->ShowWindow(SW_HIDE);
			m_pBackupDataOutputForm->ShowWindow(SW_HIDE);
			m_pQualifiedLineForm->ShowWindow(SW_HIDE);
			m_pBMLogFileForm->ShowWindow(SW_HIDE);
			m_pLineDataEditDlg->ShowWindow(SW_HIDE);
			m_pEmptyOutputWnd->ShowWindow(SW_HIDE);
		}
		else
		{
			m_pDrawingReportLogFileForm->ShowWindow(SW_HIDE);
		}
	}
}

/**
**/
//void CUserAppOutputWnd::ShowGetPdsLogFileForm(const bool& show)
//{
//	if(NULL != m_pGetPdsLogFileForm)
//	{
//		if(show)
//		{
//			m_pGetPdsLogFileForm->ShowWindow(SW_SHOWNORMAL);
//
//			m_pDrawingReportLogFileForm->ShowWindow(SW_HIDE);			
//			m_pIsoEditViewLogFileForm->ShowWindow(SW_HIDE);
//			m_pViewLogFileForm->ShowWindow(SW_HIDE);
//			m_pBackupDataOutputForm->ShowWindow(SW_HIDE);
//			m_pQualifiedLineForm->ShowWindow(SW_HIDE);
//			m_pBMLogFileForm->ShowWindow(SW_HIDE);
//			m_pWritePds->ShowWindow(SW_HIDE);
//			m_pLineDataEditDlg->ShowWindow(SW_HIDE);
//		}
//		else
//		{
//			m_pGetPdsLogFileForm->ShowWindow(SW_HIDE);
//		}
//	}
//}

void CUserAppOutputWnd::ShowLineDataForm(const bool& show)
{
	if(NULL != m_pLineDataEditDlg)
	{
		if(show)
		{
			if(m_pLineDataEditDlg->GetSafeHwnd()) m_pLineDataEditDlg->ShowWindow(SW_SHOWNORMAL);
						
			m_pDrawingReportLogFileForm->ShowWindow(SW_HIDE);			
			m_pIsoEditViewLogFileForm->ShowWindow(SW_HIDE);
			m_pViewLogFileForm->ShowWindow(SW_HIDE);
			m_pBackupDataOutputForm->ShowWindow(SW_HIDE);
			m_pQualifiedLineForm->ShowWindow(SW_HIDE);
			m_pBMLogFileForm->ShowWindow(SW_HIDE);
			m_pEmptyOutputWnd->ShowWindow(SW_HIDE);
		}
		else
		{
			m_pLineDataEditDlg->ShowWindow(SW_HIDE);
		}
	}

	((CMainFrame*)AfxGetMainWnd())->RecalcLayout();
}