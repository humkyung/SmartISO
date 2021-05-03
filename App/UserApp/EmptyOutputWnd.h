#if !defined(AFX_WRITEPDS_H__202FED34_35B6_40FE_99F4_492D19EB9F4F__INCLUDED_)
#define AFX_WRITEPDS_H__202FED34_35B6_40FE_99F4_492D19EB9F4F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WritePds.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEmptyOutputWnd form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include <GradientStatic.h>
#include <ListCtrlEx.h>
#include "EditLineDataForm.h"
#include "InsertToPDSAttribute.h"
#include "UserAppDoc.h"
#include <map>

using namespace std;

class CEmptyOutputWnd : public CFormView
{
protected:
	DECLARE_DYNCREATE(CEmptyOutputWnd)

// Form Data
public:
	///CGradientStatic m_WritePdsStatic;
	CUserAppDoc* m_pUserAppDoc;
	//{{AFX_DATA(CEmptyOutputWnd)
	enum { IDD = IDD_WRITE_PDS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
// Attributes
public:
// Operations
public:
	CEmptyOutputWnd();	           
	virtual ~CEmptyOutputWnd();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEmptyOutputWnd)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
// Implementation
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CEmptyOutputWnd)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WRITEPDS_H__202FED34_35B6_40FE_99F4_492D19EB9F4F__INCLUDED_)
