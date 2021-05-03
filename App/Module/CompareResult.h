// CompareResult.h: interface for the CCompareResult class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPARERESULT_H__82684772_543D_48BE_B212_26C4C0EAC053__INCLUDED_)
#define AFX_COMPARERESULT_H__82684772_543D_48BE_B212_26C4C0EAC053__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef MODULE_EXPORTS
	#define SMARTISO_MODULE_EXT_CLASS	__declspec(dllexport)
#else
	#define SMARTISO_MODULE_EXT_CLASS	__declspec(dllimport)
#endif

#include "LineDataRecord.h"

class SMARTISO_MODULE_EXT_CLASS CCompareMap
{
	CCompareMap(const int& match , CLineDataRecord* pLineDataRecord);
public:
	typedef enum
	{
		NO_COMPARE	= -1,
		MATCH		= 0,
		MISMATCH	= 1
	}MatchStatus;

	static CCompareMap* CreateInstance(const int& match , CLineDataRecord* pLineDataRecord);
	virtual ~CCompareMap();
	
	int m_match;
	vector<MatchStatus>* m_pFieldMatchEntry;
	CLineDataRecord* m_pLineDataRecord;
};

class SMARTISO_MODULE_EXT_CLASS CCompareBucket
{
	CCompareBucket();
	CCompareBucket(CLineDataRecord* pLineDataRecord);
public:
	bool IsMatch() const;
	void RemoveCompareMap(CCompareMap* p);
	void Clear();
	CCompareMap* GetAt(const size_t& at);
	size_t GetCount() const;
	void Add(CCompareMap* pCompareMap);
	void Sort();
	static CCompareBucket* CreateInstance(CLineDataRecord* pLineDataRecord);
	virtual ~CCompareBucket();

	CLineDataRecord* m_pLineDataRecord;
	vector<CCompareMap*>* m_pCompareOutputs;
};

class SMARTISO_MODULE_EXT_CLASS CCompareResult  
{
public:
	void ReGenCompareResult();
	void Clear();
	CCompareBucket* GetDeletedBucketAt(const size_t& at);
	size_t GetDeletedBucketCount() const;
	CCompareBucket* GetNewBucketAt(const size_t& at);
	size_t GetNewBucketCount() const;
	CCompareBucket* GetCompareBucketAt(const size_t& at);
	size_t GetCompareBucketCount() const;
	
	void AddToDeleted(CCompareBucket* pBucket);
	void AddToNew(CCompareBucket* pBucket);
	void AddToCompare(CCompareBucket* pBucket);
	CCompareResult();
	virtual ~CCompareResult();
private:
	vector<CCompareBucket*>* m_pCompareResult;
	vector<CCompareBucket*>* m_pNewCompareResult;
	vector<CCompareBucket*>* m_pDeletedCompareResult;
};

#endif // !defined(AFX_COMPARERESULT_H__82684772_543D_48BE_B212_26C4C0EAC053__INCLUDED_)
