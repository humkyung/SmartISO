// InsertToPDSAttribute.cpp : implementation file
//

#include "stdafx.h"
#include "UserApp.h"

#include "InsertToPDSAttribute.h"
#include "MainFrm.h"
#include "UserAppDoc.h"
#include "UserAppView.h"
#include "EmptyOutputWnd.h"
#include "isstring.h"
#include "WorkStatusDlg.h"
#include "Splash/SplashScreenFx.h"
#include "WorkspaceWnd.h"
#include "SmartISOLog.h"

#include <ado/SQLFile.h>

#include <fstream>
#include "tokenizer.h"
#include <vector>

//using namespace SmartISOLayer;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static vector<string> __LOG_STRING_ENTRY_;
/////////////////////////////////////////////////////////////////////////////
// CInsertToPDSAttribute
IMPLEMENT_DYNCREATE(CInsertToPDSAttribute, CFormView)

CInsertToPDSAttribute::CInsertToPDSAttribute()
: CFormView(CInsertToPDSAttribute::IDD),m_pLineDataRecordSet(NULL),m_pEmptyOutputWnd(NULL)
{
	//{{AFX_DATA_INIT(CInsertToPDSAttribute)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CInsertToPDSAttribute::~CInsertToPDSAttribute()
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

void CInsertToPDSAttribute::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInsertToPDSAttribute)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CInsertToPDSAttribute, CFormView)
	//{{AFX_MSG_MAP(CInsertToPDSAttribute)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_ORACLE, &CInsertToPDSAttribute::OnBnClickedButtonUpdateOracle)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInsertToPDSAttribute diagnostics

#ifdef _DEBUG
void CInsertToPDSAttribute::AssertValid() const
{
	CFormView::AssertValid();
}

void CInsertToPDSAttribute::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CInsertToPDSAttribute message handlers

void CInsertToPDSAttribute::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	if(m_wndLineDataReport.GetSafeHwnd())
	{
		m_wndLineDataReport.SetWindowPos(NULL , 1 , 1 , cx - 2 , (int)(cy - 1) , SWP_NOZORDER);
		CRect rect;
		m_wndLineDataReport.GetWindowRect(&rect);
		ScreenToClient(&rect);
	}
}

void CInsertToPDSAttribute::OnInitialUpdate() 
{
	static bool __init = false;
	CFormView::OnInitialUpdate();
	
	if(false == __init)
	{
		m_ImageList.Create(IDB_CHECKBOXES,16,16,RGB(255,0,255));

		CUserAppView* pView = (CUserAppView*)(GetParent());
		CUserAppDoc*  pDoc  = pView->GetDocument();
		CAppDocData* pDocData = CAppDocData::GetInstance();
				
		if(m_wndLineDataReport.SubclassWindow(GetDlgItem(IDC_LIST_INSERTTOPDS)->GetSafeHwnd()))
		{
			LONG style = GetWindowLong(m_wndLineDataReport.m_hWnd , GWL_STYLE);
			style |= WS_EX_CLIENTEDGE;
			SetWindowLong(m_wndLineDataReport.m_hWnd , GWL_EXSTYLE , style );
			m_wndLineDataReport.SetImageList(&m_ImageList , LVSIL_SMALL);
			m_wndLineDataReport.SetHeaderImageList();
		}

		__init = true;
	}
}

