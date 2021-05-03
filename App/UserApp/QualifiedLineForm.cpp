// QualifiedLineForm.cpp : implementation file
//

#include "stdafx.h"
#include "UserApp.h"
#include "MainFrm.h"
#include "UserAppDocData.h"
#include "UserAppView.h"
#include <BCMenu.h>

#include <DgnDocument.h>
#include <DgnText.h>
#include <Tokenizer.h>
#include "QualifiedLineForm.h"
#include "WorkStatusDlg.h"

#include "ResourceSwitch.h"
#include "IsoEditSettingPage.h"

#include "PdTable121.h"
#include "pdTable122.h"
#include <ado/SQLFile.h>
#include <util/FileTools.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static bool __MODIFY_THE_LINE_DATA__ = false;
/////////////////////////////////////////////////////////////////////////////
// CQualifiedLineForm

IMPLEMENT_DYNCREATE(CQualifiedLineForm, CFormView)

CQualifiedLineForm::CQualifiedLineForm()
	: CFormView(CQualifiedLineForm::IDD) , m_pUserAppDoc(NULL) , m_pLineDataForm(NULL) , m_pBoldFont(NULL)
{
	//{{AFX_DATA_INIT(CQualifiedLineForm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CQualifiedLineForm::~CQualifiedLineForm()
{
	try
	{
		SAFE_DELETE(m_pBoldFont);
		m_pBoldFont = NULL;
	}
	catch(...)
	{

	}
}

void CQualifiedLineForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQualifiedLineForm)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CQualifiedLineForm, CFormView)
	//{{AFX_MSG_MAP(CQualifiedLineForm)
	ON_WM_SIZE()
	ON_NOTIFY(NM_RCLICK, IDC_LIST_QUALIFIED_LINE, OnRclickListQualifiedLine)
	ON_COMMAND(ID_LINE_DATA_PRINT, OnLineDataPrint)
	ON_COMMAND(ID_LINE_DATA_MODIFY, OnLineDataModify)
	///ON_COMMAND(ID_LINE_DATA_COPY, OnLineDataCopy)
	ON_COMMAND(ID_LINE_DATA_DELETE, OnLineDataDelete)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_QUALIFIED_LINE, OnDblclkListQualifiedLine)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_QUALIFIED_LINE, OnKeydownListQualifiedLine)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQualifiedLineForm diagnostics

#ifdef _DEBUG
void CQualifiedLineForm::AssertValid() const
{
	CFormView::AssertValid();
}

void CQualifiedLineForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CQualifiedLineForm message handlers

BOOL CQualifiedLineForm::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

//just a helper function to create nicer fade effect.

void CQualifiedLineForm::OnInitialUpdate() 
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

		m_pBoldFont = new CFont;
		m_pBoldFont->CreateFont(18,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0,"Arial");

		m_QualifedLineReport.SubclassWindow(GetDlgItem(IDC_LIST_QUALIFIED_LINE)->m_hWnd);
		m_QualifedLineReport.SetImageList(&m_ImageList , LVSIL_SMALL);
		m_QualifedLineReport.SetHeaderImageList(); 

		__init = true;
	}
}

/**
	@brief	
	@author	humkyung
**/
void CQualifiedLineForm::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	CRect rect;
	rect.SetRectEmpty();
	if(m_QualifedLineReport.GetSafeHwnd())
	{
		m_QualifedLineReport.SetWindowPos(NULL , 0 , (int)(25), cx, (int)(cy - rect.Height() - 20) , SWP_NOZORDER);
	}

	/*
	if(::IsWindow(m_getDataFromIsoDwgButton.m_hWnd))
	{
		m_QualifedLineReport.GetWindowRect(&rect);
		ScreenToClient(&rect);

		m_getDataFromIsoDwgButton.SetWindowPos(NULL , rect.left , rect.bottom , 0 , 0 , SWP_NOSIZE);
	}
	*/
	
	CWnd* pWnd = GetDlgItem(IDC_STATIC_SELECTED_INFO);
	if(pWnd)
	{
		CRect rectThis;
		pWnd->GetWindowRect(&rectThis);
		ScreenToClient(&rectThis);

		pWnd->SetWindowPos(NULL, 5 , 5, 100 , rectThis.Height() , SWP_NOZORDER | SWP_NOSIZE);
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
		
		pWnd->SetWindowPos(NULL, rect.right +5, rect.top , 100 , rect.Height(), SWP_NOZORDER);
	}
}

