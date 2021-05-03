#include "StdAfx.h"
#include <assert.h>
#include "IsoTitleBlockArea.h"
#include "IsoModelDocData.h"
#include "IsoModel.h"
#include "ConvexHull.h"
#include "IsoElement.h"

#include <algorithm>
using namespace std;

CIsoTitleBlockArea::CIsoTitleBlockArea(const CString& sName , const double& minx , const double& miny , const double& maxx , const double& maxy , CIsoModel* pIsoModel) :CIsoArea(sName , minx , miny , maxx , maxy , pIsoModel)
{
	m_minx = minx;
	m_miny = miny;
	m_maxx = maxx;
	m_maxy = maxy;
}
CIsoTitleBlockArea::~CIsoTitleBlockArea(void)
{
}


/******************************************************************************
	@author     humkyung
	@date       2011-09-20
	@class      CIsoBMArea
	@function   ParseBMElements
	@return     void
	@brief
******************************************************************************/
void CIsoTitleBlockArea::ParseTitleBlockElements()
{
	if(m_oDgnElementMap.end() == m_oDgnElementMap.find(_T("general"))) return;

	for(vector<CDgnElement*>::iterator itr = m_oDgnElementMap[_T("general")]->begin();itr != m_oDgnElementMap[_T("general")]->end();++itr)
	{
		if((m_minx < (*itr)->origin().x) && (m_miny < (*itr)->origin().y) && (m_maxx > (*itr)->origin().x) && (m_maxy > (*itr)->origin().y))
		{
			if(!(*itr)->IsKindOf(CDgnText::TypeString())) continue;

			CString rTextString_ = static_cast<CDgnText*>((*itr))->textString();
			if(rTextString_.IsEmpty()) continue;
			CString rTextString = rTextString_.Trim ();

			CIsoTitleBlockData TitleBlockData((*itr)->origin() , rTextString);

			TitleBlockData.m_oDgnElementList.push_back(*itr);
			m_oIsoTitleBlockDataList.push_back(TitleBlockData);
		}
		if(m_oDgnElementMap[_T("general")]->end() == itr) break;	///< ENTRY의 맨 마지막이면 종료한다.
	}
}
/******************************************************************************
@author     kyj
@date       2011-11-17
@class      CIsoTitleBlockArea
@function   PrepareToCompare
@return     void
@brief		parse TitleBlock data
******************************************************************************/
void CIsoTitleBlockArea::PrepareToCompare()
{
	ParseTitleBlockElements();
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-13
    @class      CIsoBMArea
    @function   ExpandEnclosePolygon
    @return     int
    @param      vector<DPoint3d>&   oEnclosePolygon
    @brief
******************************************************************************/
int CIsoTitleBlockArea::ExpandEnclosePolygon( vector<DPoint3d>& oEnclosePolygon , const double dWidthOffset , const double dHeightOffset)
{
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
    @author     yjk
    @date       2011-??-??
    @class      CIsoTitleBlockArea
    @function   GetIsoRevisedTitleBlockDataShapePoint
    @return     vector<DPoint3d>
    @param      const           int
    @param      iStartPos       const
    @param      int             iWidth
    @param      CString         sElemdntID
    @param      DgnModelRefP    modelRef
    @brief
******************************************************************************/
vector<DPoint3d> CIsoTitleBlockArea::GetIsoRevisedTitleBlockDataShapePoint(const int iStartPos , const int iWidth , CString sElemdntID , DgnModelRefP modelRef)
{	
	vector<DPoint3d> IsoRevisedTitleBlockDataShapePoint;

	//MSElementUnion	outElmP;
	//UInt32		filePosP; 
	//if (SUCCESS == mdlAssoc_getElement(&outElmP,&filePosP,_wtoi(sElemdntID),modelRef))
	//{
	//	DPoint3d m_origin;
	//	RotMatrix		rotMatrix;
	//	MSTextSize	tilSize , textSize ; 
	//	if (SUCCESS == mdlText_extract(&m_origin,NULL,NULL,NULL,NULL,&rotMatrix,NULL,NULL,&tilSize,&textSize,&outElmP))
	//	{
	//		const int Degree = int(RAD2DEG(mdlRMatrix_toAngle(&rotMatrix)));

	//		char Contents[256] = {0,};
	//		if (SUCCESS == mdlText_extractString(Contents,&outElmP))
	//		{
	//			double Width = textSize.width;
	//			double Height = textSize.height;
	//			CString sText = (CString)Contents;
	//			int Length = sText.GetLength(); // 텍스트 갯수
	//			double eachTextWidth = Width / Length;  // 텍스트 한글자의 Width 
	//			
	//			DPoint3d LeftBottom;
	//			LeftBottom.x = m_origin.x + (iStartPos * eachTextWidth) ;
	//			LeftBottom.y = m_origin.y ;
	//			LeftBottom.z = 0;
	//			IsoRevisedTitleBlockDataShapePoint.push_back (LeftBottom);

	//			DPoint3d RightBottom;
	//			RightBottom.x = m_origin.x + (iStartPos * eachTextWidth) + (iWidth * eachTextWidth);
	//			RightBottom.y = m_origin.y ;
	//			RightBottom.z = 0;
	//			RightBottom.x -= LeftBottom.x; RightBottom.y -= LeftBottom.y;
	//			mdlRMatrix_rotatePoint(&RightBottom , &rotMatrix);
	//			RightBottom.x += LeftBottom.x; RightBottom.y += LeftBottom.y;
	//			IsoRevisedTitleBlockDataShapePoint.push_back( RightBottom );
	//			
	//			DPoint3d RightTop;
	//			RightTop.x = m_origin.x + (iStartPos * eachTextWidth) + (iWidth * eachTextWidth);  
	//			RightTop.y = m_origin.y + Height;
	//			RightTop.z = 0;
	//			RightTop.x -= LeftBottom.x; RightTop.y -= LeftBottom.y;
	//			mdlRMatrix_rotatePoint(&RightTop , &rotMatrix);
	//			RightTop.x += LeftBottom.x; RightTop.y += LeftBottom.y;
	//			IsoRevisedTitleBlockDataShapePoint.push_back( RightTop );

	//			DPoint3d LeftTop;
	//			LeftTop.x = m_origin.x + (iStartPos * eachTextWidth);
	//			LeftTop.y = m_origin.y + Height;
	//			LeftTop.z = 0;
	//			LeftTop.x -= LeftBottom.x; LeftTop.y -= LeftBottom.y;
	//			mdlRMatrix_rotatePoint(&LeftTop , &rotMatrix);
	//			LeftTop.x += LeftBottom.x; LeftTop.y += LeftBottom.y;
	//			IsoRevisedTitleBlockDataShapePoint.push_back( LeftTop );	
	//		}
	//	}
	//}

	//CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
	//const double dOffset = docData.GetOffset();
	//const double dUOR = mdlModelRef_getUorPerSub(modelRef);
	//ExpandEnclosePolygon( IsoRevisedTitleBlockDataShapePoint , dOffset * dUOR , dOffset*0.5 * dUOR);
	//IsoRevisedTitleBlockDataShapePoint.push_back( IsoRevisedTitleBlockDataShapePoint.front() );

	return IsoRevisedTitleBlockDataShapePoint;
}

map<CString , vector<DPoint3d>> CIsoTitleBlockArea::GetIsoRevisedTitleBlockDataShapePointMap(list<CIsoTitleBlockData> oCurrentTitleBlockDataList)
{
	map<CString , vector<DPoint3d>> IsoRevisedTitleBlockDataShapePointMap;
	/*vector<DPoint3d> IsoRevisedTitleBlockDataShapePoint;
	DgnModelRefP modelRef = m_pIsoModel->GetDgnDocument()->GetDgnModelRef();

	for(list<CIsoTitleBlockData>::iterator itr = oCurrentTitleBlockDataList.begin();itr != oCurrentTitleBlockDataList.end();++itr)
	{
		int iStartPos = 0;
		int iWidth = itr->m_sDataString.GetLength ();
		CString sElemdntID = itr->m_oDgnElementList[0]->elementID(); 

		IsoRevisedTitleBlockDataShapePoint = GetIsoRevisedTitleBlockDataShapePoint(iStartPos , iWidth ,sElemdntID , modelRef);

		IsoRevisedTitleBlockDataShapePointMap.insert(make_pair(itr->m_sDataString  , IsoRevisedTitleBlockDataShapePoint ));
	}*/

	return IsoRevisedTitleBlockDataShapePointMap;
}

int CIsoTitleBlockArea::CompareWith(list<CDgnElement*>& oRemovedElemList , list<CDgnElement*>& oAddedElemList , CIsoArea* rhs)
{
	assert(rhs && "rhs is NULL");
	
	if(rhs)
	{
		CDgnDocument* pDgnDoc = m_pIsoModel->GetDgnDocument();

		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
		const double scale = docData.m_dScale;
		
		double dArcMin = 0.0 , dArcMax = 0.0;
		docData.GetArcRadiusForCloudMark( dArcMin , dArcMax );

		const UInt32 uiLevel = docData.GetCloudLevel();
		const UInt32 uiColor = docData.GetCloudColor();
		const double dHeight = docData.GetCloudHeight();
		const double dWidth = docData.GetCloudWidth(); 
		const CString RevNo = docData.GetRevisionNo( pDgnDoc );
		const CString Display = docData.GetCloudDisplay();

		CIsoTitleBlockArea* pIsoTitleBlockArea = static_cast<CIsoTitleBlockArea*>(rhs);

		list<CIsoTitleBlockData> oCurrentTitleBlockDataList = m_oIsoTitleBlockDataList;				 
		list<CIsoTitleBlockData> oReferenceTitleBlockDataList = pIsoTitleBlockArea->m_oIsoTitleBlockDataList;

		for(list<CIsoTitleBlockData>::iterator itr = m_oIsoTitleBlockDataList.begin();itr != m_oIsoTitleBlockDataList.end();++itr) // 현재 도면의 TitleBlock Data
		{
			for(list<CIsoTitleBlockData>::iterator jtr = pIsoTitleBlockArea->m_oIsoTitleBlockDataList.begin();jtr != pIsoTitleBlockArea->m_oIsoTitleBlockDataList.end();++jtr) // 이전 도면의 TitleBlock Data
			{
				if((*itr) == (*jtr))
				{
					oCurrentTitleBlockDataList.erase(find(oCurrentTitleBlockDataList.begin() , oCurrentTitleBlockDataList.end() , *itr));
					oReferenceTitleBlockDataList.erase(find(oReferenceTitleBlockDataList.begin() , oReferenceTitleBlockDataList.end() , *jtr)); 
					break;
				}
			}
		}
		
		map<CString , vector<DPoint3d>> oIsoRevisedTitleBlockDataShapePointMap = GetIsoRevisedTitleBlockDataShapePointMap(oCurrentTitleBlockDataList);

		for(map<CString , vector<DPoint3d>>::iterator itr = oIsoRevisedTitleBlockDataShapePointMap.begin();itr != oIsoRevisedTitleBlockDataShapePointMap.end();++itr)
		{
			CConvexHull oConvexHull;
			vector<DPoint3d> oEnclosePolygon = oConvexHull.Create (itr->second);
			oEnclosePolygon.push_back( oEnclosePolygon.front() );
			DrawCloud(&(oEnclosePolygon[0]) , oEnclosePolygon.size() , dArcMin, dArcMax , 
				uiLevel , uiColor , 0 , 0 , 1 , NULL);
			if(_T("Cloud&RevNo") == Display)
			{
				DPoint3d StartPointForRevNo = CIsoElement::GetStartPointForRevNo(oEnclosePolygon);
				Draw_RevNo(&StartPointForRevNo, RevNo , uiLevel , uiColor , 0 , 0 , dHeight , dWidth , pDgnDoc);
			}

		}

		/// store added or removed element
		for(list<CIsoTitleBlockData>::iterator itr = pIsoTitleBlockArea->m_oIsoTitleBlockDataList.begin();itr != pIsoTitleBlockArea->m_oIsoTitleBlockDataList.end();++itr)
		{
			oRemovedElemList.insert(oRemovedElemList.end() , itr->m_oDgnElementList.begin() , itr->m_oDgnElementList.end());
		}
		for(list<CIsoTitleBlockData>::iterator itr = oCurrentTitleBlockDataList.begin();itr != oCurrentTitleBlockDataList.end();++itr)
		{
			oAddedElemList.insert(oAddedElemList.end() , itr->m_oDgnElementList.begin() , itr->m_oDgnElementList.end());
		}	
	}
	return ERROR_INVALID_PARAMETER;
}
