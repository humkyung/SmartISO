#if !defined(AFX_PDSLINEDATAFORM_H__C1B50B2B_C073_4DD7_9450_10115CECB767__INCLUDED_)
#define AFX_PDSLINEDATAFORM_H__C1B50B2B_C073_4DD7_9450_10115CECB767__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PDSLineDataForm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPDSLineDataForm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include <ResizableLib/ResizableFormView.h>
#include <BCGTabWnd.h>
#include <ListCtrlEx.h>
#include "LineListSelectOption1.h"
#include <OraDatabaseDefFile.h>
#include "PDSOracle.h"
#include "UserAppDocData.h"

class CGetPds;
class CPDSLineDataForm : public CFormView
{
protected:	
	DECLARE_DYNCREATE(CPDSLineDataForm)
// Form Data
public:
	typedef struct 
	{
		CString FieldName , FieldDesc;
	}LineDataField;

	typedef struct 
	{
		CString FieldName , ExcelCol;
	}MappingDataField;

	CGetPds* m_pGetPdsForm;
	//{{AFX_DATA(CPDSLineDataForm)
	enum { IDD = IDD_DIALOG_PDS_LINE_DATA };
	CListCtrlEx m_wndLineDataReport;
	CButtonST m_selectAllButton;
	CButtonST m_unselectAllButton;
	//}}AFX_DATA

	CBCGTabWnd m_TabCtrl;
	CLineListSelectOption1 m_opt1;
// Attributes
public:
	int m_nRecordCount;
	BOOL IsDBOpen;
	CString m_rServiceName , m_rUserId , m_rPassword , m_rPDSProjectName;
	//CGetPdsLineID* m_pLogFileForm;
	/// _ConnectionPtr m_pCON;
// Operations
public:
	bool IsNumberStr(const string& str) const;
	void ShowRecordCount(int nRecordCount);
	void InitContents();
	void SetViewLogFileForm(CGetPds* p);
	CPDSLineDataForm();           // protected constructor used by dynamic creation
	virtual ~CPDSLineDataForm();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPDSLineDataForm)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnInitialUpdate();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	vector<LineDataField>    m_LineDataFieldEntry;
	vector<MappingDataField> m_MappingDataFieldEntry;
	vector<LineWithOracle>   m_LineWithOracleList;
protected:
	CString m_rDWGFilePath;
	CLineDataRecordSet* m_pLineDataRecordSet;
	CImageList m_ImageList;

	COraDatabaseDefFile m_oraDatabaseDefFile;
	CPDSOracle m_pdsOracle;
	
	void UpdateLineDataRecord(const int& nItem , CLineDataRecord* pLineDataRecord);
	void DisplayLineDataRecordSet();
	void LoadLineDataRecordSetFromDatabase(const CString& rQueryCondition , const vector< string >& issueDates );
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CPDSLineDataForm)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonGet();
	afx_msg void OnButtonSelectAll();
	afx_msg void OnButtonUnSelectAll();
	afx_msg void OnButtonDown();
	afx_msg void OnButtonUp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	static UINT StatusThreadEntry(LPVOID pVoid);
	UINT StatusThread();

	bool IsConvertedFromExcel(const CString& rFieldDesc);
	void GetSelectedLineDataRecord(vector<LineDataRecordMap>& LineDataRecordEntry);
	void QualifyLineData();
	CString GetConditionString(const CLineDataRecord* pLineDataRecord);
	bool UpdateIsoManDatabaseWithOra(CADODB* pAdoDB , vector<CString>& tableNames , CLineDataRecord* pLineDataRecord);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PDSLINEDATAFORM_H__C1B50B2B_C073_4DD7_9450_10115CECB767__INCLUDED_)
