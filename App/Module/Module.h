// Module.h : main header file for the MODULE DLL
//

#if !defined(AFX_MODULE_H__CB360571_B9D8_4BBC_8A27_3EB662641E1C__INCLUDED_)
#define AFX_MODULE_H__CB360571_B9D8_4BBC_8A27_3EB662641E1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
///#include "ADO/Dyndb.h"
/////////////////////////////////////////////////////////////////////////////
// CModuleApp
// See Module.cpp for the implementation of this class
//

class CModuleApp : public CWinApp
{
public:
	CModuleApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModuleApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CModuleApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
};

extern CModuleApp theApp;
extern void (*pUpdateWorkStatusFunc)(CString& rTitle , int& process);
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODULE_H__CB360571_B9D8_4BBC_8A27_3EB662641E1C__INCLUDED_)
