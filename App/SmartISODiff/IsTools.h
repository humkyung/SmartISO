#pragma once
#include <math.h>

#ifndef SAFE_DELETE 
#define SAFE_DELETE(p)  { if(p) { delete (p); (p)=NULL; } } 
#endif // SAFE_DELETE 

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)  { if(p) { free (p); (p)=NULL; } } 
#endif // SAFE_RELEASE

#ifndef	SIZE_OF_ARRAY
#define	SIZE_OF_ARRAY(array) (sizeof(array)/sizeof(array[0]))
#endif

#ifndef SAFE_ROUND
#define	SAFE_ROUND(n , digits) ((floor(((n) * ::pow(10.,digits)) + 0.5))/::pow(10.,digits))
#endif

#include <string>
#include <sstream>
using namespace std;

#if (defined(_UNICODE) || defined(UNICODE))
#define	IS_T(s)			L ## s
#define	CHAR_T			TCHAR
#define	STRING_T			wstring
#define	STRLEN_T 			(int)wcslen
#define	STRCMP_T			wcscmp
#define	STRICMP_T			wcsicmp
#define	STRCPY_T			wcscpy
#define	STRNCPY_T			wcsncpy
#define	STRCHR_T			wcschr
#define	FOPEN_T(n,m) 		_wfopen(n,m)
#define	ATOI_T			_wtoi
#define	ATOL_T			_wtol
#define	ATOF_T			_wtof
#define	SPRINTF_T			swprintf
#define	SSCANF_T			swscanf
#define	OSTRINGSTREAM_T	wostringstream
#define	STRINGSTREAM_T		wstringstream
#define	IFSTREAM_T		wifstream
#define	OFSTREAM_T		wofstream
#else
#define	IS_T(s)			s
#define	CHAR_T			char
#define	STRING_T			string
#define	STRLEN_T 			(int)strlen
#define	STRCMP_T			strcmp
#define	STRICMP_T			stricmp
#define	STRCPY_T			strcpy
#define	STRNCPY_T			strncpy
#define	SPRINTF_T			sprintf
#define	STRCHR_T			strchr
#define	FOPEN_T(n,m) 		fopen(n,m)
#define	ATOI_T			atoi
#define	ATOL_T			atol
#define	ATOF_T			atof
#define	SPRINTF_T			sprintf
#define	SSCANF_T			sscanf
#define	OSTRINGSTREAM_T	ostringstream
#define	STRINGSTREAM_T		stringstream
#define	IFSTREAM_T		ifstream
#define	OFSTREAM_T		ofstream
#endif
