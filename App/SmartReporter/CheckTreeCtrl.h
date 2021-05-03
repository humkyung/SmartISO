#if !defined(AFX_CHECKTREECTRL_H__039B0267_8DE0_48E5_9B42_18E24D9B4C49__INCLUDED_)
#define AFX_CHECKTREECTRL_H__039B0267_8DE0_48E5_9B42_18E24D9B4C49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CheckTreeCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCheckTreeCtrl window

class CCheckTreeCtrl : public CTreeCtrl
{
// Construction
public:
	CCheckTreeCtrl();

// Attributes
public:
	typedef enum
	{
		UNCHECKED	= 1,
		CHECKED		= 2,
		SEMI_CHECKED	= 3
	}CHECK_STATE;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckTreeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL IsSelected(HTREEITEM hItem);
	void CheckItem(HTREEITEM hItem , const CHECK_STATE& check);
	virtual ~CCheckTreeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCheckTreeCtrl)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	int GetChildStateOf(HTREEITEM hItem);
	void CheckChildItemsOf(HTREEITEM hItem , const int& nImage);
	void CheckParentItemsOf(HTREEITEM hItem);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHECKTREECTRL_H__039B0267_8DE0_48E5_9B42_18E24D9B4C49__INCLUDED_)
