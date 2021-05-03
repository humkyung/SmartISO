// LineListRecord.cpp: implementation of the CLineDataRecord class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include "Module.h"
//#include "CompareResult.h"
#include "LineDataRecord.h"
#include "ProjectData.h"
#include "IsString.h"
#include "Tokenizer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLineDataRecord::CLineDataRecord() : m_bDeleted(false)
{
	m_pLineDataFieldSet = NULL;
	m_pResDataRecord = NULL;
	m_pRevDataRecordSet = NULL;
}

CLineDataRecord::~CLineDataRecord()
{
	try
	{
		if(NULL != m_pLineDataFieldSet) delete m_pLineDataFieldSet;
		if(NULL != m_pResDataRecord) delete m_pResDataRecord;
		if(NULL != m_pRevDataRecordSet) delete m_pRevDataRecordSet;
	}
	catch(...)
	{
		
	}
}

size_t CLineDataRecord::GetFieldCount() const
{
	if(NULL != m_pLineDataFieldSet)
	{
		return m_pLineDataFieldSet->GetFieldCount();
	}

	return 0;
}

CDataField* CLineDataRecord::GetFieldAt(const size_t &at) const
{
	if(NULL != m_pLineDataFieldSet)
	{
		const size_t nFieldCount = m_pLineDataFieldSet->GetFieldCount();
		assert((at < nFieldCount) && "range error!!!");

		if(at < nFieldCount)
		{
			return m_pLineDataFieldSet->GetFieldAt(at);
		}
	}

	return NULL;
}

CLineDataRecord* CLineDataRecord::CreateInstance()
{
	return new CLineDataRecord;
}

void CLineDataRecord::Set( CDataFieldSet* const pLineDataFieldSet )
{
	assert(pLineDataFieldSet && "pLineDataFieldSet is NULL");

	if(pLineDataFieldSet)
	{
		if(NULL != m_pLineDataFieldSet) CDataFieldSet::DeleteInstance(m_pLineDataFieldSet);

		m_pLineDataFieldSet = pLineDataFieldSet;
		return;
	}

	throw exception("pLineDataFieldSet is Null");
}

CString CLineDataRecord::index() const
{
	return m_sIndex;
}

void CLineDataRecord::SetIndex(const CString &index)
{
	m_sIndex = index;
}

void CLineDataRecord::Delete(const bool &flag)
{
	m_bDeleted = flag;
}

bool CLineDataRecord::IsDeleted() const
{
	return m_bDeleted;
}

CResDataRecord* CLineDataRecord::GetResDataRecord()
{
	return m_pResDataRecord;
}

CRevDataRecordSet* CLineDataRecord::GetRevDataRecordSet()
{
	return m_pRevDataRecordSet;
}

void CLineDataRecord::AddResDataRecord(CResDataRecord *pResDataRecord)
{
	assert(pResDataRecord && "pResDataRecord is NULL");

	if(pResDataRecord)
	{
		if(NULL != m_pResDataRecord) delete m_pResDataRecord;
		m_pResDataRecord = pResDataRecord;
	}
}

void CLineDataRecord::AddRevDataRecordSet(CRevDataRecordSet *pRevDataRecordSet)
{
	assert(pRevDataRecordSet && "pRevDataRecordSet is NULL");

	if(pRevDataRecordSet)
	{
		if(NULL != m_pRevDataRecordSet) delete m_pRevDataRecordSet;
		m_pRevDataRecordSet = pRevDataRecordSet;
	}
}

/**	\brief	필드의 값을 설정한다.
	\author	백흠경
	\param	pFieldName	a parameter of type const char *
	\param	pFieldValue	a parameter of type const char *

	\return	void	
*/
void CLineDataRecord::Set(const char *pFieldName, const char *pFieldValue)
{
	assert(pFieldName && "pFieldName is NULL");
	assert(pFieldValue&& "pFieldValue is NULL");

	if(pFieldName && pFieldValue)
	{
		string rFieldName(pFieldName);
		IsString::ToUpper(rFieldName);

		for(vector<CDataField*>::iterator itr = m_pLineDataFieldSet->m_pDataFieldSet->begin();itr != m_pLineDataFieldSet->m_pDataFieldSet->end();itr++)
		{
			string rDataFieldName((*itr)->name());
			IsString::ToUpper(rDataFieldName);

			if(rFieldName == rDataFieldName)
			{
				CString rName((*itr)->name());
				CString rDesc((*itr)->desc());
				(*itr)->Set(rName , rDesc , pFieldValue);
				
				break;
			}
		}
	}
}

/**	\brief	복사품을 생성한다.
**/
CLineDataRecord* CLineDataRecord::Clone()
{
	CLineDataRecord* p = new CLineDataRecord;
	if(p)
	{
		p->m_pLineDataFieldSet = m_pLineDataFieldSet->Clone();
	}

	return p;
}

