// UserDLL.h : main header file for the USERDLL DLL
//

#if !defined(AFX_USERDLL_H__CD06F992_AA5A_430E_BDAC_5297659CB672__INCLUDED_)
#define AFX_USERDLL_H__CD06F992_AA5A_430E_BDAC_5297659CB672__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include <comdef.h>
#include <afxtempl.h>
/////////////////////////////////////////////////////////////////////////////
// CUserDLLApp
// See UserDLL.cpp for the implementation of this class
//

class CUserDLLApp : public CWinApp
{
public:
	CUserDLLApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserDLLApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CUserDLLApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
};

extern void (*pUpdateWorkStatusFunc)(CString& rTitle , int& process);
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERDLL_H__CD06F992_AA5A_430E_BDAC_5297659CB672__INCLUDED_)
