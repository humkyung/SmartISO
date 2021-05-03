// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__B2FF1947_FE77_4268_974B_DD66656A0B3A__INCLUDED_)
#define AFX_STDAFX_H__B2FF1947_FE77_4268_974B_DD66656A0B3A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC의 리본 및 컨트롤 막대 지원

#include <afxdisp.h>
#include <atlbase.h>
#include <mscoree.h>
#include <comutil.h>

#include <IsTools.h>
#include "ResourceSwitch.h"

///#include <BugTrap.h>
#include "AdminDoc.h"

#define	PRODUCT_NAME	_T("SmartISO")

#ifdef _UNICODE
/// #pragma comment(lib, "BugTrapU.lib") // Link to Unicode DLL
#pragma comment(lib, "Htmlhelp.Lib")
#else
/// #pragma comment(lib, "BugTrap.lib") // Link to ANSI DLL
#pragma comment(lib, "Htmlhelp.Lib")
#endif

#pragma comment(linker, "\"/manifestdependency:type='win32'\
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"") 

#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF", "adoEOF")
#import "..\bin\SmartISOLayer.tlb" no_namespace, raw_interfaces_only

#pragma comment(lib,"mscoree.lib ")

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
extern COLORREF DarkenColor(COLORREF col,double factor);
extern void PrintError(const DWORD& dwErrorNo);
extern CAdminDoc* GetSDIActiveDocument();
extern BOOL ExecuteCommandLine(CString cmdLine, DWORD & exitCode);
extern CString _TR(const CString&);
#endif // !defined(AFX_STDAFX_H__B2FF1947_FE77_4268_974B_DD66656A0B3A__INCLUDED_)
