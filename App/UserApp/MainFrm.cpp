// MainFrm.cpp : implementation of the CMainFrame class
//

#include "StdAfx.h"
#include "UserApp.h"

#include "MainFrm.h"
#include "UserAppView.h"
#include <ProjectData.h>
#include <util/FileTools.h>

#include <AutoUp/AutoUp/AutoUpInf.h>

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
	ON_WM_CREATE()
	ON_COMMAND(ID_LINE_LIST_CONVERTER, OnIssueManager)
	ON_COMMAND(ID_LINE_DATA, OnLineData)
	ON_COMMAND(IDS_PDS , OnInsertToPdsAttr)
	ON_COMMAND(ID_ISO_PRODUCT, OnIsoProduct)
	ON_COMMAND(ID_CHECK_REV_AUTOMATION , &CMainFrame::OnIsoRevAutomation)
	ON_COMMAND(ID_CHECK_ISSUE_AUTOMATION , &CMainFrame::OnIsoIssueAutomation)
	ON_COMMAND(ID_DRAWING_REPORT, OnDrawingReport)
	ON_COMMAND(ID_BM_REPORT, OnBOMReport)
	ON_COMMAND(ID_DRAWING_PLOTTING, OnDrawingPlotting)
	ON_COMMAND(ID_FILE_BACKUP, OnFileBackup)
	ON_UPDATE_COMMAND_UI(ID_LINE_LIST_CONVERTER, OnUpdateIssueManager)	/// 2011.02.10 added by HumKyung
	ON_UPDATE_COMMAND_UI(ID_BM_REPORT, OnUpdateBmReport)
	ON_UPDATE_COMMAND_UI(ID_LINE_DATA, OnUpdateLineData)
	ON_UPDATE_COMMAND_UI(IDS_PDS , OnUpdateInsertToPdsAttr)
	ON_UPDATE_COMMAND_UI(ID_ISO_PRODUCT, OnUpdateIsoProduct)
	ON_UPDATE_COMMAND_UI(ID_CHECK_REV_AUTOMATION, OnUpdateIsoRevAutomation)
	ON_UPDATE_COMMAND_UI(ID_CHECK_ISSUE_AUTOMATION, OnUpdateIsoIssueAutomation)
	ON_UPDATE_COMMAND_UI(ID_LINE_LIST_CONVERTER, OnUpdateLineListConverter)
	ON_UPDATE_COMMAND_UI(ID_DRAWING_REPORT, OnUpdateDrawingReport)
	ON_UPDATE_COMMAND_UI(ID_DRAWING_PLOTTING, OnUpdateDrawingPlotting)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_UPDATE , OnUpdateUpdate)	/// 2012.06.11 added by humkyung
	ON_COMMAND(IDC_CHECK_IPLOT , &CMainFrame::OnUseIPLOT)
	ON_UPDATE_COMMAND_UI(IDC_CHECK_IPLOT, OnUpdateCheckIPLOT)
	ON_UPDATE_COMMAND_UI(IDC_COMBO_PRINTER_LIST , OnUpdateComboPrinterList)
	ON_COMMAND(IDC_BUTTON_GENERATE_ISO_PRODUCT , OnGenerateIsoProduct)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_GENERATE_ISO_PRODUCT , OnUpdateGenerateIsoProduct)
	ON_COMMAND(IDC_BUTTON_GENERATE_DRAWING_REPORT , OnGenerateDrawingReport)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_GENERATE_DRAWING_REPORT, OnUpdateGenerateDrawingReport)
	ON_COMMAND(IDC_BUTTON_GENERATE_BM , OnGenerateBM)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_GENERATE_BM , OnUpdateGenerateBM)
	ON_COMMAND(IDC_BUTTON_PRINT_DRAWING , OnPrintDrawing)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_PRINT_DRAWING, OnUpdatePrintDrawing)
	ON_COMMAND(IDC_BUTTON_UPDATE_ORACLE, OnUpdateOracle)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_UPDATE_ORACLE, OnUpdateUpdateOracle)

	/// 2012.08.26 added by humkyung
	ON_COMMAND(ID_SHOW_WORKSPACEBAR, OnShowWorkspacebar)
	ON_UPDATE_COMMAND_UI(ID_SHOW_WORKSPACEBAR , OnUpdateShowWorkspacebar)
	ON_COMMAND(ID_SHOW_OUTPUTBAR, OnShowOutputbar)
	ON_UPDATE_COMMAND_UI(ID_SHOW_OUTPUTBAR , OnUpdateShowOutputbar)
	/// up to here
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
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
static CMainFrame* __MAINFRAME__ = NULL;
CMainFrame* GetUserAppMainWnd()
{
	return __MAINFRAME__;
}

