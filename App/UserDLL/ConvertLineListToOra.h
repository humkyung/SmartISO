// ConvertLineListToOra.h: interface for the CConvertLineListToOra class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONVERTLINELISTTOORA_H__99D4617C_8E6A_43D9_A6CA_70F4BBA7A324__INCLUDED_)
#define AFX_CONVERTLINELISTTOORA_H__99D4617C_8E6A_43D9_A6CA_70F4BBA7A324__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "stdafx.h"
#include <comdef.h>
#include <afxtempl.h>
#include <vector>
#include "ado/ADODB.h"
#include "OraDatabaseDefFile.h"
#include "PDSOracle.h"

using namespace std;

#ifdef	USER_EXPORTS
#define	USER_EXT_CLASS	__declspec(dllexport)
#else
#define	USER_EXT_CLASS	__declspec(dllimport)
#endif

struct tagMapData
{
	CString rFieldName;	///< oracle field  ÀÌ¸§.
	CString rColumn;	///

	tagMapData(){}
	tagMapData(const tagMapData& rhs)
	{
		rFieldName = rhs.rFieldName;
		rColumn    = rhs.rColumn;
	}

	void operator=(const tagMapData& rhs)
	{
		rFieldName = rhs.rFieldName;
		rColumn    = rhs.rColumn;
	}
};
typedef struct tagMapData MapData;

typedef struct
{
	CString rFieldName;	///< oracle field  ÀÌ¸§.
	CString rValue;		///
	CString rCellCol;	///< ¿¢¼¿ column ÀÌ¸§.
	int nErr;
}MapValue;

class CRowData
{
public:
	CRowData(int nSheetNo , int nRowNo) : m_nSheetNo(nSheetNo) , m_nRowNo(nRowNo) , m_nAffectedRecords(0){}
	CRowData(const CRowData& rhs)
	{
		m_nSheetNo = rhs.m_nSheetNo;
		m_nRowNo = rhs.m_nRowNo;

		m_nAffectedRecords = rhs.m_nAffectedRecords;
		m_MapValueEntry.insert(m_MapValueEntry.begin() , rhs.m_MapValueEntry.begin() , rhs.m_MapValueEntry.end());
	}
	CRowData& operator=(const CRowData& rhs)
	{
		if(this != &rhs)
		{
			m_nSheetNo = rhs.m_nSheetNo;
			m_nRowNo = rhs.m_nRowNo;

			m_nAffectedRecords = rhs.m_nAffectedRecords;
			m_MapValueEntry.insert(m_MapValueEntry.begin() , rhs.m_MapValueEntry.begin() , rhs.m_MapValueEntry.end());
		}
		return (*this);
	}

	virtual ~CRowData(){}
public:
	bool IsAllEmptyData() const;
	MapValue* GetMapValueAt(const int& at);
	size_t GetMapValueCount() const;
	int GetRowNo() const;
	int GetSheetNo() const;
	CString GetConditionString(vector<STRING_T>& rhs , COraDatabaseDefFile& oraDatabaseDefFile , CPDSOracle& pdsOracle) const;
	CString GetValue(const CString& rFieldName) const;
	CString GetExecuteString(const vector<STRING_T>& KeySetting , COraDatabaseDefFile& oraDatabaseDefFile , CPDSOracle& pdsOracle);
	void Add(const MapValue& mapValue)
	{
		m_MapValueEntry.push_back(mapValue);
	}

	int m_nAffectedRecords;
private:
	bool HasNotDigitValues(const CString& rString);
	int m_nSheetNo , m_nRowNo;
	vector<MapValue> m_MapValueEntry;
};

class USER_EXT_CLASS CConvertLineListToOra   
{
public:
	enum
	{
		PROGRAM_ERROR		= 0x00,
		TABLE_ERROR		= 0x01,
		CONVERT_OK		= 0x02,
		COMPARE_NEEDED		= 0x03,
		CONFIGURATION_ERROR	= 0x04,
		NOT_FOUND_PDTABLE	= 0x05,
		CONNECTION_FAILURE	= 0x06
	};

	int Convert(const CHAR_T *pExlpath , const CHAR_T *pInipath);
	BOOL IsDBOpen;
	_ConnectionPtr m_pCON;
	_ConnectionPtr m_pTempCON;
	CConvertLineListToOra();
	virtual ~CConvertLineListToOra();
	bool LoadExcelData(const CHAR_T* pExlpath , const CHAR_T* pInipath);
	CStringArray* GetExcelName(const char *p2Exlpath);
private:
	void WriteConvResultToExcel(const STRING_T& rExcelFilePath , const STRING_T& rIniFilePath);
	bool HasAlphaCharacter(const string& str) const;
	bool UpdateOracleDatabase(CADODB* pAdoDB , vector<CString>& tableNames);
	void ReadIniFile(vector<MapData>& MapDataEntry , const STRING_T& rIniFilePath);
	CString GetCellString( VARIANT& valCell );
	bool CreateMdbTable(const CHAR_T *pMDBpath,const CHAR_T *pInipath);
	bool DropMdbTable(CString sMDBpath);
	int GetLineDataRecordCount(CADODB* pAdoDb);
	long FieldName2Long(CString& rFieldName);

	CString m_rProjectName;
	CString m_rUserID , m_rPW , m_rDataSource;
	CString m_rStdNoteFilePath;	///< standard note file path

	CPDSOracle m_pdsOracle;
	COraDatabaseDefFile m_oraDatabaseDefFile;
	vector<STRING_T> m_KeyFieldNameSetting;
	vector<CRowData> m_RowDataEntry;
};

#endif // !defined(AFX_CONVERTLINELISTTOORA_H__99D4617C_8E6A_43D9_A6CA_70F4BBA7A324__INCLUDED_)
