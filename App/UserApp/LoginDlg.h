// IDMSDlg.h : header file
//

#if !defined(AFX_IDMSDLG_H__546FAFD0_0093_42CE_8C8D_340FE6D9F3C7__INCLUDED_)
#define AFX_IDMSDLG_H__546FAFD0_0093_42CE_8C8D_340FE6D9F3C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <BitmapDialog.h>
#include <FilterEdit/FilterEdit.h>
#include <ReportCtrl/ReportCtrl.h>
#include <BtnST.h>
#include <GradientStatic.h>
#include "BitmapComboBox.h"
#include "MFCButtonEx.h"
/// #include "Socket/ClientSocket.h"
#include "ProjectDlg.h"

#include <exception>
#include <vector>
#include <string>
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CLoginDlg dialog

typedef struct
{
	STRING_T name;
	STRING_T path;
}SystemDbInfo;

class CLoginDlg : public CDialog
{
// Construction
public:
	CLoginDlg(CWnd* pParent = NULL);	// standard constructor
	~CLoginDlg();

// Dialog Data
	//{{AFX_DATA(CLoginDlg)
	enum { IDD = IDD_LOGIN };
		// NOTE: the ClassWizard will add data members here
	CGradientStatic m_wndStaticTitle;

	CBitmapComboBox m_wndProjectCombo;
	CFilterEdit m_PasswordEdit;
	CComboBox m_UserCombo , m_cboModule;
	CGradientStatic m_wndStaticMsg;
	CButton m_okButton;
	CButton m_cancelButton;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLoginDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	CFont* m_pBoldFont;	//! 2011.08.06 added by humkyung

	HICON m_hIcon;
	vector<CBitmap*> m_BitmapList;

	// Generated message map functions
	//{{AFX_MSG(CLoginDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	virtual void OnOK();
	afx_msg void OnSelchangeComboProject();
	afx_msg void OnSelendokComboUser();
	afx_msg void OnButtonProjectSetup();
	afx_msg int OnReceiveMsg();  /// 2013.11.16 added by humkyung
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CString m_sId , m_sPassword , m_sSite , m_sIniFilePath;
private:
	//int FillSiteList(const STRING_T& name);				/// 2012.08.12 added by humkyung
	//STRING_T GetAdminPasswordFrom(const STRING_T& name);	/// 2012.01.27 added by humkyung

	BOOL LoadBMPImage( LPCTSTR sBMPFile, CBitmap* bitmap, CPalette *pPal );
	void DisplayProjectInfo();
	void SaveAppSettingFile();
	void LoadAppSettingFile();

	CString m_sAppIniFilePath, m_sDatabaseFilePath;
	map<CString , list<CProjectDlg::ProjectInfo>*> m_ProjectMap;
	vector<SystemDbInfo*> m_SystemDbInfos;
///	static CClientSocket m_oClientSocket;	/// 2013.11.17 added by humkyung
public:
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonResetActivateCode();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IDMSDLG_H__546FAFD0_0093_42CE_8C8D_340FE6D9F3C7__INCLUDED_)
