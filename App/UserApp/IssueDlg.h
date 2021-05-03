#pragma once

#include <ListCtrlEx.h>
#include "resource.h"

// CIssueDlg dialog

class CIssueDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CIssueDlg)

public:
	CIssueDlg(const CString& sOutputDrawingFolder,vector<LineDataRecordMap>&,CWnd* pParent = NULL);   // standard constructor
	virtual ~CIssueDlg();

// Dialog Data
	enum { IDD = IDD_ISSUE };

	IsGui::CListCtrlEx m_wndLineDataReport;
	CMFCButton m_btnExport,m_btnOK,m_btnCancel;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
protected:
	CString m_sOutputDrawingFolder;
	CImageList m_imgList;
private:
	vector<LineDataRecordMap> m_oLineDataRecordEntry;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnUpdateButtonExport(CCmdUI* pCmdUI);
	afx_msg void OnBnClickedButtonExport();
private:
	void GetSelectedLineDataRecord(vector<LineDataRecordMap>& aLineDataRecordEntry);
};
