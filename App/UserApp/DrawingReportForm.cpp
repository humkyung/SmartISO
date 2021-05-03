// IndexReportForm.cpp : implementation file
//

#include "stdafx.h"
#include "UserApp.h"
#include "MainFrm.h"
#include "UserAppView.h"
#include "DrawingReportForm.h"
#include "UserAppDocData.h"
#include <tokenizer.h>
#include "WorkStatusDlg.h"
#include "WorkspaceWnd.h"
#include <FileTools.h>

#include <ProjectData.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	FILE_NAME_COL	1
#define	FILE_EXIST		11
#define	FILE_NOT_FOUND	12
/////////////////////////////////////////////////////////////////////////////
// CDrawingReportForm

IMPLEMENT_DYNCREATE(CDrawingReportForm, CFormView)

CDrawingReportForm::CDrawingReportForm()
	: CFormView(CDrawingReportForm::IDD), m_pBoldFont(NULL), m_pLineDataRecordSet(NULL)
{
	//{{AFX_DATA_INIT(CDrawingReportForm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pLogFileForm = NULL;
	m_rxnode = NULL;
	m_nRecordCount = 0;
}

CDrawingReportForm::~CDrawingReportForm()
{
	try
	{
		if(m_pBoldFont != NULL) delete m_pBoldFont;
		if(m_pLineDataRecordSet != NULL) delete m_pLineDataRecordSet;
		m_pLogFileForm = NULL;

		if (m_rxnode)
		{
			IsRxFree (m_rxnode);
			m_rxnode = NULL;
		}
	}catch(...)
	{
		
	}
}

void CDrawingReportForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDrawingReportForm)
	DDX_Text(pDX , IDC_STATIC_STATUS , m_rIndexReportFolder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDrawingReportForm, CFormView)
	//{{AFX_MSG_MAP(CDrawingReportForm)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_ExcelView,OnExlView)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_ExcelView, OnUpdateButtonExlView)
	//}}AFX_MSG_MAP
//	ON_NOTIFY(NM_RCLICK, 0x100, OnRclickFileList)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CDrawingReportForm diagnostics

#ifdef _DEBUG
void CDrawingReportForm::AssertValid() const
{
	CFormView::AssertValid();
}

void CDrawingReportForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDrawingReportForm message handlers

BOOL CDrawingReportForm::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{	
	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CDrawingReportForm::OnInitialUpdate() 
{
	static bool __init = false;
	CFormView::OnInitialUpdate();
	
	if(false == __init)
	{
		CWnd* pWnd = GetDlgItem(IDC_STATIC_OUTPUT_ISO_DRAWING_FOLDER);
		if(NULL != pWnd) pWnd->SetWindowText(_TR("Output Iso Drawing Folder") + _T(" : "));

		m_ImageList.Create(IDB_CHECKBOXES, 16, 3, RGB(255,0,255));
	
		CRect rectDummy;
		rectDummy.SetRectEmpty ();
		GetClientRect(rectDummy);
		
		CUserAppView* pView = (CUserAppView*)(GetParent());
		CUserAppDoc*  pDoc  = pView->GetDocument();
		
		if(m_wndLineDataReport.SubclassWindow(GetDlgItem(IDC_LIST_LINE_DATA)->GetSafeHwnd()))
		{
			m_wndLineDataReport.SetWindowPos(NULL , 0 , 30 , 0 , 0 , SWP_NOZORDER);
			LONG style = GetWindowLong(m_wndLineDataReport.m_hWnd , GWL_STYLE);
			style |= WS_EX_CLIENTEDGE;
			SetWindowLong(m_wndLineDataReport.m_hWnd , GWL_EXSTYLE , style );
			m_wndLineDataReport.SetImageList(&m_ImageList , LVSIL_SMALL);
			m_wndLineDataReport.SetHeaderImageList(&m_ImageList);
			m_wndLineDataReport.SetHeadings( _T("[],30;") + _TR("File Name") + _T(",300;File Exist,100") );
			m_wndLineDataReport.SetGridLines();
			m_wndLineDataReport.SetExtendedStyle(LVS_EX_CHECKBOXES | m_wndLineDataReport.GetExtendedStyle());
			m_wndLineDataReport.EnableHeaderCheckBox(TRUE);
		}

		m_pBoldFont = new CFont;
		m_pBoldFont->CreateFont(18,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0 , _T("Arial"));
		m_FileExist = false;
		__init = true;
	}
}

void CDrawingReportForm::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	DetermineLayout(cx,cy);
}

void CDrawingReportForm::InitContents()
{
	CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();
	pWorkspaceBar->m_wndQuery3.InitContents( this );

	CUserAppView* pView = static_cast<CUserAppView*>(GetParent());
	CUserAppDoc*  pDoc  = pView->GetDocument();
	CAppDocData* pDocData = CAppDocData::GetInstance();
	m_rIniFilePath= pDocData->GetProject()->GetIniFilePath();
	
	TCHAR szBuf[MAX_PATH] = {'\0' ,};
	if(GetPrivateProfileString( _T("Iso Edit Setting") , _T("Output Iso Dwg File Folder") , NULL , szBuf , MAX_PATH , m_rIniFilePath))
	{
		m_rIndexReportFolder = szBuf;
		UpdateData(FALSE);
	}
}

