// DrawingReportSettingDlg.cpp: implementation of the CDrawingReportSettingDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <ProjectData.h>
#include "Admin.h"
#include "AdminDoc.h"
#include "AdminView.h"
#include "DrawingReportSettingDlg.h"
#include "enablebuddybutton.h"
#include "DrawingTypeGridCell.h"
#include "FilterGridCell.h"
#include "AdminDocData.h"
#include <FileTools.h>

#include <vector>
#include <tokenizer.h>
#include <SplitPath.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/******************************************************************************
    @author     humkyung
    @date       2012-08-15
    @function   OnBrowse
    @return     void
    @brief
******************************************************************************/
void CDrawingReportFileBrowseEdit::OnBrowse()
{
	CFileDialog dlg(TRUE,_T("xls,*.*"),_T(""),OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,
		_T("Excel file(*.xls)|*.xls|"));
	CSplitPath path(m_sDrawingReportFile);
	dlg.m_ofn.lpstrInitialDir = path.GetDrive() + path.GetDirectory();
	if(IDOK == dlg.DoModal())
	{
		m_sDrawingReportFile = dlg.GetPathName();
		this->SetWindowText(m_sDrawingReportFile);
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawingReportSettingDlg::CDrawingReportSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDrawingReportSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLineDataSettingDlg)
		m_pTT = NULL;
	//}}AFX_DATA_INIT
}

CDrawingReportSettingDlg::~CDrawingReportSettingDlg()
{
	SAFE_DELETE( m_pTT );
}

void CDrawingReportSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX , IDC_EDIT_DWG_REPORT_TEMPLATE , m_sDwgReportTemplateFilePath);
	DDX_Control(pDX , IDC_EDIT_DWG_REPORT_TEMPLATE , m_wndDrawingReportFileBrowser);
	DDX_Control(pDX,IDC_COMBO_NEXT_COLUMN,m_cboNextColumn);
	DDX_Control(pDX , IDC_COMBO_START_ROW,m_cboStartRow);
}

BEGIN_MESSAGE_MAP(CDrawingReportSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CLineDataSettingDlg)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_WM_HELPINFO()
	ON_EN_CHANGE(IDC_EDIT_DWG_REPORT_TEMPLATE, &CDrawingReportSettingDlg::OnEnChangeEditDwgReportTemplate)
	ON_CBN_SELENDOK(IDC_COMBO_START_ROW, &CDrawingReportSettingDlg::OnCbnSelendokComboStartRow)
END_MESSAGE_MAP()

/**	
	@brief	setting 내용을 저장한다.
	@author	HumKyung
	@date	????.??.??
	@param
	@return	void	
*/
void CDrawingReportSettingDlg::Save()
{
	CProject* pProject = m_pAdminDoc->GetCurProject();
	CString rIniFilePath = pProject->path() + CString( _T("\\Setting\\") ) + pProject->name() + CString(_T(".ini"));
	CString revFirst,revEnd,revCombo;
	
	CString sStartRow(_T("1"));
	int at = m_cboStartRow.GetCurSel();
	if(-1 != at) m_cboStartRow.GetLBText(at,sStartRow);
	const int iStartRow = ATOI_T(sStartRow.operator LPCTSTR());

	map<STRING_T,STRING_T>* pDrawingSetup = CProjectData::GetInstance()->GetDrawingSetup();
	CEdit *pEdit = NULL;
	const static CString sApp(_T("Drawing Excel Matching"));
	int nWriteCount = 0;
	int nConut = m_wndDrawingReportCtrl.GetColumnCount();
	for( int i =0;i<nConut;++i)
	{
		CString rCellName = m_wndDrawingReportCtrl.GetItemText(0,i + 1);
		CString rName= m_wndDrawingReportCtrl.GetItemText(iStartRow , i + 1);
		if(rName.IsEmpty()) continue;
				
		CString sExlNum;
		CString sValue = rCellName + _T(":") + rName;
		sExlNum.Format(_T("drawing report name%d") ,nWriteCount);
		(*pDrawingSetup)[sExlNum.operator LPCTSTR()] = sValue.operator LPCTSTR();
		++nWriteCount;
	}

	CString sValue,revCount;
	sValue.Format(_T("%d") , nWriteCount );
	(*pDrawingSetup)[_T("drawing report count")] = sValue.operator LPCTSTR();
	(*pDrawingSetup)[_T("drawing report row")] = sStartRow.operator LPCTSTR();

	CString sNextColumn(_T("A"));
	at = m_cboNextColumn.GetCurSel();
	if(-1 != at) m_cboNextColumn.GetLBText(at,sNextColumn);
	(*pDrawingSetup)[_T("drawing report column")] = sNextColumn.operator LPCTSTR();

	/// set project setting data - 2011.09.22 added by humkyung
	CProjectData* pProjectData = CProjectData::GetInstance();
	if(pProjectData)
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_ONE_SHEET);
		if(pButton)
		{
			map<CString,CString>::const_iterator where = pProjectData->m_pProjectSettingMap->find(_T("one_sheet_drawing_report"));
			if(where != pProjectData->m_pProjectSettingMap->end())
			{
				(*(pProjectData->m_pProjectSettingMap))[_T("one_sheet_drawing_report")] = (TRUE == pButton->GetCheck()) ? _T("ON") : _T("OFF");
			}
		}

		CString sString;
		GetDlgItemText(IDC_EDIT_ROW_COUNT_PER_SHEET , sString);
		map<CString,CString>::iterator where = pProjectData->m_pProjectSettingMap->find(_T("row_count_per_sheet"));
		if(where != pProjectData->m_pProjectSettingMap->end())
		{
			(*(pProjectData->m_pProjectSettingMap))[_T("row_count_per_sheet")] = sString;
		}
	}
	/// up to here
}

