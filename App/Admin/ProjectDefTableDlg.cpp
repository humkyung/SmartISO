// ProjectDefTableDlg.cpp : implementation file
//

#include "stdafx.h"
#include "excel9.h"
#include "Admin.h"
#include "AdminView.h"
#include "ProjectDefTableDlg.h"
#include "FieldTypeGridCell.h"
#include "ExcelMapingGridCell.h"
#include "FilterGridCell.h"
#include "enablebuddybutton.h"
#include "tokenizer.h"
#include "AdminDocData.h"
#include "FileNameFormatDlg.h"
#include "WorkStatusDlg.h"
#include <ado/SQLFile.h>
#include <LineListConverter.h>
#include <ProjectData.h>
#include <Splash/SplashScreenFx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProjectDefTableDlg

IMPLEMENT_DYNCREATE(CProjectDefTableDlg, CFormView)

CProjectDefTableDlg::CProjectDefTableDlg()
	: CFormView(CProjectDefTableDlg::IDD)/* , m_wndResize( FALSE )*/
{
	//{{AFX_DATA_INIT(CProjectDefTableDlg)
	m_iCurCol = -1;
	//}}AFX_DATA_INIT
}

CProjectDefTableDlg::~CProjectDefTableDlg()
{
	try
	{
		for(vector<Map*>::const_iterator itr = m_LineDataMapData.begin();itr != m_LineDataMapData.end();++itr)
		{
			delete (*itr);
		}
		m_LineDataMapData.clear();
	}
	catch(...)
	{
	}
}

void CProjectDefTableDlg::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProjectDefTableDlg)
	DDX_Control(pDX , IDC_LIST_SHEET_NAME , m_wndSheetNameListCtrl);
	DDX_Control(pDX , IDC_LIST_CURRENT_LINE_DATA , m_wndCurrentLineDataListCtrl);
	DDX_Control(pDX , IDC_BUTTON_SelectExcel , m_btnSelectExcel);
	DDX_Control(pDX , IDC_BUTTON_DATABASE_REFERENCE_KEY , m_btnDatabaseReferenceKey);
	DDX_Control(pDX , IDC_COMBO_START_ROW , m_cboStartRow);
	DDX_Control(pDX , IDC_EDIT_DATABASE_REFERENCE_KEY , m_wndDatabaseReferenceKeyEdit);
	DDX_Text(pDX , IDC_STATIC_ShowExcelName , m_sExlFilePath);
	DDX_Text(pDX , IDC_EDIT_DATABASE_REFERENCE_KEY  , m_rDatabaseReferenceKeyFormat);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProjectDefTableDlg, CFormView)
	//{{AFX_MSG_MAP(CProjectDefTableDlg)
	ON_WM_SIZE()
	ON_CBN_EDITCHANGE(IDC_COMBO_FIELD_TYPE, OnEditchangeComboFieldType)
	ON_BN_CLICKED(IDC_BUTTON_SelectExcel, OnButtonSelectExcel)
	ON_BN_CLICKED(IDC_BUTTON_DATABASE_REFERENCE_KEY, &CProjectDefTableDlg::OnBnClickedButtonDatabaseReferenceKey)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_CURRENT_LINE_DATA, OnKeydownListCurrentLineData)
	//}}AFX_MSG_MAP
	//ON_COMMAND(ID_HORZ_RESIZE, OnHorzResize)
	ON_WM_HELPINFO()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProjectDefTableDlg diagnostics

#ifdef _DEBUG
void CProjectDefTableDlg::AssertValid() const
{
	CFormView::AssertValid();
}

void CProjectDefTableDlg::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CProjectDefTableDlg message handlers

BOOL CProjectDefTableDlg::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	CAdminApp* pApp = (CAdminApp*)AfxGetApp();
	///CResourceSwitch resSwitch(pApp->m_hMultiLangRes);

	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

