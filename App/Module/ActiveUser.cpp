// ActiveUser.cpp: implementation of the CActiveUser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include "Module.h"
#include <ADO\ADODB.h>
#include "Project.h"
#include "ActiveUser.h"
#include "ModuleData.h"
#include "ProjectData.h"

#include "RevDataRecordSet.h"

#include <string>
#include <algorithm>
#include <functional> // STL 의 함수객체
using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CActiveUser::CActiveUser(const char* pUserId , CProject* pProject , const char* comments) : m_comments(comments)
{
	m_pProject = pProject;
	(*m_id) = pUserId;

//	STRING_T rDbPath = m_pProject->path() + string("\\Database\\") + m_pProject->name() + string(".mdb");
//	
//	CADODB adoDB;
//	///if(pAdoDb)
//	{
//		string rConnectionString="Provider=Microsoft.Jet.OLEDB.4.0;Data Source = ";
//		rConnectionString += rDbPath.c_str();
//		if(TRUE == adoDB.DBConnect(rConnectionString.c_str()))
//		{
//			CString rQuery("INSERT INTO T_ACCESS(USERID,COMMENT) VALUES('" + CString(pUserId) + "','" + CString(comments) + "')");
//			
//			try
//			{
//				adoDB.ExecuteQuery(rQuery.operator LPCTSTR());
//			}
//			catch(const exception& ex)
//			{
//				AfxMessageBox(ex.what());
//			}
//			
//			rQuery = "INSERT INTO T_ACTIVE_USER(USERID) VALUES('" + CString(pUserId) + "')";
//			
//			//////////////////////////////////////////////////////////////////////////
//			/// insert user id to active user table
//			try
//			{
//				adoDB.ExecuteQuery(rQuery.operator LPCTSTR());
//			}
//			catch(const exception& ex)
//			{
//				AfxMessageBox(ex.what());
//			}
//
//			adoDB.DBDisConnect();
//		}
//		else
//		{
//			AfxMessageBox("Fail to connect project db!!!");
//		}
/////		CADODB::DestroyInstance();
//	}
}

CActiveUser::~CActiveUser()
{
	try
	{
		string rDbPath = m_pProject->path() + string("\\Database\\") + m_pProject->name() + string(".mdb");
		
		CADODB adoDB;
		///if(pAdoDb)
		{
			string rConnectionString="Provider=Microsoft.Jet.OLEDB.4.0;Data Source = ";
			rConnectionString += rDbPath.c_str();
			rConnectionString += DBPASSWORD;	/// 2012.01.27 added by humkyung
			if(TRUE == adoDB.DBConnect(rConnectionString.c_str()))
			{
				CString rQuery("INSERT INTO T_ACCESS(USERID,COMMENT) VALUES('" + CString(m_id->c_str()) + "','접속 종료')");
				
				try
				{
					adoDB.ExecuteQuery(rQuery.operator LPCTSTR());
				}
				catch(const exception& ex)
				{
					AfxMessageBox(ex.what());
				}
				
				rQuery = "DELETE FROM T_ACTIVE_USER WHERE USERID='" + CString(m_id->c_str()) + "'";
				//////////////////////////////////////////////////////////////////////////
				/// delete user id from active user table
				try
				{
					adoDB.ExecuteQuery(rQuery.operator LPCTSTR());
				}
				catch(const exception& ex)
				{
					AfxMessageBox(ex.what());
				}

				adoDB.DBDisConnect();
			}
			else
			{
				AfxMessageBox("Fail to connect project db!!!");
			}
///			CADODB::DestroyInstance();
		}
	}
	catch(...)
	{
		
	}
}

void CActiveUser::CheckOut()
{
}

void CActiveUser::DeleteAccessData(vector<long>* pAccessIndexEntry)
{
	assert(pAccessIndexEntry && "pAccessIndexEntry is NULL");

	if(pAccessIndexEntry)
	{
		string rDbPath = m_pProject->path() + string("\\Database\\") + m_pProject->name() + string(".mdb");
		
		CADODB adoDB;
		///if(pAdoDb)
		{
			string rConnectionString="Provider=Microsoft.Jet.OLEDB.4.0;Data Source = ";
			rConnectionString += rDbPath.c_str();
			rConnectionString += DBPASSWORD;	/// 2012.01.27 added by humkyung
			if(TRUE == adoDB.DBConnect(rConnectionString.c_str()))
			{
				for(vector<long>::iterator itr = pAccessIndexEntry->begin();itr != pAccessIndexEntry->end();itr++)
				{
					CString rQuery;
					rQuery.Format("DELETE FROM T_ACCESS WHERE c_idx=%d" , (*itr));
					
					try
					{
						adoDB.ExecuteQuery(rQuery.operator LPCTSTR());
					}
					catch(const exception& ex)
					{
						AfxMessageBox(ex.what());
					}
				}
					
				adoDB.DBDisConnect();
			}
			else
			{
				AfxMessageBox("Fail to connect project database!!!");
			}
			///CADODB::DestroyInstance();
		}
	}
}

