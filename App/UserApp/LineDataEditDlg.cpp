// LineDataEditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UserApp.h"
#include "UserAppDocData.h"
#include "LineDataEditDlg.h"
#include "enablebuddybutton.h"
#include "BackupDataOutputForm.h"
#include "WorkStatusDlg.h"
#include "MainFrm.h"
#include "UserAppView.h"
#include "SmartISOLog.h"
#include <ado/SQLFile.h>
#include <Tokenizer.h>

#include <ProjectData.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLineDataEditDlg dialog
IMPLEMENT_DYNCREATE(CLineDataEditDlg, CFormView)


CLineDataEditDlg::CLineDataEditDlg()
	: CFormView(CLineDataEditDlg::IDD) , m_wndResizeCtrl( TRUE )
{
	//{{AFX_DATA_INIT(CLineDataEditDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CLineDataEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLineDataEditDlg)
	DDX_Control(pDX , IDC_LIST_ISSUE_DATE , m_wndIssueDateListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLineDataEditDlg, CFormView)
	//{{AFX_MSG_MAP(CLineDataEditDlg)
	ON_WM_SIZE()
	//ON_NOTIFY(NM_CLICK , IDC_LIST_GRADE_DEFINE, OnClkListGradeDefine)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ISSUE_DATE, OnDblclkListIssueDate)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_DELETE_ISSUE, OnUpdateDeleteIssue)
	//ON_COMMAND(IDC_BUTTON_DELETE_ISSUE, OnDeleteIssue)
	
	ON_COMMAND(ID_RESIZE, OnVertResize)	/// 2012.08.28 added by humkyung
	//}}AFX_MSG_MAP
	ON_WM_HELPINFO()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ISSUE_DATE, &CLineDataEditDlg::OnLvnItemchangedListIssueDate)
END_MESSAGE_MAP()

namespace
{
	bool IsFixedRevisionField(const CString& rFieldName)
	{
		return ((rFieldName == "REV_DATE") || (rFieldName == "REV_NO") ||(rFieldName == "REV_DRWN_BY") ||
				(rFieldName == "REV_CHECKED_BY") || (rFieldName == "REV_APPROVED_BY"));
	}
}
/////////////////////////////////////////////////////////////////////////////
// CLineDataEditDlg message handlers

BOOL CLineDataEditDlg::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CLineDataEditDlg::OnInitialUpdate() 
{
	static bool __init__ = false;

	CFormView::OnInitialUpdate();
	
	if(false == __init__)
	{
		m_ImageList.Create(IDB_USER_APP , 16 , 16 , RGB(255 , 0 , 255));
		m_wndIssueDateListCtrl.SetImageList(&m_ImageList,LVSIL_SMALL);
		
		CMainFrame* pMainFrame = GetUserAppMainWnd();
		//////////////////////////////////////////////////////////////////////////
		CUserAppView* pUserAppView = (CUserAppView*)(CUserAppView::GetInstance());
		CAppDocData* pDocData  = CAppDocData::GetInstance();
		
		m_pLineDataForm = pUserAppView->m_pLineDataForm;
		
		//////////////////////////////////////////////////////////////////////////
		CRect rect;
		CWnd* pWnd = GetDlgItem(IDC_STATIC_REVISION_DATA);
		if(NULL != pWnd)
		{
			pWnd->GetWindowRect(&rect);
			pWnd->SetWindowText(_TR("Revision Data"));
		}

		ScreenToClient(&rect);
		rect.DeflateRect(5 , 18 , 5 , 5);
		{
			m_wndResizeCtrl.Create( NULL, _T("") , WS_VISIBLE | WS_CHILD , CRect(0,0,0,0), this , ID_RESIZE );

			m_wndRevDataGrid.Create(rect , this , CLineDataEditDlg::IDD + 0x100 , WS_CHILD | WS_VISIBLE);

			/// fill it up with stuff
			m_wndRevDataGrid.SetEditable(FALSE);
			m_wndRevDataGrid.EnableDragAndDrop(FALSE);
			m_wndRevDataGrid.SetRowResize(FALSE);
			m_wndRevDataGrid.SetColumnResize(TRUE);
			m_wndRevDataGrid.SetListMode(FALSE);
			m_wndRevDataGrid.SetSingleRowSelection( TRUE );
			m_wndRevDataGrid.SetSingleColSelection( TRUE );
			m_wndRevDataGrid.SetHeaderSort(FALSE);
			
			CString rSqlFilePath = CAppDocData::GetExecPath() + _T("\\Setting\\T_ISO_REVISION.XML");
			CSQLFile sqlFile(rSqlFilePath.operator LPCSTR());
			if(ERROR_SUCCESS == sqlFile.ParseXML(rSqlFilePath.operator LPCTSTR()))
			{
				const int nFieldCount = sqlFile.GetColumnCount();
				
				m_wndRevDataGrid.SetColumnCount(nFieldCount);
				m_wndRevDataGrid.SetRowCount(1);
				m_wndRevDataGrid.SetFixedRowCount(1);
				m_wndRevDataGrid.SetFixedColumnCount(1);
				
				m_RevFieldNameEntry.clear();
				m_RevFieldDescEntry.clear();
				
				for(int i = 0;i < nFieldCount;++i)
				{
					const string& rFieldName = sqlFile.GetFieldNameAt(i);
					m_RevFieldNameEntry.push_back(rFieldName.c_str());
					m_RevFieldDescEntry.push_back(rFieldName.c_str());
					m_wndRevDataGrid.SetItemText(0,i,rFieldName.c_str());
					
					if(IsFixedRevisionField(rFieldName.c_str()))
					{
						m_wndRevDataGrid.SetItemData(0 , i , 0);
					}
					else
					{
						m_wndRevDataGrid.SetItemData(0 , i , 1);
					}
				}
			}

			DisplayRevisionDataRecordSet(NULL);
		}

		pWnd = GetDlgItem(IDC_STATIC_ISSUE_DATE);
		if(NULL != pWnd) pWnd->SetWindowText(_TR("Issue Date"));

		InitContents();

		__init__ = true;
	}

	//////////////////////////////////////////////////////////////////////////

	//return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/**	@brief	project가 로드되고 난 뒤 control들의 내용을 초기화 시킨다.
	@author	humkyung
	@date	????.??.??
	@param
	@return
**/
void CLineDataEditDlg::InitContents()
{
	if(NULL != m_pUserAppDoc)
	{
		CAppDocData* pDocData = CAppDocData::GetInstance();

		CString rIniFilePath = pDocData->GetProject()->GetIniFilePath();
		
		const CString rApp = _T("Line Data Field");
		TCHAR szBuf[MAX_PATH] = {'\0' ,};
		
		CString rSqlFilePath = CAppDocData::GetExecPath() + _T("\\Setting\\T_ISO_LINE_DATA.XML");
		CSQLFile sqlFile(rSqlFilePath.operator LPCSTR());
		if(ERROR_SUCCESS == sqlFile.ParseXML(rSqlFilePath.operator LPCTSTR()))
		{
			int nCount = sqlFile.GetColumnCount();
			vector<CString> HeadingEntry;
			vector<string> oResult; 
			for(int i = 0;i < nCount;++i)
			{
				CString rHeading;
				rHeading += sqlFile.GetFieldNameAt(i).c_str() + CString(_T(",100"));
				HeadingEntry.push_back(rHeading);
			}
			
			CString rHeadings;
			for(vector<CString>::iterator itr = HeadingEntry.begin();itr != HeadingEntry.end();itr++)
			{
				rHeadings += (*itr);
				if((itr + 1) != HeadingEntry.end()) rHeadings += _T(";");
			}
		}else	AfxMessageBox( _T("Can't find T_ISO_LINE_DATA.XML file") );

		//////////////////////////////////////////////////////////////////////////
		if(m_wndIssueDateListCtrl.GetSafeHwnd())
		{
			CProjectData* pProjectData = CProjectData::GetInstance();
			m_wndIssueDateListCtrl.SetHeadings(_TR("Date") + _T(",200"));
			if(pDocData->m_pIssueDateRecordSet)
			{
				const int nCount = pDocData->m_pIssueDateRecordSet->GetCount();
				for(int i = 0;i < nCount;++i)
				{
					CIssueDateRecord* pRecord = pDocData->m_pIssueDateRecordSet->GetAt(i);
					if(NULL != pRecord)
					{
						/// 2011.08.06 added by humkyung
						CString sDisplayIssueDate = pProjectData->GetDisplayIssueDateFrom(pRecord->dateString());
						/// up to here
						const int iItem = m_wndIssueDateListCtrl.InsertItem(i , sDisplayIssueDate,8);
						m_wndIssueDateListCtrl.SetItemData(iItem , DWORD(pRecord));	/// 2013.01.05 added by humkyung
					}
				}
			}
			m_wndIssueDateListCtrl.SetHeaderImageList();
		}
	}
}

/**	
	@brief	
	@author	humkyung
*/
void CLineDataEditDlg::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	if((0 != cx) && (0 != cy) && m_wndResizeCtrl.GetSafeHwnd())
	{
		CRect rect;
		m_wndResizeCtrl.GetWindowRect(&rect);
		ScreenToClient(&rect);
		
		m_wndResizeCtrl.SetWindowPos(NULL , int(cx*0.75) , 5 , 3 , cy - 10 , SWP_NOZORDER);
	}

	this->DetermineLayout(cx , cy);
}

