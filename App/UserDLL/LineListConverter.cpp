// LineListConverter.cpp: implementation of the CLineListConverter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include <math.h>
#include <IsTools.h>
#include <Tokenizer.h>
#include <util/excel9/excel9.h>
#include "UserDLL.h"
#include "LineListConverter.h"

#include <ado/ADODB.h>

#include <vector>
#include <string>
using namespace std;
using namespace IsExcel;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern "C" __declspec(dllexport) bool Excel2Access(const char *pExlpath,const char *pMDBpath,const char *pTempMDBpath, const char *pInipath)
{
	assert(pExlpath && "pExlPath is NULL");
	assert(pMDBpath && "pMDBpath is NULL");
	assert(pInipath && "pInipath is NULL");
	bool ret = false;

	if(pExlpath && pMDBpath && pInipath)
	{
		CLineListConverter converter;

		ret = converter.Run(pExlpath , pMDBpath ,pTempMDBpath, pInipath);
	}

	return ret;
}

const CString CLineListConverter::EMPTY_CELL("-");
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLineListConverter::CLineListConverter()
{

}

CLineListConverter::~CLineListConverter()
{

}

/** \brief	엑셀의 내용을 MDB 파일로 변환시킨다.
**/
int CLineListConverter::Convert(const char *pExlpath, const char *pMDBpath, const char *pTempMDBpath, const char *pInipath)
{
	int res = PROGRAM_ERROR;

	CADODB adoDB;
	CString	strDBPath, strDbname=pMDBpath;
	strDBPath = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + strDbname;
	strDBPath += DBPASSWORD;	/// 2012.01.27 added by humkyung
	///if(pAdoDb)
	{
		if(TRUE == adoDB.DBConnect(strDBPath.operator LPCTSTR()))
		{
			//////////////////////////////////////////////////////////////////////////
			int nRecordCount = GetLineDataRecordCount(&adoDB);
			adoDB.DBDisConnect();

			if(0 == nRecordCount)
			/// empty line data table
			{
				Run(pExlpath , pMDBpath , pTempMDBpath , pInipath);
				res = INSERT_OK;
			}
			else if(nRecordCount > 0)
			//////////////////////////////////////////////////////////////////////////
			/// line data table has one more records.
			{
				CopyFile(pMDBpath,pTempMDBpath,FALSE);
				DropMdbTable(pTempMDBpath);	///< 테이블을 삭제시킨다.
				if(true == CreateMdbTable(pTempMDBpath,pInipath))
				{
					Run(pExlpath , pTempMDBpath , pTempMDBpath , pInipath);
					res = COMPARE_NEEDED;
				}
				else
				{
					res = TABLE_ERROR;
				}
			}
			else
			//////////////////////////////////////////////////////////////////////////
			/// no line data table
			{
				AfxMessageBox("No Line List Table.");
				res = TABLE_ERROR;
			}
		}
	}

	return res;
}

