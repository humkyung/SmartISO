#pragma once

#include <gui/ListCtrlEx.h>

#include <map>
using namespace std;

// CMFCTextColorListCtrl

class CMFCTextColorListCtrl : public CMFCListCtrl
{
	DECLARE_DYNAMIC(CMFCTextColorListCtrl)

public:
	CMFCTextColorListCtrl();
	virtual ~CMFCTextColorListCtrl();

	int ClearAllTextColor();
	int SetItemTextColor(const int& nRow , const int& nCol, const COLORREF& color);
protected:
	//virtual void Sort(int iColumn, BOOL bAscending); // Sort a specified column.

	COLORREF OnGetCellTextColor(int nRow,int nColumn);
	map<pair<int,int> , COLORREF > m_ItemColorMap;

	DECLARE_MESSAGE_MAP()
public:
	int OnCompareItems(LPARAM lParam1,LPARAM lParam2,int iColumn);
	afx_msg void OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
};