int CDrawingReportSettingDlg::FindGridColumn(const string& rLabel)
{
	size_t nColumnCount = m_wndDrawingReportCtrl.GetColumnCount();
	try
	{	
		for(size_t i = 0;i < nColumnCount;++i)
		{
			string desc = m_wndDrawingReportCtrl.GetItemText(0 , i);
			if(rLabel == desc ) return i;
		}
	}
	catch(...)
	{
		return -1;
	}

	return -1;
}

/**	
	@brief	PROJECT가 로드되고 난 뒤에 호출되어야 한다.
	@author	HumKyung
	@date	????.??.??
	@param	
	@return	void	
*/
void CDrawingReportSettingDlg::UpdateContents()
{
	CProject *pProject = m_pAdminDoc->GetCurProject();
	if(pProject!=NULL)
	{
		m_DataNameEntry.clear();
		GridCell::CDrawingTypeGridCell::GetDataNameEntry( m_DataNameEntry );
		m_DataNameEntry.push_back( _T("LAST REV NO") );

		CString rIniFilePath = pProject->path() + CString("\\Setting\\") + pProject->name() + CString(".ini");
		CString rApp = _T("Drawing Excel Matching");
		TCHAR szBuf[MAX_PATH] = {'0',};
		CString sExlPath = pProject->path() + CString("\\PRODUCT\\INDEX\\") + pProject->name() + CString(".xls");
		SetDlgItemText(IDC_EDIT_DRAWINGREPORT , sExlPath);
		
		map<STRING_T,STRING_T>* pDrawingSetup = CProjectData::GetInstance()->GetDrawingSetup();
		map<STRING_T,STRING_T>::const_iterator where = pDrawingSetup->find(_T("drawing report row"));
		if(where != pDrawingSetup->end())
		{
			const int at = m_cboStartRow.FindStringExact(0,where->second.c_str());
			(-1 != at) ? m_cboStartRow.SetCurSel(at) : m_cboStartRow.SetCurSel(0);
		}
		else
		{
			m_cboStartRow.SetCurSel(0);
		}
		OnCbnSelendokComboStartRow();

		where = pDrawingSetup->find(_T("drawing report column"));
		if(where != pDrawingSetup->end())
		{
			const int at = m_cboNextColumn.FindStringExact(0,where->second.c_str());
			(-1 != at) ? m_cboNextColumn.SetCurSel(at) : m_cboNextColumn.SetCurSel(0);
		}

		/// set "ONE SHEET" button - 2011.09.22 added by humkyung
		CProjectData* pProjectData = CProjectData::GetInstance();
		if(pProjectData)
		{
			CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_ONE_SHEET);

			map<CString,CString>::iterator where = pProjectData->m_pProjectSettingMap->find(_T("one_sheet_drawing_report"));
			if(where != pProjectData->m_pProjectSettingMap->end())
			{	
				pButton->SetCheck( 0 == where->second.CompareNoCase(_T("ON")) ); 
			}
			else
			{
				pButton->SetCheck( FALSE );
			}

			where = pProjectData->m_pProjectSettingMap->find(_T("row_count_per_sheet"));
			if(where != pProjectData->m_pProjectSettingMap->end())
			{	
				if(where->second.IsEmpty()) where->second = _T("30");
				SetDlgItemText( IDC_EDIT_ROW_COUNT_PER_SHEET , where->second);
			}
			else
			{
				SetDlgItemText( IDC_EDIT_ROW_COUNT_PER_SHEET , _T("30"));
			}
		}
		/// up to here

		/// 2012.08.15 added by humkyung
		const CString sApp(_T("Drawing Report"));
		if(GetPrivateProfileString(sApp , _T("Excel template file path") , NULL , szBuf , MAX_PATH , rIniFilePath))
		{
			m_sDwgReportTemplateFilePath = szBuf;
			m_wndDrawingReportFileBrowser.m_sDrawingReportFile = szBuf;
			UpdateData(FALSE);
		}
		/// up to here
	}	
}

