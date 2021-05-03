// EditLineDataForm.cpp : implementation file
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <direct.h>

#include <DgnDocument.h>
#include <DGNEnum.h>
#include <util/FileTools.h>
#include <ProjectData.h>

#include "UserApp.h"
#include "MainFrm.h"
#include "UserAppDocData.h"
#include "tokenizer.h"
#include "UserAppView.h"
#include "EditLineDataForm.h"
#include "SortSettingDlg.h"
#include "GetDataFromIDFStartUpDlg.h"
#include "LineDataEditDlg.h"

#include "ResourceSwitch.h"
#include "IsoEditSettingPage.h"

#include "PdTable121.h"
#include "PdTable122.h"
#include <idflib/idflib/IDFFile.h>
#include <comdef.h>
#include <windows.h>
#include "IsString.h"
#include "WorkStatusDlg.h"
#include "ReplaceDlg.h"
#include <ado/SQLFile.h>

#include <gui/EnableBuddyButton.h>
#include <gui/FolderDlg/FolderDlg.h>
#include <gui/ListCtrlExComboBox.h>
#include <gui/ListCtrlExEdit.h>

#include "soci.h"
#include "soci-sqlite3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	CANCELLATION_ITEM_COLOR	RGB(177 , 177 , 177)
#define	FIXED_COLUMN_COLOR		RGB(220 , 220 , 220)
#define	FIXED_ITEM_COLOR		RGB(150 , 150 , 150)
#define	IS_FIXED_COLUMN(col)	((CString(_T("[]")) == col) || (CString(_T("KEY")) == col) || (CString(_T("REV_NO")) == col) || (CString(_T("SITE")) == col))
#define	REVISION_DATA_ICON	4
#define	ARROW_EXPAND		14
#define	ARROW_LINE			15
#define	ARROW_EMPTY			16

/**
	@brief	display editor if image index is not 0
	@author	humkyung
	@date	2013.02.03
*/
BOOL CEditLineDataListCtrl::DisplayEditor(int nItem, int nSubItem)
{
	LVITEM lvItem;
	lvItem.iItem = nItem;
	lvItem.iSubItem = 0;
	lvItem.mask = LVIF_IMAGE;
	this->GetItem(&lvItem);

	return CListCtrlEx::DisplayEditor(nItem , nSubItem);
}

/////////////////////////////////////////////////////////////////////////////
// CEditLineDataForm

IMPLEMENT_DYNCREATE(CEditLineDataForm, CFormView)

CEditLineDataForm::CEditLineDataForm()
	: CFormView(CEditLineDataForm::IDD) , m_pLineDataRecordSet(NULL) , m_pQualifiedLineForm(NULL) ,m_nRecordCount(0) , m_pWndInPlaceEdit(NULL)
{
	//{{AFX_DATA_INIT(CEditLineDataForm)
		m_pWndInPlaceComboBox = NULL;
	//}}AFX_DATA_INIT
}

CEditLineDataForm::~CEditLineDataForm()
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

void CEditLineDataForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditLineDataForm)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_STATIC_SELECTED_COUNT, m_cStaticLabel);
	DDX_Text(pDX , IDC_EDIT_ISO_FOLDER , m_sSourceIsoFileFolder);
	//}}AFX_DATA_MAP
}

#ifndef WM_KICKIDLE
#define WM_KICKIDLE 0x036A
#endif

BEGIN_MESSAGE_MAP(CEditLineDataForm, CFormView)
	//{{AFX_MSG_MAP(CEditLineDataForm)
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_LINE_LIST, OnDblclkListLineList)
	ON_NOTIFY(NM_RCLICK , IDC_LIST_LINE_LIST , OnRclickLineList)
	ON_UPDATE_COMMAND_UI(ID_LINE_DATA_MODIFY, OnUpdateLineDataModify)
	ON_BN_CLICKED(IDC_BUTTON_SORT, OnButtonSort)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_ISO_FOLDER, OnButtonIsoFolder)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_ISO_FOLDER , OnUpdateIsoFolder)	/// 2012.08.13 added by humkyung
	ON_COMMAND(ID_LINE_DATA_DELETE, OnLineDataDelete)				/// 2011.02.24 added by HumKyung
	ON_COMMAND(ID_LINE_DATA_PROPERTIES , OnProperties)				/// 2011.02.24 added by HumKyung
	ON_COMMAND(ID_LINE_DATA_REPLACE , OnReplace)					/// 2013.08.24 added by HumKyung
	//}}AFX_MSG_MAP
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST_LINE_LIST , OnLvnEndlabeledit)
	ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_LIST_LINE_LIST , OnLvnBeginlabeledit)
	ON_WM_HELPINFO()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CLICK, IDC_LIST_LINE_LIST, &CEditLineDataForm::OnNMClickListLineList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_LINE_LIST, &CEditLineDataForm::OnLvnItemchangedListLineList)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditLineDataForm diagnostics

#ifdef _DEBUG
void CEditLineDataForm::AssertValid() const
{
	CFormView::AssertValid();
}

void CEditLineDataForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEditLineDataForm message handlers

BOOL CEditLineDataForm::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	CUserAppApp* pApp = (CUserAppApp*)AfxGetApp();
	///CResourceSwitch resSwitch(pApp->m_hMultiLangRes);

	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

/**	
	@brief	폼 초기화 부분
	@author	humkyung
	@date	????.??.??
	@param	
	@return	void	
*/
void CEditLineDataForm::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
		
	static bool __init = false;	
	if(false == __init)
	{		
		CRect rectDummy;
		rectDummy.SetRectEmpty ();
		GetClientRect(rectDummy);
		
		CUserAppView* pView = (CUserAppView*)(GetParent());
		CUserAppDoc*  pDoc  = pView->GetDocument();
		CAppDocData* pDocData = CAppDocData::GetInstance();
		
		SetDlgItemText(IDC_STATIC_SOURCE_ISO_FOLDER , _TR("Source Iso Folder") + _T(" : "));
		HWND hwndEdit  = GetDlgItem(IDC_EDIT_ISO_FOLDER)->GetSafeHwnd();
		HWND hwndButton= GetDlgItem(IDC_BUTTON_ISO_FOLDER)->GetSafeHwnd();
		VERIFY( ::EnableBuddyButton( hwndEdit, hwndButton, BBS_RIGHT ) );

		if(m_wndLineDataReport.SubclassWindow(GetDlgItem(IDC_LIST_LINE_LIST)->m_hWnd))
		{
		}
		m_CheckImageList.Create(IDB_CHECKBOXES, 16, 3, RGB(255,0,255));
		int i = m_CheckImageList.GetImageCount();
		m_wndLineDataReport.SetHeaderImageList(&m_CheckImageList);
		m_wndLineDataReport.SetExtendedStyle(LVS_EX_CHECKBOXES | m_wndLineDataReport.GetExtendedStyle());
		/////////////////////////////////////////////

		m_cStaticLabel.SetColor(RGB(240,240,240));
		m_cStaticLabel.SetGradientColor(RGB(240,240,240));

		__init = true;
	}
}

/**
	@brief	overrive for calling UpdateDialogControls
	@author	HumKyung	
	@date	2011.01.19
	@param
	@return
*/
BOOL CEditLineDataForm::PreTranslateMessage(MSG* pMsg) 
{
	UpdateDialogControls(this, TRUE);
	
	return CFormView::PreTranslateMessage(pMsg);
}

/**	
	@brief	MonitorFileCreates(LPVOID pVoid)
	@author	humkyung
	@description	
	@remarks	
	@reutrns	
*/
static DWORD WINAPI MonitorFileCreates(LPVOID pVoid)
{
	string x = (LPSTR)(pVoid);
	SHFILEINFO sfi;
	do
	{
		if(0 != SHGetFileInfo(x.c_str() , FILE_ATTRIBUTE_NORMAL , &sfi , sizeof(sfi) , SHGFI_ATTRIBUTES))
		{
			break;
		}
		::Sleep(100);
	}while(1);

	return 0;
}

