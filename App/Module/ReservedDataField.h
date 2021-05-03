// ReservedDataField.h: interface for the CReservedDataField class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESERVEDDATAFIELD_H__24DF79A6_E9E0_41CF_B1A0_020F6AAF6BD7__INCLUDED_)
#define AFX_RESERVEDDATAFIELD_H__24DF79A6_E9E0_41CF_B1A0_020F6AAF6BD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef MODULE_EXPORTS
	#define IDMS_MODULE_EXT_CLASS	__declspec(dllexport)
#else
	#define IDMS_MODULE_EXT_CLASS	__declspec(dllimport)
#endif

#include <string>
using namespace std;

class IDMS_MODULE_EXT_CLASS CReservedDataField  
{
	CReservedDataField();
public:
	const char* desc() const;
	void Set(const char* name , const char* desc , const char *value);
	const char* value() const;
	const char* name() const;

	static CReservedDataField* CreateInstance();
	virtual ~CReservedDataField();
private:
	string* m_name;
	string* m_value;
	string* m_desc;
};

#endif // !defined(AFX_RESERVEDDATAFIELD_H__24DF79A6_E9E0_41CF_B1A0_020F6AAF6BD7__INCLUDED_)
