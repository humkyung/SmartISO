#include "StdAfx.h"
#include <assert.h>
#include <algorithm>
#include <float.h>
#include <DgnLineString.h>
#include "AttributeElement.h"
#include "isoelement.h"
#include "IsoArea.h"
#include "IsoModelDocData.h"
#include "ConvexHull.h"
using namespace IsoElement;

DPoint3d temp;
CAttributeElement::CAttributeElement(const CString& sPartNo) : m_oIsoBMData(temp , _T("") , _T("")) , m_sPartNo( sPartNo )
{
	CIsoModelDocData& docData = CIsoModelDocData::GetInstance();

	m_pLeaderLine = new CLeaderElement;
	m_dUsedToler = 0.0;
}

CAttributeElement::~CAttributeElement(void)
{
	try
	{
		SAFE_DELETE(m_pLeaderLine);
	}
	catch(...)
	{
	}
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-04
    @class      CAttributeElement
    @function   GetPartNo
    @return     CString
    @brief
******************************************************************************/
CString CAttributeElement::GetPartNo() const
{
	return m_sPartNo;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-04
    @class      CAttributeElement
    @function   GetPartNoExceptS
    @return     CString
    @brief
******************************************************************************/
CString CAttributeElement::GetPartNoExceptS() const
{
	CString sTemp = m_sPartNo;
	if ( _T("S") == sTemp.Left(1))
	{
		sTemp = sTemp.Mid(1 , sTemp.GetLength() - 1);
	}

	return sTemp;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-04
    @class      CAttributeElement
    @function   SetIsoBMData
    @return     int
    @param      const       CIsoBMData&
    @param      oIsoBMData
    @brief
******************************************************************************/
int CAttributeElement::SetIsoBMData(const CIsoBMData& oIsoBMData)
{
	m_oIsoBMData = oIsoBMData;

	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-04
    @class      CAttributeElement
    @function   GetIsoBMData
    @return     CIsoBMData
    @brief
******************************************************************************/
CIsoBMData CAttributeElement::GetIsoBMData() const
{
	return m_oIsoBMData;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-04
    @class      CAttributeElement
    @function   GetElementType
    @return     CString
    @brief		return element type string
******************************************************************************/
CString CAttributeElement::GetElementType() const
{
	///m_oIsoBMData.matlDesc().Find("");
	return _T("");
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-07
    @class      CAttributeElement
    @function   AddLeaderlineElement
    @return     int
    @param      CDgnElement*    pElm
    @brief
******************************************************************************/
int CAttributeElement::AddLeaderlineElement( CDgnElement* pElm )
{
	assert(pElm && "pElm is NULL");

	if(pElm) m_pLeaderLine->Add( pElm );
	
	return ERROR_SUCCESS;
}


CLeaderElement* CAttributeElement::GetLeaderElement()
{
	return m_pLeaderLine;
	
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-07
    @class      CAttributeElement
    @function   GetPartNo
    @return     CDgnText*
    @brief
******************************************************************************/
CDgnText* CAttributeElement::GetPartNoTextElement()
{
	for(vector<CDgnElement*>::iterator itr = m_oDgnElementList.begin();itr != m_oDgnElementList.end();++itr)
	{
		if((*itr)->IsKindOf(CDgnText::TypeString()))	// Part No Text
		{
			CDgnText* pTextElm = static_cast<CDgnText*>(*itr);
			return pTextElm;
		}
	}

	return NULL;
}
/******************************************************************************
    @author     humkyung
    @date       2011-10-13
    @class      CAttributeElement
    @function   GetConnectedFirstLeaderLine
    @return     
    @brief		Part No Area 안에 들어있는 모든 Leader Line 을 담는 함수
******************************************************************************/
vector<CDgnElement*> CAttributeElement::GetConnectedFirstLeaderLine(vector<CDgnElement*>* pDgnElmList , vector<CDgnVolume> oVolumeList , vector<CIsoElement*>* pArrElmList)
{
	assert(pArrElmList && "pArrElmList is NULL");

	vector<CDgnElement*> oTempDgnElmList;
	if(pArrElmList)
	{
		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
	
		if(!oVolumeList.empty())
		{
			for(vector<CDgnElement*>::iterator itr = pDgnElmList->begin();itr != pDgnElmList->end();++itr)
			{
				if(((*itr)->IsKindOf(CDgnLineString::TypeString())))
				{
					CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(*itr);
					if(oTempDgnElmList.end() != find(oTempDgnElmList.begin() , oTempDgnElmList.end() , *itr))
					{
						continue;
					}
					
					if(2 == pLineStringElm->GetVertexCount())
					{
						DPoint3d pts[2] = {0,0,};
						pts[0] = pLineStringElm->GetVertexAt(0);
						pts[1] = pLineStringElm->GetVertexAt(1);

						for(vector<CDgnVolume>::iterator jtr = oVolumeList.begin();jtr != oVolumeList.end();++jtr)
						{
							if ((jtr->Contains(pts[0])) || jtr->Contains(pts[1]))
							{
								oTempDgnElmList.push_back(*itr);
								break;
							}
						}
					}
				}
			}
		}
		else
		{
			/// get the nearest line string from attribute volume - 2011.12.20 added by humkyung
			for(vector<CDgnElement*>::iterator itr = pDgnElmList->begin();itr != pDgnElmList->end();++itr)
			{
				if(((*itr)->IsKindOf(CDgnLineString::TypeString())))
				{
					CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(*itr);
					oTempDgnElmList.push_back(*itr);
				}
			}

			if(oTempDgnElmList.size() > 1)
			{
				DPoint3d ptCenter = this->volume().center();
				stable_sort( oTempDgnElmList.begin() , oTempDgnElmList.end() , CIsoElement::SortDgnElmByDistance(ptCenter) );
				oTempDgnElmList.erase(oTempDgnElmList.begin() + 1 , oTempDgnElmList.end());
			}
			/// up to here
		}

		/// check if linestring has arrow - 2011.12.12 added by humkyung
		const DPoint3d ptCenter = this->volume().center();
		vector<CDgnElement*> oOneArrowList , oTwoArrowList;
		CIsoElement::GetLeaderLineFrom(oOneArrowList , oTwoArrowList , oTempDgnElmList , pArrElmList , ptCenter);
		if(1 == oOneArrowList.size())
		{
			oTempDgnElmList.clear();
			oTempDgnElmList.push_back( oOneArrowList[0] );
		}
		/// up to here
	}

	return oTempDgnElmList;
}

/// author : yjk
/// Patr No 의 Volume 안에 들어 있는 Dimension Leader Line을 찾아서 push_back
void CAttributeElement::CollectLeaderLine(vector<CDgnElement*>* pDgnElmList , vector<CDgnVolume> oVolumeList , vector<CIsoElement*>* pArrElmList)
{
	assert(pDgnElmList && pArrElmList && "pDgnElmList or pArrElmList is NULL");
	if(pDgnElmList)
	{
		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();

		vector<CDgnElement*> oTempDgnElmList = GetConnectedFirstLeaderLine(pDgnElmList , oVolumeList , pArrElmList);
		if(oTempDgnElmList.empty()) return;
		
		int iMaxDistLeaderLine = oTempDgnElmList.size () - 1;

		if(oTempDgnElmList.size() > 1)
		{
			DPoint3d ptCenter = volume().center();
			stable_sort( oTempDgnElmList.begin() , oTempDgnElmList.end() , CIsoElement::SortDgnElmByDistance(ptCenter) );
		}
		
		AddLeaderlineElement( oTempDgnElmList[0] );
		pDgnElmList->erase( find(pDgnElmList->begin() , pDgnElmList->end() , oTempDgnElmList[0]));
		
		if(1 == m_pLeaderLine->m_oDgnElementList.size())
		{
			/// Conection Point을 구하는 로직
			m_pLeaderLine->ClearConnPointList();

			const DPoint3d center = this->volume().center();
			for(vector<CDgnElement*>::iterator itr = m_pLeaderLine->m_oDgnElementList.begin();itr != m_pLeaderLine->m_oDgnElementList.end();++itr)
			{
				if((*itr)->IsKindOf(CDgnLineString::TypeString()))
				{
					CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(*itr);
					const int iVertexCount = pLineStringElm->GetVertexCount();
					DPoint3d pts[2] = {0,0};
					pts[0] = pLineStringElm->GetVertexAt(0);
					pts[1] = pLineStringElm->GetVertexAt(iVertexCount - 1);
					const double d1 = ::DistanceBetween(center , pts[0]);
					const double d2 = ::DistanceBetween(center , pts[1]);

					vector<DPoint3d> oVertexList = pLineStringElm->GetLineStringPointList();
					if(d1 > d2)
					{
						::reverse(oVertexList.begin() , oVertexList.end());
					}
					
					for(vector<DPoint3d>::iterator jtr = oVertexList.begin();jtr != oVertexList.end();++jtr)
					{
						m_pLeaderLine->AddConnPoint(*jtr);
					}
				}
			}
		}
	}
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-07
    @class      CAttributeElement
    @function   GetAttrType
    @return     CString
    @brief		return attribute type
******************************************************************************/
CString CAttributeElement::GetAttrType()
{
	if((_T("RWELD") == m_sPartNo) || (_T("RPAD") == m_sPartNo)) return _T("RWELD");

	if(_T("S") == GetPartNo().Left(1)) return _T("PIPE SUPPORT");
	if(0 == m_sPartNo.Find(_T("EL"))) return _T("ELEVATION");
	
	CString sMatlDesc = m_oIsoBMData.matlDesc() + m_oIsoBMData.GetAllSubString();
	if((-1 != sMatlDesc.Find(_T("TEE"))) || (-1 != sMatlDesc.Find(_T("45 RED."))))
	{
		return _T("TEE");
	}
	///NOTICE: 수정필요...
	else if((-1 != sMatlDesc.Find(_T("ELBOW"))) || (-1 != sMatlDesc.Find(_T("90 EL"))))
	{
		return _T("ELBOW");
	}
	else if(-1 != sMatlDesc.Find(_T("OLET")))
	{
		return _T("OLET");
	}
	else if(-1 != sMatlDesc.Find(_T("PIPE")))
	{
		return _T("PIPE");
	}
	else if(-1 != sMatlDesc.Find(_T("REDUCER")))
	{
		return _T("REDUCER");
	}

	return _T("NONE");
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-16
    @class      CAttributeElement
    @function   ExpandVolume
    @return     int
    @param      CDgnVolume&  volume
    @brief
******************************************************************************/
int CAttributeElement::ExpandVolume( CDgnVolume& volume )
{
	DPoint3d pt;
	pt.x = volume.maxx() + m_dUsedToler;
	pt.y = volume.maxy() + m_dUsedToler;
	pt.z = 0.0;
	volume.Add( pt );

	pt.x = volume.minx() - m_dUsedToler;
	pt.y = volume.miny() - m_dUsedToler;
	pt.z = 0.0;
	volume.Add( pt );

	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-16
    @class      CAttributeElement
    @function   Add
    @return     int
    @param      CDgnElement*    p
    @brief
******************************************************************************/
int CAttributeElement::Add(CDgnElement* p)
{
	assert(p && "p is NULL");
	if(NULL != p)
	{
		if((0.0 == m_dUsedToler) && p->IsKindOf(CDgnText::TypeString()))
		{
			CDgnText* pTextElm = static_cast<CDgnText*>(p);
			m_dUsedToler = pTextElm->GetTextHeight();
		}

		return CIsoElement::Add( p );
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
	@author     humkyung
	@date       2011-09-20
	@class      FindConnectedPartNoElement
	@function   
	@return     void
	@brief      Part No 와 x 좌표 값이 같고 일정 간격으로 y 축은 /밑에 있는 Text를 찾음
******************************************************************************/
void CAttributeElement::CollectPartOfAttribute( vector<CDgnElement*>* pDgnElmList , CDgnText* pTextElm , DPoint3d ptOrigin )
{
	assert(pDgnElmList && pTextElm && "pDgnElmList or pTextElm is NULL");

	if(pDgnElmList && pTextElm)
	{
		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();

		const CString sAttrElmType = this->GetAttrType();
		const double dTextHeight = pTextElm->GetTextHeight();
		const double dToler = dTextHeight * 2.0;	/// 2011.11.16 added by humkyung
		const double dAngle = pTextElm->angle();
		const double dAngleToler = docData.GetAngleToler();

		vector<CDgnText*> oTextElementList;
		for(vector<CDgnElement*>::iterator itr = pDgnElmList->begin();itr != pDgnElmList->end();++itr)
		{
			if((*itr)->IsKindOf(CDgnText::TypeString()))
			{
				CDgnText* _pTextElm = static_cast<CDgnText*>(*itr);
				/// check angle - 2012.01.02. added by humkyung
				if(fabs(dAngle - _pTextElm->angle()) > dAngleToler) continue;
				
				if ((_T("PIPE SUPPORT") == sAttrElmType) || (_T("ELBOW") == sAttrElmType))
				{
					if((fabs(ptOrigin.x - _pTextElm->origin().x) < dTextHeight) && (ptOrigin.y > _pTextElm->origin().y))
					{
						oTextElementList.push_back( _pTextElm );
					}
				}
				else if  (_T("PIPE") == sAttrElmType) 
				{
					if ((-1 != CString(_pTextElm->textString()).Find(_T("NPS"))) && (ptOrigin.y > _pTextElm->origin().y))
					{
						oTextElementList.push_back( _pTextElm );
					}
				}
				/// 2011.11.01 added by humkyung
				else if((_T("RWELD") == sAttrElmType) || (_T("RPAD") == sAttrElmType))
				{
					if((ptOrigin.x == _pTextElm->origin().x) && (_pTextElm->origin().y > ptOrigin.y))
					{
						oTextElementList.push_back( _pTextElm );
					}
				}
				/// up to here
				else if(_T("ELEVATION") == sAttrElmType)
				{
					if((fabs(ptOrigin.x - _pTextElm->origin().x) < dTextHeight) && (ptOrigin.y < _pTextElm->origin().y))
					{
						oTextElementList.push_back( _pTextElm );
					}
				}
				else
				{
					oTextElementList.push_back( _pTextElm );
				}
			}
		}

		if (!oTextElementList.empty ())
		{
			stable_sort(oTextElementList.begin() , oTextElementList.end() , CIsoElement::SortDgnElmByDistanceFromDgnElm(pTextElm));	

			if(_T("PIPE") == sAttrElmType)
			{
				const double dDist = ::DistanceBetween(ptOrigin , oTextElementList[0]->origin());
				if(dDist < dToler)
				{
					this->Add( oTextElementList[0] );
					pDgnElmList->erase( find(pDgnElmList->begin() , pDgnElmList->end() , oTextElementList[0]) );
				}
			}
			else
			{
				DPoint3d ptStart = ptOrigin;
				for(vector<CDgnText*>::iterator itr = oTextElementList.begin();itr != oTextElementList.end();++itr)
				{
					const double dDist = ::DistanceBetween(ptStart , ((*itr)->origin()));
					if(dDist < dToler)
					{
						this->Add( *itr );
						ptStart = (*itr)->origin();
						pDgnElmList->erase( find(pDgnElmList->begin() , pDgnElmList->end() , *itr) );
					}
				}
			}
		}

		if(_T("PIPE") == sAttrElmType)
		{
			/// find a text start with '<' and end with '>'
			vector<CDgnText*> oPieceNoTextElementList;
			for(vector<CDgnElement*>::iterator itr = pDgnElmList->begin();itr != pDgnElmList->end();++itr)
			{
				if((*itr)->IsKindOf(CDgnText::TypeString()))
				{
					CDgnText* pTextElm = static_cast<CDgnText*>(*itr);

					CString sPieceNo = pTextElm->textString();

					if ((_T("<") == sPieceNo.Trim().Left(1)) && (_T(">") == sPieceNo.Trim().Right(1)))
					{
						oPieceNoTextElementList.push_back(pTextElm);
					}
				}
			}

			CDgnText* pPieceNoTextElm = NULL;
			double dMinDist = DBL_MAX;

			for(vector<CDgnText*>::iterator itr = oPieceNoTextElementList.begin();itr != oPieceNoTextElementList.end();++itr)
			{
				const double dDist = ::DistanceBetween(ptOrigin , ((*itr)->origin()));
				if(dDist < dMinDist)
				{
					dMinDist = dDist;
					pPieceNoTextElm = *itr;
				}
			}
			if (NULL != pPieceNoTextElm)
			{
				this->Add( pPieceNoTextElm );
			}	
		}
	}
}

/******************************************************************************
    @author     humkyung
    @date       2011-12-02
    @class      CAttributeElement
    @function   CreateVolumeList
    @return     int
    @param      vector<CDgnVolume>&  oVolumeList
    @brief
******************************************************************************/
int CAttributeElement::CreateVolumeList(vector<CDgnVolume>& oVolumeList)
{
	for(vector<CDgnElement*>::iterator itr = m_oDgnElementList.begin();itr != m_oDgnElementList.end();++itr)
	{
		oVolumeList.push_back( (*itr)->volume() );
	}

	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2011-12-22
    @class      CAttributeElement
    @function   GetShapeForCloudMark
    @return     int
    @param      vector<DPoint3d>&   oShapeForCloudMark
    @brief
******************************************************************************/
int CAttributeElement::GetShapeForCloudMark(vector<DPoint3d>& oShapeForCloudMark)
{
	CIsoModelDocData& docData = CIsoModelDocData::GetInstance();

	const double scale = docData.m_dScale;
	const double dOffset = docData.GetOffset() * 0.5;
	
	CDgnVolume _volume = this->volume();
	_volume.Expand(dOffset , dOffset);

	DPoint3d pt;
	pt.x = _volume.minx();
	pt.y = _volume.miny();
	pt.z = 0.0;
	oShapeForCloudMark.push_back(pt);
	pt.x = _volume.maxx();
	pt.y = _volume.miny();
	pt.z = 0.0;
	oShapeForCloudMark.push_back(pt);
	pt.x = _volume.maxx();
	pt.y = _volume.maxy();
	pt.z = 0.0;
	oShapeForCloudMark.push_back(pt);
	pt.x = _volume.minx();
	pt.y = _volume.maxy();
	pt.z = 0.0;
	oShapeForCloudMark.push_back(pt);

	if(NULL != m_pLeaderLine)
	{
		vector<DPoint3d> oConnPtList;
		m_pLeaderLine->GetConnPointList(oConnPtList);
		oShapeForCloudMark.insert( oShapeForCloudMark.end() , oConnPtList.begin() , oConnPtList.end() );
	}

	CConvexHull oConvexhull;
	oShapeForCloudMark = oConvexhull.Create( oShapeForCloudMark );
	oShapeForCloudMark.push_back( oShapeForCloudMark.front() );
	
	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-18
    @class      CAttributeElement
    @function   DrawCloudMark
    @return     int
    @param      DgnModelRefP    modelRef
    @brief
******************************************************************************/
int CAttributeElement::DrawCloudMark( CDgnDocument* pDgnDoc )
{
	assert(pDgnDoc && "pDgnDoc is NULL");

	if(pDgnDoc)
	{
		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();

		const double scale = docData.m_dScale;
		double dArcMin = 0.0 , dArcMax = 0.0;
		docData.GetArcRadiusForCloudMark(dArcMin , dArcMax);

		const UInt32 uiLevel = docData.GetCloudLevel();
		const UInt32 uiColor = docData.GetCloudColor();
		const double dHeight = docData.GetCloudHeight(); 
		const double dWidth = docData.GetCloudWidth();
		const CString RevNo = docData.GetRevisionNo( pDgnDoc );
		const CString sDisplay = docData.GetCloudDisplay();

		vector<DPoint3d> oShapeForCloudMark;
		this->GetShapeForCloudMark(oShapeForCloudMark);
		for(vector<DPoint3d>::iterator itr = oShapeForCloudMark.begin();itr != oShapeForCloudMark.end();++itr)
		{
			itr->x /= scale;
			itr->y /= scale;
			itr->z /= scale;
		}

		DrawCloud(&(oShapeForCloudMark[0]) , oShapeForCloudMark.size() , dArcMin, dArcMax , uiLevel , uiColor , 0 , 0 , 1 , pDgnDoc);
		if(_T("Cloud&RevNo") == sDisplay)
		{
			DPoint3d StartPointForRevNo = CIsoElement::GetStartPointForRevNo(oShapeForCloudMark); 
			Draw_RevNo(&StartPointForRevNo, RevNo , uiLevel , uiColor , 0 , 0 , dHeight , dWidth , pDgnDoc);
		}
		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

void CAttributeElement::Check(CDgnDocument* pDgnDoc , CString sColor)
{
	UInt32 uiColor = ATOI_T(sColor);
	for(vector<CDgnElement*>::iterator itr = m_oDgnElementList.begin();itr != m_oDgnElementList.end();++itr)
	{
		(*itr)->SetColor( uiColor );
	}

	vector<CDgnVolume> oVolumeList;
	this->CreateVolumeList(oVolumeList);
	for(vector<CDgnVolume>::iterator itr = oVolumeList.begin();itr != oVolumeList.end();++itr)
	{
		this->ExpandVolume( *itr );

		vector<DPoint3d> oPolyline;
		DPoint3d pt;
		pt.x = itr->minx();
		pt.y = itr->miny();
		pt.z = itr->minz();
		oPolyline.push_back( pt );

		pt.x = itr->maxx();
		pt.y = itr->miny();
		pt.z = itr->minz();
		oPolyline.push_back( pt );

		pt.x = itr->maxx();
		pt.y = itr->maxy();
		pt.z = itr->minz();
		oPolyline.push_back( pt );

		pt.x = itr->minx();
		pt.y = itr->maxy();
		pt.z = itr->minz();
		oPolyline.push_back( pt );

		pt.x = itr->minx();
		pt.y = itr->miny();
		pt.z = itr->minz();
		oPolyline.push_back( pt );

		::DrawPolyline(pDgnDoc , oPolyline , sColor);
	}

	if(m_pLeaderLine) m_pLeaderLine->Check(pDgnDoc , sColor);	
}