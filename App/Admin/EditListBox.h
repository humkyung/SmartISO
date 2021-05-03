#if !defined(AFX_EDITLISTBOX_H__82A4F0C9_7051_483B_8939_7017A4B2845D__INCLUDED_)
#define AFX_EDITLISTBOX_H__82A4F0C9_7051_483B_8939_7017A4B2845D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditListBox.h : header file
//
#define BITMAP_SIZE 20
/////////////////////////////////////////////////////////////////////////////
// CEditListBox window

class CEditListBox : public CListBox
{
// Construction
public:
	CEditListBox();

// Attributes
public:

// Operations
public:
	HWND Create(HWND hWndParent, RECT& rcPos, LPCTSTR szWindowName = NULL,
			DWORD dwStyle = 0, DWORD dwExStyle = 0,
			UINT nID = 0, LPVOID lpCreateParam = NULL);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditListBox)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEditListBox();

	// Generated message map functions
protected:
	void DrawBorder(HDC hdcWindow, LPRECT lpRect, UINT grfFlags = BF_RECT);
	void CalcBorderOffsetFromStyle(LPSIZE lpszOffset);
	void CloseThemeData();
	void OpenThemeData(LPCWSTR pszClassList);
	static UINT GetThemeChangedMsg();
	void TermBorderCode();
	void InitBorderCode();
	BOOL IsVisualStyleEnabled() const;
	void UpdateButtons();
	void AddButton(UINT uCmd, int iImage);
	HBITMAP LoadBmpButtonImage();
	LRESULT SendNotifyMsg(UINT code, int iSource, int iDestination = -1);
	void Term();
	void Init();
	typedef struct tagButtonInfo
	{
		RECT rect;				// Button rectangle
		UINT uCmd;				// Command to send when clicked (WM_COMMAND)
		int iImage;				// image index of button in m_wndImages
		BOOL bDisabled;			// Button disabled or not?
		BOOL bPressed;			// Is the button pressed in or out?
		BOOL bPressedMouseOver;	// Is mouse cursor hover the button?
		BOOL bValidated;		// Has the button been redrawn and validated?
		LPTSTR lpszTipText;		// Tip text
	} ButtonInfo;
	
	typedef struct tagCaptionInfo
	{
		ButtonInfo buttonInfo[4];
		RECT rect;				// Caption rectangle
		short cnButton;
		BOOL bMouseDown;		// is the mouse button being clicked?
		int iActiveButton;		// the button index being clicked.
		BOOL bMouseHOver;		// is the mouse cursor hover the one of buttons?
		int iMouseOver;			// the button index on which the mouse cursor is.
		UINT nHeight;			// the height of the caption
		BOOL bHideMoveButtons;	// don't use nor show item move buttons
		BOOL bFlatBorder;		// use flat border instead of 3D border?
	} CaptionInfo;

	CImageList m_wndImages;
	CaptionInfo m_captionInfo;

	// Hotkey support (WM_SETTEXT, WM_HOTKEY)
	ATOM m_atomHotkey;
	
	// dummy item text and management
	BOOL m_bDummy;
	LPTSTR m_szDummy;
	
	// ToolTips support for bitmap buttons
	CToolTipCtrl m_wndTip;
	BOOL m_bTipTimeOut;

	// In-Place ButtonEdit control info
	typedef struct tagEditInfo
	{
		BOOL bShowButton;	// Show or hide the buddy browse button in Edit control
		BOOL bBrowseButtonPressed;
	} EditInfo;
	
	EditInfo m_editInfo;

#ifndef _UXTHEME_H_

	typedef HANDLE HTHEME;

#endif // #ifndef _UXTHEME_H_

	typedef HTHEME (STDAPICALLTYPE *PFNOpenThemeData)(HWND hwnd, LPCWSTR pszClassList);
	typedef HRESULT (STDAPICALLTYPE *PFNCloseThemeData)(HTHEME hTheme);
	typedef HRESULT (STDAPICALLTYPE *PFN_DrawThemeBackground)(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, const RECT *pRect, OPTIONAL const RECT *pClipRect);

	BOOL m_bVisualStyleEnabled;

	HMODULE m_hThemeDLL;
	HTHEME m_hTheme;
	PFN_DrawThemeBackground m_pfnDrawThemeBackground;
	//{{AFX_MSG(CEditListBox)
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITLISTBOX_H__82A4F0C9_7051_483B_8939_7017A4B2845D__INCLUDED_)
