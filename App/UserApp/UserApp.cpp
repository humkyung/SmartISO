
// UserApp.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"

#include <crtdbg.h>
#include <util/FileVersion.h>
#include <util/FileTools.h>
#include <Splash/SplashScreenFx.h>

#include "afxwinappex.h"
#include "UserApp.h"
#include "MainFrm.h"

#include "UserAppDoc.h"
#include "UserAppView.h"

#include "UserAppDocData.h"
#include "UserAppSettingSheet.h"
#include "ResourceSwitch.h"
#include "LoginDlg.h"
#include "VersionNo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUserAppApp

BEGIN_MESSAGE_MAP(CUserAppApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CUserAppApp::OnAppAbout)
	
	ON_COMMAND(ID_APP_CLOSE, OnAppClose)
	ON_COMMAND(ID_OPTION_SETTING, OnOptionSetting)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_SETTING_ADMIN, OnSettingAdmin)
	ON_COMMAND(ID_FILE_ISSUE_MANAGER , OnIssueManager)
	ON_COMMAND(ID_FILE_SMART_REPORTER, OnSmartReporter)
	ON_COMMAND(ID_FILE_ISO_CREATOR, OnIsoCreator)
	ON_UPDATE_COMMAND_UI(ID_SETTING_ADMIN, OnUpdateSettingAdmin)
	ON_UPDATE_COMMAND_UI(ID_FILE_ISSUE_MANAGER , OnUpdateIssueManager)
	ON_UPDATE_COMMAND_UI(ID_FILE_SMART_REPORTER , OnUpdateSmartReporter)
	ON_UPDATE_COMMAND_UI(ID_FILE_ISO_CREATOR , OnUpdateIsoCreator)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()

/******************************************************************************
    @author     humkyung
    @date       2011-08-20
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
	int g_jBT_LogSound = BT_OpenLogFile( _T("SmartISO.log") , BTLF_TEXT );
	BT_SetLogSizeInEntries( g_jBT_LogSound, 1024 );
	BT_SetLogFlags( g_jBT_LogSound, BTLF_SHOWLOGLEVEL | BTLF_SHOWTIMESTAMP );
	BT_SetLogLevel( g_jBT_LogSound, BTLL_INFO );
	LPCTSTR pLogFileName = BT_GetLogFileName( g_jBT_LogSound );
	BT_AddLogFile( pLogFileName );
}
*/

// CUserAppApp construction

CUserAppApp::CUserAppApp()
{
	m_bHiColorIcons = TRUE;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	_CrtSetBreakAlloc(172);
}

// The one and only CUserAppApp object

CUserAppApp theApp;


// CUserAppApp initialization

BOOL CUserAppApp::InitInstance()
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

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(_T("IDP_OLE_INIT_FAILED"));
		return FALSE;
	}
	AfxEnableControlContainer();

	if ( FALSE == CSingleInstance::Create (_T("EB10D9F4-9573-483f-AC07-6AFD7F79A242")))
		return FALSE ;

	CLoginDlg dlg;
	if(IDOK == dlg.DoModal())
	{
		/*CAppDocData& docData = CAppDocData::GetInstance();
		docData.SetProjectName(dlg.m_rProjectNo.operator LPCTSTR());*/
	}else   return FALSE;

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

	/// SetupExceptionHandler();	/// 2011.08.20 added by humkyung
