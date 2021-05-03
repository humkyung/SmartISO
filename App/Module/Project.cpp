// Project.cpp: implementation of the CProject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include <Shlwapi.h>

#include "Module.h"
#include "ModuleData.h"
#include "ADO\ADODB.h"
#include "tokenizer.h"
#include "Project.h"
#include "ProjectData.h"
#include "AppPostgreSQL.h"
#include <ado/SQLFile.h>
#include <FileTools.h>

#include <string>
#include <algorithm>
#include <functional> // STL 의 함수객체
using namespace std;

CUnit::CUnit(const CUnit& rhs)
{
	m_oSectionList.insert( m_oSectionList.begin() , rhs.m_oSectionList.begin() , rhs.m_oSectionList.end());
	m_sName = rhs.m_sName;
}

int CUnit::Add( STRING_T sSection )
{
	vector<CSection>::iterator itr = m_oSectionList.begin();
	for(itr = m_oSectionList.begin();itr != m_oSectionList.end();++itr)
	{
		break;
	}
	if(itr == m_oSectionList.end())
	{
		CSection oSection;
		oSection.m_sName = sSection;
		m_oSectionList.push_back( oSection );
	}

	return ERROR_SUCCESS;
}

CArea::CArea(const CArea& rhs)
{
	m_oUnitList.insert( m_oUnitList.begin() , rhs.m_oUnitList.begin() , rhs.m_oUnitList.end());
	m_sName = rhs.m_sName;
}

int CArea::Add( STRING_T sUnit , STRING_T sSection )
{
	vector<CUnit>::iterator itr = m_oUnitList.begin();
	for(itr = m_oUnitList.begin();itr != m_oUnitList.end();++itr)
	{
		if(itr->m_sName == sUnit)
		{
			itr->Add( sSection );
			break;
		}
	}
	if(itr == m_oUnitList.end())
	{
		CUnit oUnit;
		oUnit.m_sName = sUnit;
		oUnit.Add( sSection );
		m_oUnitList.push_back( oUnit );
	}

	return ERROR_SUCCESS;
}

CPlant::CPlant(const CPlant& rhs)
{
	m_oAreaList.insert( m_oAreaList.begin() , rhs.m_oAreaList.begin() , rhs.m_oAreaList.end());
	m_sName = rhs.m_sName;
}

int CPlant::Add( STRING_T sArea , STRING_T sUnit , STRING_T sSection )
{
	vector<CArea>::iterator itr = m_oAreaList.begin();
	for(itr = m_oAreaList.begin();itr != m_oAreaList.end();++itr)
	{
		if(itr->m_sName == sArea)
		{
			itr->Add( sUnit , sSection );
			break;
		}
	}
	if(itr == m_oAreaList.end())
	{
		CArea oArea;
		oArea.m_sName = sArea;
		oArea.Add( sUnit , sSection );
		m_oAreaList.push_back( oArea );
	}

	return ERROR_SUCCESS;
}

SMARTISO_IMPLEMENT_MODULE_FUNC(CProject);
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProject::CProject() : m_pActiveUser(NULL) , m_pIssueDataRecordSet(NULL) , m_pLineDataRecordSet(NULL) , 
m_materialCodeTable(new CMaterialCodeTable) , m_weldDiaTable(new CWeldDiaTable)
{
	m_name          = new string;
	m_desc			= new STRING_T;	/// 2013.01.05 added by humkyung
	m_pProjectNo    = new string;
	m_comment       = new string;
	m_pClientName   = new string;
	m_path          = new string;
	m_date          = new string;
	m_project_le_id = new string;
	m_user_db_path  = new string;
	m_pDatabaseProp = new DatabaseProp();
	m_pLineDataFieldSet = CDataFieldSet::CreateInstance();
	m_pResDataFieldSet = CDataFieldSet::CreateInstance();
	m_pRevDataFieldSet = CDataFieldSet::CreateInstance();

	m_pProjectAccessEntry = NULL;

	m_pActiveUserIdEntry = NULL;
	m_pBackupSystem = new CBackupSystem(this);

	m_pPlantList = new vector<CPlant>;	/// 2011.10.30 added by humkyung
}

CProject::CProject(const char* pName , const char* pComment)
 : m_pActiveUser(NULL) , m_pProjectAccessEntry(NULL) , m_pIssueDataRecordSet(NULL) , m_pLineDataRecordSet(NULL) , 
 m_materialCodeTable(new CMaterialCodeTable) , m_weldDiaTable(new CWeldDiaTable)
{
	m_name          = new string(pName);
	m_desc			= new STRING_T;	/// 2013.01.05 added by humkyung
	m_pProjectNo    = new string;
	m_comment       = new string(pComment);
	m_pClientName   = new string;
	m_path          = new string;
	m_date          = new string;
	m_project_le_id = new string;
	m_user_db_path  = new string;
	m_pDatabaseProp = new DatabaseProp();
	m_pLineDataFieldSet = CDataFieldSet::CreateInstance();
	m_pResDataFieldSet = CDataFieldSet::CreateInstance();
	m_pRevDataFieldSet = CDataFieldSet::CreateInstance();

	m_pActiveUserIdEntry = NULL;
	m_pBackupSystem = new CBackupSystem(this);

	m_pPlantList = new vector<CPlant>;	/// 2011.10.30 added by humkyung
}

CProject::~CProject()
{
	try
	{
		if(NULL != m_pActiveUser)
		{
			delete m_pActiveUser;
		}
		
		if(NULL != m_name) delete m_name;
		SAFE_DELETE(m_desc);	/// 2013.01.05 added by humkyung
		if(NULL != m_pProjectNo) delete m_pProjectNo;
		if(NULL != m_comment) delete m_comment;
		if(NULL != m_pClientName) delete m_pClientName;
		m_pClientName = NULL;
		SAFE_DELETE(m_pDatabaseProp);		/// 2015.09.30 added by humkyung

		if(NULL != m_path) delete m_path;
		if(NULL != m_date) delete m_date;
		if(NULL != m_project_le_id) delete m_project_le_id;
		if(NULL != m_user_db_path) delete m_user_db_path;

		if(NULL != m_pLineDataFieldSet) delete m_pLineDataFieldSet;
		if(NULL != m_pResDataFieldSet) delete m_pResDataFieldSet;
		if(NULL != m_pRevDataFieldSet) delete m_pRevDataFieldSet;
		
		if(NULL != m_pActiveUserIdEntry)
		{
			for(vector<string*>::iterator itr = m_pActiveUserIdEntry->begin();itr != m_pActiveUserIdEntry->end();itr++)
			{
				delete (*itr);
			}
			m_pActiveUserIdEntry->clear();
			delete m_pActiveUserIdEntry;
		}
		if(NULL != m_pProjectAccessEntry)
		{
			for(vector<CProjectAccess*>::iterator itr = m_pProjectAccessEntry->begin();itr != m_pProjectAccessEntry->end();itr++)
			{
				delete (*itr);
			}
			m_pProjectAccessEntry->clear();
			delete m_pProjectAccessEntry;
		}

		//////////////////////////////////////////////////////////////////////////
		if(NULL != m_pBackupSystem) delete m_pBackupSystem;
		if(NULL != m_pIssueDataRecordSet) delete m_pIssueDataRecordSet;
		
		if(NULL != m_pLineDataRecordSet)
		{
			m_pLineDataRecordSet->RemoveContents();	///< line data record들을 제거한다.
			CLineDataRecordSet::DeleteInstance(m_pLineDataRecordSet);
		}

		SAFE_DELETE(m_pPlantList);	/// 2011.10.30 added by humkyung
	}
	catch(...)
	{

	}
}

void CProject::DeleteInstance(CProject* p)
{
	assert(p && "p is NULL");

	if(p)
	{
		delete p;
	}
}

/**
	@brief			CProject::Save
	@author			humkyung
	@date			2011-07-17
	@param
			const char* pFilePath	-	
	@return			void
*/
void CProject::Save(const char* pFilePath)
{
	assert(pFilePath && "pFilePath is NULL");

	if(pFilePath)
	{
		ofstream file(pFilePath);
		if(file.good())
		{
			Save(file);
		}
		file.close();
	}
}

/**	
	@brief	프로젝트의 정보를 저장한다.

	@param	file	a parameter of type ofstream &

	@remarks	시스템 데이터베이스에서 호출한다.

	@return	void	
*/
void CProject::Save(ofstream &file)
{
	file << m_name->c_str() << std::endl;
	file << m_comment->c_str() << std::endl;
	file << m_path->c_str() << std::endl;
	file << m_project_le_id->c_str() << std::endl;
	file << m_date->c_str() << std::endl;
	file << m_user_db_path->c_str() << std::endl;

	/// project no를 저장한다.
	CString rIniFilePath;
	rIniFilePath = (*m_path).c_str() + CString("\\Setting\\") + (*m_name).c_str() + CString(".ini");
	static CString rApp("General");
	WritePrivateProfileString( rApp , "Project No" , m_pProjectNo->c_str() , rIniFilePath );
}

/**	\brief	The CProject::Load function

	\param	file	a parameter of type ifstream&

	\return	void	
*/
void CProject::Load(ifstream& file)
{
	char szBuf[MAX_PATH] = {0,};

	file.getline(szBuf , MAX_PATH);
	(*m_name) = szBuf;
	file.getline(szBuf , MAX_PATH);
	(*m_comment) = szBuf;
	file.getline(szBuf , MAX_PATH);
	(*m_path) = szBuf;
	
	file.getline(szBuf , MAX_PATH);
	(*m_project_le_id) = szBuf;

	file.getline(szBuf , MAX_PATH);
	(*m_date) = szBuf;

	file.getline(szBuf , MAX_PATH);
	(*m_user_db_path) = szBuf;

	CString rIniFilePath;
	rIniFilePath = (*m_path).c_str() + CString("\\Setting\\") + (*m_name).c_str() + CString(".ini");
	LoadSetting(rIniFilePath);
}

CProject::DatabaseProp CProject::GetDatabaseProp() const
{
	return (*m_pDatabaseProp);
}