CMainFrame::CMainFrame()
{
	__MAINFRAME__ = this;
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

	m_wndRibbonBar.Create(this);
	InitializeRibbon();

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	if (!m_wndWorkspaceBar.Create(_T("WorkspaceBar"), this, CRect(0, 0, CLineListSelectOption1::WIDTH , 640), TRUE, 0x1010/*ID_VIEW_OUTPUTWND*/, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("WorkspaceBar을 만들지 못했습니다.\n");
		return FALSE;
	}

	// 출력 창을 만듭니다.
	if (!m_wndOutputBar.Create(_T("OutputBar"), this, CRect(0, 0, 640, 800), TRUE, 0x1011/*ID_VIEW_OUTPUTWND*/, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("출력 창을 만들지 못했습니다.\n");
		return FALSE;
	}

	m_wndWorkspaceBar.EnableDocking(CBRS_ALIGN_LEFT);
	m_wndOutputBar.EnableDocking(CBRS_ALIGN_BOTTOM);
	EnableDocking(CBRS_ALIGN_ANY);

	DockPane(&m_wndWorkspaceBar);
	DockPane(&m_wndOutputBar);

	/// Create properties window
	CString strPropertiesWnd;
	if (!m_wndProperties.Create(_T("Properties"), this, CRect(0, 0, 200, 200), TRUE, 0x1012 , WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_RIGHT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Properties window\n");
		return FALSE; /// failed to create
	}
	m_wndProperties.EnableDocking(CBRS_ALIGN_RIGHT);
	DockPane(&m_wndProperties);
	m_wndProperties.ShowPane(FALSE , FALSE , TRUE);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~(LONG) FWS_ADDTOTITLE;

	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/******************************************************************************
    @author     humkyung
    @date       2012-04-04
    @class      CMainFrame
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
	{
		/// 2012.08.26 added by humkyung
		bNameValid = strTemp.LoadString(ID_SHOW_WORKSPACEBAR);
		ASSERT(bNameValid);
		pMainPanel->Add(new CMFCRibbonButton(ID_SHOW_WORKSPACEBAR, strTemp, 16 , 13));
		bNameValid = strTemp.LoadString(ID_SHOW_OUTPUTBAR);
		ASSERT(bNameValid);
		pMainPanel->Add(new CMFCRibbonButton(ID_SHOW_OUTPUTBAR, strTemp, 16 , 13));
		/// up to here

		pMainPanel->Add(new CMFCRibbonSeparator());

		bNameValid = strTemp.LoadString(IDS_RIBBON_EXIT);
		ASSERT(bNameValid);
		pMainPanel->AddToBottom(new CMFCRibbonMainPanelButton(ID_APP_EXIT, strTemp, 15));
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Add "SmartISO" category
	CMFCRibbonCategory* pCategoryApp = m_wndRibbonBar.AddCategory(PRODUCT_NAME , IDB_FILESMALL, IDB_FILELARGE);
	if(pCategoryApp)
	{
		/// Create "App" panel:
		CMFCRibbonPanel* pPanelApp = pCategoryApp->AddPanel(_T("App") , m_PanelImages.ExtractIcon(0));
		if(pPanelApp)
		{
			rString.LoadString(ID_SETTING_ADMIN);
			CMFCRibbonButton* pBtnSetting = new CMFCRibbonButton(ID_SETTING_ADMIN , _TR("Setting") , 0, 3);
			pPanelApp->Add(pBtnSetting);

			pPanelApp->Add(new CMFCRibbonSeparator());

			/*bNameValid = strTemp.LoadString(ID_OPTION_SETTING);
			ASSERT(bNameValid);
			pPanelApp->Add(new CMFCRibbonButton(ID_OPTION_SETTING , strTemp, 1, 0));*/

			rString.LoadString(ID_LINE_LIST_CONVERTER);
			CMFCRibbonButton* pBtnBackup = new CMFCRibbonButton(ID_LINE_LIST_CONVERTER , _TR("Issue Manager") , 0, 4);
			pPanelApp->Add(pBtnBackup);
		}

		CMFCRibbonPanel* pPanelSmartISO = pCategoryApp->AddPanel(PRODUCT_NAME , m_PanelImages.ExtractIcon(0));
		if(pPanelSmartISO)
		{
			rString.LoadString(ID_LINE_DATA);
			CMFCRibbonButton* pBtnLineData = new CMFCRibbonButton(ID_LINE_DATA , _TR("edit line data"), 0, 5);
			pPanelSmartISO->Add(pBtnLineData);

			pPanelSmartISO->Add(new CMFCRibbonSeparator());

			rString.LoadString(ID_ISO_PRODUCT);
			CMFCRibbonButton* pBtnIsoProduct = new CMFCRibbonButton(ID_ISO_PRODUCT , _TR("iso product"), 0, 6);
			pPanelSmartISO->Add(pBtnIsoProduct);
			pPanelSmartISO->Add(new CMFCRibbonCheckBox(ID_CHECK_REV_AUTOMATION , _TR("Rev Automation")));
			pPanelSmartISO->Add(new CMFCRibbonButton(IDC_BUTTON_GENERATE_ISO_PRODUCT , _TR("Generate"), 16));

			pPanelSmartISO->Add(new CMFCRibbonSeparator());

			rString.LoadString(ID_DRAWING_REPORT);
			CMFCRibbonButton* pBtnDrawingReport = new CMFCRibbonButton(ID_DRAWING_REPORT , _TR("drawing report") , 0, 7);
			pPanelSmartISO->Add(pBtnDrawingReport);
			pPanelSmartISO->Add(new CMFCRibbonButton(IDC_BUTTON_GENERATE_DRAWING_REPORT , _TR("Generate"), 16));

			rString.LoadString(ID_BM_REPORT);
			CMFCRibbonButton* pBtnBMReport = new CMFCRibbonButton(ID_BM_REPORT , _TR("b/m report") , 0, 8);
			pPanelSmartISO->Add(pBtnBMReport);

			pPanelSmartISO->Add(new CMFCRibbonButton(IDC_BUTTON_GENERATE_BM , _TR("Generate") , 16));
		}

		CMFCRibbonPanel* pPanelUtility = pCategoryApp->AddPanel(_TR("Utility") , m_PanelImages.ExtractIcon(0));
		if(pPanelUtility)
		{
			rString.LoadString(ID_DRAWING_PLOTTING);
			CMFCRibbonButton* pBtnPlotting = new CMFCRibbonButton(ID_DRAWING_PLOTTING , _TR("drawing plotting") , 0, 9);
			pPanelUtility->Add(pBtnPlotting);

			pPanelUtility->Add(new CMFCRibbonCheckBox(IDC_CHECK_IPLOT , _TR("IPLOT")));
			CMFCRibbonComboBox *pComboPrinter = new CMFCRibbonComboBox(IDC_COMBO_PRINTER_LIST, FALSE, -1, 0, -1);
			{
				CUserAppApp *pApp = static_cast<CUserAppApp*>(AfxGetApp()) ;
				pApp->ReadPrinterList();
				for (int i = 0 ; i < pApp->m_PrinterControl.GetPrinterCount() ; ++i)
				{
					pComboPrinter->AddItem(pApp->m_PrinterControl.GetPrinterName(i)) ;
				}
				pComboPrinter->SelectItem(0);

				// Add combo button to "Favorites" panel:
				// CMFCRibbonPanel* pPanelFavorites
				pPanelUtility->Add(pComboPrinter);
			}
			pPanelUtility->Add(new CMFCRibbonButton(IDC_BUTTON_PRINT_DRAWING , _TR("Print") , 16));

			pPanelUtility->Add(new CMFCRibbonSeparator());

			CMFCRibbonButton* pBtnPDS = new CMFCRibbonButton(IDS_PDS,_TR("Update PDS database"),0,10);
			pPanelUtility->Add(pBtnPDS);
			pPanelUtility->Add(new CMFCRibbonButton(IDC_BUTTON_UPDATE_ORACLE, _TR("Update") , 16));
		}
	}

	/// 빠른 실행 도구 모음 명령을 추가합니다. - 2012.08.26 added by humkyung
	CList<UINT, UINT> lstQATCmds;
	lstQATCmds.AddTail(ID_SHOW_WORKSPACEBAR);
	lstQATCmds.AddTail(ID_SHOW_OUTPUTBAR);	
	m_wndRibbonBar.SetQuickAccessCommands(lstQATCmds);
	/// up to here

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

/**
	@brief
	@author	humkyung
*/
void CMainFrame::OnIssueManager() 
{
	CUserAppView* pUserAppView = (CUserAppView*)(CUserAppView::GetInstance());
	if(pUserAppView) pUserAppView->ShowBackupDataForm(true);
	
	m_wndWorkspaceBar.ShowBackupForm(true);
	m_wndOutputBar.ShowBackupDataOutputForm(true);
}

/**
	@brief
	@author	HumKyung
	@date	????.??.??
	@param
	@return
*/
void CMainFrame::OnLineData() 
{
	CUserAppView* pUserAppView = (CUserAppView*)(CUserAppView::GetInstance());
	if(pUserAppView) pUserAppView->ShowLineDataForm(true);

	m_wndWorkspaceBar.ShowLineDataForm(true);
	m_wndOutputBar.ShowLineDataForm(true);
}

/**
	@brief

	@author	humkyung
*/
void CMainFrame::OnIsoProduct() 
{
	CUserAppView* pUserAppView = (CUserAppView*)(CUserAppView::GetInstance());
	if(pUserAppView) pUserAppView->ShowIsoEditForm(true);

	m_wndWorkspaceBar.ShowIsoEditForm(true);
	m_wndOutputBar.ShowIsoEditViewLogFileForm(true);
}

/******************************************************************************
    @author     humkyung
    @date       2012-04-06
    @class      CMainFrame
    @function   OnIsoRevAutomation
    @return     void
    @brief
******************************************************************************/
void CMainFrame::OnIsoRevAutomation()
{
	CUserAppDoc* pDoc = CUserAppDoc::GetInstance();
	if(NULL != pDoc)
	{
		pDoc->m_bRevAutomation = !pDoc->m_bRevAutomation;
	}
}

/******************************************************************************
    @author     humkyung
    @date       2012-05-07
    @function   OnIsoIssueAutomation
    @return     void
    @brief
******************************************************************************/
void CMainFrame::OnIsoIssueAutomation()
{
	CUserAppDoc* pDoc = CUserAppDoc::GetInstance();
	if(NULL != pDoc)
	{
		pDoc->m_bIssueAutomation = !pDoc->m_bIssueAutomation;
	}
}

/**
	@brief

	@author	humkyung
*/
void CMainFrame::OnDrawingReport() 
{
	CUserAppView* pUserAppView = (CUserAppView*)(CUserAppView::GetInstance());
	if(pUserAppView) pUserAppView->ShowIndexReportForm(true);

	m_wndWorkspaceBar.ShowIndexReportForm(true);
	m_wndOutputBar.ShowDrawingReportOutputForm(true);
}

/**
	@brief
	@author	humkyung
*/
void CMainFrame::OnBOMReport() 
{
	CUserAppView* pUserAppView = (CUserAppView*)(CUserAppView::GetInstance());
	if(pUserAppView) pUserAppView->ShowBMReportForm(true);

	m_wndWorkspaceBar.ShowBOMReportForm(true);
	m_wndOutputBar.ShowBMLogFileForm(true);
}

/**
	@brief
	@author	humkyung
*/
void CMainFrame::OnDrawingPlotting() 
{
	CUserAppView* pUserAppView = (CUserAppView*)(CUserAppView::GetInstance());
	if(pUserAppView) pUserAppView->ShowDrawingPlottingForm(true);
	
	m_wndWorkspaceBar.ShowDrawingPlottingForm(true);
	m_wndOutputBar.ShowViewLogFileForm(true);
}


/**
	@brief

	@author	humkyung
*/
void CMainFrame::OnInsertToPdsAttr() 
{
	CUserAppView* pUserAppView = (CUserAppView*)(CUserAppView::GetInstance());
	if(pUserAppView) pUserAppView->ShowUpdatePDSForm(true);

	m_wndWorkspaceBar.ShowUpdatePDSForm(true);
	m_wndOutputBar.ShowUpdatePds(true);
}

/**
	@brief
	@author	HumKyung
	@date	2011.01.07
	@param	
	@return
*/
void CMainFrame::OnFileBackup() 
{
	CAppDocData* pDocData = CAppDocData::GetInstance();

	int res = AfxMessageBox(_T("DB를 백업하시겠습니까?") , MB_YESNO);
	if(res == IDYES)
	{
		CString rDBPath =  CString(pDocData->GetProject()->path()) + _T("\\Database\\") + CString(pDocData->GetProject()->name()) + _T(".MDB");
		CString rBackupDB;
		
		CFileDialog dlg(FALSE,_T(""),_T( CString(pDocData->GetProject()->name()) + _T(".BAK")),OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,	_T("Backup (*.BAK)||모든 문서(*.*)|*.*||")); dlg.DoModal();
		rBackupDB = dlg.GetPathName();
		
		CopyFile(rDBPath,rBackupDB,FALSE);
	}
}

/**
	@brief
	@author	HumKyung.BAEK
	@date	2011.02.10
	@param	pCmdUI
	@return
*/
void CMainFrame::OnUpdateIssueManager(CCmdUI* pCmdUI) 
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	CUserAppView* pUserAppView = (CUserAppView*)(CUserAppView::GetInstance());
	if(pUserAppView)
	{
		pCmdUI->SetCheck(pUserAppView->m_pBackupDataForm->IsWindowVisible());
	}
}

/**
	@brief
	@author	humkyung
	@date	2010.03.31
	@param	pCmdUI
	@return
*/
void CMainFrame::OnUpdateBmReport(CCmdUI* pCmdUI) 
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	CUserAppView* pUserAppView = (CUserAppView*)(CUserAppView::GetInstance());
	if(pUserAppView)
	{
		pCmdUI->SetCheck(pUserAppView->m_pBMReportForm->IsWindowVisible());
	}
}

