// GetPds.cpp : implementation file
//

#include "stdafx.h"
#include "UserApp.h"
#include "MainFrm.h"
#include "UserAppView.h"

#include <tokenizer.h>
#include "GetPds.h"
#include <ado/SQLFile.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGetPds

IMPLEMENT_DYNCREATE(CGetPds, CFormView)

CGetPds::CGetPds()
	: CFormView(CGetPds::IDD) ,m_pUserAppDoc(NULL) , m_pLineDataForm(NULL) , m_pBoldFont(NULL)
{
	//{{AFX_DATA_INIT(CGetPds)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CGetPds::~CGetPds()
{
	try
	{
		SAFE_DELETE( m_pBoldFont );
	}
	catch(...)
	{
		
	}
}

void CGetPds::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGetPds)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGetPds, CFormView)
	//{{AFX_MSG_MAP(CGetPds)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_GET_PDS, OnKeydownListQualifiedLine)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGetPds diagnostics

#ifdef _DEBUG
void CGetPds::AssertValid() const
{
	CFormView::AssertValid();
}

void CGetPds::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGetPds message handlers

BOOL CGetPds::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

/**
	@brief	MAP과 LIST의 내용을 CLEAR한다.

	@author	humkyung
**/
void CGetPds::ClearContents()
{
	m_ItemMap.clear();
	m_ReverseItemMap.clear();
	
	if(m_GetPdsReport.GetSafeHwnd())
	{
		m_GetPdsReport.SetRedraw(FALSE);
		m_GetPdsReport.DeleteAllItems();
		m_GetPdsReport.SetRedraw(TRUE);
	}
}

void CGetPds::InitContents()
{
	CAppDocData* pDocData = CAppDocData::GetInstance();

	if(NULL != m_pUserAppDoc)
	{
		CString rIniFilePath = pDocData->GetProject()->GetIniFilePath();
		
		char szBuf[MAX_PATH] = {'\0' ,};
		
		CString rApp = _T("Line Data Field");
		int nCount = 0;
		CString rSqlFilePath = CAppDocData::GetExecPath() + _T("\\Setting\\T_ISO_LINE_DATA.XML");
		CSQLFile sqlFile(rSqlFilePath.operator LPCSTR());
		if(ERROR_SUCCESS == sqlFile.ParseXML(rSqlFilePath.operator LPCTSTR()))
		{
			nCount = sqlFile.GetColumnCount();
			vector<CString> HeadingEntry;
			vector<string> oResult; 
			for(int i = 0;i < nCount;++i)
			{
				CString rKey;
				rKey.Format("Display%d" , i);
				if(GetPrivateProfileString(rApp , rKey , NULL , szBuf , MAX_PATH , rIniFilePath))
				{
					if(CString("True") != szBuf) continue;
					
					CString rHeading;
					rHeading += sqlFile.GetFieldNameAt(i).c_str() + CString(",150");
					HeadingEntry.push_back(rHeading);
				}
			}
			
			CString rHeadings;
			for(vector<CString>::iterator itr = HeadingEntry.begin();itr != HeadingEntry.end();itr++)
			{
				rHeadings += (*itr);
				if((itr + 1) != HeadingEntry.end()) rHeadings += ";";
			}
			m_GetPdsReport.SetHeadings(rHeadings);
			m_GetPdsReport.SetGridLines();
		}else	AfxMessageBox("Can't find T_ISO_LINE_DATA.XML file");
	}
}

