// DirTreeCtrl.cpp: 
// 
// wrapped CTreeCtrl to select and or display folders and files (optional )
// 

#include "stdafx.h"
#include <shlobj.h>
#include "DirTreeCtrl.h"
#include "resource.h"
#include "mainfrm.h"

#include "IsoEditSettingDlg.h"
#include <vector>
using namespace std;

#if(1200 == _MSC_VER)
typedef enum 
{
    SHGFP_TYPE_CURRENT  = 0,   // current value for user, verify it exists
    SHGFP_TYPE_DEFAULT  = 1,   // default value, may not exist
} SHGFP_TYPE;
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDirTreeCtrl

CDirTreeCtrl::CDirTreeCtrl()
{
	m_dont_notify = false;
	m_hDragSource = NULL;
	m_hDragHilited = NULL;

#ifdef _UNICODE
	static const char* name = "SHGetFolderPathW";
#else
	static const char* name = "SHGetFolderPathA";
#endif

	m_hDll = LoadLibrary(_T("shell32.dll"));
	if (m_hDll)
	{
		m_pSHGetFolderPath = (HRESULT (WINAPI *)(HWND hwndOwner, int nFolder, HANDLE hToken, DWORD dwFlags, LPTSTR pszPath))GetProcAddress(m_hDll, name);
		if (!m_pSHGetFolderPath)
		{
			FreeLibrary(m_hDll);
			m_hDll = 0;
		}
	}
	if (!m_pSHGetFolderPath)
	{
		m_hDll = LoadLibrary(_T("SHFolder.dll"));
		if (m_hDll)
		{
			m_pSHGetFolderPath = (HRESULT (WINAPI *)(HWND hwndOwner, int nFolder, HANDLE hToken, DWORD dwFlags, LPTSTR pszPath))GetProcAddress(m_hDll, name);
			if (!m_pSHGetFolderPath)
			{
				FreeLibrary(m_hDll);
				m_hDll = 0;
			}
		}
	}
}

CDirTreeCtrl::~CDirTreeCtrl()
{
	if (m_hDll)
		FreeLibrary(m_hDll);
}


BEGIN_MESSAGE_MAP(CDirTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CDirTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, OnItemexpanded)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_WM_VSCROLL()
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen f? Nachrichten CDirTreeCtrl 

BOOL CDirTreeCtrl::DisplayTree()
{
	m_dont_notify = TRUE;
		
	DeleteAllItems();

	if (m_pSHGetFolderPath)
	{
		LPITEMIDLIST list;
		// Use `My Desktop' as root-item */
		SHGetSpecialFolderLocation(m_pOwner->m_hWnd, CSIDL_DESKTOP, &list);
		/* Add `My Desktop' to TreeView */
		HTREEITEM hParent = AddItem(
				GetRootItem(),
				(LPCTSTR)list,
				SHGFI_PIDL | FF_DISPLAYNAME,
				CSIDL_DESKTOPDIRECTORY | FF_SYSTEM | FF_NODRAG);
		CoTaskMemFree(list);

		/* Add `My Document' , `My Computer', etc, to `My Desktop' */
		if ( !DisplaySpecialFolders() )
		{
			m_dont_notify = FALSE;
			return FALSE;
		}

		/* Add other normal folders to `My Desktop' */
		TCHAR szPath[MAX_PATH];
		memset(szPath, 0, MAX_PATH);
		m_pSHGetFolderPath(NULL, CSIDL_DESKTOPDIRECTORY, 0, SHGFP_TYPE_CURRENT, szPath);
		DisplayPath( GetRootItem(), szPath );
	}
	else
	{
		LPITEMIDLIST list;
		SHGetSpecialFolderLocation(m_pOwner->m_hWnd, CSIDL_DRIVES, &list);
		SHFILEINFO shFinfo;

		SHGetFileInfo((LPCTSTR)list, 0, &shFinfo, sizeof(shFinfo), SHGFI_PIDL | SHGFI_ICON | SHGFI_SMALLICON);	
		DestroyIcon(shFinfo.hIcon);
		int iIcon = shFinfo.iIcon;
		SHGetFileInfo((LPCTSTR)list, 0, &shFinfo, sizeof(shFinfo), SHGFI_PIDL| SHGFI_ICON| SHGFI_SMALLICON | SHGFI_OPENICON| SHGFI_DISPLAYNAME);

		CoTaskMemFree(list);

		DestroyIcon(shFinfo.hIcon);

		m_hDriveRoot = InsertItem(TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT,
											shFinfo.szDisplayName, iIcon, shFinfo.iIcon,
											0, 0, CSIDL_DRIVES | FF_SYSTEM | FF_NODRAG | FF_NODROP,
											TVI_ROOT, 0);

		if (!DisplayDrives())
		{
			m_dont_notify = FALSE;
			return FALSE;
		}

	}

	Expand(GetRootItem(), TVE_EXPAND);
	Expand(m_hDriveRoot, TVE_EXPAND);
	return TRUE;	
}
/////////////////////////////////////////////////
BOOL CDirTreeCtrl::GetSysImgList()
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
							 SHGFI_SMALLICON);

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
							 SHGFI_SMALLICON);

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
	
	SetImageList(&sysImgList, TVSIL_NORMAL);
	sysImgList.Detach();

	return TRUE;
}

