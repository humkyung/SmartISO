// User.cpp: implementation of the CUser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include "ado/ADODB.h"
#include <Tokenizer.h>
#include "Project.h"
#include "User.h"
#include "ProjectData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUser::CUser() : m_index(-1) , m_level(USER) , m_pProject(NULL)
{
	m_id = new string;
	m_name = new string;
	m_password = new string;
	m_area = new string;
	m_date = new string;
	m_assigned_date = new string;
}

CUser::CUser(const char* id , const char* name , const char* password , const char* area) : m_index(-1) , m_level(USER) , m_pProject(NULL)
{
	m_id = new string(id);
	m_name = new string(name);
	m_password = new string(password);
	m_area = new string(area);
	m_date = new string;
	m_assigned_date = new string;
}

CUser::CUser(const CUser& user)
{
	m_index = user.m_index;
	m_id = new string(user.m_id->c_str());
	m_name = new string(user.m_name->c_str());
	m_password = new string(user.m_password->c_str());
	m_area  = new string(user.m_area->c_str());
	m_date = new string(user.m_date->c_str());
	m_assigned_date = new string(user.m_assigned_date->c_str());

	m_level = user.m_level;
	m_pProject = user.m_pProject;
}

CUser::~CUser()
{
	try
	{
		if(NULL != m_id) delete m_id;
		if(NULL != m_name) delete m_name;
		if(NULL != m_password) delete m_password;
		if(NULL != m_area) delete m_area;
		if(NULL != m_date) delete m_date;
		if(NULL != m_assigned_date) delete m_assigned_date;
	}
	catch(...)
	{

	}
}

CUser* CUser::CreateInstance()
{
	return (new CUser);
}

const char* CUser::id() const
{
	return m_id->c_str();
}

const char* CUser::name() const
{
	return m_name->c_str();
}

const char* CUser::password() const
{
	return m_password->c_str();
}

const char* CUser::area() const
{
	return m_area->c_str();
}

long CUser::index() const
{
	return m_index;
}

void CUser::SetIndex(const long &index)
{
	m_index = index;
}

void CUser::Set(const char* id , const char* name , const char* password , const char* area)
{
	assert(id && name && password && area && "Null Pointer!!!");

	if(id && name && password && area)
	{
		(*m_id) = id;
		(*m_name) = name;
		(*m_password) = password;
		(*m_area) = area;
	}
}

CUser::USER_LEVEL CUser::level() const
{
	return m_level;
}

void CUser::SetLevel(const CUser::USER_LEVEL& level)
{
	m_level = level;
}

void CUser::CheckOut()
{

}

void CUser::DeleteAccessData(vector<long>* pAccessIndexEntry)
{

}

void CUser::Log(const char *msg)
{

}

void CUser::LoadAccessData()
{

}

size_t CUser::GetAccessCount()
{
	return 0;
}

CProjectAccess* CUser::GetAccessAt(const size_t &at)
{
	return NULL;
}

void CUser::AddUsers(vector<string*>* pUserIdEntry)
{
}

void CUser::SetDate(const char *date)
{
	assert(date && "date is NULL");

	if(date)
	{
		(*m_date) = date;
	}
}

const char* CUser::date() const
{
	return m_date->c_str();
}

void CUser::SetAssignedDate(const char *date)
{
	(*m_assigned_date) = date;
}

const char* CUser::assigned_date() const
{
	return m_assigned_date->c_str();
}

bool CUser::UpdateProperty(const char *name, const char *password)
{
	return false;
}

CLineDataRecordSet* CUser::GetLineDataRecordSetFromDatabase(const char *pQueryCondition , const char* pRevNo , const vector< STRING_T >& issueDates , const bool& reload)
{
	return NULL;
}

CDataFieldSet* CUser::GetLineDataFieldSet()
{
	return NULL;
}

CResDataRecord* CUser::CreateResDataRecord()
{
	return NULL;
}

void CUser::SaveIssueDateRecordSet()
{
	if(NULL != m_pProject)
	{
		m_pProject->SaveIssueDateRecordSet();
	}
}

