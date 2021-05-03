#if !defined(AFX_GETPDSLINEID_H__F042D327_192D_4F27_A743_2C77FA238208__INCLUDED_)
#define AFX_GETPDSLINEID_H__F042D327_192D_4F27_A743_2C77FA238208__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GetPdsLineID.h : header file
//
#include <GradientStatic.h>
/////////////////////////////////////////////////////////////////////////////
// CGetPdsLineID form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CGetPdsLineID : public CFormView
{
protected:
	DECLARE_DYNCREATE(CGetPdsLineID)
	CFont* m_pBoldFont;
// Form Data
public:
	//{{AFX_DATA(CGetPdsLineID)
	enum { IDD = IDD_DIALOG_GETPDS_LOG_FILE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CGradientStatic m_viewLogFileStatic;
	CScintillaWnd m_wndScintilla;
// Attributes
public:

// Operations
public:
	void ClearAllLog();
	void AddLog(const CString& rLog , const bool& bShow = true);
	CGetPdsLineID();
	virtual ~CGetPdsLineID();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGetPdsLineID)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	vector< string > m_LogTextEntry;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CGetPdsLineID)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	int CreateScintillaWnd();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GETPDSLINEID_H__F042D327_192D_4F27_A743_2C77FA238208__INCLUDED_)
