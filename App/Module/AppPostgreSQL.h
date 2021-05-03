#pragma once

#include <util/ado/SQLFile.h>
#include "soci.h"

#ifdef MODULE_EXPORTS
	#define SMARTISO_MODULE_EXT_CLASS	__declspec(dllexport)
#else
	#define SMARTISO_MODULE_EXT_CLASS	__declspec(dllimport)
#endif

class SMARTISO_MODULE_EXT_CLASS CAppPostgreSQL
{
	CAppPostgreSQL(const CAppPostgreSQL&);
	CAppPostgreSQL& operator=(const CAppPostgreSQL&);
public:
	int open();
	soci::session* session();
	int SyncTable(const CSQLFile&,const CString&);
	CAppPostgreSQL(const CString&,const CString&,const CString&,const CString&);
	~CAppPostgreSQL(void);
protected:
	CString GetConnectionString() const;
private:
	CString m_sDatabase , m_sUser , m_sPassword , m_sHost;
	soci::session* session_;
};