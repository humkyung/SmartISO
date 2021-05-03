// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include <assert.h>
#include <memory.h>

#include <util\ADO\ADODB.h>

#include "UserApp.h"
#include <FilterEdit/FilterEdit.h>
#include "LoginDlg.h"
#include "resource.h"
#include "SplitPath.h"
#include "UserAppDocData.h"
#include "ProjectDlg.h"

#include <Tokenizer.h>
#include <util/FileTools.h>
#include <util/Path.h>
#include <util/Registry.h>

/// #include "Socket/CommandObject.h"
/// #include "Socket/ClientSocket.h"
#include "VersionNo.h"

#include "soci.h"
#include "soci-sqlite3.h"

/*
#include "soapLicWebServiceSoapProxy.h"
#include "LicWebServiceSoap.nsmap"
*/

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*
extern "C" __declspec(dllimport) int __stdcall Encode(LMS_ENCODE* param);
extern "C" __declspec(dllimport) int __stdcall Decode(LMS_ENCODE* param);


CClientSocket CLoginDlg::m_oClientSocket;
*/
/////////////////////////////////////////////////////////////////////////////
// CLoginDlg dialog

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoginDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoginDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CLoginDlg::~CLoginDlg()
{
	try
	{
		for(map<CString,list<CProjectDlg::ProjectInfo>*>::iterator itr = m_ProjectMap.begin();itr != m_ProjectMap.end();++itr)
		{
			SAFE_DELETE(itr->second);
		}
	}
	catch(...)
	{
	}
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoginDlg)
	DDX_Control(pDX , IDC_STATIC_TITLE , m_wndStaticTitle);

	DDX_Control(pDX , IDC_COMBO_PROJECT  , m_wndProjectCombo);
	DDX_Control(pDX , IDC_COMBO_USER , m_UserCombo);
	DDX_Control(pDX , IDC_EDIT_PASSWORD  , m_PasswordEdit);
	DDX_Control(pDX , IDC_COMBO_MODULE , m_cboModule);
	DDX_Control(pDX , IDC_STATIC_MSG , m_wndStaticMsg);
	DDX_Control(pDX , IDOK , m_okButton);
	DDX_Control(pDX , IDCANCEL , m_cancelButton);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
	//{{AFX_MSG_MAP(CLoginDlg)
	//ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_COMBO_PROJECT, OnSelchangeComboProject)
	ON_CBN_SELENDOK(IDC_COMBO_USER, OnSelendokComboUser)
	//}}AFX_MSG_MAP
	ON_WM_HELPINFO()
	///ON_BN_CLICKED(IDC_BUTTON_RESET_ACTIVATE_CODE, &CLoginDlg::OnBnClickedButtonResetActivateCode)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg message handlers

