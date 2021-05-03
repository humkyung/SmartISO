// QCCheck.h: interface for the CQCCheck class.
//
//////////////////////////////////////////////////////////////////////
 
#if !defined(AFX_QCCHECK_H__B2422C9F_12AB_4EA4_94E7_F5C5DC5F48BF__INCLUDED_)
#define AFX_QCCHECK_H__B2422C9F_12AB_4EA4_94E7_F5C5DC5F48BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <IsPoint2d.h>
#include <IsRegExp.h>
#include <DgnText.h>
#include "NtrData.h"
#include "BMLine.h"
#include <vector>
#include <map>
#include <sstream>
using namespace std;

class CIsoBMData
{
	CString ParseSizeNum( const CString& rString );
public:
	bool GetDataFromLineDataRecord(CLineDataRecord* pLineDataRecord);
	size_t GetSubStringCount() const;
	CString GetSubStringAt(const size_t& at) const;
	CString GetAllSubString() const;
	bool Parse();
	void AddSubText(const CString& rString);
	explicit CIsoBMData(const CIsPoint2d& origin , LPCTSTR p);
	explicit CIsoBMData(const CIsPoint2d& origin , CBMLine* p);

	CIsoBMData(const CIsoBMData& rhs)
	{
		(*this) = rhs;
	}
	
	/// 대입연산자
	CIsoBMData& operator=(const CIsoBMData& rhs)
	{
		if(this != &rhs)
		{
			m_pBMLine = rhs.m_pBMLine;
			m_iNo = rhs.m_iNo;
			m_rDrawingNo	= rhs.m_rDrawingNo;
			m_rMatlDesc	= rhs.m_rMatlDesc;
			m_rMatlSpec	= rhs.m_rMatlSpec;
			m_rBMCSSYM	= rhs.m_rBMCSSYM;
			m_rMatlCode	= rhs.m_rMatlCode;
			m_rQTY		= rhs.m_rQTY;
			m_rPaintCode	= rhs.m_rPaintCode;
			m_rInsType	= rhs.m_rInsType;
			m_rInsThickness = rhs.m_rInsThickness;
			
			m_origin	= rhs.m_origin;
			m_rNPD[0]	= rhs.m_rNPD[0];
			m_rNPD[1]	= rhs.m_rNPD[1];
			m_rDataString	= rhs.m_rDataString;

			m_rPlantNo	= rhs.m_rPlantNo;
			m_rAreaNo	= rhs.m_rAreaNo;
			m_rUnitNo	= rhs.m_rUnitNo;
			m_rSectionNo	= rhs.m_rSectionNo;
			
			m_sBoltLen = rhs.m_sBoltLen;	/// 2011.09.23 added by humkyung

			m_subTextEntry.clear();	/// 2012.08.31 added by humkyung
			m_subTextEntry.insert(m_subTextEntry.begin() , rhs.m_subTextEntry.begin() , rhs.m_subTextEntry.end());
		}

		return (*this);
	}

	void operator =(const char* p)
	{
		m_rDataString = p;
	}

	CString dataString() const
	{
		return m_rDataString;
	}

	static map<STRING_T , pair<int,int> > m_oBMAttrPosMap;	/// 2011.08.28 added by humkyung

	/// NO          MATERIALS DESCRIPTION            (IN)         SPEC    SYM       CODE              QTY 
	static int m_nFieldPos[7];
	CString m_rDrawingNo;
	CString m_rMatlDesc;
	CString m_rMatlSpec;
	CString m_rBMCSSYM;
	CString m_rMatlCode;	///< Mat'l code
	CString m_rPaintCode;
	CString m_rInsType , m_rInsThickness;
	
	CString m_sBoltLen;	/// 2011.09.23 added by humkyung

	int no() const;	/// 2012.04.25 added by humkyung

	CString matlDesc() const
	{
		return m_rMatlDesc;
	}

	CString GetMainSize() const
	{
		return m_rNPD[0];
	}

	CString GetSubSize() const
	{		
		return m_rNPD[1];
	}

	CString matlSpec() const
	{
		return m_rMatlSpec;
	}
	
	CString bmcsSYM() const
	{
		return m_rBMCSSYM;
	}

	CString matlCode() const
	{
		return m_rMatlCode;
	}

	CString QTY() const
	{
		double nQTY = 0.f;
		sscanf_s(m_rQTY , _T("%lf") , &nQTY);
		STRINGSTREAM_T oss;
		oss << nQTY;
		
		return oss.str().c_str();

		///return m_rQTY;
	}

	CIsPoint2d origin() const
	{
		return m_origin;
	}

	CString plantNo() const
	{
		return m_rPlantNo;
	}
	CString areaNo() const
	{
		return m_rAreaNo;
	}
	CString unitNo() const
	{
		return m_rUnitNo;
	}
	CString sectionNo() const
	{
		return m_rSectionNo;
	}
private:
	bool ParseType1();
	bool ParseType2();

	int m_iNo;			/// 2012.04.25 added by humkyung
	CBMLine* m_pBMLine;	/// 2013.02.26 added by humkyung
	CString m_rPlantNo , m_rAreaNo , m_rUnitNo , m_rSectionNo;
	CString m_rNPD[2];
	CString m_rQTY;

	CIsPoint2d m_origin;
	CString m_rDataString;
	vector<CString> m_subTextEntry;
};

class CBoxTextString
{
public:
	CBoxTextString(const CIsPoint2d& origin) : m_origin(origin)
	{
		m_iPipeNo = m_iPipeCutLengthNo = -1;
	}

	CIsPoint2d origin() const
	{
		return m_origin;
	}

