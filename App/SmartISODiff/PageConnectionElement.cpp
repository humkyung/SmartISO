#include "StdAfx.h"
#include <assert.h>
#include <DgnText.h>
#include "ConvexHull.h"
#include "PageConnectionElement.h"
#include "IsoModelDocData.h"

using namespace IsoElement;

CPageConnectionElement::CPageConnectionElement(void) : CAttributeElement(_T(""))
{
	m_sTypeString = CPageConnectionElement::TypeString();
}

CPageConnectionElement::~CPageConnectionElement(void)
{
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-09
    @class      CPageConnectionElement
    @function   TypeString
    @return     CString
    @brief
******************************************************************************/
STRING_T CPageConnectionElement::TypeString()
{
	return _T("PAGE_CONNECTION");
}

/******************************************************************************
    @author     humkyung
    @date       2011-09-27
    @class
    @function   ==
    @return     CPageConnectionElement::operator
    @param      const   CIsoElement*
    @param      rhs
    @brief
******************************************************************************/
bool CPageConnectionElement::CompareWith(const CIsoElement* rhs) const
{
	if(this->IsKindOf(rhs->typeString()) && (m_oDgnElementList.size() == rhs->m_oDgnElementList.size()))
	{
		for(int i = 0;i < int(m_oDgnElementList.size());++i)
		{
			CDgnText* p1 = static_cast<CDgnText*>(m_oDgnElementList[i]);
			CDgnText* p2 = static_cast<CDgnText*>(rhs->m_oDgnElementList[i]);
			if(false == (STRING_T(p1->textString()) == STRING_T(p2->textString()))) return false;
		}

		return true;
	}

	return false;
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-09
    @class      CPageConnectionElement
    @function   DrawCloudMark
    @return     int
    @param      DgnModelRefP    modelRef
    @brief
******************************************************************************/
int CPageConnectionElement::DrawCloudMark( CDgnDocument* pDgnDoc )
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
		const double dHeight = docData.GetCloudHeight(); 
		const double dWidth = docData.GetCloudWidth();
		const CString RevNo = docData.GetRevisionNo( pDgnDoc );
		const CString Display = docData.GetCloudDisplay();

		CDgnVolume _volume = this->volume();

		DPoint3d pt;
		pt.x = _volume.maxx() + m_dUsedToler;
		pt.y = _volume.maxy () + m_dUsedToler;
		pt.z = 0;
		_volume.Add (pt);

		pt.x = _volume.minx() - m_dUsedToler;
		pt.y = _volume.miny () - m_dUsedToler;
		pt.z = 0;
		_volume.Add (pt);

		vector<DPoint3d> oShapeForCloudMark;
		{
			pt.x = _volume.minx();
			pt.y = _volume.miny();
			pt.z = 0.0;
			oShapeForCloudMark.push_back( pt );

			pt.x = _volume.maxx();
			pt.y = _volume.miny();
			pt.z = 0.0;
			oShapeForCloudMark.push_back( pt );

			pt.x = _volume.maxx();
			pt.y = _volume.maxy();
			pt.z = 0.0;
			oShapeForCloudMark.push_back( pt );

			pt.x = _volume.minx();
			pt.y = _volume.maxy();
			pt.z = 0.0;
			oShapeForCloudMark.push_back( pt );
		}

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
		

		/// up to here*/

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}