/**	
	@brief	The CProjectDefTableDlg::OnInitialUpdate function
	@author	HumKyung.BAEK
	@return	void	
*/
void CProjectDefTableDlg::OnInitialUpdate() 
{
	static bool __init = false;
	CFormView::OnInitialUpdate();
	CAdminView* pAdminView = static_cast<CAdminView*>(GetParent());
	CAdminDoc* pAdminDoc = pAdminView->GetDocument();

	if(false == __init)
	{
		BOOL b = m_GridCtrl.Create(CRect(0,0,0,0),this,0x100);
		if(TRUE == b)
		{
			/// 그리드 컨트롤 폰트 코드
			CFont font;
			font.CreateFont(-11,0,0,0,400,FALSE,FALSE,FALSE,HANGUL_CHARSET,3,2,1,VARIABLE_PITCH | FF_MODERN, _T("Arial")); 
			m_GridCtrl.SetFont(&font);
			font.DeleteObject();
			
			m_ImageList.Create(IDB_ADMIN_TREE , 16 , 16 , RGB(255 , 255 , 255));
			m_GridCtrl.SetImageList(&m_ImageList);

			/// fill it up with stuff
			m_GridCtrl.SetEditable(TRUE);
			m_GridCtrl.EnableDragAndDrop(FALSE);
			m_GridCtrl.SetTextBkColor(RGB(255, 255, 224));
			m_GridCtrl.SetRowResize(FALSE);
			m_GridCtrl.SetColumnResize(TRUE);
			m_GridCtrl.SetListMode(FALSE);
			m_GridCtrl.SetSingleRowSelection(TRUE);
			m_GridCtrl.SetHeaderSort(FALSE);
			m_GridCtrl.SetSingleColSelection(TRUE);
		}
		//else
		//{
		//	AfxMessageBox("Can't create grid control!!!");
		//}

		m_imgCheckBoxes.Create(IDB_CHECKBOXES,16,3,RGB(255,0,255));
		m_wndSheetNameListCtrl.SetHeadings( _T("[],30;Sheet Name, 183") );
		m_wndSheetNameListCtrl.SetExtendedStyle(LVS_EX_CHECKBOXES | m_wndSheetNameListCtrl.GetExtendedStyle());
		m_wndSheetNameListCtrl.SetHeaderImageList(&m_imgCheckBoxes);
		m_wndSheetNameListCtrl.EnableHeaderCheckBox(TRUE);

		{
			HWND hwndEdit  = GetDlgItem(IDC_STATIC_ShowExcelName)->GetSafeHwnd();
			::EnableBuddyButton( hwndEdit, m_btnSelectExcel.GetSafeHwnd() , BBS_RIGHT);
		}

		{
			HWND hwndEdit  = GetDlgItem(IDC_EDIT_DATABASE_REFERENCE_KEY)->GetSafeHwnd();
			::EnableBuddyButton( hwndEdit, m_btnDatabaseReferenceKey.GetSafeHwnd() , BBS_LEFT);
		}

		CWnd* pWnd = GetDlgItem(IDC_STATIC_LINE_DATA);
		if(NULL != pWnd) pWnd->SetWindowText(_TR("Line List Table"));
		
		__init = true;
	}
}

/**
	@brief	프로젝트가 로딩되고 난 후 폼 초기화 부분
	@author	HumKyung.BAEK
	@date
	@return
**/
BOOL CProjectDefTableDlg::UpdateContents()
{
	CAdminView* pAdminView = static_cast<CAdminView*>(GetParent());
	CAdminDoc* pAdminDoc = pAdminView->GetDocument();
	
	////////////////////////////////////////// Def Table /////////////////////////////////////////////
	InitLineDataContents();
	///////////////////////////////////////////////////////////////////////////////////////////////

	/// 2012.08.03 added by humkyung
	CProjectData* pProjectData = CProjectData::GetInstance();
	if(NULL != pProjectData)
	{
		map<CString,CString>::const_iterator where = pProjectData->m_pProjectSettingMap->find(_T("database_reference_key"));
		if(where != pProjectData->m_pProjectSettingMap->end())
		{
			m_rDatabaseReferenceKeyFormat = where->second;
			m_wndDatabaseReferenceKeyEdit.SetWindowText(m_rDatabaseReferenceKeyFormat);
		}
	}
	/// up to here

	CAdminDocData* pDocData = CAdminDocData::GetInstance();
	const CString rIniFilePath = pDocData->GetIniFilePath();
	const UINT iStartRow = GetPrivateProfileInt(_T("RowNum") , _T("Rnum") , 2 , rIniFilePath);
	CString str;
	str.Format(_T("%d") , iStartRow);
	const int at = m_cboStartRow.FindStringExact(0,str);
	(-1 != at) ? m_cboStartRow.SetCurSel(at) : m_cboStartRow.SetCurSel(0);

	CAdminDoc* pDoc = CAdminDoc::GetInstance();
	const CProject::DatabaseProp prop = pDoc->GetCurProject()->GetDatabaseProp();

	try
	{
		CAppPostgreSQL database(pDoc->GetCurProject()->name(),prop.Host.c_str(),prop.User.c_str(),prop.Password.c_str());
		DisplayCurrentLineData(database);
	}
	catch(const std::exception& ex)
	{
		AfxMessageBox(ex.what(),MB_OK|MB_ICONERROR|MB_TOPMOST);
	}
	
	///////////////////////////////////////////////////////////////////////////////////
	return 1;
}

void CProjectDefTableDlg::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	const int gap = 3;
	if(m_GridCtrl.GetSafeHwnd())
	{
		CRect rect;
		CWnd* pWnd = GetDlgItem(IDC_STATIC_LINE_DATA);
		if(pWnd)
		{
			pWnd->GetWindowRect(&rect);
			ScreenToClient(&rect);
			pWnd->SetWindowPos(NULL , 0 , gap , cx , rect.Height() , SWP_NOZORDER);
			pWnd->GetWindowRect(&rect);
			ScreenToClient(&rect);
		}
				
		m_GridCtrl.SetWindowPos(NULL , gap , 20 , cx - gap * 2 , rect.Height() - gap - 20 , SWP_NOZORDER);

		pWnd = GetDlgItem(IDC_STATIC_LINE_LIST_IMPORT);
		if(NULL != pWnd)
		{
			pWnd->GetWindowRect(&rect);
			ScreenToClient(&rect);
			pWnd->SetWindowPos(NULL , 0 , gap , cx , rect.Height() , SWP_NOMOVE | SWP_NOZORDER);
			pWnd->GetWindowRect(&rect);
			ScreenToClient(&rect);
		}

		pWnd = GetDlgItem(IDC_LIST_CURRENT_LINE_DATA);
		if(NULL != pWnd)
		{
			pWnd->GetWindowRect(&rect);
			ScreenToClient(&rect);
			pWnd->SetWindowPos(NULL , 0 , gap , cx , cy - rect.top , SWP_NOMOVE | SWP_NOZORDER);
			pWnd->GetWindowRect(&rect);
			ScreenToClient(&rect);
		}
	}

	DetermineLayout();
}

