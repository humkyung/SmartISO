#if !defined(AFX_VIEWLOGFILEFORM_H__355FE560_15F3_4688_8761_5EC7B2283549__INCLUDED_)
#define AFX_VIEWLOGFILEFORM_H__355FE560_15F3_4688_8761_5EC7B2283549__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewLogFileForm.h : header file
//
#include <GradientStatic.h>
/////////////////////////////////////////////////////////////////////////////
// CViewLogFileForm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#define __MEM_CRY_EDIT_VER__	0

class CViewLogFileForm : public CFormView
{
protected:
	
	DECLARE_DYNCREATE(CViewLogFileForm)

// Form Data
public:
	//{{AFX_DATA(CViewLogFileForm)
	enum { IDD = IDD_DIALOG_VIEW_LOG_FILE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
// Attributes
public:

// Operations
public:
	CViewLogFileForm();           // protected constructor used by dynamic creation
	virtual ~CViewLogFileForm();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewLogFileForm)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewLogFileForm)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWLOGFILEFORM_H__355FE560_15F3_4688_8761_5EC7B2283549__INCLUDED_)