/**	@brief	project가 로드되고 난 뒤 control들의 내용을 초기화 시킨다.

	@author	HumKyung.BAEK
**/
void CQualifiedLineForm::InitContents()
{
	if(NULL != m_pUserAppDoc)
	{
		CAppDocData* pDocData = CAppDocData::GetInstance();

		CString rIniFilePath = pDocData->GetProject()->GetIniFilePath();
		
		const CString rApp = "Line Data Field";
		char szBuf[MAX_PATH] = {'\0' ,};
		
		CString rSqlFilePath = CAppDocData::GetExecPath() + _T("\\Setting\\T_ISO_LINE_DATA.XML");
		CSQLFile sqlFile(rSqlFilePath.operator LPCSTR());
		if(ERROR_SUCCESS == sqlFile.ParseXML(rSqlFilePath.operator LPCTSTR()))
		{
			int nCount = sqlFile.GetColumnCount();
			vector<CString> HeadingEntry;
			vector<string> oResult; 
			for(int i = 0;i < nCount;++i)
			{
				CString rKey;
				rKey.Format(_T("Display%d") , i);
				if(GetPrivateProfileString(rApp , rKey , NULL , szBuf , MAX_PATH , rIniFilePath))
				{
					if(CString("True") != szBuf) continue;
					
					CString rHeading;
					rHeading += sqlFile.GetFieldNameAt(i).c_str() + CString(_T(",100"));
					HeadingEntry.push_back(rHeading);
				}
			}
			
			CString rHeadings;
			for(vector<CString>::iterator itr = HeadingEntry.begin();itr != HeadingEntry.end();itr++)
			{
				rHeadings += (*itr);
				if((itr + 1) != HeadingEntry.end()) rHeadings += _T(";");
			}
			m_QualifedLineReport.SetHeadings(rHeadings);
			m_QualifedLineReport.SetGridLines();
		}else	AfxMessageBox("Can't find T_ISO_LINE_DATA.XML file");
	}
}

void CQualifiedLineForm::OnRclickListQualifiedLine(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CPoint pos;
	GetCursorPos(&pos);
	//ScreenToClient(&pos);

	BCMenu menu;
	menu.LoadMenu(IDR_LINE_DATA);
	menu.LoadToolbar(IDR_LINE_DATA);
	CMenu* pMenu = menu.GetSubMenu(0);
	
	const int nSelectedItemCount = m_QualifedLineReport.GetSelectedItemCount();
	if(nSelectedItemCount > 0)
	{
		pMenu->EnableMenuItem(ID_LINE_DATA_PRINT , FALSE);
		pMenu->EnableMenuItem(ID_LINE_DATA_COPY  , FALSE);
		pMenu->EnableMenuItem(ID_LINE_DATA_MODIFY, FALSE);
		pMenu->EnableMenuItem(ID_LINE_DATA_DELETE, FALSE);
	}
	else
	{
		pMenu->EnableMenuItem(ID_LINE_DATA_PRINT , TRUE);
		pMenu->EnableMenuItem(ID_LINE_DATA_COPY  , TRUE);
		pMenu->EnableMenuItem(ID_LINE_DATA_MODIFY, TRUE);
		pMenu->EnableMenuItem(ID_LINE_DATA_DELETE, TRUE);
	}

	pMenu->EnableMenuItem(ID_LINE_DATA_INSERT, TRUE);
	pMenu->EnableMenuItem(ID_LINE_DATA_UNDELETE, TRUE);
	pMenu->EnableMenuItem(ID_LINE_DATA_QUALIFY , TRUE);
	
	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON , pos.x , pos.y , this);

	*pResult = 0;
}

/**
	\brief	

	\author	HumKyung.BAEK
**/
void CQualifiedLineForm::ClearContents()
{
	m_ItemMap.clear();
	m_ReverseItemMap.clear();
	m_QualifedLineReport.DeleteAllItems();
}

