// FileZilla - a Windows ftp client

// Copyright (C) 2002-2004 - Tim Kosse <tim.kosse@gmx.de>

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

// LocalFileListCtrl.cpp: Implementierungsdatei
//
 
#include "stdafx.h"
#include <assert.h>
#include <algorithm>
#include "UserApp.h"
#include "Options.h"
#include "FileListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileListCtrl

CFileListCtrl::CFileListCtrl(CWnd *pOwner) : 
	m_sortcolumn(0) , m_rFileFilter("*.*") , m_nColumnSize(0) , m_pColumns(NULL) , m_bDisplayFolder(true)
{
	ASSERT(pOwner);

	m_pOwner = pOwner;

	m_Fullpath = ".."; //Just anything invalid
	m_nStyle = -1;
	m_nHideColumns = 0;
	m_bUpdating = FALSE;

	m_nDragHilited = -1;

	SetColumnSize(4);
}

CFileListCtrl::~CFileListCtrl()
{
	try
	{
		if(NULL != m_pColumns) free((void*)m_pColumns);
		ClearFileData();
	}
	catch(...)
	{
		
	}
}


BEGIN_MESSAGE_MAP(CFileListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CFileListCtrl)
	ON_WM_CREATE()
	ON_MESSAGE(WM_APP+1, OnUpdateContinue)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_WM_DESTROY()
	ON_WM_DROPFILES()
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetdispinfo)
	ON_WM_KEYDOWN()
	ON_WM_VSCROLL()
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemchanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen f? Nachrichten CFileListCtrl 

void CFileListCtrl::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	POSITION selpos = GetFirstSelectedItemPosition();
	if (selpos)
	{
		int nItem = GetNextSelectedItem(selpos);
		int nIndex = m_IndexMapping[nItem];
		if (m_FileData[nIndex].bIsDir)
		{
			CString newpath;
			if (!nItem && m_Fullpath!="")
			{
				newpath=m_Fullpath;
				newpath.TrimRight('\\');
				int pos=newpath.ReverseFind('\\');
				newpath=newpath.Left(pos+1);
			}
			else
				newpath=m_Fullpath+m_FileData[nIndex].Name + "\\";
			//m_pOwner->SetLocalFolder(newpath);
			//m_pOwner->SetLocalFolderOut(newpath);

		}
		else
		{
			NMHDR hdr;
			hdr.hwndFrom = GetSafeHwnd();
			hdr.idFrom = GetDlgCtrlID();
			m_pOwner->SendMessage(WM_NOTIFY,(WPARAM)WM_LBUTTONDBLCLK,(LPARAM)&hdr);
			//int nAction = COptions::GetOptionVal(OPTION_LOCAL_DOUBLECLICK_ACTION);
			//if (nAction == 1)
			//{
			//	CString file = m_Fullpath+m_FileData[nIndex].Name;
			//	SHELLEXECUTEINFO sei = {0};
			//	sei.cbSize = sizeof(SHELLEXECUTEINFO);
			//	sei.lpFile = file;
			//	sei.nShow = SW_SHOWNORMAL;
			//	BOOL b = ShellExecuteEx(&sei);
			//}
			//else
			//{
			//	CMainFrame *pMainFrame=DYNAMIC_DOWNCAST(CMainFrame,GetParentFrame());
			//	pMainFrame->AddQueueItem(FALSE, GetItemText(nItem,0), "", m_Fullpath, CServerPath(), !nAction);
			//	if (!nAction)
			//		pMainFrame->TransferQueue(2);
			//}
			
		}
	}
	*pResult = 0;
}

void CFileListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
	
}


CString CFileListCtrl::GetFolder() const
{
	return m_Fullpath;
}

int CFileListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	int widths[6] = {130, 66 , 90 , 99 , 66};

	if (COptions::GetOptionVal(OPTION_REMEMBERLOCALCOLUMNWIDTHS))
	{
		CString tmp = COptions::GetOption(OPTION_LOCALCOLUMNWIDTHS);
		int pos = -1;
		int i;
		for (i = 0; i<3; i++)
		{
			int oldpos = pos + 1;
			pos = tmp.Find(_T(" "), oldpos);
			if (pos == -1)
				break;
			tmp.SetAt(pos, 0);
			int size=_ttoi(tmp.Mid(oldpos));
			if (size>0)
				widths[i]=size;
		}
		if (i==3)
		{
			int size=_ttoi(tmp.Mid(pos+1));
			if (size>0)
				widths[i]=size;
		}
	}

	CString str;
	str.LoadString(IDS_HEADER_FILENAME);
	InsertColumn(0,str,LVCFMT_LEFT, widths[0]);
	str.LoadString(IDS_HEADER_FILESIZE);
	InsertColumn(1,str,LVCFMT_RIGHT, widths[1]);
	str.LoadString(IDS_HEADER_FILETYPE);
	InsertColumn(2,str,LVCFMT_LEFT, widths[2]);
	str.LoadString(IDS_HEADER_LASTMODIFIED);
	InsertColumn(3,str,LVCFMT_LEFT, widths[3]);
	
	m_SortImg.Create( 8, 8, ILC_MASK, 3, 3 );
	HICON Icon;
	Icon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_EMPTY));
	m_SortImg.Add(Icon);
	Icon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_UP));
	m_SortImg.Add(Icon);
	Icon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_DOWN));
	m_SortImg.Add(Icon);
	m_SortImg.SetBkColor(CLR_NONE);
	
	SetListStyle(0);
	
	int nSort = COptions::GetOptionVal(OPTION_LOCALCOLUMNSORT);
	m_sortdir = (nSort >> 4) % 3;
	if (!m_sortdir)
		m_sortdir = 1;
	m_sortcolumn = (nSort >> 1) & 0x07;
	if (m_sortcolumn > 3)
		m_sortcolumn = 0;

	DragAcceptFiles(TRUE);

	SetExtendedStyle(LVS_EX_INFOTIP);

	return 0;
}

/////////////////////////////////////////////////
BOOL CFileListCtrl::GetSysImgList()
/////////////////////////////////////////////////
{
	CImageList sysImgList;
	SHFILEINFO shFinfo;
	
	HIMAGELIST hImageList = 0;
	CString errorMessage;
	TCHAR filename[MAX_PATH + 10];

	if (GetModuleFileName(0, filename, MAX_PATH + 10))
	{
		hImageList = (HIMAGELIST)SHGetFileInfo(filename,
							 0,
							 &shFinfo,
							 sizeof( shFinfo ),
							 SHGFI_SYSICONINDEX |
							 ((m_nStyle == LVS_ICON)?SHGFI_ICON:SHGFI_SMALLICON) );

		if (!hImageList)
		{
			int errorCode = GetLastError();
			TCHAR buffer[1000];
			memset(buffer, 0, 1000);
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, errorCode, 0, buffer, 999, 0);

			CString str;
			str.Format(_T("SHGetFileInfo failed with error %d: %s"), errorCode, buffer);
			errorMessage += str;
		}
	}
	else
	{
		int errorCode = GetLastError();
		TCHAR buffer[1000];
		memset(buffer, 0, 1000);
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, errorCode, 0, buffer, 999, 0);
		
		CString str;
		str.Format(_T("GetModuleFileName failed with error %d: %s"), errorCode, buffer);
		errorMessage += str;
	}
	if (!hImageList)
	{
		/*
		 * Fall back to C:\\
		 * Not bullerproof, but better than nothing
		 */		
		hImageList = (HIMAGELIST)SHGetFileInfo(_T("C:\\"),
							 0,
							 &shFinfo,
							 sizeof( shFinfo ),
							 SHGFI_SYSICONINDEX |
							 ((m_nStyle == LVS_ICON)?SHGFI_ICON:SHGFI_SMALLICON) );

		if (!hImageList)
		{
			int errorCode = GetLastError();
			TCHAR buffer[1000];
			memset(buffer, 0, 1000);
			FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, errorCode, 0, buffer, 999, 0);

			CString str;
			str.Format(_T("SHGetFileInfo failed with error %d: %s"), errorCode, buffer);
			if (errorMessage != _T(""))
				errorMessage += _T("\n");
			errorMessage += str;
		}
	}

	if (!hImageList)
	{
		AfxMessageBox(errorMessage);
		return FALSE;
	}

	sysImgList.Attach(hImageList);
	
	SetImageList( &sysImgList, (m_nStyle == LVS_ICON)?LVSIL_NORMAL:LVSIL_SMALL);
	sysImgList.Detach();

	return TRUE;
}

