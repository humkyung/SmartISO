// LineDataSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Admin.h"
#include "AdminDocData.h"
#include "tokenizer.h"
#include "LineDataSettingDlg.h"
#include "LineDataSettingGridCell.h"
#include <ado/SQLFile.h>
#include <ProjectData.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDC_NEW             1010
#define IDC_REMOVE			1020

//////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CTransparentCheckBox, CButton)
CTransparentCheckBox::CTransparentCheckBox()
{
}

CTransparentCheckBox::~CTransparentCheckBox()
{
}


BEGIN_MESSAGE_MAP(CTransparentCheckBox, CButton)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CTransparentCheckBox message handlers


HBRUSH CTransparentCheckBox::CtlColor(CDC* pDC, UINT /*nCtlColor*/)
{
	pDC->SetBkMode(TRANSPARENT);
	return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
}

BOOL CTransparentCheckBox::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CLineDataSettingDlg dialog


CLineDataSettingDlg::CLineDataSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLineDataSettingDlg::IDD, pParent) , m_pAdminDoc(NULL) , m_pIssueDateRecordSet(NULL)
{
	//{{AFX_DATA_INIT(CLineDataSettingDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CLineDataSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLineDataSettingDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//DDX_Control(pDX , IDC_LIST_LINE_DATA_MODULE_KEY , m_LineDataModuleKeyListCtrl);
	
	/// 2013.01.05 added by humkyung
	DDX_Control(pDX , IDC_RADIO_ISSUE_DATE_FORMAT_NO_1 , m_btnIssueDate1);
	DDX_Control(pDX , IDC_RADIO_ISSUE_DATE_FORMAT_NO_2 , m_btnIssueDate2);
	DDX_Control(pDX , IDC_RADIO_ISSUE_DATE_FORMAT_NO_3 , m_btnIssueDate3);
	DDX_Control(pDX , IDC_RADIO_ISSUE_DATE_FORMAT_NO_4 , m_btnIssueDate4);
	/// up to here

	DDX_Control(pDX , IDC_BUTTON_NEW , m_btnNew);
	DDX_Control(pDX , IDC_BUTTON_REMOVE , m_btnRemove);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLineDataSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CLineDataSettingDlg)
	ON_WM_SIZE()
	///ON_NOTIFY(NM_DBLCLK, IDC_LIST_LINE_DATA_FIELD, OnDblclkListLineDataField)
	ON_BN_CLICKED(IDC_BUTTON_NEW, OnButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, OnButtonRemove)
	
	ON_EN_CHANGE(IDC_EDIT_ISSUE_DATE_DELIMITER, OnChangeIssueDateDelimiter)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_RADIO_ISSUE_DATE_FORMAT_NO_1, &CLineDataSettingDlg::OnBnClickedRadioIssueDateFormatNo1)
	ON_BN_CLICKED(IDC_RADIO_ISSUE_DATE_FORMAT_NO_2, &CLineDataSettingDlg::OnBnClickedRadioIssueDateFormatNo2)
	ON_BN_CLICKED(IDC_RADIO_ISSUE_DATE_FORMAT_NO_3, &CLineDataSettingDlg::OnBnClickedRadioIssueDateFormatNo3)
	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDC_RADIO_ISSUE_DATE_FORMAT_NO_4, &CLineDataSettingDlg::OnBnClickedRadioIssueDateFormatNo4)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLineDataSettingDlg message handlers

