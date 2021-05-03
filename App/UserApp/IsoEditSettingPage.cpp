// IsoEditSettingPage.cpp : implementation file
//

#include "stdafx.h"
#include "UserApp.h"
#include "IsoEditSettingPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString CIsoEditSettingPage::APP( "Iso Edit Setting" );
CString CIsoEditSettingPage::m_rFileNameExt="i01";
/////////////////////////////////////////////////////////////////////////////
// CIsoEditSettingPage property page

IMPLEMENT_DYNCREATE(CIsoEditSettingPage, CPropertyPage)

CIsoEditSettingPage::CIsoEditSettingPage() : CPropertyPage(CIsoEditSettingPage::IDD)
{
	//{{AFX_DATA_INIT(CIsoEditSettingPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CIsoEditSettingPage::~CIsoEditSettingPage()
{
}

void CIsoEditSettingPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIsoEditSettingPage)
	DDX_Text( pDX , IDC_EDIT_FILE_NAME_EXT , m_rFileNameExt );
	DDX_Control( pDX , IDC_EDIT_FILE_NAME_EXT , m_fileExtFilterEdit );
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIsoEditSettingPage, CPropertyPage)
	//{{AFX_MSG_MAP(CIsoEditSettingPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIsoEditSettingPage message handlers

BOOL CIsoEditSettingPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_fileExtFilterEdit.SetRegEx( "[^\\\\/:*?\"<>|]+" );

	m_fileExtFilterEdit.CreateToolTip(this , CStringHelper::GetInstance().GetHelpString( IDS_NOT_ADMITTED_CHAR ) );
	m_fileExtFilterEdit.SetBackgroundColourFocus (RGB (150, 200, 255));
	m_fileExtFilterEdit.SetForegroundColourError (RGB (255, 255, 255));
	m_fileExtFilterEdit.SetBackgroundColourError (RGB (255, 0, 0));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/**	
	@brief	The CIsoEditSettingPage::LoadData function
	
	@author	humkyung

	@param	rIniFilePath	a parameter of type const CString&

	@return	void	
*/
void CIsoEditSettingPage::LoadData( const CString& rIniFilePath)
{
	char szBuf[MAX_PATH] = {'\0' ,};	
	if( GetPrivateProfileString( CIsoEditSettingPage::APP , _T("Source File Name Ext") , NULL , szBuf , MAX_PATH , rIniFilePath) )
	{
		CIsoEditSettingPage::m_rFileNameExt = szBuf;
	}
}

/**	
	@brief	The CIsoEditSettingPage::SaveData function
	
	@author	πÈ»Ï∞Ê

	@param	rIniFilePath	a parameter of type const CString &

	@return	void	
*/
void CIsoEditSettingPage::SaveData(const CString &rIniFilePath)
{
	UpdateData( );
	WritePrivateProfileString( CIsoEditSettingPage::APP , _T("Source File Name Ext") , CIsoEditSettingPage::m_rFileNameExt , rIniFilePath);
}
