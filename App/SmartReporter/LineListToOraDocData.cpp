// LineListToOraDocData.cpp: implementation of the CLineListToOraDocData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <Windows.h>
#include <IsString.h>
#include "SmartReporter.h"
#include "MainFrm.h"
#include "SmartReporterDoc.h"
#include "LineListToOraDocData.h"

#include "Markup/Markup.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/**
	@brief	

	@author	humkyung

	@date	2011.05.07

	@param	iProjectUnit
	@param	sAttribute

	@return	int
**/
STRING_T CCustomReportTable::GetCustomAttribute(const int& iProjectUnit , const STRING_T& sAttribute)
{
	assert(((0 == iProjectUnit) || (1 == iProjectUnit)) && "ProjectUnit is wrong");

	if((0 == iProjectUnit) || (1 == iProjectUnit))
	{
		map<STRING_T,STRING_T>::iterator where = m_pSqlState[iProjectUnit]->m_oAttributes.find(sAttribute);
		if(where != m_pSqlState[iProjectUnit]->m_oAttributes.end()) return (where->second);
	}

	return STRING_T(_T(""));
}


/**
	@brief	

	@author	humkyung

	@date	2011.05.07

	@param	iProjectUnit
	@param	sAttribute

	@return	int
**/
STRING_T CCustomReportTable::ParseCustomAttribute(const STRING_T& sAttributeType , const STRING_T& sValue , CPDSOracle &PDSOracle)
{
	STRING_T sResult;

	if((_T("CALC(NOMINAL_DIAMETER)") == sAttributeType) || (_T("CALC(INSULATION_THICKNESS)") == sAttributeType) || 
		(_T("CALC(NPD)") == sAttributeType))
	{
		sResult = (ATOI_T(sValue.c_str()) < 0) ? _T("0") : sValue;
	}
	else if(_T("CALC(FLUID_CODE)") == sAttributeType)
	{
		sResult = PDSOracle.FindStrMatchCodeNumber(FLUID_CODE_NOTE_NO , ATOI_T(sValue.c_str()));
		sResult = (!sResult.empty()) ? sResult : sValue;
	}
	else if(_T("CALC(INSULATION_PURPOSE)") == sAttributeType)
	{
		sResult = PDSOracle.FindStrMatchCodeNumber(INSULATION_PURPOSE_NOTE_NO , ATOI_T(sValue.c_str()));
		sResult = (!sResult.empty()) ? sResult : sValue;
	}
	else if(_T("CALC(SP_END_PREP)") == sAttributeType)
	{
		if(_T("0") == sValue)
		{
			sResult = _T("NULL");
		}
		else
		{
			STRING_T sTemp = PDSOracle.FindStrMatchCodeNumber( SPECIAL_END_PREP_NOTE_NO  , ATOI_T(sValue.c_str())).c_str();
			sResult = (!sTemp.empty()) ? sTemp : sValue;
		}
	}
	else if(_T("CALC(CP_EASTING)") == sAttributeType)
	{
		const double dValue = ATOF_T(sValue.c_str());
		if(dValue < 0.f)
		{
			CHAR_T szBuf[MAX_PATH + 1] = {'\0',};
			SPRINTF_T(szBuf , _T("%lf") , fabs(dValue));
			sResult = STRING_T(_T("W ")) + szBuf;
		}
		else
		{
			sResult = _T("E ") + sValue;
		}
	}
	else if(_T("CALC(CP_NORTHING)") == sAttributeType)
	{
		const double dValue = ATOF_T(sValue.c_str());
		if(dValue < 0.f)
		{
			CHAR_T szBuf[MAX_PATH + 1] = {'\0',};
			SPRINTF_T(szBuf , _T("%lf") , fabs(dValue));
			sResult = STRING_T(_T("S ")) + szBuf;
		}
		else
		{
			sResult = _T("N ") + sValue;
		}
	}
	else if(_T("CALC(CP_ELEVATION)") == sAttributeType)
	{
		sResult = _T("EL ") + sValue;
	}
	
	return sResult;
}

/**
	@brief	save custom report table

	@author	humkyung

	@date	2011.05.10

	@param	xml

	@return	int
**/
int CCustomReportTable::OnSaveCustomDataReport(CMarkup& xml)
{
	xml.AddAttrib( _T("Name") , m_sName);
	xml.AddAttrib( _T("SqlFile") , m_sSqlFile);
	xml.AddAttrib( _T("ExcelFile") , m_sExcelFile );
	xml.AddAttrib( _T("StartRow") , m_sStartRow);
	xml.AddAttrib( _T("Discipline") , m_sDiscipline);

	return ERROR_SUCCESS;
}

