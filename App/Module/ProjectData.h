// ProjectData.h: interface for the CProjectData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROJECTDATA_H__F405818B_BD35_4E97_A6F0_3479907E02C2__INCLUDED_)
#define AFX_PROJECTDATA_H__F405818B_BD35_4E97_A6F0_3479907E02C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <vector>
#include <map>
#include "Project.h"
using namespace std;

#ifdef MODULE_EXPORTS
	#define SMARTISO_MODULE_EXT_CLASS	__declspec(dllexport)
#else
	#define SMARTISO_MODULE_EXT_CLASS	__declspec(dllimport)
#endif

class SMARTISO_MODULE_EXT_CLASS CRevisionStage
{
public:
	CRevisionStage() : m_nNo(-1){}

	long m_nNo;
	STRING_T m_sDefine , m_sText;
};

class SMARTISO_MODULE_EXT_CLASS CProjectData  
{
	CProjectData();
public:
	static CProjectData* GetInstance();
	virtual ~CProjectData();

	int Load(const CString&,const CProject::DatabaseProp&);	/// 2011.08.06 added by humkyung
	int Save(const CString&,const CProject::DatabaseProp&);

	int ParseIssueDate(int& iYear , int& iMonth , int& iDay , const CString& sIssueDate , const CString& sDelimiter=_T("-")) const;	/// 2011.08.01 added by humkyung

	int SetIssueDateFormatNo(const CString& sIssueDateFormatNo);	//! 2011.07.17 added by humkyung
	int SetIssueDateDelimiter(const CString& sIssueDateDelimiter);	//! 2011.07.17 added by humkyung

	CString GetDisplayIssueDateFrom(const CString& sIssueDate) const;//! 2011.08.06 added by humkyung
	CString GetIssueDateFormat() const;	//! 2011.07.17 added by humkyung
	CString GetIssueDateFormatNo() const;	//! 2011.07.17 added by humkyung
	CString GetIssueDateDelimiter() const;	//! 2011.07.17 added by humkyung

	int GetRevisionStage(CRevisionStage& _RevisionStage , const CString& rRevNo);	//! 2011.08.20 added by humkyung
	map<STRING_T,STRING_T>* GetDrawingSetup();	/// 2015.11.03 added by humkyung
public:
	vector<string> m_primaryKeyEntry;
	vector<STRING_T> m_SmartISOKeySetting;
	vector<string> m_LineDataModuleKeySetting;
	map<CString,CString>*     m_pProjectSettingMap;
	map<int,CRevisionStage>*  m_pRevisionStageMap;
private:
	map<STRING_T,STRING_T>* m_pDrawingSetup;
};

#endif // !defined(AFX_PROJECTDATA_H__F405818B_BD35_4E97_A6F0_3479907E02C2__INCLUDED_)