void CFileListCtrl::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	SortList(pNMListView->iSubItem);
	
	*pResult = 0;
}

BOOL greater1(const CString &str1, const CString &str2, BOOL isdir1, BOOL isdir2)
{
	if (isdir1 && !isdir2)
		return FALSE;
	if (!isdir1 && isdir2)
		return TRUE;

	if (str1.CollateNoCase(str2) > 0)
		return TRUE;
	return FALSE;
}

BOOL lesser(const CString &str1, const CString &str2, BOOL isdir1, BOOL isdir2)
{
	if (isdir1 && !isdir2)
		return TRUE;
	if (!isdir1 && isdir2)
		return FALSE;
	
	if (str2.CollateNoCase(str1) > 0)
		return TRUE;
	return FALSE;
}

BOOL greaterbytype(const CString &str1, const CString &str2, const BOOL isdir1, const BOOL isdir2, const CString &fn1, const CString &fn2)
{
	if (isdir1 && !isdir2)
		return FALSE;
	if (!isdir1 && isdir2)
		return TRUE;
	if (str1>str2)
		return TRUE;
	if (str1==str2)
	{
		if (fn1.CollateNoCase(fn2) > 0)
			return TRUE;
	}
	return FALSE;
}

BOOL lesserbytype(const CString &str1, const CString &str2, const BOOL isdir1, const BOOL isdir2, const CString &fn1, const CString &fn2)
{
	if (isdir1 && !isdir2)
		return TRUE;
	if (!isdir1 && isdir2)
		return FALSE;
	if (str1<str2)
		return TRUE;
	if (str1==str2)
	{
		if (fn1.CollateNoCase(fn2) < 0)
			return TRUE;
	}
	return FALSE;
}

BOOL greaterbytime(const CTime &time1, const CTime &time2, const BOOL &isdir1, const BOOL &isdir2, const CString &fn1, const CString &fn2)
{
	if (isdir1 && !isdir2)
		return FALSE;
	if (!isdir1 && isdir2)
		return TRUE;
	if (time1>time2)
		return TRUE;
	if (time1==time2)
	{
		if (fn1.CollateNoCase(fn2) > 0)
			return TRUE;
	}
	return FALSE;
}

BOOL lesserbytime(const CTime &time1, const CTime &time2, const BOOL &isdir1, const BOOL &isdir2, const CString &fn1, const CString &fn2)
{
	if (isdir1 && !isdir2)
		return TRUE;
	if (!isdir1 && isdir2)
		return FALSE;
	if (time1<time2)
		return TRUE;
	if (time1==time2)
	{
		if (fn1.CollateNoCase(fn2) < 0)
			return TRUE;
	}
	return FALSE;
}

BOOL greaterbysize(const __int64 &size1, const __int64 &size2, BOOL isdir1, BOOL isdir2, const CString &fn1, const CString &fn2)
{
	if (isdir1 && !isdir2)
		return FALSE;
	if (!isdir1 && isdir2)
		return TRUE;
	if (size1>size2)
		return TRUE;
	if (size1==size2)
	{
		if (fn1.CollateNoCase(fn2) > 0)
			return TRUE;
	}
	return FALSE;
}

BOOL lesserbysize(const __int64 &size1, const __int64 &size2, BOOL isdir1, BOOL isdir2, const CString &fn1, const CString &fn2)
{
	if (isdir1 && !isdir2)
		return TRUE;
	if (!isdir1 && isdir2)
		return FALSE;
	if (size1<size2)
		return TRUE;
	if (size1==size2)
	{
		if (fn1.CollateNoCase(fn2) < 0)
			return TRUE;
	}
	return FALSE;
}

void CFileListCtrl::quicksortbyname(const BOOL &direction, int anf, int ende)
{
	int l=anf;
	int r=ende;
	CString tmp;
	CString ref=m_FileData[m_IndexMapping[(l+r)/2]].lName;
	BOOL bRefIsDir=m_FileData[m_IndexMapping[(l+r)/2]].bIsDir;
	do
	{
		if (direction)
		{
			while (lesser (m_FileData[m_IndexMapping[l]].lName, ref, m_FileData[m_IndexMapping[l]].bIsDir, bRefIsDir) && (l<ende)) l++;
			while (greater1(m_FileData[m_IndexMapping[r]].lName, ref, m_FileData[m_IndexMapping[r]].bIsDir, bRefIsDir) && (r>anf)) r--;
		}
		else
		{
			while (greater1(m_FileData[m_IndexMapping[l]].lName, ref, m_FileData[m_IndexMapping[l]].bIsDir, bRefIsDir) && (l<ende)) l++;
			while (lesser (m_FileData[m_IndexMapping[r]].lName, ref, m_FileData[m_IndexMapping[r]].bIsDir, bRefIsDir) && (r>anf)) r--;
		}
		if (l<=r)
		{
			int tmp=m_IndexMapping[l];
			m_IndexMapping[l]=m_IndexMapping[r];
			m_IndexMapping[r]=tmp;
			l++;
			r--;
		}
	} 
	while (l<=r);

	if (anf<r) quicksortbyname(direction, anf, r);
	if (l<ende) quicksortbyname(direction, l, ende);
}

void CFileListCtrl::quicksortbytype(const std::vector<CString> &array, const BOOL &direction, int anf, int ende)
{
	int l=anf;
	int r=ende;
	CString tmp;
	CString ref=array[m_IndexMapping[(l+r)/2]];
	CString refname=m_FileData[m_IndexMapping[(l+r)/2]].lName;
	BOOL bRefIsDir=m_FileData[m_IndexMapping[(l+r)/2]].bIsDir;
	do
	{
		if (direction)
		{
			while (lesserbytype (array[m_IndexMapping[l]], ref, m_FileData[m_IndexMapping[l]].bIsDir, bRefIsDir, m_FileData[m_IndexMapping[l]].lName, refname) && (l<ende)) l++;
			while (greaterbytype(array[m_IndexMapping[r]], ref, m_FileData[m_IndexMapping[r]].bIsDir, bRefIsDir, m_FileData[m_IndexMapping[r]].lName, refname) && (r>anf)) r--;
		}
		else
		{
			while (greaterbytype(array[m_IndexMapping[l]], ref, m_FileData[m_IndexMapping[l]].bIsDir, bRefIsDir, m_FileData[m_IndexMapping[l]].lName, refname) && (l<ende)) l++;
			while (lesserbytype (array[m_IndexMapping[r]], ref, m_FileData[m_IndexMapping[r]].bIsDir, bRefIsDir, m_FileData[m_IndexMapping[r]].lName, refname) && (r>anf)) r--;
		}
		if (l<=r)
		{
			int tmp=m_IndexMapping[l];
			m_IndexMapping[l]=m_IndexMapping[r];
			m_IndexMapping[r]=tmp;
			l++;
			r--;
		}
	} 
	while (l<=r);
	
	if (anf<r) quicksortbytype(array, direction, anf, r);
	if (l<ende) quicksortbytype(array, direction, l, ende);
}

void CFileListCtrl::quicksortbysize(const BOOL &direction, int anf, int ende)
{
	int l=anf;
	int r=ende;
	CString tmp;
	_int64 ref=m_FileData[m_IndexMapping[(l+r)/2]].nSize;
	CString refname=m_FileData[m_IndexMapping[(l+r)/2]].lName;
	BOOL bRefIsDir=m_FileData[m_IndexMapping[(l+r)/2]].bIsDir;
	do
    {
		if (direction)
		{
			while (lesserbysize (m_FileData[m_IndexMapping[l]].nSize, ref, m_FileData[m_IndexMapping[l]].bIsDir, bRefIsDir, m_FileData[m_IndexMapping[l]].lName, refname) && (l<ende)) l++;
			while (greaterbysize(m_FileData[m_IndexMapping[r]].nSize, ref, m_FileData[m_IndexMapping[r]].bIsDir, bRefIsDir, m_FileData[m_IndexMapping[r]].lName, refname) && (r>anf)) r--;
		}
		else
		{
			while (greaterbysize(m_FileData[m_IndexMapping[l]].nSize, ref, m_FileData[m_IndexMapping[l]].bIsDir, bRefIsDir, m_FileData[m_IndexMapping[l]].lName, refname) && (l<ende)) l++;
			while (lesserbysize (m_FileData[m_IndexMapping[r]].nSize, ref, m_FileData[m_IndexMapping[r]].bIsDir, bRefIsDir, m_FileData[m_IndexMapping[r]].lName, refname) && (r>anf)) r--;
		}
		if (l<=r)
		{
			int tmp=m_IndexMapping[l];
			m_IndexMapping[l]=m_IndexMapping[r];
			m_IndexMapping[r]=tmp;
			l++;
			r--;
		}
    } 
	while (l<=r);

  if (anf<r) quicksortbysize(direction, anf, r);
  if (l<ende) quicksortbysize(direction, l, ende);
}

