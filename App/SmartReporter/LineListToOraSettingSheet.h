#if !defined(AFX_LINELISTTOORASETTINGSHEET_H__A882CE14_D58A_4C66_BA5A_C268E63AD566__INCLUDED_)
#define AFX_LINELISTTOORASETTINGSHEET_H__A882CE14_D58A_4C66_BA5A_C268E63AD566__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserAppSettingSheet.h : header file
//
#include <TreePropSheet/TreePropSheet.h>
using namespace TreePropSheet;

///#include "AdminPropertyPage.h"
#include "CustomReportPropertyPage.h"
/////////////////////////////////////////////////////////////////////////////
// CLineListToOraSettingSheet
class CUser;
class CLineListToOraSettingSheet : public CTreePropSheet
{
	DECLARE_DYNAMIC(CLineListToOraSettingSheet)

// Construction
public:
	CLineListToOraSettingSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
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
	CUser* m_pSelUser;
private:
	///CAdminPropertyPage*  m_pAdminPropertyPage;
	CCustomReportPropertyPage* m_pCustomReportPropertyPage;

	HICON m_hIcon;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLineListToOraSettingSheet)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SaveData();
	virtual ~CLineListToOraSettingSheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLineListToOraSettingSheet)
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

#endif // !defined(AFX_LINELISTTOORASETTINGSHEET_H__A882CE14_D58A_4C66_BA5A_C268E63AD566__INCLUDED_)
