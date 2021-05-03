#if !defined(AFX_INSERTTOPDSATTRIBUTE_H__6E780EF8_7AB6_4622_8500_E7506285F56E__INCLUDED_)
#define AFX_INSERTTOPDSATTRIBUTE_H__6E780EF8_7AB6_4622_8500_E7506285F56E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InsertToPDSAttribute.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInsertToPDSAttribute form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include <ado/AdoRemoteDB.h>
#include "LineDataRecordSet.h"
#include <GradientStatic.h>
#include <ListCtrlEx.h>
#include "LineListSelectOption1.h"
#include "LineListSelectOption2.h"
#include <OraDatabaseDefFile.h>
#include "PDSOracle.h"
#include "UserAppDocData.h"

class CEmptyOutputWnd;
class CInsertToPDSAttribute : public CFormView
{
protected:
	DECLARE_DYNCREATE(CInsertToPDSAttribute)

// Form Data
public:
	CEmptyOutputWnd* m_pEmptyOutputWnd;
	//{{AFX_DATA(CInsertToPDSAttribute)
	enum { IDD = IDD_INSERT_TO_PDS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	IsGui::CListCtrlEx m_wndLineDataReport;
private:
	CString m_rDataSource,m_rUserID,m_rPW;
	CString m_rPDSProjectName;
	vector<LineWithOracle> m_LineWithOracleList;
// Operations
public:
	void GetSelectedLineDataRecord(vector<LineDataRecordMap>&);
	bool HasAlphaCharacter(const STRING_T& str) const;
	void InitContents();
	void SetViewLogFileForm(CEmptyOutputWnd *p);
	void LoadLineDataRecordSetFromDatabase(const CString& rQueryCondition , const vector< string >& issueDates );
	void UpdateLineDataRecord(const int& nItem , CLineDataRecord* pLineDataRecord);
	CInsertToPDSAttribute();
	virtual ~CInsertToPDSAttribute();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInsertToPDSAttribute)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	CImageList m_ImageList;
	CLineDataRecordSet* m_pLineDataRecordSet;

	COraDatabaseDefFile m_oraDatabaseDefFile;
	CPDSOracle m_pdsOracle;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CInsertToPDSAttribute)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonInsertOra();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	UINT StatusThread(vector<LineDataRecordMap>& oLineDataRecordEntry);
private:
	int GetConditionString(CString& sInnerClause , CString& sOuterClause , const CLineDataRecord* pLineDataRecord);
	bool UpdateOracleDatabase(ISmartISOLayerPtr&,const CLineDataRecord* pLineDataRecord);
	void DisplayLineDataRecordSet();
	static UINT StatusThreadEntry(LPVOID pVoid);
public:
	afx_msg void OnBnClickedButtonUpdateOracle();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INSERTTOPDSATTRIBUTE_H__6E780EF8_7AB6_4622_8500_E7506285F56E__INCLUDED_)