BOOL CDrawingReportSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	HWND hwndEdit  = GetDlgItem(IDC_EDIT_DRAWINGREPORT)->GetSafeHwnd();
	static bool __init = false;

	if(false == __init)
	{		
		BOOL b = m_wndDrawingReportCtrl.Create(CRect(0 , 0 , 0 , 0) , this , 0x100);
		if(TRUE == b)
		{
			/// 그리드 컨트롤 폰트 코드
			CFont font;
			font.CreateFont(-11 , 0 , 0 , 0 , 400 , FALSE,FALSE,FALSE,HANGUL_CHARSET,3,2,1,VARIABLE_PITCH | FF_MODERN, _T("Arial")); 
			m_wndDrawingReportCtrl.SetFont(&font);
			font.DeleteObject();

			// fill it up with stuff
			m_wndDrawingReportCtrl.SetEditable( TRUE );
			m_wndDrawingReportCtrl.EnableDragAndDrop(FALSE);
			m_wndDrawingReportCtrl.SetRowResize(FALSE);
			m_wndDrawingReportCtrl.SetColumnResize(TRUE);
			m_wndDrawingReportCtrl.SetListMode(FALSE);
			m_wndDrawingReportCtrl.SetSingleRowSelection(TRUE);
			m_wndDrawingReportCtrl.SetHeaderSort(FALSE);
			m_wndDrawingReportCtrl.SetSingleColSelection(TRUE);
		
			m_wndDrawingReportFileBrowser.EnableFileBrowseButton(); /// To show file open dialog
		}
		else
		{
			AfxMessageBox("Can't create grid control!!!");
		}

		m_pTT = new CToolTipCtrl;
		if(m_pTT)
		{
			/// Add below code
			m_pTT->Create(this, TTS_BALLOON|WS_POPUP|WS_EX_TOOLWINDOW);
			m_pTT->AddTool(&m_cboNextColumn,_T("'A'이면 한 줄로 출력합니다.") );
		}

		__init = true;
	}
	return TRUE;
}

/******************************************************************************
    @author     humkyung
    @date       ????-??-??
    @function   OnSize
    @return     void
    @param      UINT    nType
    @param      int     cx
    @param      int     cy
    @brief
******************************************************************************/
void CDrawingReportSettingDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	/// 2012.08.15 added by humkyung
	CWnd* pWnd = GetDlgItem(IDC_EDIT_DWG_REPORT_TEMPLATE);
	if(pWnd)
	{
		CRect rect;
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);
		pWnd->SetWindowPos(NULL , 0 , rect.top , cx - rect.left - 5 , rect.Height() , SWP_NOMOVE | SWP_NOZORDER);
	}
	/// up to here

	pWnd = GetDlgItem(IDC_STATIC_DRAWINGREPORT);
	if(pWnd)
	{
		CRect rect;
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);

		CWnd* pWnd = GetDlgItem(IDC_EDIT_DRAWINGREPORT);
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);
		pWnd->SetWindowPos(NULL , 0 , rect.top , cx - rect.left - 5 , rect.Height() , SWP_NOMOVE | SWP_NOZORDER);
	}

	if(m_wndDrawingReportCtrl.GetSafeHwnd())
	{
		CRect rect;
		pWnd = GetDlgItem(IDC_STATIC_SETTING_BOX);
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);

		m_wndDrawingReportCtrl.SetWindowPos(NULL , 15 , rect.bottom + 10 , cx - 20 ,  (cy - rect.bottom - 20) , SWP_NOZORDER);
		m_wndDrawingReportCtrl.GetWindowRect( &rect );
		ScreenToClient( &rect );
	}
}