void CDrawingReportForm::OnRclickFileList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
}

/**
	@brief	layout all components
	@author	humkyung
*/
void CDrawingReportForm::DetermineLayout(int cx , int cy)
{
	const int gap = 3 , LeftX = 2 , RIGHT_MARGIN = 20;
	CWnd* pWnd = NULL;
	
	//if(m_pIsoFileList && m_pIsoFileList->GetSafeHwnd())
	{
		CRect rect[3] , rcPrintButton;
		
		pWnd = GetDlgItem(IDC_STATIC_OUTPUT_ISO_DRAWING_FOLDER);
		if(pWnd && m_wndLineDataReport.GetSafeHwnd())
		{
			m_wndLineDataReport.GetWindowRect(&rect[1]);
			ScreenToClient(&rect[1]);
			
			pWnd->SetWindowPos(NULL , LeftX , gap , 0 , 0 , SWP_NOSIZE);
		}

		pWnd = GetDlgItem(IDC_STATIC_STATUS);
		if(pWnd)
		{
			CWnd* pWnd1 = GetDlgItem(IDC_STATIC_OUTPUT_ISO_DRAWING_FOLDER);
			pWnd1->GetWindowRect(&rect[0]);
			ScreenToClient(&rect[0]);
						
			pWnd->SetWindowPos(NULL , rect[0].right + 3 , rect[0].top , cx - rect[0].right - 6 , rect[0].Height() , SWP_NOZORDER);
			pWnd->GetWindowRect(&rect[1]);
			ScreenToClient(&rect[1]);
		}

		if(m_wndLineDataReport.GetSafeHwnd())
		{
			m_wndLineDataReport.GetWindowRect(&rect[0]);
			ScreenToClient(&rect[0]);
			m_wndLineDataReport.SetWindowPos(NULL , LeftX , rect[1].bottom + 5 , (cx - LeftX) , cy - rect[1].bottom - 10 , SWP_NOZORDER);
		}

		pWnd = GetDlgItem(IDC_STATIC_SELECTED_LINE);
		if(pWnd && m_wndLineDataReport.GetSafeHwnd())
		{
			CRect rectList;
			m_wndLineDataReport.GetWindowRect(&rectList);
			ScreenToClient(&rectList);		
			
			pWnd->SetWindowPos(NULL, rectList.left , rectList.bottom + 4 , 100 , 25 ,SWP_NOZORDER);
		}
		
		pWnd = GetDlgItem(IDC_STATIC_SELECTED_COUNT);
		if(pWnd)
		{
			CRect rectThis, rectStatic;
			pWnd->GetWindowRect(&rectThis);
			ScreenToClient(&rectThis);
			
			CWnd* pWnd1 = GetDlgItem(IDC_STATIC_SELECTED_LINE);
			pWnd1->GetWindowRect(&rectStatic);
			ScreenToClient(&rectStatic);
			
			pWnd->SetWindowPos(NULL, rectStatic.right, rectStatic.top  ,50 ,25 ,SWP_NOZORDER);
		}

		{
			//////////////////////////////////////////////////////////////////////////
			CRect _rect;
			if(m_wndLineDataReport.GetSafeHwnd())
			{
				m_wndLineDataReport.GetWindowRect(&_rect);
				ScreenToClient(&_rect);
			}
		}
	}

	if(::IsWindow(m_wndLineDataReport.GetSafeHwnd()))
	{
		CRect rect;
		m_wndLineDataReport.GetWindowRect(&rect);
		ScreenToClient(&rect);
	}
}

/**	
	@brief	The CDrawingReportForm::OnNotify function
	@author	백흠경
	@param	wParam	a parameter of type WPARAM
	@param	lParam	a parameter of type LPARAM
	@param	pResult	a parameter of type LRESULT*
	@return	BOOL	
*/
BOOL CDrawingReportForm::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();

	NMHDR* pNMHDR = (NMHDR*)(lParam);
	if((SELECT_ALL == wParam) && (pWorkspaceBar->m_wndQuery3.GetDlgCtrlID() == pNMHDR->idFrom))
	{
		CString rQueryCondition = pWorkspaceBar->m_wndQuery3.GetInAreaQueryCondition();
		vector< string > issueDates;
		LoadLineDataRecordSetFromDatabase( rQueryCondition , NULL , issueDates );
	}
	else if((SELECT == wParam) && (pWorkspaceBar->m_wndQuery3.GetDlgCtrlID() == pNMHDR->idFrom))
	{
		CString rQueryCondition = pWorkspaceBar->m_wndQuery3.GetQueryCondition();
		vector< string > issueDates;
		pWorkspaceBar->m_wndQuery3.GetSelectedIssueDate( issueDates );

		CString rRevNo = pWorkspaceBar->m_wndQuery3.GetSelectedRevNo();
		(rRevNo.IsEmpty()) ? LoadLineDataRecordSetFromDatabase(rQueryCondition , NULL , issueDates) : LoadLineDataRecordSetFromDatabase(rQueryCondition , rRevNo , issueDates);
	}

	return CFormView::OnNotify(wParam, lParam, pResult);
}

