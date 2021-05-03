#if !defined(AFX_GETDATAFROMIDFSTARTUPDLG_H__C95FF108_E5E9_49AB_A69E_F74008FCE163__INCLUDED_)
#define AFX_GETDATAFROMIDFSTARTUPDLG_H__C95FF108_E5E9_49AB_A69E_F74008FCE163__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GetDataFromIDFStartUpDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGetDataFromIDFStartUpDlg dialog

class CGetDataFromIDFStartUpDlg : public CDialog
{
// Construction
public:
	CGetDataFromIDFStartUpDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGetDataFromIDFStartUpDlg)
	enum { IDD = IDD_GETDATA_FROM_IDF };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CString m_rUNITX , m_rUNITY , m_rUNITZ;
	CString m_rDVCSX , m_rDVCSY , m_rDVCSZ;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGetDataFromIDFStartUpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGetDataFromIDFStartUpDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GETDATAFROMIDFSTARTUPDLG_H__C95FF108_E5E9_49AB_A69E_F74008FCE163__INCLUDED_)