UINT CLineDataEditDlg::StatusThreadEntry(LPVOID pVoid)
{
	pair<CLineDataEditDlg* , pair<CString,CString> >* param = (pair<CLineDataEditDlg* , pair<CString,CString> >*)(pVoid);
	return ((param->first)->StatusThread(param->second.first , param->second.second));
	//return (((CLineDataEditDlg*)(pVoid))->StatusThread());
}

/**	
	@brief	쓰레드 실행부
	@author	humkyung
*/
UINT CLineDataEditDlg::StatusThread(const CString& sIssueDate , const CString& sControlDate)
{
	CWorkStatusDlg* pWorkStatusDlg = CWorkStatusDlg::GetInstance();
	try
	{
		InterlockedExchange((LONG*)(&(pWorkStatusDlg->m_bThreadRunning)) , TRUE);/// so the main thread knows that this thread is still running
		{

			vector<LineDataRecordMap> aLineDataRecordEntry;
			m_pLineDataForm->GetSelectedLineDataRecord(aLineDataRecordEntry);

			const int iCount = aLineDataRecordEntry.size();
			CString sTitle(_TR("Apply Issue Date"));
			int process = 0 , step = 0;
			for(vector<LineDataRecordMap>::const_iterator itr = aLineDataRecordEntry.begin();itr != aLineDataRecordEntry.end();++itr)
			{
				//CLineDataRecord* pRecord = itr->pLineDataRecord;///(CLineDataRecord*)m_wndQualifedLineCtrl.GetItemData(i);
				if(NULL != itr->pLineDataRecord)
				{
					CString rLog;
					bool res = AddRevDataToLineDataRecord(itr->pLineDataRecord , sIssueDate , rLog);
					if(!rLog.IsEmpty())
					{
						WRITE_LOG(rLog.operator LPCTSTR());
					}
				}
				process = int(((double)(++step)/(double)iCount)*100.0);
				CWorkStatusDlg::UpdateWorkStatus(sTitle , process);
			}
		}

		InterlockedExchange((LONG*)(&(pWorkStatusDlg->m_bThreadRunning)) , FALSE);
		
		//////////////////////////////////////////////////////////////////////////
		/// close work status dialog
		if(pWorkStatusDlg) pWorkStatusDlg->SendMessage(WM_COMMAND , IDOK );

		return IDOK;
	}
	catch(...)
	{
		AfxMessageBox( _T("Unknown error") );

		InterlockedExchange((LONG*)(&(pWorkStatusDlg->m_bThreadRunning)) , FALSE);
		
		//////////////////////////////////////////////////////////////////////////
		/// close work status dialog
		if(pWorkStatusDlg) pWorkStatusDlg->SendMessage(WM_COMMAND , IDOK );
	}

	return IDCANCEL;
}

