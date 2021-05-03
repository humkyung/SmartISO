#if !defined(AFX_CUSTOMDATAREPORTDLG_H__A8E692D6_40E8_47A2_84FA_B0E1A69C1747__INCLUDED_)
#define AFX_CUSTOMDATAREPORTDLG_H__A8E692D6_40E8_47A2_84FA_B0E1A69C1747__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LineListReportDlg.h : header file
//
#include <BtnST.h>
#include <ListCtrlEx.h>
#include <ado\AdoRemoteDB.h>
#include <LabelStatic.h>
#include "CheckTreeCtrl.h"
#include "PDSArea.h"
#include "LineListToOraDocData.h"

#include <vector>
using namespace std;
/////////////////////////////////////////////////////////////////////////////
// CCustomReportData dialog
class CCustomReportData
{
public:
	enum
	{
		AREA_NAME		= 0,
		MODEL_NO		= 1,
		NOMINAL_DIA		= 3,
		FLUID_CODE		= 6,
		INSULATION_PURPOSE	= 9,
		INSULATION_THICK	= 10
	};

	void InsertAt(const STRING_T &sFieldName , const STRING_T &sValue, CPDSOracle &PDSOracle);
	static CString GetQueryString(const CString& sProjectUnit , const CString& rProjectName , const CString& rTableName , 
		CCustomReportTable* pCustomReportTable);
	int GetColCount() const;
	CCustomReportData(){}
	CCustomReportData(const CCustomReportData& rhs);
	CCustomReportData& operator=(const CCustomReportData& rhs);
	virtual ~CCustomReportData(){}

	vector<STRING_T> m_vecData;
};

class CCustomDataReportDlg : public CDialog
{
// Construction
public:
	void UpdateReportData();
	CCustomDataReportDlg(CCustomReportTable* pCustomReportTable , CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLineListReportDlg)
	enum { IDD = IDD_CUSTOM_DATA_REPORT };
		// NOTE: the ClassWizard will add data members here
	CLabelStatic m_wndLabelStatic;

	CButtonST   m_wndQueryButton;
	CButtonST   m_wndExcelExportButton;
	
	CButtonST   m_wndSelectAllAreaNoButton;
	CCheckTreeCtrl m_wndCheckAreaTreeCtrl;
	CButtonST   m_wndRefreshButton;

	CListCtrlEx m_wndReportCtrl;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLineListReportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CCustomReportTable* m_pCustomReportTable;	//! 2011.05.04 - added by humkyung

	CBitmap m_bitmap; ///< 16 color이상의 bitmap을 읽기 위해
	CImageList m_imgTree , m_imgList , m_imgState;
	vector<CCustomReportData> m_CustomReportDataEntry;

	// Generated message map functions
	//{{AFX_MSG(CLineListReportDlg)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectAllArea();
	afx_msg void OnButtonExcelExport();
	afx_msg void OnButtonQuery();
	afx_msg void OnButtonRefresh();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	UINT StatusQueryThread();
	UINT StatusExportThread();
	static UINT StatusQueryThreadEntry(LPVOID pVoid);
	static UINT StatusExportThreadEntry(LPVOID pVoid);
	void CheckAllArea(HTREEITEM hItem);
	void SyncAreaDataWith(HTREEITEM hItem);
	BOOL IsSelected(const CString& rArea , const CString& rModel);

	list<STRING_T> m_lstHeaderString;
	vector<CPDSArea> m_AreaNameEntry;
	void SaveDataForReport();
public:
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUSTOMDATAREPORTDLG_H__A8E692D6_40E8_47A2_84FA_B0E1A69C1747__INCLUDED_)