/**	@brief	리스트에 line data record를 추가한다.

	@param	pLineDataRecordEntry	a parameter of type vector<LineDataRecordMap>*

	@remarks\n
	ItemMap에 없는 line data record만을 리스트에 추가한다.

	@return	void	
*/
void CQualifiedLineForm::AddLineDataRecordEntry(vector<LineDataRecordMap>* pLineDataRecordEntry)
{
	assert(pLineDataRecordEntry && "pLineDataRecordEntry is NULL");

	if(pLineDataRecordEntry)
	{
		m_QualifedLineReport.SetRedraw(FALSE);
		
		LVITEM lvitem;
		lvitem.mask = LVIF_TEXT | LVIF_IMAGE;
		lvitem.iImage = 1;
		lvitem.cchTextMax = 32;
		
		const size_t nRecordCount = pLineDataRecordEntry->size();
		int nItem = m_QualifedLineReport.GetItemCount();
		char szBuf[MAX_PATH] = {'\0' ,};
		for(int i = 0;i < int(nRecordCount);i++)
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
					int it = m_QualifedLineReport.InsertItem(&lvitem);
					UpdateLineDataRecord(it , pLineDataRecord);

					m_ItemMap[(*pLineDataRecordEntry)[i].nItem] = it;
					m_ReverseItemMap[it] = (*pLineDataRecordEntry)[i].nItem;
				}
			}
		}
		m_QualifedLineReport.SetRedraw(TRUE);
	}
}

/**
	@brief	
	
	@author	HumKyung.BAEk
**/
void CQualifiedLineForm::UpdateLineDataRecord(const int &nItem, CLineDataRecord *pLineDataRecord)
{
	assert((nItem > -1) && "item range error");
	assert(pLineDataRecord && "pLineDataRecord is NULL");
	
	if((nItem > -1) && pLineDataRecord)
	{
		char szBuf[MAX_PATH] = {'\0' ,};
		m_QualifedLineReport.SetItemData(nItem , (DWORD)(pLineDataRecord));
		
		const int nColumnCount = m_QualifedLineReport.GetColumnCount();
		for(int k = 0;k < nColumnCount;k++)
		{
			LVCOLUMN lvcolumn;
			lvcolumn.mask = LVCF_TEXT;
			lvcolumn.cchTextMax = MAX_PATH;
			lvcolumn.pszText = szBuf;
			m_QualifedLineReport.GetColumn(k , &lvcolumn);
		
			CDataField* pField = pLineDataRecord->FindWithFieldName(lvcolumn.pszText);
			if(pField)
			{
				CString rValue = pField->value();
				rValue.Replace("\n" , " ");
				
				m_QualifedLineReport.SetItemText(nItem , k , rValue);
			}
		}
		int nCount = m_QualifedLineReport.GetItemCount();
		ShowSelectedCount(nCount);
	}
}

void CQualifiedLineForm::OnLineDataPrint() 
{
	if(m_pLineDataForm)
	{
		vector<LineDataRecordMap> LineDataRecordEntry;
		GetSelectedLineDataRecord(LineDataRecordEntry);
		
		m_pLineDataForm->PrintLineDataRecordEntry(&LineDataRecordEntry);
	}
}

/**	@brief	The CQualifiedLineForm::GetAllLineDataRecord function
	
	@author	HumKyung

	@date	????.??.??

	@param	lineDataRecordEntry	a parameter of type vector<LineDataRecordMap>&

	@return	void	
*/
void CQualifiedLineForm::GetAllLineDataRecord(vector<LineDataRecordMap>& lineDataRecordEntry)
{
	LineDataRecordMap lineDataRecordMap;
	const int nItemCount = m_QualifedLineReport.GetItemCount();
	for( int i = 0;i < nItemCount;++i)
	{
		lineDataRecordMap.nItem = i;
		lineDataRecordMap.pLineDataRecord = (CLineDataRecord*)(m_QualifedLineReport.GetItemData(lineDataRecordMap.nItem));

		if(lineDataRecordMap.pLineDataRecord) lineDataRecordEntry.push_back(lineDataRecordMap);
	}
}

