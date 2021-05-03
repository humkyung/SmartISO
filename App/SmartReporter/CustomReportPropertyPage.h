#if !defined(AFX_CUSTOMREPORTPROPERTYPAGE_H__A07021A4_528F_461D_B8E4_970EC90AFDD6__INCLUDED_)
#define AFX_CUSTOMREPORTPROPERTYPAGE_H__A07021A4_528F_461D_B8E4_970EC90AFDD6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CustomReportPropertyPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCustomReportPropertyPage dialog

class CCustomReportPropertyPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CCustomReportPropertyPage)

// Construction
public:
	CCustomReportPropertyPage();
	~CCustomReportPropertyPage();

// Dialog Data
	//{{AFX_DATA(CCustomReportPropertyPage)
	enum { IDD = IDD_CUSTOM_REPORT_PROPERTY };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	CString m_sName , m_sUnit , m_sDiscipline , m_sStartRow;
	CMFCButton m_btnAdd , m_btnDelete;
	//}}AFX_DATA

	UINT GetIconID() {return IDR_LineControlManaTYPE;}
// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CCustomReportPropertyPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CCustomReportPropertyPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSqlFile();
	afx_msg void OnButtonExcelFile();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDelete();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUSTOMREPORTPROPERTYPAGE_H__A07021A4_528F_461D_B8E4_970EC90AFDD6__INCLUDED_)