void CGetPds::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	static bool __init = false;
	
	if(false == __init)
	{
		m_ImageList.Create(IDB_USER_APP , 16 , 16 , RGB(255 , 0 , 255));

		CMainFrame* pMainFrame = GetUserAppMainWnd();
		//////////////////////////////////////////////////////////////////////////
		CUserAppView* pUserAppView = (CUserAppView*)(CUserAppView::GetInstance());
		CAppDocData* pDocData  = CAppDocData::GetInstance();

		m_pLineDataForm = pUserAppView->m_pLineDataForm;
		///m_pPdsLineDataForm = pUserAppView->m_pPDSLineDataForm;

		m_pBoldFont = new CFont;
		m_pBoldFont->CreateFont(18,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0 , _T("Arial"));

		m_GetPdsStatic.SubclassWindow(GetDlgItem(IDC_STATIC_GET_PDS)->m_hWnd);
		m_GetPdsStatic.SetGradientColor( GetSysColor(COLOR_BTNFACE) );
		m_GetPdsStatic.SetColor( DarkenColor(GetSysColor(COLOR_BTNFACE),0.15) );
		m_GetPdsStatic.SetVerticalGradient(); //set gradient to be vertical
		m_GetPdsStatic.SetFont(m_pBoldFont);
		m_GetPdsStatic.SetTextColor(RGB(0 , 0 , 0));
		m_GetPdsStatic.SetTextAlign(DT_CENTER);

		m_GetPdsReport.SubclassWindow(GetDlgItem(IDC_LIST_GET_PDS)->m_hWnd);
		m_GetPdsReport.SetImageList(&m_ImageList , LVSIL_SMALL);
		m_GetPdsReport.SetHeaderImageList(); 

		__init = true;
	}
}

void CGetPds::AddLineDataRecordEntry(vector<LineDataRecordMap>* pLineDataRecordEntry)
{
	assert(pLineDataRecordEntry && "pLineDataRecordEntry is NULL");

	if(pLineDataRecordEntry)
	{
		m_GetPdsReport.SetRedraw(FALSE);
		
		LVITEM lvitem;
		lvitem.mask = LVIF_TEXT | LVIF_IMAGE;
		lvitem.iImage = 1;
		lvitem.cchTextMax = 32;
		
		const size_t nRecordCount = pLineDataRecordEntry->size();
		int nItem = m_GetPdsReport.GetItemCount();
		char szBuf[MAX_PATH] = {'\0' ,};
		for(size_t i = 0;i < nRecordCount;i++)
		{		
			CLineDataRecord* pLineDataRecord = (*pLineDataRecordEntry)[i].pLineDataRecord;
			if(pLineDataRecord)
			{
				CString rId;
				rId.Format("%d" , pLineDataRecord->index());

				const int item = (*pLineDataRecordEntry)[i].nItem;
				map<int , int>::iterator where = m_ItemMap.find(item);
				if((m_ItemMap.end() == where) && !pLineDataRecord->IsDeleted())
				{
					lvitem.iItem = nItem++;
					lvitem.iSubItem = 0;
					lvitem.pszText  = (char*)(rId.operator const char*());
					int it = m_GetPdsReport.InsertItem(&lvitem);
					UpdateLineDataRecord(it , pLineDataRecord);

					m_ItemMap[(*pLineDataRecordEntry)[i].nItem] = it;
					m_ReverseItemMap[it] = (*pLineDataRecordEntry)[i].nItem;
				}
			}
		}
		m_GetPdsReport.SetRedraw(TRUE);
	}
}

void CGetPds::UpdateLineDataRecord(const int &nItem, CLineDataRecord *pLineDataRecord)
{
	assert((nItem > -1) && "item range error");
	assert(pLineDataRecord && "pLineDataRecord is NULL");
	
	if((nItem > -1) && pLineDataRecord)
	{
		char szBuf[MAX_PATH] = {'\0' ,};
		m_GetPdsReport.SetItemData(nItem , (DWORD)(pLineDataRecord));
		
		const int nColumnCount = m_GetPdsReport.GetColumnCount();
		for(int k = 0;k < nColumnCount;k++)
		{
			LVCOLUMN lvcolumn;
			lvcolumn.mask = LVCF_TEXT;
			lvcolumn.cchTextMax = MAX_PATH;
			lvcolumn.pszText = szBuf;
			m_GetPdsReport.GetColumn(k , &lvcolumn);
			
			const int nFieldCount = pLineDataRecord->GetFieldCount();
			for(int j = 0;j < nFieldCount;j++)
			{
				CDataField* pLineDataField = pLineDataRecord->GetFieldAt(j);
				if(CString(lvcolumn.pszText) == pLineDataField->desc())
				{
					CString rValue = pLineDataField->value();
					rValue.Replace("\n" , " ");
					
					m_GetPdsReport.SetItemText(nItem , k , rValue);
					break;
				}
			}
		}
		int nCount = m_GetPdsReport.GetItemCount();
		ShowSelectedCount(nCount);
	}
}

