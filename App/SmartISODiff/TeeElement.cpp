#include "StdAfx.h"
#include <assert.h>
#include <float.h>
#include <DgnLineString.h>
#include <algorithm>
#include "IsoModelDocData.h"
#include "TeeElement.h"

using namespace IsoElement;

CTeeElement::CTeeElement()/// : m_pMainRef(NULL) , m_pBranchRef(NULL)
{
	m_sTypeString = CTeeElement::TypeString();
}

CTeeElement::~CTeeElement(void)
{
	try
	{
		///SAFE_DELETE( m_pBranch );
	}
	catch(...)
	{
	}
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-10
    @class      CTeeElement
    @function   TypeString
    @return     CString
    @brief		return type string of tee
******************************************************************************/
STRING_T CTeeElement::TypeString()
{
	return _T("tee");
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-07
    @class      CTeeElement
    @function   GetConnPointList
    @return     int
    @param      vector<DPoint3d>&   pts
    @brief
******************************************************************************/
int CTeeElement::GetConnPointList(vector<DPoint3d>& pts)
{
	if(3 == m_ptConn.size())
	{
		pts.clear();
		pts.push_back( m_ptConn[0] );
		pts.push_back( m_ptConn[1] );
	}
	else
	{
		throw exception("invalid number of conntion point!!!");
	}
	
	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-14
    @class      CTeeElement
    @function   BuildConnPointList
    @return     void
    @param      DPoint3d    ptCenter
    @brief
******************************************************************************/
void CTeeElement::BuildConnPointList( const DPoint3d& ptCenter )
{
	CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
	const double dAngleTolder = docData.GetAngleToler();

	vector<DPoint3d> pts;
	for(vector<CDgnElement*>::iterator itr = m_oDgnElementList.begin();itr != m_oDgnElementList.end();++itr)
	{
		CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(*itr);
		const int iVertexCount = pLineStringElm->GetVertexCount();
		pts.push_back( pLineStringElm->GetVertexAt(0) );
		pts.push_back( pLineStringElm->GetVertexAt(iVertexCount - 1) );
	}

	std::stable_sort(pts.begin() , pts.end() , CIsoElement::SortDPoint3dFrom( ptCenter ));
	m_ptCenter = pts[0];

	vector<DPoint3d> oTempConnPtList;
	const int iSize = pts.size();
	oTempConnPtList.push_back( pts[iSize - 3] );
	oTempConnPtList.push_back( pts[iSize - 2] );
	oTempConnPtList.push_back( pts[iSize - 1] );
	
	DVec3d vec[2];
	int i = 0;
	for(i = 0;i < 3;++i)
	{
		vec[0].x = oTempConnPtList[i].x - m_ptCenter.x;
		vec[0].y = oTempConnPtList[i].y - m_ptCenter.y;
		vec[0].z = oTempConnPtList[i].z - m_ptCenter.z;
		
		bool isNotParallel = true;
		for(int j = 0;j < 3;++j)
		{
			if(i == j) continue;
			
			vec[1].x = oTempConnPtList[j].x - m_ptCenter.x;
			vec[1].y = oTempConnPtList[j].y - m_ptCenter.y;
			vec[1].z = oTempConnPtList[j].z - m_ptCenter.z;

			const double rad = ::AngleBetweenVectors(vec[0] , vec[1]);
			const double dAngleBetween = RAD2DEG(rad);
			if(fabs(180. - dAngleBetween) < dAngleTolder)
			{
				isNotParallel = false;
				break;
			}
		}

		if(true == isNotParallel) break;
	}

	m_ptConn.clear();
	if( i < int(oTempConnPtList.size()) )
	{
		m_ptConn.push_back( oTempConnPtList[(i+1)%3] );
		m_ptConn.push_back( oTempConnPtList[(i+2)%3] );
		m_ptConn.push_back( oTempConnPtList[i] );
	}
	else
	{
		throw exception("fail to build conn point list for tee");
	}
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-11
    @class      CTeeElement
    @function   GetStartPointOfBranch
    @return     DPoint3d
    @brief		return start point of branch
******************************************************************************/
DPoint3d CTeeElement::GetStartPointOfBranch()
{
	DPoint3d res = {0};
	if(3 == m_ptConn.size())
	{
		res = m_ptConn[2];
	}
	else
	{
		throw exception("invalid number of conntion point!!!");
	}
	
	return res;
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-09
    @class      CTeeElement
    @function   SetMainRoutine
    @return     int
    @param      CPipeRoutine*   pBranch
    @brief		set main routine
******************************************************************************/
int CTeeElement::SetMainRoutine( CPipeRoutine* pMainRoutine )
{
	m_pMainRoutineRef = pMainRoutine;
	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-09
    @class      CTeeElement
    @function   GetMainRoutine
    @return     CPipeRoutine*
    @brief		return main routine
******************************************************************************/
CPipeRoutine* CTeeElement::GetMainRoutine() const
{
	return (m_pMainRoutineRef);
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-10
    @class      CTeeElement
    @function   SetBranch
    @return     int
    @param      CPipeRoutine*   pBranch
    @brief		set branch of tee
******************************************************************************/
int CTeeElement::SetBranchRoutine( CPipeRoutine* pBranch )
{
	assert(pBranch && "pBranch is NULL");

	if(pBranch)
	{
		m_pBranchRoutineRef = pBranch;

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-10
    @class      CTeeElement
    @function   GetBranch
    @return     CPipeRoutine*
    @brief
******************************************************************************/
CPipeRoutine* CTeeElement::GetBranchRoutine() const
{
	return (m_pBranchRoutineRef);
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-19
    @class      CTeeElement
    @function   center
    @return     DPoint3d
    @brief		return center of tee element
******************************************************************************/
DPoint3d CTeeElement::center() const
{
	return m_ptCenter;
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-03
    @class      CTeeElement
    @function   Reverse
    @return     int
    @brief
******************************************************************************/
int CTeeElement::Reverse()
{
	std::reverse(m_oDgnElementList.begin() , m_oDgnElementList.end());
	
	swap(m_ptConn[0] , m_ptConn[1]);

	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-14
    @class      CTeeElement
    @function   Add
    @return     int
    @param      CDgnElement*    p
    @brief
******************************************************************************/
int CTeeElement::Add(CDgnElement* p)
{
	return CIsoElement::Add( p );
}

void CTeeElement::Check(CDgnDocument* pDgnDoc , CString sColor)
{
	UInt32 uiColor = ATOI_T(sColor);
	for(vector<CDgnElement*>::iterator itr = m_oDgnElementList.begin();itr != m_oDgnElementList.end();++itr)
	{
		(*itr)->SetColor( uiColor );
	}

	vector<DPoint3d> pts;
	DPoint3d pt = m_ptConn[2];
	pt.x += 0.1;
	pt.y += 0.1;
	pts.push_back( pt );

	pt = m_ptConn[2];
	pt.x -= 0.1;
	pt.y -= 0.1;
	pts.push_back( pt );
	
	pts.push_back( m_ptConn[2] );

	pt = m_ptConn[2];
	pt.x -= 0.1;
	pt.y += 0.1;
	pts.push_back( pt );

	pt = m_ptConn[2];
	pt.x += 0.1;
	pt.y -= 0.1;
	pts.push_back( pt );

	::DrawPolyline( pDgnDoc , pts , sColor);

	{
		pts.clear();
		DPoint3d pt = m_ptConn[1];
		pt.x += 0.1;
		pt.y += 0.1;
		pts.push_back( pt );

		pt = m_ptConn[1];
		pt.x -= 0.1;
		pt.y -= 0.1;
		pts.push_back( pt );

		::DrawPolyline( pDgnDoc , pts , sColor);
	}
}