// User.h: interface for the CUser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USER_H__16F811E8_1422_4691_A036_2E6035205DC8__INCLUDED_)
#define AFX_USER_H__16F811E8_1422_4691_A036_2E6035205DC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef MODULE_EXPORTS
	#define SMARTISO_MODULE_EXT_CLASS	__declspec(dllexport)
#else
	#define SMARTISO_MODULE_EXT_CLASS	__declspec(dllimport)
#endif

#include "ProjectAccess.h"
#include "DataFieldSet.h"
#include "LineDataRecordSet.h"
//#include "CompareResult.h"
#include "IssueDateRecordSet.h"
#include "MaterialCodeTable.h"
#include "WeldDiaTable.h"

#include <vector>
#include <list>
#include <set>
#include <string>
using namespace std;

class CProject;
class SMARTISO_MODULE_EXT_CLASS CUser  
{
public:
	bool DeleteUserWithIDX( const long& nIDX );
	virtual int CollectAllRevNo( list< STRING_T >& revNos );
	virtual int CollectSectionNoWith( vector< string >& sectionNos , const vector< string >& plantNos , const vector< string >& areaNos , const vector< string >& unitNos);
	virtual int CollectUnitNoWith( vector< string >& unitNos , const vector< string >& plantNos , const vector< string >& areaNos);
	virtual int CollectAreaNoWith( vector< string >& areaNos , const vector< string >& plantNos );
	virtual	int CollectAllPlant( );	/// 2011.10.30 added by humkyung
	virtual int CollectAllPlantNo( list< STRING_T >& plantNos );
	CWeldDiaTable* GetWeldDiaTable() const;
	CMaterialCodeTable* GetMaterialCodeTable() const;
	bool CheckDuplicatedPrimaryKey(const CString& index , const char* pPrimaryKey);
	bool CheckDuplicatedSmartISOKey(const CString& index , const TCHAR *pSmartISOKey);
	void DisplayErrorMessage(const int& error);
	enum
	{
		UNKNOWN_ERROR			= 0xff,

		COMPARE_ERROR			= 0x00,
		COMPARE_OK			= 0x01,
		
		CREATE_OK			= 0x02,
		CREATE_ERROR			= 0x03,
		USER_CANCEL			= 0x04,

		FAIL_TO_CONNECT_DATABASE	= 0x05
	};

	virtual int CreateLineDataTable(vector<CString>* ){return CREATE_OK;}
	const TCHAR* GetOutputIsoFileName(CLineDataRecord* pLineDataRecord);
	const TCHAR* GetOutputIsoFileExt(CLineDataRecord* pLineDataRecord , const CString& sRevNo = _T("LATEST"));
	const TCHAR* GetInputIsoFileName(CLineDataRecord* pLineDataRecord);
	

	//virtual int Compare(CCompareResult* , const char* , const char*){return COMPARE_ERROR;};
	virtual CRevDataRecord* CreateRevDataRecord();
	virtual void Undelete(vector<CLineDataRecord*> *pLineDataRecordEntry);
	virtual void Delete(vector<CLineDataRecord*> *pLineDataRecordEntry);
	virtual CProject* GetProject();
	virtual void SaveIssueDateRecordSet();
	virtual void SaveToDatabase(vector<CLineDataRecord*>* pLineDataRecordEntry , bool saveResData = true , bool saveRevData = true);
	virtual CResDataRecord* CreateResDataRecord();
	virtual CDataFieldSet* GetLineDataFieldSet();
	virtual CIssueDateRecordSet* GetIssueDateRecordSetFromDatabase(){return NULL;}
	virtual CLineDataRecordSet* GetLineDataRecordSetFromDatabase(const char *pQueryCondition , const char* pRevNo , const vector< STRING_T >& issueDates , const bool& reload=false);
	virtual bool UpdateProperty(const char* name , const char* password);
	const char* assigned_date() const;
	void SetAssignedDate(const char* date);
	const char* date() const;
	void SetDate(const char* date);
	virtual void AddUsers(vector<string*>* pUserIdEntry);
	virtual CProjectAccess* GetAccessAt(const size_t& at);
	virtual size_t GetAccessCount();
	virtual void LoadAccessData();
	virtual void Log(const char* msg);
	virtual void DeleteAccessData(vector<long>* pAccessIndexEntry);
	virtual void CheckOut();
	
	typedef	enum
	{
		ADMIN = 0x01,
		USER  = 0x02
	}USER_LEVEL;

	void SetLevel(const CUser::USER_LEVEL& level);
	CUser::USER_LEVEL level() const;
	void Set(const char* id , const char* name , const char* password , const char* area);
	void SetIndex(const long& index);
	long index() const;
	const char* password() const;
	const char* name() const;
	const char* id() const;
	const char* area() const;
	static CUser* CreateInstance();
	CUser(const CUser& user);
	CUser(const char* id , const char* name , const char* password , const char* area);
	CUser();
	virtual ~CUser();
protected:
	long   m_index;
	string* m_id;
	string* m_name;
	string* m_password;
	string* m_area;
	string* m_date;
	string* m_assigned_date;

	USER_LEVEL m_level;

	CProject* m_pProject;
private:
};

#endif // !defined(AFX_USER_H__16F811E8_1422_4691_A036_2E6035205DC8__INCLUDED_)
