// UserPropertyPage.cpp : implementation file
//

#include "stdafx.h"
#include "Admin.h"
#include "UserPropertyPage.h"
#include "AdminDocData.h"

#include <util/FileTools.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserPropertyPage property page

IMPLEMENT_DYNCREATE(CUserPropertyPage, CPropertyPage)

CUserPropertyPage::CUserPropertyPage() : CPropertyPage(CUserPropertyPage::IDD)
{
	//{{AFX_DATA_INIT(CUserPropertyPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CUserPropertyPage::~CUserPropertyPage()
{
}

void CUserPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUserPropertyPage)
	DDX_Text(pDX , IDC_EDIT_USER_ID  , m_rID);
	DDX_Text(pDX , IDC_EDIT_PASSWORD , m_rUserPassword);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CUserPropertyPage, CPropertyPage)
	//{{AFX_MSG_MAP(CUserPropertyPage)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserPropertyPage message handlers
BOOL CUserPropertyPage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	GetDlgItem(IDC_EDIT_PASSWORD)->SetFocus();
	
	m_rUserPassword = CAdminDoc::GetInstance()->GetCurProject()->GetAdminPassword().c_str();

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/**	
	@brief	The CUserPropertyPage::SaveData function
	
	@author	HumKyung

	@param	rIniFilePath	a parameter of type const CString&

	@date	????.??.??

	@return	void	
*/
void CUserPropertyPage::SaveData( const CString& rIniFilePath )
{
	UpdateData( TRUE );

	CAdminDoc::GetInstance()->GetCurProject()->SaveAdminPassword(m_rUserPassword.operator LPCTSTR());
}