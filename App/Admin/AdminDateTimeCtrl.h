#if !defined(AFX_ADMINDATETIMECTRL_H__6931CE37_C05B_453A_BE6A_62575168CE9D__INCLUDED_)
#define AFX_ADMINDATETIMECTRL_H__6931CE37_C05B_453A_BE6A_62575168CE9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AdminDateTimeCtrl.h : header file
//
#include "LineDataSettingGridCell.h"
/////////////////////////////////////////////////////////////////////////////
// CAdminDateTimeCtrl window

class CAdminDateTimeCtrl : public CDateTimeCtrl
{
// Construction
public:
	CAdminDateTimeCtrl( const int& row , const int& column , GridCell::CLineDataSettingGridCell* pParent );

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdminDateTimeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAdminDateTimeCtrl();

	// Generated message map functions
protected:
	int m_nRow , m_nColumn;
	GridCell::CLineDataSettingGridCell* m_pParent;
	//{{AFX_MSG(CAdminDateTimeCtrl)
	afx_msg void OnDatetimechange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUserstring(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCloseup(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADMINDATETIMECTRL_H__6931CE37_C05B_453A_BE6A_62575168CE9D__INCLUDED_)