const char* CProject::name() const
{
	return m_name->c_str();
}

/**	
	@brief	return project desc

	@author	humkyung

	@date	2013.01.05

	@return	const TCHAR
*/
const TCHAR* CProject::GetDesc() const
{
	return m_desc->c_str();
}

const char* CProject::path() const
{
	return m_path->c_str();
}

void CProject::SetName(const char *pName)
{
	assert(pName && "pName is NULL");

	if(pName)
	{
		(*m_name) = pName;
	}
}

/**	
	@brief	project desc 설정한다.

	@authro	humkyung

	@date	2013.01.05

	@return	int
*/
int CProject::SetDesc(const TCHAR* pDesc)
{
	assert(pDesc && "pDesc is NULL");

	if(pDesc)
	{
		(*m_desc) = pDesc;
		return ERROR_SUCCESS;
	}

	return ERROR_BAD_ENVIRONMENT;
}

/**	\brief	project no 설정한다.

	\param	pProjctNo	a parameter of type const char *

	\return	void	
*/
void CProject::SetProjectNo(const char *pProjctNo)
{
	assert(pProjctNo && "pName is NULL");

	if(pProjctNo)
	{
		(*m_pProjectNo) = pProjctNo;
	}
}

/**	
	@brief	프로젝트 경로를 설정한다.
	@param	pPath	a parameter of type const char *
	@return	void	
*/
void CProject::SetPath(LPCTSTR pPath)
{
	assert(pPath && "pPath is NULL");

	if(pPath)
	{
		(*m_path) = pPath;
	}
}

/**
	@brief	set database property
	@author	humkyung
	@date	2015.09.30
*/
void CProject::SetDatabaseProp(const DatabaseProp& prop)
{
	(*m_pDatabaseProp) = prop;
}

/**
	@brief	

	@author	humkyung
**/
int CProject::CheckUser(const char *user, const char *password, const char *area, const char *moudle )
{
	assert(user && password && area && moudle && "Null pointer!!!");

	if(user && password && area && moudle)
	{
		CString rMoudle = moudle;
		const STRING_T sAdminPassword;

		if( rMoudle == _T("Admin"))
		{
			if(_T("ADMIN") == STRING_T(user))
			{
				if(sAdminPassword == STRING_T(password))
				{
					return PROJECT_ADMIN;
				}
				else
				{
					return PROJECT_ADMIN_PASSWORD_ERROR;
				}
			}
			else return NOT_PROJECT_ADMIN;
		}
		else if( rMoudle == _T("User Application"))
		{
			if(_T("ADMIN") == STRING_T(user))
			{
				if(sAdminPassword == STRING_T(password))
				{
					return PROJECT_ADMIN;
				}
				else
				{
					return PROJECT_ADMIN_PASSWORD_ERROR;
				}
			}
			else return PROJECT_USER;
		}
	}

	return PROJECT_USER_ERROR;
}

/**
	@brief	FOLDER 생성, 필요한 파일들을 복사, 기본적인 ini 파일 설정을 한다.
	@author	humkyung
	@date	????.??.??
	@remarks
	1. create project sub folders
	2. copy project access file
	@param	pszFolder = project root folder
*/
bool CProject::CreateFolder(LPCTSTR pszFolder)
{
	assert(pszFolder && "pszFolder is NULL");

	if(pszFolder)
	{
		TCHAR szBuf[MAX_PATH + 1] = {0,};

		CString sProjectFolder(pszFolder);
		if(_T("\\") == sProjectFolder.Right(1)) sProjectFolder = sProjectFolder.Left(sProjectFolder.GetLength()-1);

		if(false == CFileTools::ExistFolder(sProjectFolder)) CreateDirectory(sProjectFolder , NULL);
		{
			/// get executable path
			CString sDirName;
			CFileTools::GetExecutableDirectory(sDirName);
			
			CString rPathName(sDirName);
			const int it = rPathName.ReverseFind('\\');
			if(it) rPathName = rPathName.Left(it);
			/// up to here

			SetFileAttributes(pszFolder , FILE_ATTRIBUTE_NORMAL);
			
			CString rFolder(sProjectFolder.operator LPCSTR());
			if(false == CFileTools::ExistFolder(rFolder + _T("\\Setting"))) CreateDirectory(rFolder + _T("\\Setting") , NULL);
			{
				SetFileAttributes(rFolder + _T("\\Setting") , FILE_ATTRIBUTE_NORMAL);
				
				/// copy default ini file to project setting folder
				const CString sIniFilePath(rFolder + CString(_T("\\Setting\\")) + m_name->c_str() + CString(".ini"));
				if(!CFileTools::DoesFileExist(sIniFilePath))
				{
					if(FALSE == CopyFile(rPathName + _T("\\Setting\\Default.ini") , sIniFilePath , FALSE))
					{
						AfxMessageBox( _T("Fail to copy Default ini file!!!") );
					}
				}
				if(FALSE == CopyFile(rPathName + _T("\\Setting\\interface1.xls") , rFolder + CString(_T("\\Setting\\interface1.xls") ) , FALSE))
				{
					AfxMessageBox( _T("Fail to copy interface1.xls file!!!") );
				}
				/*if(FALSE == CopyFile(rPathName + _T("\\Setting\\interface2.xls") , rFolder.c_str() + CString( _T("\\Setting\\interface2.xls") ) , TRUE))
				{
					AfxMessageBox( _T("Fail to copy interface2.xls file!!!") );
				}*/

				/// copy weld type xml file - 2012.08.20 added by humkyung
				if(FALSE == CopyFile(rPathName + _T("\\Backup\\WeldType.xml") , rFolder + CString(_T("\\Setting\\WeldType.xml") ) , FALSE))
				{
					AfxMessageBox( _T("Fail to copy WeldType.xml file!!!") );
				}
				/// up to here
				/// copy iso print temporary excel file - 2012.08.30 added by humkyung
				if(FALSE == CopyFile(rPathName + _T("\\Setting\\ISOPrintTemp.xls") , rFolder + _T("\\Setting\\ISOPrintTemp.xls") , FALSE))
				{
					AfxMessageBox( _T("Fail to copy ISOPrintTemp.xls file!!!") );
				}
				/// up to here
				
				/// copy template excel file - 2012.12.30 added by humkyung
				if(FALSE == CopyFile(rPathName + _T("\\Setting\\IsoProductTemplate.xls") , rFolder + CString(_T("\\Setting\\IsoProductTemplate.xls") ) , FALSE))
				{
					AfxMessageBox( _T("Fail to copy IsoProductTemplate.xls file!!!") );
				}
				/// up to here

				CString rApp("General");
				const CString rIniFilePath = STRING_T((*m_path) + _T("\\Setting\\") + (*m_name) + _T(".ini")).c_str();
				//////////////////////////////////////////////////////////////////////////
				/// write general setting
				WritePrivateProfileString(rApp , _T("Name") , m_name->c_str() , rIniFilePath);
				WritePrivateProfileString(rApp , _T("Project No") , m_pProjectNo->c_str() , rIniFilePath);
				WritePrivateProfileString(rApp , _T("Client Name") , m_pClientName->c_str() , rIniFilePath);
				WritePrivateProfileString(rApp , _T("Path") , rFolder , rIniFilePath);
				WritePrivateProfileString(rApp , _T("Date") , m_date->c_str() , rIniFilePath);
				WritePrivateProfileString(rApp , _T("Comment") , m_comment->c_str() , rIniFilePath);

				//////////////////////////////////////////////////////////////////////////
				/// write iso edit and rev automation setting - 2012.05.08 added by humkyung
				rApp = _T("Iso Edit Setting");
				WritePrivateProfileString(rApp , _T("Output Iso Dwg File Folder") , STRING_T(rFolder + _T("\\PRODUCT\\DGN\\")).c_str() , rIniFilePath);
				WritePrivateProfileString(rApp , _T("Product BOM Folder") , STRING_T(rFolder + _T("\\PRODUCT\\BOM\\")).c_str() , rIniFilePath);

				rApp = _T("Revision Automation");
				WritePrivateProfileString(rApp , _T("Rev Iso Folder") , STRING_T(rFolder + _T("\\ISSUE\\Latest")).c_str() , rIniFilePath);
				/// up to here
			}

			if(false == CFileTools::ExistFolder(rFolder + _T("\\Database"))) CreateDirectory(rFolder + _T("\\Database"), NULL);
			{
				SetFileAttributes(rFolder + _T("\\Database") , FILE_ATTRIBUTE_NORMAL);

				const CString sDatabaseFilePath(rFolder + CString(_T("\\Database\\")) + m_name->c_str() + CString(_T(".mdb")));
				if(!CFileTools::DoesFileExist(sDatabaseFilePath))
				{
					if(FALSE == CopyFile(rPathName + _T("\\Sample MDB\\Project.mdb") , sDatabaseFilePath , FALSE))
					{
						AfxMessageBox("Fail to copy project mdb file!!!");
					}
				}

				if(false == CFileTools::ExistFolder(rFolder + _T("\\Database\\Backup"))) CreateDirectory(rFolder + _T("\\Database\\Backup"), NULL);
				{
					SetFileAttributes(string(rFolder + "\\Database").c_str() , FILE_ATTRIBUTE_NORMAL);
				}
			}

			if(false == CFileTools::ExistFolder(rFolder + _T("\\TABLE"))) CreateDirectory(rFolder + _T("\\TABLE"), NULL);
			{
				/// copy TABLE.MDB file to project TABLE folder
				if(FALSE == CopyFile(rPathName + _T("\\Sample MDB\\TABLE.MDB") , rFolder + CString("\\TABLE\\TABLE.MDB") , FALSE))
				{
					AfxMessageBox("Fail to copy TABLE.MDB file!!!");
				}
			}
			
			/// SmartISO의 하위 디렉토리 plot_data를 만든다.
			if(false == CFileTools::ExistFolder(rFolder + _T("\\Plot_data"))) CreateDirectory(rFolder + _T("\\Plot_data") , NULL);
			SetFileAttributes(rFolder + _T("\\Plot_data") , FILE_ATTRIBUTE_NORMAL);

			//! plot_data디렉토리에 SmartISO_a3.i 파일과 SmartISO_a4.i 파일을 복사한다.
			if(FALSE == CopyFile(rPathName + _T("\\Setting\\SmartISO_a3.i") , rFolder + CString( _T("\\Plot_data\\SmartISO_a3.i")) , FALSE))
			{
				AfxMessageBox( _T("Fail to copy SmartISO_a3.i file!!!") );
			}
			if(FALSE == CopyFile(rPathName + _T("\\Setting\\SmartISO_a4.i") , rFolder + CString( _T("\\Plot_data\\SmartISO_a4.i")) , FALSE))
			{
				AfxMessageBox(_T("Fail to copy SmartISO_a4.i file!!!"));
			}

			if(false == CFileTools::ExistFolder(rFolder + _T("\\PRODUCT"))) CreateDirectory(rFolder + _T("\\PRODUCT") , NULL);
			{
				SetFileAttributes(rFolder + _T("\\PRODUCT") , FILE_ATTRIBUTE_NORMAL);

				if(false == CFileTools::ExistFolder(rFolder + _T("\\PRODUCT\\BOM"))) CreateDirectory(rFolder + _T("\\PRODUCT\\BOM") , NULL);
				SetFileAttributes(rFolder + _T("\\PRODUCT\\BOM") , FILE_ATTRIBUTE_NORMAL);

				if(false == CFileTools::ExistFolder(rFolder + _T("\\PRODUCT\\ISO"))) CreateDirectory(rFolder + _T("\\PRODUCT\\ISO") , NULL);
				SetFileAttributes(rFolder + _T("\\PRODUCT\\ISO") , FILE_ATTRIBUTE_NORMAL);

				/// create ERR folder in PRODUCT folder - 2011.09.29 added by humkyung
				if(false == CFileTools::ExistFolder(rFolder + _T("\\PRODUCT\\ERR"))) CreateDirectory(rFolder + _T("\\PRODUCT\\ERR") , NULL);
				SetFileAttributes(rFolder + _T("\\PRODUCT\\ERR") , FILE_ATTRIBUTE_NORMAL);
				/// up to here

				/// create INDEX folder in PRODUCT folder - 2012.08.30 added by humkyung
				if(false == CFileTools::ExistFolder(rFolder + _T("\\PRODUCT\\INDEX"))) CreateDirectory(rFolder + _T("\\PRODUCT\\INDEX") , NULL);
				SetFileAttributes(rFolder + _T("\\PRODUCT\\INDEX") , FILE_ATTRIBUTE_NORMAL);
				/// up to here

				/// create IDF folder in PRODUCT folder - 2012.11.10 added by humkyung
				if(false == CFileTools::ExistFolder(rFolder + _T("\\PRODUCT\\IDF"))) CreateDirectory(rFolder + _T("\\PRODUCT\\IDF") , NULL);
				SetFileAttributes(rFolder + _T("\\PRODUCT\\IDF") , FILE_ATTRIBUTE_NORMAL);
				/// up to here
			}
			
			if(false == CFileTools::ExistFolder(rFolder + _T("\\Report"))) CreateDirectory(rFolder + _T("\\Report") , NULL);
			{
				SetFileAttributes(rFolder + _T("\\Report") , FILE_ATTRIBUTE_NORMAL);
				if(FALSE == CopyFile(rPathName + _T("\\Setting\\ISOPrintTemp.xls") , rFolder + _T("\\Report\\IsoIndexForm.xls") , FALSE))
				{
					AfxMessageBox("Fail to copy IsoIndexForm.xls file!!!");
				}
			}

			if(false == CFileTools::ExistFolder(rFolder + _T("\\Temp"))) CreateDirectory(rFolder + _T("\\Temp") , NULL);
			SetFileAttributes(rFolder + _T("\\Temp") , FILE_ATTRIBUTE_NORMAL);

			/// create issue folder - 2012.05.08 added by humkyung
			{
				if(false == CFileTools::ExistFolder(rFolder + _T("\\ISSUE"))) CreateDirectory(rFolder + _T("\\ISSUE") , NULL);
				SetFileAttributes(STRING_T(rFolder + _T("\\ISSUE")).c_str() , FILE_ATTRIBUTE_NORMAL);
					if(false == CFileTools::ExistFolder(rFolder + _T("\\ISSUE\\Latest"))) CreateDirectory(rFolder + _T("\\ISSUE\\Latest") , NULL);
					SetFileAttributes(rFolder + _T("\\ISSUE\\Latest") , FILE_ATTRIBUTE_NORMAL);
			}
			/// up to here

			return true;
		}
	}

	return false;
}

