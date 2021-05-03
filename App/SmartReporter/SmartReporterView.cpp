
// SmartReporterView.cpp : implementation of the CSmartReporterView class
//

#include "stdafx.h"
#include "SmartReporter.h"

#include "SmartReporterDoc.h"
#include "SmartReporterView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSmartReporterView

IMPLEMENT_DYNCREATE(CSmartReporterView, CView)

BEGIN_MESSAGE_MAP(CSmartReporterView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CSmartReporterView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_HELPINFO()
END_MESSAGE_MAP()

// CSmartReporterView construction/destruction

CSmartReporterView::CSmartReporterView()
{
	// TODO: add construction code here

}

CSmartReporterView::~CSmartReporterView()
{
}

BOOL CSmartReporterView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CSmartReporterView drawing

void CSmartReporterView::OnDraw(CDC* /*pDC*/)
{
	CSmartReporterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CSmartReporterView printing


void CSmartReporterView::OnFilePrintPreview()
{
	AFXPrintPreview(this);
}

BOOL CSmartReporterView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CSmartReporterView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CSmartReporterView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CSmartReporterView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CSmartReporterView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}


// CSmartReporterView diagnostics

#ifdef _DEBUG
void CSmartReporterView::AssertValid() const
{
	CView::AssertValid();
}

void CSmartReporterView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSmartReporterDoc* CSmartReporterView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSmartReporterDoc)));
	return (CSmartReporterDoc*)m_pDocument;
}
#endif //_DEBUG


// CSmartReporterView message handlers

void CSmartReporterView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	GetParentFrame()->RecalcLayout();
	///ResizeParentToFit();
}

int CSmartReporterView::OnCreate(LPCREATESTRUCT lpCreateStruct)
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

	/*m_pWndOraDataReportView = new COraDataReportView();
	if(NULL != m_pWndOraDataReportView)
	{
		CCreateContext context;
		context.m_pNewViewClass = RUNTIME_CLASS(COraDataReportView);
		context.m_pCurrentDoc = GetDocument();
		if(!m_pWndOraDataReportView->Create(NULL , NULL , WS_CHILD | WS_VISIBLE | WS_BORDER , CRect(0 , 0 , 0 , 0) , this , CLineListConvertDlg::IDD + 1 , &context))
		{
			delete m_pWndOraDataReportView;
			return -1;
		}
		m_pWndOraDataReportView->ShowWindow(SW_SHOW);
	}*/

	return 0;
}

/**
	@brief	load data

	@author	humkyung

	@date	????.??.??

	@param

	@return
**/
void CSmartReporterView::LoadData()
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

/**
	@brief	create custom data report dialog which is not already created.

	@author	humkyung

	@date	2011.05.10

	@param	pCustomReportTable

	@return
**/
int CSmartReporterView::SyncTabCtrlWithCustomReportTableList()
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
	@brief	create custom data report dialog with custom data table

	@author	humkyung

	@date	2011.05.10

	@param	pCustomReportTable

	@return
**/
int CSmartReporterView::CreateCustomDataReportDialog(CCustomReportTable* pCustomReportTable)
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
	@brief

	@author	humkyung

	@date	????.??.??

	@param	nType
	@param	cx
	@param	cy

	@return
*/
void CSmartReporterView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if(m_wndTab.GetSafeHwnd())
	{
		m_wndTab.SetWindowPos(NULL ,  0 , 0 , cx , cy , SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

/******************************************************************************
    @author     humkyung
    @date       2012-06-03
    @function   OnHelpInfo
    @return     BOOL
    @param      HELPINFO*   pHelpInfo
    @brief
******************************************************************************/
BOOL CSmartReporterView::OnHelpInfo(HELPINFO* pHelpInfo)
{
	CString sFilePath = STRING_T(CLineListToOraDocData::GetExecPath() + _T("\\Help\\SmartISO.chm::/Smart Reporter.htm")).c_str();
	HWND hHtml = ::HtmlHelp(this->m_hWnd ,  sFilePath , HH_DISPLAY_TOPIC , 0);

	return TRUE;///CView::OnHelpInfo(pHelpInfo);
}
