// Project.h: interface for the CProject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROJECT_H__CF07E195_E016_4EF1_9FED_D605A02A423D__INCLUDED_)
#define AFX_PROJECT_H__CF07E195_E016_4EF1_9FED_D605A02A423D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef MODULE_EXPORTS
	#ifdef _UNICODE
		#ifdef _DEBUG
			#if _MSC_VER == 1600
				#pragma comment(lib, "Module_vc11_unicode_debug")
				#pragma message("Automatically linking with Module_vc11_unicode_debug.dll (Debug Unicode)")
			#else
				#pragma comment(lib, "Module_vc90_unicode_debug")
				#pragma message("Automatically linking with Module_vc90_unicode_debug.dll (Debug Unicode)")
			#endif
		#else
			#if _MSC_VER == 1600
				#pragma comment(lib, "Module_vc11_unicode")
				#pragma message("Automatically linking with Module_vc11_unicode.dll (Release Unicode)")
			#else
				#pragma comment(lib, "Module_vc90_unicode")
				#pragma message("Automatically linking with Module_vc90_unicode.dll (Release Unicode)")
			#endif
		#endif
	#else
		#ifdef _DEBUG
			#if _MSC_VER == 1600
				#pragma comment(lib, "Module_vc100_debug")
				#pragma message("Automatically linking with Module_vc100_debug.dll (Debug)")
			#else
				#pragma comment(lib, "Module_vc90_debug")
				#pragma message("Automatically linking with Module_vc90_debug.dll (Debug)")
			#endif
		#else
			#if _MSC_VER == 1600
				#pragma comment(lib, "Module_vc100")
				#pragma message("Automatically linking with Module_vc100.dll (Release)")
			#else
				#pragma comment(lib, "Module_vc90")
				#pragma message("Automatically linking with Module_vc90.dll (Release)")
			#endif
		#endif
	#endif // _UNICODE
#endif // MODULE_EXPORTS

#define SMARTISO_DECLARE_MODULE_FUNC()\
public:\
	static CProject* CreateInstance();\

#define	SMARTISO_IMPLEMENT_MODULE_FUNC(classname) \
CProject* classname::CreateInstance()\
{\
	return new classname;\
}\

#define CHECK_ERROR	0x00
#define PROJECT_USER	0x01
#define PROJECT_ADMIN	0x02
#define SYSTEM_ADMIN	0x04

//////////////////////////////////////////////////////////////////////////
/// error codes
#define SYSTEM_ADMIN_PASSWORD_ERROR	0x05
#define PROJECT_ADMIN_PASSWORD_ERROR	0x06
#define PROJECT_USER_PASSWORD_ERROR	0x07
#define PROJECT_USER_ERROR			0x08
#define NOT_ALLOW_AREA				0x09
#define NOT_PROJECT_ADMIN			0x10

#include "User.h"
#include "ActiveUser.h"
#include "ProjectAccess.h"
#include "BackupSystem.h"
#include "MaterialCodeTable.h"
#include "WeldDiaTable.h"
#include "AppPostgreSQL.h"

#include <vector>
#include <fstream>
using namespace std;

typedef struct  
{
	long    idx;
	string* user_id;
	string* area;
	string* assigned_date;
}PROJECT_ASSIGNED_USER;

class SMARTISO_MODULE_EXT_CLASS CSection
{
public:
	STRING_T m_sName;
};

class SMARTISO_MODULE_EXT_CLASS CUnit
{
public:
	CUnit(){}
	CUnit(const CUnit& rhs);
	int Add( STRING_T sSection );

	vector<CSection> m_oSectionList;
	STRING_T m_sName;
};

class SMARTISO_MODULE_EXT_CLASS CArea
{
public:
	CArea(){}
	CArea(const CArea& rhs);
	int Add( STRING_T sUnit , STRING_T sSection );

	vector<CUnit> m_oUnitList;
	STRING_T m_sName;
};

class SMARTISO_MODULE_EXT_CLASS CPlant
{
public:
	CPlant(){}
	CPlant(const CPlant& rhs);

	int Add( STRING_T sArea , STRING_T sUnit , STRING_T sSection );

	vector<CArea> m_oAreaList;
	STRING_T m_sName;
};

class SMARTISO_MODULE_EXT_CLASS CProject  
{
	CProject();
	CProject( const CProject& ) {}
	CProject& operator=(const CProject& ) { return *this; }
public:
	enum
	{
		UNKNOWN_ERROR		= 0xff,
		COMPARE_ERROR		= 0x00,
		COMPARE_OK			= 0x01,
		CREATE_OK			= 0x02,
		CREATE_ERROR		= 0x03,
		USER_CANCEL			= 0x04,
		FAIL_TO_CONNECT_DATABASE	= 0x05
	};

	typedef struct tagDatabaseProp
	{
		STRING_T Host , User , Password , Port;
	}DatabaseProp;

	vector<CPlant>* GetPlantList();	/// 2011.10.30 added by humkyung

