#if !defined(AFX_PICTUREWND_H__16A1B4A5_0A1D_4CE6_8EAA_57C37AAF0F55__INCLUDED_)
#define AFX_PICTUREWND_H__16A1B4A5_0A1D_4CE6_8EAA_57C37AAF0F55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PictureWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPictureWnd window

class CPictureWnd : public CWnd
{
// Construction
public:
	CPictureWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPictureWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	SIZE GetImgSize();
	int Load(LPCTSTR lpszPathName);
	virtual ~CPictureWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPictureWnd)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	SIZE m_sizeInPix;
	SIZE m_sizeInHiMetric;
	LPPICTURE m_pPicture;
private:
	void DeleteContents();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PICTUREWND_H__16A1B4A5_0A1D_4CE6_8EAA_57C37AAF0F55__INCLUDED_)
