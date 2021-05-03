#if !defined(AFX_BackupDataFORM_H__5E3BFF28_9F91_43F8_9021_852D8ACD5D71__INCLUDED_)
#define AFX_BackupDataFORM_H__5E3BFF28_9F91_43F8_9021_852D8ACD5D71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BackupDataForm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBackupDataForm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include <FilterEdit/FilterEdit.h>
#include <BtnST.h>
#include <XPStyleButtonST.h>
#include <ThemeHelperST.h>
#include <ResizeWnd.h>
#include "FileListCtrl.h"
#include <ListCtrlEx.h>
#include "LabelStatic.h"
#include "LineDataEditDlg.h"
#include "SelectionTreeListCtrl.h"

class CBackupDataOutputForm;
class CBackupDataForm : public CFormView
{
	enum
	{
		FILE_NAME_COL	= 0,
		MSTN_COL		= 10,
		IDF_COL			= 11
	};
	enum
	{
		ROOT_ICON	= 3,
		ISO_DWG_ICON= 5,
		MSTN_ICON	= 6
	};
protected:
	DECLARE_DYNCREATE(CBackupDataForm)

// Form Data
public:
	//{{AFX_DATA(CBackupDataForm)
	enum { IDD = IDD_BACKUP_DATA };
	CSelectionTreeListCtrl m_wndTreeList;

	//}}AFX_DATA

// Attributes
public:
	CBackupDataOutputForm* m_pOutputForm;
// Operations
public:
	void SetOutputForm(CBackupDataOutputForm* p);
	void InitContents();
	CBackupDataForm();           // protected constructor used by dynamic creation
	virtual ~CBackupDataForm();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBackupDataForm)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CImageList m_imageList;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CBackupDataForm)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRclickTreeList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnProperties();	/// 2015.12.05 added by HumKyung
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int OpenIsoDwgFile(const CString&,const CString&,const CString&) const;
	void LoadLineDataRecordSetFromDatabase(const CString&);
	list<CLineDataRecord*> m_oLineDataRecordList;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BackupDataFORM_H__5E3BFF28_9F91_43F8_9021_852D8ACD5D71__INCLUDED_)
