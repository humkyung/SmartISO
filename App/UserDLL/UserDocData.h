// UserDocData.h: interface for the CUserDocData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERDOCDATA_H__645ED94D_2AC4_421E_A285_BFAE3E6C1EB2__INCLUDED_)
#define AFX_USERDOCDATA_H__645ED94D_2AC4_421E_A285_BFAE3E6C1EB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef USER_EXPORTS
	#ifdef _UNICODE
		#ifdef _DEBUG
			#if _MSC_VER == 1600
				#pragma comment(lib, "UserDLL_vc100_unicode_debug")
				#pragma message("Automatically linking with UserDLL_vc100_unicode_debug.dll (Debug Unicode)")
			#else
				#pragma comment(lib, "UserDLL_vc90_unicode_debug")
				#pragma message("Automatically linking with UserDLL_vc90_unicode_debug.dll (Debug Unicode)")
			#endif
		#else
			#if _MSC_VER == 1600
				#pragma comment(lib, "UserDLL_vc100_unicode")
				#pragma message("Automatically linking with UserDLL_vc100_unicode.dll (Release Unicode)")
			#else
				#pragma comment(lib, "UserDLL_vc90_unicode")
				#pragma message("Automatically linking with UserDLL_vc90_unicode.dll (Release Unicode)")
			#endif
		#endif
	#else
		#ifdef _DEBUG
			#if _MSC_VER == 1600
				#pragma comment(lib, "UserDLL_vc100_debug")
				#pragma message("Automatically linking with UserDLL_vc100_debug.dll (Debug)")
			#else
				#pragma comment(lib, "UserDLL_vc90_debug")
				#pragma message("Automatically linking with UserDLL_vc90_debug.dll (Debug)")
			#endif
		#else
			#if _MSC_VER == 1600
				#pragma comment(lib, "UserDLL_vc100")
				#pragma message("Automatically linking with UserDLL_vc100.dll (Release)")
			#else
				#pragma comment(lib, "UserDLL_vc90")
				#pragma message("Automatically linking with UserDLL_vc90.dll (Release)")
			#endif
		#endif
	#endif // _UNICODE
#endif // USER_EXPORTS

#ifdef	USER_EXPORTS
#define	USER_EXT_CLASS	__declspec(dllexport)
#else
#define	USER_EXT_CLASS	__declspec(dllimport)
#endif

#include <IsTools.h>
#include <util\IsUtilInf.h>

#include <map>
using namespace std;

class CUserDocData  
{
	CUserDocData();
public:
	static CUserDocData& GetInstance();
	virtual ~CUserDocData();

	STRING_T FindWeldTypeOf(const STRING_T& code , const STRING_T& sWeldTypeFilePath);	/// 2012.08.20 added by humkyung
private:
	int LoadWeldTypeMap(const STRING_T& sWeldTypeFilePath);	/// 2012.08.20 added by humkyung
private:
	map<STRING_T,STRING_T>* m_pWeldTypeMap;
};

#endif // !defined(AFX_USERDOCDATA_H__645ED94D_2AC4_421E_A285_BFAE3E6C1EB2__INCLUDED_)
