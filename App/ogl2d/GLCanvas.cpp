// GLCanvas.cpp : implementation file
//

#include "stdafx.h"
#include <assert.h>
#include <math.h>
#include <IsVect2d.h>
#include "GLCanvas.h"

#include "gl/gl.h"
#include "gl/glu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////
// CSelectionDC
void CSelectionDC::Init()
{
	m_Pen.CreatePen(PS_SOLID,0,RGB(200,200,200));
	m_pPenOld   = SelectObject(&m_Pen);
	m_pBrushOld = (CBrush*)SelectStockObject(NULL_BRUSH);
	m_iOldROP   = SetROP2(R2_XORPEN);
}
	
CSelectionDC::CSelectionDC(const CDC &cdc)
{
	m_hWnd = NULL;
	if( !Attach(cdc.m_hDC) )   AfxThrowResourceException();
	Init();
}
	
CSelectionDC::CSelectionDC(CWnd *pWnd)
{
	if( !Attach(::GetDC(m_hWnd = pWnd->GetSafeHwnd())) )  AfxThrowResourceException();
	Init();
}
	
CSelectionDC::~CSelectionDC()
{
	SetROP2(m_iOldROP);
	SelectObject(m_pBrushOld);
	SelectObject(m_pPenOld);
	if( m_hWnd )
	{
		::ReleaseDC(m_hWnd,Detach());
	}
	else
	{
		Detach();
	}
}
/////////////////////////////////////////////////////////////////////////////
// CGLCanvas
/**	\brief
*/
CGLCanvas::CGLCanvas() : m_pGLSceneRef(NULL) , m_dScale(1.) , m_pGLFontRef(NULL)
{
	m_hRC = NULL;
	m_hDC = NULL;
	m_bBuffered = FALSE;
	
	m_szSize = CSize(1,1);
	
	m_ptLast = CPoint(0,0);
	m_nPressed = 0;
	
	m_rmActual    = RM_WIREFRAME;
	m_nTextHeight = 2.0;
	m_dRot = 0.0;
	m_dTrans[0] = m_dTrans[1] = m_dTrans[2] = 0.0;
	m_dTrans[2] = 1.0;
	m_nOpCode = OP_NONE;
}
 
//	parameter	:
//	description	:
//	remarks		:
//	returns		:
CGLCanvas::~CGLCanvas() 
{
}

BEGIN_MESSAGE_MAP(CGLCanvas, CWnd)
	//{{AFX_MSG_MAP(CGLCanvas)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGLCanvas message handlers
/**	\brief	The CGLCanvas::Create function

	\param	bBuffered	a parameter of type BOOL
	\param	prect	a parameter of type CRect*
	\param	pParentWnd	a parameter of type CWnd*
	\param	nId	a parameter of type int

	\return	BOOL	
*/
BOOL CGLCanvas::Create(BOOL bBuffered, CRect* prect, CWnd* pParentWnd, int nId) 
{
	if (m_strWndClassName == TEXT("")) 
	{
		// Register window class
		m_strWndClassName = AfxRegisterWndClass(
			CS_OWNDC | CS_VREDRAW | CS_SAVEBITS | CS_HREDRAW | CS_DBLCLKS,GetCursor());
	}
	ASSERT(m_strWndClassName != "");
	CWnd::Create(m_strWndClassName, "", WS_CHILD | SS_BLACKRECT | WS_VISIBLE, *prect, pParentWnd, nId);
	m_bBuffered = bBuffered;
	return TRUE;
}

