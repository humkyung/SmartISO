// DgnCanvas.h: interface for the CDgnCanvas class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../ogl2d/GLCanvas.h"
#include "../ogl2d/GLObject.h"
#include "DgnScene.h"

#define NORMAL_MODE	0x01
#define PICK_MODE	0x02

class CDgnCanvas : public CGLCanvas  
{
public:
	CDgnCanvas();
	//DECLARE_DYNCREATE(CDgnCanvas)

	virtual ~CDgnCanvas();
public:
	void SetGLObject(GLdouble x1 , GLdouble y1 , GLdouble x2 , GLdouble y2 , const CString& rTag);
	void SetGLObject(GLdouble x , GLdouble y , const GLdouble& dScaleFactor , const char* pTag);
	void Pick(float& x1 , float& y1 , float& x2 , float& y2 , CString rTag);
	void Pick(double& x , double& y , const double& dScaleFactor , CString rTag);
	void Pick(float& distance , CString rTag);
	virtual int DrawHighlights();
	void SetScale(double nScale){m_nScale = nScale;}
	CGLScene* GetScene(){return m_pGLSceneRef;}
	void SetScene(CDgnScene* pGLScene);
	int DrawScene();
	void Save(const TCHAR* pFilePath);
protected:
	virtual void OnViewFit();
	bool m_isMButtonDown;
	
	HCURSOR m_hNewCursor,m_hMoveCursor;
	CBitmap m_bmpCursor;
	HBITMAP m_hAndMask;
	HBITMAP m_hXorMask;
	
	HCURSOR m_hPenCursor;
	CBitmap m_penCursor;
	HBITMAP m_hAndMask2;
	HBITMAP m_hXorMask2;

	//{{AFX_MSG(CDgnCanvas)
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	bool DestroyGLObject(const char* pTag);
	string	 m_sTextStyle;	/// 2011.11.13 added by humkyung
	GLdouble m_textWidth;
	GLdouble m_textHeight;
private:
	int SetColor3f(unsigned char color);	/// set color - 2013.02.12 added by humkyung
	void RenderEnt(CDgnElement* pEnt);

	vector<CGLObject*> m_GLObjectSet;
};
