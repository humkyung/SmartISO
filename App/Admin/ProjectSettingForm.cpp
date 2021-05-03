// ProjectSettingForm.cpp : implementation file
//

#include "stdafx.h"
#include "Admin.h"
#include "AdminView.h"
#include "ProjectSettingForm.h"
#include "AdminDocData.h"
#include <ProjectData.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProjectSettingForm

IMPLEMENT_DYNCREATE(CProjectSettingForm, CFormView)

CProjectSettingForm::CProjectSettingForm()
	: CFormView(CProjectSettingForm::IDD)
{
	//{{AFX_DATA_INIT(CProjectSettingForm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CProjectSettingForm::~CProjectSettingForm()
{
}

void CProjectSettingForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

	DDX_Control(pDX,IDC_BUTTON_SAVE,m_btnSave);
}


BEGIN_MESSAGE_MAP(CProjectSettingForm, CFormView)
	//{{AFX_MSG_MAP(CProjectSettingForm)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	//}}AFX_MSG_MAP
	ON_WM_HELPINFO()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProjectSettingForm diagnostics

#ifdef _DEBUG
void CProjectSettingForm::AssertValid() const
{
	CFormView::AssertValid();
}

void CProjectSettingForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CProjectSettingForm message handlers

/**	
	@brief	폼 초기화.
	@author	humkyung
	@return	void	
*/
void CProjectSettingForm::OnInitialUpdate() 
{
	static bool __init__ = false;
	CFormView::OnInitialUpdate();
	
	if(false == __init__)
	{
		CAdminApp* pApp = (CAdminApp*)AfxGetApp();
		CResourceSwitch resSwitch(pApp->m_hMultiLangRes);

		CRect rectDummy;
		rectDummy.SetRectEmpty ();
		GetClientRect(rectDummy);
		
		m_ImagList.Create(IDB_SETTING , 16 , 16 , RGB(255 , 0 , 0));
		// Create tab window:
		if (!m_TabCtrl.Create (CMFCTabCtrl::STYLE_3D_VS2005 , rectDummy, this, 0x100 , CMFCTabCtrl::LOCATION_TOP))
		{
			TRACE0("Failed to create workspace tab window\n");
			return;      // fail to create
		}
		m_TabCtrl.SetImageList(m_ImagList);
		m_TabCtrl.SetActiveTabTextColor(RGB(0 , 0 , 255));

		CAdminView* pAdminView = (CAdminView*)GetParent();
		CAdminDoc*  pAdminDoc  = pAdminView->GetDocument();
		
		m_BorderSettingDlg.m_pAdminDoc = pAdminDoc;
		if(TRUE == m_BorderSettingDlg.Create(CBorderSettingDlg::IDD , &m_TabCtrl))
		{
			////m_BorderSettingDlg.ShowWindow(SW_HIDE);
			m_TabCtrl.AddTab(&m_BorderSettingDlg ,  _TR("Border Setting") , 1 /* Image number */);
		}
		
		if(TRUE == m_LineDataSettingDlg.Create(CLineDataSettingDlg::IDD , &m_TabCtrl))
		{
			m_LineDataSettingDlg.m_pAdminDoc = pAdminDoc;
			m_TabCtrl.AddTab(&m_LineDataSettingDlg ,  _TR("Line Data Setting") , 2 /* Image number */);
		}

		m_IsoEditSettingDlg.m_pAdminDoc = pAdminDoc;
		if(TRUE == m_IsoEditSettingDlg.Create(CIsoEditSettingDlg::IDD , &m_TabCtrl))
		{
			m_TabCtrl.AddTab(&m_IsoEditSettingDlg,  _TR("Product Iso Setting") , 3 /* Image number */);
		}

		/// add smart iso diff setting dialog - 2012.01.03 added by humkyung
		m_dlgSmartISODiffSetting.m_pAdminDoc = pAdminDoc;
		if(TRUE == m_dlgSmartISODiffSetting.Create(CSmartISODiffSettingDlg::IDD , &m_TabCtrl))
		{
			m_TabCtrl.AddTab(&m_dlgSmartISODiffSetting,  _TR("Rev Automation") , 6 /* Image number */);
		}
		/// up to here

		m_DrawingRepotrSettingDlg.m_pAdminDoc = pAdminDoc;
		if(TRUE == m_DrawingRepotrSettingDlg.Create(CDrawingReportSettingDlg::IDD, &m_TabCtrl))
		{
			m_TabCtrl.AddTab(&m_DrawingRepotrSettingDlg, _TR("Drawing Report Setting") , 4 /* image number */);
		}
		
		m_dlgOracleSetting.m_pAdminDoc = pAdminDoc;
		if(TRUE == m_dlgOracleSetting.Create(COracleSettingDlg::IDD, &m_TabCtrl))
		{
			m_TabCtrl.AddTab(&m_dlgOracleSetting , _TR("Oracle Loading Setting") , 5 /*Image Number */);
		}

		if(TRUE == m_RefDataSettingDlg.Create(CRefDataSettingDlg::IDD , &m_TabCtrl))
		{
			m_TabCtrl.AddTab(&m_RefDataSettingDlg , _TR("Ref Data Setting") , 6);
		}

		CAdminDocData* pDocData = CAdminDocData::GetInstance();
		
		m_Font.CreateFont(18,0,0,0 ,FW_BOLD,FALSE,FALSE,0,0,0,0,0,0, _T("Arial"));

		HICON hIcon = (HICON) LoadImage(AfxGetInstanceHandle(),
								MAKEINTRESOURCE(IDI_SAVE),
								IMAGE_ICON, 16, 16, 0);
		HICON hOldIcon = m_btnSave.SetIcon(hIcon);
		if(hOldIcon) DestroyIcon(hOldIcon);
		
		__init__ = true;
	}
}

