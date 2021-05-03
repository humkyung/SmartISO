#if !defined(AFX_DRAWINGREPORTFORM_H__30AA1E17_3CEC_4BB0_837C_52CFAC045F31__INCLUDED_)
#define AFX_DRAWINGREPORTFORM_H__30AA1E17_3CEC_4BB0_837C_52CFAC045F31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DrawingReportForm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDrawingReportForm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include <BtnST.h>
#include <GradientStatic.h>
#include <ResizeWnd.h>
#include <ListCtrlEx.h>
#include <excel9/ExcelAutomation.h>
#include "LineListSelectOption1.h"
#include "DrawingReportLogFileForm.h"
#include <IsRegExp.h>

class CDrawingReportForm : public CFormView
{
protected:
	
	DECLARE_DYNCREATE(CDrawingReportForm)

// Form Data
public:
	//{{AFX_DATA(CDrawingReportForm)
	enum { IDD = IDD_INDEX_REPORT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CGradientStatic m_StatusStaic;
	
	IsGui::CListCtrlEx m_wndLineDataReport;
	CString m_rIniFilePath;
	CString m_rIndexReportFolder;
	CLineDataRecordSet* m_pLineDataRecordSet;
	CMFCButton m_PrintButton;
	bool m_FileExist;
	int m_nRecordCount;
// Attributes
public:
	CDrawingReportLogFileForm* m_pLogFileForm;
// Operations
public:
	int Generate();
	void GetSelectedLineDataRecord(vector<LineDataRecordMap>&);
	void ShowRecordCount(const int& nValidRecordCount , const int& nRecordCount);
	CString SetNextColumn(CString nFirst , UINT nCol);
	CString GetRevIssueDateAtIndex( CLineDataRecord* pLineDataRecord , const int& nIndex );
	void SetViewLogFileForm(CDrawingReportLogFileForm* p);
	void InitContents();
	CDrawingReportForm();           // protected constructor used by dynamic creation
	virtual ~CDrawingReportForm();
	void LoadLineDataRecordSetFromDatabase(const CString &rQueryCondition , LPCTSTR pRevNo , const vector< STRING_T >& issueDates );
	void DisplayQueriedReport(const int& nRecordCount , const CString& rRevNo);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawingReportForm)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnInitialUpdate();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CImageList m_ImageList;
	CFont* m_pBoldFont;
	map<int , CLineDataRecord*> m_IsoFileMap;

	IsRegExp::RxNode *m_rxnode;
	IsRegExp::RxMatchRes m_rxmatch;
private:
	int ReplaceUserInput(CExcelAutomation& xlApp);	/// 2012.08.31 added by humkyung
	CString GetExcelTemplateFromDatabase();	/// 2012.08.15 added by humkyung
	void ShowQualifiedCount();
	string GetRevLastIssueNo(CLineDataRecord *pLineDataRecord );
	void DetermineLayout(int cx , int cy);

	UINT StatusThread();
	static UINT StatusThreadEntry(LPVOID pVoid);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CDrawingReportForm)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRclickFileList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateButtonPrint(CCmdUI* pCmdUI);
	afx_msg void OnExlView();
	afx_msg void OnUpdateButtonExlView(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWINGREPORTFORM_H__30AA1E17_3CEC_4BB0_837C_52CFAC045F31__INCLUDED_)