/**	
	@brief	The CGLCanvas::Render function

	@author	BHK

	@date	?

	@return	void	
*/
void CGLCanvas::Render() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	if(m_pGLSceneRef)
	{
		CIsVolume vol = m_pGLSceneRef->GetVolume();
		const double x = (vol.minx() + vol.maxx()) * 0.5;
		const double y = (vol.miny() + vol.maxy()) * 0.5;

		glOrtho(vol.minx() - x - 100 , vol.maxx() - x + 100 ,
			vol.miny() - y - 100 , vol.maxy() - y + 100 ,
			-1.0f , 1.0f);
		
		glMatrixMode(GL_MODELVIEW);
		
		glLoadIdentity();
		
		GLfloat lightPos[4] = {0, 0, -1000000, 1};
		GLfloat lightAmbient[4] = {0.6f, 0.6f, 0.6f, 1.0f};
		GLfloat lightSpec[4] = { 0.9f, 0.9f, 0.9f, 1.0f };
		GLfloat lightSpot[4] = {0,0,-1, 1};
		
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);
		
		
		glTranslatef((float)(m_dTrans[0]) , (float)(m_dTrans[1]) , 0); 
		glScalef((float)(m_dScale) , (float)(m_dScale) , (float)(m_dScale));
		glTranslatef((float)(-x) , (float)(-y) , 0); 

		glEnable(GL_DEPTH_TEST); // 깊이 검사를 함 : 먼곳의 물체는 가까운 곳의 물체에 가려짐
		DrawScene();		 // 선택 가능한 오브젝트(블럭 및 좌표축)를 그림

		glDisable(GL_DEPTH_TEST); // 깊이 검사를 하지 않음 : 나중에 그려지는 물체가 기존 물체 위에 그려짐
		DrawHighlights();         // 선택/가선택 오브젝트를 강조 표시
		
		if (m_bBuffered) SwapBuffers(m_hDC);//SwapBuffers(m_pDC->m_hDC);
	}
}

/**	\brief	The CGLCanvas::OnPaint function


	\return	void	
*/
void CGLCanvas::OnPaint() 
{
	MakeCurrent();
	
	GLfloat lightPos[4] = {0, 0, -1000000, 1};
	GLfloat lightAmbient[4] = {0.6f, 0.6f, 0.6f, 1.0f};
	GLfloat lightSpec[4] = { 0.9f, 0.9f, 0.9f, 1.0f };
	GLfloat lightSpot[4] = {0,0,-1, 1};
	
	// General geometric attribs
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LINE_STIPPLE);
	
	// Rendering mode
	glFrontFace(GL_CCW);
	glPolygonMode(GL_BACK,GL_NONE);
	switch (m_rmActual) {
		case RM_WIREFRAME:
			//glPolygonMode(GL_FRONT,GL_LINE);
			glPolygonMode(GL_FRONT,GL_FILL);
			glCullFace(GL_NONE);
			glShadeModel(GL_FLAT);
			break;
		case RM_FLAT:
			glPolygonMode(GL_FRONT, GL_FILL);
			glCullFace(GL_BACK);
			glShadeModel(GL_FLAT);
			break;
		case RM_SMOOTH:
			glPolygonMode(GL_FRONT, GL_FILL);
			glShadeModel(GL_SMOOTH);
			break;
	}
  
	// Erase
	glClearColor(0.0f , 0.0f , 0.0f , 1.0f);
	
	Render();

	glFlush();
	
	CPaintDC dc(this); // device context for painting
}

CString CGLCanvas::m_strWndClassName = "";

/**	\brief	The CGLCanvas::PreCreateWindow function

	\param	cs	a parameter of type CREATESTRUCT&

	\return	BOOL	
*/
BOOL CGLCanvas::PreCreateWindow(CREATESTRUCT& cs) 
{
	// Modify ClassStruct style bits
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CS_OWNDC;
	cs.lpszClass = (LPCTSTR) m_strWndClassName;
	return (CWnd::PreCreateWindow(cs));
}

