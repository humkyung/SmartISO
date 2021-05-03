// ConvertLineListToOra.cpp: implementation of the CConvertLineListToOra class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include "UserDLL.h"
#include <Tokenizer.h>
#include "ConvertLineListToOra.h"
#include <Util/IsUtilInf.h>
#include <ado/ADODB.h>
#include <excel9\excel9.h>
#include <excel9\ExcelAutomation.h>

#include <vector>
using namespace std;
using namespace IsExcel;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

OFSTREAM_T __DEBUG_STREAM__;

//extern "C" __declspec(dllexport) bool Excel2Access(const CHAR_T *pExlpath,const CHAR_T *pMDBpath,const CHAR_T *pTempMDBpath, const CHAR_T *pInipath)
//{
//	assert(pExlpath && "pExlPath is NULL");
//	assert(pMDBpath && "pMDBpath is NULL");
//	assert(pInipath && "pInipath is NULL");
//	bool ret = false;
//
//	if(pExlpath && pMDBpath && pInipath)
//	{
//		CConvertLineListToOra converter;
//
//		ret = converter.LoadExcelData(pExlpath , pInipath);
//	}
//
//	return ret;
//}

/**
**/
CString CRowData::GetValue(const CString& rFieldName) const
{
	for(vector<MapValue>::const_iterator itr = m_MapValueEntry.begin();itr != m_MapValueEntry.end();++itr)
	{
		if(rFieldName == itr->rFieldName)
		{
			return (itr->rValue);
		}
	}

	return CString( _T("") );
}

/**
**/
CString CRowData::GetExecuteString(const vector<STRING_T>& KeySetting , COraDatabaseDefFile& oraDatabaseDefFile , CPDSOracle& pdsOracle)
{
	CString rString , rValue;
	for(vector<MapValue>::iterator itr = m_MapValueEntry.begin();itr != m_MapValueEntry.end();++itr)
	{
		/// 필드이름이 Key Setting에 사용되는 값이면 Execute String에서 뺀다.
		vector<STRING_T>::const_iterator where = find(KeySetting.begin() , KeySetting.end() , itr->rFieldName.operator LPCTSTR());
		if(where != KeySetting.end()) continue;
		
		rValue = itr->rValue;
		COraDatabaseDefFile::CFieldDef defField;
		if(true == oraDatabaseDefFile.GetOraDefField(defField , itr->rFieldName.operator LPCTSTR()))
		{
			int nNoteNum = -1;
			if(string::npos != defField.m_rNote.find( _T("standard note") )) ///< see standard note table
			{
				STRING_T rNoteNum = defField.m_rNote;
				int at = rNoteNum.find( _T("standard note") );
				int len = rNoteNum.length();
				int temp = len - ( at + STRLEN_T( _T("standard note") ));
				rNoteNum = rNoteNum.substr((at + strlen("standard note")) , temp);
				SSCANF_T(rNoteNum.c_str(), _T("%d") ,&nNoteNum);
				int nRetValue = pdsOracle.FindCodeNumberUsingStr(nNoteNum , rValue.operator LPCTSTR());
				if(-1 != nRetValue) rValue.Format(_T("%d") , nRetValue);
			}

			if(string::npos != defField.m_rFieldType.find( _T("character") )) ///< string type field
			{
				rValue = _T("'") + rValue + _T("'");
			}
			/*
			else if(HasNotDigitValues(rValue))
			{
				itr->nErr = 1;
			}
			*/
		}

		///if(0 == itr->nErr)
		{
			rString += itr->rFieldName + _T("=");
			rString += rValue + _T(",");
		}
	}
	if(_T(",") == rString.Right(1)) rString = rString.Left(rString.GetLength() - 1);
	
	return rString;
}