#endif
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("SmartISO"));
	LoadStdProfileSettings(0);  // Load standard INI file options (including MRU)

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CUserAppDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CUserAppView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	m_hDll = CScintillaWnd::LoadScintillaDll();
	// try to get version from lexer dll
	if (m_hDll != NULL)
	{
		TCHAR szFilename[256];
		::GetModuleFileName(m_hDll, szFilename, 256);
		CFileVersion ver;
		ver.Open (szFilename);
		m_strVersionScintilla = ver.GetFixedFileVersion();
		m_strProductScintilla = ver.QueryValue (_T("ProductName"));
		m_strFileScintilla = szFilename;
		ver.Close();
	}
	LoadPlugins();
	this->StartupRuntimeHost(_T("SmartISOLayer") , _T("SmartISOLayer.SmartISOLayer"));

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileOpen;
	cmdInfo.m_strFileName = dlg.m_sIniFilePath + _T("?id=") + dlg.m_sId+ _T(",password=") + dlg.m_sPassword+ _T(",site=") + dlg.m_sSite;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	m_pMainWnd->ShowWindow(SW_MAXIMIZE);
	// The one and only window has been initialized, so show and update it
	{
		CRect rect;
		((CMainFrame*)m_pMainWnd)->GetWindowRect(&rect);

		CWorkspaceWnd* pWorkspaceBar = ((CMainFrame*)m_pMainWnd)->GetWorkspaceBar();
		if(NULL != pWorkspaceBar)
		{
			rect.right = rect.left + CLineListSelectOption1::WIDTH;
			pWorkspaceBar->MoveWindow(rect);
		}

		CUserAppOutputWnd* pOutputBar = ((CMainFrame*)m_pMainWnd)->GetUserAppOutputBar();
		if(NULL != pOutputBar)
		{
			rect.top = rect.top + (int)(rect.Height()*0.8);
			pOutputBar->MoveWindow(rect);
		}
	}
	((CMainFrame*)m_pMainWnd)->RecalcLayout();
	m_pMainWnd->UpdateWindow();

	/// 2013.01.07 added by humkyung
	CAppDocData* pDocData = CAppDocData::GetInstance();
	m_pMainWnd->SetWindowText(pDocData->GetProject()->name());
	/// up to here

	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	return TRUE;
}

