#pragma once

#ifdef	ISUTIL_EXPORTS
#define	ISUTIL_EXT_CLASS	__declspec(dllexport)
#else
#define	ISUTIL_EXT_CLASS	__declspec(dllimport)
#endif

#include <ADO/ADODB.h>
#include <string>
using namespace std;

class CSQLFile
{
public:
	CSQLFile(const string& rFilePath = _T(""));
	~CSQLFile(void);
private:
	string m_rFilePath;
	string m_rTableName;
	vector<string> m_FieldEntry , m_PrimaryKeyEntry;
public:
	int Parse(const string& rFilePath = _T(""));
	string GetCreateString(void) const;
	int GetFieldCount(void) const;
	string GetFieldNameAt(const int& at) const;
	string GetFieldDataAt(const int& at) const;
	bool ExistFieldName(const string& rFieldName);
	int SyncToTable(CADODB& adoDB, const string& rTableName) const;
};
