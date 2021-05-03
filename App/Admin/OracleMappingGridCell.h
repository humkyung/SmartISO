// OracleMappingGridCell.h: interface for the COracleMappingGridCell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ORACLEMAPPINGGRIDCELL_H__76DF4E6D_2F12_4C53_8C0D_B889FF4780D2__INCLUDED_)
#define AFX_ORACLEMAPPINGGRIDCELL_H__76DF4E6D_2F12_4C53_8C0D_B889FF4780D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "./GridCtrl/GridCtrl.h"
#include <vector>
using namespace std;

namespace GridCell
{
class COracleMappingGridCell  : public CGridCell
{
	DECLARE_DYNCREATE(COracleMappingGridCell)
public:
	COracleMappingGridCell();
	virtual ~COracleMappingGridCell();
public:
	BOOL Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar);
	void EndEdit();
private:
	virtual void GetDataNameEntry( vector<STRING_T>& dataNameEntry );
	CFont m_font;

};

class CLineDataFieldGridCell  : public COracleMappingGridCell
{
	DECLARE_DYNCREATE(CLineDataFieldGridCell)
public:
	CLineDataFieldGridCell();
	virtual ~CLineDataFieldGridCell();
private:
	void GetDataNameEntry( vector<STRING_T>& dataNameEntry );
};
};

#endif // !defined(AFX_ORACLEMAPPINGGRIDCELL_H__76DF4E6D_2F12_4C53_8C0D_B889FF4780D2__INCLUDED_)
