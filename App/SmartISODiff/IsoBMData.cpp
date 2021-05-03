#include "StdAfx.h"
#include <assert.h>
#include "IsRegExp.h"
#include "IsoBMData.h"

CIsoBMData::CIsoBMData(DPoint3d origin , const CString& sPartNo , const CString& sTextString)
{
	m_sPartNo = sPartNo;
	m_sDataString = sTextString;
}

CIsoBMData::CIsoBMData(const CIsoBMData& rhs)
{
	(*this) = rhs;
}

CIsoBMData::~CIsoBMData(void)
{
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-04
    @class      CIsoBMData
    @function   operator=
    @return     CIsoBMData&
    @param      const   CIsoBMData&
    @param      rhs
    @brief		대입 연산자
******************************************************************************/
CIsoBMData& CIsoBMData::operator=(const CIsoBMData& rhs)
{
	if(this != &rhs)
	{
		m_sType		= rhs.m_sType;

		m_sPartNo	= rhs.m_sPartNo;	
		m_sDrawingNo	= rhs.m_sDrawingNo;
		m_sMatlDesc	= rhs.m_sMatlDesc;
		m_sMatlSpec	= rhs.m_sMatlSpec;
		m_sBMCSSYM	= rhs.m_sBMCSSYM;
		m_sMatlCode	= rhs.m_sMatlCode;
		m_sQTY		= rhs.m_sQTY;
		m_sPaintCode	= rhs.m_sPaintCode;
		m_sInsType	= rhs.m_sInsType;
		m_sInsThickness = rhs.m_sInsThickness;

		m_sNPD[0]	= rhs.m_sNPD[0];
		m_sNPD[1]	= rhs.m_sNPD[1];
		m_sDataString	= rhs.m_sDataString;

		m_sPlantNo	= rhs.m_sPlantNo;
		m_sAreaNo	= rhs.m_sAreaNo;
		m_sUnitNo	= rhs.m_sUnitNo;
		m_sSectionNo	= rhs.m_sSectionNo;

		m_oDgnElementList.clear();
		m_oDgnElementList.insert(m_oDgnElementList.begin() , rhs.m_oDgnElementList.begin() , rhs.m_oDgnElementList.end());
		
		m_oSubTextList.insert(m_oSubTextList.begin() , rhs.m_oSubTextList.begin() , rhs.m_oSubTextList.end());

		for(map<STRING_T , int>::const_iterator itr = rhs.m_oCompareResultList.begin();itr != rhs.m_oCompareResultList.end();++itr)
		{
			m_oCompareResultList.insert(make_pair(itr->first , itr->second));
		}
	}

	return (*this);
}

map<STRING_T , pair<int,int>> CIsoBMData::m_oBMAttrPosMap;	/// 2011.08.28 added by humkyung

/**
	@brief	Main or Sub Size를 파싱한다. 분수의 경우에 소수점이 있는 것은 대분수이다.
	분수 형식을 소수점 형식으로 변환하여 리턴한다.

	@author	humkyung
*/
CString CIsoBMData::ParseSizeNum( const CString& rString )
{
	CString res(rString);

	int at = rString.Find( _T("/") );
	if(-1 != at)
	{
		CString numerator[2];
		numerator[0]        = rString.Left(at);
		CString denominator = rString.Right(rString.GetLength() - at - 1);
		
		at = numerator[0].Find( _T(".") );
		if( -1 != at )
		{
			numerator[1] = numerator[0].Right(numerator[0].GetLength() - at - 1);
			numerator[0] = numerator[0].Left(at);
			///numerator[1].TrimRight("0");
		}
		
		at = denominator.Find( _T(".") );
		if( -1 != at ) denominator.TrimRight( _T("0") );
		
		double value = 0.f;
		if(!denominator.IsEmpty() && !numerator[1].IsEmpty())
		{
			value = ATOF_T(numerator[0]) + (ATOF_T(numerator[1]) / ATOF_T(denominator));
		}
		else if(!denominator.IsEmpty() && numerator[1].IsEmpty())
		{
			value = ATOF_T(numerator[0]) / ATOF_T(denominator);
		}
		else
		{
			value = ATOF_T(numerator[0]) + ATOF_T(numerator[1]);
		}
		
		STRINGSTREAM_T oss;
		oss << value;
		
		return oss.str().c_str();
	}
	else
	{
		at = res.Find( _T(".") );
		if( -1 != at ) res.TrimRight( _T("0") );

		return res;
	}

	return CString( _T("") );
}

/******************************************************************************
    @author     humkyung
    @date       2011-09-20
    @class      CIsoBMData
    @function   AddSubText
    @return     void
    @param      CDgnText*
	@param      CString
    @param      &rString
    @brief
******************************************************************************/
void CIsoBMData::AddSubTextElm(CDgnText* pTextElm)
{

	assert(pTextElm && "pTextElm is NULL");

	if(pTextElm)
	{
		CString sTextString(pTextElm->textString());
		sTextString.Trim();

		m_oSubTextList.push_back(sTextString);
	}
}

//////////////////////////////////////////////////////////////////////////
/**	
	@brief	BM Text Line을 파싱한다.
	
	@author	humkyung

	@date	????.??.??

	@return	bool
*/
bool CIsoBMData::Parse()
{
	IsRegExp::RxNode *_rxnode = NULL;
	IsRegExp::RxMatchRes _rxmatch;

	//////////////////////////////////////////////////////////////////////////
	/// PT                                           NPS          MATL    BMCS      MATL 
	/// NO          MATERIALS DESCRIPTION            (IN)         SPEC    SYM       CODE              QTY 
	CString sString(m_sDataString);
	int nPos = 0 , nLastFindWhatLen = 0 , nCount = 0;

	for(map<STRING_T , pair<int,int> >::iterator itr = CIsoBMData::m_oBMAttrPosMap.begin();itr != CIsoBMData::m_oBMAttrPosMap.end();++itr)
	{
		if(-1 == itr->second.second)
			sString = m_sDataString.Mid(itr->second.first , m_sDataString.GetLength() - itr->second.first);
		else	sString = m_sDataString.Mid(itr->second.first , itr->second.second);

		m_oCompareResultList.insert(make_pair(itr->first , CIsoBMData::NONE));

		if(_T("MATL DESC") == itr->first)
		{
			m_sMatlDesc = sString;
			m_sMatlDesc.TrimLeft();
			m_sMatlDesc.TrimRight();
		}
		else if(_T("SIZE") == itr->first)
		{
			sString.TrimLeft();
			sString.TrimRight();
			const int nFindPos = sString.Find('X');
			if(-1 != nFindPos)
			{
				m_sNPD[0] = ParseSizeNum(sString.Left(nFindPos));
				m_sNPD[1] = ParseSizeNum(sString.Right(sString.GetLength() - nFindPos - 1));
			}
			else
			{
				m_sNPD[0] = ParseSizeNum(sString);
			}
		}
		else if(_T("MATL SPEC") == itr->first)
		{
			m_sMatlSpec = sString;
			m_sMatlSpec.TrimLeft();
			m_sMatlSpec.TrimRight();
			/// 중간에 공백이 나오기 전까지의 부분을 SPEC로 한다.
			const int at = m_sMatlSpec.Find(' ' , 0);
			if(-1 != at) m_sMatlSpec = m_sMatlSpec.Left(at);
		}
		else if(_T("SYM") == itr->first)
		{
			m_sBMCSSYM = sString;
			m_sBMCSSYM.TrimLeft();
			m_sBMCSSYM.TrimRight();
		}
		else if(_T("MATL CODE") == itr->first)
		{
			m_sMatlCode = sString;
			m_sMatlCode.TrimLeft();
			m_sMatlCode.TrimRight();
			/// 중간에 공백이 나오기 전까지의 부분을 MAT'L CODE로 한다.
			const int at = m_sMatlCode.Find(' ' , 0);
			if(-1 != at) m_sMatlCode = m_sMatlCode.Left(at);
		}
		else if(_T("QTY") == itr->first)
		{
			m_sQTY = sString;
			m_sQTY.TrimLeft();
			m_sQTY.TrimRight();
		}
	}

	/// BOLT TYPE일 경우 BOLT LENGTH를 구한다.
	if(_T("BT") == m_sBMCSSYM)
	{
		CString rSubString;
		for(vector<CString>::iterator itr = m_oSubTextList.begin();itr != m_oSubTextList.end();++itr)
		{
			rSubString += (*itr);
		}
		rSubString.MakeUpper();

		CString what(_T("[0-9]+(\\.[0-9]+)?(MM|M)"));	///< 숫자M 혹은 MM
		nPos = ::FindStringHelper(rSubString , what , 0 , nLastFindWhatLen , _rxnode, &_rxmatch);
		if( nPos >= 0 )
		{
			m_sNPD[1] = rSubString.Mid(_rxmatch.Open[0] , _rxmatch.Close[0] - _rxmatch.Open[0]);
			m_sNPD[1] = m_sNPD[1].Left(m_sNPD[1].GetLength() - 2);
		}
	}

	/// 분수 형식의 숫자를 사이언스 형식으로 바꾼다.
	if(!m_sNPD[0].IsEmpty())
	{
		int at = 0;
		if(-1 != (at = m_sNPD[0].Find(_T("/"))))
		{
			const double b = ATOF_T(m_sNPD[0].Right(m_sNPD[0].GetLength() - at - 1));
			
			double a = 0.f , c = 0.f;
			CString rQuotient = m_sNPD[0].Left(at);
			at = rQuotient.Find(_T("."));	///< 여기서 .은 소수점을 표시하는 것이 아니다.
			if(-1 != at)
			{
				a = ATOF_T(rQuotient.Left(at));
				c = ATOF_T(rQuotient.Right(rQuotient.GetLength() - at - 1));
			}
			else
			{
				c = ATOF_T(rQuotient);
			}

			m_sNPD[0].Format(_T("%lf") , a + (c / b));
		}
	}

	if(!m_sNPD[1].IsEmpty())
	{
		int at = 0;
		if(-1 != (at = m_sNPD[1].Find(_T("/"))))
		{
			const double b = ATOF_T(m_sNPD[1].Right(m_sNPD[1].GetLength() - at - 1));

			double a = 0.f , c = 0.f;
			CString rQuotient = m_sNPD[1].Left(at);
			at = rQuotient.Find( _T(".") );
			if(-1 != at)
			{
				a = ATOF_T(rQuotient.Left(at));
				c = ATOF_T(rQuotient.Right(rQuotient.GetLength() - at - 1));
			}
			else
			{
				c = ATOF_T(rQuotient);
			}
			m_sNPD[1].Format(_T("%lf") , a + (c / b));
		}
	}

	if (_rxnode)
	{
		IsRxFree (_rxnode);
		_rxnode = NULL;
	}

	return (7 == nCount);
}

/******************************************************************************
    @author     humkyung
    @date       2011-09-20
    @class      CIsoBMData
    @function   GetAllSubString
    @return     CString
    @brief	return all substrings
******************************************************************************/
CString CIsoBMData::GetAllSubString() const
{
	CString res;
	for(vector<CString>::const_iterator itr = m_oSubTextList.begin();itr != m_oSubTextList.end();++itr)
	{
		if(!itr->IsEmpty()) res += (*itr);
	}

	return res;
}

/******************************************************************************
    @author     humkyung
    @date       2011-09-21
    @class      CIsoBMData
    @function   operator==
    @return     bool
    @param      const   CIsoBMData&
    @param      rhs
    @brief		compare two iso bm data
******************************************************************************/
bool CIsoBMData::operator==(const CIsoBMData& rhs)
{
	if(m_sType == rhs.m_sType)
	{
		return ((m_sMatlCode == rhs.m_sMatlCode) && (m_sMatlDesc == rhs.m_sMatlDesc) && (m_sMatlSpec == rhs.m_sMatlSpec) && 
			(m_sNPD[0] == rhs.m_sNPD[0]) && (m_sNPD[1] == rhs.m_sNPD[1]) && (m_sQTY == rhs.m_sQTY));
	}

	return false;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-04
    @class      CIsoBMData
    @function   PartNo
    @return     CString
    @brief
******************************************************************************/
CString CIsoBMData::PartNo() const
{
	return m_sPartNo;
}

/******************************************************************************
    @author     humkyung
    @date       2011-12-19
    @class      CIsoBMData
    @function   GetCompareResultOf
    @return     int
    @param      const   STRING_T&
    @param      sItem
    @brief
******************************************************************************/
int CIsoBMData::GetCompareResultOf( const STRING_T& sItem )
{
	map<STRING_T , int>::iterator where = m_oCompareResultList.find(sItem);
	if(where != m_oCompareResultList.end())
	{
		return (where->second);
	}

	return CIsoBMData::NONE;
}

/******************************************************************************
    @author     humkyung
    @date       2011-12-19
    @class      CIsoBMData
    @function   SetCompareResult
    @return     int
    @param      const   STRING_T&
    @param      sItem   const
    @param      int&    iValue
    @brief
******************************************************************************/
int CIsoBMData::SetCompareResult( const STRING_T& sItem , const int& iValue )
{
	map<STRING_T , int>::iterator where = m_oCompareResultList.find(sItem);
	if(where != m_oCompareResultList.end())
	{
		(where->second) = iValue;
	}

	return ERROR_SUCCESS;
}