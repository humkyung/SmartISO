// FileNameEditListBox.cpp : implementation file
//

#include "stdafx.h"
#include "Admin.h"
#include <EditListBox/InPlaceCtrls.h>
#include "FileNameEditListBox.h"
#include "FileNameFormatDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DROPDOWN_HEIGHT			100
/////////////////////////////////////////////////////////////////////////////
// CFileNameEditListBox

CFileNameEditListBox::CFileNameEditListBox()
{
	SetEditStyle( LBEX_EDITBUTTON );
}

CFileNameEditListBox::~CFileNameEditListBox()
{
}


BEGIN_MESSAGE_MAP(CFileNameEditListBox, CEditListBox)
	//{{AFX_MSG_MAP(CFileNameEditListBox)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileNameEditListBox message handlers
void CFileNameEditListBox::OnBrowseButton( int iItem )
{
	/*
	EndEditing(TRUE);
	
	m_nItem = iItem;
	if(FALSE == ::IsWindow(m_LineDataFieldComboBox.m_hWnd))
	{
		DWORD dwStyle = WS_BORDER | WS_CHILD | WS_VSCROLL |        //| WS_VISIBLE
			CBS_UPPERCASE | CBS_SORT | CBS_AUTOHSCROLL | CBS_DROPDOWNLIST; // for grid style only
		CreateComboBox(dwStyle);
	}
	
	CRect rect;
	GetItemRect(iItem , &rect);
	m_LineDataFieldComboBox.SetWindowPos(NULL, rect.left, rect.top, rect.Width() + 1, rect.Height() + DROPDOWN_HEIGHT, SWP_NOZORDER | SWP_SHOWWINDOW);
	m_LineDataFieldComboBox.SetFocus();
	
	if (m_LineDataFieldComboBox.GetCount()) m_LineDataFieldComboBox.ShowDropDown(TRUE);
	*/
	//	iItem;
	///	CSHBrowseDlg dlgBrowse;
	//	if ( dlgBrowse.DoModal() ) SetEditText( dlgBrowse.GetFullPath() );
};

BOOL CFileNameEditListBox::CreateComboBox(DWORD dwStyle)
{
	if (IsWindow(m_LineDataFieldComboBox.m_hWnd)) m_LineDataFieldComboBox.DestroyWindow();

	// force as not visible child window
	dwStyle = (WS_CHILD | WS_VSCROLL | dwStyle) & ~WS_VISIBLE;

	if (!m_LineDataFieldComboBox.Create(dwStyle, CRect(0,0,0,0), this, 0x100))
	{
		TRACE0("CreateComboBox() - failed to create combo box\n");
		return FALSE;
	}

	CFileNameFormatDlg* pDlg = (CFileNameFormatDlg*)(GetParent());
	if(pDlg)
	{
		for(vector<CString>::iterator itr = pDlg->m_FieldDescEntry.begin();itr != pDlg->m_FieldDescEntry.end();itr++)
		{
			m_LineDataFieldComboBox.AddString(*itr);
		}
	}

	SendMessage(WM_SETFONT, (WPARAM)GetFont()->m_hObject);

	return TRUE;
}

void CFileNameEditListBox::EditNew()
{
	if(!IsWindow(m_LineDataFieldComboBox.m_hWnd))
	{
		DWORD dwStyle = WS_BORDER | WS_CHILD | WS_VSCROLL |        //| WS_VISIBLE
			CBS_UPPERCASE | CBS_SORT | CBS_AUTOHSCROLL | CBS_DROPDOWNLIST; // for grid style only
		
		CreateComboBox(dwStyle);
	}

	//if(-1 == m_iEdited) EndEditing(FALSE);
	if(-1 == m_iEdited)
	{
		BeginEditing( GetCount() );
	}
}

void CFileNameEditListBox::BeginEditing(int nItem)
{
	if((-1 != m_iEdited) && (m_iEdited != nItem)) EndEditing(TRUE);
	
	if ( nItem > GetCount()-1 ) 
	{
		nItem = GetCount();
		SetItemHeight(nItem , 20);
		AddString( "" );
	}

	m_iEdited = nItem;
	SetCurSel(m_iEdited);
	RedrawWindow();
	
	if(!IsWindow(m_LineDataFieldComboBox.m_hWnd))
	{
		DWORD dwStyle = WS_BORDER | WS_CHILD | WS_VSCROLL |        //| WS_VISIBLE
			CBS_UPPERCASE | CBS_SORT | CBS_AUTOHSCROLL | CBS_DROPDOWNLIST; // for grid style only
		CreateComboBox(dwStyle);
	}

	CRect rect;
	GetItemRect(nItem , &rect);
	m_LineDataFieldComboBox.SetWindowPos(NULL, rect.left, rect.top, rect.Width() + 1, rect.Height() + DROPDOWN_HEIGHT, SWP_NOZORDER | SWP_SHOWWINDOW);
	m_LineDataFieldComboBox.SetFocus();
	
	CString rString;
	GetText(m_iEdited , rString);
	int nSelItem = m_LineDataFieldComboBox.FindString(0 , rString);
	if(-1 == nSelItem) nSelItem = 0;
	m_LineDataFieldComboBox.SetCurSel(nSelItem);

	//if (m_LineDataFieldComboBox.GetCount()) m_LineDataFieldComboBox.ShowDropDown(TRUE);
}

BOOL CFileNameEditListBox::OnBeginEditing(int iItem)
{
	if((-1 != iItem) && (iItem < GetCount()))
	{
		CString rString;
		GetText(iItem , rString);
		if(rString.IsEmpty()) return TRUE;

		int nFindItem = m_LineDataFieldComboBox.FindString(0 , rString);
		if(-1 != nFindItem) return TRUE;
	}

	return FALSE;
}

void CFileNameEditListBox::EndEditing(BOOL fCancel)
{
	m_LineDataFieldComboBox.ShowWindow(SW_HIDE);
	
	CString strNewItemText;
	m_LineDataFieldComboBox.GetWindowText( strNewItemText );
	if ( strNewItemText.IsEmpty() )
	{
		DeleteString( m_iEdited );
	}
	else if ( !fCancel )
	{
		int nItem = FindString(0 , strNewItemText);
		if((-1 == nItem) || (m_iEdited == nItem))
		{
			// Replace the text
			SetItemText( m_iEdited, LPCTSTR(strNewItemText) );
			// Select the edited item
			SetCurSel( m_iEdited );
		}
		else
		{
			AfxMessageBox("You should select unused field." , MB_ICONWARNING | MB_OK);
		}
	}
		
	m_iEdited = -1;
		
	RedrawWindow();
}

void CFileNameEditListBox::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	int nItem = -1;
	
	ClientToScreen( &point );
	nItem = ItemFromPt( point, FALSE );
	TRACE1( "OnLButtonDown: %d\n", nItem );
	
	if ((-1 != nItem) && (FALSE != OnBeginEditing( nItem )) )
	{
		// Begin Editing
		BeginEditing( nItem );
	}
	else
	{
		if(-1 != m_iEdited) EndEditing(FALSE);
	}

	//CEditListBox::OnLButtonDblClk(nFlags, point);
}

void CFileNameEditListBox::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default	
	ClientToScreen( &point );
	int nItem = ItemFromPt( point, FALSE );
	TRACE1( "OnLButtonDown: %d\n", nItem );
	
	if(-1 != nItem) SetCurSel(nItem);
	if((-1 != m_iEdited) && (m_iEdited != nItem))
	{
		EndEditing(FALSE);
	}

	//CEditListBox::OnLButtonDown(nFlags, point);
}