void CUser::SaveToDatabase(vector<CLineDataRecord*>* pLineDataRecordEntry , bool saveResData , bool saveRevData)
{

}

CProject* CUser::GetProject()
{
	return m_pProject;
}

void CUser::Delete(vector<CLineDataRecord*> *pLineDataRecordEntry)
{

}

void CUser::Undelete(vector<CLineDataRecord*> *pLineDataRecordEntry)
{

}

CRevDataRecord* CUser::CreateRevDataRecord()
{
	return NULL;
}

/**
	@brief	return input file name
	@author	humkyung
*/
const TCHAR* CUser::GetInputIsoFileName(CLineDataRecord* pLineDataRecord)
{
	static string __rOutputFileName;

	if((NULL != m_pProject) && (NULL != pLineDataRecord))
	{	
		char szBuf[MAX_PATH]={'\0',};
	
		__rOutputFileName = "";
		string rIniFilePath = m_pProject->GetIniFilePath();
		vector<string> oResult;
		if(GetPrivateProfileString("Iso Edit Setting","Input File Name Format" , "" , szBuf , MAX_PATH , rIniFilePath.c_str()))
		{
			CTokenizer<CIsFromString>::Tokenize(oResult , szBuf , CIsFromString(":"));
		}
		
		for(vector<string>::iterator itr = oResult.begin();itr != oResult.end();itr++)
		{
			CDataField* pField = pLineDataRecord->FindWithDesc(itr->c_str());
			if(NULL != pField)
			{
				__rOutputFileName += pField->value();
			}
			else
			{
				__rOutputFileName += (*itr);
			}
		}

		return __rOutputFileName.c_str();
	}

	return NULL;
}

/**
	@brief	
	@author	HumKyung.BAEK
**/
const TCHAR* CUser::GetOutputIsoFileName(CLineDataRecord* pLineDataRecord)
{
	static string __rOutputFileName;

	if((NULL != m_pProject) && (NULL != pLineDataRecord))
	{	
		TCHAR szBuf[MAX_PATH]={'\0',};
	
		__rOutputFileName = _T("");
		string rIniFilePath = m_pProject->GetIniFilePath();
		vector<string> oResult;
		if(GetPrivateProfileString(_T("Iso Edit Setting") , _T("Output File Name Format") , _T("") , szBuf , MAX_PATH , rIniFilePath.c_str()))
		{
			CTokenizer<CIsFromString>::Tokenize(oResult , szBuf , CIsFromString(":"));
		}
		
		for(vector<STRING_T>::const_iterator itr = oResult.begin();itr != oResult.end();++itr)
		{
			CDataField* pField = pLineDataRecord->FindWithDesc(itr->c_str());
			if(NULL != pField)
			{
				__rOutputFileName += pField->value();
			}
			else
			{
				__rOutputFileName += (*itr);
			}
		}

		return __rOutputFileName.c_str();
	}

	return NULL;
}

