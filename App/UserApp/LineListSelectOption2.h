#if !defined(AFX_LINELISTSELECTOPTION2_H__D322F1BD_0684_4BF4_AF39_A42C9CC9E318__INCLUDED_)
#define AFX_LINELISTSELECTOPTION2_H__D322F1BD_0684_4BF4_AF39_A42C9CC9E318__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LineListSelectOption2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLineListSelectOption2 dialog

class CLineListSelectOption2 : public CDialog
{
// Construction
public:
	CLineListSelectOption2(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLineListSelectOption2)
	enum { IDD = IDD_LINE_LIST_SELECT_OPTION2 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLineListSelectOption2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLineListSelectOption2)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINELISTSELECTOPTION2_H__D322F1BD_0684_4BF4_AF39_A42C9CC9E318__INCLUDED_)
