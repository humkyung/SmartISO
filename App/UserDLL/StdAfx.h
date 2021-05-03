// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__4C6402F5_77E3_4B85_AB7A_775C9CBC97E3__INCLUDED_)
#define AFX_STDAFX_H__4C6402F5_77E3_4B85_AB7A_775C9CBC97E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
///#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
//#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <Util/IsUtilInf.h>

#include <IsTools.h>
#include <IsRegExp.h>
#define	DBPASSWORD	_T(";Jet OLEDB:Database Password=gopds")	/// 2012.08.31 added by humkyung

#define CHECK_ERROR_COUNT		14

extern int FindStringHelper (LPCTSTR pszFindWhere, LPCTSTR pszFindWhat, DWORD dwFlags, int &nLen, IsRegExp::RxNode *&rxnode, IsRegExp::RxMatchRes *rxmatch);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__4C6402F5_77E3_4B85_AB7A_775C9CBC97E3__INCLUDED_)
