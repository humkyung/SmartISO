///////////////////////////////////////////////////////////////////////
// jxglTracker.cpp: implementation file
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

#include "stdafx.h"
#include <Dwmapi.h>
#include <gl/gl.h>
#include <memory>
#include "jxglTracker.h"

jxglTracker::jxglTracker(void)
{
    m_bErased = FALSE;
	m_hdc = 0;
	m_hglrc = 0;
	m_Pen = NULL;
}

jxglTracker::~jxglTracker(void)
{
	try
	{
		if (m_Pen != NULL)
		{
			delete m_Pen;
			m_Pen = NULL;
		}
	}
	catch(...)
	{
	}
}

void jxglTracker::SetContext(HDC hdc, HGLRC hglrc)
{
	m_hdc = hdc;
	m_hglrc = hglrc;
}

CRect jxglTracker::GetTrackRect()const
{   // m_rect is not normalized
    // meaning left coordinate may be greater than right coordinate
    return m_rect;
}

void jxglTracker::DrawRectangle(CClientDC& dc , 
								const int  MinX    ,
			       const int  MinY    ,
			       const int  MaxX ,
			       const int  MaxY ,
			       const bool  Draw , 
			       const LineStyle aLineStyle)
{
	static int m_DrawMode;
	if  (!m_Pen && aLineStyle ==Solid )
	{m_Pen = new CPen(PS_SOLID, 1, RGB(0,0,0)); m_DrawMode = R2_MERGEPENNOT;}
	else if (!m_Pen && aLineStyle ==Dot )
	{m_Pen = new CPen(PS_DOT, 1, RGB(0,0,0));   m_DrawMode = R2_XORPEN;}
	else if (!m_Pen && aLineStyle == ShortDash)
	{m_Pen = new CPen(PS_DASH, 1, RGB(255,0,0));	m_DrawMode = R2_XORPEN;}
	else if (!m_Pen && aLineStyle == LongDash)
	{m_Pen = new CPen(PS_DASH, 1, RGB(0,0,0));	m_DrawMode = R2_NOTXORPEN;}
	else if (aLineStyle == Default) 
	{ m_Pen = NULL;	m_DrawMode = R2_MERGEPENNOT;}

	CPen* aOldPen = NULL;
	if (m_Pen) aOldPen = dc.SelectObject(m_Pen);
	dc.SetROP2(m_DrawMode);

	static int StoredMinX, StoredMaxX, StoredMinY, StoredMaxY;
	static bool m_IsVisible;

	if ( m_IsVisible && !Draw) // move or up  : erase at the old position 
	{
		dc.MoveTo(StoredMinX,StoredMinY); dc.LineTo(StoredMinX,StoredMaxY); 
		dc.LineTo(StoredMaxX,StoredMaxY); 
		dc.LineTo(StoredMaxX,StoredMinY); dc.LineTo(StoredMinX,StoredMinY);
		m_IsVisible = false;
	}

	StoredMinX = min ( MinX, MaxX );
	StoredMinY = min ( MinY, MaxY );
	StoredMaxX = max ( MinX, MaxX );
	StoredMaxY = max ( MinY, MaxY);

	if (Draw) // move : draw
	{
		dc.MoveTo(StoredMinX,StoredMinY); dc.LineTo(StoredMinX,StoredMaxY); 
		dc.LineTo(StoredMaxX,StoredMaxY); 
		dc.LineTo(StoredMaxX,StoredMinY); dc.LineTo(StoredMinX,StoredMinY);
		m_IsVisible = true;
	}

	if (aOldPen) dc.SelectObject(aOldPen);
}

HRESULT (WINAPI *pfDwmIsCompositionEnabled)(BOOL *pfEnabled);
/******************************************************************************
    @brief		return true if aero is enabled
	@author     humkyung
    @date       2014-08-24
    @class      OCC_3dView
    @function   IsAeroEnabled
    @return     BOOL
******************************************************************************/
BOOL jxglTracker::IsAeroEnabled()
{
	HMODULE hDwmDLL = LoadLibrary(_T("dwmapi.dll"));	/// Loads the DWM DLL
	if(!hDwmDLL) return FALSE;

	// Everything is fine upto here, we can get function address
	*(FARPROC *)&pfDwmIsCompositionEnabled = GetProcAddress(hDwmDLL,_T("DwmIsCompositionEnabled"));
	BOOL bDwmEnabled = false;
	pfDwmIsCompositionEnabled(&bDwmEnabled);
	return bDwmEnabled;
}

