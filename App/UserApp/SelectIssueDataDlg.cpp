// IsoEditOptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UserApp.h"
#include "UserAppDocData.h"
#include "SelectIssueDataDlg.h"
#include "tokenizer.h"
#include <util/FileTools.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectIssueDataDlg dialog


CSelectIssueDataDlg::CSelectIssueDataDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectIssueDataDlg::IDD, pParent) , m_pBoldFont(NULL) , m_pUserAppDoc(NULL) , m_pTargetWnd(NULL)
{
	m_bSelectCurrent = TRUE;
	m_bSelectIssued  = FALSE;
}

CSelectIssueDataDlg::~CSelectIssueDataDlg()
{
	try
	{
		delete m_pBoldFont;
		m_pBoldFont = NULL;
	}
	catch(...)
	{

	}
}

void CSelectIssueDataDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectIssueDataDlg)
	DDX_Check(pDX , IDC_RADIO_CURRENT , m_bSelectCurrent);
	DDX_Check(pDX , IDC_RADIO_ISSUED  , m_bSelectIssued);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_TitleStaticCtrl);
	DDX_Control(pDX , IDC_LIST_ISSUE_DATE , m_wndIssueDateReport);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectIssueDataDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectIssueDataDlg)
	ON_BN_CLICKED(IDC_RADIO_CURRENT, OnRadioCurrent)
	ON_BN_CLICKED(IDC_RADIO_ISSUED, OnRadioIssued)
	ON_BN_CLICKED(IDC_BUTTON_SELECT , OnButtonSelect)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectIssueDataDlg message handlers

BOOL CSelectIssueDataDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ImageList.Create(IDB_CHECKBOXES, 16, 3, RGB(255,0,255));

	m_pBoldFont = new CFont;
	m_pBoldFont->CreateFont(20,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0, _T("Arial"));
	m_TitleStaticCtrl.SetFont(m_pBoldFont);

	SetDlgItemText(IDC_STATIC_OPTION , _TR("Issue Date"));
	if(m_wndIssueDateReport.GetSafeHwnd())
	{
		m_wndIssueDateReport.SetCheckboxes();

		LONG style = GetWindowLong(m_wndIssueDateReport.m_hWnd , GWL_STYLE);
		style |= WS_EX_CLIENTEDGE;
		SetWindowLong(m_wndIssueDateReport.m_hWnd , GWL_EXSTYLE , style );
		m_wndIssueDateReport.SetImageList(&m_ImageList , LVSIL_SMALL);
		m_wndIssueDateReport.SetHeaderImageList(&m_ImageList);
		m_wndIssueDateReport.SetHeadings( _T("[],30;") + _TR("Issue Date") + _T(",150") );
		m_wndIssueDateReport.SetGridLines();
		m_wndIssueDateReport.SetExtendedStyle(LVS_EX_CHECKBOXES | m_wndIssueDateReport.GetExtendedStyle());
		m_wndIssueDateReport.EnableHeaderCheckBox(TRUE);
	}
	if(!m_bSelectIssued) m_wndIssueDateReport.EnableWindow(FALSE);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/**	
	@brief	
	@author	HumKyung
	@date	2011.01.28
	@param
	@return
**/
void CSelectIssueDataDlg::OnButtonSelect() 
{
	m_sSelectedFolder.Empty();
	if(NULL != m_pTargetWnd)
	{
		CAppDocData* pDocData = CAppDocData::GetInstance();

		WPARAM wParam = SELECT;
		if(m_bSelectCurrent)
		{
			m_sSelectedFolder = pDocData->GetProjectPath().c_str() + CString(_T("\\PRODUCT\\DGN"));
		}
		else
		{
			m_sSelectedFolder.Empty();
			wParam = SELECT_ISSUED;
		}

		NMHDR hdr;
		hdr.hwndFrom = GetSafeHwnd();
		hdr.idFrom   = GetDlgCtrlID();
		m_pTargetWnd->SendMessage(WM_NOTIFY , wParam , (LPARAM)&hdr);
	}
}

/**	
	@brief	CURRENT 라디오 버튼을 클릭했을 때...
**/
void CSelectIssueDataDlg::OnRadioCurrent() 
{
	m_bSelectCurrent = TRUE;
	m_bSelectIssued = FALSE;
	
	CWnd* pWnd = GetDlgItem(IDC_LIST_ISSUE_DATE);
	if(pWnd) pWnd->EnableWindow(FALSE);
}

/**	
	@brief	ISSUED 라디오 버튼을 클릭했을 때...
**/
void CSelectIssueDataDlg::OnRadioIssued() 
{
	m_bSelectCurrent = FALSE;
	m_bSelectIssued  = TRUE;

	CWnd* pWnd = GetDlgItem(IDC_LIST_ISSUE_DATE);
	if(pWnd) pWnd->EnableWindow(TRUE);
	/*
	SaveData( m_rIniFilePath );

	if(NULL != m_pTargetWnd)
	{
		NMHDR hdr;
		hdr.hwndFrom = GetSafeHwnd();
		hdr.idFrom = GetDlgCtrlID();
		m_pTargetWnd->SendMessage(WM_NOTIFY , (WPARAM)SELECT_EDITED_ISO_DWG_FOLDER , (LPARAM)&hdr);
	}
	*/
}

