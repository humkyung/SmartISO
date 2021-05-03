// LineDataSettingGridCell.cpp: implementation of the CLineDataSettingGridCell class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include <ProjectData.h>
#include "GridCtrl/GridCtrl.h"
#include "AdminDateTimeCtrl.h"
#include "LineDataSettingGridCell.h"
#include "tokenizer.h"

#include <vector>
#include <map>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define ISSUE_DATE_ERROR	_T("설정 이슈 날짜는 이전 리비젼 이슈 날짜보다 후의 날짜이어야 합니다.")
#define CONTROL_ISSUE_ERROR	_T("최종날짜는 이슈 보다 뒤의 날짜이어야 합니다.")
#define SET_ISSUE_ERROR		_T("이슈날짜를 먼저 설정하세요")

using namespace GridCell;

IMPLEMENT_DYNCREATE(CLineDataSettingGridCell, CGridCell)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLineDataSettingGridCell::CLineDataSettingGridCell() : m_nRow( 0 ) , m_nCol( 0 )
{

}

CLineDataSettingGridCell::~CLineDataSettingGridCell()
{

}

/**	
	@brief	The CLineDataSettingGridCell::Edit function

	@author	humkyung

	@param	nRow	a parameter of type int
	@param	nCol	a parameter of type int
	@param	rect	a parameter of type CRect
	@param	/	a parameter of type CPoint /* point *
	@param	nID	a parameter of type UINT
	@param	nChar	a parameter of type UINT

	@return	BOOL	
*/
BOOL CLineDataSettingGridCell::Edit(int nRow, int nCol, CRect rect, CPoint /* point */, UINT nID, UINT nChar)
{
	CProjectData* pProjectData = CProjectData::GetInstance();

	int i=0;
	m_bEditing = TRUE;
	
	m_nRow = nRow;
	m_nCol = nCol;
	if(NULL == m_pEditWnd)
	{
		CAdminDateTimeCtrl* pDateTimeCtrl = new CAdminDateTimeCtrl( nRow , nCol , this ); 
		pDateTimeCtrl->Create(WS_VISIBLE | WS_CHILD | DTS_SHOWNONE| DTS_SHORTDATEFORMAT | DTS_APPCANPARSE , rect, (CWnd*)GetGrid(), 1006);
		
		CTime timeTime;
		CString strDt;
		if(!CString(this->GetText()).IsEmpty())
		{
			int year = 0 , month = 0 , day = 0;
			if(ERROR_SUCCESS == pProjectData->ParseIssueDate(year , month , day , this->GetText()))
			{
				CTime aTime(year , month , day , 0 , 0, 0);
				pDateTimeCtrl->SetTime(&aTime);
			}			
		}
		DWORD dwResult = pDateTimeCtrl->GetTime(timeTime);
		
		CProjectData* pProjectData = CProjectData::GetInstance();
		const CString sIssueDateFormat = pProjectData->GetIssueDateFormat();
		strDt = timeTime.Format( sIssueDateFormat );
		strDt.MakeUpper();

		m_pEditWnd = pDateTimeCtrl;
	}
	else
	{
		CRect wndRect;
		m_pEditWnd->GetWindowRect(&wndRect);
		int height = wndRect.bottom - wndRect.top;
		rect.bottom = rect.top + height;
		m_pEditWnd->MoveWindow(rect);
		m_pEditWnd->ShowWindow(SW_NORMAL);

		if(!CString(this->GetText()).IsEmpty())
		{
			int year = 0 , month = 0 , day = 0;
			if(ERROR_SUCCESS == pProjectData->ParseIssueDate(year , month , day , this->GetText()))
			{
				CTime aTime(year , month , day , 0 , 0, 0);
				((CAdminDateTimeCtrl*)(m_pEditWnd))->SetTime(&aTime);
			}			
		}
	}
	m_pLineDataSettingDlg = static_cast<CLineDataSettingDlg*>(GetGrid()->GetParent());

	return TRUE;
}

