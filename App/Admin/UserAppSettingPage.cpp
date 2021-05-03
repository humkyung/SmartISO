// UserAppSettingPage.cpp : implementation file
//

#include "stdafx.h"
#include "Admin.h"
#include "UserAppSettingPage.h"
#include "enablebuddybutton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString CUserAppSettingPage::APP( _T("Iso Edit Setting") );
CString CUserAppSettingPage::m_MStationPath;
/////////////////////////////////////////////////////////////////////////////
// CUserAppSettingPage property page

IMPLEMENT_DYNCREATE(CUserAppSettingPage, CPropertyPage)

CUserAppSettingPage::CUserAppSettingPage() : CPropertyPage(CUserAppSettingPage::IDD)
{
	//{{AFX_DATA_INIT(CUserAppSettingPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CUserAppSettingPage::~CUserAppSettingPage()
{
}

void CUserAppSettingPage::DoDataExchange(CDataExchange* pDX)
{	
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX , IDC_EDIT_MSTATION_PATH , m_MStationPath);
	//{{AFX_DATA_MAP(CUserAppSettingPage)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserAppSettingPage, CPropertyPage)
	//{{AFX_MSG_MAP(CUserAppSettingPage)
	ON_BN_CLICKED(IDC_BUTTON_MSTATION, OnMStationOpen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserAppSettingPage message handlers

void CUserAppSettingPage::OnMStationOpen() 
{
	CFileDialog dlg(TRUE); dlg.DoModal();
	m_MStationPath = dlg.GetPathName();
	SetDlgItemText(IDC_EDIT_MSTATION_PATH, (LPSTR)(LPCTSTR)m_MStationPath);
}

BOOL CUserAppSettingPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	HWND hwndEdit  = GetDlgItem(IDC_EDIT_MSTATION_PATH)->GetSafeHwnd();
	HWND hwndButton= GetDlgItem(IDC_BUTTON_MSTATION)->GetSafeHwnd();
	VERIFY( ::EnableBuddyButton( hwndEdit, hwndButton, BBS_RIGHT ) );

	CAdminApp *pApp = static_cast<CAdminApp*>(AfxGetApp()) ;
	ASSERT(pApp) ;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/**	
	@brief	The CUserAppSettingPage::LoadData function

	@author	HumKyung

	@date	????.??.??

  	@param	rIniFilePath	a parameter of type const CString&

	@return	void	
*/
void CUserAppSettingPage::LoadData( const CString& rIniFilePath )
{
	char szBuf[MAX_PATH]={'\0',};
	if(GetPrivateProfileString(CUserAppSettingPage::APP , _T("MStation Path") , NULL , szBuf , MAX_PATH , rIniFilePath))
	{
		m_MStationPath = szBuf;
	}
}

/**	
	@brief	설정한 값들을 저장한다.

	@author	HumKyung

	@date	????.??.??

	@param	rIniFilePath	a parameter of type const CString&

	@return	void	
*/
void CUserAppSettingPage::SaveData( const CString& rIniFilePath )
{
	WritePrivateProfileString(CUserAppSettingPage::APP , _T("MStation Path") , m_MStationPath , rIniFilePath);	
}
