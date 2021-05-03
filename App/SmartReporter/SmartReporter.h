
// SmartReporter.h : main header file for the SmartReporter application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "singleinstance.h"
#include "resource.h"       // main symbols


// CSmartReporterApp:
// See SmartReporter.cpp for the implementation of this class
//

class CSmartReporterApp : public CWinAppEx , public CSingleInstance
{
public:
	CSmartReporterApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	///virtual void PreLoadState();
	///virtual void LoadCustomState();
	///virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	afx_msg void OnFileSetting();
	afx_msg void OnUpdateViewOracleDataReport(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
private:
};

extern CSmartReporterApp theApp;