/**	
	@brief	다이얼로그 초기화 부분
	@author	humkyung
**/
BOOL CLineDataSettingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CAdminDocData* pAdminDocData = CAdminDocData::GetInstance();

	TCHAR szBuf[MAX_PATH] = {'\0' ,};
	CString rApp( _T("Line Data Field") );

	if(TRUE == m_wndRevisionStageDlg.Create(CRevisionStageDlg::IDD , this))
	{
		m_wndRevisionStageDlg.ShowWindow(SW_NORMAL);
	}

	CWnd* pWnd = GetDlgItem(IDC_STATIC_ISSUE_DATE);
	if(NULL != pWnd) pWnd->SetWindowText(_TR("Issue Date"));

	if( m_issueDateGridCtrl.Create(CRect(0 , 0 , 0 , 0) , this , 0x100) )
	{
		m_issueDateGridCtrl.SetEditable(TRUE);
		m_issueDateGridCtrl.EnableDragAndDrop(FALSE);
		m_issueDateGridCtrl.SetRowResize(TRUE);
		m_issueDateGridCtrl.SetColumnResize(TRUE);
		m_issueDateGridCtrl.SetListMode(FALSE);
		m_issueDateGridCtrl.SetSingleRowSelection(TRUE);
		m_issueDateGridCtrl.SetHeaderSort(FALSE);
		m_issueDateGridCtrl.SetSingleColSelection(TRUE);
	}
		
	{
		HICON hIcon = (HICON) LoadImage(AfxGetInstanceHandle(),
									MAKEINTRESOURCE(IDI_ADD),
									IMAGE_ICON, 16, 16, 0);
		HICON hOldIcon = m_btnNew.SetIcon(hIcon);
		if(hOldIcon) DestroyIcon(hOldIcon);
	}
	{
		HICON hIcon = (HICON) LoadImage(AfxGetInstanceHandle(),
									MAKEINTRESOURCE(IDI_REMOVE),
									IMAGE_ICON, 16, 16, 0);
		HICON hOldIcon = m_btnRemove.SetIcon(hIcon);
		if(hOldIcon) DestroyIcon(hOldIcon);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/**
	@brief
	@author	humkyung
**/
void CLineDataSettingDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	static const int gap = 12;

	if(m_wndRevisionStageDlg.GetSafeHwnd())
	{
		CRect rect;
		m_wndRevisionStageDlg.GetWindowRect(&rect);

		m_wndRevisionStageDlg.SetWindowPos(NULL , 0 , 0 , cx , rect.Height() , SWP_NOZORDER);
		m_wndRevisionStageDlg.GetWindowRect(&rect);
		m_wndRevisionStageDlg.ScreenToClient(&rect);

		CWnd* pWnd = GetDlgItem(IDC_STATIC_ISSUE_DATE);
		if(pWnd)
		{
			CRect rectThis;
			pWnd->GetWindowRect(&rectThis);
			ScreenToClient(&rectThis);
			
			int nWidth = (int)((cx - gap*2));
			CRect rectGroup;
			pWnd->SetWindowPos(NULL , gap , rect.bottom + gap , nWidth , cy - rect.bottom - gap , SWP_NOZORDER);
			pWnd->GetWindowRect(&rectGroup);
			ScreenToClient(&rectGroup);
			
			m_btnNew.SetWindowPos(NULL , rectGroup.right - 61 , rectGroup.top + 40 , 25 , 25 , SWP_NOZORDER|SWP_NOSIZE);
			m_btnRemove.SetWindowPos(NULL , rectGroup.right - 35 , rectGroup.top + 40 , 25 , 25 , SWP_NOZORDER|SWP_NOSIZE);
			m_btnNew.GetWindowRect(&rectThis);
			ScreenToClient(&rectThis);
			
			/// 2011.07.17 - added by humkyung
			{
				pWnd = GetDlgItem(IDC_RADIO_ISSUE_DATE_FORMAT_NO_1);
				pWnd->SetWindowPos(NULL , rectGroup.left + 10 , rectGroup.top + 20 , 25 , 25 , SWP_NOSIZE | SWP_NOZORDER);
				pWnd->GetWindowRect(&rectThis);
				ScreenToClient(&rectThis);
				pWnd = GetDlgItem(IDC_RADIO_ISSUE_DATE_FORMAT_NO_2);
				pWnd->SetWindowPos(NULL , rectThis.right + 10 , rectGroup.top + 20 , 25 , 25 , SWP_NOSIZE | SWP_NOZORDER);

				/// 2011.08.01 added by humkyung
				pWnd->GetWindowRect(&rectThis);
				ScreenToClient(&rectThis);
				pWnd = GetDlgItem(IDC_RADIO_ISSUE_DATE_FORMAT_NO_3);
				pWnd->SetWindowPos(NULL , rectThis.right + 10 , rectGroup.top + 20 , 25 , 25 , SWP_NOSIZE | SWP_NOZORDER);

				pWnd->GetWindowRect(&rectThis);
				ScreenToClient(&rectThis);
				pWnd = GetDlgItem(IDC_RADIO_ISSUE_DATE_FORMAT_NO_4);
				pWnd->SetWindowPos(NULL , rectThis.right + 10 , rectGroup.top + 20 , 25 , 25 , SWP_NOSIZE | SWP_NOZORDER);
				
				pWnd = GetDlgItem(IDC_RADIO_ISSUE_DATE_FORMAT_NO_1);
				pWnd->GetWindowRect(&rectThis);
				ScreenToClient(&rectThis);
				/// up to this
				
				pWnd = GetDlgItem(IDC_STATIC_ISSUE_DATE_DELIMITER);
				pWnd->SetWindowPos(NULL , rectThis.left , rectThis.bottom + 10 , 0 , 0 , SWP_NOSIZE | SWP_NOZORDER);
				pWnd->GetWindowRect(&rectThis);
				ScreenToClient(&rectThis);
				pWnd = GetDlgItem(IDC_EDIT_ISSUE_DATE_DELIMITER);
				pWnd->SetWindowPos(NULL , rectThis.right + 5 , rectThis.top - 5 , 0 , 0 , SWP_NOSIZE | SWP_NOZORDER);
			}

			if(m_issueDateGridCtrl.GetSafeHwnd())
			{
				m_issueDateGridCtrl.SetWindowPos(NULL , rectGroup.left + 10 , rectThis.bottom + 10 , rectGroup.Width() - 20, rectGroup.Height() - 75 , SWP_NOZORDER);
			}
		}

		CWnd* pGroupWnd = GetDlgItem(IDC_GROUP_LINE_DATA_MODULE_KEY);
		if(pGroupWnd)
		{
			CRect rect;
			CWnd* pWnd = GetDlgItem(IDC_STATIC_ISSUE_DATE);
			if(pWnd)
			{
				pWnd->GetWindowRect(&rect);
				ScreenToClient(&rect);
				
				pGroupWnd->SetWindowPos(NULL , rect.right + gap , rect.top , rect.Width() , rect.Height() , SWP_NOZORDER);
				pGroupWnd->GetWindowRect(&rect);
				ScreenToClient(&rect);
			}
		}

		pWnd = GetDlgItem(IDC_STATIC_SHOW_SET);
		if(pWnd)
		{
			CRect rect;
			CWnd* pWnd1 = GetDlgItem(IDC_GROUP_LINE_DATA_MODULE_KEY);
			if(pWnd1)
			{
				pWnd1->GetWindowRect(&rect);
				ScreenToClient(&rect);
			}
			
			CRect rectThis;
			pWnd->SetWindowPos(NULL , rect.right + gap , rect.top , rect.Width() , rect.Height(), SWP_NOZORDER);
			pWnd->GetWindowRect(&rectThis);
			ScreenToClient(&rectThis);
		}
	}
}

