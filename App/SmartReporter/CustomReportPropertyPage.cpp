// CustomReportPropertyPage.cpp : implementation file
//

#include "stdafx.h"
#include "SmartReporter.h"
#include "CustomReportPropertyPage.h"
#include "enablebuddybutton.h"
#include "LineListToOraDocData.h"

#include "MainFrm.h"
#include "SmartReporterDoc.h"
#include "SmartReporterView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCustomReportPropertyPage property page

IMPLEMENT_DYNCREATE(CCustomReportPropertyPage, CPropertyPage)

CCustomReportPropertyPage::CCustomReportPropertyPage() : CPropertyPage(CCustomReportPropertyPage::IDD) , m_sUnit("INCH") , m_sDiscipline("1") , m_sStartRow("5")
{
	//{{AFX_DATA_INIT(CCustomReportPropertyPage)
		CLineListToOraDocData& docData = CLineListToOraDocData::GetInstance();
		m_sUnit = docData.GetProjectUnit();
	//}}AFX_DATA_INIT
}

CCustomReportPropertyPage::~CCustomReportPropertyPage()
{
}

void CCustomReportPropertyPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustomReportPropertyPage)
		DDX_Text(pDX , IDC_COMBO_HEAD_LABEL , m_sName);
		DDX_Text(pDX , IDC_COMBO_UNIT , m_sUnit);
		DDX_Text(pDX , IDC_EDIT_DISCIPLINE , m_sDiscipline);
		DDX_Text(pDX , IDC_EDIT_START_ROW , m_sStartRow);

		DDX_Control(pDX , IDC_BUTTON_ADD , m_btnAdd);
		DDX_Control(pDX , IDC_BUTTON_DELETE , m_btnDelete);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCustomReportPropertyPage, CPropertyPage)
	//{{AFX_MSG_MAP(CCustomReportPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_SQL_FILE, OnButtonSqlFile)
	ON_BN_CLICKED(IDC_BUTTON_EXCEL_FILE, OnButtonExcelFile)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustomReportPropertyPage message handlers

/**
	@brief	

	@author	humkyung

	@date	2011.05.10

	@param

	@return	BOOL
*/
BOOL CCustomReportPropertyPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	VERIFY( ::EnableBuddyButton( GetDlgItem(IDC_EDIT_SQL_FILE)->GetSafeHwnd() , GetDlgItem(IDC_BUTTON_SQL_FILE)->GetSafeHwnd() , BBS_RIGHT ) );
	VERIFY( ::EnableBuddyButton( GetDlgItem(IDC_EDIT_EXCEL_FILE)->GetSafeHwnd() , GetDlgItem(IDC_BUTTON_EXCEL_FILE)->GetSafeHwnd() , BBS_RIGHT ) );
	
	UpdateData(FALSE);

	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_UNIT);
	if(pComboBox) pComboBox->SelectString(-1 , m_sUnit);

	/// store report table names into combobox - 2011.08.12 added by humkyung
	CLineListToOraDocData& docData = CLineListToOraDocData::GetInstance();
	list<CCustomReportTable*>* pReportTableList = docData.CustomReportTableList();
	if(NULL != pReportTableList)
	{
		CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_HEAD_LABEL);
		for(list<CCustomReportTable*>::iterator itr = pReportTableList->begin();itr != pReportTableList->end();++itr)
		{
			pComboBox->AddString((*itr)->m_sName.c_str());
		}
	}
	/// up to here

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/**
	@brief	

	@author	humkyung

	@date	2011.05.10

	@param

	@return
*/
void CCustomReportPropertyPage::OnButtonSqlFile() 
{
	UpdateData(TRUE);

	if(!m_sUnit.IsEmpty())
	{
		CFileDialog dlg(TRUE,_T(""),_T("*.xml"),OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,
			_T("Xml document(*.xml)"));
		CString sInitialPath = GetAppDataPath() + _T("\\SmartISO\\SmartReporter\\") + m_sUnit;
		dlg.m_ofn.lpstrInitialDir = sInitialPath;
		if( IDOK == dlg.DoModal() )
		{
			GetDlgItem(IDC_EDIT_SQL_FILE)->SetWindowText(dlg.GetFileName());
		}
	}
	else
	{
		AfxMessageBox( _T("Please, set Unit.") );
		GetDlgItem(IDC_COMBO_UNIT)->SetFocus();
	}
}

/**
	@brief	

	@author	humkyung

	@date	2011.05.10

	@param

	@return
*/
void CCustomReportPropertyPage::OnButtonExcelFile() 
{
	UpdateData(TRUE);

	CFileDialog dlg(TRUE,_T(""),_T("*.xls"),OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,
		_T("Excel document(*.xls)"));
	CString sInitialPath = GetAppDataPath() + _T("\\SmartISO\\SmartReporter\\Excel");
	dlg.m_ofn.lpstrInitialDir = sInitialPath;
	if( IDOK == dlg.DoModal() )
	{
		GetDlgItem(IDC_EDIT_EXCEL_FILE)->SetWindowText(dlg.GetFileName());
	}
}

