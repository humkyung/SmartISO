// WorkspaceWnd.cpp : implementation file
//

#include "stdafx.h"
#include "UserApp.h"
#include "UserAppView.h"
#include "WorkspaceWnd.h"

// CWorkspaceWnd

IMPLEMENT_DYNAMIC(CWorkspaceWnd, CDockablePane)

CWorkspaceWnd::CWorkspaceWnd()
{

}

CWorkspaceWnd::~CWorkspaceWnd()
{
}


BEGIN_MESSAGE_MAP(CWorkspaceWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CWorkspaceWnd message handlers

int CWorkspaceWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty ();
	GetClientRect(rectDummy);
	if (!m_wndQueryTabCtrl.Create (CMFCTabCtrl::STYLE_3D , rectDummy, this, 0x100 , CMFCTabCtrl::LOCATION_TOP))
	{
		TRACE0("Failed to create workspace tab window\n");
		return -1;      // fail to create
	}
	m_wndQueryTabCtrl.SetImageList(IDB_USER_APP , 16 , RGB(255,255,255));
	m_wndQueryTabCtrl.SetActiveTabTextColor(RGB(0 , 0 , 255));

	if (!m_wndIsoEditTabCtrl.Create (CMFCTabCtrl::STYLE_3D , rectDummy, this, 0x101 , CMFCTabCtrl::LOCATION_TOP))
	{
		TRACE0("Failed to create workspace tab window\n");
		return -1;      // fail to create
	}
	m_wndIsoEditTabCtrl.ShowWindow(SW_HIDE);
	m_wndIsoEditTabCtrl.SetImageList(IDB_USER_APP , 16 , RGB(255,255,255));
	m_wndIsoEditTabCtrl.SetActiveTabTextColor(RGB(0 , 0 , 255));

	if (!m_wndDrawingIndexTabCtrl.Create (CMFCTabCtrl::STYLE_3D , rectDummy, this, 0x102 , CMFCTabCtrl::LOCATION_TOP))
	{
		TRACE0("Failed to create workspace tab window\n");
		return -1;      // fail to create
	}
	m_wndDrawingIndexTabCtrl.ShowWindow(SW_HIDE);
	m_wndDrawingIndexTabCtrl.SetImageList(IDB_USER_APP , 16 , RGB(255,255,255));
	m_wndDrawingIndexTabCtrl.SetActiveTabTextColor(RGB(0 , 0 , 255));

	if (!m_wndBMReportTabCtrl.Create (CMFCTabCtrl::STYLE_3D , rectDummy, this, 0x103 , CMFCTabCtrl::LOCATION_TOP))
	{
		TRACE0("Failed to create workspace tab window\n");
		return -1;      // fail to create
	}
	m_wndBMReportTabCtrl.ShowWindow(SW_HIDE);
	m_wndBMReportTabCtrl.SetImageList(IDB_USER_APP , 16 , RGB(255,255,255));
	m_wndBMReportTabCtrl.SetActiveTabTextColor(RGB(0 , 0 , 255));

	if (!m_wndDrawingPlottingTabCtrl.Create (CMFCTabCtrl::STYLE_3D , rectDummy, this, 0x104 , CMFCTabCtrl::LOCATION_TOP))
	{
		TRACE0("Failed to create workspace tab window\n");
		return -1;      // fail to create
	}
	m_wndDrawingPlottingTabCtrl.ShowWindow(SW_HIDE);
	m_wndDrawingPlottingTabCtrl.SetImageList(IDB_USER_APP , 16 , RGB(255,255,255));
	m_wndDrawingPlottingTabCtrl.SetActiveTabTextColor(RGB(0 , 0 , 255));

	if (!m_wndPDSUpdateTabCtrl.Create (CMFCTabCtrl::STYLE_3D , rectDummy, this, 0x105 , CMFCTabCtrl::LOCATION_TOP))
	{
		TRACE0("Failed to create workspace tab window\n");
		return -1;      // fail to create
	}
	m_wndPDSUpdateTabCtrl.ShowWindow(SW_HIDE);
	m_wndPDSUpdateTabCtrl.SetImageList(IDB_USER_APP , 16 , RGB(255,255,255));
	m_wndPDSUpdateTabCtrl.SetActiveTabTextColor(RGB(0 , 0 , 255));

	/// backup tab control - 2015.11.20 added by humkyung
	if (!m_wndBackupTabCtrl.Create (CMFCTabCtrl::STYLE_3D , rectDummy, this, 0x106 , CMFCTabCtrl::LOCATION_TOP))
	{
		TRACE0("Failed to create workspace tab window\n");
		return -1;      // fail to create
	}
	m_wndBackupTabCtrl.ShowWindow(SW_HIDE);
	m_wndBackupTabCtrl.SetImageList(IDB_USER_APP , 16 , RGB(255,255,255));
	m_wndBackupTabCtrl.SetActiveTabTextColor(RGB(0 , 0 , 255));
	if(m_wndQueryBackup.Create(CLineListSelectOption1::IDD, &m_wndBackupTabCtrl))
	{
		m_wndBackupTabCtrl.AddTab(&m_wndQueryBackup , _T("Query") , 2);
	}
	/// up to here

	///m_wndQuery1.m_pUserAppDoc = pDoc;
	/// here we insert the dialog test page as a child
	if(m_wndQuery1.Create(CLineListSelectOption1::IDD, &m_wndQueryTabCtrl))
	{
		m_wndQueryTabCtrl.AddTab(&m_wndQuery1 , _T("Query") , 2);
	}

	if(m_wndQuery2.Create(CLineListSelectOption1::IDD, &m_wndIsoEditTabCtrl))
	{
		m_wndIsoEditTabCtrl.AddTab(&m_wndQuery2 , _T("Query") , 2);
	}
	if(TRUE == m_IsoEditOptionDlg.Create(CIsoEditOptionDlg::IDD , &m_wndIsoEditTabCtrl))
	{
		m_wndIsoEditTabCtrl.AddTab(&m_IsoEditOptionDlg , _T("Option") , 3);
	}
	if(TRUE == m_IsoEditOptionDlg2.Create(CIsoEditOptionDlg2::IDD, &m_wndIsoEditTabCtrl))
	{
		m_wndIsoEditTabCtrl.AddTab(&m_IsoEditOptionDlg2 , _T("Detail Option") , 6);
	}

	if(m_wndQuery3.Create(CLineListSelectOption1::IDD, &m_wndDrawingIndexTabCtrl))
	{
		m_wndDrawingIndexTabCtrl.AddTab(&m_wndQuery3 , _T("Query") , 2);
	}

	if(m_wndQuery4.Create(CLineListSelectOption1::IDD, &m_wndBMReportTabCtrl))
	{
		m_wndBMReportTabCtrl.AddTab(&m_wndQuery4 , _T("Query") , 2);
	}
	if(m_BMReportOptionDlg.Create(CBMReportOptionDlg::IDD, &m_wndBMReportTabCtrl))
	{
		m_wndBMReportTabCtrl.AddTab(&m_BMReportOptionDlg , _T("Option") , 2);
	}
	
	///m_wndSelectIssueDate.m_pTargetWnd = this;
	if(TRUE == m_wndSelectIssueDate.Create(CSelectIssueDataDlg::IDD, &m_wndDrawingPlottingTabCtrl))
	{
		m_wndDrawingPlottingTabCtrl.AddTab(&m_wndSelectIssueDate , _T("Query") , 2);
	}

	if(m_wndQuery5.Create(CLineListSelectOption1::IDD, &m_wndPDSUpdateTabCtrl))
	{
		m_wndPDSUpdateTabCtrl.AddTab(&m_wndQuery5 , _T("Query") , 2);
	}

	return 0;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-27
    @class      CWorkspaceWnd
    @function   InitContents
    @return     void
    @brief
******************************************************************************/
void CWorkspaceWnd::InitContents()
{
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-27
    @class      CWorkspaceWnd
    @function   ShowLineDataForm
    @return     void
    @param      bool    bShow
    @brief
******************************************************************************/
void CWorkspaceWnd::ShowLineDataForm(const bool& bShow)
{
	m_wndQueryTabCtrl.ShowWindow(SW_SHOW);
	m_wndIsoEditTabCtrl.ShowWindow(SW_HIDE);
	m_wndDrawingIndexTabCtrl.ShowWindow(SW_HIDE);
	m_wndBMReportTabCtrl.ShowWindow(SW_HIDE);
	m_wndDrawingPlottingTabCtrl.ShowWindow(SW_HIDE);
	m_wndPDSUpdateTabCtrl.ShowWindow(SW_HIDE);
	m_wndBackupTabCtrl.ShowWindow(SW_HIDE);
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-27
    @class      CWorkspaceWnd
    @function   ShowIsoEditForm
    @return     void
    @param      bool    bShow
    @brief
******************************************************************************/
void CWorkspaceWnd::ShowIsoEditForm(const bool& bShow)
{
	m_wndQueryTabCtrl.ShowWindow(SW_HIDE);
	m_wndIsoEditTabCtrl.ShowWindow(SW_SHOW);
	m_wndDrawingIndexTabCtrl.ShowWindow(SW_HIDE);
	m_wndBMReportTabCtrl.ShowWindow(SW_HIDE);
	m_wndDrawingPlottingTabCtrl.ShowWindow(SW_HIDE);
	m_wndPDSUpdateTabCtrl.ShowWindow(SW_HIDE);
	m_wndBackupTabCtrl.ShowWindow(SW_HIDE);
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-27
    @class      CWorkspaceWnd
    @function   ShowBMReportForm
    @return     void
    @param      bool    bShow
    @brief
******************************************************************************/
void CWorkspaceWnd::ShowBOMReportForm(const bool& bShow)
{
	m_wndQueryTabCtrl.ShowWindow(SW_HIDE);
	m_wndIsoEditTabCtrl.ShowWindow(SW_HIDE);
	m_wndDrawingIndexTabCtrl.ShowWindow(SW_HIDE);
	m_wndBMReportTabCtrl.ShowWindow(SW_SHOW);
	m_wndDrawingPlottingTabCtrl.ShowWindow(SW_HIDE);
	m_wndPDSUpdateTabCtrl.ShowWindow(SW_HIDE);
	m_wndBackupTabCtrl.ShowWindow(SW_HIDE);
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-27
    @class
    @function   CWorkspaceWnd:ShowIndexReportForm
    @return     void
    @param      bool    bShow
    @brief
******************************************************************************/
void CWorkspaceWnd::ShowIndexReportForm(const bool& bShow)
{
	m_wndQueryTabCtrl.ShowWindow(SW_HIDE);
	m_wndIsoEditTabCtrl.ShowWindow(SW_HIDE);
	m_wndDrawingIndexTabCtrl.ShowWindow(SW_SHOW);
	m_wndBMReportTabCtrl.ShowWindow(SW_HIDE);
	m_wndDrawingPlottingTabCtrl.ShowWindow(SW_HIDE);
	m_wndPDSUpdateTabCtrl.ShowWindow(SW_HIDE);
	m_wndBackupTabCtrl.ShowWindow(SW_HIDE);
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-27
    @class      CWorkspaceWnd
    @function   ShowDrawingPlottingForm
    @return     void
    @param      bool    bShow
    @brief
******************************************************************************/
void CWorkspaceWnd::ShowDrawingPlottingForm(const bool& bShow)
{
	m_wndQueryTabCtrl.ShowWindow(SW_HIDE);
	m_wndIsoEditTabCtrl.ShowWindow(SW_HIDE);
	m_wndDrawingIndexTabCtrl.ShowWindow(SW_HIDE);
	m_wndBMReportTabCtrl.ShowWindow(SW_HIDE);
	m_wndDrawingPlottingTabCtrl.ShowWindow(SW_SHOW);
	m_wndPDSUpdateTabCtrl.ShowWindow(SW_HIDE);
	m_wndBackupTabCtrl.ShowWindow(SW_HIDE);
}

/******************************************************************************
	@brief
    @author     humkyung
    @date       2012-05-08
    @class      CWorkspaceWnd
    @function   ShowUpdatePDSForm
    @param      bool    bShow
******************************************************************************/
void CWorkspaceWnd::ShowUpdatePDSForm(const bool& bShow)
{
	m_wndQueryTabCtrl.ShowWindow(SW_HIDE);
	m_wndIsoEditTabCtrl.ShowWindow(SW_HIDE);
	m_wndDrawingIndexTabCtrl.ShowWindow(SW_HIDE);
	m_wndBMReportTabCtrl.ShowWindow(SW_HIDE);
	m_wndDrawingPlottingTabCtrl.ShowWindow(SW_HIDE);
	m_wndPDSUpdateTabCtrl.ShowWindow(SW_SHOW);
	m_wndBackupTabCtrl.ShowWindow(SW_HIDE);
}

/******************************************************************************
	@brief		show backup form
    @author     humkyung
    @date       2015.11.20
    @class      CWorkspaceWnd
    @param      const bool& bShow
******************************************************************************/
void CWorkspaceWnd::ShowBackupForm(const bool& bShow)
{
	m_wndQueryTabCtrl.ShowWindow(SW_HIDE);
	m_wndIsoEditTabCtrl.ShowWindow(SW_HIDE);
	m_wndDrawingIndexTabCtrl.ShowWindow(SW_HIDE);
	m_wndBMReportTabCtrl.ShowWindow(SW_HIDE);
	m_wndDrawingPlottingTabCtrl.ShowWindow(SW_HIDE);
	m_wndPDSUpdateTabCtrl.ShowWindow(SW_HIDE);
	m_wndBackupTabCtrl.ShowWindow(SW_SHOW);
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-27
    @class      CWorkspaceWnd
    @function   OnSize
    @return     void
    @param      UINT    nType
    @param      int     cx
    @param      int     cy
    @brief
******************************************************************************/
void CWorkspaceWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	if(m_wndQueryTabCtrl.GetSafeHwnd())
	{
		CRect rectTabAreaTop , rectTabAreaBottom;
		m_wndQueryTabCtrl.GetTabArea(rectTabAreaTop , rectTabAreaBottom);

		CRect rect;
		GetWindowRect(&rect);
		ScreenToClient(&rect);
		cy = rect.Height() - rectTabAreaTop.Height();
		m_wndQueryTabCtrl.SetWindowPos(NULL , 0 , 0 , cx , cy , SWP_NOZORDER);
	}

	if(m_wndIsoEditTabCtrl.GetSafeHwnd())
	{
		CRect rectTabAreaTop , rectTabAreaBottom;
		m_wndIsoEditTabCtrl.GetTabArea(rectTabAreaTop , rectTabAreaBottom);

		CRect rect;
		GetWindowRect(&rect);
		ScreenToClient(&rect);
		cy = rect.Height() - rectTabAreaTop.Height();
		m_wndIsoEditTabCtrl.SetWindowPos(NULL , 0 , 0 , cx , cy , SWP_NOZORDER);
	}

	if(m_wndDrawingIndexTabCtrl.GetSafeHwnd())
	{
		CRect rectTabAreaTop , rectTabAreaBottom;
		m_wndDrawingIndexTabCtrl.GetTabArea(rectTabAreaTop , rectTabAreaBottom);

		CRect rect;
		GetWindowRect(&rect);
		ScreenToClient(&rect);
		cy = rect.Height() - rectTabAreaTop.Height();
		m_wndDrawingIndexTabCtrl.SetWindowPos(NULL , 0 , 0 , cx , cy , SWP_NOZORDER);
	}

	if(m_wndBMReportTabCtrl.GetSafeHwnd())
	{
		CRect rectTabAreaTop , rectTabAreaBottom;
		m_wndBMReportTabCtrl.GetTabArea(rectTabAreaTop , rectTabAreaBottom);

		CRect rect;
		GetWindowRect(&rect);
		ScreenToClient(&rect);
		cy = rect.Height() - rectTabAreaTop.Height();
		m_wndBMReportTabCtrl.SetWindowPos(NULL , 0 , 0 , cx , cy , SWP_NOZORDER);
	}
	
	if(m_wndDrawingPlottingTabCtrl.GetSafeHwnd())
	{
		CRect rectTabAreaTop , rectTabAreaBottom;
		m_wndDrawingPlottingTabCtrl.GetTabArea(rectTabAreaTop , rectTabAreaBottom);

		CRect rect;
		GetWindowRect(&rect);
		ScreenToClient(&rect);
		cy = rect.Height() - rectTabAreaTop.Height();
		m_wndDrawingPlottingTabCtrl.SetWindowPos(NULL , 0 , 0 , cx , cy , SWP_NOZORDER);
	}

	/// 2012.05.08 added by humkyung
	if(m_wndPDSUpdateTabCtrl.GetSafeHwnd())
	{
		CRect rectTabAreaTop , rectTabAreaBottom;
		m_wndPDSUpdateTabCtrl.GetTabArea(rectTabAreaTop , rectTabAreaBottom);

		CRect rect;
		GetWindowRect(&rect);
		ScreenToClient(&rect);
		cy = rect.Height() - rectTabAreaTop.Height();
		m_wndPDSUpdateTabCtrl.SetWindowPos(NULL , 0 , 0 , cx , cy , SWP_NOZORDER);
	}
	/// up to here

	if(m_wndBackupTabCtrl.GetSafeHwnd())
	{
		CRect rectTabAreaTop , rectTabAreaBottom;
		m_wndBackupTabCtrl.GetTabArea(rectTabAreaTop , rectTabAreaBottom);

		CRect rect;
		GetWindowRect(&rect);
		ScreenToClient(&rect);
		cy = rect.Height() - rectTabAreaTop.Height();
		m_wndBackupTabCtrl.SetWindowPos(NULL , 0 , 0 , cx , cy , SWP_NOZORDER);
	}
}