/**	\brief	The CGLCanvas::OnSize function

	\param	nType	a parameter of type UINT
	\param	cx	a parameter of type int
	\param	cy	a parameter of type int

	\return	void	
*/
void CGLCanvas::OnSize(UINT nType, int cx, int cy) 
{  
	MakeCurrent();
	
	if (cx == 0) cx = 1; 		// 0으로 나누는 것을 방지
	
	glMatrixMode(GL_PROJECTION); 	// 투영 행렬 선택
	glLoadIdentity();            	// (투영) 행렬 초기화
	glViewport(0, 0, cx, cy); 	// 윈도우 상에 그려질 영역(뷰포트)을 설정
	
	// 윈도우의 가로, 세로 비율에 맞춰 투영 공간의 좌표 설정
	// 화면의 중심을 좌표계 원점(0,0)으로 하여, 화면과 모델링 공간의 크기를 1:1로 대응
	if(NULL != m_pGLSceneRef)
	{
		CIsVolume vol = m_pGLSceneRef->GetVolume();
		double x = (vol.minx() + vol.maxx()) * 0.5;
		double y = (vol.miny() + vol.maxy()) * 0.5;

		glOrtho(vol.minx() - x - 100 , vol.maxx() - x + 100 ,
			vol.miny() - y - 100 , vol.maxy() - y + 100 ,
			-1.0f , 1.0f);
	}
}

/**	\brief	The CGLCanvas::OnCreate function

	\param	lpCreateStruct	a parameter of type LPCREATESTRUCT

	\return	int	
*/
int CGLCanvas::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	m_hDC = *GetDC();
	
	PIXELFORMATDESCRIPTOR pfd;
	
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 32;
	
	pfd.iLayerType = PFD_MAIN_PLANE;
	int nPixelFormat = ChoosePixelFormat(m_hDC, &pfd);
	ASSERT(nPixelFormat != 0);
	VERIFY(SetPixelFormat(m_hDC, nPixelFormat, &pfd) != 0);
	
	/// 렌더링 컨텍스트를 얻는다.
	m_hRC = wglCreateContext(m_hDC);
	if (!m_hRC) 
	{
		CString str;
		str.Format("Error creating Opengl Graphic Context: %d", GetLastError());
		AfxMessageBox(str);
	}

	/// 현재의 렌더링 컨텍스트를 만든다.
	if( FALSE == wglMakeCurrent(m_hDC, m_hRC))
	{
		::AfxMessageBox( "wglMakeCurrent 함수 실패" );
		return FALSE;
	}
	//wglMakeCurrent(m_pDC->m_hDC, m_hRC);
	ASSERT(m_hRC != 0);
	
	return 0;
}

//	parameter	:
//	description	:
//	remarks		:
//	returns		:
UINT CGLCanvas::SolveOp(UINT nFlags) 
{
	if ((nFlags & MK_RBUTTON) && (nFlags & MK_LBUTTON)) 
	{
		return OP_PAN;
	}
	else
	{
		if (nFlags & MK_MBUTTON) return OP_PAN;
		else if (nFlags & MK_LBUTTON) return OP_ZOOM;
	}
	return OP_NONE;
}

void DrawTrackCross(CWnd* pWnd , int x, int y)
{
    CRect rectClient;
    pWnd->GetClientRect(&rectClient);
    
    glEnable(GL_COLOR_LOGIC_OP);
    glLogicOp(GL_XOR);
    // drawing different rubber-banding rectangle
    // depending on the mouse movement x-direction
	glColor3f(1.0, 0.0, 0.0);
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // OpenGL window coordinates are different from GDI's
    glBegin(GL_LINES);
	glVertex3f(rectClient.left , y , 0.0);
	glVertex3f(rectClient.right , y , 0.0);
	glVertex3f(x , rectClient.top , 0.0);
	glVertex3f(x , rectClient.bottom , 0.0);
	glEnd();

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glFlush(); // must flush here
    glDisable(GL_COLOR_LOGIC_OP);
}