/******************************************************************************
    @author     humkyung
    @date       ????-??-??
    @class      CUserAppApp
    @function   ExitInstance
    @return     int
    @brief
******************************************************************************/
int CUserAppApp::ExitInstance() 
{
	_CrtDumpMemoryLeaks();
	_CrtMemDumpAllObjectsSince(0); 

	m_PrinterControl.SavePrinterSelection(m_hDevMode, m_hDevNames);	
	if(NULL != m_pRuntimeHost) m_pRuntimeHost->Stop();

	return CWinApp::ExitInstance();
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
void CUserAppApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CUserAppApp customization load/save methods

void CUserAppApp::PreLoadState()
{
}

void CUserAppApp::LoadCustomState()
{
}

void CUserAppApp::SaveCustomState()
{
}

// CUserAppApp message handlers
void CUserAppApp::ReadPrinterList()
{
	static bool __INIT__ = false;

	if(false == __INIT__)
	{
		m_PrinterControl.ReadLocalPrinters();
		m_PrinterControl.RestorePrinterSelection(m_hDevMode, m_hDevNames);
	}

	__INIT__ = true;
}

CString CUserAppApp::GetDefaultPrinterEx()
{
	PRINTDLG	pd ;
	CString		printer( _T("Failed")) ;
	
	pd.lStructSize = (DWORD)sizeof(PRINTDLG) ;
	BOOL bRet = GetPrinterDeviceDefaults(&pd) ;
	if (bRet)
	{
		/// protect memory handle with ::GlobalLock and ::GlobalUnlock
		DEVMODE *pDevMode = (DEVMODE*)::GlobalLock(m_hDevMode) ;
		printer = pDevMode->dmDeviceName ;
		::GlobalUnlock(m_hDevMode) ;
	}
	return printer ;
}

/******************************************************************************
    @author     humkyung
    @date       2011-08-19
    @class      CUserAppApp
    @function   OnAppClose
    @return     void
    @brief
******************************************************************************/
void CUserAppApp::OnAppClose() 
{
	TCHAR szPathName[MAX_PATH] = {0,};
	GetModuleFileName(AfxGetInstanceHandle() , szPathName , MAX_PATH);
	
	CString rPathName(szPathName);
	int it = 0;
	it = rPathName.ReverseFind('\\');
	if(it)
	{
		rPathName = rPathName.Left(it);
		::ShellExecute(NULL, _T("open") , rPathName + _T("\\SmartISO.exe") , _T("") , rPathName , SW_SHOWNORMAL);
		
		GetUserAppMainWnd()->PostMessage(WM_CLOSE); 
	}
}

/**	
	@brief	The CUserAppApp::OnOptionSetting function

	@author	humkyung

	@return	void	
*/
void CUserAppApp::OnOptionSetting() 
{
	/*CAppDocData* pDocData = CAppDocData::GetInstance();
	
	CUserAppApp* pApp = (CUserAppApp*)AfxGetApp();
	///CResourceSwitch resSwitch(pApp->m_hMultiLangRes);
	
	CUserAppSettingSheet sheet(IDS_USER_APP_SETTING_TITLE);
	sheet.m_rIniFilePath = (pDocData->m_ProjectPath) + CString("\\Setting\\") + (pDocData->m_ProjectName) + CString(".ini");	
	sheet.m_pSelUser     = (pDocData->m_pSelUser);
	sheet.SetTreeViewMode(TRUE, TRUE, TRUE);
	sheet.SetTreeWidth(200);
	if(IDOK == sheet.DoModal())
	{
	}	*/
}

void CUserAppApp::OnFileSave() 
{
	// TODO: Add your command handler code here
	
}

/**	
	@brief	call admin program
	@author	HumKyung.Baek
	@return	void	
*/
void CUserAppApp::OnSettingAdmin() 
{
	TCHAR szPathName[MAX_PATH] = {0,};
	GetModuleFileName(AfxGetInstanceHandle() , szPathName , MAX_PATH);
	
	CString rPathName(szPathName);
	const int it = rPathName.ReverseFind('\\');
	if(it)
	{
		CAppDocData* pDocData = CAppDocData::GetInstance();
		const CString sPrjName = pDocData->GetProject()->name();
		const CString sUserId = pDocData->m_sUserId;
		const CString rParameter = _T("\"") + pDocData->GetPrjIniFilePath() + _T("\"?") + sPrjName + _T(";") + sUserId;
		
		rPathName = rPathName.Left(it);
		::ShellExecute(NULL, _T("open") , CString("\"") + rPathName + _T("\\SmartAdmin.exe\"") , rParameter, rPathName , SW_SHOWNORMAL);
		
		GetUserAppMainWnd()->PostMessage(WM_CLOSE); 
	}
}

/**	
	@brief	disable menu if user is not 'ADMIN'
	@author	HumKyung.Baek
	@return	void	
*/
void CUserAppApp::OnUpdateSettingAdmin(CCmdUI* pCmdUI) 
{
	/*CAppDocData* pDocData = CAppDocData::GetInstance();
	if(pDocData)
	{
		const BOOL bFileExist = CFileTools::DoesFileExist(CAppDocData::GetExecPath() + _T("\\SmartAdmin.exe"));
		pCmdUI->Enable(bFileExist && (STRING_T(_T("ADMIN")) == pDocData->m_pSelUser->id()));
	}*/
}

/**
	@brief	run issue manager
	@author	HumKyung
	@date	2011.02.17
	@param	
	@return
*/
void CUserAppApp::OnIssueManager() 
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	if(pDocData)
	{
		const CString sIssueManagerFilePath = CAppDocData::GetExecPath() + _T("\\IssueManager.exe");
		const CString sProjectPath = pDocData->GetProject()->path();
		CString sParam = _T("\"") + CString(pDocData->GetProject()->name()) + _T("\" \"") + sProjectPath + _T("\"");
		::ShellExecute(NULL, _T("open") , sIssueManagerFilePath , sParam , NULL , SW_SHOW);
	}
}

/**
	@brief	

	@author	HumKyung

	@date	2011.02.17

	@param	pCmdUI

	@return
*/
void CUserAppApp::OnUpdateIssueManager(CCmdUI* pCmdUI) 
{
	/*CAppDocData* pDocData = CAppDocData::GetInstance();
	if(pDocData)
	{
		const BOOL bFileExist = CFileTools::DoesFileExist(CAppDocData::GetExecPath() + _T("\\IssueManager.exe"));
		pCmdUI->Enable(bFileExist && (STRING_T(_T("ADMIN")) == pDocData->m_pSelUser->id()));
	}else	pCmdUI->Enable(FALSE);*/
}

