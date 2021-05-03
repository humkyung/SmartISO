#pragma once

#include "LineListSelectOption1.h"
#include "IssueLineDataSelectOption.h"
#include "IsoEditOptionDlg.h"
#include "IsoEditOptionDlg2.h"
#include "BMReportOptionDlg.h"
#include "SelectIssueDataDlg.h"
// CWorkspaceWnd

class CWorkspaceWnd : public CDockablePane
{
	DECLARE_DYNAMIC(CWorkspaceWnd)

public:
	CWorkspaceWnd();
	virtual ~CWorkspaceWnd();

	void InitContents();

	void ShowLineDataForm(const bool& bShow);
	void ShowIsoEditForm(const bool& bShow);
	void ShowBOMReportForm(const bool& bShow);
	void ShowIndexReportForm(const bool& bShow);
	void ShowDrawingPlottingForm(const bool& bShow);
	void ShowUpdatePDSForm(const bool& bShow);
	void ShowBackupForm(const bool& bShow);
public:
	/// backup form
	CIssueLineDataSelectOption m_wndQueryBackup;
	CMFCTabCtrl m_wndBackupTabCtrl;

	/// line data form
	CLineListSelectOption1 m_wndQuery1;
	CMFCTabCtrl m_wndQueryTabCtrl;

	/// iso edit form
	CLineListSelectOption1 m_wndQuery2;
	CIsoEditOptionDlg m_IsoEditOptionDlg;
	CIsoEditOptionDlg2 m_IsoEditOptionDlg2;
	CMFCTabCtrl m_wndIsoEditTabCtrl;

	/// Drawing index form
	CLineListSelectOption1 m_wndQuery3;
	CMFCTabCtrl m_wndDrawingIndexTabCtrl;

	/// BM Report form
	CLineListSelectOption1 m_wndQuery4;
	CBMReportOptionDlg m_BMReportOptionDlg;
	CMFCTabCtrl m_wndBMReportTabCtrl;

	/// Drawing plotting form
	CSelectIssueDataDlg m_wndSelectIssueDate;
	CMFCTabCtrl m_wndDrawingPlottingTabCtrl;

	/// for PDS update form - 2012.05.08 added by humkyung
	CLineListSelectOption1 m_wndQuery5;
	CMFCTabCtrl m_wndPDSUpdateTabCtrl;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
