// QCCheck.cpp: implementation of the CIsoBMDataFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include <math.h>
#include <algorithm>
#include <functional>
#include <IsPoly2d.h>
#include <DgnDocument.h>
#include <DgnText.h>
#include <DgnLineString.h>
#include <IsRegExp.h>
#include "IsoBMDataFactory.h"
#include <Tokenizer.h>
#include <SplitPath.h>
#include <ProjectData.h>
#include <util/FileTools.h>

#include "IsoEditError.h"

#include <set>
#include <map>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IsRegExp::RxNode __declspec(dllimport) *IsRxCompile(LPCTSTR Regexp);
int __declspec(dllimport) IsRxExec(IsRegExp::RxNode *Regexp, LPCTSTR Data, int Len, LPCTSTR Start, IsRegExp::RxMatchRes *Match, unsigned int RxOpt);
int __declspec(dllimport) IsRxReplace(LPCTSTR rep, LPCTSTR Src, int len, IsRegExp::RxMatchRes match, LPTSTR *Dest, int *Dlen);
void __declspec(dllimport) IsRxFree(IsRegExp::RxNode *Node);

int CIsoBMData::m_nFieldPos[7];
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIsoBMDataFactory::CIsoBMDataFactory() : m_rxnode(NULL)
{
}

CIsoBMDataFactory::~CIsoBMDataFactory()
{
	try
	{
		if (m_rxnode)
		{
			IsRxFree (m_rxnode);
			m_rxnode = NULL;
		}
	}
	catch(...)
	{
		
	}
}

namespace
{
	enum
	{
		FIND_MATCH_CASE	= 0x0001,
		FIND_WHOLE_WORD	= 0x0002,
		FIND_DIRECTION_UP	= 0x0010,
		REPLACE_SELECTION	= 0x0100
	};

	bool __SortByTextPosY(const CDgnText* lhs , const CDgnText* rhs)
	{
		return (lhs->origin().y > rhs->origin().y);
	}


	struct IsMatchString : public binary_function<std::tr1::shared_ptr<CBMLine> , CString, bool>
	{
	//          typedef T first_argument_type;
	//          typedef T second_argument_type;
	//          typedef T result_type;

		bool operator ()( const std::tr1::shared_ptr<CBMLine> a, const CString& b ) const
		{
			CString str = a->textString();
			str.TrimLeft();
			str.TrimRight();
			return (str == b);
		}
	};

	struct IsMatchData : public binary_function<CIsoBMData , CString , bool>
	{
	//          typedef T first_argument_type;
	//          typedef T second_argument_type;
	//          typedef T result_type;

		bool operator ()( const CIsoBMData& lhs, const CString& rhs ) const
		{
			CString str = lhs.dataString();
			str.TrimLeft();
			str.TrimRight();
			return (str == rhs);
		}
	};

	struct IsInnerPt : public binary_function<CIsPoly2d , CIsPoint2d , bool>
	{
		bool operator()(const CIsPoly2d& lhs , const CIsPoint2d& rhs) const
		{
			return lhs.IsInnerPt(rhs);
		}
	};

	struct HasSameyCoord : public binary_function<std::tr1::shared_ptr<CBMLine> , DPoint3d , bool>
	{
		bool operator()(const std::tr1::shared_ptr<CBMLine> rhs , const DPoint3d& lhs) const
		{
			return (lhs.y == rhs->yCoord());
		}
	};
};

/**	
	@brief	내부 변수 초기화.

	@author	humkyung

	@return	void
*/
void CIsoBMDataFactory::InitInternalVariables()
{
	m_ntrTextEntry.clear();
	m_prtTextEntry.clear();
	
	m_pipeDatas.clear();
	m_generalDatas.clear();
	m_supportDatas.clear();
	m_InstOrSpecialDatas.clear();

	m_cutPipeLengths.clear();
}

/******************************************************************************
    @author     humkyung
    @date       2012-08-31
    @class      CIsoBMDataFactory
    @function   IsSkipLine
    @return     bool
    @param      const   CString&
    @param      sLine
    @brief
******************************************************************************/
bool CIsoBMDataFactory::IsSkipLine(const CString& sLine) const
{
	int at = sLine.Find( _T("PT") );
	if(0 == at) return true;
	at = sLine.Find( _T("MATERIALS DESCRIPTION") );
	if(-1 != at) return true;
	at = sLine.Find( _T("___") );
	if(0 == at) return true;
	at = sLine.Find( _T("PIPE SUPPORTS") );
	if(-1 != at) return true;
	at = sLine.Find( _T("CUT PIPE LENGTH") );
	if(-1 != at) return true;
	
	return false;
}

