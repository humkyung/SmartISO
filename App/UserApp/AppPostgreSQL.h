#pragma once

#include "soci.h"

class CAppPostgreSQL
{
	CAppPostgreSQL(const CAppPostgreSQL&);
	CAppPostgreSQL& operator=(const CAppPostgreSQL&);
public:
	//int ExecuteRowSet(const CString& sql);
	CString GetConnectionString() const;
	int SyncTable(const CSQLFile&,const CString&);
	CAppPostgreSQL(const CString&,const CString&,const CString&,const CString&);
	~CAppPostgreSQL(void);
private:
	CString m_sDatabase , m_sUser , m_sPassword , m_sHost;
	soci::session* session_;
};