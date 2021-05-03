// LineListDataImportDlg.cpp : implementation file
//

#include "stdafx.h"
#include <math.h>
#include "admin.h"
#include "excel9.h"
#include "LineListDataImportDlg.h"
#include "enablebuddybutton.h"
#include <LineListConverter.h>
#include "AdminDoc.h"
#include "AdminDocData.h"
#include "WorkStatusDlg.h"
#include "Tokenizer.h"
#include "FileNameFormatDlg.h"
#include <Splash/SplashScreenFx.h>

#include <ado/SqlFile.h>
#include <ProjectData.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLineListDataImportDlg

IMPLEMENT_DYNCREATE(CLineListDataImportDlg, CFormView)

CLineListDataImportDlg::CLineListDataImportDlg()
	: CFormView(CLineListDataImportDlg::IDD)
{
	//{{AFX_DATA_INIT(CLineListDataImportDlg)
	
	//}}AFX_DATA_INIT
}

CLineListDataImportDlg::~CLineListDataImportDlg()
{
}

void CLineListDataImportDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLineListDataImportDlg)
	DDX_Control(pDX , IDC_LIST_SHEET_NAME , m_wndSheetNameListCtrl);
	DDX_Control(pDX , IDC_EDIT_RowNum , m_wndStartRowEdit);
	DDX_Control(pDX , IDC_LIST_CURRENT_LINE_DATA , m_wndCurrentLineDataListCtrl);

	DDX_Control(pDX , IDC_EDIT_DATABASE_REFERENCE_KEY , m_wndDatabaseReferenceKeyEdit);
	DDX_Control(pDX , IDC_BUTTON_DATABASE_REFERENCE_KEY , m_btnDatabaseReferenceKey);
	DDX_Text(pDX , IDC_EDIT_DATABASE_REFERENCE_KEY  , m_rDatabaseReferenceKeyFormat);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLineListDataImportDlg, CFormView)
	//{{AFX_MSG_MAP(CLineListDataImportDlg)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_SelectExcel, OnBUTTONSelectExcel)
	ON_BN_CLICKED(IDC_BUTTON_LINE_MAN_DATA_IMPORT, OnButtonLineManDataImport)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_CURRENT_LINE_DATA, OnKeydownListCurrentLineData)
	//}}AFX_MSG_MAP
	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDC_BUTTON_DATABASE_REFERENCE_KEY, &CLineListDataImportDlg::OnBnClickedButtonDatabaseReferenceKey)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLineListDataImportDlg diagnostics

#ifdef _DEBUG
void CLineListDataImportDlg::AssertValid() const
{
	CFormView::AssertValid();
}

void CLineListDataImportDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLineListDataImportDlg message handlers
BOOL CLineListDataImportDlg::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	CAdminApp* pApp = (CAdminApp*)AfxGetApp();
	///CResourceSwitch resSwitch(pApp->m_hMultiLangRes);

	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

/******************************************************************************
    @author     humkyung
    @date       ????-??-??
    @class      CLineListDataImportDlg
    @function   OnInitialUpdate
    @return     void
    @brief
******************************************************************************/
void CLineListDataImportDlg::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	
	static bool __init = false;
	if(false == __init)
	{
		CAdminDocData* pDocData = CAdminDocData::GetInstance();

		m_Font.CreateFont(18,0,0,0 ,FW_BOLD,FALSE,FALSE,0,0,0,0,0,0, _T("Arial"));
		m_16UnderlineFont.CreateFont(16,0,0,0 ,FW_NORMAL ,FALSE , TRUE ,0,0,0,0,0,0, _T("Arial"));
		
		/// 2012.08.03 added by humkyung
		{
			short shBtnColor = 30;
			m_btnDatabaseReferenceKey.SetGradientColor(DarkenColor(GetSysColor(COLOR_BTNFACE),0.15) , GetSysColor(COLOR_BTNFACE) , CButtonST::VertBar);
			m_btnDatabaseReferenceKey.SetBtnCursor(IDC_HAND_CUR);
			m_btnDatabaseReferenceKey.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
			m_btnDatabaseReferenceKey.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(0, 128, 0));
			m_btnDatabaseReferenceKey.SetFlat(FALSE);
			HWND hwndEdit  = GetDlgItem(IDC_EDIT_DATABASE_REFERENCE_KEY)->GetSafeHwnd();
			HWND hwndButton= GetDlgItem(IDC_BUTTON_DATABASE_REFERENCE_KEY)->GetSafeHwnd();
			VERIFY( ::EnableBuddyButton( hwndEdit, hwndButton, BBS_LEFT ) );
			m_wndDatabaseReferenceKeyEdit.SetFont(&m_Font);
		}
		/// up to here

		CWnd* pWnd = GetDlgItem(IDC_STATIC_1);
		pWnd->SetFont(&m_Font);
		
		COLORREF crBtnColor = ::GetSysColor(COLOR_BTNFACE) + RGB(30, 30, 30);
		
		HWND hwndEdit  = GetDlgItem(IDC_STATIC_ShowExcelName)->GetSafeHwnd();
		m_selectExcelButton.SubclassWindow(GetDlgItem(IDC_BUTTON_SelectExcel)->m_hWnd);
		VERIFY( ::EnableBuddyButton( hwndEdit, m_selectExcelButton.m_hWnd , BBS_RIGHT ) );
		
		m_wndStartRowEdit.SetRegEx( _T("[0-9]*") );
		m_wndStartRowEdit.SetBackgroundColourFocus (RGB (150, 200, 255));
		m_wndStartRowEdit.SetForegroundColourError (RGB (255, 255, 255));
		m_wndStartRowEdit.SetBackgroundColourError (RGB (255, 0, 0));
		m_wndStartRowEdit.CreateToolTip(this , _T("Number only"));
				
		pWnd = (CWnd*)GetDlgItem(IDC_LIST_SHEET_NAME);
		m_wndSheetNameListCtrl.SetHeadings( _T("Sheet Name, 183") );
		m_wndSheetNameListCtrl.SetCheckboxes( TRUE );
		m_wndSheetNameListCtrl.SetHeaderImageList();
		
		m_selectAllButton.SubclassWindow(GetDlgItem(IDC_BUTTON_SELECT_ALL)->m_hWnd);
		m_selectAllButton.SetIcon(IDI_SELECT_ALL/* , 16 , 16*/);
		m_unselectAllButton.SubclassWindow(GetDlgItem(IDC_BUTTON_UNSELECT_ALL)->m_hWnd);
		m_unselectAllButton.SetIcon(IDI_UNSELECT_ALL/* , 16 , 16*/);
		
		m_wndStartRowEdit.SetFont(&m_Font);
		
		m_convertButton.SubclassWindow(GetDlgItem(IDC_BUTTON_LINE_MAN_DATA_IMPORT)->m_hWnd);
		m_convertButton.SetIcon(IDI_OK/* , 32 , 32*/);
		m_convertButton.SetFont(&m_Font);

		BOOL b = m_GridCtrl.Create(CRect(0,0,0,0),this,0x100);
		if(TRUE == b)
		{
			//그리드 컨트롤 폰트 코드
			CFont font;
			font.CreateFont(-11,0,0,0,400,FALSE,FALSE,FALSE,HANGUL_CHARSET,3,2,1,VARIABLE_PITCH | FF_MODERN , _T("Arial")); 
			m_GridCtrl.SetFont(&font);
			font.DeleteObject();
			
			m_ImageList.Create(IDB_ADMIN_TREE , 16 , 16 , RGB(255 , 255 , 255));
			m_GridCtrl.SetImageList(&m_ImageList);
			// fill it up with stuff
			m_GridCtrl.SetEditable(TRUE);
			m_GridCtrl.EnableDragAndDrop(FALSE);
			m_GridCtrl.SetTextBkColor(RGB(255, 255, 224));
			m_GridCtrl.SetRowResize(FALSE);
			m_GridCtrl.SetColumnResize(TRUE);
			m_GridCtrl.SetListMode(FALSE);
			m_GridCtrl.SetSingleRowSelection(TRUE);
			m_GridCtrl.SetHeaderSort(FALSE);
			m_GridCtrl.SetSingleColSelection(TRUE);
			/////////////////////////////////////////////////////////////////

			try 
			{
				m_GridCtrl.SetRowCount(3);
				///m_GridCtrl.SetColumnCount(nCount + 2);
				m_GridCtrl.SetFixedRowCount(1);
				m_GridCtrl.SetFixedColumnCount(1);
			}
			catch (CMemoryException* e)
			{
				e->ReportError();
				e->Delete();
				///return FALSE;
			}

			//m_GridCtrl.SetItemFormat(1, 0, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_GridCtrl.SetItemImage(1 , 0 , 2);	/// access image
			m_GridCtrl.SetItemImage(2 , 0 , 6);	/// excel image
						
			m_GridCtrl.AutoSizeRows();	
			m_GridCtrl.AutoSizeColumns();
		}
		__init = true;
	}
}

