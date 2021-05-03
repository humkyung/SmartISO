#if !defined(AFX_ISOEDITFORM_H__0F3EDEBB_A0D9_44B0_BDC6_992D76E3D2BA__INCLUDED_)
#define AFX_ISOEDITFORM_H__0F3EDEBB_A0D9_44B0_BDC6_992D76E3D2BA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IsoEditForm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIsoEditForm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include <DgnDocument.h>
#include <GradientStatic.h>
#include <ResizeWnd.h>
#include "IsoFileListCtrl.h"
#include <ListCtrlEx.h>
#include "LineListSelectOption1.h"
#include "IsoEditOptionDlg.h"
#include "QualifiedLineForm.h"
#include "IsoEditOptionDlg2.h"

#include "DgnCanvas.h"

typedef struct  
{
	CString rOutputFilePath;
	CString rTempFilePath;

	CString	sOutputIDFFilePath , sTempIDFFilePath;	/// 2012.11.10 added by humkyung
}IsoOutPath;

typedef struct 
{
	CString rSeedFileName;
	CString rTempFileName;
}IsoTemp2OutputMatch;

class CIsoEditViewLogFileForm;
class CIsoEditForm : public CFormView
{
protected:
	
	DECLARE_DYNCREATE(CIsoEditForm)

// Form Data
public:
	//{{AFX_DATA(CIsoEditForm)
	enum { IDD = IDD_ISO_EDIT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	
	//CGradientStatic m_QualifiedStatic;
	
	CString m_rJobIsoDwgFileFolder;
	CString m_rSourceIsoDwgFileFolder , m_rEditedIsoDwgFileFolder;
	
	IsGui::CListCtrlEx m_wndLineDataReport;
	CString m_rIniFilePath;
	CQualifiedLineForm* m_pQualifiedLineForm;
	CLineDataRecordSet* m_pLineDataRecordSet;
	int m_nRecordCount;
	BOOL m_bEditIsoFolder;
	vector<IsoOutPath> m_OutPutFileFullPath;
	vector<IsoTemp2OutputMatch> m_Temp2OutMatch;
// Attributes
public:

// Operations
public:
	void GenerateIsoProduct();
	void GetSelectedLineDataRecord(vector<LineDataRecordMap>& IsoEditItemEntry);
	void ShowSelectedCount(int nCount);
	void ShowRecordCount(int nRecordCount);
	void SetViewLogFileForm(CIsoEditViewLogFileForm* p);
	void LineCONTRASTISo(const int& nRecordCount);
	void LoadLineDataRecordSetFromDatabase(const CString& rQueryCondition , const char* pRevNo , const vector< string >& issueDates );
	void InitContents();
	CIsoEditForm();           // protected constructor used by dynamic creation
	virtual ~CIsoEditForm();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIsoEditForm)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	UINT m_uTimerID;
	CIsoEditViewLogFileForm* m_pViewLogFileForm;
	CImageList m_ImageList;
	CFont* m_pBoldFont;
	map<int , int> m_ItemMap;			///< iso edit item -> qualified item
	map<int , int> m_ReverseItemMap;		///< qualified item -> iso edit item

	map<long , CLineDataRecord*> m_IsoFileMap;	///< line data record id -> line data record pointer

	CListCtrl*	m_pDragList;			// Which ListCtrl we are dragging FROM
	CDgnCanvas*	m_pDropList;			// Which ListCtrl we are dropping ON
	CImageList*	m_pDragImage;			// For creating and managing the drag-image
	BOOL		m_bDragging;			// T during a drag operation
	int		m_nDragIndex;			// Index of selected item in the List we are dragging FROM
	int		m_nDropIndex;			// Index at which to drop item in the List we are dropping ON
	CWnd*		m_pDropWnd;			// Pointer to window we are dropping on (will be cast to CListCtrl* type)
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CIsoEditForm)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnIsoEditQualify();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	///afx_msg void OnRclickListLineData(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkListLineData(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg void OnDblclkQualifiedFileList(NMHDR* pNMHDR, LRESULT* pResult) ;
	afx_msg void OnBegindragLineList(NMHDR* pNMHDR, LRESULT* pResult) ;
	afx_msg LRESULT OnKickIdle(WPARAM wParam , LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	void ExecuteSmartISODiff(CLineDataRecord* pLineDataRecord);	/// 2012.01.03 added by humkyung

	void SaveLogFile();
	void SaveErrorSumFile();
	void DeleteRemoveArea(CDgnDocument* pDgnDoc);
	void DrawLineDataToOuputDwg(CDgnDocument* pDgnDoc , CLineDataRecord *pLineDataRecord , const CString& rIniFilePath);
	void PreProcess();
	CString GetOutputFilePathWithLastRevNo( CLineDataRecord* pLineDataRecord );
	void PostProcess();
	void DrawRevDataToOutputDwg( CDgnDocument* pDgnDoc , CRevDataRecordSet* pRevDataRecordSet , const CString& rIniFilePath );
	void DrawLatestRevDataToOutputDwg(CDgnDocument* pDgnDoc , CRevDataRecordSet *pRevDataRecordSet , const CString& rIniFilePath );	/// 2011.10.25 added by humkyung
	void LoadData( const CString& rIniFilePath );
	void ProcessIsoFile(CLineDataRecord* pLineDataRecord , const CString& rSeedFilePath , const CString& rIniFilePath);
	void DropItemOnList(CListCtrl* pDragList, CDgnCanvas* pDropList);
	void DetermineLayout(int cx, int cy);
	void AddIsoEditItemEntry(vector<LineDataRecordMap>* pIsoEditItemEntry);
	void UpdateQualifiedFileList(const int &it, const int& nItem);

	static UINT StatusThreadEntry(LPVOID pVoid);
	UINT StatusThread();

	list<STRING_T> m_oLineDataFieldNameEntry;	//! store field name in line data table - 2011.07.08 added by humkyung
	static UINT TIMER_ID;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnLvnItemchangedListLineData(NMHDR *pNMHDR, LRESULT *pResult);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ISOEDITFORM_H__0F3EDEBB_A0D9_44B0_BDC6_992D76E3D2BA__INCLUDED_)