/**
	@brief	window size is changed
	@author	humkyung
**/
void CEditLineDataForm::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	if(m_wndLineDataReport.GetSafeHwnd())
	{
		CRect rect;
		{
			GetDlgItem(IDC_STATIC_SOURCE_ISO_FOLDER)->GetWindowRect(&rect);
			ScreenToClient(&rect);
		}

		m_wndLineDataReport.SetWindowPos(NULL , 0 , rect.bottom + 10 , 
			cx , (int)(cy - rect.bottom - 10) , SWP_NOZORDER);
		m_wndLineDataReport.GetWindowRect(&rect);
		ScreenToClient(&rect);

		CWnd* pWnd = GetDlgItem(IDC_EDIT_ISO_FOLDER);
		if(pWnd)
		{
			CRect thisRect , sortRect , staticRect;
			CWnd* pWnd = GetDlgItem(IDC_STATIC_SOURCE_ISO_FOLDER);
			if( pWnd )
			{
				pWnd->GetWindowRect(&staticRect);
				ScreenToClient(&staticRect);
				/*pWnd->SetWindowPos(NULL , 
					sortRect.right + 10 , int(sortRect.top + (sortRect.Height() - staticRect.Height()) * 0.5) , 0 , 0 , SWP_NOSIZE);*/
			}
			pWnd->GetWindowRect(&staticRect);
			ScreenToClient(&staticRect);

			pWnd = GetDlgItem(IDC_EDIT_ISO_FOLDER);
			pWnd->GetWindowRect(&thisRect);
			ScreenToClient(&thisRect);
			pWnd->SetWindowPos(NULL , staticRect.right + 5, thisRect.top , cx - (staticRect.right + 5) , thisRect.Height() , SWP_NOZORDER);
			pWnd->GetWindowRect( &thisRect );
			ScreenToClient(&thisRect);

			pWnd = GetDlgItem(IDC_BUTTON_ISO_FOLDER);
			if( pWnd )
			{
				CRect  buttonRect;
				pWnd->GetWindowRect(&buttonRect);
				ScreenToClient(&buttonRect);
				pWnd->SetWindowPos(NULL , 
					thisRect.right - buttonRect.Width() - 3 , int(thisRect.top + (thisRect.Height() - buttonRect.Height()) * 0.5) , 0 , 0 , SWP_NOSIZE);
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/**	
	@brief	load line data from database
	@author	HumKyung
	@date	????.??.??
	@param	rQueryCondition
	@param	issueDates
	@return
*/
void CEditLineDataForm::LoadLineDataRecordSetFromDatabase(const CString& rQueryCondition , const vector< STRING_T >& issueDates , const bool& reload)
{
	CAppDocData*  pDocData  = CAppDocData::GetInstance();
	
	if(NULL != pDocData)
	{
		CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();

		if(NULL != m_pLineDataRecordSet)
		{
			CLineDataRecordSet::DeleteInstance(m_pLineDataRecordSet);
			m_pLineDataRecordSet = NULL;
		}

		BeginWaitCursor();

		CString rSelRevNo = pWorkspaceBar->m_wndQuery1.GetSelectedRevNo();
		if(rSelRevNo.IsEmpty() || (_T("ALL") == rSelRevNo) )
		{
			m_pLineDataRecordSet = pDocData->GetProject()->GetLineDataRecordSetFromDatabase(rQueryCondition , NULL , issueDates , reload );
		}
		else
		{
			m_pLineDataRecordSet = pDocData->GetProject()->GetLineDataRecordSetFromDatabase(rQueryCondition , rSelRevNo.operator LPCTSTR() , issueDates , reload);
		}

		if(NULL != m_pLineDataRecordSet)
		{		
			size_t nRecordCount = m_pLineDataRecordSet->GetLineDataRecordCount();
			if(0 == nRecordCount)
			{
				CString rMsg;
				rMsg.LoadString(IDS_NO_RECORD);
				AfxMessageBox(rMsg);
				EndWaitCursor();

				DisplayLineDataRecordSet();

				return;
			}

			DisplayLineDataRecordSet();
			if(m_pQualifiedLineForm) m_pQualifiedLineForm->ClearContents();
		}

		EndWaitCursor();
	}
}

/**	
	@brief	line data를 파일로 프린트한다.

	@author	HumKyung

	@param	pLineDataRecordEntry	a parameter of type vector<LineDataRecordMap>*

	@return	void	
*/
void CEditLineDataForm::PrintLineDataRecordEntry(vector<LineDataRecordMap>* pLineDataRecordEntry)
{
	assert(pLineDataRecordEntry && "pLineDataRecordEntry is NULL");

	if(pLineDataRecordEntry)
	{
		CUserAppView* pView = (CUserAppView*)(GetParent());
		CUserAppDoc*  pDoc  = pView->GetDocument();
		CAppDocData*  pDocData  = CAppDocData::GetInstance();

		CString rHeader , rString;
		CDataFieldSet* pLineDataFieldSet = pDocData->GetProject()->GetLineDataFieldSet();
		CDataFieldSet* pResDataFieldSet  = pDocData->GetProject()->GetResDataFieldSet();
		CDataFieldSet* pRevDataFieldSet  = pDocData->GetProject()->GetRevDataFieldSet();
		int nFieldCount = pLineDataFieldSet->GetFieldCount();
		int i = 0;
		for( i = 0;i < nFieldCount;++i)
		{
			CDataField* pDataField = pLineDataFieldSet->GetFieldAt( i );
			rString.Format(_T("%-50s") , pDataField->desc());
			rHeader += rString;
		}
		nFieldCount = pResDataFieldSet->GetFieldCount();
		for( i = 0;i < nFieldCount;++i)
		{
			CDataField* pDataField = pResDataFieldSet->GetFieldAt( i );
			rString.Format(_T("%-50s") , pDataField->desc());
			rHeader += rString;
		}
		nFieldCount = pRevDataFieldSet->GetFieldCount();
		for( i = 0;i < nFieldCount;++i)
		{
			CDataField* pDataField = pRevDataFieldSet->GetFieldAt( i );
			rString.Format(_T("%-50s") , pDataField->desc());
			rHeader += rString;
		}

		const CString pPath = CString(pDocData->GetProject()->path()) + _T("\\Temp\\LINE_LIST.txt");
		
		ofstream ofile;
		ofile.open(pPath);
		int count=0;
		CString rValue,rDesc,rData2;

		ofile  << rHeader.operator LPCTSTR() << std::endl;	//! 헤더 부분.
		for(vector<LineDataRecordMap>::iterator itr = pLineDataRecordEntry->begin();itr != pLineDataRecordEntry->end();itr++)
		{
			CLineDataRecord* pLineDataRecord = itr->pLineDataRecord;
			
			size_t nFieldCount = pLineDataRecord->GetFieldCount();
			int j=0;
			for(j=0;j < int(nFieldCount) ;++j)
			{
				CDataField* pLineDataField = pLineDataRecord->GetFieldAt(j);
				rValue.Format( _T("%-50s") ,pLineDataField->value());
				rValue.Replace( _T("\n") , _T("") );
				rData2 += rValue;
			}
			
			/// reserved data
			CResDataRecord* pResRecord = pLineDataRecord->GetResDataRecord();
			if(pResRecord)
			{	
				nFieldCount = pResRecord->GetFieldCount();
				for(j = 0;j < int(nFieldCount) ;++j)
				{
					CDataField* pDataField = pResRecord->GetFieldAt(j);
					rValue.Format( _T("%-50s") , pDataField->value());
					rValue.Replace( _T("\n") , _T("") );
					rData2 += rValue;
				}
			}
			else if(pResDataFieldSet)
			{
				//! RESERVED DATA가 없을때 빈 공란으로 PRINT하도록 한다.
				nFieldCount = pResDataFieldSet->GetFieldCount();
				for( i = 0;i < int(nFieldCount) ;++i)
				{
					rString.Format( _T("%-50s") , _T(" ") );
					rData2 += rString;
				}
			}

			/// 최신 revision data
			CRevDataRecordSet* pRevRecordSet = pLineDataRecord->GetRevDataRecordSet();
			if(pRevRecordSet)
			{			
				size_t nRecordCount = pRevRecordSet->GetRecordCount();
				if( nRecordCount > 0 )
				{
					CRevDataRecord* pRevRecord = pRevRecordSet->GetRecordAt( nRecordCount - 1 );
					if(pRevRecord)
					{
						nFieldCount = pRevRecord->GetFieldCount();
						for(j = 0;j < int(nFieldCount) ;++j)
						{
							CDataField* pDataField = pRevRecord->GetFieldAt(j);
							if(pDataField)
							{
								rValue.Format( _T("%-50s") ,pDataField->value());
								rValue.Replace( _T("\n") , _T("") );
								rData2 += rValue;
							}
						}
					}
				}
			}

			ofile << rData2.operator LPCTSTR() << std::endl;
			
			rData2.Empty();
		}

		ofile.close();
		AfxMessageBox( _T("Print Success") );
	}
}

/**	
	@brief	line data를 삭제한다.

	@author	HumKyung

	@date	????.??.??

	@param	pLineDataRecordEntry	a parameter of type vector<LineDataRecordMap>*

	@return	int	
*/
int CEditLineDataForm::DeleteLineDataRecordEntry(vector<LineDataRecordMap>* pLineDataRecordEntry)
{
	assert(pLineDataRecordEntry && "pLineDataRecordEntry is NULL");

	if(pLineDataRecordEntry)
	{
		CString rMsg;
		rMsg.LoadString(IDS_LINE_DATA_FORM_MSG2);
		int res = AfxMessageBox(rMsg , MB_YESNO);
		if(IDYES == res)
		{
			vector<CLineDataRecord*> lineDataEntry;
			for(vector<LineDataRecordMap>::iterator itr = pLineDataRecordEntry->begin();itr != pLineDataRecordEntry->end();itr++)
			{
				lineDataEntry.push_back(itr->pLineDataRecord);
			}

			CAppDocData*  pDocData  = CAppDocData::GetInstance();
			if(pDocData)
			{
				pDocData->GetProject()->Delete(&lineDataEntry);
				DisplayLineDataRecordSet();
			}
			
			return IDYES;
		}
	}

	return IDCANCEL;
}

/**
	@brief	initialize contents of control
	@author	humkyung
	@date	????.??.??
	@param
	@return
**/
void CEditLineDataForm::InitContents()
{
	CUserAppView* pView = (CUserAppView*)(GetParent());
	CUserAppDoc*  pDoc  = pView->GetDocument();
	CAppDocData* pDocData = CAppDocData::GetInstance();

	CString rIniFilePath = pDocData->GetProject()->GetIniFilePath();
	m_rIniFilePath = rIniFilePath;
	TCHAR szBuf[1025] = {'\0' ,};
	if(GetPrivateProfileString(_T("Iso Edit Setting") , _T("MStation Path") , _T("") ,szBuf,MAX_PATH,rIniFilePath))
	{
		m_rMStationPath = szBuf;
	}
	if(GetPrivateProfileString(_T("Iso Edit Setting") , _T("Source Iso File Folder") , _T("") ,szBuf,MAX_PATH,rIniFilePath))
	{
		m_sSourceIsoFileFolder = szBuf;
		if(m_sSourceIsoFileFolder.Right(1) != _T("\\")) m_sSourceIsoFileFolder += _T("\\");
	}

	CString rApp(_T("Line Data Field"));
	int nCount = GetPrivateProfileInt(rApp , _T("Count") , 0 , rIniFilePath);
	
	CString rSqlFilePath = CAppDocData::GetExecPath() + _T("\\Setting\\T_ISO_LINE_DATA.XML");
	CSQLFile sqlFile(rSqlFilePath.operator LPCSTR());
	if(ERROR_SUCCESS == sqlFile.ParseXML(rSqlFilePath.operator LPCTSTR()))
	{
		::GetPrivateProfileString(_T("Line Data Field") , _T("Column Order") , _T("") , szBuf , 1024 , m_rIniFilePath);
		vector<STRING_T> oResult; 
		vector<CString> HeadingEntry;
		if(!CString(szBuf).IsEmpty())
		{
			CTokenizer<CIsFromString>::Tokenize(oResult , szBuf , CIsFromString(_T(":")));
			for(vector<STRING_T>::iterator itr = oResult.begin();itr != oResult.end();++itr)
			{
				CString rHeading;
				if(_T("REV_NO") == (*itr))
				{
				}
				else
				{
					rHeading += itr->c_str() + CString(_T(",100"));
					HeadingEntry.push_back(rHeading);
				}
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
		m_wndLineDataReport.SetHeadings(rHeadings);
		m_wndLineDataReport.SetGridLines();

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

				if(CString(_T("STATUS")) == lvc.pszText)
				{
					m_wndLineDataReport.SetColumnType(i , RUNTIME_CLASS(IsGui::CListCtrlExComboBox));
					IsGui::CListCtrlEx::ColumnState& columnState = m_wndLineDataReport.GetColumnState(i);
					IsGui::CListCtrlExComboBox* pComboBox = static_cast<IsGui::CListCtrlExComboBox*>(columnState.m_pInPlaceCtrlHolder);
					if(NULL != pComboBox)
					{
						pComboBox->AddString(_T("IN PROGRESS"));
						pComboBox->AddString(_T("COMPLETE"));
						pComboBox->AddString(_T("HOLD"));
						pComboBox->AddString(_T("DELETED"));
					}
				}
				else if(!IS_FIXED_COLUMN(lvc.pszText))
				{
					m_wndLineDataReport.SetColumnType(i , RUNTIME_CLASS(IsGui::CListCtrlExEdit));
				}

				if(oResult.end() == find(oResult.begin() , oResult.end() , STRING_T(lvc.pszText)))
				{
					m_wndLineDataReport.ShowColumn(i , false);
				}
			}
		}
		/// up to here
	}else	AfxMessageBox("Can't find T_ISO_LINE_DATA.XML file");

	m_wndLineDataReport.EnableHeaderCheckBox(TRUE);

	CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();
	pWorkspaceBar->m_wndQuery1.InitContents( this );
}

/**	
	@brief	The CEditLineDataForm::OnNotify function
	@author	humkyung
	@param	wParam	a parameter of type WPARAM
	@param	lParam	a parameter of type LPARAM
	@param	pResult	a parameter of type LRESULT*
	@return	BOOL	
*/
BOOL CEditLineDataForm::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();

	NMHDR* pNMHDR = (NMHDR*)(lParam);
	if((SELECT_ALL == wParam) && (pWorkspaceBar->m_wndQuery1.GetDlgCtrlID() == pNMHDR->idFrom))
	{
		m_sCurrQueryCluase = pWorkspaceBar->m_wndQuery1.GetInAreaQueryCondition();
		
		vector< STRING_T > issueDates;
		LoadLineDataRecordSetFromDatabase( m_sCurrQueryCluase /*CString("")*/ , issueDates );
	}
	else if((SELECT == wParam) && (pWorkspaceBar->m_wndQuery1.GetDlgCtrlID() == pNMHDR->idFrom))
	{
		m_sCurrQueryCluase = pWorkspaceBar->m_wndQuery1.GetQueryCondition();

		vector< STRING_T > oIssueDates;
		pWorkspaceBar->m_wndQuery1.GetSelectedIssueDate( oIssueDates );

		LoadLineDataRecordSetFromDatabase(m_sCurrQueryCluase , oIssueDates );
	}
	else if(((WM_USER + 1) == wParam) && (INPLACEEDIT_END_LABEL_EDIT == pNMHDR->code))
	{
		if(m_pWndInPlaceEdit)
		{
			CString rString;
			m_pWndInPlaceEdit->GetWindowText(rString);
			if(m_pWndInPlaceEdit->m_sInitText != rString)
			{
				m_wndLineDataReport.SetItemText(m_pWndInPlaceEdit->m_nRow,m_pWndInPlaceEdit->m_nColumn,rString);

				CLineDataRecord *pLineDataRecord = (CLineDataRecord*)m_wndLineDataReport.GetItemData(m_pWndInPlaceEdit->m_nRow);
				if(pLineDataRecord)
				{
					TCHAR szBuf[MAX_PATH] = {'\0' ,};
					LVCOLUMN lvcolumn;
					lvcolumn.mask = LVCF_TEXT;
					lvcolumn.cchTextMax = MAX_PATH;
					lvcolumn.pszText = szBuf;
					m_wndLineDataReport.GetColumn(m_pWndInPlaceEdit->m_nColumn , &lvcolumn);
					
					CString rValue=rString;
					CDataField* pField = pLineDataRecord->FindWithFieldName(lvcolumn.pszText);
					if(pField)
					{
						///rOldKey = pLineDataRecord->GetKey();
						pField->Set(lvcolumn.pszText , lvcolumn.pszText , rString.operator LPCTSTR());
						///rNewKey = pLineDataRecord->GetKey();
						pField = pLineDataRecord->FindWithFieldName(_T("KEY"));
						const CString rKey(pField->value());
						///pField->Set(_T("KEY") , _T("KEY") , rNewKey.operator LPCTSTR());

						//! update database
						CAppDocData* pDocData = CAppDocData::GetInstance();
						const CProject::DatabaseProp prop = pDocData->GetProject()->GetDatabaseProp();
						/*const CString strFilePath = pDocData->m_ProjectPath;
						const CString strFileName = pDocData->m_ProjectName;*/
						
						try
						{
							CAppPostgreSQL database(pDocData->GetProject()->name() , prop.Host.c_str(),prop.User.c_str(),prop.Password.c_str());
							CString sColumnName = CString(lvcolumn.pszText);
							CString rSql = _T("UPDATE T_ISO_LINE_DATA SET ");
							rSql += sColumnName + _T("='") + rValue + _T("' WHERE KEY='") + rKey + _T("'");
							(*database.session()) << rSql.operator LPCTSTR();

							if((_T("PLANT_NO") == sColumnName) || (_T("AREA_NO") == sColumnName) || 
								(_T("UNIT_NO") == sColumnName) || (_T("SECTION_NO") == sColumnName))
							{
								pWorkspaceBar->m_wndQuery1.UpdateSelectionTreeList(true , NULL);
							}
						}
						catch(std::exception& ex)
						{
							AfxMessageBox(ex.what() , MB_OK|MB_ICONERROR);
						}

						UpdateLineDataRecord(m_pWndInPlaceEdit->m_nRow , pLineDataRecord);
					}	
				}

				delete m_pWndInPlaceEdit;
			}
		}
		m_pWndInPlaceEdit = NULL;
		return TRUE;
	}

	return CFormView::OnNotify(wParam, lParam, pResult);
}

/**
	@brief	하나의 LINE DATA RECORD의 내용을 리스트에 갱신한다.
	@author	HumKyung
	@date	????.??.??
	@param	nItem
	@param	pLineDataRecord
	@return
**/
int CEditLineDataForm::UpdateLineDataRecord(const int &nItem, CLineDataRecord *pLineDataRecord)
{
	assert(pLineDataRecord && "pLineDataRecord is NULL");
	int res = ERROR_SUCCESS;

	int _nItem = nItem;
	if(-1 == _nItem)
	{
		for(int i = 0;i < m_wndLineDataReport.GetItemCount();++i)
		{
			if(pLineDataRecord == (CLineDataRecord*)m_wndLineDataReport.GetItemData(i))
			{
				_nItem = i;
				break;
			}
		}
	}

	assert((_nItem > -1) && "item range error");
	if((_nItem > -1) && pLineDataRecord)
	{
		CAppDocData* pDocData = CAppDocData::GetInstance();
		TCHAR szBuf[MAX_PATH + 1]={'\0',};

		m_wndLineDataReport.SetItemData(_nItem , (DWORD)(pLineDataRecord));
		
		/// 2011.02.24 added by HumKyung
		bool bCancellation = false;
		CDataField* pField = pLineDataRecord->FindWithFieldName(_T("STATUS"));
		if(pField)
		{
			bCancellation = (0 == stricmp(_T("DELETED") , pField->value()));
		}
		/// up to here

		const int nColumnCount = m_wndLineDataReport.GetColumnCount();
		for(int k = 0;k < nColumnCount;++k)
		{
			LVCOLUMN lvcolumn;
			lvcolumn.mask = LVCF_TEXT;
			lvcolumn.cchTextMax = MAX_PATH;
			lvcolumn.pszText = szBuf;
			m_wndLineDataReport.GetColumn(k , &lvcolumn);
			
			if(IS_FIXED_COLUMN(lvcolumn.pszText))
			{
				if(CString(_T("[]")) != lvcolumn.pszText) m_wndLineDataReport.SetItemBkColor(nItem , k , FIXED_COLUMN_COLOR , FALSE);
			}

			if(true == bCancellation)
				m_wndLineDataReport.SetItemTextColor(nItem , k , CANCELLATION_ITEM_COLOR , FALSE); /// 2011.02.24 added by HumKyung
			
			if((CString(_T("REV_NO")) != lvcolumn.pszText))
			{
				CDataField* pField = pLineDataRecord->FindWithFieldName(lvcolumn.pszText);
				if(pField)
				{
					CString rValue = pField->value();
					rValue.Replace(_T("\n") , _T("/"));
					
					m_wndLineDataReport.SetItemText(_nItem , k , rValue);
				}
			}
			else
			{
				CRevDataRecordSet* pRevDataRecordSet = pLineDataRecord->GetRevDataRecordSet();
				CRevDataRecord* pRevRecord = pRevDataRecordSet->FindLatestRevDataRecord();
				if(NULL == pRevRecord)
				{
					m_wndLineDataReport.SetItemText(_nItem , k , _T("X"));
				}
				else
				{
					CDataField* pField = pLineDataRecord->FindWithDesc(_T("REV_NO"));
					if(NULL != pField) m_wndLineDataReport.SetItemText(_nItem , k , pField->value());
				}
			}
		}
	}

	return res;
}

/**	
	@brief	line data record를 리스트 컨트롤에 뿌린다.
	@author	HumKyung
	@date	????.??.??
	@param	
	@return	void	
*/
void CEditLineDataForm::DisplayLineDataRecordSet()
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	const CProject::DatabaseProp prop = pDocData->GetProject()->GetDatabaseProp();
	if(m_pLineDataRecordSet)
	{
		m_wndLineDataReport.SetRedraw(FALSE);
		m_wndLineDataReport.DeleteAllItems();
		
		LVITEM lvitem;
		lvitem.mask = LVIF_TEXT | LVIF_IMAGE;
		lvitem.iImage = 0;
		lvitem.cchTextMax = 32;
		
		int nCount = 0;
		const size_t nRecordCount = m_pLineDataRecordSet->GetLineDataRecordCount();
		int nItem = 0;
		for(int i = 0;i < int(nRecordCount) ;++i)
		{			
			CLineDataRecord* pLineDataRecord = m_pLineDataRecordSet->GetLineDataRecordAt(i);
			if(pLineDataRecord && !pLineDataRecord->IsDeleted())
			{
				lvitem.iItem = nItem++;
				lvitem.iSubItem = 0;
				lvitem.pszText = _T("");
				const int it = m_wndLineDataReport.InsertItem(&lvitem);
				if(ERROR_SUCCESS == UpdateLineDataRecord(it , pLineDataRecord))
				{
				}
				++nCount;
			}
		}
		
		/// group by area no - 2015.09.09 added by humkyung
		TCHAR sColText[256];
		const int iColumnCount = m_wndLineDataReport.GetColumnCount();
		for(int i = 0;i < iColumnCount;++i)
		{
			LVCOLUMN lvc = {0};
			lvc.mask = LVCF_TEXT;
			lvc.pszText = sColText;
			lvc.cchTextMax = sizeof(sColText)-1;
			m_wndLineDataReport.GetColumn(i , &lvc);
			if(_T("AREA_NO") == CString(lvc.pszText)) m_wndLineDataReport.GroupByColumn(i);
		}
		/// up to here
		m_wndLineDataReport.SetRedraw(TRUE);
		ShowRecordCount(nCount);
	}
}

/**
	@brief	선택한 LINE LIST RECORD들을 구한다.
	@author	HumKyung
	@date	????.??.??
	@param	LineDataRecordEntry - container for LineDataRecordMap
	@return
**/
void CEditLineDataForm::GetSelectedLineDataRecord(vector<LineDataRecordMap>& LineDataRecordEntry)
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
	@brief	선택한 LINE LIST RECORD들을 구한다.
	@author	humkyung
**/
void CEditLineDataForm::OnDblclkListLineList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if( 0 != m_wndLineDataReport.GetItemCount())
	{
		CPoint point;
		::GetCursorPos(&point);
		m_wndLineDataReport.ScreenToClient(&point);
		
		LVHITTESTINFO lvhti;
		lvhti.pt = point;
		const int hItem = m_wndLineDataReport.SubItemHitTest(&lvhti);
		
		if(lvhti.flags & LVHT_ONITEMLABEL)
		{
			TCHAR szBuf[MAX_PATH] = {'\0' ,};
			LVCOLUMN lvcolumn;
			lvcolumn.mask = LVCF_TEXT;
			lvcolumn.cchTextMax = MAX_PATH;
			lvcolumn.pszText = szBuf;
			m_wndLineDataReport.GetColumn(lvhti.iSubItem , &lvcolumn);

			if(!IS_FIXED_COLUMN(lvcolumn.pszText))
			{
				CRect r;
				m_wndLineDataReport.GetSubItemRect(lvhti.iItem , lvhti.iSubItem , LVIR_BOUNDS , r);
				if(CString(_T("STATUS")) == lvcolumn.pszText)
				{
					if(NULL == m_pWndInPlaceComboBox)
					{
						m_wndLineDataReport.ClientToScreen(&r);
						ScreenToClient(&r);

						m_pWndInPlaceComboBox = new CComboBox();
						r.bottom += 500;
						if(m_pWndInPlaceComboBox->Create(WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST, r , this , 0x1111))
						{
							m_pWndInPlaceComboBox->AddString(_T("ALL"));
							m_pWndInPlaceComboBox->AddString(_T("IN PROGRESS"));
							m_pWndInPlaceComboBox->AddString(_T("COMPLETE"));
						}
						/*(this , r , dwStyle , WM_USER + 1 , 
							lvhti.iItem , lvhti.iSubItem , m_wndLineDataReport.GetItemText(lvhti.iItem , lvhti.iSubItem) , VK_LBUTTON);*/
					}
				}
				else
				{
					if(NULL == m_pWndInPlaceEdit)
					{
						m_wndLineDataReport.ClientToScreen(&r);
						ScreenToClient(&r);

						DWORD dwStyle = ES_LEFT;
						m_pWndInPlaceEdit = new CInPlaceEdit(this , r , dwStyle , WM_USER + 1 , 
							lvhti.iItem , lvhti.iSubItem , m_wndLineDataReport.GetItemText(lvhti.iItem , lvhti.iSubItem) , VK_LBUTTON);
					}
				}
			}
			else
			{
				CAppDocData* pDocData = CAppDocData::GetInstance();

				CLineDataRecord* pLineDataRecord = (CLineDataRecord*)m_wndLineDataReport.GetItemData(hItem);
				if(NULL != pLineDataRecord)
				{
					CString rIsoFileName = pDocData->GetProject()->GetInputIsoFileName(pLineDataRecord);
					rIsoFileName += CIsoEditSettingPage::m_rFileNameExt;
					CString sSourceIsoFilePath =  m_sSourceIsoFileFolder;
					if(_T("\\") != sSourceIsoFilePath.Right(1)) sSourceIsoFilePath += _T("\\");
					sSourceIsoFilePath += rIsoFileName;
					if(CFileTools::DoesFileExist(sSourceIsoFilePath))
					{
						::ShellExecute(NULL, _T("open") , m_rMStationPath , sSourceIsoFilePath , NULL , SW_SHOW);
					}
					else
					{
						AfxMessageBox(_T("There is no ") + sSourceIsoFilePath , MB_OK|MB_ICONWARNING);
					}
				}
			}
		}
	}
	*pResult = 0;
}

