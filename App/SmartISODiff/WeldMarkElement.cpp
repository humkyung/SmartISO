#include "StdAfx.h"
#include <assert.h>
#include <DgnText.h>
#include "WeldMarkElement.h"

using namespace IsoElement;

CWeldMarkElement::CWeldMarkElement(const DPoint3d& center , const double& dRadius) : m_dRadius(dRadius)
{
	m_center = center;
}

CWeldMarkElement::~CWeldMarkElement(void)
{
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-31
    @class      CWeldMarkElement
    @function   Add
    @return     int
    @param      CDgnElement*    p
    @brief
******************************************************************************/
int CWeldMarkElement::Add(CDgnElement* p)
{
	assert(p && "p is NULL");
	if(p)
	{
		CIsoElement::Add( p );
		m_ptConn.push_back( volume().center() );

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-31
    @class      CWeldMarkElement
    @function   DistanceTo
    @return     double
    @param      const   DPoint3d&
    @param      pt
    @brief		return the distance between pt and weld mark
******************************************************************************/
double CWeldMarkElement::DistanceTo( const DPoint3d& pt ) const
{
	return ::DistanceBetween(pt , m_ptConn[0]);
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-31
    @class      CWeldMarkElement
    @function   center
    @return     DPoint3d
    @brief
******************************************************************************/
DPoint3d CWeldMarkElement::center() const
{
	return m_ptConn[0];
}

/******************************************************************************
    @author     humkyung
    @date       2011-12-01
    @class
    @function   IsWeldMark
    @return     bool
    @param      CDgnLineString*  pLineStringElm
    @brief
******************************************************************************/
bool CWeldMarkElement::IsWeldMark( DPoint3d& center , double& dRadius , CDgnLineString* pLineStringElm , vector<CDgnElement*>* pDgnElmList)
{
	assert(pLineStringElm && pDgnElmList && "pLineStringElm or pDgnElmList is NULL");

	if(pLineStringElm && pDgnElmList)
	{
		const int iVertexCount = pLineStringElm->GetVertexCount();
		if(iVertexCount > MIN_VERTEX_COUNT_FOR_FLOW_MARK)
		{
			const bool isCircle = CIsoElement::IsPossibleToBeCircle( center , dRadius , pLineStringElm );
			if(true == isCircle)
			{
				bool isWeldMark = true;
				CDgnVolume volume = pLineStringElm->volume();
				for(vector<CDgnElement*>::iterator jtr = pDgnElmList->begin();jtr != pDgnElmList->end();++jtr)
				{
					if((*jtr)->IsKindOf(CDgnText::TypeString()))
					{
						CDgnText* pTextElm = static_cast<CDgnText*>(*jtr);
						if(volume.Contains(pTextElm->origin()))
						{
							isWeldMark = false;
							break;
						}
					}
				}
				return isWeldMark;
			}
		}
	}

	return false;
}

void CWeldMarkElement::Check(CDgnDocument* pDgnDoc , CString sColor)
{
	UInt32 iColor = ATOI_T(sColor);
	::DrawCircle(pDgnDoc , m_center , m_dRadius , iColor);
}