/**
	@brief
	@author	HumKyung.BAEK
	@date	2010.03.31
*/
void CMainFrame::OnUpdateLineData(CCmdUI* pCmdUI) 
{
	CUserAppView* pUserAppView = (CUserAppView*)(CUserAppView::GetInstance());
	if(pUserAppView)
	{
		pCmdUI->SetCheck(pUserAppView->m_pLineDataForm->IsWindowVisible());
	}
}

/**
	@brief
	@author	HumKyung.BAEK
	@date	2010.03.31
*/
void CMainFrame::OnUpdateInsertToPdsAttr(CCmdUI* pCmdUI) 
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	CUserAppView* pUserAppView = (CUserAppView*)(CUserAppView::GetInstance());
	if(pUserAppView)
	{
		map<CString,CString>::const_iterator where = CProjectData::GetInstance()->m_pProjectSettingMap->find(_T("update_pds"));
		if(where != CProjectData::GetInstance()->m_pProjectSettingMap->end())
		{
			if((0 == where->second.CompareNoCase(_T("ON"))))
			{
				pCmdUI->SetCheck(pUserAppView->m_pInsertToPDS->IsWindowVisible());
			}
			else
			{
				pCmdUI->Enable(FALSE);
			}
		}
		else
		{
			pCmdUI->Enable(FALSE);
		}	
	}
}