/**	
	@brief	Base가 다이얼로그이기 때문에 ENTER나 ESC키를 누르면 다이얼로그가 종료된다.
	그래서 ENTER나 ESC가 전달되기 전에 막는다.
	@author	humkyung
	@date	????.??.??
	@param	pMsg	a parameter of type MSG*
	@return	BOOL	
*/
BOOL CLineDataEditDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN ) //! prevent enter and escape key.. so good.. ^^		
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE )		
		{
			::TranslateMessage(pMsg);
			
			::DispatchMessage(pMsg);
			
			return TRUE;                    // DO NOT process further	
		}
	}
	
	UpdateDialogControls(this, TRUE);
	return CFormView::PreTranslateMessage(pMsg);
}

/**	
	@brief	모든 아이템들을 제거한다.
	@author	HumKyung
	@date	????.??.??
	@param
	@return
**/
void CLineDataEditDlg::DeleteAllQualifiedLineList( )
{
	m_ReverseItemMap.clear();
	m_ItemMap.clear();

	m_wndRevDataGrid.DeleteAllItems();	//! 2011.01.24 - added by HumKyung
}

/**
	@brief	선택한 RECORD의 개수를 표시한다.
	@author	humkyung
**/
void CLineDataEditDlg::ShowSelectedCount(int nCount)
{
	int nRecordCount = m_pLineDataForm->m_nRecordCount;
	CWnd* pWnd = GetDlgItem(IDC_STATIC_SELECTED_COUNT);
	if(pWnd)
	{
		CString rCount, rSlash =" / ", rRecodrCount;
		rCount.Format("%d",nCount);
		rRecodrCount.Format("%d",nRecordCount);
		rCount = rCount + rSlash + rRecodrCount;
		pWnd->SetWindowText((LPCTSTR)rCount);
	}
}

/**
	\brief	

	\author	HumKyung.BAEK
**/
void CLineDataEditDlg::ClearContents()
{
	m_ItemMap.clear();
	m_ReverseItemMap.clear();
}

/**	@brief	리스트에 line data record를 추가한다.

	@param	pLineDataRecordEntry	a parameter of type vector<LineDataRecordMap>*

	@remarks\n
	ItemMap에 없는 line data record만을 리스트에 추가한다.

	@return	void	
*/
void CLineDataEditDlg::AddLineDataRecordEntry(vector<LineDataRecordMap>* pLineDataRecordEntry)
{
	assert(pLineDataRecordEntry && "pLineDataRecordEntry is NULL");

	if(pLineDataRecordEntry)
	{
		/*LVITEM lvitem;
		lvitem.mask = LVIF_TEXT | LVIF_IMAGE;
		lvitem.iImage = 1;
		lvitem.cchTextMax = 32;
		
		const size_t nRecordCount = pLineDataRecordEntry->size();
		int nItem = m_wndQualifedLineCtrl.GetItemCount();
		TCHAR szBuf[MAX_PATH] = {'\0' ,};
		for(int i = 0;i < int(nRecordCount);i++)
		{		
			CLineDataRecord* pLineDataRecord = (*pLineDataRecordEntry)[i].pLineDataRecord;
			if(pLineDataRecord)
			{
				CString rId;
				rId.Format("%d" , pLineDataRecord->index());

				const int item = (*pLineDataRecordEntry)[i].nItem;
				map<int , int>::iterator where = m_ItemMap.find(item);
				if((m_ItemMap.end() == where) && !pLineDataRecord->IsDeleted())
				{
					lvitem.iItem = nItem++;
					lvitem.iSubItem = 0;
					lvitem.pszText  = (char*)(rId.operator const char*());
					int it = m_wndQualifedLineCtrl.InsertItem(&lvitem);
					UpdateLineDataRecord(it , pLineDataRecord);

					m_ItemMap[(*pLineDataRecordEntry)[i].nItem] = it;
					m_ReverseItemMap[it] = (*pLineDataRecordEntry)[i].nItem;
				}
			}
		}
		m_wndQualifedLineCtrl.SetRedraw(TRUE);*/
	}
}

/**	
	@brief	체크한 아이템들을 제거한다.

	@author	HumKyung.BAEK
**/
void CLineDataEditDlg::DeleteCheckedQualifiedLineList( )
{
	/*vector<int> CheckedItemEntry;
	for(int i = 0;i < m_wndQualifedLineCtrl.GetItemCount();++i)
	{
		if(m_wndQualifedLineCtrl.GetCheck(i)) CheckedItemEntry.push_back(i);
	}
	
	for(vector<int>::iterator itr = CheckedItemEntry.begin();itr != CheckedItemEntry.end();itr++)
	{
		map<int , int>::iterator where = m_ReverseItemMap.find(*itr);
		if(m_ReverseItemMap.end() != where)
		{
			map<int , int>::iterator at = m_ItemMap.find(where->second);
			if(m_ItemMap.end() != at)
			{
				m_ItemMap.erase(at);
			}
			
			m_ReverseItemMap.erase(where);
		}
	}
	
	m_wndQualifedLineCtrl.SetRedraw(FALSE);
	m_wndQualifedLineCtrl.DeleteAllCheckedItems();
	m_wndQualifedLineCtrl.SetRedraw(TRUE);*/
}