/**
	@brief	
	@author	humkyung
**/
BOOL CProjectDefTableDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	if(wParam == m_GridCtrl.GetDlgCtrlID())
	{
		NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)(lParam);
		if(GVN_ENDLABELEDIT == pGridView->hdr.code)
		{
			/// update T_LINE_LIST_DATA_MAPPING table - 2012.05.09 added by humkyung
			if(2 == pGridView->iRow)
			{
				CAdminView* pAdminView = static_cast<CAdminView*>(GetParent());
				CAdminDoc* pDoc = pAdminView->GetDocument();
				const CProject::DatabaseProp prop = pDoc->GetCurProject()->GetDatabaseProp();

				try
				{
					CAppPostgreSQL database(pDoc->GetCurProject()->name(),prop.Host.c_str(),prop.User.c_str(),prop.Password.c_str());
					CString sColumn = m_GridCtrl.GetItemText(1 , pGridView->iColumn);
					CString sValue  = m_GridCtrl.GetItemText(2 , pGridView->iColumn);
					CString sSql = _T("UPDATE T_LINE_LIST_DATA_MAPPING SET ") + sColumn + _T("='") + sValue + _T("'");
					(*database.session()) << sSql.operator LPCTSTR();
				}
				catch(const std::exception& ex)
				{
					AfxMessageBox(ex.what(),MB_OK|MB_ICONERROR);
				}
			}
			/// up to here
			else if((pGridView->iColumn == m_GridCtrl.GetColumnCount() - 1 ))
			{
				CString rItemText = m_GridCtrl.GetItemText(pGridView->iRow , pGridView->iColumn);
				vector<Map*>::const_iterator where = find_if(m_LineDataMapData.begin() , m_LineDataMapData.end() , [rItemText](Map* param){return (param->rFieldDesc == rItemText);});
				if(m_LineDataMapData.end() == where)
				{
					const int nColumnCount = m_GridCtrl.GetColumnCount();
					m_GridCtrl.SetColumnCount(nColumnCount + 1);
					
					CString rStr;
					rStr.Format(_T("%d") , nColumnCount-1);
					m_GridCtrl.SetItemFormat(0 , nColumnCount-1, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
					m_GridCtrl.SetItemText(0 , nColumnCount-1 , rStr);
					
					//////////////////////////////////////////////////////////////////////////
					Map* pMapData = new Map;
					pMapData->rFieldName = _T("");
					pMapData->rFieldDesc = m_GridCtrl.GetItemText(pGridView->iRow , pGridView->iColumn);
					pMapData->rExcelColumn=_T("");
					m_LineDataMapData.push_back(pMapData);
					m_GridCtrl.SetItemData(0 , pGridView->iColumn , (DWORD)(pMapData));
					//////////////////////////////////////////////////////////////////////////
					
					m_GridCtrl.SetItemFormat(0 , nColumnCount , DT_CENTER|DT_VCENTER|DT_SINGLELINE);
					m_GridCtrl.SetItemText(0 , nColumnCount , _T("*"));
					
					m_GridCtrl.SetCellType(2, nColumnCount - 1 , RUNTIME_CLASS(GridCell::CExcelMapingGridCell));
					
					m_GridCtrl.AutoSizeColumn(nColumnCount-1);
					m_GridCtrl.AutoSizeColumn(nColumnCount);
				}
				else
				{
					CString rString;
					rString.LoadString(IDS_WARNING_DUPLICATED_DESC);
					AfxMessageBox(rString);
					m_GridCtrl.SetItemText(1 , pGridView->iColumn , _T(""));
				}
			}
			else
			{
				m_GridCtrl.AutoSizeColumn(pGridView->iColumn);
				m_GridCtrl.RedrawWindow();
			}
		}
	}
	
	return CFormView::OnNotify(wParam, lParam, pResult);
 }

//////////////////////////////////////////////////////////////////////////
void CProjectDefTableDlg::OnEditchangeComboFieldType()
{
}

