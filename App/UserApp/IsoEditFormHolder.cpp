// IsoEditFormHolder.cpp : implementation file
//

#include "stdafx.h"
#include "UserApp.h"
#include <FileTools.h>
#include "UserAppDocData.h"
#include "IsoEditFormHolder.h"
#include "IsoEditForm.h"

// CIsoEditFormHolder

IMPLEMENT_DYNCREATE(CIsoEditFormHolder, CFormView)

CIsoEditFormHolder::CIsoEditFormHolder()
	: CFormView(CIsoEditFormHolder::IDD)
{
	m_pMyFrame = NULL;
	m_pViewLogFileForm = NULL;
}

CIsoEditFormHolder::~CIsoEditFormHolder()
{
	try
	{
	}
	catch(...)
	{
	}
}

void CIsoEditFormHolder::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CIsoEditFormHolder, CFormView)
	ON_WM_SIZE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CIsoEditFormHolder diagnostics

#ifdef _DEBUG
void CIsoEditFormHolder::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CIsoEditFormHolder::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CIsoEditFormHolder message handlers
BOOL CIsoEditFormHolder::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

/**
	@brief	set view log form

	@author	humkyung

	@date	2013.02.08
*/
void CIsoEditFormHolder::SetViewLogFileForm(CIsoEditViewLogFileForm *p)
{
	assert(p && "Null Parameter");

	if(p)
	{
		CIsoEditForm* pEditForm = static_cast<CIsoEditForm*>(m_wndSplitter.GetPane(0 , 0));
		if(NULL != pEditForm)
		{
			pEditForm->SetViewLogFileForm(p);
		}
	}
}

/**
	@brief	call edit form's InitContents func
	@author	humkyung
	@date	2013.02.08
*/
void CIsoEditFormHolder::InitContents()
{
	CIsoEditForm* pEditForm = static_cast<CIsoEditForm*>(m_wndSplitter.GetPane(0 , 0));
	if(NULL != pEditForm)
	{
		pEditForm->InitContents();
	}
}

/**
	@brief	return iso edit form
	@author	humkyung
	@date	2015.09.22
*/
CIsoEditForm* CIsoEditFormHolder::GetIsoEditForm()
{
	return static_cast<CIsoEditForm*>(m_wndSplitter.GetPane(0 , 0));
}

/**
	@brief	generate iso product
	@author	humkyung
	@date	2015.09.22
*/
int CIsoEditFormHolder::Generate()
{
	CIsoEditForm* pEditForm = static_cast<CIsoEditForm*>(m_wndSplitter.GetPane(0 , 0));
	//if(NULL != pEditForm) pEditForm->Generate();

	return ERROR_SUCCESS;
}

#include "LineDataEditDlg.h"
void CIsoEditFormHolder::OnInitialUpdate()
{
	if(NULL == m_wndSplitter.GetSafeHwnd())
	{
		CCreateContext ccc;
		ccc.m_pNewViewClass = RUNTIME_CLASS(CIsoEditForm);
		ccc.m_pCurrentDoc = this->GetDocument();
		ccc.m_pNewDocTemplate = NULL;
		ccc.m_pLastView = NULL;
		ccc.m_pCurrentFrame = NULL;

		// Because the CFRameWnd needs a window class, we will create
		// a new one. I just copied the sample from MSDN Help.
		// When using it in your project, you may keep CS_VREDRAW and
		// CS_HREDRAW and then throw the other three parameters.
		CString strMyClass = AfxRegisterWndClass(CS_VREDRAW |
			CS_HREDRAW,
			::LoadCursor(NULL, IDC_ARROW),
			(HBRUSH) ::GetStockObject(WHITE_BRUSH),
			::LoadIcon(NULL, IDI_APPLICATION));

		/// Create the frame window with "this" as the parent
		m_pMyFrame = new CFrameWnd;
		m_pMyFrame->Create(strMyClass , _T("") , WS_CHILD, CRect(0,0,1,1), this);
		m_pMyFrame->ShowWindow(SW_SHOW);

		m_wndSplitter.CreateStatic( m_pMyFrame , 1 , 2 ) ;
		m_wndSplitter.CreateView( 0 , 0 , RUNTIME_CLASS(CIsoEditForm) , CSize(800 , 100) , &ccc ) ;
		m_wndSplitter.CreateView( 0 , 1 , RUNTIME_CLASS(CMFCTabCtrl) , CSize(300 , 100) , &ccc ) ;
		CMFCTabCtrl* pTabCtrl = static_cast<CMFCTabCtrl*>(m_wndSplitter.GetPane(0 , 1));
		if(NULL != pTabCtrl)
		{
			CRect rectEmpty;
			rectEmpty.SetRectEmpty();

			if(TRUE == m_oDgnCanvas.Create(TRUE, &rectEmpty, pTabCtrl))
			{
				m_oDgnCanvas.SetScene(&m_oDgnScene);
						
				CIsVolume vol = m_oDgnScene.GetVolume();
				m_oDgnCanvas.m_dTrans[2] = (vol.maxx() - vol.minx())/200.f;
				m_oDgnCanvas.m_dTrans[0] = -(m_oDgnScene.m_ptCenter.x());
				m_oDgnCanvas.m_dTrans[1] = -(m_oDgnScene.m_ptCenter.y());

				pTabCtrl->AddTab(&m_oDgnCanvas ,_T("DgnCanvas"));

				CString sAppPath = CAppDocData::GetExecPath();
				m_oDgnScene.LoadGLFont(sAppPath + _T("\\fonts\\50.vft"));
			}
		}
	}

	CFormView::OnInitialUpdate();
}

void CIsoEditFormHolder::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	if((NULL != m_pMyFrame) && (m_pMyFrame->GetSafeHwnd()))
	{
		m_pMyFrame->MoveWindow(0,0,cx,cy);
	}
}

void CIsoEditFormHolder::OnDestroy()
{
	if(NULL != m_pMyFrame)
	{
		//if(m_pMyFrame->GetSafeHwnd()) m_pMyFrame->DestroyWindow();
		SAFE_DELETE(m_pMyFrame);
	}

	CFormView::OnDestroy();
}
