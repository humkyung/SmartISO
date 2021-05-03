// PDSLineDataForm.cpp : implementation file
//

#include "stdafx.h"
#include "UserApp.h"
#include "PDSLineDataForm.h"
#include "UserAppView.h"
#include "tokenizer.h"
#include "IsString.h"
#include "IsoEditSettingPage.h"
#include "GetPds.h"
#include "WorkStatusDlg.h"
#include "SQLFile.h"

#include <algorithm>
#include <functional>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static vector<string> __LOG_STRING_ENTRY_;
/////////////////////////////////////////////////////////////////////////////
// CPDSLineDataForm

IMPLEMENT_DYNCREATE(CPDSLineDataForm, CFormView)

CPDSLineDataForm::CPDSLineDataForm()
	: CFormView(CPDSLineDataForm::IDD) , m_pLineDataRecordSet(NULL)
{
	//{{AFX_DATA_INIT(CPDSLineDataForm)
	m_nRecordCount = 0;		
	//}}AFX_DATA_INIT
}

CPDSLineDataForm::~CPDSLineDataForm()
{
	try
	{
		if(NULL != m_pLineDataRecordSet)
		{
			CLineDataRecordSet::DeleteInstance(m_pLineDataRecordSet);
		}
	}
	catch(...)
	{

	}
}

void CPDSLineDataForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPDSLineDataForm)
	DDX_Control( pDX , IDC_LIST_LINE_DATA , m_wndLineDataReport );
	DDX_Control( pDX , IDC_BUTTON_SELECT_ALL , m_selectAllButton );
	DDX_Control( pDX , IDC_BUTTON_UN_SELECT_ALL , m_unselectAllButton );
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPDSLineDataForm, CFormView)
	//{{AFX_MSG_MAP(CPDSLineDataForm)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_SELECT_ALL, OnButtonSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_UN_SELECT_ALL, OnButtonUnSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_GET , OnButtonGet)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, OnButtonDown)
	ON_BN_CLICKED(IDC_BUTTON_UP, OnButtonUp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPDSLineDataForm diagnostics

#ifdef _DEBUG
void CPDSLineDataForm::AssertValid() const
{
	CFormView::AssertValid();
}

void CPDSLineDataForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPDSLineDataForm message handlers

BOOL CPDSLineDataForm::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

/**	\brief	The CPDSLineDataForm::OnInitialUpdate function


	\return	void	
*/
void CPDSLineDataForm::OnInitialUpdate() 
{
	static bool __init = false;
	CFormView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	if(false == __init)
	{
		CRect rectDummy;
		rectDummy.SetRectEmpty ();
		GetClientRect(rectDummy);
		if (!m_TabCtrl.Create (CBCGTabWnd::STYLE_3D, rectDummy, this, 0x100 , CBCGTabWnd::LOCATION_TOP))
		{
			TRACE0("Failed to create workspace tab window\n");
			return;      // fail to create
		}
		if( m_ImageList.Create(IDB_USER_APP , 16 , 16 , RGB(255 , 0 , 255)) )
		{
			m_TabCtrl.SetImageList(m_ImageList);
		}
		m_TabCtrl.SetActiveTabTextColor(RGB(0 , 0 , 255));
		
		CUserAppView* pView = (CUserAppView*)(GetParent());
		CUserAppDoc*  pDoc  = pView->GetDocument();
		CUserAppDocData* pDocData = CUserAppDocData::GetInstance();
		
		m_opt1.m_pUserAppDoc = pDoc;
		/// here we insert the dialog test page as a child
		if(TRUE == m_opt1.Create(CLineListSelectOption1::IDD, &m_TabCtrl))
		{
			m_TabCtrl.AddTab(&m_opt1 , "Query" , 2);
		}

		//m_wndLineDataReport.SetCheckboxes();
		m_wndLineDataReport.SetImageList(&m_ImageList , LVSIL_SMALL);
		m_wndLineDataReport.SetHeaderImageList();
		
		m_selectAllButton.SetIcon(IDI_SELECT_ALL , 16 , 16);
		m_selectAllButton.SetBtnCursor(IDC_HAND);

		m_unselectAllButton.SetIcon(IDI_UNSELECT_ALL , 16 , 16);
		m_unselectAllButton.SetBtnCursor(IDC_HAND);

		__init = true;
	}
}

/**	
	@brief	PDS oracle data를 가지고 IsoMan Database를 갱신한다.
	
	@author	BHK

	@return	void	
*/
void CPDSLineDataForm::OnButtonGet()
{
	CUserAppView* pView = (CUserAppView*)(GetParent());
	CUserAppDoc*  pDoc  = pView->GetDocument();
	CString rIniFilePath = pDoc->GetCurProject()->GetIniFilePath();

	char szBuf[MAX_PATH + 1] = {'\0',};
	static const CString rApp( "Oracle Setting" );
	if(GetPrivateProfileString(rApp,"Service Name",NULL,szBuf,MAX_PATH,rIniFilePath))
	{
		m_rServiceName = szBuf;
	}else	m_rServiceName.Empty();
	
	if(GetPrivateProfileString(rApp,"User Name",NULL,szBuf,MAX_PATH,rIniFilePath))
	{
		m_rUserId = szBuf;
	}else	m_rUserId.Empty();
	
	if(GetPrivateProfileString(rApp,"Password",NULL,szBuf,MAX_PATH,rIniFilePath))
	{
		m_rPassword = szBuf;
	}else	m_rPassword.Empty();
	
	if(GetPrivateProfileString(rApp,"PDS Project Name",NULL,szBuf,MAX_PATH,rIniFilePath))
	{
		m_rPDSProjectName = szBuf;
	}else m_rPDSProjectName.Empty();
	
	if(m_rServiceName.IsEmpty() || m_rUserId.IsEmpty() || m_rPassword.IsEmpty() || m_rPDSProjectName.IsEmpty()) 
	{
		AfxMessageBox("Please, Check Oracle Setting");
		return;
	}

	CWorkStatusDlg dlg;
	dlg.m_pThread = AfxBeginThread(StatusThreadEntry, this, THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	
	if (NULL == dlg.m_pThread)
	{
		AfxMessageBox("Can't create thread!!!");
	}
	else
	{
		if(IDOK == dlg.DoModal())
		{
			CString rMsg;
			rMsg.LoadString(IDS_UPDATE_DATABASE_OK);
			AfxMessageBox(rMsg);

			/// PLANT NO , REV NO COMBO BOX를 갱신한다.
			CLineListSelectOption1::UpdatePlantNoListBox();
			CLineListSelectOption1::UpdateRevNoComboBox();

			DisplayLineDataRecordSet();
			m_pGetPdsForm->ClearContents();

			CUserAppDocData* pDocData = CUserAppDocData::GetInstance();
			/// LOG 파일을 저장한다.
			if(!__LOG_STRING_ENTRY_.empty())
			{
				CString rTempFolder = pDocData->GetTempFolder();
				if("\\" != rTempFolder) rTempFolder += "\\";
				ofstream ofile(rTempFolder + "Get PDS Line Data.log");
				
				for(vector<string>::iterator itr = __LOG_STRING_ENTRY_.begin();itr != __LOG_STRING_ENTRY_.end();++itr)
				{
					ofile << itr->c_str() << std::endl;
				}
				
				::ShellExecute(NULL, "open", "notepad.exe", rTempFolder + "Get PDS Line Data.log" , NULL, SW_SHOW);
			}
		}
		else
		{
			CString rMsg;
			rMsg.LoadString(IDS_UPDATE_DATABASE_ERROR);
			AfxMessageBox(rMsg);
		}
	}
}

/**	
	@brief	컨트롤들을 초기화한다.

	@author	HumKyung.BAEK

	@return	void	
*/
void CPDSLineDataForm::InitContents()
{
	CUserAppView* pView = (CUserAppView*)(GetParent());
	CUserAppDoc*  pDoc  = pView->GetDocument();

	CString rIniFilePath = pDoc->GetCurProject()->GetIniFilePath();
	char szBuf[MAX_PATH] = {'\0' ,};
	
	if(GetPrivateProfileString(_T("Iso Edit Setting") , _T("Source Iso File Folder") , _T("") ,szBuf,MAX_PATH,rIniFilePath))
	{
		m_rDWGFilePath = szBuf;
		if(m_rDWGFilePath.Right(1) != _T("\\")) m_rDWGFilePath += _T("\\");
	}

	CString rApp( _T("Line Data Field") );
	int nCount = 0;
	if(GetPrivateProfileString(rApp , _T("Count") , NULL , szBuf , MAX_PATH , rIniFilePath))
	{
		nCount = atoi(szBuf);
	}
	
	CUserAppDocData* pDocData = CUserAppDocData::GetInstance();
	ofstream __TEST_FILE__(pDocData->GetTempFolder() + _T("\\TEST.TXT") );
	
	CString rSqlFilePath = CUserAppDocData::GetExecPath() + _T("\\Setting\\T_LINE_DATA.SQL");
	CSQLFile sqlFile(rSqlFilePath.operator LPCSTR());
	if(ERROR_SUCCESS == sqlFile.Parse())
	{
		nCount = sqlFile.GetFieldCount();
		vector<CString> HeadingEntry;
		vector<string> oResult; 
		for(int i = 0;i < nCount;++i)
		{
			CString rKey;
			rKey.Format(_T("Display%d") , i);
			if(GetPrivateProfileString(rApp , rKey , NULL , szBuf , MAX_PATH , rIniFilePath))
			{
				if(CString( _T("True") ) != szBuf) continue;
				
				CString rHeading;
				rHeading += sqlFile.GetFieldNameAt(i).c_str() + CString( _T(",150") );
				HeadingEntry.push_back(rHeading);
			}
		}
		
		CString rHeadings;
		for(vector<CString>::iterator itr = HeadingEntry.begin();itr != HeadingEntry.end();itr++)
		{
			rHeadings += (*itr);
			if((itr + 1) != HeadingEntry.end()) rHeadings += _T(";");
		}
		m_wndLineDataReport.SetHeadings(rHeadings);
		m_wndLineDataReport.SetGridLines();
	}else	AfxMessageBox( _T("Can't find T_LINE_DATA.SQL file") );

	m_opt1.InitContents( this );

	rApp = _T("Mapping");
	if(GetPrivateProfileString(rApp, _T("Count") ,NULL, szBuf, MAX_PATH, rIniFilePath))
	{
		nCount = atoi(szBuf);

		for(int i =0; i < nCount ; ++i)
		{
			MappingDataField Field;

			CString rKey;
			rKey.Format("NAME%d" , i);
			if(GetPrivateProfileString(rApp , rKey , NULL , szBuf , MAX_PATH , rIniFilePath))
			{
				Field.FieldName = szBuf;
			}

			rKey.Format("EXCEL%d" , i);
			if(GetPrivateProfileString(rApp , rKey , NULL , szBuf , MAX_PATH , rIniFilePath))
			{
				Field.ExcelCol = szBuf;
			}

			m_MappingDataFieldEntry.push_back(Field);
			__TEST_FILE__ << Field.FieldName.operator LPCTSTR() << "," << Field.ExcelCol.operator LPCTSTR() << std::endl;
		}
	}
	__TEST_FILE__.close();

	///< 오라클 매핑 상태를 읽는다.
	rApp = _T("Oracle Mapping");
	nCount = 0;
	LineWithOracle oraMap;
	m_LineWithOracleList.clear();
	if(GetPrivateProfileString(rApp, _T("Count") ,NULL, szBuf, MAX_PATH, rIniFilePath))
	{
		nCount = atoi(szBuf);
		vector<string> oResult;

		for(int i =0; i < nCount ; ++i)
		{
			CString rKey;
			rKey.Format(_T("Name%d") , i);
			if(GetPrivateProfileString(rApp , rKey , NULL , szBuf , MAX_PATH , rIniFilePath))
			{
				CTokenizer<CIsFromString>::Tokenize(oResult , szBuf , CIsFromString(":"));
				if(2 == oResult.size())
				{
					oraMap.FieldDesc = oResult[0].c_str();
					if(2 == oResult.size())
						oraMap.OracleName = oResult[1].c_str();
					else	oraMap.OracleName.Empty();
					m_LineWithOracleList.push_back(oraMap);
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	rApp = "Oracle Setting";
	if(GetPrivateProfileString(rApp , "DDL FILE PATH" , NULL ,szBuf , MAX_PATH , rIniFilePath))
	{
		///string rOraDatabaseDefFilePath = pDoc->GetCurProject()->path() + string("\\Setting\\design.ddl");
		string rOraDatabaseDefFilePath(szBuf);
		bool res = m_oraDatabaseDefFile.Read(szBuf);
		if(false == res)
		{
			AfxMessageBox(string("Fail to read " + rOraDatabaseDefFilePath).c_str());
		}
	}
	else
	{
		AfxMessageBox(_T("Design DDL File Path does not setted."));
	}
}

/**	\brief	The CPDSLineDataForm::OnNotify function

	\param	wParam	a parameter of type WPARAM
	\param	lParam	a parameter of type LPARAM
	\param	pResult	a parameter of type LRESULT*

	\return	BOOL	
*/
BOOL CPDSLineDataForm::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	NMHDR* pNMHDR = (NMHDR*)(lParam);
	if((SELECT_ALL == wParam) && (m_opt1.GetDlgCtrlID() == pNMHDR->idFrom))
	{
		CString rQueryCondition = m_opt1.GetInAreaQueryCondition();
		
		vector< string > issueDates;
		LoadLineDataRecordSetFromDatabase( rQueryCondition , issueDates );
	}
	else if((SELECT == wParam) && (m_opt1.GetDlgCtrlID() == pNMHDR->idFrom))
	{
		CString rQueryCondition = m_opt1.GetQueryCondition();
		
		vector< string > issueDates;
		m_opt1.GetSelectedIssueDate( issueDates );
		LoadLineDataRecordSetFromDatabase(rQueryCondition , issueDates );
	}

	return CFormView::OnNotify(wParam, lParam, pResult);
}

void CPDSLineDataForm::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
	static const int gap = 3;

	if( m_TabCtrl.GetSafeHwnd() )
	{
		m_TabCtrl.SetWindowPos(NULL , 0 , 0 , CLineListSelectOption1::WIDTH , cy , SWP_NOZORDER);
	}
	
	CRect rectGet;
	CWnd* pWnd = GetDlgItem( IDC_BUTTON_GET );
	if( pWnd )
	{
		int nWidth = 0;
		CRect rect;
		if( m_selectAllButton.GetSafeHwnd() )
		{
			m_selectAllButton.GetWindowRect( &rect );
			nWidth += rect.Width();
			m_unselectAllButton.GetWindowRect( &rect );
			nWidth += rect.Width();
			pWnd->GetWindowRect( &rectGet );

			pWnd->SetWindowPos( NULL , cx - nWidth - rectGet.Width() - 10 , cy - rectGet.Height() , rectGet.Width() , rectGet.Height() , SWP_NOZORDER );
		}
	}

	if( m_wndLineDataReport.GetSafeHwnd() )
	{
		m_wndLineDataReport.SetWindowPos(NULL , CLineListSelectOption1::WIDTH , 25 , cx - CLineListSelectOption1::WIDTH , (int)(cy - 25 - rectGet.Height() - gap - gap ) , SWP_NOZORDER);
		CRect rect;
		m_wndLineDataReport.GetWindowRect(&rect);
		ScreenToClient(&rect);
	}

	CRect rect;
	if( m_wndLineDataReport.GetSafeHwnd() )
	{
		m_wndLineDataReport.GetWindowRect(&rect);
		ScreenToClient(&rect);
		
				if(::IsWindow(m_unselectAllButton.m_hWnd))
		{
			CRect rectThis;
			m_unselectAllButton.GetWindowRect(&rectThis);
			ScreenToClient(&rectThis);
			m_unselectAllButton.SetWindowPos(NULL , rect.right - rectThis.Width() , rect.bottom + 2, 0 , 0 , SWP_NOSIZE);
		}
		
		if(::IsWindow(m_selectAllButton.m_hWnd))
		{
			CRect rectThis;
			m_selectAllButton.GetWindowRect(&rectThis);
			ScreenToClient(&rectThis);
			m_unselectAllButton.GetWindowRect(&rect);
			ScreenToClient(&rect);
			m_selectAllButton.SetWindowPos(NULL , rect.left - rectThis.Width() - 3, rect.top , 0 , 0 , SWP_NOSIZE);
		}

		CWnd* pWnd = GetDlgItem(IDC_STATIC_SELECTED_LINE);
		if(pWnd)
		{
			CRect rectThis;
			pWnd->GetWindowRect(&rectThis);
			ScreenToClient(&rectThis);
			
			pWnd->SetWindowPos(NULL, CLineListSelectOption1::WIDTH , rect.top + 3 ,0, 0, SWP_NOSIZE);
		}
		pWnd = GetDlgItem(IDC_STATIC_SELECTED_COUNT);
		if(pWnd)
		{
			CRect rectThis;
			pWnd->GetWindowRect(&rectThis);
			ScreenToClient(&rectThis);
			
			CWnd* pWnd1 = GetDlgItem(IDC_STATIC_SELECTED_LINE);
			pWnd1->GetWindowRect(&rect);
			ScreenToClient(&rect);
			
			pWnd->SetWindowPos(NULL, rect.right + 3 , rect.top ,0, 0, SWP_NOSIZE);
		}
		
		pWnd = GetDlgItem(IDC_BUTTON_UP);
		if(pWnd)
		{
			CRect rectThis;
			pWnd->GetWindowRect(&rectThis);
			ScreenToClient(&rectThis);
			
			pWnd->SetWindowPos(NULL, cx / 2 , rect.top ,0 , 0 , SWP_NOSIZE);
		}
		
		pWnd = GetDlgItem(IDC_BUTTON_DOWN);
		if(pWnd)
		{
			CRect rectThis;
			pWnd->GetWindowRect(&rectThis);
			ScreenToClient(&rectThis);
			
			CWnd* pWnd1 = GetDlgItem(IDC_BUTTON_UP);
			pWnd1->GetWindowRect(&rect);
			ScreenToClient(&rect);
			
			pWnd->SetWindowPos(NULL, (cx / 2) + rect.Width() , rect.top, 0, 0, SWP_NOSIZE );
		}
	}
}

/**	\brief	The CPDSLineDataForm::LoadLineDataRecordSetFromDatabase function

	\param	rQueryCondition	a parameter of type const CString&
	\param	issueDates	a parameter of type const vector< string >&

	\return	void	
*/
void CPDSLineDataForm::LoadLineDataRecordSetFromDatabase(const CString& rQueryCondition , const vector< string >& issueDates )
{
	//CUserAppView* pView = (CUserAppView*)(GetParent());
	CUserAppDocData*  pDocData  = CUserAppDocData::GetInstance();
	
	if(NULL != pDocData)
	{
		if(NULL != m_pLineDataRecordSet) CLineDataRecordSet::DeleteInstance(m_pLineDataRecordSet);

		BeginWaitCursor();
		
		CString rSelRevNo = m_opt1.GetSelectedRevNo();
		if(rSelRevNo.IsEmpty() || ("All" == rSelRevNo) )
		{
			m_pLineDataRecordSet = pDocData->m_pSelUser->GetLineDataRecordSetFromDatabase(rQueryCondition , NULL , issueDates );
		}
		else
		{
			m_pLineDataRecordSet = pDocData->m_pSelUser->GetLineDataRecordSetFromDatabase(rQueryCondition , rSelRevNo.operator LPCTSTR() , issueDates );	
		}
		
		if(NULL != m_pLineDataRecordSet)
		{
			const size_t nRecordCount = m_pLineDataRecordSet->GetLineDataRecordCount();
			if(0 == nRecordCount)
			{
				CString rMsg;
				rMsg.LoadString(IDS_NO_RECORD);
				AfxMessageBox(rMsg);
				EndWaitCursor();
				return;
			}

			DisplayLineDataRecordSet();
		}

		EndWaitCursor();
	}
}

/**	\brief	line data record set을 report 컨트롤에 표시한다.
	\author	백흠경

	\return	void	
*/
void CPDSLineDataForm::DisplayLineDataRecordSet()
{
	if(m_pLineDataRecordSet)
	{
		CUserAppDocData* pDocData = CUserAppDocData::GetInstance();

		m_wndLineDataReport.SetRedraw(FALSE);
		m_wndLineDataReport.DeleteAllItems();
		
		LVITEM lvitem;
		lvitem.mask = LVIF_TEXT | LVIF_IMAGE;
		lvitem.iImage = 0;
		lvitem.cchTextMax = 32;
		
		const size_t nRecordCount = m_pLineDataRecordSet->GetLineDataRecordCount();
		int nItem = 0;
		char szBuf[MAX_PATH] = {'\0' ,};
		int nCount=0;
		for(size_t i = 0;i < nRecordCount;i++)
		{			
			CLineDataRecord* pLineDataRecord = m_pLineDataRecordSet->GetLineDataRecordAt(i);
			if(pLineDataRecord)
			{
				CString rId;
				rId.Format("%d" , pLineDataRecord->index());

				if(!pLineDataRecord->IsDeleted())
				{
					CString rIsoFileName = pDocData->m_pSelUser->GetInputIsoFileName(pLineDataRecord);
					rIsoFileName += CIsoEditSettingPage::m_rFileNameExt;
					CString rIsoFilePath =  m_rDWGFilePath;
					if("\\" != rIsoFilePath.Right(1)) rIsoFilePath += "\\";
					rIsoFilePath += rIsoFileName;
					lvitem.iImage = (!FileExist(rIsoFilePath)) ? 0 : 1;
					
					lvitem.iItem = nItem++;
					lvitem.iSubItem = 0;
					lvitem.pszText = (char*)(rId.operator const char*());
					int it = m_wndLineDataReport.InsertItem(&lvitem);
					DWORD dw = m_wndLineDataReport.GetItemData(it);
					UpdateLineDataRecord(it , pLineDataRecord);
					++nCount;
				}
			}
		}
		m_wndLineDataReport.SetRedraw(TRUE);
		ShowRecordCount(nCount);
	}
}

/**	\brief	line data record의 필드 내용을 list control에 채운다.

	\param	nItem	a parameter of type const int &
	\param	pLineDataRecord	a parameter of type CLineDataRecord *

	\return	void	
*/
void CPDSLineDataForm::UpdateLineDataRecord(const int &nItem, CLineDataRecord *pLineDataRecord)
{
	assert((nItem > -1) && "item range error");
	assert(pLineDataRecord && "pLineDataRecord is NULL");

	if((nItem > -1) && pLineDataRecord)
	{
		CUserAppDocData* pDocData = CUserAppDocData::GetInstance();
		char szBuf[MAX_PATH]={'\0',};

		m_wndLineDataReport.SetItemData(nItem , (DWORD)(pLineDataRecord));
		
		const int nColumnCount = m_wndLineDataReport.GetColumnCount();
		for(int k = 0;k < nColumnCount;k++)
		{
			LVCOLUMN lvcolumn;
			lvcolumn.mask = LVCF_TEXT;
			lvcolumn.cchTextMax = MAX_PATH;
			lvcolumn.pszText = szBuf;
			m_wndLineDataReport.GetColumn(k , &lvcolumn);
			
			const int nFieldCount = pLineDataRecord->GetFieldCount();
			for(int j = 0;j < nFieldCount;j++)
			{
				CDataField* pLineDataField = pLineDataRecord->GetFieldAt(j);
				if(CString(lvcolumn.pszText) == pLineDataField->desc())
				{
					CString rValue = pLineDataField->value();
					rValue.Replace("\n" , " ");
					
					m_wndLineDataReport.SetItemText(nItem , k , rValue);
					break;
				}
			}
		}
	}
}

/**
	\brief	LIST CONTROL의 모든 ITEM들을 선택한다.
	\author	백흠경
**/
void CPDSLineDataForm::OnButtonSelectAll() 
{
	// TODO: Add your control notification handler code here
	if(m_wndLineDataReport.GetSafeHwnd())
	{
		m_wndLineDataReport.SelectAllItems();
		m_wndLineDataReport.SetFocus();
	}
}

/**
	\brief	
	\author	백흠경
**/
void CPDSLineDataForm::OnButtonUnSelectAll() 
{
	// TODO: Add your control notification handler code here
	m_wndLineDataReport.UnSelectAllItems();
	m_wndLineDataReport.SetFocus();
}

/**	
	@brief	PDS Oracle 내용을 ISO MAN database에 업데이트한다.
	
	@author	HumKyung

	@param	pAdoDB	a parameter of type CADODB *
	@param	pLineDataRecord	a parameter of type CLineDataRecord *

	@return	bool	
*/
bool CPDSLineDataForm::UpdateIsoManDatabaseWithOra(CADODB *pAdoDB, vector<CString>& tableNames , CLineDataRecord *pLineDataRecord)
{
	assert(pAdoDB && "pAdoDB is NULL");
	assert(pLineDataRecord && "pLineDataRecord is NULL");

	if( pAdoDB && pLineDataRecord )
	{
		CString rConditionString = GetConditionString(pLineDataRecord);
		for(vector< CString >::iterator itr = tableNames.begin();itr != tableNames.end();++itr)
		{
			CString strQuery = CString("SELECT * FROM DD_") + m_rPDSProjectName + "." + (*itr) + " WHERE " + rConditionString;
			try
			{
				__LOG_STRING_ENTRY_.push_back(CString("QUERY : " + strQuery).operator LPCSTR());
				if(TRUE == pAdoDB->OpenQuery(strQuery)) ///< 조건에 맞는 레코드를 구한다.
				{
					LONG lRecordCount = 0L;
					if(TRUE == pAdoDB->GetRecordCount(&lRecordCount))
					{	
						if(lRecordCount > 0) ///< 조건에 맞는 레코드를 구했다.
						{
							CString rValue;		///< Database 값.
							CString rTempValue;	///< Database Temp값
							for( vector<LineWithOracle>::iterator jtr = m_LineWithOracleList.begin(); jtr != m_LineWithOracleList.end(); ++jtr)
							{
								CString rLineFieldDesc   = (jtr)->FieldDesc;
								CString rOracleFieldName = (jtr)->OracleName;
								if(rOracleFieldName.IsEmpty()) continue;
								/// 엑셀에서 넘어온 데이타는 PDS의 값으로 대체하지 않는다.
								///if(IsConvertedFromExcel(rLineFieldDesc)) continue;
								
								pAdoDB->GetFieldValue(0, rOracleFieldName , &rValue);
								TrimRightZeroString(rValue);
								rTempValue = rValue;
								COraDatabaseDefFile::CFieldDef defField;
								if(true == m_oraDatabaseDefFile.GetOraDefField(defField , rOracleFieldName.operator const char*()))
								{
									CString rOraKey = rOracleFieldName;
									rOraKey.MakeUpper();
									
									int nNoteNum = -1;
									if(string::npos != defField.m_rNote.find("standard note")) ///< find standard note
									{
										string rNoteNum = defField.m_rNote;
										int at = rNoteNum.find("standard note");
										int len = rNoteNum.length();
										int temp = len - ( at + strlen("standard note"));
										rNoteNum = rNoteNum.substr((at + strlen("standard note")) , temp);
										sscanf(rNoteNum.c_str(), "%d" ,&nNoteNum);

										if(IsNumberStr(rTempValue.operator const char*()))
										{
											rTempValue = m_pdsOracle.FindStrMatchCodeNumber(nNoteNum , atoi(rTempValue)).c_str();
											if(!rTempValue.IsEmpty())
												rValue = rTempValue;
											else	rValue = "NULL";
										}
									}
								}

								/// 매치하는 Line data record에 값을 입력한다.
								CDataField* pDataField = pLineDataRecord->Find(rLineFieldDesc);
								if(pDataField)
								{
									if( _T("INSULATION THICKNESS") == rLineFieldDesc)
									{
										/// INSUL THK. 값이 1보다 작을때는 공백으로 처리한다.
										rValue = (atof(rValue.operator LPCTSTR()) < 1.f) ? " " : rValue;
									}

									pLineDataRecord->Set(pDataField->name() , rValue);
								}
								else
								{
									__LOG_STRING_ENTRY_.push_back(CString(rLineFieldDesc + _T(" 이 없습니다.")).operator LPCSTR());
								}
							}

							return true;
						}
						else// if(lRecordCount > 1)
						{
							__LOG_STRING_ENTRY_.push_back( _T("NO MATCH RECORD") );
						}
					}
					else
					{
						__LOG_STRING_ENTRY_.push_back(CString("Query 에러 : " + strQuery).operator LPCSTR());
					}
				}
				else
				{
					__LOG_STRING_ENTRY_.push_back(CStringHelper::GetInstance().GetHelpString( IDS_FAIL_TO_OPEN_QUERY ));
				}
			}
			catch(const char* es)
			{
				AfxMessageBox(es);
			}
		}
	}
}

/**	
	@brief	line data record에 대응하는 쿼리 문자열을 만든다.

	@author	HumKyung.BAEK

	@param	pLineDataRecord	a parameter of type const CLineDataRecord *

	@return	CString	
*/
CString CPDSLineDataForm::GetConditionString(const CLineDataRecord *pLineDataRecord)
{
	assert(pLineDataRecord && "pLineDataRecord is NULL");
	CString rConditionString;
	CString rOracleKey, rFieldValue,rPartition;
	string rIDMSKeyName,rPart;
	vector<string> oResult;
	
	if(pLineDataRecord)
	{
		CUserAppDocData* pDocData = CUserAppDocData::GetInstance();
		rOracleKey = pDocData->m_OracleKey;
		for(vector<string>::iterator itr = pDocData->m_IsoManKeySetting.begin();itr != pDocData->m_IsoManKeySetting.end();++itr)
		{
			if((*itr) == _T("-") || (*itr) == _T("_") || (*itr) == _T("^") || (*itr) == _T("*"))
			{
				rPart = (*itr);
				rPartition = rPart.c_str();
				continue;
			}
			const size_t nFieldCount = pLineDataRecord->GetFieldCount();
			for(int i = 0;i < nFieldCount;++i)
			{
				CDataField* pDataField = pLineDataRecord->GetFieldAt(i);
				
				rIDMSKeyName = (*itr);
				IsString::TrimWhiteSpace(rIDMSKeyName);
				if(rIDMSKeyName == pDataField->desc())
				{
					if(!rFieldValue.IsEmpty()) rFieldValue += rPartition;
					rFieldValue += pDataField->value();
				}
			}
		}
		
		COraDatabaseDefFile::CFieldDef defField;
		if(true == m_oraDatabaseDefFile.GetOraDefField(defField , rOracleKey.operator const char*()))
		{
			CString rOraKey = rOracleKey;
			rOraKey.MakeUpper();
			
			int nNoteNum = -1;
			if(string::npos != defField.m_rFieldType.find("character")) ///< string type field
			{
				rFieldValue = "'" + rFieldValue + "'";
			}
		}
		else
		{
			AfxMessageBox( CStringHelper::GetInstance().GetHelpString( IDS_DESIGN_DDL_FIELD_ERROR ));
		}
		
		rConditionString = rOracleKey + "="  + rFieldValue;
	}
	
	return rConditionString;
}

void CPDSLineDataForm::SetViewLogFileForm(CGetPds *p)
{
	assert(p && "Null Parameter");

	if(p) m_pGetPdsForm = p;
}

/**
	\brief	
	\author	백흠경
**/
void CPDSLineDataForm::OnButtonDown() 
{
	// TODO: Add your control notification handler code here
	QualifyLineData();
}

/**
	\brief
	\author	백흠경
**/
void CPDSLineDataForm::OnButtonUp() 
{
	// TODO: Add your control notification handler code here
	if(m_pGetPdsForm && (0 != m_pGetPdsForm->m_GetPdsReport.GetItemCount()))
	{	m_pGetPdsForm->DeleteQualifiedLineList();
		
		int nCount = m_pGetPdsForm->m_GetPdsReport.GetItemCount();
		m_pGetPdsForm->ShowSelectedCount(nCount);
	}	
}

void CPDSLineDataForm::ShowRecordCount(int nRecordCount)
{
	m_nRecordCount = nRecordCount;
	CWnd* pWnd = GetDlgItem(IDC_STATIC_SELECTED_COUNT);
	if(pWnd)
	{
		CString rCount;
		rCount.Format("%d",m_nRecordCount);
		pWnd->SetWindowText((LPCTSTR)rCount);
	}
}

/**
	\brief	
	\author	백흠경
**/
void CPDSLineDataForm::QualifyLineData()
{
	if(m_pGetPdsForm)
	{
		vector<LineDataRecordMap> LineDataRecordEntry;
		GetSelectedLineDataRecord(LineDataRecordEntry);
		
		m_pGetPdsForm->AddLineDataRecordEntry(&LineDataRecordEntry);
	}
}

void CPDSLineDataForm::GetSelectedLineDataRecord(vector<LineDataRecordMap>& LineDataRecordEntry)
{
	LineDataRecordMap lineDataRecordMap;
	POSITION pos = m_wndLineDataReport.GetFirstSelectedItemPosition();
	while(pos)
	{
		lineDataRecordMap.nItem = m_wndLineDataReport.GetNextSelectedItem(pos);
		lineDataRecordMap.pLineDataRecord = (CLineDataRecord*)(m_wndLineDataReport.GetItemData(lineDataRecordMap.nItem));
		
		if(lineDataRecordMap.pLineDataRecord) LineDataRecordEntry.push_back(lineDataRecordMap);
	}
}

/**	\brief	문자열이 숫자로 이루어져 있는지 확인한다.
**/
bool CPDSLineDataForm::IsNumberStr(const string& str) const
{
	const int count = str.length();
	///char szBuf[MAX_PATH] ={'\0',};
	for(int i=0; i < count; ++i)
	{
		///szBuf[i] = str[i];
		if('.' == str[i]) continue;
		if((str[i] < '0') || (str[i] > '9')) return false;
	}
	return true;
}

struct MatchFieldDesc : public binary_function<CPDSLineDataForm::LineDataField , CString , bool>
{
	bool operator()(const CPDSLineDataForm::LineDataField& Field , const CString& rFieldDesc) const
	{
		return (Field.FieldDesc == rFieldDesc);
	}
};

struct MatchFieldName : public binary_function<CPDSLineDataForm::MappingDataField , CString , bool>
{
	bool operator()(const CPDSLineDataForm::MappingDataField& Field , const CString& rFieldName) const
	{
		return (Field.FieldName == rFieldName);
	}
};

/**
	\brief	
	\author	백흠경
**/
bool CPDSLineDataForm::IsConvertedFromExcel(const CString &rFieldDesc)
{
	__LOG_STRING_ENTRY_.push_back(string("IsConvertedFromExcel(" + string(rFieldDesc.operator LPCTSTR()) + ")"));

	vector<LineDataField>::iterator where1 = find_if(m_LineDataFieldEntry.begin() , m_LineDataFieldEntry.end() , bind2nd(MatchFieldDesc() , rFieldDesc));
	if(m_LineDataFieldEntry.end() != where1)
	{
		vector<MappingDataField>::iterator where2 = find_if(m_MappingDataFieldEntry.begin() , m_MappingDataFieldEntry.end() , bind2nd(MatchFieldName() , where1->FieldName));
		if(m_MappingDataFieldEntry.end() != where2)
		{
			return (!where2->ExcelCol.IsEmpty());
		}
	}

	return false;
}

UINT CPDSLineDataForm::StatusThreadEntry(LPVOID pVoid)
{
	return ((CPDSLineDataForm*)pVoid)->StatusThread();
}

/**
	@brief	PDS에서 설정한 ORACLE 값을 읽어와 ISO MAN DATABASE를 UPDATE한다.

	@author	HumKyung

	@date	????.??.??

	@param

	@return
**/
UINT CPDSLineDataForm::StatusThread()
{
	CWorkStatusDlg* pWorkStatusDlg = CWorkStatusDlg::GetInstance();
	try
	{
		//! line data record를 읽는다.
		vector< CLineDataRecord* > LineDataRecordSet;
		const int nCount = m_pGetPdsForm->m_GetPdsReport.GetItemCount();
		for(int i = 0 ; i < nCount; ++i)
		{
			CLineDataRecord* pRecord = (CLineDataRecord*)(m_pGetPdsForm->m_GetPdsReport.GetItemData(i));
			LineDataRecordSet.push_back(pRecord);
		}
		//!
		
		CUserAppDocData* pDocData = CUserAppDocData::GetInstance();
		CString rStdFilePath = pDocData->m_rStdNoteFilePath;
				
		///m_pdsOracle.Set( m_rDataSource , m_rUserID , m_rPW , m_rPDSProjectName );
		__LOG_STRING_ENTRY_.clear();
		m_pdsOracle.Set( m_rServiceName , m_rUserId , m_rPassword , m_rPDSProjectName );
		m_pdsOracle.LoadNoteTableFromDataFile(rStdFilePath.operator const char*());
		
		///m_pdsOracle.CreateOracleView();
		
		CAdoRemoteDB oraDb;
		CString	strDBPath = _T("provider=MSDAORA;User ID=") + m_rUserId + _T(";Password=") + m_rPassword + _T(";Data Source=") + m_rServiceName;
		if(TRUE == oraDb.DBConnect(strDBPath))
		{
			vector<CString> TableNames;
			CString rValue;
			/// Oracel용
			if(m_pdsOracle.Get_PDTABLE_12_TableNames(TableNames , oraDb))
			{
				if(!LineDataRecordSet.empty())
				{
					CString rTitle( _T("Update IsoMan Database...") );
					const size_t nCount = LineDataRecordSet.size();

					int i = 0;
					for(vector< CLineDataRecord* >::iterator itr = LineDataRecordSet.begin(); itr != LineDataRecordSet.end();++itr,++i)
					{
						UpdateIsoManDatabaseWithOra(&oraDb , TableNames , (*itr));

						int process = (int)(((double)i / (double)nCount) * 100.f);
						if(pWorkStatusDlg) pWorkStatusDlg->UpdateWorkStatus(rTitle , process);
					}
				}
				else
				{
					__LOG_STRING_ENTRY_.push_back("Update할 라인 레코드가 없습니다.");
					///AfxMessageBox("Update할 라인 레코드가 없습니다.");
				}
			}
			else
			{
				__LOG_STRING_ENTRY_.push_back("PDTABLE_12번 데이블을 구할 수 없습니다.");
				///AfxMessageBox("PDTABLE_12번 데이블을 구할 수 없습니다.");
			}
			
			/// update IDMS database
			try
			{
				pDocData->m_pSelUser->SaveToDatabase(&LineDataRecordSet);	///< 수정된 라인 데이터 레코드들을 저장한다.
				
				///CString rMsg;
				///rMsg.LoadString(IDS_UPDATE_DATABASE_OK);
				///AfxMessageBox(rMsg);
			}
			catch(...)
			{
				///CString rMsg;
				///rMsg.LoadString(IDS_UPDATE_DATABASE_ERROR);
				///AfxMessageBox(rMsg);
			}
		}
		else
		{
			CString rMsg;
			rMsg.LoadString(IDS_ORACLE_CONNECTION_ERROR);
			AfxMessageBox(rMsg);
		}
		oraDb.DBDisConnect();
		
		if(pWorkStatusDlg) pWorkStatusDlg->SendMessage(WM_COMMAND , IDOK );
		
		return IDOK;
	}
	catch(...)
	{
		AfxMessageBox("Unknown error");

		InterlockedExchange((LONG*)(&(pWorkStatusDlg->m_bThreadRunning)) , FALSE);
		
		//////////////////////////////////////////////////////////////////////////
		/// close work status dialog
		if(pWorkStatusDlg) pWorkStatusDlg->SendMessage(WM_COMMAND , IDOK );
	}
	
	return IDCANCEL;
}