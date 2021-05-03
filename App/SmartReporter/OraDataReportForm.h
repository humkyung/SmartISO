#if !defined(AFX_ORADATAREPORTFORM_H__9F8AB20C_EB27_4F56_BBDC_F0398BE2E64B__INCLUDED_)
#define AFX_ORADATAREPORTFORM_H__9F8AB20C_EB27_4F56_BBDC_F0398BE2E64B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OraDataReportForm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COraDataReportForm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include <BtnST.h>
#include <BCGTabWnd.h>
#include "CustomDataReportDlg.h"

class COraDataReportForm : public CView
{
protected:
	COraDataReportForm();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(COraDataReportForm)

// Form Data
public:
	//{{AFX_DATA(COraDataReportForm)
	enum { IDD = IDD_ORA_DATA_REPORT };
		// NOTE: the ClassWizard will add data members here
	CMFCTabCtrl m_wndTab;
	CButtonST   m_wndSelectAllAreaNoButton;
	CListCtrlEx m_wndAreaNoReport;
	CButtonST   m_wndSelectAllModelNoButton;
	CListCtrlEx m_wndModelNoReport;
	//}}AFX_DATA

	CLineControlManagerDoc* GetDocument(){return NULL;}
	int SyncTabCtrlWithCustomReportTableList();
// Attributes
private:
	///vector<CCustomDataReportDlg*> m_vecCustomDataReportDlg;
// Operations
public:
	void LoadData();
	virtual ~COraDataReportForm();
private:
	int CreateCustomDataReportDialog(CCustomReportTable* pCustomReportTable);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COraDataReportForm)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CImageList m_imgList;

	CString m_rServiceName;
	CString m_rUserName;
	CString m_rUserPassword;
	CString m_rProjectName;	
	CString m_rStdNoteFilePath;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(COraDataReportForm)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonExcelExport();
	afx_msg void OnButtonSelectAllAreaNo();;
	afx_msg void OnButtonSelectAllModelNo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ORADATAREPORTFORM_H__9F8AB20C_EB27_4F56_BBDC_F0398BE2E64B__INCLUDED_)
