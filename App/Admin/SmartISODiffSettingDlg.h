#pragma once

#include "AdminDoc.h"

// CSmartISODiffSettingDlg dialog

class CSmartISODiffSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CSmartISODiffSettingDlg)

public:
	CSmartISODiffSettingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSmartISODiffSettingDlg();

	void UpdateContents();
	void Save();

// Dialog Data
	enum { IDD = IDD_SMART_ISO_DIFF_SETTING };

	CMFCPropertyGridCtrl m_wndPropList;
	
	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}

	CAdminDoc* m_pAdminDoc;
protected:
	CFont m_fntPropList;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	void InitPropList();
	void SetPropListFont();

	map<STRING_T , CMFCPropertyGridProperty*> m_oGridPropertyMap;	/// 2012.05.09 added by humkyung
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonRevIsoFolder();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