void CActiveUser::Log(const char *msg)
{
	assert(msg && "msg is NULL");

	if(msg)
	{
		string rDbPath = m_pProject->path() + string("\\Database\\") + m_pProject->name() + string(".mdb");
		
		CADODB adoDB;
		///if(pAdoDb)
		{
			string rConnectionString="Provider=Microsoft.Jet.OLEDB.4.0;Data Source = ";
			rConnectionString += rDbPath.c_str();
			rConnectionString += DBPASSWORD;	/// 2012.01.27 added by humkyung
			if(TRUE == adoDB.DBConnect(rConnectionString.c_str()))
			{
				string rQuery("INSERT INTO T_ACCESS(USERID,COMMENT) VALUES('" + (*m_id) + "','" + string(msg) + "')");
				
				try
				{
					adoDB.ExecuteQuery(rQuery.c_str());
				}
				catch(const exception& ex)
				{
					AfxMessageBox(ex.what());
				}
				
				adoDB.DBDisConnect();
			}
			else
			{
				AfxMessageBox("Fail to connect project database!!!");
			}
			///CADODB::DestroyInstance();
		}
	}
}

size_t CActiveUser::GetAccessCount()
{
	if(NULL != m_pProject)
	{
		return m_pProject->GetAccessCount();
	}

	return 0;
}

CProjectAccess* CActiveUser::GetAccessAt(const size_t &at)
{
	if(NULL != m_pProject)
	{
		return m_pProject->GetAccessAt(at);
	}

	return NULL;
}

void CActiveUser::AddUsers(vector<string*>* pUserIdEntry)
{
}

/**	@brief	사용자의 이름과 암호를 변경한다.

	@author	백흠경

	@param	name	a parameter of type const char *
	@param	password	a parameter of type const char *

	@return	bool	
*/
bool CActiveUser::UpdateProperty(const char *name, const char *password)
{
	assert(name && "name is NULL");
	assert(password && "password is NULL");

	if(name && password)
	{
		CString rDbPath = m_pProject->m_user_db_path->c_str();
		
		CADODB adoDB;
		///if(pAdoDb)
		{
			CString rConnectionString="Provider=Microsoft.Jet.OLEDB.4.0;Data Source = ";
			rConnectionString += rDbPath;
			rConnectionString += DBPASSWORD;	/// 2012.01.27 added by humkyung
			if(TRUE == adoDB.DBConnect(rConnectionString.operator LPCTSTR()))
			{
				CString rQuery("UPDATE T_USER SET USERNAME='");
				rQuery += name + CString("',USERPASS='") + password + "' ";
				rQuery += CString("WHERE USERID='") + m_id->c_str() + "'";
				
				try
				{
					adoDB.ExecuteQuery(rQuery.operator LPCTSTR());
				}
				catch(const exception& ex)
				{
					AfxMessageBox(ex.what());
				}
				
				adoDB.DBDisConnect();

				return true;
			}
			else
			{
				AfxMessageBox("Fail to connect user database!!!");
			}
			///CADODB::DestroyInstance();
		}
	}

	return false;
}

/**	
	@brief	The CActiveUser::GetLineDataRecordSetFromDatabase function
	
	@author	BHK

	@date	?

	@param	pQueryCondition	a parameter of type const char *
	@param	pRevNo	a parameter of type const char*
	@param	pIssueDate	a parameter of type const char*

	@return	CLineDataRecordSet*	
*/
//CLineDataRecordSet* CActiveUser::GetLineDataRecordSetFromDatabase(const char *pQueryCondition , const char* pRevNo , const vector< STRING_T >& issueDates , const bool& reload)
//{
//	CLineDataRecordSet* pLineListRecordSet = NULL;//CLineDataRecordSet::CreateInstance();
//	
//	CString rDbPath   = m_pProject->path() + CString("\\Database\\") + m_pProject->name() + CString(_T(".mdb"));
//	
//	CADODB adoDB;
//	///if(pAdoDb)
//	{
//		const size_t nFieldCount = m_pProject->m_pLineDataFieldSet->GetFieldCount();
//		
//		string rConnectionString = _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source = ");
//		rConnectionString += rDbPath;
//		rConnectionString += DBPASSWORD;	/// 2012.01.27 added by humkyung
//		if(TRUE == adoDB.DBConnect(rConnectionString.c_str()))
//		{
//			pLineListRecordSet = m_pProject->LoadLineDataRecordSet(&adoDB , pQueryCondition , pRevNo , issueDates , reload );
//			size_t nRecords = 0;
//			if(pLineListRecordSet)
//			{
//				//////////////////////////////////////////////////////////////////////////
//				/// load reserved data
//				nRecords = pLineListRecordSet->GetLineDataRecordCount();
//				for(int i = 0;i < int(nRecords);++i)
//				{
//					CLineDataRecord* pLineDataRecord = pLineListRecordSet->GetLineDataRecordAt(i);
//					///m_pProject->LoadResDataFromDatabase(pLineDataRecord , &adoDB);
//					m_pProject->LoadRevDataFromDatabase(pLineDataRecord , &adoDB);
//				}
//			}
//			
//			///adoDB.DBDisConnect();
//		}
//		else
//		{
//			AfxMessageBox( _T("Fail to connect project database!!!"));
//		}
//	}
//	
//	return pLineListRecordSet;
//}

