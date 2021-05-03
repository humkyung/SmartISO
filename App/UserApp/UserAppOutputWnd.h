#pragma once

#include "BackupDataOutputForm.h"
#include "QualifiedLineForm.h"
#include "IsoEditViewLogFileForm.h"
#include "ViewLogFileForm.h"
#include "DrawingReportLogFileForm.h"
#include "BMLogFileForm.h"
#include "EmptyOutputWnd.h"
#include "GetPdsLineID.h"
#include "GetPds.h"
#include "LineDataEditDlg.h"
// CUserAppOutputWnd

class CUserAppOutputWnd : public CDockablePane
{
	DECLARE_DYNAMIC(CUserAppOutputWnd)

public:
	CUserAppOutputWnd();
	virtual ~CUserAppOutputWnd();

	CBackupDataOutputForm*		m_pBackupDataOutputForm;	/// backup form
	CQualifiedLineForm*         m_pQualifiedLineForm;
	CIsoEditViewLogFileForm*    m_pIsoEditViewLogFileForm;		/// edit line data output form
	CViewLogFileForm*			m_pViewLogFileForm;
	CDrawingReportLogFileForm*  m_pDrawingReportLogFileForm;	/// drawing report output form
	CBMLogFileForm*             m_pBMLogFileForm;				/// bm output form
	CEmptyOutputWnd*            m_pEmptyOutputWnd;
	CLineDataEditDlg*			m_pLineDataEditDlg;

	///void ShowGetPdsLogFileForm(const bool& show);
	void ShowDrawingReportOutputForm(const bool& show);
	CUserAppDoc* m_pUserAppDoc;

	void InitContents();
	void HideAll();
	void ShowBackupDataOutputForm(const bool &show);
	void ShowQualifiedLineForm(const bool &show);
	void ShowIsoEditViewLogFileForm(const bool &show);
	void ShowViewLogFileForm(const bool &show);
	void ShowBMLogFileForm(const bool &show);
	void ShowUpdatePds(const bool &show);
	void ShowLineDataForm(const bool &show);
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};