/**	
	@brief	The CDrawingReportForm::LoadLineDataRecordSetFromDatabase function
	
	@author	humkyung

	@param	rQueryCondition	a parameter of type const CString &
	@param	pRevNo	a parameter of type const char*
	@param	issueDates	a parameter of type const vector< string >&

	@return	void	
*/
void CDrawingReportForm::LoadLineDataRecordSetFromDatabase(const CString &rQueryCondition, LPCTSTR pRevNo , const vector< STRING_T >& issueDates)
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	if(NULL != pDocData)
	{
		CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();

		if(NULL != m_pLineDataRecordSet) CLineDataRecordSet::DeleteInstance(m_pLineDataRecordSet);
		
		BeginWaitCursor();
		
		CString rSelRevNo = pWorkspaceBar->m_wndQuery3.GetSelectedRevNo();
		if(rSelRevNo.IsEmpty() || (_T("All") == rSelRevNo) )
		{
			m_pLineDataRecordSet = pDocData->GetProject()->GetLineDataRecordSetFromDatabase(rQueryCondition , NULL , issueDates);
		}
		else
		{
			m_pLineDataRecordSet = pDocData->GetProject()->GetLineDataRecordSetFromDatabase(rQueryCondition , rSelRevNo.operator LPCTSTR() , issueDates);
		}
		if(NULL != m_pLineDataRecordSet)
		{
			int nRecordCount = m_pLineDataRecordSet->GetLineDataRecordCount();
			if(0 == nRecordCount)
			{
				ShowRecordCount(nRecordCount , nRecordCount);

				m_IsoFileMap.clear();

				CString rMsg;
				rMsg.LoadString(IDS_NO_RECORD);
				AfxMessageBox(rMsg);
				EndWaitCursor();
				return;
			}

			DisplayQueriedReport(nRecordCount , pRevNo);
		}
		EndWaitCursor();
	}
}

/**	
	@brief	QUERY한 LINE DATA RECORD에 대응하는 ISO FILE을 표시한다.
	@author	humkyung
	@param	nRecordCount	a parameter of type int
	@return	void	
*/
void CDrawingReportForm::DisplayQueriedReport(const int& nRecordCount , const CString& rRevNo)
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	CString rIniFilePath = pDocData->GetProject()->GetIniFilePath();
	
	m_wndLineDataReport.SetRedraw( TRUE );
	m_wndLineDataReport.DeleteAllItems();

	TCHAR szBuf[MAX_PATH + 1]={'\0',};
	vector<STRING_T> oFileName;
	if( GetPrivateProfileString( _T("Iso Edit Setting") , _T("Output File Name Format") , _T("") , szBuf , MAX_PATH,rIniFilePath) )
	{
	}
	else
	{
		AfxMessageBox( _T("Please, set Output File Name Format") );
		return;
	}

	CTokenizer<CIsFromString>::Tokenize(oFileName , szBuf , CIsFromString( _T(":") ));
	CString rDesc, rFluid, rLineNo, rSheet,rLineDataValue,rISOValue;
	long nValidRecordCount = 0L;
	for(int j = 0 ; j < nRecordCount ; ++j)
	{
		CLineDataRecord* pLineDataRecord = (CLineDataRecord*)m_pLineDataRecordSet->GetLineDataRecordAt(j);
		/// DELETE된 LINE RECORD에 대해서는 작업을 생략한다.
		if((NULL == pLineDataRecord)/* || (pLineDataRecord && pLineDataRecord->IsDeleted())*/) continue;

		CString rOutputFileName;
		LVITEM lvitem;
		lvitem.mask = LVIF_TEXT | LVIF_IMAGE;
		lvitem.iImage = FILE_NOT_FOUND;
		lvitem.cchTextMax = 32;

		CString rIsoFileName = pDocData->GetProject()->GetOutputIsoFileName(pLineDataRecord);
		CString sIsoFileExt  = pDocData->GetProject()->GetOutputIsoFileExt(pLineDataRecord , rRevNo);

		CRevDataRecordSet* pRevRecordSet = pLineDataRecord->GetRevDataRecordSet();
		const size_t nRecordCount = pRevRecordSet->GetRecordCount( );
		if( nRecordCount > 0 )
		{
			rOutputFileName = rIsoFileName + _T(".") + sIsoFileExt;
		}else	rOutputFileName = rIsoFileName;

		if(CFileTools::DoesFileExist( m_rIndexReportFolder + _T("\\") + rOutputFileName ))
		{
			lvitem.iItem = m_wndLineDataReport.GetItemCount();
			lvitem.iSubItem = 0;
			lvitem.iImage   = FILE_EXIST;
			CString rString;
			rString.Format(_T("%d") , lvitem.iItem + 1);
			lvitem.pszText = (char*)(rString).operator LPCSTR();
			int it = m_wndLineDataReport.InsertItem(&lvitem);
			
			m_wndLineDataReport.SetItemText( it , 1 , rOutputFileName );
			m_wndLineDataReport.SetItemData( it , (DWORD) pLineDataRecord ); /// line data record 저장.
			m_wndLineDataReport.SetItemText( it , 2 , _T("Found"));

			/// 1부터 LineDataRecord를 저장한다.
			++nValidRecordCount;
		}
		else
		{
			lvitem.iItem = m_wndLineDataReport.GetItemCount();
			lvitem.iSubItem = 0;
			lvitem.iImage   = FILE_NOT_FOUND;
			CString rString;
			rString.Format(_T("%d") , lvitem.iItem + 1);
			lvitem.pszText = (char*)(rString).operator LPCSTR();
			const int it = m_wndLineDataReport.InsertItem(&lvitem);
			m_wndLineDataReport.SetItemText( it , 1 , rOutputFileName );
			m_wndLineDataReport.SetItemData( it , (DWORD) pLineDataRecord ); /// line data record 저장.
			m_wndLineDataReport.SetItemText( it , 2 , _T("Not Found"));

			/// 1부터 LineDataRecord를 저장한다.
			++nValidRecordCount;
		}
	}
	m_wndLineDataReport.GroupByColumn(2);	/// group by FileExist column - 2015.09.17 added by humkyung

	m_wndLineDataReport.SetRedraw( TRUE );
	ShowRecordCount( nValidRecordCount , nRecordCount );
}

