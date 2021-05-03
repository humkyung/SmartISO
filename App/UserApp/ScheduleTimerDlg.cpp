// ScheduleTimerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UserApp.h"
#include "tokenizer.h"
#include "ScheduleTimerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScheduleTimerDlg dialog


CScheduleTimerDlg::CScheduleTimerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CScheduleTimerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScheduleTimerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CScheduleTimerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScheduleTimerDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScheduleTimerDlg, CDialog)
	//{{AFX_MSG_MAP(CScheduleTimerDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScheduleTimerDlg message handlers

void CScheduleTimerDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CScheduleTimerDlg::OnOK() 
{
	CString rString;
	GetDlgItemText(IDC_EDIT_YEAR , rString);
	m_rTimer = rString;
	GetDlgItemText(IDC_EDIT_MONTH , rString);
	m_rTimer += "-" + rString;
	GetDlgItemText(IDC_EDIT_DATE , rString);
	m_rTimer += "-" + rString;
	GetDlgItemText(IDC_EDIT_HOUR , rString);
	m_rTimer += "-" + rString;
	GetDlgItemText(IDC_EDIT_MINUTE , rString);
	m_rTimer += "-" + rString;

	CDialog::OnOK();
}

BOOL CScheduleTimerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CTime t = CTime::GetCurrentTime();
	CString rCurrentDate = t.Format( _T("%Y-%m-%d-%H-%M") );
	
	vector<string> oResult;
	CTokenizer<CIsFromString>::Tokenize(oResult , rCurrentDate.operator LPCSTR() , CIsFromString("-"));
	if(5 == oResult.size())
	{
		SetDlgItemText(IDC_EDIT_YEAR, oResult[0].c_str());
		SetDlgItemText(IDC_EDIT_MONTH, oResult[1].c_str());
		SetDlgItemText(IDC_EDIT_DATE, oResult[2].c_str());
		SetDlgItemText(IDC_EDIT_HOUR, oResult[3].c_str());
		SetDlgItemText(IDC_EDIT_MINUTE, oResult[4].c_str());
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
