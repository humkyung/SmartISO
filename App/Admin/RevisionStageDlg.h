#if !defined(AFX_REVISIONSTAGEDLG_H__B13947D4_2E20_41B3_82DD_A0D8778B37F0__INCLUDED_)
#define AFX_REVISIONSTAGEDLG_H__B13947D4_2E20_41B3_82DD_A0D8778B37F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RevisionStageDlg.h : header file
//
#include "AdminDoc.h"

#include <ListCtrlEx.h>
#include <GroupCheck.h>
#include "GridCtrl/GridCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CRevisionStageDlg dialog

class CRevisionStageDlg : public CDialog
{
// Construction
public:
	int SaveData();
	int UpdateContents();
	CRevisionStageDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRevisionStageDlg)
	enum { IDD = IDD_REVISION_STAGE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CGridCtrl  m_wndRevNoDefineGridCtrl , m_wndRevUserGridCtrl;
	CMFCButton m_btnAddGrade , m_btnDelGrade; 
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRevisionStageDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CRevisionStageDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonGradeDel();
	afx_msg void OnButtonGradeAdd();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
private:
	CAdminDoc* GetDocument();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REVISIONSTAGEDLG_H__B13947D4_2E20_41B3_82DD_A0D8778B37F0__INCLUDED_)
