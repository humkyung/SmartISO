#include "StdAfx.h"
#include <assert.h>
#include <float.h>
#include <DgnLineString.h>
#include <DgnText.h>
#include "DimElement.h"
#include "PipeRoutine.h"
#include "SlopeAreaMarkElement.h"
#include "IsoModelDocData.h"
#include "ConvexHull.h"

#include <queue>
using namespace std;

using namespace IsoElement;

CDimElement::CDimElement() : m_pLeaderLineElm(NULL) , m_pDimLineElm(NULL) , m_pExtLineElm(NULL) , m_pConnectedPipeRun(NULL)
{
	m_iDimType = 0;
	m_sTypeString = CDimElement::TypeString();
}

CDimElement::~CDimElement(void)
{
	try
	{
		SAFE_DELETE(m_pLeaderLineElm);
		SAFE_DELETE(m_pDimLineElm);
		SAFE_DELETE(m_pExtLineElm);
	}
	catch(...)
	{
	}
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-22
    @class      CDimElement
    @function   TypeString
    @return     CString
    @brief
******************************************************************************/
STRING_T CDimElement::TypeString()
{
	return _T("dimension");
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-20
    @class      CDimElement
    @function   GetDimTextElement
    @return     CDgnText*
    @brief		return dimension text element
******************************************************************************/
CDgnText* CDimElement::GetDimTextElement() const
{
	for(vector<CDgnElement*>::const_iterator itr = m_oDgnElementList.begin();itr != m_oDgnElementList.end();++itr)
	{
		if((*itr)->IsKindOf(CDgnText::TypeString()))
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
    @class      CDimElement
    @function   GetDimTextEnclosePolygon
    @return     vector<DPoint3d>
    @brief		return a polygon enclose text element
******************************************************************************/
vector<DPoint3d> CDimElement::GetDimTextEnclosePolygon()
{
	vector<DPoint3d> oEnclosePolygon;

	CDgnText* pTextElm = GetDimTextElement();
	if(NULL != pTextElm)
	{
		oEnclosePolygon = pTextElm->enclosePolygon();
		return oEnclosePolygon;
	}

	return oEnclosePolygon;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-13
    @class      CDimElement
    @function   ExpandEnclosePolygon
    @return     int
    @param      vector<DPoint3d>&   oEnclosePolygon
    @brief
******************************************************************************/
int CDimElement::ExpandEnclosePolygon( vector<DPoint3d>& oEnclosePolygon , const double dWidthOffset , const double dHeightOffset)
{
	assert(4 == oEnclosePolygon.size());

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
    @author     humkyung
    @date       2011-10-17
    @class      CDimElement
    @function   GetCenterOfDimText
    @return     DPoint3d
    @brief		return center point of dim text
******************************************************************************/
DPoint3d CDimElement::GetCenterOfDimText()
{
	vector<DPoint3d> oEnclosePolygon = GetDimTextEnclosePolygon();

	double dx[2] = {0,0} , dy[2] = {0,0} , dz[2] = {0,0};
	dx[0] = oEnclosePolygon[1].x - oEnclosePolygon[0].x;
	dy[0] = oEnclosePolygon[1].y - oEnclosePolygon[0].y;
	dz[0] = oEnclosePolygon[1].z - oEnclosePolygon[0].z;

	dx[1] = oEnclosePolygon[3].x - oEnclosePolygon[0].x;
	dy[1] = oEnclosePolygon[3].y - oEnclosePolygon[0].y;
	dz[1] = oEnclosePolygon[3].z - oEnclosePolygon[0].z;

	DPoint3d ptCenter;
	ptCenter.x = oEnclosePolygon[0].x + dx[0]*0.5 + dx[1]*0.5;
	ptCenter.y = oEnclosePolygon[0].y + dy[0]*0.5 + dy[1]*0.5;
	ptCenter.z = oEnclosePolygon[0].z + dz[0]*0.5 + dz[1]*0.5;

	return ptCenter;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-17
    @class      CDimElement
    @function   GetLineListConnectedToDimText
    @return     vector<CDgnElement*>
    @param      vector<CDgnElement*>*   pDgnElmList
    @brief		dim. text에 연결된 line string들을 구한다.
******************************************************************************/
vector<CDgnElement*> CDimElement::GetLineListConnectedToDimText(vector<CDgnElement*>* pDgnElmList , const double& dWOffset , const double& dHOffset)
{
	assert(pDgnElmList && "pDgnElmList is NULL");

	vector<CDgnElement*> oCandidatedElmList;
	if(pDgnElmList)
	{
		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
		const double dAngleToler = docData.GetAngleToler();

		vector<DPoint3d> oEnclosePolygon = GetDimTextEnclosePolygon();
		ExpandEnclosePolygon( oEnclosePolygon , dWOffset , dHOffset);
		
		for(vector<CDgnElement*>::iterator itr = pDgnElmList->begin();itr != pDgnElmList->end();++itr)
		{
			if((*itr)->IsKindOf(CDgnLineString::TypeString()))
			{
				CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(*itr);
				const int iVertexCount = pLineStringElm->GetVertexCount();
				if(2 == iVertexCount)
				{
					DPoint3d pts[2];
					pts[0] = pLineStringElm->GetVertexAt( 0 );
					pts[1] = pLineStringElm->GetVertexAt( 1 );
					
					const bool isInner1 = CIsoElement::IsInnerPt(oEnclosePolygon , pts[0] );
					const bool isInner2 = CIsoElement::IsInnerPt(oEnclosePolygon , pts[1] );
					if(isInner1 && isInner2)
					{
						/// 양끝의 점이 polygon안에 둘다 들어온다는 것은 아주 작은 line string이라는 의미..
						/// 이 line string을 dim text와 처음 연결되는 line string으로 간주한다.
						oCandidatedElmList.clear();
						oCandidatedElmList.push_back( pLineStringElm );
						break;
					}
					else if( isInner1 || isInner2)
					{
						oCandidatedElmList.push_back( pLineStringElm );
					}
				}
			}
		}
	}

	return oCandidatedElmList;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-17
    @class
    @function   GetNearestElmFrom
    @return     CDgnElement*
    @param      vector<CDgnElement*>*   pDgnElmList
    @param      DPoint3d                ptOrg
    @param      DVector3d               l
    @brief		get nearest element from ptOrg along l
******************************************************************************/
CDgnElement* CDimElement::GetNearestElmWithSameAngleFrom(vector<CDgnElement*>* pDgnElmList , DPoint3d ptOrg , DVector3d l , const double& dDimTextAngle)
{
	assert(pDgnElmList && "pDgnElmList is NULL");

	CDgnElement* pNearestElm = NULL;
	if(pDgnElmList)
	{
		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
		const double dAngleToler = docData.GetAngleToler();

		/// connection point와 가장 가까운 line string을 찾는다.
		double dMinDist = -1.0;
		for(vector<CDgnElement*>::iterator itr = pDgnElmList->begin();itr != pDgnElmList->end();++itr)
		{
			if(!(*itr)->IsKindOf(CDgnLineString::TypeString())) continue;

			CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(*itr);
			if(2 == pLineStringElm->GetVertexCount())
			{
				DPoint3d pts[2]={0,};
				pts[0] = pLineStringElm->GetVertexAt(0);
				pts[1] = pLineStringElm->GetVertexAt(1);
				const double angle = DegreeBetween(pts[0] , pts[1]);

				const double toler = fabs(angle - dDimTextAngle);
				if(IsParallelAngle(angle , dDimTextAngle , dAngleToler))
				{
					DPoint3d intersect;
					if(SUCCESS == pLineStringElm->IntersectWith(intersect , &l))
					{
						const double dDist = ::DistanceBetween(ptOrg , intersect);
						if(NULL == pNearestElm)
						{
							dMinDist = dDist;
							pNearestElm = pLineStringElm;
						}
						else if(dDist < dMinDist)
						{
							dMinDist = dDist;
							pNearestElm = pLineStringElm;
						}
					}
				}
			}
		}
		/// up to here
	}

	return pNearestElm;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-05
    @class      CDimElement
    @function   CollectDimLines
    @return     int
    @param      vector<CDgnElement*>*   pDgnElmList
    @brief		dimension line들을 구한다.(좀더 정교한 로직이 필요하다)
******************************************************************************/
int CDimElement::CollectDimLines( vector<CDgnElement*>* pDgnElmList , vector<CIsoElement*>* pArrElmList )
{
	assert(pDgnElmList && "pDgnElmList is NULL");

	if(pDgnElmList)
	{
		{
			SAFE_DELETE( m_pDimLineElm );
			m_pDimLineElm = new CDimLineElement();
		}

		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
		const double dToler = docData.m_dDrawingToler;
		const double dOffset = docData.GetOffset();
		const double dAngleToler = docData.GetAngleToler();

		/// 텍스트를 둘러싸고 있는 polygon에 포함되는 line string을 구한다.
		vector<CDgnElement*> oCandidatedElmList = GetLineListConnectedToDimText(pDgnElmList , dOffset , dOffset*0.5);
		/// up to here
		
		/// arrow를 가지는 line string이 몇 개인지 검사한다. - 2011.10.24 added by humkyung
		vector<CDgnElement*> oOneArrowList , oTwoArrowList;
		const DPoint3d ptCenter = this->GetCenterOfDimText();
		CIsoElement::GetLeaderLineFrom(oOneArrowList , oTwoArrowList , oCandidatedElmList , pArrElmList , ptCenter);
		
		/// Type2일 경우를 확인하고 가능성이 있으면 나머지 하나를 찾도록 한다. - 2011.11.18 added by humkyung
		if(oTwoArrowList.empty() && (1 == oOneArrowList.size()))
		{
			const double dAngle1 = GetDimTextElement()->angle();	/// in degree
			CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(oCandidatedElmList[0]);
			const int iCount = pLineStringElm->GetVertexCount();
			const double dAngle2 = ::DegreeBetween(pLineStringElm->GetVertexAt(0) , pLineStringElm->GetVertexAt(iCount - 1));
			const double d = fabs(dAngle1 - dAngle2);
			if((d < dAngleToler) || (fabs(180 - d) < dAngleToler))
			{
				oCandidatedElmList = GetLineListConnectedToDimText(pDgnElmList , dOffset*2 , dOffset*0.5);
				CIsoElement::GetLeaderLineFrom(oOneArrowList , oTwoArrowList , oCandidatedElmList , pArrElmList , ptCenter);
			}
		}
		/// up to here

		if((oOneArrowList.empty() && (1 == oTwoArrowList.size())) || (oOneArrowList.empty() && oTwoArrowList.empty()))
		{
			oCandidatedElmList.clear();
		}
		else if(1 == oOneArrowList.size())
		{
			oCandidatedElmList.clear();
			oCandidatedElmList.insert( oCandidatedElmList.begin() , oOneArrowList.begin() , oOneArrowList.end() );
		}
		else if(2 == oOneArrowList.size())
		{
			oCandidatedElmList.clear();
			oCandidatedElmList.insert( oCandidatedElmList.begin() , oOneArrowList.begin() , oOneArrowList.end() );
		}
		/// up to here

		/// 구한 line이 하나일 경우는 지시선이라고 가정한다.
		if(!oCandidatedElmList.empty() && (1 == oCandidatedElmList.size() % 2))	/// Type1
		{
			m_iDimType = 1;

			/// remove collinear lines
			bool bLoop = true;
			while(bLoop)
			{
				bLoop = false;
				for(vector<CDgnElement*>::iterator itr = oCandidatedElmList.begin();itr != oCandidatedElmList.end();++itr)
				{
					DPoint3d pts[2]={0,0};
					CDgnLineString* lhs = static_cast<CDgnLineString*>(*itr);
					{
						const int iVertexCount = lhs->GetVertexCount();
						pts[0] = lhs->GetVertexAt(0);
						pts[1] = lhs->GetVertexAt(iVertexCount - 1);
					}
					for(vector<CDgnElement*>::iterator jtr = itr + 1;jtr != oCandidatedElmList.end();++jtr)
					{
						DPoint3d _pts[2]={0,0};
						CDgnLineString* rhs = static_cast<CDgnLineString*>(*jtr);
						{
							const int iVertexCount = rhs->GetVertexCount();
							_pts[0] = rhs->GetVertexAt(0);
							_pts[1] = rhs->GetVertexAt(iVertexCount - 1);
						}
						if(true == Collinear(pts , _pts))
						{
							oCandidatedElmList.erase(jtr);
							oCandidatedElmList.erase(itr);
							bLoop = true;
							break;
						}
					}
					if(true == bLoop) break;
				}
			}
			/// up to here

			/// sort dgn element by center of dim. text
			stable_sort(oCandidatedElmList.begin() , oCandidatedElmList.end() , CIsoElement::SortDgnElmByDistance(GetCenterOfDimText()));
			/// up to here

			CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(oCandidatedElmList[0]);
			{
				SAFE_DELETE(m_pLeaderLineElm);
				m_pLeaderLineElm = new CLeaderElement();
				m_pLeaderLineElm->Add( pLineStringElm , GetCenterOfDimText() );
				m_pLeaderLineElm->CollectPartOfLeaderLine( pDgnElmList , pArrElmList );

				pDgnElmList->erase( find(pDgnElmList->begin() , pDgnElmList->end() , pLineStringElm) );
			}
			/// up to here

			if(NULL != m_pDimLineElm)
			{
				m_pDimLineElm->CollectElements(pDgnElmList , pArrElmList , m_pLeaderLineElm , m_iDimType);
			}
		}
		else if(!oCandidatedElmList.empty() && (0 == oCandidatedElmList.size() % 2))	/// Type2
		{
			m_iDimType = 2;

			vector<DPoint3d> oEnclosePolygon = GetDimTextEnclosePolygon();
			
			DPoint3d pts[2]={0,0};
			pts[0].x = (oEnclosePolygon[1].x + oEnclosePolygon[2].x)*0.5;
			pts[0].y = (oEnclosePolygon[1].y + oEnclosePolygon[2].y)*0.5;
			pts[0].z = (oEnclosePolygon[1].z + oEnclosePolygon[2].z)*0.5;
			pts[1].x = (oEnclosePolygon[3].x + oEnclosePolygon[0].x)*0.5;
			pts[1].y = (oEnclosePolygon[3].y + oEnclosePolygon[0].y)*0.5;
			pts[1].z = (oEnclosePolygon[3].z + oEnclosePolygon[0].z)*0.5;

			for(int i = 0;i < 2;++i)
			{
				stable_sort(oCandidatedElmList.begin() , oCandidatedElmList.end() , CIsoElement::SortDgnElmByDistance(pts[i]));
				m_pDimLineElm->Add(oCandidatedElmList[0]);
				
				pDgnElmList->erase( find(pDgnElmList->begin() , pDgnElmList->end() , oCandidatedElmList[0]) );
				oCandidatedElmList.erase( find(oCandidatedElmList.begin() , oCandidatedElmList.end() , oCandidatedElmList[0]) );
			}

			m_pDimLineElm->CollectArrows( pArrElmList );
		}
		else if(oCandidatedElmList.empty())	/// Type3 - 2011.10.17 added by humkyung
		{
			m_iDimType = 3;

			if(oTwoArrowList.empty())
			{
				vector<DPoint3d> oEnclosePolygon = GetDimTextEnclosePolygon();
				
				DVector3d l;
				l.org.x = (oEnclosePolygon[0].x + oEnclosePolygon[1].x)*0.5;
				l.org.y = (oEnclosePolygon[0].y + oEnclosePolygon[1].y)*0.5;
				l.org.z = (oEnclosePolygon[0].z + oEnclosePolygon[1].z)*0.5;
				l.end.x = (oEnclosePolygon[2].x + oEnclosePolygon[3].x)*0.5;
				l.end.y = (oEnclosePolygon[2].y + oEnclosePolygon[3].y)*0.5;
				l.end.z = (oEnclosePolygon[2].z + oEnclosePolygon[3].z)*0.5;

				const double dDimTextAngle = GetDimTextElement()->angle();
				CDgnElement* pNearestElm = GetNearestElmWithSameAngleFrom(pDgnElmList , GetCenterOfDimText() , l , dDimTextAngle);
				if(pNearestElm)
				{
					m_pDimLineElm->Add( pNearestElm );
					m_pDimLineElm->CollectArrows( pArrElmList );

					pDgnElmList->erase( find(pDgnElmList->begin() , pDgnElmList->end() , pNearestElm) );
				}
			}
			else if(1 == oTwoArrowList.size())
			{
				m_pDimLineElm->Add( oTwoArrowList[0] );
				m_pDimLineElm->CollectArrows( pArrElmList );

				pDgnElmList->erase( find(pDgnElmList->begin() , pDgnElmList->end() , oTwoArrowList[0]) );
			}
		}

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-06
    @class      CDimElement
    @function   CollectExtendLines
    @return     int
    @param      vector<CDgnElement*>*   pDgnElmList
    @brief		dimension line과 연결되는 extend line을 구한다.
******************************************************************************/
int CDimElement::CollectExtendLines( vector<CDgnElement*>* pDgnElmList , vector<CIsoElement*>* pArrElmList )
{
	assert(pDgnElmList && pArrElmList &&  "pDgnElmList or pArrElmList is NULL");

	if(pDgnElmList && m_pDimLineElm && pArrElmList)
	{
		{
			SAFE_DELETE(m_pExtLineElm);
			m_pExtLineElm = new CDimExtLineElement();
		}

		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
		const double dOffset = docData.GetOffset();
		const double dToler  = CIsoModelDocData::m_dDrawingToler;

		/// dimension line의 두 connection point를 구한다.(가장 거리가 먼 두 점을 구한다.)
		vector<DPoint3d> oExtPtList;
		if(ERROR_SUCCESS == GetExtPointList(oExtPtList))
		{
			DVector3d l1;
			l1.org = oExtPtList[0];
			l1.end = oExtPtList[1];
			
			double dMinDist[2] = {DBL_MAX,DBL_MAX};
			CDgnElement* pExtendLineElm[2] = {NULL,NULL};
			for(vector<CDgnElement*>::iterator itr = pDgnElmList->begin();itr != pDgnElmList->end();++itr)
			{
				if(!(*itr)->IsKindOf(CDgnLineString::TypeString())) continue;

				CDgnLineString* _pLineStringElm = static_cast<CDgnLineString*>(*itr);
				if(2 == _pLineStringElm->GetVertexCount())
				{
					DPoint3d ptEdge[2] = {0,0};
					ptEdge[0] = _pLineStringElm->GetVertexAt(0);
					ptEdge[1] = _pLineStringElm->GetVertexAt(1);

					DPoint3d intersect;
					if(SUCCESS != _pLineStringElm->IntersectWith(intersect , &l1 , dToler)) continue;

					double dist[2]={0.0,0.0};
					dist[0] = ::DistanceBetween((oExtPtList[0]) , intersect);
					if((dist[0] < dToler) && (dist[0] < dMinDist[0]))	///NOTICE: Extend line과 dim. line은 최소 1.0 거리 안에 있어야 한다.
					{
						vector<CDgnElement*> oOneArrowList , oTwoArrowList , oCandidatedElmList;
						oCandidatedElmList.push_back( _pLineStringElm );
						CIsoElement::GetLeaderLineFrom(oOneArrowList , oTwoArrowList , oCandidatedElmList , pArrElmList , oExtPtList[0]);
						if(oOneArrowList.empty() && oTwoArrowList.empty())
						{
							dMinDist[0] = dist[0];
							pExtendLineElm[0] = _pLineStringElm;
						}
					}

					dist[1] = ::DistanceBetween((oExtPtList[1]) , intersect);
					if((dist[1] < dToler) && (dist[1] < dMinDist[1]))	///NOTICE: Extend line과 dim. line은 최소 1.0 거리 안에 있어야 한다.
					{
						vector<CDgnElement*> oOneArrowList , oTwoArrowList , oCandidatedElmList;
						oCandidatedElmList.push_back( _pLineStringElm );
						CIsoElement::GetLeaderLineFrom(oOneArrowList , oTwoArrowList , oCandidatedElmList , pArrElmList , oExtPtList[0]);
						if(oOneArrowList.empty() && oTwoArrowList.empty())
						{
							dMinDist[1] = dist[1];
							pExtendLineElm[1] = _pLineStringElm;
						}
					}
				}
			}
			
			if(NULL != pExtendLineElm[0])
			{
				m_pExtLineElm->Add( pExtendLineElm[0] );
			}
			if(NULL != pExtendLineElm[1])
			{
				m_pExtLineElm->Add( pExtendLineElm[1] );
			}
			///NOTICE: pLineStringElm_을 리스트에서 제거하지 않는다.( Extension Line은 공유하기 때문이다...)

			return ERROR_SUCCESS;
		}
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-17
    @class      CDimElement
    @function   GetDimPointList
    @return     int
    @param      vector<DPoint3d>&   oDimPtList
    @brief		return extend line direction which has longest length
******************************************************************************/
DVector3d CDimElement::GetExtendLineDirection()
{
	DVector3d oVector3d;
	oVector3d.org.x = oVector3d.org.y = oVector3d.org.z = 0.0;
	oVector3d.end.x = oVector3d.end.y = oVector3d.end.z = 0.0;

	if(m_pExtLineElm)
	{
		double dMaxDist = 0.0;
		for(vector<CDgnElement*>::iterator itr = m_pExtLineElm->m_oDgnElementList.begin();itr != m_pExtLineElm->m_oDgnElementList.end();++itr)
		{
			if((*itr)->IsKindOf(CDgnLineString::TypeString()))
			{
				CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(*itr);
				const int iVertexCount = pLineStringElm->GetVertexCount();
				DPoint3d pt1 = pLineStringElm->GetVertexAt(0);
				DPoint3d pt2 = pLineStringElm->GetVertexAt(iVertexCount - 1);

				const double d = ::DistanceBetween(pt1 , pt2);
				if(d > dMaxDist)
				{
					dMaxDist = d;
					oVector3d.org = pt1;
					oVector3d.end = pt2;
				}
			}
		}

		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
		const int iArrowType = docData.GetArrowType();
		if(2 == iArrowType)
		{
			swap(oVector3d.org  , oVector3d.end);
		}
	}

	return oVector3d;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-18
    @class      CDimElement
    @function   GetDimLengthOnDrawing
    @return     double
    @brief		return dimension length on drawing
******************************************************************************/
double CDimElement::GetDimValue() const
{
	double dDimValue = 0.0;
	CDgnText* pTextElm = GetDimTextElement();
	if(NULL != pTextElm)
	{
		dDimValue = ATOF_T(pTextElm->textString());
	}
	
	return dDimValue;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-18
    @class      CDimElement
    @function   angle
    @return     double
    @brief		return dimension angle
******************************************************************************/
double CDimElement::angle() const
{
	if(1 == m_oDgnElementList.size())
	{
		vector<DPoint3d> oExtPtList;
		GetExtPointList(oExtPtList);
		if(2 == oExtPtList.size())
		{
			DVec3d vec;
			vec.x = oExtPtList[1].x - oExtPtList[0].x;
			vec.y = oExtPtList[1].y - oExtPtList[0].y;
			vec.z = oExtPtList[1].z - oExtPtList[0].z;

			double angle = ::AngleOf(vec);
			if(angle < 0.0) angle += 2*PI;
			return RAD2DEG(angle);
		}
	}

	return 0.0;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-18
    @class      CDimElement
    @function   GetExtPointList
    @return     int
    @param      vector<DPoint3d>&   oDimPtList
    @brief		get the intersect point with dim line and extend line
******************************************************************************/
int CDimElement::GetExtPointList(vector<DPoint3d>& oExtPtList) const
{
	if(m_pDimLineElm)
	{
		oExtPtList.clear();

		vector<DPoint3d> pts;
		for(vector<CDgnElement*>::iterator itr = m_pDimLineElm->m_oDgnElementList.begin();itr != m_pDimLineElm->m_oDgnElementList.end();++itr)
		{
			if((*itr)->IsKindOf(CDgnLineString::TypeString()))
			{
				CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(*itr);
				const int iVertexCount = pLineStringElm->GetVertexCount();
				pts.push_back(pLineStringElm->GetVertexAt(0));
				pts.push_back(pLineStringElm->GetVertexAt(iVertexCount - 1));
			}
		}

		double dMaxDist = 0.0;
		DPoint3d ptExt[2] = {0,0};
		for(vector<DPoint3d>::iterator itr = pts.begin();itr != pts.end();++itr)
		{
			for(vector<DPoint3d>::iterator jtr = itr+1;jtr != pts.end();++jtr)
			{
				const double d = ::DistanceBetween((*itr) , (*jtr));
				if(d > dMaxDist)
				{
					dMaxDist = d;

					ptExt[0] = (*itr);
					ptExt[1] = (*jtr);
				}
			}
		}

		oExtPtList.push_back( ptExt[0] );
		oExtPtList.push_back( ptExt[1] );
		
		return ERROR_SUCCESS;
	}

	return ERROR_BAD_ENVIRONMENT;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-18
    @class      CDimElement
    @function   GetEnclosePolygon
    @return     int
    @param      vector<DPoint3d>&   oEnclosePolygon
	@param		dOffset create a polygon to enclose dim line and extend line if dOffset = -1
    @brief		return a polygon enclose dim line and extend line
******************************************************************************/
int CDimElement::GetEnclosePolygon(vector<DPoint3d>& oEnclosePolygon , const double dOffset)
{
	oEnclosePolygon.clear();

	vector<DPoint3d> oExtPtList;
	if(ERROR_SUCCESS == GetExtPointList(oExtPtList))
	{
		oEnclosePolygon.insert(oEnclosePolygon.begin() , oExtPtList.begin() , oExtPtList.end());
		DVector3d dir = GetExtendLineDirection();

		DVec3d vec;
		vec.x = dir.end.x - dir.org.x;
		vec.y = dir.end.y - dir.org.y;
		vec.z = dir.end.z - dir.org.z;

		if(-1.0 == dOffset)
		{
			DPoint3d pt;
			pt.x = oEnclosePolygon[1].x + vec.x;
			pt.y = oEnclosePolygon[1].y + vec.y;
			pt.z = oEnclosePolygon[1].z + vec.z;
			oEnclosePolygon.push_back( pt );

			pt.x = oEnclosePolygon[0].x + vec.x;
			pt.y = oEnclosePolygon[0].y + vec.y;
			pt.z = oEnclosePolygon[0].z + vec.z;
			oEnclosePolygon.push_back( pt );
		}
		else
		{
			::NormalizeVector(vec);

			DPoint3d pt(oEnclosePolygon[1]);
			pt.x += vec.x*dOffset;
			pt.y += vec.y*dOffset;
			pt.z += vec.z*dOffset;
			oEnclosePolygon.push_back( pt );

			pt = oEnclosePolygon[0];
			pt.x += vec.x*dOffset;
			pt.y += vec.y*dOffset;
			pt.z += vec.z*dOffset;
			oEnclosePolygon.push_back( pt );
		}

		return ERROR_SUCCESS;
	}

	return ERROR_BAD_ENVIRONMENT;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-18
    @class      CDimElement
    @function   CollectSubDimension
    @return     int
    @param      vector<CIsoElement*>*   pDimElmList
    @brief		collect sub dimension
******************************************************************************/
int CDimElement::CollectSubDimension(vector<CIsoElement*>* pDimElmList)
{
	assert(pDimElmList && "pDimElmList is NULL");
	
	int iCount = 0;
	if(pDimElmList)
	{
		DVector3d l1 , l2;
		vector<DPoint3d> oExtPtList;
		this->GetExtPointList(oExtPtList);
		l1.org = oExtPtList[0];
		l1.end = oExtPtList[1];
		
		DPoint3d ptMid;
		ptMid.x = (l1.org.x + l1.end.x)*0.5;
		ptMid.y = (l1.org.y + l1.end.y)*0.5;
		ptMid.z = (l1.org.z + l1.end.z)*0.5;
		const double radius = ::DistanceBetween(ptMid , l1.org);

		DVector3d dir = this->GetExtendLineDirection();
		DVec3d vec;
		vec.x = dir.end.x - dir.org.x;
		vec.y = dir.end.y - dir.org.y;
		vec.z = dir.end.z - dir.org.z;

		for(vector<CIsoElement*>::iterator itr = pDimElmList->begin();itr != pDimElmList->end();/*++itr*/)
		{
			CDimElement* pDimElm = static_cast<CDimElement*>(*itr);
			

			pDimElm->GetExtPointList(oExtPtList);
			l2.org.x = (oExtPtList[0].x + oExtPtList[1].x)*0.5;
			l2.org.y = (oExtPtList[0].y + oExtPtList[1].y)*0.5;
			l2.org.z = (oExtPtList[0].z + oExtPtList[1].z)*0.5;
			l2.end.x = l2.org.x + vec.x;
			l2.end.y = l2.org.y + vec.y;
			l2.end.z = l2.org.z + vec.z;
			
			DPoint3d intersect;
			if(SUCCESS == ::IntersectWith(intersect , l1 , l2))
			{
				const double d = ::DistanceBetween(ptMid , intersect);
				if(d <= radius)
				{
					m_oSubDimElmList.push_back( *itr );
					itr = pDimElmList->erase( itr );
					continue;
				}
			}
			++itr;
		}
	}

	return iCount;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-18
    @class      CDimElement
    @function   Select
    @return     int
    @param      DgnModelRefP    modelRef
    @brief		select dimension element
******************************************************************************/
int CDimElement::Select(bool bIncludeDimLine , DgnModelRefP modelRef)
{
	for(vector<CDgnElement*>::iterator itr = m_oDgnElementList.begin();itr != m_oDgnElementList.end();++itr)
	{
		///(*itr)->Select( modelRef );
	}

	if((true == bIncludeDimLine) && m_pDimLineElm)
	{
		for(vector<CDgnElement*>::iterator itr = m_pDimLineElm->m_oDgnElementList.begin();itr != m_pDimLineElm->m_oDgnElementList.end();++itr)
		{
			///(*itr)->Select( modelRef );
		}
	}

	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-22
    @class      CDimElement
    @function   FindConnectedPipeRunFrom
    @return     int
    @param      vector<CPipeRoutine::CPipeRun*>&    oPipeRunList
    @brief		get connected pipe run
******************************************************************************/
int CDimElement::FindConnectedPipeRunFrom(vector<CPipeRoutine::CPipeRun*>& oPipeRunList)
{
	m_pConnectedPipeRun = NULL;

	vector<DPoint3d> oExtPtList;
	if(ERROR_SUCCESS == GetExtPointList(oExtPtList))
	{
		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
		const double dAngleToler = docData.GetAngleToler();
		const double dToler = docData.m_dDrawingToler*200;	/// 20 times text height

		const double dDimLength = ::DistanceBetween(oExtPtList[0] , oExtPtList[1]);

		DPoint3d ptMid;
		///NOTICE: does it need to get mid point from text when dim type is 2?? - 2011.11.24 - modified by humkyung
		if(2 == this->GetDimType())
		{
			CDgnText* pTextElm = static_cast<CDgnText*>(m_oDgnElementList[0]);
			ptMid = pTextElm->center();
		}
		else
		{
			ptMid.x = (oExtPtList[0].x + oExtPtList[1].x)*0.5;
			ptMid.y = (oExtPtList[0].y + oExtPtList[1].y)*0.5;
			ptMid.z = (oExtPtList[0].z + oExtPtList[1].z)*0.5;
		}

		/// get direction along extension line from mid point
		DVector3d dir = GetExtendLineDirection();
		const double dExtendDist = ::DistanceBetween((dir.end) , (dir.org));	/// 2011.10.31 added by humkyung
		DVec3d vec;
		vec.x = dir.end.x - dir.org.x;
		vec.y = dir.end.y - dir.org.y;
		vec.z = dir.end.z - dir.org.z;

		DVector3d l;
		l.org = ptMid;
		l.end.x = ptMid.x + vec.x;
		l.end.y = ptMid.y + vec.y;
		l.end.z = ptMid.z + vec.z;
		/// up to here

		const double angle1 = angle();
		double dMinDist = DBL_MAX;
		for(vector<CPipeRoutine::CPipeRun*>::iterator itr = oPipeRunList.begin();itr != oPipeRunList.end();++itr)
		{
			const double angle2 = (*itr)->angle();
			///NOTICE: 360과의 비교는??
			if(::IsParallelAngle(angle1 , angle2 , dAngleToler)) /// 기울기가 비슷해야 한다.
			{
				/// slop area mark일 경우는 길이가 같아야 한다... - 2011.12.13 added by humkyung
				CIsoElement* pStartElm = (*itr)->GetStartElm();
				if(pStartElm && pStartElm->IsKindOf(CSlopeAreaMarkElement::TypeString()))
				{
					if(fabs(dDimLength - (*itr)->length()) > docData.m_dDrawingToler)
					{
						continue;
					}
				}
				/// up to here

				DPoint3d intersect;
				if(SUCCESS == (*itr)->IntersectWith(intersect , &l))
				{
					const double d = ::DistanceBetween(intersect , ptMid);
					if(d < dToler)
					{
						DVec3d _vec;
						_vec.x = intersect.x - ptMid.x;
						_vec.y = intersect.y - ptMid.y;
						_vec.z = intersect.z - ptMid.z;
						
						const double rad = ::AngleBetweenVectors(vec , _vec);
						const double dAngleBetween = RAD2DEG(rad);
						/// intersection point가 extend direction방향에 있어야 한다.(angle로 확인)
						if( (fabs(dAngleBetween) < dAngleToler) && (d < dMinDist))
						{
							dMinDist = d;
							m_pConnectedPipeRun = (*itr);
						}
					}
				}
			}
		}

		return ERROR_SUCCESS;
	}

	return ERROR_BAD_ENVIRONMENT;
}

/******************************************************************************
    @author     humkyung
    @date       2011-12-05
    @class      CDimElement
    @function   FindConnectedPipeRunFrom
    @return     int
    @param      vector<CDimElement*>&   oDimElmList
    @brief
******************************************************************************/
int CDimElement::FindConnectedPipeRunFrom(vector<CIsoElement*>* pDimElmList)
{
	assert(pDimElmList && "pDimElmList is NULL");

	if(pDimElmList)
	{
		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
		const double dAngleToler = docData.GetAngleToler();
		const double dToler = docData.m_dDrawingToler*10;

		const double angle1 = this->angle();

		for(vector<CIsoElement*>::iterator itr = pDimElmList->begin();itr != pDimElmList->end();++itr)
		{
			if(this == (*itr)) continue;

			CDimElement* pDimElm = static_cast<CDimElement*>(*itr);
			if(NULL != pDimElm->GetConnectedPipeRun())
			{
				const double angle2 = pDimElm->angle();

				if(::IsParallelAngle(angle1 , angle2 , dAngleToler))
				{
					const double d = this->DistanceTo(*pDimElm);
					if( d < dToler)
					{
						m_pConnectedPipeRun = pDimElm->m_pConnectedPipeRun;
						break;
					}
				}
			}
		}

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-22
    @class      CDimElement
    @function   GetConnectedPipeRun
    @return     CPipeRoutine::CPipeRun*
    @brief		return connected pipe run
******************************************************************************/
CPipeRoutine::CPipeRun* CDimElement::GetConnectedPipeRun() const
{
	return m_pConnectedPipeRun;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-20
    @class      CDimElement
    @function   CompareWith
    @return     int
    @param      CDimElement*    pDimElm
    @brief		compare two dimensions(return 0 if two dimension text are equal)
******************************************************************************/
int CDimElement::CompareWith( CDimElement* pDimElm )
{
	assert(pDimElm && "pDimElm is NULL");

	if(pDimElm)
	{
		CDgnText* pTextElm1 = GetDimTextElement();
		CDgnText* pTextElm2 = pDimElm->GetDimTextElement();

		if((NULL != pTextElm1) && (NULL != pTextElm2))
		{
			return CString(pTextElm1->textString()).Compare(CString(pTextElm2->textString()));
		}
		else if(NULL != pTextElm1)
		{
			return 1;
		}

		return -1;
	}

	return 1;
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-09
    @class      CDimElement
    @function   GetShapeForCloudMark
    @return     int
    @brief		get shape for cloudmark
******************************************************************************/
int CDimElement::GetShapeForCloudMark(vector<DPoint3d>& oEnclosePolygon)
{
	CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
	const double dCloudOffset = docData.GetCloudOffset();

	vector<DPoint3d> oExtPtList;
	this->GetExtPointList(oExtPtList);
	if(NULL != m_pConnectedPipeRun)
	{
		DVector3d dir = this->GetExtendLineDirection();
		DVec3d vec;
		vec.x = dir.end.x - dir.org.x;
		vec.y = dir.end.y - dir.org.y;
		double l = sqrt(vec.x*vec.x + vec.y*vec.y);
		vec.x /= l;
		vec.y /= l;

		for(vector<DPoint3d>::iterator itr = oExtPtList.begin();itr != oExtPtList.end();++itr)
		{
			itr->x -= vec.x * dCloudOffset;
			itr->y -= vec.y * dCloudOffset;
		}

		DVec3d hvec;
		hvec.x = oExtPtList[1].x - oExtPtList[0].x;
		hvec.y = oExtPtList[1].y - oExtPtList[0].y;
		l = sqrt(hvec.x*hvec.x + hvec.y*hvec.y);
		hvec.x /= l;
		hvec.y /= l;
		oExtPtList[0].x -= hvec.x * dCloudOffset;
		oExtPtList[0].y -= hvec.y * dCloudOffset;
		oExtPtList[1].x += hvec.x * dCloudOffset;
		oExtPtList[1].y += hvec.y * dCloudOffset;

		DPoint3d pt = m_pConnectedPipeRun->m_ptStart;
		pt.x += (vec.x - hvec.x) * dCloudOffset;
		pt.y += (vec.y - hvec.y) * dCloudOffset;
		oExtPtList.push_back( pt );

		pt = m_pConnectedPipeRun->m_ptEnd;
		pt.x += (vec.x + hvec.x) * dCloudOffset;
		pt.y += (vec.y + hvec.y) * dCloudOffset;
		oExtPtList.push_back( pt );
	}
	{
		vector<DPoint3d> _oEnclosePolygon = GetDimTextEnclosePolygon();
		ExpandEnclosePolygon( _oEnclosePolygon , dCloudOffset , dCloudOffset*0.5 );
		oExtPtList.insert( oExtPtList.end() , _oEnclosePolygon.begin() , _oEnclosePolygon.end() );
	}

	CConvexHull oConvexHull;
	oEnclosePolygon = oConvexHull.Create( oExtPtList );
	oEnclosePolygon.push_back( oEnclosePolygon.front() );

	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-09
    @class      CDimElement
    @function   GetDimType
    @return     int
    @brief
******************************************************************************/
int CDimElement::GetDimType( ) const
{
	return m_iDimType;
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-09
    @class      CDimElement
    @function   SetDimType
    @return     int
    @param      const   int&
    @param      iType
    @brief
******************************************************************************/
int CDimElement::SetDimType( const int& iType )
{
	m_iDimType = iType;
	
	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2011-12-05
    @class      CDimElement
    @function   DistanceTo
    @return     double
    @param      const   CDimElement&
    @param      oDimElm
    @brief		return the distance between two dimensions
******************************************************************************/
double CDimElement::DistanceTo( const CDimElement& oDimElm )
{
	double dMinDist = DBL_MAX;

	vector<DPoint3d> oExtPtList1 , oExtPtList2;
	this->GetExtPointList(oExtPtList1);
	oDimElm.GetExtPointList(oExtPtList2);
	for(int i = 0;i < int(oExtPtList1.size());++i)
	{
		for(int j = 0;j < int(oExtPtList2.size());++j)
		{
			const double d = ::DistanceBetween(oExtPtList1[i] , oExtPtList2[j]);
			if(d < dMinDist)
			{
				dMinDist = d;
			}
		}
	}

	return dMinDist;
}

/******************************************************************************
    @author     humkyung
    @date       2011-12-22
    @class      CDimElement
    @function   GetExtLineElement
    @return     CDimExtLineElement*
    @brief		return extension dim line element
******************************************************************************/
CDimExtLineElement* CDimElement::GetExtLineElement()
{
	return m_pExtLineElm;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-27
    @class      CDimElement
    @function   DrawCloudMark
    @return     int
    @param      DgnModelRefP    modelRef
    @brief		draw cloud mark for dimension
******************************************************************************/
int CDimElement::DrawCloudMark(CDgnDocument* pDgnDoc)
{
	assert(pDgnDoc && "pDgnDoc is NULL");

	if(pDgnDoc)
	{
		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();

		vector<DPoint3d> oEnclosePolygon;
		GetShapeForCloudMark( oEnclosePolygon );
		oEnclosePolygon.push_back( oEnclosePolygon.front() );

		const double scale = docData.m_dScale;
		const double fArcMin = (CIsoModelDocData::m_dDrawingToler*2) / scale;	/// in uor
		const double fArcMax = (CIsoModelDocData::m_dDrawingToler*20) / scale;
		const UInt32 uiLevel = docData.GetCloudLevel();
		const UInt32 uiColor = docData.GetCloudColor();
		const double dHeight = docData.GetCloudHeight();
		const double dWidth = docData.GetCloudWidth();
		const CString RevNo = docData.GetRevisionNo( pDgnDoc );
		const CString Display = docData.GetCloudDisplay();
		for(vector<DPoint3d>::iterator itr = oEnclosePolygon.begin();itr != oEnclosePolygon.end();++itr)
		{
			itr->x /= scale;
			itr->y /= scale;
			itr->z /= scale;
		}
		DrawCloud(&(oEnclosePolygon[0]) , oEnclosePolygon.size() , fArcMin, fArcMax , 
			   uiLevel , uiColor , 0 , 0 , 1 , pDgnDoc);
		if(_T("Cloud&RevNo") == Display)
		{
			DPoint3d StartPointForRevNo = CIsoElement::GetStartPointForRevNo(oEnclosePolygon); 
			Draw_RevNo(&StartPointForRevNo, RevNo , uiLevel , uiColor , 0 , 0 , dHeight , dWidth , pDgnDoc);
		}

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

void CDimElement::Check(CDgnDocument* pDgnDoc , CString sColor)
{
	CIsoElement::Check( pDgnDoc , sColor );

	if(m_pLeaderLineElm) m_pLeaderLineElm->Check( pDgnDoc , sColor );
	if(m_pDimLineElm) m_pDimLineElm->Check( pDgnDoc , sColor );
	if(m_pExtLineElm) m_pExtLineElm->Check( pDgnDoc , sColor );

	/// 텍스트를 둘러싸고 있는 polygon에 포함되는 line string을 구한다.
	CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
	const double dOffset = docData.GetOffset();

	vector<DPoint3d> oEnclosePolygon = GetDimTextEnclosePolygon();
	ExpandEnclosePolygon( oEnclosePolygon , dOffset , dOffset*0.5);
	oEnclosePolygon.push_back(oEnclosePolygon[0]);
	::DrawPolyline(pDgnDoc , oEnclosePolygon , sColor);

	{
		vector<DPoint3d> oExtPtList;
		if(ERROR_SUCCESS == GetExtPointList(oExtPtList))
		{
			DVector3d dir = GetExtendLineDirection();

			DVec3d vec;
			vec.x = dir.end.x - dir.org.x;
			vec.y = dir.end.y - dir.org.y;
			vec.z = dir.end.z - dir.org.z;

			DPoint3d pt;
			pt.x = oExtPtList[1].x + vec.x;
			pt.y = oExtPtList[1].y + vec.y;
			pt.z = oExtPtList[1].z + vec.z;
			oExtPtList.push_back( pt );

			pt.x = oExtPtList[0].x + vec.x;
			pt.y = oExtPtList[0].y + vec.y;
			pt.z = oExtPtList[0].z + vec.z;
			oExtPtList.insert(oExtPtList.begin() ,  pt );

			::DrawPolyline(pDgnDoc , oExtPtList , sColor);

			if(NULL != m_pConnectedPipeRun)
			{
				GetExtPointList(oExtPtList);

				vector<DPoint3d> pts;
				DPoint3d pt;
				pt.x = (oExtPtList[0].x + oExtPtList[1].x)*0.5;
				pt.y = (oExtPtList[0].y + oExtPtList[1].y)*0.5;
				pt.z = 0.0;
				pts.push_back(pt);

				pt.x = (m_pConnectedPipeRun->m_ptStart.x + m_pConnectedPipeRun->m_ptEnd.x)*0.5;
				pt.y = (m_pConnectedPipeRun->m_ptStart.y + m_pConnectedPipeRun->m_ptEnd.y)*0.5;
				pt.z = 0.0;
				pts.push_back(pt);

				::DrawPolyline(pDgnDoc , pts , sColor);
			}
		}
		
		if(2 == m_iDimType)
		{
			CDgnText* pTextElm = static_cast<CDgnText*>(m_oDgnElementList[0]);
			DPoint3d center = pTextElm->center();
			oExtPtList.clear();
			
			center.x -= dOffset; center.y -= dOffset;
			oExtPtList.push_back(center);
			center.x += dOffset*2; center.y += dOffset*2;
			oExtPtList.push_back(center);
			center.x -= dOffset; center.y -= dOffset;
			oExtPtList.push_back(center);
			center.x += dOffset; center.y -= dOffset;
			oExtPtList.push_back(center);
			center.x -= dOffset*2; center.y += dOffset*2;
			oExtPtList.push_back(center);
			::DrawPolyline(pDgnDoc , oExtPtList , sColor);
		}
	}

	for(vector<CIsoElement*>::iterator itr = m_oSubDimElmList.begin();itr != m_oSubDimElmList.end();++itr)
	{
		(*itr)->Check( pDgnDoc , sColor );
	}
}