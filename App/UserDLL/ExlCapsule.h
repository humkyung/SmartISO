// CExlCapsule.h: interface for the CExlCapsule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXLCAPSULE_H__917B5045_7D7E_49CF_ABF8_AF4306F8040B__INCLUDED_)
#define AFX_EXLCAPSULE_H__917B5045_7D7E_49CF_ABF8_AF4306F8040B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <util/excel9/excel9.h>

#include "UserDocData.h"
using namespace IsExcel;

class USER_EXT_CLASS CExlCapsule  
{
public:
	void SetActiveSheet(int index);
	void ExlSheetCopy(const CString& rName);
	CExlCapsule();
	virtual ~CExlCapsule();
	
	void ExlOpen(CString ExlPath);
	void ExlWrite(COleVariant CellNum, COleVariant Value);
	void ExlSaveAs(CString ExlOutputPath);
	void ExlClose();
	CString WsMakeExcelCellType(UINT unRowIndex, UINT unColumnIndex);

protected:
	_Application m_App;
	Workbooks m_books;
	_Workbook m_book;
	Worksheets m_sheets;
	_Worksheet m_sheet;
	Range m_range;
//	Borders			m_borders;

};

#endif // !defined(AFX_EXLCAPSULE_H__917B5045_7D7E_49CF_ABF8_AF4306F8040B__INCLUDED_)
