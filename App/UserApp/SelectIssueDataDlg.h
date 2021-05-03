#pragma once

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//
#include <ListCtrlEx.h>
#include <GradientStatic.h>
#include <BtnST.h>
#include "UserAppDoc.h"
/////////////////////////////////////////////////////////////////////////////
// CSelectIssueDataDlg dialog

#define CURRENT	0x00
#define ISSUED	0x01

class CSelectIssueDataDlg : public CDialog
{
// Construction
public:
	CSelectIssueDataDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelectIssueDataDlg();
// Dialog Data
	//{{AFX_DATA(CSelectIssueDataDlg)
	enum { IDD = IDD_SELECT_ISSUE_DATA };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CString m_sSelectedFolder;
		
	void GetSelectedIssueDate( vector<STRING_T>& aIssueDates );
	int InitContents( CWnd* pTargetWnd );
	void SaveData(const CString &rIniFilePath);
	void LoadData(const CString &rIniFilePath);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectIssueDataDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL m_bSelectCurrent , m_bSelectIssued;

	CGradientStatic m_TitleStaticCtrl;
	IsGui::CListCtrlEx m_wndIssueDateReport;
	CButtonST m_StartButton;
	CImageList m_ImageList;

	CWnd* m_pTargetWnd;
	CUserAppDoc* m_pUserAppDoc;

	CFont* m_pBoldFont;
	// Generated message map functions
	//{{AFX_MSG(CSelectIssueDataDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnRadioCurrent();
	afx_msg void OnRadioIssued();
	afx_msg void OnButtonSelect();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
