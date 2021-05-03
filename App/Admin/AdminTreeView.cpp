// AdminTreeView.cpp : implementation file
//

#include "stdafx.h"
#include "Admin.h"
#include "AdminTreeView.h"

#include "MainFrm.h"
#include "AdminView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString CAdminTreeView::m_rTreeItemText[5];
/////////////////////////////////////////////////////////////////////////////
// CAdminTreeView

IMPLEMENT_DYNCREATE(CAdminTreeView, CView)

CAdminTreeView::CAdminTreeView()
{
}

CAdminTreeView::~CAdminTreeView()
{
	m_ImageList.DeleteImageList();
}


BEGIN_MESSAGE_MAP(CAdminTreeView, CView)
	//{{AFX_MSG_MAP(CAdminTreeView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY(NM_CLICK, 0x100, OnClickAdminTree) 
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdminTreeView drawing

void CAdminTreeView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CAdminTreeView diagnostics

#ifdef _DEBUG
void CAdminTreeView::AssertValid() const
{
	CView::AssertValid();
}

void CAdminTreeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAdminTreeView message handlers

int CAdminTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	{
		CAdminApp* pApp = (CAdminApp*)AfxGetApp();
		///CResourceSwitch resSwitch(pApp->m_hMultiLangRes);
		m_rTreeItemText[0].LoadString(IDS_TREE_ADMINISTRATOR);
		m_rTreeItemText[1].LoadString(IDS_LINEMAN_DATA_IMPORT);
		m_rTreeItemText[2].LoadString(IDS_TREE_IDMS_DATABSE_SETTING);
		m_rTreeItemText[3].LoadString(IDS_TREE_SETTING);
	}

	DWORD style = WS_VISIBLE | WS_TABSTOP | WS_CHILD
			| TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES
			| TVS_DISABLEDRAGDROP;
	if(FALSE == m_AdminTreeCtrl.Create(style , CRect(0 , 0 , 0 , 0) , this , 0x100))
	{
		CString rString;
		rString.LoadString(IDS_ERROR_CREATE_WND);

		AfxMessageBox(rString);
		return -1;
	}
	m_AdminTreeCtrl.SetItemHeight( 18 );
	
	m_ImageList.Create(IDB_ADMIN_TREE, 16, 6, RGB(255, 255, 255));
	m_AdminTreeCtrl.SetImageList(&m_ImageList, TVSIL_NORMAL);

	TV_INSERTSTRUCT TI;
	TI.hParent  = NULL;
	TI.item.mask= TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	TI.item.pszText = (char*)m_rTreeItemText[0].operator const char*();
	TI.item.iImage  = 0;
	TI.item.iSelectedImage = 0;
	HTREEITEM hParent =  m_AdminTreeCtrl.InsertItem(&TI);	

	TI.hParent      = hParent;
	TI.item.pszText = (char*)m_rTreeItemText[2].operator const char*();
	TI.item.iImage  = 1;
	TI.item.iSelectedImage = 1;
	HTREEITEM hItem =  m_AdminTreeCtrl.InsertItem(&TI);

	TI.hParent      = hParent;
	TI.item.pszText = (char*)m_rTreeItemText[1].operator const char*();
	TI.item.iImage  = 6;
	TI.item.iSelectedImage = 6;
	hItem =  m_AdminTreeCtrl.InsertItem(&TI);

	TI.hParent      = hParent;
	TI.item.pszText = (char*)m_rTreeItemText[3].operator const char*();
	TI.item.iImage  = 2;
	TI.item.iSelectedImage = 2;
	hItem =  m_AdminTreeCtrl.InsertItem(&TI);

	m_AdminTreeCtrl.Expand(m_AdminTreeCtrl.GetRootItem() , TVE_EXPAND);

	return 0;
}

void CAdminTreeView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	if(m_AdminTreeCtrl.m_hWnd)
	{
		m_AdminTreeCtrl.MoveWindow(CRect(CPoint(0 , 0) , CPoint(cx , cy)));
	}
}

void CAdminTreeView::OnClickAdminTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;
	UINT uFlags=0;
	
	DWORD pos = GetMessagePos();
	CPoint pt(LOWORD(pos), HIWORD(pos));
	m_AdminTreeCtrl.ScreenToClient(&pt);
	
	HTREEITEM hItem = m_AdminTreeCtrl.HitTest(pt,&uFlags);
	
	if((NULL != hItem) && (uFlags & TVHT_ONITEM))
	{
		CString rItemText = m_AdminTreeCtrl.GetItemText(hItem);
		if(rItemText == _T("Definition Table"))
		{
			OnClickDefTable();
		}
		else if(rItemText == _T("Property"))
		{
			
		}
	}
}

void CAdminTreeView::OnClickDefTable()
{
	CMainFrame* pMainFrame = (CMainFrame*)(AfxGetMainWnd());	
	//////////////////////////////////////////////////////////////////////////
	CAdminView* pAdminView = (CAdminView*)(pMainFrame->m_SplitterWnd.GetPane(0 , 1));
	pAdminView->ShowDefTableForm(true);
}

BOOL CAdminTreeView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	LPNMHDR hdr;
	LPNMTREEVIEW ntv;       
	
	hdr = (LPNMHDR)lParam;
	ntv = (LPNMTREEVIEW)lParam;
		
	if(hdr->hwndFrom == m_AdminTreeCtrl.m_hWnd)	
	{
		switch(hdr->code)//common notification codes. 
		{	
		
			case TVN_SELCHANGED:
			{   
				CString rItemText = m_AdminTreeCtrl.GetItemText(ntv->itemNew.hItem);
				if(rItemText == m_rTreeItemText[1])
				{
					OnClickLinemanDataImport();
				}
				else if(rItemText == m_rTreeItemText[2])
				{
					OnClickDefTable();
				}
				else if(rItemText == m_rTreeItemText[3])
				{
					OnClickSetting();
				}
			}
			break;
		}
		
	}
	
	return CView::OnNotify(wParam, lParam, pResult);
}

void CAdminTreeView::OnClickLinemanDataImport()
{
	CMainFrame* pMainFrame = (CMainFrame*)(AfxGetMainWnd());	
	//////////////////////////////////////////////////////////////////////////
	CAdminView* pAdminView = (CAdminView*)(pMainFrame->m_SplitterWnd.GetPane(0 , 1));
	if(pAdminView) pAdminView->ShowLineManDataImportForm(true);
}

void CAdminTreeView::OnClickSetting()
{
	CMainFrame* pMainFrame = (CMainFrame*)(AfxGetMainWnd());	
	//////////////////////////////////////////////////////////////////////////
	CAdminView* pAdminView = (CAdminView*)(pMainFrame->m_SplitterWnd.GetPane(0 , 1));
	pAdminView->ShowSettingForm(true);
}