/**
	@brief	쓰레드를 생성시켜 작업을 수행한다.
	@author	humkyung
	@date	????.??.??
	@param	
	@return
*/
int CDrawingReportForm::Generate()
{	
	CAppDocData* pDocData = CAppDocData::GetInstance();
	const CString sExlpath = CString(pDocData->GetProjectPath().c_str()) + _T("\\Setting\\ISOPrintTemp.xls");
	if(CFileTools::DoesFileExist(sExlpath))
	{
		CFileDialog dlg(FALSE,_T("xls"),_T(CString(pDocData->GetProject()->name())),OFN_HIDEREADONLY,_T("Excel File(*.xls)||All Files(*.*)|*.*||"));
		if(IDOK == dlg.DoModal())
		{
			pDocData->m_Drawing_Excel_Matching.ExlPath = dlg.GetPathName().operator LPCTSTR();

			CWorkStatusDlg dlg;
			dlg.m_pThread = AfxBeginThread(StatusThreadEntry, this, THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
			if (NULL == dlg.m_pThread)
			{
				AfxMessageBox( _T("Can't create thread!!!") );
			}
			else
			{
				int res = dlg.DoModal();
				if(IDOK == res)
				{
					OnExlView();
					m_FileExist = true;

					return ERROR_SUCCESS;
				}
			}
		}
	}
	else
	{
		CString sErrMsg;
		sErrMsg.Format(_T("Can't find (%s) file") , sExlpath);
		AfxMessageBox( sErrMsg , MB_OK); 
	}

	return ERROR_BAD_ENVIRONMENT;
}

/**
	@brief	open drawing report excel file
	@author	humkyung
	@date	????.??.??
	@param
	@return
*/
void CDrawingReportForm::OnExlView()
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	const CString sExlOutputPath = pDocData->m_Drawing_Excel_Matching.ExlPath.c_str();
	
	if(CFileTools::DoesFileExist(sExlOutputPath))
	{
		::ShellExecute(NULL, _T("open") , sExlOutputPath , sExlOutputPath , NULL , SW_SHOW);
		m_FileExist = false;
	}
	else
	{
		AfxMessageBox( _T("There is no drawing report!!!") );
	}
}

/******************************************************************************
    @author     humkyung
    @date       2012-04-06
    @function   OnUpdateButtonExlView
    @return     void
    @param      CCmdUI* pCmdUI
    @brief
******************************************************************************/
void CDrawingReportForm::OnUpdateButtonExlView(CCmdUI* pCmdUI) 
{
	/*if(NULL != m_pIsoFileList)
	{
		pCmdUI->Enable(m_pIsoFileList->GetItemCount() > 1);
	}*/
}

void CDrawingReportForm::SetViewLogFileForm(CDrawingReportLogFileForm *p)
{
	assert(p && "Null Parameter");

	if(p) m_pLogFileForm = p;
}

struct ExlStruct
{
	CString ExlColumn;
	CString ExlDesc;
};