BOOL CDrawingReportSettingDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(NULL != m_pTT) m_pTT->RelayEvent((LPMSG)pMsg);

	if( pMsg->message == WM_KEYDOWN ) // Enter Key Prevent.. so good.. ^^		
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE )		
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			
			return TRUE;                    // DO NOT process further	
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

/**
	@brief	
	@author	humkyung
	@date		2012.06.05
*/
BOOL CDrawingReportSettingDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	HWND hHtml = ::HtmlHelp(this->m_hWnd , CAdminDocData::GetExecPath() + _T("\\Help\\SmartISO.chm::/Drawing Index Setting.htm") , HH_DISPLAY_TOPIC , 0);

	return TRUE;///CDialog::OnHelpInfo(pHelpInfo);
}

/******************************************************************************
	@brief		save excel file info for drawing report
    @author     humkyung
    @date       2012-08-15
    @function   SaveExcelData
    @return     int
******************************************************************************/
int CDrawingReportSettingDlg::SaveExcelData(const CString& sIniFilePath)
{
	const CString sApp(_T("Drawing Report"));
	WritePrivateProfileString(sApp , _T("Excel template file path") , m_sDwgReportTemplateFilePath , sIniFilePath);

	/// save excel file on database - 2012.08.15 added by humkyung
	{
		CAdminDoc* pDoc = CAdminDoc::GetInstance();
		const CProject::DatabaseProp prop  = pDoc->GetCurProject()->GetDatabaseProp();

		try
		{
			CAppPostgreSQL database(pDoc->GetCurProject()->name() , prop.Host.c_str() , prop.User.c_str() , prop.Password.c_str());
			soci::transaction txn(*database.session());
			{
				soci::blob b(*database.session());
				(*database.session()) << _T("SELECT DWG_REPORT_EXCEL_FILE_DATA FROM T_PROJECT_SETTING") , soci::into(b);
				CFile f(m_sDwgReportTemplateFilePath , CFile::modeRead);
				const ULONG fSize = f.GetLength();
				char* pData = (char*)calloc(1 , sizeof(char)*fSize);
				if(pData)
				{
					f.Read(pData,fSize);
					f.Close();
					
					b.write(0,pData,sizeof(char)*fSize);
					free((void*)pData);
				}
			}
			txn.commit();

			return ERROR_SUCCESS;
		}
		catch(const std::exception& ex)
		{
			AfxMessageBox(ex.what(),MB_OK|MB_ICONERROR);
		}
	}
	/// up to here

	return ERROR_BAD_ENVIRONMENT;
}

/******************************************************************************
    @author     humkyung
    @date       2012-08-15
    @class      CDrawingReportSettingDlg
    @function   OnEnChangeEditDwgReportTemplate
    @return     void
    @brief
******************************************************************************/
void CDrawingReportSettingDlg::OnEnChangeEditDwgReportTemplate()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateData();

	if(CFileTools::DoesFileExist(m_sDwgReportTemplateFilePath))
	{
		CProject* pProject = m_pAdminDoc->GetCurProject();
		CString rIniFilePath = pProject->path() + CString( _T("\\Setting\\") ) + pProject->name() + CString(_T(".ini"));
		this->SaveExcelData(rIniFilePath);
	}

	UpdateData(FALSE);
}

