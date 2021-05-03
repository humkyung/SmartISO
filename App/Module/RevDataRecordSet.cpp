// RevDataRecordSet.cpp: implementation of the CRevDataRecordSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include "Module.h"
#include "tokenizer.h"
#include "RevDataRecordSet.h"
#include "ProjectData.h"
#include "IssueDateRecord.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRevDataRecordSet::CRevDataRecordSet()
{
	m_pRevDataRecordEntry = new vector<CRevDataRecord*>;
}

CRevDataRecordSet::~CRevDataRecordSet()
{
	try
	{
		if(NULL != m_pRevDataRecordEntry)
		{
			for(vector<CRevDataRecord*>::iterator itr = m_pRevDataRecordEntry->begin();itr != m_pRevDataRecordEntry->end();itr++)
			{
				delete (*itr);
			}
			m_pRevDataRecordEntry->clear();
			delete m_pRevDataRecordEntry;
		}
	}
	catch(...)
	{
		
	}
}

/**
	@brief
	
	@author	humkyung
**/
void CRevDataRecordSet::Add(CRevDataRecord *pRecord)
{
	assert(pRecord && "pRecord is NULL");
	
	if(pRecord)
	{
		m_pRevDataRecordEntry->push_back(pRecord);
	}
}

CRevDataRecordSet* CRevDataRecordSet::CreateInstance()
{
	return new CRevDataRecordSet;
}

size_t CRevDataRecordSet::GetRecordCount() const
{
	return m_pRevDataRecordEntry->size();
}

CRevDataRecord* CRevDataRecordSet::GetRecordAt(const size_t &at)
{
	assert((at < m_pRevDataRecordEntry->size()) && "range error");
	if(at < m_pRevDataRecordEntry->size()) return (*m_pRevDataRecordEntry)[at];
	throw exception("range error");
}

/**
	@brief	
	
	@author	HumKyung.BAEK
**/
void CRevDataRecordSet::SaveToDatabase(CADODB *pAdoDb, const CString& ref)
{
	assert(pAdoDb && "pAdoDb is NULL");

	if(pAdoDb)
	{
		const size_t nRecordCount = GetRecordCount();
		for(int i = 0;i < int(nRecordCount);++i)
		{
			CRevDataRecord* pRecord = GetRecordAt(i);
			pRecord->SaveToDatabase(pAdoDb , ref);
		}
	}
}

/**	
	@brief	주어진 날짜가 이슈 날짜들 보다 이전의 날짜인지 확인한다.

	@author	humkyung
**/
bool CRevDataRecordSet::IsLastDate(const TCHAR *p)
{
	assert(p && "p is NULL");

	if(p)
	{
		CProjectData* pProjectData = CProjectData::GetInstance();

		//! 2011.07.17 added by humkyung
		/*string sIssueDateDelimiter;
		{
			CProjectData* pProjectData = CProjectData::GetInstance();
			sIssueDateDelimiter = pProjectData->GetIssueDateDelimiter().operator LPCTSTR();
		}*/

		int iYear = 0 , iMonth = 0 , iDay = 0;

		const double dDate = CIssueDateRecord::GetValueFromIssueDate(p);
		/*if(ERROR_SUCCESS == pProjectData->ParseIssueDate(iYear , iMonth , iDay , p))
		{
			dDate = iYear*365 + iMonth*30 + iDay;
		}*/

		for(vector<CRevDataRecord*>::iterator itr = m_pRevDataRecordEntry->begin();itr != m_pRevDataRecordEntry->end();itr++)
		{
			const TCHAR* pIssuedDate = (*itr)->GetIssueDate();
			double dIssuedDate = CIssueDateRecord::GetValueFromIssueDate(pIssuedDate);
			/*vector<string> oResult;
			CTokenizer<CIsFromString>::Tokenize(oResult , pIssuedDate , CIsFromString(_T("-")));
			if(3 == oResult.size())
			{
				dIssuedDate = atoi(oResult[0].c_str())*365 + atoi(oResult[1].c_str())*30 + atoi(oResult[2].c_str());*/
				
				if(dDate <= dIssuedDate) return true;
			//}
		}
	}

	return false;
}

struct IsRecordHasIndex : public binary_function<CRevDataRecord*, const long , bool>
{
//          typedef T first_argument_type;
//          typedef T second_argument_type;
//          typedef T result_type;

	bool operator ()( const CRevDataRecord* a, const long b ) const
	{
		return ((atoi(a->index().operator LPCSTR()) == b));
	}
};

struct IsRecordHasRevNo : public binary_function<CRevDataRecord*, const char* , bool>
{
//          typedef T first_argument_type;
//          typedef T second_argument_type;
//          typedef T result_type;

	bool operator ()( const CRevDataRecord* a, const char* b ) const
	{
		return ((CString(a->GetRevNo()) == CString(b)));
	}
};