/**	\brief	database에서 issue date들을 구한다.


	\return	CIssueDateRecordSet*	
*/
CIssueDateRecordSet* CActiveUser::GetIssueDateRecordSetFromDatabase()
{
	CIssueDateRecordSet* pIssueDateRecordSet = NULL;
	if(m_pProject)
	{
		pIssueDateRecordSet = m_pProject->LoadIssueDateRecordSet();
	}

	return pIssueDateRecordSet;
}

CDataFieldSet* CActiveUser::GetLineDataFieldSet()
{
	return (m_pProject->m_pLineDataFieldSet);
}

CResDataRecord* CActiveUser::CreateResDataRecord()
{
	CResDataRecord* pResDataRecord = CResDataRecord::CreateInstance();
	if(pResDataRecord)
	{
		CDataFieldSet* pResDataFieldSet = CDataFieldSet::CreateInstance();
		pResDataRecord->Set(pResDataFieldSet);

		const size_t nFieldCount = m_pProject->m_pResDataFieldSet->GetFieldCount();
		for(size_t i = 0;i < nFieldCount;i++)
		{
			CDataField* p = m_pProject->m_pResDataFieldSet->GetFieldAt(i);
			
			CDataField* pReservedDataField = CDataField::CreateInstance();
			if(pReservedDataField)
			{
				pReservedDataField->Set(p->name() , p->desc() , "");
				
				pResDataFieldSet->Add(pReservedDataField);
			}
		}
	}

	return pResDataRecord;
}

