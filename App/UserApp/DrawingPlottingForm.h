#if !defined(AFX_DRAWINGPLOTTINGFORM_H__0208A889_7931_4224_860B_3B39B0C13D60__INCLUDED_)
#define AFX_DRAWINGPLOTTINGFORM_H__0208A889_7931_4224_860B_3B39B0C13D60__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DrawingPlottingForm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDrawingPlottingForm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include <GradientStatic.h>
#include <ResizeWnd.h>
#include "IsoFileListCtrl.h"
#include "LineListSelectOption1.h"
#include "SelectIssueDataDlg.h"

class CViewLogFileForm;
class CDrawingPlottingForm : public CFormView
{
protected:           
	DECLARE_DYNCREATE(CDrawingPlottingForm)

// Form Data
public:
	//{{AFX_DATA(CDrawingPlottingForm)
	enum { IDD = IDD_DRAWING_PLOTTING };
	//}}AFX_DATA
public:
	void GetSelectedLineDataRecord(vector<LineDataRecordMap>&);
	int Generate();
	void SetViewLogFileForm(CViewLogFileForm* p);
	void InitContents();
	CDrawingPlottingForm();
	virtual ~CDrawingPlottingForm();
	
	IsGui::CListCtrlEx m_wndIsoFileList;
	CGradientStatic m_QualifiedStatic;

	int m_nRecordCount;
protected:
	CViewLogFileForm* m_pViewLogFileForm;
	CImageList m_ImageList;
	CFont* m_pBoldFont;

	CString	m_rOutputDrawingFolder;
	CString m_rSourceFilePath;
	CString m_rBorderFilePath;
	CString m_sPrinterName;

	map<int , int> m_ItemMap;
	map<int , int> m_ReverseItemMap;
	map<int , CLineDataRecord*> m_IsoFileMap;
	CLineDataRecordSet* m_pLineDataRecordSet;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawingPlottingForm)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL
// Implementation
protected:
	CString GetPlottingAreaString() const;	/// 2012.09.19 added by humkyung
	void DisplayOutputFileWithDataRecord();
	void Print_IPlot(const int& nIndex , const int& nCount , const CString &rIsoFile);
	int System_IPlot(CString ip);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CDrawingPlottingForm)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnUpdatePrint(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString DownloadIsoDwgFile(const CString& sFileName,CLineDataRecord* pLineDataRecord) const;
	static UINT StatusThreadEntry(LPVOID pVoid);
	void DetermineLayout(int cx, int cy);
	void LoadLineDataRecordSetFromDatabase(const CString &);

	UINT StatusThread();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWINGPLOTTINGFORM_H__0208A889_7931_4224_860B_3B39B0C13D60__INCLUDED_)
