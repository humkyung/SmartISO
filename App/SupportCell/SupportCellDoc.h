// SupportCellDoc.h : interface of the CSupportCellDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUPPORTCELLDOC_H__2BC2160F_5342_45D0_8505_522892363CDC__INCLUDED_)
#define AFX_SUPPORTCELLDOC_H__2BC2160F_5342_45D0_8505_522892363CDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSupportCellDoc : public CDocument
{
protected: // create from serialization only
	CSupportCellDoc();
	DECLARE_DYNCREATE(CSupportCellDoc)

// Attributes
public:
	CString m_FileName;

// Operations
public:
	void ExtractCell();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSupportCellDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSupportCellDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSupportCellDoc)
	afx_msg void OnFileOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SUPPORTCELLDOC_H__2BC2160F_5342_45D0_8505_522892363CDC__INCLUDED_)