BOOL CLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.
	auto_ptr<CFont>m_pBoldFont(new CFont);
	m_pBoldFont->CreateFont(20,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0 , _T("Arial"));

	m_wndStaticTitle.SetColor( RGB(0, 0, 0) );///GetSysColor(COLOR_BTNFACE) );
	m_wndStaticTitle.SetGradientColor( RGB(74, 84, 103) );
	m_wndStaticTitle.SetFont(m_pBoldFont.get());
	m_wndStaticTitle.SetVerticalGradient(TRUE);
	m_wndStaticTitle.SetTextAlign(DT_CENTER);
	m_wndStaticTitle.SetTextColor(RGB(255 , 255 , 255));

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	{
		CWnd* pWnd = GetDlgItem(IDC_STATIC_LOGIN);
		if(NULL != pWnd) pWnd->SetWindowText(_TR("Login"));
		pWnd = GetDlgItem(IDC_STATIC_PROJECT);
		if(NULL != pWnd) pWnd->SetWindowText(_TR("Project") + _T(" : "));
		pWnd = GetDlgItem(IDC_STATIC_USER);
		if(NULL != pWnd) pWnd->SetWindowText(_TR("User") + _T(" : "));
		pWnd = GetDlgItem(IDC_STATIC_PASSWORD);
		if(NULL != pWnd) pWnd->SetWindowText(_TR("Password") + _T(" : "));
		pWnd = GetDlgItem(IDC_STATIC_MODULE);
		if(NULL != pWnd) pWnd->SetWindowText(_TR("Module") + _T(" : "));
	}

	CString rRegEx = _T("([a-zA-Z0-9])*") ;
	rRegEx = _T("([a-zA-Z0-9!@#$%^&*()_+])*");
	m_PasswordEdit.SetRegEx(rRegEx);
	CString rToolTip( _TR("Invalid character for password")) ;
	m_PasswordEdit.CreateToolTip(this , rToolTip);
	m_PasswordEdit.SetBackgroundColourFocus (RGB (150, 200, 255));
	m_PasswordEdit.SetForegroundColourError (RGB (255, 255, 255));
	m_PasswordEdit.SetBackgroundColourError (RGB (255, 0, 0));
	
	m_wndStaticMsg.SetGradientColor( GetSysColor(COLOR_BTNFACE) );
	m_wndStaticMsg.SetColor( GetSysColor(COLOR_BTNFACE) );
	m_wndStaticMsg.SetTextColor(RGB(255 , 0 , 0));

	TCHAR szBuf[MAX_PATH + 1]={'\0' ,};
	try
	{
		CString sAppPath = CFileTools::GetAppDataPath();
		m_sAppIniFilePath = IsTools::CPath::Combine(sAppPath , _T("SmartISO") , _T("SmartISO.ini") , 0);
		m_sDatabaseFilePath = IsTools::CPath::Combine(sAppPath , _T("SmartISO") , _T("Account.db") , 0);	/// 2015.08.19 added by humkyung

		LoadAppSettingFile();
		DisplayProjectInfo();
		OnSelchangeComboProject();
	}
	catch(const exception& ex)
	{
		m_wndStaticMsg.SetTextColor(RGB(255,0,0));
		m_wndStaticMsg.SetWindowText(ex.what());
	}
	
	TCHAR szModuleFileName[MAX_PATH]={'\0' ,};
	GetModuleFileName( AfxGetApp()->m_hInstance , szModuleFileName , MAX_PATH );
	CSplitPath splitPath( szModuleFileName );
	
	CAppDocData* pDocData = CAppDocData::GetInstance();
	pDocData->m_rExecPath = splitPath.GetDrive() + splitPath.GetDirectory();

	m_cboModule.SelectString(-1 , _T("User Module"));
	
	/// set project info. as last selected - 2013.01.13 added by humkyung
	if(GetPrivateProfileString(PRODUCT_NAME , _T("Project") , _T("") , szBuf , MAX_PATH , m_sAppIniFilePath))
	{
		const int iItem = m_wndProjectCombo.FindStringExact(0 , szBuf);
		(-1 != iItem) ? m_wndProjectCombo.SetCurSel(iItem) :m_wndProjectCombo.SetCurSel(0);
	}
	if(GetPrivateProfileString(PRODUCT_NAME , _T("User") , _T("") , szBuf , MAX_PATH , m_sAppIniFilePath))
	{
		const int iItem = m_UserCombo.FindStringExact(0 , szBuf);
		(-1 != iItem) ? m_UserCombo.SetCurSel(iItem) :m_UserCombo.SetCurSel(0);
		OnSelendokComboUser();
	}
	if(GetPrivateProfileString(PRODUCT_NAME , _T("Site") , _T("") , szBuf , MAX_PATH , m_sAppIniFilePath))
	{
		SetDlgItemText(IDC_EDIT_SITE , szBuf);
	}
	if(GetPrivateProfileString(PRODUCT_NAME , _T("Module") , _T("") , szBuf , MAX_PATH , m_sAppIniFilePath))
	{
		const int iItem = m_cboModule.FindStringExact(0 , szBuf);
		(-1 != iItem) ? m_cboModule.SetCurSel(iItem) : m_cboModule.SetCurSel(0);
	}
	/// up to here
	
	HICON hIcon = (HICON)::LoadImage( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_OK),IMAGE_ICON,16,16, 0 );
	m_okButton.SetIcon(hIcon);
	hIcon = (HICON)::LoadImage( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_CANCEL),IMAGE_ICON,	16,16, 0 );
	m_cancelButton.SetIcon(hIcon);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLoginDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLoginDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