/**
	@brief	pop up menu를 생성한다.
	@author	HumKyung
	@date	2011.02.21
	@param	pNMHDR
	@param	pResult
	@return	void
**/
void CEditLineDataForm::OnRclickLineList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	if((0 != m_wndLineDataReport.GetItemCount()) && (m_wndLineDataReport.GetSelectedItemCount() > 0))
	{
		CPoint point;
		::GetCursorPos(&point);
		
		BCMenu menu;
		menu.LoadMenu(IDR_LINE_DATA);
		CMenu* pMenu = menu.GetSubMenu(0);
		if(pMenu)
		{
			CPropertiesWnd* pPropertiesWnd = CPropertiesWnd::GetInstance();
			if(TRUE == pPropertiesWnd->IsVisible())
			{
				pMenu->CheckMenuItem( ID_LINE_DATA_PROPERTIES , MF_CHECKED);
			}
			else
			{
				pMenu->CheckMenuItem( ID_LINE_DATA_PROPERTIES , MF_UNCHECKED);
			}

			/// 2015.08.27 added by humkyung
			vector<LineDataRecordMap> LineDataRecordEntry;
			GetSelectedLineDataRecord(LineDataRecordEntry);
			pMenu->EnableMenuItem(ID_LINE_DATA_DELETE , LineDataRecordEntry.empty() ? TRUE : FALSE);
			/// up to here

			pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON , point.x , point.y , this);
		}
	}

	*pResult = 0;
}

