// PDSOracle.cpp: implementation of the CPDSOracle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PDSOracle.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPDSOracle::CPDSOracle()
{

}

CPDSOracle::~CPDSOracle()
{
	try
	{
		Clear();
	}
	catch(...)
	{
	}
}

/**	\brief	The CPDSOracle::Set function

	\param	rServiceName	a parameter of type const CString&
	\param	rUserId	a parameter of type const CString&
	\param	rPassword	a parameter of type const CString&
	\param	rPDSProjectName	a parameter of type const CString&

	\return	void	
*/
void CPDSOracle::Set( const CString& rServiceName , const CString& rUserId , const CString& rPassword , const CString& rPDSProjectName )
{
	m_rServiceName = rServiceName;
	m_rUserId = rUserId;
	m_rPassword = rPassword;
	m_rPDSProjectName = rPDSProjectName;
}

/**	
	@brief	PDTABLE_12로 시작하는 테이블들의 이름을 구한다.

	@author	humkyung

	@param	tableNames	a parameter of type vector<CString>&
	@param	oraDB	a parameter of type CAdoRemoteDB&

	@return	bool	
*/
bool CPDSOracle::Get_PDTABLE_12_TableNames(vector<CString>& tableNames , CAdoRemoteDB& oraDB)
{
	tableNames.clear();

	///CString rOracleTableQury = "SELECT OBJECT_NAME FROM USER_OBJECTS WHERE OBJECT_TYPE='TABLE' AND OBJECT_NAME LIKE 'PDTABLE_12_%'";
	static const CString rOracleTableQueryFormat("SELECT TABLE_NAME FROM DBA_TABLES WHERE OWNER='DD_%s'");
	
	CString rOracleTableQuery;
	rOracleTableQuery.Format( rOracleTableQueryFormat , m_rPDSProjectName );
	rOracleTableQuery += " AND TABLE_NAME LIKE 'PDTABLE_12_%'";

#ifdef	_DEBUG
	TCHAR szBuf[MAX_PATH + 1] = {'\0',};
	GetTempPath(MAX_PATH , szBuf);
	CString rTempFolder(szBuf);
	if(_T("\\") != rTempFolder.Right(1)) rTempFolder += _T("\\");
	ofstream ofile(rTempFolder + _T("TableNames.txt"));
	ofile << rOracleTableQuery.operator LPCTSTR() << std::endl;
#endif
	if(TRUE == oraDB.OpenQuery(rOracleTableQuery.operator LPCTSTR()))
	{
		LONG lRecordCount = 0L;
		oraDB.GetRecordCount(&lRecordCount);
		
		STRING_T rValue;
		for(int i = 0;i < lRecordCount;++i)
		{
			oraDB.GetFieldValue(i,"TABLE_NAME",&rValue);
#ifdef	_DEBUG
			ofile << rValue << std::endl;
#endif
			tableNames.push_back(rValue.c_str());
		}

		return true;
	}
	else
	{
		CString rString;
		rString.Format( "GET_PDTABLE_12 Open Query Error : %s-%d" , __FILE__ , __LINE__ );
		AfxMessageBox( rString );
	}
#ifdef	_DEBUG
	ofile.close();
#endif

	return false;
}

ofstream __STNOTE__;

/**	
	@brief	NoteNum에 해당하는 NOTE TABLE을 로드한다.

	@author	humkyung

	@param	nNoteNum	a parameter of type const int &

	@remarks\n
	PDTABLE_NOTE_$nNoteNum

	@return	void	
*/
bool CPDSOracle::LoadNoteTableFromDataFile(const string &rFilePath)
{
	STRING_T rStdFilePath = rFilePath;
	ifstream ifile;
	string line;
	ifile.open(rStdFilePath.c_str());
	if(!ifile.is_open())
	{
		AfxMessageBox("StdNote파일이 없습니다.");
		return false;
	}
	
#ifdef	_DEBUG
	TCHAR szBuf[MAX_PATH + 1] = {'\0',};
	GetTempPath(MAX_PATH , szBuf);
	CString sTempFolder(szBuf);
	if(_T("\\") != sTempFolder.Right(1)) sTempFolder += _T("\\");
	__STNOTE__.open(sTempFolder + _T("STNOTE.TXT"));
#endif
	while(ifile.good() && !ifile.eof())
	{
		getline(ifile, line);
		string rFindName;
		int nNoteNum=0;
		if(-1 != line.find("Standard Note Type"))
		{
			CNoteTableForOra* pNoteTable = NULL;
			
			int at = line.find("Standard Note Type");
			int len = line.length();
			int temp = len - ( at + strlen("Standard Note Type"));
			rFindName = line.substr((at + strlen("Standard Note Type")) , temp);
			sscanf(rFindName.c_str(), "%d" ,&nNoteNum);
			
			__STNOTE__ << nNoteNum << std::endl;
			map<int , CNoteTableForOra*>::iterator where = m_noteTableForOra.find(nNoteNum);
			if(where == m_noteTableForOra.end())
			{
				pNoteTable = new CNoteTableForOra;
				if(pNoteTable)
				{
					CString rTextString = line.c_str();
					pNoteTable->ParseColumnIndex(rTextString);
					pNoteTable->Read(ifile);
					m_noteTableForOra[nNoteNum] = pNoteTable;
				}
			}
			else
			{
				pNoteTable = where->second;
				pNoteTable->Read(ifile);
			}
		}
	}

#ifdef	_DEBUG
	__STNOTE__.close();
#endif

	return true;
}

/**	
	@brief	할당한 메모리들을 해제시킨다.
	@author	humkyung
	@return	void	
*/
void CPDSOracle::Clear()
{
	for( map<int , CNoteTableForOra*>::iterator itr = m_noteTableForOra.begin();itr != m_noteTableForOra.end();++itr)
	{
		delete (itr->second);
	}
	m_noteTableForOra.clear();
}

/**	
	@brief	문자열에 해당하는 코드 숫자를 구한다.
	@author	humkyung
	@param	nNoteNum	a parameter of type const int &
	@param	str	a parameter of type const string &
	@return	-1 = 대응하는 코드 숫자가 없을때
*/
int CPDSOracle::FindCodeNumberUsingStr(const int &nNoteNum, const string &str)
{
	map<int , CNoteTableForOra*>::iterator where = m_noteTableForOra.find(nNoteNum);
	if(where != m_noteTableForOra.end())
	{
		CNoteTableForOra::NoteNode noteNode;
		
		if(true == where->second->Find(noteNode , str))
		{
			return noteNode.nNum;
		}
	}
	
	return -1;
}

/**
**/
string CPDSOracle::FindStrMatchCodeNumber(const int &nNoteNum, const int &nCodeNum)
{
	string res;
	map<int , CNoteTableForOra*>::iterator where = m_noteTableForOra.find(nNoteNum);
	if(where != m_noteTableForOra.end())
	{
		CNoteTableForOra::NoteNode noteNode;
		CNoteTableForOra* pNoteTableForOra = where->second;
		if(pNoteTableForOra)
		{
			if(pNoteTableForOra->Find(noteNode , nCodeNum)) res = noteNode.rNumString;
		}
	}
	
	return res;
}