using namespace soci;
/**
	@brief
	@author	humkyung
*/
void CLoginDlg::LoadAppSettingFile()
{
	CAppDocData* pDocData = CAppDocData::GetInstance();

	try
	{
		soci::session aSession(*soci::factory_sqlite3() , m_sDatabaseFilePath.operator LPCTSTR());
		soci::rowset<soci::row> rs(aSession.prepare << "select * from account");

		m_ProjectMap.clear();
		for(soci::rowset<row>::const_iterator itr = rs.begin();itr != rs.end();++itr)
		{
			CProjectDlg::ProjectInfo aPrjInfo;

			itr->uppercase_column_names(true);
			for(std::size_t i = 0;i < itr->size();++i)
			{
				const column_properties& props = itr->get_properties(i);
				string sColName = props.get_name();
				IsString::ToUpper(sColName);
				const std::string sValue = itr->get<std::string>(i);
				if(_T("PRJNAME") == sColName)
				{
					aPrjInfo.Name = sValue.c_str();
				}
				else if(_T("FOLDER") == sColName)
				{
					aPrjInfo.path = sValue.c_str();
				}
				else if(_T("USERNAME") == sColName)
				{
					aPrjInfo.UserName = sValue.c_str();
				}
				else if(_T("USERTYPE") == sColName)
				{
					const CString sUserType = sValue.c_str();
					aPrjInfo.UserType = (_T("Admin") == sUserType) ? CProjectDlg::eAdmin : CProjectDlg::eUser;
				}
				else if(_T("USERPASSWORD") == sColName)
				{
					aPrjInfo.UserPassword = sValue.c_str();
				}
			}
			if(!aPrjInfo.Name.IsEmpty() && !aPrjInfo.path.IsEmpty() && !aPrjInfo.UserName.IsEmpty())
			{
				map<CString,list<CProjectDlg::ProjectInfo>*>::const_iterator where = m_ProjectMap.find(aPrjInfo.Name);
				if(where == m_ProjectMap.end())
				{
					m_ProjectMap.insert(make_pair(aPrjInfo.Name , new list<CProjectDlg::ProjectInfo>));
				}
				m_ProjectMap[aPrjInfo.Name]->push_back(aPrjInfo);
			}
		}
	}
	catch(std::exception& ex)
	{
		m_wndStaticMsg.SetTextColor(RGB(255,0,0));
		m_wndStaticMsg.SetWindowText(ex.what());
	}
}

/******************************************************************************
	@brief
    @author     humkyung
    @date       2012-08-12
    @function   SaveAppSettingFile
    @return     void
******************************************************************************/
void CLoginDlg::SaveAppSettingFile()
{
	/// save login info. - 2013.01.14 added by humkyung
	int iItem = m_wndProjectCombo.GetCurSel();
	if(-1 != iItem)
	{
		CString rString;
		m_wndProjectCombo.GetLBText(iItem , rString);
		::WritePrivateProfileString(PRODUCT_NAME , _T("Project") , rString , m_sAppIniFilePath);
	}
	iItem = m_UserCombo.GetCurSel();
	if(-1 != iItem)
	{
		CString rString;
		m_UserCombo.GetLBText(iItem , rString);
		::WritePrivateProfileString(PRODUCT_NAME , _T("User") , rString , m_sAppIniFilePath);
	}

	CString rString;
	GetDlgItemText(IDC_EDIT_SITE , rString);
	WritePrivateProfileString(PRODUCT_NAME , _T("Site") , rString , m_sAppIniFilePath);
	
	iItem = m_cboModule.GetCurSel();
	if(-1 != iItem)
	{
		CString rString;
		m_cboModule.GetLBText(iItem , rString);
		WritePrivateProfileString(PRODUCT_NAME , _T("Module") , rString , m_sAppIniFilePath);
	}
	/// up to here

	/// set project information - 2016.10.07 added by humkyung
	/*CString rProject;
	m_wndProjectCombo.GetWindowText(rProject);
	m_UserCombo.GetWindowText(m_sId);
	m_PasswordEdit.GetWindowText(m_sPassword);
	GetDlgItemText(IDC_EDIT_SITE , m_sSite);

	list<CProjectDlg::ProjectInfo>* pPrjInfoList = m_ProjectMap[rProject.operator LPCSTR()];
	const CString sUserName(m_sId);
	list<CProjectDlg::ProjectInfo>::const_iterator where = find_if(pPrjInfoList->begin() , pPrjInfoList->end() , [&sUserName](CProjectDlg::ProjectInfo param){return param.UserName == sUserName;});
	CString sPrjPath(where->path);
	if(_T("\\") != sPrjPath.Right(1)) sPrjPath += _T("\\");
	m_sIniFilePath = sPrjPath + CString( _T("Setting\\") ) + rProject + CString( _T(".ini") );*/
	/// up to here

	/// save license information - 2016.10.07 added by humkyung
	CAppDocData* pDocData = CAppDocData::GetInstance();
	CString sLicenseId,sLicensePassword;
	GetDlgItemText(IDC_EDIT_SITE , sLicenseId);
	GetDlgItemText(IDC_EDIT_SITE_PASSWORD , sLicensePassword);
	pDocData->SetLicense(sLicenseId.operator LPCTSTR(),sLicensePassword.operator LPCTSTR());
	/// up to here
}

