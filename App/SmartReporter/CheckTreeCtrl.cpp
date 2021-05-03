// CheckTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "SmartReporter.h"
#include "CheckTreeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckTreeCtrl

CCheckTreeCtrl::CCheckTreeCtrl()
{
}

CCheckTreeCtrl::~CCheckTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CCheckTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CCheckTreeCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckTreeCtrl message handlers
void CCheckTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	UINT uFlags=0;
	HTREEITEM hti = HitTest(point,&uFlags);
	
	if( uFlags & TVHT_ONITEMSTATEICON )
	{
		int iImage = GetItemState( hti, TVIS_STATEIMAGEMASK ) >> 12;
		iImage = (1 == iImage) ? 2 : 1;
		SetItemState( hti, INDEXTOSTATEIMAGEMASK(iImage) , TVIS_STATEIMAGEMASK );
		
		CheckChildItemsOf(hti , iImage );
		CheckParentItemsOf(hti);

		return;
	}

	CTreeCtrl::OnLButtonDown(nFlags , point);
}

/**
**/
void CCheckTreeCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if( nChar == VK_SPACE )
	{
		HTREEITEM hti = GetSelectedItem();
		int iImage = GetItemState( hti, TVIS_STATEIMAGEMASK )>>12;
		iImage = (1 == iImage) ? 2 : 1;
		SetItemState( hti, INDEXTOSTATEIMAGEMASK(iImage) , TVIS_STATEIMAGEMASK );
		
		CheckChildItemsOf(hti , iImage );
		CheckParentItemsOf(hti);

		return;
	}

	CTreeCtrl::OnKeyDown(nChar , nRepCnt , nFlags);
}

/**
**/
int CCheckTreeCtrl::GetChildStateOf(HTREEITEM hItem)
{
	int res = 1;

	if(NULL != hItem)
	{
		HTREEITEM hChildItem = GetChildItem(hItem);
		if(NULL != hChildItem)
		{
			int iImage = GetItemState( hChildItem , TVIS_STATEIMAGEMASK ) >> 12;
			res = iImage;

			HTREEITEM hSiblingItem = hChildItem;
			do
			{
				hSiblingItem = GetNextSiblingItem(hSiblingItem);
				if(hSiblingItem)
				{
					iImage = GetItemState( hSiblingItem , TVIS_STATEIMAGEMASK ) >> 12;
					if(res != iImage)
					{
						res = 3;
						break;
					}
				}
			}while(NULL != hSiblingItem);
		}
	}

	return res;
}

/**
**/
void CCheckTreeCtrl::CheckChildItemsOf(HTREEITEM hItem , const int& nImage )
{
	if(NULL != hItem)
	{
		HTREEITEM hChildItem = GetChildItem(hItem);
		if(NULL != hChildItem)
		{
			SetItemState( hChildItem , INDEXTOSTATEIMAGEMASK(nImage) , TVIS_STATEIMAGEMASK );
			CheckChildItemsOf(hChildItem , nImage);

			HTREEITEM hSiblingItem = hChildItem;
			do
			{
				hSiblingItem = GetNextSiblingItem(hSiblingItem);
				if(hSiblingItem)
				{
					SetItemState( hSiblingItem , INDEXTOSTATEIMAGEMASK(nImage) , TVIS_STATEIMAGEMASK );
					CheckChildItemsOf(hSiblingItem , nImage);
				}
			}while(NULL != hSiblingItem);
		}
	}
}

/**
**/
void CCheckTreeCtrl::CheckParentItemsOf(HTREEITEM hItem)
{
	if(NULL != hItem)
	{
		HTREEITEM hParentItem = GetParentItem(hItem);
		if(NULL != hParentItem)
		{
			int nState = GetChildStateOf(hParentItem);
			SetItemState( hParentItem , INDEXTOSTATEIMAGEMASK(nState) , TVIS_STATEIMAGEMASK );

			CheckParentItemsOf(hParentItem);
		}
	}
}

/**
**/
void CCheckTreeCtrl::CheckItem(HTREEITEM hItem , const CHECK_STATE& check)
{
	if(NULL != hItem)
	{
		int iImage = (int)(check);
		SetItemState( hItem , INDEXTOSTATEIMAGEMASK(iImage) , TVIS_STATEIMAGEMASK );
	}
}

/**
**/
BOOL CCheckTreeCtrl::IsSelected(HTREEITEM hItem)
{
	if(NULL != hItem)
	{
		int iImage = GetItemState( hItem , TVIS_STATEIMAGEMASK ) >> 12;
		return (1 != iImage);
	}

	return FALSE;
}
