// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__54D6B5E1_76A4_44EA_ADA4_4265CABD5D7F__INCLUDED_)
#define AFX_MAINFRM_H__54D6B5E1_76A4_44EA_ADA4_4265CABD5D7F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WorkspaceWnd.h"
#include "UserAppOutputWnd.h"
#include "PropertiesWnd.h"

class CMainFrame : public CFrameWndEx
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
// Operations
public:
	CWorkspaceWnd* GetWorkspaceBar();
	CUserAppOutputWnd* GetUserAppOutputBar();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
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
	CMFCStatusBar	m_wndStatusBar;

	CWorkspaceWnd m_wndWorkspaceBar;
	CUserAppOutputWnd m_wndOutputBar;
	CPropertiesWnd    m_wndProperties;
// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnIssueManager();
	afx_msg void OnLineData();
	afx_msg void OnInsertToPdsAttr();
	afx_msg void OnIsoProduct();
	afx_msg void OnIsoRevAutomation();
	afx_msg void OnIsoIssueAutomation();
	afx_msg void OnDrawingReport();
	afx_msg void OnBOMReport();
	afx_msg void OnDrawingPlotting();
	afx_msg void OnFileBackup();
	afx_msg void OnUpdateIssueManager(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBmReport(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLineData(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGetPdsLineData(CCmdUI* pCmdUI);
	afx_msg void OnUpdateInsertToPdsAttr(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIsoProduct(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIsoRevAutomation(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIsoIssueAutomation(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLineListConverter(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawingReport(CCmdUI* pCmdUI);
	afx_msg void OnUpdateDrawingPlotting(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUpdate(CCmdUI* pCmdUI);		/// 2012.06.11 added by humkyung
	afx_msg void OnUseIPLOT();
	afx_msg void OnUpdateCheckIPLOT(CCmdUI* pCmdUI);		/// 2015.09.03 added by humkyung
	afx_msg void OnUpdateComboPrinterList(CCmdUI* pCmdUI);	/// 2015.09.03 added by humkyung
	afx_msg void OnGenerateIsoProduct();						/// 2015.09.22 added by humkyung
	afx_msg void OnUpdateGenerateIsoProduct(CCmdUI* pCmdUI);	/// 2015.09.22 added by humkyung
	afx_msg void OnGenerateDrawingReport();						/// 2015.09.22 added by humkyung
	afx_msg void OnUpdateGenerateDrawingReport(CCmdUI* pCmdUI);	/// 2015.09.22 added by humkyung
	afx_msg void OnGenerateBM();						/// 2015.09.23 added by humkyung
	afx_msg void OnUpdateGenerateBM(CCmdUI* pCmdUI);	/// 2015.09.23 added by humkyung
	afx_msg void OnPrintDrawing();						/// 2015.09.24 added by humkyung
	afx_msg void OnUpdatePrintDrawing(CCmdUI* pCmdUI);	/// 2015.09.24 added by humkyung
	afx_msg void OnUpdateOracle();						/// 2015.09.24 added by humkyung
	afx_msg void OnUpdateUpdateOracle(CCmdUI* pCmdUI);	/// 2015.09.24 added by humkyung

	/// 2012.08.26 added by humkyung
	afx_msg void OnShowWorkspacebar();
	afx_msg void OnUpdateShowWorkspacebar(CCmdUI* pCmdUI);
	afx_msg void OnShowOutputbar();
	afx_msg void OnUpdateShowOutputbar(CCmdUI* pCmdUI);
	/// up to here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void InitializeRibbon();
public:
	afx_msg void OnClose();
};

//! 다른 쓰레드에서는 Afxxxx 계열의 함수가 제대로 작동을 하지 않으므로...
extern CMainFrame* GetUserAppMainWnd();
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__54D6B5E1_76A4_44EA_ADA4_4265CABD5D7F__INCLUDED_)
