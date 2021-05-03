// LineListRecordSet.cpp: implementation of the CLineDataRecordSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include "Module.h"
#include "LineDataRecordSet.h"

#include <functional>
#include <algorithm>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLineDataRecordSet::CLineDataRecordSet()
{
	m_pLineDataRecordEntry = new vector<CLineDataRecord*>;
}

/**	
	@brief	컨테이너만 제거한다. ( line data record는 제거하지 마시오. )
*/
CLineDataRecordSet::~CLineDataRecordSet()
{
	try
	{
		if(NULL != m_pLineDataRecordEntry)
		{
			m_pLineDataRecordEntry->clear();
			delete m_pLineDataRecordEntry;
		}
	}
	catch(...)
	{
		
	}
}

size_t CLineDataRecordSet::GetLineDataRecordCount() const
{
	return m_pLineDataRecordEntry->size();
}

CLineDataRecord* CLineDataRecordSet::GetLineDataRecordAt(const size_t &at)
{
	assert((at < m_pLineDataRecordEntry->size()) && "range error!!!");

	if(at < m_pLineDataRecordEntry->size())
	{
		int nIndex = 0;
		for(vector<CLineDataRecord*>::iterator itr = m_pLineDataRecordEntry->begin();itr != m_pLineDataRecordEntry->end();++itr)
		{
			CLineDataRecord* p = *itr;
			if(nIndex == at) return p;
			++nIndex;
		}

		return (*m_pLineDataRecordEntry)[at];
	}

	return NULL;
}

CLineDataRecordSet* CLineDataRecordSet::CreateInstance()
{
	return new CLineDataRecordSet;
}

void CLineDataRecordSet::DeleteInstance(CLineDataRecordSet* pInst)
{
	if(pInst)
	{
		SAFE_DELETE( pInst );
	}
}

void CLineDataRecordSet::Add(CLineDataRecord *pLineListRecord)
{
	assert(pLineListRecord && "pLineListRecord is NULL");

	if(pLineListRecord)
	{
		m_pLineDataRecordEntry->push_back(pLineListRecord);
	}
}

struct IsRecordHasSamePrimaryKey : public binary_function<CLineDataRecord*, CLineDataRecord*, bool>
{
//          typedef T first_argument_type;
//          typedef T second_argument_type;
//          typedef T result_type;

	bool operator ()( const CLineDataRecord* a, const CLineDataRecord* b ) const
	{
		return ((a->index() != b->index()) && (CString(a->GetPrimaryKey()) == CString(b->GetPrimaryKey())));
	}
};

bool CLineDataRecordSet::ExistRecordHasSamePrimaryKey(CLineDataRecord *p)
{
	assert(p && "p is NULL");

	if(p)
	{
		vector<CLineDataRecord*>::iterator where = find_if(m_pLineDataRecordEntry->begin() , m_pLineDataRecordEntry->end() , bind2nd(IsRecordHasSamePrimaryKey() , p));
		return (m_pLineDataRecordEntry->end() != where);
	}

	return false;
}

struct IsRecordHasIndex : public binary_function<CLineDataRecord*, const CString , bool>
{
//          typedef T first_argument_type;
//          typedef T second_argument_type;
//          typedef T result_type;

	bool operator ()( const CLineDataRecord* a, const CString& b ) const
	{
		return ((a->index() == b));
	}
};

CLineDataRecord* CLineDataRecordSet::Find(const CString &sIndex)
{
	if(NULL != m_pLineDataRecordEntry)
	{
		vector<CLineDataRecord*>::iterator where = find_if(m_pLineDataRecordEntry->begin() , m_pLineDataRecordEntry->end() , bind2nd(IsRecordHasIndex() , sIndex));
		if(m_pLineDataRecordEntry->end() != where)
		{
			return (*where);
		}
	}

	return NULL;
}

/**	
	@brief	저장한 line data record들을 제거한다.

	@author	humkyung

	@return	void	
*/
void CLineDataRecordSet::RemoveContents()
{
	if(NULL != m_pLineDataRecordEntry)
	{
		for(vector<CLineDataRecord*>::iterator itr = m_pLineDataRecordEntry->begin();itr != m_pLineDataRecordEntry->end();++itr)
		{
			delete (*itr);
		}
		m_pLineDataRecordEntry->clear();
	}
}

namespace
{
	static CString __index;	///< 피 검사대상인 line의 index값.
	struct DoesRecordHasSamePrimaryKey : public binary_function<CLineDataRecord*, string, bool>
	{
	//          typedef T first_argument_type;
	//          typedef T second_argument_type;
	//          typedef T result_type;

		bool operator ()( const CLineDataRecord* a, const string& b ) const
		{
			if( a->IsDeleted() ) return false;
			return ((a->index() != __index) && (string(a->GetPrimaryKey()) == b));
		}
	};

	struct DoesRecordHasSameSmartISOKey : public binary_function<CLineDataRecord*, STRING_T , bool>
	{
	//          typedef T first_argument_type;
	//          typedef T second_argument_type;
	//          typedef T result_type;

		bool operator ()( const CLineDataRecord* a, const STRING_T& b ) const
		{
			if( a->IsDeleted() ) return false;
			return ((a->index() != __index) && (STRING_T(a->GetSmartISOKey()) == b));
		}
	};
};

/**	\brief	line id가 중복되는지를 검사한다.

	\param	index	a parameter of type const long&
	\param	pPrimaryKey	a parameter of type const char *

	\remarks
	삭제된 line은 검사에 포함되지 않는다.

	\return	bool	
*/
bool CLineDataRecordSet::CheckDuplicatedPrimaryKey(const CString& index , const char *pPrimaryKey)
{
	assert(pPrimaryKey && "pPrimaryKey is NULL");

	if(pPrimaryKey)
	{
		__index = index;
		vector<CLineDataRecord*>::iterator where = find_if(m_pLineDataRecordEntry->begin() , m_pLineDataRecordEntry->end() , bind2nd(DoesRecordHasSamePrimaryKey() , pPrimaryKey));
		return (m_pLineDataRecordEntry->end() != where);
	}

	return false;
}

/**	
	@brief	line id가 중복되는지를 검사한다.\n
			삭제된 line은 검사에 포함되지 않는다.

	@author	humkyung
	@param	index	a parameter of type const long&
	@param	pPrimaryKey	a parameter of type const char *

	@return	bool	
*/
bool CLineDataRecordSet::CheckDuplicatedSmartISOKey(const CString& index , const TCHAR *pSmartISOKey)
{
	assert(pSmartISOKey && "pSmartISOKey is NULL");

	if(pSmartISOKey)
	{
		__index = index;
		vector<CLineDataRecord*>::iterator where = find_if(m_pLineDataRecordEntry->begin() , m_pLineDataRecordEntry->end() , bind2nd(DoesRecordHasSameSmartISOKey() , pSmartISOKey));
		return (m_pLineDataRecordEntry->end() != where);
	}

	return false;
}

bool CLineDataRecordSet::IsEmpty() const
{
	return ((NULL == m_pLineDataRecordEntry) || m_pLineDataRecordEntry->empty());
}
