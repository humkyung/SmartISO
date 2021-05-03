#pragma once


// CQueryClauseListDlg dialog

class CQueryClauseListDlg : public CDialog
{
	DECLARE_DYNAMIC(CQueryClauseListDlg)

public:
	CQueryClauseListDlg(CString sQueryClause , CWnd* pParent = NULL);   // standard constructor
	virtual ~CQueryClauseListDlg();

// Dialog Data
	enum { IDD = IDD_QUERY_CLAUSE_LIST };
	CMFCListCtrl m_wndQueryClauseListCtrl;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
private:
	CString m_sQueryClause;
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnLvnKeydownListQueryClause(NMHDR *pNMHDR, LRESULT *pResult);
};