/**	
	@brief	프로젝트가 로드되고 난뒤 폼을 초기화 시킨다.
	@author	HumKyung.BAEK
	@return	void	
*/
void CLineDataSettingDlg::UpdateContents()
{
	static bool __init__ = false;

	if((false == __init__) && m_pAdminDoc)
	{
		CString rIniFilePath = m_pAdminDoc->GetCurProject()->GetIniFilePath();

		m_wndRevisionStageDlg.UpdateContents();

		TCHAR szBuf[MAX_PATH] = {'\0' ,};
		CString rApp = _T("Line Data Field");
		
		const int nCount = GetPrivateProfileInt(rApp , _T("Count") , 0 , rIniFilePath);

		vector<string> oPrimaryKeys;
		//////////////////////////////////////////////////////////////////////////
		if(GetPrivateProfileString(rApp , _T("Primary Key") , _T("") , szBuf , MAX_PATH , rIniFilePath))
		{
			CTokenizer<CIsFromString>::Tokenize(oPrimaryKeys , szBuf , CIsFromString( _T(":") ));
		}

		//vector<string> oLineDataModuleKeys , oResult;;
		////////////////////////////////////////////////////////////////////////////
		//if(GetPrivateProfileString(rApp , _T("Line Data Module Key") , _T("") , szBuf , MAX_PATH , rIniFilePath))
		//{
		//	CTokenizer<CIsFromString>::Tokenize(oLineDataModuleKeys , szBuf , CIsFromString( _T(":") ));
		//}
		//m_LineDataModuleKeyListCtrl.DeleteAllItems();
		//int i = 0;
		//for(i = 0;i < nCount;i++)
		//{
		//	CString rKey;
		//	rKey.Format(_T("NAME%d") , i);
		//	if(GetPrivateProfileString(rApp , rKey , NULL , szBuf , MAX_PATH , rIniFilePath))
		//	{
		//		CString rId;
		//		rId.Format(_T("%d") , i);

		//		CTokenizer<CIsFromString>::Tokenize(oResult , szBuf , CIsFromString(":"));
		//		int nItem = m_LineDataModuleKeyListCtrl.InsertItem(i , oResult[1].c_str());
		//		if(!oLineDataModuleKeys.empty())
		//		{
		//			vector<string>::iterator where = find(oLineDataModuleKeys.begin() , oLineDataModuleKeys.end() , oResult[1].c_str());
		//			if(oLineDataModuleKeys.end() != where)
		//			{
		//				m_LineDataModuleKeyListCtrl.SetCheck(nItem , TRUE);
		//			}
		//		}
		//	}
		//}

		/// issue date의 헤더 부분을 초기화 시킨다.		
		/// issue date의 내용을 표기한다.
		//! display issue date format and delimiter - 2011.07.17 added by humkyung
		CProjectData* pProjectData = CProjectData::GetInstance();
		{
			if(NULL != pProjectData)
			{
				const CString sIssueDateFormatNo = pProjectData->GetIssueDateFormatNo();
				CButton* pButton[4] = {NULL,NULL,NULL,NULL};
				pButton[0] = (CButton*)GetDlgItem(IDC_RADIO_ISSUE_DATE_FORMAT_NO_1);
				pButton[1] = (CButton*)GetDlgItem(IDC_RADIO_ISSUE_DATE_FORMAT_NO_2);
				pButton[2] = (CButton*)GetDlgItem(IDC_RADIO_ISSUE_DATE_FORMAT_NO_3);
				pButton[3] = (CButton*)GetDlgItem(IDC_RADIO_ISSUE_DATE_FORMAT_NO_4);

				pButton[0]->SetCheck(_T("1") == sIssueDateFormatNo);
				pButton[1]->SetCheck(_T("2") == sIssueDateFormatNo);
				pButton[2]->SetCheck(_T("3") == sIssueDateFormatNo);
				pButton[3]->SetCheck(_T("4") == sIssueDateFormatNo);
				
				const CString sIssueDateDelimiter = pProjectData->GetIssueDateDelimiter();
				SetDlgItemText(IDC_EDIT_ISSUE_DATE_DELIMITER , sIssueDateDelimiter);
			}
		}
		//! up to here

		m_pIssueDateRecordSet = m_pAdminDoc->GetCurProject()->LoadIssueDateRecordSet();
		DisplayIssueDataRecordSet();

		__init__ = true;
	}
}

