// BackupDataForm.cpp : implementation file
//

#include "stdafx.h"
#include "UserApp.h"
#include "MainFrm.h"
#include "UserAppView.h"
#include "UserAppDocData.h"
#include "enablebuddybutton.h"
#include "BackupDataForm.h"
#include "BackupDataOutputForm.h"
#include "WorkspaceWnd.h"
#include "WorkStatusDlg.h"
#include <tokenizer.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBackupDataForm

IMPLEMENT_DYNCREATE(CBackupDataForm, CFormView)

CBackupDataForm::CBackupDataForm() : CFormView(CBackupDataForm::IDD),m_pOutputForm(NULL)
{
	//{{AFX_DATA_INIT(CBackupDataForm)
	//}}AFX_DATA_INIT
}

CBackupDataForm::~CBackupDataForm()
{
	try
	{
		for(list<CLineDataRecord*>::iterator itr = m_oLineDataRecordList.begin();itr != m_oLineDataRecordList.end();++itr)
		{
			SAFE_DELETE(*itr);
		}
		m_oLineDataRecordList.clear();
	}
	catch(...)
	{

	}
}

void CBackupDataForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBackupDataForm)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBackupDataForm, CFormView)
	//{{AFX_MSG_MAP(CBackupDataForm)
	ON_WM_SIZE()
	ON_NOTIFY(NM_RCLICK , WM_USER+1 , OnRclickTreeList)
	ON_COMMAND(ID_ISSUE_MANAGER_PROPERTIES , OnProperties)				/// 2015.12.05 added by HumKyung
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBackupDataForm diagnostics

#ifdef _DEBUG
void CBackupDataForm::AssertValid() const
{
	CFormView::AssertValid();
}

void CBackupDataForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBackupDataForm message handlers

BOOL CBackupDataForm::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CBackupDataForm::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	static bool __init = false;

	if(false == __init)
	{
		CRect rect;
		rect.SetRectEmpty();

		const DWORD uStyle = TVS_HASBUTTONS|TVS_HASLINES|TVS_FULLROWSELECT|TVS_NONEVENHEIGHT|TVS_CHECKBOXES;
		const DWORD uExStyle = TVS_EX_SUBSELECT|TVS_EX_FULLROWMARK|TVS_EX_ITEMLINES;
		if(m_wndTreeList.Create(uStyle|WS_CHILD|WS_VISIBLE|WS_BORDER , rect , this , WM_USER+1))
		{
			m_wndTreeList.SetExtendedStyle(uExStyle);

			m_imageList.Create(IDB_CHECKBOXES , 16 , 16 , RGB(255 , 0 , 255));
			m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ROOT));
			m_imageList.Add(AfxGetApp()->LoadIcon(IDI_SETTING));
			m_imageList.Add(AfxGetApp()->LoadIcon(IDI_ISO_DWG));
			m_imageList.Add(AfxGetApp()->LoadIcon(IDI_MSTN));
			m_wndTreeList.SetImageList(&m_imageList,TVSIL_NORMAL);
		}

		__init = true;
	}
}

void CBackupDataForm::OnSize(UINT nType, int cx, int cy) 
{
	static bool __init = true;
	CFormView::OnSize(nType, cx, cy);
	
	if(m_wndTreeList.GetSafeHwnd()) m_wndTreeList.SetWindowPos(NULL , 0 , 0 , cx , cy , SWP_NOZORDER);
}

