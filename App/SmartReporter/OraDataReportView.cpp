// OraDataReportView.cpp : implementation file
//

#include "stdafx.h"
#include <assert.h>
#include "SmartReporter.h"
#include "OraDataReportView.h"


// COraDataReportView

IMPLEMENT_DYNCREATE(COraDataReportView, CView)

COraDataReportView::COraDataReportView()
{

}

COraDataReportView::~COraDataReportView()
{
}

BEGIN_MESSAGE_MAP(COraDataReportView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// COraDataReportView drawing

void COraDataReportView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// COraDataReportView diagnostics

#ifdef _DEBUG
void COraDataReportView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void COraDataReportView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// COraDataReportView message handlers
/**
	@brief	create custom data report dialog with custom data table

	@author	humkyung

	@date	2011.05.10

	@param	pCustomReportTable

	@return
**/
int COraDataReportView::CreateCustomDataReportDialog(CCustomReportTable* pCustomReportTable)
{
	assert(pCustomReportTable && "pCustomReportTable is NULL");

	if(pCustomReportTable)
	{
		CCustomDataReportDlg* pReportDlg = new CCustomDataReportDlg(pCustomReportTable , &m_wndTab);
		if(NULL != pReportDlg)
		{
			if(TRUE == pReportDlg->Create(CCustomDataReportDlg::IDD , &m_wndTab))
			{
				m_wndTab.AddTab(pReportDlg,  pCustomReportTable->m_sName.c_str());
			}
		}

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

/**
	@brief	create custom data report dialog which is not already created.

	@author	humkyung

	@date	2011.05.10

	@param	pCustomReportTable

	@return
**/
int COraDataReportView::SyncTabCtrlWithCustomReportTableList()
{
	CLineListToOraDocData& docData = CLineListToOraDocData::GetInstance();	
	list<CCustomReportTable*>* pCustomDataReportTableList = docData.CustomReportTableList();
	if(NULL != pCustomDataReportTableList)
	{
		for(list<CCustomReportTable*>::iterator itr = pCustomDataReportTableList->begin();itr != pCustomDataReportTableList->end();++itr)
		{
			bool bAlreadyCreated = false;
			CString sHeaderLabel = (*itr)->m_sName.c_str();
			for(int i = 0;i < m_wndTab.GetTabsNum();++i)
			{
				CString strLabel;
				m_wndTab.GetTabLabel(i , strLabel);
				if(sHeaderLabel == strLabel)
				{
					bAlreadyCreated = true;
					break;
				}
			}
			if(true == bAlreadyCreated) continue;

			CreateCustomDataReportDialog(*itr);
		}

		for(int i = m_wndTab.GetTabsNum() - 1;i >= 0 ;--i)
		{
			bool bAlreadyCreated = false;
			CString strLabel;
			m_wndTab.GetTabLabel(i , strLabel);
			for(list<CCustomReportTable*>::iterator itr = pCustomDataReportTableList->begin();itr != pCustomDataReportTableList->end();++itr)
			{
				CString sHeaderLabel = (*itr)->m_sName.c_str();
				if(sHeaderLabel == strLabel)
				{
					bAlreadyCreated = true;
					break;
				}
			}
			if(true == bAlreadyCreated) continue;
			
			m_wndTab.RemoveTab(i);
		}
	}

	return ERROR_SUCCESS;
}

/**
	@brief	load data

	@author	humkyung

	@date	????.??.??

	@param

	@return
**/
void COraDataReportView::LoadData()
{
	static const STRING_T APP( _T("General") );
	TCHAR szBuf[MAX_PATH + 1] = {'\0' ,};

	CLineListToOraDocData& docData = CLineListToOraDocData::GetInstance();
	STRING_T rIniFilePath = docData.GetIniFilePath();
	if(GetPrivateProfileString(APP.c_str() , _T("Project") , NULL , szBuf , MAX_PATH , rIniFilePath.c_str()))
	{
		m_rProjectName = szBuf;
	}else	m_rProjectName.Empty();

	if(GetPrivateProfileString(APP.c_str() , _T("Service Name") , NULL , szBuf , MAX_PATH , rIniFilePath.c_str()))
	{
		m_rServiceName = szBuf;
	}else	m_rServiceName.Empty();

	if(GetPrivateProfileString(APP.c_str() , _T("User Name") , NULL , szBuf , MAX_PATH , rIniFilePath.c_str()))
	{
		m_rUserName = szBuf;
	}else	m_rUserName.Empty();

	if(GetPrivateProfileString(APP.c_str() , _T("User Password") , NULL , szBuf , MAX_PATH , rIniFilePath.c_str()))
	{
		m_rUserPassword = szBuf;
	}else	m_rUserPassword.Empty();

	///
	if(GetPrivateProfileString(_T("MAPPING") , _T("Std Note file path") , NULL , szBuf , MAX_PATH , rIniFilePath.c_str()))
	{
		m_rStdNoteFilePath = szBuf;
	}else	m_rStdNoteFilePath.Empty();
}


int COraDataReportView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rect;
	rect.SetRectEmpty();
	if (!m_wndTab.Create(CMFCTabCtrl::STYLE_FLAT , rect, this , 0x100 , CMFCBaseTabCtrl::LOCATION_BOTTOM))
	{
		TRACE0("Failed to create workspace tab window\n");
		return -1;      /// fail to create
	}
	m_wndTab.ShowWindow(SW_SHOW);
	m_wndTab.SetActiveTabTextColor(RGB(0 , 0 , 255));
	if(FALSE == m_imgList.Create(IDB_LLTOORA_APP , 16 , 16 , RGB(255 , 0 , 255)))
	{
		TRACE0("Failed to create imagelist\n");
		return -1;      /// fail to create
	}
	m_wndTab.SetImageList(m_imgList);

	//! create custom data report dialog - 2011.05.04 added by humkyung
	CLineListToOraDocData& docData = CLineListToOraDocData::GetInstance();
	docData.LoadCustomDataReportList();

	list<CCustomReportTable*>* pCustomDataReportTableList = docData.CustomReportTableList();
	if(NULL != pCustomDataReportTableList)
	{
		for(list<CCustomReportTable*>::iterator itr = pCustomDataReportTableList->begin();itr != pCustomDataReportTableList->end();++itr)
		{
			CreateCustomDataReportDialog(*itr);
		}

		m_wndTab.SetActiveTab(0);
	}
	InvalidateRect(NULL);

	return 0;
}

void COraDataReportView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if(m_wndTab.GetSafeHwnd())
	{
		m_wndTab.SetWindowPos(NULL ,  0 , 0 , cx , cy , SWP_NOZORDER | SWP_NOACTIVATE);
	}
}