/**
	@brief	parsing sql statement

	@author	humkyung

	@date	2011.05.07

	@param	sSqlFilePath

	@return	int
**/
int CSqlState::ParseSqlState(const STRING_T& sSqlFilePath)
{
	CMarkup xml;
	if(true == xml.Load(sSqlFilePath.c_str()))
	{
		MCD_STR sLoadResult = xml.GetResult();
		
		if(true == xml.FindElem(MCD_T("SQL")))
		{
			int iCount = 0;
			while ( 1 )
			{
				STRING_T sName = xml.GetAttribName(iCount++);
				if ( sName.empty() ) break;
				STRING_T sValue = xml.GetAttrib( sName );

				m_oAttributes.insert(make_pair(sName , sValue));
			}

			m_sSqlStatement = xml.GetData();
		}
	}

	return ERROR_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLineListToOraDocData::CLineListToOraDocData() : m_sProjectUnit( _T("METRIC") )
{

}

CLineListToOraDocData::~CLineListToOraDocData()
{
	try
	{
		for(list<CCustomReportTable*>::iterator itr = m_lstCustomReportTable.begin();itr != m_lstCustomReportTable.end();++itr)
		{
			delete (*itr);
		}
		m_lstCustomReportTable.clear();
	}
	catch(...)
	{
	}
}

/**
	@brief	get CLineListToOraDocData instance

	@author	humkyung

	@date	????.??.??

	@param

	@return	CLineListToOraDocData&
**/
CLineListToOraDocData& CLineListToOraDocData::GetInstance()
{
	static CLineListToOraDocData __instance__;

	return (__instance__);
}

/**
**/
STRING_T CLineListToOraDocData::GetIniFilePath() const
{
	return m_sIniFilePath;
}

/**
**/
STRING_T CLineListToOraDocData::GetCurProjectName() const
{
	return m_rCurProjectName;
}

/**
	@brief	

	@author	humkyung

	@date	????.??.??

	@param	rIniFilePath

	@return
**/
void CLineListToOraDocData::LoadIni(const STRING_T &rIniFilePath)
{
	m_sIniFilePath = rIniFilePath;

	CString APP( _T("General") );
	TCHAR szBuf[MAX_PATH + 1] = {'\0' ,};
	
	/// get project name - 2012.05.03 added by humkyung
	if(GetPrivateProfileString(APP , _T("Name") , NULL , szBuf , MAX_PATH , m_sIniFilePath.c_str()))
	{
		m_sProjectName = szBuf;
	}
	/// up to here

	APP = _T("Oracle Setting");
	if(GetPrivateProfileString(APP , _T("PDS Project Name") , NULL , szBuf , MAX_PATH , m_sIniFilePath.c_str()))
	{
		m_rOraProjectName = szBuf;
	}else	m_rOraProjectName.Empty();
	
	if(GetPrivateProfileString(APP , _T("Service Name") , NULL , szBuf , MAX_PATH , m_sIniFilePath.c_str()))
	{
		m_rOraServiceName = szBuf;
	}else	m_rOraServiceName.Empty();
	
	if(GetPrivateProfileString(APP , _T("User Name") , NULL , szBuf , MAX_PATH , m_sIniFilePath.c_str()))
	{
		m_rOraUserName = szBuf;
	}else	m_rOraUserName.Empty();
	
	if(GetPrivateProfileString(APP , _T("Password") , NULL , szBuf , MAX_PATH , m_sIniFilePath.c_str()))
	{
		m_rOraUserPassword = szBuf;
	}else	m_rOraUserPassword.Empty();
	
	APP = _T("Iso Edit Setting");
	//! 2011.04.26 - added by humkyung
	m_sProjectUnit = _T("INCH");
	if(GetPrivateProfileString(APP , _T("UNIT") , NULL , szBuf , MAX_PATH , m_sIniFilePath.c_str()))
	{
		m_sProjectUnit = szBuf;
	}
	//!

	///
	if(GetPrivateProfileString(APP , _T("Standard Note File") , NULL , szBuf , MAX_PATH , m_sIniFilePath.c_str()))
	{
		m_rStdNoteFilePath = szBuf;
	}else	m_rStdNoteFilePath.Empty();
}

/**
	@brief	load custom data report list from xml file

	@author	humkyung

	@date	2011.05.04

	@param	

	@return	void
**/
void CLineListToOraDocData::LoadCustomDataReportList()
{
	try
	{
		CMarkup xml;
		CString sFilePath = GetAppDataPath() + _T("\\SmartISO\\SmartReporter\\Reports.xml");
		if(true == xml.Load(sFilePath.operator LPCTSTR()))
		{
			MCD_STR sLoadResult = xml.GetResult();
			
			if(true == xml.FindElem(MCD_T("SmartReporter")))
			{
				bool bRes = xml.IntoElem();
				
				while(xml.FindElem(MCD_T("Report")))
				{
					CCustomReportTable* pCustomReportTable = new CCustomReportTable;
					if(NULL != pCustomReportTable)
					{
						pCustomReportTable->m_sName = xml.GetAttrib(MCD_T("Name"));
						pCustomReportTable->m_sSqlFile = xml.GetAttrib(MCD_T("SqlFile"));
						pCustomReportTable->m_sExcelFile = xml.GetAttrib(MCD_T("ExcelFile"));
						pCustomReportTable->m_sStartRow = xml.GetAttrib(MCD_T("StartRow"));
						pCustomReportTable->m_sDiscipline = xml.GetAttrib(MCD_T("Discipline"));
						
						{
							const STRING_T sSqlFilePath = STRING_T(GetAppDataPath().operator LPCTSTR()) + _T("\\SmartISO\\SmartReporter\\METRIC\\") + pCustomReportTable->m_sSqlFile;
							pCustomReportTable->m_pSqlState[CCustomReportTable::METRIC]->ParseSqlState(sSqlFilePath);
						}
						{
							const STRING_T sSqlFilePath = STRING_T(GetAppDataPath().operator LPCTSTR()) + _T("\\SmartISO\\SmartReporter\\INCH\\") + pCustomReportTable->m_sSqlFile;
							pCustomReportTable->m_pSqlState[CCustomReportTable::INCH]->ParseSqlState(sSqlFilePath);
						}

						m_lstCustomReportTable.push_back(pCustomReportTable);
					}
				}
			}
		}
	}
	catch(const exception& ex)
	{
		AfxMessageBox(LPCTSTR(ex.what()) , MB_OK);
	}
}

/**
	@brief	save custom data report list to xml file

	@author	humkyung

	@date	2011.05.10

	@param	

	@return	void
**/
void CLineListToOraDocData::SaveCustomDataReportList()
{
	try
	{
		CMarkup xml;
		xml.SetDoc( _T("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n") );

		xml.AddElem( _T("SmartReporter") ); 
		for(list<CCustomReportTable*>::iterator itr = m_lstCustomReportTable.begin();itr != m_lstCustomReportTable.end();++itr)
		{
			xml.AddChildElem( _T("Report") );
			xml.IntoElem();
			(*itr)->OnSaveCustomDataReport(xml);
			xml.OutOfElem();
		}
		
		CString sFilePath = GetAppDataPath() + _T("\\SmartISO\\SmartReporter\\Reports.xml");
		xml.Save(sFilePath.operator LPCTSTR());
	}
	catch(const exception& ex)
	{
		AfxMessageBox(LPCTSTR(ex.what()) , MB_OK);
	}
}

/**
	@brief	

	@author	humkyung

	@date	????.??.??

	@param	rAppDataFilePath

	@return	bool
**/
bool CLineListToOraDocData::LoadAppData(const STRING_T &rAppDataFilePath)
{
	bool bRes = m_AppData.LoadData(rAppDataFilePath);
	return bRes;
}

/**	
	@brief	관리자 암호를 구한다.

	@author	humkyung
**/
STRING_T CLineListToOraDocData::GetAdminPassword() const
{
	return m_AppData.GetAdminPassword();
}

/**	
	@brief	올바른 사용자 인지를 확인한다.

	@author	humkyung
**/
bool CLineListToOraDocData::IsCorrectUser(const STRING_T &rUser, const STRING_T &rPassword)
{
	CString rString;
	rString.LoadString(IDS_ADMINISTRATOR);

	if( rUser == rString.operator LPCTSTR() )
	{
		return ( m_AppData.GetAdminPassword() == rPassword );
	}
	else
	{
		rString.LoadString(IDS_USER);
		return ( rUser == rString.operator LPCTSTR() );
	}

	return false;
}

/**	\brief	사용자 이름을 설정한다.
**/
void CLineListToOraDocData::SetUserName(const STRING_T &rUserName)
{
	m_rCurUserName = rUserName;
}

/**	\brief	관리자 암호를 설정한다.
**/
void CLineListToOraDocData::SetAdminPassword(const STRING_T &rAdminPassword)
{
	m_AppData.SetAdminPassword( rAdminPassword );
	
	CMainFrame* pFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	CSmartReporterDoc* pDoc = static_cast<CSmartReporterDoc*>(pFrame->GetActiveDocument());
	const STRING_T rAppDataFilePath = GetExecPath() + _T("\\LineListToOra.dat");
	m_AppData.SaveData( rAppDataFilePath );
}

/**
	@brief	return current user name

	@author	humkyung

	@date	????.??.??
**/
STRING_T CLineListToOraDocData::GetUserName() const
{
	return _T("Administrator");
}

/**	
	@brief	오라클 연결 문자열을 구한다.

	@author	humkyung
**/
CString CLineListToOraDocData::GetConnString() const
{
#ifdef _DEBUG
	CString rConnString = CString(_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=e:\\PROJECT\\SolutionWare\\Projects\\2008.LCMS\\Bin\\TEST.MDB"));
#else
	CString	rConnString = _T("Provider=MSDAORA;User ID=") + m_rOraUserName + _T(";Password=") + m_rOraUserPassword + _T(";Data Source=") + m_rOraServiceName;
#endif

	return rConnString;
}

CString CLineListToOraDocData::GetOraServiceName() const
{
	return m_rOraServiceName;
}

CString CLineListToOraDocData::GetOraUserName() const
{
	return m_rOraUserName;
}

CString CLineListToOraDocData::GetOraProjectName() const
{
	return m_rOraProjectName;
}

CString CLineListToOraDocData::GetOraUserPassword() const
{
	return m_rOraUserPassword;
}

CString CLineListToOraDocData::GetStdNoteFilePath() const
{
	return m_rStdNoteFilePath;
}

/*
	@brief

	@author	humkyung

	@date	????.??.??

	@param

	@return
*/
CString CLineListToOraDocData::GetProjectUnit()
{
	static const STRING_T APP( _T("General") );
	TCHAR szBuf[MAX_PATH + 1] = {'\0' ,};
	
	m_sProjectUnit = _T("INCH");
	if(GetPrivateProfileString(APP.c_str() , _T("Project Unit") , NULL , szBuf , MAX_PATH , m_sIniFilePath.c_str()))
	{
		m_sProjectUnit = szBuf;
	}
	
	return m_sProjectUnit;
}

/*
	@brief	return exection path

	@author	humkyung

	@date	2011.04.30

	@param

	@return	string
*/
STRING_T CLineListToOraDocData::GetExecPath()
{
	STRING_T sExecPath = _T("");

	TCHAR szBuf[MAX_PATH]={'\0' ,};
	::GetModuleFileName( NULL , szBuf , MAX_PATH );
	sExecPath.assign( szBuf );
	STRING_T::size_type at = sExecPath.find_last_of( '\\' );
	if( at != string::npos )
	{
		sExecPath = sExecPath.substr(0 , at );
	}

	return sExecPath;
}

/*
	@brief	check if custom report which's name is sName exists

	@author	humkyung

	@date	2011.05.10

	@param	sName

	@return	bool
*/
bool CLineListToOraDocData::ExistCustomReportTableWith(STRING_T sName)
{
	for(list<CCustomReportTable*>::iterator itr = m_lstCustomReportTable.begin();itr != m_lstCustomReportTable.end();++itr)
	{
		if((*itr)->m_sName == sName) return true;
	}

	return false;
}

/*
	@brief	add custom report table

	@author	humkyung

	@date	2011.05.10

	@param	pCustomReportTable

	@return	int
*/
int CLineListToOraDocData::AddCustomReportTable(CCustomReportTable* pCustomReportTable)
{
	assert(pCustomReportTable && "pCustomReportTable is NULL");

	if(pCustomReportTable)
	{
		m_lstCustomReportTable.push_back(pCustomReportTable);

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

/*
	@brief	delete custom report table which's name is "sName"

	@author	humkyung

	@date	2011.05.10

	@param	sName

	@return	int
*/
int CLineListToOraDocData::DeleteCustomReportTableWith(CString& sName)
{
	for(list<CCustomReportTable*>::iterator itr = m_lstCustomReportTable.begin();itr != m_lstCustomReportTable.end();++itr)
	{
		if((*itr)->m_sName == sName.operator LPCTSTR())
		{
			delete (*itr);
			m_lstCustomReportTable.erase(itr);
			break;
		}
	}

	return ERROR_SUCCESS;
}