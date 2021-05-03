// UserAppSettingSheet.cpp : implementation file
//

#include "stdafx.h"
#include "UserApp.h"
#include <Project.h>
#include "UserAppSettingSheet.h"
#include "ResourceSwitch.h"
#include "UserAppDocData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserAppSettingSheet

IMPLEMENT_DYNAMIC(CUserAppSettingSheet, CTreePropSheet)

CUserAppSettingSheet::CUserAppSettingSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CTreePropSheet(nIDCaption, pParentWnd, iSelectPage) , m_pSelUser(NULL) , m_oracleSettingPage(new COracleSettingPage)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_pUserPropertyPage = NULL;

	AddPropPages();
}

CUserAppSettingSheet::~CUserAppSettingSheet()
{
	RemovePropPages();
}


BEGIN_MESSAGE_MAP(CUserAppSettingSheet, CTreePropSheet)
	//{{AFX_MSG_MAP(CUserAppSettingSheet)
	ON_WM_QUERYDRAGICON()
	ON_WM_PAINT()
	ON_COMMAND(IDOK, OnOK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserAppSettingSheet message handlers

/**	
	@brief	The CUserAppSettingSheet::AddPropPages function

	@author	humkyung

	@return	void
*/
void CUserAppSettingSheet::AddPropPages()
{
	CUserAppApp* pApp = (CUserAppApp*)AfxGetApp();
	///CResourceSwitch resSwitch(pApp->m_hMultiLangRes);
	CUserAppDocData* pDocData = CUserAppDocData::GetInstance();

	if(STRING_T(_T("ADMIN")) == pDocData->m_pSelUser->id())
	{
		m_pUserPropertyPage   = new CUserPropertyPage;
	}
	m_pUserAppSettingPage = new CUserAppSettingPage;
	m_pIsoEditSettingPage = new CIsoEditSettingPage;
	m_pBMReportSettingPage= new CBMReportSettingPage;

	if(NULL != m_pUserPropertyPage)
	{
		SetPageIcon( m_pUserPropertyPage, m_pUserPropertyPage->GetIconID());
	}
	SetPageIcon( m_pUserAppSettingPage, m_pUserAppSettingPage->GetIconID());
	SetPageIcon( m_pIsoEditSettingPage , m_pIsoEditSettingPage->GetIconID() );
	SetPageIcon( m_pBMReportSettingPage, IDI_BM_REPORT);
	SetPageIcon( m_oracleSettingPage.get() , m_oracleSettingPage->GetIconID());

	if(NULL != m_pUserPropertyPage)
	{
		AddPage(m_pUserPropertyPage);
	}
	AddPage(m_pUserAppSettingPage);
	AddPage( m_pIsoEditSettingPage );
	AddPage(m_pBMReportSettingPage);
	AddPage(m_oracleSettingPage.get());
}

/**	
	@brief	생성한 페이지들을 제거한다.

	@author	humkyung

	@return	void	
*/
void CUserAppSettingSheet::RemovePropPages()
{
	if(m_pUserPropertyPage)
	{
		delete m_pUserPropertyPage;
	}
	m_pUserPropertyPage = NULL;
	delete m_pUserAppSettingPage;
	delete m_pIsoEditSettingPage;
}

/**	
	@brief	사용자가 설정한 세팅을 저장한다.

	@author	humkyung

	@return	void	
*/
void CUserAppSettingSheet::SaveData()
{
	if(m_pUserPropertyPage && m_pUserPropertyPage->GetSafeHwnd())
	{
		m_pUserPropertyPage->SaveData( m_rIniFilePath );
	}
	if(m_pUserAppSettingPage->GetSafeHwnd())    m_pUserAppSettingPage->SaveData( m_rIniFilePath );
	if( m_pBMReportSettingPage->GetSafeHwnd() ) m_pBMReportSettingPage->SaveData( m_rIniFilePath );
	if( m_pIsoEditSettingPage->GetSafeHwnd() )  m_pIsoEditSettingPage->SaveData( m_rIniFilePath ); 
	if(m_oracleSettingPage->GetSafeHwnd())      m_oracleSettingPage->SaveData(m_rIniFilePath);
}

/**	
	@brief	The CUserAppSettingSheet::OnInitDialog function

	@author	humkyung

	@return	BOOL	
*/
BOOL CUserAppSettingSheet::OnInitDialog()
{
	BOOL bResult = CTreePropSheet::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	SetTreeItemHeight( 18 );

	if(m_pSelUser && m_pUserPropertyPage)
	{
		m_pUserPropertyPage->m_rID = m_pSelUser->id();
		m_pUserPropertyPage->UpdateData(FALSE);
	}

	m_pUserAppSettingPage->LoadData( m_rIniFilePath );
	m_pIsoEditSettingPage->LoadData( m_rIniFilePath );
	m_pBMReportSettingPage->LoadData( m_rIniFilePath );
	m_oracleSettingPage->LoadData( m_rIniFilePath );

	return bResult;
}

void CUserAppSettingSheet::OnPaint()
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

HCURSOR CUserAppSettingSheet::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CUserAppSettingSheet::OnOK(void)
{
	SaveData();
	EndDialog(IDOK);
}