/**	
	@brief	create bm line list with dgn text
	
	@author	humkyung
	
	@param	oBMLineList			store bm line
	@param	systemBMTextEntry

	@return	int
*/
int CIsoBMDataFactory::SetupBMLine(vector<std::tr1::shared_ptr<CBMLine>>& oBMLineList , vector<CDgnText*>& systemBMTextEntry)
{
	stable_sort(systemBMTextEntry.begin() , systemBMTextEntry.end() , __SortByTextPosY);

	for(vector<CDgnText*>::iterator itr = systemBMTextEntry.begin();itr != systemBMTextEntry.end();++itr)
	{
		DPoint3d origin = (*itr)->origin();

		CBMLine* pNewBMLine = NULL;
		vector<std::tr1::shared_ptr<CBMLine>>::iterator where = find_if(oBMLineList.begin() , oBMLineList.end() , bind2nd(HasSameyCoord() , origin));
		if(where == oBMLineList.end())
		{
			std::tr1::shared_ptr<CBMLine> oNewBMLine(new CBMLine((*itr)));
			oBMLineList.push_back( oNewBMLine );
		}
		else
		{
			pNewBMLine = (*where).get();
			pNewBMLine->Add(*itr);
		}
	}

	return ERROR_SUCCESS;
}

static bool HasPipeSupportPrefix(const CString& str)
{
	const CString aryPrefix[]={_T("SUPPORT DATA:"),_T("PLAN:")};
	for(int i = 0;i < SIZE_OF_ARRAY(aryPrefix);++i)
	{
		if(-1 != str.Find(aryPrefix[i])) return true;
	}

	return false;
}

static bool HasHeaderText( const CString& str )
{
	const CString aryPrefix[]={_T("MATERIALS DESCRIPTION") , _T("COMPONENT DESCRIPTION") , _T("MATERIAL DESCRIPTION")};
	for(int i = 0;i < SIZE_OF_ARRAY(aryPrefix);++i)
	{
		if(-1 != str.Find(aryPrefix[i])) return true;
	}

	return false;
}

/**
	@brief	check whether it is speical or instrument item

	@author	humkyung

	@date	????.??.??
*/
bool CIsoBMDataFactory::IsInstOrSpecialItem( const CString& rTextString , map<CString,CString>* pDefineTextForItem )
{
	assert((NULL != pDefineTextForItem) && "pDefineTextForItem is NULL");

	if(NULL != pDefineTextForItem)
	{
		map<CString,CString>::iterator where = pDefineTextForItem->find(_T("FullText"));
		if(where != pDefineTextForItem->end())
		{
			CString sRegExp = where->second;
			int nLastFindWhatLen = 0;
			int nPos = ::FindStringHelper(rTextString , sRegExp , 0, nLastFindWhatLen, m_rxnode, &m_rxmatch);
			if(nPos > 0)
			{
				return true;
			}
		}
	}
	
	return false;
}

/**
	@brief	check whether it is speical or instrument item

	@author	humkyung

	@date	2011.06.22
*/
bool CIsoBMDataFactory::IsInstOrSpecialItem( const CIsoBMData& bmData , map<CString,CString>* pDefineTextForItem )
{
	assert((NULL != pDefineTextForItem) && "pDefineTextForItem is NULL");

	if(NULL != pDefineTextForItem)
	{
		for(map<CString,CString>::iterator itr = pDefineTextForItem->begin();itr != pDefineTextForItem->end();++itr)
		{
			if(0 == itr->first.CompareNoCase(_T("MatlCode")))
			{
				int nLastFindWhatLen = 0;
				int nPos = ::FindStringHelper(bmData.matlCode() , itr->second , 0, nLastFindWhatLen, m_rxnode, &m_rxmatch);
				if(nPos > 0)
				{
					return true;
				}
			}
		}
	}

	return false;
}