/**
	@brief
	@author	humkyung
**/
void CLoginDlg::OnDestroy() 
{
	CDialog::OnDestroy();
}

/**
	@brief	Project Name을 COMBO BOX에 표시한다.
	@author	humkyung
**/
void CLoginDlg::DisplayProjectInfo()
{
	m_wndProjectCombo.ResetContent();
	
	/// Delete all of the text from the combo box's edit control.
	m_wndProjectCombo.SetEditSel(0, -1);
	m_wndProjectCombo.Clear();

	vector<STRING_T> oResult;
	for(map<CString , list<CProjectDlg::ProjectInfo>*>::const_iterator itr = m_ProjectMap.begin();itr != m_ProjectMap.end();++itr)
	{
		m_wndProjectCombo.AddString(IDB_PROJECT , itr->first);
	}
	m_wndProjectCombo.SetCurSel(0);
}

BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN ) // Enter Key Prevent.. so good.. ^^		
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE )		
		{
			::TranslateMessage(pMsg);
			
			::DispatchMessage(pMsg);
			
			return TRUE;                    // DO NOT process further	
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
	
	
}

/**	
	@brief	프로젝트 , id , password를 확인하고 서브 프로그램을 실행시킨다.
	@author humkyung
	@date	????.??.??
	@pram	
	@return	void
*/
void CLoginDlg::OnOK() 
{
	CString rProject , rUser , rPassword;
	m_wndProjectCombo.GetWindowText(rProject);
	m_UserCombo.GetWindowText(rUser);
	m_PasswordEdit.GetWindowText(rPassword);

	////////////////////////////////////////////////////////////////////////////
	map<CString, list<CProjectDlg::ProjectInfo>*>::const_iterator where = m_ProjectMap.find(rProject);
	if(where != m_ProjectMap.end())
	{
		list<CProjectDlg::ProjectInfo>::const_iterator at = ::find_if(where->second->begin() , where->second->end() , [&rUser](CProjectDlg::ProjectInfo param){return param.UserName == rUser;});
		if((at != where->second->end()) && (CProjectDlg::eAdmin == at->UserType) && (rPassword != at->UserPassword))
		{
			m_wndStaticMsg.SetTextColor(RGB(255,0,0));
			m_wndStaticMsg.SetWindowText(_T("Password is wrong"));
			return;
		}	
	}

	if (0L == OnReceiveMsg())
	{
		CString sModule;
		const int iCurSel = m_cboModule.GetCurSel();
		if(-1 != iCurSel)
		{
			CString rString;
			m_cboModule.GetLBText(iCurSel , sModule);
			(_T("User Module") == sModule) ? CDialog::OnOK() : CDialog::OnCancel();
		}
	}
}