/**	\brief	선택된 line data record를 리스트에서 제거한다.

	\remarks\n
	ItemMap에서도 line data record의 내용을 제거한다.

	\return	void	
*/
void CLineDataEditDlg::DeleteLineData() 
{
	//vector<LineDataRecordMap> lineDataRecordMapEntry;
	//GetAllLineDataRecord(lineDataRecordMapEntry);
	//if(lineDataRecordMapEntry.size() > 0)
	//{
	//	m_pLineDataForm->DeleteLineDataRecordEntry(&lineDataRecordMapEntry);

	//	m_wndQualifedLineCtrl.SetRedraw(FALSE);
	//	m_wndQualifedLineCtrl.DeleteAllItems();
	//	for(vector<LineDataRecordMap>::iterator itr = lineDataRecordMapEntry.begin();itr != lineDataRecordMapEntry.end();itr++)
	//	{
	//		int nItem = m_ReverseItemMap[itr->nItem];
	//		m_ItemMap.erase(nItem);
	//		m_ReverseItemMap.erase(itr->nItem);
	//	}

	//	//////////////////////////////////////////////////////////////////////////
	//	/// re-index
	//	int nItem = m_wndQualifedLineCtrl.GetItemCount();
	//	for(int i = 0;i < nItem;i++)
	//	{
	//		CString rId;
	//		rId.Format("%d" , i);
	//		m_wndQualifedLineCtrl.SetItemText(i , 0 , rId);
	//	}
	//	m_wndQualifedLineCtrl.SetRedraw(TRUE);
	//}
	//else
	//{
	//	CString rMsg;
	//	rMsg.LoadString(IDS_LINE_DATA_FORM_MSG1);
	//	AfxMessageBox(rMsg , MB_OK | MB_ICONWARNING);
	//}
}

/**
	@brief	
	
	@author	HumKyung.BAEk
**/
void CLineDataEditDlg::UpdateLineDataRecord(const int &nItem, CLineDataRecord *pLineDataRecord)
{
	assert((nItem > -1) && "item range error");
	assert(pLineDataRecord && "pLineDataRecord is NULL");
	
	if((nItem > -1) && pLineDataRecord)
	{
		/*TCHAR szBuf[MAX_PATH] = {'\0' ,};
		m_wndQualifedLineCtrl.SetItemData(nItem , (DWORD)(pLineDataRecord));
		
		const int nColumnCount = m_wndQualifedLineCtrl.GetColumnCount();
		for(int k = 0;k < nColumnCount;k++)
		{
			LVCOLUMN lvcolumn;
			lvcolumn.mask = LVCF_TEXT;
			lvcolumn.cchTextMax = MAX_PATH;
			lvcolumn.pszText = szBuf;
			m_wndQualifedLineCtrl.GetColumn(k , &lvcolumn);
		
			CDataField* pField = pLineDataRecord->FindWithFieldName(lvcolumn.pszText);
			if(pField)
			{
				CString rValue = pField->value();
				rValue.Replace("\n" , " ");
				
				m_wndQualifedLineCtrl.SetItemText(nItem , k , rValue);
			}
		}
		int nCount = m_wndQualifedLineCtrl.GetItemCount();
		ShowSelectedCount(nCount);*/
	}
}

/**	@brief	The CQualifiedLineForm::GetAllLineDataRecord function
	
	@author	HumKyung

	@date	????.??.??

  	@param	lineDataRecordEntry	a parameter of type vector<LineDataRecordMap>&

	@return	void	
*/
void CLineDataEditDlg::GetAllLineDataRecord(vector<LineDataRecordMap>& lineDataRecordEntry)
{
	LineDataRecordMap lineDataRecordMap;
	/*const int nItemCount = m_wndQualifedLineCtrl.GetItemCount();
	for( int i = 0;i < nItemCount;++i)
	{
		lineDataRecordMap.nItem = i;
		lineDataRecordMap.pLineDataRecord = (CLineDataRecord*)(m_wndQualifedLineCtrl.GetItemData(lineDataRecordMap.nItem));

		if(lineDataRecordMap.pLineDataRecord) lineDataRecordEntry.push_back(lineDataRecordMap);
	}*/
}

/**	
	@brief	revision data를 표기합니다.
	@author	HumKyung
	@date	????.??.??
	@param	pRecord
	@return	void	
*/
void CLineDataEditDlg::DisplayRevisionDataRecordSet(CLineDataRecord* pRecord)
{
	assert(pRecord && "pRecord is NULL");

	if(pRecord && m_wndRevDataGrid.GetSafeHwnd())
	{
		TCHAR szBuf[MAX_PATH] = {0,};
		int nKeyInCount=0;
		CString rString;
		UserKeyInData userKeyIn;
		m_RevKeyInDataEntry.clear();
		
		CAppDocData* pDocData = CAppDocData::GetInstance();
		const CProject::DatabaseProp prop = pDocData->GetProject()->GetDatabaseProp();
		
		CProjectData* pProjectData= CProjectData::GetInstance();
		{
			const int nColCount = m_wndRevDataGrid.GetColumnCount();
			CRevDataRecordSet* pRevDataRecordSet = pRecord->GetRevDataRecordSet();
			const int nRecordCount = pRevDataRecordSet->GetRecordCount();
			m_wndRevDataGrid.SetRowCount(nRecordCount + 1);
			for(int j = 0;j < nRecordCount;++j)
			{
				CString sCondition[2];

				CRevDataRecord* pRecord = pRevDataRecordSet->GetRecordAt(j);
				for(int i = 0; i < nColCount;++i)
				{
					CDataField* pField = pRecord->Find(m_RevFieldDescEntry[i].c_str());
					if(pField)
					{
						CString rValue = pField->value();
						if(_T("REV_NO") == m_wndRevDataGrid.GetItemText(0 , i))
						{
							sCondition[1] = pField->value();
							rValue = pField->value();
						}
						else if(_T("GRADE") == m_wndRevDataGrid.GetItemText(0 , i))
						{
							const int nRevNo = atoi(pField->value());
							rValue = (*pDocData->m_pRevisionStageMap)[nRevNo].rDefine.c_str();
						}
						else if(_T("KEY") == m_wndRevDataGrid.GetItemText(0 , i))
						{
							sCondition[0] = pField->value();
						}
						/// 2011.08.06 added by humkyung
						else if(_T("REV_DATE") == m_wndRevDataGrid.GetItemText(0 , i))
						{
							rValue = pProjectData->GetDisplayIssueDateFrom(rValue);
						}
						else if(_T("REV_CONTROL_WORKING_DATE") == m_wndRevDataGrid.GetItemText(0 , i))
						{
							rValue = pProjectData->GetDisplayIssueDateFrom(rValue);
						}
						/// up to here

						m_wndRevDataGrid.SetItemText(j + 1 , i , rValue);
						m_wndRevDataGrid.SetItemData(j + 1 , i , (LPARAM)(pRecord));
						
						if(0 == i) continue;
						if(0 == m_wndRevDataGrid.GetItemData(0 , i))
						{
							m_wndRevDataGrid.SetItemState(j + 1 , i , GVNI_READONLY);
						}
					}
				}

				/// display background with gray color if it's revision is issued. - 2011.02.07 added by HumKyung
				try
				{
					CAppPostgreSQL database(pDocData->GetProject()->name(),prop.Host.c_str(),prop.User.c_str(),prop.Password.c_str());
					CString sQuery;
					sQuery.Format(_T("SELECT KEY FROM T_ISSUED_ISO_REVISION WHERE KEY='%s' AND REV_NO='%s'") , sCondition[0] , sCondition[1]);
					soci::rowset<soci::row> rs(database.session()->prepare << sQuery.operator LPCTSTR());
					for(soci::rowset<soci::row>::const_iterator itr = rs.begin();itr != rs.end();++itr)
					{
						CGridCellBase* pCell = m_wndRevDataGrid.GetCell(j + 1 , 0);
						const COLORREF clr = pCell->GetBackClr();
						for(int i = 1; i < nColCount;++i)
						{
							pCell = m_wndRevDataGrid.GetCell(j + 1 , i);
							pCell->SetBackClr(RGB(200,200,200));
							m_wndRevDataGrid.SetItemState(j+1,i,m_wndRevDataGrid.GetItemState(j+1,i) | GVNI_READONLY);
						}
					}
					///
				}
				catch(...){}
			}
		}
	}
	
	m_wndRevDataGrid.AutoSizeRows();
	m_wndRevDataGrid.AutoSizeColumns();
	m_wndRevDataGrid.RedrawWindow();
}

