#if !defined(AFX_LINELISTCONVERTDLG_H__C85FAC71_9729_4992_A241_4149B7AA1BA2__INCLUDED_)
#define AFX_LINELISTCONVERTDLG_H__C85FAC71_9729_4992_A241_4149B7AA1BA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LineListConvertDlg.h : header file
//
#include <FilterEdit/FilterEdit.h>
#include <XPStyleButtonST.h>
#include <ThemeHelperST.h>
#include <ListCtrlEx.h>

/////////////////////////////////////////////////////////////////////////////
// CLineListConvertDlg dialog
class CLineListConvertOutputForm;
class CLineListConvertDlg : public CDialog
{
// Construction
public:
	void LoadData();
	CLineListConvertDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLineListConvertDlg)
	enum { IDD = IDD_LINE_LIST_CONVERT_CTRL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CXPStyleButtonST m_selectExcelButton;
	CFilterEdit m_wndStartRowEdit;
	CXPStyleButtonST m_loadSheetNameButton;
	CListCtrlEx m_wndSheetNameListCtrl;
	CXPStyleButtonST m_selectAllButton;
	CXPStyleButtonST m_unselectAllButton;
	CComboBox  m_wndConvResultCol;
	/*CBCGButton*/CButton m_convertButton;
	CFont m_Font;
	CString m_rExcelFilePath;
	CString rMdbPath;
	CString rTempPath;
	//CResizeWnd m_resizeWnd;
	CString S_ExPath;

	CLineListConvertOutputForm* m_pOutputForm;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLineListConvertDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CThemeHelperST m_themeHelperST;
	
	CFont m_16UnderlineFont;
	int m_nConvertResCode;

	static UINT StatusThreadEntry(LPVOID pVoid);
	UINT StatusThread();
	// Generated message map functions
	//{{AFX_MSG(CLineListConvertDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonLineListConvert();
	afx_msg void OnBUTTONSelectExcel();
	afx_msg void OnButtonLoadSheetName();
	afx_msg void OnButtonSelectAll();
	afx_msg void OnButtonUnselectAll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	static const STRING_T APP;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINELISTCONVERTDLG_H__C85FAC71_9729_4992_A241_4149B7AA1BA2__INCLUDED_)
