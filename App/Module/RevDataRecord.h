// RevDataRecord.h: interface for the CRevDataRecord class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REVDATARECORD_H__1E27B277_6711_4F8A_9D61_9F644CE86A9B__INCLUDED_)
#define AFX_REVDATARECORD_H__1E27B277_6711_4F8A_9D61_9F644CE86A9B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <ado/ADODB.h>
#include "DataFieldSet.h"

class SMARTISO_MODULE_EXT_CLASS CRevDataRecord  
{
	CRevDataRecord();
public:
	void Copy(CRevDataRecord* rhs);
	CDataField* FindWithName(const TCHAR *name);
	CRevDataRecord* Clone();
	const TCHAR* GetIssueDate() const;
	const char* GetRevNo() const;
	CDataField* Find(LPCTSTR desc);
	void SetIndex(const CString& index);
	CString index() const;
	CDataField* GetFieldAt(const size_t& at);
	size_t GetFieldCount() const;;
	void Set( CDataFieldSet* const pDataFieldSet );
	
	static void DeleteInstance(CRevDataRecord* pInst);
	static CRevDataRecord* CreateInstance();
	virtual ~CRevDataRecord();
private:
	void SaveToDatabase(CADODB* pAdoDb , const CString& ref);
	CString m_sIndex;
	CDataFieldSet* m_pRevDataFieldSet;

	friend class CRevDataRecordSet;
};

#endif // !defined(AFX_REVDATARECORD_H__1E27B277_6711_4F8A_9D61_9F644CE86A9B__INCLUDED_)
