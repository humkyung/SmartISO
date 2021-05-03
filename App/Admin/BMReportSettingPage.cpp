// BMReportSettingPage.cpp : implementation file
//

#include "stdafx.h"
#include "Admin.h"
#include "BMReportSettingPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString CBMReportSettingPage::APP("B/M Report Setting");
CString CBMReportSettingPage::m_rPipeCutLength( "6000" );	///< 기본값 6M
/////////////////////////////////////////////////////////////////////////////
// CBMReportSettingPage property page

IMPLEMENT_DYNCREATE(CBMReportSettingPage, CPropertyPage)

CBMReportSettingPage::CBMReportSettingPage() : CPropertyPage(CBMReportSettingPage::IDD) , m_bETC(FALSE)
{
	//{{AFX_DATA_INIT(CBMReportSettingPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBMReportSettingPage::~CBMReportSettingPage()
{
}

void CBMReportSettingPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBMReportSettingPage)
	DDX_Check(pDX , IDC_CHECK_ETC , m_bETC);
	DDX_Text(pDX , IDC_EDIT_PIPE_CUT_LENGTH , m_rPipeCutLength);
	DDX_Control(pDX , IDC_EDIT_PIPE_CUT_LENGTH , m_wndPipeCutLengthEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBMReportSettingPage, CPropertyPage)
	//{{AFX_MSG_MAP(CBMReportSettingPage)
	ON_BN_CLICKED(IDC_CHECK_ETC, OnCheckEtc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBMReportSettingPage message handlers

/**
	@brief	
	@author	백흠경
**/
void CBMReportSettingPage::LoadData( const CString& rIniFilePath )
{
	TCHAR szBuf[MAX_PATH + 1] = {'\0' ,};
	if(GetPrivateProfileString(CBMReportSettingPage::APP , _T("Pipe Cut Length") ,_T(""), szBuf , MAX_PATH , rIniFilePath))
	{
		m_rPipeCutLength = szBuf;
		const int at = m_rPipeCutLength.Find(".");
		if( -1 != at) m_rPipeCutLength.TrimRight("0");
	}
}

/**	
	@brief	세팅 파일로 저장한다.
	@param	rIniFilePath	a parameter of type const CString&
	@return	void	
*/
void CBMReportSettingPage::SaveData(const CString& rIniFilePath)
{
	UpdateData( TRUE );

	if(FALSE == WritePrivateProfileString(CBMReportSettingPage::APP , _T("Pipe Cut Length") , m_rPipeCutLength , rIniFilePath))
	{
		AfxMessageBox("");
		return;
	}
}

/**	
	@brief	The CBMReportSettingPage::GetPipeCutLength function
	@author	백흠경
	@return	double	
*/
double CBMReportSettingPage::GetPipeCutLength()
{
	return atof( CBMReportSettingPage::m_rPipeCutLength );
}

/**	
	@brief	ETC가 SET이 되었으면 PIPE CUT LEGNTH EDIT를 활성화 시킨다.
	@author	백흠경
**/
void CBMReportSettingPage::OnCheckEtc() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_wndPipeCutLengthEdit.EnableWindow(m_bETC);
}

/**
	@breif	
	@author	백흠경
**/
BOOL CBMReportSettingPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	m_wndPipeCutLengthEdit.EnableWindow(m_bETC);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
