#if !defined(AFX_PROJECTDEFTABLEDLG_H__D9BA8AC5_B8E1_45B1_8EEB_A67B9B2F11E1__INCLUDED_)
#define AFX_PROJECTDEFTABLEDLG_H__D9BA8AC5_B8E1_45B1_8EEB_A67B9B2F11E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProjectDefTableDlg.h : header file
//

#include <gui/ListCtrlEx.h>
#include <FilterEdit/FilterEdit.h>
#include "GridCtrl/GridCtrl.h"
#include <BtnST.h>
#include <ResizeWnd.h>

/////////////////////////////////////////////////////////////////////////////
// CProjectDefTableDlg form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#define C_PLANT_NO		"PLANT_NO"
#define C_AREA_NO		"AREA_NO"
#define C_UNIT_NO		"UNIT_NO"
#define C_DRAW_NO		"DRAW_NO"
#define	C_SECTION_NO	"SECTION_NO"
#define C_LINE_NO		"LINE_NO"
#define	C_SHEET_NO		"SHEET_NO"
#define	C_FLUID_CODE	"FLUID_CODE"
#define C_PDS_AREA_NO	"PDS_AREA_NO"
#define C_PLAN_NO		"PLAN_NO"
#define C_PID_NO		"PID_NO"

#define C_REV_NO		"REV_NO"
#define C_REV_CONTROL_WORKING_DATE	"REV_CONTROL_WORKING_DATE"
#define C_REV_DATE					"REV_DATE"
#define C_REV_DRWN_BY				"REV_DRWN_BY"
#define C_REV_CHECKED_BY			"REV_CHECKED_BY"
#define REV_APPROVED_BY				"REV_APPROVED_BY"

#define C_PAINT_CODE	"PAINT_CODE"
#define C_INS_TYPE		"INS_TYPE"
#define C_INS_THICKNESS "INS_THICKNESS"
#define	C_INS_TEMP		"INS_TEMP"

#define C_OPER_PRES	"OPER_PRES"
#define C_OPER_TEMP	"OPER_TEMP"
#define C_DGN_PRES	"DGN_PRES"
#define C_DGN_TEMP	"DGN_TEMP"

typedef struct
{
	CString rFieldName;		///< ÇÊµå ÀÌ¸§
	CString rFieldDesc;		///< ÇÊµå ¼³¸í
	CString rExcelColumn;	///< ¿¢¼¿ Ä®·³µé...
}Map;

class CProjectDefTableDlg : public CFormView
{
protected:
	DECLARE_DYNCREATE(CProjectDefTableDlg)
	BOOL IsDBOpen;
	_ConnectionPtr m_pCON;
	_CommandPtr m_pCOM;
	_RecordsetPtr m_pRS;
// Form Data
public:
	//{{AFX_DATA(CProjectDefTableDlg)
	enum { IDD = IDD_DIALOG_PROJECT_DEF };
		// NOTE: the ClassWizard will add data members here
	
	CGridCtrl m_GridCtrl;
	IsGui::CListCtrlEx	m_wndSheetNameListCtrl , m_wndCurrentLineDataListCtrl;
	CImageList m_ImageList , m_imgCheckBoxes;
	CMFCButton m_btnSelectExcel , m_btnDatabaseReferenceKey;
	CComboBox m_cboStartRow;

	CString m_sExlFilePath , m_rDatabaseReferenceKeyFormat;
	CEdit m_wndDatabaseReferenceKeyEdit;
	//}}AFX_DATA
// Attributes
public:
	int TColNum;
	int m_iCurCol;
// Operations
public:
	BOOL UpdateContents();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectDefTableDlg)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	CProjectDefTableDlg();           // protected constructor used by dynamic creation
	virtual ~CProjectDefTableDlg();
	void OnButtonLineListImport();
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CProjectDefTableDlg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEditchangeComboFieldType();
	afx_msg void OnButtonSelectExcel();
	afx_msg void OnBnClickedButtonDatabaseReferenceKey();
	afx_msg void OnKeydownListCurrentLineData(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	//afx_msg void OnHorzResize();
private:
	BOOL InitLineDataContents();
	bool IsFixedRevDataFieldName(CString rFieldName);
	bool IsFixedLineDataFieldName(const CString& rFieldName);
	void DetermineLayout();
	int LoadSheetName(const CString&);
	int ImportLineDataFile(const CString& rExlFilePath , const CString& rIniFilePath);
	int DisplayCurrentLineData(CAppPostgreSQL&);
	int GetLineDataMappingData(map<CString,CString>& oMappingDataEntry , CAppPostgreSQL&);	/// 2012.08.23 added by humkyung
	CString GetCellString(VARIANT &valCell) const;
	long FieldName2Long(const CString &rFieldName) const;
	int appendHeader(ofstream& ofile) const;
	int DeleteSelectedRecordIfDuplicated();	/// 2011.07.07 - added by humkyung

	static UINT StatusThreadEntry(LPVOID pVoid);
	UINT StatusThread();

	vector<Map*> m_LineDataMapData;
public:
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROJECTDEFTABLEDLG_H__D9BA8AC5_B8E1_45B1_8EEB_A67B9B2F11E1__INCLUDED_)
