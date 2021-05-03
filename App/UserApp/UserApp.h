
// UserApp.h : main header file for the UserApp application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "StdAfx.h"

#include "resource.h"       // main symbols
#include "singleinstance.h"
#include "EnumPrinters.h"
#include <comdef.h>
#include <afxtempl.h>

#import <mscorlib.tlb> auto_rename raw_interfaces_only
///using namespace mscorlib;

// CUserAppApp:
// See UserApp.cpp for the implementation of this class
//

class CUserAppApp : public CWinAppEx , public CSingleInstance
{
public:
	CUserAppApp();

	void ReadPrinterList();
	CString GetDefaultPrinterEx();

	// @cmember the version number of the Scintilla dll
	CString m_strVersionScintilla;
	// @cmember the filename of the Scintilla dll
	CString m_strFileScintilla;
	// @cmember the product
	CString m_strProductScintilla;
	HINSTANCE   m_hDll;
// Overrides
public:
	HRESULT CallManagedFunction(BSTR szMethodName, int iNoOfParams, VARIANT * pvArgs, VARIANT * pvRet);

	virtual BOOL InitInstance();
	virtual int ExitInstance();

	CEnumPrinters m_PrinterControl;
// Implementation
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	afx_msg void OnAppClose();
	afx_msg void OnOptionSetting();
	afx_msg void OnFileSave();
	afx_msg void OnSettingAdmin();
	afx_msg void OnIssueManager();
	afx_msg void OnSmartReporter();						/// 2012.05.03 added by humkyung
	afx_msg void OnIsoCreator();						/// 2012.05.18 added by humkyung
	afx_msg void OnUpdateSettingAdmin(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIssueManager(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSmartReporter(CCmdUI* pCmdUI);	/// 2012.05.03 added by humkyung
	afx_msg void OnUpdateIsoCreator(CCmdUI* pCmdUI);	/// 2012.05.18 added by humkyung
	DECLARE_MESSAGE_MAP()
private:
	HRESULT StartupRuntimeHost(TCHAR* szAsseblyName, TCHAR* szClassNameWithNamespace);	/// 2012.08.29 added by humkyung
	int LoadPlugins(void);
public:
	CComPtr<ICorRuntimeHost>			m_pRuntimeHost;
	CComPtr<mscorlib::_AppDomain>		m_pDefAppDomain;
	CComPtr<mscorlib::_ObjectHandle>	m_pObjectHandle;
};

extern CUserAppApp theApp;
