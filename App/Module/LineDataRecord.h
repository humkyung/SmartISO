// LineDataRecord.h: interface for the CLineDataRecord class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINEDATARECORD_H__40FB7C92_E1C2_4CE1_AA21_259E1AC60183__INCLUDED_)
#define AFX_LINEDATARECORD_H__40FB7C92_E1C2_4CE1_AA21_259E1AC60183__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef MODULE_EXPORTS
	#define SMARTISO_MODULE_EXT_CLASS	__declspec(dllexport)
#else
	#define SMARTISO_MODULE_EXT_CLASS	__declspec(dllimport)
#endif

#define MATCH_ALL	-1

#include "DataFieldSet.h"
#include "ResDataRecord.h"
#include "RevDataRecordSet.h"

#include <vector>
#include <string>
using namespace std;

//class CCompareMap;
class SMARTISO_MODULE_EXT_CLASS CLineDataRecord  
{
	CLineDataRecord();
public:
	const char* GetLineDataModuleKey() const;
	CString GetSmartISOKey() const;
	CRevDataRecord* FindLatestRevDataRecord() const;
	void Copy(CLineDataRecord* rhs);
	static void DeleteInstance( const CLineDataRecord* pRecord );
	CString GetRecordString() const;
	CDataField* FindWithFieldName(LPCTSTR pFieldName) const;
	const char* GetAreaNo() const;
	CRevDataRecord* FindRevDataRecordWithRevNo(const char* pRevNo);
	CRevDataRecord* FindRevDataRecordWithIssueDate(const char* pIssueDate);
	CRevDataRecord* FindRevDataRecordWithRevNo(const CString& sRevNo);
	const char* GetPrimaryKey() const;
	const TCHAR* GetKey() const;
	bool CoincideWidth(const CLineDataRecord* p);
	//int Compare(CCompareMap* pCompareMap , CLineDataRecord* pLineDataRecord);
	CDataField* FindWithDesc(LPCTSTR desc) const;
	CLineDataRecord* Clone();
	void Set(const char* pFieldName , const char* pFieldValue);
	void AddRevDataRecordSet(CRevDataRecordSet *pRevDataRecordSet);
	void AddResDataRecord(CResDataRecord* pResDataRecord);
	CRevDataRecordSet* GetRevDataRecordSet();
	CResDataRecord* GetResDataRecord();
	bool IsDeleted() const;
	void Delete(const bool& flag);
	void Set(CDataFieldSet* const pLineDataFieldSet);
	void SetIndex(const CString& index);
	CString index() const;
	void Add(const char* pFieldValue);
	static CLineDataRecord* CreateInstance();
	CDataField* GetFieldAt(const size_t& at) const;
	size_t GetFieldCount() const;
	
	virtual ~CLineDataRecord();
private:
	CString m_sIndex;
	bool m_bDeleted;
	CDataFieldSet* m_pLineDataFieldSet;

	CResDataRecord* m_pResDataRecord;
	CRevDataRecordSet*   m_pRevDataRecordSet;
};

#endif // !defined(AFX_LINEDATARECORD_H__40FB7C92_E1C2_4CE1_AA21_259E1AC60183__INCLUDED_)