void CProjectSettingForm::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	CRect rect;

	CWnd* pWnd = GetDlgItem(IDC_BUTTON_SAVE);
	if(NULL != pWnd)
	{
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);

		pWnd->SetWindowPos(NULL , cx - rect.Width() , cy - rect.Height() , 0 , 0 , SWP_NOSIZE | SWP_NOZORDER);
	}

	if(m_TabCtrl.m_hWnd)
	{
		m_TabCtrl.SetWindowPos(NULL , 0 , 0 , cx , cy - rect.Height() - 10 , SWP_NOZORDER);
	}
}

BOOL CProjectSettingForm::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	CAdminApp* pApp = (CAdminApp*)AfxGetApp();
	CResourceSwitch resSwitch(pApp->m_hMultiLangRes);

	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

/**	
	@brief	내부 폼들의 UpdateContents 함수를 호출한다.
	
	@author	humkyung

	@return	void	
*/
void CProjectSettingForm::UpdateContents()
{
	if(m_BorderSettingDlg.GetSafeHwnd())
	{
		m_BorderSettingDlg.UpdateContents();
	}
	if(m_LineDataSettingDlg.GetSafeHwnd()) m_LineDataSettingDlg.UpdateContents();
	if(m_IsoEditSettingDlg.GetSafeHwnd()) m_IsoEditSettingDlg.UpdateContents();
	if(m_dlgSmartISODiffSetting.GetSafeHwnd()) m_dlgSmartISODiffSetting.UpdateContents();
	if(m_DrawingRepotrSettingDlg.GetSafeHwnd()) m_DrawingRepotrSettingDlg.UpdateContents();
	if(m_dlgOracleSetting.GetSafeHwnd()) m_dlgOracleSetting.UpdateContents();
	if(m_RefDataSettingDlg.GetSafeHwnd()) m_RefDataSettingDlg.UpdateContents();

	const int iTab = m_TabCtrl.GetActiveTab();
	CWnd* pWnd = m_TabCtrl.GetTabWnd(iTab);
	if(pWnd) pWnd->SetFocus();
}

/**	
	@brief	사용자가 설정한 정보들을 환경 설정 파일로 저장한다.
	@author	humkyung
	@return	void	
*/
void CProjectSettingForm::OnButtonSave() 
{
	m_btnSave.EnableWindow(FALSE);
	{
		CProjectData* pPrjData = CProjectData::GetInstance();
		/// 해당 폼이 활성화 되지 않았을 경우 윈도우가 생성되지 않으니 Save 함수를 호출하기 전에 윈도우가 생성되었는지 확인한다.
		if( m_BorderSettingDlg.GetSafeHwnd())  m_BorderSettingDlg.Save();
		if( m_LineDataSettingDlg.GetSafeHwnd())m_LineDataSettingDlg.Save();
		if(m_IsoEditSettingDlg.GetSafeHwnd())  m_IsoEditSettingDlg.Save();
		if(m_dlgSmartISODiffSetting.GetSafeHwnd()) m_dlgSmartISODiffSetting.Save();
		if(m_DrawingRepotrSettingDlg.GetSafeHwnd()) m_DrawingRepotrSettingDlg.Save();
		if(m_dlgOracleSetting.GetSafeHwnd()) m_dlgOracleSetting.Save();
		if(m_RefDataSettingDlg.GetSafeHwnd()) m_RefDataSettingDlg.Save();

		/// save project setting data - 2011.09.22 added by humkyung
		CAdminDocData::GetInstance()->SaveSettingData();
		/// up to here
	}
	m_btnSave.EnableWindow(TRUE);

	AfxMessageBox( _T("Save Done.") );
}

/******************************************************************************
    @author     humkyung
    @date       2012-06-05
    @class      CProjectSettingForm
    @function   OnHelpInfo
    @return     BOOL
    @brief
******************************************************************************/
BOOL CProjectSettingForm::OnHelpInfo(HELPINFO* pHelpInfo)
{
	const int iTab = m_TabCtrl.GetActiveTab();

	return TRUE;///CFormView::OnHelpInfo(pHelpInfo);
}