/**	
	@brief	line data record를 SmartISO database에 저장한다.
	
	@author	humkyung

	@date	????.??.??

	@param	pLineDataRecordEntry	a parameter of type vector<CLineDataRecord*>*
	@param	saveResData	a parameter of type bool
	@param	saveRevData	a parameter of type bool

	@return	void	
*/
void CActiveUser::SaveToDatabase(vector<CLineDataRecord*>* pLineDataRecordEntry , bool saveResData , bool saveRevData)
{
	assert(pLineDataRecordEntry && "pLineDataRecordEntry is NULL");

	if(pLineDataRecordEntry)
	{
		CADODB adoDB;
		///if(pAdoDb)
		{
			CString rConnectionString = _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source = ");
			rConnectionString += m_pProject->GetDatabaseFilePath();
			rConnectionString += DBPASSWORD;	/// 2012.01.27 added by humkyung
			if(TRUE != adoDB.DBConnect(rConnectionString.operator LPCTSTR())) return;
			
			CString rTitle("Save to database");
			double dProcess = 0.;
			const size_t nLineDataRecordCount = pLineDataRecordEntry->size();

			int index = 0;
			for(vector<CLineDataRecord*>::iterator itr = pLineDataRecordEntry->begin();itr != pLineDataRecordEntry->end();itr++)
			{					
				CLineDataRecord* pLineDataRecord = (*itr);
				if(NULL == pLineDataRecord) continue;

				CString rLineDataQuery;
				if(-1 == pLineDataRecord->index())
				{
					/// 새로운 RECORD를 추가한다.
					rLineDataQuery = _T("INSERT INTO T_ISO_LINE_DATA(");
					
					CString rValues;
					const size_t nFieldCount = pLineDataRecord->GetFieldCount();
					for(size_t i = 0;i < nFieldCount;i++)
					{
						CDataField* pField = pLineDataRecord->GetFieldAt(i);
						if(pField)
						{
							CString rValue(pField->value());
							if( rValue.IsEmpty() ) continue;
							
							rLineDataQuery += CString(pField->name()) + CString(",");
							rValue.Replace(_T("'") , _T("''"));
							rValues += _T("'") + rValue + _T("'") + CString(_T(","));
						}
					}
					if("," == rLineDataQuery.Right(1)) rLineDataQuery = rLineDataQuery.Left(rLineDataQuery.GetLength() - 1);
					if("," == rValues.Right(1)) rValues = rValues.Left(rValues.GetLength() - 1);
					rLineDataQuery += _T(") VALUES(") + rValues + _T(")");
					///rLineDataQuery += _T(",DELETED) VALUES(") + rValues + _T(",'No')");
				}
				else
				{
					//////////////////////////////////////////////////////////////////////////
					rLineDataQuery = _T("UPDATE T_ISO_LINE_DATA SET ");
					const size_t nFieldCount = pLineDataRecord->GetFieldCount();
					for(size_t i = 0;i < nFieldCount;i++)
					{
						CDataField* pField = pLineDataRecord->GetFieldAt(i);
						if(pField)
						{
							CString rValue(pField->value());
							/// 값이 없을때는 해당 FIELD를 UPDATE하지 않는다.
							if( rValue.IsEmpty() ) continue;

							rLineDataQuery += pField->name() + CString("=");
							rValue.Replace("'" , "''");
							rLineDataQuery += CString("'") + rValue + CString("',");
						}
					}
					/*
					if(pLineDataRecord->IsDeleted())
					{
						rLineDataQuery += "DELETED='Yes'";
					}
					else
					{
						rLineDataQuery += "DELETED='No'";
					}
					*/

					if("," == rLineDataQuery.Right(1))
					{
						rLineDataQuery = rLineDataQuery.Left(rLineDataQuery.GetLength() - 1);
					}
					CString rId;
					rId.Format(_T("'%s'") , pLineDataRecord->index());
					rLineDataQuery += _T(" WHERE KEY=") + rId;
				}
				
				
				//////////////////////////////////////////////////////////////////////////
				CString rResDataQuery;
				if(true == saveResData)
				{
					CResDataRecord* pResDataRecord = pLineDataRecord->GetResDataRecord();
					if(pResDataRecord)
					{
						CString sIndex = pResDataRecord->index();
						if(sIndex.IsEmpty())
						{
							rResDataQuery = _T("INSERT INTO T_RESERVED(REF,");
							CString rValues;
							rValues.Format("'%s'," , pLineDataRecord->index());
							
							size_t nCount = pResDataRecord->GetFieldCount();
							for(size_t i = 0;i < nCount;i++)
							{
								CDataField* pResDataField = pResDataRecord->GetFieldAt(i);
								rResDataQuery += pResDataField->name();
								
								CString rValue(pResDataField->value());
								rValue.Replace("'" , "''");
								rValues+= "'" + rValue + "'";
								if(i < nCount - 1)
								{
									rResDataQuery += ",";
									rValues+= ",";
								}
							}
							rResDataQuery += ") VALUES(" + rValues + ")";
						}
						else
						{
							rResDataQuery = "UPDATE T_RESERVED SET ";
							const size_t nFieldCount = pResDataRecord->GetFieldCount();
							for(size_t i = 0;i < nFieldCount;i++)
							{
								CDataField* pField = pResDataRecord->GetFieldAt(i);
								rResDataQuery += pField->name() + CString("=");
								
								CString rValue(pField->value());
								rValue.Replace("'" , "''");
								rResDataQuery += CString("'") + rValue + CString("',");
							}
							if("," == rResDataQuery.Right(1))
							{
								rResDataQuery = rResDataQuery.Left(rResDataQuery.GetLength() - 1);
							}
							CString rId;
							rId.Format("'%s'" , pLineDataRecord->index());
							rResDataQuery += " WHERE REF=" + rId;
						}
					}
				}
				//////////////////////////////////////////////////////////////////////////
				
				//////////////////////////////////////////////////////////////////////////
				if(true == saveRevData)
				{
					CRevDataRecordSet* pRevDataRecordSet = pLineDataRecord->GetRevDataRecordSet();
					if(pRevDataRecordSet)
					{
						pRevDataRecordSet->SaveToDatabase(&adoDB , pLineDataRecord->index());
					}
				}
				//////////////////////////////////////////////////////////////////////////
			
				try
				{
					if(!rLineDataQuery.IsEmpty()) adoDB.ExecuteQuery(rLineDataQuery.operator LPCTSTR());
					if(!rResDataQuery.IsEmpty())
					{
						adoDB.ExecuteQuery(rResDataQuery.operator LPCTSTR());

						CResDataRecord* pResDataRecord = pLineDataRecord->GetResDataRecord();
						if(-1 == pResDataRecord->index())
						{
							STRING_T rQuery , rValue;
							rQuery = "SELECT IDX FROM T_RESERVED ORDER BY IDX DESC";
							try
							{
								adoDB.OpenQuery(rQuery);
								adoDB.GetFieldValue(0 , "REF" , &rValue);
								pResDataRecord->SetIndex(rValue.c_str());
							}
							catch(const exception& ex)
							{
								OutputDebugString(ex.what());
							}	
						}
					}
				}
				catch(const exception& ex)
				{
					OutputDebugString(ex.what());
				}

				if(-1 == pLineDataRecord->index())
				{
					STRING_T rQuery , rValue;
					rQuery = "SELECT DRAW_NO FROM T_ISO_LINE_DATA ORDER BY IDX DESC";
					try
					{
						adoDB.OpenQuery(rQuery);
						adoDB.GetFieldValue(0 , "DRAW_NO" , &rValue);
						pLineDataRecord->SetIndex(rValue.c_str());

						//////////////////////////////////////////////////////////////////////////
						/// add new line data record
						m_pProject->m_pLineDataRecordSet->Add(pLineDataRecord);
					}
					catch(const exception& ex)
					{
						OutputDebugString(ex.what());
					}	
				}

				//////////////////////////////////////////////////////////////////////////
				dProcess++;
				int process = (int)((dProcess / nLineDataRecordCount) * 100.);
				if(NULL != pUpdateWorkStatusFunc) pUpdateWorkStatusFunc(rTitle , process);
			}
			///9adoDB.DBDisConnect();
		}
	}
}

