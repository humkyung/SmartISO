#if _MSC_VER > 1000
#pragma once
#endif


#include <GradientStatic.h>
#include <BtnST.h>
#include <ColorCheckButton.h>
#include "UserAppDoc.h"
/////////////////////////////////////////////////////////////////////////////
// CIsoEditOptionDlg2 dialog

class CIsoEditOptionDlg2 : public CDialog
{
// Construction
public:
	void EnableAllOptions();
	void DisableAllOptions();
	CIsoEditOptionDlg2(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIsoEditOptionDlg2();

	int InitContents();
// Dialog Data
	//{{AFX_DATA(CIsoEditOptionDlg2)
	enum { IDD = IDD_ISO_EDIT_OPTION2 };
		// NOTE: the ClassWizard will add data members here
	int m_nSelectFrom;
	//}}AFX_DATA
	CGradientStatic m_TitleStaticCtrl;
	CColorCheckButton m_chkDrwNo , m_chkErrorRead , m_chkPipeCutErr , m_chkNotFoundErr;
	CColorCheckButton m_chkFailIsoErr , m_chkPcdSymErr , m_chkHoldItemErr;
	CColorCheckButton m_chkSymCodeErr , m_chkSupportBMErr , m_chkTextModifiedErr , m_chkFGBErr , m_chkSpecBreakErr;
	CColorCheckButton m_chkBOMOverCheck , m_chkSpecBreakHoldCheck;
	CButtonST m_StartButton;

	CWnd* m_pTargetWnd;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIsoEditOptionDlg2)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
// Implementation
protected:
	CFont* m_pBoldFont;
	BOOL m_bBMNotFound , m_bBMConsistencyError;
	BOOL m_DRWNO, m_ERRREAD, m_FGBERR, m_PIPECUTERR, m_NOTFOUNERR, m_FAILISOERR,
		m_PCDSYMERR, m_HOLDITEMERR, m_SPECBREAKERR,m_SYMCODEERR, m_SUPPORTBMERR,m_TextModify,m_BOMOVER;
	// Generated message map functions
	//{{AFX_MSG(CIsoEditOptionDlg2)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckDrwNo();
	afx_msg void OnCheckErrRead();
	afx_msg void OnCheckFGBErr();
	afx_msg void OnCheckPipeCutErr();
	afx_msg void OnCheckNotFoundErr();
	afx_msg void OnCheckFailIsoErr();
	afx_msg void OnCheckPcdSymBolErr();
	afx_msg void OnCheckHoldItemErr();
	afx_msg void OnCheckSpecBreakErr();
	afx_msg void OnCheckSymBolCodeErr();
	afx_msg void OnCheckSupportBMErr();
	afx_msg void OnCheckTextModify();
	afx_msg void OnCheckBOMOver();	/// 2013.06.12 added by humkyung
	//afx_msg void OnCheckQualityControlCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	static CString m_sAppName;
};