void CBackupDataForm::InitContents()
{
	CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();
	pWorkspaceBar->m_wndQueryBackup.InitContents( this );

	CAppDocData* pDocData = CAppDocData::GetInstance();
	const CString name = pDocData->GetProject()->name();
	const CProject::DatabaseProp prop = pDocData->GetProject()->GetDatabaseProp();
	const CString sIniFilePath = pDocData->GetProject()->GetIniFilePath();

	CAppPostgreSQL database(name , prop.Host.c_str() , prop.User.c_str() , prop.Password.c_str());
	try
	{
		m_wndTreeList.InsertColumn(m_wndTreeList.GetColumnCount(),_T("file_name"));
		m_wndTreeList.InsertColumn(m_wndTreeList.GetColumnCount(),_T("draw_no"));
		m_wndTreeList.InsertColumn(m_wndTreeList.GetColumnCount(),_T("rev_no"),LVCFMT_LEFT,50);
		m_wndTreeList.InsertColumn(m_wndTreeList.GetColumnCount(),_T("plant_no"));
		m_wndTreeList.InsertColumn(m_wndTreeList.GetColumnCount(),_T("area_no"));
		m_wndTreeList.InsertColumn(m_wndTreeList.GetColumnCount(),_T("unit_no"));
		m_wndTreeList.InsertColumn(m_wndTreeList.GetColumnCount(),_T("section_no"));
		m_wndTreeList.InsertColumn(m_wndTreeList.GetColumnCount(),_T("draw_no"),LVCFMT_LEFT,50);
		m_wndTreeList.InsertColumn(m_wndTreeList.GetColumnCount(),_T("sheet_no"),LVCFMT_LEFT,50);
		m_wndTreeList.InsertColumn(m_wndTreeList.GetColumnCount(),_T("line_id"));
		m_wndTreeList.InsertColumn(m_wndTreeList.GetColumnCount(),_T("issue_date"),LVCFMT_LEFT,100);
		m_wndTreeList.InsertColumn(m_wndTreeList.GetColumnCount(),_T("CAD"),LVCFMT_CENTER);
		m_wndTreeList.InsertColumn(m_wndTreeList.GetColumnCount(),_T("class"),LVCFMT_CENTER);
		m_wndTreeList.InsertColumn(m_wndTreeList.GetColumnCount(),_T("ins_type"),LVCFMT_CENTER);
		m_wndTreeList.InsertColumn(m_wndTreeList.GetColumnCount(),_T("line_no"),LVCFMT_CENTER);
		m_wndTreeList.InsertColumn(m_wndTreeList.GetColumnCount(),_T("paint_code"),LVCFMT_CENTER);
		m_wndTreeList.InsertColumn(m_wndTreeList.GetColumnCount(),_T("ins_thickness"),LVCFMT_CENTER);
		m_wndTreeList.InsertColumn(m_wndTreeList.GetColumnCount(),_T("elecctritrace"),LVCFMT_CENTER);
		m_wndTreeList.InsertColumn(m_wndTreeList.GetColumnCount(),_T("test_type"),LVCFMT_CENTER);
		m_wndTreeList.InsertColumn(m_wndTreeList.GetColumnCount(),_T("test_press"),LVCFMT_CENTER);
		m_wndTreeList.InsertColumn(m_wndTreeList.GetColumnCount(),_T("oper_temp"),LVCFMT_CENTER);
		m_wndTreeList.InsertColumn(m_wndTreeList.GetColumnCount(),_T("oper_pres"),LVCFMT_CENTER);
		m_wndTreeList.InsertColumn(m_wndTreeList.GetColumnCount(),_T("dgn_temp"),LVCFMT_CENTER);
		m_wndTreeList.InsertColumn(m_wndTreeList.GetColumnCount(),_T("dgn_pres"),LVCFMT_CENTER);
		m_wndTreeList.InsertColumn(m_wndTreeList.GetColumnCount(),_T("pwht"),LVCFMT_CENTER);
		m_wndTreeList.InsertColumn(m_wndTreeList.GetColumnCount(),_T("radiography"),LVCFMT_CENTER);
		m_wndTreeList.InsertColumn(m_wndTreeList.GetColumnCount(),_T("steam_trace"),LVCFMT_CENTER);
		m_wndTreeList.InsertColumn(m_wndTreeList.GetColumnCount(),_T("pw_mt"),LVCFMT_CENTER);
		m_wndTreeList.InsertColumn(m_wndTreeList.GetColumnCount(),_T("remark"),LVCFMT_CENTER);
		m_wndTreeList.InsertColumn(m_wndTreeList.GetColumnCount(),_T("status"),LVCFMT_CENTER);
		m_wndTreeList.InsertColumn(m_wndTreeList.GetColumnCount(),_T("site"),LVCFMT_CENTER);
		m_wndTreeList.InsertColumn(m_wndTreeList.GetColumnCount(),_T("plan_no"),LVCFMT_CENTER);

		m_wndTreeList.InsertItem(name,ROOT_ICON,ROOT_ICON,NULL,NULL);
	}
	catch(const std::exception& ex)
	{
		AfxMessageBox(ex.what(),MB_OK|MB_ICONERROR);
	}
}