/******************************************************************************
    @author     humkyung
    @date       2012-08-15
    @function   GetExcelTemplateFromDatabase
    @return     CString
    @brief		get excel template file for Drawing Report
******************************************************************************/
CString CDrawingReportForm::GetExcelTemplateFromDatabase()
{
	CString res;

	CAppDocData* pDocData = CAppDocData::GetInstance();
	const CProject::DatabaseProp prop = pDocData->GetProject()->GetDatabaseProp();

	try
	{
		CAppPostgreSQL database(pDocData->GetProject()->name(),prop.Host.c_str(),prop.User.c_str(),prop.Password.c_str());
		{
			soci::transaction txn(*database.session());
			soci::blob b(*database.session());

			CString sSql = _T("SELECT DWG_REPORT_EXCEL_FILE_DATA FROM T_PROJECT_SETTING");
			(*database.session()) << _T("SELECT DWG_REPORT_EXCEL_FILE_DATA FROM T_PROJECT_SETTING"),into(b);
			const size_t length = b.get_len();
			if(length > 0)
			{
				TCHAR *pData = NULL;
				if(pData = (TCHAR*)calloc(1 , sizeof(TCHAR)*(length + 1)))
				{
					b.read(0,pData,sizeof(TCHAR)*length);
					res = CString(pDocData->GetProjectPath().c_str()) + _T("\\Temp\\DrawingReportTemplate.xls");
					CFile f(res  , CFile::modeCreate|CFile:: modeReadWrite);
					f.Write(pData , length);
					f.Close();

					free((void*)pData);
					pData = NULL;
				}
			}
			txn.commit();
		}
	}
	catch(const std::exception& ex)
	{
		//AfxMessageBox(ex.what(),MB_OK|MB_ICONERROR);
	}

	return res;
}

/******************************************************************************
    @author     humkyung
    @date       2012-08-31
    @class      CDrawingReportForm
    @function   ReplaceUserInput
    @return     int
    @param      CExcelAutomation&   xlApp
    @brief
******************************************************************************/
int CDrawingReportForm::ReplaceUserInput(CExcelAutomation& xlApp)
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	CProjectData* pPrjData = CProjectData::GetInstance();

	CString sValue;
	long lr = 0 , lc = 0 , ur = 0 , uc = 0;
	xlApp.GetUsedRange(lr , lc , ur , uc);
	for(long i = lr;i <= ur;++i)
	{
		for(long j = lc;j <= uc;++j)
		{
			const CString sCellIndex(xlApp.MakeExcelNoString(i , j));
			xlApp.GetCellValue(COleVariant(sCellIndex) , sValue);
			if(_T("$CLIENT") == sValue)
			{
				xlApp.SetCellValue(COleVariant(sCellIndex) , (*(pPrjData->m_pProjectSettingMap))[_T("client_name")]);
			}
			else if(_T("$PROJECT") == sValue)
			{
				xlApp.SetCellValue(COleVariant(sCellIndex) , (*(pPrjData->m_pProjectSettingMap))[_T("project_no")]);
			}
			else if(_T("$JOB") == sValue)
			{
				xlApp.SetCellValue(COleVariant(sCellIndex) , (*(pPrjData->m_pProjectSettingMap))[_T("job_no")]);
			}
			else if(_T("$DATE") == sValue)
			{
				CTime t = CTime::GetCurrentTime();
				xlApp.SetCellValue(COleVariant(sCellIndex) , t.Format(_T("%Y-%m-%d")));
			}
		}
	}

	return ERROR_SUCCESS;
}

