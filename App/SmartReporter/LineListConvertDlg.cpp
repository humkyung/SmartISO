// LineListConvertDlg.cpp : implementation file
//

#include "stdafx.h"

#include "SmartReporter.h"
#include "LineListToOraDocData.h"
#include <ConvertLineListToOra.h>
#include "LineListConvertDlg.h"
#include "enablebuddybutton.h"
#include "WorkStatusDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const STRING_T CLineListConvertDlg::APP( _T("Excel") );
/////////////////////////////////////////////////////////////////////////////
// CLineListConvertDlg dialog


CLineListConvertDlg::CLineListConvertDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLineListConvertDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLineListConvertDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CLineListConvertDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLineListConvertDlg)
	DDX_Control(pDX , IDC_LIST_SHEET_NAME , m_wndSheetNameListCtrl);
	DDX_Control(pDX , IDC_EDIT_RowNum , m_wndStartRowEdit);
	DDX_Control(pDX , IDC_COMBO_CONV_RESULT_COL , m_wndConvResultCol);
	DDX_Control(pDX , IDC_BUTTON_LINE_LIST_CONVERT , m_convertButton);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLineListConvertDlg, CDialog)
	//{{AFX_MSG_MAP(CLineListConvertDlg)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_LINE_LIST_CONVERT, OnButtonLineListConvert)
	ON_BN_CLICKED(IDC_BUTTON_SelectExcel, OnBUTTONSelectExcel)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_ALL, OnButtonSelectAll)
	ON_BN_CLICKED(IDC_BUTTON_UNSELECT_ALL, OnButtonUnselectAll)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_SHEET_NAME, OnButtonLoadSheetName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLineListConvertDlg message handlers