/**
	@brief	load sheet name from excel file.
	@author	HumKyung.BAEK
	@date	2010.03.08
	@return
*/
int CProjectDefTableDlg::LoadSheetName(const CString& sExlFilePath) 
{
	if(!sExlFilePath.IsEmpty())
	{
		try
		{	
			BeginWaitCursor();
			
			CLineListConverter ExGetname;
			CStringArray* pSheetNameArray = ExGetname.GetExcelName(sExlFilePath);
			
			m_wndSheetNameListCtrl.SetRedraw(FALSE);
			m_wndSheetNameListCtrl.DeleteAllItems();

			LVITEM lvitem;
			lvitem.mask = LVIF_TEXT | LVIF_IMAGE;
			lvitem.iImage = 0;
			lvitem.cchTextMax = 32;

			const int nSize = pSheetNameArray->GetSize();
			for(int i = 0;i < nSize;i++)
			{
				lvitem.iItem = i;
				lvitem.iSubItem = 0;
				lvitem.pszText = _T("");
				const int at = m_wndSheetNameListCtrl.InsertItem(&lvitem);
				if(-1 != at) m_wndSheetNameListCtrl.SetItemText(at,1,pSheetNameArray->GetAt(i));
			}
			m_wndSheetNameListCtrl.SetRedraw(TRUE);
			
			EndWaitCursor();
		}
		catch(...)
		{
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
	@brief	select line man data excel file
	@author	humkyung
	@date	2010.03.08
	@return
*/
void CProjectDefTableDlg::OnButtonSelectExcel() 
{
	CFileDialog dlg(TRUE,_T(""),_T("*.xls"),OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,
		_T("Excel 문서(*.xls)||모든 문서(*.*)|*.*||")); 
	if(IDOK == dlg.DoModal())
	{
		m_sExlFilePath = dlg.GetPathName();
		SetDlgItemText(IDC_STATIC_ShowExcelName, (LPSTR)(LPCTSTR)m_sExlFilePath);

		LoadSheetName(m_sExlFilePath);	/// 2011.02.14 added by HumKyung
	}
}

void CProjectDefTableDlg::DetermineLayout()
{
	CRect clientRect;
	GetClientRect(&clientRect);
	const int cx = clientRect.Width();
	const int cy = clientRect.Height();
	CRect rect;
	if(m_GridCtrl.GetSafeHwnd())
	{
		m_GridCtrl.GetWindowRect(&rect);
		ScreenToClient(&rect);
	}

	RedrawWindow();
}

/**	
	@brief	The CProjectDefTableDlg::IsFixedLineDataFieldName function
	@param	rFieldName	a parameter of type const CString&
	@remarks	고정된 field name은 수정하거나 삭제할 수 없다.
	@return	bool	
*/
bool CProjectDefTableDlg::IsFixedLineDataFieldName(const CString& rFieldName)
{
	static const CString __FIXED_FIELD_NAME[] =
	{
		"LINE_NO_LABEL_1" , "LINE_NO_LABEL_2" , "LINE_NO_LABEL_3" , "LINE_NO_LABEL_4" ,
		"LINE_NO_LABEL_5" , "LINE_NO_LABEL_6" , "LINE_NO_LABEL_7" , "LINE_NO_LABEL_8" ,
		"LINE_NO_LABEL_9" , "LINE_NO_LABEL_10"
	};

	const size_t nCount = sizeof(__FIXED_FIELD_NAME) / sizeof(__FIXED_FIELD_NAME[0]);
	const CString* p = find(__FIXED_FIELD_NAME , __FIXED_FIELD_NAME + nCount , rFieldName);
	if((p - __FIXED_FIELD_NAME) < nCount) return true;

	return ((rFieldName == C_PLANT_NO) || (rFieldName == C_AREA_NO) || (rFieldName == C_PDS_AREA_NO) ||
		(rFieldName == C_SECTION_NO) || (rFieldName == C_LINE_NO) || (rFieldName == C_DRAW_NO) ||
		(rFieldName == C_UNIT_NO)  || (rFieldName == C_SHEET_NO) || (rFieldName == C_FLUID_CODE) ||
		(rFieldName == C_PAINT_CODE) || (rFieldName == C_INS_TYPE) || (rFieldName == C_INS_TEMP) ||
		(rFieldName == C_INS_THICKNESS) || (rFieldName == C_OPER_PRES) ||
		(rFieldName == C_OPER_TEMP) || (rFieldName == C_DGN_PRES) ||
		(rFieldName == C_DGN_TEMP) ||(rFieldName == C_PLAN_NO) || (rFieldName == C_PID_NO));
}

/**	
	@brief	반드시 있어야할 필드인지 확인한다.
	@param	rFieldName	a parameter of type CString
	@return	bool	
*/
bool CProjectDefTableDlg::IsFixedRevDataFieldName(CString rFieldName)
{
	return ((rFieldName == _T("KEY")) || (rFieldName == C_REV_NO) || (rFieldName == C_REV_DATE) || 
		/*(rFieldName == C_REV_CONTROL_WORKING_DATE) || */(rFieldName == C_REV_DRWN_BY) || 
		(rFieldName == C_REV_CHECKED_BY) || (rFieldName == REV_APPROVED_BY));
}

/** 
	@brief	
	@author	HumKyung.BAEK
	@date	
	@return
**/
BOOL CProjectDefTableDlg::InitLineDataContents()
{
	CAdminView* pAdminView = static_cast<CAdminView*>(GetParent());
	CAdminDoc* pDoc = pAdminView->GetDocument();
	const CProject::DatabaseProp prop = pDoc->GetCurProject()->GetDatabaseProp();
	
	try
	{
		CAppPostgreSQL database(pDoc->GetCurProject()->name(),prop.Host.c_str(),prop.User.c_str(),prop.Password.c_str());
		vector<STRING_T> oFieldNameList;
		soci::rowset<soci::row> rs(database.session()->prepare << _T("select column_name from information_schema.columns where table_schema='public' and table_name='t_iso_line_data'"));
		for(soci::rowset<soci::row>::const_iterator itr = rs.begin();itr != rs.end();++itr) oFieldNameList.push_back(itr->get<STRING_T>(0));

		vector<STRING_T> oMappingColumnNameList;
		const int nCount = oFieldNameList.size();
		try 
		{
			m_GridCtrl.SetRowCount(3);
			m_GridCtrl.SetColumnCount(nCount + 2);
			m_GridCtrl.SetFixedRowCount(1);
			m_GridCtrl.SetFixedColumnCount(1);
		}
		catch (CMemoryException* e)
		{
			e->ReportError();
			e->Delete();
			return FALSE;
		}

		int nIndex = 0;
		for(vector<STRING_T>::const_iterator itr = oFieldNameList.begin();itr != oFieldNameList.end();++itr)
		{
			CString rValue = itr->c_str() , rString;
			
			rString.Format(_T("%d") , nIndex + 1);
			m_GridCtrl.SetItemFormat(0, nIndex + 1, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_GridCtrl.SetItemText(0, nIndex + 1, rString);
			
			m_GridCtrl.SetItemText(1, nIndex + 1, rValue);
			m_GridCtrl.SetItemState(1 , nIndex + 1 , GVNI_READONLY);

			/// display excel column corresponding to database column - 2012.05.09 added by humkyung
			STRING_T sValue;
			try
			{
				(*database.session()) << (_T("SELECT ") + STRING_T(rValue.operator LPCTSTR()) + _T(" FROM T_LINE_LIST_DATA_MAPPING")) , soci::into(sValue);
				m_GridCtrl.SetItemText(2, nIndex + 1, sValue.c_str());
				m_GridCtrl.SetCellType(2 , nIndex + 1 , RUNTIME_CLASS(GridCell::CExcelMapingGridCell));
			}
			catch(...)
			{
				m_GridCtrl.GetCell(2 , nIndex + 1)->SetBackClr(RGB(217,217,217));
				m_GridCtrl.SetItemState(2 , nIndex + 1 , GVNI_READONLY);
			}
			/// up to here

			++nIndex;
		}

		m_GridCtrl.SetItemImage(1 , 0 , 2);	/// access image
		m_GridCtrl.SetItemImage(2 , 0 , 6);	/// excel image
		
		m_GridCtrl.AutoSizeRows();	
		m_GridCtrl.AutoSizeColumns();
	}
	catch(const std::exception& ex)
	{
		AfxMessageBox(ex.what(),MB_OK|MB_ICONERROR);
	}

	return TRUE;
}

/******************************************************************************
	@brief		save key for line list
    @author     humkyung
    @date       2012-08-03
    @class      CLineListDataImportDlg
    @function   OnBnClickedButtonDatabaseReferenceKey
    @return     void
******************************************************************************/
void CProjectDefTableDlg::OnBnClickedButtonDatabaseReferenceKey()
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

		CString sString;
		GetDlgItemText(IDC_EDIT_DATABASE_REFERENCE_KEY , sString);
		CProjectData* pProjectData = CProjectData::GetInstance();
		if(NULL != pProjectData)
		{
			(*(pProjectData->m_pProjectSettingMap))[_T("database_reference_key")] = sString;
			CAdminDocData::GetInstance()->SaveSettingData();
		}
	}
}

/**
	@brief	
	@author	humkyung
	@date	2010.03.09
	@return
*/
void CProjectDefTableDlg::OnButtonLineListImport() 
{
	CAdminDoc* pDoc = CAdminDocData::GetInstance()->GetDocument();

	if(pDoc && (m_wndSheetNameListCtrl.GetItemCount() > 0))
	{
		CAdminDocData* pDocData = CAdminDocData::GetInstance();

		CProjectData* pProjectData = CProjectData::GetInstance();
		const CString sRefKey = (*(pProjectData->m_pProjectSettingMap))[_T("database_reference_key")];
		if(sRefKey.IsEmpty())
		{
			AfxMessageBox(_T("First of all, select reference key."));
			return;
		}

		CString rIniFilePath = pDocData->GetIniFilePath();

		CString str,InputNum;
		const int at = m_cboStartRow.GetCurSel();
		if(-1 == at)
		{
			AfxMessageBox(_T("First of all, select start row."));
			return;
		}
		m_cboStartRow.GetLBText(at,InputNum);
		
		int checkNum = m_wndSheetNameListCtrl.GetCheckedItemCount();
		const int nItemCount = m_wndSheetNameListCtrl.GetItemCount();
		int j=0;
		
		for(int i = 0;i < nItemCount;++i)
		{
			if(TRUE == m_wndSheetNameListCtrl.GetCheck(i))
			{
				const CString sSheetName = m_wndSheetNameListCtrl.GetItemText(i,1);
				str.Format( _T("Selected Sheet Name%d") , j);
				WritePrivateProfileString(_T("Line List Data") , str , sSheetName , rIniFilePath);
				j++;
			}
		}
		str.Format("%d",j);
		WritePrivateProfileString( _T("Line List Data") , _T("Selected Sheet Count") ,str,rIniFilePath);
		WritePrivateProfileString( _T("RowNum") , _T("Rnum") ,InputNum,rIniFilePath);

		//////////////////////////////////////////////////////////////////////////
		CWorkStatusDlg dlg;
		if(InputNum != "")
		{
			dlg.m_pThread = AfxBeginThread(StatusThreadEntry, this, THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
			
			if (NULL == dlg.m_pThread)
			{
				AfxMessageBox(_T("Can't create thread!!!"),MB_OK|MB_ICONERROR);
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

UINT CProjectDefTableDlg::StatusThreadEntry(LPVOID pVoid)
{
	return ((CProjectDefTableDlg*)pVoid)->StatusThread();
}

/**	
	@brief	쓰레드 실행부
	@author	humkyung
	@date	2010.03.09
	@return	UINT
*/
UINT CProjectDefTableDlg::StatusThread()
{
	CWorkStatusDlg* pWorkStatusDlg = CWorkStatusDlg::GetInstance();
	try
	{
		InterlockedExchange((LONG*)(&(pWorkStatusDlg->m_bThreadRunning)) , TRUE);///< so the main thread knows that this thread is still running

		CString rIniFilePath = CAdminDocData::GetInstance()->GetIniFilePath();
		const int res = ImportLineDataFile(m_sExlFilePath , rIniFilePath);
		
		InterlockedExchange((LONG*)(&(pWorkStatusDlg->m_bThreadRunning)) , FALSE);
		
		//////////////////////////////////////////////////////////////////////////
		/// close work status dialog
		pWorkStatusDlg->SendMessage(WM_COMMAND , IDOK );

		return IDOK;
	}
	catch(...)
	{
		AfxMessageBox(_T("Unknown error"));

		InterlockedExchange((LONG*)(&(pWorkStatusDlg->m_bThreadRunning)) , FALSE);
		
		//////////////////////////////////////////////////////////////////////////
		/// close work status dialog
		pWorkStatusDlg->SendMessage(WM_COMMAND , IDOK );
	}

	return IDCANCEL;
}

/**
	@brief	import line data from excel file
	@author	humkyung
	@date	2010.03.09
	@return
*/
int CProjectDefTableDlg::ImportLineDataFile(const CString& rExlFilePath , const CString& rIniFilePath)
{
	CoInitialize(NULL);  /// COM관련 개체 초기화 
	
	TCHAR szCount[3]={0,};
	TCHAR szExcel[MAX_PATH + 1]={0,};
	TCHAR szName[MAX_PATH + 1]={0,};
	CString m_strName,m_strExcel;
	
	CAdminDocData* pDocData = CAdminDocData::GetInstance();
	CAdminDoc* pDoc = pDocData->GetDocument();
	if(pDoc)
	{
		const CProject::DatabaseProp prop = pDoc->GetCurProject()->GetDatabaseProp();
		CProjectData* pProjectData = CProjectData::GetInstance();

		const CString sRefKey = (*(pProjectData->m_pProjectSettingMap))[_T("database_reference_key")];
		vector<STRING_T> oRefKey;
		CTokenizer<CIsFromString>::Tokenize(oRefKey,sRefKey.operator LPCTSTR(),CIsFromString(_T(" ~ ")));

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
		try
		{
			CAppPostgreSQL database(pDoc->GetCurProject()->name(),prop.Host.c_str(),prop.User.c_str(),prop.Password.c_str());
			soci::transaction txn(*database.session());
			{
				/// 현재의 T_LINE_LIST_DATA를 삭제한다.
				(*database.session()) << _T("DELETE FROM T_LINE_LIST_DATA");

				/// Database에서 Mapping정보를 받아온다.
				map<CString,CString> oMappingDataEntry , MappingValueEntry;
				GetLineDataMappingData(oMappingDataEntry , database);

				vector<STRING_T> oFieldNameList;
				soci::rowset<soci::row> rs(database.session()->prepare << _T("select column_name from information_schema.columns where table_schema='public' and table_name='t_line_list_data'"));
				for(soci::rowset<soci::row>::const_iterator itr = rs.begin();itr != rs.end();++itr) oFieldNameList.push_back(itr->get<STRING_T>(0));
				
				m_pCON.CreateInstance(__uuidof(Connection));	
				if(!app.CreateDispatch( _T("Excel.Application") ))
				{
					AfxMessageBox(_T("Excel Connection Failure"));
				}
				else
				{
					CWorkStatusDlg* pWorkStatusDlg = CWorkStatusDlg::GetInstance();

					/// Excel file 설정
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
					TCHAR sSheetName[MAX_PATH]={0,};
					list<STRING_T> oSelectedSheetNames;
					
					/// Excel Exception Sheet 정보 read & save
					CString str;
					const int nSelectedSheetCount = GetPrivateProfileInt( _T("Line List Data") , _T("Selected Sheet Count") , 0 , rIniFilePath);
					for(int i=0;i<nSelectedSheetCount;++i)
					{
						str.Format( _T("Selected Sheet Name%d") ,i);
						GetPrivateProfileString(_T("Line List Data") , str , _T("") , sSheetName ,MAX_PATH, rIniFilePath);
						oSelectedSheetNames.push_back(sSheetName);
					}
					///
					
					map<CString, list<map<STRING_T,STRING_T> > > ErrorCheckMap;

					CString rTitle( _T("Convert Line List...") );
					double dProcess = 0.;
					
					/// 시작 날짜를 구한다.
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
						
						if(oSelectedSheetNames.end() == find_if(oSelectedSheetNames.begin(),oSelectedSheetNames.end(),[rSheetName](STRING_T param){return CString(param.c_str()) == rSheetName;}))
						{
							continue;
						}
						
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
							for(map<CString,CString>::const_iterator itr = oMappingDataEntry.begin();itr != oMappingDataEntry.end();++itr)
							{
								const CString sFieldName(itr->first);
								/// check if column is in line data table - 2012.08.23 added by humkyung
								if(oFieldNameList.end() == find_if(oFieldNameList.begin() , oFieldNameList.end() , [sFieldName](STRING_T param){return (CString(param.c_str()) == sFieldName);}))
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
							
							/// DB Query 부분 DB에 저장
							if (bExistData)
							{
								if(_T(",") == rColumns.Right(1)) rColumns = rColumns.Left(rColumns.GetLength() - 1);
								if(_T(",") == rValues.Right(1))  rValues  = rValues.Left(rValues.GetLength() - 1);

								CString rSql(_T("INSERT INTO T_LINE_LIST_DATA(") + rColumns + _T(") VALUES(") + rValues + _T(")"));
								(*database.session()) << rSql.operator LPCTSTR();
							}

							dProcess = r;
							if(NULL != pWorkStatusDlg)
							{
								int process = (int)((dProcess / lNumRows) * 100.);
								pWorkStatusDlg->UpdateWorkStatus(rTitle , process);
							}
						}
					}

					/// write and open log file.
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
					///

					/// DB & Excel Close 부분
					m_pCON->Release();
					m_pCON = NULL;							
					//book.Close(COleVariant((short)FALSE), covOptional, covOptional);//
					//app.Quit();	
					///
				}
			}
			txn.commit();
		}
		catch(const std::exception& ex)
		{
			AfxMessageBox(ex.what(),MB_OK|MB_ICONERROR);
		}
	}
	CoUninitialize();
	
	return ERROR_SUCCESS;
}

/**
	@brief	append a header for html
	@author	humkyung
	@date	????.??.??
	@return	int
*/
int CProjectDefTableDlg::appendHeader(ofstream &ofile) const
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
	const CString rRefKey = (*(pProjectData->m_pProjectSettingMap))[_T("database_reference_key")];
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

CString CProjectDefTableDlg::GetCellString(VARIANT &valCell) const
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

long CProjectDefTableDlg::FieldName2Long(const CString &rFieldName) const
{										
	long ret = 0L;								
	for(int i = 0 ;i < rFieldName.GetLength() ;++i)
	{									
		ret += (rFieldName[i] - 'A' + 1) * long(pow(26.0 , double(rFieldName.GetLength() - i - 1)));
	}

	return (ret);
}

/******************************************************************************
	@brief		read line data mapping data
    @author     humkyung
    @date       2012-08-23
    @class      CLineListDataImportDlg
    @function   GetLineDataMappingData
    @return     int
    @param      map<CString,CString>& oMappingDataEntry
	@param      CAppPostgreSQL& database
******************************************************************************/
int CProjectDefTableDlg::GetLineDataMappingData(map<CString,CString>& oMappingDataEntry , CAppPostgreSQL& database)
{
	try
	{
		soci::rowset<soci::row> rs(database.session()->prepare << _T("select * from t_line_list_data_mapping"));
		for(soci::rowset<soci::row>::const_iterator itr = rs.begin();itr != rs.end();++itr)
		{
			for(int col = 0;col < int(itr->size());++col)
			{
				try
				{
					const STRING_T sName = itr->get_properties(col).get_name();
					const STRING_T sValue = itr->get<STRING_T>(col);
					if(!sValue.empty()) oMappingDataEntry.insert(make_pair(sName.c_str() , sValue.c_str()));
				} catch(...){}
			}
			break;
		}
		return ERROR_SUCCESS;
	}
	catch(...) { }

	return ERROR_BAD_ENVIRONMENT;
}

/**
	@brief	display current line data table
	@author	humkyung
	@date	2010.03.09
	@return
*/
int CProjectDefTableDlg::DisplayCurrentLineData(CAppPostgreSQL &database)
{
	/// delete columns
	m_wndCurrentLineDataListCtrl.SetRedraw(FALSE);
	m_wndCurrentLineDataListCtrl.DeleteAllItems();
	const int nColCount = m_wndCurrentLineDataListCtrl.GetHeaderCtrl()->GetItemCount();
	for(int i = 0;i < nColCount;++i)
	{
		m_wndCurrentLineDataListCtrl.DeleteColumn(0);
	}

	///
	CString rExecPathName = CAdminDocData::GetExecPath();

	vector<STRING_T> oColumnNameList;
	{
		soci::rowset<soci::row> rs(database.session()->prepare << _T("select column_name from information_schema.columns where table_schema='public' and table_name='t_line_list_data'"));
		for(soci::rowset<soci::row>::const_iterator itr = rs.begin();itr != rs.end();++itr) oColumnNameList.push_back(itr->get<STRING_T>(0));
	}

	/// reference key를 가져옵니다.
	STRING_T sDatabaseRefKey;
	vector<STRING_T> oRefKeyResult;
	try
	{
		(*database.session()) << _T("SELECT DATABASE_REFERENCE_KEY FROM T_PROJECT_SETTING") , soci::into(sDatabaseRefKey);
		CTokenizer<CIsFromString>::Tokenize(oRefKeyResult , sDatabaseRefKey , CIsFromString(_T(" ~ ")));
	}
	catch(const std::exception& ex)
	{
		CString str;
		str.Format(_T("%s(%d)=%s") , __FILE__,__LINE__,ex.what());
		AfxMessageBox(str,MB_OK|MB_ICONERROR|MB_TOPMOST);
	}

	list<CString> oDuplicatedLineManDataList;	/// 2011.06.23 added by humkyung

	try
	{
		int nIndex = 0;
		soci::rowset<soci::row> rs(database.session()->prepare << _T("SELECT * FROM T_LINE_LIST_DATA"));

		LONG lFieldNo = 0L;
		map<LONG , CString> ColumnMap;
		for(vector<STRING_T>::const_iterator itr = oColumnNameList.begin();itr != oColumnNameList.end();++itr)
		{
			ColumnMap.insert(make_pair(lFieldNo++ , itr->c_str()));
		}
		
		for(map<LONG,CString>::const_iterator itr = ColumnMap.begin();itr != ColumnMap.end();++itr)
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
			for(map<LONG,CString>::const_iterator itr = ColumnMap.begin();itr != ColumnMap.end();++itr)
			{
				rValue = (soci::i_ok == rtr->get_indicator(itr->second.operator LPCTSTR())) ? rtr->get<STRING_T>(itr->second.operator LPCTSTR()) : _T("");
				const STRING_T sColName(itr->second.operator LPCTSTR());
				vector<STRING_T>::const_iterator where = find_if(oRefKeyResult.begin() , oRefKeyResult.end() , [sColName](const STRING_T& param){return (0 == STRICMP_T(sColName.c_str(),param.c_str()));});
				if(where != oRefKeyResult.end())
				{
					rKey += rValue;
				}

				if(0 == nIndex)
				{
					LVITEM lvitem;
					memset(&lvitem , '\0' , sizeof(LVITEM));
					lvitem.mask = LVIF_TEXT;
					lvitem.iItem = m_wndCurrentLineDataListCtrl.GetItemCount();
					lvitem.iSubItem = 0;
					lvitem.pszText = (char*)(rValue.c_str());
					lvitem.iImage = 0; 
					const int at = m_wndCurrentLineDataListCtrl.InsertItem(&lvitem);
				}
				else
				{
					m_wndCurrentLineDataListCtrl.SetItemText(m_wndCurrentLineDataListCtrl.GetItemCount()-1,itr->first,rValue.c_str());
				}
				
				++nIndex;
			}

			/// 지금 표기한 RECORD가 이전에 입력한 RECORD와 같은 KEY를 가지고 있다면 텍스트를 붉게 표기한다.
			list<CString>::iterator where = find(KeyList.begin() , KeyList.end() , rKey.c_str());
			if(where != KeyList.end())
			{
				CString sDuplicatedLineManData;
				const int nItem = m_wndCurrentLineDataListCtrl.GetItemCount() - 1;
				for(int j = 0;j < m_wndCurrentLineDataListCtrl.GetColumnCount() - 1;++j)
				{
					/// 2011.06.23 added by humkyung
					sDuplicatedLineManData += m_wndCurrentLineDataListCtrl.GetItemText(nItem , j) + _T(",");
					m_wndCurrentLineDataListCtrl.SetItemTextColor(nItem , j , RGB(255,0,0));
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
		AfxMessageBox(ex.what(),MB_OK|MB_ICONERROR|MB_TOPMOST);
	}

	m_wndCurrentLineDataListCtrl.SetRedraw(TRUE);

	return ERROR_SUCCESS;
}

/**
	@brief	callback function for keydown
	@author	humkyung
	@date	2011.07.07
	@return	void
*/
void CProjectDefTableDlg::OnKeydownListCurrentLineData(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	if ( VK_DELETE == pLVKeyDow->wVKey)
	{
		DeleteSelectedRecordIfDuplicated();
	} 
	
	*pResult = 0;
}

/**
	@brief	
	@author	humkyung
	@date	2011.07.07
	@return	void
*/
int CProjectDefTableDlg::DeleteSelectedRecordIfDuplicated()
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

				try
				{
					CAppPostgreSQL database(pDoc->GetCurProject()->name(),prop.Host.c_str(),prop.User.c_str(),prop.Password.c_str());

					CString sSql;
					sSql.Format(_T("DELETE FROM T_LINE_LIST_DATA WHERE %s") , sClause);
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

/******************************************************************************
    @author     humkyung
    @date       2012-05-25
    @class      CProjectDefTableDlg
    @function   OnHelpInfo
    @return     BOOL
    @param      HELPINFO*   pHelpInfo
    @brief
******************************************************************************/
BOOL CProjectDefTableDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	HWND hHtml = ::HtmlHelp(this->m_hWnd , CAdminDocData::GetExecPath() + _T("\\Help\\SmartISO.chm::/데이타베이스 테이블 생성.htm") , HH_DISPLAY_TOPIC , 0);

	return CFormView::OnHelpInfo(pHelpInfo);
}