/**
	@breif	delete database records

	@author	HumKyung

	@date	????.??.??

	@param	pLineDataRecordEntry

	@return
*/
void CActiveUser::Delete(vector<CLineDataRecord*> *pLineDataRecordEntry)
{
	assert(pLineDataRecordEntry && "pLineDataRecordEntry is NULL");

	if(pLineDataRecordEntry)
	{
		CString rDbPath = m_pProject->GetDatabaseFilePath();
		
		CADODB adoDB;
		///if(pAdoDb)
		{
			CString rConnectionString = _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source = ");
			rConnectionString += rDbPath;
			rConnectionString += DBPASSWORD;	/// 2012.01.27 added by humkyung
			if(TRUE == adoDB.DBConnect(rConnectionString.operator LPCTSTR()))
			{
				for(vector<CLineDataRecord*>::iterator itr = pLineDataRecordEntry->begin();itr != pLineDataRecordEntry->end();itr++)
				{
					CString rQuery;

					CDataField* pDataField = (*itr)->FindWithFieldName(_T("KEY"));
					if(pDataField)
					{
						rQuery.Format(_T("DELETE FROM T_ISO_LINE_DATA WHERE KEY='%s'") , pDataField->value());
						try
						{
							adoDB.ExecuteQuery(rQuery.operator LPCTSTR());
							(*itr)->Delete(true);
						}
						catch(const exception& ex)
						{
							AfxMessageBox(ex.what());
						}
					}
				}
			}
			else
			{
				AfxMessageBox(_T("Fail to connect project database!!!"));
			}
		}
	}
}

void CActiveUser::Undelete(vector<CLineDataRecord*> *pLineDataRecordEntry)
{
	assert(pLineDataRecordEntry && "pLineDataRecordEntry is NULL");
	
	if(pLineDataRecordEntry)
	{
		CString rDbPath = m_pProject->GetDatabaseFilePath();
		
		CADODB adoDB;
		///if(pAdoDb)
		{
			CString rConnectionString="Provider=Microsoft.Jet.OLEDB.4.0;Data Source = ";
			rConnectionString += rDbPath;
			rConnectionString += DBPASSWORD;	/// 2012.01.27 added by humkyung
			if(TRUE == adoDB.DBConnect(rConnectionString.operator LPCTSTR()))
			{
				for(vector<CLineDataRecord*>::iterator itr = pLineDataRecordEntry->begin();itr != pLineDataRecordEntry->end();itr++)
				{
					CString rQuery;
					rQuery.Format("UPDATE T_ISO_LINE_DATA SET DELETED='No' WHERE C_IDX=%d" , (*itr)->index());
					
					try
					{
						adoDB.ExecuteQuery(rQuery.operator LPCTSTR());
						(*itr)->Delete(false);
					}
					catch(const exception& ex)
					{
						AfxMessageBox(ex.what());
					}
				}
				
				///adoDB.DBDisConnect();
			}
			else
			{
				AfxMessageBox("Fail to connect project database!!!");
			}
		}	
	}
}

CRevDataRecord* CActiveUser::CreateRevDataRecord()
{
	CRevDataRecord* pRevDataRecord = CRevDataRecord::CreateInstance();
	if(pRevDataRecord)
	{
		CDataFieldSet* pRevDataFieldSet = CDataFieldSet::CreateInstance();
		pRevDataRecord->Set(pRevDataFieldSet);
		
		const size_t nFieldCount = m_pProject->m_pRevDataFieldSet->GetFieldCount();
		for(size_t i = 0;i < nFieldCount;i++)
		{
			CDataField* p = m_pProject->m_pRevDataFieldSet->GetFieldAt(i);
			
			CDataField* pRevDataField = CDataField::CreateInstance();
			if(pRevDataField)
			{
				pRevDataField->Set(p->name() , p->desc() , "");
				
				pRevDataFieldSet->Add(pRevDataField);
			}
		}
	}
	
	return pRevDataRecord;
}

template<typename T> struct equal_ptr : public binary_function<T, T, bool>
{
//          typedef T first_argument_type;
//          typedef T second_argument_type;
//          typedef T result_type;

	bool operator ()( const T& a, const T& b ) const
	{
		return (a == b);
	}
};

