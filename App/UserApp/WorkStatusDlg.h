#if !defined(AFX_WORKSTATUSDLG_H__43787284_E6EA_498F_81C3_19B02BFD8DAA__INCLUDED_)
#define AFX_WORKSTATUSDLG_H__43787284_E6EA_498F_81C3_19B02BFD8DAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WorkStatusDlg.h : header file
//
#include <BtnST.h>
#include <TextProgressCtrl.h>
/////////////////////////////////////////////////////////////////////////////
// CWorkStatusDlg dialog

class CWorkStatusDlg : public CDialog
{
// Construction
public:
	static void UpdateWorkStatus(CString& rTitle , int& process);
	static CWorkStatusDlg* GetInstance();

	CWorkStatusDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CWorkStatusDlg();
// Dialog Data
	//{{AFX_DATA(CWorkStatusDlg)
	enum { IDD = IDD_WORK_STATUS };
	int m_process;
	//}}AFX_DATA

	static CString m_rTitle;
	CTextProgressCtrl m_processCtrl;

	CWinThread* m_pThread;
	volatile LONG m_bThreadRunning;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorkStatusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CWorkStatusDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WORKSTATUSDLG_H__43787284_E6EA_498F_81C3_19B02BFD8DAA__INCLUDED_)
