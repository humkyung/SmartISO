#pragma once

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BackupDataOuputForm.h : header file
//
#include "UserAppDoc.h"
#include <GradientStatic.h>
#include <BtnST.h>
#include "GridCtrl/GridCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CBackupDataOutputForm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CBackupDataForm;
class CBackupDataOutputForm : public CFormView
{
protected:
	
	DECLARE_DYNCREATE(CBackupDataOutputForm)

// Form Data
public:
	//{{AFX_DATA(CBackupDataOutputForm)
	enum { IDD = IDD_BACKUP_OUTPUT_FORM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
// Attributes
public:
	CUserAppDoc* m_pUserAppDoc;
	CGridCtrl m_wndRevDataGrid;
// Operations
public:
	void DisplayRevisionDataRecordSet(CLineDataRecord* pRecord);
	void InitContents();
	CBackupDataOutputForm();           // protected constructor used by dynamic creation
	virtual ~CBackupDataOutputForm();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBackupDataOutputForm)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CFont* m_pBoldFont;
	CBackupDataForm* m_pBackupDataForm;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	// Generated message map functions
	//{{AFX_MSG(CBackupDataOutputForm)
		// NOTE - the ClassWizard will add and remove member functions here.
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
