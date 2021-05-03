// PDSOracle.h: interface for the CPDSOracle class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UserDocData.h"

#include <ado/AdoRemoteDB.h>
#include "NoteTableForOra.h"

#include < map >
using namespace std;

class USER_EXT_CLASS CPDSOracle  
{
public:
	string FindStrMatchCodeNumber(const int& nNoteNum , const int& nCodeNum);
	int FindCodeNumberUsingStr(const int &nNoteNum, const string &str);
	void Clear();
	bool LoadNoteTableFromDataFile(const string &rFilePath);
	void CreateOracleView();
	bool Get_PDTABLE_12_TableNames(vector<CString>& tableNames , CAdoRemoteDB& oraDB);
	void Set( const CString& rServiceName , const CString& rUserId , const CString& rPassword , const CString& rPDSProjectName );

	CPDSOracle();
	virtual ~CPDSOracle();
private:
	CString m_rServiceName;
	CString m_rUserId;
	CString m_rPassword;
	CString m_rPDSProjectName;

	map<int , CNoteTableForOra*> m_noteTableForOra;
};
