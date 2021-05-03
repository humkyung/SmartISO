// IssueDateRecordSet.cpp: implementation of the CIssueDateRecordSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include "ProjectData.h"
#include "Module.h"
#include "tokenizer.h"
#include "IssueDateRecordSet.h"

#include <algorithm>
#include <functional>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIssueDateRecordSet::CIssueDateRecordSet()
{
	m_pIssueDateEntry = new vector<CIssueDateRecord*>;
}

CIssueDateRecordSet::~CIssueDateRecordSet()
{
	try
	{
		if(NULL != m_pIssueDateEntry)
		{
			for (vector<CIssueDateRecord*>::iterator itr = m_pIssueDateEntry->begin();itr != m_pIssueDateEntry->end();itr++)
			{
				delete (*itr);
			}
			m_pIssueDateEntry->clear();
			delete m_pIssueDateEntry;
		}
	}
	catch(...)
	{
		
	}
}

void CIssueDateRecordSet::Add(CIssueDateRecord *p)
{
	assert(p && "p is NULL");

	if(p)
	{
		m_pIssueDateEntry->push_back(p);
	}
}

struct IsIssueDate : public binary_function<CIssueDateRecord*, long, bool>
{
//          typedef T first_argument_type;
//          typedef T second_argument_type;
//          typedef T result_type;

	bool operator ()( const CIssueDateRecord* a, const long& b ) const
	{
		return (b == a->index());
	}
};

struct IsIssueDateForString : public binary_function<CIssueDateRecord*, char*, bool>
{
//          typedef T first_argument_type;
//          typedef T second_argument_type;
//          typedef T result_type;

	bool operator ()( const CIssueDateRecord* a, const char* b ) const
	{
		return (CString(b) == CString(a->dateString()));
	}
};

struct LessOrEqualIssueDate : public binary_function<CIssueDateRecord*, const TCHAR*, bool>
{
//          typedef T first_argument_type;
//          typedef T second_argument_type;
//          typedef T result_type;

	bool operator ()( const CIssueDateRecord* a, const TCHAR* b ) const
	{
		const double dates1 = CIssueDateRecord::GetValueFromIssueDate(b);
		const double dates2 = CIssueDateRecord::GetValueFromIssueDate(a->dateString());

		return (dates1 <= dates2);
	}
};

/**	
	@brief	issue date record를 추가하거나 갱신한다.
	@author	humkyung
	@param	index	a parameter of type const long &
	@param	pDateString	a parameter of type const char *
	@param	pRealDateString	a parameter of type const char*
	@return	void	
*/
void CIssueDateRecordSet::Add(const long &index, LPCTSTR pDateString)
{
	assert(pDateString && "pDateString is NULL");

	if(pDateString)
	{
		if( -1 != index )
		{
			vector<CIssueDateRecord*>::iterator where = find_if(m_pIssueDateEntry->begin() , m_pIssueDateEntry->end() , bind2nd(IsIssueDate() , index));
			if(m_pIssueDateEntry->end() != where)
			{
				(*where)->SetDateString(pDateString);
			}
			else
			{
				m_pIssueDateEntry->push_back(new CIssueDateRecord(index , pDateString));
			}
		}
		else
		{
			m_pIssueDateEntry->push_back(new CIssueDateRecord(index , pDateString));
		}
	}
}

size_t CIssueDateRecordSet::GetCount() const
{
	return m_pIssueDateEntry->size();
}

CIssueDateRecord* CIssueDateRecordSet::GetAt(const size_t &at)
{
	assert((at < m_pIssueDateEntry->size()) && "range error");

	if(at < m_pIssueDateEntry->size()) return (*m_pIssueDateEntry)[at];

	throw exception("range error");
}

/**	
	@brief	인자로 주어진 날짜 보다 같거나 이전인 issue date record가 있는지 검사한다.

	@author	humkyung

	@param	p	a parameter of type const char *

	@return	존재하면 true , otherwise false
*/
bool CIssueDateRecordSet::LessOrEqual(const TCHAR *p)
{
	assert(p && "p is NULL");

	if(p)
	{
		vector<CIssueDateRecord*>::iterator where = 
			find_if(m_pIssueDateEntry->begin() , m_pIssueDateEntry->end() , bind2nd(LessOrEqualIssueDate() , p));
		return (m_pIssueDateEntry->end() != where);
	}

	return false;
}

/**	\brief	issue date가 인자(p)와 같은 issue date record를 삭제한다.

	\param	nIDX	a parameter of type const int&

	\return	void	
*/
void CIssueDateRecordSet::DeleteRecord(const int& nIDX )
{
	if( -1 != nIDX )
	{
		vector<CIssueDateRecord*>::iterator where = find_if(m_pIssueDateEntry->begin() , m_pIssueDateEntry->end() , bind2nd(IsIssueDate() , nIDX));
		if(m_pIssueDateEntry->end() != where)
		{
			(*where)->Delete( true );
		}
	}
}


/**	\brief	삭제된 record들을 리스트에서 지운다.


	\return	void	
*/
void CIssueDateRecordSet::RemoveDeletedRecords()
{
	if( m_pIssueDateEntry )
	{
		for (vector<CIssueDateRecord*>::iterator itr = m_pIssueDateEntry->begin();itr != m_pIssueDateEntry->end();itr++)
		{
			if( (*itr)->IsDeleted() )
			{
				delete (*itr);
				m_pIssueDateEntry->erase( itr-- );
			}
		}
	}
}
