// RevDataRecordSet.h: interface for the CRevDataRecordSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REVDATARECORDSET_H__6B60BE1E_371F_41B9_9D1D_622264B1CCD7__INCLUDED_)
#define AFX_REVDATARECORDSET_H__6B60BE1E_371F_41B9_9D1D_622264B1CCD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RevDataRecord.h"

#include <vector>
using namespace std;

class SMARTISO_MODULE_EXT_CLASS CRevDataRecordSet  
{
	CRevDataRecordSet();
public:
	int DeleteRecord(CRevDataRecord* pRecord);
	CRevDataRecord* FindLatestRevDataRecord() const;
	CRevDataRecordSet* Clone();
	void Copy(CRevDataRecordSet* rhs);
	CRevDataRecord* FindWithRevNo(const char* pRevNo);
	CRevDataRecord* FindWithIssueDate(const char* pIssueDate);
	CRevDataRecord* Find(const long& index);
	bool IsLastDate(const TCHAR* p);
	void SaveToDatabase(CADODB* pAdoDb , const CString& ref);
	CRevDataRecord* GetRecordAt(const size_t& at);
	size_t GetRecordCount() const;
	void Add(CRevDataRecord* pRecord);
	
	static CRevDataRecordSet* CreateInstance();
	virtual ~CRevDataRecordSet();
private:
	void Clear();
	vector<CRevDataRecord*>* m_pRevDataRecordEntry;

	friend class CActiveUser;
};

#endif // !defined(AFX_REVDATARECORDSET_H__6B60BE1E_371F_41B9_9D1D_622264B1CCD7__INCLUDED_)
