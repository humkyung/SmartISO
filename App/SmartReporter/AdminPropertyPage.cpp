// AdminPropertyPage.cpp : implementation file
//

#include "stdafx.h"
#include "SmartReporter.h"
#include "AdminPropertyPage.h"
#include "LineListToOraDocData.h"
#include <ado/AdoRemoteDB.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdminPropertyPage property page

IMPLEMENT_DYNCREATE(CAdminPropertyPage, CPropertyPage)

CAdminPropertyPage::CAdminPropertyPage() : CPropertyPage(CAdminPropertyPage::IDD)
{
	//{{AFX_DATA_INIT(CAdminPropertyPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CAdminPropertyPage::~CAdminPropertyPage()
{
}

void CAdminPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdminPropertyPage)
	///DDX_Text(pDX , IDC_EDIT_NAME , m_rUserName);
	DDX_Text(pDX , IDC_EDIT_PASSWORD , m_rAdminPassword);
	DDX_Text(pDX , IDC_EDIT_SERVICE_NAME , m_rServiceName);
	DDX_Text(pDX , IDC_EDIT_USER_NAME , m_rUserName);
	DDX_Text(pDX , IDC_EDIT_ORA_PASSWORD , m_rUserPassword);
	DDX_Text(pDX , IDC_EDIT_PDS_PRO_NAME , m_rProjectName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAdminPropertyPage, CPropertyPage)
	//{{AFX_MSG_MAP(CAdminPropertyPage)
		ON_BN_CLICKED(IDC_BUTTON_CONNECT_TEST, OnButtonConnectTest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdminPropertyPage message handlers
BOOL CAdminPropertyPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
///	GetDlgItem(IDC_EDIT_NAME)->SetFocus();
	CString rString;
	rString.LoadString(IDS_ADMINISTRATOR);
	SetDlgItemText( IDC_EDIT_NAME , rString );

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/**	
	@brief	데이터를 저장한다.

	@author	humkyung

	@date	????.??.??

	@param	rIniFilePath	a parameter of type const CString&

	@return	void	
*/
void CAdminPropertyPage::SaveData( const CString& rIniFilePath )
{
	static const STRING_T APP(_T("General"));

	UpdateData( TRUE );

	CLineListToOraDocData& docData = CLineListToOraDocData::GetInstance();
	docData.SetAdminPassword( m_rAdminPassword.operator LPCTSTR() );

	///! 2011.04.26 - added by humkyung
	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_INCH);
	if((NULL != pButton) && pButton->GetCheck())
	{
		WritePrivateProfileString(APP.c_str() , _T("Project Unit") , _T("INCH") , rIniFilePath);
	}
	pButton = (CButton*)GetDlgItem(IDC_RADIO_METRIC);
	if((NULL != pButton) && pButton->GetCheck())
	{
		WritePrivateProfileString(APP.c_str() , _T("Project Unit") , _T("METRIC") , rIniFilePath);
	}

	///string rIniFilePath = docData.GetIniFilePath();
	WritePrivateProfileString(APP.c_str() , _T("Service Name") , m_rServiceName , rIniFilePath);
	WritePrivateProfileString(APP.c_str() , _T("User Name") , m_rUserName , rIniFilePath);
	WritePrivateProfileString(APP.c_str() , _T("User Password") , m_rUserPassword , rIniFilePath);
}

/**	\brief	ORACLE CONNECTION TEST를 실시한다.
**/
void CAdminPropertyPage::OnButtonConnectTest() 
{
	// TODO: Add your control notification handler code here
	CString UserName, PassWord,ServiceName;
	CEdit *edit = NULL;
	edit = (CEdit *)GetDlgItem(IDC_EDIT_SERVICE_NAME);
	edit->GetWindowText(ServiceName);
	edit = (CEdit*)GetDlgItem(IDC_EDIT_USER_NAME);
	edit->GetWindowText(UserName);
	edit = (CEdit*)GetDlgItem(IDC_EDIT_ORA_PASSWORD);
	edit->GetWindowText(PassWord);
	
	///OracleConnection conn;

	/*
	<%
		conStr = "Provider=MSDAORA.1;User ID=아이디;Password=비번;Data Source=xxx.xxx.xxx.xxx;Persist Security Info=False;"
	%>
	OraOLEDB.Oracle
	*/

	CAdoRemoteDB oraDB;
	CString	strDBPath = _T("Provider=MSDAORA;User ID=") + UserName + _T(";Password=") + PassWord + _T(";Data Source=") + ServiceName;
	if(TRUE == oraDB.DBConnect(strDBPath))
	{
		AfxMessageBox(_T("Connection Success!!"));
	}
	else
	{
		AfxMessageBox(_T("Connection Fail!!"));
	}
	oraDB.DBDisConnect();
}

/**
	@brief	

	@author	humkyung

	@date	????.??.??

	@param	rIniFilePath

	@return
**/
void CAdminPropertyPage::LoadData(const CString &rIniFilePath)
{
	static const STRING_T APP( _T("General") );
	CHAR_T szBuf[MAX_PATH] = {'\0' ,};

	//! 2011.04.26 - added by humkyung
	if(GetPrivateProfileString(APP.c_str() , _T("Project Unit") , NULL , szBuf , MAX_PATH , rIniFilePath))
	{
		
		if(0 == STRICMP_T(_T("INCH") , szBuf))
		{
			CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_INCH);
			if(pButton) pButton->SetCheck(TRUE);
		}
		else if(0 == STRICMP_T(_T("METRIC") , szBuf))
		{
			CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_METRIC);
			if(pButton) pButton->SetCheck(TRUE);
		}
		else
		{
			CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_INCH);
			if(pButton) pButton->SetCheck(TRUE);
		}
	}
	//!

	if(GetPrivateProfileString(APP.c_str() , _T("Project") , NULL , szBuf , MAX_PATH , rIniFilePath))
	{
		m_rProjectName = szBuf;
	}else	m_rProjectName.Empty();

	if(GetPrivateProfileString(APP.c_str() , _T("Service Name") , NULL , szBuf , MAX_PATH , rIniFilePath))
	{
		m_rServiceName = szBuf;
	}else	m_rServiceName.Empty();

	if(GetPrivateProfileString(APP.c_str() , _T("User Name") , NULL , szBuf , MAX_PATH , rIniFilePath))
	{
		m_rUserName = szBuf;
	}else	m_rUserName.Empty();

	if(GetPrivateProfileString(APP.c_str() , _T("User Password") , NULL , szBuf , MAX_PATH , rIniFilePath))
	{
		m_rUserPassword = szBuf;
	}else	m_rUserPassword.Empty();

	UpdateData(FALSE);
}
