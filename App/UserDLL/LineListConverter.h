// LineListConverter.h: interface for the CLineListConverter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINELISTCONVERTER_H__99D4617C_8E6A_43D9_A6CA_70F4BBA7A324__INCLUDED_)
#define AFX_LINELISTCONVERTER_H__99D4617C_8E6A_43D9_A6CA_70F4BBA7A324__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "stdafx.h"
#include <comdef.h>
#include <afxtempl.h>
#include <vector>
#include <ado/ADODB.h>
using namespace std;

#include "UserDocData.h"

typedef struct
{
	CString rFieldName;
	CString rColumn;
}MapData;

class USER_EXT_CLASS CLineListConverter   
{
public:
	enum
	{
		PROGRAM_ERROR	= 0x00,
		TABLE_ERROR	= 0x01,
		INSERT_OK	= 0x02,
		COMPARE_NEEDED	= 0x03
	};

	int Convert(const char *pExlpath,const char *pMDBpath, const char *pTempMDBpath, const char *pInipath);
	BOOL IsDBOpen;
	_ConnectionPtr m_pCON;
	_ConnectionPtr m_pTempCON;
	CLineListConverter();
	virtual ~CLineListConverter();
	bool Run(const char* pExlpath,const char *pMDBpath, const char *pTempMDBpath, const char* pInipath);
	CStringArray* GetExcelName(const char *p2Exlpath);
private:
	CString GetCellString( VARIANT& valCell );
	bool CreateMdbTable(const char *pMDBpath,const char *pInipath);
	bool DropMdbTable(const char *pMDBpath);
	int GetLineDataRecordCount(CADODB* pAdoDb);
	long FieldName2Long(CString& rFieldName);

	static const CString EMPTY_CELL;
};

#endif // !defined(AFX_LINELISTCONVERTER_H__99D4617C_8E6A_43D9_A6CA_70F4BBA7A324__INCLUDED_)