/**
	@brief	

	@author	humkyung

	@date	2011.05.10

	@param

	@return
*/
void CCustomReportPropertyPage::OnButtonAdd() 
{
	UpdateData(TRUE);

	CString sSqlFileName , sExcelFileName;
	GetDlgItemText(IDC_EDIT_SQL_FILE , sSqlFileName);
	GetDlgItemText(IDC_EDIT_EXCEL_FILE , sExcelFileName);
	if(!m_sName.IsEmpty() && !m_sUnit.IsEmpty() && !m_sStartRow.IsEmpty() && !sSqlFileName.IsEmpty() && !sExcelFileName.IsEmpty())
	{
		CLineListToOraDocData& docData = CLineListToOraDocData::GetInstance();
		if(false == docData.ExistCustomReportTableWith(m_sName.operator LPCTSTR()))
		{
			CCustomReportTable* pCustomReportTable = new CCustomReportTable;
			if(NULL != pCustomReportTable)
			{
				pCustomReportTable->m_sName = m_sName;
				pCustomReportTable->m_sUnit = m_sUnit;
				pCustomReportTable->m_sSqlFile = sSqlFileName;
				pCustomReportTable->m_sExcelFile = sExcelFileName;
				pCustomReportTable->m_sDiscipline = m_sDiscipline;
				pCustomReportTable->m_sStartRow = m_sStartRow;

				const STRING_T sSqlFilePath = STRING_T(GetAppDataPath().operator LPCTSTR()) + _T("\\SmartISO\\SmartReporter\\") + 
					STRING_T(m_sUnit.operator LPCTSTR()) + _T("\\") + pCustomReportTable->m_sSqlFile;
				if(_T("INCH") == m_sUnit)
				{
					pCustomReportTable->m_pSqlState[CCustomReportTable::INCH]->ParseSqlState(sSqlFilePath);
				}
				else
				{
					pCustomReportTable->m_pSqlState[CCustomReportTable::METRIC]->ParseSqlState(sSqlFilePath);
				}
				
				docData.AddCustomReportTable(pCustomReportTable);
				docData.SaveCustomDataReportList();

				AfxMessageBox( _T("Add custom report successfully!!!") );
			}
		}
		else
		{
			AfxMessageBox( _T("There is same name, please input another name.") );
			GetDlgItem(IDC_EDIT_HEAD_LABEL)->SetFocus();
		}
	}
	else
	{
		if(m_sName.IsEmpty())
		{
			AfxMessageBox( _T("First of all, set label") );
			GetDlgItem(IDC_EDIT_HEAD_LABEL)->SetFocus();
			return;
		}
		if(m_sUnit.IsEmpty())
		{
			AfxMessageBox(_T("First of all, set unit"));
			GetDlgItem(IDC_COMBO_UNIT)->SetFocus();
			return;
		}
		if(sSqlFileName.IsEmpty())
		{
			AfxMessageBox(_T("First of all, set sql file"));
			GetDlgItem(IDC_EDIT_SQL_FILE)->SetFocus();
			return;
		}
		if(sExcelFileName.IsEmpty())
		{
			AfxMessageBox(_T("First of all, set excel file"));
			GetDlgItem(IDC_EDIT_EXCEL_FILE)->SetFocus();
			return;
		}
		if(m_sStartRow.IsEmpty())
		{
			AfxMessageBox(_T("First of all, set start row"));
			GetDlgItem(IDC_EDIT_START_ROW)->SetFocus();
			return;
		}
	}
}

/**
	@brief	delete custom report which name given by user

	@author	humkyung

	@date	2011.05.10

	@param

	@return
*/
void CCustomReportPropertyPage::OnButtonDelete() 
{
	UpdateData(TRUE);

	if(!m_sName.IsEmpty())
	{
		CLineListToOraDocData& docData = CLineListToOraDocData::GetInstance();
		if(true == docData.ExistCustomReportTableWith(m_sName.operator LPCTSTR()))
		{
			if(IDYES == AfxMessageBox(_T("Do you want to delete?") , MB_YESNO|MB_ICONQUESTION))
			{
				docData.DeleteCustomReportTableWith(m_sName);
				docData.SaveCustomDataReportList();

				CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_HEAD_LABEL);
				const int iFound = pComboBox->FindString(-1 , m_sName);
				if(-1 != iFound)
				{
					pComboBox->DeleteString(iFound);
				}

				AfxMessageBox(_T("Delete custom report successfully!!!"));
			}
		}
		else
		{
			AfxMessageBox(_T("Can'f find the name, please input another name to delete"));
			GetDlgItem(IDC_EDIT_HEAD_LABEL)->SetFocus();
		}
	}
	else
	{
		if(m_sName.IsEmpty())
		{
			AfxMessageBox(_T("Input label for report to delete"));
			GetDlgItem(IDC_EDIT_HEAD_LABEL)->SetFocus();
			return;
		}
	}
}