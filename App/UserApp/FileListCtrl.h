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

#if !defined(AFX_FILELISTCTRL_H__453BE9E4_AFDA_4745_A48F_E8BDC0311E31__INCLUDED_)
#define AFX_FILELISTCTRL_H__453BE9E4_AFDA_4745_A48F_E8BDC0311E31__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LocalFileListCtrl.h : Header-Datei
//

#include <vector>
#include <map>
#include <set>
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// Fenster CFileListCtrl 
class CFileListCtrl : public CListCtrl
{

// Konstruktion
public:
	CFileListCtrl(CWnd *pOwner);

// Attribute
public:
// Operationen
public:

// ?erschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktions?erschreibungen
	//{{AFX_VIRTUAL(CFileListCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	protected:
	//}}AFX_VIRTUAL

// Implementierung
public:
	void SetColumnSize(const int& size);
	void SetFileFilter(const CString& rFileFilter);
	void SetDisplayFolder(bool disp);
	void RefreshFile(CString file);
	void UpdateColumns(int nHideColumns);
	void SetListStyle(int nStyle);
	virtual void SetFolder(CString folder);
	void ReloadHeader();
	CString GetFolder() const;
	virtual ~CFileListCtrl();
	void SaveColumnSizes();
	void SortList(int item, int direction = 0);
	void GetSortInfo(int &column, int &direction) const;

	// Drag&Drop
	BOOL DragPosition(CImageList *pImageList, CWnd* pDragWnd, CPoint point);
	void DragLeave(CImageList *pImageList);
	CString GetDropTarget() const;

	// Generierte Nachrichtenzuordnungsfunktionen
protected:
	void ClearFileData();
	CWnd *m_pOwner;
	CString GetType(CString lName, BOOL bIsDir);
	class t_FileData
	{
	public:
		t_FileData()
		{
			iIcon = -1;
		};
		CString Name;
		CString lName;
		BOOL bIsDir;
		_int64 nSize;
		bool hasTime;
		CTime Time;
		int iIcon;
	};
	CString m_NewDir;
	BOOL m_bUpdating;
	bool m_listOnly;	// indication that we can not download files here
	CString m_EditDir; // The directory we're in when label editing starts
	int m_nHideColumns;
	int m_nStyle;
	CString m_transferuser;
	CString m_transferpass;
	CString m_rFileFilter;
	bool m_bDisplayFolder;

	void DisplayDrives();
	std::vector<unsigned int> m_IndexMapping;
	std::map<CString, CString> m_TypeCache;
	void ReloadHeaderItem(int nIndex,UINT nID);
	int m_sortcolumn;
	int m_sortdir;
	std::map<CString, CString> m_permanentTypeCache;
	CImageList m_SortImg;
	BOOL GetSysImgList();
	CString m_Fullpath;
	std::vector<t_FileData> m_FileData;
	UINT GetSelectedItem();
	
	int   m_nColumnSize;
	BOOL* m_pColumns;

	void quicksortbyname(const BOOL &direction, int anf, int ende);
	void quicksortbytype(const std::vector<CString> &array, const BOOL &direction, int anf, int ende);
	void quicksortbysize(const BOOL &direction, int anf, int ende);
	void quicksortbytime(const BOOL &direction, int anf, int ende);

	// If refreshing local directory, store selected elements and restore them
	// after refresh is complete
	std::set<CString> m_selectionBuffer;

	// Drag&Drop
	int m_nDragHilited;
	
	//{{AFX_MSG(CFileListCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnUpdateContinue(WPARAM wParam, LPARAM lParam);
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ f?t unmittelbar vor der vorhergehenden Zeile zus?zliche Deklarationen ein.

#endif // AFX_FILELISTCTRL_H__453BE9E4_AFDA_4745_A48F_E8BDC0311E31__INCLUDED_