/**
	@brief	get output file extension for linedata

	@author	humkyung

	@date	2011.08.20

	@param	linedatarecord
**/
const TCHAR* CUser::GetOutputIsoFileExt(CLineDataRecord* pLineDataRecord , const CString& sRevNo)
{
	static STRING_T __rOutputFileExt;

	if((NULL != m_pProject) && (NULL != pLineDataRecord))
	{	
		TCHAR szBuf[MAX_PATH]={'\0',};
	
		__rOutputFileExt = _T("");
		STRING_T rIniFilePath = m_pProject->GetIniFilePath();
		vector<STRING_T> oResult;
		if(GetPrivateProfileString(_T("Iso Edit Setting") , _T("Output File Ext Format") , _T("") , szBuf , MAX_PATH , rIniFilePath.c_str()))
		{
			CTokenizer<CIsFromString>::Tokenize(oResult , szBuf , CIsFromString( _T(":") ));
		}
		
		CRevDataRecord* pRevDataRecord = NULL;
		if(sRevNo.IsEmpty() || _T("LATEST") == sRevNo)
		{
			pRevDataRecord = pLineDataRecord->FindLatestRevDataRecord();
		}
		else
		{
			pRevDataRecord = pLineDataRecord->FindRevDataRecordWithRevNo(sRevNo);
		}

		CString sRevNo_(sRevNo);
		if(NULL != pRevDataRecord)
		{
			sRevNo_ = pRevDataRecord->GetRevNo();
		}

		CProjectData* pProjectData = CProjectData::GetInstance();
		CRevisionStage oRevStage;
		if(ERROR_SUCCESS == pProjectData->GetRevisionStage(oRevStage , sRevNo_))
		{
			for(vector<STRING_T>::iterator itr = oResult.begin();itr != oResult.end();++itr)
			{
				if(_T("GRADE") == (*itr))
				{
					__rOutputFileExt += oRevStage.m_sDefine;
				}
				else if(_T("REV_NO") == (*itr))
				{
					__rOutputFileExt += oRevStage.m_sText;
				}
				else if(NULL != pRevDataRecord)
				{
					CDataField* pField = pRevDataRecord->Find(itr->c_str());
					__rOutputFileExt += (NULL != pField) ? pField->value() : (*itr);
				}
				else
				{
					__rOutputFileExt += (*itr);
				}
			}
		}

		return __rOutputFileExt.c_str();
	}

	return NULL;
}

void CUser::DisplayErrorMessage(const int &error)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
	CString rMsg;

	if(UNKNOWN_ERROR == error)
	{
		rMsg = "Unknown Error";
	}
	else if(USER_CANCEL == error)
	{
		rMsg = "User cancel the work.";
	}
	else if(CREATE_ERROR == error)
	{
		rMsg = "Error in processing work.";
	}
	else if(FAIL_TO_CONNECT_DATABASE == error)
	{
		rMsg = "Fail to connect database";	
	}

	AfxMessageBox(rMsg , MB_OK | MB_ICONWARNING);
}

/**
**/
bool CUser::CheckDuplicatedPrimaryKey(const CString& index , const char *pPrimaryKey)
{
	if(NULL != m_pProject)
	{
		try
		{
			return m_pProject->CheckDuplicatedPrimaryKey(index , pPrimaryKey);
		}
		catch(const exception& ex)
		{
			throw ex;
		}
	}

	throw exception("Null Project.");
}

/**
**/
bool CUser::CheckDuplicatedSmartISOKey(const CString& index , const TCHAR *pSmartISOKey)
{
	if(NULL != m_pProject)
	{
		try
		{
			return m_pProject->CheckDuplicatedSmartISOKey(index , pSmartISOKey);
		}
		catch(const exception& ex)
		{
			throw ex;
		}
	}

	throw exception("Null Project.");
}

CMaterialCodeTable* CUser::GetMaterialCodeTable() const
{
	if(m_pProject)
	{
		return (m_pProject->GetMaterialCodeTable());
	}

	return NULL;
}

CWeldDiaTable* CUser::GetWeldDiaTable() const
{
	if(m_pProject)
	{
		return (m_pProject->GetWeldDiaTable());
	}

	return NULL;
}

int CUser::CollectAllPlant( )
{
	throw exception( "bad call" );
}

int CUser::CollectAllPlantNo( list< STRING_T >& plantNos )
{
	throw exception( "bad call" );
}

int CUser::CollectAreaNoWith( vector< string >& areaNos , const vector< string >& plantNos )
{
	throw exception( "bad call" );
}

int CUser::CollectUnitNoWith( vector< string >& unitNos , const vector< string >& plantNos , const vector< string >& areaNos)
{
	throw exception( "bad call" );
}

int CUser::CollectSectionNoWith( vector< string >& sectionNos , const vector< string >& plantNos , const vector< string >& areaNos , const vector< string >& unitNos)
{
	throw exception( "bad call" );
}

int CUser::CollectAllRevNo( list< STRING_T >& revNos )
{
	throw exception( "bad call" );
}

bool CUser::DeleteUserWithIDX(const long &nIDX)
{
	if( m_pProject ) return (m_pProject->DeleteUserWithIDX( nIDX ));
	return false;
}
