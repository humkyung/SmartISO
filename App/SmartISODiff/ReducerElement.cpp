#include "StdAfx.h"
#include <assert.h>
#include <float.h>
#include <algorithm>
#include "ReducerElement.h"

using namespace IsoElement;

CReducerElement::CReducerElement(void)
{
	m_sTypeString = CReducerElement::TypeString();
}

CReducerElement::~CReducerElement(void)
{
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-31
    @class      CReducerElement
    @function   TypeString
    @return     CString
    @brief		return type string of reducer
******************************************************************************/
STRING_T CReducerElement::TypeString()
{
	return _T("reducer");
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-31
    @class      CReducerElement
    @function   CollectElements
    @return     int
    @param      vector<CDgnElement*>*   pDgnElmList
    @param      vector<CIsoElement*>*   pWeldMarkElmList
    @param      const                   DPoint3d&
    @param      ptConn
    @brief
******************************************************************************/
int CReducerElement::CollectElements( vector<CDgnElement*>* pDgnElmList , vector<CIsoElement*>* pWeldMarkElmList , const DPoint3d ptConn[2] )
{
	assert(pDgnElmList && pWeldMarkElmList && "pDgnElmList or pWeldMarkElmList is NULL");

	if(pDgnElmList && !pDgnElmList->empty() && pWeldMarkElmList)
	{
		vector<CDgnElement*> oDgnElmList;
		for(vector<CDgnElement*>::iterator itr = pDgnElmList->begin();itr != pDgnElmList->end();++itr)
		{
			if((*itr)->IsKindOf(CDgnLineString::TypeString()))
			{
				oDgnElmList.push_back( *itr );
			}
		}

		CDgnElement* pNearestDgnElm = NULL;

		DVector3d l;
		l.org = ptConn[0];
		l.end = ptConn[1];
		double dMinDist = DBL_MAX;
		for(vector<CDgnElement*>::iterator itr = oDgnElmList.begin();itr != oDgnElmList.end();++itr)
		{
			DPoint3d intersect;
			CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(*itr);
			if(/*(2 == pLineStringElm->GetVertexCount()) && */(SUCCESS == pLineStringElm->IntersectWith(intersect , &l)))
			{
				const double d = ::DistanceBetween(intersect , ptConn[1]);
				if(d < dMinDist)
				{
					dMinDist = d;
					pNearestDgnElm = *itr;
				}
			}
		}

		if(NULL != pNearestDgnElm)
		{
			CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(pNearestDgnElm);
			if(!pLineStringElm->IsClosed())
			{
				DPoint3d ptNextConn = pLineStringElm->GetVertexAt(0);
				while(m_oDgnElementList.end() == find(m_oDgnElementList.begin() , m_oDgnElementList.end() , pLineStringElm))
				{
					m_oDgnElementList.push_back( pLineStringElm );
					pDgnElmList->erase( find(pDgnElmList->begin() , pDgnElmList->end() , pLineStringElm) );
					oDgnElmList.erase( find(oDgnElmList.begin() , oDgnElmList.end() , pLineStringElm) );
					
					stable_sort(oDgnElmList.begin() , oDgnElmList.end() , CIsoElement::SortDgnElmByDistance( ptNextConn ) );
					pLineStringElm = static_cast<CDgnLineString*>((oDgnElmList)[0]);
					const int iVertexCount = pLineStringElm->GetVertexCount();
					if(CIsoElement::IsSamePoint(ptNextConn , pLineStringElm->GetVertexAt(0)))
					{
						ptNextConn = pLineStringElm->GetVertexAt(iVertexCount - 1);
					}
					else if(CIsoElement::IsSamePoint(ptNextConn , pLineStringElm->GetVertexAt(iVertexCount - 1)))
					{
						ptNextConn = pLineStringElm->GetVertexAt(0);
					}
					else
					{
						break;
					}
				}
			}
			else
			{
				m_oDgnElementList.push_back( pNearestDgnElm );
				pDgnElmList->erase( find(pDgnElmList->begin() , pDgnElmList->end() , pLineStringElm) );
			}
		}

		CDgnVolume _volume = volume();
		for(vector<CIsoElement*>::iterator itr = pWeldMarkElmList->begin();itr != pWeldMarkElmList->end();++itr)
		{
			if(_volume.CollideWith( (*itr)->volume() ))
			{
				m_ptConn.push_back( (*itr)->center() );
			}
		}

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

void CReducerElement::Check(CDgnDocument* pDgnDoc , CString sColor)
{
	UInt32 uiColor = ATOI_T(sColor);
	for(vector<CDgnElement*>::iterator itr = m_oDgnElementList.begin();itr != m_oDgnElementList.end();++itr)
	{
		(*itr)->SetColor( uiColor );
	}

	if(m_ptConn.size() > 1)
	{
		vector<DPoint3d> pts;
		DPoint3d pt = m_ptConn[0];
		pts.push_back( pt );

		pt = m_ptConn[1];
		pts.push_back( pt );
		
		::DrawPolyline( pDgnDoc , pts , sColor);
	}
}