/**	@brief	qualified line form에 line data를 넣는다.

	@author	HumKyung.BAEK

	@return	void	
*/
void CEditLineDataForm::QualifyLineData()
{
	if( m_pQualifiedLineForm )
	{
		vector<LineDataRecordMap> LineDataRecordEntry;
		GetSelectedLineDataRecord(LineDataRecordEntry);
		
		m_pQualifiedLineForm->AddLineDataRecordEntry(&LineDataRecordEntry);
	}
}

void CEditLineDataForm::OnUpdateLineDataModify(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(FALSE);
}

CString GetSortKey(CLineDataRecord* pLineDataRecord , vector<string>& rDescEntry)
{
	CString rRet;

	for(vector<STRING_T>::const_iterator itr = rDescEntry.begin();itr != rDescEntry.end();++itr)
	{
		CDataField* pDataField = pLineDataRecord->FindWithDesc(itr->c_str());
		rRet += pDataField->value();
	}

	return rRet;
}

/**	\brief	The _CompareFunction function
	\brief	compare two item's primary key.

	\param	lParam1	a parameter of type LPARAM
	\param	lParam2	a parameter of type LPARAM
	\param	lParamData	a parameter of type LPARAM

	\return	int CALLBACK
*/
int CALLBACK _ComparePrimaryKeyFunction(LPARAM lParam1, LPARAM lParam2, LPARAM lParamData)
{
	IsGui::CListCtrlEx* pListCtrl = reinterpret_cast<IsGui::CListCtrlEx*>(lParamData);
	ASSERT(pListCtrl->IsKindOf(RUNTIME_CLASS(CListCtrl)));

	IsGui::CListCtrlExItemData* pid1 = reinterpret_cast<IsGui::CListCtrlExItemData*>(lParam1);
	IsGui::CListCtrlExItemData* pid2 = reinterpret_cast<IsGui::CListCtrlExItemData*>(lParam2);
	if(pid1 && pid2)
	{
		CEditLineDataForm* pEditLineDataForm = static_cast<CEditLineDataForm*>(pListCtrl->GetParent());

		CLineDataRecord* p1 = reinterpret_cast<CLineDataRecord*>(pid1->dwData);
		CLineDataRecord* p2 = reinterpret_cast<CLineDataRecord*>(pid2->dwData);
		CString r1 = GetSortKey(p1 , pEditLineDataForm->m_sortDescEntry);///GetPrimaryKey();
		CString r2 = GetSortKey(p2 , pEditLineDataForm->m_sortDescEntry);///GetPrimaryKey();

		if (r1 < r2) return -1;
		if (r1 > r2) return 1;
		
		return 0;
	}
	
	return 0;
}

/**	
	@brief	sort line data records by primary key which defined by project admin.

	@author	humkyung

	@return	void	
*/
void CEditLineDataForm::OnButtonSort() 
{
	CSortSettingDlg dlg(_T("Sort Format Setting"));
	if(IDOK == dlg.DoModal())
	{
		WritePrivateProfileString( _T("Sorting Set") , _T("Sort Key") , dlg.m_rFileNameFormat , m_rIniFilePath);

		CString rSortSetting = dlg.m_rFileNameFormat;
		if(!rSortSetting.IsEmpty())
		{
			CTokenizer<CIsFromString>::Tokenize(m_sortDescEntry , rSortSetting.operator const char*() , CIsFromString(":"));
			VERIFY(m_wndLineDataReport.SortItems(_ComparePrimaryKeyFunction, reinterpret_cast<DWORD>(&m_wndLineDataReport)));	
		}
	}
}

void CEditLineDataForm::ShowRecordCount(int nRecordCount)
{
	long long lTotalRecordCount=0;

	/// get total record count - 2012.07.30 added by humkyung
	CAppDocData* pDocData = CAppDocData::GetInstance();
	if(pDocData)
	{
		const CProject::DatabaseProp prop = pDocData->GetProject()->GetDatabaseProp();
		const CString rProjectPath = pDocData->GetProjectPath().c_str();
		const CString sIniFilePath= pDocData->GetPrjIniFilePath();

		try
		{
			CAppPostgreSQL database(pDocData->GetProject()->name(),prop.Host.c_str(),prop.User.c_str(),prop.Password.c_str());
			{
				soci::row r;
				(*database.session()) << _T("SELECT COUNT(*) FROM T_ISO_LINE_DATA"),soci::into(r);
				lTotalRecordCount = r.get<long long>(0);
			}
		}
		catch(const std::exception& ex)
		{
			UNUSED_ALWAYS(ex);
		}
	}
	/// up to here

	m_nRecordCount = nRecordCount;
	if(m_cStaticLabel.GetSafeHwnd())
	{
		CString rCount;
		rCount.Format( _T("Selected Lines : %d/%d") , m_nRecordCount , lTotalRecordCount);
		m_cStaticLabel.SetWindowText((LPCTSTR)rCount);
	}
}

typedef struct
{
	CString sItem;
	string key;
} _prikeyindex_ ;