/**	
	@brief	Drawing Report 실행 부분.
	@author	humkyung	
	@param
	@return	UINT	
*/
UINT CDrawingReportForm::StatusThread()
{
	CWorkStatusDlg* pWorkStatusDlg = CWorkStatusDlg::GetInstance();
	try
	{
		if(NULL != pWorkStatusDlg) 
			InterlockedExchange((LONG*)(&(pWorkStatusDlg->m_bThreadRunning)) , TRUE);// so the main thread knows that this thread is still running
		
		CAppDocData* pDocData = CAppDocData::GetInstance();
		CString rIniFilePath = pDocData->GetProject()->GetIniFilePath();

		TCHAR szBuf[MAX_PATH + 1]={'\0',};
		
		const CString ExlOutputPath = pDocData->m_Drawing_Excel_Matching.ExlPath.c_str();
		CString sTemplateExlFilePath = this->GetExcelTemplateFromDatabase();	/// 2012.08.15 added by humkyung
		if(sTemplateExlFilePath.IsEmpty())
		{
			sTemplateExlFilePath = CString(pDocData->GetProjectPath().c_str()) + _T("\\Setting\\ISOPrintTemp.xls");
		}
		
		CString rTitle( _T("Drawing Report...") );
		double dProgress = 0.f;

		const int ExCount = pDocData->m_Drawing_Excel_Matching.ExCount;
		int nExlStartRow = pDocData->m_Drawing_Excel_Matching.nExlStartRow , RevCount = 0;
		const int nNextColumn = pDocData->m_Drawing_Excel_Matching.nNextColumn;
		
		ExlStruct exlStruct;
		vector<STRING_T> oExlResult;
		vector<ExlStruct> ExlResultEntry;
		for(int k = 0; k < ExCount;++k)
		{
			CString rName;
			rName.Format( _T("Name%d") ,k);
			if(GetPrivateProfileString( _T("Drawing Excel Matching") ,rName, _T("") ,szBuf,MAX_PATH,rIniFilePath))
			{
				CTokenizer<CIsFromString>::Tokenize(oExlResult , szBuf , CIsFromString( _T(":") ));
				
				exlStruct.ExlColumn = oExlResult[0].c_str();
				exlStruct.ExlDesc = oExlResult[1].c_str();
				ExlResultEntry.push_back(exlStruct);
			}
		}
		
		CExcelAutomation xlApp;
		if(true == xlApp.Open(sTemplateExlFilePath))
		{
			this->ReplaceUserInput(xlApp);

			CProjectData* pProjectData = CProjectData::GetInstance();
			/// 미리 필요한 SHEET를 복사 생성한다.
			int ROW_COUNT_PER_SHEET = 30;	/// default value
			map<CString,CString>::iterator where = pProjectData->m_pProjectSettingMap->find(_T("row_count_per_sheet"));
			if(where != pProjectData->m_pProjectSettingMap->end())
			{
				if(!where->second.IsEmpty()) ROW_COUNT_PER_SHEET = ATOF_T(where->second);
			}
			int ITEM_COUNT_PER_SHEET	= ROW_COUNT_PER_SHEET*2;
			vector<LineDataRecordMap> aLineDataRecordEntry;
			this->GetSelectedLineDataRecord(aLineDataRecordEntry);
			const int nItemCount = aLineDataRecordEntry.size();
			int nSheetCount = (int)((nItemCount) / ITEM_COUNT_PER_SHEET);
			
			/// check one sheet is set - 2011.09.22 added by humkyung
			bool bOneSheet = false;
			where = pProjectData->m_pProjectSettingMap->find(_T("one_sheet_drawing_report"));
			if(where != pProjectData->m_pProjectSettingMap->end())
			{
				bOneSheet = (0 == where->second.CompareNoCase(_T("ON")));
			}
			/// up to here

			if(bOneSheet)
			{
				/// 2011.08.02 added by humkyung
				nSheetCount = 1;
				ITEM_COUNT_PER_SHEET = ROW_COUNT_PER_SHEET = nItemCount;
			}
			else
			{
				/// 1단으로만 쓴다. - 2011.09.21 added by humkyung
				if(0 == nNextColumn)
				{
					ITEM_COUNT_PER_SHEET = ROW_COUNT_PER_SHEET;
				}
				nSheetCount = (int)((nItemCount) / ITEM_COUNT_PER_SHEET);
				/// up to here

				int nRemain = ((nItemCount) % ITEM_COUNT_PER_SHEET);
				if( nRemain > 0 ) ++nSheetCount;
				for(int i = 0; i < nSheetCount - 1;++i)
				{
					CString rName;
					rName.Format(_T("Sheet %d") , i + 2);
					xlApp.CopyActiveSheet( true , rName );
				}
			}
			
			for( int nSheet = 1; nSheet <= nSheetCount;++nSheet )
			{
				xlApp.SetActiveSheet( nSheet );
				int nRow = nExlStartRow;
				int nNO = 1;
				for(int k = 0;k < ITEM_COUNT_PER_SHEET ;++k)
				{
					const int idx = (nSheet - 1) * ITEM_COUNT_PER_SHEET + k;
					if(idx >= aLineDataRecordEntry.size()) break;
					
					CLineDataRecord* pLineDataRecord = aLineDataRecordEntry[idx].pLineDataRecord;
					if(NULL == pLineDataRecord) continue;
					int FiledCount = pLineDataRecord->GetFieldCount();
					int row = nExlStartRow + k;
					if(k >= ROW_COUNT_PER_SHEET)
					{
						nNO = nNextColumn + 1;
						row = row - ROW_COUNT_PER_SHEET;
					}
					//////////////////////////////////////////////////////////////////////////
					/// BEGIN NO , LINE DATA STATUS
					
					COleVariant varColorIndex((short)3);
					bool bCancellation = false;
					CDataField* pField = pLineDataRecord->FindWithFieldName(_T("STATUS"));
					if(pField)
					{
						bCancellation = (0 == stricmp(_T("DELETED") , pField->value()));
					}

					CString num;
					num.Format( _T("%d") ,idx);
					COleVariant ExcelOutput = num, CellNum = xlApp.MakeExcelNoString(row , nNO);
					if(true == bCancellation) xlApp.SetCellTextColorWithStrike(CellNum , CellNum , varColorIndex);
					xlApp.SetCellValue(CellNum , ExcelOutput);
					
					int nLastFindWhatLen = 0;
					for(vector<ExlStruct>::iterator itr = ExlResultEntry.begin() ; itr != ExlResultEntry.end();++itr)
					{
						int nPos = ::FindStringHelper( itr->ExlDesc.operator LPCSTR() , _T("REV [0-9]+ DATE") , 0 , nLastFindWhatLen, m_rxnode, &m_rxmatch);
						if( 0 == nPos )	/// REVISION NO에 관련된 항목.
						{
							int nRevNo = 0;
							sscanf( itr->ExlDesc.operator LPCSTR() , _T("REV %d DATE") , &nRevNo );
							STRING_T rIssueDate = GetRevIssueDateAtIndex( pLineDataRecord , nRevNo );
							if(!rIssueDate.empty())
							{
								CString nFirst = itr->ExlColumn;
								CString nLast; nLast.Format( _T("%d") , row);
								if( k >= ROW_COUNT_PER_SHEET )
								{
									nFirst =SetNextColumn(nFirst,nNextColumn);
								}
								CellNum = nFirst + nLast;
								
								ExcelOutput = CString(_T("'")) + rIssueDate.c_str();
								if(true == bCancellation) xlApp.SetCellTextColorWithStrike(CellNum , CellNum , varColorIndex);								
								xlApp.SetCellValue(CellNum , ExcelOutput);
							}
						}
						else if( _T("LAST REV NO") == itr->ExlDesc )
						{
							STRING_T rIssueNo = GetRevLastIssueNo( pLineDataRecord );
							
							CString nFirst = itr->ExlColumn;
							CString nLast; nLast.Format( _T("%d") ,row);
							if( k >= ROW_COUNT_PER_SHEET )
							{
								nFirst =SetNextColumn(nFirst,nNextColumn);
							}
							CellNum = nFirst+nLast;
							
							ExcelOutput = rIssueNo.c_str();
							if(true == bCancellation) xlApp.SetCellTextColorWithStrike(CellNum , CellNum , varColorIndex);							
							xlApp.SetCellValue(CellNum , ExcelOutput);
						}
						else /// 나머지 부분.
						{
							CDataField* pDataField = pLineDataRecord->FindWithDesc(itr->ExlDesc.operator LPCSTR());
							if(NULL != pDataField)
							{
								CString value = pDataField->value();
								COleVariant rExcelValue(value);
								if(_T("LINE STATUS") == itr->ExlDesc)
								{
									if(_T("LIVE") == value) continue; 
									CString nFirst = itr->ExlColumn;
									CString nLast; nLast.Format(_T("%d") , row);
									if( k >= ROW_COUNT_PER_SHEET )
									{
										nFirst =SetNextColumn(nFirst,nNextColumn);
									}
									CellNum = nFirst+nLast;
									if(true == bCancellation) xlApp.SetCellTextColorWithStrike(CellNum , CellNum , varColorIndex);
									xlApp.SetCellValue(CellNum , rExcelValue);
								}
								else
								{
									CString nFirst = itr->ExlColumn;
									CString nLast; nLast.Format(_T("%d") , row);
									if( k >= ROW_COUNT_PER_SHEET )
									{
										nFirst = SetNextColumn(nFirst,nNextColumn);
									}
									CellNum = nFirst+nLast;
									if(true == bCancellation) xlApp.SetCellTextColorWithStrike(CellNum , CellNum , varColorIndex);
									xlApp.SetCellValue(CellNum , rExcelValue);
								}
							}
						}
					}

					//////////////////////////////////////////////////////////////////////////
					int progress = (int)(((++dProgress) / (double)(nItemCount)) * 100);
					CWorkStatusDlg::UpdateWorkStatus(rTitle , progress );
				}
				//////////////////////////////////////////////////////////////////////////
				/// END NO , LINE DATA STATUS
			}
			
			xlApp.SaveAs(ExlOutputPath);
			xlApp.Close();
		}
		
		if(NULL != pWorkStatusDlg) 
			InterlockedExchange((LONG*)(&(pWorkStatusDlg->m_bThreadRunning)) , FALSE);
		//////////////////////////////////////////////////////////////////////////
		/// close work status dialog
		if(NULL != pWorkStatusDlg)
			pWorkStatusDlg->SendMessage(WM_COMMAND , IDOK );
		
		return IDOK;
	}
	catch(const exception& ex)
	{
		AfxMessageBox( ex.what() );
	}
	catch(...)
	{
		AfxMessageBox( _T("Unknown error") );
	}
	InterlockedExchange((LONG*)(&(pWorkStatusDlg->m_bThreadRunning)) , FALSE);

	//////////////////////////////////////////////////////////////////////////
	/// close work status dialog
	if(NULL != pWorkStatusDlg)
		pWorkStatusDlg->SendMessage(WM_COMMAND , IDCANCEL );
	
	return IDCANCEL;
}