/**
	@brief	find data field with given description
	@author	humkyung
**/
CDataField* CLineDataRecord::FindWithDesc(LPCTSTR desc) const
{
	assert(desc && "desc is NULL");

	if(desc)
	{
		STRING_T rDesc(desc);
		IsString::ToUpper(rDesc);

		size_t nCount = GetFieldCount();
		for(size_t i = 0;i < nCount;i++)
		{
			CDataField* pDataField = GetFieldAt(i);
			string rFieldDesc = pDataField->desc();
			IsString::ToUpper(rFieldDesc);
			if(rDesc == rFieldDesc) return pDataField;
		}
	}

	return NULL;
}

//int CLineDataRecord::Compare(CCompareMap* pCompareMap , CLineDataRecord *pLineDataRecord)
//{
//	assert(pCompareMap && pLineDataRecord && "Null parameter");
//	int match = 0;
//
//	if(pCompareMap && pLineDataRecord)
//	{
//		pCompareMap->m_match = 0;
//		pCompareMap->m_pFieldMatchEntry->clear();
//
//		int nComparedFieldCount = 0;
//		vector<CDataField*>::iterator jtr = pLineDataRecord->m_pLineDataFieldSet->m_pDataFieldSet->begin();
//		for(vector<CDataField*>::iterator itr = m_pLineDataFieldSet->m_pDataFieldSet->begin();itr != m_pLineDataFieldSet->m_pDataFieldSet->end();itr++,jtr++)
//		{
//			if(CString((*jtr)->GetExcelCol()).IsEmpty())
//			{
//				(pCompareMap->m_pFieldMatchEntry)->push_back(CCompareMap::NO_COMPARE);
//			}
//			else
//			{
//				nComparedFieldCount++;
//				if(string((*jtr)->value()) == string((*itr)->value()))
//				{
//					(pCompareMap->m_match)++;
//					(pCompareMap->m_pFieldMatchEntry)->push_back(CCompareMap::MATCH);
//				}
//				else
//				{
//					(pCompareMap->m_pFieldMatchEntry)->push_back(CCompareMap::MISMATCH);
//				}
//			}
//		}
//
//		if((pCompareMap->m_match) == nComparedFieldCount) (pCompareMap->m_match) = MATCH_ALL;
//	}
//
//	return (pCompareMap->m_match);
//}

bool CLineDataRecord::CoincideWidth(const CLineDataRecord *p)
{
	assert(p && "Null parameter");

	if(p)
	{
		return m_pLineDataFieldSet->CoincideWith(p->m_pLineDataFieldSet);
	}

	return false;
}

/**	
	@brief	compare old & new 에서 유일한 키의 조합
	@author	humkyung
	@return	const char*	
*/
const char* CLineDataRecord::GetPrimaryKey() const
{
	static CString __rPrimaryKey;

	CProjectData* pProjectData = CProjectData::GetInstance();
	__rPrimaryKey.Empty();
	for(vector<STRING_T>::const_iterator itr = pProjectData->m_primaryKeyEntry.begin();itr != pProjectData->m_primaryKeyEntry.end();itr++)
	{
		CDataField* pDataField = FindWithDesc(itr->c_str());
		if(pDataField)
		{
			__rPrimaryKey += pDataField->value();
		}
	}

	return __rPrimaryKey;
}

/**	
	@brief	get reference key of line data record
	
	@author	HumKyung.BAEK

	@return	const char*	
*/
const TCHAR* CLineDataRecord::GetKey() const
{
	static STRING_T res;

	res = _T("");
	CDataField* pField = FindWithFieldName(_T("KEY"));
	if(NULL != pField)
	{
		res = pField->value();
	}

	/*res = _T("");
	CProjectData* pProjectData = CProjectData::GetInstance();
	const CString rDatabaseRefKey = (*pProjectData->m_pProjectSettingMap)[_T("DATABASE_REFERENCE_KEY")];
	vector<STRING_T> oResult;
	CTokenizer<CIsFromString>::Tokenize(oResult , rDatabaseRefKey.operator LPCTSTR() , CIsFromString(_T(" ~ ")));
	oResult.push_back(_T("SHEET_NO"));
	for(vector<string>::iterator itr = oResult.begin();itr != oResult.end();++itr)
	{
		if(!itr->empty())
		{
			CDataField* pField = FindWithFieldName(itr->c_str());
			if(pField)
			{
				res += pField->value();
			}
		}
	}
	*/
	return res.c_str();
}

CRevDataRecord* CLineDataRecord::FindRevDataRecordWithRevNo(const CString& sRevNo)
{
	return m_pRevDataRecordSet->FindWithRevNo(sRevNo);
}

/**	\brief	Issue date로 revision data record를 찾는다.

	\param	pIssueDate	a parameter of type const char*

	\return	CRevDataRecord*	
*/
CRevDataRecord* CLineDataRecord::FindRevDataRecordWithIssueDate(const char* pIssueDate)
{
	assert(pIssueDate && "pIssueDate is NULL");

	if(pIssueDate)
	{
		return m_pRevDataRecordSet->FindWithIssueDate(pIssueDate);
	}

	return NULL;
}

