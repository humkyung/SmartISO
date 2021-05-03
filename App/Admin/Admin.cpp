// Admin.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Admin.h"
#include <Splash/SplashScreenFx.h>

#include "MainFrm.h"
#include "AdminDoc.h"
#include "AdminView.h"
#include "AdminDocData.h"
#include "SmartAdminSettingSheet.h"
#include "StringTable.h"

#include "VersionNo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CQGLScene  m_GLScene;
CQGLCanvas m_GLCanvas;
static HINSTANCE ghLangInst=NULL;
/////////////////////////////////////////////////////////////////////////////
// CAdminApp

BEGIN_MESSAGE_MAP(CAdminApp, CWinAppEx)
	//{{AFX_MSG_MAP(CAdminApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_APP_CLOSE, OnAppClose)
	ON_COMMAND(ID_TO_USERAPP, OnToUserApp)
	ON_COMMAND(ID_TOOLS_GENERAL_SETTING, OnGeneralSetting)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdminApp construction

/******************************************************************************
    @author     humkyung
    @date       2011-08-19
    @class
    @function   SetupExceptionHandler
    @return     void
    @brief
******************************************************************************/
/*
static void SetupExceptionHandler()
{
	BT_InstallSehFilter();
	BT_SetSupportEMail(_T("zbaekhk@gmail.com"));
	BT_SetFlags(BTF_DETAILEDMODE | BTF_EDITMAIL);
	BT_SetSupportServer(_T("localhost"), 9999);
	BT_SetSupportURL(_T("http://www.solutionware.co.kr"));

	// 대표적인 속성들은 다음과 같다.
	// BTF_DETAILEDMODE : 미니덤프나 로그파일등을 압축해서 모두 전송해준다. 
	// 정의하지 않을 경우 기본적인 정보만 전송해준다.
     // BTF_SCREENCAPTURE : 스크린샷을 첨부한다.

	// 미니덤프에 참조변수까지 남긴다.
     BT_SetFlags( BTF_DETAILEDMODE | BTF_SCREENCAPTURE );

	// Log생성
	int g_jBT_LogSound = BT_OpenLogFile( _T("SmartAdmin.log") , BTLF_TEXT );
	BT_SetLogSizeInEntries( g_jBT_LogSound, 1024 );
	BT_SetLogFlags( g_jBT_LogSound, BTLF_SHOWLOGLEVEL | BTLF_SHOWTIMESTAMP );
	BT_SetLogLevel( g_jBT_LogSound, BTLL_INFO );
	LPCTSTR pLogFileName = BT_GetLogFileName( g_jBT_LogSound );
	BT_AddLogFile( pLogFileName );
}
*/

CAdminApp::CAdminApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAdminApp object

CAdminApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CAdminApp initialization

BOOL CAdminApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	InitLanguage(); 

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(_T("IDP_OLE_INIT_FAILED"));
		return FALSE;
	}

	AfxEnableControlContainer();
	///AfxGetModuleState()->m_dwVersion = 0x0601;
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	if( FALSE == CSingleInstance::Create (_T("1AD2E844-3494-4dd5-A189-684C6C23F667")))
		return FALSE ;

	/// SetupExceptionHandler();

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

#ifndef	_DEBUG
	/// SplashScreen
	CSplashScreenFx *pSplash = new CSplashScreenFx();
	if(pSplash)
	{
		pSplash->Create(GetDesktopWindow() ,CString(CString(STRPRODUCTVER) + _T(" Initializing...")),0,CSS_FADE | CSS_CENTERSCREEN | CSS_SHADOW);
		pSplash->SetBitmap(IDB_LOGO , 1 , 1 , 1);
		pSplash->SetTextFont( _T("Arial") ,80,CSS_TEXT_NORMAL);
		pSplash->SetTextRect(CRect(1,385,495,400));
		pSplash->CreateProgressBar(CRect(5,380,495,385), CSize(0, 5), PBS_SMOOTH);
		pSplash->SetTextColor(RGB(0,0,0));
		pSplash->SetTextFormat(DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
		pSplash->Show();	
		pSplash->SetActiveWindow();

		pSplash->m_ctrlProgress.SetRange(0 , 100);
		pSplash->m_ctrlProgress.SetStep(1);
		pSplash->m_ctrlProgress.SetPos(0);
	}
#endif
	//////////////////////////////////////////////////////////////////////////
	TCHAR szPathName[MAX_PATH] = {0,};
	GetModuleFileName(AfxGetInstanceHandle() , szPathName , MAX_PATH);
	CString sExecPath(szPathName);
	const int it = sExecPath.ReverseFind('\\');
	if(it)
	{
		sExecPath = sExecPath.Left(it);
		const CString sStringFilePath = sExecPath + _T("\\Setting\\STRING.TBL");
		CStringTable& table = CStringTable::GetInstance();
		table.Parse(sStringFilePath.operator LPCTSTR());
	}

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("SmartAdmin"));

	LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	this->CleanState();	/// clean state - 2012.04.06 added by humkyung

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CAdminDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CAdminView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	//////////////////////////////////////////////////////////////////////////
	m_GLScene.LoadGLFont(sExecPath + _T("\\fonts\\50.vft"));

	if(NULL != m_hMultiLangRes) AfxSetResourceHandle(m_hMultiLangRes);

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_MAXIMIZE);
	((CMainFrame*)m_pMainWnd)->RecalcLayout();
	m_pMainWnd->UpdateWindow();

	return TRUE;
}