/**
	@brief
	@author	HumKyung.BAEK
	@date	2010.03.31
*/
void CMainFrame::OnUpdateIsoProduct(CCmdUI* pCmdUI) 
{
	CUserAppView* pUserAppView = (CUserAppView*)(CUserAppView::GetInstance());
	if(pUserAppView)
	{
		pCmdUI->SetCheck(pUserAppView->m_pIsoEditFormHolder->IsWindowVisible());
	}
}


/******************************************************************************
    @author     humkyung
    @date       2012-04-06
    @class      CMainFrame
    @function   OnUpdateIsoRevAutomation
    @return     void
    @param      CCmdUI* pCmdUI
    @brief
******************************************************************************/
void CMainFrame::OnUpdateIsoRevAutomation(CCmdUI* pCmdUI) 
{
	CUserAppDoc* pDoc = CUserAppDoc::GetInstance();
	if(NULL != pDoc)
	{
		pCmdUI->SetCheck( pDoc->m_bRevAutomation );
	}
}

/******************************************************************************
    @author     humkyung
    @date       2012-05-07
    @function   OnUpdateIsoIssueAutomation
    @return     void
    @param      CCmdUI* pCmdUI
    @brief
******************************************************************************/
void CMainFrame::OnUpdateIsoIssueAutomation(CCmdUI* pCmdUI) 
{
	CUserAppDoc* pDoc = CUserAppDoc::GetInstance();
	if(NULL != pDoc)
	{
		pCmdUI->SetCheck( pDoc->m_bIssueAutomation );
	}
}

