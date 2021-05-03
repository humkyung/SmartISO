// ProjectData.cpp: implementation of the CProjectData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Module.h"
#include "ProjectData.h"
#include "ADO/ADODB.h"
#include "Tokenizer.h"
#include "AppPostgreSQL.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProjectData::CProjectData()
{
	m_pProjectSettingMap = new map<CString,CString>;
	m_pRevisionStageMap  = new map<int,CRevisionStage>;	/// 2011.08.20 added by humkyung
	m_pDrawingSetup = new map<STRING_T,STRING_T>;	/// 2015.11.03 added by humkyung
}

CProjectData::~CProjectData()
{
	try
	{
		if(NULL != m_pProjectSettingMap)
		{
			SAFE_DELETE(m_pProjectSettingMap);
		}

		/// 2011.08.20 added by humkyung
		if(NULL != m_pRevisionStageMap)
		{
			SAFE_DELETE(m_pRevisionStageMap);
		}

		if(NULL != m_pDrawingSetup) SAFE_DELETE(m_pDrawingSetup);
		/// up to here
	} catch(...) { }
}

CProjectData* CProjectData::GetInstance()
{
	static CProjectData __instance;

	return &(__instance);
}

/**
	@brief	load project setting data from database
	@authro	humkyung
	@date	2011.08.06
	@return
*/
int CProjectData::Load(const CString& name,const CProject::DatabaseProp& prop)
{
	CAppPostgreSQL database(name , prop.Host.c_str() , prop.User.c_str() , prop.Password.c_str());
	{
		m_pProjectSettingMap->clear();

		{
			STRING_T sColNames;
			{
				/// select all column names which's data type is character
				soci::rowset<soci::row> rs(database.session()->prepare << "select column_name,data_type from information_schema.columns where table_schema='public' and table_name='t_project_setting'");
				for(soci::rowset<soci::row>::const_iterator itr = rs.begin();itr != rs.end();++itr)
				{
					const STRING_T sColName = itr->get<STRING_T>(_T("column_name"));
					const STRING_T sDataType= itr->get<STRING_T>(_T("data_type"));
					if(0 == sDataType.find(_T("character")))
					{
						if(!sColNames.empty()) sColNames += _T(",");
						sColNames += sColName;
					}
				}
				/// up to here
			}

			int row=0;
			STRING_T sSql = _T("SELECT ") + sColNames + _T(" FROM T_PROJECT_SETTING");

			soci::rowset<soci::row> rs(database.session()->prepare << sSql);
			for(soci::rowset<soci::row>::const_iterator itr = rs.begin();itr != rs.end();++itr,++row)
			{
				for(int col = 0;col < int(itr->size());++col)
				{
					const soci::column_properties& props = itr->get_properties(col);
					const STRING_T& sFieldName = props.get_name();
					const soci::data_type dataType = props.get_data_type();
					try
					{
						const STRING_T sValue = itr->get<STRING_T>(col);
						m_pProjectSettingMap->insert(make_pair(sFieldName.c_str(),sValue.c_str()));
					} catch(...){}
				}
			}
		}

		/// load revision stage - 2011.08.20 added by humkyung
		{
			int row = 0;
			STRING_T sSql = _T("SELECT * FROM T_REVISION_STAGE ORDER BY REVISION_NO");
			soci::rowset<soci::row> rs(database.session()->prepare << sSql);
			for(soci::rowset<soci::row>::const_iterator itr = rs.begin();itr != rs.end();++itr,++row)
			{
				try
				{
					CRevisionStage rs;
					rs.m_sDefine = itr->get<STRING_T>(_T("from_grade_define"));
					rs.m_nNo = itr->get<int>(_T("revision_no"));
					rs.m_sText = itr->get<STRING_T>(_T("revision_text"));
					m_pRevisionStageMap->insert(make_pair(rs.m_nNo,rs));
				}catch(...){}
			}
		}
		/// up to here

		/// load drawing setup - 2015.11.03 added by humkyung
		m_pDrawingSetup->clear();
		{
			int row = 0;
			STRING_T sSql = _T("select * from t_iso_drawing_setup");
			soci::rowset<soci::row> rs(database.session()->prepare << sSql);
			for(soci::rowset<soci::row>::const_iterator itr = rs.begin();itr != rs.end();++itr,++row)
			{
				try
				{
					const STRING_T sName = itr->get<STRING_T>(_T("name"));
					const STRING_T sValue = itr->get<STRING_T>(_T("value"));
					m_pDrawingSetup->insert(make_pair(sName,sValue));
				}catch(...){}
			}
		}
		/// up to here

		return ERROR_SUCCESS;
	}

	return ERROR_BAD_ENVIRONMENT;
}