/**	
	@brief	프로젝트이 활성화 되고 난뒤 초기화시키는 루틴.
	@author	humkyung
	@return	void	
*/
void CInsertToPDSAttribute::InitContents()
{
	CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();
	pWorkspaceBar->m_wndQuery5.InitContents( this );
	CAppDocData* pDocData = CAppDocData::GetInstance();

	CUserAppView* pView = (CUserAppView*)(GetParent());
	CUserAppDoc*  pDoc  = pView->GetDocument();
	
	CString rIniFilePath = pDocData->GetProject()->GetIniFilePath();
	CString rMStationPath,rDWGFilePath;
	TCHAR szBuf[1024] = {'\0' ,};
	if(GetPrivateProfileString(_T("Iso Edit Setting"),_T("MStation Path"),_T(""),szBuf,MAX_PATH,rIniFilePath))
	{
		rMStationPath = szBuf;
	}
	if(GetPrivateProfileString(_T("Iso Edit Setting"),_T("Source Iso File Folder"),_T(""),szBuf,MAX_PATH,rIniFilePath))
	{
		rDWGFilePath = szBuf;
		if(rDWGFilePath.Right(1)!="\\") rDWGFilePath+="\\";
	}

	CString rApp = _T("Line Data Field");
	int nCount = 0;
	CString rSqlFilePath = CAppDocData::GetExecPath() + _T("\\Setting\\T_ISO_LINE_DATA.XML");
	CSQLFile sqlFile(rSqlFilePath.operator LPCSTR());
	if(ERROR_SUCCESS == sqlFile.ParseXML(rSqlFilePath.operator LPCTSTR()))
	{
		::GetPrivateProfileString(_T("Line Data Field") , _T("Column Order") , _T("") , szBuf , 1024 , rIniFilePath);
		vector<STRING_T> oResult; 
		vector<CString> HeadingEntry;
		if(!CString(szBuf).IsEmpty())
		{
			CTokenizer<CIsFromString>::Tokenize(oResult , szBuf , CIsFromString(_T(":")));
			for(vector<STRING_T>::iterator itr = oResult.begin();itr != oResult.end();++itr)
			{
				CString rHeading;
				if(_T("REV_NO") == (*itr)) continue;
				rHeading += itr->c_str() + CString(_T(",100"));
				HeadingEntry.push_back(rHeading);
			}
		}
		
		if(oResult.end() == find(oResult.begin() , oResult.end() , _T("REV_NO")))
		{
			oResult.push_back(_T("REV_NO"));
		}
		if(oResult.end() == find(oResult.begin() , oResult.end() , _T("[]")))
		{
			oResult.push_back(_T("[]"));
		}
		
		nCount = sqlFile.GetColumnCount();
		for(int i = 0;i < nCount;++i)
		{
			STRING_T sFieldName = sqlFile.GetFieldNameAt(i);
			if(oResult.end() != find(oResult.begin() , oResult.end() , sFieldName)) continue;

			/// don't display cancellation field - 2011.02.24 added by HumKyung
			if(0 == stricmp(_T("CANCELLATION") , sFieldName.c_str()))
			{
				continue;
			}

			CString rHeading;
			rHeading += sqlFile.GetFieldNameAt(i).c_str() + CString(_T(",100"));
			HeadingEntry.push_back(rHeading);
		}
		
		CString rHeadings(_T("[],30;REV_NO,50;"));
		for(vector<CString>::iterator itr = HeadingEntry.begin();itr != HeadingEntry.end();itr++)
		{
			rHeadings += (*itr);
			if((itr + 1) != HeadingEntry.end()) rHeadings += _T(";");
		}
		m_wndLineDataReport.SetImageList(&m_ImageList , LVSIL_SMALL);
		m_wndLineDataReport.SetHeaderImageList(&m_ImageList);
		m_wndLineDataReport.SetHeadings(rHeadings);
		m_wndLineDataReport.SetGridLines();
		m_wndLineDataReport.SetExtendedStyle(LVS_EX_CHECKBOXES | m_wndLineDataReport.GetExtendedStyle());
		m_wndLineDataReport.EnableHeaderCheckBox(TRUE);

		/// set column order - 2012.07.31 added by humkyung
		if(!CString(szBuf).IsEmpty())
		{
			const int iColumnCount = m_wndLineDataReport.GetColumnCount();
			for(int i = 0;i < iColumnCount;++i)
			{
				LVCOLUMN lvc = {0};
				lvc.mask = LVCF_TEXT;
				TCHAR sColText[256];
				lvc.pszText = sColText;
				lvc.cchTextMax = sizeof(sColText)-1;
				m_wndLineDataReport.GetColumn(i , &lvc);

				if(oResult.end() == find(oResult.begin() , oResult.end() , STRING_T(lvc.pszText)))
				{
					m_wndLineDataReport.ShowColumn(i , false);
				}
			}
		}

		/*nCount = sqlFile.GetColumnCount();
		vector<CString> HeadingEntry;
		vector<string> oResult; 
		for(int i = 0;i < nCount;++i)
		{
			CString rKey;
			rKey.Format( _T("Display%d") , i);
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
		m_wndLineDataReport.SetGridLines();*/
	}else	AfxMessageBox( _T("Can't find T_ISO_LINE_DATA.XML file") , MB_OK|MB_ICONWARNING);
	
	rApp = _T("Oracle Mapping");
	nCount = 0;
	LineWithOracle oraMap;
	m_LineWithOracleList.clear();
	nCount = GetPrivateProfileInt(rApp, _T("Count") , 0 , rIniFilePath);
	
	vector<STRING_T> oResult;
	for(int i =0; i < nCount ; ++i)
	{
		CString rKey;
		rKey.Format( _T("NAME_FOR_INSERT%d") , i);
		if(GetPrivateProfileString(rApp , rKey , NULL , szBuf , MAX_PATH , rIniFilePath))
		{
			CTokenizer<CIsFromString>::Tokenize(oResult , szBuf , CIsFromString(":"));
			oraMap.FieldDesc = oResult[0].c_str();
			if(2 == oResult.size())
				oraMap.OracleName = oResult[1].c_str();
			else	oraMap.OracleName.Empty();
			m_LineWithOracleList.push_back(oraMap);
		}
	}
	
	CSplashScreenFx* pSplash = CSplashScreenFx::GetInstance();

	rApp = _T("Oracle Setting");
	//////////////////////////////////////////////////////////////////////////
	if(GetPrivateProfileString(rApp , _T("DDL FILE PATH") , NULL ,szBuf , MAX_PATH , rIniFilePath))
	{
		STRING_T rOraDatabaseDefFilePath(szBuf);
		bool res = m_oraDatabaseDefFile.Read(szBuf);
		if(false == res)
		{
			if(NULL != pSplash)
			{
				pSplash->SetTextColor(RGB(255,0,0));
				pSplash->SetText( STRING_T("Fail to read " + rOraDatabaseDefFilePath).c_str() );
				MessageBeep(MB_ICONHAND);
				pSplash->WaitUntilUserConfirm();
			}
			else
			{
				AfxMessageBox(STRING_T("Fail to read " + rOraDatabaseDefFilePath).c_str());
			}
		}
	}
	else
	{
		if(NULL != pSplash)
		{
			pSplash->SetTextColor(RGB(255,0,0));
			pSplash->SetText( _T("Design DDL File Path does not setted.") );
			MessageBeep(MB_ICONHAND);
			pSplash->WaitUntilUserConfirm();
		}
		else
		{
			AfxMessageBox(_T("Design DDL File Path does not setted."));
		}
	}
}

