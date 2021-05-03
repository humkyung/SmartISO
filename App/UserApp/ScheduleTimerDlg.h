#if !defined(AFX_SCHEDULETIMERDLG_H__5CC864B4_6B5A_480A_AD00_594E2DAA80C1__INCLUDED_)
#define AFX_SCHEDULETIMERDLG_H__5CC864B4_6B5A_480A_AD00_594E2DAA80C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScheduleTimerDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScheduleTimerDlg dialog

class CScheduleTimerDlg : public CDialog
{
// Construction
public:
	CScheduleTimerDlg(CWnd* pParent = NULL);   // standard constructor
	CString m_rTimer;
// Dialog Data
	//{{AFX_DATA(CScheduleTimerDlg)
	enum { IDD = IDD_TiMER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScheduleTimerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScheduleTimerDlg)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCHEDULETIMERDLG_H__5CC864B4_6B5A_480A_AD00_594E2DAA80C1__INCLUDED_)
