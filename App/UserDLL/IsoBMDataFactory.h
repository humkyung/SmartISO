// QCCheck.h: interface for the CIsoBMDataFactory class.
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
#include <LineDataRecord.h>
#include "NtrData.h"
#include "BMLine.h"
#include "IsoEditError.h"

#include "UserDocData.h"

#include <vector>
#include <map>
#include <sstream>
using namespace std;

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
#define BM_NOT_FOUND				12
#define BM_CONSISTENCY_ERROR		13

class USER_EXT_CLASS CIsoBMData
{
	CString ParseSizeNum( const CString& rString );
public:
	bool GetDataFromLineDataRecord(CLineDataRecord* pLineDataRecord);
	size_t GetSubStringCount() const;
	CString GetSubStringAt(const size_t& at) const;
	CString GetAllSubString() const;
	bool Parse(map<STRING_T , pair<int,int> >* pBMAttrPosMap);
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

	void operator =(const TCHAR* p)
	{
		m_rDataString = p;
	}

	CString dataString() const
	{
		return m_rDataString;
	}

	/// NO          MATERIALS DESCRIPTION            (IN)         SPEC    SYM       CODE              QTY 
	static int m_nFieldPos[7];
	CString m_rDrawingNo;
	CString m_rMatlDesc;
	CString m_rMatlSpec;
	CString m_rBMCSSYM;
	CString m_rMatlCode;	/// Mat'l code
	CString m_rPaintCode;
	CString m_rInsType , m_rInsThickness;
	
	CString m_sBoltLen;		/// 2011.09.23 added by humkyung

	int no() const;			/// 2012.04.25 added by humkyung

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
	bool ParseType1(map<STRING_T , pair<int,int> >* pBMAttrPosMap);
	bool ParseType2(map<STRING_T , pair<int,int> >* pBMAttrPosMap);

	int m_iNo;			/// 2012.04.25 added by humkyung
	CBMLine* m_pBMLine;	/// 2013.02.26 added by humkyung
	CString m_rPlantNo , m_rAreaNo , m_rUnitNo , m_rSectionNo;
	CString m_rNPD[2];
	CString m_rQTY;

	CIsPoint2d m_origin;
	CString m_rDataString;
	vector<CString> m_subTextEntry;
};


class USER_EXT_CLASS CCutPipeLengthData
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


class USER_EXT_CLASS CIsoBMDataFactory  
{
public:
	CNtrData* FindNtrData(CIsoBMData& IsoBMData);
	const CIsoBMData* FindPipe(const CString& rSize) const;
	CIsoBMDataFactory();
	virtual ~CIsoBMDataFactory();

	void ParseBMTexts(vector<CDgnText*>& oSystemBMTextEntry , map<STRING_T , pair<int,int> >* pBMAttrPosMap , map<CString,CString>* pDefineTextForItem);

	vector<CNtrData> m_ntrTextEntry;
	vector<CString> m_prtTextEntry;
private:
	int SetupBMLine(vector<std::tr1::shared_ptr<CBMLine>>& oBMLineList , vector<CDgnText*>& systemBMTextEntry);

	bool IsSkipLine(const CString& sLine) const;			/// 2012.08.31 added by humkyung
	bool IsInstOrSpecialItem( const CIsoBMData& bmData , map<CString,CString>* pDefineTextForItem );	/// 2011.06.22 added by humkyung
	bool IsInstOrSpecialItem( const CString& rTextString , map<CString,CString>* pDefineTextForItem );
	int FindSupportDetailNo(vector<CIsoBMData>& graphicAreaTextEntry , const STRING_T& rSupportNo );
	void ParseColumnIndex(const CString& rTextString);
	inline void InitInternalVariables();
	
	vector<CIsoBMData> m_generalDatas;			/// BM란의 PIPE SUPPORT , PIPE를 제외한 데이터
	vector<CIsoBMData> m_pipeDatas;				/// BM란의 파이프 데이터
	vector<CIsoBMData> m_supportDatas;			/// PIPE SUPPORT 데이터
	vector<CIsoBMData> m_InstOrSpecialDatas;	/// INSTRUMENT / SPECIAL 데이터
	vector<CIsoBMData> m_miscellDatas;			/// 

	vector<CCutPipeLengthData> m_cutPipeLengths;
	
	IsRegExp::RxNode *m_rxnode;
	IsRegExp::RxMatchRes m_rxmatch;
};

#endif // !defined(AFX_QCCHECK_H__B2422C9F_12AB_4EA4_94E7_F5C5DC5F48BF__INCLUDED_)