/**	
	@brief	BM Text에 대한 CIsoBMData를 생성한다.
	
	@author	humkyugn
	
	@param	systemBMTextEntry	a parameter of type vector<CDgnText*>&

	@return	void
*/
void CIsoBMDataFactory::ParseBMTexts(vector<CDgnText*>& oSystemBMTextEntry , map<STRING_T , pair<int,int> >* pBMAttrPosMap , 
									 map<CString,CString>* pDefineTextForItem)
{
	assert((NULL != pBMAttrPosMap) && (NULL != pDefineTextForItem) && ("pBMAttrPosMap or pDefineTextForItem is NULL"));

	if((NULL != pBMAttrPosMap) && (NULL != pDefineTextForItem))
	{
		/// setup bm line list - 2012.03.14 added by humkyung
		vector<std::tr1::shared_ptr<CBMLine>> oBMLineList;
		this->SetupBMLine(oBMLineList , oSystemBMTextEntry);
		/// up to here

		vector<std::tr1::shared_ptr<CBMLine>>::iterator wherePipeSupport = find_if(oBMLineList.begin() , oBMLineList.end() , bind2nd(IsMatchString() , _T("PIPE SUPPORTS")));
		vector<std::tr1::shared_ptr<CBMLine>>::iterator whereMISCELL = find_if(oBMLineList.begin(), oBMLineList.end() , bind2nd(IsMatchString(), _T("MISCELLANEOUS COMPONENTS"))); ///< MISCELLANEOUS COMPONENTS의 where를 찾는다.

		for(vector<std::tr1::shared_ptr<CBMLine>>::iterator itr = oBMLineList.begin();itr != oBMLineList.end();++itr)
		{
			CString rTextString = (*itr)->textString();
			if(rTextString.IsEmpty()) continue;

			/// NO          MATERIALS DESCRIPTION            (IN)         SPEC    SYM       CODE              QTY 
			/// find field position
			if(HasHeaderText(rTextString))
			{
				ParseColumnIndex(rTextString);
				continue;
			}
			if(this->IsSkipLine(rTextString)) continue;

			rTextString = rTextString.Left(5);
			rTextString.TrimLeft();
			//////////////////////////////////////////////////////////////////////////
			/// regular expression
			try
			{
				int nLastFindWhatLen = 0;
				int nPos = ::FindStringHelper(rTextString , _T("[0-9]+") , 0, nLastFindWhatLen, m_rxnode, &m_rxmatch);
				if(0 == nPos)
				{
					bool bInstOrSpecialItem = false;
					/// PIPE 부분 - PIPE SUPPORTS 부분이 둘다 있을 경우
					if(oBMLineList.end() != wherePipeSupport)
					{
						if(itr < wherePipeSupport)
						{
							/// 인스트루먼트 파이핑 스페셜리을 체크한다.
							rTextString = (*itr)->textString();
							rTextString.TrimRight();
							rTextString.TrimLeft();
							if(IsInstOrSpecialItem(rTextString , pDefineTextForItem)) bInstOrSpecialItem = true;

							CIsoBMData bmData(CIsPoint2d((*itr)->origin().x , (*itr)->origin().y) , (*itr).get());
							for(++itr;itr != oBMLineList.end();++itr)
							{
								/// 어디까지가 하나의 ITEM DATA되는지 확인
								rTextString = (*itr)->textString();
								if(rTextString.IsEmpty()) continue;
								rTextString.TrimLeft();rTextString.TrimRight();
								nPos = ::FindStringHelper(rTextString , _T("[0-9]+ ") , 0, nLastFindWhatLen, m_rxnode, &m_rxmatch);
								if(0 == nPos)
								{
									--itr;
									break;
								}
								nPos = ::FindStringHelper(rTextString , _T("\\<[0-9]+\\>") , 0 , nLastFindWhatLen , m_rxnode , &m_rxmatch); 
								if(-1 != nPos)
								{
									--itr;
									break;
								}

								if(this->IsSkipLine(rTextString))
								{
									--itr;
									break;
								}

								rTextString = (*itr)->textString();
								rTextString.TrimLeft();
								rTextString.TrimRight();
								bmData.AddSubText(rTextString);
							}

							if(true == bmData.Parse(pBMAttrPosMap))
							{
								const int nFindPos = bmData.m_rMatlDesc.Find( _T("PIPE") );
								if(-1 != nFindPos)
									m_pipeDatas.push_back(bmData);
								else if((true == bInstOrSpecialItem) || IsInstOrSpecialItem(bmData , pDefineTextForItem))	//! 2011.06.22 modified by humkyung
									m_InstOrSpecialDatas.push_back(bmData);
								else	m_generalDatas.push_back(bmData);
							}
						}
						else if( (wherePipeSupport < itr) && (itr < whereMISCELL) ) /// 서포트 부분
						{
							rTextString = (*itr)->textString();
							rTextString.TrimLeft();
							rTextString.TrimRight();
							bool has = HasPipeSupportPrefix(rTextString);
							if(false == has) continue;
							/// 
							CIsoBMData bmData(CIsPoint2d((*itr)->origin().x , (*itr)->origin().y) , (*itr).get());
							for(++itr;itr != oBMLineList.end();++itr)
							{
								rTextString = (*itr)->textString();
								if(rTextString.IsEmpty()) continue;
								rTextString.TrimLeft(); rTextString.TrimRight();						

								nPos = ::FindStringHelper(rTextString , _T("[0-9]+ SUPPORT DATA:") , 0, nLastFindWhatLen, m_rxnode, &m_rxmatch);
								if(0 == nPos)
								{
									--itr;
									break;
								}
								nPos = ::FindStringHelper(rTextString , _T("\\<[0-9]+\\>") , 0 , nLastFindWhatLen , m_rxnode , &m_rxmatch); 
								if(-1 != nPos)
								{
									--itr;
									break;
								}

								if(this->IsSkipLine(rTextString))
								{
									--itr;
									break;
								}

								rTextString = (*itr)->textString();
								rTextString.TrimLeft();
								rTextString.TrimRight();
								bmData.AddSubText(rTextString);
							}
							if(true == bmData.Parse(pBMAttrPosMap))
							{
								m_supportDatas.push_back(bmData);
							}
						}
						else	/// MISCELLANEOUS 부분
						{
							rTextString = (*itr)->textString();
							rTextString.TrimLeft();
							rTextString.TrimRight();
							/// 
							CIsoBMData bmData(CIsPoint2d((*itr)->origin().x , (*itr)->origin().y) , (*itr).get());
							for(++itr;itr != oBMLineList.end();++itr)
							{
								rTextString = (*itr)->textString();
								if(rTextString.IsEmpty()) continue;
								nPos = ::FindStringHelper(rTextString , _T("[0-9]+ ") , 0, nLastFindWhatLen, m_rxnode, &m_rxmatch);
								if(0 == nPos)
								{
									--itr;
									break;
								}
								nPos = ::FindStringHelper(rTextString , _T("\\<[0-9]+\\>") , 0 , nLastFindWhatLen , m_rxnode , &m_rxmatch); 
								if(-1 != nPos)
								{
									--itr;
									break;
								}

								rTextString.TrimLeft(); rTextString.TrimRight();
								nPos = ::FindStringHelper(rTextString , _T("[0-9]+ SUPPORT DATA:") , 0, nLastFindWhatLen, m_rxnode, &m_rxmatch);
								if(0 == nPos)
								{
									--itr;
									break;
								}
								nPos = ::FindStringHelper(rTextString , _T("[0-9]+ PLAN:") , 0, nLastFindWhatLen, m_rxnode, &m_rxmatch);
								if(0 == nPos)
								{
									--itr;
									break;
								}

								if(this->IsSkipLine(rTextString))
								{
									--itr;
									break;
								}

								rTextString = (*itr)->textString();
								rTextString.TrimLeft();
								rTextString.TrimRight();
								bmData.AddSubText(rTextString);
							}
							if(true == bmData.Parse(pBMAttrPosMap))
							{
								if(IsInstOrSpecialItem(bmData , pDefineTextForItem))	/// 2011.06.22 added by humkyung
									m_InstOrSpecialDatas.push_back(bmData);
								else	m_miscellDatas.push_back(bmData);
							}
						}
					}
					else
					{
						/// 인스트루먼트 파이핑 스페셜리을 체크한다.
						rTextString = (*itr)->textString();
						rTextString.TrimRight();
						rTextString.TrimLeft();
						if(IsInstOrSpecialItem(rTextString , pDefineTextForItem)) bInstOrSpecialItem = true;
						/// PIPE SUPPORTS 부분이 없을 경우.

						CIsoBMData bmData(CIsPoint2d((*itr)->origin().x , (*itr)->origin().y) , (*itr).get());
						for(++itr;itr != oBMLineList.end();++itr)
						{
							rTextString = (*itr)->textString();
							if(rTextString.IsEmpty()) continue;
							rTextString = rTextString.Trim();
							nPos = ::FindStringHelper(rTextString , _T("[0-9]+ ") , 0, nLastFindWhatLen, m_rxnode, &m_rxmatch);
							if(0 == nPos)
							{
								--itr;
								break;
							}
							nPos = ::FindStringHelper(rTextString , _T("\\<[0-9]+\\>") , 0 , nLastFindWhatLen , m_rxnode , &m_rxmatch); 
							if(-1 != nPos)
							{
								--itr;
								break;
							}

							if(this->IsSkipLine(rTextString))
							{
								--itr;
								break;
							}

							rTextString = (*itr)->textString();
							rTextString.TrimLeft();
							rTextString.TrimRight();
							bmData.AddSubText(rTextString);
						}
						//if(!ExtractPipeCompTextData(*itr)) continue;
						if(true == bmData.Parse(pBMAttrPosMap))
						{
							/// 'TUBE' is added - 2012.04.20 added by humkyung
							const bool isPipe = (-1 != bmData.m_rMatlDesc.Find( _T("PIPE") )) || (-1 != bmData.m_rMatlDesc.Find( _T("TUBE") ));
							/// up to here
							if(true == isPipe)
								m_pipeDatas.push_back(bmData);
							else if((true == bInstOrSpecialItem) || IsInstOrSpecialItem(bmData , pDefineTextForItem))	/// 2011.06.22 modified by humkyung
								m_InstOrSpecialDatas.push_back(bmData);
							else	m_generalDatas.push_back(bmData);
						}
					}
				}
				else
				{
					/// PIPE CUT LENGTH 부분
					CString line((*itr)->textString());
					CString what( _T("\\<[0-9]+\\>") );
					int nFoundPos = -1;
					int nMatchLen = what.GetLength();
					int nLineLen = line.GetLength();
					int nPos = -1 , nFoundCount = 0;
					do
					{
						nPos = ::FindStringHelper(line , what , 0 , nLastFindWhatLen , m_rxnode, &m_rxmatch);
						if( nPos >= 0 )
						{
							nFoundPos = (nFoundPos == -1)? nPos : nFoundPos + nPos;
							nFoundPos+= nMatchLen;
							line = line.Right( nLineLen - (nMatchLen + nPos) );
							nLineLen = line.GetLength();
							++nFoundCount;
						}
					}
					while( nPos >= 0 );

					line = (*itr)->textString();
					if( 2 == nFoundCount)
					{
						m_cutPipeLengths.push_back(CCutPipeLengthData(line.Left(42)));
						m_cutPipeLengths.push_back(CCutPipeLengthData(line.Right(line.GetLength() - 42)));
					}
					else if(1 == nFoundCount)
					{
						m_cutPipeLengths.push_back(CCutPipeLengthData(line));
					}
				}
			}
			catch(...)
			{

			}
			if(oBMLineList.end() == itr) break;	/// ENTRY의 맨 마지막이면 종료한다.
		}
	}
}

