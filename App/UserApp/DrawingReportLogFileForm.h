#if !defined(AFX_DRAWINGREPORTLOGFILEFORM_H__BEBDD114_147D_48B5_9DB6_D6F9263BB3D8__INCLUDED_)
#define AFX_DRAWINGREPORTLOGFILEFORM_H__BEBDD114_147D_48B5_9DB6_D6F9263BB3D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DrawingReportLogFileForm.h : header file
//
#include <GradientStatic.h>

/////////////////////////////////////////////////////////////////////////////
// CDrawingReportLogFileForm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CDrawingReportLogFileForm : public CFormView
{
protected:
	
	DECLARE_DYNCREATE(CDrawingReportLogFileForm)

// Form Data
public:
	//{{AFX_DATA(CDrawingReportLogFileForm)
	enum { IDD = IDD_DRAWING_REPORT_LOG_FILE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
// Attributes
public:

// Operations
public:
	void AddLog(const CString& rLog);
	CDrawingReportLogFileForm();           // protected constructor used by dynamic creation
	virtual ~CDrawingReportLogFileForm();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawingReportLogFileForm)
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
	//{{AFX_MSG(CDrawingReportLogFileForm)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWINGREPORTLOGFILEFORM_H__BEBDD114_147D_48B5_9DB6_D6F9263BB3D8__INCLUDED_)