/**
	@brief	redraw grid control
	@author	humkyung
	@date	2015.10.28
*/
void CDrawingReportSettingDlg::OnCbnSelendokComboStartRow()
{
	static const TCHAR* __pColumnLabels[]=
	{
		_T("A") , _T("B") , _T("C") , _T("D") , _T("E") , _T("F") , _T("G") , _T("H") , _T("I") , _T("J") , _T("K") , _T("L") , _T("M") , _T("N") , 
		_T("O") , _T("P") , _T("Q") , _T("R") , _T("S") , _T("T") , _T("U") , _T("V") , _T("W") , _T("X") , _T("Y") , _T("Z")
	};
	
	const COLORREF oBkColor = RGB(255, 255, 224);
	CProject *pProject = m_pAdminDoc->GetCurProject();
	if(NULL != pProject)
	{
		CString rIniFilePath = pProject->path() + CString(_T("\\Setting\\")) + pProject->name() + CString(_T(".ini"));
		CString rApp = _T("Drawing Excel Matching");
		TCHAR szBuf[MAX_PATH] = {'0',};

		/// try to close open child window - 2012.05.08 added by humkyung
		for(int i = 1;i < m_wndDrawingReportCtrl.GetRowCount();++i)
		{
			for(int j = 1;j < m_wndDrawingReportCtrl.GetColumnCount();++j)
			{
				CGridCellBase* pCell = m_wndDrawingReportCtrl.GetCell(i , j);
				pCell->EndEdit();
			}
		}
		/// up to here
		m_wndDrawingReportCtrl.DeleteAllItems();
		const int nColCount = sizeof(__pColumnLabels) / sizeof(const TCHAR*);
		m_wndDrawingReportCtrl.SetRowCount(30);
		m_wndDrawingReportCtrl.SetColumnCount(nColCount + 1);
		m_wndDrawingReportCtrl.SetFixedColumnCount(1);
		m_wndDrawingReportCtrl.SetFixedRowCount(1);
		for(int i = 0;i < nColCount;++i)
		{
			m_wndDrawingReportCtrl.SetItemFormat(0, i + 1, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_wndDrawingReportCtrl.SetItemText(0 , i + 1, __pColumnLabels[i]);
		}
		for(int i = 1;i < m_wndDrawingReportCtrl.GetRowCount();++i)
		{
			m_wndDrawingReportCtrl.SetItemFormat(i, 0 , DT_CENTER|DT_VCENTER|DT_SINGLELINE);

			CString sString;
			sString.Format(_T("%d") , i);
			m_wndDrawingReportCtrl.SetItemText(i , 0 , sString);
		}

		CString sStartRow;
		const int at = m_cboStartRow.GetCurSel();
		m_cboStartRow.GetLBText(at,sStartRow);
		const int iStartRow = ATOI_T(sStartRow.operator LPCTSTR());
		for(int i = 1;i < m_wndDrawingReportCtrl.GetRowCount();++i)
		{
			for(int j = 1;j < m_wndDrawingReportCtrl.GetColumnCount();++j)
			{
				if(i != iStartRow)
				{
					CGridCellBase* pCell = m_wndDrawingReportCtrl.GetCell(i , j);
					pCell->SetState( GVNI_READONLY );
				}
				else
				{
					m_wndDrawingReportCtrl.SetCellType(i , j , RUNTIME_CLASS(GridCell::CDrawingTypeGridCell));
					m_wndDrawingReportCtrl.GetCell(i , j)->SetBackClr(oBkColor);
				}
			}
		}

		map<STRING_T,STRING_T>* pDrawingSetup = CProjectData::GetInstance()->GetDrawingSetup();
		int revCount = 0,nComboRevCount=0;
		map<STRING_T,STRING_T>::const_iterator where = pDrawingSetup->find(_T("drawing report count"));
		const int nCount = (where != pDrawingSetup->end()) ? ATOI_T(where->second.c_str()) : 0;

		vector<STRING_T> rDrawReport;
		CString str;
		for(int k=0;k<nCount;++k)
		{
			str.Format(_T("drawing report name%d") , k);		
			where = pDrawingSetup->find(str.operator LPCTSTR());
			if(where != pDrawingSetup->end())
			{
				CTokenizer<CIsFromString>::Tokenize(rDrawReport ,where->second.c_str(), CIsFromString(":"));		
				m_wndDrawingReportCtrl.SetItemText(iStartRow , FindGridColumn(rDrawReport[0]),rDrawReport[1].c_str());
			}
		}
	}

	m_wndDrawingReportCtrl.AutoSizeRows();
	m_wndDrawingReportCtrl.AutoSizeColumn(0);
}