BOOL CLineListConvertDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CLineListToOraDocData& docData = CLineListToOraDocData::GetInstance();

	m_Font.CreateFont(18,0,0,0 ,FW_BOLD,FALSE,FALSE,0,0,0,0,0,0, _T("Arial") );
	m_16UnderlineFont.CreateFont(16,0,0,0 ,FW_NORMAL ,FALSE , TRUE ,0,0,0,0,0,0, _T("Arial") );
	
	CWnd* pWnd = GetDlgItem(IDC_STATIC_1);
	pWnd->SetFont(&m_Font);
	
	COLORREF crBtnColor = ::GetSysColor(COLOR_BTNFACE) + RGB(30, 30, 30);
	
	HWND hwndEdit  = GetDlgItem(IDC_STATIC_ShowExcelName)->GetSafeHwnd();
	m_selectExcelButton.SubclassWindow(GetDlgItem(IDC_BUTTON_SelectExcel)->m_hWnd);
	m_selectExcelButton.SetThemeHelper(&m_themeHelperST);
	m_selectExcelButton.SetFlat(FALSE);
	m_selectExcelButton.SetColor(CButtonST::BTNST_COLOR_BK_IN, crBtnColor);
	m_selectExcelButton.SetBtnCursor(IDC_APP_HAND);
	//HWND hwndButton= GetDlgItem(IDC_BUTTON_SelectExcel)->GetSafeHwnd();
	VERIFY( ::EnableBuddyButton( hwndEdit, m_selectExcelButton.m_hWnd , BBS_RIGHT ) );
	
	m_wndStartRowEdit.SetRegEx( _T("[0-9]*") );
	m_wndStartRowEdit.SetBackgroundColourFocus (RGB (150, 200, 255));
	m_wndStartRowEdit.SetForegroundColourError (RGB (255, 255, 255));
	m_wndStartRowEdit.SetBackgroundColourError (RGB (255, 0, 0));
	m_wndStartRowEdit.CreateToolTip(this , _T("Number only") );
	
	m_loadSheetNameButton.SubclassWindow(GetDlgItem(IDC_BUTTON_LOAD_SHEET_NAME)->m_hWnd);
	m_loadSheetNameButton.SetThemeHelper(&m_themeHelperST);
	m_loadSheetNameButton.SetIcon(IDI_SHEET , 16 , 16);
	m_loadSheetNameButton.SetFont(&m_16UnderlineFont);
	m_loadSheetNameButton.SetBtnCursor(IDC_APP_HAND);
	
	pWnd = (CWnd*)GetDlgItem(IDC_LIST_SHEET_NAME);
	m_wndSheetNameListCtrl.SetHeadings( _T("Sheet Name, 183") );
	m_wndSheetNameListCtrl.SetCheckboxes( TRUE );
	m_wndSheetNameListCtrl.SetHeaderImageList();
	
	m_selectAllButton.SubclassWindow(GetDlgItem(IDC_BUTTON_SELECT_ALL)->m_hWnd);
	m_selectAllButton.SetThemeHelper(&m_themeHelperST);
	m_selectAllButton.SetIcon(IDI_SELECT_ALL , 16 , 16);
	m_selectAllButton.SetBtnCursor(IDC_APP_HAND);
	m_unselectAllButton.SubclassWindow(GetDlgItem(IDC_BUTTON_UNSELECT_ALL)->m_hWnd);
	m_unselectAllButton.SetThemeHelper(&m_themeHelperST);
	m_unselectAllButton.SetIcon(IDI_UNSELECT_ALL , 16 , 16);
	m_unselectAllButton.SetBtnCursor(IDC_APP_HAND);
	
	static CHAR_T *pMap[] = { _T("A") ,_T("B") , _T("C") , _T("D") , _T("E") , _T("F") , _T("G") , _T("H") , _T("I") ,
		_T("J") , _T("K") , _T("L") ,_T("M") ,_T("N") , _T("O") , _T("P") , _T("Q") , _T("R") , _T("S") , _T("T") , _T("U") , _T("V") , _T("W") , _T("X") , _T("Y") , _T("Z")};
	int count = sizeof(pMap) / sizeof(char*); 
	for(int i=0;i < count;i++)
	{
		m_wndConvResultCol.InsertString(i , pMap[i]);
	}
	for(int i=0;i < count;++i)
	{
		m_wndConvResultCol.InsertString(i + count, CString(pMap[0]) + CString(pMap[i]));
	}

	
	CEdit* pEdit = (CEdit*)(GetDlgItem(IDC_EDIT_RowNum));
	if( pEdit )
	{
		pEdit->SetWindowText( _T("0") );
		pEdit->SetFont(&m_Font);
	}

	///m_convertButton.m_nFlatStyle = CBCGButton::BUTTONSTYLE_SEMIFLAT;
	///m_convertButton.SetImage (IDB_OK);
	///m_convertButton.SetTextHotColor (RGB (0, 0, 255));
	///m_convertButton.SetFont(&m_Font);
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/**
**/
void CLineListConvertDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	const int gap = 3;
	
	//CRect rectResize;
	//m_resizeWnd.GetWindowRect(&rectResize);
	//ScreenToClient(&rectResize);
	
	CWnd* pWnd = GetDlgItem(IDC_STATIC_1);
	if(pWnd)
	{
		CRect rectGroup , rect;
		pWnd->SetWindowPos(NULL , 5 , 16 , cx - 7 , cy - 16 - 5 , SWP_NOZORDER);
		pWnd->GetWindowRect(&rectGroup);
		ScreenToClient(&rectGroup);
		
		pWnd = GetDlgItem(IDC_STATIC_2);
		pWnd->SetWindowPos(NULL , rectGroup.left + 10 , rectGroup.top + 20 , 0 , 0 , SWP_NOSIZE);
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);
		
		CRect rectTop;
		pWnd = GetDlgItem(IDC_STATIC_ShowExcelName);
		pWnd->SetWindowPos(NULL , rect.right + 10 , rect.top , cx - rect.right - 20, rect.Height() , SWP_NOZORDER);
		pWnd->GetWindowRect(&rectTop);
		ScreenToClient(&rectTop);
		
		CRect rectThis;
		
		m_convertButton.GetWindowRect(&rectThis);
		ScreenToClient(&rectThis);
		m_convertButton.SetWindowPos(NULL , rectGroup.right - rectThis.Width() - 10 , rectGroup.bottom - rectThis.Height() - 10 , 0 , 0 , SWP_NOSIZE);
		m_convertButton.GetWindowRect(&rect);
		ScreenToClient(&rect);
		
		pWnd = GetDlgItem(IDC_STATIC_CONVERT);
		pWnd->GetWindowRect(&rectThis);
		ScreenToClient(&rectThis);
		pWnd->SetWindowPos(NULL , rectGroup.left + 10 , rect.top , 0 , 0 , SWP_NOSIZE);
		
		pWnd = GetDlgItem(IDC_EDIT_RowNum);
		pWnd->SetWindowPos(NULL , rect.left , rect.top - rectThis.Height() - 10, 0 , 0 , SWP_NOSIZE);
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);
		
		CRect rectBottom;
		pWnd = GetDlgItem(IDC_STATIC_ROW_NUMBER);
		pWnd->SetWindowPos(NULL , rectGroup.left + 10 , rect.top , 0 , 0 , SWP_NOSIZE);
		
		m_wndConvResultCol.SetWindowPos(NULL , rect.left , rect.top - rectThis.Height() - 10, 0 , 0 , SWP_NOSIZE);

		pWnd = GetDlgItem(IDC_STATIC_CONV_RESULT_COL);
		pWnd->GetWindowRect(&rectThis);
		ScreenToClient(&rectThis);
		pWnd->SetWindowPos(NULL , rectGroup.left + 10 , rect.top - rectThis.Height() - 10 , 0 , 0 , SWP_NOSIZE);


		pWnd->GetWindowRect(&rectBottom);
		ScreenToClient(&rectBottom);
		//////////////////////////////////////////////////////////////////////////
		pWnd = GetDlgItem(IDC_STATIC_3);
		pWnd->SetWindowPos(NULL , rectGroup.left + 10 , rectTop.bottom + 10 , 
			cx - rectGroup.left - 20 , rectBottom.top - rectTop.bottom - 20 , SWP_NOZORDER);
		pWnd->GetWindowRect(&rectGroup);
		ScreenToClient(&rectGroup);
		
		m_unselectAllButton.GetWindowRect(&rectThis);
		ScreenToClient(&rectThis);
		m_unselectAllButton.SetWindowPos(NULL , rectGroup.right - rectThis.Width() - 10, rectGroup.bottom - rectThis.Height() - 7 , 0 , 0 , SWP_NOSIZE);
		m_unselectAllButton.GetWindowRect(&rect);
		ScreenToClient(&rect);
		
		m_selectAllButton.GetWindowRect(&rectThis);
		ScreenToClient(&rectThis);
		m_selectAllButton.SetWindowPos(NULL , rect.left - rectThis.Width() - 2, rect.top , 0 , 0 , SWP_NOSIZE);
		
		//pWnd = GetDlgItem(IDC_BUTTON_ExcelName);
		m_loadSheetNameButton.GetWindowRect(&rectThis);
		ScreenToClient(&rectThis);
		m_loadSheetNameButton.SetWindowPos(NULL , rectGroup.right - rectThis.Width() - 10 , rectGroup.top + 20 , 0 , 0 , SWP_NOSIZE);
		
		pWnd = GetDlgItem(IDC_LIST_SHEET_NAME);
		pWnd->SetWindowPos(NULL , rectGroup.left + 10 , rectThis.bottom + 3 , 
			rectGroup.Width() - 20 , rectGroup.bottom - rectThis.bottom - 40 , SWP_NOZORDER);
	}
}

