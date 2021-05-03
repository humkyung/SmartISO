#if !defined(AFX_BMREPORTFORM_H__12B2AECF_9FA8_4979_B543_39314F6224C0__INCLUDED_)
#define AFX_BMREPORTFORM_H__12B2AECF_9FA8_4979_B543_39314F6224C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BMReportForm.h : header file
//
 
/////////////////////////////////////////////////////////////////////////////
// CBMReportForm form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include <GradientStatic.h>
#include <ResizeWnd.h>
#include "IsoFileListCtrl.h"
#include "LineListSelectOption1.h"
#include "BMReportOptionDlg.h"
#include "QCCheck.h"
#include "WeldDetailBMFile.h"
#include "WeldDiaCheck.h"
#include <XButtonXP/XButtonXP.h>

class INFDataStruct
{
public:
	STRING_T GetQueryClause() const;	/// 2012.08.31 added by humkyung
	bool HasSameValueExceptQty( const INFDataStruct& rhs ) const;	/// check if has same value except QTY - 2011.08.31 added by humkyung
	int RetrieveDataFromIsoBMData(CNtrData* pNtrData , CIsoBMData& oIsoBMData);	/// 2011.08.31 added by humkyung

	int GetHeaderString4Sum(vector<STRING_T>& csv_row) const;		/// 2011.09.24 added by humkyung
	int GetHeaderString(vector<STRING_T>& csv_row) const;
	CString GetBMReportString4TextFile() const;
	int GetBMReportString4SumCSV(vector<STRING_T>& csv_row) const;	/// 2011.09.24 added by humkyung
	int GetBMReportString4CSV(vector<STRING_T>& csv_row) const;
	CString GetMainSize() const;
	CString GetSubSize() const;
	void SetMainSize(const CString& rSize);
	void SetSubSize(const CString& rSize);
	INFDataStruct();
	INFDataStruct( const INFDataStruct& rhs );	/// copy constructor - 2011.09.24 added by humkyung

	CString m_rProjectNo, m_sMatlSpec , m_rSymbol;
	CString m_rQuantity , m_sMatlCode , m_rLineNo , m_rDwgNo , m_rPlantNo , m_rAreaNo , m_rUnitNo;
	CString m_rSectionNo, m_rSheetNo, m_rRevNo, m_rSerialNo , m_rFieldShop , m_rFluid , m_rInsulType , m_rTemper , m_rPaint;
	CString m_rInsulThk;
	CString m_sMatlDesc;	/// 2011.08.31 added by humkyung
	CString m_sKey;	/// 2012.08.31 added by humkyung

	CString m_rFormatString;
	static CString m_rHeaderItems[];
private:
	CString m_rMainSize, m_rSubSize;
	CString m_sBoltLength;	/// 2011.09.01 added by humkyung
};

class CBMOutputPath
{
public:
	CBMOutputPath(){}
	~CBMOutputPath(){}

	CBMOutputPath(const CBMOutputPath& rhs)
	{
		(*this) = rhs;
	}

	CBMOutputPath& operator=(const CBMOutputPath& rhs)
	{
		if(this != &rhs)
		{
			m_rProductFilePath = rhs.m_rProductFilePath;
			m_rTempFilePath    = rhs.m_rTempFilePath;
		}

		return (*this);
	}

	CString m_rProductFilePath;
	CString m_rTempFilePath;
};

class CBMLogFileForm;
class CBMReportForm : public CFormView
{
protected:
	
	DECLARE_DYNCREATE(CBMReportForm)

// Form Data
public:
	//{{AFX_DATA(CBMReportForm)
	enum { IDD = IDD_BM_REPORT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	IsGui::CListCtrlEx m_wndLineDataReport;
	CButton m_PipeCuttiongLengthButton;
	int m_nRecordCount;
// Attributes
public:
// Operations
public:
	int Generate();
	void GetSelectedLineDataRecord(vector<LineDataRecordMap>&);
	void ShowRecordCount(int nRecordCount);
	void ShowSelectedCount();
	void DeleteQualifiedLineList();
	void GenerateInterfaceExcel();
	void SetViewLogFileForm(CBMLogFileForm* p);

