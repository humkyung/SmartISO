// OracleMappingGridCell.cpp: implementation of the COracleMappingGridCell class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GridCtrl/GridCtrl.h"
#include "OracleMappingGridCell.h"
#include <FilterEdit/UIntEdit.h>
#include <ado/SqlFile.h>
#include "AdminDocData.h"
#include "OraDatabaseDefFile.h"

#include <VECTOR>
#include <string>
using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace GridCell;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(COracleMappingGridCell, CGridCell)

COracleMappingGridCell::COracleMappingGridCell()
{

}

COracleMappingGridCell::~COracleMappingGridCell()
{

}

/**
	@author	humkyung
**/
void COracleMappingGridCell::GetDataNameEntry( vector<STRING_T>& dataNameEntry )
{
	CAdminDocData* pAdminDocData = CAdminDocData::GetInstance();
	CString rDDLFilePath = pAdminDocData->m_rDDLFilePath;
	
	auto_ptr<COraDatabaseDefFile> pOra(new COraDatabaseDefFile);
	pOra->Read(rDDLFilePath.operator LPCTSTR());
	
	const size_t nCount = pOra->GetFieldCount();
	for(int i = 0;i < int(nCount);++i)
	{
		COraDatabaseDefFile::CFieldDef def;
		if(pOra->GetFieldAt(def , i))
		{
			dataNameEntry.push_back(def.m_rFieldName.c_str());
		}
	}
	stable_sort(dataNameEntry.begin() , dataNameEntry.end());
}

/******************************************************************************
    @author     humkyung
    @date       ????-??-??
    @class      COracleMappingGridCell
    @function   Edit
    @return     BOOL
    @param      int     nRow
    @param      int     nCol
    @param      CRect   rect
    @param      CPoint  point
    @param      UINT    nID
	@param		UINT	nChar
    @brief
******************************************************************************/
BOOL COracleMappingGridCell::Edit(int nRow, int nCol, CRect rect, CPoint /* point */, UINT nID, UINT nChar)
{
	int i=0;
	m_bEditing = TRUE;
	CAdminDocData* pAdminDocData = CAdminDocData::GetInstance();
	CString rIniFilePath = pAdminDocData->GetIniFilePath();
	vector<string> tableDescEntry;
	
	tableDescEntry.push_back( _T(" ") );
	this->GetDataNameEntry( tableDescEntry );

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
void COracleMappingGridCell::EndEdit()
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

IMPLEMENT_DYNCREATE(CLineDataFieldGridCell, COracleMappingGridCell)

CLineDataFieldGridCell::CLineDataFieldGridCell()
{

}

CLineDataFieldGridCell::~CLineDataFieldGridCell()
{

}

/******************************************************************************
    @author     humkyung
    @date       2012-09-14
    @class      CLineDataFieldGridCell
    @function   GetDataNameEntry
    @return     void
    @param      vector<STRING_T>&   dataNameEntry
    @brief
******************************************************************************/
void CLineDataFieldGridCell::GetDataNameEntry( vector<STRING_T>& dataNameEntry )
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
			dataNameEntry.push_back(rFieldName);
		}

		stable_sort(dataNameEntry.begin() , dataNameEntry.end());
	}
}