/**	\brief	테이블을 삭제한다.

	\param	pMDBpath	a parameter of type const char *

	\return	bool	
*/
bool CLineListConverter::DropMdbTable(const char *pMDBpath)
{
	CString strDBPath;
	CString strFilePath;
	strFilePath = pMDBpath;
	strDBPath = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + strFilePath;
	strDBPath += DBPASSWORD;	/// 2012.01.27 added by humkyung
	
	CADODB adoDB;
	try
	{ 
		if(TRUE == adoDB.DBConnect(strDBPath.operator LPCTSTR()))
		{
			STRING_T rQuery("DROP TABLE T_LINE_DATA");
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
		rMsg.Format("*** DOG CHECK [ADO DB] => <ExecuteQuery 오류> 설명[%s] !! ***\n", (LPCSTR)e.Description());
		AfxMessageBox(rMsg);
		IsDBOpen = FALSE;
		return FALSE;
	}

	return TRUE;
}

/**
	\brief	T_LINE_DATA 페이블을 생성한다.
	\author	백흠경
	\remarks
	C_DELETED,C_LAST_UPDATE,C_LINE_STATUS는 항상은 항상 생성해야 한다.
**/
bool CLineListConverter::CreateMdbTable(const char *pMDBpath, const char *pInipath)
{

	int a=0,i=0,nCount = 0;
	char szBuf[MAX_PATH]={0,};
	CString rFieldName,strExcerpt;

	CString rApp("Line Data Field");
	if(GetPrivateProfileString(rApp , "Count" , "" , szBuf , MAX_PATH,pInipath))
	{
		nCount = atoi(szBuf);
	}

	vector<string> oResult;
	for(i = 0; i < nCount ;i++)
	{
		CString rKey;
		rKey.Format("NAME%d" , i);
		if(GetPrivateProfileString(rApp , rKey , "" , szBuf , MAX_PATH , pInipath))
		{
			CTokenizer<CIsFromString>::Tokenize(oResult , szBuf , CIsFromString(":"));
			if(2 == oResult.size())
			{
				rFieldName = oResult[0].c_str();
				CString TT = "VARCHAR(128)";
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
	CString strDBPath;
	CString strFilePath;
	strFilePath = pMDBpath;
	strDBPath = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + strFilePath;
	strDBPath += DBPASSWORD;	/// 2012.01.27 added by humkyung
	if(TRUE == adoDB.DBConnect(strDBPath.operator LPCTSTR()))
	{
		CString bstrQuery("CREATE TABLE T_LINE_DATA(IDX IDENTITY PRIMARY KEY,"+strExcerpt+",C_DELETED VARCHAR(50) default No,C_LAST_UPDATE VARCHAR(50),C_LINE_STATUS VARCHAR(50))");
		try
		{
			adoDB.ExecuteQuery(bstrQuery.operator LPCTSTR());
		}
		catch(const char* msg)
		{
			AfxMessageBox(msg);
		}

		adoDB.DBDisConnect();
	}

	return true;
}

struct HasSheetNo : binary_function< MapData , string , bool >
{
	bool operator()( const MapData& lhs , const string& rhs ) const
	{
		return ( (lhs.rFieldName == "SHEET_NO") && !lhs.rColumn.IsEmpty() );
	}
};

/**	
	@brief	엑셀의 내용을 읽어서 MDB 파일로 저장한다.
	
	@author	백흠경,남진우

	@param	pExlpath	a parameter of type const char *
	@param	pMDBpath	a parameter of type const char *
	@param	pTempMDBpath	a parameter of type const char *
	@param	pInipath	a parameter of type const char *

	@remakrs
	Jin => 코드를 정리할 것!

	@return	bool	
*/
bool CLineListConverter::Run(const char *pExlpath, const char *pMDBpath, const char *pTempMDBpath, const char *pInipath)
{
	CoInitialize(NULL);  // COM관련 개체 초기화 
	m_pCON = NULL; // 연결 포인터
	
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
	
	m_pCON.CreateInstance(__uuidof(Connection));
		
	IsDBOpen = FALSE;

	int Num;
	TCHAR szCount[3]={0,};
	TCHAR szExcel[MAX_PATH + 1]={0,};
	TCHAR szName[MAX_PATH + 1]={0,};
	CString AA,BB,CC;
	CString m_strName,m_strExcel;

	//ini파일에서 정보를 받아온다.
	vector<MapData> MapDataEntry;

	GetPrivateProfileString("Mapping", "Count","", szCount,sizeof(szCount), pInipath);
	Num = atoi(szCount);
	for(int k=0;k<Num;k++)
	{
		AA.Format("Name%d",k);
		BB.Format("Excel%d",k);
		
		MapData mapData;
		if( GetPrivateProfileString("Mapping", AA,"", szName,MAX_PATH, pInipath) )
		{
			mapData.rFieldName = szName;
		}
		if( GetPrivateProfileString("Mapping", BB,"", szExcel,MAX_PATH, pInipath) )
		{
			mapData.rColumn = szExcel;
		}
		MapDataEntry.push_back(mapData);
	}

	// 변수에 순서대로 저장
	const size_t size = MapDataEntry.size();
	int i=0;
	for(i=0;i < int(size);i++)
	{
		if(size - 1 == i)
		{
			m_strName += MapDataEntry[i].rFieldName;
			m_strExcel += MapDataEntry[i].rColumn;
		}
		else
		{
			m_strName += MapDataEntry[i].rFieldName+",";
			m_strExcel += MapDataEntry[i].rColumn+",";
		}
	}

	//DB Connection
	CString strDBPath;
	CString strFilePath;
	strFilePath = pMDBpath;
	strDBPath = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + strFilePath;
	strDBPath += DBPASSWORD;	/// 2012.01.27 added by humkyung
	_bstr_t strConnect(strDBPath);
	
	try
	{ 
		m_pCON->Open(strConnect,"","", adConnectUnspecified);
	}catch(...)
	{
		IsDBOpen = FALSE;
		return FALSE;
	}
	
	IsDBOpen = TRUE;
	
	if(!app.CreateDispatch("Excel.Application"))
	{
		AfxMessageBox("Excel Connection Failure");
	}
	else
	{
		// Excel file 설정
		LPDISPATCH lpdisp;
		lpdisp=app.GetWorkbooks();
		ASSERT(lpdisp);
		books.AttachDispatch(lpdisp);
		CString path;
		path=pExlpath;
		book=books.Open(path,covOptional, covOptional, covOptional,covOptional,covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional, covOptional);
		sheets=book.GetWorksheets();
		long l = 0L;
		long lNumRows;
		long lNumCols;
		long lowr,lowc,r;
		long pos[2];
		char rowNum[4]={0,};
		char ExCount[3]={0,};
		char ExName[MAX_PATH]={0,};
		int ExNum;
		CString ExN[MAX_PATH];
		
		//***********************Excel Exception Sheet 정보 read & save****************************//
		GetPrivateProfileString("Excel Exception", "Count","", ExCount,sizeof(ExCount), pInipath); //
		ExNum = atoi(ExCount);																	   //
		for(int a=0;a<ExNum;a++)																   //
		{																						   //
			CC.Format("Name%d",a);																   //
			GetPrivateProfileString("Excel Exception", CC,"", ExName,MAX_PATH, pInipath);          //
			ExN[a]=ExName;																		   //
		}																						   //
		//*****************************************************************************************//
		
		CString rTitle("Convert Line List...");
		double dProcess = 0.;
		
		//! 시작 날짜를 구한다.
		SYSTEMTIME sysTime;
		GetLocalTime(&sysTime);

		CString rCurDate;
		rCurDate.Format( "%d-%d-%d" , sysTime.wYear , sysTime.wMonth , sysTime.wDay );

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
			long lVisible = sheet.GetVisible();
			//long lType = sheet.GetType();
			if(-1 != lVisible) continue;
			
			// Excel Exception sheet 처리
			bool bProcessSheetName = false;
			for(i = 0;i < ExNum;i++)
			{
				if(ExN[i] == rSheetName)
				{
					bProcessSheetName = true;
					break;
				}
			}
			if(false == bProcessSheetName) continue;
			
			// Selected Sheet내의 Range 설정
			range = sheet.GetUsedRange();
			VARIANT varValue = range.GetValue();
			
			COleSafeArray sa;
			sa.Attach(varValue);
			sa.GetUBound(1, &lNumRows);
			sa.GetUBound(2, &lNumCols);
			sa.GetLBound(1, &lowr);
			sa.GetLBound(2, &lowc);
			VARIANT valCell;
			CString strCell;
			CString m_rValues;
			
			//********Data가 있는 행의 자료 값을 ini로 부터 read**************************//
			GetPrivateProfileString("RowNum", "Rnum","",rowNum ,sizeof(rowNum), pInipath);//
			lowr = atol(rowNum);														  //
			//****************************************************************************//
			
			//****************** [lowr,0]부터 [최종Row,size]까지 Cells Value를 DB에 Input
			for(r= lowr; r <= lNumRows; ++r)
			{
				bool bExistData = false;	//! 실제적으로 데이타가 있는지?
				for(i = 0;i < int(MapDataEntry.size());++i)
				{
					strCell.Empty();
					if( !MapDataEntry[i].rColumn.IsEmpty() )
					{
						pos[0] = r;
						
						vector< string > oResult;
						CTokenizer<CIsFromString>::Tokenize( oResult , MapDataEntry[i].rColumn.operator LPCSTR() , CIsFromString("+"));
						for( vector< string >::iterator itr = oResult.begin();itr != oResult.end();++itr)
						{
							pos[1] = FieldName2Long( CString(itr->c_str()) );
							
							try
							{
								sa.GetElement(pos, &valCell);
								CString rValue = GetCellString( valCell );
								if(strCell.IsEmpty())
									strCell += GetCellString( valCell );
								else if("-" != rValue)
									strCell += GetCellString( valCell );
							}
							catch(...)
							{
								if(strCell.IsEmpty()) strCell += CLineListConverter::EMPTY_CELL;
							}
						}

						if(!strCell.IsEmpty() && ("-" != strCell)) bExistData = true;
					}
					
					if( MapDataEntry[i].rFieldName == "SHEET_NO" )
					{
						vector<MapData>::iterator where = find_if( MapDataEntry.begin() , MapDataEntry.end() , bind2nd( HasSheetNo() , "SHEET_NO" ));
						if( MapDataEntry.end() == where )
						{
							strCell = "01";
						}
					}
					
					if ( strCell.IsEmpty() ) strCell = CLineListConverter::EMPTY_CELL;
					{
						strCell.Replace("'" , "''");	///< DB query Error 방지위해 Value중 ' -> ''로 바꾼다.
						if(MapDataEntry.size() - 1 == i)///< 제일 마지막일 때
						{
							m_rValues += "'"+strCell+"'";
						}
						else
						{
							m_rValues += "'"+strCell+"'" + ",";
						}
					}
				}
				
				//***************************** DB Query 부분 DB에 저장***********************************************
				if (IsDBOpen && bExistData)
				{
					//! LAST_UPDATE 항목에 현재 날짜 , LINE_STATUS 항목에 LIVE를 기본 값으로 설정한다.
					_bstr_t bstrQuery("INSERT INTO T_LINE_DATA ("+m_strName+", DELETED,LAST_UPDATE,LINE_STATUS) values (" + m_rValues + ", 'No','" + rCurDate + "','LIVE')");
					_variant_t vRecsAffected(0L);
					try
					{
						m_pCON->BeginTrans();
						m_pCON->Execute(bstrQuery,&vRecsAffected,adOptionUnspecified);
						m_pCON->CommitTrans();
					}
					catch(_com_error &e)
					{
						m_pCON->RollbackTrans(); 
						CString rMsg;
						rMsg.Format("*** DOG CHECK [ADO DB] => <ExecuteQuery 오류> 설명[%s] !! ***\n", (LPCSTR)e.Description());
						AfxMessageBox(rMsg);
						
						bFoundError = true;
						break;
					}
					m_rValues="";
				}
				//***************************************************************************************************
			}
			if(true == bFoundError) break;
		}
		//**************************************DB Query부분 Line_No가 없는 경우를 DB에서 삭제***********************
		if (IsDBOpen)
		{
			char szBuf[MAX_PATH] = {'\0' ,};	
			CString rApp("Line Data Field");
			if(GetPrivateProfileString(rApp , "Count" , NULL , szBuf , MAX_PATH , pInipath))
			{
				CString rCondition;
				
				vector<string> oResult;
				int nCount = atoi(szBuf);
				for(int i = 0;i < nCount;i++)
				{
					CString rKey;
					rKey.Format("NAME%d" , i);
					GetPrivateProfileString(rApp , rKey , NULL , szBuf , MAX_PATH , pInipath);
					CTokenizer<CIsFromString>::Tokenize(oResult , szBuf , CIsFromString(":"));
					oResult[0].c_str();
					rCondition += oResult[0].c_str() + CString("='NULL' AND ");
				}
				if(CString("AND ") == rCondition.Right(4))
				{
					rCondition = rCondition.Left(rCondition.GetLength() - 4);
				}
				
				_bstr_t bstrQuery("DELETE FROM T_LINE_DATA WHERE " + rCondition);
				_variant_t vRecsAffected(0L);
				try
				{
					m_pCON->BeginTrans();
					m_pCON->Execute(bstrQuery,&vRecsAffected,adOptionUnspecified);
					m_pCON->CommitTrans();
				}
				catch(_com_error &e)
				{
					m_pCON->RollbackTrans(); 
					CString rMsg;
					rMsg.Format("*** DOG CHECK [ADO DB] => <ExecuteQuery 오류> 설명[%s] !! ***\n", (LPCSTR)e.Description());
					AfxMessageBox(rMsg);
				}
			}
		}
		//***********************************************************************************************************
		
		//*************** DB & Excel Close 부분*************************//
		m_pCON = NULL;							
		book.Close(COleVariant((short)FALSE), covOptional, covOptional);//
		app.Quit();							
		//**************************************************************//
		
		//AfxMessageBox("Completion"); 
	} 
	return TRUE;
}

//********************String인 filename을 long형태로 변환 A=1,B=2 etc***//
long CLineListConverter::FieldName2Long(CString &rFieldName)			
{										
	/*
	long ret = 0L;								
	for(int i = rFieldName.GetLength() - 1;i >= 0 ;i--)			
	{									
		ret += (rFieldName[i] - 'A' + 1) + i * 26;			
	}									
	return ret;
	*/

	long ret = 0L;								
	for(int i = 0 ;i < rFieldName.GetLength() ;++i)
	{									
		ret += (rFieldName[i] - 'A' + 1) * long(pow(26.0 , rFieldName.GetLength() - i - 1));
	}

	return (ret);
}										
//**********************************************************************//

CStringArray* CLineListConverter::GetExcelName(const char *p2Exlpath)
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
	
	if(!app.CreateDispatch("Excel.Application"))
	{
		AfxMessageBox("Excel Connection Failure");
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
			::MessageBox(NULL , "Error creating Excel-file:\n" + e->m_strDescription, "My Demo Dialog", MB_OK); 
		} 
		END_CATCH; 
	}

	return (&__theResult);
}

int CLineListConverter::GetLineDataRecordCount(CADODB *pAdoDb)
{
	assert(pAdoDb && "pAdoDb is NULL");

	if(pAdoDb)
	{
		STRING_T rQuery("SELECT * FROM T_LINE_DATA");
		
		try
		{
			pAdoDb->OpenQuery(rQuery);
			LONG lRecordCount;
			pAdoDb->GetRecordCount(&lRecordCount);
			return lRecordCount;
		}
		catch(const exception& ex)
		{
			AfxMessageBox(ex.what());
			return -1;
		}
	}
	return -1;
}

CString CLineListConverter::GetCellString(VARIANT &valCell)
{
	CString strCell;
	switch (V_VT(&valCell))
	{
		case VT_I2:
			strCell.Format("%d", (V_I2(&valCell)));
		break;
		case VT_I4:
			strCell.Format("%d", (V_I4(&valCell)));
		break;
		case VT_R4:
		{
			strCell.Format("%f", (V_R4(&valCell)));
			int at = strCell.Find(".");
			if(-1 != at)
			{
				CString rIntValue = strCell.Left(at);
				CString rDigiValue= strCell.Right(strCell.GetLength() - at - 1);
										
				string strDigiValue(rDigiValue.operator const char*());
				string::size_type it = strDigiValue.find_last_not_of("0");
				if((string::npos == it))
				{
					rDigiValue = "";
					strCell = rIntValue;
				}
				else
				{
					rDigiValue = strDigiValue.substr(0,it+1).c_str();
					strCell = rIntValue + "." + rDigiValue;
				}
			}
		}
		break;
		case VT_R8:
		{
			strCell.Format("%f", (V_R8(&valCell)));
			int at = strCell.Find(".");
			if(-1 != at)
			{
				CString rIntValue = strCell.Left(at);
				CString rDigiValue= strCell.Right(strCell.GetLength() - at - 1);
				
				string strDigiValue(rDigiValue.operator const char*());
				string::size_type it = strDigiValue.find_last_not_of("0");
				if((string::npos == it))
				{
					rDigiValue = "";
					strCell = rIntValue;
				}
				else
				{
					rDigiValue = strDigiValue.substr(0,it+1).c_str();
					strCell = rIntValue + "." + rDigiValue;
				}
			}
		}
		break;
		case VT_BSTR:
			strCell = (CString)V_BSTR(&valCell);
		break;
		case VT_BOOL:
			if (V_BOOL(&valCell)) 
				strCell = "true";
			else 
				strCell = "false";
		break;
		case VT_EMPTY:
			strCell = CLineListConverter::EMPTY_CELL;
		break;
	}

	return strCell;
}