struct IsRecordHasIssueDate : public binary_function<CRevDataRecord*, const char* , bool>
{
//          typedef T first_argument_type;
//          typedef T second_argument_type;
//          typedef T result_type;

	bool operator ()( const CRevDataRecord* a, const char* b ) const
	{
		return ((CString(a->GetIssueDate()) == CString(b)));
	}
};

/**
	@brief	find revision record with revision no

	@author	HumKyung

	@date	????.??.??

	@param	index

	@return
*/
CRevDataRecord* CRevDataRecordSet::Find(const long &index)
{
	if(NULL != m_pRevDataRecordEntry)
	{
		vector<CRevDataRecord*>::iterator where = find_if(m_pRevDataRecordEntry->begin() , m_pRevDataRecordEntry->end() , bind2nd(IsRecordHasIndex() , index));
		if(m_pRevDataRecordEntry->end() != where)
		{
			return (*where);
		}
	}

	return NULL;
}

CRevDataRecord* CRevDataRecordSet::FindWithIssueDate(const char* pIssueDate)
{
	if(NULL != m_pRevDataRecordEntry)
	{
		vector<CRevDataRecord*>::iterator where = find_if(m_pRevDataRecordEntry->begin() , m_pRevDataRecordEntry->end() , bind2nd(IsRecordHasIssueDate() , pIssueDate));
		if(m_pRevDataRecordEntry->end() != where)
		{
			return (*where);
		}
	}

	return NULL;
}

/**	\brief	pRevNo를 가진 revision record를 찾는다.

	\param	pRevNo	a parameter of type const char*

	\return	CRevDataRecord*	
*/
CRevDataRecord* CRevDataRecordSet::FindWithRevNo(const char* pRevNo)
{
	if(NULL != m_pRevDataRecordEntry)
	{
		vector<CRevDataRecord*>::iterator where = find_if(m_pRevDataRecordEntry->begin() , m_pRevDataRecordEntry->end() , bind2nd(IsRecordHasRevNo() , pRevNo));
		if(m_pRevDataRecordEntry->end() != where)
		{
			return (*where);
		}
	}

	return NULL;
}

void CRevDataRecordSet::Copy(CRevDataRecordSet *rhs)
{
	if(this != rhs)
	{
		Clear();

		for(vector<CRevDataRecord*>::iterator itr = rhs->m_pRevDataRecordEntry->begin();itr != rhs->m_pRevDataRecordEntry->end();itr++)
		{
			CRevDataRecord* pRevRecord = (*itr)->Clone();
			pRevRecord->Copy(*itr);
			m_pRevDataRecordEntry->push_back(pRevRecord);
		}
	}
}

void CRevDataRecordSet::Clear()
{
	for(vector<CRevDataRecord*>::iterator itr = m_pRevDataRecordEntry->begin();itr != m_pRevDataRecordEntry->end();itr++)
	{
		delete (*itr);
	}
	m_pRevDataRecordEntry->clear();
}

CRevDataRecordSet* CRevDataRecordSet::Clone()
{
	CRevDataRecordSet* pClone = new CRevDataRecordSet;
	if(pClone) pClone->Copy(this);
	
	return pClone;
}

static bool LessIssueDate(CRevDataRecord* lhs , CRevDataRecord* rhs)
{	
	const double dates1 = CIssueDateRecord::GetValueFromIssueDate(lhs->GetIssueDate());
	const double dates2 = CIssueDateRecord::GetValueFromIssueDate(rhs->GetIssueDate());
	
	return (dates1 < dates2);
}

/**	
	@brief	가장 최근의 revision data record를 구한다.
	@author	humkyung
**/
CRevDataRecord* CRevDataRecordSet::FindLatestRevDataRecord() const
{
	if(!m_pRevDataRecordEntry->empty())
	{
		/// 이슈 날짜 순으로 소팅한다.(오름 차순)
		//stable_sort(m_pRevDataRecordEntry->begin() , m_pRevDataRecordEntry->end() , LessIssueDate);
		return m_pRevDataRecordEntry->back();
	}

	return NULL;
}

/**
	@breif	

	@author	HumKyung

	@date	2011.02.07

	@param	pRecord

	@return	int
*/
int CRevDataRecordSet::DeleteRecord(CRevDataRecord* pRecord)
{
	assert(pRecord && "pRecord is NULL");

	if(pRecord)
	{
		vector<CRevDataRecord*>::iterator where = find(m_pRevDataRecordEntry->begin() , m_pRevDataRecordEntry->end() , pRecord);
		if(where != m_pRevDataRecordEntry->end())
		{
			CRevDataRecord::DeleteInstance(*where);
			m_pRevDataRecordEntry->erase(where);
		}

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}
