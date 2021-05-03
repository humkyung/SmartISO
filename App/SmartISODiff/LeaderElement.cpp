#include "StdAfx.h"
#include <assert.h>
#include <algorithm>
#include <DgnLineString.h>
#include "LeaderElement.h"
#include "IsoModelDocData.h"

using namespace IsoElement;

CLeaderElement::CLeaderElement() : m_pArrowElement(NULL)
{
}

CLeaderElement::~CLeaderElement(void)
{
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-12
    @class      CLeaderElement
    @function   ClearConnPointList
    @return     int
    @brief		clear connection point list
******************************************************************************/
int CLeaderElement::ClearConnPointList( )
{
	m_ptConn.clear();
	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-12
    @class      CLeaderElement
    @function   AddConnPoint
    @return     int
    @param      DPoint3d    pt
    @brief		add connection point
******************************************************************************/
int CLeaderElement::AddConnPoint( DPoint3d pt )
{
	m_ptConn.push_back( pt );
	return ERROR_SUCCESS;
}

CArrowElement* CLeaderElement::GetArrowElement()
{
	return m_pArrowElement;
}

/******************************************************************************
    @author     humkyung
    @date       ????.??.??
    @class      CLeaderElement
    @function   GetAngle
    @return     double
    @param      DVec3d* vec1
    @param      DVec3d* vec2
    @brief
******************************************************************************/
double CLeaderElement::GetAngle(DVec3d* vec1 , DVec3d* vec2)
{
	const double rad = ::AngleBetweenVectors(*vec1 , *vec2);
	return (rad*180.0 / PI);
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-17
    @class      CLeaderElement
    @function   Add
    @return     int
    @param      CDgnElement*    p
    @brief		add a element to leader element
******************************************************************************/
int CLeaderElement::Add(CDgnElement* p , DPoint3d ptConn)
{
	assert(p && "p is NULL");

	if(p)
	{
		if(m_oDgnElementList.empty())
		{
			CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(p);
			{
				vector<DPoint3d> oVertexList = pLineStringElm->GetLineStringPointList();
				const double d1 = ::DistanceBetween(ptConn , oVertexList[0]);
				const double d2 = ::DistanceBetween(ptConn , oVertexList[1]);
				if(d1 > d2)
				{
					std::reverse(oVertexList.begin() , oVertexList.end());
				}
				
				m_ptConn.clear();
				m_ptConn.insert( m_ptConn.end() , oVertexList.begin() , oVertexList.end() );
			}

			/*const int iVertexCount = pLineStringElm->GetVertexCount();
			DPoint3d pts[2] = {0,0};
			pts[0] = pLineStringElm->GetVertexAt(0);
			pts[1] = pLineStringElm->GetVertexAt(iVertexCount - 1);

			DPoint3d ptStart;
			const double d1 = ::DistanceBetween(ptConn , pts[0]);
			const double d2 = ::DistanceBetween(ptConn , pts[1]);
			if(d1 < d2)
			{
				ptConn = pts[1];
				ptStart= pts[0];
			}
			else
			{
				ptConn = pts[0];
				ptStart = pts[1];
			}

			m_ptConn.push_back( ptStart );
			m_ptConn.push_back( ptConn );*/
		}
		else
		{
			const int iSize = m_ptConn.size();
			ptConn = m_ptConn[iSize - 1];

			CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(p);
			{
				vector<DPoint3d> oVertexList = pLineStringElm->GetLineStringPointList();
				const double d1 = ::DistanceBetween(ptConn , oVertexList[0]);
				const double d2 = ::DistanceBetween(ptConn , oVertexList[1]);
				if(d1 > d2)
				{
					std::reverse(oVertexList.begin() , oVertexList.end());
				}
				
				m_ptConn.insert( m_ptConn.end() , oVertexList.begin() , oVertexList.end() );
			}

			
			//const int iVertexCount = pLineStringElm->GetVertexCount();
			//DPoint3d pts[2] = {0,0};
			//pts[0] = pLineStringElm->GetVertexAt(0);
			//pts[1] = pLineStringElm->GetVertexAt(iVertexCount - 1);

			//const double d1 = ::DistanceBetween(ptConn , pts[0]);
			//const double d2 = ::DistanceBetween(ptConn , pts[1]);
			//ptConn = (d1 < d2) ? pts[1] : pts[0];

			//m_ptConn.push_back( ptConn );
		}

		this->Add( p );

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-26
    @class      CLeaderElement
    @function   GetNextConnPointFrom
    @return     int
    @param      DPoint3d&           ptAdjacent
    @param      DPoint3d&           ptNextConn
    @param      CDgnLineString* pLineStringElm
    @param      const               DPoint3d&
    @param      ptConn
    @brief
******************************************************************************/
int CLeaderElement::GetNextConnPointFrom( DPoint3d& ptAdjacent , DPoint3d& ptNextConn , CDgnLineString* pLineStringElm , const DPoint3d& ptConn )
{
	assert(pLineStringElm && "pLineStringElm is NULL");

	if(pLineStringElm)
	{
		DPoint3d pts[2]={0,0};

		const int iVertexCount = pLineStringElm->GetVertexCount();
		pts[0] = pLineStringElm->GetVertexAt(0);
		pts[1] = pLineStringElm->GetVertexAt(iVertexCount - 1);

		const double d1 = ::DistanceBetween(ptConn , pts[0]);
		const double d2 = ::DistanceBetween(ptConn , pts[1]);
		if(d1 < d2)
		{
			ptAdjacent = pts[0];
			ptNextConn = pts[1];
		}
		else	
		{
			ptAdjacent = pts[1];
			ptNextConn = pts[0];
		}

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-??
    @class      CLeaderElement
    @function   CollectPartOfLeaderLine
    @return     int
    @param      vector<CDgnElement*>*   pDgnElmList
    @param      vector<CIsoElement*>*   pArrElmList
    @brief
******************************************************************************/
int CLeaderElement::CollectPartOfLeaderLine(vector<CDgnElement*>* pDgnElmList , vector<CIsoElement*>* pArrElmList )
{
	if(m_ptConn.empty()) return SUCCESS;
	
	vector<CDgnElement*> oDgnElmList;
	for(vector<CDgnElement*>::iterator itr = pDgnElmList->begin();itr != pDgnElmList->end();++itr)
	{
		if(m_oDgnElementList.end() != std::find( m_oDgnElementList.begin() , m_oDgnElementList.end() , *itr)) continue;
		if(!(*itr)->IsKindOf(CDgnLineString::TypeString())) continue;

		CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(*itr);
		if (2 == pLineStringElm->GetVertexCount())
		{
			oDgnElmList.push_back( *itr );
		}
	}

	CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
	const double dToler = CIsoModelDocData::m_dDrawingToler;
	const double dAngleToler = docData.GetAngleToler();

	do
	{
		const int iSize = m_ptConn.size();
		DPoint3d ptConn = m_ptConn[iSize - 1];  // ptConn 은 Part No 와 붙어 있는 Leader 의 맨 끝

		if(iSize > 1)
		{
			DVector3d dir;
			dir.org = m_ptConn[iSize - 1];
			dir.end = m_ptConn[iSize - 2];
			m_pArrowElement = static_cast<CArrowElement*>(CIsoElement::GetArrowElementHas(ptConn , &dir , pArrElmList));
		}
		else
		{
			m_pArrowElement = static_cast<CArrowElement*>(CIsoElement::GetArrowElementHas(ptConn , NULL , pArrElmList));
		}
		if(NULL != m_pArrowElement) break;

		stable_sort( oDgnElmList.begin() , oDgnElmList.end() , CIsoElement::SortDgnElmByDistance(ptConn) );
		CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(oDgnElmList[0]);

		DPoint3d ptAdjacent , ptNextConn;
		if(ERROR_SUCCESS != GetNextConnPointFrom( ptAdjacent , ptNextConn , pLineStringElm , ptConn )) break;

		const double dist = ::DistanceBetween(ptConn , ptAdjacent);				
		if (dist < dToler) // 일정 간격 안에 들어왔기 때문에 붙어 있는 Leader 로 간주
		{
			ptConn = ptNextConn;
			m_ptConn.push_back( ptConn );
			Add(oDgnElmList[0]);	
			pDgnElmList->erase( find(pDgnElmList->begin() , pDgnElmList->end() , oDgnElmList[0]) );
			
			oDgnElmList.erase( find(oDgnElmList.begin() , oDgnElmList.end() , oDgnElmList[0]) );
		}
		else	// 일정 간격안에 리더라인이 없으므로 각도가 같은 leader line 만 구한다
		{
			DPoint3d pts[2]={0,0};

			const int iSize = m_oDgnElementList.size();
			CDgnLineString* pLastLeaderLineElm = static_cast<CDgnLineString*>(m_oDgnElementList[iSize - 1]);
			double angle = 0.0;
			{
				const int iVertexCount = pLastLeaderLineElm->GetVertexCount();
				pts[0] = pLastLeaderLineElm->GetVertexAt(0);
				pts[1] = pLastLeaderLineElm->GetVertexAt(iVertexCount - 1);
				angle = DegreeBetween(pts[1] , pts[0]);
			}

			/// collect line string which has same degree
			vector<CDgnElement*> oSameAngleDgnElmList;
			for(vector<CDgnElement*>::iterator itr = oDgnElmList.begin();itr != oDgnElmList.end ();++itr)
			{
				DPoint3d _pts[2] = {0,0};
				CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(*itr);
				{
					const int iVertexCount = pLineStringElm->GetVertexCount();
					_pts[0] = pLineStringElm->GetVertexAt(0);
					_pts[1] = pLineStringElm->GetVertexAt(iVertexCount - 1);
				}
				if (true == Collinear(pts , _pts , dAngleToler))
				{
					oSameAngleDgnElmList.push_back(pLineStringElm);
				}
			}
			/// up to here

			vector<CIsoElement*> oSameAngleArrowElmList;
			for(vector<CIsoElement*>::iterator itr = pArrElmList->begin();itr != pArrElmList->end();++itr)
			{
				DPoint3d _pts[2]={0,0};
				CArrowElement* pArrElm = static_cast<CArrowElement*>(*itr);
				{
					vector<DPoint3d> oConnPtList;
					pArrElm->GetConnPointList( oConnPtList );
					_pts[0] = oConnPtList[0];
					_pts[1] = oConnPtList[1];
				}
				if(this->Collinear(pts , _pts , dAngleToler) && pArrElm->IsSameAngle( angle ))
				{
					oSameAngleArrowElmList.push_back( *itr );
				}
			}

			CArrowElement* pNearestArrowElm = NULL;
			if(!oSameAngleArrowElmList.empty())
			{
				stable_sort( oSameAngleArrowElmList.begin() , oSameAngleArrowElmList.end() , CIsoElement::SortIsoElmByDistance( ptConn ) );
				pNearestArrowElm = static_cast<CArrowElement*>(oSameAngleArrowElmList[0]);
			}

			if(!oSameAngleDgnElmList.empty())
			{
				stable_sort( oSameAngleDgnElmList.begin() , oSameAngleDgnElmList.end() , CIsoElement::SortDgnElmByDistance(ptConn) );

				/// check which item(arrow , line string) is near from leader
				CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(oSameAngleDgnElmList[0]);
				if(NULL != pNearestArrowElm)
				{
					double d1 = pLineStringElm->DistanceTo( ptConn );
					double d2 = pNearestArrowElm->DistanceTo( ptConn );
					if(d2 < d1)
					{
						pLineStringElm = NULL;

						vector<DPoint3d> ptConnList;
						pNearestArrowElm->GetConnPointList( ptConnList );
						double d1 = ::DistanceBetween(ptConn , ptConnList[0]);
						double d2 = ::DistanceBetween(ptConn , ptConnList[ptConnList.size() - 1]);

						ptConn = (d1 < d2) ? pts[1] : pts[0];

						m_ptConn.push_back( ptConn );
						m_pArrowElement = pNearestArrowElm;
					}
				}
				/// up to here

				if(NULL != pLineStringElm)
				{
					this->Add( oSameAngleDgnElmList[0] , ptConn );	
					pDgnElmList->erase( find(pDgnElmList->begin() , pDgnElmList->end() , oSameAngleDgnElmList[0]) );
					oDgnElmList.erase( find(oDgnElmList.begin() , oDgnElmList.end() , oSameAngleDgnElmList[0]) );
				}
			}
			else if(NULL != pNearestArrowElm)
			{
				vector<DPoint3d> ptConnList;
				oSameAngleArrowElmList[0]->GetConnPointList( ptConnList );
				const double d1 = ::DistanceBetween(ptConn , ptConnList[0]);
				const double d2 = ::DistanceBetween(ptConn , ptConnList[ptConnList.size() - 1]);

				ptConn = (d1 < d2) ? pts[1] : pts[0];

				m_ptConn.push_back( ptConn );
				m_pArrowElement = static_cast<CArrowElement*>(oSameAngleArrowElmList[0]);
			}else break;
		}
	}while(!oDgnElmList.empty() && (NULL == m_pArrowElement));
	
	/// arrow를 찾았음..
	if(NULL != m_pArrowElement)
	{
		vector<CIsoElement*>::iterator where = find(pArrElmList->begin() , pArrElmList->end() , m_pArrowElement);
		if(pArrElmList->end() != where) pArrElmList->erase( where );
	}
	
	return SUCCESS;
}

void CLeaderElement::Check(CDgnDocument* pDgnDoc , CString sColor)
{
	UInt32 uiColor = ATOI_T(sColor);
	for(vector<CDgnElement*>::iterator itr = m_oDgnElementList.begin();itr != m_oDgnElementList.end();++itr)
	{
		(*itr)->SetColor( uiColor );
	}
	
	if(m_pArrowElement) m_pArrowElement->Check(pDgnDoc , sColor);
}