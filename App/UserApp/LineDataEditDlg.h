#if !defined(AFX_LINELISTCONVERTDLG_H__C85FAC71_9729_4992_A241_4149B7AA1BA2__INCLUDED_)
#define AFX_LINELISTCONVERTDLG_H__C85FAC71_9729_4992_A241_4149B7AA1BA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LineListConvertDlg.h : header file
//
#include <GradientStatic.h>
#include <FilterEdit/FilterEdit.h>
#include <XPStyleButtonST.h>
#include <ThemeHelperST.h>
#include <ListCtrlEx.h>
#include <ResizeWnd.h>
#include "GridCtrl/GridCtrl.h"

#include "UserAppDoc.h"
#include "EditLineDataForm.h"
/////////////////////////////////////////////////////////////////////////////
// CLineDataEditDlg dialog

struct UserKeyInData
{
	CString Desc;
	CString Value;
};

class CLineListConvertOutputForm;
class CLineDataEditDlg : public CFormView
{
protected:
	DECLARE_DYNCREATE(CLineDataEditDlg)
// Construction
public:
	CLineDataEditDlg();   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLineDataEditDlg)
	enum { IDD = IDD_LINE_DATA_EDIT };
		// NOTE: the ClassWizard will add data members here
	CGridCtrl m_wndRevDataGrid;
	IsGui::CListCtrlEx m_wndIssueDateListCtrl;
	//}}AFX_DATA
	CUserAppDoc* m_pUserAppDoc;

	void InitContents();
	void DeleteAllQualifiedLineList( );
	void ShowSelectedCount(int nCount);
	void ClearContents();
	void AddLineDataRecordEntry(vector<LineDataRecordMap>* pLineDataRecordEntry);
	void DeleteCheckedQualifiedLineList( );
	void DeleteLineData() ;
	void GetAllLineDataRecord(vector<LineDataRecordMap>& lineDataRecordEntry);
	void DisplayRevisionDataRecordSet(CLineDataRecord* pRecord);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLineDataEditDlg)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	bool AddRevDataToLineDataRecord(CLineDataRecord *pLineDataRecord, const CString &rIssueDate, CString& rLog);
// Implementation
protected:
	CResizeWnd  m_wndResizeCtrl;	/// 2012.08.28 added by humkyung
	CImageList m_ImageList;
	CEditLineDataForm* m_pLineDataForm;
	CFont* m_pBoldFont;
	
	map<int , int> m_ItemMap;		/// line data form item -> qualified form item
	map<int , int> m_ReverseItemMap;	/// qualified form item -> line data form item
	
	vector< STRING_T > m_RevFieldNameEntry;
	vector< STRING_T > m_RevFieldDescEntry;
	vector<UserKeyInData> m_RevKeyInDataEntry;

	static UINT StatusThreadEntry(LPVOID pVoid);
	UINT StatusThread(const CString& sIssueDate , const CString& sControlDate);
	void UpdateLineDataRecord(const int &nItem, CLineDataRecord *pLineDataRecord);
	// Generated message map functions
	//{{AFX_MSG(CLineDataEditDlg)
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//afx_msg void OnClkListGradeDefine(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListIssueDate(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateDeleteIssue(CCmdUI* pCmdUI);
	afx_msg void OnDeleteIssue();
	afx_msg void OnVertResize();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int DetermineLayout(const int& cx , const int& cy);	/// 2012.08.28 added by humkyung
	int CheckSelectedGrade(const int& nSelGrade);
public:
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnLvnItemchangedListIssueDate(NMHDR *pNMHDR, LRESULT *pResult);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINELISTCONVERTDLG_H__C85FAC71_9729_4992_A241_4149B7AA1BA2__INCLUDED_)
