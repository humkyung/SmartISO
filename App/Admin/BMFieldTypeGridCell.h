// DrawingTypeGridCell.h: interface for the CDrawingTypeGridCell class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWINGTYPEGRIDCELL_H__E2AFD38C_5FBE_48D8_85A2_7883433F2339__INCLUDED_)
#define AFX_DRAWINGTYPEGRIDCELL_H__E2AFD38C_5FBE_48D8_85A2_7883433F2339__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "./GridCtrl/GridCtrl.h"
#include <vector>
using namespace std;

namespace GridCell
{
class CBMFieldTypeGridCell : public  CGridCell
{
	DECLARE_DYNCREATE(CBMFieldTypeGridCell)
public:
	CBMFieldTypeGridCell();
	virtual ~CBMFieldTypeGridCell();
public:
	static void GetDataNameEntry( vector<STRING_T>& dataNameEntry );
	BOOL Edit(int nRow, int nCol, CRect rect, CPoint point, UINT nID, UINT nChar);
	void EndEdit();
private:
	CFont m_font;
};
};

#endif // !defined(AFX_DRAWINGTYPEGRIDCELL_H__E2AFD38C_5FBE_48D8_85A2_7883433F2339__INCLUDED_)
