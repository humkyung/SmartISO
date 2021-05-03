// ProjectAccess.h: interface for the CProjectAccess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROJECTACCESS_H__28EF4246_60D0_482C_98B6_601727A284CE__INCLUDED_)
#define AFX_PROJECTACCESS_H__28EF4246_60D0_482C_98B6_601727A284CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef MODULE_EXPORTS
	#define SMARTISO_MODULE_EXT_CLASS	__declspec(dllexport)
#else
	#define SMARTISO_MODULE_EXT_CLASS	__declspec(dllimport)
#endif

#include <string>
using namespace std;

class SMARTISO_MODULE_EXT_CLASS CProjectAccess  
{
	CProjectAccess();
public:
	const char* comment() const;
	static CProjectAccess* CreateInstance();
	void Set(const char* id , const char* date , const char* comments);
	const char* comments();
	const char* date();
	const char* user_id();
	CProjectAccess(const char* id , const char* date , const char* comments);
	
	virtual ~CProjectAccess();
public:
	long index() const;
	long m_index;
private:
	string* m_userid;
	string* m_date;
	string* m_comments;
};

#endif // !defined(AFX_PROJECTACCESS_H__28EF4246_60D0_482C_98B6_601727A284CE__INCLUDED_)