void CLineListConvertDlg::OnBUTTONSelectExcel()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlg(TRUE,_T(""),_T("*.xls"),OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,
		_T("Excel 문서(*.xls)||모든 문서(*.*)|*.*||"));
	if( IDOK == dlg.DoModal() )
	{
		S_ExPath = dlg.GetPathName();
		SetDlgItemText(IDC_STATIC_ShowExcelName, S_ExPath);
	}
}

/**	\brief	excel line list 파일을 idms database로 변환시킨다.


	\return	void	
*/
void CLineListConvertDlg::OnButtonLineListConvert() 
{
	BeginWaitCursor();

	if(0 != m_wndSheetNameListCtrl.GetItemCount())
	{
		CLineListToOraDocData&  docData  = CLineListToOraDocData::GetInstance();
		STRING_T rIniFilePath = docData.GetIniFilePath();
		
		CString CC,rValue,ExName,InputNum;

		const int checkNum = m_wndSheetNameListCtrl.GetCheckedItemCount();
		int nItemCount = m_wndSheetNameListCtrl.GetItemCount();
		int j=0;
		
		for(int i = 0;i < nItemCount;i++)
		{
			BOOL b = m_wndSheetNameListCtrl.GetCheck(i);
			if(b)
			{
				CString rItemText = m_wndSheetNameListCtrl.GetItemText(i,0);	
				CC.Format( _T("Sheet %d") ,j);
				WritePrivateProfileString( _T("Excel") , CC , rItemText , rIniFilePath.c_str());
				j++;
			}
		}
		
		rValue.Format( _T("%d") ,j);
		WritePrivateProfileString(CLineListConvertDlg::APP.c_str() , _T("Sheet Count") , rValue , rIniFilePath.c_str());
		
		const int nSelItem = m_wndConvResultCol.GetCurSel();
		CString rConvResultCol;
		m_wndConvResultCol.GetLBText(nSelItem , rConvResultCol);
		WritePrivateProfileString(CLineListConvertDlg::APP.c_str() , _T("Conv Result Col") , rConvResultCol , rIniFilePath.c_str());

		CEdit *edit = NULL;
		edit= (CEdit *)GetDlgItem(IDC_EDIT_RowNum);
		if( edit )
		{
			edit->GetWindowText( rValue );
			WritePrivateProfileString(CLineListConvertDlg::APP.c_str() , _T("Start Row") , rValue , rIniFilePath.c_str());
		}
		
		//////////////////////////////////////////////////////////////////////////
		if( !rValue.IsEmpty() )
		{
			CWorkStatusDlg dlg;
			dlg.m_pThread = AfxBeginThread(StatusThreadEntry, this, THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);

			if (NULL == dlg.m_pThread)
			{
				AfxMessageBox( _T("Can't create thread!!!") );
			}
			else
			{
				dlg.DoModal();
				switch(m_nConvertResCode)
				{
					case CConvertLineListToOra::CONFIGURATION_ERROR:
						AfxMessageBox(_T("환경 설정 에러입니다."));
					break;
					case CConvertLineListToOra::NOT_FOUND_PDTABLE:
						AfxMessageBox(_T("PDTABLE이름을 찾을 수 없습니다."));
					break;
					case CConvertLineListToOra::CONNECTION_FAILURE:
						AfxMessageBox(_T("Database에 접속할 수 없습니다."));
					break;
					case CConvertLineListToOra::CONVERT_OK:
						AfxMessageBox( _T("완료 되었습니다.") );
					break;
				}
				
				///::ShellExecute(NULL , "open" , "NOTEPAD.EXE" , "c:\\LineListToOra.log" , NULL , SW_SHOW);
				/// 엑셀 파일을 오픈한다.
				::ShellExecute(NULL , _T("open") , _T("EXCEL.EXE") , _T("\"") + m_rExcelFilePath + _T("\"") , NULL , SW_SHOW);
			}
		}
		else
		{
			AfxMessageBox( _T("Excel Convert의 시작 행을 입력하세요") );
		}
	}
	else
	{
		AfxMessageBox( _T("Excel Sheet를 선택해 주세요") );
	}

	EndWaitCursor();
}