/**	
	@brief	
	@author	HumKyung
	@date	2011.01.19
	@param	pCmdUI
	@return	void	
*/
//void CEditLineDataForm::OnUpdateLineDataDelete(CCmdUI* pCmdUI) 
//{
//	vector<LineDataRecordMap> LineDataRecordEntry;
//	GetSelectedLineDataRecord(LineDataRecordEntry);
//	pCmdUI->Enable(!LineDataRecordEntry.empty());
//
//	//m_wndDeleteButton.EnableWindow(!LineDataRecordEntry.empty());
//}

/**	
	@brief	line data를 삭제한다.

	@author	HumKyung

	@date	????.??.??

	@param	

	@return	void	
*/
void CEditLineDataForm::OnButtonDelete() 
{
	vector<LineDataRecordMap> LineDataRecordEntry;
	GetSelectedLineDataRecord(LineDataRecordEntry);
	if(!LineDataRecordEntry.empty())
	{
		vector<LineDataRecordMap> QualifiedLineDataRecordEntry;
		m_pQualifiedLineForm->GetAllLineDataRecord(QualifiedLineDataRecordEntry);

		for(vector<LineDataRecordMap>::iterator itr = QualifiedLineDataRecordEntry.begin();itr != QualifiedLineDataRecordEntry.end();++itr)
		{
			for(vector<LineDataRecordMap>::iterator jtr = LineDataRecordEntry.begin();jtr != LineDataRecordEntry.end();++jtr)
			{
				if(itr->pLineDataRecord == jtr->pLineDataRecord)
				{
					AfxMessageBox(_T("먼저 Qualified Line List에 있는 Line Data를 제거하세요.") , MB_OK | MB_ICONWARNING);
					return;
				}
			}
		}

		DeleteLineDataRecordEntry(&LineDataRecordEntry);
	}

	///if( m_pQualifiedLineForm ) m_pQualifiedLineForm->DeleteLineData();
}

/**	
	@brief	line data를 삭제한다.
	@author	humkyung
	@date	2015.08.27
	@param	
	@return	void	
*/
void CEditLineDataForm::OnLineDataDelete() 
{
	vector<LineDataRecordMap> LineDataRecordEntry;
	GetSelectedLineDataRecord(LineDataRecordEntry);
	if(!LineDataRecordEntry.empty())
	{
		/*vector<LineDataRecordMap> QualifiedLineDataRecordEntry;
		m_pQualifiedLineForm->GetAllLineDataRecord(QualifiedLineDataRecordEntry);

		for(vector<LineDataRecordMap>::iterator itr = QualifiedLineDataRecordEntry.begin();itr != QualifiedLineDataRecordEntry.end();++itr)
		{
			for(vector<LineDataRecordMap>::iterator jtr = LineDataRecordEntry.begin();jtr != LineDataRecordEntry.end();++jtr)
			{
				if(itr->pLineDataRecord == jtr->pLineDataRecord)
				{
					AfxMessageBox(_T("먼저 Qualified Line List에 있는 Line Data를 제거하세요.") , MB_OK | MB_ICONWARNING);
					return;
				}
			}
		}*/

		if(IDYES == AfxMessageBox(_T("Do you want to delete checked datas?") , MB_YESNO , MB_ICONQUESTION))
		{
			DeleteLineDataRecordEntry(&LineDataRecordEntry);
		}
	}

	///if( m_pQualifiedLineForm ) m_pQualifiedLineForm->DeleteLineData();
}

#define	IMPORT_ISO_DWG		0
#define	UPDATE_FROM_IDF		1

static int SWITCH = IMPORT_ISO_DWG;

typedef UINT (*pfVoid)();

/*
	@brief	

	@author	HumKyung.BAEK
*/
UINT CEditLineDataForm::StatusThreadEntry(LPVOID pVoid)
{
	UINT res = 0;
	res = ((CEditLineDataForm*)pVoid)->StatusThread();
	return res;
}

static vector<CEditLineDataForm::IsoFileAreaData> __ISO_FILE_KEY_DATA__;

/******************************************************************************
    @author     humkyung
    @date       2012-08-26
    @function   DeleteProjectContents
    @return     int
    @brief		delete all files in Temp,PRODUCT folder
******************************************************************************/
int CEditLineDataForm::DeleteProjectContents()
{
	CAppDocData* pDocData = CAppDocData::GetInstance();

	CFileTools::DeleteFiles(CString(pDocData->GetProject()->path()) + _T("\\Temp") , _T("*.*") , false);
	CFileTools::DeleteFiles(CString(pDocData->GetProject()->path()) + _T("\\PRODUCT") , _T("*.*") , true);

	return ERROR_SUCCESS;
}

/**	
	@brief	쓰레드 실행부
	@author	HumKyung.BAEK
	@date	2010.03.10
	@return
*/
UINT CEditLineDataForm::StatusThread()
{
	CWorkStatusDlg* pWorkStatusDlg = CWorkStatusDlg::GetInstance();
	try
	{
		InterlockedExchange((LONG*)(&(pWorkStatusDlg->m_bThreadRunning)) , TRUE);/// so the main thread knows that this thread is still running
		{
			CAppDocData* pDocData = CAppDocData::GetInstance();
			CProjectData* pPrjData = CProjectData::GetInstance();
			const CProject::DatabaseProp prop = pDocData->GetProject()->GetDatabaseProp();
			try
			{
				/// create log file
				int nLogIndex = 1;
				CString rLogFilePath = CAppDocData::GetExecPath() + _T("\\log.html");
				OFSTREAM_T ofile(rLogFilePath.operator LPCTSTR());
				ofile.setf(ios_base:: fixed, ios_base:: floatfield); /// 소수점 방식으로 표시됨
				pDocData->appendHeader(ofile);

				const CString rDatabaseRefKey = (*pPrjData->m_pProjectSettingMap)[_T("database_reference_key")];
				vector<STRING_T> oResult;
				CTokenizer<CIsFromString>::Tokenize(oResult , rDatabaseRefKey.operator LPCTSTR() , CIsFromString(_T(" ~ ")));

				CAppPostgreSQL database(pDocData->GetProject()->name(),prop.Host.c_str(),prop.User.c_str(),prop.Password.c_str());

				CWorkStatusDlg* pWorkStatusDlg = CWorkStatusDlg::GetInstance();
				CString rTitle(_T("Import Iso Files..."));
				int nIndex = 0;
				for(vector<IsoFileAreaData>::iterator itr = __ISO_FILE_KEY_DATA__.begin();itr != __ISO_FILE_KEY_DATA__.end();++itr,++nIndex)
				{
					soci::transaction txn(*database.session());
					{
						ReadIsoFiles(*itr);	/// read information from iso file

						/// get key value
						CString rKey , rCondition;
						for(vector<STRING_T>::const_iterator jtr = oResult.begin();jtr != oResult.end();++jtr)
						{
							map<CString,CString>::const_iterator where = itr->AreaDataMap.find(jtr->c_str());
							if(where != itr->AreaDataMap.end())
							{
								rKey += where->second;
								rCondition += jtr->c_str() + CString(_T("='")) + (where->second) + _T("' AND ");
							}else	rKey += jtr->c_str();
						}
						map<CString,CString>::const_iterator where = itr->AreaDataMap.find(_T("SHEET_NO"));
						if(where != itr->AreaDataMap.end())
						{
							rKey += where->second;
							if((rCondition.GetLength() >= 5) && (_T(" AND ") == rCondition.Right(5)))
							{
								rCondition = rCondition.Left(rCondition.GetLength() - 5);
							}
						}

						/// have to handle error
						if(!rCondition.IsEmpty())
						{
							/// delete line data and bom data which has same key
							CString rSql = _T("DELETE T_ISO_LINE_DATA WHERE KEY='") + rKey + _T("'");
							(*database.session()) << rSql.operator LPCTSTR();
							rSql = _T("DELETE T_ISO_BOM_DATA WHERE KEY='") + rKey + _T("'");
							(*database.session()) << rSql.operator LPCTSTR();
							/// up to here

							/// get remark data from T_ISSUED_ISO_LINE_DATA table
							rSql = _T("SELECT * FROM T_ISSUED_ISO_LINE_DATA WHERE KEY='") + rKey + _T("'");
							STRING_T rRemark;
							soci::row r;
							(*database.session()) << rSql.operator LPCTSTR(),soci::into(r);
							if(r.size() > 0)
							{
								rRemark = r.get<STRING_T>(_T("remark"));
							}

							/// get data from line list data table
							rSql = _T("SELECT * FROM T_LINE_LIST_DATA WHERE ");
							rSql += rCondition;
							(*database.session()) << rSql.operator LPCTSTR(),soci::into(r);
							try
							{
								/// get below data from iso file
								STRING_T rFieldNames = _T("KEY,REMARK,");
								CString  rFieldValues , rTemp=_T("'%s','%s',");
								rFieldValues.Format( rTemp, rKey , rRemark.c_str() );
								///
								for(int i = 0;i < int(r.size());++i)
								{
									const soci::column_properties colProp = r.get_properties(i);
									const soci::data_type dt = colProp.get_data_type();
									STRING_T sFieldName = colProp.get_name(),sValue;
									/// skip key and remark
									if((0 == STRICMP_T(_T("KEY"),sFieldName.c_str())) || (0 == STRICMP_T(_T("REMARK"),sFieldName.c_str()))) continue;
									/// up to here
									if(soci::i_ok == r.get_indicator(i))
									{
										if(soci::dt_string == dt)
										{
											sValue = r.get<STRING_T>(i);
										}
										else if(soci::dt_integer == dt)
										{
											OSTRINGSTREAM_T oss;
											oss << dt;
											sValue = oss.str();
										}
									}

									if(!sValue.empty() && (_T("-") != sValue))
									{
										rFieldNames += sFieldName + _T(",");
										IsString::ReplaceOf(sValue , _T("'") , _T("''"));	/// replace ' with '' - 2011.09.28 added by humkyung
										rFieldValues += (soci::dt_string != dt) ? CString(sValue.c_str()) + _T(",") : _T("'") + CString(sValue.c_str()) + _T("',");
									}
									else
									{
										/// below field's value is empty, so try to get value from other side
										if(_T("PLAN_NO") == sFieldName)
										{
											map<CString , CString>::const_iterator where = itr->AreaDataMap.find(sFieldName.c_str());
											if(where != itr->AreaDataMap.end())
											{
												sValue = where->second;
											}

											if(sValue.empty() || (_T("-") == sValue))
											{
												/// determine to read plan no from idf file - 2011.09.21 added by humkyung
												map<CString,CString>::const_iterator where = (*pPrjData->m_pProjectSettingMap).find(_T("get_plan_dwg"));
												if((where != (pPrjData->m_pProjectSettingMap->end())) && (0 == where->second.CompareNoCase(_T("ON"))))
												{
												/// up to here
													sValue = GetPlanNOFromIDF(ofile , nLogIndex , itr->rFilePath);
												}
											}
										}
										/// 2012.08.13 added by humkyung
										else if(_T("SITE") == sFieldName)
										{
											sValue = pDocData->m_rSiteName;
										}
										/// up to here
										else
										{
											map<CString , CString>::const_iterator where = itr->AreaDataMap.find(sFieldName.c_str());
											if(where != itr->AreaDataMap.end())
											{
												sValue = where->second;
											}
										}

										if(!sValue.empty())
										{
											rFieldNames += sFieldName + _T(",");
											IsString::ReplaceOf(sValue , _T("'") , _T("''")); /// replace ' with '' - 2011.09.28 added by humkyung
											rFieldValues += (soci::dt_string != dt) ? CString(sValue.c_str()) + _T(",") : _T("'") + CString(sValue.c_str()) + _T("',");
										}
										/// up to here
									}
								}

								/// trim , which is end of line
								if(_T(",") == rFieldNames.substr(rFieldNames.length() - 1))
								{
									rFieldNames = rFieldNames.substr(0 , rFieldNames.length() - 1);
								}
								if(_T(",") == rFieldValues.Right(1))
								{
									rFieldValues = rFieldValues.Left(rFieldValues.GetLength() - 1);
								}

								/// insert record to T_ISO_LINE_DATA
								rSql = _T("INSERT INTO T_ISO_LINE_DATA(") + CString(rFieldNames.c_str()) + _T(") VALUES(") + rFieldValues + _T(")");
								(*database.session()) << rSql.operator LPCTSTR();
								
								/// delete revision data which has same key
								rSql = _T("DELETE T_ISO_REVISION WHERE KEY='") + rKey + _T("'");
								(*database.session()) << rSql.operator LPCTSTR();
								/// up to here

								CString rSqlFormat = _T("INSERT INTO T_ISO_REVISION SELECT * FROM T_ISSUED_ISO_REVISION WHERE KEY='%s'");
								rSql.Format(rSqlFormat , rKey);
								/// Execute query
								(*database.session()) << rSql.operator LPCTSTR();

								txn.commit();
							}
							catch(const std::exception& ex)	
							{
								ofile << std::endl << _T("<tr>") << std::endl;
								ofile << _T("<td title=\"NO=\">") << (nLogIndex++) << _T("</td>");
								ofile << _T("<td title=\"desc.=\">") << rKey.operator LPCTSTR() << _T(":") << ex.what() << _T("</td>");
								ofile << _T("</tr>") << std::endl;
							}
						}
						else if(ofile.is_open())
						{
							ofile << std::endl << _T("<tr>") << std::endl;
							ofile << _T("<td title=\"NO\">") << (nLogIndex++) << _T("</td>");
							ofile << _T("<td title=\"desc.=\">") << itr->rFilePath.operator LPCTSTR() << _T(": KEY값을 구할수 없습니다.") << _T("</td>");
							ofile << _T("</tr>") << std::endl;
						}

						if(pWorkStatusDlg)
						{
							int process = int(((double)nIndex / (double)__ISO_FILE_KEY_DATA__.size()) * 100.f);
							pWorkStatusDlg->UpdateWorkStatus(rTitle , process);
						}
					}
				}
				if(nLogIndex > 1)
				{
					::ShellExecute(NULL , _T("open") , rLogFilePath , _T("log.html") , NULL , SW_SHOWNORMAL);
				}
			}
			catch(const std::exception& ex)
			{
				AfxMessageBox(ex.what(),MB_OK|MB_ICONERROR);
			}
		}
		InterlockedExchange((LONG*)(&(pWorkStatusDlg->m_bThreadRunning)) , FALSE);
		
		//////////////////////////////////////////////////////////////////////////
		/// close work status dialog
		pWorkStatusDlg->SendMessage(WM_COMMAND , IDOK );

		return IDOK;
	}
	catch(const std::exception& ex)
	{
		AfxMessageBox(ex.what(),MB_OK|MB_ICONERROR);

		InterlockedExchange((LONG*)(&(pWorkStatusDlg->m_bThreadRunning)) , FALSE);
		
		//////////////////////////////////////////////////////////////////////////
		/// close work status dialog
		pWorkStatusDlg->SendMessage(WM_COMMAND , IDOK );
	}

	return IDCANCEL;
}