/* Insert `My Document', `My Computer' to `My Desktop' */
BOOL CDirTreeCtrl::DisplaySpecialFolders()
{

	/**
	 *  My Desktop,				CSIDL_DESKTOP
	 *  My Documents,			CSIDL_PERSONAL
	 *  My Computer,			CSIDL_DRIVES
	 *  Network Neighborhood,	CSIDL_NETWORK
	 */

	LPITEMIDLIST list;
	HTREEITEM hParent;
	TCHAR szPath[MAX_PATH];
	int systemFolders[3] = {CSIDL_PERSONAL, CSIDL_DRIVES, CSIDL_NETWORK};
	DWORD systemFoldersFlags[3] = {FF_NODRAG, FF_NODRAG | FF_NODROP, FF_NODRAG | FF_NODROP };
	int sfLength = 2;
	/* TODO: `Network Neighborhood' */
	for (int i = 0; i < sfLength; i++)
	{
		SHGetSpecialFolderLocation(m_pOwner->m_hWnd, systemFolders[i] ,&list);
		memset(szPath, 0, MAX_PATH);
		m_pSHGetFolderPath(NULL, systemFolders[i] | 0x00010000, 0, SHGFP_TYPE_CURRENT, szPath);
		hParent = AddItem(
			GetRootItem(),
			(LPCTSTR)list,
			SHGFI_PIDL | FF_DISPLAYNAME,
			systemFolders[i] | FF_SYSTEM | systemFoldersFlags[i]);
		CoTaskMemFree(list);
		if (!_tcslen(szPath) || FindSubDir(szPath))
			InsertItem(_T(""), 0, 0, hParent);
		if(systemFolders[i] == CSIDL_DRIVES)
			m_hDriveRoot = hParent;
	}
	
	return true;
}

BOOL CDirTreeCtrl::DisplayDrives()
{
	//
	// Displaying the Available Drives on this PC
	// This are the First Items in the TreeCtrl
	//
	TCHAR  szDrives[128];
	TCHAR* pDrive;

	if ( !GetLogicalDriveStrings( sizeof(szDrives)/sizeof(TCHAR), szDrives ) )
	{
		m_strError = "Error Getting Logical DriveStrings!";
		return FALSE;
	}

	pDrive = szDrives;
	while( *pDrive )
	{
		HTREEITEM ret = AddItem(m_hDriveRoot, pDrive, FF_DISPLAYNAME, FF_DRIVE | FF_NODRAG | pDrive[0]);
	//	if ( FindSubDir( pDrive ) )
			InsertItem( _T(""), 0, 0, ret );
		pDrive += _tcslen( pDrive ) + 1;
	}

	return TRUE;
}

