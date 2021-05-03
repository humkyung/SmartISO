#include "StdAfx.h"
#include <assert.h>
#include <float.h>
#include <DgnLineString.h>
#include "IsoModelDocData.h"
#include "ArrowElement.h"

using namespace IsoElement;

CArrowElement::CArrowElement(void)
{
	m_angle[0] = m_angle[1] = 0.0;
}

CArrowElement::~CArrowElement(void)
{
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-25
    @class      CArrowElement
    @function   angle
    @return     double
    @brief
******************************************************************************/
double CArrowElement::angle() const
{
	DVec3d vec;
	vec.x = m_ptConn[0].x - m_ptConn[1].x;
	vec.y = m_ptConn[0].y - m_ptConn[1].y;
	vec.z = m_ptConn[0].z - m_ptConn[1].z;

	double angle = ::AngleOf(vec);	
	if(angle < 0.0) angle += 2*PI;
	return (RAD2DEG(angle));
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-16
    @class      CArrowElement
    @function   IsLineLocatedInArrow
    @return     bool
    @param      DPoint3d            pts[2]
    @param      vector<DPoint3d>&   oEnclosePolygon
    @brief		check if line locates in arrow shape
******************************************************************************/
bool CArrowElement::DoesLineLocateInArrow(DPoint3d pts[2] , vector<DPoint3d>& oEnclosePolygon)
{
	/// check angle
	const double d1 = ::DistanceBetween((m_ptConn[1]) , (pts[0]));
	const double d2 = ::DistanceBetween((m_ptConn[1]) , (pts[1]));
	DVec3d vec;
	if(d1 < d2)
	{
		vec.x = pts[1].x - pts[0].x;
		vec.y = pts[1].y - pts[0].y;
		vec.z = pts[1].z - pts[0].z;
	}
	else
	{
		vec.x = pts[0].x - pts[1].x;
		vec.y = pts[0].y - pts[1].y;
		vec.z = pts[0].z - pts[1].z;
	}
	double angle = ::AngleOf(vec);
	if(angle < 0.0) angle += 2*PI;
	angle = RAD2DEG(angle);
	if(fabs(m_angle[1] - m_angle[0]) < 180.0)
	{
		if((m_angle[0] - angle) * (m_angle[1] - angle) > 0.0) return false;
	}
	else
	{
		if((m_angle[0] - angle) * (m_angle[1] - angle) < 0.0) return false;
	}
	/// up to here

	/// 3개의 TEST 점을 준비한다.
	DPoint3d ptTest[3]={0,0,0};
	const double dx = pts[1].x - pts[0].x;
	const double dy = pts[1].y - pts[0].y;
	const double dz = pts[1].z - pts[0].z;
	ptTest[0].x = pts[0].x + dx*0.0;
	ptTest[0].y = pts[0].y + dy*0.0;
	ptTest[0].z = pts[0].z + dz*0.0;
	ptTest[1].x = pts[0].x + dx*0.5;
	ptTest[1].y = pts[0].y + dy*0.5;
	ptTest[1].z = pts[0].z + dz*0.5;
	ptTest[2].x = pts[0].x + dx*1.0;
	ptTest[2].y = pts[0].y + dy*1.0;
	ptTest[2].z = pts[0].z + dz*1.0;
	/// up to here

	int i = 0;
	const int iSize = SIZE_OF_ARRAY(ptTest);
	for(i = 0;i < iSize;++i)
	{
		if(false == CIsoElement::IsInnerPt(oEnclosePolygon , ptTest[i])) break;
	}
	
	return (iSize == i);
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-24
    @class      CArrowElement
    @function   IsLocatedInArrow
    @return     bool
    @param      CDgnLineString* pLineStringElm
    @param      vector<DPoint3d>&   oEnclosePolygon
    @brief
******************************************************************************/
bool CArrowElement::IsLocatedInArrow(CDgnLineString* pLineStringElm , vector<DPoint3d>& oEnclosePolygon)
{
	assert(pLineStringElm && "pLineStringElm is NULL");

	if(pLineStringElm)
	{
		DPoint3d pts[2] = {0,0};

		const int iVertexCount = pLineStringElm->GetVertexCount();
		for(int i = 0;i < iVertexCount - 1;++i)
		{
			pts[0] = pLineStringElm->GetVertexAt(i);
			pts[1] = pLineStringElm->GetVertexAt(i + 1);

			if(false == DoesLineLocateInArrow(pts , oEnclosePolygon)) return false;
		}

		return true;
	}

	return false;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-24
    @class      CArrowElement
    @function   Add
    @return     int
    @param      CDgnElement*    p
    @brief
******************************************************************************/
int CArrowElement::Add(CDgnElement* p)
{
	int res = CIsoElement::Add( p );

	if((1 == m_oDgnElementList.size()) && m_oDgnElementList[0]->IsKindOf(CDgnLineString::TypeString()))
	{
		CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(m_oDgnElementList[0]);
				
		double dMaxDist = 0.0;
		DPoint3d ptArrow;
		vector<DPoint3d> oEnclosePolygon = pLineStringElm->GetPointStringExceptSamePoint();
		
		int iIndex = 0;
		for(int i = 0;i < int(oEnclosePolygon.size());++i)
		{
			const double d = ::DistanceBetween((oEnclosePolygon[i]) , (oEnclosePolygon[(i + 1)%3])) + 
				::DistanceBetween((oEnclosePolygon[(i + 2)%3]) , (oEnclosePolygon[(i + 1)%3]));
			if(d > dMaxDist)
			{
				dMaxDist = d;
				ptArrow = oEnclosePolygon[(i + 1)%3];
				
				DVec3d vec;
				vec.x = oEnclosePolygon[i].x - oEnclosePolygon[(i + 1)%3].x;
				vec.y = oEnclosePolygon[i].y - oEnclosePolygon[(i + 1)%3].y;
				vec.z = oEnclosePolygon[i].z - oEnclosePolygon[(i + 1)%3].z;
				double angle = ::AngleOf(vec);
				if(angle < 0.0) angle += 2*PI;
				m_angle[0] = RAD2DEG(angle);
				m_angleRaw[0] = m_angle[0];
				
				vec.x = oEnclosePolygon[(i + 2)%3].x - oEnclosePolygon[(i + 1)%3].x;
				vec.y = oEnclosePolygon[(i + 2)%3].y - oEnclosePolygon[(i + 1)%3].y;
				vec.z = oEnclosePolygon[(i + 2)%3].z - oEnclosePolygon[(i + 1)%3].z;
				angle = ::AngleOf(vec);
				if(angle < 0.0) angle += 2*PI;
				m_angle[1] = RAD2DEG(angle);
				m_angleRaw[1] = m_angle[1];

				iIndex = (i + 1) % 3;
			}
		}

		m_ptConn.clear();
		
		DPoint3d pt;
		pt.x = (oEnclosePolygon[(iIndex + 1)%3].x + oEnclosePolygon[(iIndex + 2)%3].x)*0.5;
		pt.y = (oEnclosePolygon[(iIndex + 1)%3].y + oEnclosePolygon[(iIndex + 2)%3].y)*0.5;
		pt.z = (oEnclosePolygon[(iIndex + 1)%3].z + oEnclosePolygon[(iIndex + 2)%3].z)*0.5;
		m_ptConn.push_back( pt );
		m_ptConn.push_back( ptArrow );

		/// store arrow shape - 2011.11.09 added by humkyung
		m_oShapeList.push_back(oEnclosePolygon[(iIndex + 1)%3]);
		m_oShapeList.push_back(oEnclosePolygon[(iIndex + 2)%3]);
		m_oShapeList.push_back(ptArrow);
		/// up to here
	}

	return res;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-17
    @class      CArrowElement
    @function   GetEnclosePolygon
    @return     vector<DPoint3d>
    @brief
******************************************************************************/
vector<DPoint3d> CArrowElement::GetEnclosePolygon()
{
	if(m_oDgnElementList.size() > 0)
	{
		return m_oShapeList;
	}

	throw exception("bad environment");
}

/******************************************************************************
    @author     humkyung
    @date       ????.??.??
    @class      CArrowElement
    @function   ExpandEnclosePolygon
    @return     int
    @param      vector<DPoint3d>&   oEnclosePolygon
    @param      const               double
    @param      dOffset
    @brief
******************************************************************************/
int CArrowElement::ExpandEnclosePolygon( vector<DPoint3d>& oEnclosePolygon/* , const double dOffset */)
{
	assert(3 == oEnclosePolygon.size());

	const double dOffset = ::DistanceBetween(m_oShapeList[0] , m_oShapeList[1]) * 0.5;

	DPoint3d pts[4] = {0,0,0,0};
	double dx[3] = {0,0} , dy[3] = {0,0} , dz[3] = {0,0};
	for(int i = 0;i < 3;++i)
	{
		dx[i] = oEnclosePolygon[(i+1)%3].x - oEnclosePolygon[i].x;
		dy[i] = oEnclosePolygon[(i+1)%3].y - oEnclosePolygon[i].y;
		dz[i] = oEnclosePolygon[(i+1)%3].z - oEnclosePolygon[i].z;
		const double length = sqrt(dx[i]*dx[i] + dy[i]*dy[i] + dz[i]*dz[i]);
		dx[i] /= length; dy[i] /= length; dz[i] /= length;
	}

	vector<DPoint3d> oExpandedEnclosePolygon;
	for(int i = 0;i < 3;++i)
	{
		DPoint3d pt = oEnclosePolygon[i];
		pt.x = pt.x + (dx[(i+2)%3] - dx[i])*dOffset;
		pt.y = pt.y + (dy[(i+2)%3] - dy[i])*dOffset;
		pt.z = pt.z + (dz[(i+2)%3] - dz[i])*dOffset;
		
		oExpandedEnclosePolygon.push_back( pt );
	}

	if(3 == oExpandedEnclosePolygon.size())
	{
		oEnclosePolygon.clear();
		oEnclosePolygon.insert(oEnclosePolygon.begin() , oExpandedEnclosePolygon.begin() , oExpandedEnclosePolygon.end());
		
		return ERROR_SUCCESS;
	}

	return ERROR_BAD_ENVIRONMENT;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-26
    @class      CArrowElement
    @function   DistanceTo
    @return     double
    @param      const   DPoint3d&
    @param      pt
    @brief		return the distance between pt and arrow's connection point
******************************************************************************/
double CArrowElement::DistanceTo( const DPoint3d& pt ) const
{
	double dMinDistance = DBL_MAX;
	for(vector<DPoint3d>::const_iterator itr = m_ptConn.begin();itr != m_ptConn.end();++itr)
	{
		const double dDist = ::DistanceBetween((*itr) , pt);
		if(dDist < dMinDistance)
		{
			dMinDistance = dDist;
		}
	}

	return dMinDistance;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-26
    @class      CArrowElement
    @function   Collinear
    @return     bool
    @param      const   double&
    @param      angle
    @brief		같은 angle을 가지는지 검사한다.
******************************************************************************/
///NOTICE: 고려해 볼것...
bool CArrowElement::IsSameAngle( const double& angle ) const
{
	return (fabs(angle - this->angle()) < 1.5);
}

void CArrowElement::Check(CDgnDocument* pDgnDoc , CString sColor)
{
	UInt32 uiColor = ATOI_T(sColor);
	for(vector<CDgnElement*>::iterator itr = m_oDgnElementList.begin();itr != m_oDgnElementList.end();++itr)
	{
		(*itr)->SetColor(uiColor);
	}

	{
		vector<DPoint3d> pts;
		DPoint3d pt = m_ptConn[0];
		pt.x += 0.01;
		pt.y += 0.01;
		pts.push_back( pt );

		pt = m_ptConn[0];
		pt.x -= 0.01;
		pt.y -= 0.01;
		pts.push_back( pt );
		
		pts.push_back( m_ptConn[0] );

		pt = m_ptConn[0];
		pt.x -= 0.01;
		pt.y += 0.01;
		pts.push_back( pt );

		pt = m_ptConn[0];
		pt.x += 0.01;
		pt.y -= 0.01;
		pts.push_back( pt );

		::DrawPolyline( pDgnDoc , pts , sColor);
	}

	CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
	const double dToler = CIsoModelDocData::m_dDrawingToler;
	vector<DPoint3d> oEnclosePolygon = GetEnclosePolygon();
	ExpandEnclosePolygon(oEnclosePolygon/* , dToler*/);
	oEnclosePolygon.push_back( oEnclosePolygon[0] );
	::DrawPolyline( pDgnDoc , oEnclosePolygon , sColor );
}