/**
	@author	백흠경
**/
int CActiveUser::CreateLineDataTable(vector<CString>* pFieldNameEntry)
{
	static CString INVISIBLE_FIELD_NAME[] = { "LAST_UPDATE" , "LINE_STATUS" };
	assert(pFieldNameEntry && "pFieldNameEntry is NULL");
	int res = CREATE_ERROR;

	if(pFieldNameEntry)
	{
		CADODB adoDB;

		CString rDatabaseFilePath= m_pProject->GetDatabaseFilePath();
		CString rConnectionString="Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" + rDatabaseFilePath;
		rConnectionString += DBPASSWORD;	/// 2012.01.27 added by humkyung
		if(adoDB.DBConnect(rConnectionString.operator LPCTSTR()))
		{
			CString rQuery = "SELECT * FROM T_ISO_LINE_DATA";
			if(adoDB.OpenQuery(rQuery.operator LPCTSTR()))
			{				
				//////////////////////////////////////////////////////////////////////////
				/// Table exist.
				AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
				CString rMsg;
				rMsg.LoadString(IDS_ASK_CREATE_TABLE);
				int ask = AfxMessageBox(rMsg , MB_YESNO);
				if(IDYES == ask)
				{
					rQuery = "DROP TABLE T_ISO_LINE_DATA";
					try
					{
						adoDB.CloseRecordSet();
						adoDB.ExecuteQuery(rQuery.operator LPCTSTR());
					}
					catch(const exception& ex)
					{
						AfxMessageBox(ex.what());
					}
					
					CString rExecute;
					const size_t nFieldCount = pFieldNameEntry->size();
					for(int i = 0; i < int(nFieldCount);i++)
					{
						CString rFieldName = (*pFieldNameEntry)[i]; 
						CString rFieldType = "VARCHAR(128)";
						
						rExecute +=' '+ rFieldName +' '+ rFieldType +"," ;
					}
					if("," == rExecute.Right(1)) rExecute = rExecute.Left(rExecute.GetLength() - 1);
					
					rQuery = "CREATE TABLE T_ISO_LINE_DATA(IDX IDENTITY PRIMARY KEY," + rExecute + ",C_DELETED VARCHAR(50) default No,C_LAST_UPDATE VARCHAR(50),C_LINE_STATUS VARCHAR(50))";
					try
					{
						adoDB.ExecuteQuery(rQuery.operator LPCTSTR());
						res = CREATE_OK;
					}
					catch(const exception& ex)
					{
						AfxMessageBox(ex.what());
						res = CREATE_ERROR;
					}
				}
				else
				{
					res = USER_CANCEL;
				}
			}
			else//catch(const exception& )
			{
				//////////////////////////////////////////////////////////////////////////
				/// Table doesn't exist.
				CString rExecute;
				const size_t nFieldCount = pFieldNameEntry->size();
				for(int i = 0; i < int(nFieldCount);i++)
				{
					CString rFieldName = (*pFieldNameEntry)[i]; 
					CString rFieldType = "VARCHAR(128)";
					
					rExecute +=' '+ rFieldName +' '+ rFieldType +"," ;
				}
				if("," == rExecute.Right(1)) rExecute = rExecute.Left(rExecute.GetLength() - 1);
				
				rQuery = "CREATE TABLE T_ISO_LINE_DATA(IDX IDENTITY PRIMARY KEY," + rExecute + ",C_DELETED VARCHAR(50) default No,C_LAST_UPDATE VARCHAR(50),C_LINE_STATUS VARCHAR(50))";
				try
				{
					adoDB.ExecuteQuery(rQuery.operator LPCTSTR());
					res = CREATE_OK;
				}
				catch(const exception& ex)
				{
					AfxMessageBox(ex.what());
					res = CREATE_ERROR;
				}
			}

			adoDB.DBDisConnect();
		}
		else
		{
			res = FAIL_TO_CONNECT_DATABASE;
		}
	}

	return res;
}

//int CActiveUser::CollectAllPlant( )
//{
//	if( m_pProject )
//	{
//		CString rDbPath   = m_pProject->path() + CString("\\Database\\") + m_pProject->name() + CString(".mdb");
//
//		CADODB adoDB;
//		{
//			string rConnectionString = _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source = ");
//			rConnectionString += rDbPath.operator LPCTSTR();
//			rConnectionString += DBPASSWORD;	/// 2012.01.27 added by humkyung
//			if( TRUE == adoDB.DBConnect( rConnectionString.c_str() ))
//			{
//				m_pProject->CollectPlant( &adoDB );
//			}
//		}
//	}
//
//	return ERROR_SUCCESS;
//}

/**	
	@brief	The CActiveUser::CollectAllPlantNo function

	@param	records	a parameter of type set< string >&

	@return	int	
*/
//int CActiveUser::CollectAllPlantNo( list< STRING_T >& plantNos )
//{
//	static const CString rFieldName[] = { "PLANT_NO" , "AREA_NO" , "UNIT_NO" , "SECTION_NO" };
//
//	if( m_pProject )
//	{
//		CString rDbPath   = m_pProject->path() + CString("\\Database\\") + m_pProject->name() + CString(".mdb");
//
//		CADODB adoDB;
//		{
//			STRING_T rConnectionString = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source = ";
//			rConnectionString += rDbPath.operator LPCTSTR();
//			rConnectionString += DBPASSWORD;	/// 2012.01.27 added by humkyung
//			if( TRUE == adoDB.DBConnect( rConnectionString.c_str() ))
//			{
//				m_pProject->CollectPlantNos( plantNos , &adoDB );
//			}
//		}
//	}
//
//	return plantNos.size();
//}

/**
	@brief	

	@author	HumKyung.BAEK
**/
int CActiveUser::CollectAreaNoWith( vector< string >& areaNos , const vector< string >& plantNos )
{
	static const CString rFieldName[] = { "PLANT_NO" , "AREA_NO" , "UNIT_NO" , "SECTION_NO" };
	
	if( m_pProject )
	{
		CString rDbPath = m_pProject->path() + CString("\\Database\\") + m_pProject->name() + CString(".MDB");
		
		CADODB adoDB;
		{
			string rConnectionString = _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source = ");
			rConnectionString += rDbPath.operator LPCTSTR();
			rConnectionString += DBPASSWORD;	/// 2012.01.27 added by humkyung
			if( TRUE == adoDB.DBConnect( rConnectionString.c_str() ))
			{
				CString rCondition;
				if(!plantNos.empty())
				{
					rCondition = _T(" WHERE PLANT_NO IN(");
					for(vector<string>::const_iterator itr = plantNos.begin();itr != plantNos.end();++itr)
					{
						rCondition += _T("'") + CString(itr->c_str()) + _T("',");
					}
					if(_T(",") == rCondition.Right(1)) rCondition = rCondition.Left(rCondition.GetLength() - 1);
					rCondition += _T(")");
				}

				CString rSql = _T("SELECT DISTINCT AREA_NO FROM T_ISO_LINE_DATA") + rCondition;
				adoDB.OpenQuery(rSql.operator LPCTSTR());
				
				LONG lRecordCount = 0L;
				adoDB.GetRecordCount(&lRecordCount);
				for(int i = 0;i < lRecordCount;++i)
				{
					STRING_T rValue;
					adoDB.GetFieldValue(i , _T("AREA_NO") , &rValue);
					areaNos.push_back(rValue);
				}
			}
		}
	}
	
	return areaNos.size();
}