void ArrayQuickSort(std::vector<CString> &array, int l, int r);
void ArrayInsertionSort(std::vector<CString> &array);
void SortArray(std::vector<CString> &array)
{
	// Sorts the Array for n < 11 using insertion sort, else using Quicksort
	
	if (array.size() > 1) 
		if (array.size() < 11) ArrayInsertionSort(array);
		else ArrayQuickSort(array, 0, array.size()-1);
}

void ArrayQuickSort(std::vector<CString> &array, int l, int r)
{
	int i,j;
	int max = array.size();
	CString v;

	if (r > l) {
		v = array[r];
		i = l;
		j = r;
		for (;;) {
			while ((i < r) && (array[i].CollateNoCase( v ) < 0)) i++;
			while ((array[j].CollateNoCase( v ) >= 0) && (j > l)) j--;
			if (i >= j) break;
			v = array[i];
			array[i] = array[j];
			array[j] = v;
		}
		v = array[i];
		array[i]=array[r];
		array[r]=v;

		ArrayQuickSort(array, l, i - 1);
		ArrayQuickSort(array, i + 1, r);
	}
}

void ArrayInsertionSort(std::vector<CString> &array)
{
	int i, j;
	CString v;
	int n = array.size();

	for (i = 1; i < n; i++)
	{
		v = array[i];
		j = i;
		while ((j > 0) && (array[j - 1].CollateNoCase(v) > 0))
		{
			array[j] = array[j - 1];
			j--;
		}
		array[j] = v;
	}
}

void CDirTreeCtrl::DisplayPath(HTREEITEM hParent, LPCTSTR strPath)
{
	//
	// Displaying the Path in the TreeCtrl
	//
	CFileFind find;
	CString   strPathFiles = strPath;
	BOOL      bFind;
	std::vector<CString> strDirArray;

	if ( strPathFiles.Right(1) != "\\" )
		strPathFiles += "\\";
	strPathFiles += "*.*";

	bFind = find.FindFile( strPathFiles );

	while ( bFind )
	{
		bFind = find.FindNextFile();
		if ( find.IsDirectory() && !find.IsDots() )
		{
			strDirArray.push_back( find.GetFilePath() );
		}
		
	}
		    
	SortArray(strDirArray);
	SetRedraw( FALSE );
	CWaitCursor wait;
    
	for (UINT i = 0; i < strDirArray.size(); i++)
	{
		HTREEITEM hItem = AddItem( hParent, strDirArray[i]);
		if (hItem && FindSubDir(strDirArray[i]))
			InsertItem(_T(""), 0, 0, hItem);
	}

    
	SetRedraw(TRUE);	
}

HTREEITEM CDirTreeCtrl::AddItem(HTREEITEM hParent, LPCTSTR strPath, UINT uFlags, DWORD lparam)
{
	/*
	 * Two extra parameters:
	 * UINT uFlags,
	 *   SHGFI_PIDL bits indicate that strPath is a ITEMIDLIST, not a path name
	 *   FF_DISPLAYNAME indicate that using szDisplayName instand of GetSubPath()
	 * lparam,
	 *   what behavior should the folder act. See DirTreeCtrl.h
	 **/
	// Adding the Item to the TreeCtrl with the current Icons
    CString strTemp = strPath;
    
	strTemp.TrimRight('\\');
	
	int iIcon;
	SHFILEINFO shFinfo;
	if (!SHGetFileInfo(strPath,
				0,
				&shFinfo,
				sizeof( shFinfo ),
				(uFlags & SHGFI_PIDL) |
				SHGFI_ICON |
				SHGFI_SMALLICON ) )
	{
		m_strError = "Error Gettting SystemFileInfo!";
		return 0;
	}
	
	iIcon = shFinfo.iIcon;
	
	// we only need the index from the system image list
	DestroyIcon(shFinfo.hIcon);
	
	if ( !SHGetFileInfo( strPath,
				0,
				&shFinfo,
				sizeof( shFinfo ),
				(uFlags & SHGFI_PIDL) |
				SHGFI_ICON | SHGFI_OPENICON |
				((uFlags & FF_DISPLAYNAME) ? SHGFI_DISPLAYNAME : 0) |
			    SHGFI_SMALLICON ) )
	{
		m_strError = "Error Gettting SystemFileInfo!";
		return 0;
	}

	// we only need the index from the system image list
	DestroyIcon( shFinfo.hIcon );

	HTREEITEM ret = InsertItem(
			TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT,
			(uFlags & FF_DISPLAYNAME) ? shFinfo.szDisplayName : GetSubPath(strPath), //Todo: use szDisplayName only
			iIcon,
			shFinfo.iIcon,
			0,
			0,
			lparam,
			hParent,
			TVI_LAST);

	return ret;
}

