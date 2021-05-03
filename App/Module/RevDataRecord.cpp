// RevDataRecord.cpp: implementation of the CRevDataRecord class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include "Module.h"
#include "RevDataRecord.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRevDataRecord::CRevDataRecord()
{
	m_pRevDataFieldSet = NULL;
}

CRevDataRecord::~CRevDataRecord()
{
	try
	{
		if(NULL != m_pRevDataFieldSet) delete m_pRevDataFieldSet;
	}
	catch(...)
	{
		
	}
}

void CRevDataRecord::Set( CDataFieldSet* const pDataFieldSet )
{
	assert(pDataFieldSet && "pDataFieldSet is NULL");

	if(pDataFieldSet)
	{
		if(NULL != m_pRevDataFieldSet) CDataFieldSet::DeleteInstance(m_pRevDataFieldSet);

		m_pRevDataFieldSet = pDataFieldSet;
		return;
	}

	throw exception("pRevDataFieldSet is Null");
}

size_t CRevDataRecord::GetFieldCount() const
{
	if(NULL != m_pRevDataFieldSet)
	{
		return m_pRevDataFieldSet->GetFieldCount();
	}

	return 0;
}

CDataField* CRevDataRecord::GetFieldAt(const size_t &at)
{
	if(NULL != m_pRevDataFieldSet)
	{
		const size_t nFieldCount = m_pRevDataFieldSet->GetFieldCount();
		assert((at < nFieldCount) && "range error!!!");

		if(at < nFieldCount)
		{
			return m_pRevDataFieldSet->GetFieldAt(at);
		}
	}

	return NULL;
}

CString CRevDataRecord::index() const
{
	return m_sIndex;
}

void CRevDataRecord::SetIndex(const CString& index)
{
	m_sIndex = index;
}

CRevDataRecord* CRevDataRecord::CreateInstance()
{
	return new CRevDataRecord;
}

void CRevDataRecord::DeleteInstance(CRevDataRecord* pInst)
{
	assert(pInst && "pInst is NULL");

	if(pInst) delete pInst;
}

/**	
	@brief	revision data record를 저장한다.

	@author	HumKyung.BAEK

	@param	pAdoDb	a parameter of type CADODB *
	@param	ref	a parameter of type const long&

	@return	void	
*/
void CRevDataRecord::SaveToDatabase(CADODB *pAdoDb , const CString& ref)
{
	assert(pAdoDb && "pAdoDb is NULLL");

	if(pAdoDb)
	{
		CString rSql;
		if(m_sIndex.IsEmpty())	///< INSERT
		{
			rSql = _T("INSERT INTO T_ISO_REVISION(");
			CString rValues;
			
			size_t nCount = GetFieldCount();
			for(int i = 0;i < int(nCount);++i)
			{
				CDataField* pRevDataField = GetFieldAt(i);
								
				CString rValue(pRevDataField->value());
				if( rValue.IsEmpty() ) continue;

				rSql += pRevDataField->name();
				
				rValue.Replace("'" , "''");
				rValues+= "'" + rValue + "'";
				if(i < int(nCount) - 1)
				{
					rSql += ",";
					rValues+= ",";
				}
			}
			if( _T(",") == rSql.Right(1) ) rSql= rSql.Left(  rSql.GetLength() - 1 );
			if( "," == rValues.Right(1)) rValues= rValues.Left(rValues.GetLength() - 1 );
			rSql += ") VALUES(" + rValues + ")";
		}
		else
		{
			rSql = "UPDATE T_ISO_REVISION SET ";
			const size_t nFieldCount = GetFieldCount();
			for(int i = 0;i < int(nFieldCount);++i)
			{
				CDataField* pField = GetFieldAt(i);
				CString rValue = pField->value();
				if( rValue.IsEmpty()) continue;

				rSql += pField->name() + CString("=");
				rSql += CString("'") + rValue + CString("',");
			}
			if("," == rSql.Right(1))
			{
				rSql = rSql.Left(rSql.GetLength() - 1);
			}

			CString rKey;
			rKey.Format("'%s'" , ref);
			rSql += " WHERE KEY=" + rKey;
		}

		if(!rSql.IsEmpty()) pAdoDb->ExecuteQuery(rSql.operator LPCTSTR());
		m_sIndex = ref;
	}
}

/**	
	@brief	The CRevDataRecord::Find function
	@author	humkyung
	@param	desc	a parameter of type const char *
	@return	CDataField*	
*/
CDataField* CRevDataRecord::Find(LPCTSTR desc)
{
	assert(desc && "desc is NULL");
	
	if(desc)
	{
		size_t nCount = GetFieldCount();
		for(size_t i = 0;i < nCount;i++)
		{
			CDataField* pResDataField = GetFieldAt(i);
			if(0 == STRICMP_T(pResDataField->desc() , desc)) return pResDataField;
		}
	}
	
	return NULL;
}

/**	
	@brief	find data field with given name
	@param	desc	a parameter of type const char *
	@return	CDataField*	
*/
CDataField* CRevDataRecord::FindWithName(const TCHAR *name)
{
	assert(name && "desc is NULL");
	
	if(name)
	{
		size_t nCount = GetFieldCount();
		for(int i = 0;i < int(nCount);++i)
		{
			CDataField* pResDataField = GetFieldAt(i);
			if(0 == STRICMP_T(pResDataField->name() , name)) return pResDataField;
		}
	}
	
	return NULL;
}

/**	
	@brief	Revision number를 구한다.


	@return	const char*	
*/
const char* CRevDataRecord::GetRevNo() const
{
	static CString __rRevNo;

	__rRevNo.Empty();
	const int nFieldCount = m_pRevDataFieldSet->GetFieldCount();
	for(int i = 0;i < nFieldCount;i++)
	{
		CDataField* pField = m_pRevDataFieldSet->GetFieldAt(i);
		if(CString("REV_NO") == pField->name())
		{
			__rRevNo = pField->value();
			break;
		}
	}

	return __rRevNo;
}

/**	
	@brief	이슈 날짜를 구한다.

	@author	humkyung

	@return	const TCHAR*	
*/
const char* CRevDataRecord::GetIssueDate() const
{
	static CString __rIssueDate;

	__rIssueDate.Empty();
	const int nFieldCount = m_pRevDataFieldSet->GetFieldCount();
	for(int i = 0;i < nFieldCount;i++)
	{
		CDataField* pField = m_pRevDataFieldSet->GetFieldAt(i);
		if(CString("REV_DATE") == pField->name())
		{
			__rIssueDate = pField->value();
			break;
		}
	}

	return __rIssueDate;
}

/**	\brief	clone을 생성한다.


	\return	CRevDataRecord*	
*/
CRevDataRecord* CRevDataRecord::Clone()
{
	CRevDataRecord* p = CRevDataRecord::CreateInstance();
	if(p)
	{
		p->m_pRevDataFieldSet = m_pRevDataFieldSet->Clone();
	}

	return p;
}

void CRevDataRecord::Copy(CRevDataRecord *rhs)
{
	if(this != rhs)
	{
		m_sIndex = rhs->m_sIndex;
		m_pRevDataFieldSet->Copy(rhs->m_pRevDataFieldSet);
	}
}
