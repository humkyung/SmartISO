// CompareResult.cpp: implementation of the CCompareResult class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>

#include "Module.h"
#include "CompareResult.h"

#include <algorithm>
#include <functional>
using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCompareMap::CCompareMap(const int& match , CLineDataRecord* pLineDataRecord) : 
m_match(match) , m_pLineDataRecord(pLineDataRecord)
{
	m_pFieldMatchEntry = new vector<CCompareMap::MatchStatus>;
}

CCompareMap::~CCompareMap()
{
	try
	{
		if(NULL != m_pFieldMatchEntry) delete m_pFieldMatchEntry;
	}
	catch(...)
	{

	}
}

CCompareMap* CCompareMap::CreateInstance(const int& match , CLineDataRecord* pLineDataRecord)
{
	return new CCompareMap(match , pLineDataRecord);
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCompareBucket::CCompareBucket() : m_pLineDataRecord(NULL)
{
	m_pCompareOutputs = new vector<CCompareMap*>;
}

CCompareBucket::CCompareBucket(CLineDataRecord* pLineDataRecord) : m_pLineDataRecord(pLineDataRecord)
{
	m_pCompareOutputs = new vector<CCompareMap*>;
}

CCompareBucket* CCompareBucket::CreateInstance(CLineDataRecord* pLineDataRecord)
{
	return new CCompareBucket(pLineDataRecord);
}

CCompareBucket::~CCompareBucket()
{
	try
	{
		Clear();

		delete m_pCompareOutputs;
	}
	catch(...)
	{

	}
}

void CCompareBucket::Clear()
{
	for(vector<CCompareMap*>::iterator itr = m_pCompareOutputs->begin();itr != m_pCompareOutputs->end();itr++)
	{
		delete (*itr);
	}
	m_pCompareOutputs->clear();
}

void CCompareBucket::Add(CCompareMap *pCompareMap)
{
	assert(pCompareMap && "NULL parameter");

	if(pCompareMap) m_pCompareOutputs->push_back(pCompareMap);
}

static bool CCompareBucketSortFunc(CCompareMap* lhs , CCompareMap* rhs)
{
	return (lhs->m_match > rhs->m_match);
}

void CCompareBucket::Sort()
{
	stable_sort(m_pCompareOutputs->begin() , m_pCompareOutputs->end() , CCompareBucketSortFunc);
}

size_t CCompareBucket::GetCount() const
{
	return m_pCompareOutputs->size();
}

CCompareMap* CCompareBucket::GetAt(const size_t& at)
{
	assert((at < m_pCompareOutputs->size()) && "range error");

	if(at < m_pCompareOutputs->size()) return (*m_pCompareOutputs)[at];
	throw exception("range error");
}

struct TestCompareMap : public binary_function<CCompareMap* , CCompareMap*, bool>
{
//          typedef T first_argument_type;
//          typedef T second_argument_type;
//          typedef T result_type;

	bool operator ()( const CCompareMap* a, const CCompareMap* b ) const
	{
		return (a->m_pLineDataRecord == b->m_pLineDataRecord);
	}
};

void CCompareBucket::RemoveCompareMap(CCompareMap *p)
{
	if(!m_pCompareOutputs->empty())
	{
		vector<CCompareMap*>::iterator where = find_if(m_pCompareOutputs->begin() , m_pCompareOutputs->end() , bind2nd(TestCompareMap() , p));
		if(m_pCompareOutputs->end() != where)
		{
			delete (*where);
			m_pCompareOutputs->erase(where);
		}
	}
}

bool CCompareBucket::IsMatch() const
{
	return ((1 == m_pCompareOutputs->size()) && (MATCH_ALL == ((*m_pCompareOutputs)[0])->m_match));
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCompareResult::CCompareResult()
{
	m_pCompareResult = new vector<CCompareBucket*>;
	m_pNewCompareResult = new vector<CCompareBucket*>;
	m_pDeletedCompareResult = new vector<CCompareBucket*>;
}

CCompareResult::~CCompareResult()
{
	try
	{
		Clear();
		if(NULL != m_pCompareResult) delete m_pCompareResult;
		if(NULL != m_pNewCompareResult) delete m_pNewCompareResult;
		if(NULL != m_pDeletedCompareResult) delete m_pDeletedCompareResult;
	}
	catch(...)
	{
		
	}
}

void CCompareResult::AddToCompare(CCompareBucket *pBucket)
{
	assert(pBucket && "pBucket is NULL");

	if(pBucket)
	{
		m_pCompareResult->push_back(pBucket);
	}
}

void CCompareResult::AddToNew(CCompareBucket *pBucket)
{
	assert(pBucket && "pBucket is NULL");

	if(pBucket)
	{
		m_pNewCompareResult->push_back(pBucket);
	}
}

void CCompareResult::AddToDeleted(CCompareBucket *pBucket)
{
	assert(pBucket && "pBucket is NULL");

	if(pBucket)
	{
		m_pDeletedCompareResult->push_back(pBucket);
	}
}

size_t CCompareResult::GetCompareBucketCount() const
{
	return m_pCompareResult->size();
}

CCompareBucket* CCompareResult::GetCompareBucketAt(const size_t &at)
{
	assert((at < m_pCompareResult->size()) && "range error");

	if(at < m_pCompareResult->size()) return (*m_pCompareResult)[at];
	throw exception("range error");
}

size_t CCompareResult::GetNewBucketCount() const
{
	return m_pNewCompareResult->size();
}

CCompareBucket* CCompareResult::GetNewBucketAt(const size_t &at)
{
	assert((at < m_pNewCompareResult->size()) && "range error");

	if(at < m_pNewCompareResult->size()) return (*m_pNewCompareResult)[at];
	throw exception("range error");
}

size_t CCompareResult::GetDeletedBucketCount() const
{
	return m_pDeletedCompareResult->size();
}

CCompareBucket* CCompareResult::GetDeletedBucketAt(const size_t &at)
{
	assert((at < m_pDeletedCompareResult->size()) && "range error");

	if(at < m_pDeletedCompareResult->size()) return (*m_pDeletedCompareResult)[at];
	throw exception("range error");
}

void CCompareResult::Clear()
{
	if(NULL != m_pCompareResult)
	{
		for(vector<CCompareBucket*>::iterator itr = m_pCompareResult->begin();itr != m_pCompareResult->end();itr++)
		{
			delete (*itr);
		}
		m_pCompareResult->clear();
	}
	
	if(NULL != m_pNewCompareResult)
	{
		for(vector<CCompareBucket*>::iterator itr = m_pNewCompareResult->begin();itr != m_pNewCompareResult->end();itr++)
		{
			delete (*itr);
		}
		m_pNewCompareResult->clear();
	}
	
	if(NULL != m_pDeletedCompareResult)
	{
		for(vector<CCompareBucket*>::iterator itr = m_pDeletedCompareResult->begin();itr != m_pDeletedCompareResult->end();itr++)
		{
			delete (*itr);
		}
		m_pDeletedCompareResult->clear();
	}
}




void CCompareResult::ReGenCompareResult()
{
	CString rTitle("Regenerate compare result...");
	double dProcess = 0.;
			
	size_t nCompareBucketCount = GetCompareBucketCount();
	for(int i = 0;i < int(nCompareBucketCount);i++)
	{
		CCompareBucket* pCompareBucket = GetCompareBucketAt(i);
		if(pCompareBucket->IsMatch())
		{
			CCompareMap* pCompareMap = pCompareBucket->GetAt(0);
			for(int j = nCompareBucketCount - 1;j >= 0;j--)
			{
				if(i == j) continue;
				CCompareBucket* pCompareBucket1 = GetCompareBucketAt(j);
				pCompareBucket1->RemoveCompareMap(pCompareMap);
			}
		}

		dProcess++;
		int process = (int)((dProcess / nCompareBucketCount) * 100.);
		if(NULL != pUpdateWorkStatusFunc) pUpdateWorkStatusFunc(rTitle , process);
	}

	for(vector<CCompareBucket*>::iterator itr = m_pCompareResult->begin();itr != m_pCompareResult->end();itr++)
	{
		if(0 == (*itr)->GetCount())
		{
			m_pDeletedCompareResult->push_back(*itr);
			m_pCompareResult->erase(itr--);
		}
	}
}
