// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <DgnV7Lib.h>
#include "IsRegExp.h"
#include <vector>

#define	MIN_VERTEX_COUNT_FOR_FLOW_MARK	25

#ifndef PI  
	#define PI  3.1415926535897932384626433832795
#endif

extern CString __INI_FILE_PATH__;
extern int FindStringHelper (LPCTSTR pszFindWhere, LPCTSTR pszFindWhat, DWORD dwFlags, int &nLen, IsRegExp::RxNode *&rxnode, IsRegExp::RxMatchRes *rxmatch);
extern double DegreeBetween(DPoint3d pt1 , DPoint3d pt2);
extern bool IsParallelAngle(const double& angle1 , const double& angle2 , const double& toler);
extern double DistanceFromPointToLine( DPoint3d pt , DPoint3d l[2]);

extern void NormalizeVector(DVec3d& vec);
extern double DistanceBetween(const DPoint3d &pt1 , const DPoint3d &pt2);
extern int IntersectWith(DPoint3d &intsec, const DVector3d& l1 , const DVector3d& l2);
extern double AngleOf(const DVec3d& vec);
extern double AngleBetweenVectors(const DVec3d& vec1 , const DVec3d& vec2);

extern int DrawCloud(DPoint3d *ptCloud , int nLineCount , double fArcMin, double fArcMax , 
			   UInt32 nLayer , UInt32 nColor, int nStyle , UInt32 nWeight , float fScale, CDgnDocument* pDgnDoc);
extern int Draw_RevNo(DPoint3d *ptStart , CString RevNo , UInt32 nLayer , UInt32 nColor, int nStyle , UInt32 nWeight , double dHeight , double dWidth , CDgnDocument* pDgnDoc);
extern void DrawPolyline(CDgnDocument* pDgnDoc , vector<DPoint3d> oPolygon , CString sColor);
extern void DrawCircle(CDgnDocument* pDgnDoc , DPoint3d center , double dRadius , UInt32 uiColor);