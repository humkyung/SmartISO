// OraDataReportForm.cpp : implementation file
//

#include "stdafx.h"
#include <assert.h>
#include "MainFrm.h"
#include "LineControlManagerDoc.h"
#include "LineControlManager.h"
#include "OraDataReportForm.h"
#include "enablebuddybutton.h"
#include "LineListToOraDocData.h"
#include <ado/ADODB.h>
#include <PDSOracle.h>

#include <set>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COraDataReportForm

IMPLEMENT_DYNCREATE(COraDataReportForm, CView)

COraDataReportForm::COraDataReportForm()
	/*: CView(COraDataReportForm::IDD)*/
{
	//{{AFX_DATA_INIT(COraDataReportForm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

COraDataReportForm::~COraDataReportForm()
{
}

void COraDataReportForm::DoDataExchange(CDataExchange* pDX)
{
	CView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COraDataReportForm)
	//DDX_Control(pDX , IDC_BUTTON_EXCEL_EXPORT , m_wndExcelExportButton);
	///DDX_Control(pDX , IDC_SELECT_ALL_AREA , m_wndSelectAllAreaNoButton);
	///DDX_Control(pDX , IDC_LIST_AREA_NO , m_wndAreaNoReport);
	///DDX_Control(pDX , IDC_SELECT_ALL_MODEL , m_wndSelectAllModelNoButton);
	///DDX_Control(pDX , IDC_LIST_MODEL_NO, m_wndModelNoReport);
	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COraDataReportForm, CView)
	//{{AFX_MSG_MAP(COraDataReportForm)
	ON_WM_SIZE()
	///ON_BN_CLICKED(IDC_BUTTON_EXCEL_EXPORT , OnButtonExcelExport)
	///ON_BN_CLICKED(IDC_SELECT_ALL_AREA , OnButtonSelectAllAreaNo)
	///ON_BN_CLICKED(IDC_SELECT_ALL_MODEL, OnButtonSelectAllModelNo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COraDataReportForm diagnostics

#ifdef _DEBUG
void COraDataReportForm::AssertValid() const
{
	CView::AssertValid();
}

void COraDataReportForm::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COraDataReportForm message handlers

BOOL COraDataReportForm::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