/**	
	@brief	The CGLCanvas::OnMouseMove function
	@author	humkyung
	@param	nFlags	a parameter of type UINT
	@param	point	a parameter of type CPoint
	@return	void	
*/
void CGLCanvas::OnMouseMove(UINT nFlags, CPoint point) 
{
	static bool __bMouseCursorLineOn__ = true;

	CRect rect;
	GetClientRect(&rect);
	
	bool bSomethingChanged = false;

	if(m_pGLSceneRef)
	{
		CSelectionDC dc(this);
		/// erase mouse cursor line
		if((true == __bMouseCursorLineOn__) && (FALSE == m_tracker.IsAeroEnabled()))
		{
			dc.MoveTo(0 , m_ptLast.y);
			dc.LineTo(rect.right , m_ptLast.y);
			dc.MoveTo(m_ptLast.x , 0);
			dc.LineTo(m_ptLast.x , rect.bottom);
		}
		/// up to here

		CIsVolume vol = m_pGLSceneRef->GetVolume();
		const double width = vol.maxx() - vol.minx();
		const double height= vol.maxy() - vol.miny();

		const static double nRatio=0.9;

		const double dx = ((double)(point.x - m_ptLast.x));
		const double dy = ((double)(point.y - m_ptLast.y));
		m_ptLast = point;

		/// draw cursor line - 2011.11.13 added by humkyung
		if((true == __bMouseCursorLineOn__) && (FALSE == m_tracker.IsAeroEnabled()))
		{
			dc.MoveTo(0 , m_ptLast.y);
			dc.LineTo(rect.right , m_ptLast.y);
			dc.MoveTo(m_ptLast.x , 0);
			dc.LineTo(m_ptLast.x , rect.bottom);

			__bMouseCursorLineOn__ = true;
		}
		/// up to here

		switch (SolveOp(nFlags)) 
		{
			case OP_PAN:
				m_dTrans[0] += dx*width /(rect.Width());
				m_dTrans[1] -= dy*height/(rect.Height());
				Invalidate(FALSE);

				__bMouseCursorLineOn__ = false;
				break;
			case OP_ROTATE:
			{
				CIsVect2d dir(dx , dy);
				m_dRot = dir.angle();

				Invalidate(FALSE);

				__bMouseCursorLineOn__ = false;
			}
				break;
			case OP_ZOOM:
				if (dy == 0) break;
				if (dy > 0)
					m_dScale *= 1.0*nRatio + dy*0.1;
				else
					m_dScale /= (1.0*nRatio - dy*0.1);
				Invalidate(FALSE);
				
				__bMouseCursorLineOn__ = false;
				break;
			default:
			{
				if (!nFlags)
				{
					if (PickObjects(point) > 0) // 커서 위치의 오브젝트 수 검사
					{
						//SetCursor(m_CursorPick); // 손 모양 커서 설정
						m_PreselectedObject = m_SelectBuff[3]; // 셀렉션 버퍼의 첫번째 오브젝트
					}
					else 	m_PreselectedObject = -1; // 아무 것도 선택되지 않음
					
					if (m_PreselectedObject != m_Prev_PreselectedObject) // 가선택 오브젝트가 바뀌면...
					{ 
						m_Prev_PreselectedObject = m_PreselectedObject;
						bSomethingChanged = true;
					}
				}
			}
				break;
		}

		if (bSomethingChanged)
		{
			Render(); // 화면 갱신
			__bMouseCursorLineOn__ = false;
		}
	}

	CWnd::OnMouseMove(nFlags, point);
}

/**	\brief	The CGLCanvas::OnMButtonDown function

	\param	nFlags	a parameter of type UINT
	\param	point	a parameter of type CPoint

	\return	void	
*/

void CGLCanvas::OnMButtonDown(UINT nFlags, CPoint point) 
{
	SetCapture();
	
	CWnd::OnMButtonDown(nFlags, point);
}

/**	\brief	The CGLCanvas::OnMButtonUp function

	\param	nFlags	a parameter of type UINT
	\param	point	a parameter of type CPoint

	\return	void	
*/
void CGLCanvas::OnMButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	
	CWnd::OnMButtonUp(nFlags, point);
}

/**	\brief	The CGLCanvas::OnMButtonDblClk function

	\param	nFlags	a parameter of type UINT
	\param	point	a parameter of type CPoint

	\return	void	
*/
void CGLCanvas::OnMButtonDblClk(UINT nFlags,CPoint point)
{
	OnViewFit();
	CWnd::OnMButtonDblClk(nFlags,point);
}