/**
	@brief	save data into database
	@authro	humkyung
	@date	2011.07.17
	@return
*/
int CProjectData::Save(const CString& name,const CProject::DatabaseProp& prop)
{
	CAppPostgreSQL database(name , prop.Host.c_str() , prop.User.c_str() , prop.Password.c_str());
	{
		const CString sIssueDateFormatNo = GetIssueDateFormatNo();
		const CString sIssueDateDelimiter= GetIssueDateDelimiter();

		CString sSql;
		try
		{
			sSql.Format(_T("INSERT INTO T_PROJECT_SETTING(ISSUE_DATE_FORMAT_NO,ISSUE_DATE_DELIMITER) SELECT '%s','%s' WHERE NOT EXISTS(SELECT 1 FROM T_PROJECT_SETTING)"),
				sIssueDateFormatNo,sIssueDateDelimiter);
			(*database.session()) << sSql.operator LPCTSTR();
			
			sSql.Format(_T("UPDATE T_PROJECT_SETTING SET ISSUE_DATE_FORMAT_NO='%s',ISSUE_DATE_DELIMITER='%s'"),sIssueDateFormatNo,sIssueDateDelimiter);
			(*database.session()) << sSql.operator LPCTSTR();

			for(map<STRING_T,STRING_T>::const_iterator itr = m_pDrawingSetup->begin();itr != m_pDrawingSetup->end();++itr)
			{
				sSql.Format(_T("insert into t_iso_drawing_setup(name,value) select '%s','%s' where not exists(select 1 from t_iso_drawing_setup where name='%s')"),
					itr->first.c_str(),itr->second.c_str(),itr->first.c_str());
				(*database.session()) << sSql.operator LPCTSTR();
				
				sSql.Format(_T("update t_iso_drawing_setup set value='%s' where name='%s'"),itr->second.c_str(),itr->first.c_str());
				(*database.session()) << sSql.operator LPCTSTR();
			}
		}
		catch(const std::exception& ex)
		{
			AfxMessageBox(ex.what(),MB_OK|MB_ICONERROR);
		}
	}

	return ERROR_SUCCESS;
}

