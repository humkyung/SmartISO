// CustomDataReportDlg.cpp : implementation file
//

#include "stdafx.h"
#include <assert.h>
#include "MainFrm.h"
#include "SmartReporterDoc.h"
#include "LineListToOraDocData.h"
#include "SmartReporter.h"
#include "CustomDataReportDlg.h"
#include <excel9/ExcelAutomation.h>
#include "WorkStatusDlg.h"

#include "IsString.h"

#include <fstream>
#include <set>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CString _rSourceExcelFilePath , _rOutputExcelFilePath;

CCustomReportData::CCustomReportData(const CCustomReportData& rhs)
{
	*this = rhs;
}

CCustomReportData& CCustomReportData::operator=(const CCustomReportData& rhs)
{
	if(this != &rhs)
	{
		m_vecData.clear();
		m_vecData.insert(m_vecData.begin() , rhs.m_vecData.begin() , rhs.m_vecData.end());
	}

	return (*this);
}

/**
**/
int CCustomReportData::GetColCount() const
{
	return m_vecData.size();
}

/**
	@brief	get query string for custom report

	@author	humkyung

	@date	2011.05.04

	@return	CString
*/
CString CCustomReportData::GetQueryString(const CString& sProjectUnit , const CString &rProjectName, const CString &rTableName , 
					  CCustomReportTable* pCustomReportTable)
{
	assert(pCustomReportTable && "pCustomReportTable is NULL");

	if(NULL != pCustomReportTable)
	{
		STRING_T sSql = (_T("INCH") == sProjectUnit) ? 
			(pCustomReportTable->m_pSqlState[CCustomReportTable::INCH]->SqlStatement()) : (pCustomReportTable->m_pSqlState[CCustomReportTable::METRIC]->SqlStatement());
		
		IsString::ReplaceOf(sSql , _T("{PROJECTNAME}") , rProjectName.operator LPCTSTR());
		IsString::ReplaceOf(sSql , _T("{TABLENAME}") , rTableName.operator LPCTSTR());

		return sSql.c_str();
	}
	
	return CString( _T("") );
}

/**
	@brief

  	@author	humkyung

	@date	2011.05.04

	@param	at
	@param	rValue
	@param	PDSOracle

	@return
**/
void CCustomReportData::InsertAt(const STRING_T& sFieldName , const STRING_T &sValue, CPDSOracle &PDSOracle)
{
	m_vecData.push_back(sValue);
}

/////////////////////////////////////////////////////////////////////////////
// CCustomDataReportDlg dialog


CCustomDataReportDlg::CCustomDataReportDlg(CCustomReportTable* pCustomReportTable , CWnd* pParent /*=NULL*/)
	: CDialog(CCustomDataReportDlg::IDD, pParent) , m_pCustomReportTable(pCustomReportTable)
{
	//{{AFX_DATA_INIT(CCustomDataReportDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCustomDataReportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustomDataReportDlg)
	DDX_Control(pDX , IDC_STATIC_LABEL , m_wndLabelStatic);

	DDX_Control(pDX , IDC_BUTTON_EXCEL_EXPORT , m_wndExcelExportButton);
	DDX_Control(pDX , IDC_BUTTON_QUERY , m_wndQueryButton);

	DDX_Control(pDX , IDC_SELECT_ALL_AREA , m_wndSelectAllAreaNoButton);
	
	DDX_Control(pDX , IDC_TREE_AREA , m_wndCheckAreaTreeCtrl);
	DDX_Control(pDX , IDC_BUTTON_REFRESH , m_wndRefreshButton);

	DDX_Control(pDX , IDC_LIST_LINE_LIST , m_wndReportCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCustomDataReportDlg, CDialog)
	//{{AFX_MSG_MAP(CCustomDataReportDlg)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_EXCEL_EXPORT, OnButtonExcelExport)
	ON_BN_CLICKED(IDC_BUTTON_QUERY, OnButtonQuery)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, OnButtonRefresh)
	ON_BN_CLICKED(IDC_SELECT_ALL_AREA, OnSelectAllArea)
	//}}AFX_MSG_MAP
	ON_WM_HELPINFO()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustomDataReportDlg message handlers

void CCustomDataReportDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	const int GAP = 5;

	CRect StaticRect;
	CWnd* pWnd = GetDlgItem(IDC_STATIC_AREA_NO);
	if(pWnd)
	{
		pWnd->GetWindowRect(&StaticRect);
		ScreenToClient(&StaticRect);
		pWnd->SetWindowPos(NULL , 0 , 0 , StaticRect.Width() , cy - StaticRect.top , SWP_NOZORDER | SWP_NOMOVE);
		pWnd->GetWindowRect(&StaticRect);
		ScreenToClient(&StaticRect);
		
		if(m_wndCheckAreaTreeCtrl.GetSafeHwnd())
		{
			CRect rect;
			m_wndCheckAreaTreeCtrl.GetWindowRect(&rect);
			ScreenToClient(&rect);
			
			int dx = rect.left - StaticRect.left;
			int dy = rect.top  - StaticRect.top;
			m_wndCheckAreaTreeCtrl.SetWindowPos(NULL , 0 , 0 , StaticRect.Width() - 2*dx, StaticRect.Height() - (dy + 2*GAP) , SWP_NOZORDER | SWP_NOMOVE);
		}
	}

	if(m_wndRefreshButton.GetSafeHwnd())
	{
		CRect rect;
		m_wndRefreshButton.GetWindowRect(&rect);
		ScreenToClient(&rect);

		m_wndRefreshButton.SetWindowPos(NULL , StaticRect.right + GAP , (int)((cy - rect.Height()) * 0.5) , 0 , 0 , SWP_NOZORDER | SWP_NOSIZE);
	}

	if(m_wndReportCtrl.GetSafeHwnd())
	{
		CRect rect;

		if(m_wndExcelExportButton.GetSafeHwnd())
		{
			m_wndExcelExportButton.GetWindowRect(rect);
			ScreenToClient(&rect);
			if(m_wndRefreshButton.GetSafeHwnd())
			{
				m_wndRefreshButton.GetWindowRect(&StaticRect);
				ScreenToClient(&StaticRect);
				
				m_wndReportCtrl.SetWindowPos(NULL , StaticRect.right + GAP , rect.bottom + GAP , cx - StaticRect.right - GAP , cy - rect.bottom - GAP , SWP_NOZORDER);
				m_wndReportCtrl.GetWindowRect(&rect);
				ScreenToClient(&rect);

				if(m_wndLabelStatic.GetSafeHwnd())
				{
					CRect rectThis;
					m_wndLabelStatic.GetWindowRect(&rectThis);
					ScreenToClient(&rectThis);
					m_wndLabelStatic.SetWindowPos(NULL , rect.left , GAP , cx - rect.left , rectThis.Height() , SWP_NOZORDER);
				}
			}
		}
	}
}