void CFileListCtrl::quicksortbytime(const BOOL &direction, int anf, int ende)
{
	int l=anf;
	int r=ende;
	CString tmp;
	CTime ref=m_FileData[m_IndexMapping[(l+r)/2]].Time;
	CString refname=m_FileData[m_IndexMapping[(l+r)/2]].lName;
	BOOL bRefIsDir=m_FileData[m_IndexMapping[(l+r)/2]].bIsDir;
	do
    {
		if (direction)
		{
			while (lesserbytime (m_FileData[m_IndexMapping[l]].Time, ref, m_FileData[m_IndexMapping[l]].bIsDir, bRefIsDir, m_FileData[m_IndexMapping[l]].lName, refname) && (l<ende)) l++;
			while (greaterbytime(m_FileData[m_IndexMapping[r]].Time, ref, m_FileData[m_IndexMapping[r]].bIsDir, bRefIsDir, m_FileData[m_IndexMapping[r]].lName, refname) && (r>anf)) r--;
		}
		else
		{
			while (greaterbytime(m_FileData[m_IndexMapping[l]].Time, ref, m_FileData[m_IndexMapping[l]].bIsDir, bRefIsDir, m_FileData[m_IndexMapping[l]].lName, refname) && (l<ende)) l++;
			while (lesserbytime (m_FileData[m_IndexMapping[r]].Time, ref, m_FileData[m_IndexMapping[r]].bIsDir, bRefIsDir, m_FileData[m_IndexMapping[r]].lName, refname) && (r>anf)) r--;
		}
		if (l<=r)
		{
			int tmp=m_IndexMapping[l];
			m_IndexMapping[l]=m_IndexMapping[r];
			m_IndexMapping[r]=tmp;
			l++;
			r--;
		}
    } 
	while (l<=r);

  if (anf<r) quicksortbytime(direction, anf, r);
  if (l<ende) quicksortbytime(direction, l, ende);
}

void CFileListCtrl::SortList(int item, int direction)
{
	UINT nID[4]={IDS_HEADER_FILENAME,IDS_HEADER_FILESIZE,IDS_HEADER_FILETYPE,IDS_HEADER_LASTMODIFIED};
	if(item > 3) return;
	
	if (!direction)
	{
		if (item!=m_sortcolumn)
			m_sortdir=1;
		else
		{
			m_sortdir=(++m_sortdir%3);
			if (!m_sortdir)
				m_sortdir++;
		}
			
	}
	else if (direction != -1)
		m_sortdir = direction;

	if (item == -1)
		item = m_sortcolumn;

	CHeaderCtrl *header=GetHeaderCtrl();
	if (header)
	{
		CString headertext;
		headertext.LoadString(nID[m_pColumns[m_sortcolumn]]);
		HDITEM *hdi=new HDITEM;
		hdi->pszText=headertext.GetBuffer(0);
		hdi->cchTextMax=0;
		hdi->mask= HDI_TEXT | HDI_FORMAT;
		hdi->fmt=((m_pColumns[m_sortcolumn]!=1)?HDF_LEFT:HDF_RIGHT) | HDF_STRING;
		hdi->mask= HDI_TEXT | HDI_IMAGE | HDI_FORMAT;
		hdi->iImage=0; // My ascending image list index
		header->SetItem( m_sortcolumn, hdi );
	
		headertext.ReleaseBuffer();
		headertext.LoadString(nID[m_pColumns[item]]);
		hdi->pszText=headertext.GetBuffer(0);
		hdi->mask= HDI_TEXT | HDI_IMAGE | HDI_FORMAT;
		hdi->iImage= m_sortdir; // My ascending image list index
		hdi->fmt=((m_pColumns[item]!=1)?HDF_LEFT:HDF_RIGHT) | HDF_IMAGE | HDF_STRING | HDF_BITMAP_ON_RIGHT;
		header->SetItem( item, hdi );
		delete hdi;
		headertext.ReleaseBuffer();
	}
	m_sortcolumn = item;
	int count = GetItemCount();
	if (count <= 1 ||
	  GetItemCount() != m_IndexMapping.size() ||
	  GetItemCount() != m_FileData.size())
		return;
	
	std::set<int> SelectedItemsList;
	int i;
	for (i = 1; i < count; i++)
	{
		if (GetItemState( i, LVIS_SELECTED))
		{
			SelectedItemsList.insert(m_IndexMapping[i]);
			SetItemState( i, 0, LVIS_SELECTED);
		}
	}

	if (!m_pColumns[item])
	{ //Sort by filename
		BOOL dir=m_sortdir==1;
		quicksortbyname(dir, 1, count - 1);
	}
	if (m_pColumns[item]==2)
	{ //Sort by filetype
	  //Since this is a column that is filled while displaying,
	  //we have to load the filetypes for every file

		std::vector<CString> array;
		array.resize(count + 1);
		array[0] = _T("");
		for (int i = 1; i < count; i++)
		{
			array[i] = GetType(m_FileData[i].lName, m_FileData[i].bIsDir);
			array[i].MakeLower();
		}
		BOOL dir = (m_sortdir == 1);
		quicksortbytype(array, dir, 1, count - 1);
	}
	else if (m_pColumns[m_sortcolumn] == 1)
	{
		BOOL dir = (m_sortdir == 1);
		quicksortbysize(dir, 1, count - 1);
	}
	else if (m_pColumns[m_sortcolumn] == 3)
	{
		BOOL dir = m_sortdir==1;
		quicksortbytime(dir, 1, count - 1);
	}

	for (i = 1; i < count; i++)
	{
		if (SelectedItemsList.empty())
			break;
		int nIndex = m_IndexMapping[i];
		std::set<int>::iterator iter = SelectedItemsList.find(nIndex);
		if (iter != SelectedItemsList.end())
		{
			SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
			SelectedItemsList.erase(iter);
		}
	}

	RedrawItems(0, count);
}

void CFileListCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	/*
	if (nChar==46)
		OnLocalcontextDelete();
	else */if (nChar==VK_F2)
	{
		// To Use F2 as hot Key to get EditCtrl on the ListView it must have 
		// the Style LVS_EDITLABELS
		ASSERT( GetStyle() & LVS_EDITLABELS );
		// don't do an Edit Label when the multiple Items are selected
		if( GetSelectedCount( ) == 1 )
		{
			UINT nListSelectedItem = GetSelectedItem();
			VERIFY( EditLabel( nListSelectedItem ) != NULL );
		}
		else
			CListCtrl::OnKeyDown( nChar, nRepCnt, nFlags );
	}
	else if (nChar==VK_BACK)
	{
		if (GetItemCount() && m_IndexMapping.size())
		{
			int nIndex=m_IndexMapping[0];
			if (m_FileData[nIndex].bIsDir)
			{
				if (m_Fullpath!="")
				{
					CString newpath;
					newpath=m_Fullpath;
					newpath.TrimRight('\\');
					int pos=newpath.ReverseFind('\\');
					newpath=newpath.Left(pos+1);
					//m_pOwner->SetLocalFolder(newpath);
					//m_pOwner->SetLocalFolderOut(newpath);
				}
			}
		}
	}
	CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

//DEL void CFileListCtrl::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
//DEL {
//DEL 	//Start of a Drag&Drop operation
//DEL 	*pResult = 0;
//DEL 
//DEL 	if (m_Fullpath == _T("") || m_Fullpath == _T("\\"))
//DEL 		return;
//DEL 
//DEL 	POSITION selpos=GetFirstSelectedItemPosition();
//DEL 	while (selpos)
//DEL 	{
//DEL 		int nItem = GetNextSelectedItem(selpos);
//DEL 		if (!nItem)
//DEL 			return;
//DEL 	}
//DEL 	EnsureVisible(((LPNMLISTVIEW)pNMHDR)->iItem, FALSE);
//DEL 	
//DEL 	//Let the main window handle the rest
//DEL 	//CMainFrame *pMainFrame=DYNAMIC_DOWNCAST(CMainFrame,GetParentFrame());
//DEL 	//pMainFrame->OnBegindrag(this, ((LPNMLISTVIEW)pNMHDR)->ptAction);
//DEL }

