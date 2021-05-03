#if !defined(AFX_ISOEDITVIEWLOGFILEFORM_H__355FE560_15F3_4688_8761_5EC7B2283549__INCLUDED_)
#define AFX_ISOEDITVIEWLOGFILEFORM_H__355FE560_15F3_4688_8761_5EC7B2283549__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IsoEditViewLogFileForm.h : header file
//
#include <GradientStatic.h>
#include "MFCTextColorListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CIsoEditViewLogFileForm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#define __MEM_CRY_EDIT_VER__	0
#define	QC_CHECK_ITEM_COUNT		13

class CIsoEditViewLogFileForm : public CFormView
{
protected:
	
	DECLARE_DYNCREATE(CIsoEditViewLogFileForm)

// Form Data
public:
	//{{AFX_DATA(CIsoEditViewLogFileForm)
	enum { IDD = IDD_DIALOG_VIEW_LOG_FILE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CMFCTabCtrl m_wndTabCtrl;
	CMFCTextColorListCtrl m_wndErrorListCtrl;
	CScintillaWnd m_wndScintilla;

// Attributes
public:

// Operations
public:
	int DisplayErrorSum();						/// 2012.08.10 added by humkyung
	int SaveErrorSum(const CString &rFilePath);	/// 2012.08.10 added by humkyung
	void DisplayLog();
	bool SaveLog( const CString& rFilePath );
	void ClearAllLog();
	void AddLog(const CString& rLog , const bool& bShow = true );
	CIsoEditViewLogFileForm();           // protected constructor used by dynamic creation
	virtual ~CIsoEditViewLogFileForm();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIsoEditViewLogFileForm)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	///CCrystalTextBuffer m_textBuffer;
	CFont* m_pBoldFont;
	vector< string > m_LogTextEntry;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CIsoEditViewLogFileForm)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDblclkErrorListCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	int CreateScintillaWnd();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonExcelExport();
	afx_msg void OnUpdateExcelExport(CCmdUI* pCmdUI);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ISOEDITVIEWLOGFILEFORM_H__355FE560_15F3_4688_8761_5EC7B2283549__INCLUDED_)