/**
	@brief	create custom data report dialog with custom data table

	@author	humkyung

	@date	2011.05.10

	@param	pCustomReportTable

	@return
**/
int COraDataReportForm::CreateCustomDataReportDialog(CCustomReportTable* pCustomReportTable)
{
	assert(pCustomReportTable && "pCustomReportTable is NULL");

	if(pCustomReportTable)
	{
		CCustomDataReportDlg* pReportDlg = new CCustomDataReportDlg(pCustomReportTable , &m_wndTab);
		if(NULL != pReportDlg)
		{
			if(TRUE == pReportDlg->Create(CCustomDataReportDlg::IDD , &m_wndTab))
			{
				m_wndTab.AddTab(pReportDlg,  pCustomReportTable->m_sName.c_str());
			}
		}

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

/**
	@brief	create custom data report dialog which is not already created.

	@author	humkyung

	@date	2011.05.10

	@param	pCustomReportTable

	@return
**/
int COraDataReportForm::SyncTabCtrlWithCustomReportTableList()
{
	CLineListToOraDocData& docData = CLineListToOraDocData::GetInstance();	
	list<CCustomReportTable*>* pCustomDataReportTableList = docData.CustomReportTableList();
	if(NULL != pCustomDataReportTableList)
	{
		for(list<CCustomReportTable*>::iterator itr = pCustomDataReportTableList->begin();itr != pCustomDataReportTableList->end();++itr)
		{
			bool bAlreadyCreated = false;
			CString sHeaderLabel = (*itr)->m_sName.c_str();
			for(int i = 0;i < m_wndTab.GetTabsNum();++i)
			{
				CString strLabel;
				m_wndTab.GetTabLabel(i , strLabel);
				if(sHeaderLabel == strLabel)
				{
					bAlreadyCreated = true;
					break;
				}
			}
			if(true == bAlreadyCreated) continue;

			CreateCustomDataReportDialog(*itr);
		}

		for(int i = m_wndTab.GetTabsNum() - 1;i >= 0 ;--i)
		{
			bool bAlreadyCreated = false;
			CString strLabel;
			m_wndTab.GetTabLabel(i , strLabel);
			for(list<CCustomReportTable*>::iterator itr = pCustomDataReportTableList->begin();itr != pCustomDataReportTableList->end();++itr)
			{
				CString sHeaderLabel = (*itr)->m_sName.c_str();
				if(sHeaderLabel == strLabel)
				{
					bAlreadyCreated = true;
					break;
				}
			}
			if(true == bAlreadyCreated) continue;
			
			m_wndTab.RemoveTab(i);
		}
	}

	return ERROR_SUCCESS;
}

/**
	@brief	

	@author	humkyung

	@date	????.??.??

	@param	

	@return
**/
void COraDataReportForm::OnInitialUpdate() 
{
	static bool __init__ = false;

	CView::OnInitialUpdate();
	
	if(false == __init__)
	{
		CRect rect;
		rect.SetRectEmpty();
		if (!m_wndTab.Create(CMFCTabCtrl::STYLE_3D_VS2005 , rect, this , 0x100 , CMFCBaseTabCtrl::LOCATION_BOTTOM))
		{
			TRACE0("Failed to create workspace tab window\n");
			return;      /// fail to create
		}
		m_wndTab.SetActiveTabTextColor(RGB(0 , 0 , 255));
		if(FALSE == m_imgList.Create(IDB_LLTOORA_APP , 16 , 16 , RGB(255 , 0 , 255)))
		{
			TRACE0("Failed to create imagelist\n");
			return;      /// fail to create
		}
		m_wndTab.SetImageList(m_imgList);
		
		//! create custom data report dialog - 2011.05.04 added by humkyung
		CLineListToOraDocData& docData = CLineListToOraDocData::GetInstance();
		docData.LoadCustomDataReportList();

		list<CCustomReportTable*>* pCustomDataReportTableList = docData.CustomReportTableList();
		if(NULL != pCustomDataReportTableList)
		{
			for(list<CCustomReportTable*>::iterator itr = pCustomDataReportTableList->begin();itr != pCustomDataReportTableList->end();++itr)
			{
				CreateCustomDataReportDialog(*itr);
			}
		}

		__init__=true;
	}
}

/**
	@brief	

	@author	humkyung

	@date	????.??.??

	@param	

	@return
**/
void COraDataReportForm::OnSize(UINT nType, int cx, int cy) 
{
	static const int GAP=5;
	CView::OnSize(nType, cx, cy);
	
	if(m_wndTab.GetSafeHwnd())
	{
		m_wndTab.SetWindowPos(NULL ,  0 , 0 , cx , cy , SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

/**	\brief	report를 뽑아 엑셀 파일로 저장한다.
**/
void COraDataReportForm::OnButtonExcelExport() 
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	CLineControlManagerDoc* pDoc = static_cast<CLineControlManagerDoc*>(pFrame->GetActiveDocument());
	/*
	LoadData();

	CAdoRemoteDB RemoteDB;
	CString	strDBPath = "provider=MSDAORA;User ID="+ m_rUserName +";Password="+ m_rUserPassword + ";Data Source=" + m_rServiceName;
	if(TRUE == RemoteDB.DBConnect(strDBPath))
	{
		vector<CString> TableNames;
		CString rValue;
		
		/// 오라클 테이블 이름을 구한다.
		CPDSOracle PDSOracle;
		PDSOracle.Set(m_rServiceName , m_rUserName , m_rUserPassword , m_rProjectName);
		PDSOracle.LoadNoteTableFromDataFile(m_rStdNoteFilePath.operator const char*());
		if(PDSOracle.Get_PDTABLE_12_TableNames(TableNames , RemoteDB))
		{
			pDoc->QueryOracleForReport(RemoteDB , PDSOracle , m_rProjectName , TableNames);
	*/
			///pDoc->ReportToExcel();
			
			/*
			m_dlgLineListReport.Report(RemoteDB , m_rProjectName , TableNames);
			m_dlgNozzleReport.Report(RemoteDB , m_rProjectName , TableNames);
			m_dlgInstReport.Report(RemoteDB , m_rProjectName , TableNames);
			m_dlgSpecialItemReport.Report(RemoteDB , m_rProjectName , TableNames);
			*/
	/*	}

		set<CString> AreaNameSet , ModelNoSet;
		for(vector<CLineListReportData>::iterator itr = pDoc->m_LineListReportDataEntry.begin();itr != pDoc->m_LineListReportDataEntry.end();++itr)
		{
			AreaNameSet.insert(itr->m_rDatas[AREA_NAME]);
			ModelNoSet.insert(itr->m_rDatas[MODEL_NO]);
		}

		m_wndAreaNoReport.DeleteAllItems();
		for(set<CString>::iterator jtr = AreaNameSet.begin();jtr != AreaNameSet.end();++jtr)
		{
			m_wndAreaNoReport.InsertItem(m_wndAreaNoReport.GetItemCount() , jtr->operator LPCSTR());
		}

		m_wndModelNoReport.DeleteAllItems();
		for(jtr = ModelNoSet.begin();jtr != ModelNoSet.end();++jtr)
		{
			m_wndModelNoReport.InsertItem(m_wndModelNoReport.GetItemCount() , jtr->operator LPCSTR());
		}
	}

	m_dlgLineListReport.UpdateData();
	m_dlgNozzleReport.UpdateData();
	*/
}

/**
**/
void COraDataReportForm::OnButtonSelectAllAreaNo() 
{
	// TODO: Add your control notification handler code here
	m_wndAreaNoReport.CheckAllItems();
}

/**
**/
void COraDataReportForm::OnButtonSelectAllModelNo() 
{
	// TODO: Add your control notification handler code here
	m_wndModelNoReport.CheckAllItems();
}

/**
	@brief	load data

	@author	humkyung

	@date	????.??.??

	@param

	@return
**/
void COraDataReportForm::LoadData()
{
	static const IS_STRING APP( _T("General") );
	IS_CHAR szBuf[MAX_PATH + 1] = {'\0' ,};

	CLineListToOraDocData& docData = CLineListToOraDocData::GetInstance();
	IS_STRING rIniFilePath = docData.GetIniFilePath();
	if(GetPrivateProfileString(APP.c_str() , _T("Project") , NULL , szBuf , MAX_PATH , rIniFilePath.c_str()))
	{
		m_rProjectName = szBuf;
	}else	m_rProjectName.Empty();

	if(GetPrivateProfileString(APP.c_str() , _T("Service Name") , NULL , szBuf , MAX_PATH , rIniFilePath.c_str()))
	{
		m_rServiceName = szBuf;
	}else	m_rServiceName.Empty();

	if(GetPrivateProfileString(APP.c_str() , _T("User Name") , NULL , szBuf , MAX_PATH , rIniFilePath.c_str()))
	{
		m_rUserName = szBuf;
	}else	m_rUserName.Empty();

	if(GetPrivateProfileString(APP.c_str() , _T("User Password") , NULL , szBuf , MAX_PATH , rIniFilePath.c_str()))
	{
		m_rUserPassword = szBuf;
	}else	m_rUserPassword.Empty();

	///
	if(GetPrivateProfileString(_T("MAPPING") , _T("Std Note file path") , NULL , szBuf , MAX_PATH , rIniFilePath.c_str()))
	{
		m_rStdNoteFilePath = szBuf;
	}else	m_rStdNoteFilePath.Empty();
}