LPCTSTR CDirTreeCtrl::GetSubPath(LPCTSTR strPath)
{
	//
	// getting the last SubPath from a PathString
	// e.g. C:\temp\readme.txt
	// the result = readme.txt
	static CString strTemp;
	int     iPos;

	strTemp = strPath;
	strTemp.TrimRight('\\');
	iPos = strTemp.ReverseFind( '\\' );
	if ( iPos != -1 )
	    strTemp = strTemp.Mid( iPos + 1);

	return (LPCTSTR)strTemp;
}

BOOL CDirTreeCtrl::FindSubDir( LPCTSTR strPath)
{
	//
	// Are there subDirs ?
	//
	CFileFind find;
	CString   strTemp = strPath;
	BOOL      bFind;

	if ( strTemp[strTemp.GetLength()-1] == '\\' )
		strTemp += "*.*";
	else
		strTemp += "\\*.*";
		
	bFind = find.FindFile( strTemp );
	
	
	while ( bFind )
	{
		bFind = find.FindNextFile();

		if ( find.IsDirectory() && !find.IsDots() )
		{
			return TRUE;
		}
		
	}

	return FALSE;

}

void CDirTreeCtrl::OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	 
	ExpandItem( pNMTreeView->itemNew.hItem, TVE_EXPAND );

	*pResult = 0;
}

CString CDirTreeCtrl::GetFullPath(HTREEITEM hItem) const
{
	if (!hItem)
		return _T("");

	// get the Full Path of the item
	CString strReturn;
	CString strTemp;
	HTREEITEM hParent = hItem;
	TVITEM item;
	TCHAR szPath[MAX_PATH];

	strReturn = _T("");

	do
	{
		item.hItem = hParent;
		item.mask = TVIF_HANDLE;
		GetItem(&item);
		if (!item.lParam)
		{	// a normal folder
			strTemp = GetItemText(hParent);
		}
		else if (item.lParam & FF_HBYTES & FF_SYSTEM)
		{
			switch (item.lParam & FF_LBYTES)
			{
			case CSIDL_DRIVES:
				if (strReturn == _T(""))
					return _T("\\");
				else
					return strReturn;
			case CSIDL_PERSONAL:	/* My Documents */
			case CSIDL_DESKTOPDIRECTORY:		/* My Desktop */
				if (!m_pSHGetFolderPath)
					return _T("");
				memset(szPath, 0, MAX_PATH);
				m_pSHGetFolderPath(NULL, item.lParam & 0xFFFF, 0, SHGFP_TYPE_CURRENT, szPath);
				strTemp = szPath;
				strTemp += _T("\\");
				strReturn = strTemp + strReturn;
				return strReturn;
			case CSIDL_NETWORK:		/* Network Neighborhood */
				break;
			}
		}
		else if (item.lParam & FF_HBYTES & FF_DRIVE )
			return CString( (TCHAR)(item.lParam&FF_LBYTES) ) + _T(":\\") + strReturn;

		strTemp += _T("\\");
		strReturn = strTemp + strReturn;
	} while (hParent = GetParentItem(hParent));
    
	return strReturn;
}

