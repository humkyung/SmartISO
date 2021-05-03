// AdminDocData.cpp: implementation of the CAdminDocData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <util/FileTools.h>
#include "Admin.h"
#include "AdminDocData.h"
#include <ProjectData.h>
#include "StringTable.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAdminDocData::CAdminDocData()
{
	m_headerImgList.Create(IDB_LIST_HEADER , 8 , 8 , RGB(255 , 0 , 255));
	m_pSelUser = NULL;
}

CAdminDocData::~CAdminDocData()
{
	try
	{
		///if(NULL != m_pProjectSettingMap) delete m_pProjectSettingMap;
		///m_pProjectSettingMap = NULL;
	}
	catch(...)
	{
	}
}

CAdminDocData* CAdminDocData::GetInstance()
{
	static CAdminDocData __instance;

	return &(__instance);
}

/**	
	@brief	
	@author	humkyung
	@return	void	
*/
void CAdminDocData::LoadData()
{
	TCHAR szBuf[MAX_PATH + 1] = {'\0' ,};

	CString rApp = _T("Oracle Setting");
	if(::GetPrivateProfileString(rApp , _T("DDL FILE PATH"), NULL , szBuf , MAX_PATH , m_rIniFilePath))
	{
		m_rDDLFilePath = szBuf;
	}
	else	m_rDDLFilePath.Empty();

	CAdminDoc* pDoc = CAdminDocData::GetInstance()->GetDocument();
	CProjectData::GetInstance()->Load(pDoc->GetCurProject()->name(),pDoc->GetCurProject()->GetDatabaseProp());
}

/**	
	@brief	get document
	@author	HumKyung.BAEK
	@date	2010.03.09
	@return	void	
*/
CAdminDoc* CAdminDocData::GetDocument()
{
	POSITION pos = AfxGetApp()->GetFirstDocTemplatePosition();
	CDocTemplate* pDocTemplate = AfxGetApp()->GetNextDocTemplate(pos);
	pos = pDocTemplate->GetFirstDocPosition();
	CAdminDoc* pDoc = (CAdminDoc*)pDocTemplate->GetNextDoc(pos);

	return pDoc;
}

/**	
	@brief	return execute path
	
	@author	HumKyung.BAEK

	@date	2010.03.15

	@return	CString	
*/
CString CAdminDocData::GetExecPath()
{
	static CString sExecPath;

	if(sExecPath.IsEmpty())
	{
		CFileTools::GetExecutableDirectory(sExecPath);
		sExecPath.TrimRight('\\');
	}

	return sExecPath;
}

/******************************************************************************
    @author     humkyung
    @date       2011-08-29
    @class      CAdminDocData
    @function   GetIniFilePath
    @return     CString
    @brief
******************************************************************************/
CString CAdminDocData::GetIniFilePath() const
{
	return m_rIniFilePath;
}

/******************************************************************************
    @author     humkyung
    @date       2011-08-29
    @class      CAdminDocData
    @function   SetIniFilePath
    @return     void
    @param      const           CString&
    @param      sIniFilePath
    @brief
******************************************************************************/
void CAdminDocData::SetIniFilePath(const CString& sIniFilePath)
{
	m_rIniFilePath = sIniFilePath;
}

/******************************************************************************
	@brief
    @author     humkyung
    @date       2011-09-22
    @class      CAdminDocData
    @function   SaveSettingData
    @return     int
******************************************************************************/
int CAdminDocData::SaveSettingData()
{
	CAdminDoc* pDoc = CAdminDoc::GetInstance();
	const CProject::DatabaseProp prop = pDoc->GetCurProject()->GetDatabaseProp();

	try
	{
		CAppPostgreSQL database(pDoc->GetCurProject()->name(),prop.Host.c_str(),prop.User.c_str(),prop.Password.c_str());

		map<STRING_T,STRING_T> oFieldInfoList;
		soci::rowset<soci::row> rs(database.session()->prepare << _T("select column_name,data_type from information_schema.columns where table_schema='public' and table_name='t_project_setting'"));
		for(soci::rowset<soci::row>::const_iterator itr = rs.begin();itr != rs.end();++itr)
		{
			oFieldInfoList.insert(make_pair(itr->get<STRING_T>(0),itr->get<STRING_T>(1)));
		}

		CString sSql = _T("UPDATE T_PROJECT_SETTING SET ");
		CProjectData* pProjectData = CProjectData::GetInstance();
		for(map<CString,CString>::const_iterator itr = pProjectData->m_pProjectSettingMap->begin();itr != pProjectData->m_pProjectSettingMap->end();++itr)
		{
			STRING_T sDataType = _T("");
			CString sFieldName(itr->first);
			sFieldName.MakeLower();
			map<STRING_T,STRING_T>::const_iterator where = oFieldInfoList.find(sFieldName.operator LPCTSTR());
			if(where != oFieldInfoList.end()) sDataType = where->second;
			IsString::ToLower(sDataType);
			if(STRING_T(_T("character varying")) != sDataType) continue;

			sSql += itr->first + _T("='") + itr->second + _T("',");
		}
		sSql.TrimRight(',');
		(*database.session()) << sSql.operator LPCTSTR();

		pProjectData->Save(pDoc->GetCurProject()->name(),prop);

		return ERROR_SUCCESS;
	}
	catch(const std::exception& ex)
	{
		AfxMessageBox(ex.what(),MB_OK|MB_ICONERROR);
	}

	return ERROR_BAD_ENVIRONMENT;
}
