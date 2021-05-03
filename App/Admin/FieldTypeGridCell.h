// FieldTypeGridCell.h: interface for the CFieldTypeGridCell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINEWEIGHTGRIDCELL_H__726B14E3_249F_4487_993D_33F8DE3C155C__INCLUDED_)
#define AFX_LINEWEIGHTGRIDCELL_H__726B14E3_249F_4487_993D_33F8DE3C155C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "./GridCtrl/GridCtrl.h"

namespace GridCell
{
class CFieldTypeGridCell : public CGridCell
{
	DECLARE_DYNCREATE(CFieldTypeGridCell)
public:
	CFieldTypeGridCell();
	virtual ~CFieldTypeGridCell();
public:
	BOOL Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar);
	void EndEdit();
private:
	CFont m_font;
};
};

#endif // !defined(AFX_LINEWEIGHTGRIDCELL_H__726B14E3_249F_4487_993D_33F8DE3C155C__INCLUDED_)