BOOL CDirTreeCtrl::SetSelPath(CString strPath)
{
	m_dont_notify = true;
	if (strPath == _T("\\") || strPath == _T(""))
	{
		SelectItem(m_hDriveRoot);
		ExpandItem(m_hDriveRoot, TVE_EXPAND);
		m_dont_notify = false;
		return TRUE;
	}

	if (!IsValidPath(strPath))
	{
		m_dont_notify = FALSE;
		return FALSE;
	}

	// Setting the Selection in the Tree
	HTREEITEM hParent  = m_hDriveRoot;
	int       iLen    = strPath.GetLength() + 2;
	LPTSTR    pszPath = new TCHAR[iLen];
	LPTSTR    pPath   = pszPath;
	BOOL      bRet    = FALSE;
    
	_tcscpy( pszPath, strPath );
	_tcsupr( pszPath );
	
	if ( pszPath[_tcslen(pszPath)-1] != '\\' )
		_tcscat( pszPath, _T("\\") );
    
	int iLen2 = _tcslen( pszPath );
	
	for (WORD i = 0; i < iLen2; i++ )
	{
		if ( pszPath[i] == '\\' )
		{
			pszPath[i] = '\0';
			/* Set path from `My Computer(hDrives' */
			if( hParent == m_hDriveRoot)
			{
				TVITEM item;
				hParent = GetChildItem(m_hDriveRoot);
				while(hParent)
				{
					item.hItem = hParent;
					item.mask = 0;
					GetItem(&item);
					if( toupper((item.lParam& FF_LBYTES)) == toupper(pszPath[0]) )
					{
						break;
					}
					hParent = GetNextItem( hParent, TVGN_NEXT);
				}

			}
			else
			{
				hParent = SearchSiblingItem( hParent, pPath );
			}
			if ( !hParent )  // Not found!
				break;
			else
			{				
				HTREEITEM hFound = GetChildItem(hParent);
				CString   strTemp;
				while (hFound)
				{
					Expand(hFound,TVE_COLLAPSE);
					hFound = GetNextItem( hFound, TVGN_NEXT );
				}

				Expand( hParent, TVE_EXPAND );
			}
			pPath += _tcslen(pPath) + 1;
		}
	}
	
	delete [] pszPath;
	
	if (hParent) // Ok the last subpath was found
	{		
		Select(hParent, TVGN_FIRSTVISIBLE); // select the last expanded item
		SelectItem(hParent);

		bRet = TRUE;
	}
	else
	{
		bRet = FALSE;
	}
	SetRedraw( TRUE );
	

	m_dont_notify = false;
	return bRet;
}

HTREEITEM CDirTreeCtrl::SearchSiblingItem( HTREEITEM hItem, LPCTSTR strText)
{
	HTREEITEM hFound = GetChildItem( hItem );
	CString   strTemp;
	while ( hFound )
	{
		strTemp = GetItemText( hFound );
        strTemp.MakeUpper();
		if ( strTemp == strText )
			return hFound;
		hFound = GetNextItem( hFound, TVGN_NEXT );
	}

	return NULL;
}


void CDirTreeCtrl::ExpandItem(HTREEITEM hItem, UINT nCode)
{	
	if (!GetParentItem(hItem))
		return;
	if ( nCode == TVE_EXPAND )
	{
		TVITEM item;
		item.hItem=hItem;
		item.mask=TVIF_HANDLE|TVIF_STATE;
		GetItem(&item);

		if (item.state&TVIS_EXPANDEDONCE)
		{
			return;
		}
		HTREEITEM hChild = GetChildItem( hItem );
		while ( hChild )
		{
			DeleteItem( hChild );
			hChild = GetChildItem( hItem );
		}
		/* weather we are selecting `My Computer' */
		if( hItem != m_hDriveRoot )
		{
			CString strPath = GetFullPath( hItem );
			DisplayPath( hItem, strPath );
		}
		else
		{
			DisplayDrives();
		}
	}
}

