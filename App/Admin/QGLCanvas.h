// QGLCanvas.h: interface for the CQGLCanvas class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QGLCANVAS_H__0C7DE378_8828_4D53_B946_F54F00F5FC6F__INCLUDED_)
#define AFX_QGLCANVAS_H__0C7DE378_8828_4D53_B946_F54F00F5FC6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../ogl2d/GLCanvas.h"
#include "QGLScene.h"
#include "../ogl2d/GLObject.h"
#include "../ogl2d/jxglTracker.h"

#define NORMAL_MODE	0x01
#define PICK_MODE	0x02

class CQGLCanvas : public CGLCanvas  
{
public:
	CQGLCanvas();
	//DECLARE_DYNCREATE(CQGLCanvas)

	virtual ~CQGLCanvas();
public:
	void SetGLObjectColor(const GLdouble& red,const GLdouble& green,const GLdouble& blue);	/// set object color - 2015.11.03 added by humkyung
	void SetGLObject(GLdouble x1 , GLdouble y1 , GLdouble x2 , GLdouble y2 , const CString& rTag);
	void SetGLObject(GLdouble x , GLdouble y , const GLdouble& dScaleFactor , const char* pTag);
	void Pick(float& x1 , float& y1 , float& x2 , float& y2 , const CString&);
	void Pick(double& x , double& y , const double& dScaleFactor , CString rTag);
	void Pick(float& distance , CString rTag);
	virtual int DrawHighlights();
	void SetScale(double nScale){m_nScale = nScale;}
	CGLScene* GetScene(){return m_pGLSceneRef;}
	void SetScene(CQGLScene* pGLScene);
	int DrawScene();
	void Save(const char* pFilePath);
protected:
	virtual void OnViewFit();
	virtual HCURSOR GetCursor();
	bool m_isMButtonDown;
	
	HCURSOR m_hNewCursor,m_hMoveCursor;
	CBitmap m_bmpCursor;
	HBITMAP m_hAndMask;
	HBITMAP m_hXorMask;
	
	HCURSOR m_hPenCursor;
	CBitmap m_penCursor;
	HBITMAP m_hAndMask2;
	HBITMAP m_hXorMask2;

	jxglTracker m_tracker;

	//{{AFX_MSG(CQViewerView)
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	bool DestroyGLObject(const char* pTag);
	string	 m_sTextStyle;	/// 2011.11.13 added by humkyung
	GLdouble m_dTextWidth;
	GLdouble m_dTextHeight;
private:
	void RenderEnt(CDgnElement* pEnt);

	vector<CGLObject*> m_GLObjectSet;
	GLdouble m_red,m_green,m_blue;
};

#endif // !defined(AFX_QGLCANVAS_H__0C7DE378_8828_4D53_B946_F54F00F5FC6F__INCLUDED_)
