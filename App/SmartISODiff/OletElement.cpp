#include "StdAfx.h"
#include <assert.h>
#include <algorithm>
#include <DgnLineString.h>
#include "WeldMarkElement.h"
#include "OletElement.h"

using namespace IsoElement;
COletElement::COletElement(void) : m_pBranch(NULL)
{
	m_sTypeString = COletElement::TypeString();
}

COletElement::~COletElement(void)
{
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-10
    @class      COletElement
    @function   TypeString
    @return     CString
    @brief		return type string of olet
******************************************************************************/
STRING_T COletElement::TypeString()
{
	return _T("olet");
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-14
    @class      COletElement
    @function   CollectElements
    @return     int
    @param      vector<CDgnElement*>*   pDgnElmList
    @param      const                   DPoint3d&
    @param      ptConn
    @brief		olet을 구성하는 3개의 point를 구한다.
******************************************************************************/
int COletElement::CollectElements( vector<CDgnElement*>* pDgnElmList , vector<CIsoElement*>* pWeldMarkElmList , const DPoint3d& ptConn )
{
	assert(pDgnElmList && pWeldMarkElmList && "pDgnElmList or pWeldMarkElmList is NULL");

	if(pDgnElmList && pWeldMarkElmList)
	{
		/// fine a weld mark contains the connection point
		CWeldMarkElement* pWeldMarkElm = NULL;
		for(vector<CIsoElement*>::iterator itr = pWeldMarkElmList->begin();itr != pWeldMarkElmList->end();++itr)
		{
			if((*itr)->volume().Contains( ptConn ))
			{
				pWeldMarkElm = static_cast<CWeldMarkElement*>(*itr);
				break;
			}
		}
		/// up to here

		CDgnElement* pFirstDgnElm = NULL;
		if(NULL != pWeldMarkElm)
		{
			CDgnVolume volume = pWeldMarkElm->volume();
			for(vector<CDgnElement*>::iterator itr = pDgnElmList->begin();itr != pDgnElmList->end();++itr)
			{
				CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(*itr);
				const int iVertexCount = pLineStringElm->GetVertexCount();
				
				DPoint3d pts[2]={0,0};
				pts[0] = pLineStringElm->GetVertexAt(0);
				pts[1] = pLineStringElm->GetVertexAt(iVertexCount - 1);
				if(volume.Contains(pts[0]) || volume.Contains(pts[1]))
				{
					pFirstDgnElm = (*itr);

					const double d1 = ::DistanceBetween(pts[0] , ptConn);
					const double d2 = ::DistanceBetween(pts[1] , ptConn);
					(d1 < d2) ? m_ptConn.push_back( pts[0] ) : m_ptConn.push_back( pts[1] );
					break;
				}
			}

			/// couldn't find pipe elment connected to weld mark
			if(m_ptConn.empty()) m_ptConn.push_back( volume.center() );
		}
		else
		{
			m_ptConn.push_back( ptConn );
		}

		if(1 != m_ptConn.size()) throw exception("unexpected condition");
		if(1 == m_ptConn.size())
		{
			vector<CDgnElement*> oDgnElmList;
			oDgnElmList.insert(oDgnElmList.begin() , pDgnElmList->begin() , pDgnElmList->end());
			if(NULL != pFirstDgnElm) oDgnElmList.erase( find(oDgnElmList.begin() , oDgnElmList.end() , pFirstDgnElm) );
			stable_sort( oDgnElmList.begin() , oDgnElmList.end() , CIsoElement::SortDgnElmByDistance( m_ptConn[0] ) );
			if(oDgnElmList.size() >= 2)
			{
				vector<DPoint3d> pts;
				CDgnLineString* pLineStringElm1 = static_cast<CDgnLineString*>( oDgnElmList[0] );
				int iVertexCount = pLineStringElm1->GetVertexCount();
				DPoint3d pt = pLineStringElm1->GetVertexAt( 0 );
				pts.push_back( pt );
				pt = pLineStringElm1->GetVertexAt( iVertexCount - 1 );
				pts.push_back( pt );
				CDgnLineString* pLineStringElm2 = static_cast<CDgnLineString*>( oDgnElmList[1] );
				iVertexCount = pLineStringElm2->GetVertexCount();
				pt = pLineStringElm2->GetVertexAt( 0 );
				pts.push_back( pt );
				pt = pLineStringElm2->GetVertexAt( iVertexCount - 1 );
				pts.push_back( pt );

				stable_sort(pts.begin() , pts.end() , CIsoElement::SortDPoint3dFrom( m_ptConn[0] ));
				m_ptConn.insert(m_ptConn.begin() , pts[0] );
				
				return ERROR_SUCCESS;
			}
		}

		return ERROR_BAD_ENVIRONMENT;
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-11
    @class      COletElement
    @function   GetConnPointList
    @return     int
    @param      vector<DPoint3d>&   pts
    @brief
******************************************************************************/
int COletElement::GetConnPointList(vector<DPoint3d>& pts)
{
	if(2 == m_ptConn.size())
	{
		pts.clear();
		pts.push_back( m_ptConn[0] );
		/// 2nd point is start point of branch...
	}
	else
	{
		throw exception("invalid number of conntion point!!!");
	}
	
	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-11
    @class      COletElement
    @function   GetStartPointOfBranch
    @return     DPoint3d
    @brief		return start point of branch
******************************************************************************/
DPoint3d COletElement::GetStartPointOfBranch()
{
	DPoint3d res = {0};
	if(2 == m_ptConn.size())
	{
		res = m_ptConn[1];
	}
	else
	{
		throw exception("invalid number of conntion point!!!");
	}
	
	return res;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-11
    @class      COletElement
    @function   SetBranch
    @return     int
    @param      CPipeRoutine*   pBranch
    @brief
******************************************************************************/
int COletElement::SetBranch( CPipeRoutine* pBranch )
{
	assert(pBranch && "pBranch is NULL");

	if(pBranch)
	{
		SAFE_DELETE( m_pBranch );

		m_pBranch = pBranch;

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-11
    @class      COletElement
    @function   GetBranch
    @return     CPipeRoutine*
    @brief
******************************************************************************/
CPipeRoutine* COletElement::GetBranch() const
{
	return m_pBranch;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-27
    @class      COletElement
    @function   center
    @return     DPoint3d
    @brief		return center point of olet
******************************************************************************/
DPoint3d COletElement::center() const
{
	return m_ptConn[0];
}

void COletElement::Check(CDgnDocument* pDgnDoc , CString sColor)
{
	vector<DPoint3d> pts;
	DPoint3d pt = m_ptConn[1];
	pt.x += 0.1;
	pt.y += 0.1;
	pts.push_back( pt );

	pt = m_ptConn[1];
	pt.x -= 0.1;
	pt.y -= 0.1;
	pts.push_back( pt );
	
	pts.push_back( m_ptConn[1] );

	pt = m_ptConn[1];
	pt.x -= 0.1;
	pt.y += 0.1;
	pts.push_back( pt );

	pt = m_ptConn[1];
	pt.x += 0.1;
	pt.y -= 0.1;
	pts.push_back( pt );

	::DrawPolyline( pDgnDoc , pts , sColor);
}