void CProject::SetComment(const char *pComment)
{
	assert(pComment && "pComment is NULL");

	if(pComment)
	{
		(*m_comment) = pComment;
	}
}

const char* CProject::comment() const
{
	return m_comment->c_str();
}

const char* CProject::date() const
{
	return m_date->c_str();
}

void CProject::SetDate(const char *date)
{
	assert(date && "date is NULL");

	if(date)
	{
		(*m_date) = date;
	}
}

/**
	@brief	client name을 리턴한다.

	@author	BHK
*/
const char* CProject::GetClientName() const
{
	if(NULL != m_pClientName) return m_pClientName->c_str();
	return "";
}

/**
	@brief	client name을 설정한다.

	@author	BHK
*/
void CProject::SetClientName(const char *pClientName)
{
	assert(pClientName && "pClientName is NULL");

	if(pClientName) (*m_pClientName) = pClientName;
}

/**
	\brief	
	
	\author	BHK
**/
CUser* CProject::CheckIn(const char* pUserId , const char* password , const char* module, const char* area)
{
	assert(pUserId && "pUserId is NULL");
	assert(password && "password is NULL");
	CUser* ptr = NULL;

	if(pUserId && password)
	{
		int res = CheckUser(pUserId , password ,area, module);
		if((PROJECT_ADMIN == res) || (PROJECT_USER == res))
		{
			if(NULL != m_pActiveUser) delete m_pActiveUser;
			const CString rMsg = _T("사용자 접속");
			m_pActiveUser = new CActiveUser(pUserId , this , rMsg);
			if(m_pActiveUser)
			{
				m_pActiveUser->Set(pUserId , pUserId , password , area);
				if(_T("관리자 접속") == rMsg)
					m_pActiveUser->SetLevel(CUser::ADMIN);
				else	m_pActiveUser->SetLevel(CUser::USER);
			}

			return m_pActiveUser;
		}
	}
	
	return NULL;
}

size_t CProject::GetAccessCount()
{
	if(NULL != m_pProjectAccessEntry) return m_pProjectAccessEntry->size();
	return 0;
}

CProjectAccess* CProject::GetAccessAt(const size_t &at)
{
	assert((at < m_pProjectAccessEntry->size()) && "range error!!!");

	if(at < m_pProjectAccessEntry->size()) return (*m_pProjectAccessEntry)[at];

	return NULL;
}

/**	
	@brief	The CProject::LoadFromIniFile function
	@author	humkyung
	@param	pIniFilePath	a parameter of type const char *
	@return	CProject	
*/
CProject* CProject::LoadFromIniFile(LPCTSTR pIniFilePath)
{
	assert(pIniFilePath && "pIniFilePath is NULL");
	CProject* res = NULL;

	if(pIniFilePath)
	{
		TCHAR szBuf[MAX_PATH] = {0,};
		
		res = new CProject;
		if(NULL != res)
		{
			if(GetPrivateProfileString( _T("General") , _T("Name") , NULL , szBuf , MAX_PATH , pIniFilePath))
			{
				(*res->m_name) = szBuf;
			}
			else
			{
				delete res;
				return NULL;
			}

			if(GetPrivateProfileString("General" , "Comment" , NULL , szBuf , MAX_PATH , pIniFilePath))
			{
				(*res->m_comment) = szBuf;
			}
						
			if(GetPrivateProfileString("General" , "Path" , NULL , szBuf , MAX_PATH , pIniFilePath))
			{
				(*res->m_path) = szBuf;
			}
			else
			{
				delete res;
				AfxMessageBox("Not found project path!!!");
				return NULL;
			}
					
			/*
			if(GetPrivateProfileString("General" , "LE" , NULL , szBuf , MAX_PATH , pIniFilePath))
			{
				(*res->m_project_le_id) = szBuf;
			}
			else
			{
				delete res;
				AfxMessageBox("No project le!!!");
				return NULL;
			}
			*/
			
			if(GetPrivateProfileString("General" , "Date" , NULL , szBuf , MAX_PATH , pIniFilePath))
			{
				(*res->m_date) = szBuf;
			}
			else
			{
				delete res;
				AfxMessageBox("no create date");
				return NULL;
			}

			/*
			if(GetPrivateProfileString("General" , "User Db" , NULL , szBuf , MAX_PATH , pIniFilePath))
			{
				(*res->m_user_db_path) = szBuf;
			}
			else
			{
				delete res;
				AfxMessageBox("No user database");
				return NULL;
			}
			*/
			
			res->LoadSetting(pIniFilePath);
			///res->LoadUserData();
		}
	}

	return res;
}