void CFileListCtrl::OnDropFiles(HDROP hDropInfo) 
{
	if (!GetItemCount())
	{
		DragFinish(hDropInfo);
		return;
	}
	int dropcount=DragQueryFile(hDropInfo,0xFFFFFFFF,0,0);
	LPTSTR fullnames=0;
	int fulllen=0;
	for (int i=0;i<dropcount;i++)
	{
		int len=DragQueryFile(hDropInfo,i,0,0)+1;
		fullnames=(LPTSTR)realloc(fullnames,(fulllen+len+1)*sizeof(TCHAR));
		DragQueryFile(hDropInfo,i,&fullnames[fulllen],len);
		fulllen+=len;
	}
	fullnames[fulllen]=0;
	CString path=m_Fullpath;

	SHFILEOPSTRUCT opstruct;
	opstruct.hwnd=m_hWnd;
	opstruct.wFunc=FO_COPY;
	opstruct.pFrom=fullnames;
	opstruct.pTo=path;
	opstruct.fFlags=FOF_ALLOWUNDO;
	SHFileOperation(&opstruct);
	DragFinish(hDropInfo);
	free(fullnames);
	//CMainFrame *pMainFrame=DYNAMIC_DOWNCAST(CMainFrame,GetParentFrame());
	//pMainFrame->RefreshViews(1);
}

// this Function Returns the first Selected Item in the List
UINT CFileListCtrl::GetSelectedItem()
{
	// this Function Valid Only when a Single Item is Selected
	ASSERT( GetSelectedCount( ) == 1 );
	UINT nNoOfItems = GetItemCount( );
	UINT nListItem;
	for (nListItem = 0; nListItem < nNoOfItems; nListItem++)
		if (GetItemState( nListItem, LVIS_SELECTED )  )
			break;
	ASSERT(nListItem < nNoOfItems);
	return nListItem;
}

#define VK_A		65
BOOL CFileListCtrl::PreTranslateMessage(MSG* pMsg) 
{
	// If edit control is visible in tree view control, sending a
	// WM_KEYDOWN message to the edit control will dismiss the edit
	// control.  When ENTER key was sent to the edit control, the parent
	// window of the tree view control is responsible for updating the
	// item's label in TVN_ENDLABELEDIT notification code.
	if ( pMsg->message == WM_KEYDOWN )
	{
		CEdit* edit = GetEditControl();
		if (edit)
		{
			if( GetKeyState( VK_CONTROL )&128 && pMsg->wParam == VK_A )
			{
				edit->SetSel(0, -1);
				return TRUE;
			}
			else if( pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_CONTROL || pMsg->wParam == VK_INSERT || pMsg->wParam == VK_SHIFT )
			{
				edit->SendMessage(WM_KEYDOWN, pMsg->wParam, pMsg->lParam);
				return TRUE;
			}
		}
		else
		{
			if( GetKeyState( VK_CONTROL )&128 && pMsg->wParam == VK_A )
			{
				m_bUpdating = TRUE;
				if (GetItemCount())
					SetItemState(0, GetItemCount()==1 || m_Fullpath==""?LVIS_SELECTED:0,LVIS_SELECTED);
				for (int i=1;i<GetItemCount();i++)
					SetItemState(i, LVIS_SELECTED,LVIS_SELECTED);
			
				m_bUpdating = FALSE;
				//UpdateStatusBar();
				return TRUE;
			}
			//Handle the enter key
			/*
			else if (pMsg->wParam==VK_RETURN)
			{
				CMainFrame *pMainFrame = DYNAMIC_DOWNCAST(CMainFrame, GetParentFrame());
				POSITION selpos=GetFirstSelectedItemPosition();
				BOOL bOpen=TRUE;
				int openindex=-1;
				int openitem=-1;
				if (selpos)
				{
					int nItem = openitem = GetNextSelectedItem(selpos);
					int index=openindex=m_IndexMapping[nItem];
					if (m_FileData[index].bIsDir)
					{
					
					}
					else
					{
						bOpen=FALSE;
						pMainFrame->AddQueueItem(FALSE,GetItemText(nItem,0),"",m_Fullpath,CServerPath(),TRUE);
						openindex=-1;
						openitem=-1;
					}
				}
				else
					return TRUE;
				while (selpos)
				{
					if (!openindex)
						return TRUE;
					bOpen=FALSE;
					int nItem = GetNextSelectedItem(selpos);
					if(!nItem || m_Fullpath=="")
						return TRUE;
					int index=m_IndexMapping[nItem];
					if (m_FileData[index].bIsDir)
						pMainFrame->UploadDir(m_Fullpath+m_FileData[index].Name+"\\*.*", m_FileData[index].Name+"\\", TRUE, m_transferuser, m_transferpass);
					else
						pMainFrame->AddQueueItem(FALSE, m_FileData[index].Name, "", m_Fullpath, CServerPath(), TRUE, m_transferuser, m_transferpass);
				}
				if (bOpen)
				{
					CString newpath;
					if (!openitem && m_Fullpath!="")
					{
						m_Fullpath.TrimRight('\\');
						int pos=m_Fullpath.ReverseFind('\\');
						if (pos!=-1)
							m_Fullpath=m_Fullpath.Left(pos+1);
						else
							m_Fullpath="";
						newpath=m_Fullpath;
					}
					else
						newpath=m_Fullpath+m_FileData[openindex].Name+"\\";
					m_pOwner->SetLocalFolder(newpath);
					m_pOwner->SetLocalFolderOut(newpath);
				}
				else
				{
					ASSERT(m_Fullpath!="");
					if (openindex>=0)
						pMainFrame->UploadDir(m_Fullpath+m_FileData[openindex].Name+"\\*.*", m_FileData[openindex].Name+"\\", TRUE, m_transferuser, m_transferpass);
					pMainFrame->TransferQueue(2);					
				}

				return TRUE;
			}
			*/
		}
	}
	else if ( pMsg->message == WM_CHAR )
	{
		CEdit* edit = GetEditControl();
		if (edit)
		{
			if (pMsg->wParam=='/' ||
				pMsg->wParam=='\\' ||
				pMsg->wParam==':' ||
				pMsg->wParam=='*' ||
				pMsg->wParam=='?' ||
				pMsg->wParam=='"' ||
				pMsg->wParam=='<' ||
				pMsg->wParam=='>' ||
				pMsg->wParam=='|')
			{
				MessageBeep(0xFFFFFFFF);
				return TRUE;
			}
		}
	}
	return CListCtrl::PreTranslateMessage(pMsg);
}

void CFileListCtrl::ReloadHeader()
{
	ReloadHeaderItem(0,IDS_HEADER_FILENAME);
	int i=1;
	if (!(m_nHideColumns&1))
	{
		ReloadHeaderItem(i,IDS_HEADER_FILESIZE);
		i++;
	}
	if (!(m_nHideColumns&2))
	{
		ReloadHeaderItem(i,IDS_HEADER_FILETYPE);
		i++;
	}
	if (!(m_nHideColumns&4))
		ReloadHeaderItem(i,IDS_HEADER_LASTMODIFIED);
}

void CFileListCtrl::ReloadHeaderItem(int nIndex, UINT nID)
{
	CHeaderCtrl *header=GetHeaderCtrl();
	TCHAR text[100];
	HDITEM item;
	memset(&item,0,sizeof(HDITEM));
	item.cchTextMax=100;
	item.mask=HDI_TEXT;
	item.pszText=text;
	header->GetItem(nIndex,&item);
	CString str;
	str.LoadString(nID);
	_tcscpy(text,str);
	header->SetItem(nIndex,&item);
}

struct IsInDisplayFileList : public binary_function<CString, CString, bool>
{
//          typedef T first_argument_type;
//          typedef T second_argument_type;
//          typedef T result_type;

	bool operator ()( const CString& a, const CString& b ) const
	{
		string str = b.operator const char*();
		string::size_type at = str.find_last_of(".");
		if((0 != at) && (string::npos != at))
		{
			str.assign(str.substr(0 , at));
		}
		
		return (a == CString(str.c_str()));
	}
};

