
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "SmartReporter.h"
#include "Resource.h"

#include "MainFrm.h"
#include "VersionNo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_OFF_2007_AQUA, &CMainFrame::OnUpdateApplicationLook)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLACK);
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

	//BOOL bNameValid;
	//// set the visual manager and style based on persisted value
	//OnApplicationLook(theApp.m_nAppLook);

	//if (!m_wndMenuBar.Create(this))
	//{
	//	TRACE0("Failed to create menubar\n");
	//	return -1;      // fail to create
	//}

	//m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	//// prevent the menu bar from taking the focus on activation
	//CMFCPopupMenu::SetForceMenuFocus(FALSE);

	//if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	//	!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	//{
	//	TRACE0("Failed to create toolbar\n");
	//	return -1;      // fail to create
	//}

	//CString strToolBarName;
	//bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	//ASSERT(bNameValid);
	//m_wndToolBar.SetWindowText(strToolBarName);

	//CString strCustomize;
	//bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	//ASSERT(bNameValid);
	//m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	//// Allow user-defined toolbars operations:
	//InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	/*
	CRect Size;
	Size.SetRect(0,0,0,63);
	if (!m_wndHeaderPicture.CreateEx(this, TBSTYLE_TRANSPARENT , WS_CHILD | WS_VISIBLE | CBRS_TOP, Size))
	{
		TRACE0("Failed to create header\n");
	}

	m_wndHeaderPicture.LoadBackgroundPicture(_T("logo.bmp"));
	//The rect is transparent, until you set the drawrect...
	m_wndHeaderPicture.SetDrawRect(Size);

	m_wndHeaderPicture.SetPlaceMent(DT_LEFT);
	m_wndHeaderPicture.SetPicturePlaceMent(DT_RIGHT);
	m_wndHeaderPicture.SetVAlign(TRUE); //Center the picture verticaly, if needed
	
	m_wndHeaderPicture.SetTextCol(RGB(0 , 0 , 0));
	m_wndHeaderPicture.SetTextShadowCol(RGB(0 , 0 , 255));
	CString sHeaderText;
	sHeaderText.Format(_T("%s %s") , _T("Line Control Manager Ver") , STRPRODUCTVER);
	sHeaderText.TrimLeft();
	sHeaderText.TrimRight();
	m_wndHeaderPicture.SetWindowText(sHeaderText);
	*/

	/*
	m_wndHeaderPicture.SetGradientStartColor(RGB(20,20,20));
	m_wndHeaderPicture.SetGradientEndColor(RGB(160,32,240));
	m_wndHeaderPicture.SetGradient(TRUE);

	
	
	m_wndHeaderPicture.SetTextCol(RGB(100,155,155));
	m_wndHeaderPicture.SetTextShadowCol(RGB(255,255,100));
	m_wndHeaderPicture.SetWindowText("Convert Line List To Oracle");
	*/

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	m_wndRibbonBar.Create(this);
	InitializeRibbon();

	// TODO: Delete these five lines if you don't want the toolbar and menubar to be dockable
	///m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	///m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	/*DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);*/

	///ShowWindow(SW_MAXIMIZE); // MDI 초기 화면시 화면을 최대로 크기 조절

	/*
	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Enable toolbar and docking window menu replacement
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// enable quick (Alt+drag) toolbar customization
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// load user-defined toolbar images
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			m_UserImages.SetImageSize(CSize(16, 16), FALSE);
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}
	*/

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	
	cs.style &= ~(FWS_PREFIXTITLE | FWS_ADDTOTITLE);

	return TRUE;
}

/******************************************************************************
    @author     humkyung
    @date       2012-05-04
    @function   InitializeRibbon
    @return     void
    @brief
******************************************************************************/
void CMainFrame::InitializeRibbon()
{
	BOOL bNameValid;

	CString strTemp , rString;
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

	/*rString.LoadString(ID_SETTING_ADMIN);
	CMFCRibbonButton* pBtnSetting = new CMFCRibbonButton(ID_SETTING_ADMIN , rString , 0, 3);
	pMainPanel->Add(pBtnSetting);

	bNameValid = strTemp.LoadString(ID_FILE_ISSUE_MANAGER);
	ASSERT(bNameValid);
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_ISSUE_MANAGER, strTemp, 0, 1));

	bNameValid = strTemp.LoadString(ID_FILE_SMART_REPORTER);
	ASSERT(bNameValid);
	pMainPanel->Add(new CMFCRibbonButton(ID_FILE_SMART_REPORTER, strTemp, 0, 11));*/

	bNameValid = strTemp.LoadString(IDS_RIBBON_EXIT);
	ASSERT(bNameValid);
	pMainPanel->AddToBottom(new CMFCRibbonMainPanelButton(ID_APP_EXIT, strTemp, 15));

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Add "SmartISO" category
	CMFCRibbonCategory* pCategoryApp = m_wndRibbonBar.AddCategory(_T("SmartReporter") , IDB_FILESMALL, IDB_FILELARGE);
	if(pCategoryApp)
	{
		/// Create "App" panel:
		CMFCRibbonPanel* pPanelApp = pCategoryApp->AddPanel(_T("App") , m_PanelImages.ExtractIcon(0));
		if(pPanelApp)
		{
			rString.LoadString(ID_APP_EXIT);
			CMFCRibbonButton* pBtnAppExit = new CMFCRibbonButton(ID_APP_EXIT , _T("Exit SmartReporter") , 0, 2);
			pPanelApp->Add(pBtnAppExit);
			
			pPanelApp->Add(new CMFCRibbonSeparator());

			rString.LoadString(ID_VIEW_ORA_DATA_REPORT);
			CMFCRibbonButton* pBtnLineData = new CMFCRibbonButton(ID_VIEW_ORA_DATA_REPORT , rString , 0, 5);
			pPanelApp->Add(pBtnLineData);
		}
		
		CMFCRibbonPanel* pPanelSetting = pCategoryApp->AddPanel(_T("Setting") , m_PanelImages.ExtractIcon(0));
		if(pPanelSetting)
		{
			rString.LoadString(ID_FILE_SETTING);
			CMFCRibbonButton* pBtnIsoProduct = new CMFCRibbonButton(ID_FILE_SETTING , rString , 0, 6);
			pPanelSetting->Add(pBtnIsoProduct);
		}
	}

	m_wndRibbonBar.AddToTabs(new CMFCRibbonButton(ID_APP_ABOUT, _T("\na"), m_PanelImages.ExtractIcon (0)));
}

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


// CMainFrame message handlers

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* scan menus */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// base class does the real work

	if (!CFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}


	// enable customization button for all user toolbars
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}

