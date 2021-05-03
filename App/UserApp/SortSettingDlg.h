#if !defined(AFX_SORTSETTINGDLG_H__534486DA_C376_4502_A4DB_5A03BC467891__INCLUDED_)
#define AFX_SORTSETTINGDLG_H__534486DA_C376_4502_A4DB_5A03BC467891__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SortSettingDlg.h : header file
//
#include "StdAfx.h"
#include "resource.h"
#include "UserAppDocData.h"
#include <FilterEdit/FilterEdit.h>
#include <BtnST.h>
#include <ThemeHelperST.h>
#include "FileNameEditListBox1.h"

/////////////////////////////////////////////////////////////////////////////
// CSortSettingDlg dialog

class CSortSettingDlg : public CDialog
{
// Construction
public:
	CSortSettingDlg(CString sTitle , CWnd* pParent = NULL);   // standard constructor
	virtual ~CSortSettingDlg();
// Dialog Data
	//{{AFX_DATA(CSortSettingDlg)
	enum { IDD = IDD_DIALOG_SORTSETTING };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	
	EditListBox::CEditListBoxBuddy	m_EditListBoxBuddy;
	CMFCButton m_btnOK , m_btnCancel;
	CFileNameEditListBox m_FileNameEditListBox;
	vector<CString> m_FieldDescEntry;
	CString m_rFileNameFormat;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSortSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString m_sTitle;	/// 2012.07.30 added by humkyung
	CFont* m_pFont;
	CThemeHelperST m_themeHelperST;
	// Generated message map functions
	//{{AFX_MSG(CFileNameFormatDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SORTSETTINGDLG_H__534486DA_C376_4502_A4DB_5A03BC467891__INCLUDED_)