/**	\brief	The CGLCanvas::OnLButtonDown function

	\param	nFlags	a parameter of type UINT
	\param	point	a parameter of type CPoint

	\return	void	
*/
void CGLCanvas::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetFocus();
	SetCapture();

	CWnd::OnLButtonDown(nFlags, point);
}

/**	\brief	The CGLCanvas::OnLButtonUp function

	\param	nFlags	a parameter of type UINT
	\param	point	a parameter of type CPoint

	\return	void	
*/
void CGLCanvas::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	CWnd::OnLButtonUp(nFlags, point);
}

//	parameter	:
//	description	:
//	remarks		:
//	returns		:
void CGLCanvas::OnRButtonDown(UINT nFlags, CPoint point) 
{
	float mx , my;
	WIN2GL(&mx , &my , point);

	CString rMsg;
	rMsg.Format("X = %lf,Y = %lf\n" , mx , my);
	AfxMessageBox(rMsg);
	
	SetCapture();
	
	CWnd::OnRButtonDown(nFlags, point);
}

//	parameter	:
//	description	:
//	remarks		:
//	returns		:
void CGLCanvas::OnRButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	
	CWnd::OnRButtonUp(nFlags, point);
}


//	parameter	:
//	description	:
//	remarks		:
//	returns		:
void CGLCanvas::WIN2GL(float *uX, float *uY, CPoint point)
{
	if(m_pGLSceneRef)
	{
		GLdouble Modelmatrix[16];
		GLdouble Projmatrix[16];
		GLint Viewport[16];

		VERIFY(wglMakeCurrent(m_hDC,m_hRC));                
		glMatrixMode(GL_MODELVIEW);    //현재 행렬스택을 모델뷰 메트릭스로
		glLoadIdentity();              //단위 행렬로 초기화
		glPushMatrix();
		
		CIsVolume vol = m_pGLSceneRef->GetVolume();
		GLdouble x = (vol.minx() + vol.maxx()) * 0.5;
		GLdouble y = (vol.miny() + vol.maxy()) * 0.5;

		//glRotated(float(RAD2DEG(m_dRot)), 1.0, 0.0, 0.0);                     //X회전
		glTranslated(m_dTrans[0] , m_dTrans[1] , 0.);
		glScaled(m_dScale, m_dScale, 1.);     //스케일링
		glTranslated((float)(-x) , (float)(-y) , 0);
		
		//위의 회전과 스케일링으로 만들어진 매트릭스 얻어오기
		::glGetDoublev(GL_MODELVIEW_MATRIX , Modelmatrix);         
		::glGetDoublev(GL_PROJECTION_MATRIX , Projmatrix);
		::glGetIntegerv(GL_VIEWPORT , Viewport);  
		
		CRect rect;
		GetClientRect(&rect);

		GLdouble z = 0;
		gluUnProject(point.x , rect.Height() - point.y , 0. , Modelmatrix,Projmatrix,Viewport, &x , &y , &z);
		glPopMatrix();
		VERIFY(wglMakeCurrent(NULL,NULL));

		(*uX) = (GLfloat)(x);
		(*uY) = (GLfloat)(y);
	}
}

/**	
	@brief	The CGLCanvas::TextOutGL function

	@author	humkyung

	@param	x	a parameter of type float
	@param	y	a parameter of type float
	@param	z	a parameter of type float
	@param	nScale	a parameter of type float
	@param	nRotate	a parameter of type float
	@param	str	a parameter of type 
                                const char *
	@param	r	a parameter of type unsigned char
	@param	g	a parameter of type unsigned char
	@param	b	a parameter of type unsigned char

	@return	void	
*/
void CGLCanvas::TextOutGL(	float x,float y,float z,
				float nXScale,float nYScale , 
				float nRotate,	/* I - Font to use */
				const char *str,unsigned char r,unsigned char g,unsigned char b)/* I - String to display */
{
	assert(str && "str is NULL");

	if(str && m_pGLFontRef)
	{
		glColor3f((float)r/255.,(float)g/255.,(float)b/255.);

		m_pGLFontRef->SetXScale(nXScale);
		m_pGLFontRef->SetYScale(nYScale);
		m_pGLFontRef->SetZScale(1.f);
		m_pGLFontRef->SetZRotate(nRotate);
		m_pGLFontRef->GLDrawText(x,y,z,str);
		glColor3f((float)1,(float)1,(float)1);
	}
}

