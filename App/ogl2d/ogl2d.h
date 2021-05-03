// ogl2d.h : main header file for the OGL2D DLL
//

#if !defined(AFX_OGL2D_H__EC7394FF_0915_4607_A1DD_38235D85BB5D__INCLUDED_)
#define AFX_OGL2D_H__EC7394FF_0915_4607_A1DD_38235D85BB5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// COgl2dApp
// See ogl2d.cpp for the implementation of this class
//
#include "GLFont.h"

#include <map>
#include <string>
using namespace std;

class COgl2dApp : public CWinApp
{
public:
	CGLFont* GetGLFont(const string& rFontName);
	COgl2dApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COgl2dApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(COgl2dApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	map<string , CGLFont*> m_GLFontMap;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OGL2D_H__EC7394FF_0915_4607_A1DD_38235D85BB5D__INCLUDED_)