/**
	@brief	load issued line data
	@author	humkyung
*/
void CBackupDataForm::LoadLineDataRecordSetFromDatabase(const CString& sWhereClause)
{
	CAppDocData*  pDocData = CAppDocData::GetInstance();
	if(NULL != pDocData)
	{
		CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();
		CAppDocData* pDocData = CAppDocData::GetInstance();
		const CString name = pDocData->GetProject()->name();
		const CProject::DatabaseProp prop = pDocData->GetProject()->GetDatabaseProp();

		BeginWaitCursor();

		CAppPostgreSQL database(name , prop.Host.c_str() , prop.User.c_str() , prop.Password.c_str());
		try
		{
			for(list<CLineDataRecord*>::iterator itr = m_oLineDataRecordList.begin();itr != m_oLineDataRecordList.end();++itr)
			{
				SAFE_DELETE(*itr);
			}
			m_oLineDataRecordList.clear();
			m_wndTreeList.DeleteAllItems();
			HTREEITEM hRoot = m_wndTreeList.InsertItem(name,3,3,NULL,NULL);
			m_wndTreeList.SetItemData(hRoot,NULL);

			map<STRING_T,HTREEITEM> oTreeItemMap;
			STRING_T sSql = STRING_T(_T("SELECT A.* FROM T_ISSUED_ISO_LINE_DATA A JOIN T_REVISION_STAGE B ON A.REV_NO=B.REVISION_TEXT")) + 
				(!sWhereClause.IsEmpty() ? (_T(" ") + sWhereClause).operator LPCTSTR() : _T("")) +
				_T(" ORDER BY A.KEY,B.REVISION_NO DESC");
			soci::rowset<soci::row> rs(database.session()->prepare << sSql);
			int iCount = 0;
			TCHAR sColText[256]={0,};
			for(soci::rowset<soci::row>::const_iterator itr = rs.begin();itr != rs.end();++itr,++iCount)
			{
				CLineDataRecord* pLineDataRecord = CLineDataRecord::CreateInstance();
				m_oLineDataRecordList.push_back(pLineDataRecord);
				CDataFieldSet* pDataFieldSet = CDataFieldSet::CreateInstance();
				for(int i=0;i < itr->size();++i)
				{
					const soci::column_properties colProp = itr->get_properties(i);
					STRING_T sFieldName = colProp.get_name(),sValue;
					try
					{
						sValue = itr->get<STRING_T>(i);
					}catch(...){}
					CDataField* pDataField = CDataField::CreateInstance();
					pDataField->Set(sFieldName.c_str(),_T(""),sValue.c_str());
					pDataFieldSet->Add(pDataField);
				}
				pLineDataRecord->Set(pDataFieldSet);

				HTREEITEM hItem=NULL;
				for(int col = 0;col < m_wndTreeList.GetColumnCount();++col)
				{
					LVCOLUMN lvc = {0};
					lvc.mask = LVCF_TEXT;
					lvc.pszText = sColText;
					lvc.cchTextMax = sizeof(sColText)-1;
					m_wndTreeList.GetColumn(col,&lvc);
					if(0 == STRICMP_T(_T("CAD"),lvc.pszText))
					{
						CDataField* pDataField = pLineDataRecord->FindWithFieldName(_T("KEY"));
						const STRING_T sKey = (NULL != pDataField) ? pDataField->value() : _T(""); 
						pDataField = pLineDataRecord->FindWithFieldName(_T("REV_NO"));
						const STRING_T sRevNo = (NULL != pDataField) ? pDataField->value() : _T("");
						sSql = STRING_T(_T("SELECT COUNT(*) FROM T_ISO_DWG_FILE WHERE KEY='") + sKey + _T("' AND REV_NO='") + sRevNo + _T("' AND FILE_DESC='DGN V7'"));
						long long lCount=0L;
						(*database.session()) << sSql,soci::into(lCount);
						if(lCount > 0)
						{
							m_wndTreeList.SetItem(hItem,MSTN_COL,TVIF_IMAGE,_T(""),MSTN_ICON,MSTN_ICON,0,0,0);
						}
					}
					else
					{
						CDataField* pDataField = pLineDataRecord->FindWithFieldName(lvc.pszText);
						const STRING_T sValue = (NULL != pDataField) ? pDataField->value() : _T(""); 
						if(0 == col)
						{
							pDataField = pLineDataRecord->FindWithFieldName(_T("KEY"));
							const STRING_T sKey = (NULL != pDataField) ? pDataField->value() : _T(""); 
							const map<STRING_T,HTREEITEM>::const_iterator where = oTreeItemMap.find(sKey);
							if(where == oTreeItemMap.end())
							{
								hItem = m_wndTreeList.InsertItem(sValue.c_str(),ISO_DWG_ICON,ISO_DWG_ICON,hRoot,hRoot);
								oTreeItemMap.insert(make_pair(sKey,hItem));
							}
							else
							{
								hItem = m_wndTreeList.InsertItem(sValue.c_str(),ISO_DWG_ICON,ISO_DWG_ICON,where->second,where->second);
							}
							m_wndTreeList.SetItemData(hItem,(DWORD)pLineDataRecord);
						}
						else
						{
							m_wndTreeList.SetItemText(hItem,sValue.c_str(),col);
						}
					}
				}
			}
			m_wndTreeList.Expand(m_wndTreeList.GetRootItem(),TVE_EXPAND);
		}
		catch(const std::exception& ex)
		{
			AfxMessageBox(ex.what(),MB_OK|MB_ICONERROR);
		}
		EndWaitCursor();
	}
}

