#if !defined(AFX_FILENAMEFORMATDLG_H__8F61CDD2_03F1_4E63_AD98_49E34249A4F7__INCLUDED_)
#define AFX_FILENAMEFORMATDLG_H__8F61CDD2_03F1_4E63_AD98_49E34249A4F7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileNameFormatDlg.h : header file
//
#include "AdminDoc.h"
#include <FilterEdit/FilterEdit.h>
#include <BtnST.h>
#include <XButtonXP/XButtonXP.h>
#include <ThemeHelperST.h>
#include "FileNameEditListBox.h"
/////////////////////////////////////////////////////////////////////////////
// CFileNameFormatDlg dialog

class CFileNameFormatDlg : public CDialog
{
// Construction
public:
	CFileNameFormatDlg(CWnd* pParent = NULL , const CString& sFieldDescEntry = _T(""));   // standard constructor
	virtual ~CFileNameFormatDlg();
// Dialog Data
	//{{AFX_DATA(CFileNameFormatDlg)
	enum { IDD = IDD_FILE_NAME_FORMAT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CFilterEdit m_DelimiterEdit;
	CButtonST m_AddButton;
	CFileNameEditListBox	m_FileNameEditListBox;
	EditListBox::CEditListBoxBuddy	m_EditListBoxBuddy;
	CXButtonXP m_okButton;
	CXButtonXP m_cancelButton;
	CToolTipCtrl m_ctrlTT;

	vector<CString> m_FieldDescEntry;
	CAdminDoc* m_pAdminDoc;

	CString m_rFileNameFormat;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileNameFormatDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CFont* m_pFont;
	CThemeHelperST m_themeHelperST;
	// Generated message map functions
	//{{AFX_MSG(CFileNameFormatDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAdd();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILENAMEFORMATDLG_H__8F61CDD2_03F1_4E63_AD98_49E34249A4F7__INCLUDED_)
