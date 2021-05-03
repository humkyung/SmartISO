// BMReportSettingPage.cpp : implementation file
//

#include "stdafx.h"
#include "UserApp.h"
#include "BMReportSettingPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString CBMReportSettingPage::APP( _T("B/M Report Setting") );
CString CBMReportSettingPage::m_rPipeCutLength( _T("6000") );	///< 기본값 6M
/////////////////////////////////////////////////////////////////////////////
// CBMReportSettingPage property page

CBMReportSettingPage::CBMReportSettingPage() : m_bETC(FALSE)
{
	//{{AFX_DATA_INIT(CBMReportSettingPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBMReportSettingPage::~CBMReportSettingPage()
{
}

/////////////////////////////////////////////////////////////////////////////
// CBMReportSettingPage message handlers

/**
	@brief	

	@author	humkyung
**/
void CBMReportSettingPage::LoadData( const CString& rIniFilePath )
{
	TCHAR szBuf[MAX_PATH + 1] = {'\0' ,};
	if(GetPrivateProfileString(CBMReportSettingPage::APP , _T("Pipe Cut Length") , _T("") , szBuf , MAX_PATH , rIniFilePath))
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
	///UpdateData( TRUE );

	if(FALSE == WritePrivateProfileString(CBMReportSettingPage::APP , _T("Pipe Cut Length") , m_rPipeCutLength , rIniFilePath))
	{
		AfxMessageBox( _T("") );
		return;
	}
}

/**	
	@brief	The CBMReportSettingPage::GetPipeCutLength function
	
	@author	humkyung

	@return	double	
*/
double CBMReportSettingPage::GetPipeCutLength()
{
	return atof( CBMReportSettingPage::m_rPipeCutLength );
}