/******************************************************************************
    @author     humkyung
    @date       2012-08-13
    @class      CEditLineDataForm
    @function   OnUpdateIsoFolder
    @return     void
    @param      CCmdUI* pCmdUI
    @brief
******************************************************************************/
void CEditLineDataForm::OnUpdateIsoFolder(CCmdUI* pCmdUI) 
{
	/*CAppDocData* pDocData = CAppDocData::GetInstance();
	pCmdUI->Enable(_T("ALL") != pDocData->m_rSiteName);*/
}

/**	
	@brief	select a folder containing iso files
	@author	HumKyung
	@date	2010.03.08
	@param
	@return	void	
*/
void CEditLineDataForm::OnButtonIsoFolder() 
{
	CUserAppView* pView = (CUserAppView*)(GetParent());
	CUserAppDoc*  pDoc  = pView->GetDocument();
	CAppDocData* pDocData = CAppDocData::GetInstance();

	STRING_T rIniFilePath = pDocData->GetProject()->GetIniFilePath();
	CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();

	/// 프로젝트 path를 가져온다.
	TCHAR szBuf[MAX_PATH] = {'\0',};
	/// Install File Path를 구한다.
	CString rString;
	if(CFileTools::GetExecutableDirectory(rString))
	{
		if(_T("\\") != rString.Right(1)) rString += _T("\\");
		rString += _T("BackUp");
		m_rInstallPath = rString.operator LPCTSTR();
	}
	else
	{
		AfxMessageBox(_T("Fail, Read Install Path!") , MB_OK | MB_ICONWARNING);
		return;
	}
	
	if(GetPrivateProfileString(_T("General") , _T("path") , NULL , szBuf , MAX_PATH , rIniFilePath.c_str()))
	{
		m_rProjectPath = szBuf;
	}
	else
	{
		AfxMessageBox(_T("Fail, Read Ini File [General] -> Path!") , MB_OK | MB_ICONWARNING);
	}

	CString sIsoFolderPath;
	GetDlgItemText(IDC_EDIT_ISO_FOLDER , sIsoFolderPath);
	const UINT uFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
	CFolderDialog oFolderDlg(_T("Select Iso Folder") , sIsoFolderPath , this , uFlags);
	if(IDOK == oFolderDlg.DoModal())
	{
		sIsoFolderPath = oFolderDlg.GetFolderPath();
		__ISO_FILE_KEY_DATA__.clear();

		SetDlgItemText(IDC_EDIT_ISO_FOLDER , sIsoFolderPath);
		UpdateData(TRUE);

		/// write 'source iso file folder'
		CString rApp( _T("Iso Edit Setting") );
		TCHAR szBuf[MAX_PATH] = {'\0' ,};
		WritePrivateProfileString(rApp , _T("Source Iso File Folder") , sIsoFolderPath , m_rIniFilePath);

		/// find source iso files
		vector<CString> oFoundFiles;
		CFileTools::FindFiles(oFoundFiles , sIsoFolderPath , _T("*") + CIsoEditSettingPage::m_rFileNameExt , false);
		for(vector<CString>::const_iterator itr = oFoundFiles.begin();itr != oFoundFiles.end();++itr)
		{
			IsoFileAreaData aAreaData;
			aAreaData.rFilePath = (*itr);
			__ISO_FILE_KEY_DATA__.push_back( aAreaData );
		}

		/*if(ERROR_SUCCESS == RunRIS())
		{
			const CString sOutputFilePath(CFileTools::GetAppDataPath() + + _T("\\") + CString(PRODUCT_NAME) + _T("\\ris.db"));
			ReadRISResultFile(sOutputFilePath);
		}*/

		//////////////////////////////////////////////////////////////////////////
		CWorkStatusDlg dlg;
		if(!__ISO_FILE_KEY_DATA__.empty())
		{
			//this->DeleteProjectContents();

			/// get drawing information from oracle database - 2012.08.29 added by humkyung
#ifndef	_DEBUG
			if(ERROR_SUCCESS == RunRIS())
			{
				const CString sOutputFilePath(CFileTools::GetAppDataPath() + + _T("\\") + CString(PRODUCT_NAME) + _T("\\ris.db"));
				ReadRISResultFile(sOutputFilePath);
			}
#endif
			/// up to here

			dlg.m_pThread = AfxBeginThread(StatusThreadEntry, this, THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);

			if (NULL == dlg.m_pThread)
			{
				AfxMessageBox(_T("Can't create thread!!!"));
			}
			else
			{
				SWITCH = IMPORT_ISO_DWG;
				if(IDOK == dlg.DoModal())
				{
					pWorkspaceBar->m_wndQuery1.UpdateSelectionTreeList();

					/// load line data recordset
					vector<STRING_T> issueDates;
					LoadLineDataRecordSetFromDatabase(_T("") , issueDates , true );
				}
			}
		}
	}
}

/**	
	@brief	read iso files
	@author	humkyung
	@date	2010.03.10
	@return	void	
*/
void CEditLineDataForm::ReadIsoFiles(IsoFileAreaData& iso_file_key_data)
{
	CAppDocData* pDocData = CAppDocData::GetInstance();

	vector<int> ParsingTypes;
	ParsingTypes.push_back(TEXT_ELM);
	{
		auto_ptr<CDgnDocument> dgndoc(CDgnDocument::CreateInstance());
		//////////////////////////////////////////////////////////////////////////
		dgndoc->Read(iso_file_key_data.rFilePath , CDgnDocument::UOR_BASE , &ParsingTypes);
		{
			const size_t count = dgndoc->GetEntityCount();
			for(int i = 0;i < int(count);++i)
			{
				CDgnElement* pEnt = dgndoc->GetEntityAt(i);
				if(pEnt && pEnt->IsKindOf(CDgnText::TypeString()))
				{
					CDgnText* pText2d = (CDgnText*)(pEnt);

					DPoint3d origin = pText2d->origin();			
					CIsVolume vol;
					vol.Add(CIsPoint3d(origin.x , origin.y , 0));
					
					IsoDwgDataArea* pIsoDwgDataArea = pDocData->FindIsoDwgDataAreaCollideWith(vol);
					if(NULL != pIsoDwgDataArea)
					{
						CString str(pText2d->textString());
						/// remove - from C_LINE_ID value - 2013.09.08 added by humkyung
						if(_T("LINE_ID") == pIsoDwgDataArea->sFieldName)
						{
							str.Replace(_T("-") , _T(""));
						}
						/// up to here
						iso_file_key_data.AreaDataMap.insert(make_pair(pIsoDwgDataArea->sFieldName , str));
					}
				}
			}
		}
	}
}

