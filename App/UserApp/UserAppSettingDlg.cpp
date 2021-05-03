// UserAppSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UserApp.h"
#include "UserAppSettingDlg.h"
#include "UserAppView.h"
#include "enablebuddybutton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString CUserAppSettingPageDlg::
/////////////////////////////////////////////////////////////////////////////
// CUserAppSettingDlg property page

IMPLEMENT_DYNCREATE(CUserAppSettingDlg, CPropertyPage)

CUserAppSettingDlg::CUserAppSettingDlg() : CPropertyPage(CUserAppSettingDlg::IDD)
{
	//{{AFX_DATA_INIT(CUserAppSettingDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CUserAppSettingDlg::~CUserAppSettingDlg()
{
}

void CUserAppSettingDlg::DoDataExchange(CDataExchange* pDX)
{	
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX , IDC_EDIT_MSTATION_PATH , m_MStationPath);
	//{{AFX_DATA_MAP(CUserAppSettingDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUserAppSettingDlg, CPropertyPage)
	//{{AFX_MSG_MAP(CUserAppSettingDlg)
	ON_BN_CLICKED(IDC_BUTTON_MSTATION, OnMStationOpen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserAppSettingDlg message handlers

void CUserAppSettingDlg::OnMStationOpen() 
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE); dlg.DoModal();
	m_MStationPath = dlg.GetPathName();
	SetDlgItemText(IDC_EDIT1, (LPSTR)(LPCTSTR)m_MStationPath);
}

BOOL CUserAppSettingDlg::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	HWND hwndEdit  = GetDlgItem(IDC_EDIT_MSTATION_PATH)->GetSafeHwnd();
	HWND hwndButton= GetDlgItem(IDC_BUTTON_MSTATION)->GetSafeHwnd();
	VERIFY( ::EnableBuddyButton( hwndEdit, hwndButton, BBS_RIGHT ) );

	hwndEdit  = GetDlgItem(IDC_EDIT_PRINTER)->GetSafeHwnd();
	hwndButton= GetDlgItem(IDC_BUTTON_PRINTER)->GetSafeHwnd();
	VERIFY( ::EnableBuddyButton( hwndEdit, hwndButton, BBS_RIGHT ) );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
