// IssueDateRecord.h: interface for the CIssueDateRecord class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISSUEDATERECORD_H__B4824662_3219_4C93_9229_90F58EDA0341__INCLUDED_)
#define AFX_ISSUEDATERECORD_H__B4824662_3219_4C93_9229_90F58EDA0341__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef MODULE_EXPORTS
	#define SMARTISO_MODULE_EXT_CLASS	__declspec(dllexport)
#else
	#define SMARTISO_MODULE_EXT_CLASS	__declspec(dllimport)
#endif

#include <string>
using namespace std;
class SMARTISO_MODULE_EXT_CLASS CIssueDateRecord  
{
public:
	static double GetValueFromIssueDate(const STRING_T& sIssueDate);	//! 2011.07.17 added by humkyung

	double GetValue() const;	//! 2011.07.17

	void Delete( const bool deleted);
	bool IsDeleted() const;
	void SetDateString(const char* p);
	const TCHAR* dateString() const;
	void SetIndex(long index);
	long index() const;
	CIssueDateRecord(long index , LPCTSTR p);
	CIssueDateRecord();
	virtual ~CIssueDateRecord();
private:
	long m_index;
	bool m_deleted;
	string* m_pDateString;
};

#endif // !defined(AFX_ISSUEDATERECORD_H__B4824662_3219_4C93_9229_90F58EDA0341__INCLUDED_)