BOOL CDirTreeCtrl::IsValidPath(LPCTSTR strPath)
{
	// This function check the Pathname
	
	HTREEITEM hChild;
	CString   strItem;
	CString   strTempPath = strPath;
	BOOL      bFound = FALSE;
	CFileFind find;

	hChild = GetChildItem( GetRootItem() );
	strTempPath.MakeUpper();
	strTempPath.TrimRight('\\');

/*	while ( hChild )
	{
		strItem = GetItemText( hChild );
		strItem.MakeUpper();
		if ( strItem == strTempPath.Mid( 0, strItem.GetLength() ) )
		{
			bFound = TRUE;
			break;
		}
		hChild = GetNextItem( hChild, TVGN_NEXT );
	}
    
	if ( !bFound )
		return FALSE;*/

	strTempPath += "\\nul";
	if ( find.FindFile( strTempPath ) )
		return TRUE;
     
	return FALSE;
}

void CDirTreeCtrl::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	if (!m_dont_notify)
	{
		CIsoEditSettingDlg* pOwner = static_cast<CIsoEditSettingDlg*>(m_pOwner);

		pOwner->SetLocalFolderOut(GetDlgCtrlID() , GetFullPath(pNMTreeView->itemNew.hItem));
	}
	*pResult = 0;
}

void CDirTreeCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CTreeCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}

int CDirTreeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	GetSysImgList()	;
		
	return 0;
}

void CDirTreeCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar==VK_F2)
	{
		// To Use F2 as hot Key to get EditCtrl on the ListView it must have 
		// the Style TVS_EDITLABELS
		ASSERT( GetStyle() & TVS_EDITLABELS );
		// don't do an Edit Label when the multiple Items are selected
		//Don't allow to rename "My Computer"
		if (GetRootItem()==GetSelectedItem())
			return;
		//Don't allow to rename the drives
		if (GetRootItem()==GetParentItem(GetSelectedItem()))
			return;
		VERIFY( EditLabel( GetSelectedItem() ) != NULL );
	}

	CTreeCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

#define VK_A		65
BOOL CDirTreeCtrl::PreTranslateMessage(MSG* pMsg) 
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
			if( pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_CONTROL || pMsg->wParam == VK_INSERT || pMsg->wParam == VK_SHIFT )
			{
				edit->SendMessage(WM_KEYDOWN, pMsg->wParam, pMsg->lParam);
				return TRUE;
			}
		}
	}
	if (pMsg->message == WM_RBUTTONDOWN)
	{
		if (pMsg->hwnd!=GetSafeHwnd())
			return FALSE;
		/*CEdit* edit = GetEditControl();
		if (edit)
			return FALSE;*/
		HTREEITEM olditem=GetSelectedItem();
		CPoint point=pMsg->pt;
		ScreenToClient(&point);
		HTREEITEM item=HitTest(point,0);
		if (item)
		{
			if (item!=olditem)
			{
				SelectItem(item);
				ExpandItem(item,TVE_EXPAND);
				//m_pOwner->SetLocalFolderOut(GetFullPath(item));
				SetFocus();
			}
			
		}
		return TRUE;
	}
	return CTreeCtrl::PreTranslateMessage(pMsg);
}

bool CDirTreeCtrl::DragPosition(CImageList *pImageList, CWnd* pDragWnd, CPoint point)
{
	ScreenToClient(&point);
	HTREEITEM hItem = HitTest(point);

	if (!hItem)
	{
		SetDragHighlight(0, pImageList);
		return false;
	}

	/* we should not drop mDragSource to folders like "My Computer", etc. */
	TVITEM item;
	item.hItem = hItem;
	item.mask = TVIF_HANDLE;
	GetItem(&item);
	if (item.lParam & FF_NODROP) 
	{
		SetDragHighlight(0, pImageList);
		return false;
	}

	if (pDragWnd == this)
	{
		CString dragPath = GetFullPath(m_hDragSource);
		CString path = GetFullPath(hItem);
		if (dragPath.GetLength() < path.GetLength() &&
			path.Left(dragPath.GetLength()) == dragPath)
		{
			SetDragHighlight(0, pImageList);
			return FALSE;
		}
		
		/* draging "C:", "D:", "My Computer", "My Desktop", etc, is not allowed */
		TVITEM dragsource;
		dragsource.hItem = m_hDragSource;
		dragsource.mask = TVIF_HANDLE;
		GetItem(&dragsource);
		if (dragsource.lParam & FF_NODRAG) 
		{
			SetDragHighlight(0, pImageList);
			return false;
		}
	}

	SetDragHighlight(hItem, pImageList);

	return true;
}

