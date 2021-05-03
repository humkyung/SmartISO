// ExcelMapingGridCell.h: interface for the CExcelMapingGridCell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXCELMAPINGGRIDCELL_H__E81AEECF_802C_40A2_A0A9_513C089DF2B1__INCLUDED_)
#define AFX_EXCELMAPINGGRIDCELL_H__E81AEECF_802C_40A2_A0A9_513C089DF2B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "./GridCtrl/GridCtrl.h"
class CExcelMapingGridCell  : public CGridCell
{
	DECLARE_DYNCREATE(CExcelMapingGridCell)
public:
	CExcelMapingGridCell();
	virtual ~CExcelMapingGridCell();
public:
	BOOL Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar);
	void EndEdit();
private:
	CFont m_font;
};

#endif // !defined(AFX_EXCELMAPINGGRIDCELL_H__E81AEECF_802C_40A2_A0A9_513C089DF2B1__INCLUDED_)