/**
	@brief

	@author	HumKyung.BAEK

	@date	2010.03.08

	@return
*/
void CLineListDataImportDlg::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	CWnd* pWnd = (CWnd*)GetDlgItem(IDC_LIST_CURRENT_LINE_DATA);
	if(pWnd)
	{
		CRect rect;
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);
		pWnd->SetWindowPos(NULL , rect.left , rect.top , rect.Width() , cy - rect.top - 5 , SWP_NOZORDER | SWP_NOMOVE);
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);

		if(m_GridCtrl.GetSafeHwnd())
			m_GridCtrl.SetWindowPos(NULL , rect.left , rect.bottom + 25 , rect.Width() , cy - rect.bottom - 25, SWP_NOZORDER);
	}
}

/**
	@brief	select line man data excel file

	@author	HumKyung.BAEK

	@date	2010.03.08

	@return
*/
void CLineListDataImportDlg::OnBUTTONSelectExcel() 
{
	CFileDialog dlg(TRUE,_T(""),_T("*.xls"),OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,
		_T("Excel 문서(*.xls)||모든 문서(*.*)|*.*||")); 
	if(IDOK == dlg.DoModal())
	{
		S_ExPath = dlg.GetPathName();
		SetDlgItemText(IDC_STATIC_ShowExcelName, (LPSTR)(LPCTSTR)S_ExPath);

		LoadSheetName();	//! 2011.02.14 added by HumKyung
	}
}

/**
	@brief	load sheet name from excel file.

	@author	HumKyung.BAEK

	@date	2010.03.08

	@return
*/
int CLineListDataImportDlg::LoadSheetName() 
{
	if(!S_ExPath.IsEmpty())
	{
		try
		{	
			GetDlgItemText(IDC_STATIC_ShowExcelName , m_rExcelFilePath);
			///m_loadSheetNameButton.EnableWindow(FALSE);
			
			BeginWaitCursor();
			
			CLineListConverter ExGetname;
			CStringArray* pSheetNameArray = ExGetname.GetExcelName(m_rExcelFilePath);
			
			m_wndSheetNameListCtrl.SetRedraw(FALSE);
			m_wndSheetNameListCtrl.DeleteAllItems();
			int nSize = pSheetNameArray->GetSize();
			for(int i = 0;i < nSize;i++)
			{
				m_wndSheetNameListCtrl.InsertItem(0 , pSheetNameArray->GetAt(i));
			}
			m_wndSheetNameListCtrl.SetRedraw(TRUE);
			
			EndWaitCursor();
			
			///m_loadSheetNameButton.EnableWindow(TRUE);
		}
		catch(...)
		{
			///m_loadSheetNameButton.EnableWindow(TRUE);
		}

		return ERROR_SUCCESS;
	}
	else
	{
		AfxMessageBox( _T("Excel File을 선택하세요") );
	}

	return ERROR_BAD_ENVIRONMENT;
}