CString CDirTreeCtrl::GetDropTarget() const
{
	return GetFullPath(m_hDragHilited);
}

// Struct for children list
// Unfortunately declared outside scope of RefreshDir due to linkage problems 
// (at least in VC++ 6)
struct t_children
{
	HTREEITEM hItem;
	CString text;
};

void CDirTreeCtrl::RefreshDir(HTREEITEM hRefreshItem)
{
	if (!hRefreshItem)
		return;

	CString path = GetFullPath(hRefreshItem);

	//
	// Displaying the Path in the TreeCtrl
	//
	CFileFind find;
	CString   strPathFiles = path;
	BOOL      bFind;
	std::vector<CString> strDirArray;
	
	if ( strPathFiles.Right(1) != "\\" )
		strPathFiles += "\\";
	strPathFiles += "*.*";

	bFind = find.FindFile( strPathFiles );

	while ( bFind )
	{
		bFind = find.FindNextFile();
		if ( find.IsDirectory() && !find.IsDots() )
		{		
			strDirArray.push_back( find.GetFilePath() );
		}
	}

	// Get children list which is already sorted
	std::vector<t_children> childrenArray;
	HTREEITEM hChildItem = GetChildItem(hRefreshItem);
	while (hChildItem)
	{
		t_children child;
		child.hItem = hChildItem;
		child.text = GetItemText(hChildItem);
		childrenArray.push_back(child);
		hChildItem = GetNextSiblingItem(hChildItem);
	}

	SortArray(strDirArray);
	
	SetRedraw(FALSE);
	CWaitCursor wait;
    
	// Search for new/deleted entries
	unsigned int j = 0;
	for (UINT i = 0; i < strDirArray.size(); i++)
	{
		CString subPath = GetSubPath(strDirArray[i]);
		int comp;
		if (j < childrenArray.size())
			comp = subPath.CollateNoCase(childrenArray[j].text);
		else
			comp = -1;
		if (!comp)
			j++;
		else if (comp < 0)
		{
			HTREEITEM hItem = AddItem( hRefreshItem, strDirArray[i] );
			if (FindSubDir(strDirArray[i]))
				InsertItem(_T(""), 0, 0, hItem);
		}
		else
		{
			i--;

			DeleteItem(childrenArray[j].hItem);
			j++;
		}
	}
	//SortChildren(hRefreshItem);
    
	SetRedraw(TRUE);

}

void CDirTreeCtrl::RefreshDir(CString dir)
{
	if (dir == "")
		return;

	if (dir.Right(1) != _T("\\"))
		dir += "\\";
	
	HTREEITEM hParent = GetRootItem();
	while (dir != _T(""))
	{
		int pos = dir.Find(_T("\\"));
		if (pos == -1)
			return;

		CString sub = dir.Left(pos);
		dir = dir.Mid(pos + 1);

		HTREEITEM hItem = GetChildItem(hParent);
		while (hItem)
		{
			if (!GetItemText(hItem).CollateNoCase(sub))
			{
				hParent = hItem;
				break;
			}
			hItem = GetNextSiblingItem(hItem);
		}
		if (!hItem)
			return;
	}
	if (hParent != GetRootItem())
		RefreshDir(hParent);
}

void CDirTreeCtrl::SetDragHighlight(HTREEITEM hItem, CImageList* pImageList)
{
	if (hItem == m_hDragHilited)
		return;

	if (pImageList)
		pImageList->DragShowNolock(false);

	if (m_hDragHilited)
		SetItemState(m_hDragHilited, 0, TVIS_DROPHILITED);
	if (hItem)
		SetItemState(hItem, TVIS_DROPHILITED, TVIS_DROPHILITED);
	UpdateWindow();

	if (pImageList)
		pImageList->DragShowNolock(true);

	m_hDragHilited = hItem;
}