/**
	@brief

	@author	HumKyung.BAEK

	@date	2010.03.31
*/
void CMainFrame::OnUpdateLineListConverter(CCmdUI* pCmdUI) 
{
	CUserAppView* pUserAppView = (CUserAppView*)(CUserAppView::GetInstance());
	if(pUserAppView)
	{
		pCmdUI->SetCheck(pUserAppView->m_pBackupDataForm->IsWindowVisible());
	}
}

/**
	@brief

	@author	humkyung

	@date	2010.03.31
*/
void CMainFrame::OnUpdateDrawingReport(CCmdUI* pCmdUI) 
{
	CUserAppView* pUserAppView = (CUserAppView*)(CUserAppView::GetInstance());
	if(pUserAppView)
	{
		pCmdUI->SetCheck(pUserAppView->m_pDrawingReportForm->IsWindowVisible());
	}
}

/**
	@brief
	@author	humkyung
	@date	2010.03.31
*/
void CMainFrame::OnUpdateDrawingPlotting(CCmdUI* pCmdUI) 
{
	CUserAppView* pUserAppView = (CUserAppView*)(CUserAppView::GetInstance());
	if(pUserAppView)
	{
		pCmdUI->SetCheck(pUserAppView->m_pDrawingPlottingForm->IsWindowVisible());
	}
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-27
    @class      CMainFrame
    @function   GetUserAppOutputBar
    @return     CUserAppOutputWnd*
    @brief
******************************************************************************/
CUserAppOutputWnd* CMainFrame::GetUserAppOutputBar()
{
	return &(m_wndOutputBar);
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-27
    @class      CMainFrame
    @function   GetWorkspaceBar
    @return     CWorkspaceWnd*
    @brief
******************************************************************************/
CWorkspaceWnd* CMainFrame::GetWorkspaceBar()
{
	return &(m_wndWorkspaceBar);
}

/**
	@brief	
	@author	humkyung
	@date	2012.06.10
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

/******************************************************************************
    @author     humkyung
    @date       2012-08-26
    @function   OnShowWorkspacebar
    @return     void
    @brief
******************************************************************************/
void CMainFrame::OnShowWorkspacebar() 
{
	m_wndWorkspaceBar.ShowPane(TRUE , FALSE , FALSE);
}

/******************************************************************************
    @author     humkyung
    @date       2012-08-26
    @function   OnUpdateShowWorkspacebar
    @return     void
    @param      CCmdUI* pCmdUI
    @brief
******************************************************************************/
void CMainFrame::OnUpdateShowWorkspacebar(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_wndWorkspaceBar.IsVisible());
}

/******************************************************************************
    @author     humkyung
    @date       2012-08-26
    @function   OnShowOutputbar
    @return     void
    @brief
******************************************************************************/
void CMainFrame::OnShowOutputbar() 
{
	m_wndOutputBar.ShowPane(TRUE , FALSE , FALSE);
}

/******************************************************************************
    @author     humkyung
    @date       2012-08-26
    @function   OnUpdateShowOutputbar
    @return     void
    @param      CCmdUI* pCmdUI
    @brief
******************************************************************************/
void CMainFrame::OnUpdateShowOutputbar(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_wndOutputBar.IsVisible());
}

