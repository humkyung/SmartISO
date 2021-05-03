#pragma once

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <TreeListCtrl.h>	/// 2012.04.05 added by humkyung

class CSelectionTreeListCtrl : public CTreeListCtrl
{
	DECLARE_DYNAMIC(CSelectionTreeListCtrl)
public:
	CSelectionTreeListCtrl();
	virtual ~CSelectionTreeListCtrl();
protected:
	DECLARE_MESSAGE_MAP()
	int FindTreeItemAndCheck(HTREEITEM hItem, BOOL bCheck);
public:
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	int CheckAll();	/// 2012.09.24 added by humkyung
};
