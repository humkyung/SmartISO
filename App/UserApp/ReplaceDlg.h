#pragma once


// CReplaceDlg dialog

class CReplaceDlg : public CDialog
{
	DECLARE_DYNAMIC(CReplaceDlg)

public:
	CReplaceDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CReplaceDlg();

// Dialog Data
	enum { IDD = IDD_REPLACE };

	CString m_sFind , m_sReplace;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