/**
	@brief	display line man data mapping table
	@author	HumKyung.BAEK
	@date	2010.03.09
	@return
*/
BOOL CLineListDataImportDlg::UpdateContents()
{
	CAdminDoc* pDoc = CAdminDocData::GetInstance()->GetDocument();
	if(pDoc)
	{
		CAdminDocData* pDocData = CAdminDocData::GetInstance();
		const CString rIniFilePath = pDocData->GetIniFilePath();

		/// 2012.08.03 added by humkyung
		CProjectData* pProjectData = CProjectData::GetInstance();
		if(NULL != pProjectData)
		{
			map<CString,CString>::const_iterator where = pProjectData->m_pProjectSettingMap->find(_T("DATABASE_REFERENCE_KEY"));
			if(where != pProjectData->m_pProjectSettingMap->end())
			{
				m_rDatabaseReferenceKeyFormat = where->second;
				m_wndDatabaseReferenceKeyEdit.SetWindowText(m_rDatabaseReferenceKeyFormat);
			}
		}
		/// up to here

		const UINT iStartRow = GetPrivateProfileInt(_T("RowNum") , _T("Rnum") , 2 , rIniFilePath);
		CString rString;
		rString.Format(_T("%d") , iStartRow);
		m_wndStartRowEdit.SetWindowText(rString);

		const CProject::DatabaseProp prop = pDoc->GetCurProject()->GetDatabaseProp();
		try
		{
			CAppPostgreSQL database(pDoc->GetCurProject()->name(),prop.Host.c_str(),prop.User.c_str(),prop.Password.c_str());
			DisplayCurrentLineData(database);
			
			///
			CString rExecPathName = CAdminDocData::GetExecPath();
			const CString rSqlFilePath = rExecPathName + _T("\\Setting\\T_LINE_LIST_DATA_MAPPING.XML");
			
			CStringList ColumnNameList;

			CSQLFile sqlFile(rSqlFilePath.operator LPCSTR());
			if(ERROR_SUCCESS != sqlFile.ParseXML(rSqlFilePath.operator LPCTSTR())) return FALSE;
			for(int i = 0;i < sqlFile.GetColumnCount();++i)
			{
				const CString rColunmName = sqlFile.GetFieldNameAt(i).c_str();
				ColumnNameList.AddTail(rColunmName);
			}

			int nIndex = 1;
			m_GridCtrl.SetColumnCount(ColumnNameList.GetCount() + 1);

			int i = 0;
			try
			{
				soci::rowset<soci::row> rs(database.session()->prepare << _T("SELECT * FROM T_LINE_LIST_DATA_MAPPING"));

				nIndex = 0;
				LONG lFieldNo = 0L;

				STRING_T rValue;
				POSITION pos = ColumnNameList.GetHeadPosition();
				while(pos)
				{
					const CString rColName = ColumnNameList.GetNext(pos);
					rValue = (soci::i_ok == rs.begin()->get_indicator(rColName.operator LPCTSTR())) ? rs.begin()->get<STRING_T>(rColName.operator LPCTSTR()) : _T("");
					if(0 == i)
					{
						CString rString;
						rString.Format("%d" , lFieldNo + 1);
						m_GridCtrl.SetItemText(0 , nIndex + 1 , rString);
						m_GridCtrl.SetItemText(1 , nIndex + 1 , rColName);
						
						CGridCellBase* pCell = m_GridCtrl.GetCell(1 , nIndex + 1);
						if(pCell) pCell->SetState( GVNI_READONLY );

						lFieldNo++;
					}
					m_GridCtrl.SetItemText(2 , nIndex + 1 , rValue.c_str());
					
					++nIndex;
				}
			}
			catch(const std::exception& ex)
			{
				AfxMessageBox(ex.what(),MB_OK|MB_ICONERROR);
			}

			m_GridCtrl.AutoSizeRow(0);
			m_GridCtrl.AutoSizeColumns();
		}
		catch(const std::exception& ex)
		{
			AfxMessageBox(ex.what(),MB_OK|MB_ICONERROR);
		}
	}

	return TRUE;
}

UINT CLineListDataImportDlg::StatusThreadEntry(LPVOID pVoid)
{
	return ((CLineListDataImportDlg*)pVoid)->StatusThread();
}

/**	
	@brief	쓰레드 실행부

	@author	HumKyung.BAEK

	@date	2010.03.09

	@return	UINT
*/
UINT CLineListDataImportDlg::StatusThread()
{
	CWorkStatusDlg* pWorkStatusDlg = CWorkStatusDlg::GetInstance();
	try
	{
		InterlockedExchange((LONG*)(&(pWorkStatusDlg->m_bThreadRunning)) , TRUE);///< so the main thread knows that this thread is still running

		CString rIniFilePath = CAdminDocData::GetInstance()->GetIniFilePath();
		int res = ImportLineDataFile(S_ExPath , rIniFilePath);
		
		InterlockedExchange((LONG*)(&(pWorkStatusDlg->m_bThreadRunning)) , FALSE);
		
		//////////////////////////////////////////////////////////////////////////
		/// close work status dialog
		pWorkStatusDlg->SendMessage(WM_COMMAND , IDOK );

		return IDOK;
	}
	catch(...)
	{
		AfxMessageBox("Unknown error");

		InterlockedExchange((LONG*)(&(pWorkStatusDlg->m_bThreadRunning)) , FALSE);
		
		//////////////////////////////////////////////////////////////////////////
		/// close work status dialog
		pWorkStatusDlg->SendMessage(WM_COMMAND , IDOK );
	}

	return IDCANCEL;
}

