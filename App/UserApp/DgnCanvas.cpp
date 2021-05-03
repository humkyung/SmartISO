// DgnCanvas.cpp: implementation of the CDgnCanvas class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include <math.h>
#include "resource.h"
#include "DgnCanvas.h"

#include <IsArc2d.h>

#include "../ogl2d/GLRectObject.h"
#include "../ogl2d/GLTextObject.h"
#include "../ogl2d/GLLineObject.h"
#include "../ogl2d/GLSimplePolyObject.h"
#include "../ogl2d/GLArc2dObject.h"
#include "../ogl2d/GLEllipse2dObject.h"

#include "gl/gl.h"
#include "gl/glu.h"

//////////////////////////////////////////////////////////////////////////
#include <DgnLine.h>
#include <DgnPointString.h>
#include <DgnLineString.h>
#include <DgnArc.h>
#include <DgnEllipse.h>
#include <DgnText.h>
#include <DgnComplexShapeHeader.h>
#include <DgnCellHeader.h>
//////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//IMPLEMENT_DYNCREATE(CDgnCanvas, CGLCanvas)

BEGIN_MESSAGE_MAP(CDgnCanvas, CGLCanvas)
	//{{AFX_MSG_MAP(CDgnCanvas)
	ON_WM_MBUTTONDBLCLK()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	// Standard printing commands
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/**	\brief
*/
CDgnCanvas::CDgnCanvas() : m_textWidth(0) , m_textHeight(0)
{
	m_pGLSceneRef = NULL;
	m_dScale   = 1.0;
	m_isMButtonDown = false;
}

/**	\brief
*/
CDgnCanvas::~CDgnCanvas()
{
	try
	{
		for(vector<CGLObject*>::iterator itr = m_GLObjectSet.begin();itr != m_GLObjectSet.end();itr++)
		{
			delete (*itr);
		}
		m_GLObjectSet.clear();
	}
	catch(...)
	{
	}
}

/**	\brief	The CDgnCanvas::SetScene function

	\param	pGLScene	a parameter of type CDgnScene*

	\return	void	
*/
void CDgnCanvas::SetScene(CDgnScene* pGLScene) 
{
	assert(pGLScene && "pGLScene is NULL");

	if(pGLScene)
	{
		pGLScene->m_pCanvasRef = this;
		
		m_pGLSceneRef = pGLScene;
		m_dTrans[0] = 0;//-pGLScene->m_ptCenter.x();
		m_dTrans[1] = 0;//-pGLScene->m_ptCenter.y();

		m_SelectedBlock = -1;		// 선택된 블럭 없음
		m_PreselectedObject = -1;	// 가선택된 오브젝트 없음
		m_Prev_PreselectedObject = -1;	// 이전에 가선택된 오브젝트 없음

		this->Invalidate();
	}
}

/**	
	@brief	OpenGL화면을 그린다.

	@author	humkyung

	@return	void	
*/
int CDgnCanvas::DrawScene()
{
	if(m_pGLSceneRef)
	{
		//////////////////////////////////////////////////////////////////////////
		glInitNames(); // 네임 스택 초기화
		glPushName(0);
		long names = 0;

		CDgnScene* pGLScene = static_cast<CDgnScene*>(m_pGLSceneRef);
		if(pGLScene->m_DgnDoc.get())
		{
			size_t nCount = pGLScene->m_DgnDoc->GetEntityCount();
			for(int i = 0;i < int(nCount);++i)
			{
				CDgnElement* pEnt = pGLScene->m_DgnDoc->GetEntityAt(i);
				glLoadName(names++); //! 선택시 사용할 오브젝트 이름
				RenderEnt(pEnt);
			}
		}
	}

	return ERROR_SUCCESS;
}

//	parameter	:
//	description	:
//	remarks		:
//	returns		:
void CDgnCanvas::Save(const char* pFilePath)
{
	assert(pFilePath && "pFilePath is NULL");

	if(pFilePath)
	{
	}
}

