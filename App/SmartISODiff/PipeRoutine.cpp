#include "StdAfx.h"
#include <assert.h>
#include <float.h>
#include "ElbowElement.h"
#include "DimElement.h"
#include "TeeElement.h"
#include "OletElement.h"
#include "ReducerElement.h"
#include "InlineElement.h"
#include "RWeldElement.h"
#include "PipeRoutine.h"
#include "IsoModel.h"
#include "IsoArea.h"
#include "IsoModelDocData.h"
#include "ConvexHull.h"
#include "IsoElement.h"
#include "aLineDiff/diff.h"

#include <algorithm>

using namespace diff;

using namespace IsoElement;

CPipeRoutine::CPipeRun::CPipeRun(CIsoElement* pStartElm , DPoint3d ptStart , DPoint3d ptEnd , STRING_T sTypeString)
{
	if(NULL != pStartElm) m_oElmList.push_back( pStartElm );
	m_sTypeString = sTypeString;

	m_ptStart = ptStart;
	m_ptEnd   = ptEnd;

	m_dAngle = DBL_MAX;
}

CPipeRoutine::CPipeRun::~CPipeRun()
{
	int d = 1;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-25
    @class      CPipeRoutine
    @function   CPipeRun::IsStartElmBranch
    @return     bool
    @brief		check start elm is branch?
******************************************************************************/
bool CPipeRoutine::CPipeRun::IsStartElmBranch() const
{
	if(!m_oElmList.empty())
	{
		return m_oElmList[0]->IsBranchElm();
	}

	return false;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-25
    @class      CPipeRoutine
    @function   CPipeRun::GetStartElm
    @return     CIsoElement*
    @brief		return start element
******************************************************************************/
CIsoElement* CPipeRoutine::CPipeRun::GetStartElm() const
{
	if(!m_oElmList.empty())
	{
		return m_oElmList[0];
	}

	return NULL;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-25
    @class      CPipeRoutine
    @function   CPipeRun::typeString
    @return     CString
    @brief
******************************************************************************/
STRING_T CPipeRoutine::CPipeRun::typeString() const
{
	STRING_T sTypeString;

	if(_T("AUTO") != m_sTypeString)
	{
		sTypeString = m_sTypeString.c_str();
	}
	else
	{
		sTypeString = m_oElmList[0]->typeString();
	}
	sTypeString += _T("-");
	if(!m_oDimElmList.empty()) sTypeString += STRING_T(m_oDimElmList[0]->GetDimTextElement()->textString());
	sTypeString += _T("/");

	return sTypeString;
}

/******************************************************************************
    @author     humkyung
    @date       2011-12-13
    @class      CPipeRoutine
    @function   CPipeRun::length
    @return     double
    @brief
******************************************************************************/
double CPipeRoutine::CPipeRun::length() const
{
	return ::DistanceBetween(m_ptStart , m_ptEnd);
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-28
    @class      CPipeRoutine
    @function   CPipeRun::angle
    @return     double
    @brief		return angle of pipe run
******************************************************************************/
double CPipeRoutine::CPipeRun::angle() const
{
	if(DBL_MAX != m_dAngle) return m_dAngle;

	DVec3d vec;
	vec.x = m_ptEnd.x - m_ptStart.x;
	vec.y = m_ptEnd.y - m_ptStart.y;
	vec.z = m_ptEnd.z - m_ptStart.z;

	double angle = ::AngleOf(vec);
	if(angle < 0.0) angle += 2*PI;
	return RAD2DEG(angle);
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-03
    @class      CPipeRoutine
    @function   CPipeRun::SetAngle
    @return     int
    @param      const   double&
    @param      angle
    @brief
******************************************************************************/
int CPipeRoutine::CPipeRun::SetAngle( const double& angle )
{
	m_dAngle = angle;

	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-03
    @class      CPipeRoutine
    @function   CPipeRun::AppendElm
    @return     int
    @param      CIsoElement*    pIsoElm
    @brief
******************************************************************************/
int CPipeRoutine::CPipeRun::AppendElement(CIsoElement* pIsoElm)
{
	assert(pIsoElm && "pIsoElm is NULL");
	if(pIsoElm)
	{
		m_oElmList.push_back( pIsoElm );
		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-09
    @class      CPipeRoutine
    @function   CPipeRun::GetShapeForCloudMark
    @return     int
    @param      vector<DPoint3d>&   oShapeForCloudMark
    @brief		get shape for cloud mark
******************************************************************************/
int CPipeRoutine::CPipeRun::GetShapeForCloudMark(vector<DPoint3d>& oShapeForCloudMark)
{
	if(!m_oDimElmList.empty())
	{
		m_oDimElmList[0]->GetShapeForCloudMark( oShapeForCloudMark );

		/*for(vector<CIsoElement*>::iterator itr = m_oElmList.begin();itr != m_oElmList.end();++itr)
		{
			if((*itr)->IsKindOf(CPipeSupportElement::TypeString()))
			{
				CPipeSupportElement* pPipeSuppElm = static_cast<CPipeSupportElement*>(*itr);
				CAttributeElement* pAttrElm = pPipeSuppElm->GetAttrElement();
				if(pAttrElm)
				{
					pAttrElm->GetShapeForCloudMark(oShapeForCloudMark);
				}
			}
		}*/

		return ERROR_SUCCESS;
	}

	return ERROR_BAD_ENVIRONMENT;
}

/******************************************************************************
    @author     humkyung
    @date       2011-12-22
    @class      CPipeRoutine
    @function   CPipeRun::DrawCloudMarkForAttr
    @return     int
    @param      DgnModelRefP    modelRef
    @brief
******************************************************************************/
int CPipeRoutine::CPipeRun::DrawCloudMarkForAttr( CDgnDocument* pDgnDoc )
{
	assert(pDgnDoc && "pDgnDoc is NULL");
	if(pDgnDoc)
	{
		for(vector<CIsoElement*>::iterator itr = m_oElmList.begin();itr != m_oElmList.end();++itr)
		{
			if((*itr)->IsKindOf(CPipeSupportElement::TypeString()))
			{
				CPipeSupportElement* pPipeSuppElm = static_cast<CPipeSupportElement*>(*itr);
				pPipeSuppElm->DrawCloudMark( pDgnDoc );
			}
		}

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-22
    @class      CPipeRoutine
    @function   CPipeRun::Select
    @return     int
    @param      DgnModelRefP    modelRef
    @brief
******************************************************************************/
int CPipeRoutine::CPipeRun::DrawCloudMark(bool bIncludeDimLine , CDgnDocument* pDgnDoc)
{
	assert(pDgnDoc && "pDgnDoc is NULL");
	if(pDgnDoc)
	{
		if(!m_oDimElmList.empty())
		{
			return m_oDimElmList[0]->DrawCloudMark( pDgnDoc );
		}

		return ERROR_BAD_ENVIRONMENT;
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-22
    @class      CPipeRoutine
    @function   CompareWith
    @return     int
    @param      CPipeRun*   pPipeRun
    @brief		compare two dimensions
******************************************************************************/
int CPipeRoutine::CPipeRun::CompareWith( CPipeRun* pPipeRun )
{
	assert(pPipeRun && "pPipeRun is NULL");
	if(pPipeRun)
	{
		if(!m_oDimElmList.empty() && (!pPipeRun->m_oDimElmList.empty()))
		{
			const int iSize1 = m_oElmList.size();
			const int iSize2 = pPipeRun->m_oElmList.size();
			for(int i = 0;i < iSize1;++i)
			{
				if(i < iSize2)
				{
					///TODO: do something	
				}
				else
				{
					m_oElmList[i]->DrawCloudMark( NULL );
				}
			}

			return m_oDimElmList[0]->CompareWith( pPipeRun->m_oDimElmList[0] );
		}

		return ERROR_BAD_ENVIRONMENT;
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-22
    @class      CPipeRun
    @function   IntersectWith
    @return     int
    @param      DPoint3d&   intersect
    @param      DVector3d*  l
    @brief
******************************************************************************/
int CPipeRoutine::CPipeRun::IntersectWith(DPoint3d& intersect , DVector3d* l)
{
	assert(l && "l is NULL");
	if(l)
	{
		DVector3d l2;
		l2.org = m_ptStart;
		l2.end = m_ptEnd;
		if(SUCCESS == ::IntersectWith(intersect , (*l) , l2))
		{
			DPoint3d ptCenter;
			ptCenter.x = (l2.org.x + l2.end.x)*0.5;
			ptCenter.y = (l2.org.y + l2.end.y)*0.5;
			ptCenter.z = (l2.org.z + l2.end.z)*0.5;
			const double dRadius = ::DistanceBetween(ptCenter , l2.org);
			const double dist = ::DistanceBetween(ptCenter , intersect);
			if(dist <= dRadius) return SUCCESS;
		}
	}

	return (!SUCCESS); 
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-28
    @class      CPipeRoutine
    @function   CPipeRun::Length
    @return     double
    @brief
******************************************************************************/
double CPipeRoutine::CPipeRun::Length( )
{
	return ::DistanceBetween(m_ptStart , m_ptEnd);
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-28
    @class      CPipeRoutine
    @function   CPipeRun::DistanceTo
    @return     double
    @param      DPoint3d    pt
    @brief		return the distance between pipe run and pt
******************************************************************************/
double CPipeRoutine::CPipeRun::DistanceTo( DPoint3d pt )
{
	DPoint3d l[2]={0,};
	l[0] = m_ptStart;
	l[1] = m_ptEnd;
	return DistanceFromPointToLine(pt , l);
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-28
    @class      CPipeRoutine
    @function   CPipeRun::AssignDimension
    @return     int
    @param      vector<CIsoElement*>&   oDimElmList
    @brief
******************************************************************************/
int CPipeRoutine::CPipeRun::AssignDimension(vector<CIsoElement*>& oDimElmList)
{
	CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
	const double dToler = CIsoModelDocData::m_dDrawingToler;

	if(!oDimElmList.empty())
	{
		std::stable_sort(oDimElmList.begin() , oDimElmList.end() , CDimElement::SortByLognerLength());

		CDimElement* pLongestDimElm = static_cast<CDimElement*>(oDimElmList[0]);
		m_oDimElmList.push_back( pLongestDimElm );

		vector<DPoint3d> oExtPtList;
		pLongestDimElm->GetExtPointList(oExtPtList);
		DPoint3d ptMid;
		ptMid.x = (oExtPtList[0].x + oExtPtList[1].x)*0.5;
		ptMid.y = (oExtPtList[0].y + oExtPtList[1].y)*0.5;
		ptMid.z = (oExtPtList[0].z + oExtPtList[1].z)*0.5;
		const double dMaxDist = DistanceTo(ptMid);
		oDimElmList.erase( oDimElmList.begin() );

		for(vector<CIsoElement*>::iterator itr = oDimElmList.begin();itr != oDimElmList.end();/*++itr*/)
		{
			CDimElement* pDimElm = static_cast<CDimElement*>( *itr );

			pDimElm->GetExtPointList(oExtPtList);
			ptMid.x = (oExtPtList[0].x + oExtPtList[1].x)*0.5;
			ptMid.y = (oExtPtList[0].y + oExtPtList[1].y)*0.5;
			ptMid.z = (oExtPtList[0].z + oExtPtList[1].z)*0.5;
			const double d = DistanceTo(ptMid);
			if(fabs(d - dMaxDist) < dToler)
			{
				m_oDimElmList.push_back( pDimElm );
				itr = oDimElmList.erase( itr );
				continue;
			}
			++itr;
		}

		for(vector<CDimElement*>::iterator itr = m_oDimElmList.begin();itr != m_oDimElmList.end();++itr)
		{
			(*itr)->CollectSubDimension( &oDimElmList );
		}

		///assert(oDimElmList.empty());	///NOTICE: 에러처리는 어떻게 - 2011.11.11
	}

	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-28
    @class      CPipeRoutine
    @function   CPipeRun::Separate
    @return     int
    @param      vector<CPipeRoutine::CPipeRun*>&    oPipeRunList
    @brief
******************************************************************************/
int CPipeRoutine::CPipeRun::Separate( vector<CPipeRoutine::CPipeRun*>& oPipeRunList )
{
	CIsoElement* pStartElm = GetStartElm();

	std::stable_sort(m_oDimElmList.begin() , m_oDimElmList.end() , CDimElement::SortByDistance(m_ptStart));
	for(vector<CDimElement*>::iterator itr = m_oDimElmList.begin() + 1;itr != m_oDimElmList.end();/*++itr*/)
	{
		///TODO: 좌표와 element를 다시 계산하여 넘겨줄수 있도록 한다.
		CPipeRun* pPipeRun = new CPipeRun(pStartElm , m_ptStart , m_ptEnd);
		pPipeRun->m_oDimElmList.push_back( *itr );
		oPipeRunList.push_back( pPipeRun );

		itr = m_oDimElmList.erase( itr );
	}

	return ERROR_SUCCESS;
}

int CPipeRoutine::CPipeRun::Check( CDgnDocument* pDgnDoc , CString sColor )
{
	for(vector<CDimElement*>::iterator itr = m_oDimElmList.begin();itr != m_oDimElmList.end();++itr)
	{
		(*itr)->Check( pDgnDoc , sColor );
	}

	vector<DPoint3d> oPolyline;
	oPolyline.push_back( m_ptStart );
	oPolyline.push_back( m_ptEnd );
	::DrawPolyline(pDgnDoc , oPolyline , sColor);
	
	return ERROR_SUCCESS;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CPipeRoutine::CPipeRoutine(CIsoArea* pIsoArea , CIsoElement* pStartElm) : m_pIsoArea(pIsoArea) , m_pStartElm(pStartElm)
{
}

/******************************************************************************
    @author     humkyung
    @date       ????.??.??
    @class      CPipeRoutine
    @function   ~CPipeRoutine
    @return
    @param      void
    @brief		delete pipe runs
******************************************************************************/
CPipeRoutine::~CPipeRoutine(void)
{
	try
	{
		CleanUpPipeRunList();
	}
	catch(...)
	{
	}
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-25
    @class      CPipeRoutine
    @function   GetIsoArea
    @return     CIsoArea*
    @brief		return iso area pointer
******************************************************************************/
CIsoArea* CPipeRoutine::GetIsoArea() const
{
	return m_pIsoArea;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-10
    @class      CPipeRoutine
    @function   Add
    @return     int
    @param      CIsoElement*    pIsoElm
    @brief		add a pipe element to pipe routine(also connection points are inserted)
******************************************************************************/
int CPipeRoutine::Add( DPoint3d pt , CIsoElement* pIsoElm )
{
	assert(pIsoElm && "pIsoElm is NULL");

	if(pIsoElm)
	{
		if(m_ptConnList.empty())
		{
			vector<DPoint3d> pts;
			CPipeElement* pPipeElm = (CPipeElement*)(pIsoElm);
			pPipeElm->GetConnPointList(pts);
			if(pts.size() > 1)
			{
				const int iSize = pts.size();
				m_ptConnList.push_back( pts[0] );
				m_ptConnList.push_back( pts[iSize - 1] );
			}
			else
			{
				m_ptConnList.push_back( pt );
			}
			
			m_oPipeElmList.push_back(pIsoElm);
		}
		else
		{
			DPoint3d ptNextConn = pt;
			vector<DPoint3d> oConnPtList;
			pIsoElm->GetConnPointList(oConnPtList);
			if(2 == oConnPtList.size())
			{
				const double d1 = ::DistanceBetween(oConnPtList[0] , pt);
				const double d2 = ::DistanceBetween(oConnPtList[1] , pt);
				
				ptNextConn = (d1 < d2) ? oConnPtList[1] : oConnPtList[0];
			}

			///NOTICE: 시작과 끝점이 비슷한 위치에 있을 경우에는 다음 연결점을 제대로 찾지 못할 경우가 있음 - 2011.11.11
			const int iSize = m_ptConnList.size();
			const double d1 = ::DistanceBetween(m_ptConnList[0] , pt);
			const double d2 = ::DistanceBetween(m_ptConnList[iSize - 1] , pt);
			if(d1 < d2)
			{
				m_ptConnList.insert(m_ptConnList.begin() , ptNextConn);
				m_oPipeElmList.insert(m_oPipeElmList.begin() , pIsoElm);

				if(2 == oConnPtList.size())
				{
					const double d1 = ::DistanceBetween(oConnPtList[0] , ptNextConn);
					const double d2 = ::DistanceBetween(oConnPtList[1] , ptNextConn);
					if(d1 > d2)
					{
						pIsoElm->Reverse();
					}
				}
			}
			else
			{
				if(2 == oConnPtList.size())
				{
					const double d1 = ::DistanceBetween(oConnPtList[0] , ptNextConn);
					const double d2 = ::DistanceBetween(oConnPtList[1] , ptNextConn);
					if(d1 < d2)
					{
						pIsoElm->Reverse();
					}
				}

				m_ptConnList.push_back(ptNextConn);
				m_oPipeElmList.push_back(pIsoElm);
			}
		}

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-11
    @class
    @function   ExistInConnPtList
    @return     bool
    @param      const   DPoint3d&
    @param      pt
    @brief
******************************************************************************/
bool CPipeRoutine::ExistInConnPtList( const DPoint3d& pt ) const
{
	for(vector<DPoint3d>::const_iterator itr = m_ptConnList.begin();itr != m_ptConnList.end();++itr)
	{
		if(CIsoElement::IsSamePoint((*itr) , pt))
		{
			return true;
		}
	}

	return false;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-17
    @class      CPipeRoutine
    @function   GetFittingElmConnectedTo
    @return     CIsoElement*
    @param      DPoint3d&       ptNextConn
    @param      const           DPoint3d&
    @param      ptConn          vector<CIsoElement*>*
    @param      pFittingElmList
    @brief		get fitting element which is connected to ptConn
******************************************************************************/
CIsoElement* CPipeRoutine::GetFittingElmConnectedTo(vector<DPoint3d>& oNextConnPtList , const DPoint3d& ptConn , vector<CIsoElement*>* pFittingElmList)
{
	assert(pFittingElmList && "pFittingElmList is NULL");

	if(pFittingElmList)
	{
		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
		const double dToler = CIsoModelDocData::m_dDrawingToler;

		for(vector<CIsoElement*>::iterator itr = pFittingElmList->begin();itr != pFittingElmList->end();++itr)
		{
			/// skip already added fitting
			vector<CIsoElement*>::iterator where = std::find(m_oPipeElmList.begin() , m_oPipeElmList.end() , *itr);
			if(m_oPipeElmList.end() != where) continue;
			/// up to here

			vector<DPoint3d> oConnPtList;
			(*itr)->GetConnPointList(oConnPtList);
			if(oConnPtList.size() > 1)
			{
				std::stable_sort(oConnPtList.begin() , oConnPtList.end() , CIsoElement::SortDPoint3dFrom(ptConn));
			}

			if(!oConnPtList.empty())
			{
				if(true == CIsoElement::IsSamePoint(ptConn , oConnPtList[0] , dToler))
				{
					/*if(oConnPtList.size() > 1)
					{
						for(vector<DPoint3d>::iterator jtr = oConnPtList.begin();jtr != oConnPtList.end();++jtr)
						{
							if(false == ExistInConnPtList(*jtr))
							{
								oNextConnPtList.push_back( *jtr );
							}
						}
					}
					else
					{
						oNextConnPtList.push_back( oConnPtList[0] );
					}*/
					oNextConnPtList.push_back( oConnPtList[ oConnPtList.size() - 1] );
					return (*itr);
				}
			}
		}
	}

	return NULL;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-10
    @class      CPipeRoutine
    @function   CollectConnectedPipe
    @return     int
    @param      vector<CIsoElement*>*   pPipeElmList
    @brief		collect pipes which could be in routine
******************************************************************************/
int CPipeRoutine::CollectConnectedPipe(vector<DPoint3d> oConnPtList , CIsoElement* pFrom , 
									   vector<CIsoElement*>* pPipeElmList , vector<CIsoElement*>* pFittingElmList )
{
	assert(pPipeElmList && "pPipeElmList is NULL");

	if(pPipeElmList)
	{
		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
		const double dToler = CIsoModelDocData::m_dDrawingToler;
		const double dAngleToler = docData.GetAngleToler();

		/// 2011.12.06 - added by humkyung
		if(NULL != pFrom)
		{
			m_oPipeElmList.push_back( pFrom );
			
			vector<DPoint3d> oConnPtList;
			pFrom->GetConnPointList(oConnPtList);
			m_ptConnList.insert(m_ptConnList.end() , oConnPtList.begin() , oConnPtList.end());

			vector<CIsoElement*>::iterator where = std::find(pPipeElmList->begin() , pPipeElmList->end() , pFrom);
			if(pPipeElmList->end() != where)
			{
				pPipeElmList->erase(where);
			}
			where = std::find(pFittingElmList->begin() , pFittingElmList->end() , pFrom);
			if(pFittingElmList->end() != where)
			{
				pFittingElmList->erase(where);
			}
		}
		/// up to here

		///NOTICE: GetFittingElmConnectedTo에서 NextConnPoint를 넘겨주는 부분 고려.... - 2011.11.11
		vector<DPoint3d> oNextConnPtList;
		bool bLoop = false;
		do
		{
			bLoop = false;	/// set initial value as 'false'
			oNextConnPtList.clear();

			for(vector<DPoint3d>::iterator itr = oConnPtList.begin();itr != oConnPtList.end();++itr)
			{
				vector<DPoint3d> _oNextConnPtList;
				if(NULL != pFittingElmList)
				{
					CIsoElement* pFittingElm = GetFittingElmConnectedTo(_oNextConnPtList , *itr , pFittingElmList);
					if(NULL != pFittingElm)
					{
						Add( *itr , pFittingElm );
						pFittingElmList->erase( std::find(pFittingElmList->begin() , pFittingElmList->end() , pFittingElm) );
						
						oNextConnPtList.insert( oNextConnPtList.end() , _oNextConnPtList.begin() , _oNextConnPtList.end() );

						/// check if there is pipe support for elbow - 2011.12.22 added by humkyung
						if(pFittingElm->IsKindOf(CElbowElement::TypeString()))
						{
							CElbowElement* pElbowElm = static_cast<CElbowElement*>(pFittingElm);
							DPoint3d center = pElbowElm->center();
							_oNextConnPtList.clear();
							pFittingElm = GetFittingElmConnectedTo(_oNextConnPtList , center , pFittingElmList);
							if(NULL != pFittingElm)
							{
								m_oPipeElmList.push_back(pFittingElm);
								pFittingElmList->erase( std::find(pFittingElmList->begin() , pFittingElmList->end() , pFittingElm) );
							}
						}
						/// up to here

						bLoop = true;
						continue;
					}
				}

				vector<CPipeElement*> oChacheElmList;
				/// 이미 routine에 들어가 있는 파이프는 리스트에 넣지 않는다.
				for(vector<CIsoElement*>::iterator jtr = pPipeElmList->begin();jtr != pPipeElmList->end();++jtr)
				{
					if(CPipeElement::TypeString() != (*jtr)->typeString()) continue;

					vector<CIsoElement*>::iterator where = std::find(m_oPipeElmList.begin() , m_oPipeElmList.end() , *jtr);
					if(m_oPipeElmList.end() != where) continue;
					oChacheElmList.push_back( (CPipeElement*)(*jtr) );
				}
				/// up to here

				if(!oChacheElmList.empty())
				{
					std::stable_sort( oChacheElmList.begin() , oChacheElmList.end() , CIsoElement::SortIsoElmByDistance( *itr ) );
					for(vector<CPipeElement*>::iterator jtr = oChacheElmList.begin();jtr != oChacheElmList.end();++jtr)
					{
						if((*jtr)->DistanceTo( *itr ) < dToler)
						{
							/// 다음 연결 포인터를 구한다.
							vector<DPoint3d> pts;
							oChacheElmList[0]->GetConnPointList(pts);
							if(!pts.empty())
							{
								const int iSize = pts.size();
								const double d1 = ::DistanceBetween((pts[0]) , (*itr));
								const double d2 = ::DistanceBetween((pts[iSize - 1]) , (*itr));
								(d1 < d2) ? oNextConnPtList.push_back( pts[iSize - 1] ) : oNextConnPtList.push_back( pts[0] );
							}
							/// up to here

							Add( *itr , oChacheElmList[0] );
							pPipeElmList->erase( std::find(pPipeElmList->begin() , pPipeElmList->end() , oChacheElmList[0]) );

							bLoop = true;
							break;
						}
					}
				}
			}

			oConnPtList.clear();
			oConnPtList.insert(oConnPtList.begin() , oNextConnPtList.begin() , oNextConnPtList.end());			
		}while(true == bLoop);

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-11
    @class      CPipeRoutine
    @function   DistanceTo
    @return     double
    @param      CPipeRoutine*   pTo
    @brief		return the distance between two pipe routines if two routines are collinear
******************************************************************************/
double CPipeRoutine::DistanceToIfCollinear( CPipeRoutine* pTo )
{
	assert(pTo && "pTo is NULL");

	if(pTo)
	{
		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
		const double dAngleToler = docData.GetAngleToler();

		const int iSize1 = m_ptConnList.size();
		const int iSize2 = pTo->m_ptConnList.size();

		if((iSize1 > 1) && (iSize2 > 1))
		{
			DPoint3d pts1[2]={0,}, pts2[2]={0,};
			
			double dMinDist = DBL_MAX;
			for(int i = 0;i < 2;++i)
			{
				if(0 == i)
				{
					pts1[0] = m_ptConnList[0];
					pts1[1] = m_ptConnList[1];
					/// 2011.11.14 added by humkyung
					if(!m_oPipeElmList.empty() && (CElbowElement::TypeString() == m_oPipeElmList[0]->typeString()))
					{
						CElbowElement* pElbowElm = static_cast<CElbowElement*>(m_oPipeElmList[0]);
						pts1[1] = pElbowElm->center();
					}
					/// up to here
				}
				else
				{
					pts1[0] = m_ptConnList[iSize1 - 1];
					pts1[1] = m_ptConnList[iSize1 - 2];
					/// 2011.11.14 added by humkyung
					if(!m_oPipeElmList.empty() && (CElbowElement::TypeString() == m_oPipeElmList.back()->typeString()))
					{
						CElbowElement* pElbowElm = static_cast<CElbowElement*>(m_oPipeElmList.back());
						pts1[1] = pElbowElm->center();
					}
					/// up to here
				}

				for(int j = 0;j < 2;++j)
				{
					if(0 == j)
					{
						pts2[0] = pTo->m_ptConnList[0];
						pts2[1] = pTo->m_ptConnList[1];
						/// 2011.11.14 added by humkyung
						if(!pTo->m_oPipeElmList.empty() && (CElbowElement::TypeString() == pTo->m_oPipeElmList[0]->typeString()))
						{
							CElbowElement* pElbowElm = static_cast<CElbowElement*>(pTo->m_oPipeElmList[0]);
							pts2[1] = pElbowElm->center();
						}
						/// up to here
					}
					else
					{
						pts2[0] = pTo->m_ptConnList[iSize2 - 1];
						pts2[1] = pTo->m_ptConnList[iSize2 - 2];
						/// 2011.11.14 added by humkyung
						if(!pTo->m_oPipeElmList.empty() && (CElbowElement::TypeString() == pTo->m_oPipeElmList.back()->typeString()))
						{
							CElbowElement* pElbowElm = static_cast<CElbowElement*>(pTo->m_oPipeElmList.back());
							pts2[1] = pElbowElm->center();
						}
						/// up to here
					}

					if(CIsoElement::Collinear(pts1 , pts2 , dAngleToler))
					{
						const double d = ::DistanceBetween((pts1[0]) , (pts2[0]));
						if(d < dMinDist)
						{
							dMinDist = d;
						}
					}
				}
			}

			return dMinDist;
		}
	}

	return DBL_MAX;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-28
    @class      CPipeRoutine
    @function   FindConnectedWeldMarkTo
    @return     CIsoElement*
    @param      DPoint3d                pt1
    @param      DPoint3d                pt2
    @param      vector<CIsoElement*>*   pAttrElmList
    @brief		find connected weld mark
******************************************************************************/
int CPipeRoutine::FindConnectedWeldMarkTo(DPoint3d pt1 , DPoint3d pt2 , vector<CIsoElement*>* pWeldMarkElmList)
{
	assert(pWeldMarkElmList && "pWeldMarkElmList is NULL");
	
	if(pWeldMarkElmList)
	{
		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
		const double dToler = CIsoModelDocData::m_dDrawingToler;

		DVector3d l1;
		l1.org = pt1;
		l1.end = pt2;
		DPoint3d ptMid;
		ptMid.x = (pt1.x + pt2.x)*0.5;
		ptMid.y = (pt1.y + pt2.y)*0.5;
		ptMid.z = (pt1.z + pt2.z)*0.5;
		const double radius = ::DistanceBetween(ptMid , pt1);
		
		vector<CIsoElement*> oWeldMarkElmList;
		for(vector<CIsoElement*>::iterator itr = pWeldMarkElmList->begin();itr != pWeldMarkElmList->end();++itr)
		{
			CDgnVolume volume = (*itr)->volume();

			if(volume.Contains(pt1) || volume.Contains(pt2))
			{
				oWeldMarkElmList.push_back( *itr );
			}
		}

		return oWeldMarkElmList.size();
	}

	return 0;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-28
    @class      CPipeRoutine
    @function   Reverse
    @return     int
    @brief		reverse contents of pipe routine
******************************************************************************/
int CPipeRoutine::Reverse()
{
	std::reverse(m_ptConnList.begin() , m_ptConnList.end());
	std::reverse(m_oPipeElmList.begin() , m_oPipeElmList.end());
	for(vector<CIsoElement*>::iterator itr = m_oPipeElmList.begin();itr != m_oPipeElmList.end();++itr)
	{
		(*itr)->Reverse();
	}

	/// re-create pipe run if pipe run exists
	if(!m_oPipeRunList.empty())
	{
		throw exception("unexpected environment");
	}

	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-04
    @class      CPipeRoutine
    @function   AlignToCompare
    @return     int
    @param      
    @brief		align pipe routine(left/top -> right/bottom)
******************************************************************************/
int CPipeRoutine::AlignToCompare()
{
	if(!m_ptConnList.empty())
	{
		if(NULL == m_pStartElm)
		{
			/// 좌상->우하 기준으로 정렬
			const int iSize = m_ptConnList.size();
			if( (m_ptConnList[0].x > m_ptConnList[iSize - 1].x) || 
				((m_ptConnList[0].x == m_ptConnList[iSize - 1].x) && (m_ptConnList[0].y < m_ptConnList[iSize - 1].y)))
			{
				this->Reverse();
			}
		}
		else
		{
			/// reverse if routine start from start element when start element exists - 2011.11.10 added by humkyung
			DPoint3d ptStart={0,};
			if(CTeeElement::TypeString() == m_pStartElm->typeString())
			{
				CTeeElement* pTeeElm = static_cast<CTeeElement*>(m_pStartElm);
				ptStart = pTeeElm->GetStartPointOfBranch();
			}
			else if(COletElement::TypeString() == m_pStartElm->typeString())
			{
				COletElement* pOletElm = static_cast<COletElement*>(m_pStartElm);
				ptStart = pOletElm->GetStartPointOfBranch();
			}
			else if(CRWeldElement::TypeString() == m_pStartElm->typeString())
			{
				CRWeldElement* pRWeldElm = static_cast<CRWeldElement*>(m_pStartElm);
				ptStart = pRWeldElm->GetStartPointOfBranch();
			}
			else
			{
				return ERROR_SUCCESS;
			}

			vector<DPoint3d> oConnPtList;
			m_oPipeElmList[0]->GetConnPointList(oConnPtList);
			for(vector<DPoint3d>::iterator itr = oConnPtList.begin();itr != oConnPtList.end();++itr)
			{
				if(CIsoElement::IsSamePoint( ptStart , *itr ))
				{
					return ERROR_SUCCESS;
				}
			}

			this->Reverse();
			/// up to here
		}
	}
	
	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-11
    @class      CPipeRoutine
    @function   Join
    @return     int
    @param      CPipeRoutine*   pPipeRoutine
    @brief		join two pipe routines
******************************************************************************/
int CPipeRoutine::Join( CPipeRoutine* pPipeRoutine , vector<CIsoElement*>* pWeldMarkElmList )
{
	assert(pPipeRoutine && "pPipeRoutine is NULL");
	
	if(pPipeRoutine)
	{
		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
		const double dToler = CIsoModelDocData::m_dDrawingToler;

		/// set start element - 2011.12.02 added by humkyung
		if((NULL == this->m_pStartElm) && (NULL != pPipeRoutine->m_pStartElm))
		{
			this->m_pStartElm = pPipeRoutine->m_pStartElm;
		}
		/// up to here

		if(m_ptConnList.empty())
		{
			m_ptConnList.insert( m_ptConnList.begin() , pPipeRoutine->m_ptConnList.begin() , pPipeRoutine->m_ptConnList.end() );		
			m_oPipeElmList.insert(m_oPipeElmList.begin() , pPipeRoutine->m_oPipeElmList.begin() , pPipeRoutine->m_oPipeElmList.end() );
			m_oPipeRunList.insert(m_oPipeRunList.begin() , pPipeRoutine->m_oPipeRunList.begin() , pPipeRoutine->m_oPipeRunList.end() );
			
			return ERROR_SUCCESS;
		}
		else
		{
			const int iSize1 = m_ptConnList.size();
			const int iSize2 = pPipeRoutine->m_ptConnList.size();
			double dDist[4] = {0.0,0.0,0.0,0.0};
			dDist[0] = ::DistanceBetween(m_ptConnList[0] , (pPipeRoutine->m_ptConnList[0]));
			dDist[1] = ::DistanceBetween(m_ptConnList[0] , (pPipeRoutine->m_ptConnList[iSize2 - 1]));
			dDist[2] = ::DistanceBetween(m_ptConnList[iSize1 - 1] , (pPipeRoutine->m_ptConnList[0]));
			dDist[3] = ::DistanceBetween(m_ptConnList[iSize1 - 1] , (pPipeRoutine->m_ptConnList[iSize2 - 1]));

			/// 2011.11.25 added by humkyung
			bool bNeedCheckWeldMark[4]={false,false,false,false};
			if((this->GetPipeElmCount() > 0) && (pPipeRoutine->GetPipeElmCount() > 0))
			{
				bNeedCheckWeldMark[0] = (
											this->m_oPipeElmList[0]->IsKindOf(CPipeElement::TypeString()) && 
											pPipeRoutine->m_oPipeElmList[0]->IsKindOf(CPipeElement::TypeString())
										);
				bNeedCheckWeldMark[1] = (
											this->m_oPipeElmList[0]->IsKindOf(CPipeElement::TypeString()) && 
											pPipeRoutine->m_oPipeElmList[pPipeRoutine->m_oPipeElmList.size() - 1]->IsKindOf(CPipeElement::TypeString())
										);
				bNeedCheckWeldMark[2] = (
											this->m_oPipeElmList[this->m_oPipeElmList.size() - 1]->IsKindOf(CPipeElement::TypeString()) && 
											pPipeRoutine->m_oPipeElmList[0]->IsKindOf(CPipeElement::TypeString())
										);
				bNeedCheckWeldMark[3] = (
											this->m_oPipeElmList[this->m_oPipeElmList.size() - 1]->IsKindOf(CPipeElement::TypeString()) && 
											pPipeRoutine->m_oPipeElmList[pPipeRoutine->m_oPipeElmList.size() - 1]->IsKindOf(CPipeElement::TypeString())
										);
			}
			/// up to here

			/// pipe run list should be empty!!!
			if(!m_oPipeRunList.empty()) throw exception("bad environment");

			/// 앞쪽에 연결한다
			if((dDist[0] < dDist[1]) && (dDist[0] < dDist[2]) && (dDist[0] < dDist[3]))
			{	
				/// 일정 길이가 되어야 inline-element를 생성한다. - 2011.11.10 added by humkyung
				if((dDist[0] > dToler) && 
					(!bNeedCheckWeldMark[0] || FindConnectedWeldMarkTo(m_ptConnList[0] , pPipeRoutine->m_ptConnList[0] , pWeldMarkElmList) > 0))
				{
					
					CInlineElement* pInlineElm = new CInlineElement(pPipeRoutine->m_ptConnList[0] , m_ptConnList[0]);
					m_oPipeElmList.insert(m_oPipeElmList.begin() , (CIsoElement*)pInlineElm);
				}

				pPipeRoutine->Reverse();
				m_ptConnList.insert(m_ptConnList.begin() , pPipeRoutine->m_ptConnList.begin() , pPipeRoutine->m_ptConnList.end());
				m_oPipeElmList.insert(m_oPipeElmList.begin() , pPipeRoutine->m_oPipeElmList.begin() , pPipeRoutine->m_oPipeElmList.end());
				m_oPipeRunList.insert(m_oPipeRunList.begin() , pPipeRoutine->m_oPipeRunList.begin() , pPipeRoutine->m_oPipeRunList.end() );

				return ERROR_SUCCESS;
			}
			else if((dDist[1] < dDist[0]) && (dDist[1] < dDist[2]) && (dDist[1] < dDist[3]))
			{
				/// 일정 길이가 되어야 inline-element를 생성한다. - 2011.11.10 added by humkyung
				if((dDist[1] > dToler) && 
					(!bNeedCheckWeldMark[1] || FindConnectedWeldMarkTo(m_ptConnList[0] , pPipeRoutine->m_ptConnList[iSize2 - 1] , pWeldMarkElmList) > 0))
				{
					CInlineElement* pInlineElm = new CInlineElement(pPipeRoutine->m_ptConnList[iSize2 - 1] , m_ptConnList[0]);
					m_oPipeElmList.insert(m_oPipeElmList.begin() , (CIsoElement*)pInlineElm);
				}
				m_ptConnList.insert(m_ptConnList.begin() , pPipeRoutine->m_ptConnList.begin() , pPipeRoutine->m_ptConnList.end());

				m_oPipeElmList.insert(m_oPipeElmList.begin() , pPipeRoutine->m_oPipeElmList.begin() , pPipeRoutine->m_oPipeElmList.end());
				m_oPipeRunList.insert(m_oPipeRunList.begin() , pPipeRoutine->m_oPipeRunList.begin() , pPipeRoutine->m_oPipeRunList.end() );

				return ERROR_SUCCESS;
			}
			/// up to here
			/// 맨 뒤쪽으로 연결한다
			else if((dDist[2] < dDist[0]) && (dDist[2] < dDist[1]) && (dDist[2] < dDist[3]))
			{
				/// 일정 길이가 되어야 inline-element를 생성한다. - 2011.11.10 added by humkyung
				if((dDist[2] > dToler) && 
					(!bNeedCheckWeldMark[2] || FindConnectedWeldMarkTo(m_ptConnList[iSize1 - 1] , pPipeRoutine->m_ptConnList[0] , pWeldMarkElmList) > 0))
				{
					CInlineElement* pInlineElm = new CInlineElement(m_ptConnList[iSize1 - 1] , pPipeRoutine->m_ptConnList[0]);
					m_oPipeElmList.push_back((CIsoElement*)pInlineElm);
				}
				m_ptConnList.insert(m_ptConnList.end() , pPipeRoutine->m_ptConnList.begin() , pPipeRoutine->m_ptConnList.end());

				m_oPipeElmList.insert(m_oPipeElmList.end() , pPipeRoutine->m_oPipeElmList.begin() , pPipeRoutine->m_oPipeElmList.end());
				m_oPipeRunList.insert(m_oPipeRunList.end() , pPipeRoutine->m_oPipeRunList.begin() , pPipeRoutine->m_oPipeRunList.end() );

				return ERROR_SUCCESS;
			}
			else if((dDist[3] < dDist[0]) && (dDist[3] < dDist[1]) && (dDist[3] < dDist[2]))
			{
				/// 일정 길이가 되어야 inline-element를 생성한다. - 2011.11.10 added by humkyung
				if((dDist[3] > dToler) && 
					(!bNeedCheckWeldMark[3] || FindConnectedWeldMarkTo(m_ptConnList[iSize1 - 1] , pPipeRoutine->m_ptConnList[iSize2 - 1] , pWeldMarkElmList) > 0))
				{
					CInlineElement* pInlineElm = new CInlineElement(m_ptConnList[iSize1 - 1] , pPipeRoutine->m_ptConnList[iSize2 - 1]);
					m_oPipeElmList.push_back((CIsoElement*)pInlineElm);
				}

				pPipeRoutine->Reverse();
				m_ptConnList.insert(m_ptConnList.end() , pPipeRoutine->m_ptConnList.begin() , pPipeRoutine->m_ptConnList.end());
				m_oPipeElmList.insert(m_oPipeElmList.end() , pPipeRoutine->m_oPipeElmList.begin() , pPipeRoutine->m_oPipeElmList.end());
				m_oPipeRunList.insert(m_oPipeRunList.end() , pPipeRoutine->m_oPipeRunList.begin() , pPipeRoutine->m_oPipeRunList.end() );

				return ERROR_SUCCESS;
			}
			/// up to here

			return ERROR_BAD_ENVIRONMENT;
		}
	}

	return ERROR_SUCCESS;

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-11
    @class      CPipeRoutine
    @function   CompareWith
    @return     int
    @param      CPipeRoutine*   pPipeRoutine
    @brief		compare two pipe routines
******************************************************************************/
int CPipeRoutine::ComparePipeRunWith( const int iStartIndex1 , int iCount , CPipeRoutine* pPipeRoutine , const int iStartIndex2 , CIsoArea* pIsoArea )
{
	assert(pIsoArea && "pIsoArea is NULL");
	CDgnDocument* pDgnDoc = pIsoArea->GetIsoModel()->GetDgnDocument();

	if(pPipeRoutine)
	{
		vector<CIsoElement*> oBranchElmList[2];
		
		iCount = (-1 == iCount) ? (m_oPipeRunList.size() - iStartIndex1) : iCount;
		for(int i = 0;i < iCount;++i)
		{
			CPipeRun* pPipeRun[2] = {NULL,NULL};

			if((iStartIndex1 + i) < int(m_oPipeRunList.size()))
			{
				pPipeRun[0] = m_oPipeRunList[iStartIndex1 + i];
			}
			/// branch element를 리스트에 저장한다. - 2011.10.24 added by humkyung
			if(((iStartIndex1 + i) < int(m_oPipeRunList.size())) && (m_oPipeRunList[iStartIndex1 + i]->IsStartElmBranch()))
			{
				oBranchElmList[0].push_back( m_oPipeRunList[iStartIndex1 + i]->GetStartElm() );
			}
			/// up to here

			if((iStartIndex2 + i) < int(pPipeRoutine->m_oPipeRunList.size()))
			{
				pPipeRun[1] = pPipeRoutine->m_oPipeRunList[iStartIndex2 + i];
			}
			/// branch element를 리스트에 저장한다. - 2011.10.24 added by humkyung
			if(((iStartIndex2 + i) < int(pPipeRoutine->m_oPipeRunList.size())) && (pPipeRoutine->m_oPipeRunList[iStartIndex2 + i]->IsStartElmBranch()))
			{
				oBranchElmList[1].push_back( pPipeRoutine->m_oPipeRunList[iStartIndex2 + i]->GetStartElm() );
			}
			/// up to here

			if((NULL != pPipeRun[0]) && (NULL != pPipeRun[1]))
			{
				if(0 != pPipeRun[0]->CompareWith(pPipeRun[1]))
				{
					pPipeRun[0]->DrawCloudMark( false , pDgnDoc );
				}
			}
			else if(NULL != pPipeRun[0])
			{
				pPipeRun[0]->DrawCloudMark( true , pDgnDoc );
			}
		}

		for(int i = 0;(i < int(oBranchElmList[0].size())) && (i < int(oBranchElmList[1].size()));++i)
		{
			CPipeRoutine* pPipeRoutineB = pIsoArea->GetPipeRoutineStartWith( oBranchElmList[0][i] );
			CPipeRoutine* pPipeRoutineA = pPipeRoutine->GetIsoArea()->GetPipeRoutineStartWith( oBranchElmList[1][i] );
			if((NULL != pPipeRoutineA) && (NULL != pPipeRoutineB))
			{
				pPipeRoutineB->CompareWith( pPipeRoutineA , pIsoArea );
			}
		}

		return ERROR_SUCCESS;
	}
	else
	{
		iCount = (-1 == iCount) ? (m_oPipeRunList.size() - iStartIndex1) : iCount;
		if(int(m_oPipeRunList.size()) < (iStartIndex1 + iCount))
		{
			iCount = m_oPipeRunList.size() - iStartIndex1;
		}

		this->DrawCloudMark(iStartIndex1 , iCount , true , pDgnDoc);
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-24
    @class      CPipeRoutine
    @function   CompareWith
    @return     int
    @param      CPipeRoutine*   pPipeRoutine
    @param      CIsoArea*       pIsoArea
    @brief		compare two pipe routine
******************************************************************************/
int CPipeRoutine::CompareWith(CPipeRoutine* pPipeRoutine , CIsoArea* pIsoArea )
{
	assert(pIsoArea && "pIsoArea is NULL");

	if(pIsoArea)
	{
		STRING_T sFittingListB , sFittingListA;		
		{
			OSTRINGSTREAM_T oss;
			for(vector<CPipeRoutine::CPipeRun* >::iterator itr = m_oPipeRunList.begin();itr != m_oPipeRunList.end();++itr)
			{
				oss << (*itr)->typeString();
				if((itr + 1) != m_oPipeRunList.end()) oss << std::endl;
			}
			sFittingListB = oss.str();
		}

		if(NULL != pPipeRoutine)
		{
			{
				OSTRINGSTREAM_T oss;
				for(vector<CPipeRoutine::CPipeRun* >::iterator itr = pPipeRoutine->m_oPipeRunList.begin();itr != pPipeRoutine->m_oPipeRunList.end();++itr)
				{
					oss << (*itr)->typeString();
					if((itr + 1) != pPipeRoutine->m_oPipeRunList.end()) oss << std::endl;
				}
				sFittingListA = oss.str();
			}
		}

		int iStartIndexA = 0 , iStartIndexB = 0;
		CDgnDocument* pDgnDoc = pIsoArea->GetIsoModel()->GetDgnDocument();
		vector<CDiff::Item*> oResult = CDiff::DiffText(sFittingListA , sFittingListB);
		for(vector<CDiff::Item*>::iterator itr = oResult.begin();itr != oResult.end();++itr)
		{
			if((*itr)->insertedB > 0)
			{
				/// 동일한 fitting들을 가지고 있다. 그래서 둘을 서로 비교한다.
				if((*itr)->StartB > iStartIndexB)
				{
					const int iCount = (*itr)->StartB - iStartIndexB;
					ComparePipeRunWith( iStartIndexB , iCount , pPipeRoutine , iStartIndexA , pIsoArea );
					iStartIndexB += iCount;
					iStartIndexA += iCount;
				}
				/// up to here

				DrawCloudMark( (*itr)->StartB , (*itr)->insertedB , true , pDgnDoc );	/// 추가된 fitting들에 대해서 cloud mark를 그린다.
				iStartIndexB += (*itr)->insertedB;
				iStartIndexA += (*itr)->deletedA;

				/// insert된 fitting중에서 branch류가 있으면 branch와 연결된 pipe routine에 cloud mark를 그린다.
				for(int i = 0;i < (*itr)->insertedB;++i)
				{
					CPipeRoutine *pBranchPipeRoutineB = NULL , *pBranchPipeRoutineA = NULL;
					if(((*itr)->StartB + i < int(m_oPipeRunList.size())) && m_oPipeRunList[(*itr)->StartB + i]->GetStartElm()->IsBranchElm())
					{
						pBranchPipeRoutineB = pIsoArea->GetPipeRoutineStartWith(m_oPipeRunList[(*itr)->StartB + i]->GetStartElm());
						
						if((NULL != pPipeRoutine) && (i < (*itr)->deletedA))
						{
							if(pPipeRoutine->m_oPipeRunList[(*itr)->StartA + i]->GetStartElm()->IsBranchElm())
							{
								pBranchPipeRoutineA = pPipeRoutine->GetIsoArea()->GetPipeRoutineStartWith(pPipeRoutine->m_oPipeRunList[(*itr)->StartA + i]->GetStartElm());
							}
						}
						
						if(NULL != pBranchPipeRoutineB) pBranchPipeRoutineB->ComparePipeRunWith( 0 , -1 , pBranchPipeRoutineA , 0 , pIsoArea );
					}
				}
				/// up to here
			}
			else if((*itr)->deletedA > 0)
			{
				/// 동일한 fitting들을 가지고 있다. 그래서 둘을 서로 비교한다.
				if((*itr)->StartB > iStartIndexB)
				{
					const int iCount = (*itr)->StartB - iStartIndexB;
					ComparePipeRunWith( iStartIndexB , iCount , pPipeRoutine , iStartIndexA , pIsoArea );
					iStartIndexB += iCount;
					iStartIndexA += iCount;
				}
				/// up to here

				iStartIndexA += (*itr)->deletedA;
			}
		}

		ComparePipeRunWith( iStartIndexB , -1 , pPipeRoutine , iStartIndexA , pIsoArea );

		/// deallocate memory for CDiff::Item
		for(vector<CDiff::Item*>::iterator itr = oResult.begin();itr != oResult.end();++itr)
		{
			SAFE_DELETE( *itr );
		}
		/// up to here

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-17
    @class      CPipeRoutine
    @function   GetStartElm
    @return     CIsoElement*
    @brief		return start element
******************************************************************************/
CIsoElement* CPipeRoutine::GetStartElm()
{
	return m_pStartElm;
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-09
    @class      CPipeRoutine
    @function   CleanUpPipeRunList
    @return     int
    @brief
******************************************************************************/
int CPipeRoutine::CleanUpPipeRunList()
{
	for(vector<CPipeRun*>::iterator	itr = m_oPipeRunList.begin();itr != m_oPipeRunList.end();++itr)
	{
		SAFE_DELETE(*itr);
	}
	m_oPipeRunList.clear();

	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-22
    @class      CPipeRoutine
    @function   CreatePipeRun
    @return     int
    @brief		create pipe run list
******************************************************************************/
int CPipeRoutine::CreatePipeRun()
{
	CleanUpPipeRunList();

	CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
	const double dToler = CIsoModelDocData::m_dDrawingToler;

	/// find a dimension to one of elbow , tee , unknown...
	vector<CIsoElement*> oFittingElmList;
	vector<CIsoElement*>::iterator itr = m_oPipeElmList.begin();
	while(itr != m_oPipeElmList.end())
	{
		vector<CIsoElement*> oIsoElmList;

		/// skip fitting element
		oFittingElmList.clear();
		while((itr != m_oPipeElmList.end()) && (*itr)->IsFittingElm())
		{
			oFittingElmList.push_back( *itr );
			++itr;
		}
		/// treat mutiple fittings(branch) - 2011.10.27 added by humkyung
		if(!oFittingElmList.empty())
		{
			for(vector<CIsoElement*>::iterator jtr = oFittingElmList.begin();jtr != oFittingElmList.end();)
			{
				if((*jtr)->ShouldHaveOwnDim())
				{
					vector<DPoint3d> ptConnList;
					(*jtr)->GetConnPointList(ptConnList);
					if(2 == ptConnList.size())
					{
						m_oPipeRunList.push_back( new CPipeRun(*jtr , ptConnList[0] , ptConnList[ptConnList.size() - 1]) );
					}

					if((jtr + 1) != oFittingElmList.end())
					{
						if(!(*(jtr + 1))->ShouldHaveOwnDim())
						{
							stable_sort( ptConnList.begin() , ptConnList.end() , CIsoElement::SortDPoint3dFrom((*(jtr+1))->center()) );
							
							DPoint3d center = (*(jtr+1))->center();
							const double dist = ::DistanceBetween(ptConnList[0] , center);
							if(  dist > dToler )
							{
								m_oPipeRunList.push_back( new CPipeRun(*jtr , ptConnList[0] , center , _T("dist")) );
							}
						}
						jtr = oFittingElmList.erase( find(oFittingElmList.begin() , oFittingElmList.end() , *jtr) );
					}
					else
					{
						jtr = oFittingElmList.erase( find(oFittingElmList.begin() , oFittingElmList.end() , *jtr) );
					}
				}
				else if(((jtr + 1) != oFittingElmList.end()) && (!(*(jtr + 1))->ShouldHaveOwnDim()))
				{
					///NOTICE: PipeRun이 실제보다 길이가 길어짐...
					m_oPipeRunList.push_back( new CPipeRun(*jtr , (*jtr)->center() , (*(jtr+1))->center() , _T("dist")) );
					jtr = oFittingElmList.erase( jtr );
				}
				else if(((jtr + 1) != oFittingElmList.end()) && ((*(jtr + 1))->ShouldHaveOwnDim()))
				{
					vector<DPoint3d> oConnPtList;
					(*(jtr+1))->GetConnPointList(oConnPtList);
					stable_sort( oConnPtList.begin() , oConnPtList.end() , CIsoElement::SortDPoint3dFrom((*jtr)->center()) );
					m_oPipeRunList.push_back( new CPipeRun(*jtr , (*jtr)->center() , oConnPtList[0] , _T("dist")) );
					jtr = oFittingElmList.erase( jtr );
				}
				else
				{
					++jtr;
				}
			}
		}
		/// up to here

		/// store non-fittings to list
		for(;(itr != m_oPipeElmList.end()) && !(*itr)->IsFittingElm();++itr)
		{
			oIsoElmList.push_back(*itr);
		}
		/// store a fitting element which is located at end of pipe run - 2011.12.02 added by humkyung
		CIsoElement* pEndElm = NULL;
		if(itr != m_oPipeElmList.end())
		{
			pEndElm = (*itr);
		}
		/// up to here

		if(!oIsoElmList.empty())
		{
			vector<DPoint3d> pts , ptConnList;

			CIsoElement* pIsoElm = oIsoElmList[0];
			pIsoElm->GetConnPointList(pts);
			ptConnList.insert( ptConnList.begin() , pts.begin() , pts.end() );
			pIsoElm = oIsoElmList[oIsoElmList.size() - 1];
			pIsoElm->GetConnPointList(ptConnList);
			ptConnList.insert( ptConnList.begin() , pts.begin() , pts.end() );

			/// select two points which have the longest distance
			double dMaxDist = 0.0;
			DPoint3d ptEdge[2]={0,0} , ptExtendEdge[2]={0,0};
			for(vector<DPoint3d>::iterator jtr = ptConnList.begin();jtr != ptConnList.end();++jtr)
			{
				for(vector<DPoint3d>::iterator ktr = jtr+1;ktr != ptConnList.end();++ktr)
				{
					const double d = ::DistanceBetween((*jtr) , (*ktr));
					if(d > dMaxDist)
					{
						dMaxDist = d;
						ptEdge[0] = *jtr;
						ptEdge[1] = *ktr;
						ptExtendEdge[0] = ptEdge[0];
						ptExtendEdge[1] = ptEdge[1];
					}
				}
			}
			/// up to here

			if(dMaxDist > 0.0)
			{
				CIsoElement* pStartElm = NULL;
				if(!oFittingElmList.empty())
				{
					pStartElm = oFittingElmList[ oFittingElmList.size() - 1 ];
					if(pStartElm->IsKindOf(CElbowElement::TypeString()))
					{
						ptExtendEdge[0] = pStartElm->center();
					}
				}
				else
				{
					pStartElm = oIsoElmList[0];
					oIsoElmList.erase(oIsoElmList.begin());
				}

				/// get end point of pipe run when end fitting is elbow - 2011.12.02 added by humkyung
				if((NULL != pEndElm) && (pEndElm->IsKindOf(CElbowElement::TypeString()) || pEndElm->IsKindOf(CTeeElement::TypeString())))
				{
					ptExtendEdge[1] = pEndElm->center();
				}
				/// up to here
				
				CPipeRun* pPipeRun = new CPipeRun(pStartElm , ptExtendEdge[0] , ptExtendEdge[1]);
				if(pPipeRun)
				{
					for(vector<CIsoElement*>::iterator jtr = oIsoElmList.begin();jtr != oIsoElmList.end();++jtr)
					{
						pPipeRun->AppendElement(*jtr);
					}
					///const double dAngle = pPipeRun->angle();	/// force to calculate angle
					/*pPipeRun->m_ptStart = ptExtendEdge[0];
					pPipeRun->m_ptEnd = ptExtendEdge[1];*/
					m_oPipeRunList.push_back( pPipeRun );
				}
			}
			/// 2011.12.06 added by humkyung
			else if(!oFittingElmList.empty() && (NULL == pEndElm))
			{
				if(oFittingElmList[0]->IsKindOf(CTeeElement::TypeString()) || oFittingElmList[0]->IsKindOf(CElbowElement::TypeString()))
				{
					DPoint3d center = oFittingElmList[0]->center();
					vector<DPoint3d> oConnPtList;
					oFittingElmList[0]->GetConnPointList(oConnPtList);
					CPipeRun* pPipeRun = new CPipeRun(oFittingElmList[0] , center , oConnPtList[1]);
					if(pPipeRun)
					{
						m_oPipeRunList.push_back( pPipeRun );
					}
				}
			}
			/// up to here

			oFittingElmList.clear();
		}
	}

	if(!oFittingElmList.empty())
	{
		CIsoElement* pIsoElm = oFittingElmList[oFittingElmList.size() - 1];
		if(pIsoElm->IsKindOf(CTeeElement::TypeString()) || pIsoElm->IsKindOf(CElbowElement::TypeString())) 
		{
			vector<DPoint3d> pts;
			pIsoElm->GetConnPointList(pts);

			DPoint3d ptEdge[2]={0,0};
			ptEdge[0] = pIsoElm->center();
			ptEdge[1] = pts[1];

			m_oPipeRunList.push_back( new CPipeRun(pIsoElm , ptEdge[0] , ptEdge[1]) );
		}
	}

	/// set reducer's angle - 2011.11.03 added by humkyung
	int iIndex = 0;
	for(vector<CPipeRoutine::CPipeRun* >::iterator itr = m_oPipeRunList.begin();itr != m_oPipeRunList.end();++itr,++iIndex)
	{
		if((*itr)->GetStartElm() && (CReducerElement::TypeString() == (*itr)->GetStartElm()->typeString()))
		{
			if((iIndex - 1) >= 0)
			{
				(*itr)->SetAngle( (*(itr - 1))->angle() );
			}
			else if((iIndex + 1) < int(m_oPipeRunList.size()))
			{
				(*itr)->SetAngle( (*(itr + 1))->angle() );
			}
		}
	}
	/// up to here

	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-22
    @class      CPipeRoutine
    @function   GetPipeRunList
    @return     int
    @param      vector<CPipeRoutine::CPipeRun*>&    oPipeRunList
    @brief		get pipe run list
******************************************************************************/
int CPipeRoutine::GetPipeRunList(vector<CPipeRoutine::CPipeRun*>& oPipeRunList)
{
	oPipeRunList.clear();
	oPipeRunList.insert( oPipeRunList.begin() , m_oPipeRunList.begin() , m_oPipeRunList.end() );

	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-17
    @class      CPipeRoutine
    @function   AssignDimension
    @return     int
    @param      vector<CIsoElement*>*   pDimElmList
    @brief		assign dimension to pipe
******************************************************************************/
int CPipeRoutine::AssignDimension(vector<CIsoElement*>* pDimElmList)
{
	assert(pDimElmList && "pDimElmList is NULL");

	if(pDimElmList)
	{
		for(vector<CPipeRun* >::iterator itr = m_oPipeRunList.begin();itr != m_oPipeRunList.end();++itr)
		{
			vector<CIsoElement*> oConnectedDimElmList;
			for(vector<CIsoElement*>::iterator jtr = pDimElmList->begin();jtr != pDimElmList->end();++jtr)
			{
				CDimElement* pDimElm = static_cast<CDimElement*>(*jtr);
				if((*itr) == pDimElm->GetConnectedPipeRun())
				{
					oConnectedDimElmList.push_back(*jtr);
				}
			}

			if(!oConnectedDimElmList.empty())
			{
				for(vector<CIsoElement*>::iterator jtr = oConnectedDimElmList.begin();jtr != oConnectedDimElmList.end();++jtr)
				{
					pDimElmList->erase( find(pDimElmList->begin() , pDimElmList->end() , *jtr) );
				}

				(*itr)->AssignDimension( oConnectedDimElmList );
			}
		}

		/// divide pipe run if it has one more dimensions - 2011.10.28 added by humkyung
		for(vector<CPipeRun* >::iterator itr = m_oPipeRunList.begin();itr != m_oPipeRunList.end();/*++itr*/)
		{
			if((*itr)->m_oDimElmList.size() > 1)
			{
				vector<CPipeRun*> oPipeRunList;
				(*itr)->Separate(oPipeRunList);
				for(vector<CPipeRun*>::iterator jtr = oPipeRunList.begin();jtr != oPipeRunList.end();++jtr)
				{
					++itr;
					itr = m_oPipeRunList.insert(itr , *jtr);
				}
				continue;
			}
			++itr;
		}
		/// up to here

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-17
    @class      CPipeRoutine
    @function   FindDimensionWith
    @return     CIsoElement*
    @param      DPoint3d    pt1
    @param      DPoint3d    pt2
    @brief		find a dimension with two dim points
******************************************************************************/
CIsoElement* CPipeRoutine::FindDimensionWith(DPoint3d pt1 , DPoint3d pt2)
{
	return NULL;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-18
    @class      CPipeRoutine
    @function   DrawCloudMark
    @return     int
    @param      int             iStart
    @param      int             iCount
    @param      DgnModelRefP    modelRef
    @brief		draw cloud mark from iStart while iCount
******************************************************************************/
int CPipeRoutine::DrawCloudMark(int iStart , int iCount , bool bIncludeDimLine , CDgnDocument* pDgnDoc)
{
	assert(pDgnDoc && "pDgnDoc is NULL");
	
	if(pDgnDoc)
	{
		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
		const double dAngleToler = docData.GetAngleToler();

		const double scale = docData.m_dScale;
		double dArcMin = 0.0 , dArcMax = 0.0;;
		docData.GetArcRadiusForCloudMark(dArcMin , dArcMax);

		const UInt32 uiLevel = docData.GetCloudLevel();
		const UInt32 uiColor = docData.GetCloudColor();
		const double dHeight = docData.GetCloudHeight(); 
		const double dWidth = docData.GetCloudWidth();
		const CString RevNo = docData.GetRevisionNo( pDgnDoc );
		const CString Display = docData.GetCloudDisplay();

		double angle = DBL_MAX;
		vector<DPoint3d> oShapeForCloudMark;
		for(int i = 0;i < iCount && (iStart + i < int(m_oPipeRunList.size()));++i)
		{
			const double _angle = m_oPipeRunList[iStart + i]->angle();
			if(DBL_MAX == angle)
			{
				angle = _angle;
			}

			if((fabs(angle - _angle) > dAngleToler) && !oShapeForCloudMark.empty())
			{
				/// draw cloudmark
				CConvexHull oConvexhull;
				oShapeForCloudMark = oConvexhull.Create( oShapeForCloudMark );
				oShapeForCloudMark.push_back( oShapeForCloudMark.front() );
				for(vector<DPoint3d>::iterator itr = oShapeForCloudMark.begin();itr != oShapeForCloudMark.end();++itr)
				{
					itr->x /= scale;
					itr->y /= scale;
					itr->z /= scale;
				}
				
				DrawCloud(&(oShapeForCloudMark[0]) , oShapeForCloudMark.size() , dArcMin, dArcMax , 
					uiLevel , uiColor , 0 , 0 , 1 , pDgnDoc);
				if(_T("Cloud&RevNo") == Display)
				{
					DPoint3d StartPointForRevNo = CIsoElement::GetStartPointForRevNo(oShapeForCloudMark);
					Draw_RevNo(&StartPointForRevNo, RevNo , uiLevel , uiColor , 0 , 0 , dHeight , dWidth , pDgnDoc);
				}

				oShapeForCloudMark.clear();
			}

			vector<DPoint3d> oEnclosePolygon;
			m_oPipeRunList[iStart + i]->GetShapeForCloudMark( oEnclosePolygon );
			oShapeForCloudMark.insert( oShapeForCloudMark.end() , oEnclosePolygon.begin() , oEnclosePolygon.end() );

			angle = _angle;

			/// 2011.12.22 added by humkyung
			m_oPipeRunList[iStart + i]->DrawCloudMarkForAttr( pDgnDoc );
			/// up to here
		}

		if(!oShapeForCloudMark.empty())
		{
			CConvexHull oConvexhull;
			oShapeForCloudMark = oConvexhull.Create( oShapeForCloudMark );
			oShapeForCloudMark.push_back( oShapeForCloudMark.front() );
			for(vector<DPoint3d>::iterator itr = oShapeForCloudMark.begin();itr != oShapeForCloudMark.end();++itr)
			{
				itr->x /= scale;
				itr->y /= scale;
				itr->z /= scale;
			}
			
			DrawCloud(&(oShapeForCloudMark[0]) , oShapeForCloudMark.size() , dArcMin, dArcMax , 
				uiLevel , uiColor , 0 , 0 , 1 , pDgnDoc);
			if(_T("Cloud&RevNo") == Display)
			{
				DPoint3d StartPointForRevNo = CIsoElement::GetStartPointForRevNo(oShapeForCloudMark); 
				Draw_RevNo(&StartPointForRevNo, RevNo , uiLevel , uiColor , 0 , 0 , dHeight , dWidth , pDgnDoc);
			}
			return ERROR_SUCCESS;
		}
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-10
    @class      CPipeRoutine
    @function   GetPipeElmCount
    @return     int
    @brief		return number of pipe element
******************************************************************************/
int CPipeRoutine::GetPipeElmCount() const
{
	return m_oPipeElmList.size();
}

int CPipeRoutine::Check(CDgnDocument* pDgnDoc , CString sColor)
{
	for(vector<CIsoElement*>::iterator itr = m_oPipeElmList.begin();itr != m_oPipeElmList.end();++itr)
	{
		(*itr)->Check( pDgnDoc , sColor);
	}

	int iColor = ATOI_T(sColor);
	for(vector<CPipeRun*>::iterator itr = m_oPipeRunList.begin();itr != m_oPipeRunList.end();++itr)
	{
		sColor.Format(_T("%d") , iColor++);
		(*itr)->Check( pDgnDoc , sColor );
	}

	return ERROR_SUCCESS;
}