void jxglTracker::DrawTrackRect(int x1, int y1, int x2, int y2 , const bool Draw)
{    
	BOOL enabled = this->IsAeroEnabled();
	if(TRUE == enabled)
	{
		wglMakeCurrent(m_hdc, m_hglrc);
		// drawing different rubber-banding rectangle depending on the mouse movement x-direction
		if(x1 < x2)
		{
			glColor4f(1.0f, 0.2f, 0.0f, 0.0f);
		}
		else
		{
			glColor4f(0.0f, 0.2f, 1.0f, 0.0f);
		}

		// OpenGL window coordinates are different from GDI's
		CRect rectClient;
		m_pWnd->GetClientRect(&rectClient);
		glRecti(x1, rectClient.Height() - y1, x2, rectClient.Height() - y2);

		glFlush();  // must flush here
	}
	else
	{
		static int m_DrawMode = R2_XORPEN;

		std::auto_ptr<CPen> pPen(new CPen(PS_SOLID, 1, RGB(0,0,0)));
		HGDIOBJ hOldObj = ::SelectObject(m_hdc , pPen.get());
		::SetROP2(m_hdc , m_DrawMode);

		static		int StoredMinX, StoredMaxX, StoredMinY, StoredMaxY;
		static		bool m_IsVisible;

		if (!Draw) // move or up  : erase at the old position 
		{
			::MoveToEx(m_hdc , StoredMinX,StoredMinY , NULL); ::LineTo(m_hdc , StoredMinX,StoredMaxY); 
			::LineTo(m_hdc , StoredMaxX,StoredMaxY); 
			::LineTo(m_hdc , StoredMaxX,StoredMinY); ::LineTo(m_hdc , StoredMinX,StoredMinY);
			m_IsVisible = false;
		}

		StoredMinX = min ( x1, x2);
		StoredMinY = min ( y1, y2);
		StoredMaxX = max ( x1, x2);
		StoredMaxY = max ( y1, y2);

		if (Draw) // move : draw
		{
			::MoveToEx(m_hdc , StoredMinX,StoredMinY , NULL); ::LineTo(m_hdc , StoredMinX,StoredMaxY); 
			::LineTo(m_hdc , StoredMaxX,StoredMaxY); 
			::LineTo(m_hdc , StoredMaxX,StoredMinY); LineTo(m_hdc , StoredMinX,StoredMinY);
			m_IsVisible = true;
		}

		::SelectObject(m_hdc , hOldObj);

	}
}

void jxglTracker::DrawTrackRect(const CRect& rect , const bool Draw)
{
    DrawTrackRect(rect.left, rect.top, rect.right, rect.bottom , Draw);
}

