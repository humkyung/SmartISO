#include "StdAfx.h"
#include <assert.h>
#include <algorithm>
#include <float.h>
#include <DgnLineString.h>
#include "IsoModelDocData.h"
#include "DimLineElement.h"

using namespace IsoElement;

CDimLineElement::CDimLineElement()
{
}

CDimLineElement::~CDimLineElement(void)
{
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-26
    @class      CDimLineElement
    @function   CollectArrows
    @return     int
    @param      vector<CIsoElement*>*   pArrElmList
    @brief		collect arrows
******************************************************************************/
int CDimLineElement::CollectArrows( vector<CIsoElement*>* pArrElmList )
{
	assert(pArrElmList && "pArrElmList is NULL");

	if(pArrElmList)
	{
		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
		const double dAngleToler = docData.GetAngleToler();

		DPoint3d pts[2] = {0,0};
		for(vector<CDgnElement*>::iterator itr = m_oDgnElementList.begin();itr != m_oDgnElementList.end();++itr)
		{
			CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>( *itr );
			const int iVertexCount = pLineStringElm->GetVertexCount();
			pts[0] = pLineStringElm->GetVertexAt(0);
			pts[1] = pLineStringElm->GetVertexAt(iVertexCount - 1);
			DVector3d dir;
			dir.org = pts[0];
			dir.end = pts[1];

			CIsoElement* pArrowElm = CIsoElement::GetArrowElementHas(pts[0] , &dir , pArrElmList , dAngleToler);
			if(NULL != pArrowElm)
			{
				m_oArrElmList.push_back( static_cast<CArrowElement*>(pArrowElm) );
				pArrElmList->erase( find(pArrElmList->begin() , pArrElmList->end() , pArrowElm) );
			}

			dir.org = pts[1];
			dir.end = pts[0];
			pArrowElm = CIsoElement::GetArrowElementHas(pts[1] , &dir , pArrElmList , dAngleToler);
			if(NULL != pArrowElm)
			{
				m_oArrElmList.push_back( static_cast<CArrowElement*>(pArrowElm) );
				pArrElmList->erase( find(pArrElmList->begin() , pArrElmList->end() , pArrowElm) );
			}
		}

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-29
    @class      CDimLineElement
    @function   GetArrowCount
    @return     int
    @brief		return arrow count
******************************************************************************/
int CDimLineElement::GetArrowCount() const
{
	return m_oArrElmList.size();
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-29
    @class      CDimLineElement
    @function   CollectElements4Type1
    @return     int
    @param      vector<CDgnElement*>*   pDgnElmList
    @param      vector<CIsoElement*>*   pArrElmList
    @param      CLeaderElement*         pLeaderElm
    @brief
******************************************************************************/
int CDimLineElement::CollectElements4Type1( vector<CDgnElement*>* pDgnElmList , vector<CIsoElement*>* pArrElmList , CLeaderElement* pLeaderElm)
{
	assert(pDgnElmList && pArrElmList && pLeaderElm && "param is NULL");

	CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
	const double dToler = docData.m_dDrawingToler;
	const double dAngleToler = docData.GetAngleToler();

	if(pDgnElmList && pArrElmList && pLeaderElm)
	{
		/// connection point를 구한다.
		DPoint3d ptConnection;
		DVector3d l;
		{
			vector<DPoint3d> oConnPtList;
			pLeaderElm->GetConnPointList(oConnPtList);
			ptConnection = oConnPtList[ oConnPtList.size() - 1 ];

			const int iElmSize = pLeaderElm->m_oDgnElementList.size();
			CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(pLeaderElm->m_oDgnElementList[iElmSize - 1]);
			const int iVertexCount = pLineStringElm->GetVertexCount();
			l.org = pLineStringElm->GetVertexAt(iVertexCount - 2);
			l.end = pLineStringElm->GetVertexAt(iVertexCount - 1);
		}
		/// up to here

		/// connection point와 가장 가까운 line string을 찾는다.
		CDgnElement* pDimLineElm = NULL;
		double dMinDist = DBL_MAX;
		for(vector<CDgnElement*>::iterator itr = pDgnElmList->begin();itr != pDgnElmList->end();++itr)
		{
			if(!(*itr)->IsKindOf(CDgnLineString::TypeString())) continue;

			CDgnLineString* _pLineStringElm = static_cast<CDgnLineString*>(*itr);
			if(2 == _pLineStringElm->GetVertexCount())
			{
				DPoint3d intersect;
				if(SUCCESS == _pLineStringElm->IntersectWith(intersect , &l , dToler))
				{
					const double d = ::DistanceBetween(ptConnection , intersect);
					if(d < dMinDist)
					{
						dMinDist = d;
						pDimLineElm = _pLineStringElm;
					}
				}
			}
		}

		/// check again with perpendicular line
		if((NULL != pDimLineElm) && (dMinDist > dToler))
		{
			CDgnLineString* _pLineStringElm = static_cast<CDgnLineString*>(pDimLineElm);
			pDimLineElm = NULL;

			double dx = l.end.x - l.org.x;
			double dy = l.end.y - l.org.y;
			l.org = ptConnection;
			/// rotate by 90 degree
			l.end.x = l.org.x - dy;
			l.end.y = l.org.y + dx;

			DPoint3d intersect;
			if(SUCCESS == _pLineStringElm->IntersectWith(intersect , &l , dToler))
			{
				const double d = ::DistanceBetween(ptConnection , intersect);
				if((d < dToler) && (d < dMinDist))
				{
					dMinDist = d;
					pDimLineElm = _pLineStringElm;
				}
			}
		}
		/// up to here

		if(NULL != pDimLineElm)
		{
			this->Add( pDimLineElm );
			pDgnElmList->erase( find(pDgnElmList->begin() , pDgnElmList->end() , pDimLineElm) );

			this->CollectArrows( pArrElmList );
		}

		/// try to collect part of dim. line - 2011.11.29 added by humkyung
		if((1 == this->GetArrowCount()) && (1 == m_oDgnElementList.size()))
		{
			DPoint3d lhs[2]={0,0} , rhs[2]={0,0};

			CDgnLineString* pLineStringElm1 = static_cast<CDgnLineString*>(m_oDgnElementList[0]);
			int iVertexCount = pLineStringElm1->GetVertexCount();
			lhs[0] = pLineStringElm1->GetVertexAt(0);
			lhs[1] = pLineStringElm1->GetVertexAt(iVertexCount - 1);

			pDimLineElm = NULL;
			double dMinDist = DBL_MAX;
			for(vector<CDgnElement*>::iterator itr = pDgnElmList->begin();itr != pDgnElmList->end();++itr)
			{
				if(!(*itr)->IsKindOf(CDgnLineString::TypeString())) continue;
					
				CDgnLineString* pLineStringElm2 = static_cast<CDgnLineString*>(*itr);
				int iVertexCount = pLineStringElm2->GetVertexCount();
				if(2 == iVertexCount)
				{
					rhs[0] = pLineStringElm2->GetVertexAt(0);
					rhs[1] = pLineStringElm2->GetVertexAt(iVertexCount - 1);

					if(true == CIsoElement::Collinear(lhs , rhs , dAngleToler))
					{
						const double d = min(pLineStringElm2->DistanceTo(lhs[0]) , pLineStringElm2->DistanceTo(lhs[1]));
						if((d < dToler) && (d < dMinDist))
						{
							dMinDist = d;
							pDimLineElm = pLineStringElm2;
						}
					}
				}
			}

			if(NULL != pDimLineElm)
			{
				this->Add( pDimLineElm );
				pDgnElmList->erase( find(pDgnElmList->begin() , pDgnElmList->end() , pDimLineElm) );

				this->CollectArrows( pArrElmList );
			}
		}
		/// up to here

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-29
    @class      CDimLineElement
    @function   CollectElements
    @return     int
    @param      vector<CDgnElement*>*   pDgnElmList
    @param      DVector3d&              l
    @param      const                   int&
    @param      iDimType
    @brief
******************************************************************************/
int CDimLineElement::CollectElements( vector<CDgnElement*>* pDgnElmList , vector<CIsoElement*>* pArrElmList , CLeaderElement* pLeaderElm , const int& iDimType )
{
	CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
	const double dToler = docData.m_dDrawingToler;

	if(1 == iDimType)
	{
		CollectElements4Type1(pDgnElmList , pArrElmList , pLeaderElm);
	}

	return ERROR_SUCCESS;
}

void CDimLineElement::Check(CDgnDocument* pDgnDoc , CString sColor)
{
	CIsoElement::Check( pDgnDoc , sColor );

	for(vector<CArrowElement*>::iterator itr = m_oArrElmList.begin();itr != m_oArrElmList.end();++itr)
	{
		(*itr)->Check(pDgnDoc , sColor);
	}
}