/**	
	@brief	The CQualifiedLineForm::GetSelectedLineDataRecord function
	
	@author	humkyung

	@date	????.??.??

	@param	LineDataRecordEntry	a parameter of type vector<LineDataRecordMap>&

	@return	void	
*/
void CQualifiedLineForm::GetSelectedLineDataRecord(vector<LineDataRecordMap>& LineDataRecordEntry)
{
	LineDataRecordMap lineDataRecordMap;
	POSITION pos = m_QualifedLineReport.GetFirstSelectedItemPosition();
	while(pos)
	{
		lineDataRecordMap.nItem = m_QualifedLineReport.GetNextSelectedItem(pos);
		lineDataRecordMap.pLineDataRecord = (CLineDataRecord*)(m_QualifedLineReport.GetItemData(lineDataRecordMap.nItem));
		
		if(lineDataRecordMap.pLineDataRecord) LineDataRecordEntry.push_back(lineDataRecordMap);
	}
}

struct HasSameIndex : public binary_function<CLineDataRecord* , CString , bool>
{
	bool operator()(const CLineDataRecord* lhs , const CString& rhs) const
	{
		return (lhs->index() == rhs);
	}
};

/**	\brief	line data를 편집합니다.
	\author	백흠경

	\return	void	
*/
void CQualifiedLineForm::OnLineDataModify() 
{
}

/**	
	@brief	선택된 line data record를 리스트에서 제거한다.
	
	@remarks\n
	ItemMap에서도 line data record의 내용을 제거한다.

	@author	HumKyung

	@date	????.??.??

	@param	
	
	@return	void	
*/
void CQualifiedLineForm::OnLineDataDelete() 
{
	vector<LineDataRecordMap> lineDataRecordMapEntry;
	GetAllLineDataRecord(lineDataRecordMapEntry);
	if(lineDataRecordMapEntry.size() > 0)
	{
		m_pLineDataForm->DeleteLineDataRecordEntry(&lineDataRecordMapEntry);

		m_QualifedLineReport.SetRedraw(FALSE);
		m_QualifedLineReport.DeleteAllItems();
		for(vector<LineDataRecordMap>::iterator itr = lineDataRecordMapEntry.begin();itr != lineDataRecordMapEntry.end();itr++)
		{
			int nItem = m_ReverseItemMap[itr->nItem];
			m_ItemMap.erase(nItem);
			m_ReverseItemMap.erase(itr->nItem);
		}

		//////////////////////////////////////////////////////////////////////////
		/// re-index
		int nItem = m_QualifedLineReport.GetItemCount();
		for(int i = 0;i < nItem;i++)
		{
			CString rId;
			rId.Format(_T("%d") , i);
			m_QualifedLineReport.SetItemText(i , 0 , rId);
		}
		m_QualifedLineReport.SetRedraw(TRUE);
	}
	else
	{
		CString rMsg;
		rMsg.LoadString(IDS_LINE_DATA_FORM_MSG1);
		AfxMessageBox(rMsg , MB_OK | MB_ICONWARNING);
	}
}

void CQualifiedLineForm::OnDblclkListQualifiedLine(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	
	*pResult = 0;
}

#define	UPDATE_FROM_ISO_DWG	0
#define	UPDATE_FROM_IDF		1

static int SWITCH = UPDATE_FROM_ISO_DWG;

UINT CQualifiedLineForm::StatusThreadEntry(LPVOID pVoid)
{
	///return (UPDATE_FROM_ISO_DWG == SWITCH) ? ((CQualifiedLineForm*)pVoid)->StatusISOThread() : ((CQualifiedLineForm*)pVoid)->StatusIDFThread();
	return IDOK;
}

static vector<string> ERROR_ENTRY;

