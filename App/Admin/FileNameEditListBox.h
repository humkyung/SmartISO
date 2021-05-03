#if !defined(AFX_FILENAMEEDITLISTBOX_H__F85FB04B_8B17_4931_B373_75FCA4DEA9B8__INCLUDED_)
#define AFX_FILENAMEEDITLISTBOX_H__F85FB04B_8B17_4931_B373_75FCA4DEA9B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileNameEditListBox.h : header file
//
#include <EditListBox/EditListBox.h>
/////////////////////////////////////////////////////////////////////////////
// CFileNameEditListBox window

class CFileNameEditListBox : public EditListBox::CEditListBox
{
// Construction
public:
	CFileNameEditListBox();

// Attributes
public:
// Operations
public:
	CComboBox m_LineDataFieldComboBox;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileNameEditListBox)
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual void EndEditing( BOOL fCancel );
	virtual void BeginEditing(int nItem);
	virtual BOOL OnBeginEditing(int iItem);
	virtual void EditNew();
	virtual void OnBrowseButton( int iItem );
	virtual ~CFileNameEditListBox();

	// Generated message map functions
protected:
	BOOL CreateComboBox(DWORD dwStyle);
	//{{AFX_MSG(CFileNameEditListBox)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILENAMEEDITLISTBOX_H__F85FB04B_8B17_4931_B373_75FCA4DEA9B8__INCLUDED_)
