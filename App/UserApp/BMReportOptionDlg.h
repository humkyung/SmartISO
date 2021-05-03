#if !defined(AFX_BMREPORTOPTIONDLG_H__89B629CE_1FAE_4B86_BFCE_B3A5398C062C__INCLUDED_)
#define AFX_BMREPORTOPTIONDLG_H__89B629CE_1FAE_4B86_BFCE_B3A5398C062C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BMReportOptionDlg.h : header file
//

#include <GradientStatic.h>
#include <BtnST.h>
/////////////////////////////////////////////////////////////////////////////
// CBMReportOptionDlg dialog

class CBMReportOptionDlg : public CDialog
{
// Construction
public:
	BOOL GetSaveErrorSumFile() const;
	bool GetMakeWeldDetailCheck() const;
	bool GetPipeSupportCheck() const;
	bool GetGenerateINFSumFileCheck() const;
	bool GetGenerateINFFileCheck() const;
	bool GetMakeInsBMCheck() const;
	bool GetMakePaintCodeCheck() const;
	bool GetSaveLogFileCheck() const;
	bool GetLogFileSynchronizingCheck() const;
	bool GetMakePrtFileCheck() const;
	bool GetMakeNtrFileCheck() const;
	bool GetMakeWeldDiaCheck() const;
	CBMReportOptionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBMReportOptionDlg();
// Dialog Data
	//{{AFX_DATA(CBMReportOptionDlg)
	enum { IDD = IDD_BM_REPORT_OPTION };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CGradientStatic m_TitleStaticCtrl;
	CButtonST m_StartButton;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBMReportOptionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CFont* m_pBoldFont;
	// Generated message map functions
	//{{AFX_MSG(CBMReportOptionDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BMREPORTOPTIONDLG_H__89B629CE_1FAE_4B86_BFCE_B3A5398C062C__INCLUDED_)
