// LineListToOraDocData.h: interface for the CLineListToOraDocData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINELISTTOORADOCDATA_H__5D85C7F6_844A_4B45_AE15_060D27762D7F__INCLUDED_)
#define AFX_LINELISTTOORADOCDATA_H__5D85C7F6_844A_4B45_AE15_060D27762D7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AppData.h"
#include "Markup/Markup.h"
#include "../UserDLL/PDSOracle.h"

#include <string>
#include <list>
#include <map>
using namespace std;

class CSqlState
{
public:
	CSqlState(){}
	~CSqlState(){}

	int ParseSqlState(const STRING_T& m_sSqlFilePath);
	MCD_STR SqlStatement() const
	{
		return m_sSqlStatement;
	}

	map<MCD_STR , MCD_STR> m_oAttributes;
private:
	MCD_STR m_sSqlStatement;
};

class CCustomReportTable
{
public:
	typedef enum
	{
		METRIC = 0,
		INCH   = 1
	};

	CCustomReportTable()
	{
		m_pSqlState[0] = new CSqlState;
		m_pSqlState[1] = new CSqlState;
	}
	~CCustomReportTable()
	{
		try
		{
			delete m_pSqlState[0];
			delete m_pSqlState[1];
			m_pSqlState[0] = NULL;
			m_pSqlState[1] = NULL;
		}
		catch(...)
		{
		}
	}
	
	int OnSaveCustomDataReport(CMarkup& xml);	//! 2011.05.10 added by humkyung

	STRING_T GetCustomAttribute(const int& iProjectUnit , const STRING_T& sAttribute);
	STRING_T ParseCustomAttribute(const STRING_T& sAttributeType , const STRING_T& sValue , CPDSOracle &PDSOracle);

	MCD_STR m_sUnit;	//! 2011.05.10 added by humkyung
	MCD_STR m_sName;
	MCD_STR m_sSqlFile;
	MCD_STR m_sExcelFile;
	MCD_STR m_sStartRow;
	MCD_STR m_sDiscipline;

	CSqlState* m_pSqlState[2];
};

class CLineListToOraDocData  
{
	CLineListToOraDocData();
public:
	CString GetStdNoteFilePath() const;
	CString GetOraUserPassword() const;
	CString GetOraProjectName() const;
	CString GetOraUserName() const;
	CString GetOraServiceName() const;
	CString GetConnString() const;
	STRING_T GetUserName() const;
	CString GetProjectUnit();		//! 2011.04.26 - added by humkyung
	void SetAdminPassword( const STRING_T& rAdminPassword );
	void SetUserName( const STRING_T& rUserName );
	bool IsCorrectUser( const STRING_T& rUser , const STRING_T& rPassword );
	STRING_T GetAdminPassword() const;
	void LoadCustomDataReportList();
	void SaveCustomDataReportList();	//! 2011.05.10 added by humkyung
	bool LoadAppData(const STRING_T &rAppDataFilePath);
	void LoadIni( const STRING_T& rIniFilePath );
	STRING_T GetCurProjectName() const;
	STRING_T GetIniFilePath() const;
	static CLineListToOraDocData& GetInstance();
	static STRING_T GetExecPath();
	list<CCustomReportTable*>* CustomReportTableList()
	{
		return (&m_lstCustomReportTable);
	}
	int AddCustomReportTable(CCustomReportTable* pCustomReportTable);
	int DeleteCustomReportTableWith(CString& sName);
	bool ExistCustomReportTableWith(STRING_T sName);

	virtual ~CLineListToOraDocData();
private:
	STRING_T m_sIniFilePath;
	STRING_T m_rCurUserName;	///
	STRING_T m_rCurProjectName;	/// 현재 작업 중인 프로젝트 이름.

	CAppData m_AppData;			///

	CString m_rOraServiceName;
	CString m_rOraUserName;
	CString m_rOraUserPassword;
	CString m_rOraProjectName;	
	CString m_rStdNoteFilePath;

	CString	m_sProjectUnit;					//! 2011.04.26 - added by humkyung

	list<CCustomReportTable*> m_lstCustomReportTable;	//! 2011.05.04 - added by humkyung
public:	
	CString m_sProjectName;						//! 2011.08.12 - added by humkyung
};

#endif // !defined(AFX_LINELISTTOORADOCDATA_H__5D85C7F6_844A_4B45_AE15_060D27762D7F__INCLUDED_)
