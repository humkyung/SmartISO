#if !defined(AFX_QUALIFIEDLINEFORM_H__38050511_704B_4DAC_AA02_C9784AF18E8B__INCLUDED_)
#define AFX_QUALIFIEDLINEFORM_H__38050511_704B_4DAC_AA02_C9784AF18E8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// QualifiedLineForm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CQualifiedLineForm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include <GradientStatic.h>
#include <ListCtrlEx.h>
#include "UserAppDoc.h"
#include "EditLineDataForm.h"

#include <map>
using namespace std;

class CQualifiedLineForm : public CFormView
{
protected:
	
	DECLARE_DYNCREATE(CQualifiedLineForm)

// Form Data
public:
	//{{AFX_DATA(CQualifiedLineForm)
	enum { IDD = IDD_DIALOG_QUALIFIED_LINE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CUserAppDoc* m_pUserAppDoc;
// Attributes
public:
	IsGui::CListCtrlEx     m_QualifedLineReport;
// Operations
public:
	void GetDataFromIsoDwg();
	int  GetDataFromIDF();
	bool ModifyLineData();
	void CopyLineData();
	void DeleteLineData();
	void DeleteAllQualifiedLineList();
	void DeleteSelectedQualifiedLineList();
	void ShowSelectedCount(int nCount);
	void GetAllLineDataRecord(vector<LineDataRecordMap>& lineDataRecordEntry);
	void AddLineDataRecordEntry(vector<LineDataRecordMap>* pLineDataRecordEntry);
	void ClearContents();
	void InitContents();
	CQualifiedLineForm();           // protected constructor used by dynamic creation
	virtual ~CQualifiedLineForm();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQualifiedLineForm)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CImageList m_ImageList;
	CEditLineDataForm* m_pLineDataForm;
	CFont* m_pBoldFont;

	map<int , int> m_ItemMap;		/// line data form item -> qualified form item
	map<int , int> m_ReverseItemMap;	/// qualified form item -> line data form item
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	///UINT StatusISOThread();
	UINT StatusIDFThread();
	static UINT StatusThreadEntry(LPVOID pVoid);
	// Generated message map functions
	//{{AFX_MSG(CQualifiedLineForm)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRclickListQualifiedLine(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLineDataPrint();
	afx_msg void OnLineDataModify();
	///afx_msg void OnLineDataCopy();
	afx_msg void OnLineDataDelete();
	afx_msg void OnDblclkListQualifiedLine(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownListQualifiedLine(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void UpdateLineDataRecord(const int &nItem, CLineDataRecord *pLineDataRecord);
	void GetSelectedLineDataRecord(vector<LineDataRecordMap>& LineDataRecordEntry);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QUALIFIEDLINEFORM_H__38050511_704B_4DAC_AA02_C9784AF18E8B__INCLUDED_)
