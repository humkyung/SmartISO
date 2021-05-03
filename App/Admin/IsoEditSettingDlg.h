#if !defined(AFX_ISOEDITSETTINGDLG_H__6D361A0C_C766_4441_8287_3E0EE0B8A5FB__INCLUDED_)
#define AFX_ISOEDITSETTINGDLG_H__6D361A0C_C766_4441_8287_3E0EE0B8A5FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IsoEditSettingDlg.h : header file
//
#include "AdminDoc.h"
#include <BtnSt.h>
#include <FilterEdit/FilterEdit.h>
#include <FilterEdit/UIntEdit.h>
//#include "DirTreeCtrl.h"
#include "GridCtrl/GridCtrl.h"
#include "BOMTextsWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CIsoEditSettingDlg dialog

class CIsoEditSettingDlg : public CDialog
{
// Construction
public:
	void CheckFolder(const CString& rString);
	void UpdateContents();
	void SetLocalFolderOut(const int& nId , CString folder);
	void Save();
	CIsoEditSettingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIsoEditSettingDlg();
// Dialog Data
	//{{AFX_DATA(CIsoEditSettingDlg)
	enum { IDD = IDD_ISO_EDIT_SETTING };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CButtonST m_inputFileNameButton;
	CButtonST m_outputFileNameButton;
	CButtonST m_wndOutputFileExtButton;	/// 2011.08.20 added by humkyung
	CEdit m_InputFileNameFormatEdit;
	CString m_rInputFileNameFormat;
	CEdit m_OutputFileNameFormatEdit;
	CString m_rOutputFileNameFormat;
	
	/// 2011.08.18 added by humkyung
	CEdit m_OutputFileExtFormatEdit;
	CString m_rOutputFileExtFormat;
	/// up to here

	CGridCtrl m_wndHeaderInfoGridCtrl;			/// 2011.08.29 added by humkyung
	CBMOTextsWnd m_wndBOMTexts;					/// 2015.11.26 added by humkyung
	CGridCtrl m_wndBMFormatGridCtrl;			/// 2011.08.31 added by humkyung

	CAdminDoc* m_pAdminDoc;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIsoEditSettingDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CFont* m_pFont;
	// Generated message map functions
	//{{AFX_MSG(CIsoEditSettingDlg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnButtonInputFileName();
	afx_msg void OnButtonOutputFileName();
	afx_msg void OnButtonOutputFileExt();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CAdminDoc* GetDocument();
public:
	afx_msg void OnBnClickedCheckLevel();
	afx_msg void OnBnClickedCheckColor();
	afx_msg void OnBnClickedCheckStyle();
	afx_msg void OnBnClickedCheckWeight();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnBnClickedRadioColumnData();
	afx_msg void OnBnClickedRadioMatlDesc();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnBnClickedButtonRefresh();
private:
	int DisplayBOMCOLDef();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ISOEDITSETTINGDLG_H__6D361A0C_C766_4441_8287_3E0EE0B8A5FB__INCLUDED_)
