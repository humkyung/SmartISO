// MFCTextColorListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "UserApp.h"
#include "MFCTextColorListCtrl.h"
#include "UserAppDocData.h"
#include <util/FileTools.h>

// CMFCTextColorListCtrl

IMPLEMENT_DYNAMIC(CMFCTextColorListCtrl, CMFCListCtrl)

CMFCTextColorListCtrl::CMFCTextColorListCtrl()
{

}

CMFCTextColorListCtrl::~CMFCTextColorListCtrl()
{
}


BEGIN_MESSAGE_MAP(CMFCTextColorListCtrl, CMFCListCtrl)
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CMFCTextColorListCtrl::OnNMDblclk)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnClick)
END_MESSAGE_MAP()


// CMFCTextColorListCtrl message handlers

/******************************************************************************
    @author     humkyung
    @date       2012-10-28
    @function   ClearAllTextColor
    @return     int
    @brief		clear text color map
******************************************************************************/
int CMFCTextColorListCtrl::ClearAllTextColor()
{
	m_ItemColorMap.clear();
	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2012-10-28
    @function   SetTextColor
    @return     int
    @param      const   int&
    @param      nRow    const
    @param      int&    nCol
    @param      const   COLORREF&
    @param      color
    @brief		set cell(nRow , nCol)'s text color
******************************************************************************/
int CMFCTextColorListCtrl::SetItemTextColor(const int& nRow , const int& nCol , const COLORREF& color)
{
	map<pair<int,int>,COLORREF>::iterator where = m_ItemColorMap.find(make_pair(nRow , nCol));
	if(where != m_ItemColorMap.end())
	{
		return where->second = color;
	}
	else
	{
		m_ItemColorMap.insert(make_pair(make_pair(nRow , nCol) , color));
	}

	return ERROR_SUCCESS;
}

/**
	@brief	return cell color

	@author humkyung	

	@date 2012-10-28

	@param	

	@return		
*/
COLORREF CMFCTextColorListCtrl::OnGetCellTextColor(int nRow , int nColumn)
{
	map<pair<int,int>,COLORREF>::iterator where = m_ItemColorMap.find(make_pair(nRow , nColumn));
	if(where != m_ItemColorMap.end())
	{
		return where->second;
	}

	return CMFCListCtrl::OnGetCellTextColor(nRow , nColumn);
}

/******************************************************************************
    @author     humkyung
    @date       2012-10-28
    @function   OnNMDblclk
    @return     void
    @param      NMHDR   *pNMHDR
    @param      LRESULT *pResult
    @brief		open selected iso file when double click
******************************************************************************/
void CMFCTextColorListCtrl::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<NMITEMACTIVATE*>(pNMHDR);
	// TODO: Add your control notification handler code here

	POSITION pos = this->GetFirstSelectedItemPosition();
	int nItem = this->GetNextSelectedItem(pos);
	if(nItem != -1)
	{
		CAppDocData* pDocData = CAppDocData::GetInstance();
		const CString sIniFilePath = pDocData->GetProject()->GetIniFilePath();
		CString rApp( _T("Iso Edit Setting") ) , sJobIsoDwgFileFolder;
		TCHAR szBuf[MAX_PATH] = {'\0' ,};
		
		if(GetPrivateProfileString( rApp , _T("Select From") , NULL , szBuf , MAX_PATH , sIniFilePath))
		{
			if( CString( _T("Source")) == szBuf )
			{
				if(GetPrivateProfileString( rApp , _T("Source Iso File Folder") , NULL , szBuf , MAX_PATH , sIniFilePath))
				{
					sJobIsoDwgFileFolder = szBuf;
				}
			}
			else
			{
				if(GetPrivateProfileString( rApp , _T("Output Iso Dwg File Folder") , NULL , szBuf , MAX_PATH , sIniFilePath))
				{
					sJobIsoDwgFileFolder = szBuf;
				}
			}
		}
		else	
		{
			if(GetPrivateProfileString( rApp , _T("Source Iso File Folder") , NULL , szBuf , MAX_PATH , sIniFilePath))
			{
				sJobIsoDwgFileFolder = szBuf;
			}
		}

		CString rValue = this->GetItemText(nItem , 0);

		if( _T("\\") != sJobIsoDwgFileFolder.Right(1)) sJobIsoDwgFileFolder += _T("\\");
		rValue = sJobIsoDwgFileFolder + rValue;
		if(CFileTools::DoesFileExist(rValue))
		{		
			if( GetPrivateProfileString(_T("Iso Edit Setting") , _T("MStation Path") , _T("") ,szBuf,MAX_PATH , sIniFilePath) )
			{
				CString rMSFilePath = szBuf;
				if( !rMSFilePath.IsEmpty() )
				{
					::ShellExecute(NULL, _T("open") ,rMSFilePath,rValue,NULL,SW_SHOW);
				}
				else
				{
					CString rString;
					rString.LoadString( IDS_NO_MSTATION_PATH );
					AfxMessageBox( rString );
				}
			}
			else
			{
				CString rString;
				rString.LoadString( IDS_NO_MSTATION_PATH );
				AfxMessageBox( rString );
			}
		}
		else
		{
			AfxMessageBox( CStringHelper::GetInstance().GetHelpString( IDS_NO_FILE ) );
		}
	}

	*pResult = 0;
}

/**
	@brief	user click on column

	@author	humkyung

	@date	2013.05.02
*/
void CMFCTextColorListCtrl::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	const int iColumn = pNMListView->iSubItem;

	///NOTICE: below codes force to set item data with row number
	for(int i = 0 ;i < this->GetItemCount();++i)
	{
		this->SetItemData(i , DWORD_PTR(i));
	}
	/// up to here

	bool bAscending = (iColumn == m_wndHeader.GetSortColumn() ? !m_wndHeader.IsAscending() : TRUE);
	Sort(iColumn, bAscending);
	
	*pResult = 0;
}

/**
	@brief	compare two items

	@author	humkyung

	@date	2013.05.02
*/
int CMFCTextColorListCtrl::OnCompareItems(LPARAM lParam1,LPARAM lParam2,int iColumn)
{
	const int iRow1 = int(lParam1);
	const int iRow2 = int(lParam2);

	CString sStr1 = this->GetItemText(iRow1 , iColumn);
	CString sStr2 = this->GetItemText(iRow2 , iColumn);

	bool bAscending = (iColumn == m_wndHeader.GetSortColumn() ? !m_wndHeader.IsAscending() : TRUE);
	return STRCMP_T(sStr1 , sStr2);
}