// IssueDateRecordSet.h: interface for the CIssueDateRecordSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISSUEDATERECORDSET_H__D08B1A67_4B89_4D84_B1FD_F6ABB1A9B9D5__INCLUDED_)
#define AFX_ISSUEDATERECORDSET_H__D08B1A67_4B89_4D84_B1FD_F6ABB1A9B9D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IssueDateRecord.h"
#include <vector>
class SMARTISO_MODULE_EXT_CLASS CIssueDateRecordSet  
{
public:
	void RemoveDeletedRecords();
	void DeleteRecord(const int& nIDX);
	bool LessOrEqual(const TCHAR* p);
	CIssueDateRecord* GetAt(const size_t& at);
	size_t GetCount() const;
	void Add(const long& index , LPCTSTR pDateString);
	void Add(CIssueDateRecord* p);
	CIssueDateRecordSet();
	virtual ~CIssueDateRecordSet();
private:
	vector<CIssueDateRecord*>* m_pIssueDateEntry;
};

#endif // !defined(AFX_ISSUEDATERECORDSET_H__D08B1A67_4B89_4D84_B1FD_F6ABB1A9B9D5__INCLUDED_)
