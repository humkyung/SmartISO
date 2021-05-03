// SmartAdminSettingSheet.cpp : implementation file
//

#include "stdafx.h"
#include "Admin.h"
#include <Project.h>
#include "SmartAdminSettingSheet.h"
#include "ResourceSwitch.h"
#include "AdminDocData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSmartAdminSettingSheet

IMPLEMENT_DYNAMIC(CSmartAdminSettingSheet, CTreePropSheet)

CSmartAdminSettingSheet::CSmartAdminSettingSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CTreePropSheet(nIDCaption, pParentWnd, iSelectPage) , m_pCurProject(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	AddPropPages();
}

CSmartAdminSettingSheet::~CSmartAdminSettingSheet()
{
	try
	{
		RemovePropPages();
	}
	catch(...)
	{
	}
}


BEGIN_MESSAGE_MAP(CSmartAdminSettingSheet, CTreePropSheet)
	//{{AFX_MSG_MAP(CSmartAdminSettingSheet)
	ON_WM_QUERYDRAGICON()
	ON_WM_PAINT()
	ON_COMMAND(IDOK, OnOK)
	//}}AFX_MSG_MAP
	ON_WM_HELPINFO()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSmartAdminSettingSheet message handlers

/**	
	@brief	The CSmartAdminSettingSheet::AddPropPages function
	@author	humkyung
	@return	void
*/
void CSmartAdminSettingSheet::AddPropPages()
{
	CAdminApp* pApp = (CAdminApp*)AfxGetApp();
	CAdminDocData* pDocData = CAdminDocData::GetInstance();

	m_pIsoEditSettingPage = new CIsoEditSettingPage;
	m_pBMReportSettingPage= new CBMReportSettingPage;

	SetPageIcon( m_pIsoEditSettingPage , IDI_SETTING );
	SetPageIcon( m_pBMReportSettingPage, IDI_LENGTH);

	AddPage( m_pIsoEditSettingPage );
	AddPage(m_pBMReportSettingPage);
}

/**	
	@brief	생성한 페이지들을 제거한다.
	@author	humkyung
	@return	void	
*/
void CSmartAdminSettingSheet::RemovePropPages()
{
	///delete m_pUserAppSettingPage;
	delete m_pIsoEditSettingPage;
}

/**	
	@brief	사용자가 설정한 세팅을 저장한다.
	@author	humkyung
	@return	void	
*/
void CSmartAdminSettingSheet::SaveData()
{
	///if(m_pUserAppSettingPage->GetSafeHwnd()) m_pUserAppSettingPage->SaveData( m_rIniFilePath );
	if( m_pBMReportSettingPage->GetSafeHwnd() ) m_pBMReportSettingPage->SaveData( m_rIniFilePath );
	if( m_pIsoEditSettingPage->GetSafeHwnd() )  m_pIsoEditSettingPage->SaveData( m_rIniFilePath ); 
}

/**	
	@brief	The CSmartAdminSettingSheet::OnInitDialog function

	@author	humkyung

	@return	BOOL	
*/
BOOL CSmartAdminSettingSheet::OnInitDialog()
{
	BOOL bResult = CTreePropSheet::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	SetTreeItemHeight( 18 );

	///m_pUserAppSettingPage->LoadData( m_rIniFilePath );
	m_pIsoEditSettingPage->LoadData( m_rIniFilePath );
	m_pBMReportSettingPage->LoadData( m_rIniFilePath );

	return bResult;
}

void CSmartAdminSettingSheet::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		CTreePropSheet::OnPaint();
	}
}

HCURSOR CSmartAdminSettingSheet::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSmartAdminSettingSheet::OnOK(void)
{
	SaveData();
	EndDialog(IDOK);
}

/******************************************************************************
    @author     humkyung
    @date       2012-05-25
    @class      CSmartAdminSettingSheet
    @function   OnHelpInfo
    @return     BOOL
    @param      HELPINFO*   pHelpInfo
    @brief
******************************************************************************/
BOOL CSmartAdminSettingSheet::OnHelpInfo(HELPINFO* pHelpInfo)
{
	HWND hHtml = ::HtmlHelp(this->m_hWnd , CAdminDocData::GetExecPath() + _T("\\Help\\SmartISO.chm::/General Setting.htm") , HH_DISPLAY_TOPIC , 0);

	return CTreePropSheet::OnHelpInfo(pHelpInfo);
}
