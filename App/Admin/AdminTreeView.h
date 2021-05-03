#if !defined(AFX_ADMINTREEVIEW_H__7BE2B9E6_8848_4015_9091_3B44CFE79130__INCLUDED_)
#define AFX_ADMINTREEVIEW_H__7BE2B9E6_8848_4015_9091_3B44CFE79130__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdminTreeView.h : header file
//

#include <gui/TreeCtrlEx.h>

/////////////////////////////////////////////////////////////////////////////
// CAdminTreeView view

class CAdminTreeView : public CView
{
protected:
	CAdminTreeView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CAdminTreeView)

// Attributes
public:
	CImageList  m_ImageList;
	CTreeCtrlEx m_AdminTreeCtrl;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdminTreeView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CAdminTreeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CAdminTreeView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg void OnClickAdminTree(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
private:
	void OnClickLinemanDataImport();
	void OnClickSetting();
	void OnClickDefTable();

	static CString m_rTreeItemText[5];
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADMINTREEVIEW_H__7BE2B9E6_8848_4015_9091_3B44CFE79130__INCLUDED_)
