// DrawingTypeGridCell.cpp: implementation of the CDrawingTypeGridCell class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include <GridCtrl/GridCtrl.h>
#include <ado/SqlFile.h>
#include "DrawingTypeGridCell.h"
#include "tokenizer.h"
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

IMPLEMENT_DYNCREATE(CDrawingTypeGridCell, CGridCell)
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDrawingTypeGridCell::CDrawingTypeGridCell()
{

}

CDrawingTypeGridCell::~CDrawingTypeGridCell()
{

}

/******************************************************************************
	@brief
    @author     humkyung
    @date       2011-10-31
    @class      CDrawingTypeGridCell
    @function   GetDataNameEntry
    @return     void
    @param      vector<STRING_T>&   dataNameEntry
******************************************************************************/
void CDrawingTypeGridCell::GetDataNameEntry(vector<STRING_T>& oDataNameEntry)
{
	CAdminDocData* pAdminDocData = CAdminDocData::GetInstance();
	CString rIniFilePath = pAdminDocData->GetIniFilePath();
	TCHAR szBuf[MAX_PATH] = {'\0', };
	
	const CString rSqlFilePath = CAdminDocData::GetExecPath() + _T("\\Setting\\T_ISO_LINE_DATA.XML");
	CSQLFile sqlFile(rSqlFilePath.operator LPCSTR());
	if(ERROR_SUCCESS == sqlFile.ParseXML(rSqlFilePath.operator LPCSTR()))
	{
		const int nCount = sqlFile.GetColumnCount();

		for(int i = 0;i < nCount;++i)
		{
			const STRING_T rFieldName = sqlFile.GetFieldNameAt(i);
			oDataNameEntry.push_back(rFieldName);
		}

		stable_sort(oDataNameEntry.begin() , oDataNameEntry.end());
	}

	for(int i = 0;i < 14;++i)
	{
		STRINGSTREAM_T oss;
		oss << _T("REV ") << i << _T(" DATE");
		oDataNameEntry.push_back(oss.str());
	}
}

BOOL CDrawingTypeGridCell::Edit(int nRow, int nCol, CRect rect, CPoint /* point */, UINT nID, UINT nChar)
{
	int i=0;
	m_bEditing = TRUE;
	CAdminDocData* pAdminDocData = CAdminDocData::GetInstance();
	CString rIniFilePath = pAdminDocData->GetIniFilePath();
	vector<STRING_T> tableDescEntry;
	
	tableDescEntry.push_back(" ");
	CDrawingTypeGridCell::GetDataNameEntry( tableDescEntry );
	
	if(NULL == m_pEditWnd)
	{
		int nHeight = rect.Height();
		rect.bottom = rect.bottom + 5*nHeight + ::GetSystemMetrics(SM_CYHSCROLL);

		CComboBox* pComboBox = new CComboBox;
		BOOL b = pComboBox->Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST | CBS_HASSTRINGS | CBS_SORT ,rect , (CWnd*)GetGrid() , 101);
		if(TRUE == b)
		{
			m_font.CreateFont(-11,0,0,0,400,FALSE,FALSE,FALSE,HANGUL_CHARSET,3,2,1,VARIABLE_PITCH | FF_MODERN, _T("Arial")); 
			pComboBox->SetFont(&m_font);
			
			int nIndex = 0;
			for(vector<string>::iterator itr = tableDescEntry.begin();itr != tableDescEntry.end();++itr)
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
				rString.Format("%c" , nChar);
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
		/*
		if(pComboBox)
		{
			
			int nCount = pComboBox->GetCount();
			for(int i = nCount - 1;i >= 0;--i)
			{
				pComboBox->DeleteString(i);
			}
			
			int nIndex = 0;
			for(vector<string>::iterator itr = tableDescEntry.begin();itr != tableDescEntry.end();++itr)
			{
				pComboBox->InsertString(nIndex++ , itr->c_str());
			}
		}
		*/	
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
void CDrawingTypeGridCell::EndEdit()
{
	if (NULL != m_pEditWnd)
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