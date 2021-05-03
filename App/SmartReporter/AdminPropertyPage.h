#if !defined(AFX_ADMINPROPERTYPAGE_H__D8E33B39_4D30_4781_A4BE_A3D218B71746__INCLUDED_)
#define AFX_ADMINPROPERTYPAGE_H__D8E33B39_4D30_4781_A4BE_A3D218B71746__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserProperyPage.h : header file
//
#include <BtnST.h>
/////////////////////////////////////////////////////////////////////////////
// CAdminPropertyPage dialog

class CAdminPropertyPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CAdminPropertyPage)
	void SaveData( const CString& rIniFilePath );
// Construction
public:
	void LoadData(const CString& rIniFilePath);
	
	CAdminPropertyPage();
	~CAdminPropertyPage();

// Dialog Data
	//{{AFX_DATA(CAdminPropertyPage)
	enum { IDD = IDD_DIALOG_PROPERTY };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
		CString m_rAdminPassword;
		CString m_rServiceName;
		CString m_rUserName;
		CString m_rUserPassword;
		CString m_rProjectName;
	//}}AFX_DATA

	UINT GetIconID() {return IDR_MAINFRAME;}
// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CAdminPropertyPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAdminPropertyPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonConnectTest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADMINPROPERTYPAGE_H__D8E33B39_4D30_4781_A4BE_A3D218B71746__INCLUDED_)