	int& pipeNo()
	{
		return m_iPipeNo;
	}
	const int pipeNo() const
	{
		return m_iPipeNo;
	}

	int& pipeCutLengthNo()
	{
		return m_iPipeCutLengthNo;
	}
	const int pipeCutLengthNo() const
	{
		return m_iPipeCutLengthNo;
	}

	vector<CString> m_textStringEntry;
private:
	int m_iPipeNo , m_iPipeCutLengthNo;
	CIsPoint2d m_origin;
};

class CCutPipeLengthData
{
public:
	CString dataString() const;
	explicit CCutPipeLengthData(const char* p)
	{
		m_iNo = -1;
		m_rDataString = p;

		m_rNPD = (m_rDataString.Right(m_rDataString.GetLength() - 21));
		m_rNPD = m_rNPD.Left(8);
		m_rNPD.TrimRight();

		this->ParseNo();
		m_rNPD = ParseSizeNum(m_rNPD);
	}
	CCutPipeLengthData(const CCutPipeLengthData& data)
	{
		m_iNo = data.m_iNo;
		m_rDataString = data.m_rDataString;
		m_rNPD = data.m_rNPD;
	}

	int no() const;	/// 2012.04.25 added by humkyung
	int ParseNo();	/// 2012.04.25 added by humkyung

	CString length() const
	{
		CString rLength(m_rDataString.Right(m_rDataString.GetLength() - 7));
		rLength = rLength.Left(10);
		rLength.TrimRight();
		return rLength;
	}
	CString NPD() const
	{
		return m_rNPD;
	}
private:
	CString ParseSizeNum( const CString& rString );

	int m_iNo;	/// 2012.04.25 added by humkyung
	CString m_rDataString;
	CString m_rNPD;
};


class CQCCheck  
{
public:
	CNtrData* FindNtrData(CIsoBMData& IsoBMData);
	const CIsoBMData* FindPipe(const CString& rSize) const;
	bool Check(CString& rCheckMsg , const CString& rFilePath , const bool& bCheckIsoDwg = true);
	CQCCheck();
	virtual ~CQCCheck();

	vector<CNtrData> m_ntrTextEntry;
	vector<CString> m_prtTextEntry;

	/// 2013.06.12 added by humkyung
	static bool HasHeaderText( const CString& str );
	/// up to here
private:
	int SetupBMLine(vector<std::tr1::shared_ptr<CBMLine>>& oBMLineList , vector<CDgnText*>& systemBMTextEntry);

	bool IsSkipLine(const CString& sLine) const;			/// 2012.08.31 added by humkyung
	bool IsInstOrSpecialItem( const CIsoBMData& bmData );	/// 2011.06.22 added by humkyung
	bool IsInstOrSpecialItem( const CString& rTextString );
	int FindSupportDetailNo(vector<CIsoBMData>& graphicAreaTextEntry , const string& rSupportNo );
	void ParseColumnIndex(const CString& rTextString);
	void ParseBMTexts(vector<CDgnText*>& systemBMTextEntry);
	/// 2013.06.12 added by humkyung
	bool CheckBOMOver(CString &rResMsg) const;
	/// up to here
	int  CheckPCDSymbolError(CString& rResMsg);
	int  CheckF_G_BError(CString& rResMsg);
	int  CheckSupportBMError(CString& rResMsg);
	int  CheckPipeCuttingLengthError(CString& rResMsg);
	bool CheckSpeckBreakError(CString& rResMsg);
	int  CheckSymbolCodeNotFoundError(CString& rResMsg);
	int  CheckErrorReadError(CString& rResMsg);
	bool CheckDrawingNoError(CString& rResMsg);
	int  CheckTextModifiedError(CString& rResMsg);
	int  CheckHoldItemError(CString& rResMsg);
	int  CheckIsoFailError(CString& rResMsg);
	int  CheckNotFoundError(CString& rResMsg);
	void ExtractTextData(const CString& rFilePath);
	inline void InitInternalVariables();
	
	vector<CIsoBMData> m_generalDatas;			/// BM란의 PIPE SUPPORT , PIPE를 제외한 데이터
	vector<CIsoBMData> m_pipeDatas;				/// BM란의 파이프 데이터
	vector<CIsoBMData> m_supportDatas;			/// PIPE SUPPORT 데이터
	vector<CIsoBMData> m_InstOrSpecialDatas;	/// INSTRUMENT / SPECIAL 데이터
	vector<CIsoBMData> m_miscellDatas;			/// 

	vector<CCutPipeLengthData> m_cutPipeLengths;
	vector<CString> m_drawingNoDatas;
	vector<CString> m_modifiedTextEntry;
	vector<CIsoBMData> m_graphicAreaDatas;

	vector<CBoxTextString*> m_boxTextStringEntry;

	bool m_bBOMNotFound , m_bBOMOver;
	int  m_nModifiedText;	/// 수정된 텍스트의 갯수
	static bool m_DRWNO, m_ERRREAD, m_FGBERR, m_PIPECUTERR, m_NOTFOUNERR, m_FAILISOERR,
		m_PCDSYMERR, m_HOLDITEMERR, m_SPECBREAKERR,m_SYMCODEERR , m_BOMOVER;
	static bool m_bCheckSupportBMErr,m_bCheckTextModify;
	
	IsRegExp::RxNode *m_rxnode;
	IsRegExp::RxMatchRes m_rxmatch;

	friend class CBMReportForm;
	friend class CWeldDetailBMFile;
	friend class CIsoEditOptionDlg2;
};

#endif // !defined(AFX_QCCHECK_H__B2422C9F_12AB_4EA4_94E7_F5C5DC5F48BF__INCLUDED_)