/**	
	@brief	The CFieldTypeGridCell::EndEdit function
	@author	humkyung
	@date	????.??.??
	@return	void	
*/
void CLineDataSettingGridCell::EndEdit()
{
	if (m_pEditWnd && (TRUE == m_pEditWnd->IsWindowVisible()) )
	{
		CString sIssueDate , sDisplayDateTime;
		CDateTimeCtrl* pDateTimeCtrl = ((CDateTimeCtrl*)m_pEditWnd);
		if( pDateTimeCtrl )
		{
			CTime timeTime;
			
			DWORD dwResult = pDateTimeCtrl->GetTime(timeTime);
			sIssueDate = timeTime.Format(_T("%Y-%m-%d"));

			/// 2013.01.05 added by humkyung
			if(0 == m_nCol)
			{
				m_pLineDataSettingDlg->m_oPureIssueDateMap[this] = sIssueDate;
			}
			else
			{
				m_pLineDataSettingDlg->m_oPureIssueDateMap[this] = sIssueDate;
			}
			/// up to here

			/// 2011.07.17/2012.12.30 added/modified by humkyung
			CProjectData* pProjectData = CProjectData::GetInstance();
			sDisplayDateTime = pProjectData->GetDisplayIssueDateFrom(sIssueDate);
			sDisplayDateTime.MakeUpper();
			///

			/// 2013.01.03 added by humkyung
			const CString sDelimiter = pProjectData->GetIssueDateDelimiter();
			/// up to here
			/*strDt = timeTime.Format( _T("%Y") + sDelimiter + _T("%m") + sDelimiter + _T("%d") );
			strDt.MakeUpper();*/
		}

		if(1 == m_nCol)
		{
			if( false == m_pLineDataSettingDlg->CheckIssueDate( sIssueDate ) )
			{
				AfxMessageBox(ISSUE_DATE_ERROR , MB_OK|MB_ICONWARNING);
				GetGrid()->SetFocusCell(m_nRow, m_nCol);
			}
			else
			{
				GetGrid()->SetItemText(m_nRow , m_nCol , sDisplayDateTime );
			}
		}
		
		m_pEditWnd->ShowWindow(SW_HIDE);
	}
	m_bEditing = FALSE;
}

/**	
	@brief	The CFieldTypeGridCell::EndEdit function
	@author	humkyung
	@date	????.??.??
	@return	void	
*/
BOOL CLineDataSettingGridCell::DayCompare(CString rIssue, CString rRealIssue)
{
	/// 2011.07.17 added by humkyung
	CProjectData* pProjectData = CProjectData::GetInstance();
	const CString sIssueDateFormatNo = pProjectData->GetIssueDateFormatNo();
	const string sIssueDateFormat   = pProjectData->GetIssueDateFormat().operator LPCTSTR();
	///

	CString rIssuedate = rIssue, rRealIssueDate = rRealIssue;
	int year[2] = {0,0} , month[2] = {0,0} , date[2] = {0,0};

	if(ERROR_SUCCESS != pProjectData->ParseIssueDate(year[0] , month[0] , date[0] , rIssue , _T("-")))
	{
		AfxMessageBox(_T("Error while parsing issue date") , MB_OK|MB_ICONWARNING);
		return	FALSE;
	}

	if(ERROR_SUCCESS != pProjectData->ParseIssueDate(year[1] , month[1] , date[1] , rRealIssue , _T("-")))
	{
		AfxMessageBox(_T("Error while parsing real issue date") , MB_OK|MB_ICONWARNING);
		return FALSE;
	}

	if( year[0] > year[1] )
	{
		return FALSE;
	}
	else if( year[0] == year[1])
	{
		if( month[0] > month[1] )
		{
			return FALSE;
		}
		else if( month[0] == month[1])
		{
			if( date[0] > date[1] )
			{
				return FALSE;
			}
			else
			{
				return TRUE;
			}
		}
		else
		{
			return TRUE;
		}
	}
	else
	{
		return TRUE;
	}
}