/**	
	@brief	select next grade
	
	@author	HumKyung.BAEK

	@param	pNMHDR	a parameter of type NMHDR*
	@param	pResult	a parameter of type LRESULT*

	@return	void	
*/
//void CLineDataEditDlg::OnClkListGradeDefine(NMHDR* pNMHDR, LRESULT* pResult)
//{
//	POSITION pos = m_wndGradeDefineListCtrl.GetFirstSelectedItemPosition();
//	while(pos)
//	{
//		const int nItem = m_wndGradeDefineListCtrl.GetNextSelectedItem(pos);
//		const CString rString = m_wndGradeDefineListCtrl.GetItemText(nItem , 0);
//
//		CWnd* pWnd = (CWnd*)GetDlgItem(IDC_STATIC_REVISION_STATE);
//		pWnd->SetWindowText(_T("Next grade(") + rString + _T(")"));
//
//		/// 2011.08.20 added by humkyung
//		if(m_wndRevNoInGradeListCtrl.IsWindowVisible())
//		{
//			m_wndRevNoInGradeListCtrl.DeleteAllItems();
//
//			CProjectData* pProjectData = CProjectData::GetInstance();
//			for(map<int,CRevisionStage>::iterator itr = pProjectData->m_pRevisionStageMap->begin();itr != pProjectData->m_pRevisionStageMap->end();++itr)
//			{
//				if(rString == CString(itr->second.m_sDefine.c_str()))
//				{
//					const int iItem = m_wndRevNoInGradeListCtrl.InsertItem(m_wndRevNoInGradeListCtrl.GetItemCount() , itr->second.m_sText.c_str());
//					m_wndRevNoInGradeListCtrl.SetItemData(iItem , DWORD(itr->first));
//				}
//			}
//		}
//		/// up to here
//
//		break;
//	}
//}

/**	
	@brief	이슈 날짜를 추가시킨다.
	@author	humkyung
	@date	????.??.??
	@param	pNMHDR	a parameter of type NMHDR*
	@param	pResult	a parameter of type LRESULT*
	@return	void	
*/
void CLineDataEditDlg::OnDblclkListIssueDate(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CWorkStatusDlg dlg;

	/// get selected issue date
	int nItem = -1;
	POSITION pos = m_wndIssueDateListCtrl.GetFirstSelectedItemPosition();
	if (pos)
	{
		nItem = m_wndIssueDateListCtrl.GetNextSelectedItem(pos);
	}
	
	if((-1 != nItem)/* && (aLineDataRecordEntry.size() > 0)*/)
	{	
		/// 2011.08.20 added by humkyung
		/*if(m_wndRevNoInGradeListCtrl.IsWindowVisible())
		{
			if(0 == m_wndRevNoInGradeListCtrl.GetSelectedItemCount())
			{
				AfxMessageBox(_T("Revision No.를 선택하세요.") , MB_OK);
				return;
			}
		}*/
		/// up to here

		CAppDocData* pDocData = CAppDocData::GetInstance();
		CIssueDateRecord* pIssueRecord = (CIssueDateRecord*)(m_wndIssueDateListCtrl.GetItemData(nItem));
		CString rIssueDate = pIssueRecord->dateString();
		
		CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_MODIFY);
		BOOL bChecked = pButton->GetCheck();	
		if(!bChecked) /// ISSUE DATE를 추가시킨다.
		{
			/// 허용 가능한 ISSUE 날짜를 확인한다.
			/*if(pDocData->IsLastDate(rControlDate))
			{
				CString rString;
				rString.LoadString(IDS_LAST_DATE);
				AfxMessageBox(rString);
				return;
			}*/
			
			UpdateData(TRUE);
			
			START_LOG();	/// 2013.01.05 added by humkyung
			{
				/// create a thread - 2013.01.06 added by humkyung
				pair<CLineDataEditDlg* , pair<CString,CString> > param = make_pair(this , make_pair(rIssueDate , _T("")));
				dlg.m_pThread = AfxBeginThread(StatusThreadEntry, &param , THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
				if (NULL == dlg.m_pThread)
				{
					AfxMessageBox( _T("Can't create thread!!!") );
				}
				else
				{
					const int res = dlg.DoModal();
				}
				/// up to here
			}
			CLOSE_LOG();

			OPEN_LOG();
		}
		else
		{
			/// REV. DATA를 수정한다.
			///CurRevDateModify(rIssueDate, rControlDate);
		}

		/*pos = m_wndQualifedLineCtrl.GetFirstSelectedItemPosition();
		nItem = m_wndQualifedLineCtrl.GetNextSelectedItem(pos);
		CLineDataRecord* pRecord = (CLineDataRecord*)m_wndQualifedLineCtrl.GetItemData(nItem);
		if(pRecord) DisplayRevisionDataRecordSet(pRecord);*/
	}
	else
	{
		/// 2011.01.06 - added by HumKyung
		AfxMessageBox(_T("먼저 LINE DATA를 선택하세요.") , MB_OK);
	}

	*pResult = 0;
}

