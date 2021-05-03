// DgnScene.h: interface for the CDgnScene class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <DgnDocument.h>

#include "../ogl2d/GLScene.h"
#include "../ogl2d/GLFont.h"

#include <string>
#include <map>
using namespace std;

class CDgnCanvas;
class CDgnScene : public CGLScene  
{
public:
	CDgnScene();
	virtual ~CDgnScene();
public:

private:
	void ParseLine();
	void Set(CDgnDocument* pDGNDoc);
public:
	bool IsEmpty();
	bool LoadGLFont(const TCHAR* pFilePath);
	CGLFont* GetGLFont(const int& font);
	virtual void AddScene(const TCHAR* pFilePath);
	static CDgnScene* GetInstance();
private:
	auto_ptr<CDgnDocument> m_DgnDoc;;

	STRING_T m_strGLFontPath;
	map<int,CGLFont*>* m_pGLFontMap;

	//////////////////////////////////////////////////////////////////////////
	static CDgnScene* m_pInst;
	//////////////////////////////////////////////////////////////////////////

	friend CDgnCanvas;
};
