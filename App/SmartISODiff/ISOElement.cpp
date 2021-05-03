#include "StdAfx.h"
#include <assert.h>
#include <algorithm>

#include <DgnComplexShapeHeader.h>
#include <DgnArc.h>

#include "IsoElement.h"

#include "ElbowElement.h"
#include "TeeElement.h"
#include "OletElement.h"
#include "ReducerElement.h"
#include "RWeldElement.h"
#include "InlineElement.h"
#include "ArrowElement.h"
#include "IsoModelDocData.h"
#include "Minball2dImpl.h"

using namespace IsoElement;

CIsoElement::CIsoElement()
{
	m_sTypeString = _T("none");
}

CIsoElement::~CIsoElement(void)
{
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-22
    @class      CIsoElement
    @function   IsFittingElm
    @return     bool
    @brief		return true if it is fitting element
******************************************************************************/
bool CIsoElement::IsFittingElm() const
{
	const STRING_T sTypeString = typeString();
	return	(
				(CElbowElement::TypeString() == sTypeString) || 
				(CTeeElement::TypeString() == sTypeString) || 
				(COletElement::TypeString() == sTypeString) || 
				(CReducerElement::TypeString() == sTypeString) || 
				(CInlineElement::TypeString() == sTypeString) ||
				(CRWeldElement::TypeString() == sTypeString)
			);

	return false;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-22
    @class      CIsoElement
    @function   IsBranchElm
    @return     bool
    @brief		return true if it is branch element(tee or olet)
******************************************************************************/
bool CIsoElement::IsBranchElm() const
{
	const STRING_T sTypeString = typeString();
	return	(
				(CTeeElement::TypeString() == sTypeString) || 
				(COletElement::TypeString() == sTypeString)||
				(CRWeldElement::TypeString() == sTypeString)
			);

	return false;
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-03
    @class      CIsoElement
    @function   ShouldHaveOwnDim
    @return     bool
    @brief		return true if it have it's own dimension
******************************************************************************/
bool CIsoElement::ShouldHaveOwnDim() const
{
	const STRING_T sTypeString = typeString();

	return (
			(CReducerElement::TypeString() == sTypeString) || 
			(CInlineElement::TypeString() == sTypeString)
		);
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-17
    @class      CDimElement
    @function   Collinear
    @return     bool
    @param      CLineStringElement* lhs
    @param      CLineStringElement* rhs
	@param		dMinAngle	degree
    @brief		
******************************************************************************/
bool CIsoElement::Collinear(DPoint3d lhs[2] , DPoint3d rhs[2] , const double& dMinAngle)
{
	assert(lhs && rhs && "lhs or rhs is NULL");

	if(lhs && rhs)
	{
		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
		const double dToler = CIsoModelDocData::m_dDrawingToler;
		DPoint3d ptMid;
		ptMid.x = (lhs[0].x + lhs[1].x)*0.5;
		ptMid.y = (lhs[0].y + lhs[1].y)*0.5;
		ptMid.z = 0.0;

		///NOTICE: check distance - 2011.11.10 added by humkyung
		const double d = DistanceFromPointToLine(ptMid , rhs);
		if(d < dToler)
		{
		/// up to here
			vector<DPoint3d> oPtList;

			oPtList.push_back( lhs[0] );
			oPtList.push_back( lhs[1] );
			oPtList.push_back( rhs[0] );
			oPtList.push_back( rhs[1] );

			DVec3d vec;
			vec.x = oPtList[1].x - oPtList[0].x;
			vec.y = oPtList[1].y - oPtList[0].y;
			vec.z = oPtList[1].z - oPtList[0].z;
			double angle1 = ::AngleOf(vec);
			if(angle1 < 0.0) angle1 += 2*PI;

			vec.x = oPtList[3].x - oPtList[2].x;
			vec.y = oPtList[3].y - oPtList[2].y;
			vec.z = oPtList[3].z - oPtList[2].z;
			double angle2 = ::AngleOf(vec);
			if(angle2 < 0.0) angle2 += 2*PI;

			const double delta1 = RAD2DEG(fabs(angle1 - angle2));
			if(IsParallelAngle( RAD2DEG(angle1) , RAD2DEG(angle2) , dMinAngle))
			{
				/// find two points which has longest distance
				DPoint3d ptConn[2]={0,0} , ptMaxConn[2]={0,0};
				double /*dMinDist = -1.0 , */dMaxDist = 0.0;
				for(int i = 0;i < 2;++i)
				{
					for(int j = 2;j < 4;++j)
					{
						const double d = ::DistanceBetween((oPtList[i]) , (oPtList[j]));
						if(d > dMaxDist)
						{
							dMaxDist = d;
							ptMaxConn[0] = oPtList[i];
							ptMaxConn[1] = oPtList[j];
						}
					}
				}
				/// up to here

				vec.x = ptMaxConn[1].x - ptMaxConn[0].x;
				vec.y = ptMaxConn[1].y - ptMaxConn[0].y;
				vec.z = ptMaxConn[1].z - ptMaxConn[0].z;
				double angle3 = ::AngleOf(vec);
				if(angle3 < 0.0) angle3 += 2*PI;

				const double delta2 = RAD2DEG(fabs(angle1 - angle3));
				return ((delta2 < dMinAngle) || (fabs(180.0 - delta2) < dMinAngle));
			}
		}
	}

	return false;
}
/******************************************************************************
    @author     humkyung
    @date       2011-09-27
    @class      CIsoElement
    @function   typeString
    @return     STRING_T
    @brief
******************************************************************************/
STRING_T CIsoElement::typeString() const
{
	return m_sTypeString;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-12
    @class      CPipeElement
    @function   GetConnPointList
    @return     int
    @param      vector<DPoint3d>&   pts
    @brief		return connection point of iso element
******************************************************************************/
int CIsoElement::GetConnPointList(vector<DPoint3d>& pts)
{
	pts.clear();
	pts.insert( pts.begin() , m_ptConn.begin() , m_ptConn.end() );
	
	return ERROR_SUCCESS;
}

vector<DPoint3d> CIsoElement::GetptConn()
{
	return m_ptConn;	/// connection point list
}
/******************************************************************************
    @author     humkyung
    @date       2011-09-27
    @class      CIsoElement
    @function   Add
    @return     int
    @param      CDgnElement*    p
    @brief
******************************************************************************/
int CIsoElement::Add(CDgnElement* p)
{
	assert(p && "p is NULL");
	if(p) m_oDgnElementList.push_back(p);

	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2011-09-27
    @class      CIsoElement
    @function   DgnElementP
    @return     CDgnElement*
    @param      const   int&
    @param      at
    @brief
******************************************************************************/
CDgnElement* CIsoElement::DgnElementP( const int& at )
{
	if( at < int(m_oDgnElementList.size()) ) return m_oDgnElementList[at];

	throw std::range_error(("index out of range"));
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-06
    @class      CIsoElement
    @function   size
    @return     size_t
    @brief		return number of element
******************************************************************************/
size_t CIsoElement::size() const
{
	return m_oDgnElementList.size();
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-04
    @class      CIsoElement
    @function   volume
    @return     CDgnVolume
    @brief		return a volume
******************************************************************************/
CDgnVolume CIsoElement::volume()
{
	CDgnVolume vol;
	for(vector<CDgnElement*>::iterator itr = m_oDgnElementList.begin();itr != m_oDgnElementList.end();++itr)
	{
		DPoint3d pt3d;
		pt3d.x = (*itr)->volume().minx();
		pt3d.y = (*itr)->volume().miny();
		pt3d.z = (*itr)->volume().minz();
		vol.Add(pt3d);

		pt3d.x = (*itr)->volume().maxx();
		pt3d.y = (*itr)->volume().maxy();
		pt3d.z = (*itr)->volume().maxz();
		vol.Add(pt3d);
	}

	return vol;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-10
    @class      CIsoElement
    @function   DistanceTo
    @return     double
    @param      const   DPoint3d&
    @param      pt
    @brief		return distance between pt and iso element
******************************************************************************/
double CIsoElement::DistanceTo( const DPoint3d& pt ) const
{
	double dMinDist = -1.0;
	for(vector<CDgnElement*>::const_iterator itr = m_oDgnElementList.begin();itr != m_oDgnElementList.end();++itr)
	{
		const double dDist = (*itr)->DistanceTo( pt );
		if(-1.0 == dMinDist)
		{
			dMinDist = dDist;
		}
		else if(dDist < dMinDist)
		{
			dMinDist = dDist;
		}
	}

	return dMinDist;
}

/******************************************************************************
    @author     yjk
    @date       2011-10-11
    @class      CAttributeElement
    @function   HasDimArrowElement
    @return     bool
    @brief      LeaderlineElement End Point 에 Arrow가 존재 하는지 검사 , 있으면 true , 없으면 false
******************************************************************************/
CIsoElement* CIsoElement::GetArrowElementHas(const DPoint3d& ptEnd , DVector3d* pDir , vector<CIsoElement*>* pArrElmList , double dAngleToler)
{
	CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
	const double dToler = CIsoModelDocData::m_dDrawingToler;

	double angle1 = 0.0;
	if(NULL != pDir)
	{
		DVec3d vec;
		vec.x = pDir->end.x - pDir->org.x;
		vec.y = pDir->end.y - pDir->org.y;
		vec.z = pDir->end.z - pDir->org.z;

		angle1 = ::AngleOf(vec);
		if(angle1 < 0.0) angle1 += 2*PI;
		angle1 = RAD2DEG(angle1);
	}

	for(vector<CIsoElement*>::iterator itr = pArrElmList->begin() ;itr != pArrElmList->end() ; ++itr)
	{
		CDgnVolume volume_ = (*itr)->volume();
		CArrowElement* pArr = static_cast<CArrowElement*>(*itr);

		double angle2 = 0.0;
		if(NULL != pDir)
		{	
			angle2 = pArr->angle();
		}

		if((fabs(angle1 - angle2) < dAngleToler) || (fabs(360.0 - fabs(angle1 - angle2)) < dAngleToler))
		{
			if(NULL != pDir)
			{
				DVector3d l2;
				l2.org = pArr->m_oShapeList[0];
				l2.end = pArr->m_oShapeList[1];

				DPoint3d intersect;
				if(SUCCESS == ::IntersectWith(intersect , (*pDir) , l2))
				{
					DPoint3d ptMid[2];
					double radius[2] = {0,0};
					ptMid[0].x = (l2.org.x + l2.end.x)*0.5;
					ptMid[0].y = (l2.org.y + l2.end.y)*0.5;
					ptMid[0].z = (l2.org.z + l2.end.z)*0.5;
					radius[0] = ::DistanceBetween(l2.end , ptMid[0]) + (dToler * 0.5);

					ptMid[1].x = (pDir->org.x + pDir->end.x)*0.5;
					ptMid[1].y = (pDir->org.y + pDir->end.y)*0.5;
					ptMid[1].z = (pDir->org.z + pDir->end.z)*0.5;
					radius[1] = ::DistanceBetween((pDir->end) , ptMid[1]) + (dToler * 0.5);
					
					if((::DistanceBetween(intersect , ptMid[0]) <= radius[0]) && (::DistanceBetween(intersect , ptMid[1]) <= radius[1]))
					{
						return (*itr);
					}
				}
			}
			else
			{
				vector<DPoint3d> oEnclosePolygon = pArr->GetEnclosePolygon();
				pArr->ExpandEnclosePolygon(oEnclosePolygon/* , dToler*/);
				if(CIsoElement::IsInnerPt( oEnclosePolygon , ptEnd ))
				{
					return (*itr);
				}
			}
		}
	}

	return NULL;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-27
    @class      CIsoElement
    @function   DrawCloudMark
    @return     int
    @param      DgnModelRefP    modelRef
    @brief
******************************************************************************/
int CIsoElement::DrawCloudMark( DgnModelRefP modelRef )
{
	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-27
    @class      CIsoElement
    @function   center
    @return     DPoint3d
    @brief		return center of iso element
******************************************************************************/
DPoint3d CIsoElement::center() const
{
	DPoint3d center;
	center.x = center.y = center.z = 0.0;

	if(!m_ptConn.empty())
	{
		DPoint3d pt1 = m_ptConn[0];
		DPoint3d pt2 = m_ptConn[m_ptConn.size() - 1];
		
		center.x = (pt1.x + pt2.x)*0.5;
		center.y = (pt1.y + pt2.y)*0.5;
		center.z = (pt1.z + pt2.z)*0.5;
	}

	return center;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-28
    @class      CIsoElement
    @function   Reverse
    @return     int
    @brief		reverse dgn element and connection point list
******************************************************************************/
int CIsoElement::Reverse()
{
	std::reverse(m_oDgnElementList.begin() , m_oDgnElementList.end());
	std::reverse(m_ptConn.begin() , m_ptConn.end());

	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-25
    @class      CPipeElement
    @function   IsKindOf
    @return     bool
    @param      const       STRING_T&
    @param      sTypeString
    @brief
******************************************************************************/
const bool CIsoElement::IsKindOf(const STRING_T& sTypeString) const
{
	return (m_sTypeString == sTypeString);
}


void CIsoElement::Check(CDgnDocument* pDgnDoc , CString sColor)
{
	UInt32 uiColor = ATOI_T(sColor);
	for(vector<CDgnElement*>::iterator itr = m_oDgnElementList.begin();itr != m_oDgnElementList.end();++itr)
	{
		(*itr)->SetColor( uiColor );
	}
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-13
    @class      CIsoElement
    @function   IsSamePoint
    @return     bool
    @param      DPoint3d    pt1
    @param      DPoint3d    pt2
    @param      const       double
    @param      dToler
    @brief
******************************************************************************/
bool CIsoElement::IsSamePoint( DPoint3d pt1 , DPoint3d pt2 , const double dToler )
{
	const double d = ::DistanceBetween(pt1 , pt2);
	return (d < dToler);
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-13
    @class      CIsoElement
    @function   IsInnerPt
    @return     bool
    @param      vector<DPoint3d>    polygon
    @param      DPoint3d            pt
    @brief		pt가 polygon안에 들어 있는지 검사한다.
******************************************************************************/
bool CIsoElement::IsInnerPt(vector<DPoint3d> polygon , DPoint3d pt)
{
	double x = 0.0;		// x intersection of e with ray 
	vector<DPoint3d> P;
	// Shift so that q is the origin. Note this destroys the polygon.
	// This is done for pedogical clarity.
	vector<DPoint3d>::iterator itr;
	for(itr = polygon.begin();itr != polygon.end();++itr)
	{
		if((itr + 1) != polygon.end())
		{
			DPoint3d res[2]={0,0};

			res[0] = (*itr);
			res[0].x -= pt.x;
			res[0].y -= pt.y;
			res[0].z -= pt.z;

			res[1] = *(itr + 1);
			res[1].x -= pt.x;
			res[1].y -= pt.y;
			res[1].z -= pt.z;

			if(res[0].x*res[1].y == res[0].y*res[1].x) return false;
		}
		if(CIsoElement::IsSamePoint((*itr) , pt)) return true;

		DPoint3d pt_;
		pt_.x = itr->x - pt.x;
		pt_.y = itr->y - pt.y;
		pt_.z = itr->z - pt.z;
		P.push_back( pt_ );
	}

	int     Rcross = 0;	// number of right edge/ray crossings
	int     Lcross = 0;	// number of left edge/ray crossings
	const int n = P.size();
	int i = 0 , i1=0;
	// For each edge e=(i-1,i), see if crosses ray.
	for(itr = P.begin();itr != P.end();++itr,++i) 
	{
		//* First see if q=(0,0) is a vertex.
		i1 = ( i + n - 1 ) % n;

		//* if e "straddles" the x-axis...
		//* The commented-out statement is logically equivalent to the one 
		//* following. 
		if( ( P[i].y  > 0. ) != ( P[i1].y > 0. ) ) 
		{
			//* e straddles ray, so compute intersection with ray.
			x = (P[i].x * (double)P[i1].y - P[i1].x * (double)P[i].y)/(double)(P[i1].y - P[i].y);

			//* crosses ray if strictly positive intersection.
			if (x >= (0)) Rcross++;
		}

		//* if e straddles the x-axis when reversed... */
		//* if( ( ( P[i] [Y] < 0 ) && ( P[i1][Y] >= 0 ) ) ||
		//* ( ( P[i1][Y] < 0 ) && ( P[i] [Y] >= 0 ) ) )  {
		if ( ( P[i].y < 0 ) != ( P[i1].y < 0 ) ) 
		{ 
			//* e straddles ray, so compute intersection with ray.
			x = (P[i].x * P[i1].y - P[i1].x * P[i].y)/(double)(P[i1].y - P[i].y);

			//* crosses ray if strictly positive intersection.
			if (x <= (0)) Lcross++;
		}
	}

	//* q on the edge if left and right cross are not the same parity.
	if((Rcross%2 ) != (Lcross%2)) return true;

	//* q inside if an odd number of crossings.
	if((Rcross % 2) == 1) return true;
	else		      return false;
}
/******************************************************************************
	@author     kyj
	@date       2011-11-22
	@class      CAttributeElement
	@function   GetLeftTopPointOfEnclosePolygon
	@return     DPoint3d
	@param      vector<DPoint3d> EnclosePolygon
	@brief		RevNo 를 그리기 위한 시작점을 리턴
******************************************************************************/
DPoint3d CIsoElement::GetStartPointForRevNo(vector<DPoint3d> EnclosePolygon)
{
	double MinX = -1;
	double MaxY = -1;

	for(vector<DPoint3d>::iterator itr = EnclosePolygon.begin();itr != EnclosePolygon.end(); ++itr)
	{
		if (-1 == MinX)
		{
			MinX = itr->x;
			MaxY = itr->y;
		}
		else if (MinX > itr->x)
		{
			MinX = itr->x;
			MaxY = itr->y;
		}
	}

	DPoint3d LeftTopPoint;
	LeftTopPoint.x = MinX;
	LeftTopPoint.y = MaxY;
	LeftTopPoint.z = 0;
	return LeftTopPoint;
}

/******************************************************************************
    @author     humkyung
    @date       2011-12-01
    @class      CIsoElement
    @function   IsPossibleToBeCircle
    @return     bool
    @param      CDgnLineString*  pLineStringElm
    @brief		check if it can be circle
******************************************************************************/
bool CIsoElement::IsPossibleToBeCircle(DPoint3d& center , double& dRadius , CDgnElement* pDgnElm)
{
	assert(pDgnElm && "pDgnElm is NULL");

	if(pDgnElm)
	{
		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
		const double dToler = docData.m_dDrawingToler*1.5;

		if(pDgnElm->IsKindOf(CDgnLineString::TypeString()))
		{
			CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(pDgnElm);

			vector<DPoint3d> oPointList = pLineStringElm->GetLineStringPointList();
			if(oPointList.size() > 8)
			{
				CMinball2dImpl oMinball;
				if(ERROR_SUCCESS == oMinball.CalculateFrom(oPointList))
				{
					center = oMinball.center();
					dRadius= oMinball.radius();

					for(vector<DPoint3d>::iterator itr = oPointList.begin();itr != oPointList.end();++itr)
					{
						const double dx = itr->x - center.x;
						const double dy = itr->y - center.y;
						const double r = sqrt(dx*dx + dy*dy);
						const double d = fabs(dRadius - r);
						if( d > dToler) return false;
					}

					return true;
				}
			}
		}
		else if(pDgnElm->IsKindOf(CDgnComplexShapeHeader::TypeString()))
		{
			CDgnComplexShapeHeader* pCmplxShapeElm = static_cast<CDgnComplexShapeHeader*>(pDgnElm);
			const size_t iEntCount = pCmplxShapeElm->GetEntityCount();
			if((2 == iEntCount) && pCmplxShapeElm->GetEntityAt(0)->IsKindOf(CDgnArc::TypeString()) && pCmplxShapeElm->GetEntityAt(1)->IsKindOf(CDgnArc::TypeString()))
			{
				CDgnArc* pArcElm1 = static_cast<CDgnArc*>(pCmplxShapeElm->GetEntityAt(0));
				CDgnArc* pArcElm2 = static_cast<CDgnArc*>(pCmplxShapeElm->GetEntityAt(1));

				return CIsoElement::IsSamePoint(pArcElm1->origin() , pArcElm2->origin() , docData.m_dDrawingToler);
			}
		}
	}

	return false;
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-17
    @class		CDimElement
    @function   GetLeaderLineFrom
    @return     int
    @param      vector<CDgnElement*>&   oOneArrowList
    @param      vector<CDgnElement*>&   oTwoArrowList
    @param      const                   vector<CDgnElement*>&
    @param      oCandidatedElmList
    @brief
******************************************************************************/
int CIsoElement::GetLeaderLineFrom(vector<CDgnElement*>& oOneArrowList , vector<CDgnElement*>& oTwoArrowList , 
								   const vector<CDgnElement*>& oCandidatedElmList , vector<CIsoElement*>* pArrElmList , const DPoint3d& ptCenter)
{
	assert(pArrElmList && "pArrElmList is NULL");

	if((NULL != pArrElmList) && !oCandidatedElmList.empty())
	{
		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
		const double dAngleToler = docData.GetAngleToler();

		oOneArrowList.clear();
		oTwoArrowList.clear();

		vector<CDgnElement*> oElmHasArrowList;
		for(vector<CDgnElement*>::const_iterator itr = oCandidatedElmList.begin();itr != oCandidatedElmList.end();++itr)
		{
			if(!(*itr)->IsKindOf(CDgnLineString::TypeString())) continue;
			DPoint3d pts[2]={0,0};

			CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(*itr);
			const int iVertexCount = pLineStringElm->GetVertexCount();
			if(iVertexCount < 2) continue;
			pts[0] = pLineStringElm->GetVertexAt(0);
			pts[1] = pLineStringElm->GetVertexAt(iVertexCount - 1);
			DVector3d dir[2];
			dir[0].org = pLineStringElm->GetVertexAt(0);
			dir[0].end = pLineStringElm->GetVertexAt(1);
			dir[1].org = pLineStringElm->GetVertexAt(iVertexCount - 1);
			dir[1].end = pLineStringElm->GetVertexAt(iVertexCount - 2);

			CIsoElement* pArrowElm1 = CIsoElement::GetArrowElementHas(pts[0] , &dir[0] , pArrElmList , dAngleToler);
			CIsoElement* pArrowElm2 = CIsoElement::GetArrowElementHas(pts[1] , &dir[1] , pArrElmList , dAngleToler);
			if( pArrowElm1 && pArrowElm2 )
			{
				oTwoArrowList.push_back(*itr);
			}
			else if( pArrowElm1 || pArrowElm2 )
			{
				vector<DPoint3d> oConnPtList;
				if(pArrowElm1)
				{
					pArrowElm1->GetConnPointList(oConnPtList);
					if(::DistanceBetween(oConnPtList[1] , pts[0]) < ::DistanceBetween(oConnPtList[1] , pts[1]))
					{
						swap(pts[0] , pts[1]);
					}

					if(::DistanceBetween(ptCenter , pts[0]) < ::DistanceBetween(ptCenter , pts[1]))
					{
						oOneArrowList.push_back(*itr);
					}
				}
				else if(pArrowElm2)
				{
					pArrowElm2->GetConnPointList(oConnPtList);
					if(::DistanceBetween(oConnPtList[1] , pts[0]) < ::DistanceBetween(oConnPtList[1] , pts[1]))
					{
						swap(pts[0] , pts[1]);
					}

					if(::DistanceBetween(ptCenter , pts[0]) < ::DistanceBetween(ptCenter , pts[1]))
					{
						oOneArrowList.push_back(*itr);
					}
				}
			}
		}

		return ERROR_SUCCESS;
	}

	return ERROR_BAD_ENVIRONMENT;
}