/**	
	@brief	The CInsertToPDSAttribute::OnNotify function
	@author	humkyung
	@param	wParam	a parameter of type WPARAM
	@param	lParam	a parameter of type LPARAM
	@param	pResult	a parameter of type LRESULT*
	@return	BOOL	
*/
BOOL CInsertToPDSAttribute::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	NMHDR* pNMHDR = (NMHDR*)(lParam);
	/*if((SELECT_ALL == wParam) && (m_opt1.GetDlgCtrlID() == pNMHDR->idFrom))
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
	}*/

	CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();

	if((SELECT_ALL == wParam) && (pWorkspaceBar->m_wndQuery5.GetDlgCtrlID() == pNMHDR->idFrom))
	{	
		CString rQuertCondition = pWorkspaceBar->m_wndQuery5.GetInAreaQueryCondition();
		vector< string > issueDates;
		LoadLineDataRecordSetFromDatabase( rQuertCondition , issueDates);
	}
	else if((SELECT == wParam) && (pWorkspaceBar->m_wndQuery5.GetDlgCtrlID() == pNMHDR->idFrom))
	{
		CString rQueryCondition = pWorkspaceBar->m_wndQuery5.GetQueryCondition();
		vector< string > issueDates;
		pWorkspaceBar->m_wndQuery4.GetSelectedIssueDate( issueDates );
		LoadLineDataRecordSetFromDatabase(rQueryCondition , issueDates );
	}

	return CFormView::OnNotify(wParam, lParam, pResult);
}

