#pragma once

#include <BtnST.h>
#include "CustomDataReportDlg.h"
// COraDataReportView view

class COraDataReportView : public CView
{
	DECLARE_DYNCREATE(COraDataReportView)

public:
	COraDataReportView();           // protected constructor used by dynamic creation
	virtual ~COraDataReportView();

protected:
	CImageList m_imgList;

	CString m_rServiceName;
	CString m_rUserName;
	CString m_rUserPassword;
	CString m_rProjectName;	
	CString m_rStdNoteFilePath;

	CMFCTabCtrl m_wndTab;
public:
	int SyncTabCtrlWithCustomReportTableList();
	void LoadData();
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
private:
	int CreateCustomDataReportDialog(CCustomReportTable* pCustomReportTable);
};


