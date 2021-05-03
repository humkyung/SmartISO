// ReservedDataRecord.cpp: implementation of the CResDataRecord class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include "Module.h"
#include "ResDataRecord.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CResDataRecord::CResDataRecord() : m_pDataFieldSet(NULL)
{
}

CResDataRecord::~CResDataRecord()
{
	try
	{
		if(NULL != m_pDataFieldSet)
		{
			delete m_pDataFieldSet;
		}
	}
	catch(...)
	{
		
	}
}

CResDataRecord* CResDataRecord::CreateInstance()
{
	return new CResDataRecord;
}

void CResDataRecord::DeleteInstance(CResDataRecord* pInst)
{
	assert(pInst && "pInst is NULL");

	if(pInst) delete pInst;
}

void CResDataRecord::Set( CDataFieldSet* const pDataFieldSet )
{
	assert(pDataFieldSet && "pReservedDataFieldSet is NULL");

	if(pDataFieldSet)
	{
		if(NULL != m_pDataFieldSet) CDataFieldSet::DeleteInstance(m_pDataFieldSet);

		m_pDataFieldSet = pDataFieldSet;
		return;
	}

	throw exception("pReservedDataFieldSet is Null");
}

size_t CResDataRecord::GetFieldCount() const
{
	if(NULL != m_pDataFieldSet)
	{
		return m_pDataFieldSet->GetFieldCount();
	}

	return 0;
}

CDataField* CResDataRecord::GetFieldAt(const size_t &at)
{
	if(NULL != m_pDataFieldSet)
	{
		const size_t nFieldCount = m_pDataFieldSet->GetFieldCount();
		assert((at < nFieldCount) && "range error!!!");

		if(at < nFieldCount)
		{
			return m_pDataFieldSet->GetFieldAt(at);
		}
	}

	return NULL;
}

void CResDataRecord::SetIndex(const CString &sIndex)
{
	m_sIndex = sIndex;
}

CString CResDataRecord::index() const
{
	return m_sIndex;
}

CDataField* CResDataRecord::Find(const char *desc)
{
	assert(desc && "desc is NULL");

	if(desc)
	{
		size_t nCount = GetFieldCount();
		for(size_t i = 0;i < nCount;i++)
		{
			CDataField* pResDataField = GetFieldAt(i);
			if(0 == strcmp(pResDataField->desc() , desc)) return pResDataField;
		}
	}

	return NULL;
}

void CResDataRecord::Copy(CResDataRecord *rhs)
{
	if(this != rhs)
	{
		m_sIndex = rhs->m_sIndex;
		if(NULL == m_pDataFieldSet)
			m_pDataFieldSet = rhs->m_pDataFieldSet->Clone();
		else
			m_pDataFieldSet->Copy(rhs->m_pDataFieldSet);
	}
}

/************************************************************************/
/************************************************************************/
CResDataRecord* CResDataRecord::Clone()
{
	CResDataRecord* pClone = new CResDataRecord;
	if(pClone) pClone->Copy(this);

	return pClone;
}
