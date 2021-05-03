// DrawingReportSettingDlg.h: interface for the CDrawingReportSettingDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWINGREPORTSETTINGDLG_H__5C13B872_F1B6_4DDD_A45A_09C9EFE1027A__INCLUDED_)
#define AFX_DRAWINGREPORTSETTINGDLG_H__5C13B872_F1B6_4DDD_A45A_09C9EFE1027A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AdminDoc.h"
#include "GridCtrl/GridCtrl.h"
#include "PictureWnd.h"
#include <gui/ListCtrlEx.h>

#define	DATA_NAME_LIST_WIDTH	300

class CDrawingReportFileBrowseEdit : public CMFCEditBrowseCtrl
{
        virtual void OnBrowse();
public:
        CString m_sDrawingReportFile;
};

class CDrawingReportSettingDlg  : public CDialog
{
public:
	CDrawingReportSettingDlg(CWnd* pParent = NULL);
	~CDrawingReportSettingDlg();

	void Save();
	void UpdateContents();
	int FindGridColumn(const string& rLabel);
	// Dialog Data
	//{{AFX_DATA(CBorderSettingDlg)
	enum { IDD = IDD_DRAWING_REPORT };
	// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CAdminDoc* m_pAdminDoc;
	CString m_sDwgReportTemplateFilePath;	/// 2012.08.15 added by humkyung
	CDrawingReportFileBrowseEdit m_wndDrawingReportFileBrowser;	/// 2012.08.15 added by humkyung
	CButton m_ExlSelectButton;
	CComboBox m_cboNextColumn,m_cboStartRow;
	CGridCtrl m_wndDrawingReportCtrl;
	CFont m_Font;
	CButton m_wndCheckRevData;
	
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	CImageList m_ImageList;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//{{AFX_VIRTUAL(CFileNameFormatDlg)
	//////////////////////////////////////////////////////////////////////////
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEnChangeEditDwgReportTemplate();
	DECLARE_MESSAGE_MAP()
	//}}AFX_VIRTUAL
private:
	CToolTipCtrl* m_pTT;
	vector< STRING_T > m_DataNameEntry;
public:
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
private:
	int SaveExcelData(const CString& sIniFilePath);	/// 2012.08.15 added by humkyung
public:
	afx_msg void OnCbnSelendokComboStartRow();
};

#endif // !defined(AFX_DRAWINGREPORTSETTINGDLG_H__5C13B872_F1B6_4DDD_A45A_09C9EFE1027A__INCLUDED_)
