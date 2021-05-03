// UserAppDoc.h : interface of the CUserAppDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_USERAPPDOC_H__F4D26E08_F0A4_4862_A5C6_A3D5DC33097B__INCLUDED_)
#define AFX_USERAPPDOC_H__F4D26E08_F0A4_4862_A5C6_A3D5DC33097B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Project.h>

class CUserAppDoc : public CDocument
{
protected: // create from serialization only
	CUserAppDoc();
	DECLARE_DYNCREATE(CUserAppDoc)

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUserAppDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	static CUserAppDoc* GetInstance();	/// 2011.10.27 added by humkyung

	void SetTitle(LPCTSTR lpszTitle);
	virtual ~CUserAppDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	BOOL m_bRevAutomation;		/// 2012.04.06 added by humkyung
	BOOL m_bIssueAutomation;	/// 2012.05.10 added by humkyung
	BOOL m_bUseIPLOT;			/// 2015.09.03 added by humkyung
protected:
	//CString m_rIniFilePath;
// Generated message map functions
protected:
	//{{AFX_MSG(CUserAppDoc)
	afx_msg void OnAppClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int AddNewIsoDwgDataToDatabase(const CString& rIniFilePath);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERAPPDOC_H__F4D26E08_F0A4_4862_A5C6_A3D5DC33097B__INCLUDED_)