/******************************************************************************
	@brief		check or uncheck IPLOT
    @author     humkyung
    @date       2015-09-03
    @class      CMainFrame
    @return     void
******************************************************************************/
void CMainFrame::OnUseIPLOT()
{
	CUserAppDoc* pDoc = CUserAppDoc::GetInstance();
	if(NULL != pDoc) pDoc->m_bUseIPLOT = !pDoc->m_bUseIPLOT;
}

/******************************************************************************
    @author     humkyung
    @date       2015-09-03
    @class      CMainFrame
    @function   OnUpdateCheckIPLOT
    @return     void
    @param      CCmdUI* pCmdUI
******************************************************************************/
void CMainFrame::OnUpdateCheckIPLOT(CCmdUI* pCmdUI) 
{
	CUserAppDoc* pDoc = CUserAppDoc::GetInstance();
	if(NULL != pDoc) pCmdUI->SetCheck( pDoc->m_bUseIPLOT );
}

/******************************************************************************
    @author     humkyung
    @date       2015-09-03
    @class      CMainFrame
    @return     void
    @param      CCmdUI* pCmdUI
******************************************************************************/
void CMainFrame::OnUpdateComboPrinterList(CCmdUI* pCmdUI) 
{
	CUserAppDoc* pDoc = CUserAppDoc::GetInstance();
	if(NULL != pDoc) pCmdUI->Enable(!pDoc->m_bUseIPLOT);
}