void CFileListCtrl::SetFolder(CString folder)
{
	if (m_bUpdating)
	{
		m_NewDir = folder;
		return;
	}
	else if (GetEditControl() && folder == m_Fullpath)
	{
		m_NewDir = folder;
		return;
	}
	else
		m_NewDir = _T("");

	m_NewDir = folder;
	
	BOOL bDidHaveFocus = GetFocus() == this;
	
	folder.TrimRight(_T("\\")); 
	folder += _T("\\");

	if (folder == m_Fullpath || (folder == _T("\\") && m_Fullpath == _T("")))
	{
		for (int i = 1; i < GetItemCount(); i++)
		{
			if (GetItemState( i, LVIS_SELECTED))
			{
				m_selectionBuffer.insert(m_FileData[m_IndexMapping[i]].lName);
				SetItemState(i, 0, LVIS_SELECTED);
			}
		}
	}
	else	m_selectionBuffer.clear();

	ClearFileData();
	m_IndexMapping.clear();
	m_Fullpath = folder;
	if (m_Fullpath == _T("\\"))
	{
		m_Fullpath="";
		DisplayDrives();
		return;
	}
	m_listOnly = false;
	m_bUpdating=TRUE;

	SetRedraw(FALSE);
	EnableWindow(FALSE);
	
	
	CString strPathFiles = m_Fullpath;
	
	if ( strPathFiles.Right(1) != "\\" )
		strPathFiles += "\\";
	strPathFiles += m_rFileFilter;

	WIN32_FIND_DATA find;
	HANDLE hFind = FindFirstFile( strPathFiles, &find);

	CString folder2=m_Fullpath;
	folder2.TrimRight('\\');
	
	int pos=folder2.ReverseFind('\\');
	folder2=folder2.Left(pos+1);

	t_FileData FileData;
	FileData.Name="..";
	FileData.lName="..";
	FileData.bIsDir=TRUE;
	m_IndexMapping.push_back(0);
	FileData.nSize=-1;
	FileData.hasTime = false;
	m_FileData.push_back(FileData);
	
	int nItemCount = 1;
			
	while (hFind != INVALID_HANDLE_VALUE)
	{
		if (!(find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && (!_tcscmp(find.cFileName, _T("..")) || !_tcscmp(find.cFileName, _T(".")))))
		{
			t_FileData FileData;
			FileData.Name=find.cFileName;
			FileData.lName=find.cFileName;
			FileData.lName.MakeLower();
			
			TRY
			{
				FileData.Time = find.ftLastWriteTime;
				FileData.hasTime = true;
			}
			CATCH_ALL(e)
			{
				FileData.hasTime = false;
			}
			END_CATCH_ALL;
		
			if( find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if(m_bDisplayFolder)
				{
					FileData.bIsDir=TRUE;
					FileData.nSize=-1;

					m_FileData.push_back(FileData);
					m_IndexMapping.push_back(nItemCount);
					
					nItemCount++;
				}
			}
			else
			{
				FileData.bIsDir=FALSE;
				FileData.nSize=(_int64)find.nFileSizeLow + ((_int64)find.nFileSizeHigh<<32);
				
				//////////////////////////////////////////////////////////////////////////
				m_FileData.push_back(FileData);
				m_IndexMapping.push_back(nItemCount);
				
				nItemCount++;
			}
		}
//#define SPEEDTEST 20000
#ifndef SPEEDTEST
		if (nItemCount==100)
		{
			VERIFY(PostMessage(WM_APP + 1, bDidHaveFocus, (LPARAM)hFind));
			return;
		}
#endif //SPEEDTEST
		if (!FindNextFile(hFind, &find))
		{
			FindClose(hFind);
			hFind=0;
#ifdef SPEEDTEST
			//Speed test code with SPEEDTEST files
			if (nItemCount > 1 && nItemCount<SPEEDTEST) 
				hFind = FindFirstFile( strPathFiles, &find);
			else
				break;
#else
			break;
#endif //SPEEDTEST
		}
	}

	m_bUpdating = FALSE;

	SetItemCount(nItemCount);
	
	SortList(m_sortcolumn , m_sortdir);

	for (int i = 1; i < GetItemCount(); i++)
	{
		if (m_selectionBuffer.empty())
			break;
		const CString& name = m_FileData[m_IndexMapping[i]].lName;
		std::set<CString>::iterator iter = m_selectionBuffer.find(name);
		if (iter != m_selectionBuffer.end())
		{
			SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
			m_selectionBuffer.erase(iter);
		}
	}

	EnableWindow(TRUE);
	
	SetRedraw( TRUE );

	if (bDidHaveFocus)
		SetFocus();

//	UpdateStatusBar();
}

void CFileListCtrl::OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM* pItem= &(pDispInfo)->item;

	if (static_cast<int>(m_IndexMapping.size())<=pItem->iItem)
		return;
	if (static_cast<int>(m_FileData.size())<=m_IndexMapping[pItem->iItem])
		return;
	
	if (pItem->mask & LVIF_IMAGE && !pItem->iSubItem)
	{
		if (m_FileData[m_IndexMapping[pItem->iItem]].iIcon!=-1)
			pItem->iImage=m_FileData[m_IndexMapping[pItem->iItem]].iIcon;
		else
		{
			CString path;
			if (m_FileData[m_IndexMapping[pItem->iItem]].lName=="..")
				path="alkjhgfdfghjjhgfdghuztxvbhzt";
			else
			{
				path=m_Fullpath+m_FileData[m_IndexMapping[pItem->iItem]].Name;
				if (m_Fullpath=="")
					path+="\\";			
			}
			
			int iIcon=-1;
			SHFILEINFO shFinfo;
			memset(&shFinfo,0,sizeof(SHFILEINFO));
			if (SHGetFileInfo( path,
				m_FileData[m_IndexMapping[pItem->iItem]].bIsDir?FILE_ATTRIBUTE_DIRECTORY:FILE_ATTRIBUTE_NORMAL,
					&shFinfo,
					sizeof( SHFILEINFO ),
					SHGFI_ICON | ((m_FileData[m_IndexMapping[pItem->iItem]].lName=="..")?SHGFI_USEFILEATTRIBUTES:0) ) )
			{
				iIcon = shFinfo.iIcon;
				// we only need the index from the system image ctrl
				DestroyIcon( shFinfo.hIcon );
				m_FileData[m_IndexMapping[pItem->iItem]].iIcon=iIcon;
			}
			pItem->iImage=iIcon;
		}
	}
	if (pItem->mask & LVIF_TEXT)
	{
		if (!pItem->pszText)
			return;
		if (!m_pColumns[pItem->iSubItem])
		{
			_tcsncpy(pItem->pszText, m_FileData[m_IndexMapping[pItem->iItem]].Name, pItem->cchTextMax);
		}
		else if (m_pColumns[pItem->iSubItem]==2 && (pItem->iItem||(m_Fullpath=="")))
		{
			CString type=GetType(m_FileData[m_IndexMapping[pItem->iItem]].lName, m_FileData[m_IndexMapping[pItem->iItem]].bIsDir);
			_tcsncpy(pItem->pszText, type, pItem->cchTextMax);
		}
		else if (m_pColumns[pItem->iSubItem]==1)
		{
			__int64 size=m_FileData[m_IndexMapping[pItem->iItem]].nSize;
			if (size!=-1)
			{
				int nFormat = COptions::GetOptionVal(OPTION_LOCALFILESIZEFORMAT);
				if (!nFormat)
					if (size<1024)
						nFormat=1;
					else if (size<(1024*1024))
						nFormat=2;
					else
						nFormat=3;
				
				CString tmp, sizestr;
				switch (nFormat)
				{
				case 1:
					sizestr.Format(_T("%I64d"), size);
					break;
				case 2:
					tmp.LoadString(IDS_SIZE_KBS);
					sizestr.Format(_T("%I64d %s"), (size + 1023) / 1024, tmp);
					break;
				case 3:
					tmp.LoadString(IDS_SIZE_MBS);
					sizestr.Format(_T("%I64d %s"), (size + 1024 * 1024 - 1) / 1024 / 1024, tmp);
					break;
				default:
					ASSERT(FALSE);
				}
				_tcsncpy(pItem->pszText, sizestr, pItem->cchTextMax);
			}
		}
		else if (m_pColumns[pItem->iSubItem]==3 && m_Fullpath!=_T("") && pItem->iItem && m_FileData[m_IndexMapping[pItem->iItem]].hasTime)
		{
			CTime time=m_FileData[m_IndexMapping[pItem->iItem]].Time;
			SYSTEMTIME timeDest;
			time.GetAsSystemTime( timeDest ) ;
		
			TCHAR text[512];
			if (!GetDateFormat(
				LOCALE_USER_DEFAULT,               // locale for which date is to be formatted
				DATE_SHORTDATE,             // flags specifying function options
				&timeDest,  // date to be formatted
				0,          // date format string
				text,          // buffer for storing formatted string
				512                // size of buffer
				))
				return;
			CString text2=text;
			
			if (!GetTimeFormat(
				LOCALE_USER_DEFAULT,               // locale for which date is to be formatted
				TIME_NOSECONDS|TIME_FORCE24HOURFORMAT,             // flags specifying function options
				&timeDest,  // date to be formatted
				0,          // date format string
				text,          // buffer for storing formatted string
				512                // size of buffer
				))
				return;
			text2+=" ";
			text2+=text;
			_tcsncpy(pItem->pszText, text2, pItem->cchTextMax);
		}
		pItem->pszText[pItem->cchTextMax - 1] = 0;
	}
	*pResult = 0;
}

