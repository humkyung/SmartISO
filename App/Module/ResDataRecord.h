// ResDataRecord.h: interface for the CResDataRecord class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESDATARECORD_H__212CADA5_8E9C_4422_999E_5397328C600B__INCLUDED_)
#define AFX_RESDATARECORD_H__212CADA5_8E9C_4422_999E_5397328C600B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DataFieldSet.h"

class SMARTISO_MODULE_EXT_CLASS CResDataRecord  
{
	CResDataRecord();
public:
	CResDataRecord* Clone();
	void Copy(CResDataRecord* rhs);
	CDataField* Find(const char* desc);
	CString index() const;
	void SetIndex(const CString& sIndex);
	CDataField* GetFieldAt(const size_t &at);
	size_t GetFieldCount() const;
	void Set( CDataFieldSet* const pDataFieldSet );
	static void DeleteInstance(CResDataRecord* pInst);
	static CResDataRecord* CreateInstance();
	
	virtual ~CResDataRecord();
private:
	CString m_sIndex;
	CDataFieldSet* m_pDataFieldSet;
};

#endif // !defined(AFX_RESDATARECORD_H__212CADA5_8E9C_4422_999E_5397328C600B__INCLUDED_)
