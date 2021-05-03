// BackupHistoryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UserApp.h"
#include "BackupHistoryDlg.h"
#include "UserAppDocData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBackupHistoryDlg dialog


CBackupHistoryDlg::CBackupHistoryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBackupHistoryDlg::IDD, pParent) , m_pIssueFileList(NULL)
{
	//{{AFX_DATA_INIT(CBackupHistoryDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CBackupHistoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBackupHistoryDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBackupHistoryDlg, CDialog)
	//{{AFX_MSG_MAP(CBackupHistoryDlg)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_SHOW_LOG, OnButtonShowLog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBackupHistoryDlg message handlers

BOOL CBackupHistoryDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_backupHistoryStatic.SubclassWindow(GetDlgItem(IDC_STATIC_BACKUP_HISTORY)->m_hWnd);
	m_backupHistoryStatic.SetBkColor(GetSysColor(COLOR_BTNFACE) , DarkenColor(GetSysColor(COLOR_BTNFACE),0.15) , CLabelStatic::VertGradient);
	
	m_pIssueFileList = new CFileListCtrl(this);
	if(m_pIssueFileList)
	{
		m_pIssueFileList->Create(LVS_REPORT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_EX_STATICEDGE | LVS_SHOWSELALWAYS | LVS_SHAREIMAGELISTS | LVS_EDITLABELS | LVS_OWNERDATA , 
			CRect(250 , 30 , 0 , 0) , this , 0x100);
		LONG style = GetWindowLong(m_pIssueFileList->m_hWnd , GWL_STYLE);
		style |= WS_EX_CLIENTEDGE;
		SetWindowLong(m_pIssueFileList->m_hWnd , GWL_EXSTYLE , style );
		m_pIssueFileList->SetDisplayFolder(true);
	}
	m_showLogButton.SubclassWindow(GetDlgItem(IDC_BUTTON_SHOW_LOG)->m_hWnd);
	m_showLogButton.SetIcon(IDI_SHOW_LOG , 16 , 16);
	m_showLogButton.SetGradientColor(DarkenColor(GetSysColor(COLOR_BTNFACE),0.15) , GetSysColor(COLOR_BTNFACE) , CButtonST::VertBar);
	m_showLogButton.SetBtnCursor(IDC_HAND_CUR);
	m_showLogButton.SetTooltipText(_T("Select ISSUE to restore"));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBackupHistoryDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	const int gap = 3;

	CRect rect;
	if(m_backupHistoryStatic.GetSafeHwnd())
	{
		m_backupHistoryStatic.GetWindowRect(&rect);
		ScreenToClient(&rect);
		m_backupHistoryStatic.SetWindowPos(NULL , 0 , 0  , cx , rect.Height() , SWP_NOZORDER);
	}
	
	if(m_pIssueFileList)
	{
		m_pIssueFileList->SetWindowPos(NULL , 0 , rect.bottom , cx , cy - rect.bottom - 35, SWP_NOZORDER);
	}

	if(m_showLogButton.GetSafeHwnd())
	{
		m_showLogButton.SetWindowPos(NULL , 0 + gap , cy - 32 , 100 , 30 , SWP_NOZORDER);
	}
}

void CBackupHistoryDlg::InitContents()
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	CString rProjectPath = pDocData->GetProject()->path();

	if(m_pIssueFileList)
	{
		m_pIssueFileList->SetFolder(rProjectPath + _T("\\ISSUE"));
	}
}

BOOL CBackupHistoryDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	NMHDR* hdr = (NMHDR*)(lParam);
	if( (wParam == WM_LBUTTONDBLCLK) && ( m_pIssueFileList->GetDlgCtrlID() == hdr->idFrom))
	{
		POSITION pos = m_pIssueFileList->GetFirstSelectedItemPosition();
		int nItem = m_pIssueFileList->GetNextSelectedItem(pos);
		if(nItem != 0 )
		{			
			CString rBackupPath = m_pIssueFileList->GetFolder();
			CString rFileName = m_pIssueFileList->GetItemText(nItem,0);
			CString rFullPath = rBackupPath+rFileName;
			::ShellExecute(NULL,"open","MSaccess.exe",rFullPath,NULL,SW_SHOW);
		}
	}
	return CDialog::OnNotify(wParam, lParam, pResult);
}

/**
	@brief	

	@author	HumKyung

	@date	????.??.??

	@param

	@return
*/
void CBackupHistoryDlg::OnButtonShowLog() 
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	CString rDBPath = pDocData->GetProject()->GetDatabaseFilePath();

	POSITION pos = m_pIssueFileList->GetFirstSelectedItemPosition();
	if(NULL != pos)
	{
		int nItem = m_pIssueFileList->GetNextSelectedItem(pos);
		if(nItem != 0 )
		{
			const int ret = AfxMessageBox( _T("예전 DB로 복구 하시겠습니까?") ,MB_YESNO);
			if(ret == IDYES)
			{
				CString rBackupPath = m_pIssueFileList->GetFolder();
				CString rFileName = m_pIssueFileList->GetItemText(nItem,0);
				CString rFullPath = rBackupPath + rFileName + _T("\\SET\\") + CString(pDocData->GetProject()->name()) + _T(".MDB");
					
				CopyFile(rFullPath,rDBPath,FALSE);
				
				AfxMessageBox( _T("DB복구 완료") );
			}
		}
	}
}