void CDgnCanvas::OnMButtonDblClk(UINT nFlags, CPoint point) 
{
	CGLCanvas::OnMButtonDblClk(nFlags, point);
}

static int __pick_mode = 0;

void CDgnCanvas::OnMButtonDown(UINT nFlags, CPoint point)
{
	m_isMButtonDown = true;
	if(true == m_isMButtonDown)
	{
		SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(IDC_SIZEALL)));
	}

	SetCapture();
	CGLCanvas::OnMButtonDown(nFlags,point);
}

void CDgnCanvas::OnMButtonUp(UINT nFlags, CPoint point)
{
	m_isMButtonDown = false;
	ReleaseCapture();
	CGLCanvas::OnMButtonUp(nFlags,point);
}

void CDgnCanvas::OnMouseMove(UINT nFlags, CPoint point)
{
	CGLCanvas::OnMouseMove(nFlags,point);
}

void CDgnCanvas::OnViewFit()
{
	CRect rect;
	GetClientRect(&rect);

	CIsVolume vol=m_pGLSceneRef->GetVolume();
	m_dTrans[2] = (vol.maxx() - vol.minx()) / rect.Width();
	m_dTrans[0] = 0;//-(m_pGLSceneRef->m_ptCenter.x());
	m_dTrans[1] = 0;//-(m_pGLSceneRef->m_ptCenter.y());
	m_dScale = 1.0;
	
	RedrawWindow();
}

/**
	@brief	set opengl color corresponding to dgn color

	@author	humkyung

	@date	2013.02.12
*/
int CDgnCanvas::SetColor3f(unsigned char color)
{
	switch(color)
	{
		case 3:
			glColor3f(1.0 , 0.0 , 0.0);
			break;
		case 4:
			glColor3f(1.0 , 1.0 , 0.0);
			break;
		case 7:
			glColor3f(0.0 , 0.0 , 1.0);
			break;
		default:
			glColor3f(1.0 , 1.0 , 1.0);
			break;
	}

	return ERROR_SUCCESS;
}