/**
	@brief	
	@author	HumKyung.BAEK
	@date	2010.03.09
	@return
*/
void CLineListDataImportDlg::OnButtonLineManDataImport() 
{
	CAdminDoc* pDoc = CAdminDocData::GetInstance()->GetDocument();

	if(pDoc && (m_wndSheetNameListCtrl.GetItemCount() > 0))
	{
		CAdminDocData* pDocData = CAdminDocData::GetInstance();

		CProjectData* pProjectData = CProjectData::GetInstance();
		const CString rRefKey = (*(pProjectData->m_pProjectSettingMap))[_T("DATABASE_REFERENCE_KEY")];
		if(rRefKey.IsEmpty())
		{
			AfxMessageBox(_T("First of all, select reference key."));
			return;
		}

		CString rIniFilePath = pDocData->GetIniFilePath();

		CString CC,jj,ExName,InputNum;
		CEdit* pEdit= (CEdit *)GetDlgItem(IDC_EDIT_RowNum);
		if(pEdit) pEdit->GetWindowText(InputNum);
		
		int checkNum = m_wndSheetNameListCtrl.GetCheckedItemCount();
		int nItemCount = m_wndSheetNameListCtrl.GetItemCount();
		int j=0;
		
		for(int i = 0;i < nItemCount;++i)
		{
			BOOL b = m_wndSheetNameListCtrl.GetCheck(i);
			if(b)
			{
				ExName = m_wndSheetNameListCtrl.GetItemText(i,0);
				CC.Format( _T("Name%d") , j);
				WritePrivateProfileString(_T("Excel Exception") , CC , ExName , rIniFilePath);
				j++;
			}
		}
		jj.Format("%d",j);
		WritePrivateProfileString( _T("Excel Exception") , _T("Count") ,jj,rIniFilePath);
		WritePrivateProfileString( _T("RowNum") , _T("Rnum") ,InputNum,rIniFilePath);

		//////////////////////////////////////////////////////////////////////////
		CWorkStatusDlg dlg;
		if(InputNum != "")
		{
			dlg.m_pThread = AfxBeginThread(StatusThreadEntry, this, THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
			
			if (NULL == dlg.m_pThread)
			{
				AfxMessageBox("Can't create thread!!!");
			}
			else
			{
				dlg.DoModal();
				
				try
				{
					const CProject::DatabaseProp prop = pDoc->GetCurProject()->GetDatabaseProp();
					CAppPostgreSQL database(pDoc->GetCurProject()->name(),prop.Host.c_str(),prop.User.c_str(),prop.Password.c_str());
					DisplayCurrentLineData(database);
				}
				catch(const std::exception& ex)
				{
					AfxMessageBox(ex.what(),MB_OK|MB_ICONERROR);
				}
			}
		}
		else
		{
			AfxMessageBox("Excel Convert의 시작 행을 입력하세요");
		}
	}
	else
	{
		AfxMessageBox("Excel Sheet를 선택해 주세요");
	}
}

CString CLineListDataImportDlg::GetCellString(VARIANT &valCell)
{
	CString strCell;
	switch (V_VT(&valCell))
	{
		case VT_I2:
			strCell.Format("%d", (V_I2(&valCell)));
		break;
		case VT_I4:
			strCell.Format("%d", (V_I4(&valCell)));
		break;
		case VT_R4:
		{
			strCell.Format("%f", (V_R4(&valCell)));
			int at = strCell.Find(".");
			if(-1 != at)
			{
				CString rIntValue = strCell.Left(at);
				CString rDigiValue= strCell.Right(strCell.GetLength() - at - 1);
										
				string strDigiValue(rDigiValue.operator const char*());
				string::size_type it = strDigiValue.find_last_not_of("0");
				if((string::npos == it))
				{
					rDigiValue = "";
					strCell = rIntValue;
				}
				else
				{
					rDigiValue = strDigiValue.substr(0,it+1).c_str();
					strCell = rIntValue + "." + rDigiValue;
				}
			}
		}
		break;
		case VT_R8:
		{
			strCell.Format("%f", (V_R8(&valCell)));
			int at = strCell.Find(".");
			if(-1 != at)
			{
				CString rIntValue = strCell.Left(at);
				CString rDigiValue= strCell.Right(strCell.GetLength() - at - 1);
				
				string strDigiValue(rDigiValue.operator const char*());
				string::size_type it = strDigiValue.find_last_not_of("0");
				if((string::npos == it))
				{
					rDigiValue = "";
					strCell = rIntValue;
				}
				else
				{
					rDigiValue = strDigiValue.substr(0,it+1).c_str();
					strCell = rIntValue + "." + rDigiValue;
				}
			}
		}
		break;
		case VT_BSTR:
			strCell = (CString)V_BSTR(&valCell);
		break;
		case VT_BOOL:
			if (V_BOOL(&valCell)) 
				strCell = "true";
			else 
				strCell = "false";
		break;
		case VT_EMPTY:
			strCell = "-";
		break;
	}

	return strCell;
}

long CLineListDataImportDlg::FieldName2Long(CString &rFieldName)
{										
	/*
	long ret = 0L;								
	for(int i = rFieldName.GetLength() - 1;i >= 0 ;i--)			
	{									
		ret += (rFieldName[i] - 'A' + 1) + i * 26;			
	}									
	return ret;
	*/

	long ret = 0L;								
	for(int i = 0 ;i < rFieldName.GetLength() ;++i)
	{									
		ret += (rFieldName[i] - 'A' + 1) * long(pow(26.0 , double(rFieldName.GetLength() - i - 1)));
	}

	return (ret);
}

/******************************************************************************
    @author     humkyung
    @date       2012-08-23
    @class      CLineListDataImportDlg
    @function   GetLineDataMappingData
    @return     int
    @param      map<CString         CString>&
    @param      oMappingDataEntry   CADODB&
    @param      adoDB
    @brief		read line data mapping data
******************************************************************************/
int CLineListDataImportDlg::GetLineDataMappingData(map<CString,CString>& oMappingDataEntry , CADODB& adoDB)
{
	adoDB.OpenQuery(_T("SELECT * FROM T_LINE_LIST_DATA_MAPPING"));

	LONG lRecordCount = 0L;
	adoDB.GetRecordCount(&lRecordCount);
	if(1 != lRecordCount) return ERROR_BAD_ENVIRONMENT;

	for(int i = 0;i < adoDB.GetFieldCount();++i)
	{
		STRING_T rValue;
		STRING_T sName = adoDB.GetFieldName(i);
		adoDB.GetFieldValue(0 , sName , &rValue);
		oMappingDataEntry.insert(make_pair(sName.c_str() , rValue.c_str()));
	}

	return ERROR_SUCCESS;
}

namespace
{

	struct IsFieldName : public binary_function<CADODB::FieldInfo , STRING_T , bool>
	{
		bool operator ()(const CADODB::FieldInfo& lhs , const STRING_T& name) const
		{
			return (lhs._name == name);
		}
	};
}

/**
	@brief	import line data from excel file

	@author	HumKyung.BAEK

	@date	2010.03.09

	@return
*/
int CLineListDataImportDlg::ImportLineDataFile(const CString rExlFilePath , const CString& rIniFilePath)
{
	_ConnectionPtr m_pCON;
	_ConnectionPtr m_pTempCON;

	CoInitialize(NULL);  // COM관련 개체 초기화 
	
	TCHAR szCount[3]={0,};
	TCHAR szExcel[MAX_PATH + 1]={0,};
	TCHAR szName[MAX_PATH + 1]={0,};
	CString AA,BB,CC;
	CString m_strName,m_strExcel;
	
	CAdminDocData* pDocData = CAdminDocData::GetInstance();
	CAdminDoc* pDoc = pDocData->GetDocument();
	if(pDoc)
	{
		CProjectData* pProjectData = CProjectData::GetInstance();

		const CString rRefKey = (*(pProjectData->m_pProjectSettingMap))[_T("C_DATABASE_REFERENCE_KEY")];
		vector<STRING_T> oRefKey;
		CTokenizer<CIsFromString>::Tokenize(oRefKey , rRefKey.operator LPCTSTR() , CIsFromString(_T(" ~ ")));

		CADODB adoDB;
		CString rConnectionString = CString("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=") + pDoc->m_ProjectPath + CString("\\Database\\") + pDoc->m_ProjectName + CString(_T(".MDB"));
		rConnectionString += _T(";Jet OLEDB:Database Password=gopds");	/// 2012.01.27 added by humkyung
		if(TRUE == adoDB.DBConnect(rConnectionString.operator LPCTSTR()))
		{
			/// 현재의 T_LINE_LIST_DATA를 삭제한다.
			adoDB.ExecuteQuery(_T("DELETE * FROM T_LINE_LIST_DATA"));

			/// Database에서 Mapping정보를 받아온다.
			map<CString,CString> oMappingDataEntry , MappingValueEntry;
			GetLineDataMappingData(oMappingDataEntry , adoDB);
			vector<CADODB::FieldInfo> oLineListDataFieldInfoList;
			adoDB.GetFieldInfoListOfTable(oLineListDataFieldInfoList , _T("T_LINE_LIST_DATA"));
			
			m_pCON = NULL; /// 연결 포인터
			
			_Application app;  
			_Workbook book;
			Workbooks books;
			Worksheets sheets;
			_Worksheet sheet;
			Range range; 
			
			COleVariant
				coVTrue((short)TRUE),
				coVFalse((short)FALSE),
				covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
			
			m_pCON.CreateInstance(__uuidof(Connection));	
			if(!app.CreateDispatch( _T("Excel.Application") ))
			{
				AfxMessageBox(_T("Excel Connection Failure"));
			}
			else
			{
				CWorkStatusDlg* pWorkStatusDlg = CWorkStatusDlg::GetInstance();

				// Excel file 설정
				LPDISPATCH lpdisp;
				lpdisp=app.GetWorkbooks();
				ASSERT(lpdisp);
				books.AttachDispatch(lpdisp);
				book=books.Open(rExlFilePath , covOptional, covOptional, covOptional,covOptional,covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional);
				sheets=book.GetWorksheets();
				long l = 0L;
				long lNumRows;
				long lNumCols;
				long lowr,lowc,r;
				long pos[2];
				TCHAR rowNum[4]={0,};
				TCHAR ExCount[3]={0,};
				TCHAR ExName[MAX_PATH]={0,};
				int ExNum;
				CString ExN[MAX_PATH];
				
				/// Excel Exception Sheet 정보 read & save
				GetPrivateProfileString( _T("Excel Exception") , _T("Count") , _T("") , ExCount,sizeof(ExCount), rIniFilePath);	//
				ExNum = atoi(ExCount);																	   //
				for(int a=0;a<ExNum;a++)																   //
				{																						   //
					CC.Format( _T("Name%d") ,a);																   //
					GetPrivateProfileString(_T("Excel Exception") , CC , _T("") , ExName,MAX_PATH, rIniFilePath);       //
					ExN[a]=ExName;																		   //
				}																						   //
				///
				
				map<CString, list<map<STRING_T,STRING_T> > > ErrorCheckMap;

				CString rTitle( _T("Convert Line List...") );
				double dProcess = 0.;
				
				//! 시작 날짜를 구한다.
				SYSTEMTIME sysTime;
				GetLocalTime(&sysTime);
				
				CString rCurDate;
				rCurDate.Format( _T("%d-%d-%d") , sysTime.wYear , sysTime.wMonth , sysTime.wDay );
				
				bool bFoundError = false;
				//******************* 실제 로직 부분 *******************
				const long nSheetCount = sheets.GetCount();
				for(l = 0;l < nSheetCount;l++)
				{		
					sheet=sheets.GetItem(COleVariant((short)(l + 1)));
					CString rSheetName = sheet.GetName();
					long lVisible = sheet.GetVisible();
					if(-1 != lVisible) continue;
					
					/// Excel Exception sheet 처리
					bool bProcessSheetName = false;
					for(int i = 0;i < ExNum;i++)
					{
						if(ExN[i] == rSheetName)
						{
							bProcessSheetName = true;
							break;
						}
					}
					if(false == bProcessSheetName) continue;
					
					/// Selected Sheet내의 Range 설정
					range = sheet.GetUsedRange();
					VARIANT varValue = range.GetValue();
					
					COleSafeArray sa;
					sa.Attach(varValue);
					sa.GetUBound(1, &lNumRows);
					sa.GetUBound(2, &lNumCols);
					sa.GetLBound(1, &lowr);
					sa.GetLBound(2, &lowc);
					VARIANT valCell;
					CString strCell;
					CString m_rValues;
					
					/// Data가 있는 행의 자료 값을 ini로 부터 read
					lowr = GetPrivateProfileInt(_T("RowNum") , _T("Rnum") ,  2 , rIniFilePath);
					/// up to here
					
					//****************** [lowr,0]부터 [최종Row,size]까지 Cells Value를 DB에 Input
					for(r= lowr; r <= lNumRows; ++r)
					{
						MappingValueEntry.clear();

						CString rColumns , rValues;
						bool bExistData = false;	/// 실제적으로 데이타가 있는지?
						for(map<CString,CString>::iterator itr = oMappingDataEntry.begin();itr != oMappingDataEntry.end();++itr)
						{
							/// check if column is in line data table - 2012.08.23 added by humkyung
							if(oLineListDataFieldInfoList.end() == find_if(oLineListDataFieldInfoList.begin() , oLineListDataFieldInfoList.end() , bind2nd(IsFieldName() , itr->first.operator LPCTSTR())))
							{
								continue;
							}
							/// up to here

							strCell.Empty();
							if( !(itr->second.IsEmpty()) )	/// excel column
							{
								pos[0] = r;
								pos[1] = FieldName2Long( itr->second );
								try
								{
									sa.GetElement(pos, &valCell);
									CString rValue = GetCellString( valCell );
									if(strCell.IsEmpty())
										strCell += GetCellString( valCell );
									else if(_T("-") != rValue)
										strCell += GetCellString( valCell );
								}
								catch(...)
								{
									if(strCell.IsEmpty()) strCell += _T("-");
								}
								
								if(!strCell.IsEmpty() && (_T("-") != strCell)) bExistData = true;
							}
							MappingValueEntry.insert(make_pair(itr->first , strCell));

							if ( strCell.IsEmpty() ) strCell = _T("-");
							{
								strCell.Replace(_T("'") , _T("''"));	///< DB query Error 방지위해 Value중 ' -> ''로 바꾼다.
								rColumns+= itr->first + _T(",");
								rValues += _T("'") + strCell + _T("',");
							}
						}
						
						/// check error if there are no value key
						CString rKey , rString;
						map<STRING_T,STRING_T> DataMap;
						rString.Format(_T("%d") , r);
						DataMap[_T("ANO")] = rString.operator LPCTSTR();
						for(vector<STRING_T>::iterator jtr = oRefKey.begin();jtr != oRefKey.end();++jtr)
						{
							if(jtr->empty()) continue;
							map<CString,CString>::iterator where = MappingValueEntry.find(jtr->c_str());
							if(where == MappingValueEntry.end())
							{
								///rKey += where->second;
								DataMap.insert(make_pair(jtr->c_str() , STRING_T(STRING_T(_T("no ")) + *jtr).c_str()));
							}
						}
						ErrorCheckMap[rString].push_back(DataMap);
						///
						
						//! DB Query 부분 DB에 저장
						if (bExistData)
						{
							if(_T(",") == rColumns.Right(1)) rColumns = rColumns.Left(rColumns.GetLength() - 1);
							if(_T(",") == rValues.Right(1))  rValues  = rValues.Left(rValues.GetLength() - 1);

							CString rSql(_T("INSERT INTO T_LINE_LIST_DATA(") + rColumns + _T(") VALUES(") + rValues + _T(")"));
							adoDB.ExecuteQuery(rSql.operator LPCTSTR());
						}

						dProcess = r;
						if(NULL != pWorkStatusDlg)
						{
							int process = (int)((dProcess / lNumRows) * 100.);
							pWorkStatusDlg->UpdateWorkStatus(rTitle , process);
						}
					}
				}

				//! write and open log file.
				bool bErrorHappened = false , onoff = true;
				::DeleteFile(CString(CAdminDocData::GetExecPath() + _T("\\log.html")));
				const CString rLogFilePath = CString(CAdminDocData::GetExecPath() + _T("\\log.html")) , colors[]={"gray" , "white"};
				ofstream ofile(rLogFilePath.operator LPCTSTR());
				appendHeader(ofile);
				for(map<CString, list<map<STRING_T,STRING_T> > >::iterator ktr = ErrorCheckMap.begin();ktr != ErrorCheckMap.end();++ktr)
				{
					if(ktr->second.size() > 1)
					{
						for(list<map<STRING_T,STRING_T> >::iterator jtr = ktr->second.begin();jtr != ktr->second.end();++jtr)
						{
							ofile << std::endl << _T("<tr bgcolor=") << colors[onoff].operator LPCTSTR() << _T(">") << std::endl;
							ofile << _T("<td title=\"") << _T("ANO") << _T("\">");
							ofile << _T("-") << _T("</td>") << std::endl;
							for(map<string,string>::iterator ltr = jtr->begin();ltr != jtr->end();++ltr)
							{	
								ofile << _T("<td title=\"") << ltr->first.c_str() << _T("\">");
								ofile << ltr->second.c_str() << _T("</td>") << std::endl;
							}
							ofile << _T("</tr>") << std::endl;
						}
						bErrorHappened = true;
						onoff = !onoff;
					}
				}
				if(bErrorHappened)
				{
					::ShellExecute(NULL , _T("open") , rLogFilePath , _T("log.html") , NULL , SW_SHOWNORMAL);
				}
				ofile.close();
				//!
			}

			/// DB & Excel Close 부분
			m_pCON = NULL;							
			book.Close(COleVariant((short)FALSE), covOptional, covOptional);//
			app.Quit();							
			///
		}
	}
	
	return ERROR_SUCCESS;
}

/**
	@brief	display current line data table
	@author	HumKyung.BAEK
	@date	2010.03.09
	@return
*/
int CLineListDataImportDlg::DisplayCurrentLineData(CAppPostgreSQL &database)
{
	int i = 0;
	/// delete columns
	m_wndCurrentLineDataListCtrl.SetRedraw(FALSE);
	m_wndCurrentLineDataListCtrl.DeleteAllItems();
	const int nColCount = m_wndCurrentLineDataListCtrl.GetHeaderCtrl()->GetItemCount();
	for(i = 0;i < nColCount;++i)
	{
		m_wndCurrentLineDataListCtrl.DeleteColumn(0);
	}

	///
	CString rExecPathName = CAdminDocData::GetExecPath();
	const CString rSqlFilePath = rExecPathName + _T("\\Setting\\T_LINE_LIST_DATA.XML");
	
	CStringList ColumnNameList;
	{
		CSQLFile sqlFile(rSqlFilePath.operator LPCSTR());
		if(ERROR_SUCCESS != sqlFile.ParseXML(rSqlFilePath.operator LPCTSTR())) return ERROR_BAD_ENVIRONMENT;
		for(int i = 0;i < sqlFile.GetColumnCount();++i)
		{
			const CString rColunmName = sqlFile.GetFieldNameAt(i).c_str();
			ColumnNameList.AddTail(rColunmName);
		}
		///adoDB.GetColumnNameListOfTable(ColumnNameList , "T_LINE_LIST_DATA");
	}
	
	/// reference key를 가져옵니다.
	STRING_T rDatabaseRefKey;
	vector<STRING_T> oRefKeyResult;
	try
	{
		(*database.session()) << _T("SELECT DATABASE_REFERENCE_KEY FROM T_PROJECT_SETTING") , soci::into(rDatabaseRefKey);
		CTokenizer<CIsFromString>::Tokenize(oRefKeyResult , rDatabaseRefKey , CIsFromString(_T(" ~ ")));
	}
	catch(const std::exception& ex)
	{
		AfxMessageBox(ex.what(),MB_OK|MB_ICONERROR);
	}

	list<CString> oDuplicatedLineManDataList;	//! 2011.06.23 added by humkyung

	try
	{
		int nIndex = 0;
		soci::rowset<soci::row> rs(database.session()->prepare << _T("SELECT * FROM T_LINE_LIST_DATA"));
	
		LONG lFieldNo = 0L;
		map<LONG , CString> ColumnMap;
		POSITION pos = ColumnNameList.GetHeadPosition();
		while(pos)
		{
			CString rColName = ColumnNameList.GetNext(pos);
			ColumnMap.insert(make_pair(lFieldNo++ , rColName));
		}
		
		for(map<LONG,CString>::iterator itr = ColumnMap.begin();itr != ColumnMap.end();++itr)
		{
			m_wndCurrentLineDataListCtrl.InsertColumn(itr->first, itr->second);
			m_wndCurrentLineDataListCtrl.SetColumnWidth(itr->first, 100);
		}
		
		int nDuplicatedKey = 0;
		list<CString> KeyList;
		for(soci::rowset<soci::row>::const_iterator rtr = rs.begin();rtr != rs.end();++rtr)
		{
			nIndex = 0;
			
			STRING_T rValue , rKey;
			for(map<LONG,CString>::iterator itr = ColumnMap.begin();itr != ColumnMap.end();++itr)
			{
				rValue = (soci::i_ok == rtr->get_indicator(itr->second.operator LPCTSTR())) ? rtr->get<STRING_T>(itr->second.operator LPCTSTR()) : _T("");
				vector<STRING_T>::const_iterator where = find(oRefKeyResult.begin() , oRefKeyResult.end() , (itr->second).operator LPCTSTR());
				if(where != oRefKeyResult.end())
				{
					rKey += rValue;
				}

				if(0 == nIndex)
				{
					LVITEM lvitem;
					memset(&lvitem , '\0' , sizeof(LVITEM));
					lvitem.mask = LVIF_TEXT;
					lvitem.iItem = i;
					lvitem.iSubItem = 0;
					lvitem.pszText = (char*)(rValue.c_str());
					lvitem.iImage = 0; 
					m_wndCurrentLineDataListCtrl.InsertItem(&lvitem);
				}
				else
				{
					m_wndCurrentLineDataListCtrl.SetItemText(i , itr->first , rValue.c_str());
				}
				
				++nIndex;
			}

			/// 지금 표기한 RECORD가 이전에 입력한 RECORD와 같은 KEY를 가지고 있다면 텍스트를 붉게 표기한다.
			list<CString>::const_iterator where = find(KeyList.begin() , KeyList.end() , rKey.c_str());
			if(where != KeyList.end())
			{
				CString sDuplicatedLineManData;
				const int nItem = m_wndCurrentLineDataListCtrl.GetItemCount() - 1;
				for(int j = 0;j < m_wndCurrentLineDataListCtrl.GetColumnCount() - 1;++j)
				{
					/// 2011.06.23 added by humkyung
					sDuplicatedLineManData += m_wndCurrentLineDataListCtrl.GetItemText(i , j) + _T(",");
					///
					m_wndCurrentLineDataListCtrl.SetItemTextColor(i , j , RGB(255,0,0));
				}
				++nDuplicatedKey;

				/// 2011.06.23 added by humkyung
				if(_T(",") == sDuplicatedLineManData.Right(1))
				{
					sDuplicatedLineManData = sDuplicatedLineManData.Left(sDuplicatedLineManData.GetLength() - 1);
				}
				oDuplicatedLineManDataList.push_back(sDuplicatedLineManData);
				///
			}
			else
			{
				KeyList.push_back(rKey.c_str());
			}
		}
		
		if(nDuplicatedKey > 0)
		{
			CSplashScreenFx* pSplash = CSplashScreenFx::GetInstance();
			if(NULL == pSplash)
			{
				CString rString;
				rString.Format(_T("LINE LIST DATA TABLE에 중복된 KEY를 가진 레코드가 %d개 있습니다.\n로그 파일을 생성할까요?") , nDuplicatedKey);
				if(IDYES == AfxMessageBox(rString , MB_YESNO))
				{
					/// write duplicated line man data to log file - 2011.06.23 added by humkyung
					CAdminDoc* pDoc = CAdminDocData::GetInstance()->GetDocument();
					const CString sDBErrFilePath = pDoc->m_ProjectPath + CString( _T("\\Database\\DBError.txt") );
					ofstream ofile(sDBErrFilePath.operator LPCTSTR());
					if(ofile.is_open())
					{
						for(list<CString>::const_iterator itr = oDuplicatedLineManDataList.begin();itr != oDuplicatedLineManDataList.end();++itr)
						{
							ofile << (*itr).operator LPCTSTR() << std::endl;
						}
						ofile.close();

						::ShellExecute(NULL , _T("open") , sDBErrFilePath , NULL , NULL , SW_SHOWNORMAL);
					}
					///
				}
			}
		}
	}
	catch(const std::exception& ex)
	{
		AfxMessageBox(ex.what(),MB_OK|MB_ICONERROR);
	}

	m_wndCurrentLineDataListCtrl.SetRedraw(TRUE);

	return ERROR_SUCCESS;
}

/**
	@brief	append a header for html

	@author	humkyung

	@date	????.??.??

	@return	int
*/
int CLineListDataImportDlg::appendHeader(ofstream &ofile)
{
	ofile << _T("<!DOCTYPE HTML PUBLIC ");
	ofile << _T("\"-//W3C//DTD HTML 4.01 Transitional//EN\" ");
	ofile << _T("\"http://www.w3.org/TR/html4/loose.dtd\">") << std::endl;
	ofile << _T("<html>") << std::endl;
	ofile << _T("<head>") << std::endl;
	ofile << _T("<title>Admin Log Messages</title>") << std::endl;
	ofile << _T("<style type=\"text/css\">") << std::endl;
	ofile << _T("<!--") << std::endl;
	ofile << _T("body, table {font-family: arial,sans-serif; font-size: x-small;}") << std::endl;
	ofile << _T("th {background: #336699; color: #FFFFFF; text-align: left;}") << std::endl;
	ofile << _T("-->") << std::endl;
	ofile << _T("</style>") << std::endl;
	ofile << _T("</head>") << std::endl;
	ofile << _T("<body bgcolor=\"#FFFFFF\" topmargin=\"6\" leftmargin=\"6\">") << std::endl;
	ofile << _T("<p style=\"font-family:verdana;color:red;font-size:20px\">") << _T("</p>") << std::endl;
	ofile << _T("<hr size=\"1\" noshade>") << std::endl;
	ofile << _T("Log session start time ");
	SYSTEMTIME st;
	GetLocalTime( &st );
	CString strTimeData;
	strTimeData.Format(_T("%d년 %d월 %d일 - %d시 %d분") , st.wYear , st.wMonth , st.wDay , st.wHour , st.wMinute);
	ofile << strTimeData.operator LPCSTR();

	ofile << _T("<br>") << std::endl;
	ofile << _T("<br>") << std::endl;
	ofile << _T("<table cellspacing=\"0\" cellpadding=\"4\" border=\"1\" bordercolor=\"#224466\" width=\"100%\">") << std::endl;
	
	ofile << _T("<tr>") << std::endl;
	
	CProjectData* pProjectData = CProjectData::GetInstance();
	const CString rRefKey = (*(pProjectData->m_pProjectSettingMap))[_T("C_DATABASE_REFERENCE_KEY")];
	vector<string> oRefKey;
	CTokenizer<CIsFromString>::Tokenize(oRefKey , rRefKey.operator LPCTSTR() , CIsFromString(_T(" ~ ")));
	ofile << _T("<th>ANO</th>") << std::endl;
	for(vector<string>::iterator itr = oRefKey.begin();itr != oRefKey.end();++itr)
	{
		if(itr->empty()) continue;
		ofile << _T("<th>") << itr->c_str() << _T("</th>") << std::endl;
	}
	ofile << _T("</tr>") << std::endl;

	return ERROR_SUCCESS;
}

/**
	@brief	
	@author	humkyung
	@date	2011.07.07
	@return	void
*/
int CLineListDataImportDlg::DeleteSelectedRecordIfDuplicated()
{
	if(1 == m_wndCurrentLineDataListCtrl.GetSelectedItemCount())
	{
		if(IDYES == AfxMessageBox(_T("정말로 삭제하시겠습까?") , MB_YESNO))
		{
			TCHAR colname[256]=_T("");

			CString sClause;
			const int nItem = m_wndCurrentLineDataListCtrl.GetFirstSelectedItem();
			for(int i = 0;i < m_wndCurrentLineDataListCtrl.GetColumnCount();++i)
			{	
				LVCOLUMN lvcol;
				ZeroMemory(&lvcol, sizeof(lvcol));
				lvcol.mask=LVCF_TEXT;
				lvcol.cchTextMax=sizeof(colname)-1;
				lvcol.pszText=colname;
				m_wndCurrentLineDataListCtrl.GetColumn(i, &lvcol);

				const CString sValue = m_wndCurrentLineDataListCtrl.GetItemText(nItem , i);
				if(!sValue.IsEmpty())
				{
					sClause += CString(colname) + _T("='") + sValue + _T("' AND ");
				}
			}
			if(_T(" AND ") == sClause.Right(5)) sClause = sClause.Left(sClause.GetLength() - 5);

			CAdminDoc* pDoc = CAdminDocData::GetInstance()->GetDocument();
			if(pDoc)
			{
				const CProject::DatabaseProp prop = pDoc->GetCurProject()->GetDatabaseProp();

				CAppPostgreSQL database(pDoc->GetCurProject()->name(),prop.Host.c_str(),prop.User.c_str(),prop.Password.c_str());
				try
				{
					CString sSql;
					sSql.Format(_T("DELETE * FROM T_LINE_LIST_DATA WHERE %s") , sClause);
					(*database.session()) << sSql.operator LPCTSTR();

					/// re-display line man data table - 2011.07.07 added by humkyung
					DisplayCurrentLineData(database);
				}
				catch(const std::exception& ex)
				{
					AfxMessageBox(ex.what(),MB_OK|MB_ICONERROR);
				}
			}
		}
		
		return ERROR_SUCCESS;
	}

	return ERROR_BAD_ENVIRONMENT;
}

/**
	@brief	callback function for keydown
	@author	humkyung
	@date	2011.07.07
	@return	void
*/
void CLineListDataImportDlg::OnKeydownListCurrentLineData(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	if ( VK_DELETE == pLVKeyDow->wVKey)
	{
		DeleteSelectedRecordIfDuplicated();
	} 
	
	*pResult = 0;
}

/******************************************************************************
    @author     humkyung
    @date       2012-05-25
    @class      CLineListDataImportDlg
    @function   OnHelpInfo
    @return     BOOL
    @param      HELPINFO*   pHelpInfo
    @brief
******************************************************************************/
BOOL CLineListDataImportDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	HWND hHtml = ::HtmlHelp(this->m_hWnd , CAdminDocData::GetExecPath() + _T("\\Help\\SmartISO.chm::/Line List Data Import.htm") , HH_DISPLAY_TOPIC , 0);

	return CFormView::OnHelpInfo(pHelpInfo);
}

