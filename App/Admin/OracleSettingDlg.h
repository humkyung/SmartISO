#if !defined(AFX_ORACLESETTINGDLG_H__C3868156_3768_4756_8B5E_421AC1241369__INCLUDED_)
#define AFX_ORACLESETTINGDLG_H__C3868156_3768_4756_8B5E_421AC1241369__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OracleSettingDlg.h : header file
//

#include "StdAfx.h"

#include <afxdisp.h>
#include <atlbase.h>
#include <mscoree.h>
#include <comutil.h>

#include "AdminDoc.h"
#include "GridCtrl/GridCtrl.h"
#include <ReportCtrl/ReportCtrl.h>
#include <GroupCheck.h>
#include <OraDatabaseDefFile.h>

//#import <mscorlib.tlb> auto_rename raw_interfaces_only
//using namespace mscorlib;

typedef struct
{
	CString rFieldName;		///< 필드 이름
	CString rFieldDesc;		///< 필드 설명
}MappingMap;
/////////////////////////////////////////////////////////////////////////////
// COracleSettingDlg dialog

class COracleSettingDlg : public CDialog
{
	DECLARE_DYNCREATE(COracleSettingDlg)
// Construction
public:
	void Save();
	void UpdateContents();
	COracleSettingDlg(CWnd* pParent = NULL);// standard constructor
	virtual ~COracleSettingDlg();
	CAdminDoc* m_pAdminDoc;
	CString m_OraFilePath;
	CString m_rIDMSKeyFormat;
	CEdit m_IDMSKeyFormatEdit;
	COraDatabaseDefFile* m_pOraDatabaseDefFile;
	CComboBox m_oraKeyComboBox;
	CGridCtrl m_ctrlMappingForInsertDataToPDS;
	CMFCButton m_btnNewMapping , m_btnDeleteMapping;	/// 2012.09.05 added by humkyung
	CButton m_wndPDSDataUpdateGroupCheckButton;			/// 2012.09.09 added by humkyung
// Dialog Data
	//{{AFX_DATA(COracleSettingDlg)
	enum { IDD = IDD_ORACLE_SETTING };
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COracleSettingDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	vector<MappingMap> m_MappingMapData , m_MappingMapDataForInsert;

	//CComPtr<ICorRuntimeHost>	m_pRuntimeHost;
	//CComPtr<_AppDomain>			m_pDefAppDomain;
protected:
	// Generated message map functions
	//{{AFX_MSG(COracleSettingDlg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonIdmskey();
	afx_msg void OnBnClickedButtonConnectionTest();
	afx_msg void OnBnClickedButtonDesignConnectionTest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckLoadingRule1();
	afx_msg void OnBnClickedCheckLoadingRule2();
	afx_msg void OnBnClickedNewMapping();
	afx_msg void OnBnClickedDeleteMapping();
	afx_msg void OnBnClickedUpdatePDSData();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ORACLESETTINGDLG_H__C3868156_3768_4756_8B5E_421AC1241369__INCLUDED_)
