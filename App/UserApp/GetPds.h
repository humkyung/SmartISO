#if !defined(AFX_GETPDS_H__4AAA57F2_FB0F_4A40_B047_422943A30080__INCLUDED_)
#define AFX_GETPDS_H__4AAA57F2_FB0F_4A40_B047_422943A30080__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GetPds.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGetPds form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include <GradientStatic.h>
#include <ListCtrlEx.h>
#include "EditLineDataForm.h"
#include "UserAppDoc.h"
#include <map>

using namespace std;

class CGetPds : public CFormView
{
protected:
	DECLARE_DYNCREATE(CGetPds)

public:
	CGradientStatic m_GetPdsStatic;
	CUserAppDoc* m_pUserAppDoc;
	IsGui::CListCtrlEx  m_GetPdsReport;
	CButtonST m_GetPdsButton;

// Form Data
public:
	//{{AFX_DATA(CGetPds)
	enum { IDD = IDD_GET_PDS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

	void DeleteQualifiedLineList();
	void ShowSelectedCount(int nCount);
	void AddLineDataRecordEntry(vector<LineDataRecordMap>* pLineDataRecordEntry);
	void ClearContents();
	void InitContents();
	CGetPds();
	virtual ~CGetPds();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGetPds)
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
	///CPDSLineDataForm* m_pPdsLineDataForm;
	CFont* m_pBoldFont;
	
	map<int , int> m_ItemMap;		/// line data form item -> qualified form item
	map<int , int> m_ReverseItemMap;	/// qualified form item -> line data form item

private:
	void UpdateLineDataRecord(const int &nItem, CLineDataRecord *pLineDataRecord);
	void GetSelectedLineDataRecord(vector<LineDataRecordMap>& LineDataRecordEntry);
	
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CGetPds)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeydownListQualifiedLine(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GETPDS_H__4AAA57F2_FB0F_4A40_B047_422943A30080__INCLUDED_)
