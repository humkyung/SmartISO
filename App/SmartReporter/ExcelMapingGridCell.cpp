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

IMPLEMENT_DYNCREATE(CExcelMapingGridCell, CGridCell)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CExcelMapingGridCell::CExcelMapingGridCell()
{

}

CExcelMapingGridCell::~CExcelMapingGridCell()
{

}

/**
**/
BOOL CExcelMapingGridCell::Edit(int nRow, int nCol, CRect rect, CPoint /* point */, UINT nID, UINT nChar)
{
	int i=0;
	m_bEditing = TRUE;
	
	if(NULL == m_pEditWnd)
	{
		int nHeight = rect.Height();
		rect.bottom = rect.bottom + 5*nHeight + ::GetSystemMetrics(SM_CYHSCROLL);

		CComboBox* pComboBox = new CComboBox;
		BOOL b = pComboBox->Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWN | CBS_HASSTRINGS ,rect , (CWnd*)GetGrid() , 101);
		if(TRUE == b)
		{
			m_font.CreateFont(-11,0,0,0,400,FALSE,FALSE,FALSE,HANGUL_CHARSET,3,2,1,VARIABLE_PITCH | FF_MODERN, _T("±¼¸²") ); 
			pComboBox->SetFont(&m_font);
			
			static char *pMap[] = { "A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"};
			int count = sizeof(pMap) / sizeof(char*);
			for(i=0;i < count;i++)
			{
				pComboBox->InsertString(i , CString(pMap[i]));
			}
			
			for(i=0;i < count;i++)
			{
				pComboBox->InsertString(i + count, CString(pMap[0]) + CString(pMap[i]));
			}

			CString rCellText = GetText();
			if(FALSE == rCellText.IsEmpty())
			{
				count = pComboBox->GetCount();
				for(int i = 0;i < count;i++)
				{
					CString rLBText;
					pComboBox->GetLBText(i , rLBText);
					if(rLBText == GetText())
					{
						pComboBox->SetCurSel(i);
						break;
					}
				}
			}
			else
			{
				pComboBox->SetCurSel( 0 );
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

/**	\brief	The CFieldTypeGridCell::EndEdit function


	\return	void	
*/
void CExcelMapingGridCell::EndEdit()
{
	if (m_pEditWnd)
	{
		CComboBox* pComboBox = ((CComboBox*)m_pEditWnd);

		//const int lw = pLw->GetCurrentItemLineWeight();
		//m_lineWeight = GetLineWeight(lw);
		//CString rLw(FormatLineWeightDesc());
		CString rText;
		pComboBox->GetWindowText(rText);
		SetText(rText);
		
		m_pEditWnd->ShowWindow(SW_HIDE);
	}

	m_bEditing = FALSE;
}