void CLineListConvertDlg::OnButtonLoadSheetName() 
{
	if(!S_ExPath.IsEmpty())
	{
		try
		{	
			GetDlgItemText(IDC_STATIC_ShowExcelName , m_rExcelFilePath);
			m_loadSheetNameButton.EnableWindow(FALSE);
			
			BeginWaitCursor();
			
			CConvertLineListToOra ExGetname;
			CStringArray* pSheetNameArray = ExGetname.GetExcelName(CStringA(m_rExcelFilePath));
			
			m_wndSheetNameListCtrl.SetRedraw(FALSE);
			m_wndSheetNameListCtrl.DeleteAllItems();
			int nSize = pSheetNameArray->GetSize();
			for(int i = 0;i < nSize;i++)
			{
				m_wndSheetNameListCtrl.InsertItem(0 , pSheetNameArray->GetAt(i));
			}
			m_wndSheetNameListCtrl.SetRedraw(TRUE);
			
			EndWaitCursor();
			
			m_loadSheetNameButton.EnableWindow(TRUE);
		}
		catch(...)
		{
			m_loadSheetNameButton.EnableWindow(TRUE);
		}
	}
	else
	{
		AfxMessageBox( _T("Excel File을 선택하세요") );
	}
}

void CLineListConvertDlg::OnButtonSelectAll() 
{
	// TODO: Add your control notification handler code here
	const int nItemCount = m_wndSheetNameListCtrl.GetItemCount();
	for(int i = 0;i < nItemCount;i++)
	{
		m_wndSheetNameListCtrl.SetCheck(i , 1);
	}	
}

