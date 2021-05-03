// ExcelMapingGridCell.cpp: implementation of the CExcelMapingGridCell class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "GridCtrl/GridCtrl.h"
#include "ExcelMapingGridCell.h"
#include <FilterEdit/UIntEdit.h>

#include <map>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace GridCell;

IMPLEMENT_DYNCREATE(CExcelMapingGridCell, CGridCell)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExcelMapingGridCell::CExcelMapingGridCell()
{
	m_nRow = 0;
	m_nColumn = 0;
}

CExcelMapingGridCell::~CExcelMapingGridCell()
{

}

BOOL CExcelMapingGridCell::Edit(int nRow, int nCol, CRect rect, CPoint /* point */, UINT nID, UINT nChar)
{
	m_bEditing = TRUE;
	
	if(NULL == m_pEditWnd)
	{
		m_nRow = nRow;
		m_nColumn = nCol;

		int nHeight = rect.Height();
		rect.bottom = rect.bottom + 5*nHeight + ::GetSystemMetrics(SM_CYHSCROLL);

		CComboBox* pComboBox = new CComboBox;
		BOOL b = pComboBox->Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_HASSTRINGS ,rect , (CWnd*)GetGrid() , 101);
		if(TRUE == b)
		{
			m_font.CreateFont(-11,0,0,0,400,FALSE,FALSE,FALSE,HANGUL_CHARSET,3,2,1,VARIABLE_PITCH | FF_MODERN,_T("±¼¸²")); 
			pComboBox->SetFont(&m_font);
			
			static TCHAR *pMap[] = 
			{
				_T("A"),_T("B"),_T("C"),_T("D"),_T("E"),_T("F"),_T("G"),_T("H"),_T("I"),_T("J"),_T("K"),_T("L"),_T("M"),
				_T("N"),_T("O"),_T("P"),_T("Q"),_T("R"),_T("S"),_T("T"),_T("U"),_T("W"),_T("X"),_T("Y"),_T("Z")
			};
			const int iCount = sizeof(pMap) / sizeof(TCHAR*);
			for(int i=0;i < iCount;i++)
			{
				pComboBox->InsertString(i , pMap[i]);
			}
			pComboBox->InsertString(pComboBox->GetCount(),_T(""));
			
			for(int i=0;i < iCount;i++)
			{
				pComboBox->InsertString(i + iCount, CString(pMap[0]) + CString(pMap[i]));
			}

			CString rCellText = GetText();
			if(FALSE == rCellText.IsEmpty())
			{
				const int at = pComboBox->FindStringExact(0,rCellText);
				(-1 != at) ? pComboBox->SetCurSel(at) : pComboBox->SetCurSel(0);
			}
			else
			{
				CString rString;
				rString.Format(_T("%c") , nChar);
				rString.MakeUpper();

				const int at = pComboBox->FindStringExact(0,rString);
				(-1 != at) ? pComboBox->SetCurSel(at) : pComboBox->SetCurSel(0);
			}
		}
		

		m_pEditWnd = pComboBox;
		m_pEditWnd->SetFocus();
	}
	else
	{
		CRect wndRect;
		m_pEditWnd->GetWindowRect(&wndRect);
		int height = wndRect.bottom - wndRect.top;
		rect.bottom = rect.top + height;
		m_pEditWnd->MoveWindow(rect);
		m_pEditWnd->ShowWindow(SW_NORMAL);
		m_pEditWnd->SetFocus();
	}
	
	return TRUE;
}

/**	
	@brief	The CFieldTypeGridCell::EndEdit function
	@author	humkyung
	@return	void	
*/
void CExcelMapingGridCell::EndEdit()
{
	if (NULL != m_pEditWnd)
	{
		CComboBox* pComboBox = ((CComboBox*)m_pEditWnd);

		CString rText;
		pComboBox->GetWindowText(rText);
		if(rText != this->GetText())
		{
			SetText(rText);
			
			/// Send Notification to gridctrl's parent - 2012.05.09 added by humkyung
			NM_GRIDVIEW nmgv;
			nmgv.iRow         = m_nRow;
			nmgv.iColumn      = m_nColumn;
			nmgv.hdr.hwndFrom = GetGrid()->GetSafeHwnd();
			nmgv.hdr.idFrom   = GetGrid()->GetDlgCtrlID();
			nmgv.hdr.code     = GVN_ENDLABELEDIT;

			CWnd *pOwner = GetGrid()->GetOwner();
			if (pOwner && IsWindow(pOwner->m_hWnd))
				pOwner->SendMessage(WM_NOTIFY, nmgv.hdr.idFrom, (LPARAM)&nmgv);
			/// up to here
		}
		m_pEditWnd->ShowWindow(SW_HIDE);
	}

	m_bEditing = FALSE;
}