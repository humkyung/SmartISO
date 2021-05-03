#include "StdAfx.h"
#include <assert.h>
#include <DgnEnum.h>
#include "IsoModelDocData.h"
#include "IsoModel.h"
#include "IsoBMArea.h"
#include "ConvexHull.h"
#include "IsoElement.h"

#include <algorithm>
using namespace std;

CIsoBMArea::CIsoBMArea(const CString& sName , const double& minx , const double& miny , const double& maxx , const double& maxy , CIsoModel* pIsoModel) :
CIsoArea(sName , minx , miny , maxx , maxy , pIsoModel)
{
}

CIsoBMArea::~CIsoBMArea(void)
{
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-16
    @class      CIsoBMArea
    @function   Add
    @return     void
    @param      CDgnElement*    pEle
    @brief
******************************************************************************/
void CIsoBMArea::Add(CDgnElement* pEle)
{
	assert(pEle && "pEle is NULL");

	if(pEle)
	{
		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();

		CString sType = _T("general");
		/// element를 분류하여 저장한다.(분류 예 : pipe , dimension , general)
		const int iEntType = pEle->elementType();
		UInt32 uiLevel = pEle->level();
		CString sColor;
		sColor.Format(_T("%d") , pEle->color());
		const UInt32 uiWeight = pEle->weight();
		switch(iEntType)
		{	
		case LINE_ELM:
		case LINE_STRING_ELM:
			if(0 != pEle->style()) return;
			if (docData.IsPipeLineLevel(uiLevel) && docData.IsPipeLineColor(sColor) && docData.IsPipeLineWeight(uiWeight))
			{
				sType = _T("pipe");
			}
			break;
		case TEXT_ELM:
			{
			}
			break;
		}

		map<CString , vector<CDgnElement*>*>::iterator where = m_oDgnElementMap.find(sType);
		if(where == m_oDgnElementMap.end())
		{
			m_oDgnElementMap.insert(make_pair(sType , new vector<CDgnElement*>));
		}

		m_oDgnElementMap[sType]->push_back(pEle);
	}
}

/******************************************************************************
	@author     humkyung
	@date       2011-09-20
	@class      CIsoBMArea
	@function   SortByElemPosY
	@return     bool
	@param      const           CIsoElement*
	@param      lhs             const
	@param      CIsoElement*    rhs
	@brief
******************************************************************************/
bool CIsoBMArea::SortByElemPosY(const CDgnElement* lhs , const CDgnElement* rhs)
{
	return (lhs->origin().y > rhs->origin().y);
}
bool CIsoBMArea::SortByElemPosX(const CDgnElement* lhs , const CDgnElement* rhs)
{
	return (lhs->origin().x > rhs->origin().x);
}

struct IsMatchString : public binary_function<CDgnElement*, CString, bool>
{
	//          typedef T first_argument_type;
	//          typedef T second_argument_type;
	//          typedef T result_type;

	bool operator ()( CDgnElement* a, const CString& b ) const
	{
		if(a->IsKindOf(CDgnText::TypeString()))
		{
			CDgnText* pTextElm = static_cast<CDgnText*>(a);
			CString str = pTextElm->textString();
			str.TrimLeft();
			str.TrimRight();
			return (str == b);
		}

		return 0;
	}
};

/******************************************************************************
    @author     humkyung
    @date       2011-12-19
    @class      CIsoBMArea
    @function   IsPartOfMatlDesc
    @return     bool
    @param      CDgnText*    pTextElm
    @brief
******************************************************************************/
bool CIsoBMArea::IsPartOfMatlDesc(const CString& sTextString)
{
	static const CString sHeaderWords[] = {_T("PT ") , _T("NO ") , _T("ERECTION MATERIAL") , _T("SUPPORTS") , _T("INSTRUMENTS")};
	static const CString sSupportWords = _T("PIPE SUPPORTS");
	
	for(int i = 0;i < SIZE_OF_ARRAY(sHeaderWords);++i)
	{
		if(0 == sTextString.Find(sHeaderWords[i])) return false;
	}

	CString _sTextString = sTextString;
	_sTextString.Trim();
	
	int nLastFindWhatLen = 0;
	int nPos = ::FindStringHelper(_sTextString , _T("[0-9]+ ") , 0, nLastFindWhatLen, m_rxnode, &m_rxmatch);
	if(0 == nPos)
	{
		return false;
	}

	nPos = ::FindStringHelper(_sTextString , _T("MISCELLANEOUS COMPONENTS") ,  0, nLastFindWhatLen, m_rxnode, &m_rxmatch);
	if(0 == nPos)
	{
		return false;
	}

	_sTextString = sTextString;
	nPos = ::FindStringHelper(_sTextString , sSupportWords , 0 , nLastFindWhatLen , m_rxnode , &m_rxmatch); 
	if(-1 != nPos)
	{
		return false;
	}

	_sTextString = sTextString;
	nPos = ::FindStringHelper(_sTextString , _T("CUT PIPE LENGTH") , 0 , nLastFindWhatLen , m_rxnode , &m_rxmatch); 
	if(-1 != nPos)
	{
		return false;
	}

	nPos = ::FindStringHelper(_sTextString , _T("\\<[0-9]+\\>") , 0 , nLastFindWhatLen , m_rxnode , &m_rxmatch); 
	if(-1 != nPos)
	{
		return false;
	}
	///!

	return true;
}

/******************************************************************************
	@author     humkyung
	@date       2011-09-20
	@class      CIsoBMArea
	@function   ParseBMElements
	@return     void
	@brief
******************************************************************************/
void CIsoBMArea::ParseBMElements()
{
	if(m_oDgnElementMap.end() == m_oDgnElementMap.find(_T("general"))) return;

	stable_sort(m_oDgnElementMap[_T("general")]->begin() , m_oDgnElementMap[_T("general")]->end() , CIsoBMArea::SortByElemPosY);
	vector<CDgnElement*>::iterator wherePipeSupport = find_if(m_oDgnElementMap[_T("general")]->begin() , m_oDgnElementMap[_T("general")]->end() , bind2nd(IsMatchString() , _T("PIPE SUPPORTS")));
	vector<CDgnElement*>::iterator whereMISC = find_if(m_oDgnElementMap[_T("general")]->begin(), m_oDgnElementMap[_T("general")]->end() , bind2nd(IsMatchString(), _T("MISCELLANEOUS COMPONENTS"))); ///< MISCELLANEOUS COMPONENTS의 where를 찾는다.

	for(vector<CDgnElement*>::iterator itr = m_oDgnElementMap[_T("general")]->begin();itr != m_oDgnElementMap[_T("general")]->end();++itr)
	{
		if(!(*itr)->IsKindOf(CDgnText::TypeString())) continue;

		CString rTextString = static_cast<CDgnText*>((*itr))->textString();
		if(rTextString.IsEmpty()) continue;

		/// NO          MATERIALS DESCRIPTION            (IN)         SPEC    SYM       CODE              QTY 
		/// find field position
		int at = rTextString.Find( _T("MATERIALS DESCRIPTION") );

		CString rTextString_ = rTextString.Left(5);
		rTextString_.TrimLeft();
		//////////////////////////////////////////////////////////////////////////
		/// regular expression
		try
		{
			int nLastFindWhatLen = 0;
			int nPos = ::FindStringHelper(rTextString_ , _T("[0-9]+") , 0 , nLastFindWhatLen, m_rxnode, &m_rxmatch);
			if(0 == nPos)
			{
				const CString sPartNo = rTextString_.Left(nLastFindWhatLen);

				bool bInstOrSpecialItem = false;
				/// PIPE 부분 - PIPE SUPPORTS 부분이 둘다 있을 경우
				if(m_oDgnElementMap[_T("general")]->end() != wherePipeSupport)
				{
					if(itr < wherePipeSupport)
					{
						//! 인스트루먼트 파이핑 스페셜리을 체크한다.
						rTextString_ = rTextString;
						rTextString_.TrimRight();
						rTextString_.TrimLeft();
						///if(IsInstOrSpecialItem(rTextString_)) bInstOrSpecialItem = true;

						CIsoBMData bmData((*itr)->origin() , sPartNo , rTextString);
						bmData.m_oDgnElementList.push_back(*itr);
						for(++itr;itr != m_oDgnElementMap[_T("general")]->end();++itr)
						{
							if(!(*itr)->IsKindOf(CDgnText::TypeString())) continue;

							/// 어디까지가 하나의 ITEM DATA되는지 확인
							CDgnText* pTextElm = static_cast<CDgnText*>((*itr));
							rTextString = pTextElm->textString();
							if((0 == rTextString.Find(_T("PT "))) || (0 == rTextString.Find(_T("NO ")))) break;

							if(rTextString.IsEmpty()) continue;
							if(this->IsPartOfMatlDesc(rTextString))
							{
								/*rTextString_ = rTextString;
								rTextString_.Trim();*/
								bmData.AddSubTextElm( pTextElm );
								bmData.m_oDgnElementList.push_back(*itr);	
							}
							else
							{
								--itr;
								break;
							}
						}

						bmData.Parse();
						{
							const int nFindPos = bmData.m_sMatlDesc.Find( _T("PIPE") );
							m_oIsoBMDataList.push_back(bmData);
						}
					}
					else if( (wherePipeSupport < itr) && (itr < whereMISC) ) ///< 서포트 부분
					{
						rTextString_ = rTextString;
						rTextString_.Trim();
						/*int at = rTextString.Find( _T("SUPPORT DATA:") );
						if(at == -1) continue;*/
						/// 
						CIsoBMData bmData((*itr)->origin() , sPartNo , rTextString);
						bmData.m_oDgnElementList.push_back(*itr);
						for(++itr;itr != m_oDgnElementMap[_T("general")]->end();++itr)
						{
							if(!(*itr)->IsKindOf(CDgnText::TypeString())) continue;

							CDgnText* pTextElm = static_cast<CDgnText*>((*itr));
							rTextString = pTextElm->textString();
							if((0 == rTextString.Find(_T("PT "))) || (0 == rTextString.Find(_T("NO ")))) break;

							if(rTextString.IsEmpty()) continue;
							if(this->IsPartOfMatlDesc(rTextString))
							{
								/*rTextString_ = rTextString;
								rTextString_.Trim();*/
								bmData.AddSubTextElm( pTextElm );
								bmData.m_oDgnElementList.push_back(*itr);
							}
							else
							{
								--itr;
								break;
							}
						}
						bmData.Parse();
						///m_supportDatas.push_back(bmData);
						m_oIsoBMDataList.push_back(bmData);
					}
					else	///< MISCELLANEOUS 부분
					{
						rTextString_ = rTextString;
						rTextString_.Trim();
						/// 
						CIsoBMData bmData((*itr)->origin() , sPartNo , rTextString);
						bmData.m_oDgnElementList.push_back(*itr);
						for(++itr;itr != m_oDgnElementMap[_T("general")]->end();++itr)
						{
							if(!(*itr)->IsKindOf(CDgnText::TypeString())) continue;

							CDgnText* pTextElm = static_cast<CDgnText*>((*itr));
							rTextString = pTextElm->textString();
							if((0 == rTextString.Find(_T("PT "))) || (0 == rTextString.Find(_T("NO ")))) break;
							if(rTextString.IsEmpty()) continue;

							if(this->IsPartOfMatlDesc(rTextString))
							{
								/*rTextString_ = rTextString;
								rTextString_.Trim();*/
								bmData.AddSubTextElm( pTextElm );
								bmData.m_oDgnElementList.push_back(*itr);	
							}
							else
							{
								--itr;
								break;
							}
						}
						bmData.Parse();
						//if(IsInstOrSpecialItem(bmData))	//! 2011.06.22 added by humkyung
						//	m_InstOrSpecialDatas.push_back(bmData);
						//else	m_miscellDatas.push_back(bmData);
						m_oIsoBMDataList.push_back(bmData);
					}
				}
				else
				{
					/// 인스트루먼트 파이핑 스페셜리을 체크한다.
					rTextString_ = rTextString;
					rTextString_.Trim();
					///if(IsInstOrSpecialItem(rTextString)) bInstOrSpecialItem = true;
					/// PIPE SUPPORTS 부분이 없을 경우.

					CIsoBMData bmData((*itr)->origin() , sPartNo , rTextString);
					bmData.m_oDgnElementList.push_back(*itr);
					for(++itr;itr != m_oDgnElementMap[_T("general")]->end();++itr)
					{
						if(!(*itr)->IsKindOf(CDgnText::TypeString())) continue;

						CDgnText* pTextElm = static_cast<CDgnText*>((*itr));
						rTextString = pTextElm->textString();
						if((0 == rTextString.Find(_T("PT "))) || (0 == rTextString.Find(_T("NO ")))) break;
						if(rTextString.IsEmpty()) continue;

						if(this->IsPartOfMatlDesc(rTextString))
						{
							/*rTextString_ = rTextString;
							rTextString_.Trim();*/
							bmData.AddSubTextElm( pTextElm );
							bmData.m_oDgnElementList.push_back(*itr);
						}
						else
						{
							--itr;
							break;
						}
					}
					bmData.Parse();
					{
						const int nFindPos = bmData.m_sMatlDesc.Find( _T("PIPE") );
						m_oIsoBMDataList.push_back(bmData);
					}
				}
			}
			else
			{
				/// PIPE CUT LENGTH 부분
				CString line(rTextString);
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

				line = rTextString;
			}
		}
		catch(...)
		{

		}
		if(m_oDgnElementMap[_T("general")]->end() == itr) break;	///< ENTRY의 맨 마지막이면 종료한다.
	}
}

/******************************************************************************
	@author     humkyung
	@date       2011-09-27
	@class      CIsoBMArea
	@function   PrepareToCompare
	@return     void
	@brief		parse bm data
******************************************************************************/
void CIsoBMArea::PrepareToCompare()
{
	ParseBMElements();
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-13
    @class      CIsoBMArea
    @function   ExpandEnclosePolygon
    @return     int
    @param      vector<DPoint3d>&   oEnclosePolygon
    @brief
******************************************************************************/
int CIsoBMArea::ExpandEnclosePolygon( vector<DPoint3d>& oEnclosePolygon , const double dWidthOffset , const double dHeightOffset)
{
	DPoint3d pts[4] = {0,0,0,0};
	double dx[2] = {0,0} , dy[2] = {0,0} , dz[2] = {0,0};
	dx[0] = oEnclosePolygon[1].x - oEnclosePolygon[0].x;
	dy[0] = oEnclosePolygon[1].y - oEnclosePolygon[0].y;
	dz[0] = oEnclosePolygon[1].z - oEnclosePolygon[0].z;
	double length = sqrt(dx[0]*dx[0] + dy[0]*dy[0] + dz[0]*dz[0]);
	dx[0] /= length; dy[0] /= length; dz[0] /= length;

	dx[1] = oEnclosePolygon[3].x - oEnclosePolygon[0].x;
	dy[1] = oEnclosePolygon[3].y - oEnclosePolygon[0].y;
	dz[1] = oEnclosePolygon[3].z - oEnclosePolygon[0].z;
	length = sqrt(dx[1]*dx[1] + dy[1]*dy[1] + dz[1]*dz[1]);
	dx[1] /= length; dy[1] /= length; dz[1] /= length;

	pts[0] = oEnclosePolygon[0];
	pts[0].x -= dx[0]*dWidthOffset; pts[0].y -= dy[0]*dWidthOffset; pts[0].z -= dz[0]*dWidthOffset;
	pts[0].x -= dx[1]*dHeightOffset; pts[0].y -= dy[1]*dHeightOffset; pts[0].z -= dz[1]*dHeightOffset;
	pts[1] = oEnclosePolygon[1];
	pts[1].x += dx[0]*dWidthOffset; pts[1].y += dy[0]*dWidthOffset; pts[1].z += dz[0]*dWidthOffset;
	pts[1].x -= dx[1]*dHeightOffset; pts[1].y -= dy[1]*dHeightOffset; pts[1].z -= dz[1]*dHeightOffset;
	pts[2] = oEnclosePolygon[2];
	pts[2].x += dx[0]*dWidthOffset; pts[2].y += dy[0]*dWidthOffset; pts[2].z += dz[0]*dWidthOffset;
	pts[2].x += dx[1]*dHeightOffset; pts[2].y += dy[1]*dHeightOffset; pts[2].z += dz[1]*dHeightOffset;
	pts[3] = oEnclosePolygon[3];
	pts[3].x -= dx[0]*dWidthOffset; pts[3].y -= dy[0]*dWidthOffset; pts[3].z -= dz[0]*dWidthOffset;
	pts[3].x += dx[1]*dHeightOffset; pts[3].y += dy[1]*dHeightOffset; pts[3].z += dz[1]*dHeightOffset;
	oEnclosePolygon.clear();
	oEnclosePolygon.push_back( pts[0] );
	oEnclosePolygon.push_back( pts[1] );
	oEnclosePolygon.push_back( pts[2] );
	oEnclosePolygon.push_back( pts[3] );

	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     yjk
    @date       2011-??-??
    @class      CIsoBMArea
    @function   GetSubTextShapePoint
    @return     vector<DPoint3d>
    @param      CString         sElemdntID
    @param      DgnModelRefP    modelRef
    @brief		get a shape enclosing text element
******************************************************************************/
vector<DPoint3d> CIsoBMArea::GetSubTextShapePoint(CDgnText* pDgnText , CDgnDocument* pDgnDoc)
{
	assert(pDgnText && pDgnDoc && "pDgnText or pDgnDoc is NULL");

	vector<DPoint3d> SubTextShapePoint;
	if(pDgnText && pDgnDoc)
	{
		DPoint3d orign = pDgnText->origin();
		const double dLen = pDgnText->GetTextLength();
		const double dTextHeight = pDgnText->GetTextHeight();
		
		CString sText = pDgnText->textString();
		double eachTextWidth = dLen / sText.GetLength();
		int iStartPos = (sText.GetLength() - sText.TrimLeft().GetLength());
		int iWidth = sText.Trim().GetLength();

		DPoint3d LeftBottom;
		LeftBottom.x = orign.x + (iStartPos * eachTextWidth) ;
		LeftBottom.y = orign.y ;
		LeftBottom.z = 0;
		SubTextShapePoint.push_back (LeftBottom);

		DPoint3d RightBottom;
		RightBottom.x = orign.x + (iStartPos * eachTextWidth) + (iWidth * eachTextWidth);
		RightBottom.y = orign.y ;
		RightBottom.z = 0;
		RightBottom.x -= LeftBottom.x; RightBottom.y -= LeftBottom.y;
		RightBottom.x += LeftBottom.x; RightBottom.y += LeftBottom.y;
		SubTextShapePoint.push_back( RightBottom );

		DPoint3d RightTop;
		RightTop.x = orign.x + (iStartPos * eachTextWidth) + (iWidth * eachTextWidth);  
		RightTop.y = orign.y + dTextHeight;
		RightTop.z = 0;
		RightTop.x -= LeftBottom.x; RightTop.y -= LeftBottom.y;
		RightTop.x += LeftBottom.x; RightTop.y += LeftBottom.y;
		SubTextShapePoint.push_back( RightTop );

		DPoint3d LeftTop;
		LeftTop.x = orign.x + (iStartPos * eachTextWidth);
		LeftTop.y = orign.y + dTextHeight;
		LeftTop.z = 0;
		LeftTop.x -= LeftBottom.x; LeftTop.y -= LeftBottom.y;
		LeftTop.x += LeftBottom.x; LeftTop.y += LeftBottom.y;
		SubTextShapePoint.push_back( LeftTop );

		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
		const double dOffset = docData.GetOffset();
		const double scale = docData.m_dScale;
		ExpandEnclosePolygon( SubTextShapePoint , dOffset / scale , dOffset*0.5 / scale);
		SubTextShapePoint.push_back( SubTextShapePoint.front() );
	}

	return SubTextShapePoint;
}

/******************************************************************************
    @author     humkyung
    @date       2011-??-??
    @class      CIsoBMArea
    @function   GetIsoRevisedBMDataShapePoint
    @return     vector<DPoint3d>
    @param      const           int
    @param      iStartPos       const
    @param      int             iWidth
    @param      CString         sElemdntID
    @param      DgnModelRefP    modelRef
    @brief
******************************************************************************/
vector<DPoint3d> CIsoBMArea::GetIsoRevisedBMDataShapePoint(const int iStartPos , const int iWidth , CDgnText* pDgnText , CDgnDocument* pDgnDoc)
{	
	assert(pDgnText && pDgnDoc && "pDgnText or pDgnDoc is NULL");

	vector<DPoint3d> IsoRevisedBMDataShapePoint;
	if(pDgnText && pDgnDoc)
	{
		CString sText = pDgnText->textString();
		DPoint3d origin = pDgnText->origin();
		const double dTextHeight = pDgnText->GetTextHeight();
		const double dTextLength = pDgnText->GetTextLength();

		const int Length = sText.GetLength();			// 텍스트 갯수
		double eachTextWidth = dTextLength / Length;	// 텍스트 한글자의 Width 

		DPoint3d LeftBottom;
		LeftBottom.x = origin.x + (iStartPos * eachTextWidth) ;
		LeftBottom.y = origin.y ;
		LeftBottom.z = 0;
		IsoRevisedBMDataShapePoint.push_back (LeftBottom);

		DPoint3d RightBottom;
		RightBottom.x = origin.x + (iStartPos * eachTextWidth) + (iWidth * eachTextWidth);
		RightBottom.y = origin.y ;
		RightBottom.z = 0;
		RightBottom.x -= LeftBottom.x; RightBottom.y -= LeftBottom.y;
		RightBottom.x += LeftBottom.x; RightBottom.y += LeftBottom.y;
		IsoRevisedBMDataShapePoint.push_back( RightBottom );

		DPoint3d RightTop;
		RightTop.x = origin.x + (iStartPos * eachTextWidth) + (iWidth * eachTextWidth);  
		RightTop.y = origin.y + dTextHeight;
		RightTop.z = 0;
		RightTop.x -= LeftBottom.x; RightTop.y -= LeftBottom.y;
		RightTop.x += LeftBottom.x; RightTop.y += LeftBottom.y;
		IsoRevisedBMDataShapePoint.push_back( RightTop );

		DPoint3d LeftTop;
		LeftTop.x = origin.x + (iStartPos * eachTextWidth);
		LeftTop.y = origin.y + dTextHeight;
		LeftTop.z = 0;
		LeftTop.x -= LeftBottom.x; LeftTop.y -= LeftBottom.y;
		LeftTop.x += LeftBottom.x; LeftTop.y += LeftBottom.y;
		IsoRevisedBMDataShapePoint.push_back( LeftTop );

		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
		const double dOffset = docData.GetOffset();
		const double scale = docData.m_dScale;
		ExpandEnclosePolygon( IsoRevisedBMDataShapePoint , dOffset / scale , dOffset*0.5 / scale);
		IsoRevisedBMDataShapePoint.push_back( IsoRevisedBMDataShapePoint.front() );
	}

	return IsoRevisedBMDataShapePoint;
}

/******************************************************************************
	@author     kyj
	@date       2011-10-24
	@class      CIsoBMArea
	@function   GetBMAttrTextHeaderWidth
	@return     해당 Text 의 Width
	@brief		
******************************************************************************/
int CIsoBMArea::GetBMAttrTextHeaderWidth(CString TextHeader)
{
	int res= 0;
	
	for(map<STRING_T , pair<int,int>>::iterator itr= CIsoBMData::m_oBMAttrPosMap.begin();itr != CIsoBMData::m_oBMAttrPosMap.end();++itr)
	{
		if(TextHeader == CString(itr->first.c_str()))
		{
			res = itr->second.second ;
			
			break;
		}
	}
	return res;
}
/******************************************************************************
@author     kyj
@date       2011-10-24
@class      CIsoBMArea
@function   GetBMAttrTextHeaderPos 
@return     해당 Text 의 Start Pos
@brief		
******************************************************************************/
int CIsoBMArea::GetBMAttrTextHeaderStartPos(CString TextHeader)
{
	int res= 0;
	
	for(map<STRING_T , pair<int,int>>::iterator itr= CIsoBMData::m_oBMAttrPosMap.begin();itr != CIsoBMData::m_oBMAttrPosMap.end();++itr)
	{
		if(TextHeader == CString(itr->first.c_str()))
		{
			res = itr->second.first;
			
			break;
		}
	}
	return res;
}

/******************************************************************************
    @author     yjk
    @date       2011-??-??
    @class      CIsoBMArea
    @function   GetIsoRevisedBMDataShapePointMap
    @return     vector<DPoint3d>>
    @param      list<CIsoBMData>    oIsoRevisionBMDataList
    @brief
******************************************************************************/
map<pair<int , CString>, vector<DPoint3d>> CIsoBMArea::GetIsoRevisedBMDataShapePointMap(list<CIsoBMData> oIsoRevisionBMDataList)
{
	map<pair<int , CString>, vector<DPoint3d>> IsoRevisedBMDataShapePointMap;
	
	CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
	const double scale = docData.m_dScale;

	CString sRevisedColumn = _T("") ;
	CDgnDocument* pDgnDoc  = m_pIsoModel->GetDgnDocument();
	
	vector<DPoint3d> IsoRevisedBMDataShapePoint;
	for(list<CIsoBMData>::iterator itr = oIsoRevisionBMDataList.begin();itr != oIsoRevisionBMDataList.end();++itr)
	{
		int iPartNo = ATOI_T(itr->PartNo());
		int iStartPos = 0;
		int iWidth = 0;
		IsoRevisedBMDataShapePoint.clear ();

		if (CIsoBMData::MODIFIED == itr->GetCompareResultOf(_T("MATL DESC")))
		{
			sRevisedColumn = _T("MATL DESC");
			iStartPos = GetBMAttrTextHeaderStartPos(sRevisedColumn);
			iWidth = itr->m_sMatlDesc.Trim().GetLength();
			IsoRevisedBMDataShapePoint = GetIsoRevisedBMDataShapePoint(iStartPos , iWidth , (CDgnText*)(itr->m_oDgnElementList[0]) , pDgnDoc);
			
			if(itr->m_oSubTextList.size() > 0)
			{
				int index = 1;
				vector<DPoint3d> SubTextListShapePoint;
				for(vector<CString>::iterator jtr = itr->m_oSubTextList.begin(); jtr != itr->m_oSubTextList.end();++jtr)
				{
					///CString sElemdntID = itr->m_oDgnElementList[index]->elementID(); 
					SubTextListShapePoint = GetSubTextShapePoint((CDgnText*)(itr->m_oDgnElementList[index]) , pDgnDoc);
					
					IsoRevisedBMDataShapePoint.insert(IsoRevisedBMDataShapePoint.end() , SubTextListShapePoint.begin() , SubTextListShapePoint.end());

					index++;	
				}
			}

			CConvexHull oConvexHull;
			vector<DPoint3d> oEnclosePolygon = oConvexHull.Create (IsoRevisedBMDataShapePoint);
			IsoRevisedBMDataShapePoint = oEnclosePolygon ;

			IsoRevisedBMDataShapePointMap.insert(make_pair(make_pair(iPartNo , sRevisedColumn) , IsoRevisedBMDataShapePoint ));
		}
		if (CIsoBMData::MODIFIED == itr->GetCompareResultOf(_T("SIZE")))///_T("Revision") == itr->m_sNPD[2])
		{
			sRevisedColumn = _T("SIZE");
			iStartPos = GetBMAttrTextHeaderStartPos(sRevisedColumn);
			int iMainLen = itr->m_sNPD[0].Trim().GetLength(); // Main
			int iSubLen = itr->m_sNPD[1].Trim().GetLength(); // Sub
			iWidth = iMainLen + iSubLen;
			IsoRevisedBMDataShapePoint = GetIsoRevisedBMDataShapePoint(iStartPos , iWidth , (CDgnText*)(itr->m_oDgnElementList[0]) , pDgnDoc);
			IsoRevisedBMDataShapePointMap.insert(make_pair(make_pair(iPartNo , sRevisedColumn) , IsoRevisedBMDataShapePoint ));

		}
		if (CIsoBMData::MODIFIED == itr->GetCompareResultOf(_T("MATL SPEC")))///_T("Revision") == itr->m_sMatlSpec[1])
		{
			sRevisedColumn = _T("MATL SPEC");
			iStartPos = GetBMAttrTextHeaderStartPos(sRevisedColumn);
			iWidth = itr->m_sMatlSpec.Trim().GetLength ();
			IsoRevisedBMDataShapePoint = GetIsoRevisedBMDataShapePoint(iStartPos , iWidth , (CDgnText*)(itr->m_oDgnElementList[0]) , pDgnDoc);
			IsoRevisedBMDataShapePointMap.insert(make_pair(make_pair(iPartNo , sRevisedColumn) , IsoRevisedBMDataShapePoint ));

		}
		if (CIsoBMData::MODIFIED == itr->GetCompareResultOf(_T("SYM")))///_T("Revision") == itr->m_sBMCSSYM[1])
		{
			sRevisedColumn = _T("SYM");
			iStartPos = GetBMAttrTextHeaderStartPos(sRevisedColumn);
			iWidth = itr->m_sBMCSSYM.Trim().GetLength (); 
			IsoRevisedBMDataShapePoint = GetIsoRevisedBMDataShapePoint(iStartPos , iWidth , (CDgnText*)(itr->m_oDgnElementList[0]) , pDgnDoc);
			IsoRevisedBMDataShapePointMap.insert(make_pair(make_pair(iPartNo , sRevisedColumn) , IsoRevisedBMDataShapePoint ));

		}
		if (CIsoBMData::MODIFIED == itr->GetCompareResultOf(_T("QTY")))///_T("Revision") == itr->m_sQTY[1])
		{
			sRevisedColumn = _T("QTY");
			iStartPos = GetBMAttrTextHeaderStartPos(sRevisedColumn);
			iWidth = itr->m_sQTY.Trim().GetLength();
			IsoRevisedBMDataShapePoint = GetIsoRevisedBMDataShapePoint(iStartPos , iWidth , (CDgnText*)(itr->m_oDgnElementList[0]) , pDgnDoc);
			IsoRevisedBMDataShapePointMap.insert(make_pair(make_pair(iPartNo , sRevisedColumn) , IsoRevisedBMDataShapePoint ));
			
		}
		if ((CIsoBMData::NONE == itr->GetCompareResultOf(_T("MATL DESC"))) && (CIsoBMData::NONE == itr->GetCompareResultOf(_T("SIZE"))) && 
			(CIsoBMData::NONE == itr->GetCompareResultOf(_T("MATL SPEC"))) && (CIsoBMData::NONE == itr->GetCompareResultOf(_T("SYM"))) && 
			(CIsoBMData::NONE == itr->GetCompareResultOf(_T("QTY"))))
		{
			sRevisedColumn = _T("NEW BM Data");
			iStartPos = 0;
			iWidth = itr->DataString().GetLength ();
			IsoRevisedBMDataShapePoint = GetIsoRevisedBMDataShapePoint(iStartPos , iWidth , (CDgnText*)(itr->m_oDgnElementList[0]) , pDgnDoc);
			if(itr->m_oSubTextList.size() > 0)
			{
				int index = 1;
				vector<DPoint3d> SubTextListShapePoint;
				for(vector<CString>::iterator jtr = itr->m_oSubTextList.begin(); jtr != itr->m_oSubTextList.end();++jtr)
				{
					///CString sElemdntID = itr->m_oDgnElementList[index]->elementID(); 
					SubTextListShapePoint = GetSubTextShapePoint((CDgnText*)(itr->m_oDgnElementList[index]) , pDgnDoc);
					IsoRevisedBMDataShapePoint.insert(IsoRevisedBMDataShapePoint.end() , SubTextListShapePoint.begin() , SubTextListShapePoint.end());

					CConvexHull oConvexHull;
		
					vector<DPoint3d> oEnclosePolygon = oConvexHull.Create (IsoRevisedBMDataShapePoint);

					oEnclosePolygon.push_back( oEnclosePolygon.front() );
					IsoRevisedBMDataShapePoint = oEnclosePolygon ;

					index++;	
				}
			}
			IsoRevisedBMDataShapePointMap.insert(make_pair(make_pair(iPartNo , sRevisedColumn) , IsoRevisedBMDataShapePoint ));
		}
	}

	return IsoRevisedBMDataShapePointMap;
}
/******************************************************************************
	@author     
	@date       2011-11-09
	@class      CIsoBMArea
	@function   GetIsoRevisedBMDataListCompareWith
	@return     list<CIsoBMData> 
	@brief	    현재 도면의 BM Data 과 이전 도면의 BM Data 비교 하여 어느 컬럼이 바뀌었는지 확인 IsoRevisedBMDataList를 만들어서 리턴 함
******************************************************************************/
list<CIsoBMData> CIsoBMArea::GetIsoRevisedBMDataListCompareWith(list<CIsoBMData> oCurrentBMDataList , list<CIsoBMData> oReferenceBMDataList )
{
	list<CIsoBMData> IsoRevisedBMDataList;
	
	for(list<CIsoBMData>::iterator itr = oCurrentBMDataList.begin();itr != oCurrentBMDataList.end();++itr) // 현재 도면의 BM Data
	{
		bool bNewBMData = true;
		bool bFound = false;

		for(list<CIsoBMData>::iterator jtr = oReferenceBMDataList.begin();jtr != oReferenceBMDataList.end();++jtr) // 이전 도면의 BM Data
		{
			bFound = false;
			if (itr->m_sMatlCode.Trim() == jtr->m_sMatlCode.Trim() ) // MATL CODE 가 같은 Text 를 비교 함
			{ 
				bNewBMData = false; // 새로운 BM Data가 아니다

				if(itr->m_sMatlDesc.Trim() != jtr->m_sMatlDesc.Trim() ) 
				{
					itr->SetCompareResult(_T("MATL DESC") , CIsoBMData::MODIFIED);
					bFound = true;
				}
				if(itr->GetMainSize().Trim()  != jtr->GetMainSize().Trim() )
				{
					itr->SetCompareResult(_T("SIZE") , CIsoBMData::MODIFIED);
					bFound = true;
				}
				if(itr->GetSubSize().Trim() != jtr->GetSubSize().Trim() )
				{
					itr->SetCompareResult(_T("SIZE") , CIsoBMData::MODIFIED);
					bFound = true;
				}
				if(itr->m_sMatlSpec.Trim() != jtr->m_sMatlSpec.Trim() )
				{
					itr->SetCompareResult(_T("MATL SPEC") , CIsoBMData::MODIFIED);
					bFound = true;
				}
				if(itr->m_sBMCSSYM.Trim() != jtr->m_sBMCSSYM.Trim() )
				{
					itr->SetCompareResult(_T("SYM") , CIsoBMData::MODIFIED);
					bFound = true;
				}
				if (itr->QTY().Trim() != jtr->QTY().Trim() )
				{
					itr->SetCompareResult(_T("QTY") , CIsoBMData::MODIFIED);
					bFound = true;
				}
				if (true == bFound) break; // BM Text 가 이전 도면의 BM Text 와 비교 하여 바뀐것이 있다면 (Matl Code 기준)
			}
		}
		if((true == bFound) || (true == bNewBMData)) 
		{
			IsoRevisedBMDataList.push_back(*itr);
		}
	}

	return IsoRevisedBMDataList;
}

/******************************************************************************
	@author     humkyung
	@date       2011-09-20
	@class      CIsoBMArea
	@function   CompareWith
	@return     int
	@param      list<CIsoElement*>& oRemovedElemList
	@param      list<CIsoElement*>& oAddedElemList
	@param      CIsoBMArea*         rhs
	@brief		compare two iso bm area
******************************************************************************/
int CIsoBMArea::CompareWith(CIsoArea* rhs)
{
	assert(rhs && "rhs is NULL");
	
	if(rhs)
	{
		CDgnDocument* pDgnDoc = m_pIsoModel->GetDgnDocument();

		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
		const double scale = docData.m_dScale;
		double dArcMin = 0.0 , dArcMax = 0.0;;
		docData.GetArcRadiusForCloudMark(dArcMin , dArcMax);

		const UInt32 uiLevel = docData.GetCloudLevel();
		const UInt32 uiColor = docData.GetCloudColor();
		const double dHeight = docData.GetCloudHeight();
		const double dWidth = docData.GetCloudWidth();
		const CString RevNo = docData.GetRevisionNo( pDgnDoc );
		const CString Display = docData.GetCloudDisplay();

		CIsoBMArea* pIsoBMArea = static_cast<CIsoBMArea*>(rhs);

		list<CIsoBMData> oCurrentBMDataList = m_oIsoBMDataList;				 
		list<CIsoBMData> oReferenceBMDataList = pIsoBMArea->m_oIsoBMDataList;  

		for(list<CIsoBMData>::iterator itr = m_oIsoBMDataList.begin();itr != m_oIsoBMDataList.end();++itr) // 현재 도면의 BM Data
		{
			for(list<CIsoBMData>::iterator jtr = pIsoBMArea->m_oIsoBMDataList.begin();jtr != pIsoBMArea->m_oIsoBMDataList.end();++jtr) // 이전 도면의 BM Data
			{
				if((*itr) == (*jtr))
				{
					/// 2011.11.17 - modified by humkyung
					list<CIsoBMData>::iterator where = find(oCurrentBMDataList.begin() , oCurrentBMDataList.end() , *itr);
					if(where != oCurrentBMDataList.end())
					{
						oCurrentBMDataList.erase(where);
					}
					where = find(oReferenceBMDataList.begin() , oReferenceBMDataList.end() , *jtr);
					if(where != oReferenceBMDataList.end())
					{
						oReferenceBMDataList.erase(where);
					}
					/// up to here
					break;
				}
			}
		}

		list<CIsoBMData> oIsoRevisionBMDataList = GetIsoRevisedBMDataListCompareWith(oCurrentBMDataList , oReferenceBMDataList );

		map<pair<int , CString>, vector<DPoint3d>> oIsoRevisedBMDataShapePointMap = GetIsoRevisedBMDataShapePointMap(oIsoRevisionBMDataList);
		
		vector<DPoint3d> CollectGroupShapePoint;

		map<int , CString> m_oDeletedPartNoMap;
		for(map<pair<int , CString>, vector<DPoint3d>>::iterator itr = oIsoRevisedBMDataShapePointMap.begin();itr != oIsoRevisedBMDataShapePointMap.end();++itr)
		{
			CollectGroupShapePoint.clear ();
			int CurrentPartNo = itr->first.first  ;
			CString sRevisedBMText = itr->first.second  ;

			map<int , CString>::iterator where = m_oDeletedPartNoMap.find(CurrentPartNo);
				
			if(where != m_oDeletedPartNoMap.end()) // 맵에 담어져 있다면..
			{
				continue;
			}
			
			CollectGroupShapePoint = itr->second;

			int NextPartNo = CurrentPartNo + 1 ;
			
			bool bFlag = true;
			while(bFlag)
			{
				std::pair<int , CString> FindCondition;
				FindCondition.first = NextPartNo;
				FindCondition.second = itr->first.second;
				map<pair<int , CString>, vector<DPoint3d>>::iterator where = oIsoRevisedBMDataShapePointMap.find(FindCondition);
				
				if(where != oIsoRevisedBMDataShapePointMap.end()) // 같이 그룹화 할 Shape 임
				{
					vector<DPoint3d> ShapePtList = oIsoRevisedBMDataShapePointMap[FindCondition];
					m_oDeletedPartNoMap.insert (make_pair(NextPartNo , _T("")));
					for(vector<DPoint3d>::iterator jtr = ShapePtList.begin(); jtr != ShapePtList.end();++jtr)
					{
						DPoint3d ShapePt;
						ShapePt.x = jtr->x;
						ShapePt.y = jtr->y;
						ShapePt.z = 0;
						CollectGroupShapePoint.push_back (ShapePt);
					}	
					NextPartNo = NextPartNo + 1;
				}
				else
				{
					bFlag = false;
				}
			}
			
			CConvexHull oConvexHull;
			vector<DPoint3d> oEnclosePolygon = oConvexHull.Create (CollectGroupShapePoint);
			oEnclosePolygon.push_back( oEnclosePolygon.front() );
			DrawCloud(&(oEnclosePolygon[0]) , oEnclosePolygon.size() , dArcMin, dArcMax , 
				   uiLevel , uiColor , 0 , 0 , 1 , pDgnDoc);
			/*if(_T("Cloud&RevNo") == Display)
			{
				DPoint3d StartPointForRevNo = CIsoElement::GetStartPointForRevNo(oEnclosePolygon); 
				Draw_RevNo(&StartPointForRevNo, RevNo , uiLevel , uiColor , 0 , 0 , dHeight , dWidth , pDgnDoc);
			}*/
		}
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-04
    @class      CIsoBMArea
    @function   GetBMDataCorrespondingToPartNo
    @return     bool
    @param      CIsoBMData& oIsoBMData
    @param      const       CString&
    @param      sPartNo
    @brief
******************************************************************************/
bool CIsoBMArea::GetBMDataCorrespondingToPartNo(CIsoBMData& oIsoBMData , const CString& sPartNo)
{
	for(list<CIsoBMData>::iterator itr = m_oIsoBMDataList.begin();itr != m_oIsoBMDataList.end();++itr)
	{
		if(sPartNo == itr->PartNo())
		{
			oIsoBMData = (*itr);
			return true;
		}
	}

	return false;
}