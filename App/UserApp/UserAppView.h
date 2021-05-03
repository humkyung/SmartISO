// UserAppView.h : interface of the CUserAppView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERAPPVIEW_H__ED5DFFB5_E4FE_4221_A429_57D1D24B8C7F__INCLUDED_)
#define AFX_USERAPPVIEW_H__ED5DFFB5_E4FE_4221_A429_57D1D24B8C7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UserAppDoc.h"
#include "BackupDataForm.h"
#include "EditLineDataForm.h"
#include "InsertToPDSAttribute.h"
#include "IsoEditForm.h"
#include "IsoEditFormHolder.h"
#include "DrawingReportForm.h"
#include "BMReportForm.h"
#include "DrawingPlottingForm.h"

class CUserAppView : public CFormView
{
protected: // create from serialization only
	CUserAppView();
	DECLARE_DYNCREATE(CUserAppView)

public:
	//{{AFX_DATA(CUserAppView)
	enum{ IDD = IDD_USERAPP_FORM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
// Attributes
public:
	//CUser* m_pSelUser;
	CUserAppDoc* GetDocument();

	CBackupDataForm* m_pBackupDataForm;
	CEditLineDataForm* m_pLineDataForm;
	CInsertToPDSAttribute* m_pInsertToPDS;
	//CIsoEditForm* m_pIsoEditForm;
	CIsoEditFormHolder* m_pIsoEditFormHolder;	/// 2013.02.08 added by humkyung
	CDrawingReportForm* m_pDrawingReportForm;
	CBMReportForm* m_pBMReportForm;
	CDrawingPlottingForm* m_pDrawingPlottingForm;
// Operations
public:
	static CUserAppView* GetInstance();

	void ShowBMReportForm(const bool &show);
	void ShowIndexReportForm(const bool &show);
	void ShowBackupDataForm(const bool &show);
	void ShowLineDataForm(const bool &show);
	void ShowIsoEditForm(const bool &show);
	void ShowUpdatePDSForm(const bool &show);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserAppView)
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
	void ShowDrawingPlottingForm(const bool &show);
	void InitContents();
	virtual ~CUserAppView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CUserAppView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in UserAppView.cpp
inline CUserAppDoc* CUserAppView::GetDocument()
   { return (CUserAppDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERAPPVIEW_H__ED5DFFB5_E4FE_4221_A429_57D1D24B8C7F__INCLUDED_)