/**	
	@brief	쓰레드 실행부, IDF 파일에서 하나의 3D 좌표를 읽어와 122테이블에서 DWG NO.를 구한다.

	@remarks

  	@author	BHK,KHS

**/
UINT CQualifiedLineForm::StatusIDFThread()
{
	CWorkStatusDlg* pWorkStatusDlg = CWorkStatusDlg::GetInstance();
	try
	{
		InterlockedExchange((LONG*)(&(pWorkStatusDlg->m_bThreadRunning)) , TRUE);// so the main thread knows that this thread is still running
		
		CAppDocData* pDocData = CAppDocData::GetInstance();
		map<CString , CLineDataRecord*> IDFFileNameMap;
		vector<CString> inputIDFFileNameEntry;
		const int nCount = m_QualifedLineReport.GetItemCount();

		int i = 0;
		for(i = 0;i < nCount;i++)
		{
			CLineDataRecord* pLineDataRecord = (CLineDataRecord*)m_QualifedLineReport.GetItemData(i);
			CString rInputIsoFileName = pDocData->GetProject()->GetInputIsoFileName(pLineDataRecord);
			CString rInputIDFFileName = rInputIsoFileName;

			inputIDFFileNameEntry.push_back(rInputIDFFileName);
			IDFFileNameMap[rInputIDFFileName] = pLineDataRecord;
		}
		
		CString rIDFFileFolder = pDocData->m_rIDFFileFolder;
		if(_T("\\") != rIDFFileFolder.Right(1)) rIDFFileFolder += _T("\\");
		const size_t nIDFCount = inputIDFFileNameEntry.size();
		for(vector<CString>::iterator itr = inputIDFFileNameEntry.begin();itr != inputIDFFileNameEntry.end();++itr)
		{
			CString rIDFFilePath = rIDFFileFolder + (*itr) + _T(".IDF");
			if(CFileTools::DoesFileExist(rIDFFilePath))
			{
				CString rTitle = rIDFFilePath;
				double dProcess = 0.f;

				CIDFFile& idf = CIDFFile::GetInstance();
				idf.Read(rIDFFilePath.operator LPCTSTR());
				double  x = 0 , y = 0 , z = 0;
				set<string> DwgIndexNoEntry;
	
				/// IDF 파일에서 3D 좌표를 가져온다.
				vector<CIsPoint3d> Point3dList;
				if(ERROR_SUCCESS == idf.GetCoodinateList(Point3dList))
				{
					CPdTable122& _122Table_ = CPdTable122::GetInstance();
					
					set<string> DrawingTitleEntry;

					/// 3D좌표로 122 테이블에서 dwg_index_no를 가져온다.
					int nDwgIndexNoCount = 0;
					for(vector<CIsPoint3d>::iterator jtr = Point3dList.begin();jtr != Point3dList.end();++jtr)
					{
						 nDwgIndexNoCount =_122Table_.FindDwgIndexNo(DwgIndexNoEntry, jtr->x() , jtr->y() , jtr->z());
						 if(0 != nDwgIndexNoCount)
						 {
							 ///ofile << jtr->x() << "," << jtr->y() << "," << jtr->z() << std::endl;
							 break;
						 }
					}
					
					if(0 != nDwgIndexNoCount)
					{
						CPdTable121& _121Table_ = CPdTable121::GetInstance();

						//! dwg_index_no로 121 테이블에서 drawing_title 를 가져온다.
						const int nDrawingTitleCount = _121Table_.FindDrawingTitle( DrawingTitleEntry, DwgIndexNoEntry);
						if(0 != nDrawingTitleCount) 
						{
							for(set<string>::iterator jtr = DrawingTitleEntry.begin();jtr != DrawingTitleEntry.end();++jtr)
							{
								CLineDataRecord* pLineDataRecord = IDFFileNameMap[itr->operator LPCTSTR()];
								if(NULL != pLineDataRecord)
								{
									const size_t nFieldCount = pLineDataRecord->GetFieldCount();
									for(int i = 0;i < int(nFieldCount);i++)
									{
										CDataField* pField = pLineDataRecord->GetFieldAt(i);
										const CString C_PLAN_NO = _T("PLAN_NO");
										if(C_PLAN_NO == pField->name())
										{
											CString rName(pField->name());
											CString rDesc(pField->desc());
											CString rValue(jtr->c_str());
											pField->Set(rName , rDesc , rValue);
										}
									}
								}
							}
						}	
					}
					else
					{
						//! 2011.06.23 - message is modified by humkyung
						//! 2011.07.12 - insert message into error list instead of showing message box
						ERROR_ENTRY.push_back( _T("도면을 찾을 수 없습니다") ); 
					}
				}

				//! UPDATE PROGRESS BAR
				dProcess++;
				int process = (int)((dProcess / nCount) * 100.f);
				CWorkStatusDlg::UpdateWorkStatus(rTitle , process);
			}
			else
			{
				ERROR_ENTRY.push_back(CString((*itr) + ".IDF 파일을 찾을 수 없습니다. '").operator LPCSTR());
				///InterlockedExchange((LONG*)(&(pWorkStatusDlg->m_bThreadRunning)) , FALSE);
				//////////////////////////////////////////////////////////////////////////
				/// close work status dialog
				///pWorkStatusDlg->SendMessage(WM_COMMAND , IDCANCEL );
				////return IDCANCEL;
			}
		}
		//////////////////////////////////////////////////////////////////////////
		/// save to database
		vector<CLineDataRecord*> lineDataRecordEntry;
		for(i = 0;i < nCount;i++)
		{
			CLineDataRecord* pLineDataRecord = (CLineDataRecord*)m_QualifedLineReport.GetItemData(i);
			lineDataRecordEntry.push_back(pLineDataRecord);
		}
		pDocData->GetProject()->SaveToDatabase(&lineDataRecordEntry);

		InterlockedExchange((LONG*)(&(pWorkStatusDlg->m_bThreadRunning)) , FALSE);
		//////////////////////////////////////////////////////////////////////////
		/// close work status dialog
		pWorkStatusDlg->SendMessage(WM_COMMAND , IDOK );
		
		return IDOK;
	}
	catch(...)
	{
		AfxMessageBox("Unknown error");
		
		//m_getDataFromIsoDwgButton.EnableWindow(TRUE);
		InterlockedExchange((LONG*)(&(pWorkStatusDlg->m_bThreadRunning)) , FALSE);
		
		//////////////////////////////////////////////////////////////////////////
		/// close work status dialog
		pWorkStatusDlg->SendMessage(WM_COMMAND , IDOK );
	}
	
	return IDCANCEL;
}

