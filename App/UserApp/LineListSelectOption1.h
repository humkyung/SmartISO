#if !defined(AFX_LINELISTSELECTOPTION1_H__4577F61D_3DF1_4C3F_8B97_37230F2438C7__INCLUDED_)
#define AFX_LINELISTSELECTOPTION1_H__4577F61D_3DF1_4C3F_8B97_37230F2438C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LineListSelectOption1.h : header file
//
#include <GradientStatic.h>
#include <PropTree/PropTree.h>
#include <BtnST.h>
#include "UserAppDoc.h"
#include <ListCtrlEx.h>
#include <ListBoxST.h>
#include "SelectionTreeListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CLineListSelectOption1 dialog

class CLineListSelectOption1 : public CDialog
{
// Construction
public:
	static void UpdateRevNoComboBox(bool bReload = true , CLineListSelectOption1* pTarget = NULL);
	static void UpdateSelectionTreeList(bool bRealod = true , CLineListSelectOption1* pTarget = NULL);

	CString GetInAreaQueryCondition() const;
	void SetEnableSelectButton();
	CString GetSelectedRevNo() const;
	void GetSelectedIssueDate( vector< string >& issueDates );
	CString GetQueryCondition() const;
	void InitContents( CWnd* pTargetWnd );
	CLineListSelectOption1(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLineListSelectOption1();
// Dialog Data
	//{{AFX_DATA(CLineListSelectOption1)
	enum { IDD = IDD_LINE_LIST_SELECT_OPTION1 };
		// NOTE: the ClassWizard will add data members here	
	CSelectionTreeListCtrl m_cSelectionTreeList;
	CImageList m_cImages;
	
	BOOL m_bPlantListBox;
	BOOL m_bAreaListBox;
	BOOL m_bUnitListBox;
	BOOL m_bSectionListBox;
	BOOL m_bSelectBoutton;
	//}}AFX_DATA
	CGradientStatic m_TitleStaticCtrl;

	static const int WIDTH;	/// ���� ��
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLineListSelectOption1)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	
// Implementation
protected:
	CWnd* m_pTargetWnd;
	CImageList m_imgList;
	CFont* m_pBoldFont;

	IsGui::CListCtrlEx m_wndIssueDateReport;
	CMFCButton m_btnSelectAll;
	CMFCButton m_btnSelect;

	CString m_rQueryCondition;
	map<CString , CString> m_LineDataFieldMap;
	vector<CString> m_WBSStringVector;
	// Generated message map functions
	//{{AFX_MSG(CLineListSelectOption1)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSelectAll();
	afx_msg void OnCheckIssueDate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnButtonSelect();
	afx_msg void OnDblclkListPlant();
	afx_msg void OnDblclkListArea();
	afx_msg void OnDblclkListUnit();
	afx_msg void OnDblclkListSection();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void OnDblclkQueryItem(CListBoxST& listbox , const int& nSel);
	bool CheckIssueDateToQuery() const;
	void GetSelSectionNos( vector< STRING_T >& oSectionNos , const STRING_T& sPlantNo , const STRING_T& sAreaNo , const STRING_T& sUnitNo );
	void GetSelUnitNos( vector< STRING_T >& oUnitNos , const STRING_T& sPlantNo , const STRING_T& sAreaNo );
	void GetSelAreaNos( vector< STRING_T >& oAreaNos , const STRING_T& sPlantNo );
	void GetSelPlantNos( vector< STRING_T >& oPlantNos );

	void OnUpdateRevNoComboBox(bool bReload);
	void OnUpdateSelectionTreeList(bool bReload);

	bool AreAllItemsChecked(HTREEITEM hItem);
	int GetCheckedItemList( vector< STRING_T >& oCheckedList , HTREEITEM hItem);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINELISTSELECTOPTION1_H__4577F61D_3DF1_4C3F_8B97_37230F2438C7__INCLUDED_)
