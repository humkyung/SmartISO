// GLScene.h: interface for the CGLScene class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLSCENE_H__805BF65E_C607_45AE_A7D7_E8D9E2810A82__INCLUDED_)
#define AFX_GLSCENE_H__805BF65E_C607_45AE_A7D7_E8D9E2810A82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <IsVolume.h>
#include <IsPoint3d.h>
#include "GLObject.h"

#include <map>
using namespace std;

#ifndef _OGL2D
	#ifdef _UNICODE
		#ifdef _DEBUG
			#if _MSC_VER == 1600
				#pragma comment(lib, "ogl2d_vc100_unicode_debug")
				#pragma message("Automatically linking with ogl2d_vc100_unicode_debug.dll (Debug Unicode)")
			#else
				#pragma comment(lib, "ogl2d_vc90_unicode_debug")
				#pragma message("Automatically linking with ogl2d_vc90_unicode_debug.dll (Debug Unicode)")
			#endif
		#else
			#if _MSC_VER == 1600
				#pragma comment(lib, "ogl2d_vc100_unicode")
				#pragma message("Automatically linking with ogl2d_vc100_unicode.dll (Release Unicode)")
			#else
				#pragma comment(lib, "ogl2d_vc90_unicode")
				#pragma message("Automatically linking with ogl2d_vc90_unicode.dll (Release Unicode)")
			#endif
		#endif
	#else
		#ifdef _DEBUG
			#if _MSC_VER == 1600
				#pragma comment(lib, "ogl2d_vc100_debug")
				#pragma message("Automatically linking with ogl2d_vc100_debug.dll (Debug)")
			#else
				#pragma comment(lib, "ogl2d_vc90_debug")
				#pragma message("Automatically linking with ogl2d_vc90_debug.dll (Debug)")
			#endif
		#else
			#if _MSC_VER == 1600
				#pragma comment(lib, "ogl2d_vc100")
				#pragma message("Automatically linking with ogl2d_vc100.dll (Release)")
			#else
				#pragma comment(lib, "ogl2d_vc90")
				#pragma message("Automatically linking with ogl2d_vc90.dll (Release)")
			#endif
		#endif
	#endif // _UNICODE
#endif // USER_EXPORTS

#ifdef	_OGL2D
#define	_OGL2D_EXT_CLASS	__declspec(dllexport)
#else
#define	_OGL2D_EXT_CLASS	__declspec(dllimport)
#endif

class CGLCanvas;
class _OGL2D_EXT_CLASS CGLScene  
{
public:
	CGLScene();
	virtual ~CGLScene();
public:
	virtual void AddScene(const char* pFilePath)=0;
	virtual CIsVolume GetVolume() const;
public:
	int Add(CGLObject* pObj);
	virtual void Clear();
	CIsPoint3d m_ptCenter;
	map<long , CGLObject*>* m_pGLObjectMap;
protected:
	long m_nUpperBound;
	
	CIsVolume  m_vol;
	CGLCanvas* m_pCanvasRef;

	friend class CGLCanvas;
};

#endif // !defined(AFX_GLSCENE_H__805BF65E_C607_45AE_A7D7_E8D9E2810A82__INCLUDED_)