/**	
	@brief	사용자가 설정한 값들을 저장한다.
	@author	humkyung
	@return	void	
*/
void CLineDataSettingDlg::Save()
{
	CProject* pProject = m_pAdminDoc->GetCurProject();
	if(NULL != pProject)
	{
		const CString sIniFilePath = pProject->path() + CString( _T("\\Setting\\") ) + pProject->name() + CString( _T(".ini") );
		const CProject::DatabaseProp prop = pProject->GetDatabaseProp();
		
		CString rApp( _T("Line Data Field") );

		LVITEM lvitem;
		lvitem.mask = LVIF_IMAGE;

		/// set issue date format no and delimiter - 2011.07.17 added by humkyung
		CProjectData* pProjectData = CProjectData::GetInstance();
		if(NULL != pProjectData)
		{
			const CString sIssueDateFormatNo = pProjectData->GetIssueDateFormatNo();
			CButton* pButton[4] = {NULL,NULL,NULL,NULL};
			pButton[0] = (CButton*)GetDlgItem(IDC_RADIO_ISSUE_DATE_FORMAT_NO_1);
			pButton[1] = (CButton*)GetDlgItem(IDC_RADIO_ISSUE_DATE_FORMAT_NO_2);
			pButton[2] = (CButton*)GetDlgItem(IDC_RADIO_ISSUE_DATE_FORMAT_NO_3);
			pButton[3] = (CButton*)GetDlgItem(IDC_RADIO_ISSUE_DATE_FORMAT_NO_4);
			if(TRUE == pButton[0]->GetCheck())
			{
				pProjectData->SetIssueDateFormatNo(_T("1"));
			}
			else if(TRUE == pButton[1]->GetCheck())
			{
				pProjectData->SetIssueDateFormatNo(_T("2"));
			}
			else if(TRUE == pButton[2]->GetCheck())
			{
				pProjectData->SetIssueDateFormatNo(_T("3"));
			}
			else if(TRUE == pButton[3]->GetCheck())
			{
				pProjectData->SetIssueDateFormatNo(_T("4"));
			}
			
			CString sIssueDateDelimiter;
			GetDlgItemText(IDC_EDIT_ISSUE_DATE_DELIMITER , sIssueDateDelimiter);
			pProjectData->SetIssueDateDelimiter(sIssueDateDelimiter);
			pProjectData->Save(pProject->name(),prop);
		}

		const int nIssueDateCount = m_issueDateGridCtrl.GetRowCount();
		for(int i = 1; i < nIssueDateCount ; ++i)
		{
			CGridCellBase* pCell = m_issueDateGridCtrl.GetCell(i , 1);
			if(pCell)
			{
				const int nIDX = (int)(pCell->GetData());

				CString rIssueDateString = m_oPureIssueDateMap[m_issueDateGridCtrl.GetCell(i , 1)];
				if( rIssueDateString.IsEmpty()) continue;
				if(rIssueDateString.IsEmpty()) rIssueDateString = _T(" ");
				
				/// 2011.08.06 added by humkyung
				int iYear = 0 , iMonth = 0 , iDay = 0;
				if(ERROR_SUCCESS == pProjectData->ParseIssueDate(iYear , iMonth , iDay , rIssueDateString , _T("-")))
				{
					rIssueDateString.Format(_T("%04d-%02d-%02d") , iYear , iMonth , iDay);
					m_pIssueDateRecordSet->Add(nIDX , rIssueDateString);
				}
				/// up to here
			}
		}

		m_pAdminDoc->GetCurProject()->SaveIssueDateRecordSet();
		DisplayIssueDataRecordSet();

		m_wndRevisionStageDlg.SaveData();
	}
}

