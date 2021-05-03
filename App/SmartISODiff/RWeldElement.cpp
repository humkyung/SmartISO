#include "StdAfx.h"
#include <assert.h>
#include <algorithm>
#include "IsoModelDocData.h"
#include "WeldMarkElement.h"
#include "RWeldElement.h"

using namespace IsoElement;

CRWeldElement::CRWeldElement(void) : m_pBranch(NULL) , m_pStartDgnElmOfBranch(NULL)
{
	m_sTypeString = CRWeldElement::TypeString();
}

CRWeldElement::~CRWeldElement(void)
{
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-02
    @class      CRWeldElement
    @function   CollectElements
    @return     int
    @param      vector<CDgnElement*>*   pDgnElmList
    @param      vector<CIsoElement*>*   pWeldMarkElmList
    @param      const                   DPoint3d&
    @param      ptConn
    @brief
******************************************************************************/
int CRWeldElement::CollectElements( vector<CDgnElement*>* pDgnElmList , vector<CIsoElement*>* pWeldMarkElmList , const DPoint3d& ptConn )
{
	assert(pDgnElmList && pWeldMarkElmList && "pDgnElmList or pWeldMarkElmList is NULL");

	CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
	const double dAngleToler = docData.GetAngleToler();

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
			m_ptConn.push_back( pWeldMarkElm->center() );
			pWeldMarkElmList->erase( find(pWeldMarkElmList->begin() , pWeldMarkElmList->end() , pWeldMarkElm) );

			vector<CDgnElement*> oConnectedElmList;

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
					oConnectedElmList.push_back( *itr );
				}
			}

			for(vector<CDgnElement*>::iterator itr = oConnectedElmList.begin();itr != oConnectedElmList.end();++itr)
			{
				CDgnLineString* pLineStringElm = static_cast<CDgnLineString*>(*itr);
				int iVertexCount = pLineStringElm->GetVertexCount();
				const double angle1 = DegreeBetween(pLineStringElm->GetVertexAt(0) , pLineStringElm->GetVertexAt(iVertexCount - 1));

				vector<CDgnElement*>::iterator jtr = oConnectedElmList.begin();
				for(jtr = oConnectedElmList.begin();jtr != oConnectedElmList.end();++jtr)
				{
					if(itr == jtr) continue;

					pLineStringElm = static_cast<CDgnLineString*>(*jtr);
					iVertexCount = pLineStringElm->GetVertexCount();
					const double angle2 = DegreeBetween(pLineStringElm->GetVertexAt(0) , pLineStringElm->GetVertexAt(iVertexCount - 1));
					if(IsParallelAngle(angle1 , angle2 , dAngleToler)) break;
				}

				if(jtr == oConnectedElmList.end())
				{
					m_pStartDgnElmOfBranch = (*itr);
					break;
				}
			}

			if(NULL != m_pStartDgnElmOfBranch)
			{
				oConnectedElmList.erase( find(oConnectedElmList.begin() , oConnectedElmList.end() , m_pStartDgnElmOfBranch) );
				m_oDgnElementList.insert( m_oDgnElementList.begin() , oConnectedElmList.begin() , oConnectedElmList.end() );
			}
			
			return ERROR_SUCCESS;
		}
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-31
    @class      CRWeldElement
    @function   TypeString
    @return     CString
    @brief
******************************************************************************/
STRING_T CRWeldElement::TypeString()
{
	return _T("rweld");
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-02
    @class      CRWeldElement
    @function   SetBranch
    @return     int
    @param      CPipeRoutine*   pBranch
    @brief
******************************************************************************/
int CRWeldElement::SetBranch( CPipeRoutine* pBranch )
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
    @date       2011-11-02
    @class      CRWeldElement
    @function   GetBranch
    @return     CPipeRoutine*
    @brief
******************************************************************************/
CPipeRoutine* CRWeldElement::GetBranch() const
{
	return m_pBranch;
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-02
    @class      CRWeldElement
    @function   GetStartElmOfBranchFrom
    @return     CIsoElement*
    @param      vector<CIsoElement*>*   pPipeElmList
    @brief
******************************************************************************/
CIsoElement* CRWeldElement::GetStartElmOfBranchFrom(vector<CIsoElement*>* pPipeElmList)
{
	assert(pPipeElmList && "pPipeElmList is NULL");

	if(pPipeElmList)
	{
		for(vector<CIsoElement*>::iterator itr = pPipeElmList->begin();itr != pPipeElmList->end();++itr)
		{
			if((*itr)->m_oDgnElementList.end() != find((*itr)->m_oDgnElementList.begin() , (*itr)->m_oDgnElementList.end() , m_pStartDgnElmOfBranch))
			{
				return (*itr);
			}
		}
	}

	return NULL;
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-10
    @class      CRWeldElement
    @function   GetStartPointOfBranch
    @return     DPoint3d
    @brief
******************************************************************************/
DPoint3d CRWeldElement::GetStartPointOfBranch()
{
	if(!m_ptConn.empty())
	{
		return m_ptConn[0];
	}

	throw exception("invalid number of conntion point!!!");
}

void CRWeldElement::Check(CDgnDocument* pDgnDoc , CString sColor)
{
	UInt32 uiColor = ATOI_T(sColor);

	CIsoElement::Check( pDgnDoc , sColor );

	///if(m_pStartDgnElmOfBranch) m_pStartElmOfBranch->ChangeColor( 3 );
}