/**	
	@brief	The CInsertToPDSAttribute::LoadLineDataRecordSetFromDatabase function
	@author	humkyung
	@param	rQueryCondition	a parameter of type const CString &
	@param	issueDates	a parameter of type const vector< string >&
	@return	void	
*/
void CInsertToPDSAttribute::LoadLineDataRecordSetFromDatabase(const CString &rQueryCondition, const vector< string >& issueDates )
{
	CAppDocData*  pDocData  = CAppDocData::GetInstance();
	
	if(NULL != pDocData)
	{
		CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();

		if(NULL != m_pLineDataRecordSet) CLineDataRecordSet::DeleteInstance(m_pLineDataRecordSet);
		
		BeginWaitCursor();
		
		CString rSelRevNo = pWorkspaceBar->m_wndQuery5.GetSelectedRevNo();
		if(rSelRevNo.IsEmpty() || ("All" == rSelRevNo) )
		{
			m_pLineDataRecordSet = pDocData->GetProject()->GetLineDataRecordSetFromDatabase(rQueryCondition , NULL , issueDates);
		}
		else
		{
			m_pLineDataRecordSet = pDocData->GetProject()->GetLineDataRecordSetFromDatabase(rQueryCondition , rSelRevNo.operator LPCTSTR() , issueDates);	
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

void CInsertToPDSAttribute::DisplayLineDataRecordSet()
{
	if(m_pLineDataRecordSet)
	{
		m_wndLineDataReport.SetRedraw(FALSE);
		m_wndLineDataReport.DeleteAllItems();
		
		LVITEM lvitem;
		lvitem.mask = LVIF_TEXT | LVIF_IMAGE;
		lvitem.iImage = 0;
		lvitem.cchTextMax = 32;
		
		const size_t nRecordCount = m_pLineDataRecordSet->GetLineDataRecordCount();
		int nItem = 0;
		char szBuf[MAX_PATH] = {'\0' ,};
		int nCount = 0;
		for(size_t i = 0;i < nRecordCount;i++)
		{			
			CLineDataRecord* pLineDataRecord = m_pLineDataRecordSet->GetLineDataRecordAt(i);
			if(pLineDataRecord)
			{
				CString rId;
				rId.Format("%d" , pLineDataRecord->index());

				if(!pLineDataRecord->IsDeleted())
				{
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
		m_wndLineDataReport.GroupByColumn(m_wndLineDataReport.GetSelectedColumn());
		m_wndLineDataReport.SetRedraw(TRUE);
	}
}

/**	
	@brief	
	@author	humkyung
**/
void CInsertToPDSAttribute::UpdateLineDataRecord(const int &nItem, CLineDataRecord *pLineDataRecord)
{
	assert((nItem > -1) && "item range error");
	assert(pLineDataRecord && "pLineDataRecord is NULL");

	if((nItem > -1) && pLineDataRecord)
	{
		CAppDocData* pDocData = CAppDocData::GetInstance();
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

void CInsertToPDSAttribute::SetViewLogFileForm(CEmptyOutputWnd *p)
{
	assert(p && "Null Parameter");
	
	if(p) m_pEmptyOutputWnd = p;
}

/**	
	@brief	SmartISO LineData Record에 해당되는 Oracle Record 찾은 뒤 update 시킨다.
	@author	humkyung
	@param	pLineDataRecord	a parameter of type const CLineDataRecord *
	@return	bool	
**/
bool CInsertToPDSAttribute::UpdateOracleDatabase(ISmartISOLayerPtr& ptr,const CLineDataRecord *pLineDataRecord)
{
	assert(pLineDataRecord && "pLineDataRecord is NULL");

	if(pLineDataRecord)
	{
		CUserAppApp* pApp = static_cast<CUserAppApp*>(AfxGetApp());
		CAppDocData* pDocData = CAppDocData::GetInstance();
		
		CString strQuery , sConditionString[2];
		const int res = GetConditionString(sConditionString[0] , sConditionString[1] , pLineDataRecord);
		for( vector<LineWithOracle>::iterator jtr = m_LineWithOracleList.begin(); jtr != m_LineWithOracleList.end(); ++jtr)
		{
			CString rLineFieldDesc   = (jtr)->FieldDesc;
			CString rOracleFieldName = (jtr)->OracleName;
			if(rOracleFieldName.IsEmpty()) continue;

			CDataField* pDataField = pLineDataRecord->FindWithDesc(rLineFieldDesc);
			if(pDataField)
			{				
				CString rLineValue = pDataField->value();
				rLineValue.TrimLeft(); rLineValue.TrimRight();
				COraDatabaseDefFile::CFieldDef defField;
				if(true == m_oraDatabaseDefFile.GetOraDefField(defField , rOracleFieldName.operator LPCTSTR()))
				{
					CString rOraKey = rOracleFieldName;
					rOraKey.MakeUpper();

					int nNoteNum = -1;
					if(STRING_T::npos != defField.m_rNote.find( _T("standard note") ))	/// find standard note
					{
						STRING_T rNoteNum = defField.m_rNote;
						int at = rNoteNum.find( _T("standard note") );
						int len = rNoteNum.length();
						int temp = len - ( at + strlen(_T("standard note") ));
						rNoteNum = rNoteNum.substr((at + strlen(_T("standard note"))) , temp);
						sscanf(rNoteNum.c_str(), _T("%d") ,&nNoteNum);
						int nRetValue = m_pdsOracle.FindCodeNumberUsingStr(nNoteNum , rLineValue.operator LPCTSTR());
						rLineValue.Format(_T("%d") , nRetValue);
					}

					rLineValue = defField.ConvertValue(rLineValue.operator LPCTSTR()).c_str();
				}
				strQuery += rOracleFieldName + _T("=") + rLineValue + _T(",");
			}
		}
		if(!strQuery.IsEmpty())
		{
			if(_T(",") == strQuery.Right(1)) strQuery = strQuery.Left(strQuery.GetLength() - 1);
			strQuery += _T(" WHERE ");
			strQuery+= sConditionString[0];

			{
				CString sPrjName(pDocData->GetProject()->name());
				BSTR bstrPrjName(sPrjName.AllocSysString());
				BSTR bstrQuery(strQuery.AllocSysString());
				BSTR bstrConditionString(sConditionString[1].AllocSysString());
				long res=0L;
				ptr->UpdateOracleWith(bstrPrjName,bstrQuery,bstrConditionString,&res);
				if(0 == res)
				{
					/// execution is ok
				}
				else
				{
					WRITE_LOG((_T("ERROR : ") + strQuery).operator LPCTSTR());
				}
				::SysFreeString(bstrPrjName);
				::SysFreeString(bstrQuery);
				::SysFreeString(bstrConditionString);
			}
		}
		else
		{
			WRITE_LOG(_T("ERROR : There is no record to update"));
			return false;
		}
		return true;
	}
	
	return false;
}

/**	
	@brief	문자열이 하나 이상의 알파벳을 가지고 있으면 true , 그렇지 않으면 false
	@author	humkyung
	@param	str	a parameter of type const string&
	@return	bool	
*/
bool CInsertToPDSAttribute::HasAlphaCharacter(const STRING_T& str) const
{
	const int count = str.length();
	for(int i=0; i < count; ++i)
	{
		if(isalpha(str[i])) return true;
	}
	return false;
}


/**	
	@brief	The CInsertToPDSAttribute::GetConditionString function
	@author	humkyung
	@param	pLineDataRecord	a parameter of type const CLineDataRecord *
	@return	int	
*/
int CInsertToPDSAttribute::GetConditionString(CString& sInnerClause , CString& sOuterClause , const CLineDataRecord *pLineDataRecord)
{
	assert(pLineDataRecord && "pLineDataRecord is NULL");
	CString rOracleKey, rFieldValue ,rPartition;
	STRING_T sKeyName,sPart;
	vector<STRING_T> oResult;

	int res = ERROR_BAD_ENVIRONMENT;
	if(pLineDataRecord)
	{
		CAppDocData* pDocData = CAppDocData::GetInstance();
		rOracleKey = pDocData->m_oOracleSetting.m_OracleKey;
		for(vector<STRING_T>::iterator itr = pDocData->m_oOracleSetting.m_IsoManKeySetting.begin();itr != pDocData->m_oOracleSetting.m_IsoManKeySetting.end();++itr)
		{
			if((*itr) == _T("-") || (*itr) == _T("_") || (*itr) == _T("^") || (*itr) == _T("*"))
			{
				sPart = (*itr);
				rPartition = sPart.c_str();
				continue;
			}
			const size_t nFieldCount = pLineDataRecord->GetFieldCount();
			for(int i = 0;i < int(nFieldCount);++i)
			{
				CDataField* pDataField = pLineDataRecord->GetFieldAt(i);
				
				sKeyName = (*itr);
				IsString::TrimWhiteSpace(sKeyName);
				if(sKeyName == pDataField->desc())
				{
					if(!rFieldValue.IsEmpty()) rFieldValue += rPartition;
					rFieldValue += pDataField->value();
				}
			}
		}

		COraDatabaseDefFile::CFieldDef defField;
		if(true == m_oraDatabaseDefFile.GetOraDefField(defField , rOracleKey.operator LPCTSTR()))
		{
			CString rOraKey = rOracleKey;
			rOraKey.MakeUpper();
			
			int nNoteNum = -1;
			if(STRING_T::npos != defField.m_rFieldType.find( _T("character")) ) /// string type field
			{
				sInnerClause = rOracleKey + _T("=''") + rFieldValue + _T("''");
				sOuterClause = rOracleKey + _T("='") + rFieldValue + _T("'");
			}

			res = ERROR_SUCCESS;
		}
		else
		{
			AfxMessageBox( _T("def field error") );
		}
	}

	return res;
}

BOOL CInsertToPDSAttribute::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

/**
	@brief
	@author	humkyung
**/
UINT CInsertToPDSAttribute::StatusThreadEntry(LPVOID pVoid)
{
	CInsertToPDSAttribute* pUpdatePDSForm = static_cast<CInsertToPDSAttribute*>(pVoid);
	if(NULL != pUpdatePDSForm)
	{
		vector<LineDataRecordMap> oLineDataRecordEntry;
		pUpdatePDSForm->GetSelectedLineDataRecord(oLineDataRecordEntry);
		return pUpdatePDSForm->StatusThread(oLineDataRecordEntry);
	}

	return IDCANCEL;
}

/**
	@brief	PDS에서 설정한 ORACLE 값을 읽어와 SmartISO DATABASE를 UPDATE한다.
	@author	humkyung
**/
UINT CInsertToPDSAttribute::StatusThread(vector<LineDataRecordMap>& oLineDataRecordEntry)
{
	CWorkStatusDlg* pWorkStatusDlg = CWorkStatusDlg::GetInstance();
	try
	{
		CUserAppApp* pApp = static_cast<CUserAppApp*>(AfxGetApp());
		CAppDocData* pDocData = CAppDocData::GetInstance();
		const CString rStdFilePath = pDocData->m_rStdNoteFilePath;

		m_pdsOracle.Set( m_rDataSource , m_rUserID , m_rPW , m_rPDSProjectName );
		m_pdsOracle.LoadNoteTableFromDataFile(rStdFilePath.operator LPCTSTR());

		{
			CoInitialize(NULL);
			ISmartISOLayerPtr ptr(__uuidof(_SmartISOLayer));
			
			VARIANT_BOOL varRet;
			BSTR bstrHost = pDocData->m_oOracleSetting.m_sHost.AllocSysString();
			BSTR bstrService = pDocData->m_oOracleSetting.m_sService.AllocSysString();
			BSTR bstrUserName= pDocData->m_oOracleSetting.m_sDesUser.AllocSysString();
			BSTR bstrPassword= pDocData->m_oOracleSetting.m_sDesPassword.AllocSysString();
			CString sPrjName(pDocData->GetProject()->name());
			BSTR bstrPrjName = sPrjName.AllocSysString();
			if(VARIANT_TRUE == ptr->ConnectOracle(bstrHost,bstrService,bstrUserName,bstrPassword,&varRet))
			{
				long res=0L;
				ptr->CreateView(bstrPrjName,&res);
				if(0 == res)
				{
					CString rTitle( _T("Update PDS Oracle Database...") );
					int i = 0 , nCount = oLineDataRecordEntry.size();
					for(vector<LineDataRecordMap>::const_iterator itr = oLineDataRecordEntry.begin(); itr != oLineDataRecordEntry.end();++itr,++i)
					{
						CLineDataRecord* pLineDataRecord = itr->pLineDataRecord;
						this->UpdateOracleDatabase(ptr,pLineDataRecord);

						int process = (int)(((double)i / (double)nCount) * 100.f);
						if(pWorkStatusDlg) pWorkStatusDlg->UpdateWorkStatus(rTitle , process);
					}
				}
				else
				{
					WRITE_LOG(_T("Fail to create view"));
				}
				ptr->DisconnectOracle(&varRet);
				::SysFreeString(bstrHost);
				::SysFreeString(bstrService);
				::SysFreeString(bstrUserName);
				::SysFreeString(bstrPassword);
				::SysFreeString(bstrPrjName);
			}
			else
			{
				CString rMsg;
				rMsg.LoadString(IDS_ORACLE_CONNECTION_ERROR);
				WRITE_LOG(rMsg.operator LPCTSTR());
			}
			ptr->Release();
			CoUninitialize();
		}

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
	@brief	선택한 LINE LIST RECORD들을 구한다.
	@author	HumKyung
	@date	????.??.??
	@param	LineDataRecordEntry - container for LineDataRecordMap
	@return
**/
void CInsertToPDSAttribute::GetSelectedLineDataRecord(vector<LineDataRecordMap>& LineDataRecordEntry)
{
	LineDataRecordEntry.clear();
	LineDataRecordMap lineDataRecordMap;

	const int iItemCount = m_wndLineDataReport.GetItemCount();
	for(int i = 0;i < iItemCount;++i)
	{
		if(TRUE == m_wndLineDataReport.GetCheck(i))
		{
			lineDataRecordMap.nItem = i;
			lineDataRecordMap.pLineDataRecord = (CLineDataRecord*)(m_wndLineDataReport.GetItemData(lineDataRecordMap.nItem));
			if(lineDataRecordMap.pLineDataRecord) LineDataRecordEntry.push_back(lineDataRecordMap);		
		}
	}
}

/**
	@brief	update oracle with selected line data
	@author	humkyung
	@date	2015.10.29
*/
void CInsertToPDSAttribute::OnBnClickedButtonUpdateOracle()
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	CUserAppView* pView = (CUserAppView*)(GetParent());
	CUserAppDoc*  pDoc  = pView->GetDocument();
	
	CString rIniFilePath = pDocData->GetProject()->GetIniFilePath();
	CString rStdFilePath = pDocData->m_rStdNoteFilePath;

	CWorkStatusDlg dlg;
	dlg.m_pThread = AfxBeginThread(StatusThreadEntry, this, THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	if (NULL == dlg.m_pThread)
	{
		AfxMessageBox( _T("Can't create thread!!!") );
	}
	else
	{
		START_LOG();	/// start log - 2012.09.09 added by humkyung
		if(IDOK == dlg.DoModal())
		{			
		}

		CString rMsg;
		rMsg.LoadString((LOG_COUNT() > 0) ? IDS_UPDATE_DATABASE_ERROR : IDS_UPDATE_DATABASE_OK);
		AfxMessageBox(rMsg);
		
		CLOSE_LOG();	/// close log - 2012.01.04 added by humkyung
		OPEN_LOG();		/// open log - 2012.01.04 added by humkyung
	}
}
