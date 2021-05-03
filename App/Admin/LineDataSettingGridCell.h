// LineDataSettingGridCell.h: interface for the CLineDataSettingGridCell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINEDATASETTINGGRIDCELL_H__A420101A_7B7A_49A0_B1E4_E2524A8295A2__INCLUDED_)
#define AFX_LINEDATASETTINGGRIDCELL_H__A420101A_7B7A_49A0_B1E4_E2524A8295A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "./GridCtrl/GridCtrl.h"
#include "LineDataSettingDlg.h"

namespace GridCell
{
class CLineDataSettingGridCell  : public CGridCell
{
	DECLARE_DYNCREATE(CLineDataSettingGridCell)
public:
	CLineDataSettingGridCell();
	virtual ~CLineDataSettingGridCell();
public:
	BOOL Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar);
	void EndEdit();
private:
	BOOL DayCompare(CString rIssue, CString rRealIssue);
	CFont m_font;
protected:
	int m_nRow , m_nCol;
	CLineDataSettingDlg* m_pLineDataSettingDlg; 
};
};

#endif // !defined(AFX_LINEDATASETTINGGRIDCELL_H__A420101A_7B7A_49A0_B1E4_E2524A8295A2__INCLUDED_)
