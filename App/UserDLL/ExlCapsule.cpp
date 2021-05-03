// CExlCapsule.cpp: implementation of the CExlCapsule class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UserDLL.h"
#include <util/excel9/excel9.h>
#include "ExlCapsule.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace IsExcel;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExlCapsule::CExlCapsule()
{

}

CExlCapsule::~CExlCapsule()
{

}
void CExlCapsule::ExlOpen(CString ExlPath)
{	
	CoInitialize(NULL);
	COleVariant vTrue((short)TRUE),  vFalse((short)FALSE), VOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	
	if(!m_App.CreateDispatch("Excel.Application"))
	{
		AfxMessageBox("Excel Connection Faliure");
	}
	else
	{
		LPDISPATCH lpdisp;
		lpdisp=m_App.GetWorkbooks();
		ASSERT(lpdisp);
		m_books.AttachDispatch(lpdisp);
		m_book = m_books.Open(ExlPath,VOptional, VOptional, VOptional,VOptional,VOptional, VOptional, VOptional, VOptional, VOptional, VOptional, VOptional, VOptional);
		m_sheets = m_book.GetWorksheets();
		m_sheet = m_book.GetActiveSheet();
	}
}

void CExlCapsule::ExlWrite(COleVariant CellNum, COleVariant Value)
{
	try
	{
		m_range = m_sheet.GetRange(CellNum,CellNum);
		m_range.SetValue2(Value);
	}
	catch(_com_error &e)
	{
		CString rMsg;
		rMsg.Format("*** <CExlCapsule 오류> 설명[%s] !! ***\n", (LPCSTR)e.Description());
		AfxMessageBox(rMsg);
	}
}

void CExlCapsule::ExlSaveAs(CString ExlOutputPath)
{	
	COleVariant vTrue((short)TRUE),  vFalse((short)FALSE), VOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	
	m_App.SetDisplayAlerts(FALSE);
	m_book.SaveAs(COleVariant(ExlOutputPath),COleVariant((long)-4143),VOptional,VOptional,vFalse,vFalse,(long)0, VOptional, VOptional, VOptional, VOptional);	
}

void CExlCapsule::ExlClose()
{
	COleVariant vTrue((short)TRUE),  vFalse((short)FALSE), VOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	
	m_book.Close(vFalse, VOptional, VOptional);
	m_App.Quit();
	m_App.ReleaseDispatch();
}

CString CExlCapsule::WsMakeExcelCellType(UINT unRowIndex, UINT unColumnIndex)
{
	char szResult[10] = {0,};
	if(unColumnIndex > 26)
		sprintf(szResult, "%c%c%d",
		'A'+(unColumnIndex-1)/26-1, 'A'+(unColumnIndex-1)%26, unRowIndex);
	else
		sprintf(szResult, "%c%d", 'A' + (unColumnIndex-1)%26, unRowIndex);
	return szResult;
}

/**
	\brief	
	\author	백흠경
**/
void CExlCapsule::ExlSheetCopy(const CString& rName)
{
	COleVariant vTrue((short)TRUE),  vFalse((short)FALSE), vOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	/// 새로운 sheet를 만든다.
	VARIANT varAfter;
	varAfter.vt = VT_DISPATCH;	
	varAfter.pdispVal = m_sheet.m_lpDispatch;
	m_sheet.Copy(vOptional , varAfter );

	m_sheet = m_App.GetActiveSheet();
	m_sheet.SetName(rName);

/*
	COleVariant vTrue((short)TRUE),  vFalse((short)FALSE), VOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);

	// 시트에서 사용된 range를 복사한다.
	Range range;
	range = m_sheet.GetUsedRange();
	range.Copy(VOptional);

	// 새로운 sheet를 만든다.
	VARIANT varAfter;
	varAfter.vt = VT_DISPATCH;	
	varAfter.pdispVal = m_sheet.m_lpDispatch;
	m_sheets.Add(VOptional, varAfter, VOptional, VOptional);

	// 새로운 sheet에 range를 붙인다.
	m_sheet = m_App.GetActiveSheet();
	m_sheet.Paste(VOptional, VOptional);
*/
}

void CExlCapsule::SetActiveSheet(int index)
{
	m_sheet = m_sheets.GetItem(COleVariant((short)index));
}
