#pragma once

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OracleSettingPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COracleSettingPage dialog

class COracleSettingPage : public CPropertyPage
{
	DECLARE_DYNCREATE(COracleSettingPage)

// Construction
public:
	void LoadData(const CString& rIniFilePath);
	void SaveData(const CString& rIniFilePath);
	COracleSettingPage();
	~COracleSettingPage();

// Dialog Data
	//{{AFX_DATA(COracleSettingPage)
	enum { IDD = IDD_ORACLE_SETTING_PAGE };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
		CString m_rServiceName;
		CString m_rUserName;
		CString m_rPassword;
		CString m_rPDSProjectName;
	//}}AFX_DATA


	UINT GetIconID(){ return IDI_ORACLE; }
// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(COracleSettingPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(COracleSettingPage)
	afx_msg void OnButtonConnectTest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
