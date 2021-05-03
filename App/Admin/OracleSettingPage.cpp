// OracleSettingPage.cpp : implementation file
//

#include "stdafx.h"
#include "Admin.h"
#include "AdminDocData.h"
#include "OracleSettingPage.h"
#include <ado/AdoRemoteDB.h>

#include <tokenizer.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COracleSettingPage property page

IMPLEMENT_DYNCREATE(COracleSettingPage, CPropertyPage)

COracleSettingPage::COracleSettingPage() : CPropertyPage(COracleSettingPage::IDD)
{
	//{{AFX_DATA_INIT(COracleSettingPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

COracleSettingPage::~COracleSettingPage()
{
}

void COracleSettingPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COracleSettingPage)
		DDX_Text(pDX , IDC_EDIT_SERVICE_NAME , m_rServiceName);
		DDX_Text(pDX , IDC_EDIT_USER_NAME , m_rUserName);
		DDX_Text(pDX , IDC_EDIT_PASSWORD , m_rPassword);
		DDX_Text(pDX , IDC_EDIT_PDS_PRO_NAME , m_rPDSProjectName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COracleSettingPage, CPropertyPage)
	//{{AFX_MSG_MAP(COracleSettingPage)
	ON_BN_CLICKED(IDC_BUTTON_CONNECT_TEST, OnButtonConnectTest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COracleSettingPage message handlers

void COracleSettingPage::SaveData(const CString& rIniFilePath)
{
	UpdateData(TRUE);

	CString rApp("Oracle Setting");
	WritePrivateProfileString(rApp , "Service Name", m_rServiceName , rIniFilePath);
	WritePrivateProfileString(rApp , "User Name" , m_rUserName , rIniFilePath);
	WritePrivateProfileString(rApp , "Password" , m_rPassword , rIniFilePath);
	WritePrivateProfileString(rApp , "PDS Project Name" , m_rPDSProjectName , rIniFilePath);
}

void COracleSettingPage::LoadData(const CString &rIniFilePath)
{
	char szBuf[MAX_PATH] = {'\0' ,};

	CString rApp("Oracle Setting");
	if(GetPrivateProfileString(rApp , "Service Name" , NULL , szBuf , MAX_PATH , rIniFilePath))
	{
		m_rServiceName = szBuf;
	}

	if(GetPrivateProfileString(rApp , "User Name" , NULL , szBuf , MAX_PATH , rIniFilePath))
	{
		m_rUserName = szBuf;
	}

	if(GetPrivateProfileString(rApp , "Password" , NULL , szBuf , MAX_PATH , rIniFilePath))
	{
		m_rPassword = szBuf;
	}
	if(GetPrivateProfileString(rApp , "PDS Project Name" , NULL , szBuf , MAX_PATH , rIniFilePath))
	{
		m_rPDSProjectName = szBuf;
	}
}

/**
**/
void COracleSettingPage::OnButtonConnectTest() 
{
	// TODO: Add your control notification handler code here
	CString UserName, PassWord,ServiceName;
	CEdit *edit;
	edit = (CEdit *)GetDlgItem(IDC_EDIT_SERVICE_NAME);
	edit->GetWindowText(ServiceName);
	edit = (CEdit*)GetDlgItem(IDC_EDIT_USER_NAME);
	edit->GetWindowText(UserName);
	edit = (CEdit*)GetDlgItem(IDC_EDIT_PASSWORD);
	edit->GetWindowText(PassWord);
	CAdoRemoteDB oraDB;
	CString	strDBPath = "provider=MSDAORA;User ID="+ UserName +";Password="+ PassWord + ";Data Source="+ServiceName;
	if(TRUE == oraDB.DBConnect(strDBPath))
	{
		AfxMessageBox("Connection Success!!");
	}
	else
	{
		AfxMessageBox("Connection Fail!!");
	}
	oraDB.DBDisConnect();
}