/**	
	@brief	새로운 issue date를 추가한다.
	@author	humkyung
	@return	void	
*/
void CLineDataSettingDlg::OnButtonNew() 
{
	int nRowCount = m_issueDateGridCtrl.GetRowCount();
	int nColCount = m_issueDateGridCtrl.GetColumnCount();

	m_issueDateGridCtrl.SetRowCount(nRowCount + 1);

	for(int i =0 ; i< nColCount ; ++i)
	{
		m_issueDateGridCtrl.SetItemFormat(nRowCount , i , DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		CGridCellBase* pCell = m_issueDateGridCtrl.GetCell(nRowCount , i);
		if( pCell ) pCell->SetData( -1 );	/// 새로 생성된 issue date이다.(database에 저장되기 전)
		if(1 == i)
		{
			m_issueDateGridCtrl.SetCellType(nRowCount, i , RUNTIME_CLASS(GridCell::CLineDataSettingGridCell));
			m_oPureIssueDateMap.insert(make_pair(pCell , _T("")));
		}
	}

	/*pCell = m_issueDateGridCtrl.GetCell( nRowCount , 1 );
	if( pCell )
	{
		m_oPureIssueDateMap.insert(make_pair(pCell , _T("")));
	}*/
	/// up to here
}

/**	
	@brief	선택한 issue 날짜를 삭제한다.
	@author	humkyung
	@return	void	
*/
void CLineDataSettingDlg::OnButtonRemove() 
{
	CCellRange range = m_issueDateGridCtrl.GetSelectedCellRange();
	CCellID CellId = range.GetTopLeft();
	if((CellId.row >= 1) && (CellId.row <= m_issueDateGridCtrl.GetRowCount()-1))
	{
		CGridCellBase* pCell = m_issueDateGridCtrl.GetCell( CellId.row , 0 );
		//if( -1 == (int)(pCell->GetData()) )
		{
			const int ret = AfxMessageBox( _T("Do you want to delete selected row(s)?") , MB_YESNO);
			if(IDYES == ret)
			{
				int nIDX = pCell->GetData();
				m_pIssueDateRecordSet->DeleteRecord( nIDX );

				/// 2013.01.05 addeed by humkyung
				map<CGridCellBase* , CString>::iterator where = m_oPureIssueDateMap.find(pCell);
				if(where != m_oPureIssueDateMap.end())
				{
					m_oPureIssueDateMap.erase(where);
				}
				where = m_oPureIssueDateMap.find(m_issueDateGridCtrl.GetCell( CellId.row , 1 ));
				if(where != m_oPureIssueDateMap.end())
				{
					m_oPureIssueDateMap.erase(where);
				}
				/// up to here

				m_issueDateGridCtrl.DeleteRow(CellId.row);
				m_issueDateGridCtrl.RedrawWindow();
			}
		}
		//else
		{
		//	CString rString;
		//	rString.LoadString(IDS_WARNING_DELETE_ISSUE_DATE);
		//	AfxMessageBox(rString , MB_OK);
		}
	}
}

/**	
	@brief	The CLineDataSettingDlg::CheckIssueDate function
	@author	humkyung
	@param	rIssueDate	a parameter of type const CString &
	@return	bool	
*/
bool CLineDataSettingDlg::CheckIssueDate(const CString &rIssueDate)
{
	return (m_pIssueDateRecordSet->LessOrEqual( rIssueDate ) ? false : true);
}

/**
**/
BOOL CLineDataSettingDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN ) // Enter Key Prevent.. 
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
	@brief	display issue date
	@author	humkyung
	@date	????.??.??
