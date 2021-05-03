#include "StdAfx.h"
#include <assert.h>
#include <DgnText.h>
#include "IsoModelDocData.h"
#include "SlopeAreaMarkElement.h"

using namespace IsoElement;

CSlopeAreaMarkElement::CSlopeAreaMarkElement()
{
	m_sTypeString = CSlopeAreaMarkElement::TypeString();
}

CSlopeAreaMarkElement::~CSlopeAreaMarkElement(void)
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
    @date       2011-12-13
    @class      CSlopeAreaMarkElement
    @function   TypeString
    @return     STRING_T
    @brief
******************************************************************************/
STRING_T CSlopeAreaMarkElement::TypeString()
{
	return _T("slope area mark");
}

/******************************************************************************
    @author     humkyung
    @date       2011-12-13
    @class      CSlopeAreaMarkElement
    @function   CleanUpPipeRunList
    @return     int
    @brief
******************************************************************************/
int CSlopeAreaMarkElement::CleanUpPipeRunList()
{
	for(vector<CPipeRoutine::CPipeRun*>::iterator	itr = m_oPipeRunList.begin();itr != m_oPipeRunList.end();++itr)
	{
		SAFE_DELETE(*itr);
	}
	m_oPipeRunList.clear();

	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2011-12-13
    @class      CSlopeAreaMarkElement
    @function   CreatePipeRun
    @return     bool
    @brief
******************************************************************************/
int CSlopeAreaMarkElement::CreatePipeRun()
{
	if(1 == m_oDgnElementList.size())
	{
		CleanUpPipeRunList();

		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
		const double dToler = CIsoModelDocData::m_dDrawingToler;

		if(m_oDgnElementList[0]->IsKindOf(CDgnLineString::TypeString()))
		{
			CDgnLineString* pDgnLineStringElm = static_cast<CDgnLineString*>(m_oDgnElementList[0]);
			vector<DPoint3d> oPointList = pDgnLineStringElm->GetLineStringPointList();
			for(vector<DPoint3d>::iterator itr = oPointList.begin();(itr + 1) != oPointList.end();++itr)
			{
					m_oPipeRunList.push_back( new CPipeRoutine::CPipeRun(this , *itr , *(itr+1) , _T("dist")) );
			}
			
			return ERROR_SUCCESS;
		}
	}

	return ERROR_BAD_ENVIRONMENT;
}

/******************************************************************************
    @author     humkyung
    @date       2011-12-13
    @class      CSlopeAreaMarkElement
    @function   GetPipeRunList
    @return     int
    @param      vector<CPipeRoutine::CPipeRun*>&    oPipeRunList
    @brief
******************************************************************************/
int CSlopeAreaMarkElement::GetPipeRunList(vector<CPipeRoutine::CPipeRun*>& oPipeRunList)
{
	oPipeRunList.clear();
	oPipeRunList.insert( oPipeRunList.begin() , m_oPipeRunList.begin() , m_oPipeRunList.end() );

	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2011-12-13
    @class      CSlopeAreaMarkElement
    @function   IsSlopeAreaMark
    @return     bool
    @param      CDgnLineString*  pLineStringElm
    @brief
******************************************************************************/
bool CSlopeAreaMarkElement::IsSlopeAreaMark(CDgnLineString* pLineStringElm)
{
	assert(pLineStringElm && "pLineStringElm is NULL");

	if(pLineStringElm && pLineStringElm->IsClosed())
	{
		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
		const double dToler = docData.m_dDrawingToler;
		const double dAngleToler = docData.GetAngleToler();

		vector<DPoint3d> oPointList = pLineStringElm->GetPointStringExceptSamePoint();
		const int iVertexCount = oPointList.size();
		if(4 == iVertexCount)
		{
			DPoint3d pts[3] = {0,0,0};
			for(int i = 0 ;i < iVertexCount;++i)
			{
				pts[0] = pLineStringElm->GetVertexAt((i + iVertexCount - 1) % iVertexCount);
				pts[1] = pLineStringElm->GetVertexAt(i);
				pts[2] = pLineStringElm->GetVertexAt((i + 1) % iVertexCount);

				DVec3d vec[2];
				vec[0].x = pts[0].x - pts[1].x;
				vec[0].y = pts[0].y - pts[1].y;
				vec[0].z = pts[0].z - pts[1].z;
				vec[1].x = pts[2].x - pts[1].x;
				vec[1].y = pts[2].y - pts[1].y;
				vec[1].z = pts[2].z - pts[1].z;

				const double rad = ::AngleBetweenVectors(vec[0] , vec[1]);
				const double angle = RAD2DEG(rad);
				if(fabs(90.0 - angle) < dAngleToler) return false;
			}

			const double d1 = ::DistanceBetween(oPointList[0] , oPointList[1]);
			const double d2 = ::DistanceBetween(oPointList[2] , oPointList[3]);
			
			const double d3 = ::DistanceBetween(oPointList[1] , oPointList[2]);
			const double d4 = ::DistanceBetween(oPointList[3] , oPointList[0]);
			return ((fabs(d1 - d2) < dToler) && (fabs(d3 - d4) < dToler));
		}
	}

	return false;
}

void CSlopeAreaMarkElement::Check(CString sColor)
{
	UInt32 uiColor = ATOI_T(sColor);
	for(vector<CDgnElement*>::iterator itr = m_oDgnElementList.begin();itr != m_oDgnElementList.end();++itr)
	{
		(*itr)->SetColor( uiColor );
	}
}