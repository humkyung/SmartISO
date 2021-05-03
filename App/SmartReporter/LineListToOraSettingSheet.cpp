// LineListToOraSettingSheet.cpp : implementation file
//

#include "stdafx.h"
#include "SmartReporter.h"
#include "LineListToOraSettingSheet.h"
///#include "ResourceSwitch.h"
#include "LineListToOraDocData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLineListToOraSettingSheet

IMPLEMENT_DYNAMIC(CLineListToOraSettingSheet, CTreePropSheet)

CLineListToOraSettingSheet::CLineListToOraSettingSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CTreePropSheet(nIDCaption, pParentWnd, iSelectPage)/// , m_oracleSettingPage(new COracleSettingPage)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	AddPropPages();
}

CLineListToOraSettingSheet::~CLineListToOraSettingSheet()
{
	RemovePropPages();
}


BEGIN_MESSAGE_MAP(CLineListToOraSettingSheet, CTreePropSheet)
	//{{AFX_MSG_MAP(CLineListToOraSettingSheet)
	ON_WM_QUERYDRAGICON()
	ON_WM_PAINT()
	ON_COMMAND(IDOK, OnOK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLineListToOraSettingSheet message handlers

/**	
	@brief	페이지를 추가한다.

	@author	humkyung

	@date	????.??.??

	@return	void
*/
void CLineListToOraSettingSheet::AddPropPages()
{
	//m_pAdminPropertyPage = new CAdminPropertyPage;
	//if( m_pAdminPropertyPage )
	//{
	//	SetPageIcon( m_pAdminPropertyPage, m_pAdminPropertyPage->GetIconID());
	//	AddPage(m_pAdminPropertyPage);
	//}

	m_pCustomReportPropertyPage = new CCustomReportPropertyPage;
	if(NULL != m_pCustomReportPropertyPage)
	{
		SetPageIcon( m_pCustomReportPropertyPage , m_pCustomReportPropertyPage->GetIconID() );
		AddPage(m_pCustomReportPropertyPage);
	}
}

/**	
	@brief	생성한 페이지들을 제거한다.
	
	@author	humkyung

	@date	????.??.??

	@return	void	
*/
void CLineListToOraSettingSheet::RemovePropPages()
{
///	if( m_pAdminPropertyPage ) delete m_pAdminPropertyPage;
	if( NULL != m_pCustomReportPropertyPage ) delete m_pCustomReportPropertyPage;
}

/**	
	@brief	사용자가 설정한 세팅을 저장한다.


	@return	void	
*/
void CLineListToOraSettingSheet::SaveData()
{
	CLineListToOraDocData& docData = CLineListToOraDocData::GetInstance();
	CString rIniFilePath = docData.GetIniFilePath().c_str();
///	if(m_pAdminPropertyPage->GetSafeHwnd()) m_pAdminPropertyPage->SaveData(rIniFilePath);
}

/**	\brief	The CLineListToOraSettingSheet::OnInitDialog function


	\return	BOOL	
*/
BOOL CLineListToOraSettingSheet::OnInitDialog()
{
	BOOL bResult = CTreePropSheet::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	SetTreeItemHeight( 18 );

	CLineListToOraDocData& docData = CLineListToOraDocData::GetInstance();
	///m_pAdminPropertyPage->m_rUserName     = m_pSelUser->name();
	//m_pAdminPropertyPage->m_rAdminPassword = docData.GetAdminPassword().c_str();
	//m_pAdminPropertyPage->LoadData(docData.GetIniFilePath().c_str());
	//m_pAdminPropertyPage->UpdateData(FALSE);

	return bResult;
}

void CLineListToOraSettingSheet::OnPaint()
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

HCURSOR CLineListToOraSettingSheet::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/**
**/
void CLineListToOraSettingSheet::OnOK(void)
{
	SaveData();
	EndDialog(IDOK);
}