/**
	@brief	generate iso product
	@author	humkyung
*/
void CMainFrame::OnGenerateIsoProduct() 
{
	CUserAppView* pUserAppView = (CUserAppView*)(CUserAppView::GetInstance());
	if(pUserAppView && pUserAppView->m_pIsoEditFormHolder->IsWindowVisible())
	{
		CIsoEditForm* pForm = pUserAppView->m_pIsoEditFormHolder->GetIsoEditForm();
		pForm->GenerateIsoProduct();
	}
}

/******************************************************************************
    @author     humkyung
    @date       2015-09-22
    @class      CMainFrame
    @return     void
    @param      CCmdUI* pCmdUI
******************************************************************************/
void CMainFrame::OnUpdateGenerateIsoProduct(CCmdUI* pCmdUI) 
{
	vector<LineDataRecordMap> aIsoEditItemEntry;

	CUserAppView* pUserAppView = (CUserAppView*)(CUserAppView::GetInstance());
	if(pUserAppView && pUserAppView->m_pIsoEditFormHolder->IsWindowVisible())
	{
		CIsoEditForm* pForm = pUserAppView->m_pIsoEditFormHolder->GetIsoEditForm();
		pForm->GetSelectedLineDataRecord(aIsoEditItemEntry);
	}
	pCmdUI->Enable(aIsoEditItemEntry.size() > 0);
}

/**
	@brief	generate drawing report
	@author	humkyung
*/
void CMainFrame::OnGenerateDrawingReport() 
{
	CUserAppView* pUserAppView = (CUserAppView*)(CUserAppView::GetInstance());
	if(pUserAppView && pUserAppView->m_pDrawingReportForm->IsWindowVisible())
	{
		pUserAppView->m_pDrawingReportForm->Generate();
	}
}

