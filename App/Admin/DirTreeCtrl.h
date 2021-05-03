#ifndef AFX_DIRTREECTRL_H__1E4F97A0_B41E_11D2_955E_204C4F4F5020__INCLUDED_
#define AFX_DIRTREECTRL_H__1E4F97A0_B41E_11D2_955E_204C4F4F5020__INCLUDED_

// DirTreeCtrl.h : Header-Datei
//

/////////////////////////////////////////////////////////////////////////////
// Fenster CDirTreeCtrl 
class CLocalView;

class CDirTreeCtrl : public CTreeCtrl
{
// Konstruktion
public:
	CDirTreeCtrl();

// Attribute
public:

// Operationen
public:

// Überschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktionsüberschreibungen
	//{{AFX_VIRTUAL(CDirTreeCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementierung
public:
	bool m_dont_notify;
	BOOL SetSelPath( CString strPath );
	CString GetFullPath( HTREEITEM hItem ) const;
	LPCTSTR GetSubPath( LPCTSTR strPath );
	BOOL DisplayTree();
	virtual ~CDirTreeCtrl();
	CWnd *m_pOwner;

	// Drag&Drop
	bool DragPosition(CImageList *pImageList, CWnd* pDragWnd, CPoint point);
	void SetDragHighlight(HTREEITEM hItem, CImageList* pImageList);
	CString GetDropTarget() const;
	HTREEITEM m_hDragSource;
	void RefreshDir(CString dir);
	
	// Generierte Nachrichtenzuordnungsfunktionen
protected:
	CString m_transferpass;
	CString m_transferuser;
	BOOL IsValidPath(LPCTSTR strPath);
	void ExpandItem(HTREEITEM hItem, UINT nCode);
	HTREEITEM SearchSiblingItem(HTREEITEM hItem, LPCTSTR strText);
	BOOL FindSubDir(LPCTSTR strPath);
	HTREEITEM AddItem(HTREEITEM hParent, LPCTSTR strPath, UINT uFlags=0, DWORD lparam=0);

	/* Folder Flags */
	#define FF_SYSTEM			0x00010000	/* Lower two bytes indicate what folder it is (My Computer, etc) */
	#define FF_DRIVE			0x00020000  /* A disc drive (ex: C:, D: ...)
											   Drive letter stored in lowermost byte */
	#define FF_DISPLAYNAME		0x00100000
	#define FF_NODROP			0x10000000	/* We should not drop any thing into this folder */
	#define FF_NODRAG			0x20000000	/* We should not drag this folder to any folder */
	#define FF_HBYTES			0xFFFF0000
	#define FF_LBYTES			0x0000FFFF

	void DisplayPath(HTREEITEM hParent, LPCTSTR strPath);
	BOOL DisplayDrives();
	BOOL DisplaySpecialFolders();
	HTREEITEM m_hDriveRoot;

	CString m_strError;
	BOOL GetSysImgList();
	void RefreshDir(HTREEITEM hRefreshItem);

	// Drag&Drop
	HTREEITEM m_hDragHilited;

	HINSTANCE m_hDll;
	HRESULT (WINAPI *m_pSHGetFolderPath)(HWND hwndOwner, int nFolder, HANDLE hToken, DWORD dwFlags, LPTSTR pszPath);

	//{{AFX_MSG(CDirTreeCtrl)
	afx_msg void OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio fügt zusätzliche Deklarationen unmittelbar vor der vorhergehenden Zeile ein.

#endif // AFX_DIRTREECTRL_H__1E4F97A0_B41E_11D2_955E_204C4F4F5020__INCLUDED_
