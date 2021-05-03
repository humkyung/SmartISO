#if !defined(AFX_LINELISTTOORADLG_H__FA2D87CF_0D3E_4CD9_9E90_AEA288618F59__INCLUDED_)
#define AFX_LINELISTTOORADLG_H__FA2D87CF_0D3E_4CD9_9E90_AEA288618F59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LineListToOraDlg.h : header file
//
#include <ListCtrlEx.h>
#include "GridCtrl/GridCtrl.h"
#include "..\UserDLL\OraDatabaseDefFile.h"
/////////////////////////////////////////////////////////////////////////////
// CLineListToOraDlg dialog

class CLineListToOraDlg : public CDialog
{
// Construction
public:
	void LoadData();
	void SaveData();
	CLineListToOraDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLineListToOraDlg)
	enum { IDD = IDD_LINE_LIST_TO_ORA };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	CEdit m_wndStdNoteFilePath;
	CButton m_wndSelectStdNoteFile;
	CEdit m_wndDesignDDLFilePath;
	CButton m_wndSelectDesignDDLFile;
	///CComboBox m_wndKeyComboBox;
	CListCtrlEx m_wndKeyListCtrl;
	CGridCtrl m_wndGridCtrl;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLineListToOraDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	COraDatabaseDefFile m_OracleDefFile;
	// Generated message map functions
	//{{AFX_MSG(CLineListToOraDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonDesignDdl();
	afx_msg void OnSelendokComboKey();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSelchangeComboKey();
	afx_msg void OnButtonStdNote();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void FillKeyListCtrl(const bool& NewData);
	void InitKeyListCtrl();
	void UpdateGridCell();
	void InitGridCtrl();

	static const STRING_T APP;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINELISTTOORADLG_H__FA2D87CF_0D3E_4CD9_9E90_AEA288618F59__INCLUDED_)
