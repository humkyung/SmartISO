#if !defined(AFX_BMLOGFILEFORM_H__E74A43F8_303C_498F_A1CD_3FAE949BC0BE__INCLUDED_)
#define AFX_BMLOGFILEFORM_H__E74A43F8_303C_498F_A1CD_3FAE949BC0BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BMLogFileForm.h : header file
//
#include <GradientStatic.h>

/////////////////////////////////////////////////////////////////////////////
// CBMLogFileForm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CBMLogFileForm : public CFormView
{
protected:
	
	DECLARE_DYNCREATE(CBMLogFileForm)

// Form Data
public:
	//{{AFX_DATA(CBMLogFileForm)
	enum { IDD = IDD_DIALOG_BM_LOG_FILE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CMFCTabCtrl m_wndTabCtrl;
	CMFCListCtrl m_wndBMDataListCtrl;
	CScintillaWnd m_wndScintilla;
// Attributes
public:

// Operations
public:
	int DisplayBOMData(const CString& sKey,const CString& sRevNo);	/// 2012.08.31 added by humkyung
	bool Save(const CString& rFilePath);
	void ClearAllLog();
	void AddLog(const CString& rLog);
	CBMLogFileForm();           // protected constructor used by dynamic creation
	virtual ~CBMLogFileForm();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBMLogFileForm)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	vector<CString> m_LogEntry;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CBMLogFileForm)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	int CreateScintillaWnd();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BMLOGFILEFORM_H__E74A43F8_303C_498F_A1CD_3FAE949BC0BE__INCLUDED_)
