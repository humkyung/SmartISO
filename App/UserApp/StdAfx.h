// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__FF9D7BA1_6013_4871_B7D6_BDE1EB5E6590__INCLUDED_)
#define AFX_STDAFX_H__FF9D7BA1_6013_4871_B7D6_BDE1EB5E6590__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#include <LineDataRecord.h>
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC의 리본 및 컨트롤 막대 지원
#include <afxsock.h>			/// for CSocket

#include <afxdisp.h>
#include <atlbase.h>
#include <mscoree.h>
#include <comutil.h>

#include <IsTools.h>

/// #include <BugTrap.h>

#ifdef _UNICODE
/// #pragma comment(lib, "BugTrapU.lib") // Link to Unicode DLL
#pragma comment(lib, "Htmlhelp.Lib")
#else
/// #pragma comment(lib, "BugTrap.lib") // Link to ANSI DLL
#pragma comment(lib, "Htmlhelp.Lib")
#endif
#pragma comment(lib,"mscoree.lib ")

#include "ScintillaWnd.h"

#include "StringHelper.h"
#include <IsRegExp.h>
#include "UserAppDoc.h"

#define	PRODUCT_NAME	_T("SmartISO")
#define	PUBLISHER		_T("TechSun")

#define	NOR_OPER_PRES	_T("NOR_OPER_PRES")
#define	NOR_OPER_TEMP	_T("NOR_OPER_TEMP")
#define	NOR_DGN_PRES	_T("NOR_DGN_PRES")
#define	NOR_DGN_TEMP	_T("NOR_DGN_TEMP")

#define	FIELD_OPER_PRES	_T("OPER_PRES")
#define	FIELD_OPER_TEMP	_T("OPER_TEMP")
#define	FIELD_DGN_PRES	_T("DGN_PRES")
#define	FIELD_DGN_TEMP	_T("DGN_TEMP")

#define	PROVIDER	_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=")	//! 2011.02.07 - added by HumKyung
#define	DBPASSWORD	_T(";Jet OLEDB:Database Password=gopds")	/// 2012.08.31 added by humkyung
#define	LICENDPOINT			"http://www.solutionware.co.kr/LicWebService/LicWebService.asmx"

typedef struct
{
	int nItem;
	CLineDataRecord* pLineDataRecord;
}LineDataRecordMap;

typedef enum
{
	SELECT			= WM_USER + 120,
	SELECT_ALL		= WM_USER + 121,
	SELECT_ISSUED	= WM_USER + 122
}SelectOptionMsg;

struct SScintillaColors
{	int		iItem;
	COLORREF	rgb;
};

extern SScintillaColors g_rgbSyntaxCpp[];

#pragma comment(linker, "\"/manifestdependency:type='win32'\
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"") 
#import "..\bin\SmartISOLayer.tlb" no_namespace, raw_interfaces_only

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
extern int (* SmartISODiff)(const LPCTSTR ,const LPCTSTR , const LPCTSTR);	/// 2012.01.02 added by humkyung

extern bool __load_user_defined_table;
extern CUserAppDoc* GetSDIActiveDocument();
extern BOOL CompareTwoTimes(const CString& rCurrentTime , const CString rScheduleTime);
extern COLORREF DarkenColor(COLORREF col,double factor);
extern void TrimRightZeroString( CString& rString );
extern int FindStringHelper (LPCTSTR pszFindWhere, LPCTSTR pszFindWhat, DWORD dwFlags, int &nLen, IsRegExp::RxNode *&rxnode, IsRegExp::RxMatchRes *rxmatch);
extern BOOL ExecuteCommandLine(CString cmdLine, DWORD & exitCode);
extern CString _TR(const CString&);
#endif // !defined(AFX_STDAFX_H__FF9D7BA1_6013_4871_B7D6_BDE1EB5E6590__INCLUDED_)