	void InitContents();
	CBMReportForm();           // protected constructor used by dynamic creation
	virtual ~CBMReportForm();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBMReportForm)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool MakeWeldDiaCheck(const CString& rOutputFilePath);
	CWeldDiaCheck m_weldDiaCheck;
	CWeldDetailBMFile m_weldDetailBMFile;
	CQCCheck m_qcCheck;
	
	vector<CIsoBMData> m_paintCodeEntry;
	vector<CIsoBMData> m_insBMEntry;
	vector<CIsoBMData> m_SupportBMEntry;
	
	vector< STRING_T >  m_InfAllItemEntry;		///< PIPE BULK ITEM + OTHER ITEM
	vector< STRING_T >  m_PipingBulkItemEntry;	///< PIPE BULK ITEM 컨테이너
	vector< STRING_T >  m_OthersItemEntry;		///< OHTER ITEM 컨테이너
	vector< CString >  m_LogTextEntry;		///< LOG 파일에 저장할 문자열 컨테이너
	
	vector<INFDataStruct> m_oINFExlEntry;

	map<int , int> m_ItemMap;			///< iso edit item -> qualified item
	map<int , int> m_ReverseItemMap;		///< qualified item -> iso edit item

	CBMLogFileForm* m_pViewLogFileForm;
	
	CString m_rOutputDrawingFolder;			///< ISO EDIT에서 생성된 파일들이 들어 있는 폴더.
	///CResizeWnd  m_ResizeWnd;
	CImageList m_ImageList;
	CFont* m_pBoldFont;
	map<int , CLineDataRecord*> m_IsoFileMap;
	CLineDataRecordSet* m_pLineDataRecordSet;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CBMReportForm)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnUpdateButtonStart(CCmdUI* pCmdUI);
	afx_msg void OnKeydownListLineData(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonIssue();
	//}}AFX_MSG
	/*afx_msg void OnHorzResize();*/
	DECLARE_MESSAGE_MAP()
private:
	int WriteCSVFilesToExcelFile( const CString& sExlFilePath , const CString& sCSVFile1 , const CString& sCSVFile2 );	/// 2013.01.22 added by humkyung
	void DropItemOnList(CListCtrl* pDragList, CListCtrl* pDropList);						/// 2012.05.08 added by humkyung 
	int CreateIssueFolder(const CString& sProjectFolder , const CString& sIssueDate);		/// 2012.05.08 added by humkyung

	bool GenerateLogText(vector<CString>& LogTextEntry , const int &nItem);
	bool GeneratePipeSupportCheck(const CString& rOutputFilePath);
	void InitVariables();
	vector<CBMOutputPath> m_OutputFileFullPathEntry;	///< 생성되는 파일의 TEMP 쪽의 이름과 PRODUCT쪽의 이름을 가지고 있다.

	void SaveErrorSumFile();
	void AddToQualifiedListCtrl(vector<long>* pIsoEditItemEntry);
	void GenerateINFSumFile(const CBMOutputPath &oBMFilePath , const CBMOutputPath& oINFSumFilePath , const CBMOutputPath& oINFAllSumFilePath , const CBMOutputPath& oPipingINFFilePath , const CBMOutputPath& oOthersINFFilePath);
	void ProcessIsoFile(const int& nItem , const CString &rOutputFilePath , const CString& rIniFilePath);
	bool GenerateInsulationBM(const CString& rOutputFilePath);
	bool GeneratePaintCode(const CString& rOutputFilePath);
	void MakeOracleDBFFile();
	bool MakeWeldDetailFile(const CString& rOutputFilePath);
	bool GenerateLogFile();
	void LogFileSynchronizing(const vector<CString>& LogTextEntry);
	bool GenerateINFFile(CAppPostgreSQL& , const CString& rINFFilePath );
	bool MakePRTFile(const CString& rPrtFilePath);
	bool MakeNTRFile(const CString& rNtrFilePath);
	void DetermineLayout(int cx , int cy);
	void LoadLineDataRecordSetFromDatabase(const CString &rQueryCondition , const vector< string >& issueDates );
	void SyncOutputFileWithDataRecord();

	static UINT StatusThreadEntry(LPVOID pVoid);
	UINT StatusThread();
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnNMDblclkListLineData(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListLineData(NMHDR *pNMHDR, LRESULT *pResult);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BMREPORTFORM_H__12B2AECF_9FA8_4979_B543_39314F6224C0__INCLUDED_)
