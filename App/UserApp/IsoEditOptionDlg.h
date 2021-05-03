#if !defined(AFX_ISOEDITOPTIONDLG_H__A387A98D_2A6E_4E8A_9DF0_F7F4DAA8BEE5__INCLUDED_)
#define AFX_ISOEDITOPTIONDLG_H__A387A98D_2A6E_4E8A_9DF0_F7F4DAA8BEE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IsoEditOptionDlg.h : header file
//
#include <GradientStatic.h>
#include <BtnST.h>
#include "UserAppDoc.h"
/////////////////////////////////////////////////////////////////////////////
// CIsoEditOptionDlg dialog

typedef enum
{
	SELECT_SOURCE_ISO_DWG_FOLDER	= WM_USER + 110,
	SELECT_EDITED_ISO_DWG_FOLDER	= WM_USER + 111,
	LOG_FILE_SYNCHRONIZING		= WM_USER + 112,
	SAVE_LOG_FILE				= WM_USER + 114,
	REMOVE_TEMP_FILE			= WM_USER + 115,
	SHOW_EXISTING_FILE			= WM_USER + 116,
	QUALITY_CONTROL_CHECK		= WM_USER + 117,
	BOM_OVER_CHECK				= WM_USER + 118,
	SPEC_BREAK_HOLD_CHECK		= WM_USER + 119
}IsoEditOptionMsg;

#define SOURCE	0x00
#define EDITED	0x01

class CIsoEditOptionDlg : public CDialog
{
// Construction
public:
	BOOL GetOptionCheck( const IsoEditOptionMsg& option );
	void LoadData( const CString& rIniFilePath);
	void SaveData( const CString& rIniFilePath );
	CIsoEditOptionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CIsoEditOptionDlg();
// Dialog Data
	//{{AFX_DATA(CIsoEditOptionDlg)
	enum { IDD = IDD_ISO_EDIT_OPTION };
		// NOTE: the ClassWizard will add data members here
	BOOL m_bSelectSourceIsoDwgFileFolder , m_bSelectEditedIsoDwgFileFolder;
	BOOL m_bLOG_FILE_SYNCHRONIZING , m_bSAVE_LOG_FILE;
	BOOL m_bREMOVE_TEMP_FILE , m_bSHOW_EXISTING_FILE;
	BOOL m_bQUALITY_CONTROL_CHECK/* , m_bPIPE_SUPPORT_CHECK , m_bSPEC_BREAK_HOLD_CHECK*/;

	CString m_rIniFilePath;
	static CString APP;
	//}}AFX_DATA
	CGradientStatic m_TitleStaticCtrl;
	CButtonST m_StartButton;

	CWnd* m_pTargetWnd;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIsoEditOptionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CFont* m_pBoldFont;
	// Generated message map functions
	//{{AFX_MSG(CIsoEditOptionDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckPlottingAfterEditing();
	afx_msg void OnRadioSource();
	afx_msg void OnRadioEdited();
	afx_msg void OnCheckQualityControlCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ISOEDITOPTIONDLG_H__A387A98D_2A6E_4E8A_9DF0_F7F4DAA8BEE5__INCLUDED_)
