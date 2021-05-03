
// SmartReporter.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
//#include <Util/IsUtilInf.h>
#include "afxwinappex.h"
#include "SmartReporter.h"
#include "MainFrm.h"
#include "VersionNo.h"

#include "SmartReporterDoc.h"
#include "SmartReporterView.h"
#include "LineListToOraSettingSheet.h"

#include <Splash/SplashScreenFx.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSmartReporterApp

BEGIN_MESSAGE_MAP(CSmartReporterApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CSmartReporterApp::OnAppAbout)
	ON_COMMAND(ID_FILE_SETTING, OnFileSetting)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ORA_DATA_REPORT, OnUpdateViewOracleDataReport)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CSmartReporterApp construction

CSmartReporterApp::CSmartReporterApp()
{

	m_bHiColorIcons = TRUE;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CSmartReporterApp object

CSmartReporterApp theApp;

// CSmartReporterApp initialization

BOOL CSmartReporterApp::InitInstance()
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

	if ( FALSE == CSingleInstance::Create (_T("C66ABC99-C462-4742-A92E-49E7352F65A5")))
		return FALSE ;

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

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("SmartReporter"));
	LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	///TODO: locale을 자동으로 읽어오도록 해야 할것 같음... - 2012.05.04 added by humkyung
	LANGID lsys = GetSystemDefaultLangID ();
	if(0x0412 == lsys)
	{
		locale::global(locale("kor"));
	}
	else if(0x0411 == lsys)
	{
		 setlocale( LC_ALL, _T("Japanese") );
	}
	/// up to here

	//! terminate if can't connect internet.
	//! 보안과 버젼을 확인하여 새로운 버젼이 나왔으면 다운로드하여 설치하도록 한다.
	/*const int res = AutoUpdate();
	if((RETURN_ERROR == res) || (RETURN_SECURITYERROR == res)) return FALSE;	*/

	/*CLoginDlg LoginDlg;
	if( IDOK == LoginDlg.DoModal() )
	{
		CLineListToOraDocData& docData = CLineListToOraDocData::GetInstance();

		docData.m_sProjectName = LoginDlg.m_rProjectName.MakeUpper();
		
		CString sProjectFilePath = GetAppDataPath() + _T("\\") + CString(APPNAME) + _T("\\") + docData.m_sProjectName + _T(".ini");
		if( FALSE == FileExist( sProjectFilePath ) )
		{
			/// copy default ini as project ini - 2011.08.13 added by humkyung
			CopyFile(GetExecPath() + _T("\\Backup\\Default.ini") , sProjectFilePath , TRUE);

			::WritePrivateProfileString( _T("General") , _T("Project") , docData.m_sProjectName , sProjectFilePath );
		}
	}
	else	return FALSE;*/

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CSmartReporterDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CSmartReporterView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_MAXIMIZE);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	return TRUE;
}

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CSmartReporterApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CSmartReporterApp customization load/save methods

/*void CSmartReporterApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CSmartReporterApp::LoadCustomState()
{
}

void CSmartReporterApp::SaveCustomState()
{
}
*/
// CSmartReporterApp message handlers
/**
**/
void CSmartReporterApp::OnFileSetting() 
{	
	CLineListToOraSettingSheet sheet(IDS_LINELIST_TO_ORA_SETTING_TITLE);
	sheet.SetTreeViewMode(TRUE, TRUE, TRUE);
	sheet.SetTreeWidth(200);
	if(IDOK == sheet.DoModal())
	{
	}	

	{
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CSmartReporterDoc* pDoc = (CSmartReporterDoc*)(pMainFrame->GetActiveDocument());
		POSITION pos = pDoc->GetFirstViewPosition();
		while(NULL != pos)
		{
			CSmartReporterView* pView = (CSmartReporterView*)(pDoc->GetNextView(pos));
			if((NULL != pView) && pView->IsKindOf(RUNTIME_CLASS(CSmartReporterView)))
			{
				pView->SyncTabCtrlWithCustomReportTableList();
			}
		}
	}
}

void CSmartReporterApp::OnUpdateViewOracleDataReport(CCmdUI* pCmdUI) 
{
	CString rString;
	rString.LoadString(IDS_ADMINISTRATOR);
	
	CLineListToOraDocData& docData = CLineListToOraDocData::GetInstance();
	pCmdUI->Enable((rString.operator LPCTSTR() == docData.GetUserName()));	
	pCmdUI->SetCheck(TRUE);
}


BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString sString;
	sString.Format(_T("SmartReporter Version %s") , _T(STRPRODUCTVER));
	sString.TrimLeft();
	sString.TrimRight();

	SetDlgItemText(IDC_STATIC_ABOUT , sString);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
