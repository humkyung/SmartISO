// SmartISODiff.h : main header file for the RevISO DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CRevISOApp
// See RevISO.cpp for the implementation of this class
//

class CSmartISODiffApp : public CWinApp
{
public:
	CSmartISODiffApp();
	

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};