/**
	@brief	return "NO"

	@author	humkyung

	@date	2012.04.25

	@return	 int
*/
int CIsoBMData::no() const
{
	return m_iNo;
}

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
		
		at = numerator[0].Find(".");
		if( -1 != at )
		{
			numerator[1] = numerator[0].Right(numerator[0].GetLength() - at - 1);
			numerator[0] = numerator[0].Left(at);
		}
		
		at = denominator.Find(".");
		if( -1 != at ) denominator.TrimRight("0");
		
		double value = 0.f;
		if(!denominator.IsEmpty() && !numerator[1].IsEmpty())
		{
			value = atof(numerator[0]) + (atof(numerator[1]) / atof(denominator));
		}
		else if(!denominator.IsEmpty() && numerator[1].IsEmpty())
		{
			value = atof(numerator[0]) / atof(denominator);
		}
		else
		{
			value = atof(numerator[0]) + atof(numerator[1]);
		}
		
		STRINGSTREAM_T oss;
		oss << value;
		
		return oss.str().c_str();
	}
	else
	{
		at = res.Find( _T(".") );
		if( (-1 != at ) && ((res.GetLength() - at - 1) > 1) ) res.TrimRight( _T("0") );

		return res;
	}

	return CString( _T("") );
}

/**	
	@brief	하나의 BM Text Line을 파싱한다.
	
	@author	humkyung

	@date	2013.02.14

	@return	bool
*/
bool CIsoBMData::ParseType1(map<STRING_T , pair<int,int> >* pBMAttrPosMap)
{
	assert((NULL != pBMAttrPosMap) && ("pBMAttrPosMap is NULL"));
	if(NULL != pBMAttrPosMap)
	{
		if(pBMAttrPosMap->empty()) return false;	/// 2013.05.03 added by humkyung

		//////////////////////////////////////////////////////////////////////////
		/// PT                                           NPS          MATL    BMCS      MATL 
		/// NO          MATERIALS DESCRIPTION            (IN)         SPEC    SYM       CODE              QTY 
		CString sString(m_rDataString);
		int nPos = 0 , nLastFindWhatLen = 0 , nCount = 0;

		for(map<STRING_T , pair<int,int> >::iterator itr = pBMAttrPosMap->begin();itr != pBMAttrPosMap->end();++itr)
		{
			if(-1 == itr->second.second)
				sString = m_rDataString.Mid(itr->second.first , m_rDataString.GetLength() - itr->second.first);
			else	sString = m_rDataString.Mid(itr->second.first , itr->second.second);
			if(_T("MATL DESC") == itr->first)
			{
				m_rMatlDesc = sString;
				m_rMatlDesc.TrimLeft();
				m_rMatlDesc.TrimRight();
			}
			else if(_T("SIZE") == itr->first)
			{
				sString.TrimLeft();
				sString.TrimRight();
				const int nFindPos = sString.Find('X');
				if(-1 != nFindPos)
				{
					m_rNPD[0] = ParseSizeNum(sString.Left(nFindPos));
					m_rNPD[1] = ParseSizeNum(sString.Right(sString.GetLength() - nFindPos - 1));
				}
				else
				{
					m_rNPD[0] = ParseSizeNum(sString);
				}
			}
			else if(_T("MATL SPEC") == itr->first)
			{
				m_rMatlSpec = sString;
				m_rMatlSpec.TrimLeft();
				m_rMatlSpec.TrimRight();
				/// 중간에 공백이 나오기 전까지의 부분을 SPEC로 한다.
				const int at = m_rMatlSpec.Find(' ' , 0);
				if(-1 != at) m_rMatlSpec = m_rMatlSpec.Left(at);
			}
			else if(_T("SYM") == itr->first)
			{
				m_rBMCSSYM = sString;
				m_rBMCSSYM.TrimLeft();
				m_rBMCSSYM.TrimRight();
			}
			else if(_T("MATL CODE") == itr->first)
			{
				m_rMatlCode = sString;
				m_rMatlCode.TrimLeft();
				m_rMatlCode.TrimRight();
				/// 중간에 공백이 나오기 전까지의 부분을 MAT'L CODE로 한다.
				const int at = m_rMatlCode.Find(' ' , 0);
				if(-1 != at) m_rMatlCode = m_rMatlCode.Left(at);
			}
			/// extract bolt length - 2011.09.23 added by humkyung
			else if(_T("BOLT LEN") == itr->first)
			{
				sString.TrimLeft();
				sString.TrimRight();
				m_sBoltLen = ParseSizeNum(sString);
			}
			/// up to here
			else if(_T("QTY") == itr->first)
			{
				m_rQTY = sString;
				m_rQTY.TrimLeft();
				m_rQTY.TrimRight();
			}
		}

		return true;
	}

	return false;
}