/**
	@brief	

	@author	BHK

	@date	?
*/
void CDgnCanvas::RenderEnt(CDgnElement *pEnt)
{
	assert(pEnt && "pEnt is NULL");

	if(pEnt)
	{
		SetColor3f(pEnt->color());	/// 2013.02.12 added by humkyung
		if(pEnt->IsKindOf(CDgnLine::TypeString()))
		{
			CDgnLine* pDgnLine = static_cast<CDgnLine*>(pEnt);
			const DPoint3d ptStart = pDgnLine->GetStartPoint();
			const DPoint3d ptEnd = pDgnLine->GetEndPoint();

			CGLLineObject glLine(ptStart.x , ptStart.y , ptEnd.x , ptEnd.y , _T(""));
			glLine.Render();
		}
		else if(pEnt->IsKindOf(CDgnPointString::TypeString()))
		{
			CDgnPointString* pDgnPointString = static_cast<CDgnPointString*>(pEnt);
			
			CGLSimplePolyObject glSimplePoly;
			const int nCount = pDgnPointString->GetPointCount();
			for(int i = 0;i < nCount;++i)
			{
				DPoint3d pt = pDgnPointString->GetPointAt(i);
				glSimplePoly.Add(CIsPoint3d(pt.x , pt.y , 0.f));
			}
			glSimplePoly.Render();
		}
		else if(pEnt->IsKindOf(CDgnLineString::TypeString()))
		{
			CDgnLineString* pDgnLineString = static_cast<CDgnLineString*>(pEnt);
			
			CGLSimplePolyObject glSimplePoly;
			const int nCount = pDgnLineString->GetVertexCount();
			for(int i = 0;i < nCount;++i)
			{
				DPoint3d pt = pDgnLineString->GetVertexAt(i);
				glSimplePoly.Add(CIsPoint3d(pt.x , pt.y , 0.f));
			}
			glSimplePoly.Render();
		}
		else if(pEnt->IsKindOf(CDgnArc::TypeString()))
		{
			CDgnArc* pDgnArc = static_cast<CDgnArc*>(pEnt);

			static CIsPoint2d ptEllipse[256];
			
			const double rotateAngle = DEG2RAD(pDgnArc->rotateAngle());
			const double nStartAngle = DEG2RAD(pDgnArc->startAngle());
			const double nSweepAngle = DEG2RAD(pDgnArc->sweepAngle());

			CIsPoint2d ptStart(pDgnArc->majorAxis()*cos(nStartAngle) , pDgnArc->minorAxis()*sin(nStartAngle));
			ptStart = ptStart.Rotate(pDgnArc->rotateAngle());

			///const double nStep = nSweepAngle / 36;
			///double cs = cos(nStep);
			///double sn = sin(nStep);
			
			const double major = pDgnArc->majorAxis();
			const double minor = pDgnArc->minorAxis();
			if((0 != major) && (0 != minor))
			{
				CIsPoint2d origin(pDgnArc->origin().x , pDgnArc->origin().y);
				CIsArc2d arc2d(origin , origin + ptStart , pDgnArc->sweepAngle() , minor / major);
				CGLArc2dObject glArc2d(36);
				glArc2d.Set(arc2d);
				glArc2d.Render();
			}
		}
		else if(pEnt->IsKindOf(CDgnEllipse::TypeString()))
		{
			CDgnEllipse* pDgnEllipse = static_cast<CDgnEllipse*>(pEnt);

			const double rotateAngle = DEG2RAD(pDgnEllipse->rotateAngle());
			const double nStartAngle = DEG2RAD(pDgnEllipse->startAngle());
			const double nSweepAngle = DEG2RAD(pDgnEllipse->sweepAngle());

			///const double nStep = nSweepAngle / 36;
			///double cs = cos(nStep);
			///double sn = sin(nStep);
			
			const double major = pDgnEllipse->majorAxis();
			const double minor = pDgnEllipse->minorAxis();
			if((0 != major) && (0 != minor))
			{
				CIsEllipse2d ellipse2d;
				ellipse2d.SetOrigin(CIsPoint2d(pDgnEllipse->origin().x , pDgnEllipse->origin().y));
				ellipse2d.SetMajorAxis(major);
				ellipse2d.SetMinorAxis(minor);
				ellipse2d.SetStartAngle(pDgnEllipse->startAngle());
				ellipse2d.SetSweepAngle(pDgnEllipse->sweepAngle());
				ellipse2d.SetRotatedAngle(pDgnEllipse->rotateAngle());

				CGLEllipse2dObject glEllipse2d;
				glEllipse2d.Set(ellipse2d);
				glEllipse2d.Render();
				/*
				const double ab=major / minor;
				const double ba=minor / major;
				
				ptEllipse[0].Set(pDGNEllipse2d->majorAxis()*cos(nStartAngle) , pDGNEllipse2d->minorAxis()*sin(nStartAngle));
				for(int i = 1;i < 36;i++)
				{
					ptEllipse[i].Set(ptEllipse[i-1].x()*cs - ab*ptEllipse[i-1].y()*sn , ba*ptEllipse[i-1].x()*sn + ptEllipse[i-1].y()*cs);
				}
				ptEllipse[i].Set(pDGNEllipse2d->majorAxis()*cos(nStartAngle + nSweepAngle) , pDGNEllipse2d->minorAxis()*sin(nStartAngle + nSweepAngle));
				
				cs = cos(rotateAngle);
				sn = sin(rotateAngle);
				for(i = 0;i < 36 + 1;i++)
				{
					double x = ptEllipse[i].x();
					double y = ptEllipse[i].y();
					ptEllipse[i].Set(cs * x - sn * y , sn * x + cs * y);
				}

				CIsPoint2d origin = pDGNEllipse2d->origin();
				glBegin(GL_LINE_STRIP);
				for(i=0;i < 36 + 1;i++)
				{
					glVertex3f((float)(ptEllipse[i].x() + origin.x()),(float)(ptEllipse[i].y() + origin.y()),(float)0.);
				}
				glEnd();
				*/
			}
		}
		else if(pEnt->IsKindOf(CDgnText::TypeString()))
		{
			CDgnText* pDgnText = static_cast<CDgnText*>(pEnt);

			CDgnScene* pScene = static_cast<CDgnScene*>(m_pGLSceneRef);
			CGLFont* pFont = pScene->GetGLFont(0);
			if(NULL != pFont) SetGLFont(pFont);
			
			TextOutGL(pDgnText->origin().x , pDgnText->origin().y , (float)0.,
				pDgnText->GetTextWidth() , pDgnText->GetTextHeight() , 
				pDgnText->angle() , pDgnText->textString(),255,255,255);
		}
		else if(pEnt->IsKindOf(CDgnComplexShapeHeader::TypeString()))
		{
			CDgnComplexShapeHeader* pDGNComplexShape2d = static_cast<CDgnComplexShapeHeader*>(pEnt);

			size_t nCount = pDGNComplexShape2d->GetEntityCount();
			for(int i = 0;i < int(nCount);++i)
			{
				CDgnElement* pEnt = pDGNComplexShape2d->GetEntityAt(i);
				RenderEnt(pEnt);
			}
		}
		else if(pEnt->IsKindOf(CDgnCellHeader::TypeString()))
		{
			CDgnCellHeader* pDGNCellHeader2d = static_cast<CDgnCellHeader*>(pEnt);

			size_t nCount = pDGNCellHeader2d->GetEntityCount();
			for(int i = 0;i < int(nCount);++i)
			{
				CDgnElement* pEnt = pDGNCellHeader2d->GetEntityAt(i);
				RenderEnt(pEnt);
			}
		}
	}
}