/**	\brief	The CGLCanvas::TextOutGL function

	\param	pDC	a parameter of type CDC *
	\param	x	a parameter of type float
	\param	y	a parameter of type float
	\param	z	a parameter of type float
	\param	nScale	a parameter of type float
	\param	nRotate	a parameter of type float
	\param	str	a parameter of type 
                          const char *
	\param	r	a parameter of type unsigned char
	\param	g	a parameter of type unsigned char
	\param	b	a parameter of type unsigned char

	\return	void	
*/
void CGLCanvas::TextOutGL(CDC *pDC,
			  float x,float y,float z,
			  float nXScale,float nYScale,
			  float nRotate,	/* I - Font to use */
			  const char *str,unsigned char r,unsigned char g,unsigned char b)/* I - String to display */
{
	assert(pDC && "pDC is NULL");
	assert(str && "str is NULL");

	if(pDC && str && m_pGLFontRef)
	{
		m_pGLFontRef->SetXScale(nXScale);
		m_pGLFontRef->SetYScale(nYScale);
		m_pGLFontRef->SetZScale(1.);
		m_pGLFontRef->SetZRotate(nRotate);
		m_pGLFontRef->GLDrawText(pDC,x,y,z,str);
	}
}

void CGLCanvas::SetGLFont(CGLFont *pGLFont)
{
	m_pGLFontRef = pGLFont;
}

HCURSOR CGLCanvas::GetCursor()
{
	return LoadCursor(NULL, MAKEINTRESOURCE(IDC_CROSS));
}

void CGLCanvas::Clear()
{
	m_pGLFontRef = NULL;
}

BOOL CGLCanvas::OnEraseBkgnd(CDC* pDC) 
{
	return FALSE;
}

void CGLCanvas::OnViewFit()
{

}

void CGLCanvas::OnDrawSelection(CDC* pDC)
{
	pDC->MoveTo(m_rcSelect.left ,m_rcSelect.bottom);
	pDC->LineTo(m_rcSelect.right,m_rcSelect.bottom);
	pDC->LineTo(m_rcSelect.right,m_rcSelect.top   );
	pDC->LineTo(m_rcSelect.left ,m_rcSelect.top   );
	pDC->LineTo(m_rcSelect.left ,m_rcSelect.bottom);
}

void CGLCanvas::OnDrawLine(CDC* pDC)
{
	pDC->MoveTo(m_rcSelect.left ,m_rcSelect.top);
	pDC->LineTo(m_rcSelect.right,m_rcSelect.bottom);
}

void CGLCanvas::Zoom(const CRect &rcClient, const CRect &rcSelection)
{

}

BOOL CGLCanvas::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{	
	if (nFlags & (MK_SHIFT | MK_LBUTTON)) // 시프트 키나 마우스 왼쪽 버튼을 같이 누르면 세밀하게 조절
		zDelta >>= 2;

	float NewScale = m_dScale * ( 1.0f - zDelta * 0.0025);
	// 최소/최대 확대 비율 범위를 넘지 않도록 제한
	if (NewScale > 10000.0f)
		NewScale = 10000.0f;
	else if (NewScale < 0.001)
		NewScale = 0.001;
	
	float ScaleFactor = NewScale / m_dScale;

	CRect rect;
	this->GetWindowRect(&rect);
	//ScreenToClient(&rect);

	// 주: pt는 스크린 좌상단을 기점(0,0)으로 하는 좌표임
	m_dTrans[0] = m_dTrans[0] * ScaleFactor + (ScaleFactor - 1.0f) * (rect.left + rect.Width() / 2 - pt.x);
	m_dTrans[1] = m_dTrans[1] * ScaleFactor - (ScaleFactor - 1.0f) * (rect.top + rect.Height() / 2 - pt.y);
	m_dScale = NewScale;

	RedrawWindow(); // 화면 갱신
	
	return CWnd::OnMouseWheel(nFlags,zDelta,pt);
}