/**	
	@brief	여러개로 이루어진 BM Text Line을 파싱한다.
	
	@author	humkyung

	@date	2013.02.14

	@return	bool
*/
bool CIsoBMData::ParseType2(map<STRING_T , pair<int,int> >* pBMAttrPosMap)
{
	assert((NULL != pBMAttrPosMap) && "pBMAttrPosMap is NULL");

	if(NULL != pBMAttrPosMap)
	{
		//////////////////////////////////////////////////////////////////////////
		/// PT                                           NPS          MATL    BMCS      MATL 
		/// NO          MATERIALS DESCRIPTION            (IN)         SPEC    SYM       CODE              QTY 
		CString sString(m_rDataString);
		int nPos = 0 , nLastFindWhatLen = 0 , nCount = 0;

		int iIndex = 0 , iTextCount = m_pBMLine->GetTextCount();
		for(map<STRING_T , pair<int,int> >::iterator itr = pBMAttrPosMap->begin();(itr != pBMAttrPosMap->end()) && (iIndex < iTextCount);++itr,++iIndex)
		{
			sString = m_pBMLine->GetTextAt(iIndex);
			if(_T("MATL DESC") == itr->first)
			{
				m_rMatlDesc = sString;
				m_rMatlDesc.TrimLeft();
				m_rMatlDesc.TrimRight();
			}
			else if(_T("SIZE") == itr->first)
			{
				sString.TrimLeft();
				sString.TrimRight();
				const int nFindPos = sString.Find('X');
				if(-1 != nFindPos)
				{
					m_rNPD[0] = ParseSizeNum(sString.Left(nFindPos));
					m_rNPD[1] = ParseSizeNum(sString.Right(sString.GetLength() - nFindPos - 1));
				}
				else
				{
					m_rNPD[0] = ParseSizeNum(sString);
				}
			}
			else if(_T("MATL SPEC") == itr->first)
			{
				m_rMatlSpec = sString;
				m_rMatlSpec.TrimLeft();
				m_rMatlSpec.TrimRight();
				/// 중간에 공백이 나오기 전까지의 부분을 SPEC로 한다.
				const int at = m_rMatlSpec.Find(' ' , 0);
				if(-1 != at) m_rMatlSpec = m_rMatlSpec.Left(at);
			}
			else if(_T("SYM") == itr->first)
			{
				m_rBMCSSYM = sString;
				m_rBMCSSYM.TrimLeft();
				m_rBMCSSYM.TrimRight();
			}
			else if(_T("MATL CODE") == itr->first)
			{
				m_rMatlCode = sString;
				m_rMatlCode.TrimLeft();
				m_rMatlCode.TrimRight();

				/// 중간에 공백이 나오기 전까지의 부분을 MAT'L CODE로 한다.
				const int at = m_rMatlCode.Find(' ' , 0);
				if(-1 != at) m_rMatlCode = m_rMatlCode.Left(at);
			}
			/// extract bolt length - 2011.09.23 added by humkyung
			else if(_T("BOLT LEN") == itr->first)
			{
				sString.TrimLeft();
				sString.TrimRight();
				m_sBoltLen = ParseSizeNum(sString);
			}
			/// up to here
			else if(_T("QTY") == itr->first)
			{
				m_rQTY = sString;
				m_rQTY.TrimLeft();
				m_rQTY.TrimRight();
			}
		}

		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
/**	
	@brief	BM Text Line을 파싱한다.
	
	@author	humkyung

	@date	????.??.??

	@return	bool
*/
bool CIsoBMData::Parse(map<STRING_T , pair<int,int> >* pBMAttrPosMap)
{
	assert((NULL != pBMAttrPosMap) && ("pBMAttrPosMap is NULL"));
	bool res = false;

	if(NULL != pBMAttrPosMap)
	{
		IsRegExp::RxNode *_rxnode = NULL;
		IsRegExp::RxMatchRes _rxmatch;

		if((NULL != m_pBMLine) && (m_pBMLine->GetTextCount() > 1))
		{
			res = ParseType2(pBMAttrPosMap);
		}
		else
		{
			res = ParseType1(pBMAttrPosMap);
		}
		if(false == res) return false;

		int nPos = 0 , nLastFindWhatLen = 0;
		/// find bolt length in matl desc if it is bolt material and bolt len position is not set
		CProjectData* pProjectData = CProjectData::GetInstance();
		if((pProjectData->m_pProjectSettingMap->end() != pProjectData->m_pProjectSettingMap->find(_T("C_BOLT_LENGTH_LOCATION"))))
		{
			if(_T("2") == (*pProjectData->m_pProjectSettingMap)[_T("C_BOLT_LENGTH_LOCATION")])
			{
				/// get bolt length keyword - 2012.08.27 added by humkyung
				CString sBoltLengthKeyword(_T("BOLT LENGTH"));
				if(pProjectData->m_pProjectSettingMap->end() != pProjectData->m_pProjectSettingMap->find(_T("C_BOLT_LENGTH_KEYWORD")))
				{
					sBoltLengthKeyword = (*pProjectData->m_pProjectSettingMap)[_T("C_BOLT_LENGTH_KEYWORD")];
				}
				/// up to here

				CString rSubString(m_rDataString);
				for(vector<CString>::iterator itr = m_subTextEntry.begin();itr != m_subTextEntry.end();++itr)
				{
					rSubString += (*itr);
				}
				rSubString.MakeUpper();
				const int at = rSubString.Find(sBoltLengthKeyword);
				if(at >= 0)
				{
					CString what(_T("[0-9]+(\\.[0-9]+)?(MM|M)"));	/// 숫자M 혹은 MM
					nPos = ::FindStringHelper(rSubString , what , 0 , nLastFindWhatLen , _rxnode, &_rxmatch);
					if( nPos >= 0 )
					{
						rSubString = rSubString.Mid(_rxmatch.Open[0] , _rxmatch.Close[0] - _rxmatch.Open[0]);
						rSubString = rSubString.TrimRight('M');
						m_sBoltLen = this->ParseSizeNum(rSubString);
						//m_rNPD[1] = m_sBoltLen;
					}
				}
			}
		}

		/// 분수 형식의 숫자를 사이언스 형식으로 바꾼다.
		if(!m_rNPD[0].IsEmpty())
		{
			int at = 0;
			if(-1 != (at = m_rNPD[0].Find(_T("/"))))
			{
				const double b = atof(m_rNPD[0].Right(m_rNPD[0].GetLength() - at - 1));

				double a = 0.f , c = 0.f;
				CString rQuotient = m_rNPD[0].Left(at);
				at = rQuotient.Find(_T("."));	/// 여기서 .은 소수점을 표시하는 것이 아니다.
				if(-1 != at)
				{
					a = atof(rQuotient.Left(at));
					c = atof(rQuotient.Right(rQuotient.GetLength() - at - 1));
				}
				else
				{
					c = atof(rQuotient);
				}

				m_rNPD[0].Format(_T("%lf") , a + (c / b));
			}
		}

		if(!m_rNPD[1].IsEmpty())
		{
			int at = 0;
			if(-1 != (at = m_rNPD[1].Find(_T("/"))))
			{
				const double b = atof(m_rNPD[1].Right(m_rNPD[1].GetLength() - at - 1));

				double a = 0.f , c = 0.f;
				CString rQuotient = m_rNPD[1].Left(at);
				at = rQuotient.Find(".");
				if(-1 != at)
				{
					a = atof(rQuotient.Left(at));
					c = atof(rQuotient.Right(rQuotient.GetLength() - at - 1));
				}
				else
				{
					c = atof(rQuotient);
				}
				m_rNPD[1].Format("%lf" , a + (c / b));
			}
		}

		if (_rxnode)
		{
			IsRxFree (_rxnode);
			_rxnode = NULL;
		}

		return true;
	}

	return res;
}

/**
	@brief	add sub text string

	@author	humkyung
**/
void CIsoBMData::AddSubText(const CString &rString)
{
	m_subTextEntry.push_back(rString);
}

/**
**/
const CIsoBMData* CIsoBMDataFactory::FindPipe(const CString &rSize) const
{
	for(vector<CIsoBMData>::const_iterator itr = m_pipeDatas.begin();itr != m_pipeDatas.end();++itr)
	{
		if(rSize == itr->GetMainSize())
		{
			return &(*itr);
		}
	}

	return NULL;
}

/**	
	@brief	칼럼의 index위치를 분류한다.
	
	@author	백흠경

	@param	rTextString	a parameter of type const CString &

	@return	void	
*/
void CIsoBMDataFactory::ParseColumnIndex(const CString &rTextString)
{
}

/**
	@brief	MainSize,SubSize and MatlCode가 같은지 비교한다.
	
	@author	백흠경
*/
struct MatchSizeAndMatlCode : public binary_function<CNtrData , CIsoBMData , bool>
{
	bool operator()(const CNtrData& lhs , const CIsoBMData& rhs) const
	{
		CString _ntrMain = lhs.GetMainSize();
		CString _ntrSub = lhs.GetSubSize();
		if(_ntrSub.IsEmpty()) _ntrSub = _ntrMain;

		CString _bmMain = rhs.GetMainSize();
		CString _bmSub = rhs.GetSubSize();
		if(_bmSub.IsEmpty()) _bmSub = _bmMain;

		CString _ntrMatlCode = lhs.GetMatlCode();
		CString _bmMatlCode = rhs.m_rMatlCode;

		return ((_ntrMain == _bmMain) && (_ntrSub == _bmSub) && (_ntrMatlCode == _bmMatlCode));
	}
};

/*
	@brief	

	@author	BHK
*/
CNtrData* CIsoBMDataFactory::FindNtrData(CIsoBMData& IsoBMData)
{
	vector<CNtrData>::iterator where = find_if(m_ntrTextEntry.begin(), m_ntrTextEntry.end(), bind2nd(MatchSizeAndMatlCode(), IsoBMData));
	return ((where != m_ntrTextEntry.end()) ? &(*where) : NULL);
}

/**	
	@brief	SUPPORT의 DETAIL NUMBER를 구한다.

	@param	graphicAreaTextEntry	a parameter of type vector<CIsoBMData*>&
	@param	rSupportNo	a parameter of type const string&

	@return	void	
*/
int CIsoBMDataFactory::FindSupportDetailNo(vector<CIsoBMData>& graphicAreaTextEntry , const string& rSupportNo )
{
	static const STRING_T SEE_DETAIL("SEE DETAIL");

	CIsPoint2d origin;
	vector<CIsoBMData>::iterator itr = graphicAreaTextEntry.begin();
	for(itr = graphicAreaTextEntry.begin() ; itr != graphicAreaTextEntry.end() ; ++itr)
	{
		STRING_T rText = (itr)->dataString();
		if( rText == rSupportNo )
		{
			origin = (itr)->origin();
			break;
		}
	}

	string rLastSeeDetailText;
	double nMinDist = -1;
	for( itr = graphicAreaTextEntry.begin() ; itr != graphicAreaTextEntry.end() ; ++itr )
	{
		string rText = (itr)->dataString() , rTemp;
		
		if( 0 == strncmp( rText.c_str() , SEE_DETAIL.c_str() , SEE_DETAIL.length()) )
		{
			CIsPoint2d dest = (itr)->origin();
			double dist = origin.DistanceTo( dest );

			if( (-1 == nMinDist) || (dist < nMinDist) )
			{
				nMinDist = dist;
				rLastSeeDetailText = rText;
			}
		}
	}

	if( !rLastSeeDetailText.empty() )
	{
		string rDetailNo = rLastSeeDetailText.substr( 11 ); 
		return atoi( rDetailNo.c_str() );
	}

	return 0;
}

/**
	@brief	constructor

	@author	humkyung
*/
CIsoBMData::CIsoBMData(const CIsPoint2d& origin , LPCTSTR p)
{
	m_origin = origin;
	m_pBMLine = NULL;
	m_rDataString = p;
}

/**
	@brief	constructor

	@author	humkyung

	@date	2013.02.26
*/
CIsoBMData::CIsoBMData(const CIsPoint2d& origin , CBMLine* p)
{
	m_origin = origin;
	m_pBMLine = p;
	m_rDataString = p->textString();
}

/**
**/
CString CIsoBMData::GetAllSubString() const
{
	CString res;
	for(vector<CString>::const_iterator itr = m_subTextEntry.begin();itr != m_subTextEntry.end();++itr)
	{
		if(!itr->IsEmpty()) res += (*itr);
	}

	return res;
}

/**
**/
CString CIsoBMData::GetSubStringAt(const size_t& at) const
{
	if(at < m_subTextEntry.size()) return m_subTextEntry[at];
	throw exception("range error in CIsoBMData::GetSubStringAt");
}

/**
**/
size_t CIsoBMData::GetSubStringCount() const
{
	return m_subTextEntry.size();
}

/**
**/
CString CCutPipeLengthData::dataString() const
{
	return m_rDataString;
}

/**
	@brief	LineDataRecord에서 필요한 DATA를 구한다.

	@author	BHK
**/
bool CIsoBMData::GetDataFromLineDataRecord(CLineDataRecord *pLineDataRecord)
{
	assert(pLineDataRecord && "pLineDataRecord is NULL");

	if(pLineDataRecord)
	{
		CDataField* pField = pLineDataRecord->FindWithFieldName( "C_INS_TYPE" );
		if( pField ) m_rInsType = pField->value();
		pField = pLineDataRecord->FindWithFieldName( "C_INS_THICKNESS" );
		if( pField ) m_rInsThickness = pField->value();
		pField = pLineDataRecord->FindWithFieldName( "C_PAINT_CODE" );
		if( pField ) m_rPaintCode = pField->value();

		pField = pLineDataRecord->FindWithFieldName( "C_PLANT_NO" );
		if( pField ) m_rPlantNo = pField->value();
		pField = pLineDataRecord->FindWithFieldName( "C_AREA_NO" );
		if( pField ) m_rAreaNo = pField->value();
		pField = pLineDataRecord->FindWithFieldName( "C_UNIT_NO" );
		if( pField ) m_rUnitNo = pField->value();
		pField = pLineDataRecord->FindWithFieldName( "C_SECTION_NO" );
		if( pField ) m_rSectionNo = pField->value();
		
		return true;
	}

	return false;
}

/**
	@brief	return no

	@author	humkyung

	@date	2012.04.25

	@return	int
*/
int CCutPipeLengthData::no() const
{
	return m_iNo;
}

/**
	@brief	get no

	@author	humkyung

	@date	2012.04.25

	@return	int
*/
int CCutPipeLengthData::ParseNo()
{
	int at = m_rDataString.Find("<");
	if(-1 != at)
	{
		m_iNo = atoi(m_rDataString.Mid(at + 1));
		return ERROR_SUCCESS;
	}

	return ERROR_BAD_ENVIRONMENT;
}

/**
	@brief	Main or Sub Size를 파싱한다. 분수의 경우에 소수점이 있는 것은 대분수이다.
	분수 형식을 소수점 형식으로 변환하여 리턴한다.

	@author	BHK
*/
CString CCutPipeLengthData::ParseSizeNum( const CString& rString )
{
	CString res(rString);

	int at = rString.Find("/");
	if(-1 != at)
	{
		CString numerator[2];
		numerator[0]        = rString.Left(at);
		CString denominator = rString.Right(rString.GetLength() - at - 1);
		
		at = numerator[0].Find(".");
		if( -1 != at )
		{
			numerator[1] = numerator[0].Right(numerator[0].GetLength() - at - 1);
			numerator[0] = numerator[0].Left(at);
			///numerator[1].TrimRight("0");
		}
		
		at = denominator.Find(".");
		if( -1 != at ) denominator.TrimRight("0");
		
		double value = 0.f;
		if(!denominator.IsEmpty() && !numerator[1].IsEmpty())
		{
			value = atof(numerator[0]) + (atof(numerator[1]) / atof(denominator));
		}
		else if(!denominator.IsEmpty() && numerator[1].IsEmpty())
		{
			value = atof(numerator[0]) / atof(denominator);
		}
		else
		{
			value = atof(numerator[0]) + atof(numerator[1]);
		}
		
		stringstream oss;
		oss << value;
		
		return oss.str().c_str();
	}
	else
	{
		at = res.Find(".");
		if( -1 != at ) res.TrimRight("0");

		return res;
	}

	return CString("");
}