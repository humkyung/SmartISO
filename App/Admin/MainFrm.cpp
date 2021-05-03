// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Admin.h"

#include "MainFrm.h"
#include "AdminView.h"
#include "AdminTreeView.h"
#include "AdminDocData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	//{{AFX_MSG_MAP(CMainFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_TOOLS_LINELIST_DATA , &CMainFrame::OnToolsDatabaseSetting)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_LINELIST_DATA, &CMainFrame::OnUpdateToolsDatabaseSetting)
	ON_COMMAND(ID_TOOLS_SETTING , &CMainFrame::OnToolsSetting)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_SETTING, &CMainFrame::OnUpdateToolsSetting)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_UPDATE , OnUpdateUpdate)	/// 2012.06.11 added by humkyung
	ON_COMMAND(IDC_BUTTON_LINE_LIST_IMPORT, OnImportLineList)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_LINE_LIST_IMPORT, OnUpdateImportLineList)
	ON_WM_CLOSE()	/// 2012.06.11 added by humkyung
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_UPDATE,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{	
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// set the visual manager used to draw all user interface elements
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));

	// set the visual style to be used the by the visual manager
	CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);

	m_wndRibbonBar.Create(this);
	InitializeRibbon();

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetPaneWidth(1 , 200);
	m_wndStatusBar.SetPaneTextColor(1 , RGB(255,0,0));
	m_wndStatusBar.SetPaneText(1 , _T(""));

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	EnableDocking(CBRS_ALIGN_ANY);
	
	this->SetTimer(0 , 60*60*1000 , NULL);	/// set up timer(1 hour term) - 2012.06.11 added by humkyung

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	return TRUE;
}

void CMainFrame::InitializeRibbon()
{
	BOOL bNameValid;

	CString strTemp;
	bNameValid = strTemp.LoadString(IDS_RIBBON_FILE);
	ASSERT(bNameValid);

	// Load panel images:
	m_PanelImages.SetImageSize(CSize(16, 16));
	m_PanelImages.Load(IDB_BUTTONS);

	// Init main button:
	m_MainButton.SetImage(IDB_MAIN);
	m_MainButton.SetText(_T("\nf"));
	m_MainButton.SetToolTipText(strTemp);

	m_wndRibbonBar.SetApplicationButton(&m_MainButton, CSize (45, 45));
	CMFCRibbonMainPanel* pMainPanel = m_wndRibbonBar.AddMainCategory(strTemp, IDB_FILESMALL, IDB_FILELARGE);

	/*bNameValid = strTemp.LoadString(ID_TO_USERAPP);
	ASSERT(bNameValid);
	pMainPanel->Add(new CMFCRibbonButton(ID_TO_USERAPP, strTemp, 0, 0));*/

	bNameValid = strTemp.LoadString(IDS_RIBBON_EXIT);
	ASSERT(bNameValid);
	pMainPanel->AddToBottom(new CMFCRibbonMainPanelButton(ID_APP_EXIT, strTemp, 15));

	CString rString;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Add "SmartISO" category
	CMFCRibbonCategory* pCategoryApp = m_wndRibbonBar.AddCategory(PRODUCT_NAME , IDB_FILESMALL, IDB_FILELARGE);
	if(pCategoryApp)
	{
		/// 2012.10.25 added by humkyung
		CMFCRibbonPanel* pPanelApp = pCategoryApp->AddPanel(_T("App") , m_PanelImages.ExtractIcon(0));
		if(pPanelApp)
		{
			/*rString.LoadString(ID_APP_CLOSE);
			CMFCRibbonButton* pBtnExcelExport = new CMFCRibbonButton(ID_APP_CLOSE , _T("Close") , 0, 0);
			pPanelApp->Add(pBtnExcelExport);*/

			CMFCRibbonButton* pBtnUserApp = new CMFCRibbonButton(ID_TO_USERAPP , PRODUCT_NAME , 0, 5);
			pPanelApp->Add(pBtnUserApp);
		}
		/// up to here

		CMFCRibbonPanel* pPanelSmartISO = pCategoryApp->AddPanel(_T("SmartISO") , m_PanelImages.ExtractIcon(0));
		if(pPanelSmartISO)
		{
			CMFCRibbonButton* pBtnLineData = new CMFCRibbonButton(ID_TOOLS_LINELIST_DATA , _TR("Line List"), 0, 2);
			pPanelSmartISO->Add(pBtnLineData);
			pPanelSmartISO->Add(new CMFCRibbonButton(IDC_BUTTON_LINE_LIST_IMPORT , _TR("Import"), 16));

			pPanelSmartISO->Add(new CMFCRibbonSeparator());

			rString.LoadString(ID_TOOLS_GENERAL_SETTING);
			CMFCRibbonButton* pBtnGeneralSettings = new CMFCRibbonButton(ID_TOOLS_GENERAL_SETTING , rString , 0, 3);
			pPanelSmartISO->Add(pBtnGeneralSettings);

			rString.LoadString(ID_TOOLS_SETTING);
			CMFCRibbonButton* pBtnProjectSettings = new CMFCRibbonButton(ID_TOOLS_SETTING , rString , 0, 4);
			pPanelSmartISO->Add(pBtnProjectSettings);
		}
	}

	m_wndRibbonBar.AddToTabs(new CMFCRibbonButton(ID_APP_ABOUT, _T("\na"), m_PanelImages.ExtractIcon (0)));
}
/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

