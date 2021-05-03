// AdminDoc.h : interface of the CAdminDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADMINDOC_H__1DF30CF7_B21A_4AE2_AC49_2261F3A5E776__INCLUDED_)
#define AFX_ADMINDOC_H__1DF30CF7_B21A_4AE2_AC49_2261F3A5E776__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Project.h>

typedef enum
{
	CLEAR_MODIFIED = 0x00,
	RESERVED_DATA_TABLE_MODIFIED = 0x01,
	REVISION_DATA_TABLE_MODIFIED = 0x02
}ModifyFlag;

class CAdminDoc : public CDocument
{
protected: // create from serialization only
	CAdminDoc();
	DECLARE_DYNCREATE(CAdminDoc)

// Attributes
public:
	CString m_rIniFilePath;
	CString m_ProjectName;
	CString m_ProjectPath;

	int m_modifyFlag;
// Operations
public:
	//CSystemDb* m_pSystemDb;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdminDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL
// Implementation
public:
	static CAdminDoc* GetInstance();	/// 2012.01.27 added by humkyung

	void SetTitle(LPCTSTR lpszTitle);
	CProject* GetCurProject();
	virtual ~CAdminDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CProject* m_pCurProject;
// Generated message map functions
protected:
	//{{AFX_MSG(CAdminDoc)
	afx_msg void OnAppClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADMINDOC_H__1DF30CF7_B21A_4AE2_AC49_2261F3A5E776__INCLUDED_)