void CProject::LoadActiveUserData()
{
	if(NULL == m_pActiveUserIdEntry) m_pActiveUserIdEntry = new vector<string*>;

	if(NULL != m_pActiveUserIdEntry)
	{
		//////////////////////////////////////////////////////////////////////////
		/// clean up previous active project user data
		for(vector<string*>::iterator itr = m_pActiveUserIdEntry->begin();itr != m_pActiveUserIdEntry->end();itr++)
		{
			delete (*itr);
		}
		m_pActiveUserIdEntry->clear();
		//////////////////////////////////////////////////////////////////////////
		
		string rDbPath = (*m_path) + "\\Database\\" + (*m_name) + ".mdb";
		
		CADODB adoDB;
		///if(pAdoDb)
		{
			string rConnectionString="Provider=Microsoft.Jet.OLEDB.4.0;Data Source = ";
			rConnectionString += rDbPath.c_str();
			rConnectionString += DBPASSWORD;	/// 2012.01.27 added by humkyung
			if(TRUE == adoDB.DBConnect(rConnectionString.c_str()))
			{
				string rQuery("SELECT * FROM T_ACTIVE_USER");
				
				try
				{
					adoDB.OpenQuery(rQuery.c_str());
					
					LONG lRecordCount = 0L;
					if(adoDB.GetRecordCount(&lRecordCount))
					{
						for(int i = 0;i < lRecordCount;i++)
						{
							STRING_T rUserId , rDate , rComments;
							adoDB.GetFieldValue(i , "USERID" , &rUserId);
							
							m_pActiveUserIdEntry->push_back(new STRING_T(rUserId));
						}
					}
				}
				catch(const exception& ex)
				{
					AfxMessageBox(ex.what());
				}
				
				///adoDB.DBDisConnect();
			}
			else
			{
				AfxMessageBox("Fail to connect user db!!!");
			}
			
			///CADODB::DestroyInstance();
		}
	}
}