void CFileListCtrl::DisplayDrives()
{
	m_listOnly = true;
	m_Fullpath = _T("");
	TCHAR  szDrives[128];
	LPTSTR pDrive;

	GetLogicalDriveStrings( sizeof(szDrives), szDrives );
	
	pDrive = szDrives;
	int count=0;
	while( *pDrive )
	{
		CString path=pDrive;
		t_FileData FileData;
		path.TrimRight('\\');
		FileData.Name=path;
		path.MakeLower();
		FileData.lName=path;
		FileData.nSize=-1;
		FileData.bIsDir=TRUE;
		m_FileData.push_back(FileData);
		m_IndexMapping.push_back(count);
		pDrive += _tcslen(pDrive) + 1;
		count++;
	}
	SetItemCount(count);
	SortList(m_sortcolumn,m_sortdir);

	for (int i = 1; i < GetItemCount(); i++)
	{
		if (m_selectionBuffer.empty())
			break;
		const CString& name = m_FileData[m_IndexMapping[i]].lName;
		std::set<CString>::iterator iter = m_selectionBuffer.find(name);
		if (iter != m_selectionBuffer.end())
		{
			SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
			m_selectionBuffer.erase(iter);
		}
	}

//	UpdateStatusBar();
}

BOOL CFileListCtrl::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult) 
{
	if (message==WM_NOTIFY)
	{
		LPNMHDR  lpnmh = (LPNMHDR) lParam;
		
		if (lpnmh->code==LVN_ODFINDITEM)
		{
			NMLVFINDITEM* pFindItem = (NMLVFINDITEM*)lParam;
			LVFINDINFO pFindInfo=pFindItem->lvfi;
		
			CString part=pFindInfo.psz;
			part.MakeLower();
			int start=pFindItem->iStart;
			
			for (int i=start;i<(GetItemCount()+start);i++)
			{
				CString fn=m_FileData[m_IndexMapping[i%GetItemCount()]].Name.Left(part.GetLength());
				fn.MakeLower();
				if (fn==part)
				{
					*pLResult=i%GetItemCount();
					return TRUE;
				}
			}
			*pLResult=-1;
			return TRUE;
		}
	}
	return CListCtrl::OnChildNotify(message, wParam, lParam, pLResult);
}

void CFileListCtrl::SetListStyle(int nStyle)
{
	if (!nStyle)
		nStyle = LVS_REPORT;
	else if (nStyle == 1)
		nStyle = LVS_LIST;
	else if (nStyle == 2)
		nStyle = LVS_ICON;
	else if (nStyle == 3)
		nStyle = LVS_SMALLICON;
	
	if (m_nStyle == nStyle)
		return;

	m_nStyle = nStyle;

	int remove = ~m_nStyle & (LVS_REPORT | LVS_ICON | LVS_SMALLICON | LVS_LIST);
	ModifyStyle(remove, m_nStyle, SWP_NOZORDER);
	
	GetSysImgList();
	CHeaderCtrl *header=GetHeaderCtrl( );
	if (header)
		header->SetImageList(&m_SortImg);
	Arrange(LVA_DEFAULT);
	if (m_nStyle!=LVS_REPORT)
		SortList(0,1);
	SetItemCount(GetItemCount());
}

void CFileListCtrl::UpdateColumns(int nHideColumns)
{
	CString str;
	if (nHideColumns&1 && !(m_nHideColumns&1))
	{
		m_pColumns[1]=m_pColumns[2];
		m_pColumns[2]=m_pColumns[3];
		DeleteColumn(1);
		if (m_sortcolumn==1)
			SortList(0,1);
		if (m_sortcolumn>1)
			m_sortcolumn--;
	}
	if (m_nHideColumns&1 && !(nHideColumns&1))
	{
		m_pColumns[3]=m_pColumns[2];
		m_pColumns[2]=m_pColumns[1];
		m_pColumns[1]=1;
		str.LoadString(IDS_HEADER_FILESIZE);
		InsertColumn(1,str,LVCFMT_RIGHT,66);
		if (m_sortcolumn>=1)
			m_sortcolumn++;
	}
	if (nHideColumns&2 && !(m_nHideColumns&2))
	{
		for (int i=1;i<3;i++)
		{
			if (m_pColumns[i]==2)
			{
				int j=i;
				while (j<3)
				{
					m_pColumns[j]=m_pColumns[j+1];
					j++;
				}
				DeleteColumn(i);		
				if (m_sortcolumn==i)
					SortList(0,1);
				if (m_sortcolumn>i)
					m_sortcolumn--;
				break;
			}
		}
	}
	if (m_nHideColumns&2 && !(nHideColumns&2))
	{
		for (int i=1;i<3;i++)
		{
			if (m_pColumns[i]==3)
			{
				int j=3;
				while (j!=i)
				{
					m_pColumns[j]=m_pColumns[j-1];
					j--;
				}
				m_pColumns[i]=2;
				str.LoadString(IDS_HEADER_FILETYPE);
				InsertColumn(i,str,LVCFMT_LEFT,100);
				if (m_sortcolumn>=i)
					m_sortcolumn++;
				break;
			}
		}
	}
	if (nHideColumns&4 && !(m_nHideColumns&4))
	{
		for (int i=1;i<4;i++)
		{
			if (m_pColumns[i]==3)
			{
				m_pColumns[i]=3;
				DeleteColumn(i);		
				if (m_sortcolumn==i)
					SortList(0,1);
				if (m_sortcolumn>i)
					m_sortcolumn--;
				break;
			}
		}
	}
	if (m_nHideColumns&4 && !(nHideColumns&4))
	{
		for (int i=1;i<4;i++)
		{
			if (m_pColumns[i]==3)
			{
				m_pColumns[i]=3;
				str.LoadString(IDS_HEADER_LASTMODIFIED);
				InsertColumn(i,str,LVCFMT_LEFT,99);
				if (m_sortcolumn>=i)
					m_sortcolumn++;
				break;
			}
		}
	}
	m_nHideColumns=nHideColumns;
}

void CFileListCtrl::SaveColumnSizes()
{
	int nSize[4];
	nSize[0]=GetColumnWidth(0);
	int index=1;
	nSize[1]=60;
	nSize[2]=100;
	nSize[3]=99;

	if (!(m_nHideColumns&1))
	{
		nSize[1]=GetColumnWidth(index);
		index++;
	}
	
	if (!(m_nHideColumns&2))
	{
		nSize[2]=GetColumnWidth(index);
		index++;
	}
	
	if (!(m_nHideColumns&4))
	{
		nSize[3]=GetColumnWidth(index);
		index++;
	}

	CString str;
	str.Format(_T("%d %d %d %d"), nSize[0], nSize[1], nSize[2], nSize[3]);
	COptions::SetOption(OPTION_LOCALCOLUMNWIDTHS, str);
}

void CFileListCtrl::OnDestroy() 
{
	if (COptions::GetOptionVal(OPTION_REMEMBERLOCALCOLUMNWIDTHS))
		SaveColumnSizes();

	if (COptions::GetOptionVal(OPTION_LOCALCOLUMNSORT))
	{
		int nSort = 1;
		nSort |= m_pColumns[m_sortcolumn] << 1;
		nSort |= m_sortdir << 4;
		COptions::SetOption(OPTION_LOCALCOLUMNSORT, nSort);
	}
	
	CListCtrl::OnDestroy();	
}

