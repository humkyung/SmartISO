// LineDataRecordSet.h: interface for the CLineDataRecordSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINEDATARECORDSET_H__D9623F9C_1B5D_4755_87BF_64F89FD18654__INCLUDED_)
#define AFX_LINEDATARECORDSET_H__D9623F9C_1B5D_4755_87BF_64F89FD18654__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineDataRecord.h"

class SMARTISO_MODULE_EXT_CLASS CLineDataRecordSet  
{
	CLineDataRecordSet();
public:
	bool IsEmpty() const;
	bool CheckDuplicatedSmartISOKey(const CString& index , const TCHAR *pSmartISOKey);
	bool CheckDuplicatedPrimaryKey(const CString& index , const char* pPrimaryKey);
	CLineDataRecord* Find(const CString& sIndex);
	bool ExistRecordHasSamePrimaryKey(CLineDataRecord* p);
	void Add(CLineDataRecord* pLineListRecord);
	static void DeleteInstance(CLineDataRecordSet* pInst);
	static CLineDataRecordSet* CreateInstance();
	CLineDataRecord* GetLineDataRecordAt(const size_t& at);
	size_t GetLineDataRecordCount() const;
	void RemoveContents();

	virtual ~CLineDataRecordSet();
private:
	vector<CLineDataRecord*>* m_pLineDataRecordEntry;

	friend class CProject;
};

#endif // !defined(AFX_LINEDATARECORDSET_H__D9623F9C_1B5D_4755_87BF_64F89FD18654__INCLUDED_)