/**
	@brief	

	@author	humkyung

	@date	2011.05.04

	@param

	@return
**/
BOOL CCustomDataReportDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_wndLabelStatic.SetWindowText(m_pCustomReportTable->m_sName.c_str());

	m_wndLabelStatic.SetBorder(TRUE);
	m_wndLabelStatic.SetFont3D(TRUE);
	m_wndLabelStatic.SetText3DHiliteColor(RGB(255,0,255));

	if(m_imgTree.Create( 16 , 16 , ILC_COLOR32  , 1 , 1))
	{
		m_bitmap.LoadBitmap(IDB_TREE_IMG);
		m_imgTree.Add(&m_bitmap, RGB(0,0,0));

		m_wndCheckAreaTreeCtrl.SetImageList(&m_imgTree , TVSIL_NORMAL);
	}

	if(m_imgState.Create( IDB_STATE, 13, 1, RGB(255,255,255) ))
	{
		m_wndCheckAreaTreeCtrl.SetImageList( &m_imgState , TVSIL_STATE );
	}
	
	m_wndRefreshButton.SetIcon(IDI_REFRESH , 24 , 24);

	short shBtnColor = 30;
	m_wndQueryButton.SetGradientColor(DarkenColor(GetSysColor(COLOR_BTNFACE),0.15) , GetSysColor(COLOR_BTNFACE) , CButtonST::VertBar);
	m_wndQueryButton.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_wndQueryButton.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(0, 128, 0));
	m_wndQueryButton.SetBtnCursor(IDC_APP_HAND);

	m_wndExcelExportButton.SetGradientColor(DarkenColor(GetSysColor(COLOR_BTNFACE),0.15) , GetSysColor(COLOR_BTNFACE) , CButtonST::VertBar);
	m_wndExcelExportButton.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_wndExcelExportButton.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(0, 128, 0));
	m_wndExcelExportButton.SetBtnCursor(IDC_APP_HAND);
	m_wndExcelExportButton.SetIcon(IDI_SHEET , 16 , 16);

	m_wndSelectAllAreaNoButton.SetGradientColor(DarkenColor(GetSysColor(COLOR_BTNFACE),0.15) , GetSysColor(COLOR_BTNFACE) , CButtonST::VertBar);
	m_wndSelectAllAreaNoButton.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_wndSelectAllAreaNoButton.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(0, 128, 0));
	m_wndSelectAllAreaNoButton.SetBtnCursor(IDC_APP_HAND);
	m_wndSelectAllAreaNoButton.SetIcon(IDI_CHECK , 16 , 16);

	if(m_imgList.Create(IDB_BITMAP_LIST_HEAD , 8 , 8 , RGB(255 , 0 , 255)))
	{
		///m_wndReportCtrl.SetHeaderImageList(&m_imgList);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/**	
	@brief	query한 내용을 display한다.
**/
void CCustomDataReportDlg::UpdateReportData()
{
	m_wndCheckAreaTreeCtrl.DeleteAllItems();
	for(vector<CPDSArea>::iterator itr = m_AreaNameEntry.begin();itr != m_AreaNameEntry.end();++itr)
	{
		HTREEITEM hItem = m_wndCheckAreaTreeCtrl.InsertItem(itr->name() , 0 , 0);
		m_wndCheckAreaTreeCtrl.CheckItem(hItem , CCheckTreeCtrl::CHECKED);
		size_t nCount = itr->GetModelCount();
		for(int i = 0;i < (int)nCount;++i)
		{
			CPDSModel* pModelRef = itr->GetModelAt(i);
			HTREEITEM hChildItem = m_wndCheckAreaTreeCtrl.InsertItem( pModelRef->name() , 1 , 1 , hItem);
			m_wndCheckAreaTreeCtrl.CheckItem(hChildItem , CCheckTreeCtrl::CHECKED);
			m_wndCheckAreaTreeCtrl.SetItemData(hChildItem , (DWORD)(pModelRef));
		}
	}

	m_wndReportCtrl.SetRedraw(FALSE);
	if(0 == m_wndReportCtrl.GetColumnCount())
	{
		CString sHeadings;
		for(list<STRING_T>::iterator itr = m_lstHeaderString.begin();itr != m_lstHeaderString.end();++itr)
		{
			sHeadings += CString(itr->c_str()) + _T(",100;");
		}
		if(_T(";") == sHeadings.Right(1)) sHeadings = sHeadings.Left(sHeadings.GetLength() - 1);
		m_wndReportCtrl.SetHeadings(sHeadings);
	}
	
	m_wndReportCtrl.DeleteAllItems();

#ifdef _DEBUG
	/*CCustomReportData test;
	test.m_vecData.push_back(_T("aaa"));
	m_CustomReportDataEntry.push_back(test);*/
#endif

	LVITEM lvitem;
	memset(&lvitem , '\0' , sizeof(LVITEM));
	lvitem.mask = LVIF_TEXT/* | LVIF_IMAGE*/;
	lvitem.iImage = 0;
	lvitem.cchTextMax = 32;
	for(vector<CCustomReportData>::iterator jtr = m_CustomReportDataEntry.begin();jtr !=  m_CustomReportDataEntry.end();++jtr)
	{
		lvitem.iItem = m_wndReportCtrl.GetItemCount();
		lvitem.iSubItem = 0;
		///lvitem.iImage = 0;
		lvitem.pszText = LPTSTR((jtr)->m_vecData[0].c_str());
		const int nItem = m_wndReportCtrl.InsertItem(&lvitem);
		for(int i = 1;i < (jtr)->GetColCount();++i)
		{
			m_wndReportCtrl.SetItemText(nItem , i , (jtr)->m_vecData[i].c_str());
		}
		m_wndReportCtrl.SetItemData(nItem , (DWORD)&(*jtr));
	}
	m_wndReportCtrl.SetRedraw(TRUE);
}

/**
**/
void CCustomDataReportDlg::OnSelectAllArea() 
{
	CheckAllArea(m_wndCheckAreaTreeCtrl.GetRootItem());
}


/**	
	@brief	oracle report를 엑셀 파일로 저장한다.

	@author	humkyung

	@date	2011.05.04

	@param	

	@return
**/
void CCustomDataReportDlg::OnButtonExcelExport() 
{
	CWorkStatusDlg dlg;
	dlg.m_pThread = AfxBeginThread(StatusExportThreadEntry , this, THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	
	if (NULL == dlg.m_pThread)
	{
		AfxMessageBox( _T("Can't create thread!!!") );
	}
	else
	{
		CMainFrame* pFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
		CSmartReporterDoc* pDoc = static_cast<CSmartReporterDoc*>(pFrame->GetActiveDocument());
		CLineListToOraDocData& docData = CLineListToOraDocData::GetInstance();

		_rSourceExcelFilePath = CString(CLineListToOraDocData::GetExecPath().c_str()) + _T("\\Excel\\") + m_pCustomReportTable->m_sExcelFile.c_str();
		
		CTime t = CTime::GetCurrentTime();
		CString rCurrentDate = t.Format( _T("%Y%m%d") );
		_rOutputExcelFilePath = CString(CLineListToOraDocData::GetExecPath().c_str()) + _T("\\Report\\");
		_rOutputExcelFilePath+= docData.GetOraProjectName() + "_" + CString(m_pCustomReportTable->m_sExcelFile.c_str()) +  _T("_") + rCurrentDate + _T(".xls");

		if(IDOK == dlg.DoModal())
		{
			::ShellExecute(NULL , _T("open") , _T("EXCEL.EXE") , _T("\"") + _rOutputExcelFilePath + _T("\"") , NULL , SW_SHOW);
		}
	}
}

/**
	@brief	query custom data

	@author	humkyung

	@date	2011.05.04

	@param

	@return
**/
void CCustomDataReportDlg::OnButtonQuery() 
{
	CWorkStatusDlg dlg(this);
	dlg.m_pThread = AfxBeginThread(StatusQueryThreadEntry, this, THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	
	if (NULL == dlg.m_pThread)
	{
		AfxMessageBox( _T("Can't create thread!!!") );
	}
	else
	{
		if(IDOK == dlg.DoModal())
		{
			UpdateReportData();
			SyncAreaDataWith(m_wndCheckAreaTreeCtrl.GetRootItem());
		}
	}
}

/**
	@brief	save query data as excel file

	@author	humkyung

	@date	2011.05.04

	@param

	@return
**/
void CCustomDataReportDlg::SaveDataForReport()
{
	CLineListToOraDocData& docData = CLineListToOraDocData::GetInstance();

	try
	{
		CWorkStatusDlg* pWorkStatusDlg = CWorkStatusDlg::GetInstance();

		CString rColString( _T("ABCDEFGHIJKLMNOPQRSTUVWXYZ") );
		CExcelAutomation ExcelAutomation;
		if(ExcelAutomation.Open(_rSourceExcelFilePath))
		{
			/// PROJECT NAME을 출력한다.
			ExcelAutomation.SetCellValue( _T("B2") , docData.GetOraProjectName());
			
			/// REPORT DATE를 출력한다.
			CTime t = CTime::GetCurrentTime();
			CString rCurrentDate = t.Format( _T("%Y-%m-%d") );
			ExcelAutomation.SetCellValue( _T("U2") , rCurrentDate);
		}

		{
			CString rTitle( _T("Export Excel...") );

			int nRow = ATOI_T(m_pCustomReportTable->m_sStartRow.c_str());
			const size_t nCount = m_wndReportCtrl.GetItemCount();
			for(int i = 0;i < (int)nCount;++i)
			{
				CCustomReportData* pData = (CCustomReportData*)(m_wndReportCtrl.GetItemData(i));
				if(pData)
				{
					CString rArea = pData->m_vecData[CCustomReportData::AREA_NAME].c_str();
					CString rModel= pData->m_vecData[CCustomReportData::MODEL_NO].c_str();
					if(IsSelected(rArea , rModel))
					{
						CString rCellNo;
						for(int j = 0;j < pData->GetColCount();++j)
						{
							rCellNo.Format( _T("%c%d") , rColString.GetAt(j) , nRow);
							ExcelAutomation.SetCellValue(rCellNo , pData->m_vecData[j].c_str());

							int process = (int)(((double)i / (double)nCount) * 100.f);
							if(pWorkStatusDlg) pWorkStatusDlg->UpdateWorkStatus(rTitle , process);
						}
						nRow++;
					}
				}
			}

			ExcelAutomation.SaveAs(_rOutputExcelFilePath);
			ExcelAutomation.Close();
		}
	}
	catch(const exception& ex)
	{
		AfxMessageBox( CString(ex.what()) );
	}
}

/**
**/
void CCustomDataReportDlg::OnButtonRefresh() 
{
	SyncAreaDataWith(m_wndCheckAreaTreeCtrl.GetRootItem());

	m_wndReportCtrl.SetRedraw(FALSE);
	m_wndReportCtrl.DeleteAllItems();

	LVITEM lvitem;
	lvitem.mask = LVIF_TEXT/* | LVIF_IMAGE*/;
	lvitem.iImage = 0;
	lvitem.cchTextMax = 32;
	for(vector<CCustomReportData>::iterator jtr = m_CustomReportDataEntry.begin();jtr !=  m_CustomReportDataEntry.end();++jtr)
	{
		CString rArea = jtr->m_vecData[CCustomReportData::AREA_NAME].c_str();
		CString rModel= jtr->m_vecData[CCustomReportData::MODEL_NO].c_str();
		if(IsSelected(rArea , rModel))
		{
			lvitem.iItem = m_wndReportCtrl.GetItemCount();
			lvitem.iSubItem = 0;
			lvitem.pszText = LPTSTR((jtr)->m_vecData[0].c_str());
			const int nItem = m_wndReportCtrl.InsertItem(&lvitem);
			for(int i = 1;i < (jtr)->GetColCount();++i)
			{
				m_wndReportCtrl.SetItemText(nItem , i , (jtr)->m_vecData[i].c_str());
			}
			m_wndReportCtrl.SetItemData(nItem , (DWORD)&(*jtr));
		}
	}
	m_wndReportCtrl.SetRedraw(TRUE);
}

/**	\brief	
**/
BOOL CCustomDataReportDlg::IsSelected(const CString &rArea, const CString &rModel)
{
	for(vector<CPDSArea>::iterator itr = m_AreaNameEntry.begin();itr != m_AreaNameEntry.end();++itr)
	{
		if(rArea == itr->name())
		{
			return itr->IsModelSelected(rModel);
		}
	}

	return FALSE;
}

/**	
	@brief	TreeCtrl의 선택된 상태를 data와 일치시킨다.
**/
void CCustomDataReportDlg::SyncAreaDataWith(HTREEITEM hItem)
{
	if(NULL != hItem)
	{
		CPDSModel* pModel = (CPDSModel*)(m_wndCheckAreaTreeCtrl.GetItemData(hItem));
		if(NULL != pModel)
		{
			pModel->Select(m_wndCheckAreaTreeCtrl.IsSelected(hItem));
		}
		SyncAreaDataWith(m_wndCheckAreaTreeCtrl.GetChildItem(hItem));

		HTREEITEM hSiblingItem = hItem;
		do
		{
			hSiblingItem = m_wndCheckAreaTreeCtrl.GetNextSiblingItem(hSiblingItem);
			if(hSiblingItem)
			{
				CPDSModel* pModel = (CPDSModel*)(m_wndCheckAreaTreeCtrl.GetItemData(hSiblingItem));
				if(NULL != pModel)
				{
					pModel->Select(m_wndCheckAreaTreeCtrl.IsSelected(hSiblingItem));
				}
				SyncAreaDataWith(m_wndCheckAreaTreeCtrl.GetChildItem(hSiblingItem));
			}
		}while(NULL != hSiblingItem);
	}
}

/**
**/
void CCustomDataReportDlg::CheckAllArea(HTREEITEM hItem)
{
	if(NULL != hItem)
	{
		m_wndCheckAreaTreeCtrl.CheckItem(hItem , CCheckTreeCtrl::CHECKED);
		CheckAllArea(m_wndCheckAreaTreeCtrl.GetChildItem(hItem));

		HTREEITEM hSiblingItem = hItem;
		do
		{
			hSiblingItem = m_wndCheckAreaTreeCtrl.GetNextSiblingItem(hSiblingItem);
			if(NULL != hSiblingItem)
			{
				m_wndCheckAreaTreeCtrl.CheckItem(hSiblingItem , CCheckTreeCtrl::CHECKED);
				CheckAllArea(m_wndCheckAreaTreeCtrl.GetChildItem(hSiblingItem));
			}
		}while(NULL != hSiblingItem);
	}
}

/**
	@brief

	@author	humkyung

	@date	2011.05.04

	@param

	@return
**/
UINT CCustomDataReportDlg::StatusQueryThreadEntry(LPVOID pVoid)
{
	return ((CCustomDataReportDlg*)pVoid)->StatusQueryThread();
}

UINT CCustomDataReportDlg::StatusExportThreadEntry(LPVOID pVoid)
{
	return ((CCustomDataReportDlg*)pVoid)->StatusExportThread();
}

/** 
	@brief	쓰레드 실행부
**/
UINT CCustomDataReportDlg::StatusExportThread()
{
	CWorkStatusDlg* pWorkStatusDlg = CWorkStatusDlg::GetInstance();
	try
	{
		InterlockedExchange((LONG*)(&(pWorkStatusDlg->m_bThreadRunning)) , TRUE);/// so the main thread knows that this thread is still running

		SaveDataForReport();
		
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
		pWorkStatusDlg->SendMessage(WM_COMMAND , IDOK );
	}
	
	return IDCANCEL;
}

/**
	@brief	query custom data

	@author	humkyung

	@date	2011.05.04

	@param

	@return	UNIT
**/
UINT CCustomDataReportDlg::StatusQueryThread()
{
	CWorkStatusDlg* pWorkStatusDlg = CWorkStatusDlg::GetInstance();
	InterlockedExchange((LONG*)(&(pWorkStatusDlg->m_bThreadRunning)) , TRUE);/// so the main thread knows that this thread is still running
	try
	{
		CLineListToOraDocData& docData = CLineListToOraDocData::GetInstance();
		const CString sProjectUnit = docData.GetProjectUnit();
		
		CString rTitle( _T("Query ") + CString(m_pCustomReportTable->m_sName.c_str()) + _T("..."));
		CAdoRemoteDB RemoteDB;
		if(TRUE == RemoteDB.DBConnect(docData.GetConnString().operator LPCTSTR()))
		{
			vector<CString> oTableNames;
			CString rValue;
			
			const CString rProjectName = docData.GetOraProjectName();
			//! 오라클 테이블 이름을 구한다.
			CPDSOracle PDSOracle;
			PDSOracle.Set(docData.GetOraServiceName() , docData.GetOraUserName() , docData.GetOraUserPassword() , rProjectName);
			PDSOracle.LoadNoteTableFromDataFile(docData.GetStdNoteFilePath().operator LPCTSTR());
#ifdef NDEBUG
			if(PDSOracle.Get_PDTABLE_12_TableNames(oTableNames , RemoteDB))
#elif _DEBUG
			oTableNames.push_back( _T("ORACLE") );
			if(1)
#endif
			{
				m_CustomReportDataEntry.clear();

				size_t nCount = oTableNames.size() , index = 0;
				for(vector<CString>::iterator itr = oTableNames.begin();itr != oTableNames.end();++itr , ++index)
				{
					CString rSQL = CCustomReportData::GetQueryString(sProjectUnit , rProjectName , (*itr) , m_pCustomReportTable);
					if(TRUE == RemoteDB.OpenQuery(rSQL.operator LPCTSTR()))
					{	
						LONG lRecordCount = 0;
						RemoteDB.GetRecordCount(&lRecordCount);
						
						const int nFieldCount = RemoteDB.GetFieldCount();
						{
							//! set list control's header
							CString sHeadings;
							for(int i = 0;i < nFieldCount;++i)
							{
								CString sFieldName = RemoteDB.GetFieldNameAt(i).c_str();	///< FIELD 이름을 구한다.
								m_lstHeaderString.push_back(sFieldName.operator LPCTSTR());
							}
						}

						for(int i = 0;i < lRecordCount;++i)
						{
							CCustomReportData oCustomReportData;
							
							STRING_T rValue , rFieldName;
							for(int j = 0;j < nFieldCount;++j)
							{
								rFieldName = RemoteDB.GetFieldNameAt(j).c_str();		///< FIELD 이름을 구한다.
								RemoteDB.GetFieldValue(i , rFieldName , &rValue);	///< FIELD 값을 구한다.
								
								STRING_T sAttributeType;
								if(_T("INCH") == sProjectUnit)
								{
									sAttributeType = m_pCustomReportTable->GetCustomAttribute(CCustomReportTable::INCH , rFieldName);
								}
								else
								{
									sAttributeType = m_pCustomReportTable->GetCustomAttribute(CCustomReportTable::METRIC , rFieldName);
								}

								if(STRING_T( _T("") ) != sAttributeType)
								{
									rValue = m_pCustomReportTable->ParseCustomAttribute(sAttributeType , rValue , PDSOracle).c_str();
								}
								oCustomReportData.InsertAt(rFieldName , rValue , PDSOracle);
							}
							m_CustomReportDataEntry.push_back(oCustomReportData);
						}
					}
					
					int process = (int)(((double)index / (double)nCount) * 100.);
					if(pWorkStatusDlg) pWorkStatusDlg->UpdateWorkStatus(rTitle , process);
				}

				CPDSArea::QueryAreaName(m_AreaNameEntry , RemoteDB , rProjectName , m_pCustomReportTable->m_sDiscipline.c_str());
				for(vector<CPDSArea>::iterator jtr = m_AreaNameEntry.begin();jtr != m_AreaNameEntry.end();++jtr)
				{
					jtr->QueryModelName(RemoteDB , rProjectName , m_pCustomReportTable->m_sDiscipline.c_str());
				}

				InterlockedExchange((LONG*)(&(pWorkStatusDlg->m_bThreadRunning)) , FALSE);
				if(pWorkStatusDlg) pWorkStatusDlg->SendMessage(WM_COMMAND , IDOK );

				return IDOK;
			}
			else
			{
				AfxMessageBox( _T("12번 테이블을 읽을 수 없습니다.") );
			}
		}
		else
		{
			AfxMessageBox(_T("Can't connect to database") , MB_OK);
		}
	}
	catch(const exception& ex)
	{
		AfxMessageBox( CString(ex.what()) );
	}

	InterlockedExchange((LONG*)(&(pWorkStatusDlg->m_bThreadRunning)) , FALSE);
	if(pWorkStatusDlg) pWorkStatusDlg->SendMessage(WM_COMMAND , IDCANCEL);

	return IDCANCEL;
}

/******************************************************************************
    @author     humkyung
    @date       2012-06-03
    @function   OnHelpInfo
    @return     BOOL
    @param      HELPINFO*   pHelpInfo
    @brief
******************************************************************************/
BOOL CCustomDataReportDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	CString sFilePath = STRING_T(CLineListToOraDocData::GetExecPath() + _T("\\Help\\SmartISO.chm::/Smart Reporter.htm")).c_str();
	HWND hHtml = ::HtmlHelp(this->m_hWnd ,  sFilePath , HH_DISPLAY_TOPIC , 0);

	return TRUE;///CDialog::OnHelpInfo(pHelpInfo);
}