/******************************************************************************
    @author     humkyung
    @date       2011-11-04
    @class      CMainFrame
    @function   OnToolsDatabaseSetting
    @return     void
    @brief
******************************************************************************/
void CMainFrame::OnToolsDatabaseSetting()
{
	CAdminDoc* pDoc = GetSDIActiveDocument();
	if(pDoc)
	{
		POSITION pos = pDoc->GetFirstViewPosition();
		while(pos)
		{
			CView* pView = pDoc->GetNextView(pos);
			if(pView->IsKindOf(RUNTIME_CLASS(CAdminView)))
			{
				CAdminView* pAdminView = static_cast<CAdminView*>(pView);
				pAdminView->ShowDefTableForm(true);
				break;
			}
		}
	}
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-04
    @class      CMainFrame
    @function   OnUpdateToolsDatabaseSetting
    @return     void
    @param      CCmdUI  *pCmdUI
    @brief
******************************************************************************/
void CMainFrame::OnUpdateToolsDatabaseSetting(CCmdUI *pCmdUI)
{
	CAdminDoc* pDoc = GetSDIActiveDocument();
	if(pDoc)
	{
		POSITION pos = pDoc->GetFirstViewPosition();
		while(pos)
		{
			CView* pView = pDoc->GetNextView(pos);
			if(pView->IsKindOf(RUNTIME_CLASS(CAdminView)))
			{
				CAdminView* pAdminView = static_cast<CAdminView*>(pView);
				pCmdUI->SetCheck(pAdminView->m_pProjectDefTableDlg->IsWindowVisible());
				break;
			}
		}
	}
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-04
    @class      CMainFrame
    @function   OnToolsSetting
    @return     void
    @brief
******************************************************************************/
void CMainFrame::OnToolsSetting()
{
	CAdminDoc* pDoc = GetSDIActiveDocument();
	if(pDoc)
	{
		POSITION pos = pDoc->GetFirstViewPosition();
		while(pos)
		{
			CView* pView = pDoc->GetNextView(pos);
			if(pView->IsKindOf(RUNTIME_CLASS(CAdminView)))
			{
				CAdminView* pAdminView = static_cast<CAdminView*>(pView);
				pAdminView->ShowSettingForm(true);
				break;
			}
		}
	}
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-04
    @class      CMainFrame
    @function   OnUpdateToolsSetting
    @return     void
    @param      CCmdUI  *pCmdUI
    @brief
******************************************************************************/
void CMainFrame::OnUpdateToolsSetting(CCmdUI *pCmdUI)
{
	CAdminDoc* pDoc = GetSDIActiveDocument();
	if(pDoc)
	{
		POSITION pos = pDoc->GetFirstViewPosition();
		while(pos)
		{
			CView* pView = pDoc->GetNextView(pos);
			if(pView->IsKindOf(RUNTIME_CLASS(CAdminView)))
			{
				CAdminView* pAdminView = static_cast<CAdminView*>(pView);
				pCmdUI->SetCheck(pAdminView->m_pProjectSettingForm->IsWindowVisible());
				break;
			}
		}
	}
}

/**
	@brief	import line list
	@author	humkyung
	@date	2015.11.05
*/
void CMainFrame::OnImportLineList() 
{
	CAdminDoc* pDoc = GetSDIActiveDocument();
	if(pDoc)
	{
		POSITION pos = pDoc->GetFirstViewPosition();
		while(pos)
		{
			CView* pView = pDoc->GetNextView(pos);
			if(pView->IsKindOf(RUNTIME_CLASS(CAdminView)))
			{
				CAdminView* pAdminView = static_cast<CAdminView*>(pView);
				pAdminView->m_pProjectDefTableDlg->OnButtonLineListImport();
				break;
			}
		}
	}
}

/******************************************************************************
    @author     humkyung
    @date       2015.10.29
    @param      CCmdUI* pCmdUI
******************************************************************************/
void CMainFrame::OnUpdateImportLineList(CCmdUI* pCmdUI) 
{
	CAdminDoc* pDoc = GetSDIActiveDocument();
	if(pDoc)
	{
		POSITION pos = pDoc->GetFirstViewPosition();
		while(pos)
		{
			CView* pView = pDoc->GetNextView(pos);
			if(pView->IsKindOf(RUNTIME_CLASS(CAdminView)))
			{
				CAdminView* pAdminView = static_cast<CAdminView*>(pView);
				pCmdUI->Enable(pAdminView->m_pProjectDefTableDlg->IsWindowVisible());
				break;
			}
		}
	}
}

/**
	@brief	

	@author	humkyung

	@date	2012.06.11
*/
void CMainFrame::OnClose()
{
	this->KillTimer(0);

	CFrameWndEx::OnClose();
}

/******************************************************************************
    @author     humkyung
    @date       2012-06-11
    @class      CMainFrame
    @function   OnUpdateUpdate
    @return     void
    @param      CCmdUI* pCmdUI
    @brief
******************************************************************************/
void CMainFrame::OnUpdateUpdate(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable();
}