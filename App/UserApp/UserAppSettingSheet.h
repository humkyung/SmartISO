#if !defined(AFX_USERAPPSETTINGSHEET_H__A882CE14_D58A_4C66_BA5A_C268E63AD566__INCLUDED_)
#define AFX_USERAPPSETTINGSHEET_H__A882CE14_D58A_4C66_BA5A_C268E63AD566__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserAppSettingSheet.h : header file
//
#include <TreePropSheet/TreePropSheet.h>
using namespace TreePropSheet;

///#include "UserPropertyPage.h"
///#include "UserAppSettingPage.h"
#include "IsoEditSettingPage.h"
#include "BMReportSettingPage.h"
///#include "OracleSettingPage.h"
/////////////////////////////////////////////////////////////////////////////
// CUserAppSettingSheet
class CUser;
class CUserAppSettingSheet : public CTreePropSheet
{
	DECLARE_DYNAMIC(CUserAppSettingSheet)

// Construction
public:
	CUserAppSettingSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
private:
	/**
	 * Adds all pages to this Settings-Dialog.
	 */
	void AddPropPages();
	/**
	 * Removes the pages and frees up memory.
	 */
	void RemovePropPages();
// Attributes
public:
	CString m_rIniFilePath;
	//CUser* m_pSelUser;
private:
	///CUserPropertyPage*  m_pUserPropertyPage;
	///CUserAppSettingPage* m_pUserAppSettingPage;
	CIsoEditSettingPage* m_pIsoEditSettingPage;
	///CBMReportSettingPage* m_pBMReportSettingPage;
	///auto_ptr<COracleSettingPage> m_oracleSettingPage;

	HICON m_hIcon;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserAppSettingSheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SaveData();
	virtual ~CUserAppSettingSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CUserAppSettingSheet)
		// NOTE - the ClassWizard will add and remove member functions here.
	virtual void OnOK(void);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERAPPSETTINGSHEET_H__A882CE14_D58A_4C66_BA5A_C268E63AD566__INCLUDED_)
