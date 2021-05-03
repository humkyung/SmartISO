#if !defined(AFX_USERAPPSETTINGDLG_H__C0B3493A_3FCB_4CBE_BF2C_744F961D11F6__INCLUDED_)
#define AFX_USERAPPSETTINGDLG_H__C0B3493A_3FCB_4CBE_BF2C_744F961D11F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserAppSettingDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUserAppSettingDlg dialog

class CUserAppSettingDlg : public CPropertyPage
{
	DECLARE_DYNCREATE(CUserAppSettingDlg)

// Construction
public:
	CUserAppSettingDlg();
	~CUserAppSettingDlg();
	CString m_MStationPath;
	CEdit m_MSEdit;

// Dialog Data
	//{{AFX_DATA(CUserAppSettingDlg)
	enum { IDD = IDD_SETTING };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CUserAppSettingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CUserAppSettingDlg)
	afx_msg void OnMStationOpen();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERAPPSETTINGDLG_H__C0B3493A_3FCB_4CBE_BF2C_744F961D11F6__INCLUDED_)