/**
	@brief  receive message form server
	@author humkyung
	@date   2013.06.27
*/
int CLoginDlg::OnReceiveMsg()
{
	CString rProject;
	m_wndProjectCombo.GetWindowText(rProject);
	if(ERROR_SUCCESS == CProjectDlg::CreateProjectIfNeed(rProject))
	{
		SaveAppSettingFile();

		CString sAppName;
		const int iCurSel = m_cboModule.GetCurSel();
		if(-1 != iCurSel)
		{
			CString rString;
			m_cboModule.GetLBText(iCurSel , rString);
			sAppName = (_T("User Module") == rString) ? _T("") : _T("SmartAdmin.exe");
		}
		
		CString rProject;
		m_wndProjectCombo.GetWindowText(rProject);
		m_UserCombo.GetWindowText(m_sId);
		m_PasswordEdit.GetWindowText(m_sPassword);

		list<CProjectDlg::ProjectInfo>* pPrjInfoList = m_ProjectMap[rProject.operator LPCSTR()];
		const CString sUserName(m_sId);
		list<CProjectDlg::ProjectInfo>::const_iterator where = find_if(pPrjInfoList->begin() , pPrjInfoList->end() , [&sUserName](CProjectDlg::ProjectInfo param){return param.UserName == sUserName;});
		CString sPrjPath(where->path);
		if(_T("\\") != sPrjPath.Right(1)) sPrjPath += _T("\\");
		m_sIniFilePath = sPrjPath + CString( _T("Setting\\") ) + rProject + CString( _T(".ini") );

		if(!sAppName.IsEmpty())
		{
			CString rPathName;
			CFileTools::GetExecutableDirectory(rPathName);
			const int at = rPathName.ReverseFind('\\');
			if(at) rPathName = rPathName.Left(at);

			const CString rParameter = _T("\"") + m_sIniFilePath + _T("\"?") + rProject + _T(";") + m_sId;
			HINSTANCE hInst = ::ShellExecute(NULL, _T("open") , _T("\"") + rPathName + _T("\\") + sAppName + _T("\"") , rParameter , rPathName , SW_SHOWNORMAL);
			if((int)hInst < 32)
			{
				CString rString;
				if(ERROR_FILE_NOT_FOUND == (int)hInst)
				{
					AfxMessageBox(_T("Can't find Admin Application"));
				}
				else
				{
					AfxMessageBox(_T("Fail to run Admin Application"));
				}
			}
		}

		return 0L;
	}
	else
	{
		m_okButton.EnableWindow(TRUE);
	}

	return -1L;
}

/******************************************************************************
    @author     humkyung
    @date       ????-??-??
    @class      CLoginDlg
    @function   LoadBMPImage
    @return     BOOL
    @param      LPCTSTR     sBMPFile
    @param      CBitmap     *bitmap
    @param      CPalette    *pPal
    @brief
******************************************************************************/
BOOL CLoginDlg::LoadBMPImage(LPCTSTR sBMPFile, CBitmap *bitmap, CPalette *pPal)
{
	CFile file;
	if( !file.Open( sBMPFile, CFile::modeRead) ) return FALSE;
	
	BITMAPFILEHEADER bmfHeader;
	
	// Read file header
	if (file.Read((LPSTR)&bmfHeader, sizeof(bmfHeader)) != sizeof(bmfHeader))
		return FALSE;
	
	// File type should be 'BM'
	if (bmfHeader.bfType != ((WORD) ('M' << 8) | 'B'))
		return FALSE;
	
	// Get length of the remainder of the file and allocate memory
	DWORD nPackedDIBLen = file.GetLength() - sizeof(BITMAPFILEHEADER);
	HGLOBAL hDIB = ::GlobalAlloc(GMEM_FIXED, nPackedDIBLen);
	if (hDIB == 0)
		return FALSE;
	
	// Read the remainder of the bitmap file.
	if (file.Read((LPSTR)hDIB, nPackedDIBLen) != nPackedDIBLen )
	{
		::GlobalFree(hDIB);
		return FALSE;
	}
	
	
	BITMAPINFOHEADER &bmiHeader = *(LPBITMAPINFOHEADER)hDIB ;
	BITMAPINFO &bmInfo = *(LPBITMAPINFO)hDIB ;
	
	// If bmiHeader.biClrUsed is zero we have to infer the number
	// of colors from the number of bits used to specify it.
	int nColors = bmiHeader.biClrUsed ? bmiHeader.biClrUsed : 
	1 << bmiHeader.biBitCount;
	
	LPVOID lpDIBBits;
	if( bmInfo.bmiHeader.biBitCount > 8 )
		lpDIBBits = (LPVOID)((LPDWORD)(bmInfo.bmiColors + bmInfo.bmiHeader.biClrUsed) + 
		((bmInfo.bmiHeader.biCompression == BI_BITFIELDS) ? 3 : 0));
	else
		lpDIBBits = (LPVOID)(bmInfo.bmiColors + nColors);
	
	// Create the logical palette
	if( pPal != NULL )
	{
		// Create the palette
		if( nColors <= 256 )
		{
			UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * nColors);
			LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];
			
			pLP->palVersion = 0x300;
			pLP->palNumEntries = nColors;
			
			for( int i=0; i < nColors; i++)
			{
				pLP->palPalEntry[i].peRed = bmInfo.bmiColors[i].rgbRed;
				pLP->palPalEntry[i].peGreen = bmInfo.bmiColors[i].rgbGreen;
				pLP->palPalEntry[i].peBlue = bmInfo.bmiColors[i].rgbBlue;
				pLP->palPalEntry[i].peFlags = 0;
			}
			
			pPal->CreatePalette( pLP );
			
			delete[] pLP;
		}
	}
	
	CClientDC dc(NULL);
	CPalette* pOldPalette = NULL;
	if( pPal )
	{
		pOldPalette = dc.SelectPalette( pPal, FALSE );
		dc.RealizePalette();
	}
	
	HBITMAP hBmp = CreateDIBitmap( dc.m_hDC,                // handle to device context 
		&bmiHeader,     // pointer to bitmap size and format data 
		CBM_INIT,       // initialization flag 
		lpDIBBits,      // pointer to initialization data 
		&bmInfo,        // pointer to bitmap color-format data 
		DIB_RGB_COLORS);                // color-data usage 
	bitmap->Attach( hBmp );
	
	if( pOldPalette )
		dc.SelectPalette( pOldPalette, FALSE );
	
	::GlobalFree(hDIB);
	//                 bitmap->Attach(hBmp); // bug fix by Jeremy Blake
	return TRUE;
}

