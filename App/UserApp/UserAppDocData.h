// UserAppDocData.h: interface for the CAppDocData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERAPPDOCDATA_H__F0CFEA27_B6E2_44FF_AC3A_4BCD550E6FDE__INCLUDED_)
#define AFX_USERAPPDOCDATA_H__F0CFEA27_B6E2_44FF_AC3A_4BCD550E6FDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <idflib/idflib/IDFFile.h>

#include <IsVolume.h>
#include <DgnText.h>
#include <Project.h>
#include <IssueDateRecordSet.h>
#include <PCDFile.h>

#include <list>
#include <map>
using namespace std;

#define C_PLANT_NO	_T("PLANT_NO")
#define C_AREA_NO	_T("AREA_NO")
#define C_UNIT_NO	_T("UNIT_NO")
#define C_SHEET_NO	_T("SHEET_NO")

#define CHECK_ERROR_COUNT			15

#define CAN_NOT_FIND_ISO_DWG_NAME	0
#define ERROR_READ					1
#define PIPE_CUTTING_LENGTH			2
#define NOT_FOUND_ERROR				3
#define FAIL_ISO_ERROR				4
#define PCD_SYSMBOL_ERROR			5
#define HOLD_ITEM_ERROR				6
#define SYMBOL_CODE_NOT_FOUND		7
#define SUPPORT_BM_ERROR			8
#define TEXT_MODIFIED				9
#define F_G_B_ERROR					10
#define SPEC_BREAK_ERROR			11
#define	BOM_OVER					12	/// 2013.06.12 added by humkyung
#define BM_NOT_FOUND				13
#define BM_CONSISTENCY_ERROR		14

typedef struct
{
	CString     sFieldName;
	CIsVolume   volume;
}IsoDwgDataArea;

typedef struct 
{
	CString rDesc;
	double PosX;
	double PosY;
	double dTextScale;	/// 2011.09.22 added by humkyung
}RevPos;

typedef struct  
{
	int    TextColorNo;
	int    FontNo;
	double widthMulti;
	double heightMulti;
	int    iTextWeight;	/// 2011.09.21 added by humkyung

	IsoDwgDataArea borderArea;
	IsoDwgDataArea SystemBMArea;
	IsoDwgDataArea IsoGraphicArea;
	list<IsoDwgDataArea> KeyDataAreaList;
}BorderSetting;

typedef struct 
{
	CString SmartISOKey;
	CString OraKey;
}OraMapKey;

typedef struct 
{
	CString FieldDesc;
	CString OracleName;
}LineWithOracle;

class CIsoEditError
{
public:
	CIsoEditError()
	{
		memset(m_nError , 0 , sizeof(int) * CHECK_ERROR_COUNT);
	}
	CIsoEditError(const CIsoEditError& rhs)
	{
		(*this) = rhs;
	}

	CIsoEditError& operator=(const CIsoEditError& rhs)
	{
		if(this != &rhs)
		{
			m_rFileName = rhs.m_rFileName;
			memcpy(m_nError , rhs.m_nError , sizeof(int) * CHECK_ERROR_COUNT);
		}

		return (*this);
	}

	CString OutputErrorString()
	{
		CString res;
		res.Format("%-40s" , m_rFileName);
		int nCount = sizeof(m_nError) / sizeof(m_nError[0]);
		for(int i = 0;i < nCount;++i)
		{
			 CString rTemp;
			 rTemp.Format("%d: %-4d", i + 1 , m_nError[i]);
			 res += rTemp;
		}

		return res;
	}

	CString m_rFileName;
	int m_nError[CHECK_ERROR_COUNT];
};

typedef struct
{
	long nNo;
	STRING_T rDefine , rText;
}RevisionStage;

typedef struct
{
	CString rPrefix , rDesc;
	int ShowOnRevTable;
}GradeDefine;

/// 2011.08.31 added by humkyung
class CBMOutputForamt
{
public:
	int Read();
	vector<STRING_T> m_oFieldFormatList;
};
/// up to here

typedef struct tagOracleSetting
{
	vector<STRING_T> m_IsoManKeySetting;
	CString m_OracleKey;

	CString m_sHost , m_sService;
	CString m_sPrjUser , m_sPrjPassword;
	CString m_sDesUser , m_sDesPassword;
}OracleSetting;

class CAppDocData  
{
	CAppDocData();
public:
	typedef struct
	{
		STRING_T ExlPath;
		int    ExCount;
		int    nExlStartRow;
		int    nNextColumn;
	}Drawing_Excel_Matching;

	typedef struct
	{
		int iLevel , iColor , iStyle , iWeight;
	}TextModifiedErrorException;

	/// 2012.08.14 added by humkyung
	enum IncreaseRevNo 
	{
		MajorRevNo,
		MinorRevNo
	};
	/// up to here

	static CAppDocData* GetInstance();
	static CString GetExecPath();
	virtual ~CAppDocData();
// Attributes
public:
	enum
	{
		ALPHABETIC	= 0x01,
		NUMERIC		= 0x02,
		ALPHPLUSNUM	= 0x03
	};

	CString m_rExecPath;
	
	CString m_sUserId;		/// 2015.12.09 added by humkyung
	CString m_rSiteName;	/// 2012.08.13 added by humkyung
	
	/// store regular expression for special item - 2011.06.22 added by humkyung
	map<CString,CString> m_oDefineInstOrSpecialItem;
	/// up to here