/******************************************************************************
    @author     humkyung
    @date       2012-05-10
    @class      CAdminApp
    @function   OnGeneralSetting
    @return     void
    @brief
******************************************************************************/
void CAdminApp::OnGeneralSetting() 
{
	CAdminDocData* pDocData = CAdminDocData::GetInstance();
	
	CAdminApp* pApp = (CAdminApp*)AfxGetApp();
	
	CSmartAdminSettingSheet sheet(IDS_ADMIN_SETTING_TITLE);
	sheet.m_rIniFilePath = (pDocData->m_ProjectPath) + CString(_T("\\Setting\\")) + (pDocData->m_ProjectName) + CString(".ini");	
	sheet.m_pCurProject  = CAdminDoc::GetInstance()->GetCurProject();
	sheet.SetTreeViewMode(TRUE, TRUE, TRUE);
	sheet.SetTreeWidth(200);
	if(IDOK == sheet.DoModal())
	{
	}	
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/******************************************************************************
    @author     humkyung
    @date       2011-08-19
    @class      CAboutDlg
    @function   OnInitDialog
    @return     BOOL
    @brief
******************************************************************************/
BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetDlgItemText(IDC_STATIC_ABOUT , CString(_T("Admin 버전 ")) + STRPRODUCTVER);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// App command to run the dialog
void CAdminApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CAdminApp message handlers

BOOL CAdminApp::InitLanguage()
{
	// Begin of Multiple Language support
	/*
	if ( CMultiLanguage::m_nLocales <= 0 )	// Not detected yet
	{
		CMultiLanguage::DetectLangID();		// Detect language as user locale
		CMultiLanguage::DetectUILanguage();	// Detect language in MUI OS
	}
	TCHAR szModuleFileName[MAX_PATH];		// Get Module File Name and path
	int ret = ::GetModuleFileName(theApp.m_hInstance, szModuleFileName, MAX_PATH);
	if ( ret == 0 || ret == MAX_PATH )
		ASSERT(FALSE);
	// Load resource-only language DLL. It will use the languages
	// detected above, take first available language,
	// or you can specify another language as second parameter to
	// LoadLangResourceDLL. And try that first.
	ghLangInst = CMultiLanguage::LoadLangResourceDLL( szModuleFileName );
	if (ghLangInst)
		AfxSetResourceHandle( ghLangInst ); 
	// End of Multiple Language support
	*/
	//Set the new language
	///m_hMultiLangRes = LoadLibrary("Lang\\AdminKorLang.dll");
	///if (m_hMultiLangRes) AfxSetResourceHandle(m_hMultiLangRes);

	return TRUE;
}

void CAdminApp::OnAppClose() 
{
	TCHAR szPathName[MAX_PATH] = {0,};
	GetModuleFileName(AfxGetInstanceHandle() , szPathName , MAX_PATH);
	
	CString rPathName(szPathName);
	int it = 0;
	it = rPathName.ReverseFind('\\');
	if(it)
	{
		rPathName = rPathName.Left(it);
		::ShellExecute(NULL, _T("open") , rPathName + _T("\\IMCS.exe") , _T("") , rPathName , SW_SHOWNORMAL);
		
		AfxGetMainWnd()->PostMessage(WM_CLOSE); 
	}
}

/**
	@brief	프로그램을 종료하고 UserApp로 돌아갑니다.

	@author	humkyung

	@return	none
*/
void CAdminApp::OnToUserApp() 
{
	TCHAR szPathName[MAX_PATH] = {0,};
	GetModuleFileName(AfxGetInstanceHandle() , szPathName , MAX_PATH);
	
	CString rPathName(szPathName);
	int it = 0;
	it = rPathName.ReverseFind('\\');
	if(it)
	{
		CAdminDocData* pDocData = CAdminDocData::GetInstance();
		/// get admin password - 2012.01.27 added by humkyung
		const CString sPassword;/// = CAdminDoc::GetInstance()->GetCurProject()->GetAdminPassword().c_str();
		/// up to here

		const CString rParameter = _T("\"") + pDocData->GetIniFilePath() + _T("\"?id=ADMIN,password=") + sPassword;

		rPathName = rPathName.Left(it);
		::ShellExecute(NULL, _T("open") , CString(_T("\"")) + rPathName + _T("\\SmartISO.exe\"") , rParameter, rPathName , SW_SHOWNORMAL);
		
		AfxGetMainWnd()->PostMessage(WM_CLOSE); 
	}
}