/**	
	@brief	system database를 클릭했을때 호출된다.
	@author	humkyung
	@param	pNMHDR	a parameter of type NMHDR*
	@param	pResult	a parameter of type LRESULT*
	@return	void	
*/
BOOL CLoginDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	NMHDR* pNMHDR = (NMHDR*)(lParam);
	if(FILTER_EDIT_RETURN == wParam)
	{	
		if(pNMHDR->hwndFrom == GetDlgItem(IDC_EDIT_PASSWORD)->m_hWnd)
		{
			OnOK();
		}
	}
	
	return CDialog::OnNotify(wParam, lParam, pResult);
}

/**
	@brief	PROJECT를 선택했을 때...
	@author	humkyung
*/
void CLoginDlg::OnSelchangeComboProject() 
{
	const int at = m_wndProjectCombo.GetCurSel();
	if(-1 != at)
	{
		CString sProject;
		m_wndProjectCombo.GetLBText(at , sProject);

		map<CString, list<CProjectDlg::ProjectInfo>*>::const_iterator where = m_ProjectMap.find(sProject);
		if(where != m_ProjectMap.end())
		{
			m_UserCombo.ResetContent();
			m_UserCombo.SetEditSel(0, -1);
			m_UserCombo.Clear();
			for(list<CProjectDlg::ProjectInfo>::const_iterator itr = where->second->begin();itr != where->second->end();++itr)
			{
				if(-1 == m_UserCombo.FindStringExact(0 , itr->UserName)) m_UserCombo.AddString(itr->UserName);
			}
		}
	}
}

/**
	@brief	when select user
	@author	humkyung
*/
void CLoginDlg::OnSelendokComboUser() 
{
	const int nCurSel = m_UserCombo.GetCurSel();
	if(-1 != nCurSel)
	{
		CString sProject , sUserName;
		m_wndProjectCombo.GetWindowText(sProject);
		m_UserCombo.GetLBText(nCurSel , sUserName);

		list<CProjectDlg::ProjectInfo>* pPrjInfoList = m_ProjectMap[sProject];
		list<CProjectDlg::ProjectInfo>::const_iterator where = find_if(pPrjInfoList->begin() , pPrjInfoList->end() , [&sUserName](CProjectDlg::ProjectInfo param){return param.UserName == sUserName;});
		if(CProjectDlg::eAdmin == where->UserType)
		{
			m_okButton.EnableWindow(TRUE);
			m_PasswordEdit.EnableWindow(TRUE);
			m_cboModule.EnableWindow(TRUE);
		}
		else
		{
			m_PasswordEdit.EnableWindow(FALSE);
			m_cboModule.SelectString(-1 , _T("User Module"));
			m_cboModule.EnableWindow(FALSE);
		}
		m_PasswordEdit.SetFocus();
	}
}

/**
	@breif	
	@author	humkyung
	@date	2009.10.28
	@param
	@return
*/
void CLoginDlg::OnButtonProjectSetup() 
{
	CProjectDlg dlg;
	if(IDOK == dlg.DoModal())
	{
		this->LoadAppSettingFile();	/// 2012.01.26 added by humkyung
		this->DisplayProjectInfo();
	}
}

