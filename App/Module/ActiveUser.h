// ActiveUser.h: interface for the CActiveUser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACTIVEUSER_H__BEA796F0_74BB_4BB7_8E65_53EB6C1FBDC8__INCLUDED_)
#define AFX_ACTIVEUSER_H__BEA796F0_74BB_4BB7_8E65_53EB6C1FBDC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <ado/ADODB.h>
#include "User.h"
//#include "CompareResult.h"

#include <vector>
#include <list>
#include <set>
#include <string>
using namespace std;

class CActiveUser : public CUser  
{
public:
	//virtual int CollectAllRevNo( list< STRING_T >& revNos );
	virtual int CollectSectionNoWith( vector< string >& sectionNos , const vector< string >& plantNos , const vector< string >& areaNos , const vector< string >& unitNos);
	virtual int CollectUnitNoWith( vector< string >& unitNos , const vector< string >& plantNos , const vector< string >& areaNos);
	virtual int CollectAreaNoWith( vector< string >& areaNos , const vector< string >& plantNos );
	//int CollectAllPlant( );	/// 2011.10.30 added by humkyung
	//virtual int CollectAllPlantNo( list< STRING_T >& plantNos );
	virtual int CreateLineDataTable(vector<CString>* pFileNameEntry);
	virtual CRevDataRecord* CreateRevDataRecord();
	virtual void Undelete(vector<CLineDataRecord*> *pLineDataRecordEntry);
	virtual void Delete(vector<CLineDataRecord*> *pLineDataRecordEntry);
	virtual void SaveToDatabase(vector<CLineDataRecord*>* pLineDataRecord , bool saveResData = true , bool saveRevData = true);
	virtual CResDataRecord* CreateResDataRecord();
	virtual CDataFieldSet* GetLineDataFieldSet();
	virtual CIssueDateRecordSet* GetIssueDateRecordSetFromDatabase();
	//virtual CLineDataRecordSet* GetLineDataRecordSetFromDatabase(const char *pQueryCondition , const char* pRevNo , const vector< STRING_T >& issueDates , const bool& reload=false);
	virtual bool UpdateProperty(const char* name , const char* password);
	virtual void AddUsers(vector<string*>* pUserIdEntry);
	virtual CProjectAccess* GetAccessAt(const size_t& at);
	virtual size_t GetAccessCount();
	virtual void Log(const char* msg);
	virtual void DeleteAccessData(vector<long>* pAccessIndexEntry);
	void CheckOut();
	CActiveUser(const char* pUserId , CProject* pProject , const char* comments);
	virtual ~CActiveUser();
private:
	string m_comments;
};

#endif // !defined(AFX_ACTIVEUSER_H__BEA796F0_74BB_4BB7_8E65_53EB6C1FBDC8__INCLUDED_)