/**
	@brief	현재 선택된 ITEM을 그려준다.

	@author	humkyung

	@date	?
*/
int CDgnCanvas::DrawHighlights()
{
	CDgnScene* pGLScene = static_cast<CDgnScene*>(m_pGLSceneRef);
	if(m_pGLSceneRef)
	{
		long block = 0; /// 선택 블록

		if (m_PreselectedObject > 0)
		{
			block = m_PreselectedObject;
			
			glColor3ub(255 , 175 , 50);
			glLineWidth(1.75f);
			
			if(block < int(pGLScene->m_DgnDoc->GetEntityCount()))
			{
				CDgnElement* pEnt = pGLScene->m_DgnDoc->GetEntityAt(block);
				if(pEnt) RenderEnt(pEnt);
			}
			else
			{
				CGLObject* pObj = m_GLObjectSet.at(block - pGLScene->m_DgnDoc->GetEntityCount());
				if(pObj) pObj->Render();
			}

			glLineWidth(1.0f);
		}
		
		return ERROR_SUCCESS;
	}

	return ERROR_BAD_ENVIRONMENT;
}

/**
	@brief	drawing text at postion given by user

	@author	humkyung

	@date	????.??.??
*/
void CDgnCanvas::Pick(double& x , double& y , const double& dScaleFactor , CString rTag)
{
	__pick_mode = 1;
	SetFocus();

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if(WM_LBUTTONDOWN == msg.message)
		{
			CPoint pt;
			pt.x = LOWORD(msg.lParam);
			pt.y = HIWORD(msg.lParam);
			
			float uX = 0 , uY = 0;
			WIN2GL(&uX , &uY , pt);
			
			x = uX;
			y = uY;
			SetGLObject(x , y , dScaleFactor , rTag);
			Render();
			RedrawWindow();

			__pick_mode = 0;

			break;
		}
		else
		{
			// Not an accelerator, so dispatch as normal.
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

void CDgnCanvas::Pick(float& x1 , float& y1 , float& x2 , float& y2 , CString rTag)
{
	static bool LButtonDown = false;
	__pick_mode = 2;
	SetFocus();

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if(WM_LBUTTONDOWN == msg.message)
		{
			CPoint point;
			point.x = LOWORD(msg.lParam);
			point.y = HIWORD(msg.lParam);

			CRect rcClient;
			GetClientRect(&rcClient);
			MapWindowPoints(NULL,(LPPOINT)&rcClient,2);
			ClipCursor(&rcClient);
			SetCapture();
			
			m_rcSelect.SetRect(point.x,point.y,point.x,point.y);
			//
			CSelectionDC   dc(this);
			OnDrawSelection(&dc);
			
			LButtonDown = true;
		}
		else if(WM_MOUSEMOVE == msg.message)
		{
			CPoint point;
			point.x = LOWORD(msg.lParam);
			point.y = HIWORD(msg.lParam);

			if( LButtonDown )
			{
				CSelectionDC   dc(this);

				OnDrawSelection(&dc);
				m_rcSelect.right  = point.x;
				m_rcSelect.bottom = point.y;
				OnDrawSelection(&dc);
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else if(WM_LBUTTONUP == msg.message)
		{
			if( LButtonDown )
			{
				CSelectionDC   dc(this);
				OnDrawSelection(&dc);
				//
				ClipCursor(NULL);
				ReleaseCapture();
				//m_bZooming  = FALSE;
				//
				CRect rcClient;
				GetClientRect(&rcClient);
				rcClient.NormalizeRect();
				m_rcSelect.NormalizeRect();
				//
				WIN2GL(&x1 , &y1 , m_rcSelect.TopLeft());
				WIN2GL(&x2 , &y2 , m_rcSelect.BottomRight());
				//
				SetGLObject(x1 , y1 , x2 , y2 , rTag);
				InvalidateRect( NULL,TRUE );

				__pick_mode = 0;
				LButtonDown = false;
				break;
			}
			else
			{
				// Not an accelerator, so dispatch as normal.
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			// Not an accelerator, so dispatch as normal.
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

void CDgnCanvas::Pick(float& distance , CString rTag)
{
	static bool LButtonDown = false;
	SetFocus();

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if(WM_LBUTTONDOWN == msg.message)
		{
			CPoint point;
			point.x = LOWORD(msg.lParam);
			point.y = HIWORD(msg.lParam);

			CRect rcClient;
			GetClientRect(&rcClient);
			MapWindowPoints(NULL,(LPPOINT)&rcClient,2);
			ClipCursor(&rcClient);
			SetCapture();
			
			m_rcSelect.SetRect(point.x,point.y,point.x,point.y);
			//
			CSelectionDC   dc(this);
			OnDrawLine(&dc);
			
			LButtonDown = true;
		}
		else if(WM_MOUSEMOVE == msg.message)
		{
			CPoint point;
			point.x = LOWORD(msg.lParam);
			point.y = HIWORD(msg.lParam);

			if( LButtonDown )
			{
				CSelectionDC   dc(this);
				//
				OnDrawLine(&dc);
				m_rcSelect.right  = m_rcSelect.left;
				m_rcSelect.bottom = point.y;
				OnDrawLine(&dc);
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else if(WM_LBUTTONUP == msg.message)
		{
			if( LButtonDown )
			{
				CSelectionDC   dc(this);
				OnDrawLine(&dc);
				//
				ClipCursor(NULL);
				ReleaseCapture();
				//m_bZooming  = FALSE;
				//
				CRect rcClient;
				GetClientRect(&rcClient);
				rcClient.NormalizeRect();
				m_rcSelect.NormalizeRect();
				//
				float x1 , y1 , x2 , y2;
				WIN2GL(&x1 , &y1 , m_rcSelect.TopLeft());
				WIN2GL(&x2 , &y2 , m_rcSelect.BottomRight());
				float dx = x2  - x1 , dy = y2 - y1;
				distance = sqrt(dx * dx + dy * dy);
				//
				InvalidateRect( NULL,TRUE );

				LButtonDown = false;
				break;
			}
			else
			{
				// Not an accelerator, so dispatch as normal.
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			// Not an accelerator, so dispatch as normal.
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

/**
	@brief	
	
	@author	humkyung

	@date	????.??.??
**/
void CDgnCanvas::SetGLObject(GLdouble x, GLdouble y, const GLdouble& dScaleFactor , const char *pTag)
{
	assert(pTag && "pTag is NULL");

	if(pTag)
	{
		CGLObject* pGLObject = NULL;
		for(vector<CGLObject*>::iterator itr = m_GLObjectSet.begin();itr != m_GLObjectSet.end();itr++)
		{
			if((*itr)->IsKindOf(CGLTextObject::TypeString()))
			{
				CGLTextObject* pObj = static_cast<CGLTextObject*>(*itr);
				if(string(pTag) == string(pObj->GetTextString()))
				{
					pGLObject = pObj;
					break;
				}
			}
		}

		if(NULL != pGLObject)
		{
			CGLTextObject* pGLTextObj = static_cast<CGLTextObject*>(pGLObject);
			
			pGLTextObj->SetWidthFactor(m_textWidth * dScaleFactor);
			pGLTextObj->SetTextHeight(m_textHeight * dScaleFactor);
			const GLdouble dTextLength = static_cast<CGLTextObject*>(pGLObject)->GetTextLength();
			const GLdouble dTextHeight = static_cast<CGLTextObject*>(pGLObject)->GetTextHeight();
			
			pGLTextObj->Set(x - (dTextLength*0.5) , y - (dTextHeight*0.5), pTag);
		}
		else
		{
			CGLTextObject* pGLTextObj = new CGLTextObject(x , y , pTag);
			if(pGLTextObj)
			{
				pGLTextObj->SetTextStyle( m_sTextStyle );

				pGLTextObj->SetWidthFactor(m_textWidth * dScaleFactor);
				pGLTextObj->SetTextHeight(m_textHeight * dScaleFactor);

				const GLdouble dTextLength = pGLTextObj->GetTextLength();
				const GLdouble dTextHeight = pGLTextObj->GetTextHeight();
				pGLTextObj->Set(x - (dTextLength*0.5) , y - (dTextHeight*0.5), pTag);

				m_GLObjectSet.push_back(pGLTextObj);
			}
		}
	}
}

/**
	@brief	

	@author	HumKyung

	@date	????.??.??

	@param
	@param
	@param
	@param
	@param

	@return
*/
void CDgnCanvas::SetGLObject(GLdouble x1 , GLdouble y1 , GLdouble x2 , GLdouble y2 , const CString& rTag)
{
	CGLObject* pGLObject = NULL;
	for(vector<CGLObject*>::iterator itr = m_GLObjectSet.begin();itr != m_GLObjectSet.end();itr++)
	{
		if((*itr)->IsKindOf(CGLRectObject::TypeString()) && (rTag == CString((*itr)->tag())))
		{
			pGLObject = (*itr);
			break;
		}
	}
	
	if(NULL != pGLObject)
	{
		CGLRectObject* pGLRectObject = static_cast<CGLRectObject*>(pGLObject);
		pGLRectObject->Set(x1 , y1 , x2 , y2 , rTag);
	}
	else
	{
		m_GLObjectSet.push_back(new CGLRectObject(x1 , y1 , x2 , y2 , rTag));
	}	
}

/**
	@brief

	@author	humkyung
**/
bool CDgnCanvas::DestroyGLObject(const TCHAR* pTag)
{
	assert(pTag && "pTag is NULL");

	if(pTag)
	{
		CGLObject* pGLObject = NULL;
		for(vector<CGLObject*>::iterator itr = m_GLObjectSet.begin();itr != m_GLObjectSet.end();itr++)
		{
			if(string(pTag) == string((*itr)->tag()))
			{
				delete (*itr);
				m_GLObjectSet.erase(itr--);
				return true;
			}
		}
	}

	return false;
}