void CGetPds::GetSelectedLineDataRecord(vector<LineDataRecordMap>& LineDataRecordEntry)
{
	LineDataRecordMap lineDataRecordMap;
	POSITION pos = m_GetPdsReport.GetFirstSelectedItemPosition();
	while(pos)
	{
		lineDataRecordMap.nItem = m_GetPdsReport.GetNextSelectedItem(pos);
		lineDataRecordMap.pLineDataRecord = (CLineDataRecord*)(m_GetPdsReport.GetItemData(lineDataRecordMap.nItem));
		
		if(lineDataRecordMap.pLineDataRecord) LineDataRecordEntry.push_back(lineDataRecordMap);
	}
}

/**
	@brief

	@author	humkyung
**/
void CGetPds::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	CRect rect;
	if(m_GetPdsStatic.GetSafeHwnd())
	{
		m_GetPdsStatic.GetWindowRect(&rect);
		ScreenToClient(&rect);

		m_GetPdsStatic.SetWindowPos(NULL , 0 , 0 , cx , rect.Height() , SWP_NOMOVE);
	}
	
	if(m_GetPdsReport.GetSafeHwnd())
	{
		m_GetPdsReport.SetWindowPos(NULL , 0 , (int)(25), cx, (int)(cy - rect.Height() - 20) , SWP_NOZORDER);
		
		CWnd* pWnd = GetDlgItem(IDC_STATIC_SELECTED_INFO);
		if(pWnd)
		{
			CRect rectThis;
			pWnd->GetWindowRect(&rectThis);
			ScreenToClient(&rectThis);
			
			m_GetPdsReport.GetWindowRect(&rect);
			ScreenToClient(&rect);
			
			pWnd->SetWindowPos(NULL, rect.left, rect.bottom , 200, 20, SWP_NOZORDER);
		}	
		pWnd = GetDlgItem(IDC_STATIC_SELECTED_COUNT);
		if(pWnd)
		{
			CRect rectThis;
			pWnd->GetWindowRect(&rectThis);
			ScreenToClient(&rectThis);
			
			CWnd* pWnd1 = GetDlgItem(IDC_STATIC_SELECTED_INFO);
			if(pWnd1) pWnd1->GetWindowRect(&rect);
			ScreenToClient(&rect);
			
			pWnd->SetWindowPos(NULL, rect.right +5, rect.top, 50, rect.Height(), SWP_NOZORDER);
		}
	}
}

void CGetPds::OnKeydownListQualifiedLine(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	if(VK_DELETE == pLVKeyDow->wVKey)
	{
		DeleteQualifiedLineList();
		int nCount = m_GetPdsReport.GetItemCount();
		ShowSelectedCount(nCount);
	}

	*pResult = 0;
}

void CGetPds::ShowSelectedCount(int nCount)
{
	/*int nRecordCount = m_pPdsLineDataForm->m_nRecordCount;
	CWnd* pWnd = GetDlgItem(IDC_STATIC_SELECTED_COUNT);
	if(pWnd)
	{
		CString rCount, rSlash =" / ", rRecodrCount;
		rCount.Format("%d",nCount);
		rRecodrCount.Format("%d",nRecordCount);
		rCount = rCount + rSlash + rRecodrCount;
		pWnd->SetWindowText((LPCTSTR)rCount);
	}*/
}

void CGetPds::DeleteQualifiedLineList()
{
	vector<int> selectedItemEntry;
	
	POSITION pos = m_GetPdsReport.GetFirstSelectedItemPosition();
	while(pos)
	{
		int nItem = m_GetPdsReport.GetNextSelectedItem(pos);
		selectedItemEntry.push_back(nItem);
	}
	
	for(vector<int>::iterator itr = selectedItemEntry.begin();itr != selectedItemEntry.end();itr++)
	{
		map<int , int>::iterator where = m_ReverseItemMap.find(*itr);
		if(m_ReverseItemMap.end() != where)
		{
			map<int , int>::iterator at = m_ItemMap.find(where->second);
			if(m_ItemMap.end() != at)
			{
				m_ItemMap.erase(at);
			}
			
			m_ReverseItemMap.erase(where);
		}
	}
	
	m_GetPdsReport.SetRedraw(FALSE);
	m_GetPdsReport.DeleteAllSelectedItems();
	m_GetPdsReport.SetRedraw(TRUE);
}