/******************************************************************************
    @author     humkyung
    @date       2012-05-03
    @class      CUserAppApp
    @function   OnSmartReporter
    @return     void
    @brief
******************************************************************************/
void CUserAppApp::OnSmartReporter() 
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	if(pDocData)
	{
		const CString sSmartReporterFilePath = CAppDocData::GetExecPath() + _T("\\SmartReporter.exe");
		const CString sProjectPath = pDocData->GetProject()->path();
		CString sParam = _T("\"") + sProjectPath + _T("\\Setting\\") + CString(pDocData->GetProject()->name()) + _T(".ini\"");
		::ShellExecute(NULL, _T("open") , sSmartReporterFilePath , sParam , NULL , SW_SHOW);
	}
}

/******************************************************************************
    @author     humkyung
    @date       2012-05-03
    @class      CUserAppApp
    @function   OnUpdateSmartReporter
    @return     void
    @param      CCmdUI* pCmdUI
    @brief
******************************************************************************/
void CUserAppApp::OnUpdateSmartReporter(CCmdUI* pCmdUI) 
{
	/*CAppDocData* pDocData = CAppDocData::GetInstance();
	if(pDocData)
	{
		const BOOL bFileExist = CFileTools::DoesFileExist(CAppDocData::GetExecPath() + _T("\\SmartReporter.exe"));
		pCmdUI->Enable(bFileExist && (STRING_T(_T("ADMIN")) == pDocData->m_pSelUser->id()));
	}else	pCmdUI->Enable(FALSE);*/
}

/******************************************************************************
    @author     humkyung
    @date       2012-05-18
    @class      CUserAppApp
    @function   OnIsoCreator
    @return     void
    @brief
******************************************************************************/
void CUserAppApp::OnIsoCreator() 
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	if(pDocData)
	{
		const CString sIsoCreatorFilePath = CAppDocData::GetExecPath() + _T("\\IsoCreator.exe");
		const CString sProjectPath = pDocData->GetProject()->path();
		CString sParam = _T("\"\" \"") + CString(pDocData->GetProject()->name()) + _T("\" \"") + sProjectPath + _T("\"");
		::ShellExecute(NULL, _T("open") , sIsoCreatorFilePath , sParam , NULL , SW_SHOW);
	}
}

/******************************************************************************
    @author     humkyung
    @date       2012-05-18
    @class      CUserAppApp
    @function   OnUpdateIsoCreator
    @return     void
    @param      CCmdUI* pCmdUI
    @brief
******************************************************************************/
void CUserAppApp::OnUpdateIsoCreator(CCmdUI* pCmdUI) 
{
	/*CAppDocData* pDocData = CAppDocData::GetInstance();
	if(pDocData)
	{
		const BOOL bFileExist = CFileTools::DoesFileExist(CAppDocData::GetExecPath() + _T("\\IsoCreator.exe"));
		pCmdUI->Enable(bFileExist && (STRING_T(_T("ADMIN")) == pDocData->m_pSelUser->id()));
	}else	pCmdUI->Enable(FALSE);*/
}

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

	SetDlgItemText(IDC_STATIC_ABOUT , CString(_T("SmartISO Version ")) + STRPRODUCTVER);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

