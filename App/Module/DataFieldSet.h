// DataFieldSet.h: interface for the CDataFieldSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAFIELDSET_H__B0ECED93_83C3_4B76_A723_95436530CF8B__INCLUDED_)
#define AFX_DATAFIELDSET_H__B0ECED93_83C3_4B76_A723_95436530CF8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DataField.h"
#include <vector>

class SMARTISO_MODULE_EXT_CLASS CDataFieldSet  
{
	CDataFieldSet();
public:
	void Copy(CDataFieldSet* rhs);
	CDataField* FindWithFieldName(LPCTSTR pFieldName);
	bool CoincideWith(CDataFieldSet* p);
	CDataFieldSet* Clone();
	void Clear();
	static void DeleteInstance(CDataFieldSet* pLineDataFieldSet);
	static CDataFieldSet* CreateInstance();
	CDataField* GetFieldAt(const size_t& at);
	size_t GetFieldCount();
	void Add(CDataField* pDataField);
	
	virtual ~CDataFieldSet();
private:
	vector<CDataField*>* m_pDataFieldSet;

	friend class CLineDataRecord;
};

#endif // !defined(AFX_DATAFIELDSET_H__B0ECED93_83C3_4B76_A723_95436530CF8B__INCLUDED_)
