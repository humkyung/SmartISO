// WorkStatusDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UserApp.h"
#include "WorkStatusDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern "C" __declspec(dllimport) void SetUpdateWorkStatusFunc(void (*f)(CString& , int&));
extern "C" __declspec(dllimport) void SetUserDLLUpdateWorkStatusFunc(void (*f)(CString& , int&));

static CWorkStatusDlg* m_pWorkStatusDlg = NULL;
/////////////////////////////////////////////////////////////////////////////
// CWorkStatusDlg dialog

CWorkStatusDlg::CWorkStatusDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWorkStatusDlg::IDD, pParent) , m_pThread(NULL) , m_process(0) , m_bThreadRunning(FALSE)
{
	//{{AFX_DATA_INIT(CWorkStatusDlg)
	m_pWorkStatusDlg = this;
	//}}AFX_DATA_INIT
}

CWorkStatusDlg::~CWorkStatusDlg()
{
	try
	{
		m_pWorkStatusDlg = NULL;
		if(m_pThread != NULL)
		{
			delete m_pThread;
		}
	}
	catch(...)
	{
		
	}
}

void CWorkStatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWorkStatusDlg)
	DDX_Control(pDX , IDC_PROGRESS , m_processCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWorkStatusDlg, CDialog)
	//{{AFX_MSG_MAP(CWorkStatusDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorkStatusDlg message handlers

BOOL CWorkStatusDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_processCtrl.SetRange(0 , 100);
	m_processCtrl.SetStep(1);
	m_processCtrl.SetPos(0);
	m_processCtrl.SetShowText(TRUE);
	
	if(NULL != m_pThread)
	{
		m_pThread->m_bAutoDelete = FALSE;
		m_pThread->ResumeThread();

		SetUserDLLUpdateWorkStatusFunc(UpdateWorkStatus);
		SetUpdateWorkStatusFunc(UpdateWorkStatus);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CWorkStatusDlg* CWorkStatusDlg::GetInstance()
{
	return m_pWorkStatusDlg;
}

void CWorkStatusDlg::UpdateWorkStatus(CString &rTitle, int &process)
{
	if((NULL != m_pWorkStatusDlg) && (::IsWindow(m_pWorkStatusDlg->m_hWnd)))
	{
		m_pWorkStatusDlg->m_processCtrl.SetWindowText(rTitle);
		m_pWorkStatusDlg->m_processCtrl.SetPos(process);
	}
}

void CWorkStatusDlg::OnOK() 
{
	// TODO: Add extra validation here
	if (m_bThreadRunning)
	{
		WaitForSingleObject(m_pThread->m_hThread, INFINITE);
		if(m_bThreadRunning)
		{
			// we gave the thread a chance to quit. Since the thread didn't
			// listen to us we have to kill it.
			TerminateThread(m_pThread->m_hThread, (DWORD)-1);
			InterlockedExchange((LONG*)(&m_bThreadRunning) , FALSE);
		}
	}

	CDialog::OnOK();
}

void CWorkStatusDlg::OnCancel() 
{
	// TODO: Add extra validation here
	if (m_bThreadRunning)
	{
		WaitForSingleObject(m_pThread->m_hThread, INFINITE);
		if(m_bThreadRunning)
		{
			// we gave the thread a chance to quit. Since the thread didn't
			// listen to us we have to kill it.
			TerminateThread(m_pThread->m_hThread, (DWORD)-1);
			InterlockedExchange((LONG*)(&m_bThreadRunning) , FALSE);
		}
	}

	CDialog::OnCancel();
}