static vector<STRING_T> ERROR_ENTRY;

/**	
	@brief	쓰레드 실행부, IDF 파일에서 하나의 3D 좌표를 읽어와 122테이블에서 DWG NO.를 구한다.
	@remarks
  	@author	BHK,KHS
	@date	????.??.??
**/
CString CEditLineDataForm::GetPlanNOFromIDF(OFSTREAM_T& ofile , int& iLogIndex , const CString& sIsoFilePath)
{
	CString sRes;

	CAppDocData* pDocData = CAppDocData::GetInstance();
	CProjectData* pPrjData = CProjectData::GetInstance();
	
	CString sIDFFilePath = sIsoFilePath;
	const int iPos = sIDFFilePath.ReverseFind('.');
	sIDFFilePath = sIDFFilePath.Left(iPos) + _T(".IDF");
	if(CFileTools::DoesFileExist(sIDFFilePath))
	{
		CString rTitle = sIDFFilePath;
		
		CIDFFile& idf = CIDFFile::GetInstance();
		idf.Read(sIDFFilePath.operator LPCTSTR());
		double  x = 0 , y = 0 , z = 0;
		set<STRING_T> oDwgIndexNoEntry;
		
		/// IDF 파일에서 3D 좌표를 가져온다.
		vector<CIsPoint3d> Point3dList;
		if(ERROR_SUCCESS == idf.GetCoodinateList(Point3dList))
		{
			CPdTable122& _122Table_ = CPdTable122::GetInstance();
			
			set<STRING_T> DrawingTitleEntry;
			
			/// 3D좌표로 122 테이블에서 dwg_index_no를 가져온다.
			int nDwgIndexNoCount = 0;
			for(vector<CIsPoint3d>::iterator jtr = Point3dList.begin();jtr != Point3dList.end();++jtr)
			{
				nDwgIndexNoCount =_122Table_.FindDwgIndexNo(oDwgIndexNoEntry, jtr->x() , jtr->y() , jtr->z());
				if(0 != nDwgIndexNoCount) break;
			}
			
			if(0 != nDwgIndexNoCount)
			{
				CPdTable121& _121Table_ = CPdTable121::GetInstance();
				/// dwg_index_no로 121 테이블에서 drawing_title 를 가져온다.
				/// fixed #111 - 2013.06.04 added by humkyung
				const CString sUsePlanDwgBy = (*(pPrjData->m_pProjectSettingMap))[_T("use_plan_dwg_by")];
				int nDrawingTitleCount = (_T("DRAWING_TITLE") == sUsePlanDwgBy) ? 
					_121Table_.FindDrawingTitle( DrawingTitleEntry, oDwgIndexNoEntry) : _121Table_.FindDrawingNo( DrawingTitleEntry, oDwgIndexNoEntry);
				if(0 != nDrawingTitleCount) 
				{
					for(set<STRING_T>::iterator jtr = DrawingTitleEntry.begin();jtr != DrawingTitleEntry.end();++jtr)
					{
						return (jtr->c_str());
					}
				}
				/// up to here
			}
			else
			{
				/// 2011.06.23 - message is modified by humkyung
				/// leave a log message instead of show message box
				ofile << std::endl << _T("<tr>") << std::endl;
				ofile << _T("<td title=\"NO\">") << (iLogIndex++) << _T("</td>");
				ofile << _T("<td title=\"desc.=\">") << sIsoFilePath.operator LPCTSTR() << _T("에 대한 도면 이름을 찾을 수 없습니다.") << _T("</td>");
				ofile << _T("</tr>") << std::endl;
			}
		}
	}
	else
	{
		ERROR_ENTRY.push_back(CString(sIDFFilePath + _T(".IDF 파일을 찾을 수 없습니다. '")).operator LPCSTR());
	}
	
	return sRes;
}

/******************************************************************************
    @brief		리스트에 담겨진 line data에서 필드가 sName인 값을 sValue로 변경한다.
	@author     humkyung
    @date       2013-02-07
    @function   SavePartDataOf
    @return     int
    @param      const STRING_T& sName	필드명
    @param      const STRING_T& sValue	값
	@param		list<CLineDataRecord*>& oLineDataList
******************************************************************************/
int CEditLineDataForm::SavePartDataOf(const STRING_T& sName , const STRING_T& sValue , list<CLineDataRecord*>& oLineDataList)
{
	CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();
	CAppDocData* pDocData = CAppDocData::GetInstance();
	const CProject::DatabaseProp prop = pDocData->GetProject()->GetDatabaseProp();

	try
	{
		CAppPostgreSQL database(pDocData->GetProject()->name(),prop.Host.c_str(),prop.User.c_str(),prop.Password.c_str());
		{
			for(list<CLineDataRecord*>::const_iterator itr = oLineDataList.begin();itr != oLineDataList.end();++itr)
			{
				CDataField* pField = (*itr)->FindWithFieldName(sName.c_str());
				if(pField)
				{
					pField->Set(sName.c_str() , sName.c_str() , sValue.c_str());
					pField = (*itr)->FindWithFieldName(_T("KEY"));
					const CString rKey(pField->value());

					CString rSql = _T("UPDATE T_ISO_LINE_DATA SET ");
					rSql += CString(sName.c_str()) + _T("='") + CString(sValue.c_str()) + _T("' WHERE KEY='") + rKey + _T("'");
					(*database.session()) << rSql.operator LPCTSTR();
					if((_T("PLANT_NO") == sName) || (_T("AREA_NO") == sName) || 
						(_T("UNIT_NO") == sName) || (_T("SECTION_NO") == sName))
					{
						pWorkspaceBar->m_wndQuery1.UpdateSelectionTreeList(true , NULL);
					}
				}

				/// updata line data in listctrl
				for(int row = 0;row < m_wndLineDataReport.GetItemCount();++row)
				{
					CLineDataRecord* pLineDataRecord = (CLineDataRecord*)(m_wndLineDataReport.GetItemData(row));
					if((*itr) == pLineDataRecord)
					{
						UpdateLineDataRecord(row , pLineDataRecord);
						break;
					}
				}
			}	
		}
	}
	catch(const std::exception& ex)
	{
		UNUSED_ALWAYS(ex);
	}

	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2012-08-12
    @function   OnLvnEndlabeledit
    @return     void
    @param      NMHDR   *pNMHDR
    @param      LRESULT *pResult
    @brief
******************************************************************************/
void CEditLineDataForm::OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	GV_DISPINFO* pDispinfo = (GV_DISPINFO*)pNMHDR;

	CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();
	CLineDataRecord *pLineDataRecord = (CLineDataRecord*)m_wndLineDataReport.GetItemData(pDispinfo->item.row);
	if(pLineDataRecord)
	{
		TCHAR szBuf[MAX_PATH] = {'\0' ,};
		LVCOLUMN lvcolumn;
		lvcolumn.mask = LVCF_TEXT;
		lvcolumn.cchTextMax = MAX_PATH;
		lvcolumn.pszText = szBuf;
		m_wndLineDataReport.GetColumn(pDispinfo->item.col , &lvcolumn);

		CDataField* pField = pLineDataRecord->FindWithFieldName(lvcolumn.pszText);
		if(pField)
		{
			CString rValue = pDispinfo->item.strText;

			pField->Set(lvcolumn.pszText , lvcolumn.pszText , rValue.operator LPCTSTR());
			pField = pLineDataRecord->FindWithFieldName(_T("KEY"));
			const CString rKey(pField->value());

			/// update database
			CAppDocData* pDocData = CAppDocData::GetInstance();
			const CProject::DatabaseProp prop = pDocData->GetProject()->GetDatabaseProp();
			try
			{
				CAppPostgreSQL database(pDocData->GetProject()->name() , prop.Host.c_str(),prop.User.c_str(),prop.Password.c_str());
				CString sColumnName = CString(lvcolumn.pszText);
				CString rSql = _T("UPDATE T_ISO_LINE_DATA SET ");
				rSql += sColumnName + _T("='") + rValue + _T("' WHERE KEY='") + rKey + _T("'");
				(*database.session()) << rSql.operator LPCTSTR();

				if((_T("PLANT_NO") == sColumnName) || (_T("AREA_NO") == sColumnName) || 
					(_T("UNIT_NO") == sColumnName) || (_T("SECTION_NO") == sColumnName))
				{
					pWorkspaceBar->m_wndQuery1.UpdateSelectionTreeList(true , NULL);
				}
			}
			catch(std::exception& ex)
			{
				AfxMessageBox(ex.what() , MB_OK|MB_ICONERROR);
			}

			UpdateLineDataRecord(pDispinfo->item.row , pLineDataRecord);
		}	
	}

	*pResult = 0;
}

void CEditLineDataForm::OnLvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
}

/**	
	@brief	run *.ris file
	@author	HumKyung
	@date	2011.01.25
	@return	int	
*/
int CEditLineDataForm::RunRIS()
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	try
	{
		CoInitialize(NULL);
		ISmartISOLayerPtr ptr(__uuidof(SmartISOLayer));

		VARIANT_BOOL varRet;
		CString sOutputFilePath(CFileTools::GetAppDataPath() + _T("\\") + CString(PRODUCT_NAME) + _T("\\ris.db"));
		::DeleteFile(sOutputFilePath);
		BSTR bstrOutputFilePath = sOutputFilePath.AllocSysString();
		BSTR bstrHost = pDocData->m_oOracleSetting.m_sHost.AllocSysString();
		BSTR bstrService = pDocData->m_oOracleSetting.m_sService.AllocSysString();
		BSTR bstrUserName= pDocData->m_oOracleSetting.m_sPrjUser.AllocSysString();
		BSTR bstrPassword= pDocData->m_oOracleSetting.m_sPrjPassword.AllocSysString();
		CString sPrjName(pDocData->GetProject()->name());
		BSTR bstrPrjName = sPrjName.AllocSysString();
		ptr->DumpPrjDatabase(bstrOutputFilePath,bstrHost,bstrService,bstrUserName,bstrPassword,&varRet);

		::SysFreeString(bstrOutputFilePath);
		::SysFreeString(bstrHost);
		::SysFreeString(bstrService);
		::SysFreeString(bstrUserName);
		::SysFreeString(bstrPassword);

		ptr->Release();
		CoUninitialize();

		return ERROR_SUCCESS;
	}
	catch(const std::exception& ex)
	{
		AfxMessageBox(ex.what(),MB_OK|MB_ICONERROR);
	}

	return ERROR_BAD_ENVIRONMENT;
}

