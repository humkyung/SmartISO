// IssueDateRecord.cpp: implementation of the CIssueDateRecord class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include "Module.h"
#include "IssueDateRecord.h"
#include "ProjectData.h"
#include "Tokenizer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIssueDateRecord::CIssueDateRecord() : m_index(-1) , m_deleted( false )
{
	m_pDateString = new STRING_T;
}

CIssueDateRecord::CIssueDateRecord(long index , LPCTSTR p) : m_index(index) , m_deleted( false )
{
	m_pDateString = new STRING_T(p);
}

CIssueDateRecord::~CIssueDateRecord()
{
	try
	{
		if(NULL != m_pDateString) delete m_pDateString;
	}
	catch(...)
	{

	}
}

void CIssueDateRecord::SetIndex(long index)
{
	m_index = index;
}

long CIssueDateRecord::index() const
{
	return m_index;
}

const TCHAR* CIssueDateRecord::dateString() const
{
	return m_pDateString->c_str();
}

void CIssueDateRecord::SetDateString(const char *p)
{
	assert(p && "p is NULL");

	if(p)
	{
		(*m_pDateString) = p;
	}
}

/**	\brief	삭제 여부를 확인한다.


	\return	bool	
*/
bool CIssueDateRecord::IsDeleted() const
{
	return m_deleted;
}

/**	\brief	The CIssueDateRecord::Delete function

	\param	deleted	a parameter of type const bool

	\return	void	
*/
void CIssueDateRecord::Delete(const bool deleted)
{
	m_deleted = deleted;
}

/**	
	@brief	get value

	@author	humkyung

	@date	2011.07.17

	@return	double
*/
double CIssueDateRecord::GetValueFromIssueDate(const STRING_T& sIssueDate)
{
	double dValue = 0.f;

	CProjectData* pProjectData = CProjectData::GetInstance();
	
	/*const CString sIssueDateFormatNo = pProjectData->GetIssueDateFormatNo();
	const string sIssueDateDelimiter = pProjectData->GetIssueDateDelimiter().operator LPCTSTR();*/
	
	vector<STRING_T> oResult;
	int year = 0 , month = 0 , date = 0;
	CTokenizer<CIsFromString>::Tokenize(oResult , sIssueDate , CIsFromString(_T("-")));
	/// 2012.09.21 added by humkyung
	if(3 == oResult.size())
	{
		year = ATOI_T(oResult[0].c_str());
		month= ATOI_T(oResult[1].c_str());
		date = ATOI_T(oResult[2].c_str());
	}
	/// up to here
		
	dValue = year * 372 + month * 31. + date;

	return dValue;
}

/**	
	@brief	get value which is combined year,month,date from issue date record

	@author	humkyung

	@date	2011.07.17

	@return	double
*/
double CIssueDateRecord::GetValue() const
{
	return CIssueDateRecord::GetValueFromIssueDate(string(dateString()));
}