/**
	@brief	pLineDataRecord에 새로운 revision date를 추가한다.
	@author	humkyung
	@date	????.??.??
	@param	pLineDataRecord
	@param	rIssueDate
	@param	rControlDate
	@param	rLog
	@return	bool
**/
bool CLineDataEditDlg::AddRevDataToLineDataRecord(CLineDataRecord *pLineDataRecord, const CString &rIssueDate, CString& rLog)
{
	assert(pLineDataRecord && "pLineDataRecord is NULL");

	if(pLineDataRecord)
	{
		CRevDataRecordSet* pRevRecordSet = pLineDataRecord->GetRevDataRecordSet();
		if(NULL != pRevRecordSet)
		{
			/// 선택한 ISSUE날짜가 REV ISSUE 날짜들 보다 이전 혹은 동일한지 확인한다.
			if(pRevRecordSet->IsLastDate(rIssueDate))
			{
				rLog = _TR("IT IS LAST DATE");
				return false;
			}
			
			CAppDocData* pDocData = CAppDocData::GetInstance();
			CRevDataRecord* pRevRecord = pDocData->GetProject()->CreateRevDataRecord();
			if(NULL != pRevRecord)
			{
				CProjectData* pProjectData = CProjectData::GetInstance();

				const CString rKey = pLineDataRecord->FindWithFieldName(_T("KEY"))->value();

				RevisionStage stage;
				CRevDataRecord* pLastRecord = pRevRecordSet->FindLatestRevDataRecord();
				int iRevNo = -1 , nNextGrade = -1;
				if(NULL != pLastRecord)
				{
					CDataField* pField = pLastRecord->FindWithName(_T("REV_NO"));
					map<int,RevisionStage>::const_iterator where = find_if(pDocData->m_pRevisionStageMap->begin(),pDocData->m_pRevisionStageMap->end(),
						[pField](const std::pair<int,RevisionStage>& param){return (0 == STRICMP_T(pField->value(),param.second.rText.c_str()));});
					if(where != pDocData->m_pRevisionStageMap->end())
					{
						iRevNo = where->second.nNo;
					}
				}
				
				const int nErrNo = pDocData->GetNextRevisionStage(stage , iRevNo);
				if(ERROR_SUCCESS == nErrNo)
				{
					CString sGradeDesc;
					/// get revision description - 2015.09.17 added by humkyung
					vector<GradeDefine>::const_iterator where = find_if(pDocData->m_pGradeDefineEntry->begin() , pDocData->m_pGradeDefineEntry->end() , [stage](GradeDefine param){return (CString(stage.rDefine.c_str()) == param.rPrefix);});
					if(where != pDocData->m_pGradeDefineEntry->end())
					{
						sGradeDesc = where->rDesc;
					}
					/// up to here

					CProjectData* pPrjData = CProjectData::GetInstance();
					try
					{
						CDataField* pField = pRevRecord->FindWithName(_T("KEY"));
						pField->Set(_T("key") , _T("key") , rKey);

						CString rString;
						rString = stage.rText.c_str();
						pField = pRevRecord->FindWithName(_T("REV_NO"));
						pField->Set(_T("rev_no") , _T("rev_no") , rString);
						
						pField = pRevRecord->FindWithName(_T("REV_DRWN_BY"));
						rString = (*(pPrjData->m_pProjectSettingMap))[_T("rev_drwn_by")];
						pField->Set(_T("rev_drwn_by") , _T("rev_drwn_by") , rString);

						pField = pRevRecord->FindWithName(_T("REV_CHECKED_BY"));
						rString = (*(pPrjData->m_pProjectSettingMap))[_T("rev_checked_by")];
						pField->Set(_T("rev_checked_by") , _T("rev_checked_by") , rString);

						pField = pRevRecord->FindWithName(_T("REV_APPROVED_BY"));
						rString = (*(pPrjData->m_pProjectSettingMap))[_T("rev_approved_by")];
						pField->Set(_T("rev_approved_by") , _T("rev_approved_by") , rString);

						pField = pRevRecord->FindWithName(_T("REV_DATE"));
						/// 2011.08.06 added by humkyung
						int iYear = 0 , iMonth = 0 , iDay = 0;
						CString sDate;
						pProjectData->ParseIssueDate(iYear , iMonth , iDay , rIssueDate , _T("-"));
						sDate.Format(_T("%04d-%02d-%02d") , iYear , iMonth , iDay);
						/// up to here
						pField->Set(_T("rev_date") , _T("rev_date") , sDate);

						/// 2011.01.26 - added by HumKyung
						pField = pRevRecord->FindWithName(_T("REV_DESC"));
						pField->Set(_T("rev_desc") , _T("rev_desc") , sGradeDesc);
						///

						/// 2011.07.12 - added by humkyung
						rString = (*(pPrjData->m_pProjectSettingMap))[_T("REV_REVIEWED_BY")];
						pField = pRevRecord->FindWithName(_T("rev_reviewed_by"));
						pField->Set(_T("rev_reviewed_by") , _T("rev_reviewed_by") , rString);
						///

						/// save revision recordset
						try
						{
							CProject* pPrj = pDocData->GetProject();
							const CProject::DatabaseProp prop = pPrj->GetDatabaseProp();
							CAppPostgreSQL database(pPrj->name() , prop.Host.c_str() , prop.User.c_str() , prop.Password.c_str());
							try
							{
								soci::transaction txn(*database.session());
								CString rSql , rNames , rValues;
								for(int i = 0;i < int(pRevRecord->GetFieldCount());++i)
								{
									CDataField* pField = pRevRecord->GetFieldAt(i);
									rNames += CString(pField->name()) + _T(",");
									rValues+= _T("'") + CString(pField->value())+ _T("',");
								}
								rNames = rNames.Left(rNames.GetLength() - 1);
								rValues= rValues.Left(rValues.GetLength() - 1);

								rSql.Format(_T("INSERT INTO T_ISO_REVISION(%s) VALUES(%s)") , rNames , rValues);
								(*database.session()) << rSql.operator LPCTSTR();
								pRevRecordSet->Add(pRevRecord);

								list<CLineDataRecord*> oLineDataRecordList;
								oLineDataRecordList.push_back(pLineDataRecord);
								m_pLineDataForm->SavePartDataOf(_T("REV_NO"),stage.rText,oLineDataRecordList);

								txn.commit();

								if(m_pLineDataForm) m_pLineDataForm->UpdateLineDataRecord(-1 , pLineDataRecord);
							}
							catch(const std::exception& ex)
							{
								CRevDataRecord::DeleteInstance(pRevRecord);
								rLog += ex.what();
							}
						}
						catch(const std::exception& ex)
						{
							CRevDataRecord::DeleteInstance(pRevRecord);
							rLog += ex.what();
						}
					}
					catch(std::exception& ex)
					{
						CRevDataRecord::DeleteInstance(pRevRecord);
						rLog += ex.what();
					}

					return true;
				}
				else
				{
					CRevDataRecord::DeleteInstance(pRevRecord);
					rLog += _TR("Fail to get next revision no");
				}
			}
		}
	}

	return false;
}

