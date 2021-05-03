// StringHelper.h: interface for the CStringHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRINGHELPER_H__C77908AD_4693_490C_A5DA_08A25665BBE5__INCLUDED_)
#define AFX_STRINGHELPER_H__C77908AD_4693_490C_A5DA_08A25665BBE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include < map >
using namespace std;

class CStringHelper  
{
	CStringHelper();
public:
	const char* GetHelpString(const UINT& nID);
	static CStringHelper& GetInstance();
	virtual ~CStringHelper();
private:
	map< UINT , CString > m_stringMap;
};

#endif // !defined(AFX_STRINGHELPER_H__C77908AD_4693_490C_A5DA_08A25665BBE5__INCLUDED_)
