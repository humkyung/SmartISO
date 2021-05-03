// PDSOracle.cpp: implementation of the CPDSOracle class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UserApp.h"
#include "PDSOracle.h"

#include "UserAppDocData.h"

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

/**	\brief	IDMS_PDTABLE_VIEW를 생성하고 PDTABLE_12_%에서 oracle map filed 해당하는 내용을 IDMS_PDTABLE_VIEW에 합친다.


	\return	void	
*/
void CPDSOracle::CreateOracleView() ///< 현재 사용 안함.
{
	CUserAppDocData* pDocData = CUserAppDocData::GetInstance();
	CString rAllFieldString( pDocData->m_OracleKey );
	rAllFieldString += ",NOR_OPER_PRES,NOR_OPER_TEMP,NOR_DGN_PRES,NOR_DGN_TEMP";
	
	static const CString rCreateStringFormat("CREATE OR REPLACE VIEW DD_%s.IDMS_PDTABLE_VIEW AS ");
	
	CString rCreateString;
	rCreateString.Format( rCreateStringFormat , m_rPDSProjectName );
	
	CAdoRemoteDB oraDB;
	CString	strDBPath = "provider=MSDAORA;User ID="+ m_rUserId +";Password="+ m_rPassword + ";Data Source="+m_rServiceName;
	if(TRUE == oraDB.DBConnect(strDBPath))
	{
		vector<CString> tableNames;
		Get_PDTABLE_12_TableNames(tableNames , oraDB);
		
		CString rPartNum;
		for(vector<CString>::iterator itr = tableNames.begin();itr != tableNames.end();++itr)
		{
			rPartNum = (*itr);
			const int nLength = CString("PDTABLE_12_").GetLength();
			rPartNum = rPartNum.Right(rPartNum.GetLength() - nLength);
			
			rCreateString += "SELECT " + rAllFieldString + "," + rPartNum + " AS \"PART\" FROM " + (*itr);
			if(tableNames.end() != (itr + 1)) rCreateString += " UNION ALL ";
		}

		ofstream ofile( "C:\\db.txt" );
		ofile << rCreateString.operator LPCSTR() << std::endl;
		ofile.close();
	
		oraDB.ExecuteQuery(rCreateString.operator LPCTSTR());
		
		
		oraDB.DBDisConnect();
	}
}

/**	\brief	PDTABLE_12로 시작하는 테이블들의 이름을 구한다.

	\param	tableNames	a parameter of type vector<CString>&
	\param	oraDB	a parameter of type CAdoRemoteDB&

	\return	bool	
*/
bool CPDSOracle::Get_PDTABLE_12_TableNames(vector<CString>& tableNames , CAdoRemoteDB& oraDB)
{
	tableNames.clear();

	///CString rOracleTableQury = "SELECT OBJECT_NAME FROM USER_OBJECTS WHERE OBJECT_TYPE='TABLE' AND OBJECT_NAME LIKE 'PDTABLE_12_%'";
	static const CString rOracleTableQueryFormat("SELECT TABLE_NAME FROM DBA_TABLES WHERE OWNER='DD_%s'");
	
	CString rOracleTableQuery;
	rOracleTableQuery.Format( rOracleTableQueryFormat , m_rPDSProjectName );
	rOracleTableQuery += " AND TABLE_NAME LIKE 'PDTABLE_12_%'";

	CUserAppDocData* pDocData = CUserAppDocData::GetInstance();
	CString rTempFolder = pDocData->GetTempFolder();
	if("\\" != rTempFolder) rTempFolder += "\\";
	ofstream ofile(rTempFolder + "TableNames.txt");
	
	ofile << rOracleTableQuery.operator LPCTSTR() << std::endl;
	if(TRUE == oraDB.OpenQuery(rOracleTableQuery.operator LPCTSTR()))
	{
		LONG lRecordCount = 0L;
		oraDB.GetRecordCount(&lRecordCount);
		
		STRING_T rValue;
		for(int i = 0;i < lRecordCount;++i)
		{
			oraDB.GetFieldValue(i,"TABLE_NAME",&rValue);
			ofile << rValue << std::endl;
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
	ofile.close();

	return false;
}

ofstream __STNOTE__;

/**	\brief	NoteNum에 해당하는 NOTE TABLE을 로드한다.

	\param	nNoteNum	a parameter of type const int &

	\remarks\n
	PDTABLE_NOTE_$nNoteNum

	\return	void	
*/
bool CPDSOracle::LoadNoteTableFromDataFile(const string &rFilePath)
{
	string rStdFilePath = rFilePath;
	ifstream ifile;
	string line;
	ifile.open(rStdFilePath.c_str());
	if(!ifile.is_open())
	{
		AfxMessageBox("StdNote파일이 없습니다.");
		return false;
	}
	
	__STNOTE__.open("C:\\TEMP\\STNOTE.TXT");
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

	__STNOTE__.close();
	/*
	CAdoRemoteDB oraDB;
	/// MDB Test 용
	/// CADODB* oraDB = CADODB::GetInstance();
	/// CString	rConnectString = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=D:\\프로젝트\\한화건설\\ORAC.HWENC3D.COM.MDB";//MDB Test 용
	CString	rConnectString = "provider=MSDAORA;User ID="+ m_rUserId +";Password="+ m_rPassword + ";Data Source="+m_rServiceName;
	if(TRUE == oraDB.DBConnect(rConnectString))
	{
		vector<CString> tableNames;
		CString rValue;
		
		CString rNoteTable;
		rNoteTable.Format("DD_%s.PDTABLE_NOTE_%d" ,m_rPDSProjectName, nNoteNum);
		CString rOracleTableQury = "SELECT * FROM " + rNoteTable;

		if(oraDB.OpenQuery(rOracleTableQury))
		{	LONG lRecordCount = 0L;
			oraDB.GetRecordCount(&lRecordCount);
		
			CNoteTableForOra* pNoteTable = new CNoteTableForOra;
			m_noteTableForOra[nNoteNum] = pNoteTable;
		
			for(int i = 0;i < lRecordCount;++i)
			{
				CNoteTableForOra::NoteNode noteNode;
				oraDB.GetFieldValue(i , "STD_NOTE_CODE_LIST" , &rValue);
				noteNode.nNum = atoi(rValue);
				oraDB.GetFieldValue(i , "STD_NOTE_CODE_TEXT" , &rValue);
				noteNode.rNumString = rValue;
				pNoteTable->Add(noteNode);
			}
			
			oraDB.DBDisConnect();
		}
	}
	else
	{
		CString rMsg;
		rMsg.LoadString(IDS_CONNECT_ORACLE_ERROR);
		AfxMessageBox(rMsg);
	}
	*/
}

/**	\brief	할당한 메모리들을 해제시킨다.


	\return	void	
*/
void CPDSOracle::Clear()
{
	for( map<int , CNoteTableForOra*>::iterator itr = m_noteTableForOra.begin();itr != m_noteTableForOra.end();++itr)
	{
		delete (itr->second);
	}
	m_noteTableForOra.clear();
}

/**	\brief	문자열에 해당하는 코드 숫자를 구한다.

	\param	nNoteNum	a parameter of type const int &
	\param	str	a parameter of type const string &

	\return	-1 = 대응하는 코드 숫자가 없을때
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
