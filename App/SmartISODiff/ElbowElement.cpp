#include "StdAfx.h"
#include <assert.h>
#include <algorithm>
#include <float.h>
#include <DgnLineString.h>
#include "IsoModelDocData.h"
#include "ElbowElement.h"

using namespace IsoElement;

CElbowElement::CElbowElement(void)
{
	m_sTypeString = CElbowElement::TypeString();
	m_iType = 1;
}

CElbowElement::~CElbowElement(void)
{
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-10
    @class      CElbowElement
    @function   TypeString
    @return     CString
    @brief		return type string of elbow
******************************************************************************/
STRING_T CElbowElement::TypeString()
{
	return _T("elbow");
}

/******************************************************************************
    @author     humkyung
    @date       2011-12-01
    @class      CElbowElement
    @function   CheckElbowType
    @return     int
    @param      vector<CIsoElement*>*   pWeldMarkElmList
    @brief		check elbow type(default type is "1")
******************************************************************************/
int CElbowElement::CheckElbowType(vector<CIsoElement*>* pWeldMarkElmList)
{
	assert(pWeldMarkElmList && "pWeldMarkElmList is NULL");

	if(pWeldMarkElmList)
	{
		CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(m_oDgnElementList[0]);
		const int iVertexCount = pLineStringElm->GetVertexCount();
		DPoint3d ptStart = pLineStringElm->GetVertexAt(0);
		DPoint3d ptEnd = pLineStringElm->GetVertexAt(iVertexCount - 1);

		for(vector<CIsoElement*>::iterator itr = pWeldMarkElmList->begin();itr != pWeldMarkElmList->end();++itr)
		{
			CDgnVolume volume = (*itr)->volume();
			if(volume.Contains(ptStart) || volume.Contains(ptEnd))
			{
				return 2;
			}
		}
	}

	return 1;	/// defalut value
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-27
    @class      CElbowElement
    @function   ChooseElement4Type2
    @return     int
    @param      DPoint3d*   pPoint1
    @param      DPoint3d*   pPoint2
    @brief		choose part of elbow elements
******************************************************************************/
int CElbowElement::ChooseElement4Type2(DPoint3d* pPoint1 , DPoint3d* pPoint2)
{
	assert(pPoint1 && pPoint2 && "pPoint1 or pPoint2 is NULL");

	if(pPoint1 && pPoint2)
	{
		DVec3d vec[2];
		vec[0].x = pPoint1[1].x - pPoint1[0].x;
		vec[0].y = pPoint1[1].y - pPoint1[0].y;
		vec[0].z = pPoint1[1].z - pPoint1[0].z;
		vec[1].x = pPoint2[1].x - pPoint2[0].x;
		vec[1].y = pPoint2[1].y - pPoint2[0].y;
		vec[1].z = pPoint2[1].z - pPoint2[0].z;

		const double rad = ::AngleBetweenVectors(vec[0] , vec[1]);
		const double angle = RAD2DEG(rad);
		if(!((angle < 1.5) || (fabs(180 - angle) < 1.5)))	///NOTICE: user angle tolerance!!!
		{
			double dMinDist = DBL_MAX;
			int iIndices[2] = {0,0};
			for(int i = 0;i < 2;++i)
			{
				for(int j = 0;j < 2;++j)
				{
					const double d = ::DistanceBetween(pPoint1[i] , pPoint2[j]);
					if(d < dMinDist)
					{
						dMinDist = d;
						iIndices[0] = i;
						iIndices[1] = j;
					}
				}
			}

			m_ptConn[0] = pPoint1[ (iIndices[0] + 1)%2 ];
			m_ptConn[1] = pPoint2[ (iIndices[1] + 1)%2 ];

			return ERROR_SUCCESS;
		}

		return ERROR_BAD_ENVIRONMENT;
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-14
    @class      CElbowElement
    @function   CollectElements
    @return     int
    @param      vector<CDgnElement*>*   pDgnElmList
    @brief
******************************************************************************/
int CElbowElement::CollectElements( vector<CDgnElement*>* pDgnElmList , vector<CIsoElement*>* pWeldMarkElmList )
{
	assert(pDgnElmList && "pDgnElmList is NULL");

	if(pDgnElmList && (1 == m_oDgnElementList.size()))
	{
		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
		const double dToler[2] = {CIsoModelDocData::m_dDrawingToler , CIsoModelDocData::m_dDrawingToler*5};

		m_iType = this->CheckElbowType(pWeldMarkElmList);

		CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(m_oDgnElementList[0]);
		int iVertexCount = pLineStringElm->GetVertexCount();

		DPoint3d pts[2] = {0,0};
		pts[0] = pLineStringElm->GetVertexAt(0);
		pts[1] = pLineStringElm->GetVertexAt(iVertexCount - 1);

		vector<CDgnElement*> oTempElmList;
		oTempElmList.insert( oTempElmList.begin() , pDgnElmList->begin() , pDgnElmList->end() );
		stable_sort( oTempElmList.begin() , oTempElmList.end() , CIsoElement::SortDgnElmByDistance( pts[0] ));
		for(int i = 0;i < SIZE_OF_ARRAY(dToler);++i)
		{
			if(!oTempElmList.empty())
			{
				pLineStringElm = static_cast<CDgnLineString*>(oTempElmList[0]);
				iVertexCount = pLineStringElm->GetVertexCount();
				DPoint3d pts_[2] = {0,0};
				pts_[0] = pLineStringElm->GetVertexAt(0);
				pts_[1] = pLineStringElm->GetVertexAt(iVertexCount - 1);
				
				double dist[2] = {0,0};
				dist[0] = ::DistanceBetween(pts_[0] , pts[0]);
				dist[1] = ::DistanceBetween(pts_[1] , pts[0]);
				if((dist[0] < dToler[i]) || (dist[1] < dToler[i]))
				{
					if(1 == m_iType)
					{
						(dist[0] < dist[1]) ? (m_ptConn[0] = pts_[1]) : (m_ptConn[0] = pts_[0]);
						m_oDgnElementList.insert( m_oDgnElementList.begin() , pLineStringElm );

						oTempElmList.erase( find(oTempElmList.begin() , oTempElmList.end() , pLineStringElm) );
						pDgnElmList->erase( find(pDgnElmList->begin() , pDgnElmList->end() , pLineStringElm) );
					}
					else if(2 == m_iType)
					{
						if(ERROR_SUCCESS == ChooseElement4Type2(pts , pts_))
						{
							m_oDgnElementList.insert( m_oDgnElementList.begin() , pLineStringElm );

							oTempElmList.erase( find(oTempElmList.begin() , oTempElmList.end() , pLineStringElm) );
							pDgnElmList->erase( find(pDgnElmList->begin() , pDgnElmList->end() , pLineStringElm) );
						}
					}
					break;
				}
			}
		}

		stable_sort( oTempElmList.begin() , oTempElmList.end() , CIsoElement::SortDgnElmByDistance( pts[1] ));
		for(int i = 0;i < SIZE_OF_ARRAY(dToler);++i)
		{
			if(!oTempElmList.empty())
			{
				pLineStringElm = static_cast<CDgnLineString*>(oTempElmList[0]);
				iVertexCount = pLineStringElm->GetVertexCount();
				DPoint3d pts_[2] = {0,0};
				pts_[0] = pLineStringElm->GetVertexAt(0);
				pts_[1] = pLineStringElm->GetVertexAt(iVertexCount - 1);
				
				double dist[2] = {0,0};
				dist[0] = ::DistanceBetween(pts_[0] , pts[1]);
				dist[1] = ::DistanceBetween(pts_[1] , pts[1]);
				if((dist[0] < dToler[i]) || (dist[1] < dToler[i]))
				{
					if(1 == m_iType)
					{
						(dist[0] < dist[1]) ? (m_ptConn[1] = pts_[1]) : (m_ptConn[1] = pts_[0]);
						m_oDgnElementList.push_back( pLineStringElm );

						oTempElmList.erase( find(oTempElmList.begin() , oTempElmList.end() , pLineStringElm) );
						pDgnElmList->erase( find(pDgnElmList->begin() , pDgnElmList->end() , pLineStringElm) );
					}
					else if(2 == m_iType)
					{
						if(ERROR_SUCCESS == ChooseElement4Type2(pts , pts_))
						{
							m_oDgnElementList.insert( m_oDgnElementList.begin() , pLineStringElm );

							oTempElmList.erase( find(oTempElmList.begin() , oTempElmList.end() , pLineStringElm) );
							pDgnElmList->erase( find(pDgnElmList->begin() , pDgnElmList->end() , pLineStringElm) );
						}
					}
				}
			}
		}

		/// remove connected weld mark - 2011.11.18 added by humkyung
		if(NULL != pWeldMarkElmList)
		{
			for(vector<DPoint3d>::iterator jtr = m_ptConn.begin();jtr != m_ptConn.end();++jtr)
			{
				for(vector<CIsoElement*>::iterator itr = pWeldMarkElmList->begin();itr != pWeldMarkElmList->end();/*++itr*/)
				{
					if((*itr)->volume().Contains( *jtr ))
					{
						itr = pWeldMarkElmList->erase( itr );
						continue;
					}
					++itr;
				}
			}
		}
		/// up to here

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-27
    @class      CElbowElement
    @function   center
    @return     DPoint3d
    @brief		return center point of elbow
******************************************************************************/
DPoint3d CElbowElement::center() const
{
	DPoint3d intersect;
	intersect.x = intersect.y = intersect.z = 0.0;

	const int iSize = m_oDgnElementList.size();
	if(m_oDgnElementList[iSize - 1]->IsKindOf(CDgnLineString::TypeString()))
	{
		CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(m_oDgnElementList[iSize - 1]);
		const int iVertexCount = pLineStringElm->GetVertexCount();
		DVector3d l;
		l.org = pLineStringElm->GetVertexAt(0);
		l.end = pLineStringElm->GetVertexAt(iVertexCount - 1);

		m_oDgnElementList[0]->IntersectWith(intersect , &l , DBL_MAX);
	}

	return intersect;
}

void CElbowElement::Check(CDgnDocument* pDgnDoc , CString sColor)
{
	UInt32 uiColor = ATOI_T(sColor);
	for(vector<CDgnElement*>::iterator itr = m_oDgnElementList.begin();itr != m_oDgnElementList.end();++itr)
	{
		(*itr)->SetColor( uiColor );
	}

	{
		CDgnVolume volume;
		DPoint3d pt = m_ptConn[0];
		pt.x += 0.05;
		pt.y += 0.05;
		volume.Add( pt );

		pt = m_ptConn[0];
		pt.x -= 0.05;
		pt.y -= 0.05;
		volume.Add( pt );

		vector<DPoint3d> oPolyline;
		pt.x = volume.minx();
		pt.y = volume.miny();
		pt.z = volume.minz();
		oPolyline.push_back( pt );

		pt.x = volume.maxx();
		pt.y = volume.miny();
		pt.z = volume.minz();
		oPolyline.push_back( pt );

		pt.x = volume.maxx();
		pt.y = volume.maxy();
		pt.z = volume.minz();
		oPolyline.push_back( pt );

		pt.x = volume.minx();
		pt.y = volume.maxy();
		pt.z = volume.minz();
		oPolyline.push_back( pt );

		pt.x = volume.minx();
		pt.y = volume.miny();
		pt.z = volume.minz();
		oPolyline.push_back( pt );

		::DrawPolyline(pDgnDoc , oPolyline , sColor);
	}

	{
		CDgnVolume volume;
		DPoint3d pt = m_ptConn[1];
		pt.x += 0.05;
		pt.y += 0.05;
		volume.Add( pt );

		pt = m_ptConn[1];
		pt.x -= 0.05;
		pt.y -= 0.05;
		volume.Add( pt );

		vector<DPoint3d> oPolyline;
		pt.x = volume.minx();
		pt.y = volume.miny();
		pt.z = volume.minz();
		oPolyline.push_back( pt );

		pt.x = volume.maxx();
		pt.y = volume.miny();
		pt.z = volume.minz();
		oPolyline.push_back( pt );

		pt.x = volume.maxx();
		pt.y = volume.maxy();
		pt.z = volume.minz();
		oPolyline.push_back( pt );

		pt.x = volume.minx();
		pt.y = volume.maxy();
		pt.z = volume.minz();
		oPolyline.push_back( pt );

		pt.x = volume.minx();
		pt.y = volume.miny();
		pt.z = volume.minz();
		oPolyline.push_back( pt );

		::DrawPolyline(pDgnDoc , oPolyline, sColor);
	}

	{
		vector<DPoint3d> pts;
		
		DPoint3d _center = center();
		DPoint3d pt = _center;
		pt.x += 0.01;
		pt.y += 0.01;
		pts.push_back( pt );

		pt = _center;
		pt.x -= 0.01;
		pt.y -= 0.01;
		pts.push_back( pt );

		pts.push_back( _center );

		pt = _center;
		pt.x -= 0.01;
		pt.y += 0.01;
		pts.push_back( pt );

		pt = _center;
		pt.x += 0.01;
		pt.y -= 0.01;
		pts.push_back( pt );

		::DrawPolyline( pDgnDoc , pts , sColor);
	}
}