	CString m_rStdNoteFilePath;

	CString m_rIsoDwgFileFolder;
	CString m_rIDFFileFolder;
	CString m_rSpecFileFolder;
	CString m_ProjectArea;
	
	BorderSetting m_borderSetting;

	vector<IsoDwgDataArea> m_IsoDwgDataAreaEntry;

	vector<IsoDwgDataArea> m_RemoveAreaEntry;
	vector<CIsoEditError>  m_IsoEditErrorEntry;

	map<CString , RevPos> m_oLatestRevData;	/// 2011.10.25 added by humkyung

	vector<STRING_T> m_primaryKeySetting , m_LineDataModuleKeySetting;

	CImageList m_headerImgList;
	int m_ExCount;
	
	OracleSetting m_oOracleSetting;
	
	Drawing_Excel_Matching m_Drawing_Excel_Matching;
	CBMOutputForamt m_oBMOutputFormat;		/// 2011.08.31 added by humkyung
	TextModifiedErrorException m_oTextModifiedErrorException;	/// 2011.11.04 added by humkyung
	
	//CIDFFile m_oIDFFile;					/// 2013.05.26 added by humkyung

	static TCHAR* m_pCheckMessages[];
	static int m_nIsoEditErrorCount[CHECK_ERROR_COUNT];
	static vector<CString> ERROR_STRING_ENTRY;
private:
	CString m_rIniFilePath;
	STRING_T m_rProjectPath;	/// PROJECT PATH
	CProject* m_pProject;		/// 2015.08.20 added by humkyung
	STRING_T m_sLicenseId,m_sLicensePassword;	/// license id,password - 2016.10.07 added by humkyung
// Operations
public:
	int SetToken(const STRING_T&);	/// 2016.10.06 added by humkyung
	int SetLicense(const STRING_T&,const STRING_T&);	/// 2016.10.07 added by humkyung

	static long FieldName2Long(const CString &rFieldName);
	int Initialize(const CString&);		/// 2015.09.26 added by humkyung
	CString GetFileVersion() const;		/// 2013.11.17 added by humkyung
	int OpenIsoFile(const STRING_T& sIsoFilePath) const;	/// 2013.05.11 added by humkyung
	CString GetPrjIniFilePath() const;	/// 2012.09.19 added by humkyung
	STRING_T GetConnectionString();		/// 2012.08.31 added by humkyung
	STRING_T GetIsoFolder4RevAutomation() const;	/// 2012.01.04 added by humkyung

	bool IsTextModifiedErrorException(CDgnText* pDgnText2d) const;	/// 2011.11.04 added by humkyung

	bool IsThisGradeShow(const int& iPrefixNo) const;
	int GetNextRevisionStage(RevisionStage& stage , const int& nRevNo);
	STRING_T GetWorkingUnit() const { return m_sWorkingUnit; }
	STRING_T GetDVCSE() const { return m_sDVCS[0]; }
	STRING_T GetDVCSN() const { return m_sDVCS[1]; }
	STRING_T GetDVCSEL() const { return m_sDVCS[2]; }

	STRING_T drawing_type() const { return m_rDrawingType; }
	STRING_T GetProjectPath() const;
	STRING_T GetUnitString() const;
	CString GetProductBOMFolder() const;
	void ClearIsoEditError();
	IsoDwgDataArea* FindIsoDwgDataAreaCollideWith(CIsVolume vol);
	IsoDwgDataArea* FindIsoDwgDataAreaWithFieldName(const CString& sFieldName);
	CString GetFirstRevNoString() const;
	CString GetRevNoString(CLineDataRecord* pLineDataRecord , const CString& rRevNo = _T("LATEST"));
	int GetRevisionStage(RevisionStage& RevisionStage , const int& iRevNo);
	void SetArea(CString &rArea);
	CString GetWeldType(const CString &matl_spec , const CString &rMatlCode, const CString &rItem);
	const CString& GetTempFolder() const;
	CPCDFile& GetPCDFile(const CString& matl_spec);
	bool IsLastDate(const CString& rIssueDate);
	bool IsFixedLineDataFieldName(CString rFieldName);
	void InitContents();
	int appendHeader(ofstream &ofile);
	
	int SetProject(CProject*);
	CProject* GetProject() const;

	CIssueDateRecordSet* m_pIssueDateRecordSet;
	map<int,RevisionStage>*   m_pRevisionStageMap;
	vector<GradeDefine>*	  m_pGradeDefineEntry;
private:
	int ReadSettingFromDatabase();	/// 2012.08.14 added by humkyung
	int ReadIsoEditSetting( const CString& sIniFilePath );	/// 2011.11.04 added by humkyung
	void InitBorderSettings(const CString& rIniFilePath);
	void LoadBorderSetting(const CString& rIniFilePath);

	map<CString , CPCDFile*> m_pcdFileMap;

	STRING_T m_rUnit;
	STRING_T m_rDrawingType;

	/// 2009.03.11¿¡ Ãß°¡.
	STRING_T m_sWorkingUnit;
	STRING_T m_sDVCS[3];

	STRING_T m_sIsoFolder4RevAutomation;	/// 2012.01.04 added by humkyung
	STRING_T m_sToken;						/// 2016.10.06 added by humkyung
};

#endif // !defined(AFX_USERAPPDOCDATA_H__F0CFEA27_B6E2_44FF_AC3A_4BCD550E6FDE__INCLUDED_)
