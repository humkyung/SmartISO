// SelectionTreeListCtrl.cpp : implementation file
//


#include "stdafx.h"
#include "UserApp.h"
#include "SelectionTreeListCtrl.h"
#include "UserAppDocData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CSelectionTreeListCtrl

IMPLEMENT_DYNAMIC(CSelectionTreeListCtrl, CTreeCtrl)

CSelectionTreeListCtrl::CSelectionTreeListCtrl()
{

}

CSelectionTreeListCtrl::~CSelectionTreeListCtrl()
{
	try
	{
	}
	catch(...)
	{
	}
}


BEGIN_MESSAGE_MAP(CSelectionTreeListCtrl, CTreeCtrl)
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(NM_CLICK, &CSelectionTreeListCtrl::OnNMClick)
END_MESSAGE_MAP()

// CSelectionTreeListCtrl message handlers

/**
	@brief	
	@author humkyung	
	@date 2009-11-02 오전 10:23:55	
	@param	
	@return		
**/
void CSelectionTreeListCtrl::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMTREEVIEW* pNMTV = (NMTREEVIEW*)pNMHDR;

	CPoint ptTree= pNMTV->ptDrag;

	TVHITTESTINFO htinfo;
	htinfo.pt    = ptTree;
	htinfo.flags = TVHT_ONITEMICON;
	HTREEITEM hItem = HitTest (&htinfo);
	if(hItem)
	{
		int nImage = -1 , nSelectedImage = -1;
		GetItemImage(hItem , nImage , nSelectedImage);

		if(BST_UNCHECKED == GetCheck(hItem))
		{
			/// uncheck all of child items
			FindTreeItemAndCheck(hItem, FALSE);
		}
		else
		{
			/// check all of child items
			FindTreeItemAndCheck(hItem, TRUE);
			
			HTREEITEM hParent = this->GetParentItem(hItem);
			while(hParent)
			{
				SetCheck(hParent , TRUE);
				hParent = this->GetParentItem(hParent);
			}
		}
		NMHDR hdr;
		hdr.hwndFrom = GetSafeHwnd();
		hdr.idFrom = GetDlgCtrlID();
		hdr.code = WM_USER + 100;
		GetParent()->SendMessage(WM_NOTIFY,(WPARAM)hItem,(LPARAM)&hdr);

		*pResult = 0;
	}
	else
	{
		*pResult = 0;
	}
}

/**
	@brief	
	@author KHS	
	@date 2009-11-03 오전 10:32:49	
	@param	
	@return		
**/
int CSelectionTreeListCtrl::FindTreeItemAndCheck(HTREEITEM hItem, BOOL bCheck)
{
	if(hItem)
	{
		SetCheck(hItem, bCheck);
	
		HTREEITEM hChild = GetChildItem(hItem);
		do
		{
			if(1 == FindTreeItemAndCheck(hChild, bCheck))
			{
				return 1;
			}

		}while((hChild = GetNextSiblingItem(hChild)) != NULL);
	}
	
	return 0;
}

/******************************************************************************
    @author     humkyung
    @date       2012-09-24
    @class      CSelectionTreeListCtrl
    @function   CheckAll
    @return     int
    @brief
******************************************************************************/
int CSelectionTreeListCtrl::CheckAll()
{
	int res = ERROR_SUCCESS;
	FindTreeItemAndCheck(GetRootItem() , TRUE);
	return res;
}

/**
	@brief	
	@author humkyung
	@date 2009-08-05 오후 2:10:20
	@param
	@return
*/
void CSelectionTreeListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	POINT pt;
	::GetCursorPos(&pt);
	ScreenToClient(&pt);
	HTREEITEM hItem = GetFirstSelected();
	if(NULL != hItem)
	{
		UINT uiCol=0;
		CHeaderCtrl* pHeaderCtrl = GetHeaderCtrl();
		const int nItemCount = pHeaderCtrl->GetItemCount();
		for(int i = 0;i < nItemCount;++i)
		{
			CRect rect;
			pHeaderCtrl->GetItemRect(i , &rect);
			const int nHScroll = GetScrollPos(SB_HORZ);
			if((rect.left<=(point.x + nHScroll)) && ((rect.right)>=(point.x + nHScroll)))
			{
				uiCol = i;
				break;
			}
		}
		NMHDR hdr;
		hdr.hwndFrom = GetSafeHwnd();
		hdr.idFrom = GetDlgCtrlID();
		hdr.code = WM_LBUTTONDBLCLK;
		GetParent()->SendMessage(WM_NOTIFY,(WPARAM)uiCol,(LPARAM)&hdr);
	}

	CTreeListCtrl::OnLButtonDblClk(nFlags, point);
}