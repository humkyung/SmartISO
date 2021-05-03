// QGLScene.h: interface for the CQGLScene class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QGLSCENE_H__543B52F2_8409_4089_B9FA_914B8D1743D4__INCLUDED_)
#define AFX_QGLSCENE_H__543B52F2_8409_4089_B9FA_914B8D1743D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <DgnDocument.h>

#include "../ogl2d/GLScene.h"
#include "../ogl2d/GLFont.h"

#include <string>
#include <map>
using namespace std;

class CQGLCanvas;
class CQGLScene : public CGLScene  
{
public:
	CQGLScene();
	virtual ~CQGLScene();
public:

private:
	void ParseLine();
public:
	bool IsEmpty();
	bool LoadGLFont(const char* pFilePath);
	CGLFont* GetGLFont(const int& font);
	void Set(CDgnDocument* pDGNDoc);
	virtual void AddScene(const char* pFilePath);
	static CQGLScene* GetInstance();
private:
	CDgnDocument* m_pDGNDocRef;

	string m_strGLFontPath;
	map<int,CGLFont*>* m_pGLFontMap;

	//////////////////////////////////////////////////////////////////////////
	static CQGLScene* m_pInst;
	//////////////////////////////////////////////////////////////////////////

	friend CQGLCanvas;
};

#endif // !defined(AFX_QGLSCENE_H__543B52F2_8409_4089_B9FA_914B8D1743D4__INCLUDED_)