BOOL jxglTracker::TrackRubberBand(CWnd* pWnd, CPoint point, BOOL bMakeCurrent)
{
	m_pWnd = pWnd;
	ASSERT(m_pWnd != 0);
	CRect rectClient;
	m_pWnd->GetClientRect(&rectClient);
	if (::GetCapture() != NULL)
	{   // don't handle if capture already set
		return FALSE;
	}
	// set mouse capture because we are going to work on this window
	pWnd->SetCapture();
	ASSERT(pWnd == CWnd::GetCapture());
	//pWnd->UpdateWindow();

	const BOOL enabled = this->IsAeroEnabled();
	if(TRUE == enabled)
	{
		// save the old contexts
		HDC hDC = wglGetCurrentDC();
		HGLRC hglRC = wglGetCurrentContext();
		wglMakeCurrent(m_hdc, m_hglrc);

		// set drawing mode to front-buffer, etc
		glDrawBuffer(GL_FRONT);

		glPushAttrib(GL_ALL_ATTRIB_BITS);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_COLOR_LOGIC_OP);
		glLogicOp(GL_XOR);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// save the current projection matrix and set up a new convenient projection matrix
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho(0, rectClient.Width(), 0, rectClient.Height(), -1, 1);

		glViewport(-1, -1, rectClient.Width() + 2, rectClient.Height() + 2);
    
		// save the current model view matrix
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
    
		BOOL bMoved = FALSE;
		CPoint ptOld = point;
		CRect rectOld = CRect(ptOld, ptOld);
		CPoint ptNew;
    
		MSG msg;
		BOOL bStop = FALSE;
		for (;;)
		{  // loop forever until LButtonUp, RButtonDown or ESC keyDown
        
			VERIFY(::GetMessage(&msg, NULL, 0, 0));
        
			/*if (CWnd::GetCapture() != pWnd)
			{
				break;
			}*/
        
			if(msg.message == WM_LBUTTONUP || msg.message == WM_MOUSEMOVE)
			{
				ptNew.x = (int)(short)LOWORD(msg.lParam);
				ptNew.y = (int)(short)HIWORD(msg.lParam);
				m_rect = CRect(ptOld, ptNew);
            
				if (bMoved)
				{
					m_bErased = TRUE;
					DrawTrackRect(rectOld , true);
				}
				rectOld = m_rect;
				if (msg.message != WM_LBUTTONUP)
				{
					bMoved = TRUE;
				}
            
				if (msg.message == WM_MOUSEMOVE)
				{
					m_bErased = FALSE;
					DrawTrackRect(m_rect , true);
					//pWnd->SendMessage(msg.message , msg.wParam , msg.lParam);
				}
				else
				{
					bStop = TRUE;
					ASSERT(msg.message == WM_LBUTTONUP);
				}
			}
			else if(msg.message == WM_KEYDOWN)
			{
				if (msg.wParam == VK_ESCAPE)
				{
					bStop = TRUE;
				}
			}
			else if(msg.message == WM_RBUTTONDOWN)
			{ 
				bStop = TRUE;
			}
			else
			{
				DispatchMessage(&msg);
			}
        
			if(bStop)
			{
				break;
			}
        
		}  // for (;;)
    
		// release mouse capture
		ReleaseCapture();
    
		if(!m_bErased)
		{  // do a final erase if needed
			DrawTrackRect(m_rect , true);
		}

		// restore the old modelview and projection matrices
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		// restore all the drawing modes
		glPopAttrib();
		glDrawBuffer(GL_BACK);

		wglMakeCurrent(hDC, hglRC);
	
		pWnd->SendMessage(msg.message , msg.wParam , msg.lParam);

		// note: m_rect's Width and Height may be negative
		return (abs(m_rect.Width()) >= 1 && abs(m_rect.Height()) >= 1);
	}
	else
	{
		BOOL bMoved = FALSE;
		CPoint ptOld = point;
		CRect rectOld = CRect(ptOld, ptOld);
		CPoint ptNew;

		MSG msg;
		BOOL bStop = FALSE;
		for (;;)
		{  // loop forever until LButtonUp, RButtonDown or ESC keyDown
        
			VERIFY(::GetMessage(&msg, NULL, 0, 0));
        
			/*if (CWnd::GetCapture() != pWnd)
			{
				break;
			}*/
        
			if(msg.message == WM_LBUTTONUP || msg.message == WM_MOUSEMOVE)
			{
				ptNew.x = (int)(short)LOWORD(msg.lParam);
				ptNew.y = (int)(short)HIWORD(msg.lParam);
				m_rect = CRect(ptOld, ptNew);
            
				/*if (bMoved)
				{
					m_bErased = TRUE;
					DrawTrackRect(rectOld , false);
				}*/
				rectOld = m_rect;
				if (msg.message != WM_LBUTTONUP)
				{
					bMoved = TRUE;
				}
            
				if (msg.message == WM_MOUSEMOVE)
				{
					DrawTrackRect(m_rect , false);
					m_bErased = FALSE;
					//pWnd->SendMessage(msg.message , msg.wParam , msg.lParam);
					DrawTrackRect(m_rect , true);
				}
				else
				{
					bStop = TRUE;
					ASSERT(msg.message == WM_LBUTTONUP);
				}
			}
			else if(msg.message == WM_KEYDOWN)
			{
				if (msg.wParam == VK_ESCAPE)
				{
					bStop = TRUE;
				}
			}
			else if(msg.message == WM_RBUTTONDOWN)
			{ 
				bStop = TRUE;
			}
			else
			{
				DispatchMessage(&msg);
			}
        
			if(bStop)
			{
				break;
			}
        
		}  // for (;;)
    
		// release mouse capture
		ReleaseCapture();
    
		if(!m_bErased)
		{  // do a final erase if needed
			DrawTrackRect(m_rect , true);
		}
	}
}