// DrawingTypeGridCell.cpp: implementation of the CBMFieldTypeGridCell class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include <GridCtrl/GridCtrl.h>
#include "BMFieldTypeGridCell.h"
#include <Tokenizer.h>
#include <FilterEdit/UIntEdit.h>
#include "AdminDocData.h"

#include <VECTOR>
#include <string>
using namespace std;

#include <map>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace GridCell;

IMPLEMENT_DYNCREATE(CBMFieldTypeGridCell, CGridCell)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBMFieldTypeGridCell::CBMFieldTypeGridCell()
{

}

CBMFieldTypeGridCell::~CBMFieldTypeGridCell()
{

}

/******************************************************************************
    @author     humkyung
    @date       2011-08-31
    @class      CBMFieldTypeGridCell
    @function   GetDataNameEntry
    @return     void
    @param      vector<STRING_T>&   dataNameEntry
    @brief
******************************************************************************/
void CBMFieldTypeGridCell::GetDataNameEntry( vector<STRING_T>& dataNameEntry )
{
	const STRING_T FieldTypes[] = 
	{
		_T("DRAWING NO") , /*_T("SHT NO") ,*/ _T("SYM") , _T("MATL SPEC") , _T("MATL CODE") , 
		_T("SIZE1") , _T("SIZE2") , _T("QTY") , _T("BOLT LEN") , _T("MATL DESC") , _T("")
	};

	for(int i = 0;i < SIZE_OF_ARRAY(FieldTypes);++i)
	{
		dataNameEntry.push_back(FieldTypes[i]);
	}
}

/******************************************************************************
    @author     humkyung
    @date       2011-08-31
    @class      CBMFieldTypeGridCell
    @function   Edit
    @return     BOOL
    @param      int     nRow
    @param      int     nCol
    @param      CRect   rect
    @param      CPoint  point
    @param      UINT    nID
    @param      UINT    nChar
    @brief
******************************************************************************/
BOOL CBMFieldTypeGridCell::Edit(int nRow, int nCol, CRect rect, CPoint /* point */, UINT nID, UINT nChar)
{
	int i=0;
	m_bEditing = TRUE;
	CAdminDocData* pAdminDocData = CAdminDocData::GetInstance();
	CString rIniFilePath = pAdminDocData->GetIniFilePath();
	vector<STRING_T> oTableDescEntry;
	
	CBMFieldTypeGridCell::GetDataNameEntry( oTableDescEntry );
	/// remove already selected item from entry - 2011.09.21 added by humkyung
	CGridCtrl* pGridCtrl = GetGrid();
	for(int i = 0;i < pGridCtrl->GetColumnCount();++i)
	{
		if(i == nCol) continue;
		CString sItemText = pGridCtrl->GetItemText(nRow , i);
		if(!sItemText.IsEmpty())
		{
			vector<STRING_T>::iterator where = find(oTableDescEntry.begin() , oTableDescEntry.end() , sItemText.operator LPCTSTR());
			if(where != oTableDescEntry.end()) oTableDescEntry.erase(where);
		}
	}
	/// up to here

	if(NULL == m_pEditWnd)
	{
		int nHeight = rect.Height();
		rect.bottom = rect.bottom + 5*nHeight + ::GetSystemMetrics(SM_CYHSCROLL);

		CComboBox* pComboBox = new CComboBox;
		BOOL b = pComboBox->Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_HASSTRINGS ,rect , (CWnd*)GetGrid() , 101);
		if(TRUE == b)
		{
			m_font.CreateFont(-11,0,0,0,400,FALSE,FALSE,FALSE,HANGUL_CHARSET,3,2,1,VARIABLE_PITCH | FF_MODERN, _T("Arial")); 
			pComboBox->SetFont(&m_font);
			
			int nIndex = 0;
			for(vector<string>::iterator itr = oTableDescEntry.begin();itr != oTableDescEntry.end();++itr)
			{
				pComboBox->InsertString(nIndex++ , itr->c_str());
			}

			int count = 0;
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
				CString rString;
				rString.Format(_T("%c") , nChar);
				rString.MakeUpper();

				count = pComboBox->GetCount();
				for(int i = 0;i < count;i++)
				{
					CString rLBText;
					pComboBox->GetLBText(i , rLBText);
					
					int at = rString.Find(rLBText , 0);
					if(-1 != at )
					{
						pComboBox->SetCurSel(i);
						break;
					}
				}
			}
		}
		

		m_pEditWnd = pComboBox;
		m_pEditWnd->SetFocus();
	}
	else
	{
		CComboBox* pComboBox = static_cast<CComboBox*>(m_pEditWnd);
		if(pComboBox)
		{
			pComboBox->ResetContent();

			int nIndex = 0;
			for(vector<string>::iterator itr = oTableDescEntry.begin();itr != oTableDescEntry.end();++itr)
			{
				pComboBox->InsertString(nIndex++ , itr->c_str());
			}
		}
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
void CBMFieldTypeGridCell::EndEdit()
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