/**
	@brief	set output form
	@author	humkyung
*/
void CBackupDataForm::SetOutputForm(CBackupDataOutputForm *p)
{
	assert(p && "Null Parameter");
	if(p) m_pOutputForm=p;
}

/******************************************************************************
	@brief		get CAD file from issue data and then open the file.
    @author     humkyung
    @date       2015-11-24
    @function   OpenCADFile
    @return     CString
******************************************************************************/
int CBackupDataForm::OpenIsoDwgFile(const CString& sFileName,const CString& sKey,const CString& sRevNo) const
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	const CProject::DatabaseProp prop = pDocData->GetProject()->GetDatabaseProp();

	try
	{
		CAppPostgreSQL database(pDocData->GetProject()->name(),prop.Host.c_str(),prop.User.c_str(),prop.Password.c_str());
		{
			soci::transaction txn(*database.session());
			soci::blob b(*database.session());

			CString sSql; 
			sSql.Format(_T("SELECT FILE FROM T_ISO_DWG_FILE WHERE KEY='%s' AND REV_NO='%s' AND FILE_DESC='DGN V7'"),sKey,sRevNo);
			(*database.session()) << sSql.operator LPCTSTR(),into(b);
			const size_t length = b.get_len();
			if(length > 0)
			{
				TCHAR *pData = NULL;
				if(pData = (TCHAR*)calloc(1 , sizeof(TCHAR)*(length + 1)))
				{
					b.read(0,pData,sizeof(TCHAR)*length);
					const CString sFilePath = CString(pDocData->GetProjectPath().c_str()) + _T("\\Temp\\") + sFileName;
					CFile f(sFilePath, CFile::modeCreate|CFile:: modeReadWrite);
					f.Write(pData , length);
					f.Close();

					free((void*)pData);
					pData = NULL;

					const CString sIniFilePath = pDocData->GetProject()->GetIniFilePath();
					TCHAR szBuf[MAX_PATH + 1] = {'\0' ,};
					if(GetPrivateProfileString(_T("Iso Edit Setting") , _T("MStation Path") , _T("") ,szBuf,MAX_PATH,sIniFilePath))
					{
						::ShellExecute(NULL,_T("open"),szBuf,sFilePath,NULL,SW_SHOW);
					}
				}
			}
			txn.commit();
			return ERROR_SUCCESS;
		}
	}
	catch(const std::exception& ex)
	{
		//AfxMessageBox(ex.what(),MB_OK|MB_ICONERROR);
	}

	return ERROR_BAD_ENVIRONMENT;
}

