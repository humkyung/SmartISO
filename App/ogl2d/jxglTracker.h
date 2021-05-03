#ifndef JXGL_TRACKER
#define JXGL_TRACKER

///////////////////////////////////////////////////////////////////////
// jxglTracker.h: header file
//
//
// Written by Junlin Xu <junlin.xu@gmail.com>
// Copyright (c) 2006-2009. All Rights Reserved.
//
// This code may be used in compiled form in any way you desire. 
// This file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name and all copyright 
// notices remains intact. 
//
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
//
//////////////////////////////////////////////////////////////////////

#ifdef	_OGL2D
#define	_OGL2D_EXT_CLASS	__declspec(dllexport)
#else
#define	_OGL2D_EXT_CLASS	__declspec(dllimport)
#endif

class _OGL2D_EXT_CLASS jxglTracker
{
public:
	enum LineStyle { Solid, Dot, ShortDash, LongDash, Default };

   jxglTracker(void);
   ~jxglTracker(void);
   void SetContext(HDC hdc, HGLRC hglrc);
   void DrawRectangle(CClientDC& dc , 
					const int  MinX    ,
					const int  MinY    ,
					const int  MaxX ,
					const int  MaxY ,
					const bool  Draw , 
					const LineStyle aLineStyle = Default);
   CRect GetTrackRect()const; // un-normalized
   BOOL TrackRubberBand(CWnd* pWnd, CPoint point, BOOL bMakeCurrent);
   void DrawTrackRect(const CRect& rect , const bool Draw);
   BOOL IsAeroEnabled();
private:
   void DrawTrackRect(int x1, int y1, int x2, int y2 , const bool Draw);
   BOOL m_bErased;
   CWnd* m_pWnd;
   CRect m_rect;

   HDC m_hdc;
   HGLRC m_hglrc;
   CPen*  m_Pen;
};


#endif