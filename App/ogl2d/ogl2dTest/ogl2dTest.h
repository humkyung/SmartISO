// ogl2dTest.h : main header file for the OGL2DTEST application
//

#if !defined(AFX_OGL2DTEST_H__1B250811_24AF_4D3F_AA2E_166ABE292DCB__INCLUDED_)
#define AFX_OGL2DTEST_H__1B250811_24AF_4D3F_AA2E_166ABE292DCB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// COgl2dTestApp:
// See ogl2dTest.cpp for the implementation of this class
//

class COgl2dTestApp : public CWinApp
{
public:
	COgl2dTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COgl2dTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(COgl2dTestApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OGL2DTEST_H__1B250811_24AF_4D3F_AA2E_166ABE292DCB__INCLUDED_)
