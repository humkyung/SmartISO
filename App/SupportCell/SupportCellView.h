// SupportCellView.h : interface of the CSupportCellView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUPPORTCELLVIEW_H__3B922E39_8DC1_4448_AD7D_1C4334350FD2__INCLUDED_)
#define AFX_SUPPORTCELLVIEW_H__3B922E39_8DC1_4448_AD7D_1C4334350FD2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <ListCtrlEx.h>

class CSupportCellView : public CFormView
{
protected: // create from serialization only
	CSupportCellView();
	DECLARE_DYNCREATE(CSupportCellView)

public:
	//{{AFX_DATA(CSupportCellView)
	enum{ IDD = IDD_SUPPORTCELL_FORM };
		// NOTE: the ClassWizard will add data members here
	IsGui::CListCtrlEx m_cellInfoListCtrl;
	//}}AFX_DATA

// Attributes
public:
	CSupportCellDoc* GetDocument();
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSupportCellView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSupportCellView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSupportCellView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in SupportCellView.cpp
inline CSupportCellDoc* CSupportCellView::GetDocument()
   { return (CSupportCellDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUPPORTCELLVIEW_H__3B922E39_8DC1_4448_AD7D_1C4334350FD2__INCLUDED_)