void CLineListConvertDlg::OnButtonUnselectAll() 
{
	// TODO: Add your control notification handler code here
	const int nItemCount = m_wndSheetNameListCtrl.GetItemCount();
	for(int i = 0;i < nItemCount;i++)
	{
		m_wndSheetNameListCtrl.SetCheck(i , 0);
	}
}

UINT CLineListConvertDlg::StatusThreadEntry(LPVOID pVoid)
{
	return ((CLineListConvertDlg*)pVoid)->StatusThread();
}

/** 
	@brief	쓰레드 실행부
**/
UINT CLineListConvertDlg::StatusThread()
{
	CWorkStatusDlg* pWorkStatusDlg = CWorkStatusDlg::GetInstance();
	try
	{
		CLineListToOraDocData& docData = CLineListToOraDocData::GetInstance();
		STRING_T rIniFilePath = docData.GetIniFilePath();

		InterlockedExchange((LONG*)(&(pWorkStatusDlg->m_bThreadRunning)) , TRUE);/// so the main thread knows that this thread is still running

		m_nConvertResCode = CConvertLineListToOra::CONVERT_OK;
		CConvertLineListToOra LineListToOraConverter;
		m_nConvertResCode = LineListToOraConverter.Convert(S_ExPath , rIniFilePath.c_str());
		
		InterlockedExchange((LONG*)(&(pWorkStatusDlg->m_bThreadRunning)) , FALSE);
		
		//////////////////////////////////////////////////////////////////////////
		/// close work status dialog
		if(pWorkStatusDlg) pWorkStatusDlg->SendMessage(WM_COMMAND , IDOK );

		return IDOK;
	}
	catch(...)
	{
		AfxMessageBox( _T("Unknown error")) ;

		InterlockedExchange((LONG*)(&(pWorkStatusDlg->m_bThreadRunning)) , FALSE);
		
		//////////////////////////////////////////////////////////////////////////
		/// close work status dialog
		pWorkStatusDlg->SendMessage(WM_COMMAND , IDOK );
	}
	
	return IDCANCEL;
}

/**	\brief	Base가 다이얼로그이기 때문에 ENTER나 ESC키를 누르면 다이얼로그가 종료된다.
	그래서 ENTER나 ESC가 전달되기 전에 막는다.

	\param	pMsg	a parameter of type MSG*

	\return	BOOL	
*/
BOOL CLineListConvertDlg::PreTranslateMessage(MSG* pMsg) 
{
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
**/
void CLineListConvertDlg::LoadData()
{
	static const STRING_T APP( _T("Excel") );

	CHAR_T szBuf[MAX_PATH + 1]={'\0' ,};

	CLineListToOraDocData& docData = CLineListToOraDocData::GetInstance();
	const STRING_T rIniFilePath = docData.GetIniFilePath();
	if(GetPrivateProfileString(APP.c_str() , _T("Start Row") , NULL , szBuf , MAX_PATH , rIniFilePath.c_str()))
	{
		m_wndStartRowEdit.SetWindowText(szBuf);
	}

	if(GetPrivateProfileString(CLineListConvertDlg::APP.c_str() , _T("Conv Result Col") , NULL , szBuf , MAX_PATH , rIniFilePath.c_str()))
	{
		int nItem = m_wndConvResultCol.FindString(0 , szBuf);
		if(-1 != nItem) m_wndConvResultCol.SetCurSel(nItem);
	}
}
