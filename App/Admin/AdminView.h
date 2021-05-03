// AdminView.h : interface of the CAdminView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADMINVIEW_H__6869CA48_8997_4CFB_BF68_07170949C185__INCLUDED_)
#define AFX_ADMINVIEW_H__6869CA48_8997_4CFB_BF68_07170949C185__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AdminDoc.h"
#include "ProjectDefTableDlg.h"
#include "ProjectSettingForm.h"

class CAdminView : public CFormView
{
protected: // create from serialization only
	CAdminView();
	DECLARE_DYNCREATE(CAdminView)

public:
	//{{AFX_DATA(CAdminView)
	enum{ IDD = IDD_ADMIN_FORM };
		// NOTE: the ClassWizard will add data members here
	CProjectDefTableDlg* m_pProjectDefTableDlg;
	CProjectSettingForm* m_pProjectSettingForm;
	//}}AFX_DATA

// Attributes
public:
	CAdminDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdminView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	void UpdateContents();
	void ShowSettingForm(const bool& show);
	void ShowDefTableForm(const bool& show);
	virtual ~CAdminView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CAdminView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in AdminView.cpp
inline CAdminDoc* CAdminView::GetDocument()
   { return (CAdminDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADMINVIEW_H__6869CA48_8997_4CFB_BF68_07170949C185__INCLUDED_)