GLint CGLCanvas::PickObjects(CPoint point)
{
	GLint hits, viewport[4];
	CRect rect;
	
	glSelectBuffer(SELECT_BUFFER_LENGTH, m_SelectBuff);	//셀렉션 버퍼 설정
	glGetIntegerv(GL_VIEWPORT, (int *) viewport); // OnSize 내에서 설정한 뷰포트를 얻음
	
	glMatrixMode(GL_PROJECTION); // 투영 행렬 선택
	glPushMatrix();              // (투영) 행렬 저장
	
	glRenderMode(GL_SELECT); // 선택 모드로 전환
	glLoadIdentity();        // (투영) 행렬 초기화
	// 선택 영역 설정 : 윈도우와 OpenGL 좌표계는 +Y 방향이 서로 반대임 
	gluPickMatrix(point.x, viewport[3] - point.y, 5 , 5 , (int *) viewport);
	
	this->GetWindowRect(rect); // 윈도우 좌표를 얻음
	// 투영 행렬 설정 : OnSize 내에서 호출한 것과 동일하게 설정
	CIsVolume vol = m_pGLSceneRef->GetVolume();
	const double x = (vol.minx() + vol.maxx()) * 0.5;
	const double y = (vol.miny() + vol.maxy()) * 0.5;

	glOrtho(vol.minx() - x - 100 , vol.maxx() - x + 100 ,
		vol.miny() - y - 100 , vol.maxy() - y + 100 ,
		-1.0f , 1.0f);
	
	DrawScene(); // 오브젝트를 다시 그려 선택 영역 내의 오브젝트를 셀렉션 버퍼에 저장
	hits = glRenderMode(GL_RENDER); // 렌더링 모드로 전환 : 선택 영역 내의 오브젝트 갯수 반환
	
	glMatrixMode(GL_PROJECTION); // 투영 행렬 선택
	glPopMatrix();               // 저장된 (투영) 행렬 복구
	
	glMatrixMode(GL_MODELVIEW); // 모델뷰 행열 선택
	
	return hits; //선택 영역 내의 오브젝트 갯수 반환
}

/**
	@brief	등록된 GLObject들의 Render함수를 호출한다.
	@author	BHK
	@date	?
	@return	int
*/
int CGLCanvas::DrawScene()
{
	if(m_pGLSceneRef)
	{
		//////////////////////////////////////////////////////////////////////////
		glInitNames(); // 네임 스택 초기화
		glPushName(0);

		for(map<long , CGLObject*>::const_iterator itr = m_pGLSceneRef->m_pGLObjectMap->begin();itr != m_pGLSceneRef->m_pGLObjectMap->end();++itr)
		{
			glLoadName(itr->first); //! 선택시 사용할 오브젝트 이름
			itr->second->Render();
		}
	}

	return ERROR_SUCCESS;	
}

/**
	@brief	현재 선택된 GLObject를 다시 그린다.

	@author	BHK

	@date	?

	@return	int
*/
int CGLCanvas::DrawHighlights()
{
	long block = 0L; // 선택 블록
	if (m_PreselectedObject > 0)
	{
		block = m_PreselectedObject;
		
		glColor3ub(255 , 175 , 50);
		glLineWidth(1.75f);
		
		map<long , CGLObject*>::iterator where = m_pGLSceneRef->m_pGLObjectMap->find(block);
		if(where != m_pGLSceneRef->m_pGLObjectMap->end()) where->second->Render();

		glLineWidth(1.0f);
	}

	return ERROR_SUCCESS;
}

void CGLCanvas::SetOpMode(const UINT &mode)
{
	m_nOpCode = mode;
}
