#include "StdAfx.h"
#include <assert.h>
#include "PipeSupportElement.h"
#include "IsoModelDocData.h"

using namespace IsoElement;

CPipeSupportElement::CPipeSupportElement(DPoint3d pt , CAttributeElement* pAttrElm)
{
	m_ptOrigin = pt;
	m_ptConn.push_back( pt );
	
	m_sTypeString = CPipeSupportElement::TypeString();

	m_oAttrElmList.push_back(pAttrElm);
}

CPipeSupportElement::~CPipeSupportElement(void)
{
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-10
    @class      CPipeSupportElement
    @function   TypeString
    @return     CString
    @brief		return type string of pipe support
******************************************************************************/
STRING_T CPipeSupportElement::TypeString()
{
	return _T("pipe support");
}

DPoint3d CPipeSupportElement::GetOrigin(void)
{
	return m_ptOrigin;
}

/******************************************************************************
    @author     humkyung
    @date       2011-12-22
    @class      CPipeSupportElement
    @function   AddAttributeElement
    @return     int
    @param      CAttributeElement*  pAttrElm
    @brief
******************************************************************************/
int CPipeSupportElement::AddAttributeElement(CAttributeElement* pAttrElm)
{
	assert(pAttrElm && "pAttrElm is NULL");

	if(pAttrElm)
	{
		m_oAttrElmList.push_back( pAttrElm );

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-12-22
    @class      CPipeSupportElement
    @function   GetAttrElement
    @return     CAttributeElement*
    @brief
******************************************************************************/
int CPipeSupportElement::GetAttrElement(vector<CAttributeElement*>& oAttrElmList)
{
	oAttrElmList.clear();
	oAttrElmList.insert(oAttrElmList.end() , m_oAttrElmList.begin() , m_oAttrElmList.end());

	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-18
    @class      CPipeSupportElement
    @function   DrawCloudMark
    @return     int
    @param      DgnModelRefP    modelRef
    @brief
******************************************************************************/
int CPipeSupportElement::DrawCloudMark( CDgnDocument* pDgnDoc )
{
	assert(pDgnDoc && "pDgnDoc is NULL");

	if(pDgnDoc)
	{
		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
		const double scale = docData.m_dScale;
		double dArcMin = 0.0 , dArcMax = 0.0;
		docData.GetArcRadiusForCloudMark(dArcMin , dArcMax);
		const UInt32 uiLevel = docData.GetCloudLevel();
		const UInt32 uiColor = docData.GetCloudColor();

		vector<DPoint3d> oShapeForCloudMark;
		for(vector<CAttributeElement*>::iterator itr = m_oAttrElmList.begin();itr != m_oAttrElmList.end();++itr)
		{
			(*itr)->GetShapeForCloudMark(oShapeForCloudMark);
			///(*itr)->DrawCloudMark( modelRef );
		}
		for(vector<DPoint3d>::iterator itr = oShapeForCloudMark.begin();itr != oShapeForCloudMark.end();++itr)
		{
			itr->x /= scale;
			itr->y /= scale;
			itr->z /= scale;
		}
		DrawCloud(&(oShapeForCloudMark[0]) , oShapeForCloudMark.size() , dArcMin, dArcMax , uiLevel , uiColor , 0 , 0 , 1 , pDgnDoc);		
	}

	return ERROR_INVALID_PARAMETER;
}

void CPipeSupportElement::Check( CDgnDocument* pDgnDoc , CString sColor)
{
	const double d = CIsoModelDocData::m_dDrawingToler*10.0;
	vector<DPoint3d> pts;
	DPoint3d pt = m_ptOrigin;
	pt.x += d;
	pt.y += d;
	pts.push_back( pt );

	pt = m_ptOrigin;
	pt.x -= d;
	pt.y -= d;
	pts.push_back( pt );
	
	pts.push_back( m_ptOrigin );

	pt = m_ptOrigin;
	pt.x -= d;
	pt.y += d;
	pts.push_back( pt );

	pt = m_ptOrigin;
	pt.x += d;
	pt.y -= d;
	pts.push_back( pt );

	::DrawPolyline( pDgnDoc , pts , sColor);
}