	bool UpdateUserWithIDX(const long& nId, const string& rArea);
	bool DeleteUserWithIDX( const long& nId );
	CDataFieldSet* GetRevDataFieldSet() const;
	CDataFieldSet* GetResDataFieldSet() const;
	CDataFieldSet* GetLineDataFieldSet() const;
	bool IsEmptyLineDataRecordSet();
	const char* GetProjectNo() const;
	const TCHAR* GetDesc() const;	/// 2013.01.05 added by humkyung
	bool CheckDuplicatedPrimaryKey(const CString& index , const char* pPrimaryKey);
	bool CheckDuplicatedSmartISOKey(const CString& index , const TCHAR *pSmartISOKey);
	void SaveIssueDateRecordSet();
	CBackupSystem* GetBackupSystem() const;
	const char* GetIniFilePath();
	const char* GetDatabaseFilePath();
	static CProject* LoadFromIniFile(LPCTSTR pIniFilePath);
	
	CUser* CheckIn(const char* pUserId , const char* password , const char* module , const char* area = _T(""));
	void SetDate(const char* date);
	const char* date() const;
	
	void SetClientName(const char* pClientName);
	const char* GetClientName() const;

	const char* comment() const;
	void SetComment(const char* pComment);
	bool CreateFolder(LPCTSTR pszFolder);
	int CheckUser(const char* user , const char* password ,  const char* area , const char* moudle);
	void SetDatabaseProp(const DatabaseProp& prop);
	void SetPath(LPCTSTR pPath);
	void SetName(const char* pName);
	int SetDesc(const TCHAR* pDesc);	/// 2013.01.05 added by humkyung
	void SetProjectNo(const char* pProjctNo);
	const char* path() const;
	const char* name() const;
	DatabaseProp GetDatabaseProp() const;
	int SetPDSName(LPCTSTR);
	LPCTSTR GetPDSName() const;
	void Load(ifstream& file);
	void Save(ofstream& file);
	void Save(const char* pFilePath);
	static void DeleteInstance(CProject* p);
	CProject(const char* pName , const char* pComment);
	virtual ~CProject();

	void LoadSetting(LPCTSTR pIniFilePath);
	CIssueDateRecordSet* LoadIssueDateRecordSet();
	CLineDataRecordSet* GetLineDataRecordSetFromDatabase(LPCTSTR pQueryCondition , LPCTSTR pRevNo , const vector< STRING_T >& issueDates , const bool& reload=false);
	CLineDataRecordSet* GetIssuedLineDataRecordSet(LPCTSTR pQueryCondition);	/// 2015.09.23 added by humkyung
	const TCHAR* GetOutputIsoFileName(CLineDataRecord* pLineDataRecord);
	const TCHAR* GetOutputIsoFileExt(CLineDataRecord* pLineDataRecord , const CString& sRevNo=_T("LATEST"));
	CWeldDiaTable* GetWeldDiaTable() const;
	CMaterialCodeTable* GetMaterialCodeTable() const;
	int CollectAllRevNo( list< STRING_T >& revNos ) const;
	const TCHAR* GetInputIsoFileName(CLineDataRecord* pLineDataRecord);
	CRevDataRecord* CreateRevDataRecord();
	void Delete(vector<CLineDataRecord*> *pLineDataRecordEntry);
	void Undelete(vector<CLineDataRecord*> *pLineDataRecordEntry);
	void SaveToDatabase(vector<CLineDataRecord*>* pLineDataRecordEntry , bool saveResData=true , bool saveRevData=true);

	SMARTISO_DECLARE_MODULE_FUNC()
private:
	void CollectRevNos(list<STRING_T>& RevNoEntry , CAppPostgreSQL*) const;
	void LoadRevDataFromDatabase(CLineDataRecord *pLineDataRecord , CAppPostgreSQL*);
	void CollectAllLineDataRecord(CAppPostgreSQL*);
	CLineDataRecordSet* LoadLineDataRecordSet(CAppPostgreSQL*, LPCTSTR pQueryCondition , const char* pRevNo , const vector< STRING_T >& issueDates , const bool& reload=false);
	CLineDataRecordSet* LoadIssuedLineDataRecordSet(CAppPostgreSQL*, LPCTSTR pQueryCondition);	/// 2015.09.23 added by humkyung
	void LoadActiveUserData();
	CProjectAccess* GetAccessAt(const size_t& at);
	size_t GetAccessCount();
	
	string* m_name;
	STRING_T* m_desc;	/// 2013.01.05 added by humkyung
	string* m_pProjectNo;
	string* m_comment;
	string* m_pClientName;
	string* m_path;
	string* m_date;
	string* m_project_le_id;
	string* m_user_db_path;
	DatabaseProp *m_pDatabaseProp;	/// 2015.09.30 added by humkyung

	vector<CPlant>* m_pPlantList;	/// 2011.10.30 added by humkyung
	CLineDataRecordSet* m_pLineDataRecordSet;	/// contain all line data records

	//////////////////////////////////////////////////////////////////////////
	/// project table field.
	CDataFieldSet* m_pLineDataFieldSet;
	CDataFieldSet* m_pResDataFieldSet;
	CDataFieldSet* m_pRevDataFieldSet;
	//////////////////////////////////////////////////////////////////////////

	vector<string*>* m_pActiveUserIdEntry;

	vector<CProjectAccess*>* m_pProjectAccessEntry;
	auto_ptr<CMaterialCodeTable> m_materialCodeTable;
	auto_ptr<CWeldDiaTable> m_weldDiaTable;

	CIssueDateRecordSet* m_pIssueDataRecordSet;
	CBackupSystem* m_pBackupSystem;

	CActiveUser* m_pActiveUser;

	friend class CUser;
	friend class CActiveUser;
};

#endif // !defined(AFX_PROJECT_H__CF07E195_E016_4EF1_9FED_D605A02A423D__INCLUDED_)
