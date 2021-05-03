// ogl2d.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "ogl2d.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// COgl2dApp

BEGIN_MESSAGE_MAP(COgl2dApp, CWinApp)
	//{{AFX_MSG_MAP(COgl2dApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COgl2dApp construction

COgl2dApp::COgl2dApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only COgl2dApp object

COgl2dApp theApp;

/**
	@brief	CGLFont pointer를 리턴한다. 로딩되어 있지 않을시에는 Font를 로딩하도록 한다.

	@author	BHK

	@date	2009.07.07
*/
CGLFont* COgl2dApp::GetGLFont(const string &rFontName)
{
	map<string , CGLFont*>::iterator where = m_GLFontMap.find(rFontName);
	if(where == m_GLFontMap.end())
	{
		CGLFont* pGLFont = new CGLFont;
		if(pGLFont)
		{
			//////////////////////////////////////////////////////////////////////////
			TCHAR szPathName[MAX_PATH] = {0,};
			GetModuleFileName(AfxGetInstanceHandle() , szPathName , MAX_PATH);
			
			string rPathName(szPathName);
			string::size_type at = rPathName.find_last_of('\\');
			if(at != string::npos) rPathName = rPathName.substr(0 , at);
			pGLFont->Read(string(rPathName + _T("\\fonts\\") + rFontName + _T(".vft")).c_str());

			m_GLFontMap.insert(make_pair(rFontName , pGLFont));
			return pGLFont;
		}
		return NULL;
	}

	return (where->second);
}
