// MFCButtonEx.cpp : implementation file
//

#include "stdafx.h"
#include "MFCButtonEx.h"


// CMFCButtonEx

IMPLEMENT_DYNAMIC(CMFCButtonEx, CMFCButton)

CMFCButtonEx::CMFCButtonEx()
{

}

CMFCButtonEx::~CMFCButtonEx()
{
}


BEGIN_MESSAGE_MAP(CMFCButtonEx, CMFCButton)
END_MESSAGE_MAP()

// CMFCButtonEx message handlers
//=============================================================================	
// Sets icon from resource id
CMFCButtonEx& CMFCButtonEx::SetIcon(UINT nIDResource)
//=============================================================================	
{
	HICON hIcon = (HICON) LoadImage(AfxGetInstanceHandle(),
								MAKEINTRESOURCE(nIDResource),
								IMAGE_ICON,
								0,
								0,
								0);
	HICON hOldIcon = CMFCButton::SetIcon(hIcon);
	if(hOldIcon) DestroyIcon(hOldIcon);

	return (*this);
}

