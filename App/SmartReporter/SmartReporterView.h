
// SmartReporterView.h : interface of the CSmartReporterView class
//


#pragma once

#include "SmartReporterDoc.h"

#include "LineListToOraDlg.h"
#include "LineListConvertDlg.h"
#include "OraDataReportView.h"

class CSmartReporterView : public CView
{
protected: // create from serialization only
	CSmartReporterView();
	DECLARE_DYNCREATE(CSmartReporterView)

// Attributes
	CImageList m_imgList;

	CString m_rServiceName;
	CString m_rUserName;
	CString m_rUserPassword;
	CString m_rProjectName;	
	CString m_rStdNoteFilePath;
public:
	//{{AFX_DATA(CLineListToOraView)
	enum{ IDD = IDD_LINELISTTOORA_FORM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CMFCTabCtrl m_wndTab;
	CButtonST   m_wndSelectAllAreaNoButton;
	CListCtrlEx m_wndAreaNoReport;
	CButtonST   m_wndSelectAllModelNoButton;
	CListCtrlEx m_wndModelNoReport;

	CSmartReporterDoc* GetDocument() const;

// Operations
public:
	void LoadData();
	int SyncTabCtrlWithCustomReportTableList();
	int CreateCustomDataReportDialog(CCustomReportTable* pCustomReportTable);
// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CSmartReporterView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	///CLineListToOraDlg	m_wndLineListToOraDlg;
	///CLineListConvertDlg	m_wndLineListConvertDlg;
	///COraDataReportView*	m_pWndOraDataReportView;
protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};

#ifndef _DEBUG  // debug version in SmartReporterView.cpp
inline CSmartReporterDoc* CSmartReporterView::GetDocument() const
   { return reinterpret_cast<CSmartReporterDoc*>(m_pDocument); }
#endif

