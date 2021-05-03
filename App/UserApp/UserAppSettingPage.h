#if !defined(AFX_USERAPPSETTINGPAGE_H__C0B3493A_3FCB_4CBE_BF2C_744F961D11F6__INCLUDED_)
#define AFX_USERAPPSETTINGPAGE_H__C0B3493A_3FCB_4CBE_BF2C_744F961D11F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserAppSettingDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUserAppSettingPage dialog

class CUserAppSettingPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CUserAppSettingPage)
	void LoadData( const CString& rIniFilePath );
	void SaveData( const CString& rIniFilePath );
// Construction
public:
	
	CUserAppSettingPage();
	~CUserAppSettingPage();
	static CString m_MStationPath;
	CEdit m_MSEdit;

	UINT GetIconID() {return IDI_SETTING;}
// Dialog Data
	//{{AFX_DATA(CUserAppSettingPage)
	enum { IDD = IDD_SETTING };
		// NOTE - ClassWizard will add data members here.
	//}}AFX_DATA
	static CString APP;

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CUserAppSettingPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CUserAppSettingPage)
	afx_msg void OnMStationOpen();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERAPPSETTINGPAGE_H__C0B3493A_3FCB_4CBE_BF2C_744F961D11F6__INCLUDED_)