/**
	@brief	
	
	@author	humkyung
**/
int CActiveUser::CollectUnitNoWith( vector< string >& unitNos , const vector< string >& plantNos , const vector< string >& areaNos)
{
	static const CString rFieldName[] = { "PLANT_NO" , "AREA_NO" , "UNIT_NO" , "SECTION_NO" };
	
	if( m_pProject )
	{
		CString rDbPath   = m_pProject->path() + CString("\\Database\\") + m_pProject->name() + CString(".mdb");
		
		CADODB adoDB;
		{
			string rConnectionString = _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source = ");
			rConnectionString += rDbPath.operator LPCTSTR();
			rConnectionString += DBPASSWORD;	/// 2012.01.27 added by humkyung
			if( TRUE == adoDB.DBConnect( rConnectionString.c_str() ))
			{
				CString rCondition[2];
				if(!plantNos.empty())
				{
					for(vector<STRING_T>::const_iterator itr = plantNos.begin();itr != plantNos.end();++itr)
					{
						rCondition[0] += _T("'") + CString(itr->c_str()) + _T("',");
					}
					if(_T(",") == rCondition[0].Right(1)) rCondition[0] = rCondition[0].Left(rCondition[0].GetLength() - 1);
					if(!rCondition[0].IsEmpty())
					{
						rCondition[0] = _T("PLANT_NO IN(") + rCondition[0] + _T(")");
					}
				}
				if(!areaNos.empty())
				{
					for(vector<STRING_T>::const_iterator itr = areaNos.begin();itr != areaNos.end();++itr)
					{
						if(itr->empty()) continue;
						rCondition[1] += _T("'") + CString(itr->c_str()) + _T("',");
					}
					if(_T(",") == rCondition[1].Right(1)) rCondition[1] = rCondition[1].Left(rCondition[1].GetLength() - 1);
					if(!rCondition[1].IsEmpty())
					{
						rCondition[1] = _T("AREA_NO IN(") + rCondition[1] + _T(")");
					}
				}
				
				CString rSql = _T("SELECT DISTINCT UNIT_NO FROM T_ISO_LINE_DATA");
				if(!rCondition[0].IsEmpty() && !rCondition[1].IsEmpty())
				{
					rSql += _T(" WHERE ") + rCondition[0] + _T(" AND ") + rCondition[1];
				}
				else if(!rCondition[0].IsEmpty() || !rCondition[1].IsEmpty())
				{
					rSql += _T(" WHERE ") + rCondition[0] + rCondition[1];
				}
				
				adoDB.OpenQuery(rSql.operator LPCTSTR());
				
				LONG lRecordCount = 0L;
				adoDB.GetRecordCount(&lRecordCount);
				for(int i = 0;i < lRecordCount;++i)
				{
					STRING_T rValue;
					adoDB.GetFieldValue(i , _T("UNIT_NO") , &rValue);
					unitNos.push_back(rValue);
				}
			}
		}
	}
	
	return unitNos.size();
}

