// ReservedDataFieldSet.h: interface for the CReservedDataFieldSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESERVEDDATAFIELDSET_H__C88F88D6_486D_421D_BF85_198BBB4B17FD__INCLUDED_)
#define AFX_RESERVEDDATAFIELDSET_H__C88F88D6_486D_421D_BF85_198BBB4B17FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ReservedDataField.h"

#include <vector>
using namespace std;

class CReservedDataFieldSet  
{
	CReservedDataFieldSet();
public:
	CReservedDataField* GetFieldAt(const size_t &at);
	size_t GetFieldCount();
	void Add(CReservedDataField *pReservedDataField);
	static void DeleteInstance(CReservedDataFieldSet *pReservedDataFieldSet);
	static CReservedDataFieldSet* CreateInstance();
	
	virtual ~CReservedDataFieldSet();
private:
	vector<CReservedDataField*>* m_pReservedDataFieldSet;
};

#endif // !defined(AFX_RESERVEDDATAFIELDSET_H__C88F88D6_486D_421D_BF85_198BBB4B17FD__INCLUDED_)
