#if !defined(AFX_LINELISTDATAIMPORTDLG_H__D8D53A9E_DAE8_4386_9BC7_5B7C4462A5C2__INCLUDED_)
#define AFX_LINELISTDATAIMPORTDLG_H__D8D53A9E_DAE8_4386_9BC7_5B7C4462A5C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LineListDataImportDlg.h : header file
//
#include <FilterEdit/FilterEdit.h>
#include <XButtonXP/XButtonXP.h>
#include <ListCtrlEx.h>
#include "GridCtrl/GridCtrl.h"
#include <BtnSt.h>
#include <ado/ADODB.h>
#include <afxhtml.h>
/////////////////////////////////////////////////////////////////////////////
// CLineListDataImportDlg form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CLineListDataImportDlg : public CFormView
{
protected:
	DECLARE_DYNCREATE(CLineListDataImportDlg)

// Form Data
public:
	//{{AFX_DATA(CLineListDataImportDlg)
	enum { IDD = IDD_LINELIST_DATA_IMPORT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
// Attributes
public:
	CXButtonXP m_selectExcelButton;
	CFilterEdit m_wndStartRowEdit;
	IsGui::CListCtrlEx	m_wndSheetNameListCtrl , m_wndCurrentLineDataListCtrl;
	CXButtonXP m_selectAllButton;
	CXButtonXP m_unselectAllButton;
	CXButtonXP m_convertButton;
	CFont m_Font;
	CString m_rExcelFilePath;
	CString rMdbPath;
	CString rTempPath;
	CString S_ExPath;

	CButtonST m_btnDatabaseReferenceKey;
	CEdit m_wndDatabaseReferenceKeyEdit;
	CString m_rDatabaseReferenceKeyFormat;

	CImageList m_ImageList;
	CGridCtrl m_GridCtrl;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLineListDataImportDlg)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	static UINT StatusThreadEntry(LPVOID pVoid);
	UINT StatusThread();
// Implementation
public:
	BOOL UpdateContents();
	CLineListDataImportDlg();
	virtual ~CLineListDataImportDlg();
protected:
	
	CFont m_16UnderlineFont;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CLineListDataImportDlg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBUTTONSelectExcel();
	afx_msg void OnButtonLineManDataImport();
	afx_msg void OnKeydownListCurrentLineData(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int GetLineDataMappingData(map<CString,CString>& oMappingDataEntry , CADODB& adoDB);	/// 2012.08.23 added by humkyung
	int DeleteSelectedRecordIfDuplicated();	/// 2011.07.07 - added by humkyung
	int LoadSheetName();
	int appendHeader(ofstream& ofile);
	int DisplayCurrentLineData(CAppPostgreSQL&);
	int ImportLineDataFile(const CString rExlFilePath , const CString& rIniFilePath);
	CString GetCellString(VARIANT &valCell);
	long FieldName2Long(CString &rFieldName);
public:
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnBnClickedButtonDatabaseReferenceKey();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINELISTDATAIMPORTDLG_H__D8D53A9E_DAE8_4386_9BC7_5B7C4462A5C2__INCLUDED_)