/**
	@brief	PLANT NO , AREA NO , UNIT NO에 해당하는 SECTION NO들을 구한다.
	
	@author	humkyung
**/
int CActiveUser::CollectSectionNoWith( vector< string >& sectionNos , const vector< string >& plantNos , const vector< string >& areaNos , const vector< string >& unitNos)
{
	static const CString rFieldName[] = { "PLANT_NO" , "AREA_NO" , "UNIT_NO" , "SECTION_NO" };
	
	if( m_pProject )
	{
		CString rDbPath   = m_pProject->path() + CString("\\Database\\") + m_pProject->name() + CString(".mdb");
		
		CADODB adoDB;
		///if(pAdoDb)
		{
			string rConnectionString =_T( "Provider=Microsoft.Jet.OLEDB.4.0;Data Source = ");
			rConnectionString += rDbPath.operator LPCTSTR();
			rConnectionString += DBPASSWORD;	/// 2012.01.27 added by humkyung
			if( TRUE == adoDB.DBConnect( rConnectionString.c_str() ))
			{
				CString rCondition[3];
				if(!plantNos.empty())
				{
					for(vector<STRING_T>::const_iterator itr = plantNos.begin();itr != plantNos.end();++itr)
					{
						rCondition[0] += _T("'") + CString(itr->c_str()) + _T("',");
					}
					if(_T(",") == rCondition[0].Right(1)) rCondition[0] = rCondition[0].Left(rCondition[0].GetLength() - 1);
					if(!rCondition[0].IsEmpty())
					{
						rCondition[0] = _T("PLANT_NO IN(") + rCondition[0] + _T(")");
					}
				}
				if(!areaNos.empty())
				{
					for(vector<STRING_T>::const_iterator itr = areaNos.begin();itr != areaNos.end();++itr)
					{
						if(itr->empty()) continue;
						rCondition[1] += _T("'") + CString(itr->c_str()) + _T("',");
					}
					if(_T(",") == rCondition[1].Right(1)) rCondition[1] = rCondition[1].Left(rCondition[1].GetLength() - 1);
					if(!rCondition[1].IsEmpty())
					{
						rCondition[1] = _T("AREA_NO IN(") + rCondition[1] + _T(")");
					}
				}
				if(!unitNos.empty())
				{
					for(vector<STRING_T>::const_iterator itr = unitNos.begin();itr != unitNos.end();++itr)
					{
						if(itr->empty()) continue;
						rCondition[2] += _T("'") + CString(itr->c_str()) + _T("',");
					}
					if(_T(",") == rCondition[2].Right(1)) rCondition[2] = rCondition[2].Left(rCondition[2].GetLength() - 1);
					if(!rCondition[2].IsEmpty())
					{
						rCondition[2] = _T("UNIT_NO IN(") + rCondition[2] + _T(")");
					}
				}
				
				CString rSql = _T("SELECT DISTINCT SECTION_NO FROM T_ISO_LINE_DATA");
				if(!rCondition[0].IsEmpty() || !rCondition[1].IsEmpty() || !rCondition[2].IsEmpty())
				{
					rSql += _T(" WHERE ");
					for(int i = 0;i < sizeof(rCondition) / sizeof(rCondition[0]);++i)
					{
						if(rCondition[i].IsEmpty()) continue;
						rSql += rCondition[i] + _T(" AND ");
					}
				}
				if(_T(" AND ") == rSql.Right(5)) rSql = rSql.Left(rSql.GetLength() - 5);
				
				adoDB.OpenQuery(rSql.operator LPCTSTR());
				
				LONG lRecordCount = 0L;
				adoDB.GetRecordCount(&lRecordCount);
				for(int i = 0;i < lRecordCount;++i)
				{
					STRING_T rValue;
					adoDB.GetFieldValue(i , _T("SECTION_NO") , &rValue);
					sectionNos.push_back(rValue);
				}
			}
		}
	}

	return sectionNos.size();
}

/**	\brief	The CActiveUser::CollectAllRevNo function

	\param	revNos	a parameter of type vector< string >&

	\return	int	
*/
//int CActiveUser::CollectAllRevNo( list< STRING_T >& revNos )
//{
//	static const CString rFieldName[] = { "PLANT_NO" , "AREA_NO" , "UNIT_NO" , "SECTION_NO" };
//	
//	if( m_pProject )
//	{
//		CString rDbPath   = m_pProject->path() + CString("\\Database\\") + m_pProject->name() + CString(".mdb");
//
//		CADODB adoDB;
//		///if(pAdoDb)
//		{
//			string rConnectionString = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source = ";
//			rConnectionString += rDbPath.operator LPCTSTR();
//			rConnectionString += DBPASSWORD;	/// 2012.01.27 added by humkyung
//			if( TRUE == adoDB.DBConnect( rConnectionString.c_str() ))
//			{
//				m_pProject->CollectRevNos( revNos , &adoDB );
//				///CADODB::DestroyInstance();
//			}
//		}
//
//		/*
//		if( m_pProject->IsEmptyLineDataRecordSet() )
//		{
//			CString rDbPath   = m_pProject->path() + CString("\\Database\\") + m_pProject->name() + CString(".mdb");
//			
//			CADODB* pAdoDb = CADODB::GetInstance();
//			if(pAdoDb)
//			{
//				string rConnectionString = "Provider=Microsoft.Jet.OLEDB.4.0;Data Source = ";
//				rConnectionString += rDbPath.operator LPCTSTR();
//				if( TRUE == adoDB.DBConnect( rConnectionString.c_str() ))
//				{
//					m_pProject->CollectAllLineDataRecord( pAdoDb );
//					CADODB::DestroyInstance();
//				}
//			}
//		}
//		
//		if( m_pProject->m_pLineDataRecordSet )
//		{
//			const size_t nRecordCount = m_pProject->m_pLineDataRecordSet->GetLineDataRecordCount();
//			for( int i = 0;i < nRecordCount;++i)
//			{
//				CLineDataRecord* pRecord = m_pProject->m_pLineDataRecordSet->GetLineDataRecordAt( i );
//				if( pRecord->IsDeleted() ) continue;
//				
//				CRevDataRecordSet* pRevDataRecordSet = pRecord->GetRevDataRecordSet();
//				if( pRevDataRecordSet )
//				{
//					const size_t nRecordCount = pRevDataRecordSet->GetRecordCount();
//					for( int i = 0;i < nRecordCount;++i)
//					{
//						CRevDataRecord* pRevRecord = pRevDataRecordSet->GetRecordAt( i );
//						string rString = pRevRecord->GetRevNo();
//
//						vector< string >::iterator where = find(revNos.begin() , revNos.end() , rString );
//						if( where == revNos.end() ) revNos.push_back( rString );
//					}
//				}
//			}
//		}
//		*/
//	}
//	
//	return revNos.size();
//}