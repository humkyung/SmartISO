#if !defined(AFX_PROJECTFORMDLG_H__59CB1EC2_FF6C_4A39_9E40_300F09E67894__INCLUDED_)
#define AFX_PROJECTFORMDLG_H__59CB1EC2_FF6C_4A39_9E40_300F09E67894__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProjectFormDlg.h : header file
//
#include "StdAfx.h"

#include <afxdisp.h>
#include <atlbase.h>
#include <mscoree.h>
#include <comutil.h>

#include <ado/SQLFile.h>
/////////////////////////////////////////////////////////////////////////////
// CProjectFormDlg form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include <ListCtrlEx.h>
#include <BtnST.h>
#include <FilterEdit/FilterEdit.h>
#include "LabelStatic.h"
#include "..\Module\AppPostgreSQL.h"
#include "resource.h"
#include <WinXPButtonST.h>

#include "MFCButtonEx.h"

//#import <mscorlib.tlb> auto_rename raw_interfaces_only
//using namespace mscorlib;

class CProjectDlg : public CDialog
{
protected:
	
	DECLARE_DYNCREATE(CProjectDlg)

// Form Data
public:
	typedef enum eUserType
	{
		eAdmin = 0x01,
		eUser  = 0x02
	};
	typedef struct
	{
		CString  Name;
		STRING_T desc;		/// 2013.01.05 added by humkyung
		CString  path;
		STRING_T pds_name;	/// 2011.01.24 - added by HumKyung
		STRING_T project_no;
		STRING_T job_no;
		STRING_T client_name;
		STRING_T comment;

		/// 2015.08.24 added by humkyung
		enum eUserType UserType;
		CString UserName , UserPassword;
		/// up to here
		CString sHost , sUserName, sPassword , sServiceName , sPDSProjectName;	/// 2012.05.11 added by humkyung
		CString sDate;	/// 2012.08.22 added by humkyung
	}ProjectInfo;

	//{{AFX_DATA(CProjectFormDlg)
	enum { IDD = IDD_PROJECT };
		// NOTE: the ClassWizard will add data members here
	CFilterEdit m_NameEdit;			/// project name edit
	CEdit       m_wndProjectFolder;	/// project folder edit
	CFilterEdit m_CommentEdit;		/// project comment edit
	CFilterEdit m_projectNoEdit;	/// project no edit
	CEdit       m_wndClientName;	/// client name edit

	CButtonST m_SelectLeButton;
	CMFCButtonEx m_btnCreateProject;
	CMFCButtonEx m_btnDeleteProject;

	IsGui::CListCtrlEx m_ProjectReportCtrl;
	//}}AFX_DATA

// Attributes
public:
	CFont m_font;
	CString m_rProjectLeId;
	CImageList m_ImageList;
private:
	//CComPtr<ICorRuntimeHost>	m_pRuntimeHost;
	//CComPtr<_AppDomain>			m_pDefAppDomain;
// Operations
public:
	static int CreateProjectIfNeed(const CString& sPrjName);

	void UpdateContents();
	CProjectDlg();           // protected constructor used by dynamic creation
	virtual ~CProjectDlg();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProjectDlg)
	public:
	virtual BOOL OnInitDialog();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CImageList m_headerImgList;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CProjectFormDlg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonDel();
	afx_msg void OnUpdateButtonDel(CCmdUI* pCmdUI);
	afx_msg void OnAppClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	static int SyncTableToTableDefFile(CAppPostgreSQL& database);
	void ClearPropertyField();
	list<ProjectInfo> m_ProjectInfoEntry;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnClose();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROJECTFORMDLG_H__59CB1EC2_FF6C_4A39_9E40_300F09E67894__INCLUDED_)
