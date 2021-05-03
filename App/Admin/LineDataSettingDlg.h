#if !defined(AFX_LINEDATASETTINGDLG_H__A8EC9C7C_2B10_44DE_9C63_8EB48FDA67DD__INCLUDED_)
#define AFX_LINEDATASETTINGDLG_H__A8EC9C7C_2B10_44DE_9C63_8EB48FDA67DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LineDataSettingDlg.h : header file
//
#include "AdminDoc.h"
#include <ListCtrlEx.h>
#include <BtnST.h>
#include "GridCtrl/GridCtrl.h"
#include "RevisionStageDlg.h"

class CTransparentCheckBox : public CButton
{
	DECLARE_DYNAMIC(CTransparentCheckBox)

public:
	CTransparentCheckBox();
	virtual ~CTransparentCheckBox();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

/////////////////////////////////////////////////////////////////////////////
// CLineDataSettingDlg dialog

class CLineDataSettingDlg : public CDialog
{
// Construction
public:
	bool CheckIssueDate( const CString& rIssueDate );
	void Save();
	void UpdateContents();
	CLineDataSettingDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLineDataSettingDlg)
	enum { IDD = IDD_LINE_DATA_SETTING };
		// NOTE: the ClassWizard will add data members here
	CRevisionStageDlg m_wndRevisionStageDlg;

	//CListCtrlEx m_LineDataModuleKeyListCtrl;
	CGridCtrl   m_issueDateGridCtrl;
	//}}AFX_DATA
	CTransparentCheckBox m_btnIssueDate1 , m_btnIssueDate2 , m_btnIssueDate3 , m_btnIssueDate4;	///2013.01.05 added by humkyung

	CMFCButton m_btnNew,m_btnRemove;
	
	CImageList m_ImageList;
	CAdminDoc* m_pAdminDoc;

	map<CGridCellBase* , CString> m_oPureIssueDateMap;	/// 2013.01.05 added by humkyung
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLineDataSettingDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CIssueDateRecordSet* m_pIssueDateRecordSet;
	// Generated message map functions
	//{{AFX_MSG(CLineDataSettingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	///afx_msg void OnDblclkListLineDataField(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonNew();
	afx_msg void OnButtonRemove();

	afx_msg void OnChangeIssueDateDelimiter();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void DisplayIssueDataRecordSet(const CString& sOldDelimiter = _T("") , const CString& sNewDelimiter = _T(""));
public:
	afx_msg void OnBnClickedRadioIssueDateFormatNo1();
	afx_msg void OnBnClickedRadioIssueDateFormatNo2();
	afx_msg void OnBnClickedRadioIssueDateFormatNo3();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnBnClickedRadioIssueDateFormatNo4();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINEDATASETTINGDLG_H__A8EC9C7C_2B10_44DE_9C63_8EB48FDA67DD__INCLUDED_)
