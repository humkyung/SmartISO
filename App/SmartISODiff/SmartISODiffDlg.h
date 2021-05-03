#pragma once


// CSmartISODiffDlg dialog

class CSmartISODiffDlg : public CDialog
{
	DECLARE_DYNCREATE(CSmartISODiffDlg)

public:
	CSmartISODiffDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSmartISODiffDlg();

// Dialog Data
	enum { IDD = IDD_SMART_ISO_DIFF_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
