// ReservedDataFieldSet.cpp: implementation of the CReservedDataFieldSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include "Module.h"
#include "ReservedDataFieldSet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CReservedDataFieldSet::CReservedDataFieldSet()
{
	m_pReservedDataFieldSet = new vector<CReservedDataField*>;
}

CReservedDataFieldSet::~CReservedDataFieldSet()
{
	try
	{
		if(NULL != m_pReservedDataFieldSet)
		{
			for(vector<CReservedDataField*>::iterator itr = m_pReservedDataFieldSet->begin();itr != m_pReservedDataFieldSet->end();itr++)
			{
				delete (*itr);
			}
			m_pReservedDataFieldSet->clear();
			delete m_pReservedDataFieldSet;
		}
	}
	catch(...)
	{
		
	}
}

CReservedDataFieldSet* CReservedDataFieldSet::CreateInstance()
{
	return new CReservedDataFieldSet;
}

void CReservedDataFieldSet::DeleteInstance(CReservedDataFieldSet *pReservedDataFieldSet)
{
	delete pReservedDataFieldSet;
}

void CReservedDataFieldSet::Add(CReservedDataField *pReservedDataField)
{
	assert(pReservedDataField && "pReservedDataField is NULL");

	if(pReservedDataField) m_pReservedDataFieldSet->push_back(pReservedDataField);
}

size_t CReservedDataFieldSet::GetFieldCount()
{
	return m_pReservedDataFieldSet->size();
}

CReservedDataField* CReservedDataFieldSet::GetFieldAt(const size_t &at)
{
	assert((at < m_pReservedDataFieldSet->size()) && "range error!!!");

	if(at < m_pReservedDataFieldSet->size()) return (*m_pReservedDataFieldSet)[at];

	throw exception("range error!!!");
}
