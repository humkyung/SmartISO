#pragma once

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserProperyPage.h : header file
//
#include <BtnST.h>
/////////////////////////////////////////////////////////////////////////////
// CUserPropertyPage dialog

class CUserPropertyPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CUserPropertyPage)
	void SaveData( const CString& rIniFilePath );
// Construction
public:
	
	CUserPropertyPage();
	~CUserPropertyPage();

// Dialog Data
	//{{AFX_DATA(CUserPropertyPage)
	enum { IDD = IDD_DIALOG_PROPERTY };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	CString m_rID;
	//}}AFX_DATA

	UINT GetIconID() {return IDR_MAINFRAME;}
// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CUserPropertyPage)
	protected:
	CString m_rUserPassword;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CUserPropertyPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