/**	
	@brief	iso 파일에서 사용자가 지정한 데이터들을 읽어 database에 입력한다.
	
	@author	HumKyung.BAEK
**/
void CQualifiedLineForm::GetDataFromIsoDwg() 
{
	CWorkStatusDlg dlg;
	dlg.m_pThread = AfxBeginThread(StatusThreadEntry, this, THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	if (NULL == dlg.m_pThread)
	{
		AfxMessageBox("Can't create thread!!!");
	}
	else
	{
		SWITCH = UPDATE_FROM_ISO_DWG;

		///m_getDataFromIsoDwgButton.EnableWindow(FALSE);
		dlg.DoModal();
		///m_getDataFromIsoDwgButton.EnableWindow(TRUE);

		m_QualifedLineReport.SetRedraw(FALSE);
		const int nCount = m_QualifedLineReport.GetItemCount();
		int i = 0;
		for(i = 0;i < nCount;i++)
		{
			CLineDataRecord* pLineDataRecord = (CLineDataRecord*)m_QualifedLineReport.GetItemData(i);
			UpdateLineDataRecord(i , pLineDataRecord);
		}
		m_QualifedLineReport.SetRedraw(TRUE);

		m_pLineDataForm->m_wndLineDataReport.SetRedraw(FALSE);
		for(i = 0;i < nCount;i++)
		{
			CLineDataRecord* pLineDataRecord = (CLineDataRecord*)m_QualifedLineReport.GetItemData(i);
			const int nItem = m_ReverseItemMap[i];
			m_pLineDataForm->UpdateLineDataRecord(nItem , pLineDataRecord);
		}
		m_pLineDataForm->m_wndLineDataReport.SetRedraw(TRUE);
	}
}

/**	\brief	DELETE를 눌렀을때 리스트와 ItemMap의 line data record를 제거한다.

	\param	pNMHDR	a parameter of type NMHDR*
	\param	pResult	a parameter of type LRESULT*
	\remarks
	단 실제적인 line data record는 제거되지 않는다.

	\return	void	
*/
void CQualifiedLineForm::OnKeydownListQualifiedLine(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	if(VK_DELETE == pLVKeyDow->wVKey)
	{
		DeleteSelectedQualifiedLineList();
		int nCount = m_QualifedLineReport.GetItemCount();
		ShowSelectedCount(nCount);
	}

	*pResult = 0;
}

/**
	\brief	선택한 RECORD의 개수를 표시한다.
	\author	백흠경
**/
void CQualifiedLineForm::ShowSelectedCount(int nCount)
{
	int nRecordCount = m_pLineDataForm->m_nRecordCount;
	CWnd* pWnd = GetDlgItem(IDC_STATIC_SELECTED_COUNT);
	if(pWnd)
	{
		CString rCount, rSlash =" / ", rRecodrCount;
		rCount.Format("%d",nCount);
		rRecodrCount.Format("%d",nRecordCount);
		rCount = rCount + rSlash + rRecodrCount;
		pWnd->SetWindowText((LPCTSTR)rCount);
	}
}

/**	\brief	모든 아이템들을 제거한다.
**/
void CQualifiedLineForm::DeleteAllQualifiedLineList( )
{
	m_ReverseItemMap.clear();
	m_ItemMap.clear();

	m_QualifedLineReport.SetRedraw(FALSE);
	m_QualifedLineReport.DeleteAllItems();
	m_QualifedLineReport.SetRedraw(TRUE);
}

/**	\brief	선택한 아이템들을 제거한다.
**/
void CQualifiedLineForm::DeleteSelectedQualifiedLineList( )
{
	vector<int> selectedItemEntry;
	POSITION pos = m_QualifedLineReport.GetFirstSelectedItemPosition();
	while(pos)
	{
		int nItem = m_QualifedLineReport.GetNextSelectedItem(pos);
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
	
	m_QualifedLineReport.SetRedraw(FALSE);
	m_QualifedLineReport.DeleteAllSelectedItems();
	m_QualifedLineReport.SetRedraw(TRUE);
}

/**	@brief	delete line data interface 함수.

	@author	HumKyung

	@date	????.??.??

	@param

	@return	void	
*/
void CQualifiedLineForm::DeleteLineData()
{
	OnLineDataDelete();
}

/**	\brief	copy line data interface 함수.


	\return	void	
*/
void CQualifiedLineForm::CopyLineData()
{
	///OnLineDataCopy();
}

/**	\brief	modify line data interface 함수.
	\author	백흠경

	\return	void	
*/
bool CQualifiedLineForm::ModifyLineData()
{
	OnLineDataModify();
	return __MODIFY_THE_LINE_DATA__;
}

/**
	@brief	IDF 파일이 좌표를 가지고 PDTABLE122,PDTABLE121에서 PLAN NO.를 구합니다.

	@author	BHK
*/
int CQualifiedLineForm::GetDataFromIDF()
{
	CWorkStatusDlg dlg;
	dlg.m_pThread = AfxBeginThread(StatusThreadEntry, this, THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	if (NULL == dlg.m_pThread)
	{
		AfxMessageBox("Can't create thread!!!");
	}
	else
	{
		ERROR_ENTRY.clear();

		SWITCH = UPDATE_FROM_IDF;
		///m_getDataFromIsoDwgButton.EnableWindow(FALSE);
		dlg.DoModal();
		///m_getDataFromIsoDwgButton.EnableWindow(TRUE);
		
		m_QualifedLineReport.SetRedraw(FALSE);
		const int nCount = m_QualifedLineReport.GetItemCount();
		int i = 0;
		for(i = 0;i < nCount;i++)
		{
			CLineDataRecord* pLineDataRecord = (CLineDataRecord*)m_QualifedLineReport.GetItemData(i);
			UpdateLineDataRecord(i , pLineDataRecord);
		}
		m_QualifedLineReport.SetRedraw(TRUE);
		
		m_pLineDataForm->m_wndLineDataReport.SetRedraw(FALSE);
		for(i = 0;i < nCount;i++)
		{
			CLineDataRecord* pLineDataRecord = (CLineDataRecord*)m_QualifedLineReport.GetItemData(i);
			const int nItem = m_ReverseItemMap[i];
			m_pLineDataForm->UpdateLineDataRecord(nItem , pLineDataRecord);
		}
		m_pLineDataForm->m_wndLineDataReport.SetRedraw(TRUE);

		//! if have errors
		if(!ERROR_ENTRY.empty())
		{
			CString rMsg;
			for(vector<string>::iterator itr = ERROR_ENTRY.begin();itr != ERROR_ENTRY.end();++itr)
			{
				rMsg += CString(itr->c_str()) + "\n";
			}

			AfxMessageBox(rMsg);
		}
	}
	return ERROR_SUCCESS;
}