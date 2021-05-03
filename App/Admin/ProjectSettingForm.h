#if !defined(AFX_PROJECTSETTINGFORM_H__796C1AB9_E41C_4C2A_986B_EB8FF11A0DC2__INCLUDED_)
#define AFX_PROJECTSETTINGFORM_H__796C1AB9_E41C_4C2A_986B_EB8FF11A0DC2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProjectSettingForm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProjectSettingForm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include <BtnST.h>

#include "BorderSettingDlg.h"
#include "LineDataSettingDlg.h"
#include "IsoEditSettingDlg.h"
#include "DrawingReportSettingDlg.h"
#include "OracleSettingDlg.h"
#include "RefDataSettingDlg.h"

#include "SmartISODiffSettingDlg.h"	/// 2012.01.03 added by humkyung

class CProjectSettingForm : public CFormView
{
protected:
	
	DECLARE_DYNCREATE(CProjectSettingForm)

// Form Data
public:
	//{{AFX_DATA(CProjectSettingForm)
	enum { IDD = IDD_DIALOG_PROJECT_SETTING };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CLineDataSettingDlg m_LineDataSettingDlg;
	CBorderSettingDlg   m_BorderSettingDlg;
	CIsoEditSettingDlg  m_IsoEditSettingDlg;
	CDrawingReportSettingDlg m_DrawingRepotrSettingDlg;
	COracleSettingDlg   m_dlgOracleSetting;
	CRefDataSettingDlg  m_RefDataSettingDlg;
	
	CSmartISODiffSettingDlg m_dlgSmartISODiffSetting;

	CMFCTabCtrl m_TabCtrl;
	CMFCButton m_btnSave;
// Attributes
public:
	CImageList m_ImagList;
// Operations
public:
	void UpdateContents();
	CProjectSettingForm();           // protected constructor used by dynamic creation
	virtual ~CProjectSettingForm();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectSettingForm)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CFont m_Font;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CProjectSettingForm)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROJECTSETTINGFORM_H__796C1AB9_E41C_4C2A_986B_EB8FF11A0DC2__INCLUDED_)