/**	\brief	Rev No로 revision data record를 찾는다.

	\param	pRevNo	a parameter of type const char*

	\return	CRevDataRecord*	
*/
CRevDataRecord* CLineDataRecord::FindRevDataRecordWithRevNo(const char* pRevNo)
{
	assert(pRevNo && "pRevNo is NULL");

	if(pRevNo)
	{
		if( CString("LATEST") == pRevNo )
		{
			const size_t nRecordCount = m_pRevDataRecordSet->GetRecordCount();
			return (nRecordCount > 0) ? m_pRevDataRecordSet->GetRecordAt( nRecordCount - 1 ) : NULL;
		}else	return m_pRevDataRecordSet->FindWithRevNo(pRevNo);
	}

	return NULL;
}

const char* CLineDataRecord::GetAreaNo() const
{
	size_t nCount = GetFieldCount();
	for(size_t i = 0;i < nCount;i++)
	{
		CDataField* pDataField = GetFieldAt(i);
		if(0 == strcmp(pDataField->name() , "AREA_NO")) return (pDataField->value());
	}

	return NULL;
}

/**	
	@brief	필드 이름으로 필드를 찾는다.
	@param	pFieldName	a parameter of type const char *
	@return	CDataField*	
*/
CDataField* CLineDataRecord::FindWithFieldName(LPCTSTR pFieldName) const
{
	CDataField* pField = NULL;
	if( m_pLineDataFieldSet )
	{
		pField = m_pLineDataFieldSet->FindWithFieldName(pFieldName);
	}

	return pField;
}

/**	\brief	The CLineDataRecord::GetRecordString function


	\return	CString	
*/
CString CLineDataRecord::GetRecordString() const
{
	CString rString;
	for(vector<CDataField*>::iterator itr = m_pLineDataFieldSet->m_pDataFieldSet->begin();itr != m_pLineDataFieldSet->m_pDataFieldSet->end();itr++)
	{
		rString += (*itr)->name() + CString("='") + (*itr)->value() + "',";
	}
	rString.TrimRight(",");

	return rString;
}

void CLineDataRecord::DeleteInstance(const CLineDataRecord *pRecord)
{
	assert( pRecord && "pRecord is NULL" );

	if( pRecord ) delete pRecord;
}

/**
	\brief	
	\author	백흠경
**/
void CLineDataRecord::Copy(CLineDataRecord *rhs)
{
	if(this != rhs)
	{
		m_sIndex = rhs->m_sIndex;
		m_bDeleted = rhs->m_bDeleted;
	
		m_pLineDataFieldSet->Copy(rhs->m_pLineDataFieldSet);

		if(NULL == m_pResDataRecord)
		{
			if(NULL != rhs->m_pResDataRecord)
			{
				m_pResDataRecord = rhs->m_pResDataRecord->Clone();
			}
		}else	m_pResDataRecord->Copy(rhs->m_pResDataRecord);

		if(NULL == m_pRevDataRecordSet)
		{
			if(NULL != rhs->m_pRevDataRecordSet)
			{
				m_pRevDataRecordSet = rhs->m_pRevDataRecordSet->Clone();
			}
		}else	m_pRevDataRecordSet->Copy(rhs->m_pRevDataRecordSet);
	}
}

/**	\brief	가장 최근의 revision data record를 구한다.
**/
CRevDataRecord* CLineDataRecord::FindLatestRevDataRecord() const
{
	return m_pRevDataRecordSet->FindLatestRevDataRecord();
}

/**	
	@brief	SmartISO key 조합을 구한다.
	@author	humkyung
**/
CString CLineDataRecord::GetSmartISOKey() const
{
	static CString __rSmartISOKey;

	CProjectData* pProjectData = CProjectData::GetInstance();
	__rSmartISOKey.Empty();
	for(vector<STRING_T>::const_iterator itr = pProjectData->m_SmartISOKeySetting.begin();itr != pProjectData->m_SmartISOKeySetting.end();++itr)
	{
		CDataField* pDataField = FindWithDesc(itr->c_str());
		if(pDataField)
		{
			__rSmartISOKey += pDataField->value();
		}else	__rSmartISOKey += itr->c_str();
	}

	return __rSmartISOKey;
}

/**
	@brief	line data copy or modify 에서 유일한 키의 조합
	@author	humkyung
**/
const char* CLineDataRecord::GetLineDataModuleKey() const
{
	static CString __rLineDataModuleKey__;

	CProjectData* pProjectData = CProjectData::GetInstance();
	__rLineDataModuleKey__.Empty();
	for(vector<STRING_T>::const_iterator itr = pProjectData->m_LineDataModuleKeySetting.begin();itr != pProjectData->m_LineDataModuleKeySetting.end();++itr)
	{
		CDataField* pDataField = FindWithDesc(itr->c_str());
		if(pDataField)
		{
			__rLineDataModuleKey__ += pDataField->value();
		}
	}

	return __rLineDataModuleKey__;
}