/**
	@brief	get display issue date
	@authro	humkyung
	@date	2011.08.06
	@return
*/
CString CProjectData::GetDisplayIssueDateFrom(const CString& sIssueDate) const
{
	static const CString sMonthAbbrList[] = 
	{ 
		_T("JAN") , _T("FEB") , _T("MAR") , _T("APR") , _T("MAY") , 
		_T("JUN") , _T("JUL") , _T("AUG") , _T("SEP") , _T("OCT") , 
		_T("NOV") , _T("DEC")
	};

	CString res;

	vector<STRING_T> oResult;
	CTokenizer<CIsFromString>::Tokenize(oResult , sIssueDate.operator LPCTSTR() , CIsFromString(_T("-")));
	if(3 == oResult.size())
	{
		int iYear  = atoi(oResult[0].c_str());
		int iMonth = atoi(oResult[1].c_str());
		int iDay   = atoi(oResult[2].c_str());

		const CString sIssueDateFormatNo = GetIssueDateFormatNo();
		const CString sIssueDateDelimiter= GetIssueDateDelimiter();
	
		if(_T("1") == sIssueDateFormatNo)
		{
			res.Format(_T("%04d%s%02d%s%02d") , iYear , sIssueDateDelimiter , iMonth , sIssueDateDelimiter , iDay);
		}
		else if(_T("2") == sIssueDateFormatNo)
		{
			res.Format(_T("%02d%s%02d%s%04d") , iMonth , sIssueDateDelimiter , iDay , sIssueDateDelimiter , iYear);
		}
		/// 2012.08.16 added by humkyung
		else if(_T("4") == sIssueDateFormatNo)
		{
			res.Format(_T("%s%s%02d%s'%02d") , sMonthAbbrList[iMonth - 1] , sIssueDateDelimiter , iDay , sIssueDateDelimiter , iYear%100);
		}
		/// up to here
		else
		{
			res.Format(_T("%02d%s%s%s%04d") , iDay , sIssueDateDelimiter , sMonthAbbrList[iMonth - 1] , sIssueDateDelimiter , iYear);
		}
	}

	return res;
}

/**
	@brief	get issue date delimiter
	@authro	humkyung
	@date	2011.07.17
	@return
*/
CString CProjectData::GetIssueDateDelimiter() const
{
	CString sIssueDateDelimiter(_T("-"));

	map<CString,CString>::iterator where = m_pProjectSettingMap->find(_T("ISSUE_DATE_DELIMITER"));
	if(where != m_pProjectSettingMap->end())
	{
		if(!where->second.IsEmpty()) sIssueDateDelimiter = (where->second);
	}
	
	return sIssueDateDelimiter;
}

/**
	@brief	get issue date format no
	@authro	humkyung
	@date	2011.07.17
	@return
*/
CString CProjectData::GetIssueDateFormatNo() const
{
	CString sDateFormatNo(_T("1"));

	map<CString,CString>::iterator where = m_pProjectSettingMap->find(_T("ISSUE_DATE_FORMAT_NO"));
	if(where != m_pProjectSettingMap->end())
	{
		if(!where->second.IsEmpty()) sDateFormatNo = (where->second);
	}
	
	return sDateFormatNo;
}

/**
	@brief	parse issue date and get year,month,day

	@authro	humkyung

	@date	2011.08.01

	@return
*/
int CProjectData::ParseIssueDate(int& iYear , int& iMonth , int& iDay , const CString& sIssueDate , const CString& sDelimiter) const
{
	static const STRING_T sMonthAbbrList[] = 
	{ 
		_T("JAN") , _T("FEB") , _T("MAR") , _T("APR") , _T("MAY") , 
		_T("JUN") , _T("JUL") , _T("AUG") , _T("SEP") , _T("OCT") , 
		_T("NOV") , _T("DEC")
	};

	const CString sIssueDateFormatNo = GetIssueDateFormatNo();
	//const string  sIssueDateDelimiter= GetIssueDateDelimiter().operator LPCTSTR();
	
	vector<STRING_T> oResult;
	CTokenizer<CIsFromString>::Tokenize( oResult , sIssueDate.operator LPCTSTR() , CIsFromString(sDelimiter.operator LPCTSTR()));
	if(3 == oResult.size())
	{
		/*if(_T("1") == sIssueDateFormatNo)
		{*/
			iYear = atoi( oResult[0].c_str() );
			iMonth= atoi( oResult[1].c_str() );
			iDay = atoi( oResult[2].c_str() );
		/*}
		else if(_T("2") == sIssueDateFormatNo)
		{
			iMonth= atoi( oResult[0].c_str() );
			iDay = atoi( oResult[1].c_str() );
			iYear = atoi( oResult[2].c_str() );
		}
		/// 2012.12.30 added by humkyung
		else if(_T("4") == sIssueDateFormatNo)
		{
			const STRING_T* p = find(sMonthAbbrList , sMonthAbbrList + sizeof(sMonthAbbrList)/sizeof(sMonthAbbrList[0]) , oResult[0]);
			iMonth = p - sMonthAbbrList + 1;
			if((iMonth < 1) || (iMonth > 12)) return ERROR_BAD_ENVIRONMENT;

			iDay  = atoi( oResult[1].c_str() );			
			iYear = atoi( oResult[2].c_str() );
		}
		/// up to here
		else
		{
			iDay  = atoi( oResult[0].c_str() );
			///iMonth=atoi( oResult[1].c_str() );

			const STRING_T* p = find(sMonthAbbrList , sMonthAbbrList + sizeof(sMonthAbbrList)/sizeof(sMonthAbbrList[0]) , oResult[1]);
			iMonth = p - sMonthAbbrList + 1;
			if((iMonth < 1) || (iMonth > 12)) return ERROR_BAD_ENVIRONMENT;
			iYear = atoi( oResult[2].c_str() );
		}*/

		return ERROR_SUCCESS;
	}

	return ERROR_BAD_ENVIRONMENT;
}

