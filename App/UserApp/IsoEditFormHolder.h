#pragma once

#include "IsoEditViewLogFileForm.h"
#include "DgnScene.h"
#include "DgnCanvas.h"
#include "IsoEditForm.h"

// CIsoEditFormHolder form view

class CIsoEditFormHolder : public CFormView
{
	DECLARE_DYNCREATE(CIsoEditFormHolder)

protected:
	CIsoEditFormHolder();           // protected constructor used by dynamic creation
	
	CIsoEditViewLogFileForm* m_pViewLogFileForm;
	CSplitterWnd m_wndSplitter;
	CDgnScene m_oDgnScene;
	CDgnCanvas m_oDgnCanvas;
private:
	CFrameWnd* m_pMyFrame;
public:
	void InitContents();
	CIsoEditForm* GetIsoEditForm();
	int Generate();
	void SetViewLogFileForm(CIsoEditViewLogFileForm *p);

	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual ~CIsoEditFormHolder();

	enum { IDD = IDD_ISOEDITFORMHOLDER };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
};


