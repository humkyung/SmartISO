#include "StdAfx.h"
#include <assert.h>
#include <limits>
#include <DgnLineString.h>
#include <algorithm>
#include "IsoModelDocData.h"
#include "PipeElement.h"
#include "PipeSupportElement.h"
#include "OletElement.h"

using namespace IsoElement;

CPipeElement::CPipeElement(const bool bHasFlowMark) : m_bHasFlowMark(bHasFlowMark)
{
	m_sTypeString = CPipeElement::TypeString();
}

CPipeElement::~CPipeElement(void)
{
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-10
    @class      CPipeElement
    @function   TypeString
    @return     CString
    @brief		return type string of pipe
******************************************************************************/
STRING_T CPipeElement::TypeString()
{
	return _T("pipe");
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-05
    @class      CPipeElement
    @function   Add
    @return     int
    @param      CDgnElement*    p
    @brief		요소를 추가한다. 요소는 연결 위치에 따라 리스트의 맨 앞에 혹은 맨 뒤에 추가된다.
******************************************************************************/
int CPipeElement::Add(CDgnElement* p)
{
	assert(p && "p is NULL");
	
	if(p && (p->IsKindOf(CDgnLineString::TypeString())))
	{
		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
		const double dToler = CIsoModelDocData::m_dDrawingToler;

		CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(p);
		if(pLineStringElm)
		{
			const int iVertexCount = pLineStringElm->GetVertexCount();
			/// flowmark를 요소일 경우 connection point를 추가하지 않고 그냥 리턴한다. - 2011.10.06 added by humkyung
			if(pLineStringElm->GetVertexCount() > MIN_VERTEX_COUNT_FOR_FLOW_MARK )
			{
				return ERROR_BAD_ENVIRONMENT;
			}
			/// up to here
			
			DPoint3d pts[2] = {0,0};
			pts[0] = pLineStringElm->GetVertexAt(0);
			pts[1] = pLineStringElm->GetVertexAt(iVertexCount - 1);
			
			if(m_ptConn.empty())
			{
				m_ptConn.push_back( pts[0] );
				m_ptConn.push_back( pts[1] );
				m_oDgnElementList.push_back( p );

				return ERROR_SUCCESS;
			}
			else
			{
				const int iSize = m_ptConn.size();
				double dDist[4] = {0.0,0.0,0.0,0.0};
				dDist[0] = ::DistanceBetween(m_ptConn[0] , pts[0]);
				dDist[1] = ::DistanceBetween(m_ptConn[0] , pts[1]);
				dDist[2] = ::DistanceBetween(m_ptConn[iSize - 1] , pts[0]);
				dDist[3] = ::DistanceBetween(m_ptConn[iSize - 1] , pts[1]);

				/// 앞쪽에 연결한다
				if((dDist[0] < dDist[1]) && (dDist[0] < dDist[2]) && (dDist[0] < dDist[3]))
				{
					m_ptConn.insert(m_ptConn.begin() , pts[1]);
					m_oDgnElementList.insert(m_oDgnElementList.begin() , p);

					return ERROR_SUCCESS;
				}
				else if((dDist[1] < dDist[0]) && (dDist[1] < dDist[2]) && (dDist[1] < dDist[3]))
				{
					m_ptConn.insert(m_ptConn.begin() , pts[0]);
					m_oDgnElementList.insert(m_oDgnElementList.begin() , p);

					return ERROR_SUCCESS;
				}
				/// up to here
				/// 맨 뒤쪽으로 연결한다
				else if((dDist[2] < dDist[0]) && (dDist[2] < dDist[1]) && (dDist[2] < dDist[3]))
				{
					m_ptConn.push_back(pts[1]);
					m_oDgnElementList.push_back(p);

					return ERROR_SUCCESS;
				}
				else if((dDist[3] < dDist[0]) && (dDist[3] < dDist[1]) && (dDist[3] < dDist[2]))
				{
					m_ptConn.push_back(pts[0]);
					m_oDgnElementList.push_back(p);
					

					return ERROR_SUCCESS;
				}
				/// up to here

				return ERROR_BAD_ENVIRONMENT;
			}
		}
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-21
    @class      CPipeElement
    @function   GetFlowMarkArrowPoint
    @return     DPoint3d
    @brief		get flow mark arrow point
******************************************************************************/
DPoint3d CPipeElement::GetFlowMarkArrowPoint()
{
	assert(m_bHasFlowMark && "this pipe doesn't have flow mark");

	DPoint3d ptArrow;
	ptArrow.x = ptArrow.y = ptArrow.z = 0.0;

	double dMaxDist = 0.0;
	DPoint3d ptLongestLine[2][2]={{0,0} , {0,0}};
	if(m_bHasFlowMark && (m_oDgnElementList.size() > 1))
	{
		if(!m_oDgnElementList[1]->IsKindOf(CDgnLineString::TypeString()))
		{
			ptArrow = m_oDgnElementList[1]->origin();
			return ptArrow;
		}

		CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(m_oDgnElementList[1]);
		const int iVertexCount = pLineStringElm->GetVertexCount();
		int iSelIndex = -1;
		for(int i = 0;i < iVertexCount;++i)
		{
			DPoint3d pts[2]={0,0};
			pts[0] = pLineStringElm->GetVertexAt(i);
			pts[1] = pLineStringElm->GetVertexAt((i + 1) % iVertexCount);
			const double d = ::DistanceBetween(pts[0] , pts[1]);
			if(d > dMaxDist)
			{
				iSelIndex = i;
				dMaxDist = d;
				ptLongestLine[0][0] = pts[0];
				ptLongestLine[0][1] = pts[1];
			}
		}

		dMaxDist = 0.0;
		for(int i = 0;i < iVertexCount;++i)
		{
			if(i == iSelIndex) continue;
			
			DPoint3d pts[2]={0,0};
			pts[0] = pLineStringElm->GetVertexAt(i);
			pts[1] = pLineStringElm->GetVertexAt((i + 1) % iVertexCount);
			if(
				CIsoElement::IsSamePoint(pts[0] , ptLongestLine[0][0]) || CIsoElement::IsSamePoint(pts[0] , ptLongestLine[0][1]) ||
				CIsoElement::IsSamePoint(pts[1] , ptLongestLine[0][0]) || CIsoElement::IsSamePoint(pts[1] , ptLongestLine[0][1])
				)
			{
				const double d = ::DistanceBetween(pts[0] , pts[1]);
				if(d > dMaxDist)
				{
					dMaxDist = d;
					 ptLongestLine[1][0] = pts[0];
					 ptLongestLine[1][1] = pts[1];
				}
			}
		}
		
		double dMinDist = -1.0;
		for(int i = 0;i < 2;++i)
		{
			for(int j = 0;j < 2;++j)
			{
				const double d = ::DistanceBetween((ptLongestLine[0][i]) , (ptLongestLine[1][j]));
				if(-1.0 == dMinDist)
				{
					dMinDist = d;
					ptArrow = ptLongestLine[0][i];
				}
				else if(d < dMinDist)
				{
					dMinDist = d;
					ptArrow = ptLongestLine[0][i];
				}
			}
		}
	}
	/// up to here

	return ptArrow;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-14
    @class      CPipeElement
    @function   BuildConnPointList
    @return     void
    @param      CDgnElement*    pElm1
    @param      CDgnElement*    pElm2
    @brief		build connection point list
******************************************************************************/
void CPipeElement::BuildConnPointList(CDgnElement* pElm1 , CDgnElement* pElm2)
{
	assert(pElm1 && pElm2 && "pElm1 or pElm2 is NULL");

	if(pElm1 && pElm2)
	{
		DPoint3d pts[2][2];
		CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(pElm1);
		int iVertexCount = pLineStringElm->GetVertexCount();
		pts[0][0] = pLineStringElm->GetVertexAt(0);
		pts[0][1] = pLineStringElm->GetVertexAt(iVertexCount - 1);
		pLineStringElm = static_cast<CDgnLineString*>(pElm2);
		iVertexCount = pLineStringElm->GetVertexCount();
		pts[1][0] = pLineStringElm->GetVertexAt(0);
		pts[1][1] = pLineStringElm->GetVertexAt(iVertexCount - 1);
		
		if(m_bHasFlowMark)
		{
			double dMinDist = -1.0;
			int iIndex[2] = {0,0};
			m_ptFlowMarkArrowPoint = GetFlowMarkArrowPoint();
			for(int i = 0;i < 2;++i)
			{
				for(int j = 0;j < 2;++j)
				{
					double d = ::DistanceBetween(pts[i][j] , m_ptFlowMarkArrowPoint);
					if(-1.0 == dMinDist)
					{
						dMinDist = d;
						iIndex[0] = i;
						iIndex[1] = j;
					}
					else if(d < dMinDist)
					{
						dMinDist = d;
						iIndex[0] = i;
						iIndex[1] = j;
					}
				}
			}
			
			m_ptConn.clear();
			m_ptConn.push_back( pts[iIndex[0]][(iIndex[1]+1)%2] );

			double dMaxDist = 0.0;
			iIndex[0] = (iIndex[0]+1)%2;
			for(int i = 0;i < 2;++i)
			{
				double d = ::DistanceBetween(pts[iIndex[0]][i] , m_ptFlowMarkArrowPoint);
				if(d > dMaxDist)
				{
					dMaxDist = d;
					iIndex[1] = i;
				}
			}

			m_ptConn.insert( m_ptConn.begin() , pts[iIndex[0]][iIndex[1]] );
		}
		else
		{
			double dMaxDist = -1.0;
			for(int i = 0;i < 2;++i)
			{
				for(int j = 0;j < 2;++j)
				{
					const double d = ::DistanceBetween((pts[0][i]) , (pts[1][j]));
					if(-1.0 == dMaxDist)
					{
						dMaxDist = d;
						m_ptConn.clear();
						m_ptConn.push_back( pts[0][i] );
						m_ptConn.push_back( pts[1][j] );
					}else if(d > dMaxDist)
					{
						dMaxDist = d;
						m_ptConn.clear();
						m_ptConn.push_back( pts[0][i] );
						m_ptConn.push_back( pts[1][j] );
					}
				}
			}
		}
	}
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-05
    @class      CPipeElement
    @function   CollectConnectPipe
    @return     int
    @param      vector<CDgnElement*>*   pDgnElementList
    @brief		Pipe Element를 확장시킨다.
******************************************************************************/
int CPipeElement::CollectConnectedPipe(vector<CIsoElement*>* pPipeElmList , vector<CIsoElement*>* pFittingElmList)
{
	assert(pPipeElmList && "pPipeElmList is NULL");

	if(pPipeElmList && !m_oDgnElementList.empty())
	{
		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
		const double dToler = CIsoModelDocData::m_dDrawingToler;

		bool bLoop = true;
		while(bLoop)
		{
			bLoop = false;

			DPoint3d pts[2] = {0,0};
			if(m_ptConn.size() >= 2)
			{
				pts[0] = m_ptConn[0];
				pts[1] = m_ptConn[m_ptConn.size() - 1];
			}else	break;

			bool bFoundConnectedFitting = false;
			if(NULL != pFittingElmList)
			{
				for(vector<CIsoElement*>::iterator itr = pFittingElmList->begin();itr != pFittingElmList->end();++itr)
				{
					vector<DPoint3d> ptConnList;
					(*itr)->GetConnPointList(ptConnList);
					for(vector<DPoint3d>::iterator jtr = ptConnList.begin();jtr != ptConnList.end();++jtr)
					{
						if(::DistanceBetween((*jtr) , (pts[0])) < dToler)
						{
							bFoundConnectedFitting = true;
							break;
						}
					}
				}
			}

			if(false == bFoundConnectedFitting)
			{
				CIsoElement* pConnectedPipe = NULL;
				/// start쪽에 연결된 pipe를 구한다.
				for(vector<CIsoElement*>::iterator itr = pPipeElmList->begin();itr != pPipeElmList->end();++itr)
				{
					vector<DPoint3d> ptConnList;
					(*itr)->GetConnPointList(ptConnList);
					for(vector<DPoint3d>::iterator jtr = ptConnList.begin();jtr != ptConnList.end();++jtr)
					{
						if(::DistanceBetween((*jtr) , (pts[0])) < dToler)
						{
							pConnectedPipe = *itr;
							break;
						}
					}
				}

				if(NULL != pConnectedPipe)
				{
					for(vector<CDgnElement*>::iterator itr = pConnectedPipe->m_oDgnElementList.begin();itr != pConnectedPipe->m_oDgnElementList.end();++itr)
					{
						Add( *itr );
					}

					pPipeElmList->erase( find(pPipeElmList->begin() , pPipeElmList->end() , pConnectedPipe) );
					SAFE_DELETE( pConnectedPipe );

					bLoop = true;
				}
			}

			/// check at second connection point
			bFoundConnectedFitting = false;
			if(NULL != pFittingElmList)
			{
				for(vector<CIsoElement*>::iterator itr = pFittingElmList->begin();itr != pFittingElmList->end();++itr)
				{
					vector<DPoint3d> ptConnList;
					(*itr)->GetConnPointList(ptConnList);
					for(vector<DPoint3d>::iterator jtr = ptConnList.begin();jtr != ptConnList.end();++jtr)
					{
						if(::DistanceBetween((*jtr) , (pts[1])) < dToler)
						{
							bFoundConnectedFitting = true;
							break;
						}
					}
				}
			}

			if(false == bFoundConnectedFitting)
			{
				CIsoElement* pConnectedPipe = NULL;
				/// start쪽에 연결된 pipe를 구한다.
				for(vector<CIsoElement*>::iterator itr = pPipeElmList->begin();itr != pPipeElmList->end();++itr)
				{
					vector<DPoint3d> ptConnList;
					(*itr)->GetConnPointList(ptConnList);
					for(vector<DPoint3d>::iterator jtr = ptConnList.begin();jtr != ptConnList.end();++jtr)
					{
						if(::DistanceBetween((*jtr) , (pts[1])) < dToler)
						{
							pConnectedPipe = *itr;
							break;
						}
					}
				}

				if(NULL != pConnectedPipe)
				{
					for(vector<CDgnElement*>::iterator itr = pConnectedPipe->m_oDgnElementList.begin();itr != pConnectedPipe->m_oDgnElementList.end();++itr)
					{
						Add( *itr );
					}

					pPipeElmList->erase( find(pPipeElmList->begin() , pPipeElmList->end() , pConnectedPipe) );
					SAFE_DELETE( pConnectedPipe );

					bLoop = true;
				}
			}
		}

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-05
    @class      CPipeElement
    @function   DistanceTo
    @return     double
    @param      CPipeElement*   pTo
    @brief		return distance between this and pTo
******************************************************************************/
double CPipeElement::DistanceWith( CPipeElement* pTo )
{
	assert(pTo && "pTo is NULL");

	if(pTo)
	{
		vector<DPoint3d> pts[2];
		GetConnPointList( pts[0] );
		pTo->GetConnPointList( pts[1] );

		double dMinDist = -1.0;
		for(vector<DPoint3d>::iterator itr = pts[0].begin();itr != pts[0].end();++itr)
		{
			for(vector<DPoint3d>::iterator jtr = pts[1].begin();jtr != pts[1].end();++jtr)
			{
				const double dDist = ::DistanceBetween((*itr) , (*jtr));
				if(-1.0 == dMinDist)
				{
					dMinDist = dDist;
				}
				else if(dDist < dMinDist) dMinDist = dDist;
			}
		}

		return dMinDist;
	}

	return -1.0;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-07
    @class      CPipeElement
    @function   GetConnPointList
    @return     int
    @param      vector<DPoint3d>&   pts
    @brief
******************************************************************************/
int CPipeElement::GetConnPointList(vector<DPoint3d>& pts)
{
	pts.clear();
	if(!m_ptConn.empty())
	{
		const int iSize = m_ptConn.size();
		pts.push_back( m_ptConn[0] );
		if(iSize > 1)
		{
			pts.push_back( m_ptConn[iSize - 1] );
		}
	}
	else
	{
		throw exception("index out of range");
	}

	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-21
    @class      CPipeElement
    @function   HasFlowMark
    @return     bool
    @brief		return true if it has flowmark
******************************************************************************/
bool CPipeElement::HasFlowMark() const
{
	return m_bHasFlowMark;
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-02
    @class      CPipeElement
    @function   angle
    @return     double
    @brief		return the angle of pipe element
******************************************************************************/
double CPipeElement::angle() const
{
	if(m_ptConn.size() > 1)
	{
		return DegreeBetween( m_ptConn[0] , m_ptConn[m_ptConn.size() - 1] );
	}

	throw exception("invalid index number");
}

void CPipeElement::Check(CDgnDocument* pDgnDoc , CString sColor)
{
	UInt32 uiColor = ATOI_T(sColor);
	for(vector<CDgnElement*>::iterator itr = m_oDgnElementList.begin();itr != m_oDgnElementList.end();++itr)
	{
		(*itr)->SetColor( uiColor );
	}

	if(m_bHasFlowMark)
	{
		///CIsoElement::Test_DrawBoundary( m_oTemp , iColor );

		CDgnVolume volume;
		
		DPoint3d pt = m_ptFlowMarkArrowPoint;
		pt.x += 0.05; pt.y += 0.05;
		volume.Add( pt );

		pt = m_ptFlowMarkArrowPoint;
		pt.x -= 0.05; pt.y -= 0.05;
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

		::DrawPolyline(pDgnDoc , oPolyline , sColor );
	}
}