/**
**/
CString CRowData::GetConditionString(vector<STRING_T>& rhs , COraDatabaseDefFile& oraDatabaseDefFile , CPDSOracle& pdsOracle) const
{
	CString res;

	if(!rhs.empty())
	{
		for(vector<STRING_T>::iterator itr = rhs.begin();itr != rhs.end();++itr)
		{
			res += itr->c_str() + CString( _T(" = ") );

			CString rValue = GetValue(itr->c_str());
			rValue.TrimLeft(); rValue.TrimRight();
			COraDatabaseDefFile::CFieldDef defField;
			if(true == oraDatabaseDefFile.GetOraDefField(defField , itr->c_str()))
			{
				int nNoteNum = -1;
				if(string::npos != defField.m_rNote.find( _T("standard note") )) ///< see standard note table
				{
					STRING_T rNoteNum = defField.m_rNote;
					int at = rNoteNum.find( _T("standard note") );
					int len = rNoteNum.length();
					int temp = len - ( at + STRLEN_T( _T("standard note") ));
					rNoteNum = rNoteNum.substr((at + STRLEN_T( _T("standard note") )) , temp);
					SSCANF_T(rNoteNum.c_str(), _T("%d") ,&nNoteNum);
					int nRetValue = pdsOracle.FindCodeNumberUsingStr(nNoteNum , rValue.operator LPCTSTR());
					if(-1 != nRetValue) rValue.Format(_T("%d") , nRetValue);
				}

				if(string::npos != defField.m_rFieldType.find( _T("character") )) ///< string type field
				{
					rValue = _T("'") + rValue + _T("'");
				}
			}

			res += rValue + _T(" AND ");
		}
		if(_T(" AND ") == res.Right(5)) res = res.Left(res.GetLength() - 5);
	}

	return res;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConvertLineListToOra::CConvertLineListToOra()
{

}

CConvertLineListToOra::~CConvertLineListToOra()
{

}

/** \brief	엑셀의 내용을 오라클 데이터 베이스에 넣는다.
**/
int CConvertLineListToOra::Convert(const CHAR_T *pExlpath , const CHAR_T *pInipath)
{
	int res = PROGRAM_ERROR;

	__DEBUG_STREAM__.open( _T("c:\\LineListToOra.log") );

	LoadExcelData(pExlpath , pInipath);

	if(m_rProjectName.IsEmpty() || m_rUserID.IsEmpty() || m_rPW.IsEmpty() || m_rDataSource.IsEmpty()) return CONFIGURATION_ERROR;
	if(m_rStdNoteFilePath.IsEmpty())
	{
		AfxMessageBox( _T("Standard Note 파일 경로를 설정해 주세요.") );
		return res;
	}

	m_pdsOracle.Set( m_rDataSource , m_rUserID , m_rPW , m_rProjectName );
	m_pdsOracle.LoadNoteTableFromDataFile(m_rStdNoteFilePath.operator LPCTSTR());

	CAdoRemoteDB oraDb;
	CString	strDBPath = _T("Provider=MSDAORA;User ID=") + m_rUserID + _T(";Password=") + m_rPW + _T(";Data Source=") + m_rDataSource;
	if(TRUE == oraDb.DBConnect(strDBPath))
	{
		vector<CString> tableNames;
		CString rValue;
		/// 오라클 테이블 이름을 구한다.
		if(m_pdsOracle.Get_PDTABLE_12_TableNames(tableNames , oraDb))
		{
			UpdateOracleDatabase(&oraDb , tableNames);
			WriteConvResultToExcel(pExlpath , pInipath);
			
			res = CONVERT_OK;
		}
		else
		{
			///__DEBUG_STREAM__ << "12번 테이블을 구할 수 없습니다." << std::endl;
			res = NOT_FOUND_PDTABLE;
		}
	}
	else
	{
		///__DEBUG_STREAM__ << "Database에 접근할 수 없습니다." << std::endl;
		res = CONNECTION_FAILURE;
	}
	oraDb.DBDisConnect();

	///WriteConvResultToExcel(pExlpath , pInipath);
	__DEBUG_STREAM__.close();

	return res;
}

/**	\brief	테이블을 삭제한다.

	\param	pMDBpath	a parameter of type const char *

	\return	bool	
*/
bool CConvertLineListToOra::DropMdbTable(CString sMDBpath)
{
	STRING_T strDBPath;
	CString strFilePath(sMDBpath);
	strDBPath = STRING_T(_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=")) + strFilePath.operator LPCTSTR();
	
	CADODB adoDB;
	try
	{ 
		if(TRUE == adoDB.DBConnect(strDBPath))
		{
			STRING_T rQuery( _T("DROP TABLE T_LINE_DATA") );
			try
			{
				adoDB.ExecuteQuery(rQuery);
			}
			catch(...)
			{
				return FALSE;
			}
			adoDB.DBDisConnect();
		}
	}
	catch(_com_error &e)
	{
		CString rMsg;
		rMsg.Format( _T("*** DOG CHECK [ADO DB] => <ExecuteQuery 오류> 설명[%s] !! ***\n") , (LPCTSTR)e.Description());
		AfxMessageBox(rMsg);
		IsDBOpen = FALSE;
		return FALSE;
	}

	return TRUE;
}
bool CConvertLineListToOra::CreateMdbTable(const CHAR_T *pMDBpath, const CHAR_T *pInipath)
{

	int a=0,i=0,nCount = 0;
	CHAR_T szBuf[MAX_PATH]={0,};
	CString rFieldName,strExcerpt;

	CString rApp( _T("Line Data Field") );
	if(GetPrivateProfileString(rApp , _T("Count") , _T("") , szBuf , MAX_PATH , pInipath))
	{
		nCount = ATOI_T(szBuf);
	}

	vector<STRING_T> oResult;
	for(i = 0; i < nCount ;i++)
	{
		CString rKey;
		rKey.Format( _T("NAME%d") , i);
		if(GetPrivateProfileString(rApp , rKey , _T("") , szBuf , MAX_PATH , pInipath))
		{
			CTokenizer<CIsFromString>::Tokenize(oResult , szBuf , CIsFromString( _T(":") ));
			if(2 == oResult.size())
			{
				rFieldName = oResult[0].c_str();
				CString TT = _T("VARCHAR(128)");
				a++;
				if((nCount) == a)
				{
					strExcerpt +=' '+rFieldName+' '+ TT;
					a=0;
				}
				else
				{
					strExcerpt +=' '+rFieldName +' '+ TT+"," ;
				}
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	
	CADODB adoDB;
	STRING_T strDBPath;
	CString strFilePath;
	strFilePath = pMDBpath;
	strDBPath = STRING_T(_T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=")) + strFilePath.operator LPCTSTR();
	if(TRUE == adoDB.DBConnect(strDBPath))
	{
		STRING_T bstrQuery( _T("CREATE TABLE T_LINE_DATA(IDX IDENTITY PRIMARY KEY,") + STRING_T(strExcerpt.operator LPCTSTR()) + _T(",C_DELETED VARCHAR(50) default No)"));
		try
		{
			adoDB.ExecuteQuery(bstrQuery);
		}
		catch(const char* msg)
		{
			AfxMessageBox( CString(msg) );
		}

		adoDB.DBDisConnect();
	}

	return true;
}

struct HasSheetNo : binary_function< MapData , STRING_T , bool >
{
	bool operator()( const MapData& lhs , const STRING_T& rhs ) const
	{
		return ( (lhs.rFieldName == _T("SHEET_NO")) && !lhs.rColumn.IsEmpty() );
	}
};

/**	\brief	엑셀의 내용을 읽어서 MDB 파일로 저장한다.

	\param	pExlpath	a parameter of type const char *
	\param	pMDBpath	a parameter of type const char *
	\param	pTempMDBpath	a parameter of type const char *
	\param	pInipath	a parameter of type const char *

	\remakrs\n
	Jin => 코드를 정리할 것!

	\return	bool	
*/
bool CConvertLineListToOra::LoadExcelData(const CHAR_T *pExlpath , const CHAR_T *pInipath)
{
	static const STRING_T APP( _T("Excel") );
	CoInitialize(NULL);	/// COM관련 개체 초기화 
		
	_Application app;  
	_Workbook book;
	Workbooks books;
	Worksheets sheets;
	_Worksheet sheet;
	Range range; 
	
	COleVariant
		coVTrue((short)TRUE),
		coVFalse((short)FALSE),
		covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
		
	char szCount[3]={0,};
	CHAR_T szExcel[MAX_PATH]={0,},szBuf[MAX_PATH]={'\0' ,};
	CHAR_T szName[MAX_PATH]={0,};

	/// ini파일에서 정보를 받아온다.
	vector<MapData> MapDataEntry;
	ReadIniFile(MapDataEntry , pInipath);

	if(!app.CreateDispatch( _T("Excel.Application")) )
	{
		AfxMessageBox(_T("Excel Connection Failure"));
	}
	else
	{
		// Excel file 설정
		LPDISPATCH lpdisp;
		lpdisp=app.GetWorkbooks();
		ASSERT(lpdisp);
		books.AttachDispatch(lpdisp);
		CString rString;
		CString rExcelFilePath = pExlpath;
		book=books.Open(rExcelFilePath,covOptional, covOptional, covOptional,covOptional,covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional);
		sheets=book.GetWorksheets();
		long l = 0L;
		long lNumRows;
		long lNumCols;
		long lowr = 0L,lowc=1L;
		long pos[2];
		char rowNum[4]={0,};
		char ExCount[3]={0,};
		char ExName[MAX_PATH]={0,};
		int nWorkSheetCount = 0;
		vector<CString> SheetEntry;

		//***********************Excel Exception Sheet 정보 read & save****************************//
		if(GetPrivateProfileString(APP.c_str(), _T("Sheet Count") , _T("") , szBuf , MAX_PATH , pInipath))
		{
			nWorkSheetCount = ATOI_T(szBuf);
		}
		for(int i = 0;i < nWorkSheetCount; ++i)																   //
		{																						   //
			rString.Format( _T("Sheet %d") ,i);																   //
			if(GetPrivateProfileString(APP.c_str(), rString , _T("") , szBuf , MAX_PATH, pInipath))
			{
				SheetEntry.push_back(szBuf);
			}
		}				
		
		///********Data가 있는 행의 자료 값을 ini로 부터 read**************************//
		long nStartRow = 1L;
		if(GetPrivateProfileString(APP.c_str(), _T("Start Row") , _T("") , szBuf , MAX_PATH , pInipath))
		{
			nStartRow = ATOL_T(szBuf);
		}
		
		CString rTitle( _T("Load Excel Datas...") );
		double dProcess = 0.;

		bool bFoundError = false;
		//******************* 실제 로직 부분
		const long nSheetCount = sheets.GetCount();
		for(l = 0;l < nSheetCount;l++)
		{
			dProcess = l;
			if(NULL != pUpdateWorkStatusFunc)
			{
				int process = (int)((dProcess / nSheetCount) * 100.);
				pUpdateWorkStatusFunc(rTitle , process);
			}
			
			sheet=sheets.GetItem(COleVariant((short)(l + 1)));
			
			CString rSheetName = sheet.GetName();
			/// 사용자가 선택한 SHEET만 읽도록 한다.
			vector<CString>::iterator where = find(SheetEntry.begin() , SheetEntry.end() , rSheetName);
			if(SheetEntry.end() == where) continue;

			long lVisible = sheet.GetVisible();
			//long lType = sheet.GetType();
			if(-1 != lVisible) continue;
			
			range = sheet.GetUsedRange();
			
			Range rangeRows = range.GetRows();	///< row의 Range를 구한다.
			long lFirstRow = rangeRows.GetRow();
			long lRowCount = rangeRows.GetCount();
			long lDist = 1 - lFirstRow;

			VARIANT varValue = range.GetValue();
			COleSafeArray sa;
			sa.Attach(varValue);
			DWORD dwSize = sa.GetElemSize();
			if(0 == dwSize) continue;
			
			sa.GetUBound(1, &lNumRows);
			sa.GetUBound(2, &lNumCols);
			sa.GetLBound(1, &lowr);
			sa.GetLBound(2, &lowc);
			VARIANT valCell;
			CString strCell;
			CString rNames , rValues;
			
			///****************** [lowr,0]부터 [최종Row,size]까지 Cells Value를 DB에 Input
			for(long r = nStartRow + lDist; r <= lNumRows; ++r)
			{
				rNames.Empty();
				rValues.Empty();
				
				CRowData RowData(l + 1 , r - lDist);
				for(int i = 0;i < (int)MapDataEntry.size();++i)
				{
					MapValue mapValue;
					if( !MapDataEntry[i].rColumn.IsEmpty() )
					{
						mapValue.rFieldName = MapDataEntry[i].rFieldName;
						mapValue.rCellCol = MapDataEntry[i].rColumn;
						mapValue.nErr = 0;

						/// 필드에 대응하는 셀들을 값을 구한다.
						pos[0] = r;

						vector< STRING_T > oResult;
						CTokenizer<CIsFromString>::Tokenize( oResult , MapDataEntry[i].rColumn.operator LPCTSTR() , CIsFromString( _T("+") ));
						for( vector< STRING_T >::iterator itr = oResult.begin();itr != oResult.end();++itr)
						{
							pos[1] = FieldName2Long( CString(itr->c_str()) );

							try
							{
								sa.GetElement(pos, &valCell);
								mapValue.rValue += GetCellString( valCell );
							}
							catch(...)
							{
								mapValue.rValue += "-";
							}
						}

						if ( mapValue.rValue.IsEmpty() ) mapValue.rValue = "-";
						RowData.Add(mapValue);
					}
				}
				if(!RowData.IsAllEmptyData()) m_RowDataEntry.push_back(RowData);
			}
			if(true == bFoundError) break;
		}
				
		//*************** Excel Close 부분*************************//
		book.Close(COleVariant((short)FALSE), covOptional, covOptional);//
		app.Quit();							
		//**************************************************************//

		if(!m_RowDataEntry.empty())
		{
		}
		return TRUE;
	} 
	
	return FALSE;
}

//********************String인 filename을 long형태로 변환 A=1,B=2 etc***//
long CConvertLineListToOra::FieldName2Long(CString &rFieldName)			
{										
	long ret = 0L;								
	for(int i = rFieldName.GetLength() - 1;i >= 0 ;i--)			
	{									
		ret += (rFieldName[i] - 'A' + 1) + i * 26;			
	}									
	return ret;								
}										
//**********************************************************************//

CStringArray* CConvertLineListToOra::GetExcelName(const char *p2Exlpath)
{
	static CStringArray __theResult;
	__theResult.RemoveAll();

	_Application app;  
	_Workbook book;
	Workbooks books;
	Worksheets sheets;
	_Worksheet sheet;
	Range range; 
	
	COleVariant
		coVTrue((short)TRUE),
		coVFalse((short)FALSE),
		covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	
	if(!app.CreateDispatch( _T("Excel.Application") ))
	{
		AfxMessageBox(_T("Excel Connection Failure"));
		return false;
	}
	else
	{
		// Excel file 설정
		TRY
		{
			LPDISPATCH lpdisp;
			lpdisp=app.GetWorkbooks();
			ASSERT(lpdisp);
			books.AttachDispatch(lpdisp);
			CString path;
			path=p2Exlpath;
			book=books.Open(path , covOptional, covOptional, covOptional,covOptional,covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional);
			sheets=book.GetWorksheets();
			long l = 0L;

			CString rTitle("Load sheet names...");
			double dProgress = 0.;

			long nSheetCount = sheets.GetCount();
			for(l = 0;l < nSheetCount;l++)
			{
				sheet = sheets.GetItem(COleVariant((short)(l + 1)));
				CString rName = sheet.GetName();
				long lVisible = sheet.GetVisible();
				long lType = sheet.GetType();
				if(-1 == lVisible) __theResult.Add(rName);//pSheetNameEntry->push_back(rName.operator const char*());

				//////////////////////////////////////////////////////////////////////////
				dProgress++;
				int progress = (int)((dProgress / nSheetCount) * 100.);
				if(NULL != pUpdateWorkStatusFunc) pUpdateWorkStatusFunc(rTitle , progress);
			}
			book.Close(COleVariant((short)FALSE), covOptional, covOptional);//
			app.Quit();
			return (&__theResult);
		}
		CATCH(COleDispatchException, e) 
		{ 
			TRACE(e->m_strDescription); 
			::MessageBox(NULL , _T("Error creating Excel-file:\n") + e->m_strDescription, _T("My Demo Dialog") , MB_OK); 
		} 
		END_CATCH; 
	}

	return (&__theResult);
}

int CConvertLineListToOra::GetLineDataRecordCount(CADODB *pAdoDb)
{
	assert(pAdoDb && _T("pAdoDb is NULL"));

	if(pAdoDb)
	{
		STRING_T rQuery( _T("SELECT * FROM T_LINE_DATA") );
		
		try
		{
			pAdoDb->OpenQuery(rQuery);
			LONG lRecordCount;
			pAdoDb->GetRecordCount(&lRecordCount);
			return lRecordCount;
		}
		catch(const exception& ex)
		{
			AfxMessageBox( CString(ex.what()) );
			return -1;
		}
	}
	return -1;
}

/**
**/
CString CConvertLineListToOra::GetCellString(VARIANT &valCell)
{
	CString strCell;
	switch (V_VT(&valCell))
	{
		case VT_I2:
			strCell.Format( _T("%d") , (V_I2(&valCell)));
		break;
		case VT_I4:
			strCell.Format( _T("%d") , (V_I4(&valCell)));
		break;
		case VT_R4:
		{
			strCell.Format( _T("%f") , (V_R4(&valCell)));
			int at = strCell.Find( _T(".") );
			if(-1 != at)
			{
				CString rIntValue = strCell.Left(at);
				CString rDigiValue= strCell.Right(strCell.GetLength() - at - 1);
										
				STRING_T strDigiValue(rDigiValue.operator LPCTSTR());
				string::size_type it = strDigiValue.find_last_not_of( _T("0") );
				if((string::npos == it))
				{
					rDigiValue = _T("") ;
					strCell = rIntValue;
				}
				else
				{
					rDigiValue = strDigiValue.substr(0,it+1).c_str();
					strCell = rIntValue + _T(".") + rDigiValue;
				}
			}
		}
		break;
		case VT_R8:
		{
			strCell.Format(_T("%f") , (V_R8(&valCell)));
			int at = strCell.Find(_T("."));
			if(-1 != at)
			{
				CString rIntValue = strCell.Left(at);
				CString rDigiValue= strCell.Right(strCell.GetLength() - at - 1);
				
				STRING_T strDigiValue(rDigiValue.operator LPCTSTR());
				string::size_type it = strDigiValue.find_last_not_of( _T("0") );
				if((string::npos == it))
				{
					rDigiValue = _T("");
					strCell = rIntValue;
				}
				else
				{
					rDigiValue = strDigiValue.substr(0,it+1).c_str();
					strCell = rIntValue + _T(".") + rDigiValue;
				}
			}
		}
		break;
		case VT_BSTR:
			strCell = (CString)V_BSTR(&valCell);
		break;
		case VT_BOOL:
			if (V_BOOL(&valCell)) 
				strCell = _T("true");
			else 
				strCell = _T("false");
		break;
		case VT_EMPTY:
			strCell = _T("NULL");
		break;
	}

	return strCell;
}

/**	\brief	세팅 파일을 읽는다.
**/
void CConvertLineListToOra::ReadIniFile(vector<MapData>& MapDataEntry , const STRING_T& rIniFilePath)
{
	static const STRING_T APP( _T("MAPPING") );

	MapDataEntry.clear();

	CHAR_T szBuf[MAX_PATH + 1]={'\0' ,};
	
	CString rApp( _T("General") );
	m_rProjectName.Empty();
	if(GetPrivateProfileString(rApp , _T("Project") , _T("") , szBuf , MAX_PATH , rIniFilePath.c_str()))
	{
		m_rProjectName = szBuf;
	}

	m_rDataSource.Empty();
	if(GetPrivateProfileString(rApp , _T("Service Name") , NULL , szBuf , MAX_PATH , rIniFilePath.c_str()))
	{
		m_rDataSource = szBuf;
	}

	if(GetPrivateProfileString(rApp , _T("User Name") , NULL , szBuf , MAX_PATH , rIniFilePath.c_str()))
	{
		m_rUserID = szBuf;
	}else	m_rUserID.Empty();

	if(GetPrivateProfileString(rApp , _T("User Password") , NULL , szBuf , MAX_PATH , rIniFilePath.c_str()))
	{
		m_rPW = szBuf;
	}else	m_rPW.Empty();

	if(GetPrivateProfileString(APP.c_str() , _T("Col Count") , _T("") , szBuf , MAX_PATH , rIniFilePath.c_str()))
	{
		CString rKey;

		vector<STRING_T> oResult;
		const int nColCount = ATOI_T(szBuf);
		for(int i = 1;i <= nColCount;++i)
		{
			rKey.Format(_T("Col %d") , i);
			
			MapData mapData;
			if( GetPrivateProfileString(APP.c_str() , rKey , _T("") , szBuf , MAX_PATH, rIniFilePath.c_str()) )
			{
				CTokenizer<CIsFromString>::Tokenize(oResult , szBuf , CIsFromString( _T("=") ));
				if(2 == oResult.size())
				{
					mapData.rFieldName = oResult[0].c_str();
					mapData.rColumn = oResult[1].c_str();

					MapDataEntry.push_back(mapData);
				}
			}
		}
	}

	m_KeyFieldNameSetting.clear();
	if(GetPrivateProfileString(APP.c_str() , _T("Key") , _T("") , szBuf , MAX_PATH , rIniFilePath.c_str()))
	{
		CTokenizer<CIsFromString>::Tokenize(m_KeyFieldNameSetting , szBuf , CIsFromString( _T(":") ));
	}

	///
	if(GetPrivateProfileString(APP.c_str() , _T("ddl file path") , NULL , szBuf , MAX_PATH , rIniFilePath.c_str()))
	{
		m_oraDatabaseDefFile.Read(szBuf);
	}

	if(GetPrivateProfileString(APP.c_str() , _T("Std Note file path") , NULL , szBuf , MAX_PATH , rIniFilePath.c_str()))
	{
		m_rStdNoteFilePath = szBuf;
	}else	m_rStdNoteFilePath.Empty();
}

/**	ORACLE Database를 갱신한다.
**/
bool CConvertLineListToOra::UpdateOracleDatabase(CADODB* pAdoDB , vector<CString>& tableNames)
{
	assert(pAdoDB && _T("pAdoDB is NULL"));
	if(pAdoDB)
	{
		static const CString strQueryFormat( _T("SELECT * FROM %s WHERE ") );
		
		CString rTitle( _T("Update Oracle...") );
		double dProcess = 0. , index = 0;
		size_t nRowDataCount = m_RowDataEntry.size();
		for(vector<CRowData>::iterator jtr = m_RowDataEntry.begin();jtr != m_RowDataEntry.end();++jtr,++index)
		{
			CString rString;
			rString += jtr->GetExecuteString(m_KeyFieldNameSetting , m_oraDatabaseDefFile , m_pdsOracle);
			rString += _T(" WHERE ") + jtr->GetConditionString(m_KeyFieldNameSetting , m_oraDatabaseDefFile , m_pdsOracle);
			try
			{	
				for( vector< CString >::iterator itr = tableNames.begin();itr != tableNames.end();++itr)
				{
				
					STRING_T strQuery;
					strQuery = STRING_T( _T("UPDATE DD_") ) + m_rProjectName.operator LPCTSTR() + STRING_T(_T(".")) + (*itr).operator LPCTSTR() + STRING_T(_T(" SET "));
					strQuery+= rString;

					VARIANT RecordsAffected;
					if(TRUE == pAdoDB->ExecuteQuery(strQuery/* , &RecordsAffected*/))
					{
						CString rMsg;
						rMsg.Format( _T("%s") , strQuery);
						__DEBUG_STREAM__ << rMsg.operator LPCTSTR() << std::endl;

						jtr->m_nAffectedRecords += RecordsAffected.intVal;
					}
					else
					{
						CString rMsg;
						rMsg.Format( _T("%s") , strQuery);
						__DEBUG_STREAM__ << rMsg.operator LPCTSTR() << std::endl;

						rMsg.Format(_T("Error Condition : %s") , pAdoDB->GetErrString());
						__DEBUG_STREAM__ << rMsg.operator LPCTSTR() << std::endl;
					}
				}
			}
			catch( ... )
			{
			}

			dProcess = index;
			if(NULL != pUpdateWorkStatusFunc)
			{
				int process = (int)((dProcess / nRowDataCount) * 100.);
				pUpdateWorkStatusFunc(rTitle , process);
			}
		}
	}
	
	return false;
}

/**	
	@brief	문자열이 알파벳을 가지고 있다면 true - 그렇지 않다면 false
**/
bool CConvertLineListToOra::HasAlphaCharacter(const string &str) const
{
	const int count = str.length();
	for(int i = 0; i < count; ++i)
	{
		if(isalpha(str[i])) return true;
	}
	
	return false;
}

/**
**/
void CConvertLineListToOra::WriteConvResultToExcel(const STRING_T& rExcelFilePath , const STRING_T& rIniFilePath)
{
	CHAR_T szBuf[MAX_PATH + 1] = {'\0' ,};

	static const STRING_T APP( _T("Excel") );
	if(GetPrivateProfileString(APP.c_str() , _T("Conv Result Col") , NULL , szBuf , MAX_PATH , rIniFilePath.c_str()))
	{
		CString rCellNo;
		
		CExcelAutomation ExcelAutomation;
		if(ExcelAutomation.Open(rExcelFilePath.c_str()))
		{	
			for(vector<CRowData>::iterator itr = m_RowDataEntry.begin();itr != m_RowDataEntry.end();++itr)
			{
				ExcelAutomation.SetActiveSheet(itr->GetSheetNo());
				
				rCellNo.Format( _T("%s%d") , szBuf , itr->GetRowNo());
				if(itr->m_nAffectedRecords > 0)
				{
					ExcelAutomation.SetCellValue(COleVariant(rCellNo) , COleVariant( _T("SUCCESS") ));
					ExcelAutomation.SetCellFillColor(rCellNo , rCellNo , (short)8);			///< CYAN으로 표시할 것.
				}
				else
				{
					ExcelAutomation.SetCellValue(COleVariant(rCellNo) , COleVariant( _T("FAIL") ));
					ExcelAutomation.SetCellFillColor(rCellNo , rCellNo , (short)3);			///< RED으로 표시할 것.
				}

				/*
				const size_t nCount = itr->GetMapValueCount();
				for(int i = 0;i < nCount;++i)
				{
					MapValue* pValue = itr->GetMapValueAt(i);
					if(pValue && (0 != pValue->nErr))
					{
						rCellNo.Format("%s%d" , pValue->rCellCol , itr->GetRowNo());
						ExcelAutomation.SetCellFillColor(rCellNo , rCellNo , (short)3);	///< 붉은 색으로 표시할 것.
					}
				}
				*/
			}
		}
		ExcelAutomation.SaveAs(rExcelFilePath.c_str());
		ExcelAutomation.Close();
	}
}

/**
**/
int CRowData::GetSheetNo() const
{
	return m_nSheetNo;
}

/**
**/
int CRowData::GetRowNo() const
{
	return m_nRowNo;
}

/**	\brief	 숫자 형식이 아닌 문자를 가지고 있는지 검사한다.
**/
bool CRowData::HasNotDigitValues(const CString &rString)
{
	const int nLength = rString.GetLength();
	for(int i = 0;i < nLength;++i)
	{
		TCHAR ch = rString.GetAt(i);

		if(('.' != ch) && !isdigit(ch)) return false;
	}

	return true;
}

size_t CRowData::GetMapValueCount() const
{
	return m_MapValueEntry.size();
}

/**
**/
MapValue* CRowData::GetMapValueAt(const int &at)
{
	assert((at < int(m_MapValueEntry.size())) && "range error");
	if(at < int(m_MapValueEntry.size())) return &(m_MapValueEntry.at(at));

	return NULL;
}

/**	
	@brief	모든 데이터가 NULL인지 확인한다.
**/
bool CRowData::IsAllEmptyData() const
{
	for(vector<MapValue>::const_iterator itr = m_MapValueEntry.begin();itr != m_MapValueEntry.end();++itr)
	{
		if("NULL" != itr->rValue) return false;
	}

	return true;
}
