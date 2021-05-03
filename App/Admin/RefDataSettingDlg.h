#if !defined(AFX_REFDATASETTINGDLG_H__3783057A_212C_475D_BACA_954EB4C9F03E__INCLUDED_)
#define AFX_REFDATASETTINGDLG_H__3783057A_212C_475D_BACA_954EB4C9F03E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RefDataSettingDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRefDataSettingDlg dialog

class CRefDataSettingDlg : public CDialog
{
// Construction
public:
	void UpdateContents();
	void Save();
	CRefDataSettingDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRefDataSettingDlg)
	enum { IDD = IDD_REF_DATA_SETTING };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CMFCPropertyGridCtrl m_wndPropList;
	
	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRefDataSettingDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CFont m_fntPropList;
	// Generated message map functions
	//{{AFX_MSG(CRefDataSettingDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void InitPropList();
	void SetPropListFont();

	map<STRING_T , CMFCPropertyGridProperty*> m_oGridPropertyMap;	/// 2012.05.06 added by humkyung
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REFDATASETTINGDLG_H__3783057A_212C_475D_BACA_954EB4C9F03E__INCLUDED_)