/******************************************************************************
    @author     humkyung
    @date       2012-05-26
    @function   OnHelpInfo
    @return     BOOL
    @param      HELPINFO*   pHelpInfo
    @brief
******************************************************************************/
BOOL CLoginDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	CString sHelpPage(CAppDocData::GetExecPath() + _T("\\Help\\SmartISO.chm::/Project Log-in.htm"));
	HWND hHtml = ::HtmlHelp(this->m_hWnd, sHelpPage , HH_DISPLAY_TOPIC, 0);

	return TRUE;///CDialog::OnHelpInfo(pHelpInfo);
}

/**
	@brief	reset activate code
	@author	humkyung
	@date	2014.05.23
*/
/*
void CLoginDlg::OnBnClickedButtonResetActivateCode()
{
	TCHAR szBuf[206]={'\0',};

	Packet packet;
	InitializePacket(&packet);
	packet.Code = REQ_RESET_ACTIVATE_CODE;

	USES_CONVERSION;
	CString rString;
	GetDlgItemText(IDC_EDIT_SITE , rString);
	strcpy((char*)(packet.ID) , T2CA(rString));
	GetDlgItemText(IDC_EDIT_SITE_PASSWORD , rString);
	strcpy((char*)(packet.Password) , T2CA(rString));
	
	GetDlgItem(IDC_BUTTON_RESET_ACTIVATE_CODE)->EnableWindow(FALSE);
	m_okButton.EnableWindow(FALSE);
	{
		LMS_ENCODE lms;
		lms.size = sizeof(Packet);
		memcpy(lms.buf,&packet,sizeof(Packet));
		Encode(&lms);

		_ns1__Reset request;
		{
			request.oLMS = new ns1__LMSStuctType();
			request.oLMS->buf = new xsd__base64Binary();
			request.oLMS->buf->__ptr = new unsigned char[LMS_BUF_SIZE];
			memcpy(request.oLMS->buf->__ptr,lms.buf,lms.size);
			request.oLMS->buf->__size = LMS_BUF_SIZE;
			request.oLMS->size = lms.size;
		}
		_ns1__ResetResponse response;

		LicWebServiceSoapProxy soapProxy;
		if(SOAP_OK == soapProxy.Reset(LICENDPOINT,LICENDPOINT"/Reset",&request,response))
		{
		}
		delete request.oLMS->buf->__ptr;
		delete request.oLMS->buf;
		delete request.oLMS;
	}
	GetDlgItem(IDC_BUTTON_RESET_ACTIVATE_CODE)->EnableWindow(TRUE);
	m_okButton.EnableWindow(TRUE);

	CString sAddress(_T("127.0.0.1"));
	DWORD dwPortNo = 2002;
	CRegistry registry;
	{
		registry.Open(HKEY_LOCAL_MACHINE , _T("Software\\") + CString(PUBLISHER) + _T("\\SmartISO\\License"));
		registry.Read(_T("IP") , sAddress);
		registry.Read(_T("Port") , dwPortNo);
		registry.Close();
	}

	if(TRUE == m_oClientSocket.Init(sAddress , dwPortNo))
	{
		TCHAR szBuf[206]={'\0',};

		m_oClientSocket.AttachWindow(this->GetSafeHwnd());

		Packet packet;
		InitializePacket(&packet);
		packet.Code = REQ_RESET_ACTIVATE_CODE;

		USES_CONVERSION;
		CString rString;
		GetDlgItemText(IDC_EDIT_SITE , rString);
		strcpy((char*)(packet.ID) , T2CA(rString));
		GetDlgItemText(IDC_EDIT_SITE_PASSWORD , rString);
		strcpy((char*)(packet.Password) , T2CA(rString));
		strcpy((char*)(packet.AppName) , ("SmartISO"));
		packet.Major = 1;packet.Minor=0;packet.Maintenance = 0;packet.Build = 0;

		vector<STRING_T> oResult;
		CAppDocData* pDocData = CAppDocData::GetInstance();
		CTokenizer<CIsComma>::Tokenize(oResult , pDocData->GetFileVersion().operator LPCTSTR() , CIsComma());
		if(4 == oResult.size())
		{
			packet.Major = ATOI_T(oResult[0].c_str());
			packet.Minor = ATOI_T(oResult[1].c_str());
			packet.Maintenance = ATOI_T(oResult[2].c_str());
			packet.Build = ATOI_T(oResult[3].c_str());
		}
		GetDlgItem(IDC_BUTTON_RESET_ACTIVATE_CODE)->EnableWindow(FALSE);
		m_okButton.EnableWindow(FALSE);
		m_oClientSocket.Send(&packet);
	}
}
*/