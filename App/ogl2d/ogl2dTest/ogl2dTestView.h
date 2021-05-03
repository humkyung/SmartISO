// ogl2dTestView.h : interface of the COgl2dTestView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGL2DTESTVIEW_H__1336E922_BF61_472C_AE2C_859F2C910351__INCLUDED_)
#define AFX_OGL2DTESTVIEW_H__1336E922_BF61_472C_AE2C_859F2C910351__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../GLCanvas.h"

class COgl2dTestView : public CView
{
protected: // create from serialization only
	COgl2dTestView();
	DECLARE_DYNCREATE(COgl2dTestView)

// Attributes
public:
	COgl2dTestDoc* GetDocument();
	CGLCanvas m_glCanvas;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COgl2dTestView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COgl2dTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(COgl2dTestView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ogl2dTestView.cpp
inline COgl2dTestDoc* COgl2dTestView::GetDocument()
   { return (COgl2dTestDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OGL2DTESTVIEW_H__1336E922_BF61_472C_AE2C_859F2C910351__INCLUDED_)
