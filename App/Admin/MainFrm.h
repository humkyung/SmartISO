// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__1252E091_488E_47D2_9E18_EE554E555458__INCLUDED_)
#define AFX_MAINFRM_H__1252E091_488E_47D2_9E18_EE554E555458__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainFrame : public CFrameWndEx
{
	
public: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMFCRibbonBar   m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCStatusBar  m_wndStatusBar;
	///CToolBar    m_wndToolBar;
// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdateUpdate(CCmdUI* pCmdUI);	/// 2012.06.11 added by humkyung
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnToolsDatabaseSetting();
	afx_msg void OnUpdateToolsDatabaseSetting(CCmdUI *pCmdUI);
	afx_msg void OnToolsSetting();
	afx_msg void OnUpdateToolsSetting(CCmdUI *pCmdUI);
	afx_msg void OnImportLineList();						/// 2015.11.05 added by humkyung
	afx_msg void OnUpdateImportLineList(CCmdUI* pCmdUI);	/// 2015.11.05 added by humkyung
	afx_msg void OnClose();								/// 2012.06.11 added by humkyung

	void InitializeRibbon();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__1252E091_488E_47D2_9E18_EE554E555458__INCLUDED_)
