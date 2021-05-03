#pragma once

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IsoFileSettingPage.h : header file
//
#include <FilterEdit/FilterEdit.h>

#include "Resource.h"
/////////////////////////////////////////////////////////////////////////////
// CIsoEditSettingPage dialog

class CIsoEditSettingPage : public CPropertyPage
{
	DECLARE_DYNCREATE(CIsoEditSettingPage)

// Construction
public:
	void SaveData( const CString& rIniFilePath);
	static void LoadData( const CString& rIniFilePath);
	CIsoEditSettingPage();
	~CIsoEditSettingPage();

	UINT GetIconID() {return IDD_ISO_FILE_SETTING;}
	static CString APP;
// Dialog Data
	//{{AFX_DATA(CIsoEditSettingPage)
	enum { IDD = IDD_ISO_FILE_SETTING };
		// NOTE - ClassWizard will add data members here.
		static CString m_rFileNameExt;
		CFilterEdit m_fileExtFilterEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CIsoEditSettingPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CIsoEditSettingPage)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
