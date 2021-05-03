// Admin.h : main header file for the ADMIN application
//

#if !defined(AFX_ADMIN_H__23E14FFE_9F28_421B_91CD_797B33E1FEBE__INCLUDED_)
#define AFX_ADMIN_H__23E14FFE_9F28_421B_91CD_797B33E1FEBE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "singleinstance.h"
#include "QGLCanvas.h"
#include "QGLScene.h"
/////////////////////////////////////////////////////////////////////////////
// CAdminApp:
// See Admin.cpp for the implementation of this class
//

class CAdminApp : public CWinAppEx , public CSingleInstance
{
public:
	CAdminApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdminApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL
	
	HINSTANCE m_hOldLangRes;
	HINSTANCE m_hMultiLangRes;

// Implementation
	//{{AFX_MSG(CAdminApp)
	afx_msg void OnAppAbout();
	afx_msg void OnAppClose();
	afx_msg void OnToUserApp();
	afx_msg void OnGeneralSetting();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL InitLanguage();
};

extern CQGLScene  m_GLScene;
extern CQGLCanvas m_GLCanvas;
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADMIN_H__23E14FFE_9F28_421B_91CD_797B33E1FEBE__INCLUDED_)