LRESULT CFileListCtrl::OnUpdateContinue(WPARAM wParam, LPARAM lParam)
{
	if (!m_bUpdating)
		return 0;

	if (m_NewDir != _T(""))
	{
		m_bUpdating = false;
		SetFolder(m_NewDir);
		return 0;
	}
	
	ASSERT(lParam);

	int nOldItemCount = m_FileData.size();
	int nItemCount = nOldItemCount;
	HANDLE hFind = (HANDLE)lParam;

	WIN32_FIND_DATA find;
	if (!FindNextFile(hFind, &find))
	{
		FindClose(hFind);
		hFind=0;
	}
	
	while ( hFind )
	{
		if (!(find.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY && (!_tcscmp(find.cFileName, _T("..")) || !_tcscmp(find.cFileName, _T(".")))))
		{
			t_FileData FileData;
			FileData.Name=find.cFileName;
			FileData.lName=find.cFileName;
			FileData.lName.MakeLower();

			TRY
			{
				FileData.Time = find.ftLastWriteTime;
				FileData.hasTime = true;
			}
			CATCH_ALL(e)
			{
				FileData.hasTime = false;
			}
			END_CATCH_ALL;

			if ( find.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)
			{
				FileData.bIsDir=TRUE;
				FileData.nSize=-1;
			}
			else
			{
				FileData.bIsDir=FALSE;
				FileData.nSize=(_int64)find.nFileSizeLow + ((_int64)find.nFileSizeHigh<<32);
			}
			
			m_FileData.push_back(FileData);
			m_IndexMapping.push_back(nItemCount);
			
			nItemCount++;
		}
		if ((nItemCount-nOldItemCount)==100)
		{
			VERIFY(PostMessage(WM_APP + 1, wParam, (LPARAM)hFind));
			return 0;
		}		
		if (!FindNextFile(hFind, &find))
		{
			FindClose(hFind);
			hFind=0;
		}
	
	}

	m_bUpdating=FALSE;

	SetItemCount(nItemCount);
	
	SortList(m_sortcolumn,m_sortdir);

	EnableWindow(TRUE);
	
	SetRedraw( TRUE );

	if (wParam)
		SetFocus();

	for (int i = 1; i < GetItemCount(); i++)
	{
		if (m_selectionBuffer.empty())
			break;
		const CString& name = m_FileData[m_IndexMapping[i]].lName;
		std::set<CString>::iterator iter = m_selectionBuffer.find(name);
		if (iter != m_selectionBuffer.end())
		{
			SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
			m_selectionBuffer.erase(iter);
		}
	}

//	UpdateStatusBar();

	return 0;
}