/**	
	@brief	read *.out file
	@author	HumKyung
	@date	2011.01.25
	@return	int	
*/
int CEditLineDataForm::ReadRISResultFile(const CString& sFilePath)
{
	/// 121 , 122 Table을 읽어 온다.
	try
	{
		soci::session aSession(*soci::factory_sqlite3() , sFilePath.operator LPCTSTR());
		CPdTable121& s121Table = CPdTable121::GetInstance();
		s121Table.Read(aSession);
		
		CPdTable122& s122Table = CPdTable122::GetInstance();
		s122Table.Read(aSession);
	}
	catch(...)
	{
	}

	return ERROR_SUCCESS;
}

/**	@brief	OnProperties
	@author	HumKyung
	@date	2011.02.24
	@parm	
	@return	void
*/
void CEditLineDataForm::OnProperties()
{
	list<CLineDataRecord*> oLineDataRecordList;
	POSITION pos = m_wndLineDataReport.GetFirstSelectedItemPosition();
	while(pos)
	{
		const int nItem = m_wndLineDataReport.GetNextSelectedItem(pos);
		CLineDataRecord* pLineDataRecord = (CLineDataRecord*)m_wndLineDataReport.GetItemData(nItem);
		if(NULL != pLineDataRecord) oLineDataRecordList.push_back( pLineDataRecord );
	}
	if(!oLineDataRecordList.empty())
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
		pPropertiesWnd->DisplayDataOf(oLineDataRecordList);
	}
}

/**	@brief	replace selected columns data with given data
	@author	humkyung
	@date	2013.08.24
	@parm	
	@return	void
*/
void CEditLineDataForm::OnReplace()
{
	CPoint CurrentPosition;
	::GetCursorPos(&CurrentPosition);
	m_wndLineDataReport.ScreenToClient(&CurrentPosition);

	LVHITTESTINFO hitTestInfo;
	hitTestInfo.pt = CurrentPosition;
	const int nIndex = m_wndLineDataReport.SubItemHitTest(&hitTestInfo);
	if((-1 != nIndex) && (LVHT_ONITEMLABEL == hitTestInfo.flags))
	{
		IsGui::CListCtrlEx::ColumnState ColState = m_wndLineDataReport.GetColumnState(hitTestInfo.iSubItem);
		if(NULL == ColState.m_pInPlaceCtrlHolder) return;

		TCHAR szBuf[MAX_PATH + 1]={'\0',};
		LVCOLUMN lvcolumn;
		lvcolumn.mask = LVCF_TEXT;
		lvcolumn.cchTextMax = MAX_PATH;
		lvcolumn.pszText = szBuf;
		m_wndLineDataReport.GetColumn(hitTestInfo.iSubItem , &lvcolumn);

		CReplaceDlg dlg;
		dlg.m_sFind = m_wndLineDataReport.GetItemText(nIndex , hitTestInfo.iSubItem);
		if(IDOK == dlg.DoModal())
		{
			CAppDocData* pDocData = CAppDocData::GetInstance();
			const CProject::DatabaseProp prop = pDocData->GetProject()->GetDatabaseProp();

			try
			{
				CAppPostgreSQL database(pDocData->GetProject()->name(),prop.Host.c_str(),prop.User.c_str(),prop.Password.c_str());
				CString sSql;
				for(int row = 0;row < m_wndLineDataReport.GetItemCount();++row)
				{
					CLineDataRecord* pLineDataRecord = (CLineDataRecord*)(m_wndLineDataReport.GetItemData(row));
					const TCHAR* pKey = pLineDataRecord->GetKey();

					CString sText = m_wndLineDataReport.GetItemText(row , hitTestInfo.iSubItem);
					if(-1 != sText.Find(dlg.m_sFind))
					{
						sText.Replace(dlg.m_sFind , dlg.m_sReplace);
						sSql.Format(_T("UPDATE T_ISO_LINE_DATA SET %s='%s' WHERE KEY='%s'") , lvcolumn.pszText , sText , pKey);
						try
						{
							(*database.session()) << sSql.operator LPCTSTR();
							pLineDataRecord->Set(lvcolumn.pszText , sText);
							m_wndLineDataReport.SetItemText(row , hitTestInfo.iSubItem , sText);
						}
						catch(const std::exception& ex)
						{
							///TODO: try to display error text on listctrl
							AfxMessageBox(ex.what(),MB_OK|MB_ICONERROR);
						}
					}
				}
			}
			catch(const std::exception& ex)
			{
				AfxMessageBox(ex.what(),MB_OK|MB_ICONERROR);
			}
		}
	}
}

/******************************************************************************
    @author     humkyung
    @date       2012-05-24
    @class      CEditLineDataForm
    @function   OnHelpInfo
    @return     BOOL
    @param      HELPINFO*   pHelpInfo
    @brief
******************************************************************************/
BOOL CEditLineDataForm::OnHelpInfo(HELPINFO* pHelpInfo)
{
	HWND hHtml = ::HtmlHelp(this->m_hWnd , CAppDocData::GetExecPath() + _T("\\Help\\SmartISO.chm::/edit line data.htm") , HH_DISPLAY_TOPIC , 0);

	return TRUE;///CFormView::OnHelpInfo(pHelpInfo);
}

/******************************************************************************
    @author     humkyung
    @date       2012-07-31
    @class      CEditLineDataForm
    @function   OnDestroy
    @return     void
    @brief		save column order
******************************************************************************/
void CEditLineDataForm::OnDestroy()
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	if(pDocData)
	{
		CString sColumnOrder;
		const int iColumnCount = m_wndLineDataReport.GetColumnCount();
		LPINT pnOrder = (LPINT) malloc(iColumnCount*sizeof(int));
		m_wndLineDataReport.GetColumnOrderArray(pnOrder , iColumnCount);
		for(int i = 0;i < iColumnCount;++i)
		{
			const int iOrder = pnOrder[i];
			if(true == m_wndLineDataReport.IsColumnVisible(iOrder))
			{
				LVCOLUMN lvc = {0};
				lvc.mask = LVCF_TEXT;
				TCHAR sColText[256];
				lvc.pszText = sColText;
				lvc.cchTextMax = sizeof(sColText)-1;
				m_wndLineDataReport.GetColumn(iOrder , &lvc);

				if((_T("[]") != CString(lvc.pszText)) && (_T("REV_NO") != CString(lvc.pszText))) sColumnOrder += CString(lvc.pszText) + _T(":");
			}
		}
		sColumnOrder = sColumnOrder.TrimRight(_T(":"));
		::WritePrivateProfileString(_T("Line Data Field") , _T("Column Order") , sColumnOrder , pDocData->GetPrjIniFilePath());
	}

	CFormView::OnDestroy();
}

/**
	@brief	when click on control
	@autho	humkyung
	@date	2013.02.03
*/
void CEditLineDataForm::OnNMClickListLineList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	CPoint CurrentPosition;
    ::GetCursorPos(&CurrentPosition);
	m_wndLineDataReport.ScreenToClient(&CurrentPosition);

    LVHITTESTINFO hitTestInfo;
	hitTestInfo.pt = CurrentPosition;
	const int nIndex = m_wndLineDataReport.SubItemHitTest(&hitTestInfo);
	/*if((-1 != nIndex) && (0 == hitTestInfo.iSubItem) && (LVHT_ONITEMICON == hitTestInfo.flags))
	{
		LVITEM lvItem;
		lvItem.iItem = nIndex;
		lvItem.iSubItem = 0;
		lvItem.mask = LVIF_IMAGE;
		m_wndLineDataReport.GetItem(&lvItem);
		if(REVISION_DATA_ICON == lvItem.iImage)
		{
			lvItem.iImage = ARROW_EXPAND;
			m_wndLineDataReport.SetItem(&lvItem);

			CAppDocData* pDocData = CAppDocData::GetInstance();
			CADODB adoDB;
			STRING_T sConnString = pDocData->GetConnectionString();
			if(adoDB.DBConnect(sConnString))
			{
				CLineDataRecord* pLineDataRecord = (CLineDataRecord*)(m_wndLineDataReport.GetItemData(nIndex));

				const TCHAR* pKey = pLineDataRecord->GetKey();
				STRING_T sSql = _T("SELECT * FROM T_ISSUED_ISO_LINE_DATA WHERE KEY='") + STRING_T(pKey) + _T("'");
				if(TRUE == adoDB.OpenQuery(sSql))
				{
					LVITEM lvItem;
					lvItem.mask = LVIF_TEXT | LVIF_IMAGE;
					lvItem.iImage = ARROW_LINE;
					lvItem.cchTextMax = 32;
					lvItem.iItem = ++nIndex;
					lvItem.iSubItem = 0;
					lvItem.pszText = _T("");

					LVCOLUMN lvc = {0};
					lvc.mask = LVCF_TEXT;
					TCHAR sColText[256];
					lvc.pszText = sColText;
					lvc.cchTextMax = sizeof(sColText)-1;
					const int iNewItem = m_wndLineDataReport.InsertItem(&lvItem);
					const int iColumnCount = m_wndLineDataReport.GetColumnCount();
					for(int i = 0;i < iColumnCount;++i)
					{
						STRING_T sValue;
						m_wndLineDataReport.GetColumn(i , &lvc);
						if(TRUE == adoDB.GetFieldValue(0 , lvc.pszText , &sValue))
						{
							m_wndLineDataReport.SetItemText(iNewItem , i , sValue.c_str());
							m_wndLineDataReport.SetItemTextColor(iNewItem , i , FIXED_ITEM_COLOR , FALSE);

						}
					}
				}
			}
		}
		else if(ARROW_EXPAND == lvItem.iImage)
		{
			lvItem.iImage = REVISION_DATA_ICON;
			m_wndLineDataReport.SetItem(&lvItem);

			m_wndLineDataReport.DeleteItem(nIndex + 1);
		}
	}
	else */if((-1 != nIndex) && (LVHT_ONITEMLABEL == hitTestInfo.flags))
	{
		CLineDataRecord *pLineDataRecord = (CLineDataRecord*)m_wndLineDataReport.GetItemData(nIndex);
		if(NULL != pLineDataRecord) m_pQualifiedLineForm->DisplayRevisionDataRecordSet(pLineDataRecord);

		CPropertiesWnd* pPropertiesWnd = CPropertiesWnd::GetInstance();
		if(pPropertiesWnd->IsVisible())
		{
			list<CLineDataRecord*> oLineDataRecordList;
			if(NULL != pLineDataRecord) oLineDataRecordList.push_back( pLineDataRecord );
			pPropertiesWnd->DisplayDataOf(oLineDataRecordList);
		}
	}

	*pResult = 0;
}

/**
	@brief	force uncheck if iso drawing file is not found
	@author	humkyung
	@date	2015.10.22
*/
void CEditLineDataForm::OnLvnItemchangedListLineList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}