/**	
	@brief	데이터를 환경 파일로 저장한다.
	@param	rIniFilePath	a parameter of type const CString &
	@return	void	
*/
void CSelectIssueDataDlg::SaveData(const CString &rIniFilePath)
{	
	/*
	CString rValue = ( TRUE == m_bSelectSourceIsoDwgFileFolder ) ? "Source" : "Edited";
	WritePrivateProfileString( CSelectIssueDataDlg::APP , "Select From" , rValue , rIniFilePath );
	*/
}

/**	
	@brief	환경 설정 파일에서 데이터를 읽는다.

	@param	rIniFilePath	a parameter of type const CString &

	@return	void	
*/
void CSelectIssueDataDlg::LoadData(const CString &rIniFilePath)
{
	/*
	TCHAR szBuf[MAX_PATH + 1] = {'\0' ,};
	if( GetPrivateProfileString( CSelectIssueDataDlg::APP , "Select From" , NULL , szBuf , MAX_PATH , rIniFilePath ))
	{
		if( CString(szBuf) == "Source" )
		{
			m_bSelectSourceIsoDwgFileFolder = TRUE;
			m_bSelectEditedIsoDwgFileFolder = FALSE;
		}
		else
		{
			m_bSelectSourceIsoDwgFileFolder = FALSE;
			m_bSelectEditedIsoDwgFileFolder = TRUE;
		}
	}
	UpdateData( FALSE );
	*/
}

/**
	@brief
	@author	HumKyung
	@date	2011.01.28
	@return
*/
int CSelectIssueDataDlg::InitContents( CWnd* pTargetWnd )
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	const CProject::DatabaseProp prop = pDocData->GetProject()->GetDatabaseProp();

	m_pTargetWnd = pTargetWnd;

	m_wndIssueDateReport.DeleteAllItems();

	int iCount = 0;
	try
	{
		CAppPostgreSQL database(pDocData->GetProject()->name(),prop.Host.c_str(),prop.User.c_str(),prop.Password.c_str());
		const STRING_T sSql = _T("SELECT DISTINCT ISSUE_DATE FROM T_ISSUED_ISO_LINE_DATA");
		soci::rowset<soci::row> rs(database.session()->prepare << sSql);
		for(soci::rowset<soci::row>::const_iterator itr = rs.begin();itr != rs.end();++itr)
		{
			STRING_T sValue;
			if(soci::i_ok == itr->get_indicator(_T("issue_date")))
			{
				sValue = itr->get<STRING_T>(_T("issue_date"));
				const int at = m_wndIssueDateReport.InsertItem(m_wndIssueDateReport.GetItemCount() , _T("") , 0);
				m_wndIssueDateReport.SetItemText(at , 1 , sValue.c_str());
			}
		}
	}
	catch(const std::exception& ex)
	{
		AfxMessageBox(ex.what(),MB_OK|MB_ICONERROR);
	}
	
	return ERROR_SUCCESS;
}

/**
	@brief	get selected issue date
	@author	humkyung
	@date	2015.09.23
*/
void CSelectIssueDataDlg::GetSelectedIssueDate(vector<STRING_T>& aIssueDateEntry)
{
	aIssueDateEntry.clear();
	CString rString;
	const int nItemCount = m_wndIssueDateReport.GetItemCount();
	for( int i = 0;i < nItemCount;++i)
	{
		if( m_wndIssueDateReport.GetCheck( i ) )
		{
			rString = m_wndIssueDateReport.GetItemText(i , 0);
			aIssueDateEntry.push_back( rString.operator LPCTSTR() );
		}
	}
}

/**
	@brief
	@author	HumKyung
	@date	2011.01.28
	@param	nType
	@param	cx
	@param	cy
	@return
*/
void CSelectIssueDataDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	const int gap = 5;

	CSize szSize;
	CWnd* pWnd = GetDlgItem(IDC_BUTTON_SELECT);
	if(pWnd)
	{
		CRect rect;
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);
		szSize.cx = rect.Width();
		szSize.cy = rect.Height();

		pWnd->SetWindowPos(NULL , cx - rect.Width() - gap , cy - rect.Height() , rect.Width() , rect.Height() , SWP_NOZORDER);
	}

	pWnd = GetDlgItem(IDC_STATIC_OPTION);
	if(pWnd)
	{
		CRect rect;
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);
		
		pWnd->SetWindowPos(NULL , 0 , 0 , cx - (rect.left + gap) , cy - rect.top - szSize.cy - 5 , SWP_NOZORDER | SWP_NOMOVE);
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);
		szSize.cx = rect.Width();
		szSize.cy = rect.Height();
	}

	pWnd = GetDlgItem(IDC_LIST_ISSUE_DATE);
	if(pWnd)
	{
		CRect rect;
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);
		
		pWnd->SetWindowPos(NULL , 0 , 0 , szSize.cx - gap*3 , szSize.cy - 30 , SWP_NOZORDER | SWP_NOMOVE);
	}

	pWnd = GetDlgItem(IDC_STATIC_SELECT_FROM);
	if(pWnd)
	{
		CRect rect;
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);

		pWnd->SetWindowPos(NULL , 0 , 0 , cx - (rect.left + gap) , rect.Height() , SWP_NOZORDER | SWP_NOMOVE);
	}

	pWnd = GetDlgItem(IDC_STATIC_TITLE);
	if(pWnd)
	{
		CRect rect;
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);

		pWnd->SetWindowPos(NULL , 0 , 0 , cx - (rect.left + gap) , rect.Height() , SWP_NOZORDER | SWP_NOMOVE);
	}
}