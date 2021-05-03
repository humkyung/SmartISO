#include "StdAfx.h"
#include <ado/SQLFile.h>
#include "AppPostgreSQL.h"

#include "soci.h"
#include "soci-postgresql.h"

CAppPostgreSQL::CAppPostgreSQL(const CString& sDatabase,const CString& sHost,const CString& sUser,const CString& sPassword)
{
	m_sDatabase = sDatabase;
	m_sUser = sUser;
	m_sPassword = sPassword;
	m_sHost = sHost;

	session_ = NULL;
}

CAppPostgreSQL::CAppPostgreSQL(const CAppPostgreSQL& rhs)
{
}

CAppPostgreSQL::~CAppPostgreSQL(void)
{
	try
	{
		if(NULL != session_) SAFE_DELETE(session_);
	}
	catch(...)
	{
	}
}

CAppPostgreSQL& CAppPostgreSQL::operator =(const CAppPostgreSQL& rhs)
{
	return (*this);
}

/**
	@brief	return session instance
	@author	humkyung
	@date	2015.09.30
*/
soci::session* CAppPostgreSQL::session()
{
	if(NULL == session_)
	{
		session_ = new soci::session(*soci::factory_postgresql() , GetConnectionString().operator LPCTSTR());
	}

	return session_;
}

/**
	@brief	establish to connect database
	@author	humkyung
	@date	2015.10.01
*/
int CAppPostgreSQL::open()
{
	try
	{
		session()->open(this->GetConnectionString().operator LPCTSTR());
		return ERROR_SUCCESS;
	}
	catch(const std::exception& ex)
	{
		int d = 1;
	}

	return ERROR_BAD_ENVIRONMENT;
}

/**
	@brief	return connection string
	@author	humkyung
	@date	2015.09.29
*/
CString CAppPostgreSQL::GetConnectionString() const
{
	CString res;
	res.Format(_T("dbname=%s user=%s password=%s hostaddr=%s") , m_sDatabase , m_sUser , m_sPassword , m_sHost);
	return res;
}

/**
	@brief	synchronize table with given sql file definition
	@author	humkyung
	@date	2015.09.29
*/
int CAppPostgreSQL::SyncTable(const CSQLFile& aSqlFile,const CString& sTableName)
{
	try
	{
		soci::session* database = session();
		const STRING_T sql = aSqlFile.GetCreateString();
		(*database) << sql;
		CString str(sTableName);
		str.MakeLower();
		soci::rowset<soci::row> rs(database->prepare << "select column_name,data_type from information_schema.columns where table_schema='public' and table_name='" << str.operator LPCSTR() << "'");

		vector<CString> aNeedToAddColumns;
		const int iCols = aSqlFile.GetColumnCount();
		for(int col=0;col < iCols;++col)
		{
			const STRING_T sColName = aSqlFile.GetFieldNameAt(col);
			bool bFound = false;
			for(soci::rowset<soci::row>::const_iterator itr = rs.begin();itr != rs.end();++itr)
			{
				const STRING_T column_name = itr->get<STRING_T>(_T("column_name"));
				if(0 == STRICMP_T(sColName.c_str() , column_name.c_str()))
				{
					bFound = true;
					break;
				}
			}

			if(false == bFound)
			{
				CString str;
				str.Format(_T("ALTER TABLE %s ADD COLUMN %s %s") , sTableName , aSqlFile.GetFieldNameAt(col).c_str() , aSqlFile.GetFieldTypeAt(col).c_str());
				str.MakeLower();
				aNeedToAddColumns.push_back(str);
			}
		}

		for(vector<CString>::const_iterator itr = aNeedToAddColumns.begin();itr != aNeedToAddColumns.end();++itr)
		{
			(*database) << itr->operator LPCTSTR();
		}

		return ERROR_SUCCESS;
	}
	catch(const exception& ex)
	{
		int d = 1;
	}

	return ERROR_BAD_ENVIRONMENT;
}