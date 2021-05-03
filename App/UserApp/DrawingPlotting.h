#if !defined(AFX_DRAWINGPLOTTING_H__71B5DEFB_F77A_4C93_8205_E12FE73DE86B__INCLUDED_)
#define AFX_DRAWINGPLOTTING_H__71B5DEFB_F77A_4C93_8205_E12FE73DE86B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DrawingPlotting.h : header file
//
#include "IsoFileListCtrl.h"
#include "ReportCtrl/ReportCtrl.h"
/////////////////////////////////////////////////////////////////////////////
// CDrawingPlotting dialog

class CDrawingPlotting : public CDialog
{
// Construction
public:
	static UINT StatusThreadEntry(LPVOID pVoid);
	CDrawingPlotting(CWnd* pParent = NULL);   // standard constructor
	~CDrawingPlotting();
	CIsoFileListCtrl* m_pIsoFileList;
	CLineDataRecordSet* m_pLineDataRecordSet;
// Dialog Data
	//{{AFX_DATA(CDrawingPlotting)
	enum { IDD = IDD_DIALOG_PRINT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawingPlotting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void System_IPlot(CString ip);
	void Print_IPlot(const CString &rIsoFile);
	void LoadData(const CString &rIniFilePath);
	CString m_rOutputDrawingFolder;
	CString m_rPrintName;
	CString m_rSourceFilePath;
	// Generated message map functions
	//{{AFX_MSG(CDrawingPlotting)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	UINT StatusThread();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWINGPLOTTING_H__71B5DEFB_F77A_4C93_8205_E12FE73DE86B__INCLUDED_)
