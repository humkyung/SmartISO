// BOMTextsWnd.cpp : implementation file
//

#include "stdafx.h"
#include "BOMTextsWnd.h"


// CBMOTextsWnd

IMPLEMENT_DYNAMIC(CBMOTextsWnd, CWnd)

CBMOTextsWnd::CBMOTextsWnd()
{

}

CBMOTextsWnd::~CBMOTextsWnd()
{
}


BEGIN_MESSAGE_MAP(CBMOTextsWnd, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CBMOTextsWnd message handlers



void CBMOTextsWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	CRect rect;
	GetWindowRect(&rect);
	ScreenToClient(&rect);

	CBrush brush(RGB(255,255,255));
	dc.FillRect(&rect , &brush);

	dc.SelectStockObject(ANSI_FIXED_FONT);
	for(std::map<double,CString>::reverse_iterator itr = m_oBmTexts.rbegin();itr != m_oBmTexts.rend();++itr)
	{
		dc.DrawText(itr->second , rect , DT_LEFT);
		CSize siz = dc.GetTextExtent(itr->second);
		rect.top += siz.cy;
	}

	CPen pen(PS_SOLID , 1 , RGB(255,0,0));
	CPen* pOldPen = dc.SelectObject(&pen);
	for(std::list<int>::const_iterator itr = m_oBmHeaderIndices.begin();itr != m_oBmHeaderIndices.end();++itr)
	{
		CString str;
		for(int i = 0;i < *itr;++i) str += _T("0");
		CSize siz = dc.GetTextExtent(str);

		dc.MoveTo(siz.cx,0);
		dc.LineTo(siz.cx,rect.bottom);
	}
}