/**
	@brief	
	@author	HumKyung
	@date	2010.03.25
	@param
	@return
**/
int CLineDataEditDlg::CheckSelectedGrade(const int& nSelGrade)
{
	/*CLineDataRecord* pDataRecord = NULL;
	POSITION pos = m_wndQualifedLineCtrl.GetFirstSelectedItemPosition();
	while(pos)
	{
		const int nItem = m_wndQualifedLineCtrl.GetNextSelectedItem(pos);
		pDataRecord = (CLineDataRecord*)m_wndQualifedLineCtrl.GetItemData(nItem);
	}
	
	if(pDataRecord)
	{
		CAppDocData* pDocData = CAppDocData::GetInstance();
				
		int nCurGrade = -1;
		CRevDataRecord* pRevRecord = pDataRecord->GetRevDataRecordSet()->FindLatestRevDataRecord();
		if(NULL != pRevRecord)
		{
			nCurGrade = atoi(pRevRecord->FindWithName(_T("REV_NO"))->value());
			nCurGrade = int(nCurGrade / 100) * 100;
		}
		
		return (nSelGrade >= nCurGrade) ? ERROR_SUCCESS : ERROR_BAD_ENVIRONMENT;
	}*/

	return ERROR_BAD_ENVIRONMENT;
}

/**
	@brief

	@author	HumKyung

	@date	2011.02.07

	@param

	@return
**/
void CLineDataEditDlg::OnUpdateDeleteIssue(CCmdUI* pCmdUI)
{
	if(m_wndRevDataGrid.GetSelectedCount() > 0)
	{
		CCellRange cellRange = m_wndRevDataGrid.GetSelectedCellRange();
		const int iMinRow = cellRange.GetMinRow();
		CGridCellBase* pCell = m_wndRevDataGrid.GetCell(iMinRow , 1);
		const COLORREF clr = pCell->GetBackClr();

		pCmdUI->Enable(clr != RGB(200,200,200));
	}else	pCmdUI->Enable(FALSE);
}

/**
	@brief
	@author	HumKyung
	@date	2011.02.07
	@param
	@return
**/
void CLineDataEditDlg::OnDeleteIssue()
{
	if(IDYES == AfxMessageBox(_T("리비전 데이타를 삭제하시겠습니까?") , MB_YESNO | MB_ICONWARNING))
	{
		/*CLineDataRecord* pLineDataRecord = NULL;
		POSITION pos = m_wndQualifedLineCtrl.GetFirstSelectedItemPosition();
		while(pos)
		{
			const int nItem = m_wndQualifedLineCtrl.GetNextSelectedItem(pos);
			pLineDataRecord = (CLineDataRecord*)m_wndQualifedLineCtrl.GetItemData(nItem);
			break;
		}

		if((NULL != pLineDataRecord) && (m_wndRevDataGrid.GetSelectedCount() > 0))
		{
			CCellRange cellRange = m_wndRevDataGrid.GetSelectedCellRange();
			const int iMinRow = cellRange.GetMinRow();
			CRevDataRecord* pRecord = (CRevDataRecord*)m_wndRevDataGrid.GetItemData(iMinRow , 0);
			if(pRecord)
			{
				const CString sKey = pRecord->Find(_T("KEY"))->value();
				const CString sRevNo = pRecord->Find(_T("REV_NO"))->value();
				const int res = pLineDataRecord->GetRevDataRecordSet()->DeleteRecord(pRecord);
				if(ERROR_SUCCESS == res)
				{
					CAppDocData* pDocData = CAppDocData::GetInstance();
					const CString strFilePath = pDocData->m_ProjectPath;
					const CString strFileName = pDocData->m_ProjectName;
					
					CADODB adoDB;
					const CString rConnectionString = PROVIDER + strFilePath + _T("\\DataBase\\") + strFileName + _T(".mdb");
					if(adoDB.DBConnect(rConnectionString.operator LPCTSTR()))
					{

						CString sQuery;
						sQuery.Format(_T("DELETE * FROM T_ISO_REVISION WHERE KEY='%s' AND REV_NO='%s'") , sKey , sRevNo);
						adoDB.ExecuteQuery(sQuery.operator LPCTSTR());
					}

					DisplayRevisionDataRecordSet(pLineDataRecord);
				}
			}
		}*/
	}
}

