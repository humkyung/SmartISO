// LineWeightGridCell.cpp: implementation of the CFieldTypeGridCell class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "GridCtrl/GridCtrl.h"
#include "FieldTypeGridCell.h"

#include <map>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace GridCell;

IMPLEMENT_DYNCREATE(CFieldTypeGridCell, CGridCell)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFieldTypeGridCell::CFieldTypeGridCell()
{
}

CFieldTypeGridCell::~CFieldTypeGridCell()
{

}

/**	\brief	The CFieldTypeGridCell::Edit function

	\param	nRow	a parameter of type int
	\param	nCol	a parameter of type int
	\param	rect	a parameter of type CRect
	\param	/	a parameter of type CPoint /* point *
	\param	nID	a parameter of type UINT
	\param	nChar	a parameter of type UINT

	\return	BOOL	
*/
BOOL CFieldTypeGridCell::Edit(int nRow, int nCol, CRect rect, CPoint /* point */, UINT nID, UINT nChar)
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
			m_font.CreateFont(-11,0,0,0,400,FALSE,FALSE,FALSE,HANGUL_CHARSET,3,2,1,VARIABLE_PITCH | FF_MODERN,"굴림"); 
			pComboBox->SetFont(&m_font);

			pComboBox->InsertString(0 , "텍스트");
			pComboBox->InsertString(1 , "메모");
			pComboBox->InsertString(2 , "숫자");
			pComboBox->InsertString(3 , "날짜/시간");
			pComboBox->InsertString(4 , "통화");
			pComboBox->InsertString(5, "예/아니요");

			const int count = pComboBox->GetCount();
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

		m_pEditWnd = pComboBox;
	}
	else
	{
		CRect wndRect;
		m_pEditWnd->GetWindowRect(&wndRect);
		int height = wndRect.bottom - wndRect.top;
		rect.bottom = rect.top + height;
		m_pEditWnd->MoveWindow(rect);
		m_pEditWnd->ShowWindow(SW_NORMAL);
	}
	
	return TRUE;
}

/**	\brief	The CFieldTypeGridCell::EndEdit function


	\return	void	
*/
void CFieldTypeGridCell::EndEdit()
{
	if (m_pEditWnd)
	{
		CComboBox* pComboBox = ((CComboBox*)m_pEditWnd);

		CString rText;
		pComboBox->GetWindowText(rText);
		SetText(rText);
		
		m_pEditWnd->ShowWindow(SW_HIDE);
	}

	m_bEditing = FALSE;
}