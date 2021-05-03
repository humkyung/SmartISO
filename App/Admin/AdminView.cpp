// AdminView.cpp : implementation of the CAdminView class
//

#include "stdafx.h"
#include "Admin.h"
#include <Splash/SplashScreenFx.h>

#include "MainFrm.h"

#include "AdminDoc.h"
#include "AdminView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdminView

IMPLEMENT_DYNCREATE(CAdminView, CFormView)

BEGIN_MESSAGE_MAP(CAdminView, CFormView)
	//{{AFX_MSG_MAP(CAdminView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdminView construction/destruction

CAdminView::CAdminView()
	: CFormView(CAdminView::IDD)
{
	//{{AFX_DATA_INIT(CAdminView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// TODO: add construction code here
}

CAdminView::~CAdminView()
{
	try
	{
		/// don't delete form created by CRumtimeClass
		/*
		if(NULL != m_pAdminUserDlg) delete m_pAdminUserDlg;
		if(NULL != m_pAdminProjectDefTableDlg) delete m_pAdminProjectDefTableDlg;
		if(NULL != m_pProjectAccessDlg) delete m_pProjectAccessDlg;
		*/
		///
	}
	catch(...)
	{

	}
}

void CAdminView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdminView)
	//}}AFX_DATA_MAP
}

BOOL CAdminView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CAdminView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
}

/////////////////////////////////////////////////////////////////////////////
// CAdminView printing

BOOL CAdminView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CAdminView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CAdminView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CAdminView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CAdminView diagnostics

#ifdef _DEBUG
void CAdminView::AssertValid() const
{
	CFormView::AssertValid();
}

void CAdminView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CAdminDoc* CAdminView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAdminDoc)));
	return (CAdminDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAdminView message handlers



int CAdminView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	/*CRuntimeClass* pLineListDataImportClass = RUNTIME_CLASS(CLineListDataImportDlg);
	m_pLineListDataImportDlg = (CLineListDataImportDlg*)pLineListDataImportClass->CreateObject();
	if(NULL != m_pLineListDataImportDlg)
	{
		CCreateContext context;
		context.m_pNewViewClass = pLineListDataImportClass;
		context.m_pCurrentDoc = GetDocument();
		if(!m_pLineListDataImportDlg->Create(NULL , NULL , WS_CHILD | WS_BORDER , CRect(0 , 0 , 100 , 100) , this , CLineListDataImportDlg::IDD , &context))
		{
			delete m_pLineListDataImportDlg;
			return -1;
		}
	}*/

	CRuntimeClass* pProjectDefTableClass = RUNTIME_CLASS(CProjectDefTableDlg);
	m_pProjectDefTableDlg = (CProjectDefTableDlg*)pProjectDefTableClass->CreateObject();
	if(NULL != m_pProjectDefTableDlg)
	{
		CCreateContext context;
		context.m_pNewViewClass = pProjectDefTableClass;
		context.m_pCurrentDoc = GetDocument();
		if(!m_pProjectDefTableDlg->Create(NULL , NULL , WS_CHILD | WS_BORDER | WS_VISIBLE, CRect(0 , 0 , 100 , 100) , this , CProjectDefTableDlg::IDD , &context))
		{
			delete m_pProjectDefTableDlg;
			return -1;
		}
	}

	CRuntimeClass* pProjectSettingClass = RUNTIME_CLASS(CProjectSettingForm);
	m_pProjectSettingForm = (CProjectSettingForm*)pProjectSettingClass->CreateObject();
	if(NULL != m_pProjectSettingForm)
	{
		CCreateContext context;
		context.m_pNewViewClass = pProjectSettingClass;
		context.m_pCurrentDoc = GetDocument();
		if(!m_pProjectSettingForm->Create(NULL , NULL , WS_CHILD | WS_BORDER , CRect(0 , 0 , 0 , 0) , this , AFX_IDW_PANE_FIRST , &context))
		{
			delete m_pProjectSettingForm;
			return -1;
		}
	}
	
	return 0;
}

void CAdminView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	if(m_pProjectDefTableDlg->GetSafeHwnd())
	{
		m_pProjectDefTableDlg->MoveWindow(CRect(0 , 0 , cx , cy));
		m_pProjectSettingForm->SetWindowPos(NULL , 0 , 0 , cx , cy , SWP_NOZORDER);
	}
}

void CAdminView::OnDestroy() 
{
	CFormView::OnDestroy();
	
	if(NULL != m_pProjectDefTableDlg)
	{
		m_pProjectDefTableDlg->DestroyWindow();
	}
}

/******************************************************************************
    @author     humkyung
    @date       ????-??-??
    @class      CAdminView
    @function   ShowDefTableForm
    @return     void
    @param      const   bool
    @param      &show
    @brief
******************************************************************************/
void CAdminView::ShowDefTableForm(const bool &show)
{
	if(NULL != m_pProjectDefTableDlg)
	{
		if(show)
		{
			m_pProjectDefTableDlg->ShowWindow(SW_SHOWNORMAL);
			m_pProjectDefTableDlg->SetFocus();
			
			m_pProjectSettingForm->ShowWindow(SW_HIDE);
		}
		else
		{
			m_pProjectDefTableDlg->ShowWindow(SW_HIDE);
		}
	}
}

void CAdminView::ShowSettingForm(const bool &show)
{
	if(NULL != m_pProjectSettingForm)
	{
		if(show)
		{
			CAdminDoc* pDoc = GetDocument();

			if((RESERVED_DATA_TABLE_MODIFIED == pDoc->m_modifyFlag) || (REVISION_DATA_TABLE_MODIFIED == pDoc->m_modifyFlag))
			{
				m_pProjectSettingForm->m_BorderSettingDlg.UpdateContents();
				pDoc->m_modifyFlag &= (~RESERVED_DATA_TABLE_MODIFIED) & (~REVISION_DATA_TABLE_MODIFIED);
			}

			m_pProjectSettingForm->ShowWindow(SW_SHOWNORMAL);
			
			m_pProjectDefTableDlg->ShowWindow(SW_HIDE);
			m_pProjectSettingForm->UpdateContents();
		}
		else
		{
			m_pProjectSettingForm->ShowWindow(SW_HIDE);
		}
	}
}

/**	
	@brief	

	@author	HumKyung.BAEK

	@date	2010.03.08

	@return	void	
*/
//void CAdminView::ShowLineListDataImportForm(const bool &show)
//{
//	if(NULL != m_pLineListDataImportDlg)
//	{
//		if(show)
//		{
//			m_pLineListDataImportDlg->ShowWindow(SW_SHOWNORMAL);
//			m_pLineListDataImportDlg->SetFocus();
//			
//			m_pAdminProjectDefTableDlg->ShowWindow(SW_HIDE);
//			m_pProjectSettingForm->ShowWindow(SW_HIDE);
//		}
//		else
//		{
//			m_pLineListDataImportDlg->ShowWindow(SW_HIDE);
//		}
//	}
//}

/**	
	@brief	자식 윈도우의 UpdateContents 함수를 호출한다.
	@author	HumKyung.BAEK
	@return	void	
*/
void CAdminView::UpdateContents()
{
	CSplashScreenFx* pSplash = CSplashScreenFx::GetInstance();
	if(pSplash) pSplash->m_ctrlProgress.SetPos(0);

	if(m_pProjectDefTableDlg)	m_pProjectDefTableDlg->UpdateContents();
	if(pSplash) pSplash->m_ctrlProgress.SetPos(30);

	if(m_pProjectSettingForm)	m_pProjectSettingForm->UpdateContents();
	if(pSplash) pSplash->m_ctrlProgress.SetPos(100);
}
