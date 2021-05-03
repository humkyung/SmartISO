//////////////////////////////////////////////////////////////////////
//  ColorCursor.cpp : implementation of the CColorCursor class.
//  Author		    : Jiju George T (NeST)
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ColorCursor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CColorCursor::CColorCursor()
{

}

CColorCursor::~CColorCursor()
{

}

//////////////////////////////////////////////////////////////////////
//  CreateCursorFromBitmap 
//  Function to create a cursor from HBITMAP. 
//  Pass bitmaps having standard cursor sizes like 16*16, 32*32...
//////////////////////////////////////////////////////////////////////
HCURSOR CColorCursor::CreateCursorFromBitmap(HBITMAP hSourceBitmap,
											 COLORREF clrTransparent,
											 DWORD   xHotspot,DWORD   yHotspot)
{
	HCURSOR hRetCursor = NULL;

	do
	{
		if(NULL == hSourceBitmap)
		{
			break;
		}

		//Create the AND and XOR masks for the bitmap
		HBITMAP hAndMask = NULL;
		HBITMAP hXorMask = NULL;
		GetMaskBitmaps(hSourceBitmap,clrTransparent,hAndMask,hXorMask);
		if(NULL == hAndMask || NULL == hXorMask)
		{
			break;
		}

		//Create the cursor using the masks and the hotspot values provided
		ICONINFO iconinfo = {0};
		iconinfo.fIcon		= FALSE;
		iconinfo.xHotspot	= xHotspot;
		iconinfo.yHotspot	= yHotspot;
		iconinfo.hbmMask	= hAndMask;
		iconinfo.hbmColor	= hXorMask;

		hRetCursor = ::CreateIconIndirect(&iconinfo);

	}
	while(0);

	return hRetCursor;
}


//////////////////////////////////////////////////////////////////////
//  GetMaskBitmaps
//  Function to AND and XOR masks for a cursor from a HBITMAP. 
//////////////////////////////////////////////////////////////////////
void CColorCursor::GetMaskBitmaps(HBITMAP hSourceBitmap, COLORREF clrTransparent, 
								  HBITMAP &hAndMaskBitmap, HBITMAP &hXorMaskBitmap)
{
	// Get the system display DC
	HDC hDC = ::GetDC(NULL);
	
	// Create helper DC
	HDC hMainDC = ::CreateCompatibleDC(hDC);
	HDC hAndMaskDC = ::CreateCompatibleDC(hDC);
	HDC hXorMaskDC = ::CreateCompatibleDC(hDC);
	
	// Get the dimensions of the source bitmap
	BITMAP bm;
	::GetObject(hSourceBitmap, sizeof(BITMAP), &bm);
	
	// Create the mask bitmaps
	hAndMaskBitmap = ::CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL); // monochrome
	hXorMaskBitmap  = ::CreateCompatibleBitmap(hDC, bm.bmWidth, bm.bmHeight); // color
	
	// Release the system display DC
	::ReleaseDC(NULL, hDC);
	
	// Select the bitmaps to helper DC
	HBITMAP hOldMainBitmap = (HBITMAP)::SelectObject(hMainDC, hSourceBitmap);
	HBITMAP hOldAndMaskBitmap  = (HBITMAP)::SelectObject(hAndMaskDC, hAndMaskBitmap);
	HBITMAP hOldXorMaskBitmap  = (HBITMAP)::SelectObject(hXorMaskDC, hXorMaskBitmap);
	
	// Assign the monochrome AND mask bitmap pixels so that a pixels of the source bitmap
	//    with 'clrTransparent' will be white pixels of the monochrome bitmap
	::SetBkColor(hMainDC, clrTransparent);
	::BitBlt(hAndMaskDC, 0, 0, bm.bmWidth, bm.bmHeight, hMainDC, 0, 0, SRCCOPY);
	
	// Assign the color XOR mask bitmap pixels so that a pixels of the source bitmap
	//    with 'clrTransparent' will be black and rest the pixels same as corresponding
	//    pixels of the source bitmap
	::SetBkColor(hXorMaskDC, RGB(0, 0, 0));
	::SetTextColor(hXorMaskDC, RGB(255, 255, 255));
	::BitBlt(hXorMaskDC, 0, 0, bm.bmWidth, bm.bmHeight, hAndMaskDC, 0, 0, SRCCOPY);
	::BitBlt(hXorMaskDC, 0, 0, bm.bmWidth, bm.bmHeight, hMainDC, 0,0, SRCAND);
	
	// Deselect bitmaps from the helper DC
	::SelectObject(hMainDC, hOldMainBitmap);
	::SelectObject(hAndMaskDC, hOldAndMaskBitmap);
	::SelectObject(hXorMaskDC, hOldXorMaskBitmap);
	
	// Delete the helper DC
	::DeleteDC(hXorMaskDC);
	::DeleteDC(hAndMaskDC);
	::DeleteDC(hMainDC);

	/*
	HDC hDC					= ::GetDC(NULL);
	HDC hMainDC				= ::CreateCompatibleDC(hDC); 
	HDC hAndMaskDC			= ::CreateCompatibleDC(hDC); 
	HDC hXorMaskDC			= ::CreateCompatibleDC(hDC); 

	//Get the dimensions of the source bitmap
	BITMAP bm;
	::GetObject(hSourceBitmap,sizeof(BITMAP),&bm);

	
	hAndMaskBitmap	= ::CreateCompatibleBitmap(hDC,bm.bmWidth,bm.bmHeight);
	hXorMaskBitmap	= ::CreateCompatibleBitmap(hDC,bm.bmWidth,bm.bmHeight);

	//Select the bitmaps to DC
	HBITMAP hOldMainBitmap = (HBITMAP)::SelectObject(hMainDC,hSourceBitmap);
	HBITMAP hOldAndMaskBitmap	= (HBITMAP)::SelectObject(hAndMaskDC,hAndMaskBitmap);
	HBITMAP hOldXorMaskBitmap	= (HBITMAP)::SelectObject(hXorMaskDC,hXorMaskBitmap);

	//Scan each pixel of the souce bitmap and create the masks
	COLORREF MainBitPixel;
	for(int x=0;x<bm.bmWidth;++x)
	{
		for(int y=0;y<bm.bmHeight;++y)
		{
			MainBitPixel = ::GetPixel(hMainDC,x,y);
			if(MainBitPixel == clrTransparent)
			{
				::SetPixel(hAndMaskDC,x,y,RGB(255,255,255));
				::SetPixel(hXorMaskDC,x,y,RGB(0,0,0));
			}
			else
			{
				::SetPixel(hAndMaskDC,x,y,RGB(0,0,0));
				::SetPixel(hXorMaskDC,x,y,MainBitPixel);
			}
		}
	}
	
	::SelectObject(hMainDC,hOldMainBitmap);
	::SelectObject(hAndMaskDC,hOldAndMaskBitmap);
	::SelectObject(hXorMaskDC,hOldXorMaskBitmap);

	::DeleteDC(hXorMaskDC);
	::DeleteDC(hAndMaskDC);
	::DeleteDC(hMainDC);

	::ReleaseDC(NULL,hDC);
	*/
}
