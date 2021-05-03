#if !defined(AFX_BACKUPHISTORYDLG_H__B346C9A6_05E1_475B_9454_2611B8E991B9__INCLUDED_)
#define AFX_BACKUPHISTORYDLG_H__B346C9A6_05E1_475B_9454_2611B8E991B9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BackupHistoryDlg.h : header file
//
#include <BtnST.h>
#include "FileListCtrl.h"
#include <ListCtrlEx.h>
#include "LabelStatic.h"

/////////////////////////////////////////////////////////////////////////////
// CBackupHistoryDlg dialog

class CBackupHistoryDlg : public CDialog
{
// Construction
public:
	void InitContents();
	CBackupHistoryDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBackupHistoryDlg)
	enum { IDD = IDD_BACKUP_HISTORY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CLabelStatic m_backupHistoryStatic;
	CFileListCtrl* m_pIssueFileList;
	CButtonST    m_showLogButton;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBackupHistoryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBackupHistoryDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonShowLog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BACKUPHISTORYDLG_H__B346C9A6_05E1_475B_9454_2611B8E991B9__INCLUDED_)