/******************************************************************************
    @author     humkyung
    @date       2012-08-03
    @class      CLineListDataImportDlg
    @function   OnBnClickedButtonDatabaseReferenceKey
    @return     void
    @brief
******************************************************************************/
void CLineListDataImportDlg::OnBnClickedButtonDatabaseReferenceKey()
{
	CAdminApp* pApp = (CAdminApp*)AfxGetApp();
	
	CFileNameFormatDlg dlg;
	dlg.m_rFileNameFormat = m_rDatabaseReferenceKeyFormat;
	dlg.m_pAdminDoc = static_cast<CAdminDoc*>(this->GetDocument());
	if(IDOK == dlg.DoModal())
	{
		m_rDatabaseReferenceKeyFormat = dlg.m_rFileNameFormat;
		CString rFileNameFormat(m_rDatabaseReferenceKeyFormat);
		rFileNameFormat.Replace(_T(":") , _T(" ~ "));
		m_wndDatabaseReferenceKeyEdit.SetWindowText(rFileNameFormat);

		CString rString;
		GetDlgItemText(IDC_EDIT_DATABASE_REFERENCE_KEY , rString);
		CProjectData* pProjectData = CProjectData::GetInstance();
		if(NULL != pProjectData)
		{
			(*(pProjectData->m_pProjectSettingMap))[_T("C_DATABASE_REFERENCE_KEY")] = rString;
		
			CAdminDoc* pDoc = (CAdminDoc*)GetDocument();
			if(pDoc)
			{
				const CString sDatabasePath = pDoc->m_ProjectPath + CString(_T("\\Database\\")) + pDoc->m_ProjectName + CString(_T(".MDB"));
				CAdminDocData* pInst = CAdminDocData::GetInstance();
				if(pInst) pInst->SaveSettingData( sDatabasePath );
			}
		}
	}
}
