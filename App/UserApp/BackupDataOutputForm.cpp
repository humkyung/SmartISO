// BackupDataOutputForm.cpp : implementation file
//

#include "stdafx.h"
#include "UserApp.h"
#include <CompareResult.h>
#include "MainFrm.h"
#include "UserAppView.h"
#include "BackupDataForm.h"
#include "BackupDataOutputForm.h"
#include "UserAppDocData.h"
#include "WorkStatusDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBackupDataOutputForm

IMPLEMENT_DYNCREATE(CBackupDataOutputForm, CFormView)

CBackupDataOutputForm::CBackupDataOutputForm()
	: CFormView(CBackupDataOutputForm::IDD) , m_pUserAppDoc(NULL) , m_pBackupDataForm(NULL) , m_pBoldFont(NULL)
{
	//{{AFX_DATA_INIT(CBackupDataOutputForm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBackupDataOutputForm::~CBackupDataOutputForm()
{
	try
	{
		if(NULL != m_pBoldFont) SAFE_DELETE(m_pBoldFont);
	}
	catch(...)
	{

	}
}

void CBackupDataOutputForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBackupDataOutputForm)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBackupDataOutputForm, CFormView)
	//{{AFX_MSG_MAP(CBackupDataOutputForm)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBackupDataOutputForm diagnostics

#ifdef _DEBUG
void CBackupDataOutputForm::AssertValid() const
{
	CFormView::AssertValid();
}

void CBackupDataOutputForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBackupDataOutputForm message handlers

BOOL CBackupDataOutputForm::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

/**
	@author	humkyung
**/
void CBackupDataOutputForm::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	static bool __init = false;
	
	if(false == __init)
	{
		CMainFrame* pMainFrame = GetUserAppMainWnd();
		//////////////////////////////////////////////////////////////////////////
		CUserAppView* pUserAppView = (CUserAppView*)(CUserAppView::GetInstance());
		m_pBackupDataForm = pUserAppView->m_pBackupDataForm;

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
			m_wndRevDataGrid.Create(rect , this , CBackupDataOutputForm::IDD + 0x100 , WS_CHILD | WS_VISIBLE);

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
				
				/*m_RevFieldNameEntry.clear();
				m_RevFieldDescEntry.clear();*/
				
				for(int i = 0;i < nFieldCount;++i)
				{
					const string& rFieldName = sqlFile.GetFieldNameAt(i);
					/*m_RevFieldNameEntry.push_back(rFieldName.c_str());
					m_RevFieldDescEntry.push_back(rFieldName.c_str());*/
					m_wndRevDataGrid.SetItemText(0,i,rFieldName.c_str());
					
					/*if(IsFixedRevisionField(rFieldName.c_str()))
					{
						m_wndRevDataGrid.SetItemData(0 , i , 0);
					}
					else
					{
						m_wndRevDataGrid.SetItemData(0 , i , 1);
					}*/
				}
			}

			//DisplayRevisionDataRecordSet(NULL);
		}

		__init = true;
	}
}

/**
	@brief	
	@author	humkyung	
**/
void CBackupDataOutputForm::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	if(m_wndRevDataGrid.GetSafeHwnd())
	{
		CRect rect;
		CWnd* pWnd = GetDlgItem(IDC_STATIC_REVISION_DATA);
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);
		pWnd->SetWindowPos(NULL,0,0,cx-rect.left-5,cy-rect.top-5,SWP_NOMOVE|SWP_NOZORDER);

		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);
		rect.DeflateRect(5 , 18 , 5 , 5);
		m_wndRevDataGrid.SetWindowPos(NULL,rect.left,rect.top,rect.Width(),rect.Height(),SWP_NOZORDER);
	}
}

void CBackupDataOutputForm::InitContents()
{
	/*
	m_CurrentLineDataDlg.InitContents();
	m_compareLineDataDlg.InitContents();
	m_newLineDataDlg.InitContents();
	m_deletedLineDataDlg.InitContents();
	*/
}


/**	
	@brief	revision data를 표기합니다.
	@author	HumKyung
	@date	2015.11.23
	@param	pRecord
	@return	void	
*/
void CBackupDataOutputForm::DisplayRevisionDataRecordSet(CLineDataRecord* pRecord)
{
	assert(pRecord && "pRecord is NULL");

	if(pRecord && m_wndRevDataGrid.GetSafeHwnd())
	{
		TCHAR szBuf[MAX_PATH] = {0,};
		CString rString;
		
		CAppDocData* pDocData = CAppDocData::GetInstance();
		const CProject::DatabaseProp prop = pDocData->GetProject()->GetDatabaseProp();
		try
		{
			const CString sKey = pRecord->FindWithFieldName(_T("KEY"))->value();
			const CString sRevNo = pRecord->FindWithFieldName(_T("REV_NO"))->value();

			CAppPostgreSQL database(pDocData->GetProject()->name(),prop.Host.c_str(),prop.User.c_str(),prop.Password.c_str());
			CString sql;
			sql.Format(_T("SELECT * FROM T_ISSUED_ISO_REVISION WHERE KEY='%s' AND REV_NO='%s'") , sKey , sRevNo);
			soci::rowset<soci::row> rs(database.session()->prepare << sql.operator LPCTSTR());
			int row=1,iColCount = m_wndRevDataGrid.GetColumnCount();
			m_wndRevDataGrid.SetRowCount(1);
			for(soci::rowset<soci::row>::const_iterator itr = rs.begin();itr != rs.end();++itr,++row)
			{
				m_wndRevDataGrid.SetRowCount(m_wndRevDataGrid.GetRowCount()+1);
				for(int col = 1;col < iColCount;++col)
				{
					const CString sColName = m_wndRevDataGrid.GetItemText(0,col);
					STRING_T sValue;
					try
					{
						sValue = itr->get<STRING_T>(sColName.operator LPCTSTR());
					}catch(...){}
					m_wndRevDataGrid.SetItemText(row , col , sValue.c_str());
					m_wndRevDataGrid.SetItemState(row,col,m_wndRevDataGrid.GetItemState(row,col) | GVNI_READONLY);
				}
			}
		}
		catch(...){}
	}
	
	m_wndRevDataGrid.AutoSizeRows();
	m_wndRevDataGrid.AutoSizeColumns();
	m_wndRevDataGrid.RedrawWindow();
}