UINT CDrawingReportForm::StatusThreadEntry(LPVOID pVoid)
{
	return ((CDrawingReportForm*)pVoid)->StatusThread();
}

/**	
	@brief	line data record에서 nIndex번째 issue date를 구한다.
	@author	HumKyung
	@date	????.??.??
	@param	pLineDataRecord	a parameter of type CLineDataRecord *
	@param	nIndex	a parameter of type const int &
	@return	string	
*/
CString CDrawingReportForm::GetRevIssueDateAtIndex(CLineDataRecord *pLineDataRecord, const int &nIndex)
{
	assert( pLineDataRecord && "pLineDataRecord is NULL" );

	if( pLineDataRecord )
	{
		CProjectData* pProjectData = CProjectData::GetInstance();

		CRevDataRecordSet* pRevDataRecordSet = (CRevDataRecordSet*)pLineDataRecord->GetRevDataRecordSet();
		if( NULL != pRevDataRecordSet )
		{
			int l=0 , nRevRecordCount =  pRevDataRecordSet->GetRecordCount();
			if( nRevRecordCount <= nIndex ) return CString( _T("") );

			CRevDataRecord* pRevRecord = pRevDataRecordSet->GetRecordAt( nIndex );
			CString sIssueDate = pRevRecord->GetIssueDate();
			return pProjectData->GetDisplayIssueDateFrom(sIssueDate);
		}
	}

	return CString( _T("") );
}

