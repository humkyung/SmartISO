// ogl2dTestDoc.h : interface of the COgl2dTestDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGL2DTESTDOC_H__B07D79C1_CD07_42F5_BB34_CA2792A4755C__INCLUDED_)
#define AFX_OGL2DTESTDOC_H__B07D79C1_CD07_42F5_BB34_CA2792A4755C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class COgl2dTestDoc : public CDocument
{
protected: // create from serialization only
	COgl2dTestDoc();
	DECLARE_DYNCREATE(COgl2dTestDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COgl2dTestDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COgl2dTestDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(COgl2dTestDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OGL2DTESTDOC_H__B07D79C1_CD07_42F5_BB34_CA2792A4755C__INCLUDED_)