/**
	@brief	get issue date format

	@authro	humkyung

	@date	2011.07.17

	@return
*/
CString CProjectData::GetIssueDateFormat() const
{
	CString sDateFormat(_T("%Y-%m-%d"));

	const CString sDelimiter = GetIssueDateDelimiter();
	const CString sFormatNo  = GetIssueDateFormatNo();
	if(_T("1") == sFormatNo)
	{
		sDateFormat = _T("%Y") + sDelimiter + _T("%m") + sDelimiter + _T("%d");
	}
	else if(_T("2") == sFormatNo)
	{
		sDateFormat = _T("%m") + sDelimiter + _T("%d") + sDelimiter + _T("%Y");
	}
	//! 2011.08.01 - added by humkyung
	else if(_T("3") == sFormatNo)
	{
		sDateFormat = _T("%d") + sDelimiter + _T("%b") + sDelimiter +  _T("%Y");
	}
	//! up to here
	
	return sDateFormat;
}

/**
	@brief	set issue date format no
	@authro	humkyung
	@date	2011.07.17
	@return
*/
int CProjectData::SetIssueDateFormatNo(const CString& sIssueDateFormatNo)
{
	map<CString,CString>::iterator where = m_pProjectSettingMap->find(_T("ISSUE_DATE_FORMAT_NO"));
	if(where != m_pProjectSettingMap->end())
	{
		where->second = sIssueDateFormatNo;
	}

	return ERROR_SUCCESS;
}

/**
	@brief	set issue date format no
	@authro	humkyung
	@date	2011.07.17
	@return
*/
int CProjectData::SetIssueDateDelimiter(const CString& sIssueDateDelimiter)
{
	map<CString,CString>::iterator where = m_pProjectSettingMap->find(_T("ISSUE_DATE_DELIMITER"));
	if(where != m_pProjectSettingMap->end())
	{
		where->second = sIssueDateDelimiter;
	}

	return ERROR_SUCCESS;
}

/**	
	@brief	get revision stage from revision no

	@author	HumKyung.BAEK

	@date	2010.05.25
**/
int CProjectData::GetRevisionStage(CRevisionStage& _RevisionStage , const CString& rRevNo)
{
	if(m_pRevisionStageMap)
	{
		map<int,CRevisionStage>::iterator where = m_pRevisionStageMap->find(atoi(rRevNo));
		if(where != m_pRevisionStageMap->end())
		{
			_RevisionStage = where->second;
			return ERROR_SUCCESS;
		}
	}

	return ERROR_BAD_ENVIRONMENT;
}

/**
	@brief	return drawing setup
	@author	humkyung
	@date	2015.11.03
*/
map<STRING_T,STRING_T>* CProjectData::GetDrawingSetup()
{
	return m_pDrawingSetup;
}