/**	
	@brief	line data record에서 nIndex번째 issue no를 구한다.
	@author	humkyung
	@param	pLineDataRecord	a parameter of type CLineDataRecord *
	@return	string	
*/
string CDrawingReportForm::GetRevLastIssueNo(CLineDataRecord *pLineDataRecord )
{
	assert( pLineDataRecord && "pLineDataRecord is NULL" );

	if( pLineDataRecord )
	{
		CRevDataRecordSet* pRevDataRecordSet = (CRevDataRecordSet*)pLineDataRecord->GetRevDataRecordSet();
		if( NULL != pRevDataRecordSet )
		{
			int nRevRecordCount =  pRevDataRecordSet->GetRecordCount();
			if( nRevRecordCount < 1 ) return string("");

			CRevDataRecord* pRevRecord = pRevDataRecordSet->GetRecordAt( nRevRecordCount - 1 );
			return pRevRecord->GetRevNo();
		}
	}

	return string("");
}

CString CDrawingReportForm::SetNextColumn(CString nFirst, UINT nCol)
{
	CString res;
    long nFirstCol = CAppDocData::FieldName2Long( nFirst );
    long nNextCol = nFirstCol + nCol;

    CString rString;
    do
	{
		int nDenom = nNextCol % 26;
		
		rString.Format( "%c" , 'A' + nDenom - 1);
		
		res = rString + res;
		
		nNextCol /= 26;
		
	}while( nNextCol > 0);

	return res;
}

/**
	@brief	RECORD의 COUNT를 화면에 표기한다. show error message when valid record count is not equal to record count.
	@author	humkyung
*/
void CDrawingReportForm::ShowRecordCount(const int& nValidRecordCount , const int& nRecordCount)
{
	m_nRecordCount = nRecordCount;
	CWnd* pWnd = GetDlgItem(IDC_STATIC_SELECTED_COUNT);
	if(pWnd)
	{
		CString rCount;
		rCount.Format(_T("%d / %d") , nValidRecordCount , m_nRecordCount);
		pWnd->SetWindowText((LPCTSTR)rCount);
	}

	if(nValidRecordCount != m_nRecordCount)
	{
		AfxMessageBox( _T("Count of PRODUCT file is different to line list number.") );
	}
}

/**	
	@brief	The CBMReportForm::GetSelectedLineDataRecord function
	@param	IsoEditItemEntry	a parameter of type vector<long>&
	@return	void	
*/
void CDrawingReportForm::GetSelectedLineDataRecord(vector<LineDataRecordMap>& LineDataRecordEntry)
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

/******************************************************************************
    @author     humkyung
    @date       2012-04-06
    @function   OnUpdateButtonPrint
    @return     void
    @param      CCmdUI* pCmdUI
    @brief
******************************************************************************/
void CDrawingReportForm::OnUpdateButtonPrint(CCmdUI* pCmdUI) 
{
	if(m_wndLineDataReport.GetSafeHwnd())
	{
		pCmdUI->Enable(m_wndLineDataReport.GetItemCount() > 1);
	}
}

/******************************************************************************
    @author     humkyung
    @date       2012-04-06
    @function   PreTranslateMessage
    @return     BOOL
    @param      MSG*    pMsg
    @brief
******************************************************************************/
BOOL CDrawingReportForm::PreTranslateMessage(MSG* pMsg)
{
	UpdateDialogControls(this, TRUE);

	return CFormView::PreTranslateMessage(pMsg);
}

/******************************************************************************
    @author     humkyung
    @date       2012-05-24
    @class      CDrawingReportForm
    @function   OnHelpInfo
    @return     BOOL
    @param      HELPINFO*   pHelpInfo
    @brief
******************************************************************************/
BOOL CDrawingReportForm::OnHelpInfo(HELPINFO* pHelpInfo)
{
	HWND hHtml = ::HtmlHelp(this->m_hWnd , CAppDocData::GetExecPath() + _T("\\Help\\SmartISO.chm::/Drawing Report.htm") , HH_DISPLAY_TOPIC , 0);

	return CFormView::OnHelpInfo(pHelpInfo);
}