**/
void CLineDataSettingDlg::DisplayIssueDataRecordSet(const CString& sOldDelimiter , const CString& sNewDelimiter)
{
	CProjectData* pProjectData = CProjectData::GetInstance();
	
	if(!sOldDelimiter.IsEmpty() && !sNewDelimiter.IsEmpty())
	{
		const int nCount = m_issueDateGridCtrl.GetRowCount();
		for(int i = 1;i < nCount;++i)
		{
			CString sItemText = m_issueDateGridCtrl.GetItemText(i , 0);
			sItemText.Replace(sOldDelimiter , sNewDelimiter);
			m_issueDateGridCtrl.SetItemText(i , 0 , sItemText);

			sItemText = m_issueDateGridCtrl.GetItemText(i , 1);
			sItemText.Replace(sOldDelimiter , sNewDelimiter);
			m_issueDateGridCtrl.SetItemText(i , 1 , sItemText);
		}

		m_issueDateGridCtrl.Invalidate();
	}
	else if(NULL != m_pIssueDateRecordSet)
	{
		m_issueDateGridCtrl.SetRedraw(FALSE);
		m_issueDateGridCtrl.DeleteAllItems();

		static const TCHAR* __pColumnLabels[]={_T("Idx") , _T("Issue Date")};
		static const int    __pColumnWidths[]={50 , 150};
		const int nColCount = sizeof(__pColumnLabels) / sizeof(const TCHAR*);
		m_issueDateGridCtrl.SetRowCount( 1 );
		m_issueDateGridCtrl.SetColumnCount(nColCount);
		m_issueDateGridCtrl.SetFixedRowCount(1);
		m_issueDateGridCtrl.SetFixedColumnCount(1);
		for(int i = 0;i < nColCount;++i)
		{
			m_issueDateGridCtrl.SetItemText(0 , i, __pColumnLabels[i]);
			m_issueDateGridCtrl.SetColumnWidth(i,__pColumnWidths[i]);
		}

		const size_t nCount = m_pIssueDateRecordSet->GetCount();
		m_issueDateGridCtrl.SetRowCount( nCount + 1 );
		for(int i = 0;i < int(nCount);i++)
		{
			CIssueDateRecord* pRecord = m_pIssueDateRecordSet->GetAt(i);
			if( pRecord )
			{
				m_issueDateGridCtrl.SetItemFormat( i + 1 , 0 , DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				CString str;
				str.Format(_T("%d") , pRecord->index());
				m_issueDateGridCtrl.SetItemText(i + 1 , 0 , str);
				CGridCellBase* pCell = m_issueDateGridCtrl.GetCell( i + 1 , 0 );
				if( pCell ) pCell->SetData( pRecord->index() );

				m_issueDateGridCtrl.SetItemFormat( i + 1 , 1 , DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				CString sDisplayIssueDate = pProjectData->GetDisplayIssueDateFrom(pRecord->dateString());
				m_issueDateGridCtrl.SetItemText(i + 1 , 1 , sDisplayIssueDate);
				pCell = m_issueDateGridCtrl.GetCell( i + 1 , 1 );
				if( pCell )
				{
					pCell->SetState( GVNI_READONLY );
					pCell->SetData( pRecord->index() );
					pCell->SetBackClr( RGB(255, 255, 224) );
				}
				/// 2013.01.05 added by humkyung
				m_oPureIssueDateMap.insert(make_pair(pCell , pRecord->dateString()));
				/// up to here
			}
		}
		m_issueDateGridCtrl.SetRowHeight(0,20);
		m_issueDateGridCtrl.SetRedraw(TRUE);
	}
}

/**
	@breif	change issue date label with delimiter given by user

	@author	humkyung

	@date	2011.08.01
*/
void CLineDataSettingDlg::OnChangeIssueDateDelimiter() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	CString sDelimiter;
	GetDlgItemText(IDC_EDIT_ISSUE_DATE_DELIMITER , sDelimiter);
	
	CString sString;
	sString.Format(_T("Year%sMonth%sDay") , sDelimiter , sDelimiter);
	SetDlgItemText(IDC_RADIO_ISSUE_DATE_FORMAT_NO_1 , sString);

	sString.Format(_T("Month%sDay%sYear") , sDelimiter , sDelimiter);
	SetDlgItemText(IDC_RADIO_ISSUE_DATE_FORMAT_NO_2 , sString);

	CTime time = CTime::GetCurrentTime();
	CString sMonthFormat = time.Format(_T("%b"));
	sMonthFormat.MakeUpper();
	sString.Format(_T("Day%s%s%sYear") , sDelimiter , sMonthFormat , sDelimiter);
	SetDlgItemText(IDC_RADIO_ISSUE_DATE_FORMAT_NO_3 , sString);

	sString.Format(_T("%s%sDay%sYear") , sMonthFormat , sDelimiter , sDelimiter);
	SetDlgItemText(IDC_RADIO_ISSUE_DATE_FORMAT_NO_4 , sString);

	/// 2011.08.06 added by humkyung
	CProjectData* pProjectData = CProjectData::GetInstance();
	const CString sOldDelimiter = pProjectData->GetIssueDateDelimiter();
	pProjectData->SetIssueDateDelimiter(sDelimiter);
	DisplayIssueDataRecordSet(sOldDelimiter , sDelimiter);
	/// up to here
}

/******************************************************************************
    @author    humkyung
    @date      2011-08-08
    @class     CLineDataSettingDlg
    @function  OnBnClickedRadioIssueDateFormatNo1
    @return    void
    @brief	user pick issue format no "1"
******************************************************************************/
void CLineDataSettingDlg::OnBnClickedRadioIssueDateFormatNo1()
{
	CProjectData* pProjectData = CProjectData::GetInstance();
	pProjectData->SetIssueDateFormatNo(_T("1"));
	
	DisplayIssueDataRecordSet();
}

/******************************************************************************
    @author    humkyung
    @date      2011-08-08
    @class     CLineDataSettingDlg
    @function  OnBnClickedRadioIssueDateFormatNo2
    @return    void
    @brief	user pick issue format no "2"
******************************************************************************/
void CLineDataSettingDlg::OnBnClickedRadioIssueDateFormatNo2()
{
	CProjectData* pProjectData = CProjectData::GetInstance();
	pProjectData->SetIssueDateFormatNo(_T("2"));

	DisplayIssueDataRecordSet();
}

/******************************************************************************
    @author    humkyung
    @date      2011-08-08
    @class     CLineDataSettingDlg
    @function  OnBnClickedRadioIssueDateFormatNo3
    @return    void
    @brief	user pick issue format no "3"
******************************************************************************/
void CLineDataSettingDlg::OnBnClickedRadioIssueDateFormatNo3()
{
	CProjectData* pProjectData = CProjectData::GetInstance();
	pProjectData->SetIssueDateFormatNo(_T("3"));

	DisplayIssueDataRecordSet();
}

/******************************************************************************
    @author     humkyung
    @date       2012-06-05
    @class      CLineDataSettingDlg
    @function   OnHelpInfo
    @return     BOOL
    @brief
******************************************************************************/
BOOL CLineDataSettingDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	HWND hHtml = ::HtmlHelp(this->m_hWnd , CAdminDocData::GetExecPath() + _T("\\Help\\SmartISO.chm::/Line Data Setting.htm") , HH_DISPLAY_TOPIC , 0);

	return TRUE;///CDialog::OnHelpInfo(pHelpInfo);
}

/******************************************************************************
    @author    humkyung
    @date      2013-01-05
    @class     CLineDataSettingDlg
    @function  OnBnClickedRadioIssueDateFormatNo3
    @return    void
    @brief	user pick issue format no "4"
******************************************************************************/
void CLineDataSettingDlg::OnBnClickedRadioIssueDateFormatNo4()
{
	CProjectData* pProjectData = CProjectData::GetInstance();
	pProjectData->SetIssueDateFormatNo(_T("4"));

	DisplayIssueDataRecordSet();
}
