#if !defined(AFX_BMREPORTSETTINGPAGE_H__96980D62_449F_4350_B6A8_D2D7E77855E1__INCLUDED_)
#define AFX_BMREPORTSETTINGPAGE_H__96980D62_449F_4350_B6A8_D2D7E77855E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BMReportSettingPage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBMReportSettingPage dialog

class CBMReportSettingPage : public CPropertyPage
{
public:
	static void LoadData( const CString& rIniFilePath );
	void SaveData( const CString& rIniFilePath );
// Construction
public:
	static double GetPipeCutLength();
	CBMReportSettingPage();
	~CBMReportSettingPage();

	UINT GetIconID() {return IDI_BM_REPORT;}
// Dialog Data
	//{{AFX_DATA(CBMReportSettingPage)
	enum { IDD = 0};///IDD_BM_REPORT_SETTING };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	BOOL m_bETC;
	CWnd m_wndPipeCutLengthEdit;
	//}}AFX_DATA
	static CString APP;
// Overrides
// Implementation
protected:
	static CString m_rPipeCutLength;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BMREPORTSETTINGPAGE_H__96980D62_449F_4350_B6A8_D2D7E77855E1__INCLUDED_)