BOOL CBackupDataForm::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();

	NMHDR* pNMHDR = (NMHDR*)(lParam);
	if((SELECT_ALL == wParam) && (pWorkspaceBar->m_wndQueryBackup.GetDlgCtrlID() == pNMHDR->idFrom))
	{
		const CString str = pWorkspaceBar->m_wndQueryBackup.GetQueryCondition();
	}
	else if((SELECT == wParam) && (pWorkspaceBar->m_wndQueryBackup.GetDlgCtrlID() == pNMHDR->idFrom))
	{
		const CString str = pWorkspaceBar->m_wndQueryBackup.GetQueryCondition();
		LoadLineDataRecordSetFromDatabase(str);
	}
	else if(((WM_USER + 100)== pNMHDR->code) && m_wndTreeList.GetSafeHwnd() == pNMHDR->hwndFrom)
	{
		HTREEITEM hItem = m_wndTreeList.GetSelectedItem();
		if(NULL != hItem)
		{
			CLineDataRecord* pLineDataRecord = (CLineDataRecord*)m_wndTreeList.GetItemData(hItem);
			if(NULL != pLineDataRecord) m_pOutputForm->DisplayRevisionDataRecordSet(pLineDataRecord);

			CPropertiesWnd* pPropertiesWnd = CPropertiesWnd::GetInstance();
			if(pPropertiesWnd->IsVisible())
			{
				list<CLineDataRecord*> aLineDataRecordList;
				aLineDataRecordList.push_back(pLineDataRecord);
				pPropertiesWnd->DisplayDataOf(aLineDataRecordList,FALSE);
			}
		}
	}
	else if((WM_LBUTTONDBLCLK == pNMHDR->code) && m_wndTreeList.GetSafeHwnd() == pNMHDR->hwndFrom)
	{
		HTREEITEM hItem = m_wndTreeList.GetSelectedItem();
		if((NULL != hItem) && (MSTN_COL == (int)wParam))
		{
			const int nImage = m_wndTreeList.GetItemImageEx(hItem,MSTN_COL);
			if(MSTN_ICON == nImage)
			{
				TCHAR sColText[MAX_PATH]={0,};
				LVCOLUMN lvc = {0};
				lvc.mask = LVCF_TEXT;
				lvc.pszText = sColText;
				lvc.cchTextMax = sizeof(sColText)-1;
				m_wndTreeList.GetColumn(FILE_NAME_COL,&lvc);

				CLineDataRecord* pLineDataRecord = (CLineDataRecord*)m_wndTreeList.GetItemData(hItem);
				CDataField* pDataField = pLineDataRecord->FindWithFieldName(_T("KEY"));
				const CString sKey = pDataField->value();
				pDataField = pLineDataRecord->FindWithFieldName(_T("REV_NO"));
				const CString sRevNo = pDataField->value();
				OpenIsoDwgFile(lvc.pszText,sKey,sRevNo);
			}
		}
	}

	return CFormView::OnNotify(wParam, lParam, pResult);
}

/**
	@brief	pop up menu를 생성한다.
	@author	humkyung
	@date	2015.12.05
	@param	pNMHDR
	@param	pResult
	@return	void
**/
void CBackupDataForm::OnRclickTreeList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CPoint point,ptClient;
	::GetCursorPos(&point);
	ptClient = point;
	m_wndTreeList.ScreenToClient(&ptClient);

	HTREEITEM hItem = m_wndTreeList.HitTest(ptClient);
	if(NULL != hItem)
	{
		m_wndTreeList.SelectItem(hItem);

		BCMenu menu;
		menu.LoadMenu(IDR_ISSUE_MANAGER);
		CMenu* pMenu = menu.GetSubMenu(0);
		if(pMenu)
		{
			CPropertiesWnd* pPropertiesWnd = CPropertiesWnd::GetInstance();
			if(TRUE == pPropertiesWnd->IsVisible())
			{
				pMenu->CheckMenuItem( ID_ISSUE_MANAGER_PROPERTIES , MF_CHECKED);
			}
			else
			{
				pMenu->CheckMenuItem( ID_ISSUE_MANAGER_PROPERTIES , MF_UNCHECKED);
			}

			pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON , point.x , point.y , this);
		}
	}

	*pResult = 0;
}

/**	@brief	OnProperties
	@author	humkyung
	@date	2015.12.05
	@parm	
	@return	void
*/
void CBackupDataForm::OnProperties()
{
	HTREEITEM hItem = m_wndTreeList.GetSelectedItem();
	if(NULL != hItem)
	{
		CLineDataRecord* pLineDataRecord = (CLineDataRecord*)m_wndTreeList.GetItemData(hItem);
		if(NULL != pLineDataRecord)
		{
			CPropertiesWnd* pPropertiesWnd = CPropertiesWnd::GetInstance();
			if(!pPropertiesWnd->IsVisible())
			{
				pPropertiesWnd->ShowPane(TRUE , FALSE , FALSE);
			}
			else
			{
				pPropertiesWnd->ShowPane(FALSE , FALSE , FALSE);
			}

			list<CLineDataRecord*> aLineDataRecordList;
			aLineDataRecordList.push_back(pLineDataRecord);
			pPropertiesWnd->DisplayDataOf(aLineDataRecordList,FALSE);
		}
	}
}