//////////////////////////////////////////////////////////////////////////
/**
	@brief	load project settings
	@author	HumKyung.BAEK
**/
void CProject::LoadSetting(LPCTSTR pIniFilePath)
{
	assert(pIniFilePath && "pIniFilePath is NULL");

	if(pIniFilePath)
	{
		TCHAR szBuf[MAX_PATH] = {0,};
		
		CProjectData* pProjectData = CProjectData::GetInstance();
		//////////////////////////////////////////////////////////////////////////
		m_pLineDataFieldSet->Clear();
		//////////////////////////////////////////////////////////////////////////
		CString rApp( _T("General") );
		/*if(GetPrivateProfileString(rApp , _T("Project No") , NULL , szBuf , MAX_PATH , pIniFilePath))
		{
			m_pProjectNo->assign( szBuf );
		}
		else
		{
			AfxMessageBox( _T("Project No isn't Exist") );
		}*/

		rApp = "Line Data Field";
		size_t nFieldCount = 0;
		if(GetPrivateProfileString("Line Data Field" , "Count" , NULL , szBuf , MAX_PATH , pIniFilePath))
		{
			nFieldCount = atoi(szBuf);
		}

		//! prepare line data field set
		{
			const CString rSqlFilePath = GetExecPath() + _T("\\Setting\\T_ISO_LINE_DATA.XML");
			CSQLFile sqlFile(rSqlFilePath.operator LPCTSTR());
			if(ERROR_SUCCESS == sqlFile.ParseXML(rSqlFilePath.operator LPCTSTR()))
			{
				nFieldCount = sqlFile.GetColumnCount();
				CString rKey;
				vector<string> oResult;
				for(int i = 0;i < int(nFieldCount);++i)
				{
					const string rFieldName = sqlFile.GetFieldNameAt(i);
					
					CDataField* pLineDataField = CDataField::CreateInstance();
					if(pLineDataField)
					{
						pLineDataField->Set(rFieldName.c_str() , rFieldName.c_str() , _T(""));
						
						m_pLineDataFieldSet->Add(pLineDataField);
					}
				}
			}
			
			//////////////////////////////////////////////////////////////////////////
			/// 강제 추가 부분.
			CDataField* pLastUpdateDataField = CDataField::CreateInstance();
			if( pLastUpdateDataField )
			{
				pLastUpdateDataField->Set( "LAST_UPDATE" , "LAST UPDATE" , "" );
				m_pLineDataFieldSet->Add( pLastUpdateDataField );
			}
			CDataField* pLineStatusDataField = CDataField::CreateInstance();
			if( pLineStatusDataField )
			{
				pLineStatusDataField->Set( "LINE_STATUS" , "LINE STATUS" , "" );
				m_pLineDataFieldSet->Add( pLineStatusDataField );
			}
			//////////////////////////////////////////////////////////////////////////
		}

		if(GetPrivateProfileString(rApp , "Primary Key" , "" , szBuf , MAX_PATH , pIniFilePath))
		{
			CTokenizer<CIsFromString>::Tokenize(pProjectData->m_primaryKeyEntry , szBuf , CIsFromString(":"));
		}

		if(GetPrivateProfileString(rApp , "Line Data Module Key" , "" , szBuf , MAX_PATH , pIniFilePath))
		{
			CTokenizer<CIsFromString>::Tokenize(pProjectData->m_LineDataModuleKeySetting , szBuf , CIsFromString(":"));
		}else	pProjectData->m_LineDataModuleKeySetting.clear();

		//////////////////////////////////////////////////////////////////////////
		{
			const CString rSqlFilePath = GetExecPath() + _T("\\Setting\\T_RESERVED.SQL");
			CSQLFile sqlFile(rSqlFilePath.operator LPCTSTR());
			if(ERROR_SUCCESS == sqlFile.Parse())
			{
				nFieldCount = sqlFile.GetColumnCount();
				for(int i = 0;i < int(nFieldCount);++i)
				{
					const string rFieldName = sqlFile.GetFieldNameAt(i);

					CDataField* pResDataField = CDataField::CreateInstance();
					if(pResDataField)
					{
						pResDataField->Set(rFieldName.c_str() , rFieldName.c_str() , _T(""));
						m_pResDataFieldSet->Add(pResDataField);
					}
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		{
			const CString rSqlFilePath = GetExecPath() + _T("\\Setting\\T_ISO_REVISION.XML");
			CSQLFile sqlFile(rSqlFilePath.operator LPCTSTR());
			if(ERROR_SUCCESS == sqlFile.ParseXML(rSqlFilePath.operator LPCTSTR()))
			{
				nFieldCount = sqlFile.GetColumnCount();
				for(int i = 0;i < int(nFieldCount);++i)
				{
					const STRING_T rFieldName = sqlFile.GetFieldNameAt(i);
					CDataField* pDataField = CDataField::CreateInstance();
					if(pDataField)
					{
						pDataField->Set(rFieldName.c_str() , rFieldName.c_str() , _T(""));
						m_pRevDataFieldSet->Add(pDataField);
					}
				}
			}
		}

		rApp = _T("Oracle Setting");
		if(GetPrivateProfileString(rApp , _T("SmartISOKey") , _T("") , szBuf , MAX_PATH , pIniFilePath))
		{
			CTokenizer<CIsFromString>::Tokenize(pProjectData->m_SmartISOKeySetting , szBuf , CIsFromString(":"));
		}
	}
}

const char* CProject::GetDatabaseFilePath()
{
	static CString rDbPath;
	rDbPath = path() + CString("\\Database\\") + name() + CString(".mdb");
	
	return rDbPath;
}

const char* CProject::GetIniFilePath()
{
	static CString rIniFilePath;

	rIniFilePath = m_path->c_str() + CString("\\Setting\\") + m_name->c_str() + CString(".ini");

	return rIniFilePath;
}

CBackupSystem* CProject::GetBackupSystem() const
{
	return m_pBackupSystem;
}

/**	
	@brief	issue date table을 로드한다.
	@author	humkyung
	@return	CIssueDateRecordSet*	
*/
CIssueDateRecordSet* CProject::LoadIssueDateRecordSet()
{
	CIssueDateRecordSet* pIssueDateRecordSet = NULL;

	try
	{
		CAppPostgreSQL database(name() , m_pDatabaseProp->Host.c_str() , m_pDatabaseProp->User.c_str() , m_pDatabaseProp->Password.c_str());
		soci::session* session = database.session();
		soci::rowset<soci::row> rs(session->prepare << "SELECT * FROM T_ISSUE_DATE ORDER BY IDX DESC");
		if(NULL == m_pIssueDataRecordSet) m_pIssueDataRecordSet = new CIssueDateRecordSet;
		if(NULL != m_pIssueDataRecordSet)
		{
			for(soci::rowset<soci::row>::const_iterator itr = rs.begin();itr != rs.end();++itr)
			{
				const int index = itr->get<int>(_T("idx"));
				const STRING_T sIssueDate = itr->get<STRING_T>(_T("issue_date"));
				m_pIssueDataRecordSet->Add(index , sIssueDate.c_str());
			}
		}
	}
	catch(const std::exception& ex)
	{
		AfxMessageBox(ex.what());
	}

	return m_pIssueDataRecordSet;
}

/**	
	@brief	The CProject::SaveIssueDateRecordSet function
	@author	humkyung
	@date	????.??.??
	@return	void	
*/
void CProject::SaveIssueDateRecordSet()
{
	if(NULL != m_pIssueDataRecordSet)
	{
		CADODB adoDB;
		///if(pAdoDb)
		try
		{
			CAppPostgreSQL database(name() , m_pDatabaseProp->Host.c_str() , m_pDatabaseProp->User.c_str(),m_pDatabaseProp->Password.c_str());
			const size_t nRecordCount = m_pIssueDataRecordSet->GetCount();

			CString rQuery;
			for(size_t i = 0;i < nRecordCount;i++)
			{					
				CIssueDateRecord* pRecord = m_pIssueDataRecordSet->GetAt(i);
				if(NULL == pRecord) continue;

				CString rLineDataQuery;
				if(-1 == pRecord->index())
				{
					if( !pRecord->IsDeleted() )
					{
						rQuery.Format( _T("INSERT INTO T_ISSUE_DATE(ISSUE_DATE) VALUES('%s')") , pRecord->dateString());
					}
				}
				else
				{
					if( pRecord->IsDeleted() )
					{
						//////////////////////////////////////////////////////////////////////////
						rQuery.Format( _T("DELETE FROM T_ISSUE_DATE WHERE IDX=%d") , pRecord->index());
					}
					else
					{
						//////////////////////////////////////////////////////////////////////////
						rQuery.Format( _T("UPDATE T_ISSUE_DATE SET ISSUE_DATE='%s' WHERE IDX=%d") , pRecord->dateString() , pRecord->index());
					}
				}

				try
				{
					(*database.session()) << rQuery.operator LPCTSTR();
					if(-1 == pRecord->index())
					{
						rQuery = _T("SELECT IDX FROM T_ISSUE_DATE ORDER BY IDX DESC");
						soci::rowset<int> rs(database.session()->prepare << rQuery.operator LPCTSTR());
						for(soci::rowset<int>::const_iterator itr = rs.begin();itr != rs.end();++itr)
						{
							pRecord->SetIndex(*itr);
						}
					}
				}
				catch(std::exception& ex)
				{
					OutputDebugString(ex.what());
				}
			}

			m_pIssueDataRecordSet->RemoveDeletedRecords();	///< 삭제된 record들을 지운다.
		}
		catch(std::exception ex)
		{
			OutputDebugString(ex.what());
		}
	}
}

/**	
	@brief	쿼리 조건 , rev no. , issue date에 맞는 line data record들을 선택한다.
	@author	BHK
	@date	????.??.??
	@param	pAdoDb	a parameter of type CADODB *
	@param	pQueryCondition	a parameter of type const char *
	@param	pRevNo	a parameter of type const char*
	@param	issueDates	a parameter of type const vector<string>&
	@return	CLineDataRecordSet*	
*/
CLineDataRecordSet* CProject::LoadLineDataRecordSet(CAppPostgreSQL *database, LPCTSTR pQueryCondition, const char* pRevNo , const vector< STRING_T >& issueDates , const bool& reload)
{
	assert(database && pQueryCondition && "Parameter is NULL");
	CLineDataRecordSet* pLineDataRecordSet = NULL;
	
	if(database && pQueryCondition)
	{
		if((NULL == m_pLineDataRecordSet) || m_pLineDataRecordSet->IsEmpty() || (true == reload))
		{
			if(m_pLineDataRecordSet) m_pLineDataRecordSet->RemoveContents();
			CollectAllLineDataRecord(database);
		}
		if(NULL == m_pLineDataRecordSet) return pLineDataRecordSet;

		const size_t nFieldCount = m_pLineDataFieldSet->GetFieldCount();
			
		CString rQuery("SELECT KEY FROM T_ISO_LINE_DATA");
		if(_T("") != pQueryCondition) rQuery += _T(" ") + CString(pQueryCondition);
		
		vector<CLineDataRecord*> lineDataRecordEntry;
		pLineDataRecordSet = CLineDataRecordSet::CreateInstance();
		try
		{
			soci::rowset<soci::row> rs(database->session()->prepare << rQuery.operator LPCTSTR());
			for(soci::rowset<soci::row>::const_iterator itr = rs.begin();itr != rs.end();++itr)
			{
				const STRING_T sValue = itr->get<STRING_T>(_T("key"));
				CLineDataRecord* pLineDataRecord = m_pLineDataRecordSet->Find(sValue.c_str());
				if(NULL == pLineDataRecord)
				{
					AfxMessageBox( _T("Database Error : No line data record.") );
				}

				if(NULL != pLineDataRecord)
				{
					bool matchRevNo = false , matchIssueDate = false;
					bool bIsCorrectLineDataRecord = false;
					if( !issueDates.empty() )
					{
						if(STRING_T("LATEST") == pRevNo)
						{
							CRevDataRecord* pRevDataRecord = pLineDataRecord->FindLatestRevDataRecord();
							if(NULL != pRevDataRecord)
							{
								string rIssueDate = pRevDataRecord->GetIssueDate();
								vector<STRING_T>::const_iterator where = find(issueDates.begin() , issueDates.end() , rIssueDate);
								if(where != issueDates.end()) bIsCorrectLineDataRecord = true;
							}
						}
						else
						{
							for( vector< STRING_T >::const_iterator itr = issueDates.begin();itr != issueDates.end();++itr)
							{
								CRevDataRecord* p = pLineDataRecord->FindRevDataRecordWithIssueDate( itr->c_str() );
								if(NULL != p)
								{
									if((STRING_T(pRevNo) == p->GetRevNo()))
									{
										bIsCorrectLineDataRecord = true;
										break;
									}
								}
							}
						}
					}
					else	bIsCorrectLineDataRecord = true;

					if(true == bIsCorrectLineDataRecord)
					{
						pLineDataRecordSet->Add(pLineDataRecord);
					}
				}
			}
		}
		catch(const exception& ex)
		{
			delete pLineDataRecordSet;
			pLineDataRecordSet = NULL;
			AfxMessageBox(ex.what());
		}
	}
	
	return pLineDataRecordSet;
}

/**	
	@brief	쿼리 조건에 맞는 issued line data record들을 선택한다.
	@author	humkyung
	@date	2015.09.23
	@param	pAdoDb	a parameter of type CADODB *
	@param	pQueryCondition	a parameter of type const char *
	@return	CLineDataRecordSet*	
*/
CLineDataRecordSet* CProject::LoadIssuedLineDataRecordSet(CAppPostgreSQL* database, LPCTSTR pQueryCondition)
{
	assert(database && pQueryCondition && "Parameter is NULL");
	CLineDataRecordSet* pLineDataRecordSet = CLineDataRecordSet::CreateInstance();;
	
	if(database && pQueryCondition)
	{
		const size_t nFieldCount = m_pLineDataFieldSet->GetFieldCount();
		CString sql(_T("SELECT * FROM T_ISSUED_ISO_LINE_DATA"));
		if(!CString(pQueryCondition).IsEmpty()) sql += _T(" WHERE ") + CString(pQueryCondition);
		
		try
		{
			CString rTitle("Collect line data records...");
			double dProcess = 0.f;
			
			soci::rowset<soci::row> rs(database->session()->prepare << (sql.operator LPCTSTR()));
			for(soci::rowset<soci::row>::const_iterator itr = rs.begin();itr != rs.end();++itr)
			{
				STRING_T rValue;
				
				CLineDataRecord* pLineDataRecord = CLineDataRecord::CreateInstance();
				CDataFieldSet* pLineDataFieldSet = CDataFieldSet::CreateInstance();
				pLineDataRecord->Set(pLineDataFieldSet);
				
				rValue = itr->get<STRING_T>(_T("key"));
				pLineDataRecord->SetIndex(rValue.c_str());
				for(LONG j = 0;j < int(nFieldCount);++j)
				{
					CDataField* p = m_pLineDataFieldSet->GetFieldAt(j);
					CDataField* pLineDataField = CDataField::CreateInstance();
					if(pLineDataField)
					{
						STRING_T sFieldName(p->name());
						IsString::ToLower(sFieldName);
						try
						{
							rValue = itr->get<STRING_T>(sFieldName);
							pLineDataField->Set(p->name() , p->desc() , rValue.c_str());
						}
						catch(...)
						{
							pLineDataField->Set(p->name() , p->desc() , _T(""));
						}
						pLineDataField->SetExcelCol(p->GetExcelCol());
						pLineDataFieldSet->Add(pLineDataField);
					}
				}
				rValue = itr->get<STRING_T>(_T("status"));
				pLineDataRecord->Delete(_T("DELETED") == rValue);

				pLineDataRecordSet->Add(pLineDataRecord);
			}
		}
		catch(const exception& ex)
		{
			SAFE_DELETE(pLineDataRecordSet);
			AfxMessageBox(ex.what());
		}
	}
	
	return pLineDataRecordSet;
}

/**	
	@brief	모든 line data record들을 선택한다.
	@author	humkyung
	@date	????.??.??
	@param	database a parameter of type CAppPostgreSQL*
	@return	void	
*/
void CProject::CollectAllLineDataRecord(CAppPostgreSQL *database)
{
	if(NULL == m_pLineDataRecordSet) m_pLineDataRecordSet = CLineDataRecordSet::CreateInstance();
	
	if( m_pLineDataRecordSet )
	{
		const size_t nFieldCount = m_pLineDataFieldSet->GetFieldCount();
		CString rQuery(_T("SELECT * FROM T_ISO_LINE_DATA"));
		
		try
		{
			CString rTitle("Collect all line data records...");
			double dProcess = 0.f;
			
			soci::rowset<soci::row> rs(database->session()->prepare <<rQuery.operator LPCTSTR()); 
			for(soci::rowset<soci::row>::const_iterator itr = rs.begin();itr != rs.end();++itr)
			{
				STRING_T rValue;
				
				CLineDataRecord* pLineDataRecord = CLineDataRecord::CreateInstance();
				CDataFieldSet* pLineDataFieldSet = CDataFieldSet::CreateInstance();
				pLineDataRecord->Set(pLineDataFieldSet);
				
				/// index값으로 IDX에서 C_KEY로 바꿈. - 2009.12.07
				rValue = itr->get<STRING_T>(_T("key"));
				pLineDataRecord->SetIndex(rValue.c_str());
				for(LONG j = 0;j < int(nFieldCount);++j)
				{
					CDataField* p = m_pLineDataFieldSet->GetFieldAt(j);
					
					CDataField* pLineDataField = CDataField::CreateInstance();
					if(pLineDataField)
					{
						try
						{
							STRING_T sFieldName(p->name());
							IsString::ToLower(sFieldName);
							rValue = itr->get<STRING_T>(sFieldName);
							pLineDataField->Set(p->name() , p->desc() , rValue.c_str());
						}
						catch(...)
						{
							pLineDataField->Set(p->name() , p->desc() , _T(""));
						}
						pLineDataField->SetExcelCol(p->GetExcelCol());
						
						pLineDataFieldSet->Add(pLineDataField);
					}
				}
				
				///<!-- add DELETED
				rValue = itr->get<STRING_T>(_T("status"));
				if(_T("DELETED") == rValue)
				{
					pLineDataRecord->Delete(true);
				}
				else
				{
					pLineDataRecord->Delete(false);
				}
				///-->
				
				m_pLineDataRecordSet->Add(pLineDataRecord);
			}

			//////////////////////////////////////////////////////////////////////////
			/// load reserved and revision data
			size_t nRecords = m_pLineDataRecordSet->GetLineDataRecordCount();
			for(int i = 0;i < (int)nRecords;++i)
			{
				CLineDataRecord* pLineDataRecord = m_pLineDataRecordSet->GetLineDataRecordAt(i);
				LoadRevDataFromDatabase(pLineDataRecord , database);
			}
		}
		catch(const exception& ex)
		{
			delete m_pLineDataRecordSet;
			m_pLineDataRecordSet = NULL;
			AfxMessageBox(ex.what());
		}
	}
}

//CLineDataRecordSet* CProject::NewLineDataRecordSet(CADODB *pAdoDb, const char *pQueryCondition, const char *pIssueDate)
//{
//	assert(pAdoDb && pQueryCondition && "Parameter is NULL");
//	CLineDataRecordSet* pLineDataRecordSet = NULL;
//	
//	if(pAdoDb && pQueryCondition)
//	{
//		const size_t nFieldCount = m_pLineDataFieldSet->GetFieldCount();
//			
//		CString rQuery("SELECT * FROM T_LINE_DATA");
//		if("" != pQueryCondition) rQuery += " " + CString(pQueryCondition);
//			
//		pLineDataRecordSet = CLineDataRecordSet::CreateInstance();
//		try
//		{
//			CString rTitle("Load Database...");
//			double dProcess = 0.f;
//
//			pAdoDb->OpenQuery(rQuery.operator LPCTSTR());
//			LONG lRecordCount = 0L;
//			pAdoDb->GetRecordCount(&lRecordCount);
//			for(LONG i = 0;i < lRecordCount;i++)
//			{
//				STRING_T rValue;
//				
//				CLineDataRecord* pLineDataRecord = CLineDataRecord::CreateInstance();
//				CDataFieldSet* pLineDataFieldSet = CDataFieldSet::CreateInstance();
//				pLineDataRecord->Set(pLineDataFieldSet);
//						
//				pAdoDb->GetFieldValue(i , "IDX" , &rValue);
//				pLineDataRecord->SetIndex(rValue.c_str());
//				for(size_t j = 0;j < nFieldCount;j++)
//				{
//					CDataField* p = m_pLineDataFieldSet->GetFieldAt(j);
//							
//					CDataField* pLineDataField = CDataField::CreateInstance();
//					if(pLineDataField)
//					{
//						if(pAdoDb->GetFieldValue(i , p->name() , &rValue))
//						{
//							pLineDataField->Set(p->name() , p->desc() , rValue.c_str());
//							pLineDataField->SetExcelCol(p->GetExcelCol());
//						}
//						else
//						{
//							pLineDataField->Set(p->name() , p->desc() , "");
//							pLineDataField->SetExcelCol(p->GetExcelCol());
//						}
//							
//						pLineDataFieldSet->Add(pLineDataField);
//					}
//				}
//						
//				///<!-- add DELETED
//				pAdoDb->GetFieldValue(i , "DELETED" , &rValue);
//						
//				if("Yes" == rValue)
//				{
//					pLineDataRecord->Delete(true);
//				}
//				else
//				{
//					pLineDataRecord->Delete(false);
//				}
//					///-->
//					
//				pLineDataRecordSet->Add(pLineDataRecord);
//						
//				//////////////////////////////////////////////////////////////////////////
//				dProcess++;
//				int process = (int)((dProcess / lRecordCount) * 100.);
//				if(NULL != pUpdateWorkStatusFunc) pUpdateWorkStatusFunc(rTitle , process);
//			}
//
//		}
//		catch(const exception& ex)
//		{
//			delete pLineDataRecordSet;
//			pLineDataRecordSet = NULL;
//			AfxMessageBox(ex.what());
//		}
//	}
//	
//	return pLineDataRecordSet;
//}

/**	
	@brief	line data record와 관련된 revision data를 읽는다.
	@param	pLineDataRecord	a parameter of type CLineDataRecord *
	@param	pAdoDb	a parameter of type CADODB *
	@return	void	
*/
void CProject::LoadRevDataFromDatabase(CLineDataRecord *pLineDataRecord, CAppPostgreSQL* database)
{
	if(database && pLineDataRecord)
	{
		const size_t nFieldCount = m_pRevDataFieldSet->GetFieldCount();
		
		try
		{
			CRevDataRecordSet* pRevDataRecordSet = pLineDataRecord->GetRevDataRecordSet();
			if(NULL == pRevDataRecordSet)
			{
				/// create empty revision data record set.
				pRevDataRecordSet = CRevDataRecordSet::CreateInstance();
				pLineDataRecord->AddRevDataRecordSet(pRevDataRecordSet);
			}
			
			CString rQuery;
			rQuery.Format(_T("SELECT A.* FROM T_ISO_REVISION A,T_REVISION_STAGE B WHERE A.KEY='%s' AND A.REV_NO=B.REVISION_TEXT ORDER BY B.REVISION_NO DESC") , pLineDataRecord->index());
			soci::rowset<soci::row> rs(database->session()->prepare << (rQuery.operator LPCTSTR()));
			{
				for(soci::rowset<soci::row>::const_iterator itr = rs.begin();itr != rs.end();++itr)
				{
					STRING_T rValue;
					try
					{
						rValue = itr->get<STRING_T>(_T("rev_no"));
					}
					catch(...)
					{
						rValue = _T("");
					}
					
					CRevDataRecord* pRevDataRecord = pLineDataRecord->FindRevDataRecordWithRevNo(rValue.c_str());
					if(NULL == pRevDataRecord)
					{
						pRevDataRecord                  = CRevDataRecord::CreateInstance();
						pRevDataRecord->SetIndex(rValue.c_str());
						CDataFieldSet* pRevDataFieldSet = CDataFieldSet::CreateInstance();
						pRevDataRecord->Set(pRevDataFieldSet);
						for(int j = 0;j < int(nFieldCount);++j)
						{
							CDataField* p = m_pRevDataFieldSet->GetFieldAt(j);
							
							CDataField* pRevDataField = CDataField::CreateInstance();
							if(pRevDataField)
							{
								STRING_T sFieldName(p->name());
								IsString::ToLower(sFieldName);
								try
								{
									rValue = itr->get<STRING_T>(sFieldName);
									pRevDataField->Set(p->name() , p->desc() , rValue.c_str());
								}
								catch(...)
								{
									pRevDataField->Set(p->name() , p->desc() , _T(""));
								}
								
								pRevDataFieldSet->Add(pRevDataField);
							}
						}
						
						pRevDataRecordSet->Add(pRevDataRecord);
					}
				}
			}
		}
		catch(const exception& ex)
		{
			AfxMessageBox(ex.what());
		}
	}
}

/**
**/
bool CProject::CheckDuplicatedPrimaryKey(const CString& index , const char *pPrimaryKey)
{
	if(m_pLineDataRecordSet)
	{
		return m_pLineDataRecordSet->CheckDuplicatedPrimaryKey(index , pPrimaryKey);
	}

	throw exception("No LineDataRecordSet");
}

/**
**/
bool CProject::CheckDuplicatedSmartISOKey(const CString& index , const TCHAR *pSmartISOKey)
{
	if(m_pLineDataRecordSet)
	{
		return m_pLineDataRecordSet->CheckDuplicatedSmartISOKey(index , pSmartISOKey);
	}

	throw exception("No LineDataRecordSet");
}

CMaterialCodeTable* CProject::GetMaterialCodeTable() const
{
	CString rDbPath = CString(m_path->c_str()) + "\\TABLE\\TABLE.MDB";

	m_materialCodeTable->Load(rDbPath);
	return (m_materialCodeTable.get());
}

/**
	@brief	return weld data table

	@author	humkyung

	@date	
*/
CWeldDiaTable* CProject::GetWeldDiaTable() const
{
	CString rDbPath = CString(m_path->c_str()) + _T("\\TABLE\\TABLE.MDB");

	m_weldDiaTable->Load(rDbPath);
	return (m_weldDiaTable.get());
}

/**	
	@brief	The CProject::GetProjectNo function

	@author	humkyung

	@return	const char*	
*/
const char* CProject::GetProjectNo() const
{
	if( m_pProjectNo ) return m_pProjectNo->c_str();
	return NULL;
}

/**	\brief	line data를 읽었는지를 확인한다.


	\return	bool	
*/
bool CProject::IsEmptyLineDataRecordSet()
{
	return (NULL == m_pLineDataRecordSet);
}

CDataFieldSet* CProject::GetLineDataFieldSet() const
{
	return m_pLineDataFieldSet;
}

CDataFieldSet* CProject::GetResDataFieldSet() const
{
	return m_pResDataFieldSet;
}

CDataFieldSet* CProject::GetRevDataFieldSet() const
{
	return m_pRevDataFieldSet;
}

/**		
	@brief	rev no를 읽는다.
	@author	humkyung
**/
void CProject::CollectRevNos(list<STRING_T>& RevNoEntry , CAppPostgreSQL *database) const
{
	assert( database && "database is NULL" );

	if(database)
	{
		CString rQuery("SELECT DISTINCT REV_NO FROM T_ISO_REVISION");
		
		try
		{
			RevNoEntry.clear();

			soci::rowset<soci::row> rs(database->session()->prepare << (rQuery.operator LPCTSTR()));
			for(soci::rowset<soci::row>::const_iterator itr = rs.begin();itr != rs.end();++itr)
			{
				const STRING_T rValue = itr->get<STRING_T>(_T("rev_no"));
				RevNoEntry.push_back(rValue);
			}
		}
		catch(const exception& ex)
		{
			AfxMessageBox(ex.what());
		}
	}
}

bool CProject::UpdateUserWithIDX(const long &nIDX , const string &rArea)
{
	bool res = false;
	string rDbPath = (*m_path) + "\\Database\\" + (*m_name) + ".mdb";
	
	CADODB adoDB;
	///if(pAdoDb)
	{
		string rConnectionString="Provider=Microsoft.Jet.OLEDB.4.0;Data Source = ";
		rConnectionString += rDbPath.c_str();
		rConnectionString += DBPASSWORD;	/// 2012.01.27 added by humkyung
		if(TRUE == adoDB.DBConnect(rConnectionString.c_str()))
		{
			//////////////////////////////////////////////////////////////////////////
			/// delete user
			CString rQuery;
			rQuery.Format("UPDATE T_USER SET AREA='%s' WHERE IDX=%d", rArea.c_str(), nIDX);
			try
			{
				adoDB.ExecuteQuery(rQuery.operator LPCTSTR());
				adoDB.DBDisConnect();
				res = true;
			}
			catch(const exception& ex)
			{
				AfxMessageBox(ex.what());
			}
		}
		else
		{
			AfxMessageBox("Fail to connect user db!!!");
		}
		///CADODB::DestroyInstance();
	}

	return res;
}

/**	
	\brief	The CProject::DeleteUser function
	\param	nId	a parameter of type const long &
	\return	bool	
*/
bool CProject::DeleteUserWithIDX(const long &nIDX)
{
	bool res = false;
	string rDbPath = (*m_path) + "\\Database\\" + (*m_name) + ".mdb";
	
	CADODB adoDB;
	///if(pAdoDb)
	{
		string rConnectionString="Provider=Microsoft.Jet.OLEDB.4.0;Data Source = ";
		rConnectionString += rDbPath.c_str();
		rConnectionString += DBPASSWORD;	/// 2012.01.27 added by humkyung
		if(TRUE == adoDB.DBConnect(rConnectionString.c_str()))
		{
			//////////////////////////////////////////////////////////////////////////
			/// delete user
			CString rString;
			rString.Format("%d" , nIDX );
			
			CString rQuery("DELETE FROM T_USER WHERE IDX=" + rString );
			try
			{
				adoDB.ExecuteQuery(rQuery.operator LPCTSTR());
				res = true;
			}
			catch(const exception& ex)
			{
				AfxMessageBox(ex.what());
			}
		}
		else
		{
			AfxMessageBox("Fail to connect user db!!!");
		}
		///CADODB::DestroyInstance();
	}

	return res;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-30
    @class      CProject
    @function   GetPlantList
    @return     vector<CPlant>*
    @brief		return plant list
******************************************************************************/
vector<CPlant>* CProject::GetPlantList()
{
	return m_pPlantList;
}

/**	
	@brief	The CActiveUser::GetLineDataRecordSetFromDatabase function
	@author	humkyung	
	@date	2015.08.20
	@param	pQueryCondition	a parameter of type const char *
	@param	pRevNo	a parameter of type const char*
	@param	pIssueDate	a parameter of type const char*
	@return	CLineDataRecordSet*	
*/
CLineDataRecordSet* CProject::GetLineDataRecordSetFromDatabase(LPCTSTR pQueryCondition , LPCTSTR pRevNo , const vector< STRING_T >& issueDates , const bool& reload)
{
	CLineDataRecordSet* pLineListRecordSet = NULL;//CLineDataRecordSet::CreateInstance();
	{
		const size_t nFieldCount = this->m_pLineDataFieldSet->GetFieldCount();
		
		try
		{
			CAppPostgreSQL database(name() , m_pDatabaseProp->Host.c_str() , m_pDatabaseProp->User.c_str() , m_pDatabaseProp->Password.c_str());
			pLineListRecordSet = this->LoadLineDataRecordSet(&database, pQueryCondition , pRevNo , issueDates , reload );
			size_t nRecords = 0;
			if(pLineListRecordSet)
			{
				//////////////////////////////////////////////////////////////////////////
				/// load reserved data
				nRecords = pLineListRecordSet->GetLineDataRecordCount();
				for(int i = 0;i < int(nRecords);++i)
				{
					CLineDataRecord* pLineDataRecord = pLineListRecordSet->GetLineDataRecordAt(i);
					///m_pProject->LoadResDataFromDatabase(pLineDataRecord , &adoDB);
					this->LoadRevDataFromDatabase(pLineDataRecord , &database);
				}
			}
		}
		catch(const std::exception& ex)
		{
			AfxMessageBox(ex.what());
		}

		//CADODB adoDB;
		/////if(pAdoDb)
		//string rConnectionString = _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source = ");
		//rConnectionString += rDbPath;
		//rConnectionString += DBPASSWORD;	/// 2012.01.27 added by humkyung
		//if(TRUE == adoDB.DBConnect(rConnectionString.c_str()))
		//{
		//	pLineListRecordSet = this->LoadLineDataRecordSet(&adoDB , pQueryCondition , pRevNo , issueDates , reload );
		//	size_t nRecords = 0;
		//	if(pLineListRecordSet)
		//	{
		//		//////////////////////////////////////////////////////////////////////////
		//		/// load reserved data
		//		nRecords = pLineListRecordSet->GetLineDataRecordCount();
		//		for(int i = 0;i < int(nRecords);++i)
		//		{
		//			CLineDataRecord* pLineDataRecord = pLineListRecordSet->GetLineDataRecordAt(i);
		//			///m_pProject->LoadResDataFromDatabase(pLineDataRecord , &adoDB);
		//			this->LoadRevDataFromDatabase(pLineDataRecord , &adoDB);
		//		}
		//	}
		//	
		//	///adoDB.DBDisConnect();
		//}
		//else
		//{
		//	AfxMessageBox( _T("Fail to connect project database!!!"));
		//}
	}
	
	return pLineListRecordSet;
}

/**	
	@brief	get issued line data
	@author	humkyung	
	@date	2015.09.23
	@param	pQueryCondition	a parameter of type const char *
	@param	pRevNo	a parameter of type const char*
	@param	pIssueDate	a parameter of type const char*
	@return	CLineDataRecordSet*	
*/
CLineDataRecordSet* CProject::GetIssuedLineDataRecordSet(LPCTSTR pQueryCondition)
{
	CLineDataRecordSet* pLineListRecordSet = NULL;
	
	try
	{
		CAppPostgreSQL database(name() , m_pDatabaseProp->Host.c_str() , m_pDatabaseProp->User.c_str() , m_pDatabaseProp->Password.c_str());
		const size_t nFieldCount = this->m_pLineDataFieldSet->GetFieldCount();
		pLineListRecordSet = this->LoadIssuedLineDataRecordSet(&database, pQueryCondition);
	}
	catch(const std::exception& ex)
	{
		AfxMessageBox(ex.what());
	}
	
	return pLineListRecordSet;
}

const TCHAR* CProject::GetOutputIsoFileName(CLineDataRecord* pLineDataRecord)
{
	static string __rOutputFileName;

	if(NULL != pLineDataRecord)
	{	
		TCHAR szBuf[MAX_PATH]={'\0',};
	
		__rOutputFileName = _T("");
		string rIniFilePath = this->GetIniFilePath();
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
const TCHAR* CProject::GetOutputIsoFileExt(CLineDataRecord* pLineDataRecord , const CString& sRevNo)
{
	static STRING_T __rOutputFileExt;

	if(NULL != pLineDataRecord)
	{	
		TCHAR szBuf[MAX_PATH]={'\0',};
	
		__rOutputFileExt = _T("");
		STRING_T rIniFilePath = this->GetIniFilePath();
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

/**
	@brief	collect all rev no
	@author	humkyung
	@date	2015.08.20
*/
int CProject::CollectAllRevNo( list< STRING_T >& revNos ) const
{
	try
	{
		CAppPostgreSQL database(name() , m_pDatabaseProp->Host.c_str() , m_pDatabaseProp->User.c_str() , m_pDatabaseProp->Password.c_str());
		this->CollectRevNos( revNos , &database);
	}
	catch(...)
	{
	}
	
	return revNos.size();
}

/**
	@brief	return input file name
	@author	humkyung
*/
const TCHAR* CProject::GetInputIsoFileName(CLineDataRecord* pLineDataRecord)
{
	static string __rOutputFileName;

	if(NULL != pLineDataRecord)
	{	
		TCHAR szBuf[MAX_PATH]={'\0',};
	
		__rOutputFileName = "";
		STRING_T rIniFilePath = this->GetIniFilePath();
		vector<string> oResult;
		if(GetPrivateProfileString(_T("Iso Edit Setting") ,_T("Input File Name Format") , _T("") , szBuf , MAX_PATH , rIniFilePath.c_str()))
		{
			CTokenizer<CIsFromString>::Tokenize(oResult , szBuf , CIsFromString(":"));
		}
		
		for(vector<STRING_T>::const_iterator itr = oResult.begin();itr != oResult.end();itr++)
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

CRevDataRecord* CProject::CreateRevDataRecord()
{
	CRevDataRecord* pRevDataRecord = CRevDataRecord::CreateInstance();
	if(pRevDataRecord)
	{
		CDataFieldSet* pRevDataFieldSet = CDataFieldSet::CreateInstance();
		pRevDataRecord->Set(pRevDataFieldSet);
		
		const size_t nFieldCount = this->m_pRevDataFieldSet->GetFieldCount();
		for(size_t i = 0;i < nFieldCount;i++)
		{
			CDataField* p = this->m_pRevDataFieldSet->GetFieldAt(i);
			
			CDataField* pRevDataField = CDataField::CreateInstance();
			if(pRevDataField)
			{
				pRevDataField->Set(p->name() , p->desc() , "");
				
				pRevDataFieldSet->Add(pRevDataField);
			}
		}
	}
	
	return pRevDataRecord;
}

/**
	@breif	delete database records
	@author	HumKyung
	@date	????.??.??
	@param	pLineDataRecordEntry
	@return
*/
void CProject::Delete(vector<CLineDataRecord*> *pLineDataRecordEntry)
{
	assert(pLineDataRecordEntry && "pLineDataRecordEntry is NULL");

	if(pLineDataRecordEntry)
	{
		CString rDbPath = this->GetDatabaseFilePath();
		
		CADODB adoDB;
		///if(pAdoDb)
		{
			CString rConnectionString = _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source = ");
			rConnectionString += rDbPath;
			rConnectionString += DBPASSWORD;	/// 2012.01.27 added by humkyung
			if(TRUE == adoDB.DBConnect(rConnectionString.operator LPCTSTR()))
			{
				for(vector<CLineDataRecord*>::iterator itr = pLineDataRecordEntry->begin();itr != pLineDataRecordEntry->end();itr++)
				{
					CString rQuery;

					CDataField* pDataField = (*itr)->FindWithFieldName(_T("KEY"));
					if(pDataField)
					{
						rQuery.Format(_T("DELETE FROM T_ISO_LINE_DATA WHERE KEY='%s'") , pDataField->value());
						try
						{
							adoDB.ExecuteQuery(rQuery.operator LPCTSTR());
							(*itr)->Delete(true);
						}
						catch(const exception& ex)
						{
							AfxMessageBox(ex.what());
						}
					}
				}
			}
			else
			{
				AfxMessageBox(_T("Fail to connect project database!!!"));
			}
		}
	}
}

void CProject::Undelete(vector<CLineDataRecord*> *pLineDataRecordEntry)
{
	assert(pLineDataRecordEntry && "pLineDataRecordEntry is NULL");
	
	if(pLineDataRecordEntry)
	{
		CString rDbPath = this->GetDatabaseFilePath();
		
		CADODB adoDB;
		///if(pAdoDb)
		{
			CString rConnectionString="Provider=Microsoft.Jet.OLEDB.4.0;Data Source = ";
			rConnectionString += rDbPath;
			rConnectionString += DBPASSWORD;	/// 2012.01.27 added by humkyung
			if(TRUE == adoDB.DBConnect(rConnectionString.operator LPCTSTR()))
			{
				for(vector<CLineDataRecord*>::iterator itr = pLineDataRecordEntry->begin();itr != pLineDataRecordEntry->end();itr++)
				{
					CString rQuery;
					rQuery.Format("UPDATE T_ISO_LINE_DATA SET DELETED='No' WHERE IDX=%d" , (*itr)->index());
					
					try
					{
						adoDB.ExecuteQuery(rQuery.operator LPCTSTR());
						(*itr)->Delete(false);
					}
					catch(const exception& ex)
					{
						AfxMessageBox(ex.what());
					}
				}
				
				///adoDB.DBDisConnect();
			}
			else
			{
				AfxMessageBox("Fail to connect project database!!!");
			}
		}	
	}
}

template<typename T> struct equal_ptr : public binary_function<T, T, bool>
{
//          typedef T first_argument_type;
//          typedef T second_argument_type;
//          typedef T result_type;

	bool operator ()( const T& a, const T& b ) const
	{
		return (a == b);
	}
};

void CProject::SaveToDatabase(vector<CLineDataRecord*>* pLineDataRecordEntry , bool saveResData , bool saveRevData)
{
	assert(pLineDataRecordEntry && "pLineDataRecordEntry is NULL");

	if(pLineDataRecordEntry)
	{
		CADODB adoDB;
		///if(pAdoDb)
		{
			CString rConnectionString = _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source = ");
			rConnectionString += this->GetDatabaseFilePath();
			rConnectionString += DBPASSWORD;	/// 2012.01.27 added by humkyung
			if(TRUE != adoDB.DBConnect(rConnectionString.operator LPCTSTR())) return;
			
			CString rTitle("Save to database");
			double dProcess = 0.;
			const size_t nLineDataRecordCount = pLineDataRecordEntry->size();

			int index = 0;
			for(vector<CLineDataRecord*>::iterator itr = pLineDataRecordEntry->begin();itr != pLineDataRecordEntry->end();itr++)
			{					
				CLineDataRecord* pLineDataRecord = (*itr);
				if(NULL == pLineDataRecord) continue;

				CString rLineDataQuery;
				if(-1 == pLineDataRecord->index())
				{
					/// 새로운 RECORD를 추가한다.
					rLineDataQuery = _T("INSERT INTO T_ISO_LINE_DATA(");
					
					CString rValues;
					const size_t nFieldCount = pLineDataRecord->GetFieldCount();
					for(size_t i = 0;i < nFieldCount;i++)
					{
						CDataField* pField = pLineDataRecord->GetFieldAt(i);
						if(pField)
						{
							CString rValue(pField->value());
							if( rValue.IsEmpty() ) continue;
							
							rLineDataQuery += CString(pField->name()) + CString(",");
							rValue.Replace(_T("'") , _T("''"));
							rValues += _T("'") + rValue + _T("'") + CString(_T(","));
						}
					}
					if("," == rLineDataQuery.Right(1)) rLineDataQuery = rLineDataQuery.Left(rLineDataQuery.GetLength() - 1);
					if("," == rValues.Right(1)) rValues = rValues.Left(rValues.GetLength() - 1);
					rLineDataQuery += _T(") VALUES(") + rValues + _T(")");
					///rLineDataQuery += _T(",DELETED) VALUES(") + rValues + _T(",'No')");
				}
				else
				{
					//////////////////////////////////////////////////////////////////////////
					rLineDataQuery = _T("UPDATE T_ISO_LINE_DATA SET ");
					const size_t nFieldCount = pLineDataRecord->GetFieldCount();
					for(size_t i = 0;i < nFieldCount;i++)
					{
						CDataField* pField = pLineDataRecord->GetFieldAt(i);
						if(pField)
						{
							CString rValue(pField->value());
							/// 값이 없을때는 해당 FIELD를 UPDATE하지 않는다.
							if( rValue.IsEmpty() ) continue;

							rLineDataQuery += pField->name() + CString("=");
							rValue.Replace("'" , "''");
							rLineDataQuery += CString("'") + rValue + CString("',");
						}
					}
					/*
					if(pLineDataRecord->IsDeleted())
					{
						rLineDataQuery += "DELETED='Yes'";
					}
					else
					{
						rLineDataQuery += "DELETED='No'";
					}
					*/

					if("," == rLineDataQuery.Right(1))
					{
						rLineDataQuery = rLineDataQuery.Left(rLineDataQuery.GetLength() - 1);
					}
					CString rId;
					rId.Format(_T("'%s'") , pLineDataRecord->index());
					rLineDataQuery += _T(" WHERE KEY=") + rId;
				}
				
				
				//////////////////////////////////////////////////////////////////////////
				CString rResDataQuery;
				if(true == saveResData)
				{
					CResDataRecord* pResDataRecord = pLineDataRecord->GetResDataRecord();
					if(pResDataRecord)
					{
						CString sIndex = pResDataRecord->index();
						if(sIndex.IsEmpty())
						{
							rResDataQuery = _T("INSERT INTO T_RESERVED(REF,");
							CString rValues;
							rValues.Format("'%s'," , pLineDataRecord->index());
							
							size_t nCount = pResDataRecord->GetFieldCount();
							for(size_t i = 0;i < nCount;i++)
							{
								CDataField* pResDataField = pResDataRecord->GetFieldAt(i);
								rResDataQuery += pResDataField->name();
								
								CString rValue(pResDataField->value());
								rValue.Replace("'" , "''");
								rValues+= "'" + rValue + "'";
								if(i < nCount - 1)
								{
									rResDataQuery += ",";
									rValues+= ",";
								}
							}
							rResDataQuery += ") VALUES(" + rValues + ")";
						}
						else
						{
							rResDataQuery = "UPDATE T_RESERVED SET ";
							const size_t nFieldCount = pResDataRecord->GetFieldCount();
							for(size_t i = 0;i < nFieldCount;i++)
							{
								CDataField* pField = pResDataRecord->GetFieldAt(i);
								rResDataQuery += pField->name() + CString("=");
								
								CString rValue(pField->value());
								rValue.Replace("'" , "''");
								rResDataQuery += CString("'") + rValue + CString("',");
							}
							if("," == rResDataQuery.Right(1))
							{
								rResDataQuery = rResDataQuery.Left(rResDataQuery.GetLength() - 1);
							}
							CString rId;
							rId.Format("'%s'" , pLineDataRecord->index());
							rResDataQuery += " WHERE REF=" + rId;
						}
					}
				}
				//////////////////////////////////////////////////////////////////////////
				
				//////////////////////////////////////////////////////////////////////////
				if(true == saveRevData)
				{
					CRevDataRecordSet* pRevDataRecordSet = pLineDataRecord->GetRevDataRecordSet();
					if(pRevDataRecordSet)
					{
						pRevDataRecordSet->SaveToDatabase(&adoDB , pLineDataRecord->index());
					}
				}
				//////////////////////////////////////////////////////////////////////////
			
				try
				{
					if(!rLineDataQuery.IsEmpty()) adoDB.ExecuteQuery(rLineDataQuery.operator LPCTSTR());
					if(!rResDataQuery.IsEmpty())
					{
						adoDB.ExecuteQuery(rResDataQuery.operator LPCTSTR());

						CResDataRecord* pResDataRecord = pLineDataRecord->GetResDataRecord();
						if(-1 == pResDataRecord->index())
						{
							STRING_T rQuery , rValue;
							rQuery = "SELECT IDX FROM T_RESERVED ORDER BY IDX DESC";
							try
							{
								adoDB.OpenQuery(rQuery);
								adoDB.GetFieldValue(0 , "REF" , &rValue);
								pResDataRecord->SetIndex(rValue.c_str());
							}
							catch(const exception& ex)
							{
								OutputDebugString(ex.what());
							}	
						}
					}
				}
				catch(const exception& ex)
				{
					OutputDebugString(ex.what());
				}

				if(-1 == pLineDataRecord->index())
				{
					STRING_T rQuery , rValue;
					rQuery = "SELECT DRAW_NO FROM T_ISO_LINE_DATA ORDER BY IDX DESC";
					try
					{
						adoDB.OpenQuery(rQuery);
						adoDB.GetFieldValue(0 , "DRAW_NO" , &rValue);
						pLineDataRecord->SetIndex(rValue.c_str());

						//////////////////////////////////////////////////////////////////////////
						/// add new line data record
						this->m_pLineDataRecordSet->Add(pLineDataRecord);
					}
					catch(const exception& ex)
					{
						OutputDebugString(ex.what());
					}	
				}

				//////////////////////////////////////////////////////////////////////////
				dProcess++;
				int process = (int)((dProcess / nLineDataRecordCount) * 100.);
				if(NULL != pUpdateWorkStatusFunc) pUpdateWorkStatusFunc(rTitle , process);
			}
			///9adoDB.DBDisConnect();
		}
	}
}