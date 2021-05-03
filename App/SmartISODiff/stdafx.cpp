// stdafx.cpp : source file that includes just the standard includes
// RevISO.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include <assert.h>
#include <float.h>
#include <vector>

#include <IsEntity.h>

#include "IsRegExp.h"
#include "IsoModelDocData.h"

CString __INI_FILE_PATH__;

enum
{
	FIND_MATCH_CASE		= 0x0001,
	FIND_WHOLE_WORD		= 0x0002,
	FIND_DIRECTION_UP	= 0x0010,
	REPLACE_SELECTION	= 0x0100
};

IsRegExp::RxNode __declspec(dllimport) *IsRxCompile(LPCTSTR Regexp);
int __declspec(dllimport) IsRxExec(IsRegExp::RxNode *Regexp, LPCTSTR Data, int Len, LPCTSTR Start, IsRegExp::RxMatchRes *Match, unsigned int RxOpt);
int __declspec(dllimport) IsRxReplace(LPCTSTR rep, LPCTSTR Src, int len, IsRegExp::RxMatchRes match, LPTSTR *Dest, int *Dlen);
void __declspec(dllimport) IsRxFree(IsRegExp::RxNode *Node);
int FindStringHelper (LPCTSTR pszFindWhere, LPCTSTR pszFindWhat, DWORD dwFlags, int &nLen, IsRegExp::RxNode *&rxnode, IsRegExp::RxMatchRes *rxmatch)
{
	int pos;
	
	if (rxnode)
		IsRxFree (rxnode);
	rxnode = IsRxCompile (pszFindWhat);
	if (rxnode && IsRxExec (rxnode, pszFindWhere, _tcslen (pszFindWhere), pszFindWhere, rxmatch, (dwFlags & FIND_MATCH_CASE) != 0 ? RX_CASE : 0))
	{
		pos = rxmatch->Open[0];
		nLen = rxmatch->Close[0] - rxmatch->Open[0];
	}
	else
	{
		pos = -1;
	}
	return pos;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-19
    @class
    @function   RadianToDegree
    @return     double
    @param      const   double
    @param      Radian
    @brief		convert randian to degree
******************************************************************************/
//double RAD2DEG(const double Radian)
//{
//	return (double)(Radian * 180.0 / PI);
//}

/******************************************************************************
    @author     humkyung
    @date       2011-10-26
    @class
    @function   DegreeBetween
    @return     double
    @param      DPoint3d    pt1
    @param      DPoint3d    pt2
    @brief		return degreen between two points
******************************************************************************/
double DegreeBetween(DPoint3d pt1 , DPoint3d pt2)
{
	DVec3d vec;
	vec.x = pt2.x - pt1.x;
	vec.y = pt2.y - pt1.y;
	vec.z = pt2.z - pt1.z;

	double angle = ::AngleOf(vec);
	if(angle < 0.0) angle += 2*PI;
	return RAD2DEG(angle);
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-02
    @class
    @function   IsParallelAngle
    @return     bool
    @param      const   double&
    @param      angle1  const
    @param      double& angle2
    @param      const   double&
    @param      toler
    @brief
******************************************************************************/
bool IsParallelAngle(const double& angle1 , const double& angle2 , const double& toler)
{
	const double d = fabs(angle1 - angle2);
	return ((d < toler) || (fabs(180. - d) < toler));
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-10
    @class
    @function   DistanceFromPointToLine
    @return     double
    @param      DPoint3d    pt
    @param      DPoint3d[2] l
    @brief
******************************************************************************/
double DistanceFromPointToLine( DPoint3d pt , DPoint3d l[2] )
{
	const double dx = l[1].x - l[0].x;
	const double dy = l[1].y - l[0].y;
	const double mag = sqrt(dx*dx + dy*dy);
	if(0.f != mag)
	{
		double u = ((pt.x - l[0].x)*(l[1].x - l[0].x) + (pt.y - l[0].y)*(l[1].y - l[0].y)) / (mag*mag);
		double xp= l[0].x + u*(l[1].x - l[0].x);
		double yp= l[0].y + u*(l[1].y - l[0].y);

		const double dx = xp - pt.x;
		const double dy = yp - pt.y;
		return sqrt(dx*dx + dy*dy);
	}
	else
	{
		const double dx = pt.x - l[0].x;
		const double dy = pt.y - l[0].y;
		return sqrt(dx*dx + dy*dy);
	}

	return DBL_MAX;
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-14
    @class
    @function   DistanceBetween
    @return     double
    @param      const   DPoint3d
    @param      &pt1    DPoint3d
    @param      &pt2
    @brief
******************************************************************************/
double DistanceBetween(const DPoint3d &pt1 , const DPoint3d &pt2)
{
	const double dx = pt2.x - pt1.x;
	const double dy = pt2.y - pt1.y;

	return sqrt(dx*dx + dy*dy);
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-14
    @class
    @function   IntersectWith
    @return     int
    @param      DPoint3d    &intsec
    @param      const       DVector3d&
    @param      l1          const
    @param      DVector3d&  l2
    @brief
******************************************************************************/
int IntersectWith(DPoint3d &intsec, const DVector3d& l1 , const DVector3d& l2)
{
	const double Ax = l1.end.x - l1.org.x;
	const double Bx = l2.org.x - l2.end.x;
	
	const double Ay = l1.end.y - l1.org.y;
	const double By = l2.org.y - l2.end.y;
	
	const double Cx = l1.org.x - l2.org.x;
	const double Cy = l1.org.y - l2.org.y;
	
	const double f = Ay*Bx - Ax*By;	/* both denominator*/
	/// lines are collinear.
	if(0. == f) return (!SUCCESS);

	const double d = By*Cx - Bx*Cy;	/* alpha numerator*/
	const double e = Ax*Cy - Ay*Cx;	/* beta numerator*/
	
	/*compute intersection coordinates*/
	const double alpha = d/f;
	
	if(l1.org.x == l1.end.x)
	{
		intsec.x = l1.org.x;
	}
	else
	{
		intsec.x = l1.org.x  + (alpha * Ax);
	}

	if(l1.org.y == l1.end.y)
	{
		intsec.y = l1.org.y;
	}
	else
	{
		intsec.y = l1.org.y + (alpha * Ay);
	}
	intsec.z = 0.0;

	return SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-14
    @class
    @function   AngleOf
    @return     double
    @param      DVec3d& vec
    @brief
******************************************************************************/
double AngleOf(const DVec3d& vec)
{
	const double l = sqrt((vec.x*vec.x) + (vec.y*vec.y));
	double alpha = 0.0;

	if(l > 0)
	{
		alpha = acos(vec.x / l);
		
		const double cross = (1.0*vec.y - 0.0*vec.x);
		if(cross < 0) alpha = -alpha;
	}
	else
	{
		alpha = 0.0;
	}

	return alpha;
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-14
    @class
    @function   NormalizeVector
    @return     void
    @param      DVec3d& vec
    @brief
******************************************************************************/
void NormalizeVector(DVec3d& vec)
{
	const double l = sqrt(vec.x*vec.x + vec.y*vec.y);
	if(0.0 != l)
	{
		vec.x /= l;
		vec.y /= l;
		vec.z = 0.0;
	}
	else
	{
		throw exception("length error");
	}
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-14
    @class
    @function   AngleBetweenTwoVectors
    @return     double
    @param      const   DVec3d&
    @param      vec1    const
    @param      DVec3d& vec2
    @brief
******************************************************************************/
double AngleBetweenVectors(const DVec3d& vec1 , const DVec3d& vec2)
{
	const double dot = vec1.x*vec2.x + vec1.y*vec2.y;
	const double l1 = sqrt(vec1.x*vec1.x + vec1.y*vec1.y);
	const double l2 = sqrt(vec2.x*vec2.x + vec2.y*vec2.y);

	double val = dot / (l1 * l2);
	val = (val > 1.0) ? 1.0 : val;
	double rad = acos(val);

	return rad;
}

/*----------------------------------------------------------------------+
|																		|
| name		getPolygonArea												|
|																		|
| author	BSI					6/91									|
|																		|
+----------------------------------------------------------------------*/
double getPolygonArea(DPoint3d* pts , int nCount)
{
	int p = 0 , q = 0;
	double area = 0.0;

	for(p = nCount - 1,q = 0;q < nCount;p = q++)
	{
		area += pts[p].x*pts[q].y - pts[p].y*pts[q].x;
	}

	return area;
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-07
    @class
    @function   GetCoordinateOfArc
    @return     int
    @param      double      Amin
    @param      double      Amax
    @param      DPoint3d    *startSegment
    @param      DPoint3d    *endSegment
    @param      double      *Pjx
    @param      double      *Pjy
    @param      double      *Rf
    @param      DPoint3d    *center
    @brief
******************************************************************************/
int GetCoordinateOfArc(double Amin, double Amax, DPoint3d *startSegment, DPoint3d *endSegment, double *Pjx, double *Pjy, double *Rc, DPoint3d *center)
{
	//get random number
	const double dRandumNo = (((rand() % 10) / 10.0));

	/// arc length which is between minimum and maximum range
	double dArcLength = (dRandumNo * (Amax - Amin)) +  Amin;

	//calculate delta length
	double dx = endSegment[0].x - startSegment[0].x;
	double dy = endSegment[0].y - startSegment[0].y;
	const double dLength = sqrt( (dy*dy) + (dx*dx) );
	dx /= dLength;
	dy /= dLength;

	/// arc length는 segment를 넘을 수 없다.
	if(dArcLength > dLength)
	{
		dArcLength = dLength;

		Pjx[0] = endSegment[0].x;
		Pjy[0] = endSegment[0].y;
	}
	else
	{
		/// next corner
		Pjx[0] = (dx * dArcLength) + startSegment[0].x;
		Pjy[0] = (dy * dArcLength) + startSegment[0].y;
	}

	/// get radius of arc(sweep angle is always 120.)
	(*Rc) = 0.5 * dArcLength / cos(30. * PI / 180.);

	/// using the alpha angle of sub-line calculate the center of the Arc
	const double cs = cos(30. * PI / 180.0);
	const double sn = sin(30. * PI / 180.0);
	center[0].x = startSegment[0].x + (cs*dx - sn*dy)*(*Rc);
	center[0].y = startSegment[0].y + (sn*dx + cs*dy)*(*Rc);
	center[0].z = 0;

	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2012-01-01
    @class
    @function   DrawCloud
    @return     int
    @param      DPoint3d    *ptCloud
    @param      int         nLineCount
    @param      double      fArcMin
    @param      double      fArcMax
    @param      			         UInt32
    @param      nLayer      UInt32
    @param      nColor      int
    @param      nStyle      UInt32
    @param      nWeight     float
    @param      fScale      CDgnDocument*
    @param      pDgnDoc
    @brief
******************************************************************************/
int DrawCloud(DPoint3d *ptCloud , int nLineCount , double fArcMin, double fArcMax , 
			   UInt32 nLayer , UInt32 nColor, int nStyle , UInt32 nWeight , float fScale, CDgnDocument* pDgnDoc)
{
	assert(pDgnDoc && "pDgnDoc is NULL");
	
	if(NULL != pDgnDoc)
	{
		fScale = fScale;

		double dSweepAngle;
		double dStartAngle;
		double dStartDist;
		double constDeltaL;

		double Rc = 0.0;
		int mid = int(nLineCount * 0.5);
		DPoint3d tmp;

		/// set ccw
		if(ptCloud && nLineCount > 2)
		{
			double area = getPolygonArea(ptCloud , nLineCount);
			if(area < 0.0)
			{
				//! reverse
				for(int i = 0;i < mid;i++)
				{
					tmp = ptCloud[ nLineCount - i - 1];
					ptCloud[nLineCount - i - 1] = ptCloud[i];
					ptCloud[i] = tmp;
				}
			}
		}
		/// up to here

		DPoint3d	startPoint;
		DPoint3d	endPoint;
		DPoint3d	startSegment;
		DPoint3d	endSegment;
		DPoint3d	center;

		double Pjx = ptCloud[0].x;
		double Pjy = ptCloud[0].y;

		CDgnComplexShapeHeader* pCmplxShapeHeader = static_cast<CDgnComplexShapeHeader*>(CDgnComplexShapeHeader::CreateInstance());
		if(NULL != pCmplxShapeHeader)
		{
			for(int i = 0 ; i < (nLineCount - 1); i++)
			{		
				//Added By Suchita
				startPoint.x = Pjx;
				startPoint.y = Pjy;		

				if (i + 1 == nLineCount)
					endPoint = ptCloud[0];
				else
					endPoint = ptCloud[i+1];

				startSegment.x = Pjx;
				startSegment.y = Pjy;		
				endSegment.x = endPoint.x;
				endSegment.y = endPoint.y;

				//get the distance of the line
				const double dx = endSegment.x - startSegment.x;
				const double dy = endSegment.y - startSegment.y;
				constDeltaL = sqrt((dx * dx) + (dy * dy));
				do
				{
					/// function to calculate center point and radius of the arc
					GetCoordinateOfArc(fArcMin , fArcMax , &startSegment, &endSegment, &Pjx, &Pjy, &Rc, &center);
					
					/// get the length of the sub-line
					const double dx = Pjx - startPoint.x;
					const double dy = Pjy - startPoint.y;
					dStartDist = sqrt((dy*dy) + (dx*dx));

					dSweepAngle = 120.0 * PI / 180.;
					
					DVec3d vec;
					vec.x = startSegment.x - center.x;
					vec.y = startSegment.y - center.y;
					vec.z = startSegment.z - center.z;
					dStartAngle = ::AngleOf(vec);
					if(dStartAngle < 0.0) dStartAngle += 2*PI;

					/// create arc element
					CDgnArc* pDgnArc = static_cast<CDgnArc*>(CDgnArc::CreateInstance());
					if(pDgnArc)
					{
						pDgnArc->SetLineColor( 3 );
						pDgnArc->SetLineWeight(2);

						pDgnArc->SetOrigin( center );
						pDgnArc->SetAxis(Rc , Rc);
						pDgnArc->SetStartAngle(RAD2DEG(dStartAngle));
						pDgnArc->SetSweepAngle(RAD2DEG(dSweepAngle));

						pCmplxShapeHeader->AddElement( pDgnArc );
					}
									
					/// set start of the next arc point to the end of previous arc point
					startSegment.x = Pjx;
					startSegment.y = Pjy;
				}while((dStartDist - constDeltaL) < 0);
			}
		
			pDgnDoc->Add( pCmplxShapeHeader );

			return ERROR_SUCCESS;
		}

		return ERROR_BAD_ENVIRONMENT;
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-13
    @class      CIsoElement
    @function   Test_DrawBoundary
    @return     void
    @param      vector<DPoint3d>    oPolygon
    @brief
******************************************************************************/
void DrawPolyline(CDgnDocument* pDgnDoc , vector<DPoint3d> oPolygon , CString sColor)
{
	assert(pDgnDoc && "pDgnDoc is NULL");

	if(pDgnDoc)
	{
		UInt32 uiColor = ATOI_T(sColor);
		CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
		const double scale = docData.m_dScale;

		CDgnLineString* pDgnLineString = static_cast<CDgnLineString*>(CDgnLineString::CreateInstance());
		if(NULL != pDgnLineString)
		{
			pDgnLineString->SetLineColor( uiColor );
			pDgnLineString->SetLineWeight(0);

			for(vector<DPoint3d>::iterator itr = oPolygon.begin();itr != oPolygon.end();++itr)
			{
				itr->x /= scale;
				itr->y /= scale;
				itr->z = 0.0;

				pDgnLineString->AddVertex( *itr );
			}

			pDgnDoc->Add( pDgnLineString );
		}
	}
}

/******************************************************************************
    @author     humkyung
    @date       2011-12-01
    @class
    @function   DrawCircle
    @return     void
    @param      DPoint3d        center
    @param      double          dRadius
    @param      DgnModelRefP    modelRef
    @param      const           bool&
    @param      bSelect
    @brief
******************************************************************************/
void DrawCircle(CDgnDocument* pDgnDoc , DPoint3d center , double dRadius , UInt32 uiColor)
{
	CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
	const double scale = docData.m_dScale;

	center.x /= scale;
	center.y /= scale;
	center.z = 0.0;
	dRadius /= scale;

	CDgnEllipse* pDgnEllipse = static_cast<CDgnEllipse*>(CDgnEllipse::CreateInstance());
	if(NULL != pDgnEllipse)
	{
		pDgnEllipse->SetLineColor( uiColor );
		pDgnEllipse->SetLineWeight(0);

		pDgnEllipse->SetOrigin( center );
		pDgnEllipse->SetAxis(dRadius , dRadius);

		pDgnDoc->Add( pDgnEllipse );
	}
}

int Draw_RevNo(DPoint3d *ptStart , CString RevNo , UInt32 nLayer , UInt32 nColor, int nStyle , UInt32 nWeight , double dHeight , double dWidth , CDgnDocument* pDgnDoc)
{
	//
	//double Length = RevNo.GetLength();

	//double dUOR = mdlModelRef_getUorPerSub(modelRef);
	//
	//const double dLength = Length * dHeight * dUOR * 2 ;
	//
	//MSElementUnion cellElm;
	//
	//if (SUCCESS == mdlCell_create (&cellElm , _T("REVNU"), NULL , FALSE))
	//{
	//	MSElementDescr* cellElP;

	//	if(SUCCESS == mdlElmdscr_new (&cellElP, NULL, &cellElm))
	//	{
	//		char *sRevNo = ConvertToChar(RevNo);

	//		DPoint3d RightPt;
	//		RightPt.x = ptStart->x;
	//		RightPt.y = ptStart->y;
	//		RightPt.z = 0;

	//		DPoint3d LeftPt;
	//		LeftPt.x = RightPt.x - dLength;
	//		LeftPt.y = RightPt.y;
	//		LeftPt.z = 0;

	//		const double dAngle = 60 * PI / 180 ; // 프로그램에서 각도는 라디안으로 바꿔야 함
	//		RotMatrix       rotMatrixP;  
	//		mdlRMatrix_fromAngle(&rotMatrixP , dAngle);

	//		DPoint3d TopPt;
	//		TopPt = RightPt;

	//		TopPt.x -= LeftPt.x;
	//		TopPt.y -= LeftPt.y;
	//		TopPt.z = 0;

	//		mdlRMatrix_rotatePoint(&TopPt , &rotMatrixP); 

	//		TopPt.x += LeftPt.x;
	//		TopPt.y += LeftPt.y;
	//		TopPt.z = 0;

	//		DPoint3d Point[4];

	//		Point[0].x = RightPt.x;
	//		Point[0].y = RightPt.y;
	//		Point[0].z = 0;

	//		Point[1].x = LeftPt.x;
	//		Point[1].y = LeftPt.y;
	//		Point[1].z = 0;

	//		Point[2].x = TopPt.x;
	//		Point[2].y = TopPt.y;
	//		Point[2].z = 0;

	//		Point[3].x = RightPt.x;
	//		Point[3].y = RightPt.y;
	//		Point[3].z = 0;

	//		MSElement		LineStringElement; 
	//		MSElementDescr* LineStringElP = NULL;
	//		if (SUCCESS == mdlLineString_create(&LineStringElement, NULL, Point , 4))
	//		{
	//			mdlElement_setSymbology(&LineStringElement, &nColor, &nWeight, &nStyle);
	//			mdlElement_setProperties(&LineStringElement , &nLayer , NULL , NULL , NULL , NULL , NULL , NULL , NULL);
	//			
	//			if(SUCCESS == mdlElmdscr_new (&LineStringElP, NULL, &LineStringElement))
	//			{
	//				mdlElmdscr_appendDscr (cellElP, LineStringElP); 	
	//			}

	//			MSElement TextElement;
	//			MSElementDescr* TextElP = NULL;

	//			DPoint3d TextPoint;

	//			TextPoint.x = (Point[0].x + Point[1].x) * 0.5;
	//			TextPoint.y = (Point[0].y + Point[2].y) * 0.5 - (dLength * 0.1) ;
	//			TextPoint.z = 0;

	//			TextSizeParam tSize; 
	//			RotMatrix rMatrix; 
	//			TextParam tParam; 
	//			mdlRMatrix_getIdentity(&rMatrix); 
	//			tSize.mode = TXT_BY_TILE_SIZE | TXT_NO_TRANSFORM; 
	//			tSize.size.height = dHeight * dUOR;
	//			tSize.size.width = dWidth * dUOR;
	//			tParam.just = TXTJUST_CC; 
	//			tParam.font = 0; 
	//			tParam.style = 0; 
	//			tParam.viewIndependent = TRUE;

	//			if(SUCCESS == mdlText_create(&TextElement , NULL , sRevNo , &TextPoint , &tSize , NULL , &tParam , NULL))
	//			{
	//				mdlElement_setSymbology(&TextElement, &nColor, &nWeight, &nStyle);
	//				mdlElement_setProperties(&TextElement , &nLayer , NULL , NULL , NULL , NULL , NULL , NULL , NULL);
	//				
	//				if(SUCCESS == mdlElmdscr_new (&TextElP, NULL, &TextElement))
	//				{
	//					mdlElmdscr_appendDscr (cellElP, TextElP); 	
	//					mdlElmdscr_add(cellElP);
	//				}
	//			}
	//		}
	//	}
	//}

	return ERROR_SUCCESS;
}