// DataFieldSet.cpp: implementation of the CDataFieldSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include "Module.h"
#include "DataFieldSet.h"

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

CDataFieldSet::CDataFieldSet()
{
	m_pDataFieldSet = new vector<CDataField*>;
}

CDataFieldSet::~CDataFieldSet()
{
	try
	{
		if(NULL != m_pDataFieldSet)
		{
			for(vector<CDataField*>::iterator itr = m_pDataFieldSet->begin();itr != m_pDataFieldSet->end();itr++)
			{
				delete (*itr);
			}
			m_pDataFieldSet->clear();
			delete m_pDataFieldSet;
		}
	}
	catch(...)
	{
		
	}
}

void CDataFieldSet::Add(CDataField *pDataField)
{
	assert(pDataField && "pDataField is NULL");

	if(pDataField) m_pDataFieldSet->push_back(pDataField);
}

size_t CDataFieldSet::GetFieldCount()
{
	return m_pDataFieldSet->size();
}

CDataField* CDataFieldSet::GetFieldAt(const size_t &at)
{
	assert((at < m_pDataFieldSet->size()) && "range error!!!");

	if(at < m_pDataFieldSet->size()) return (*m_pDataFieldSet)[at];

	throw exception("range error!!!");
}

CDataFieldSet* CDataFieldSet::CreateInstance()
{
	return new CDataFieldSet;
}

void CDataFieldSet::DeleteInstance(CDataFieldSet *pLineDataFieldSet)
{
	delete pLineDataFieldSet;
}

void CDataFieldSet::Clear()
{
	for(vector<CDataField*>::iterator itr = m_pDataFieldSet->begin();itr != m_pDataFieldSet->end();itr++)
	{
		delete (*itr);
	}
	m_pDataFieldSet->clear();
}

CDataFieldSet* CDataFieldSet::Clone()
{
	CDataFieldSet* p = new CDataFieldSet;
	if(p)
	{
		for(vector<CDataField*>::iterator itr = m_pDataFieldSet->begin();itr != m_pDataFieldSet->end();itr++)
		{
			p->m_pDataFieldSet->push_back((*itr)->Clone());
		}
	}

	return p;
}

bool CDataFieldSet::CoincideWith(CDataFieldSet *p)
{
	assert(p && "Null parameter");

	if(p)
	{
		int index = 0;
		for(vector<CDataField*>::iterator itr = m_pDataFieldSet->begin();itr != m_pDataFieldSet->end();itr++,index++)
		{
			BOOL bEmpty = CString((*itr)->GetExcelCol()).IsEmpty();
			if(FALSE == bEmpty)
			{
				CDataField* pField = p->GetFieldAt(index);
				(*itr)->CoincideWith(pField);
			}
		}

		return true;
	}

	return false;
}

struct HasFieldName : binary_function< CDataField* , const char* , bool >
{
	bool operator()( CDataField* lhs , const char* rhs ) const
	{
		return (0 == STRICMP_T(lhs->name() , rhs));
	}
};

/**	
	@brief	The CDataFieldSet::FindWithFieldName function
	@param	pFieldName	a parameter of type const char *
	@return	CDataField*	
*/
CDataField* CDataFieldSet::FindWithFieldName(LPCTSTR pFieldName)
{
	assert( pFieldName && "pFieldName is NULL");

	if( pFieldName )
	{
		vector<CDataField*>::iterator where = find_if(m_pDataFieldSet->begin() , m_pDataFieldSet->end() , bind2nd(HasFieldName() , pFieldName));
		if( m_pDataFieldSet->end() != where) return (*where);
	}

	return NULL;
}

void CDataFieldSet::Copy(CDataFieldSet *rhs)
{
	if(this != rhs)
	{
		Clear();
		for(vector<CDataField*>::iterator itr = rhs->m_pDataFieldSet->begin();itr != rhs->m_pDataFieldSet->end();++itr)
		{
			CDataField* pDataField = (*itr)->Clone();
			pDataField->Copy(*itr);
			m_pDataFieldSet->push_back(pDataField);
		}
	}
}