/******************************************************************************
    @author     humkyung
    @date       2012-01-02
    @class      CUserAppApp
    @function   LoadPlugins
    @return     int
    @param      void
    @brief
******************************************************************************/
int CUserAppApp::LoadPlugins(void)
{
	const CString sDiffFilePath = CAppDocData::GetExecPath() + _T("\\Plugin\\SmartISODiff.dll");

	HINSTANCE hInstDll = LoadLibrary(sDiffFilePath);
	if(hInstDll)
	{
		SmartISODiff = (int (*)(const LPCTSTR,const LPCTSTR,const LPCTSTR))GetProcAddress(hInstDll , "SmartISODiff");
	}

	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2012-08-29
    @class      CProjectDlg
    @function   StartupReuntimeHost
    @return     HRESULT
    @brief
******************************************************************************/
HRESULT CUserAppApp::StartupRuntimeHost(TCHAR* szAsseblyName, TCHAR* szClassNameWithNamespace)
{
	//Retrieve a pointer to the ICorRuntimeHost interface
	HRESULT hr = CorBindToRuntimeEx(
		NULL,	//Specify the version of the runtime that will be loaded. 
		L"wks",//Indicate whether the server or workstation build should be loaded. 	
		//Control whether concurrent or non-concurrent garbage collection
		//Control whether assemblies are loaded as domain-neutral. 
		STARTUP_LOADER_SAFEMODE | STARTUP_CONCURRENT_GC, 
		CLSID_CorRuntimeHost,
		IID_ICorRuntimeHost,
		//Obtain an interface pointer to ICorRuntimeHost 
		(void**)&m_pRuntimeHost
		);
	if (FAILED(hr)) return 1;
	//Start the CLR
	hr = m_pRuntimeHost->Start();

	CComPtr<IUnknown> pUnknown;

	//Retrieve the IUnknown default AppDomain
	hr = m_pRuntimeHost->GetDefaultDomain(&pUnknown);
	if (FAILED(hr)) return hr;

	hr = pUnknown->QueryInterface(&m_pDefAppDomain.p);
	if (FAILED(hr)) return hr;

	_bstr_t _bstrAssemblyName(szAsseblyName);
	_bstr_t _bstrszClassNameWithNamespace(szClassNameWithNamespace);

	//Creates an instance of the Assembly
	hr = m_pDefAppDomain->CreateInstance( 
		_bstrAssemblyName,
		_bstrszClassNameWithNamespace,
		&m_pObjectHandle
		);
	if (FAILED(hr)) return hr;

	return hr;
}

/******************************************************************************
    @author     humkyung
    @date       2012-08-29
    @class      CUserAppApp
    @function   CallManagedFunction
    @return     HRESULT
    @param      BSTR    szMethodName
    @param      int     iNoOfParams
    @param      VARIANT *
    @param      pvArgs  VARIANT
    @param      *       pvRet
    @brief
******************************************************************************/
HRESULT CUserAppApp::CallManagedFunction(BSTR szMethodName, int iNoOfParams, VARIANT * pvArgs, VARIANT * pvRet)
{
	HRESULT hr;

	try
	{	
		CComVariant VntUnwrapped;
		hr = m_pObjectHandle->Unwrap(&VntUnwrapped);
		if (FAILED(hr)) return hr;
		
		if ((VntUnwrapped.vt != VT_DISPATCH) && (VntUnwrapped.vt != VT_UNKNOWN))
			return E_FAIL;
		
		CComPtr<IDispatch> pDisp;
		pDisp = VntUnwrapped.pdispVal;
		
		DISPID dispid;
		
		DISPPARAMS dispparamsArgs = {NULL, NULL, 0, 0};
		dispparamsArgs.cArgs = iNoOfParams;
		dispparamsArgs.rgvarg = pvArgs;
		
		hr = pDisp->GetIDsOfNames (
			IID_NULL, 
			&szMethodName,
			1,
			LOCALE_SYSTEM_DEFAULT,
			&dispid
			);
		if (FAILED(hr)) return hr;
		
		//Invoke the method on the Dispatch Interface
		hr = pDisp->Invoke (
			dispid,
			IID_NULL,
			LOCALE_SYSTEM_DEFAULT,
			DISPATCH_METHOD,
			&dispparamsArgs,
			pvRet,
			NULL,
			NULL
			);
		if (FAILED(hr)) return hr;
	}
	catch(_com_error e)
	{
		//Exception handling.	
		int d = 1;
	}

	return hr;
}