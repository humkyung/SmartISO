#pragma once

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditLineDataForm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditLineDataForm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include <LineDataRecordSet.h>
#include <ListCtrlEx.h>
#include <XButtonXP/XButtonXP.h>

#include "LineListSelectOption1.h"
#include "LineListSelectOption2.h"
#include "InplaceEdit.h"

/// 2013.02.03 added by humkyung
class CEditLineDataListCtrl : public IsGui::CListCtrlEx
{
public:
	CEditLineDataListCtrl(){}
	~CEditLineDataListCtrl(){}
protected:
	BOOL DisplayEditor(int nItem, int nSubItem);
};
/// up to here

class CLineDataEditDlg;
class CEditLineDataForm : public CFormView
{
protected:
	
	DECLARE_DYNCREATE(CEditLineDataForm)
	
	typedef struct
	{
		CString rFilePath;
		map<CString,CString> AreaDataMap;
	}IsoFileAreaData;
// Form Data
public:
	//{{AFX_DATA(CEditLineDataForm)
	enum { IDD = IDD_LINE_LIST_FORM };
		// NOTE: the ClassWizard will add data members here
	CEditLineDataListCtrl m_wndLineDataReport;
	CGradientStatic m_cStaticLabel;
	
	CInPlaceEdit* m_pWndInPlaceEdit;
	CComboBox* m_pWndInPlaceComboBox;
	//}}AFX_DATA
// Attributes
public:
	CLineDataEditDlg* m_pQualifiedLineForm;

	CEditLineDataForm();           // protected constructor used by dynamic creation
	virtual ~CEditLineDataForm();
// Operations
public:
	int SavePartDataOf(const STRING_T& sName , const STRING_T& sValue , list<CLineDataRecord*>& oLineDataList);	/// 2013.02.07 added by humkyung
	int DeleteLineDataRecordEntry(vector<LineDataRecordMap>* pLineDataRecordEntry);
	void PrintLineDataRecordEntry(vector<LineDataRecordMap>* pLineDataRecordEntry);
	void InitContents();
	void LoadLineDataRecordSetFromDatabase(const CString& rQueryCondition , const vector< STRING_T >& issueDates , const bool& reload=false);
	int UpdateLineDataRecord(const int& nItem , CLineDataRecord* pLineDataRecord);
	void GetSelectedLineDataRecord(vector<LineDataRecordMap>& LineDataRecordEntry);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditLineDataForm)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL
public:
	void ShowRecordCount(int nRecordCount);
	vector<string> m_sortDescEntry;
	CString m_rMStationPath,m_rIniFilePath;
	int m_nRecordCount;
// Implementation
protected:
	CString m_sSourceIsoFileFolder;
	CLineDataRecordSet* m_pLineDataRecordSet;
	CImageList m_CheckImageList;

	CListCtrl*	m_pDragList;	//Which ListCtrl we are dragging FROM
	CListCtrl*	m_pDropList;	//Which ListCtrl we are dropping ON
	CImageList*	m_pDragImage;	//For creating and managing the drag-image
	BOOL		m_bDragging;	//T during a drag operation
	int		m_nDragIndex;	//Index of selected item in the List we are dragging FROM
	int		m_nDropIndex;	//Index at which to drop item in the List we are dropping ON
	CWnd*		m_pDropWnd;	//Pointer to window we are dropping on (will be cast to CListCtrl* type)
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CEditLineDataForm)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDblclkListLineList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickLineList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateLineDataModify(CCmdUI* pCmdUI);
	afx_msg void OnButtonSort();
	afx_msg void OnButtonUp();
	afx_msg void OnButtonDown();
	afx_msg void OnButtonDelete();
	afx_msg void OnUpdateLineDataDelete(CCmdUI* pCmdUI);
	afx_msg void OnLineDataDelete();
	afx_msg void OnButtonIsoFolder();
	afx_msg void OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnProperties();	/// 2011.02.24 added by HumKyung
	afx_msg void OnReplace();		/// 2013.08.24 added by HumKyung
	afx_msg void OnUpdateIsoFolder(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	int DeleteProjectContents();	/// 2012.08.26 added by humkyung
	int ReadRISResultFile(const CString& sFilePath);
	int RunRIS();
	STRING_T m_rRisPath;
	STRING_T m_rExeFilePath;
	STRING_T m_rInstallPath;
	STRING_T m_rProjectFile;
	STRING_T m_rProjectPath;
	CString m_sCurrQueryCluase;	/// 2012.08.13 added by humkyung

	void QualifyLineData();
	void DropItemOnList(CListCtrl* pDragList, CListCtrl* pDropList);
	void DisplayLineDataRecordSet();

	void ReadIsoFiles(IsoFileAreaData& iso_file_key_data);

	static UINT StatusThreadEntry(LPVOID pVoid);
	UINT StatusThread();
	CString GetPlanNOFromIDF(OFSTREAM_T& ofile , int& iLogIndex , const CString& sIsoFilePath);	//! 2011.07.12 modified by humkyung
public:
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnDestroy();
	//afx_msg void OnBnClickedButtonSaveQueryClause();
	afx_msg void OnNMClickListLineList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListLineList(NMHDR *pNMHDR, LRESULT *pResult);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