void CFileListCtrl::RefreshFile(CString file)
{
	// Refreshes a single file in a list

	// Do not refresh while updating
	if (m_bUpdating)
		return;

	int i;
	int pos = file.ReverseFind('\\');
	if ( pos==-1 || pos==(file.GetLength()-1) )
		return;

	WIN32_FIND_DATA find;
	HANDLE hFind;
	CString name;
	BOOL bDir=FALSE;
	if (file.Left(pos+1).CollateNoCase(m_Fullpath))
	{
		if (m_Fullpath.CollateNoCase(file.Left(m_Fullpath.GetLength())))
			return;
		
		file=file.Mid(m_Fullpath.GetLength());
		int pos=file.Find('\\');
		if ( pos==-1 || pos==(file.GetLength()-1) || !pos)
			return;
		file=file.Left(pos);
		
		hFind = FindFirstFile( m_Fullpath+file+"\\.", &find);
		
		name=file;
		name.MakeLower();
		
		for (i=0;i<static_cast<int>(m_FileData.size());i++)
		{
			if (name==m_FileData[i].lName)
				break;
		}

		bDir=TRUE;
	}
	else
	{
		name=file.Mid(pos+1);
		name.MakeLower();
	
		for (i=0;i<static_cast<int>(m_FileData.size());i++)
		{
			if (name==m_FileData[i].lName)
				break;
		}
		hFind = FindFirstFile( file, &find);	
	}
	
	unsigned int nIndex=i;
	if (!hFind || hFind==INVALID_HANDLE_VALUE)
	{
		//File does not exist
		if (nIndex!=m_FileData.size())
		{ //But file is still visible in list
			if (!bDir && m_FileData[nIndex].bIsDir)
			{
				FindClose(hFind);
				return;
			}
			if (bDir && !m_FileData[nIndex].bIsDir)
			{
				FindClose(hFind);
				return;
			}
			
			m_FileData.erase(m_FileData.begin()+nIndex);
			unsigned int j;
			for (j=0; j<static_cast<int>(m_IndexMapping.size()); j++)
			{
				if (m_IndexMapping[j]==nIndex)
					break;
			}
			ASSERT(j!=m_IndexMapping.size());
			m_IndexMapping.erase(m_IndexMapping.begin()+j);
			for (i=1;i<static_cast<int>(m_IndexMapping.size());i++)
			{
				if (m_IndexMapping[i]>nIndex)
					m_IndexMapping[i]--;
			}
			SetItemState( j, 0, LVIS_SELECTED);
			for (int nItem=j+1;nItem<GetItemCount();nItem++)
			{
				if (GetItemState( nItem, LVIS_SELECTED))
				{
					SetItemState( nItem, 0, LVIS_SELECTED);
					SetItemState( nItem-1, LVIS_SELECTED, LVIS_SELECTED);
				}
			}
			
			SetItemCount(GetItemCount()-1);
		}
	}
	else
	{
		if (!bDir)
		{
			if (find.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY || !_tcscmp(find.cFileName, _T(".")) || !_tcscmp(find.cFileName, _T("..")) )
			{
				FindClose(hFind);
				return;
			}
		}
		else if (!(find.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
		{
			FindClose(hFind);
			return;
		}

		t_FileData FileData;
		int count=GetItemCount();
		int datapos;
		BOOL bSelected=FALSE;
		if (nIndex!=m_FileData.size())
		{ //File is already visible in list
			if (!bDir && m_FileData[nIndex].bIsDir)
			{
				FindClose(hFind);	
				return;
			}
			else if (bDir && !m_FileData[nIndex].bIsDir)
			{
				FindClose(hFind);	
				return;
			}
			m_FileData[nIndex].Name=find.cFileName;
			m_FileData[nIndex].lName=find.cFileName;
			m_FileData[nIndex].lName.MakeLower();
			m_FileData[nIndex].nSize=bDir ? -1 : ((_int64)find.nFileSizeLow + ((_int64)find.nFileSizeHigh<<32));
			m_FileData[nIndex].Time=find.ftLastWriteTime;
			FileData=m_FileData[nIndex];
			unsigned int j;
			for (j=0; j<static_cast<int>(m_IndexMapping.size()); j++)
			{
				if (m_IndexMapping[j]==nIndex)
					break;
			}
			ASSERT(j!=m_IndexMapping.size());
			m_IndexMapping.erase(m_IndexMapping.begin()+j);
			if (GetItemState( j, LVIS_SELECTED))
				bSelected=TRUE;
			SetItemState( j, 0, LVIS_SELECTED);
			for (int nItem=j+1;nItem<GetItemCount();nItem++)
			{
				if (GetItemState( nItem, LVIS_SELECTED))
				{
					SetItemState( nItem, 0, LVIS_SELECTED);
					SetItemState( nItem-1, LVIS_SELECTED, LVIS_SELECTED);
				}
			}
			datapos=nIndex;
			count--;
		}
		else
		{
			FileData.Name=find.cFileName;
			FileData.lName=find.cFileName;
			FileData.lName.MakeLower();
			FileData.bIsDir=bDir;
			FileData.nSize=bDir ? -1 : ((_int64)find.nFileSizeLow + ((_int64)find.nFileSizeHigh<<32));
			FileData.Time=find.ftLastWriteTime;
			m_FileData.push_back(FileData);
			datapos=count;
			SetItemCount(GetItemCount()+1);
		}
		CString filetype;
		if (m_pColumns[m_sortcolumn]==2)
		{
			filetype=GetType(FileData.lName, FileData.bIsDir);
			filetype.MakeLower();
		}
		
		if (count>1)
		{ //Binary search for optimal position to insert file sorted.
			int anf=1;
			int ende=count-1;
			
			int mitte;
			while(anf<=ende)
			{
				mitte=(anf+ende)/2;
				
				t_FileData CompareData=m_FileData[m_IndexMapping[mitte]];
				BOOL res;
				if (!m_pColumns[m_sortcolumn]) //Chose compare function based on column and direction
					if (m_sortdir==1)
						res=lesser(CompareData.lName, FileData.lName, CompareData.bIsDir, FileData.bIsDir);
					else
						res=greater1(CompareData.lName, FileData.lName, CompareData.bIsDir, FileData.bIsDir);
				else if (m_pColumns[m_sortcolumn]==1)
					if (m_sortdir==1)
						res=lesserbysize(CompareData.nSize, FileData.nSize, CompareData.bIsDir, FileData.bIsDir, CompareData.lName, FileData.lName);
					else
						res=greaterbysize(CompareData.nSize, FileData.nSize, CompareData.bIsDir, FileData.bIsDir, CompareData.lName, FileData.lName);
				else if (m_pColumns[m_sortcolumn]==2)
				{
					CString typecompare=GetType(CompareData.lName, CompareData.bIsDir);
					typecompare.MakeLower();
					if (m_sortdir==1)
						res=lesserbytype(typecompare, filetype, CompareData.bIsDir, FileData.bIsDir, CompareData.lName, FileData.lName);
					else
						res=greaterbytype(typecompare, filetype, CompareData.bIsDir, FileData.bIsDir, CompareData.lName, FileData.lName);
				}
				else if (m_pColumns[m_sortcolumn]==3)
					if (m_sortdir==1)
						res=lesserbytime(CompareData.Time, FileData.Time, CompareData.bIsDir, FileData.bIsDir, CompareData.lName, FileData.lName);
					else
						res=greaterbytime(CompareData.Time, FileData.Time, CompareData.bIsDir, FileData.bIsDir, CompareData.lName, FileData.lName);
						
				if (res)
					anf=mitte+1;
				else
				{
					ende=mitte-1;
					mitte--;
				}
			}
			m_IndexMapping.insert(m_IndexMapping.begin()+mitte+1, datapos);
			for (int nItem=GetItemCount()-1; nItem>(mitte+1); nItem--)
			{
				if (GetItemState( nItem-1, LVIS_SELECTED))
				{
					SetItemState( nItem-1, 0, LVIS_SELECTED);
					SetItemState( nItem, LVIS_SELECTED, LVIS_SELECTED);
				}
			}
			if (bSelected)
				SetItemState( mitte+1, LVIS_SELECTED, LVIS_SELECTED);
		}
		else
			m_IndexMapping.push_back(count);
	}
	if (hFind && hFind!=INVALID_HANDLE_VALUE)
		FindClose(hFind);
	RedrawItems(0, GetItemCount());
	//UpdateStatusBar();
}

static CString PathFindExtension(CString path)
{
	int pos=path.ReverseFind('.');
	if (pos==-1)
		return "";
	return path.Mid(pos);
}

CString CFileListCtrl::GetType(CString lName, BOOL bIsDir)
{
	CString type;
	std::map<CString, CString>::iterator typeIter=m_TypeCache.find(m_Fullpath+lName);
	if (typeIter==m_TypeCache.end())
	{
		CString str = PathFindExtension(lName);
		str.MakeLower();
		std::map<CString, CString>::iterator permTypeIter=m_permanentTypeCache.find(str);
		if (permTypeIter!=m_permanentTypeCache.end())
		{
			m_TypeCache[m_Fullpath+lName]=permTypeIter->second;
			type=permTypeIter->second;
		}
		else
		{
			SHFILEINFO shFinfo;
			CString path;
			path=m_Fullpath+lName;
			if (m_Fullpath=="")
				path+="\\";
			memset(&shFinfo,0,sizeof(SHFILEINFO));
			if (SHGetFileInfo(path,
				bIsDir?FILE_ATTRIBUTE_DIRECTORY:FILE_ATTRIBUTE_NORMAL,
				&shFinfo,
				sizeof( shFinfo ),
				SHGFI_TYPENAME))
			{
				type=shFinfo.szTypeName;
				if (type=="")
				{
					type=PathFindExtension(lName);
					if (type!="") 
						type=type.Mid(1);
					type.MakeUpper();
					if (type!="")
						type+="-file";
					else
						type="File";
				}
				else
				{
					CString str2=PathFindExtension(lName);
					if (!bIsDir && str2!="")
					{
						str2.MakeLower();
						m_permanentTypeCache[str2]=type;
					}
				}
			}
			else
			{
				type=PathFindExtension(lName);
				if (type!="") 
					type=type.Mid(1);
				type.MakeUpper();
				if (type!="")
					type+="-file";
				else
					type="File";
			}
			m_TypeCache[m_Fullpath+lName]=type;		
		}
	}
	else
		type=typeIter->second;
	return type;
}

void CFileListCtrl::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Code f? die Behandlungsroutine der Steuerelement-Benachrichtigung hier einf?en
	
	*pResult = 0;

	//UpdateStatusBar();
}

void CFileListCtrl::GetSortInfo(int &column, int &direction) const
{
	column = m_sortcolumn;
	direction = m_sortdir;
}

BOOL CFileListCtrl::DragPosition(CImageList *pImageList, CWnd* pDragWnd, CPoint point)
{
	ScreenToClient(&point);
	int nItem = HitTest(point);

	int nIndex = -1;

	if (m_listOnly)
		return FALSE;

	if (nItem != -1)
	{
		nIndex = m_IndexMapping[nItem];
		if (!m_FileData[nIndex].bIsDir)
			nIndex = -1;
	}
	if (pDragWnd == this)
	{
		POSITION selpos = GetFirstSelectedItemPosition();
		while (selpos)
		{
			int nSelectedItem = GetNextSelectedItem(selpos);
			if (nSelectedItem == nItem)
			{
				DragLeave(pImageList);
				return FALSE;
			}
		}
	}
	if (nIndex != -1)
	{
		if (nItem != m_nDragHilited)
		{
			pImageList->DragShowNolock(false);
			if (m_nDragHilited != -1)
			{
				SetItemState(m_nDragHilited, 0, LVIS_DROPHILITED);
				RedrawItems(m_nDragHilited, m_nDragHilited);
			}
			m_nDragHilited = nItem;
			SetItemState(nItem, LVIS_DROPHILITED, LVIS_DROPHILITED);
			RedrawItems(nItem, nIndex);
			UpdateWindow();
			pImageList->DragShowNolock(true);
		}
	}
	else
		DragLeave(pImageList);

	return TRUE;
}

void CFileListCtrl::DragLeave(CImageList *pImageList)
{
	if (m_nDragHilited != -1)
	{
		if (pImageList)
			pImageList->DragShowNolock(false);
		SetItemState(m_nDragHilited, 0, LVIS_DROPHILITED);
		RedrawItems(m_nDragHilited, m_nDragHilited);
		UpdateWindow();
		if (pImageList)
			pImageList->DragShowNolock(true);
		m_nDragHilited = -1;
	}
}

CString CFileListCtrl::GetDropTarget() const
{
	if (m_nDragHilited == -1)
		return _T("");

	CString subdir = m_FileData[m_IndexMapping[m_nDragHilited]].Name;
	if (subdir == _T(".."))
	{
		CString path = GetFolder();
		path.TrimRight('\\');
		int pos = path.ReverseFind('\\');
		if (pos == -1)
			return "";

		return path.Left(pos + 1);
	}

	return m_Fullpath + subdir;
}

void CFileListCtrl::SetDisplayFolder(bool disp)
{
	m_bDisplayFolder = disp;
}

void CFileListCtrl::SetFileFilter(const CString &rFileFilter)
{
	m_rFileFilter = rFileFilter;
}

void CFileListCtrl::SetColumnSize(const int &size)
{
	assert((size > 0) && "size error!!!");

	if(size > 0)
	{
		m_nColumnSize = size;
		if(NULL != m_pColumns) free((void*)m_pColumns);
		m_pColumns = (BOOL*)calloc(1 , sizeof(BOOL) * m_nColumnSize);
		for(int i = 0;i < m_nColumnSize;i++)
			m_pColumns[i] = i;
	}
}

void CFileListCtrl::ClearFileData()
{
	m_FileData.clear();
}