/******************************************************************************
    @author     humkyung
    @date       2015-09-22
    @class      CMainFrame
    @return     void
    @param      CCmdUI* pCmdUI
******************************************************************************/
void CMainFrame::OnUpdateGenerateDrawingReport(CCmdUI* pCmdUI) 
{
	vector<LineDataRecordMap> aLineDataRecordEntry;

	CUserAppView* pUserAppView = (CUserAppView*)(CUserAppView::GetInstance());
	if(pUserAppView && pUserAppView->m_pDrawingReportForm->IsWindowVisible())
	{
		pUserAppView->m_pDrawingReportForm->GetSelectedLineDataRecord(aLineDataRecordEntry);
	}
	pCmdUI->Enable(aLineDataRecordEntry.size() > 0);
}

/**
	@brief	generate B/M
	@author	humkyung
*/
void CMainFrame::OnGenerateBM() 
{
	CUserAppView* pUserAppView = (CUserAppView*)(CUserAppView::GetInstance());
	if(pUserAppView && pUserAppView->m_pBMReportForm->IsWindowVisible())
	{
		pUserAppView->m_pBMReportForm->Generate();
	}
}

/******************************************************************************
    @author     humkyung
    @date       2015-09-23
    @class      CMainFrame
    @return     void
    @param      CCmdUI* pCmdUI
******************************************************************************/
void CMainFrame::OnUpdateGenerateBM(CCmdUI* pCmdUI) 
{
	vector<LineDataRecordMap> aLineDataRecordEntry;
	CUserAppView* pUserAppView = (CUserAppView*)(CUserAppView::GetInstance());
	if(pUserAppView && pUserAppView->m_pBMReportForm->IsWindowVisible())
	{
		pUserAppView->m_pBMReportForm->GetSelectedLineDataRecord(aLineDataRecordEntry);
	}
	pCmdUI->Enable(aLineDataRecordEntry.size() > 0);
}

/**
	@brief	print drawing
	@author	humkyung
	@date	2015.09.24
*/
void CMainFrame::OnPrintDrawing() 
{
	CUserAppView* pUserAppView = (CUserAppView*)(CUserAppView::GetInstance());
	if(pUserAppView && pUserAppView->m_pDrawingPlottingForm->IsWindowVisible())
	{
		pUserAppView->m_pDrawingPlottingForm->Generate();
	}
}

/******************************************************************************
    @author     humkyung
    @date       2015-09-24
    @class      CMainFrame
    @param      CCmdUI* pCmdUI
******************************************************************************/
void CMainFrame::OnUpdatePrintDrawing(CCmdUI* pCmdUI) 
{
	vector<LineDataRecordMap> aLineDataRecordEntry;
	CUserAppView* pUserAppView = (CUserAppView*)(CUserAppView::GetInstance());
	if(pUserAppView && pUserAppView->m_pDrawingPlottingForm->IsWindowVisible())
	{
		pUserAppView->m_pDrawingPlottingForm->GetSelectedLineDataRecord(aLineDataRecordEntry);
	}
	pCmdUI->Enable(aLineDataRecordEntry.size() > 0);
}

/**
	@brief	update oralce
	@author	humkyung
	@date	2015.10.29
*/
void CMainFrame::OnUpdateOracle() 
{
	CUserAppView* pUserAppView = (CUserAppView*)(CUserAppView::GetInstance());
	if(pUserAppView && pUserAppView->m_pInsertToPDS->IsWindowVisible())
	{
		pUserAppView->m_pInsertToPDS->OnBnClickedButtonUpdateOracle();
	}
}

/******************************************************************************
    @author     humkyung
    @date       2015.10.29
    @param      CCmdUI* pCmdUI
******************************************************************************/
void CMainFrame::OnUpdateUpdateOracle(CCmdUI* pCmdUI) 
{
	vector<LineDataRecordMap> aLineDataRecordEntry;
	CUserAppView* pUserAppView = (CUserAppView*)(CUserAppView::GetInstance());
	if(pUserAppView && pUserAppView->m_pInsertToPDS->IsWindowVisible())
	{
		pUserAppView->m_pInsertToPDS->GetSelectedLineDataRecord(aLineDataRecordEntry);
	}
	pCmdUI->Enable(aLineDataRecordEntry.size() > 0);
}