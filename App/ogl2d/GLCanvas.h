#if !defined(AFX_GLCANVAS_H__7C4843E4_C7E5_4AF9_A0B3_1CCA1B810648__INCLUDED_)
#define AFX_GLCANVAS_H__7C4843E4_C7E5_4AF9_A0B3_1CCA1B810648__INCLUDED_

#include "GLScene.h"
#include "GLFont.h"
#include "jxglTracker.h"

extern COORD a;;
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GLCanvas.h : header file
//

#ifdef	_OGL2D
#define	_OGL2D_EXT_CLASS	__declspec(dllexport)
#else
#define	_OGL2D_EXT_CLASS	__declspec(dllimport)
#endif

#define SELECT_BUFFER_LENGTH 512  // 선택 버퍼 크기

class _OGL2D_EXT_CLASS CSelectionDC : public CDC
{
private:
	int      m_iOldROP;
	CPen     m_Pen;
	CPen*    m_pPenOld;
	CBrush*  m_pBrushOld;
	HWND     m_hWnd;
	
private:
	void Init();
	
public:
	CSelectionDC(const CDC &cdc);
	CSelectionDC(CWnd *pWnd);
	
	virtual ~CSelectionDC();
};

/////////////////////////////////////////////////////////////////////////////
// CGLCanvas window

class _OGL2D_EXT_CLASS CGLCanvas : public CWnd
{
	static CString m_strWndClassName;
public:
	/**
	* @cmember,menum Operation being performed through mouse 
	*/
	enum EOperation 
	{ 
		OP_NONE,    ///@@emem No operation
		OP_ROTATE,  ///@@emem Rotating
		OP_PAN,     ///@@emem Panning
		OP_ZOOM,    ///@@emem Zooming
		OP_SELECT,
		OP_TRACKER
	};
	
	/**
	* @cmember,menum Rendering mode
	*/
	enum ERenderingMode 
	{ 
		RM_WIREFRAME,  ///@@emem Render objects in wireframe
		RM_FLAT,       ///@@emem Render objects in flat mode
		RM_SMOOTH      ///@@emem Render objects in smooth mode
	};
	
	//@access Construction
	CGLCanvas();

	//@access Initialization
	BOOL Create(BOOL bBuffered=FALSE, CRect* prect=NULL, CWnd* pParentWnd=NULL,int nId=0);
	
	//@access Overridables
	/**
	* @cmember,mfunc Rendering function
	*/
	virtual void Render();
	virtual void Render(CDC* pDC){}

	//@access Operations
	inline void MakeCurrent(void);
	UINT SolveOp(UINT nFlags);
	//@access Public Data Members
public:
	// Drawing mode
	BOOL m_bBuffered; ///@cmember Wether to buffer OpenGL output or not
	HGLRC m_hRC;      ///@cmember OpenGL rendering context
	HDC   m_hDC;
	
	// Mouse status vars
	CPoint m_ptLast;    ///@cmember Last Point captured 
	double m_dRot;   ///@cmember Rotation through x,y axis 
	double m_dScale;
	double m_dTrans[3]; ///@cmember Translation along x,y,z 
	double m_nTextHeight;
	UINT   m_nPressed;  ///@cmember Number of buttons pressed
	
	// Canvas width and height
	double m_dAspect;   ///@cmember Canvas sizex/sizey
	CSize m_szSize;     ///@cmember Size of the canvas 
	
	ERenderingMode m_rmActual; ///@cmember Current rendering mode
	char m_szFontPath[MAX_PATH];
	CGLFont* m_pGLFontRef;

	bool  SetFontPath(const char* pFontPath){strcpy(m_szFontPath,pFontPath);return true;}

	/**	
	@brief	The TextLength function

	@param	str	a parameter of type const char*
	@param	nScale	a parameter of type double

	@return	float
	*/
	float TextLength(const char* str/*,double nScale*/)
	{
		if(m_pGLFontRef)
			return m_pGLFontRef->TextLength(str/*,m_pGLFontRef->m_dWidthFactor,nScale*/);

		return 0.f;
	}

	void TextOutGL(	float x,float y,float z,float nXScale, float nYScale , 
		float nRotate,const char *str,unsigned char r,unsigned char g,unsigned char b);
	void TextOutGL(CDC *pDC, float x,float y,float z, float nXScale , float nYScale ,
		float nRotate, const char *str,unsigned char r,unsigned char g,unsigned char b);
	void SetRenderingMode(ERenderingMode rm) {m_rmActual = rm;}
	ERenderingMode GetRenderingMode() {return m_rmActual;}

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COGLCanvas)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetOpMode(const UINT& mode);
	void Zoom(const CRect &rcClient,const CRect& rcSelection);
	virtual void Clear();
	void SetGLFont(CGLFont* pGLFont);
	virtual ~CGLCanvas();

	// Generated message map functions
	CGLScene*  m_pGLSceneRef;
protected:
	//{{AFX_MSG(COGLCanvas)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDblClk(UINT nFlags,CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnMouseWheel(UINT nFlags,short zDelta,CPoint pt);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	void WIN2GL(float *uX, float *uY, CPoint point);
protected:
	virtual int DrawScene();
	virtual int DrawHighlights();
	virtual void OnViewFit();
	virtual HCURSOR GetCursor();
	virtual void OnDrawSelection(CDC* pDC);
	virtual void OnDrawLine(CDC* pDC);
	
	UINT   m_nOpCode;
	CRect  m_rcSelect;
	double m_nScale;
	jxglTracker m_tracker;

	/// 오브젝트 선택 관련
	GLuint m_SelectBuff[SELECT_BUFFER_LENGTH]; // 선택 버퍼
	long m_SelectedBlock;         // 선택된 블럭
	long m_PreselectedObject;      // 가선택된 블럭
	long m_Prev_PreselectedObject; // 이전의 가선택된 블럭
private:
	GLint PickObjects(CPoint point);
};

_OGL2D_EXT_CLASS void CGLCanvas::MakeCurrent(void) 
{
	if (wglGetCurrentContext() != m_hRC)
		wglMakeCurrent(m_hDC, m_hRC);
		//wglMakeCurrent(m_pDC->m_hDC, m_hRC);
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GLCANVAS_H__7C4843E4_C7E5_4AF9_A0B3_1CCA1B810648__INCLUDED_)