BOOL CLineDataEditDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	return CFormView::OnHelpInfo(pHelpInfo);
}

/******************************************************************************
    @author     humkyung
    @date       2012-08-28
    @function   DetermineLayout
    @return     int
    @param      const   int&
    @param      cx      const
    @param      int&    cy
    @brief		recalculate layout
******************************************************************************/
int CLineDataEditDlg::DetermineLayout(const int& cx , const int& cy)
{
	CRect rect;
	rect.SetRectEmpty();

	/*CWnd* pWnd = GetDlgItem(IDC_STATIC_SELECTED_INFO);
	if(pWnd)
	{
		CRect rectThis;
		pWnd->GetWindowRect(&rectThis);
		ScreenToClient(&rectThis);

		pWnd->SetWindowPos(NULL, 5 , 5, 100 , rectThis.Height() , SWP_NOZORDER | SWP_NOSIZE);
	}	

	pWnd = GetDlgItem(IDC_STATIC_SELECTED_COUNT);
	if(pWnd)
	{
		CRect rectThis;
		pWnd->GetWindowRect(&rectThis);
		ScreenToClient(&rectThis);

		CWnd* pWnd1 = GetDlgItem(IDC_STATIC_SELECTED_INFO);
		if(pWnd1) pWnd1->GetWindowRect(&rect);
		ScreenToClient(&rect);

		pWnd->SetWindowPos(NULL, rect.right + 5, rect.top , 100 , rect.Height(), SWP_NOZORDER);
	}*/

	CRect resize;
	if(m_wndResizeCtrl.GetSafeHwnd())
	{
		m_wndResizeCtrl.GetWindowRect(&resize);
		ScreenToClient(&resize);
	}

	CWnd *pWnd = GetDlgItem(IDC_STATIC_REVISION_DATA);
	if(pWnd)
	{
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);
		pWnd->SetWindowPos(NULL, 0 , 0 , (resize.left - rect.left - 5) , cy , SWP_NOMOVE | SWP_NOZORDER);
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);

		if(m_wndRevDataGrid.GetSafeHwnd())
		{
			m_wndRevDataGrid.SetWindowPos(NULL, rect.left + 10 , rect.top + 20 , rect.Width() - 20 , rect.Height() - 25 , SWP_NOZORDER);
		}
	}

	pWnd = GetDlgItem(IDC_STATIC_REVISION_STATE);
	if(pWnd)
	{
		pWnd = GetDlgItem(IDC_STATIC_REVISION_DATA);
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);

		pWnd = GetDlgItem(IDC_STATIC_REVISION_STATE);
		pWnd->SetWindowPos(NULL, resize.right + 5 , rect.top , int((cx - rect.right) * 0.5 - 10) , rect.Height(), SWP_NOZORDER);
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);

		/*if(m_wndGradeDefineListCtrl.GetSafeHwnd())
		{
			if(!m_wndRevNoInGradeListCtrl.IsWindowVisible())
			{
				m_wndGradeDefineListCtrl.SetWindowPos(NULL, rect.left + 5 , rect.top + 20 , rect.Width() - 10 , rect.Height() - 30 , SWP_NOZORDER);
			}
			else
			{
				CRect _rect;
				m_wndGradeDefineListCtrl.SetWindowPos(NULL, rect.left + 5 , rect.top + 20 , rect.Width() - 10 , int((rect.Height() - 20)*0.5) , SWP_NOZORDER);
				m_wndGradeDefineListCtrl.GetWindowRect(&_rect);
				ScreenToClient(&_rect);

				m_wndRevNoInGradeListCtrl.SetWindowPos(NULL , rect.left + 5 , _rect.bottom + 5 , rect.Width() - 10 , int((rect.Height() - 20)*0.5) - 5 , SWP_NOZORDER);
			}
		}*/
	}

	pWnd = GetDlgItem(IDC_STATIC_REVISION_DATA);
	if(NULL != pWnd)
	{
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);

		pWnd = GetDlgItem(IDC_STATIC_ISSUE_DATE);
		if(NULL != pWnd)
		{
			pWnd->SetWindowPos(NULL, rect.right + 15 , rect.top , cx - (rect.right+20) , rect.Height(), SWP_NOZORDER);
			pWnd->GetWindowRect(&rect);
			ScreenToClient(&rect);
		}

		if(m_wndIssueDateListCtrl.GetSafeHwnd())
		{
			rect.DeflateRect(5,20,5,5);
			m_wndIssueDateListCtrl.SetWindowPos(NULL, rect.left , rect.top , rect.Width() , rect.Height() , SWP_NOZORDER);
		}
	}

	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2012-08-28
    @function   OnVertResize
    @return     void
    @brief
******************************************************************************/
void CLineDataEditDlg::OnVertResize() 
{
	CRect rect;
	m_wndResizeCtrl.GetWindowRect( rect );
	ScreenToClient( rect );

	rect.left += m_wndResizeCtrl.m_adjust_width;
	m_wndResizeCtrl.SetWindowPos(NULL ,  rect.left , rect.top , 3 , rect.Height() , SWP_NOZORDER );

	CRect wndRect;
	GetClientRect(&wndRect);
	DetermineLayout(wndRect.Width() , wndRect.Height());

	RedrawWindow();
}

void CLineDataEditDlg::OnLvnItemchangedListIssueDate(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
