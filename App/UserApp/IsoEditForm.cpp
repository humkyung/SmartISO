// IsoEditForm.cpp : implementation file
//

#include "stdafx.h"
#include <afxpriv.h>
#include <ado/SQLFile.h>
#include <ProjectData.h>
#include <DgnDocument.h>
#include <DgnText.h>
#include <Tokenizer.h>
#include "UserApp.h"
#include "MainFrm.h"
#include "UserAppDocData.h"
#include "UserAppView.h"
#include "IsoEditForm.h"
#include "WorkStatusDlg.h"
#include "IsoEditViewLogFileForm.h"
#include "QCCheck.h"
#include "IsoEditSettingPage.h"
#include "WorkspaceWnd.h"
#include "SmartISOLog.h"
#include <util/FileTools.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	FILE_NAME_COL			1
#define	QULIFIED_FILE_NAME_COL	0

UINT CIsoEditForm::TIMER_ID = 0;

#define	MSTN_FILE		9
#define	FILE_EXIST		11
#define	FILE_NOT_FOUND	12
/////////////////////////////////////////////////////////////////////////////
// CIsoEditForm

IMPLEMENT_DYNCREATE(CIsoEditForm, CFormView)

CIsoEditForm::CIsoEditForm()
	: CFormView(CIsoEditForm::IDD) , m_pBoldFont(NULL) , m_pLineDataRecordSet(NULL) , m_pQualifiedLineForm(NULL)
{
	//{{AFX_DATA_INIT(CIsoEditForm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pViewLogFileForm = NULL;
	m_nRecordCount = 0;
	m_bEditIsoFolder = FALSE;
	m_OutPutFileFullPath.clear();
	m_Temp2OutMatch.clear();
	m_uTimerID = NULL;

}

CIsoEditForm::~CIsoEditForm()
{
	try
	{
		if(NULL != m_pBoldFont) delete m_pBoldFont;
		if(m_pLineDataRecordSet != NULL) delete m_pLineDataRecordSet;
		m_pViewLogFileForm = NULL;
	}
	catch(...)
	{
		
	}
}

void CIsoEditForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIsoEditForm)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Text(pDX , IDC_STATIC_ISO_DWG_FOLDER , m_rJobIsoDwgFileFolder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIsoEditForm, CFormView)
	//{{AFX_MSG_MAP(CIsoEditForm)
	ON_WM_SIZE()
	ON_COMMAND(ID_ISO_EDIT_QUALIFY, OnIsoEditQualify)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	///ON_NOTIFY(NM_RCLICK, IDC_LIST_LINE_DATA, OnRclickListLineData)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_LINE_DATA, OnDblclkListLineData)
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_QUALIFIED_LINE, OnDblclkQualifiedFileList)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST_LINE_DATA , OnBegindragLineList)
	ON_MESSAGE(WM_KICKIDLE , OnKickIdle)
	ON_WM_HELPINFO()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_LINE_DATA, &CIsoEditForm::OnLvnItemchangedListLineData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIsoEditForm diagnostics

#ifdef _DEBUG
void CIsoEditForm::AssertValid() const
{
	CFormView::AssertValid();
}

void CIsoEditForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

LRESULT CIsoEditForm::OnKickIdle(WPARAM wParam , LPARAM lParam)
{
	UpdateDialogControls(this , FALSE);
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CIsoEditForm message handlers

BOOL CIsoEditForm::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

/**
	@brief	
	@author	HumKyung.BAEK
*/
void CIsoEditForm::OnInitialUpdate() 
{
	static bool __init__ = false;
	CFormView::OnInitialUpdate();
	
	if(false == __init__)
	{
		m_ImageList.Create(IDB_CHECKBOXES, 16, 3, RGB(255,0,255));

		CRect rectDummy;
		rectDummy.SetRectEmpty ();
		GetClientRect(rectDummy);
		
		CAppDocData* pDocData = CAppDocData::GetInstance();

		if(m_wndLineDataReport.SubclassWindow(GetDlgItem(IDC_LIST_LINE_DATA)->GetSafeHwnd()))
		{
			m_wndLineDataReport.SetWindowPos(NULL , 0 , 30 , 0 , 0 , SWP_NOZORDER);
			m_wndLineDataReport.SetCheckboxes();

			LONG style = GetWindowLong(m_wndLineDataReport.m_hWnd , GWL_STYLE);
			style |= WS_EX_CLIENTEDGE;
			SetWindowLong(m_wndLineDataReport.m_hWnd , GWL_EXSTYLE , style );
			m_wndLineDataReport.SetImageList(&m_ImageList , LVSIL_SMALL);
			m_wndLineDataReport.SetHeaderImageList(&m_ImageList);
			m_wndLineDataReport.SetHeadings( _T("[],30;") + _TR("File Name") + _T(",200;Rev No,100;Status,0") );
			m_wndLineDataReport.SetGridLines();
			m_wndLineDataReport.SetExtendedStyle(LVS_EX_CHECKBOXES | m_wndLineDataReport.GetExtendedStyle());
			m_wndLineDataReport.EnableHeaderCheckBox(TRUE);
		}

		m_pBoldFont = new CFont;
		m_pBoldFont->CreateFont(18,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0 , _T("Arial"));
		/////////////////////////////////////////////
		//// Set up initial variables
		m_bDragging = false;
		m_nDragIndex = -1;
		m_nDropIndex = -1;
		m_pDragImage = NULL;

		/// 2011.07.08 added by humkyung
		const CString rSqlFilePath = CAppDocData::GetExecPath() + _T("\\Setting\\T_ISO_LINE_DATA.XML");
		CSQLFile sqlFile(rSqlFilePath.operator LPCSTR());
		if(ERROR_SUCCESS == sqlFile.ParseXML(rSqlFilePath.operator LPCTSTR()))
		{
			const int nCount = sqlFile.GetColumnCount();
			list<STRING_T> oFieldNameEntry;
			for(int i = 0;i < nCount;++i)
			{
				const STRING_T rFieldName = sqlFile.GetFieldNameAt(i);
				m_oLineDataFieldNameEntry.push_back(rFieldName);
			}
		}
		///

		__init__ = true;
	}
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-27
    @class      CIsoEditForm
    @function   OnSize
    @return     void
    @param      UINT    nType
    @param      int     cx
    @param      int     cy
    @brief
******************************************************************************/
void CIsoEditForm::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);	
	DetermineLayout(cx , cy);
}

/**	
	@brief	JOB이 로드되고 난 뒤에 폼 초기화.

	@author	HumKyung.BAEK

	@date	????.??.??

	@param

	@return	void	
*/
void CIsoEditForm::InitContents()
{
	CUserAppView* pView = static_cast<CUserAppView*>(GetParent());
	CUserAppDoc*  pDoc  = (CUserAppDoc*)(this->GetDocument());
	CAppDocData* pDocData = CAppDocData::GetInstance();
	m_rIniFilePath= pDocData->GetProject()->GetIniFilePath();
	
	CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();

	pWorkspaceBar->m_IsoEditOptionDlg.m_rIniFilePath = m_rIniFilePath;

	LoadData( m_rIniFilePath );
	pWorkspaceBar->m_wndQuery2.InitContents( this );
	pWorkspaceBar->m_IsoEditOptionDlg.m_pTargetWnd = this;
	pWorkspaceBar->m_IsoEditOptionDlg2.m_pTargetWnd = this;
	pWorkspaceBar->m_IsoEditOptionDlg.LoadData( m_rIniFilePath );

	/// 2011.01.19 - added by HumKyung
	/*if(pDocData)
	{
		if((STRING_T(_T("ADMIN")) != pDocData->m_pSelUser->id()))
		{
			pWorkspaceBar->m_wndIsoEditTabCtrl.RemoveTab(2);
		}
		else
		{
			pWorkspaceBar->m_IsoEditOptionDlg2.InitContents();
		}
	}*/
}

/**
	@biref	form의 size에 맞게 control들의 위치를 재조정한다.
	@author	humkyung
*/
void CIsoEditForm::DetermineLayout(int cx , int cy)
{
	const int gap = 3 , LeftX = 2;
	CWnd* pWnd = NULL;
	
	if(m_wndLineDataReport.GetSafeHwnd())
	{
		CRect rect[2];
		{			
			//////////////////////////////////////////////////////////////////////////
			m_wndLineDataReport.GetWindowRect(&rect[0]);
			ScreenToClient(&rect[0]);
			m_wndLineDataReport.SetWindowPos(NULL , LeftX , rect[0].top , cx - gap , cy - rect[0].top, SWP_NOZORDER);
			
			pWnd = GetDlgItem(IDC_STATIC_1);
			if(pWnd)
			{
				m_wndLineDataReport.GetWindowRect(&rect[1]);
				ScreenToClient(&rect[1]);
				
				pWnd->SetWindowPos(NULL , rect[1].left + 3 , rect[1].top - 20 , 0 , 0 , SWP_NOSIZE);
			}
			
			pWnd = GetDlgItem(IDC_STATIC_ISO_DWG_FOLDER);
			if(pWnd)
			{
				CWnd* pWnd1 = GetDlgItem(IDC_STATIC_1);
				pWnd1->GetWindowRect(&rect[0]);
				ScreenToClient(&rect[0]);
				
				m_wndLineDataReport.GetWindowRect(&rect[1]);
				ScreenToClient(&rect[1]);
				
				pWnd->SetWindowPos(NULL , rect[1].left + rect[0].Width() + 3 , rect[1].top - 20 ,
					rect[1].Width() - rect[0].Width() - 4 , 20 , SWP_NOZORDER);
			}
		}
	}

	if(m_wndLineDataReport.GetSafeHwnd())
	{
		CRect thisRect , rect , startRect;
		
		m_wndLineDataReport.GetWindowRect(&rect);
		ScreenToClient(&rect);
	}
	
	if(::IsWindow(m_wndLineDataReport.GetSafeHwnd()))
	{
		CRect rect;
		m_wndLineDataReport.GetWindowRect(&rect);
		ScreenToClient(&rect);
	}

	RedrawWindow();
}

void CIsoEditForm::OnIsoEditQualify() 
{
	vector<LineDataRecordMap> IsoEditItemEntry;
	GetSelectedLineDataRecord(IsoEditItemEntry);

	AddIsoEditItemEntry(&IsoEditItemEntry);
}

struct MatchFileName : public binary_function<IsoTemp2OutputMatch , CString , bool>
{
	bool operator()(const IsoTemp2OutputMatch& lhs , const CString& rhs) const
	{
		CString _lhs = lhs.rSeedFileName;
		CString _rhs = rhs;
		_lhs.MakeUpper();
		_rhs.MakeUpper();

		return (_lhs == _rhs);
	}
};

/**
	@brief	insert qualified item not duplicated.
	@author	humkyung
	@remarks 존재하는 파일만 qualified list에 추가한다.
**/
void CIsoEditForm::AddIsoEditItemEntry(vector<LineDataRecordMap>* pIsoEditItemEntry)
{
	assert(pIsoEditItemEntry && "pIsoEditItemEntry is NULL");

	if(pIsoEditItemEntry && !pIsoEditItemEntry->empty())
	{
		if(NULL == (*pIsoEditItemEntry)[0].pLineDataRecord) return;
		{
			CString sItemText = m_wndLineDataReport.GetItemText((*pIsoEditItemEntry)[0].nItem, 1);

			vector<IsoTemp2OutputMatch>::iterator where = find_if(m_Temp2OutMatch.begin(), m_Temp2OutMatch.end(), bind2nd(MatchFileName(), sItemText));
			if(where != m_Temp2OutMatch.end())
			{
				CString sFilePath = (where)->rTempFileName;
				if(CFileTools::DoesFileExist(sFilePath))
				{
					CDgnScene* pScene = static_cast<CDgnScene*>(m_pDropList->GetScene());
					pScene->AddScene(sFilePath);
				}
				else
				{
					AfxMessageBox( _T("There is no file.") );
				}
			}
			else
			{
				AfxMessageBox( _T("There is no product file.") , MB_OK|MB_ICONEXCLAMATION);
			}
		}
	}
}

/**	
	@brief	The CIsoEditForm::GetSelectedLineDataRecord function
	@param	IsoEditItemEntry	a parameter of type vector<long>&
	@return	void	
*/
void CIsoEditForm::GetSelectedLineDataRecord(vector<LineDataRecordMap>& LineDataRecordEntry)
{
	LineDataRecordEntry.clear();
	LineDataRecordMap aLineDataRecordMap;

	const int iItemCount = m_wndLineDataReport.GetItemCount();
	for(int i = 0;i < iItemCount;++i)
	{
		if(TRUE == m_wndLineDataReport.GetCheck(i))
		{
			aLineDataRecordMap.nItem = i;
			aLineDataRecordMap.pLineDataRecord = (CLineDataRecord*)m_wndLineDataReport.GetItemData(i);
			if(NULL != aLineDataRecordMap.pLineDataRecord)	LineDataRecordEntry.push_back(aLineDataRecordMap);	
		}
	}
}

/**	
	@brief	선택한 DGN 파일을 오픈한다.

	@param	pNMHDR	a parameter of type NMHDR*
	@param	pResult	a parameter of type LRESULT*

	@return	void	
*/
void CIsoEditForm::OnDblclkQualifiedFileList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
}	

/**	
	@brief	The CIsoEditForm::OnNotify function
	@author	HumKyung.BAEK
	@param	wParam	a parameter of type WPARAM
	@param	lParam	a parameter of type LPARAM
	@param	pResult	a parameter of type LRESULT*
	@remarks	SELECT일 경우 이슈 날짜를 확인하여 쿼리를 한다.
	@return	BOOL	
*/
BOOL CIsoEditForm::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();

	NMHDR* pNMHDR = (NMHDR*)(lParam);

	CString rApp( _T("Iso Edit Setting") );
	TCHAR szBuf[MAX_PATH] = {'\0' ,};
	if((SELECT_ALL == wParam) && (pWorkspaceBar->m_wndQuery2.GetDlgCtrlID() == pNMHDR->idFrom))
	{
		if(GetPrivateProfileString(rApp , _T("Source Iso File Folder") , NULL , szBuf , MAX_PATH , m_rIniFilePath))
		{
			m_rJobIsoDwgFileFolder = szBuf;
			UpdateData(FALSE);
		}

		CString rQueryCondition = pWorkspaceBar->m_wndQuery2.GetInAreaQueryCondition();
		vector< STRING_T > issueDates;
		LoadLineDataRecordSetFromDatabase( rQueryCondition , NULL , issueDates );
	}
	else if((SELECT == wParam) && (pWorkspaceBar->m_wndQuery2.GetDlgCtrlID() == pNMHDR->idFrom))
	{
		if(GetPrivateProfileString(rApp , _T("Source Iso File Folder") , NULL , szBuf , MAX_PATH , m_rIniFilePath))
		{
			m_rJobIsoDwgFileFolder = szBuf;
			UpdateData(FALSE);
		}

		CString rQueryCondition = pWorkspaceBar->m_wndQuery2.GetQueryCondition();
		
		vector< string > issueDates;
		pWorkspaceBar->m_wndQuery2.GetSelectedIssueDate( issueDates );
		LoadLineDataRecordSetFromDatabase(rQueryCondition , NULL , issueDates );
	}
	/// 아래의 코드는 데이터 베이스를 쿼리하는 부분이 아니라 옵션을 수정했을때 처리하는 부분이다.
	else if( (SELECT_SOURCE_ISO_DWG_FOLDER == wParam) && (pWorkspaceBar->m_IsoEditOptionDlg.GetDlgCtrlID() == pNMHDR->idFrom) )
	{
		if(GetPrivateProfileString(rApp , _T("Source Iso File Folder") , NULL , szBuf , MAX_PATH , m_rIniFilePath))
		{
			m_rJobIsoDwgFileFolder = szBuf;
		}

		m_bEditIsoFolder = FALSE;
		UpdateData( FALSE );
	}
	else if( (SELECT_EDITED_ISO_DWG_FOLDER == wParam) && (pWorkspaceBar->m_IsoEditOptionDlg.GetDlgCtrlID() == pNMHDR->idFrom) )
	{
		m_rJobIsoDwgFileFolder = m_rEditedIsoDwgFileFolder;
		m_bEditIsoFolder = TRUE;
		UpdateData( FALSE );
	}
	else if( (QUALITY_CONTROL_CHECK == wParam) && (pWorkspaceBar->m_IsoEditOptionDlg.GetDlgCtrlID() == pNMHDR->idFrom) )
	{
		BOOL res = pWorkspaceBar->m_IsoEditOptionDlg.GetOptionCheck( QUALITY_CONTROL_CHECK );
		if( TRUE == res )
		{
			pWorkspaceBar->m_IsoEditOptionDlg2.EnableAllOptions();
		}
		else
		{
			pWorkspaceBar->m_IsoEditOptionDlg2.DisableAllOptions();
		}
	}

	return CFormView::OnNotify(wParam, lParam, pResult);
}

/**	
	@brief	database에서 쿼리 조건에 맞는 레코드를 로딩한다.
	@author	HumKyung.BAEK
	@param	rQueryCondition	a parameter of type const CString &
	@return	void	
*/
void CIsoEditForm::LoadLineDataRecordSetFromDatabase(const CString &rQueryCondition, LPCTSTR pRevNo , const vector<STRING_T>& issueDates)
{	
	CAppDocData* pDocData = CAppDocData::GetInstance();
	if(NULL != pDocData)
	{
		CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();

		if(NULL != m_pLineDataRecordSet) CLineDataRecordSet::DeleteInstance(m_pLineDataRecordSet);
		
		BeginWaitCursor();
		
		CString rSelRevNo = pWorkspaceBar->m_wndQuery2.GetSelectedRevNo();
		if(rSelRevNo.IsEmpty() || (_T("All") == rSelRevNo) )
		{
			m_pLineDataRecordSet = pDocData->GetProject()->GetLineDataRecordSetFromDatabase(rQueryCondition , NULL , issueDates);
		}
		else
		{
			m_pLineDataRecordSet = pDocData->GetProject()->GetLineDataRecordSetFromDatabase(rQueryCondition , rSelRevNo.operator LPCTSTR() , issueDates);	
		}

		if(NULL != m_pLineDataRecordSet)
		{
			int nRecordCount = m_pLineDataRecordSet->GetLineDataRecordCount();
			if(0 == nRecordCount)
			{
				CString rMsg;
				rMsg.LoadString(IDS_NO_RECORD);
				AfxMessageBox(rMsg);
				EndWaitCursor();

				/// line list와 qualified list의 아이템을 제거한다.
				m_wndLineDataReport.SetRedraw(FALSE);
				m_wndLineDataReport.DeleteAllItems();
				m_wndLineDataReport.SetRedraw(TRUE);

				return;
			}
			LineCONTRASTISo(nRecordCount);						
			if(m_pQualifiedLineForm) m_pQualifiedLineForm->ClearContents();
		}
		EndWaitCursor();
	}
}

/**	
	@brief	쿼리한 line data record와 일치하는 iso drawing을 리스트에 보여준다.
	@author	humkyung
	@param	nRecordCount = count of line data record
	@remarks
	REV NO. 항목에는 "바로 이전의 REV NO. -> 맨 나중의 REV NO."를 표기한다.
**/
void CIsoEditForm::LineCONTRASTISo(const int& nRecordCount)
{
	CUserAppView* pView = (CUserAppView*)(GetParent());
	CUserAppDoc*  pDoc  = (CUserAppDoc*)(this->GetDocument());
	CAppDocData* pDocData = CAppDocData::GetInstance();
	CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();
	CString rIniFilePath = pDocData->GetProject()->GetIniFilePath();

	m_wndLineDataReport.SetRedraw(FALSE);
	m_wndLineDataReport.DeleteAllItems();

	TCHAR szBuf[MAX_PATH + 1]={'\0',};
	CString rDesc, rFluid, rLineNo, rSheet,rLineDataValue,rISOValue;
	CString rIsoFilePath;
	int nItem=0;
	int nSelectCount=0 , nMiner = 0;
	BOOL bShowExistingFile = pWorkspaceBar->m_IsoEditOptionDlg.GetOptionCheck( SHOW_EXISTING_FILE );
	for(int j = 0; j < nRecordCount;++j)
	{
		CLineDataRecord* pLineDataRecord = (CLineDataRecord*)m_pLineDataRecordSet->GetLineDataRecordAt(j);	
		const CString index = pLineDataRecord->index();
		if(!pLineDataRecord->IsDeleted())
		{
			CString rIsoFileName;
			rIsoFileName = pDocData->GetProject()->GetInputIsoFileName(pLineDataRecord);

			if(m_bEditIsoFolder == TRUE) rIsoFileName = pDocData->GetProject()->GetOutputIsoFileName(pLineDataRecord);
			CString rLastRevNum = (m_bEditIsoFolder == TRUE) ? pDocData->GetFirstRevNoString() : CIsoEditSettingPage::m_rFileNameExt;
			CString rTemp = CIsoEditSettingPage::m_rFileNameExt + _T("->") , rEditLastRevNum = rLastRevNum , rDot = _T(".");

			/// REVISION이 하나도 없을 경우에는 LIST에 표시하지 않는다.
			CRevDataRecordSet* pRevDataRecordSet = (CRevDataRecordSet*)pLineDataRecord->GetRevDataRecordSet();
			if(pRevDataRecordSet)
			{
				const int nRevCount = pRevDataRecordSet->GetRecordCount();
				if(nRevCount > 0)
				{
					for(int i = 0; i < nRevCount; ++i)
					{
						CRevDataRecord* pRevDataRecord = (CRevDataRecord*)pRevDataRecordSet->GetRecordAt(i);
						if(i == nRevCount - 2)		/// 제일 마지막 바로 이전 revision record
						{
							if(TRUE == m_bEditIsoFolder) rLastRevNum = CString(pDocData->GetRevNoString( pLineDataRecord , pRevDataRecord->GetRevNo() ));
							if(m_bEditIsoFolder == TRUE) rEditLastRevNum = pDocData->GetRevNoString( pLineDataRecord , pRevDataRecord->GetRevNo() );
						}
						else if(i == nRevCount - 1)	///< 제일 마지막 revision record
						{
							rLastRevNum += CString( _T("->") ) + pDocData->GetRevNoString( pLineDataRecord , pRevDataRecord->GetRevNo() );
						}
					}
				}
				else
				{
					rLastRevNum = (TRUE == m_bEditIsoFolder) ? pDocData->GetFirstRevNoString() : _T("i01");
					continue;
				}
			}else	continue;

			rLineDataValue = rIsoFileName + CIsoEditSettingPage::m_rFileNameExt;
			if(m_bEditIsoFolder == TRUE) rLineDataValue = rIsoFileName + _T(".") + rEditLastRevNum;
			rLineDataValue.MakeUpper();
						
			rIsoFilePath = m_rJobIsoDwgFileFolder;
			if(_T("\\") != rIsoFilePath.Right(1)) rIsoFilePath += _T("\\");
			rIsoFilePath += rLineDataValue;
			
			if(bShowExistingFile)
			{
				if(CFileTools::DoesFileExist(rIsoFilePath))
				{
					m_IsoFileMap[long(pLineDataRecord)] = pLineDataRecord;

					const int nItemCount = m_wndLineDataReport.GetItemCount();
					if(m_bEditIsoFolder == TRUE)
					{
						CString rString;

						nItem = m_wndLineDataReport.InsertItem(j, rString , FILE_EXIST);
						m_wndLineDataReport.SetItemText(nItem , 1 , rIsoFileName + rDot + rEditLastRevNum);
					}
					else
					{
						CString rString;

						nItem = m_wndLineDataReport.InsertItem(j, rString , FILE_EXIST);
						m_wndLineDataReport.SetItemText(nItem , 1, rIsoFileName + CIsoEditSettingPage::m_rFileNameExt);
					}
					m_wndLineDataReport.SetItemData(nItem , (DWORD)pLineDataRecord );
					m_wndLineDataReport.SetItemText(nItem,2,_T(rLastRevNum));
					m_wndLineDataReport.SetItemText(nItem,3,_T("Found"));	/// 2013.05.12 added by humkyung
				}
				else ++nMiner;
			}
			else
			{	
				const int nItemCount = m_wndLineDataReport.GetItemCount();
				if(CFileTools::DoesFileExist(rIsoFilePath))
				{
					m_IsoFileMap[long(pLineDataRecord)] = pLineDataRecord;
					if(m_bEditIsoFolder == TRUE)
					{
						CString rString;
						nItem = m_wndLineDataReport.InsertItem(j, rString , FILE_EXIST);

						m_wndLineDataReport.SetItemText(nItem , 1 , rIsoFileName + rDot + rEditLastRevNum);
					}
					else
					{
						CString rString;
						nItem = m_wndLineDataReport.InsertItem(j, rString , FILE_EXIST);

						m_wndLineDataReport.SetItemText(nItem , 1 , rIsoFileName + CIsoEditSettingPage::m_rFileNameExt);
					}
					m_wndLineDataReport.SetItemData( nItem , (LONG)pLineDataRecord );
					m_wndLineDataReport.SetItemText(nItem,2,_T(rLastRevNum));
					m_wndLineDataReport.SetItemText(nItem,3,_T("Found"));	/// 2013.05.12 added by humkyung
				}
				else
				{
					m_IsoFileMap[long(pLineDataRecord)] = NULL;
					if(m_bEditIsoFolder == TRUE)
					{
						CString rString;
						nItem = m_wndLineDataReport.InsertItem(j, rString , FILE_NOT_FOUND);

						m_wndLineDataReport.SetItemText(nItem , 1, rIsoFileName + rDot + rEditLastRevNum);
					}
					else
					{
						CString rString;
						nItem = m_wndLineDataReport.InsertItem(j, rString , FILE_NOT_FOUND);

						m_wndLineDataReport.SetItemText(nItem , 1 , rIsoFileName + CIsoEditSettingPage::m_rFileNameExt);
					}
					m_wndLineDataReport.SetItemData( nItem , NULL );
					m_wndLineDataReport.SetItemText(nItem,2,_T(rLastRevNum));
					m_wndLineDataReport.SetItemText(nItem,3,_T("Not Found"));	/// 2013.05.12 added by humkyung
				}
			}
			++nSelectCount;
		}
	}
	nSelectCount -= nMiner;
	m_wndLineDataReport.GroupByColumn(3);	/// 2013.05.12 added by humkyung
	m_wndLineDataReport.SetRedraw(TRUE);
	ShowRecordCount(nSelectCount);
}

/**	
	@brief	쓰레드를 생성시켜 IsoEdit 실행시킨다.
	@author	HumKyung.BAEK
	@date	2010.05.17
	@return	void	
*/
void CIsoEditForm::GenerateIsoProduct() 
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();
	//////////////////////////////////////////////////////////////////////////
	/// check border setting
	CString rBorderSettingErrorMsg;
	rBorderSettingErrorMsg.LoadString(IDS_BORDER_SETTING_ERROR);
	if(-1 == pDocData->m_borderSetting.FontNo)
	{
		AfxMessageBox(rBorderSettingErrorMsg);
		return;
	}
	
	if(-1 == pDocData->m_borderSetting.widthMulti)
	{
		AfxMessageBox(rBorderSettingErrorMsg);
		return;
	}
	
	if(-1 == pDocData->m_borderSetting.heightMulti)
	{
		AfxMessageBox(rBorderSettingErrorMsg);
		return;
	}
	//////////////////////////////////////////////////////////////////////////

	const int nItemCount = m_wndLineDataReport.GetCheckedItemCount();
	if( nItemCount )
	{
		CWorkStatusDlg dlg;
		dlg.m_pThread = AfxBeginThread(StatusThreadEntry, this, THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
		if (NULL == dlg.m_pThread)
		{
			AfxMessageBox( _T("Can't create thread!!!") );
		}
		else
		{
			START_LOG();	/// start log - 2012.01.04 added by humkyung

			m_OutPutFileFullPath.clear();
			PreProcess();	///< 작업을 수행하기 앞서 해야할 일들을 수행한다.
			dlg.DoModal();

			CLOSE_LOG();	/// close log - 2012.01.04 added by humkyung
			OPEN_LOG();		/// open log - 2012.01.04 added by humkyung

			/// LOG TEXT를 표시한다.
			const BOOL bSyncLog = pWorkspaceBar->m_IsoEditOptionDlg.GetOptionCheck(LOG_FILE_SYNCHRONIZING);
			if(TRUE == bSyncLog)
			{
				if(NULL != m_pViewLogFileForm) m_pViewLogFileForm->DisplayLog();
				if(NULL != m_pViewLogFileForm) m_pViewLogFileForm->DisplayErrorSum();	/// display error sum - 2012.08.10 added by humkyung
			}

			/// create IDF folder - 2012.11.10 added by humkyung
			const CString sProductIDFFolder = pDocData->GetProjectPath().c_str() + CString(_T("\\PRODUCT\\IDF"));
			CFileTools::CreateFolder(sProductIDFFolder);
			/// up to here

			for(vector<IsoOutPath>::const_iterator itr = m_OutPutFileFullPath.begin(); itr != m_OutPutFileFullPath.end(); ++itr)
			{
				if(FALSE == CopyFile((itr)->rTempFilePath , (itr)->rOutputFilePath , FALSE))
				{
					AfxMessageBox( _T("Can't copy ") + (itr)->rTempFilePath + _T(" to ") + (itr)->rOutputFilePath);
				}

				/// copy idf file corresponding to iso file - 2012.11.10 added by humkyung
				if(CFileTools::DoesFileExist(itr->sTempIDFFilePath))
				{
					CopyFile(itr->sTempIDFFilePath , itr->sOutputIDFFilePath , FALSE);
				}
				/// up to here
			}
		}
		PostProcess();	/// 작업을 수행하고 나서 해야할 일들을 수행한다.
	}
	else
	{
		AfxMessageBox( _T("There are no checked iso files.") );
	}
}


void CIsoEditForm::OnBegindragLineList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//This routine sets the parameters for a Drag and Drop operation.
	//It sets some variables to track the Drag/Drop as well
	// as creating the drag image to be shown during the drag.

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	//// Save the index of the item being dragged in m_nDragIndex
	////  This will be used later for retrieving the info dragged
	m_nDragIndex = pNMListView->iItem;

	//// Create a drag image
	POINT pt;
	int nOffset = -10; //offset in pixels for drag image (positive is up and to the left; neg is down and to the right)
	if(m_wndLineDataReport.GetSelectedCount() > 0) //more than one item is selected
	{
		pt.x = nOffset;
		pt.y = nOffset;

		m_pDragImage = m_wndLineDataReport.CreateDragImage(m_nDragIndex, &pt);
		ASSERT(m_pDragImage); //make sure it was created
		//We will call delete later (in LButtonUp) to clean this up

		CBitmap bitmap;
		if(m_wndLineDataReport.GetSelectedCount() > 1) //more than 1 item in list is selected
			//bitmap.LoadBitmap(IDB_BITMAP_MULTI);
			bitmap.LoadBitmap(IDB_BITMAP_MULTI_BOXES);
		else
			bitmap.LoadBitmap(IDB_BITMAP_BOX);
		m_pDragImage->Replace(0, &bitmap, &bitmap);

		//// Change the cursor to the drag image
		////	(still must perform DragMove() in OnMouseMove() to show it moving)
		m_pDragImage->BeginDrag(0, CPoint(nOffset, nOffset - 4));
		m_pDragImage->DragEnter(GetDesktopWindow(), pNMListView->ptAction);
		
		//// Set dragging flag and others
		m_bDragging = TRUE;	//we are in a drag and drop operation
		m_nDropIndex = -1;	//we don't have a drop index yet
		m_pDragList = &m_wndLineDataReport; //make note of which list we are dragging from
		m_pDropWnd =  &m_wndLineDataReport;	//at present the drag list is the drop list

		//// Capture all mouse messages
		SetCapture ();
	}

	*pResult = 0;
}

void CIsoEditForm::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//While the mouse is moving, this routine is called.
	//This routine will redraw the drag image at the present
	// mouse location to display the dragging.
	//Also, while over a CListCtrl, this routine will highlight
	// the item we are hovering over.

	/// If we are in a drag/drop procedure (m_bDragging is true)
	if (m_bDragging)
	{
		/// Move the drag image
		CPoint pt(point);	//get our current mouse coordinates
		ClientToScreen(&pt); //convert to screen coordinates
		m_pDragImage->DragMove(pt); //move the drag image to those coordinates
		// Unlock window updates (this allows the dragging image to be shown smoothly)
		m_pDragImage->DragShowNolock(false);
		
		/// Get the CWnd pointer of the window that is under the mouse cursor
		CWnd* pDropWnd = WindowFromPoint (pt);
		ASSERT(pDropWnd); /// make sure we have a window

		/// If we drag outside current window we need to adjust the highlights displayed
		if (pDropWnd != m_pDropWnd)
		{
			if (m_nDropIndex != -1) //If we drag over the CListCtrl header, turn off the hover highlight
			{
				TRACE("m_nDropIndex is -1\n");
				CListCtrl* pList = (CListCtrl*)m_pDropWnd;
				//VERIFY (pList->SetItemState (m_nDropIndex, 0, LVIS_DROPHILITED));
				// redraw item
				//VERIFY (pList->RedrawItems (m_nDropIndex, m_nDropIndex));
				//pList->UpdateWindow ();
				m_nDropIndex = -1;
			}
			else //If we drag out of the CListCtrl altogether
			{
				TRACE("m_nDropIndex is not -1\n");
				//CListCtrl* pList = (CListCtrl*)m_pDropWnd;
				//int i = 0;
				//int nCount = pList->GetItemCount();
				//for(i = 0; i < nCount; i++)
				//{
				//	pList->SetItemState(i, 0, LVIS_DROPHILITED);
				//}
				//pList->RedrawItems(0, nCount);
				//pList->UpdateWindow();
			}
		}

		// Save current window pointer as the CListCtrl we are dropping onto
		m_pDropWnd = pDropWnd;

		// Convert from screen coordinates to drop target client coordinates
		pDropWnd->ScreenToClient(&pt);

		/// If we are hovering over a CListCtrl we need to adjust the highlights
		if(pDropWnd->IsKindOf(RUNTIME_CLASS (CDgnCanvas)))
		{
			TRACE("m_nDropIndex is not 100\n");
			/// Note that we can drop here
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			UINT uFlags;
			CDgnCanvas* pCanvas = (CDgnCanvas*)pDropWnd;

			// Turn off hilight for previous drop target
			//pList->SetItemState (m_nDropIndex, 0, LVIS_DROPHILITED);
			// Redraw previous item
			//pList->RedrawItems (m_nDropIndex, m_nDropIndex);
			
			// Get the item that is below cursor
			m_nDropIndex = ((CListCtrl*)pDropWnd)->HitTest(pt, &uFlags);
			// Highlight it
			//pList->SetItemState(m_nDropIndex, LVIS_DROPHILITED, LVIS_DROPHILITED);
			// Redraw item
			//pList->RedrawItems(m_nDropIndex, m_nDropIndex);
			//pList->UpdateWindow();
		}
		else
		{
			//If we are not hovering over a CListCtrl, change the cursor
			// to note that we cannot drop here
			SetCursor(LoadCursor(NULL, IDC_NO));
		}
		// Lock window updates
		m_pDragImage->DragShowNolock(true);
	}

	CFormView::OnMouseMove(nFlags, point);
}

void CIsoEditForm::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
		//This routine is the end of the drag/drop operation.
	//When the button is released, we are to drop the item.
	//There are a few things we need to do to clean up and
	// finalize the drop:
	//	1) Release the mouse capture
	//	2) Set m_bDragging to false to signify we are not dragging
	//	3) Actually drop the item (we call a separate function to do that)

	//If we are in a drag and drop operation (otherwise we don't do anything)
	if (m_bDragging)
	{
		// Release mouse capture, so that other controls can get control/messages
		ReleaseCapture ();

		// Note that we are NOT in a drag operation
		m_bDragging = FALSE;

		// End dragging image
		m_pDragImage->DragLeave (GetDesktopWindow ());
		m_pDragImage->EndDrag ();
		delete m_pDragImage; //must delete it because it was created at the beginning of the drag

		CPoint pt (point); //Get current mouse coordinates
		ClientToScreen (&pt); //Convert to screen coordinates
		// Get the CWnd pointer of the window that is under the mouse cursor
		CWnd* pDropWnd = WindowFromPoint (pt);
		ASSERT (pDropWnd); //make sure we have a window pointer
		// If window is CListCtrl, we perform the drop
		if (pDropWnd->IsKindOf (RUNTIME_CLASS (CDgnCanvas)))
		{
			m_pDropList = (CDgnCanvas*)pDropWnd; //Set pointer to the list we are dropping on
			DropItemOnList(m_pDragList, m_pDropList); //Call routine to perform the actual drop
		}
	}

	CFormView::OnLButtonUp(nFlags, point);
}

void CIsoEditForm::DropItemOnList(CListCtrl *pDragList, CDgnCanvas* pDropWnd)
{
	if(NULL != pDropWnd)
	{
		vector<LineDataRecordMap> IsoEditItemEntry;
		GetSelectedLineDataRecord(IsoEditItemEntry);
		AddIsoEditItemEntry(&IsoEditItemEntry);
	}
}

/**	
	@brief	The CIsoEditForm::StatusThread function

	@author	humkyung

	@return	UINT	
*/
UINT CIsoEditForm::StatusThread()
{
	CWorkStatusDlg* pWorkStatusDlg = CWorkStatusDlg::GetInstance();
	try
	{
		InterlockedExchange((LONG*)(&(pWorkStatusDlg->m_bThreadRunning)) , TRUE);/// so the main thread knows that this thread is still running
		
		vector<int> SelItemEntry;
		const int nItemCount = m_wndLineDataReport.GetItemCount();
		for( int i = 0;i < nItemCount;++i)
		{
			if(TRUE == m_wndLineDataReport.GetCheck(i)) SelItemEntry.push_back( i );
		}
		if( SelItemEntry.empty() ) return IDCANCEL;

		CAppDocData*  pDocData  = CAppDocData::GetInstance();	
		if(pDocData)
		{
			TCHAR szBuf[MAX_PATH + 1] = {'\0' ,};
			const CString rIniFilePath = pDocData->GetPrjIniFilePath();
			
			CString rISOFileFolder = m_rJobIsoDwgFileFolder;
			if(_T("\\") != rISOFileFolder.Right(1)) rISOFileFolder += _T("\\");
			
			const bool bSyncLog = false;

			/// Iso Edit 에러 개수를 초기화 한다.
			pDocData->ClearIsoEditError();

			if(NULL != m_pViewLogFileForm) m_pViewLogFileForm->ClearAllLog();
			CString rTitle("Iso Editing...") , rLog;
			const int nSelItemCount = SelItemEntry.size();
			double dProgress = 0.f;
			for(vector<int>::iterator itr = SelItemEntry.begin(); itr != SelItemEntry.end();++itr)
			{	
				CLineDataRecord* pLineDataRecord = (CLineDataRecord*)m_wndLineDataReport.GetItemData(*itr);
				if(NULL == pLineDataRecord) continue;
				
				CString rSeedFilePath = rISOFileFolder + m_wndLineDataReport.GetItemText(*itr , FILE_NAME_COL);
				rLog.Format(_T("[ %3d / %3d ] : %s") , ((int)dProgress) + 1 , nSelItemCount , m_wndLineDataReport.GetItemText(*itr , FILE_NAME_COL));
				if(NULL != m_pViewLogFileForm) m_pViewLogFileForm->AddLog(rLog , bSyncLog);
				ProcessIsoFile(pLineDataRecord , rSeedFilePath , rIniFilePath);
				
				int progress = (int)((++dProgress) / nSelItemCount * 100);
				CWorkStatusDlg::UpdateWorkStatus(rTitle , progress);
			}
			
			CString rCheckMsg;
			rCheckMsg.Format(_T("ERROR SUMMARY OF %d DGN FILE(S)") , SelItemEntry.size());
			if(NULL != m_pViewLogFileForm) m_pViewLogFileForm->AddLog(rCheckMsg , bSyncLog);
			rCheckMsg = _T("=============================================");
			if(NULL != m_pViewLogFileForm) m_pViewLogFileForm->AddLog(rCheckMsg , bSyncLog );
			int nTotalErrorCount = 0;
			for(int i = 0;i < CHECK_ERROR_COUNT ;++i)
			{
				rCheckMsg.Format(_T("%30s : %d") , CAppDocData::m_pCheckMessages[i] , CAppDocData::m_nIsoEditErrorCount[i]);
				if(NULL != m_pViewLogFileForm) m_pViewLogFileForm->AddLog(rCheckMsg , bSyncLog);
				nTotalErrorCount += CAppDocData::m_nIsoEditErrorCount[i];
			}
			rCheckMsg = _T("=============================================");
			if(NULL != m_pViewLogFileForm) m_pViewLogFileForm->AddLog(rCheckMsg , bSyncLog);
			rCheckMsg.Format("%30s  : %d" , "TOTAL ERROR COUNT " , nTotalErrorCount);
			if(NULL != m_pViewLogFileForm) m_pViewLogFileForm->AddLog(rCheckMsg , bSyncLog);
		}

		InterlockedExchange((LONG*)(&(pWorkStatusDlg->m_bThreadRunning)) , FALSE);
		//////////////////////////////////////////////////////////////////////////
		/// close work status dialog
		pWorkStatusDlg->SendMessage(WM_COMMAND , IDOK );
		
		return IDOK;
	}
	catch(...)
	{
		AfxMessageBox("Unknown error");
		
		InterlockedExchange((LONG*)(&(pWorkStatusDlg->m_bThreadRunning)) , FALSE);
		
		//////////////////////////////////////////////////////////////////////////
		/// close work status dialog
		pWorkStatusDlg->SendMessage(WM_COMMAND , IDOK );
	}
	
	return IDCANCEL;
}

UINT CIsoEditForm::StatusThreadEntry(LPVOID pVoid)
{
	return ((CIsoEditForm*)pVoid)->StatusThread();
}

/**	
	@brief	The CIsoEditForm::ProcessIsoFile function
	@author	humkyung
	@param	pLineDataRecord	a parameter of type CLineDataRecord*
	@param	rSeedFilePath	a parameter of type const CString&
	@param	rIniFilePath	a parameter of type const CString&
	@remarks\n
	1. RESERVED DATA를 iso file에 write한다.
	2. REVISION DATA를 iso file에 write한다.
	3. call smart iso diff module if rev. automation button is check
	@return	void	
*/
void CIsoEditForm::ProcessIsoFile(CLineDataRecord* pLineDataRecord , const CString& rSeedFilePath , const CString& rIniFilePath)
{
	assert(pLineDataRecord && "pLineDataRecord is NULL");

	if(pLineDataRecord)
	{
		CAppDocData*  pDocData  = CAppDocData::GetInstance();
		CUserAppDoc* pDoc = CUserAppDoc::GetInstance();
		CRevDataRecordSet* pRevDataRecordSet = (CRevDataRecordSet*)pLineDataRecord->GetRevDataRecordSet();
		const int nRevCount = pRevDataRecordSet->GetRecordCount();

		TCHAR szBuf[MAX_PATH + 1] = {'\0' ,};
		CString sOutputFilePath , rTempOutPutFilePath;
		
		const CString sApp(_T("Iso Edit Setting"));
		if(GetPrivateProfileString(sApp , _T("Output Iso Dwg File Folder") , _T("") ,szBuf , MAX_PATH,rIniFilePath))
		{
			sOutputFilePath = szBuf;	
		}
		if('\\' != sOutputFilePath[sOutputFilePath.GetLength() - 1]) sOutputFilePath += _T("\\");
		const CString rOutputIsoFileName = pDocData->GetProject()->GetOutputIsoFileName(pLineDataRecord);
		const CString sOutputIsoFileExt  = pDocData->GetProject()->GetOutputIsoFileExt(pLineDataRecord);
		sOutputFilePath += rOutputIsoFileName;
		sOutputFilePath += _T(".") + sOutputIsoFileExt;

		rTempOutPutFilePath = CString(pDocData->GetProject()->path()) + _T("\\Temp\\");
		rTempOutPutFilePath += rOutputIsoFileName;
		rTempOutPutFilePath += _T(".") + sOutputIsoFileExt;

		IsoOutPath oTemp;
		{
			oTemp.rOutputFilePath = sOutputFilePath;
			oTemp.rTempFilePath   = rTempOutPutFilePath;

			/// save idf file information - 2012.11.10 added by humkyung
			if(GetPrivateProfileString(sApp , _T("Source Iso File Folder") , _T("") ,szBuf , MAX_PATH,rIniFilePath))
			{
				CString sIDFFilePath = szBuf;
				if(_T("\\") != sIDFFilePath.Right(1)) sIDFFilePath += _T("\\");
				const CString sIDFFileName = pDocData->GetProject()->GetInputIsoFileName(pLineDataRecord) + CString(_T(".IDF"));
				oTemp.sTempIDFFilePath += sIDFFilePath + sIDFFileName;
				oTemp.sOutputIDFFilePath = CString(pDocData->GetProject()->path()) + _T("\\PRODUCT\\IDF\\") + sIDFFileName;
			}
			/// up to here
		}

		m_OutPutFileFullPath.push_back(oTemp);

		IsoTemp2OutputMatch oMatch;
		const int len = rSeedFilePath.GetLength();
		const int at = rSeedFilePath.ReverseFind('\\');
		CString rTempSeedFileName = rSeedFilePath.Right(len - at - 1);
		oMatch.rSeedFileName = rTempSeedFileName;
		oMatch.rTempFileName = rTempOutPutFilePath;

		m_Temp2OutMatch.push_back(oMatch);

		CDgnDocument* pDgnDoc = CDgnDocument::CreateInstance();
		//////////////////////////////////////////////////////////////////////////
		if(pDgnDoc->OpenForWrite(rTempOutPutFilePath , CDgnDocument::UOR_BASE , rSeedFilePath))
		{
			DeleteRemoveArea(pDgnDoc);
			//static const CString rApp(_T("Reserved Data Field"));
			////////////////////////////////////////////////////////////////////////////
			//// Reserved Draw
			//const int nCount = GetPrivateProfileInt(rApp , _T("Count") , 0 , rIniFilePath);
			//
			//CString rKey;
			//vector<STRING_T> oResult;
			//for(int i = 0;i < nCount;++i)
			//{
			//	CResDataRecord* pResDataRecord = pLineDataRecord->GetResDataRecord();
			//	if(NULL == pResDataRecord) continue;
			//	CDataField* pResDataField = pResDataRecord->GetFieldAt(i);		 
			//	CString rInputText = pResDataField->value();
			//	rKey.Format(_T("Pos%d") , i);
			//	if(GetPrivateProfileString(rApp , rKey , _T("") , szBuf , MAX_PATH , rIniFilePath))
			//	{
			//		CTokenizer<CIsComma>::Tokenize(oResult , szBuf , CIsComma());
			//		DPoint3d origin;
			//		origin.x = atof(oResult[0].c_str()); origin.y = atof(oResult[1].c_str()); origin.z = 0.0;
			//		
			//		CDgnText* pText2d = static_cast<CDgnText*>(CDgnText::CreateInstance());
			//		if(pText2d)
			//		{
			//			pText2d->SetLineColor( pDocData->m_borderSetting.TextColorNo );
			//			pText2d->SetLineWeight( pDocData->m_borderSetting.iTextWeight );
			//			pText2d->Set(pDocData->m_borderSetting.FontNo , origin , rInputText);
			//			pText2d->SetTextWidth(pDocData->m_borderSetting.widthMulti);
			//			pText2d->SetTextHeight(pDocData->m_borderSetting.heightMulti);
			//			pDgnDoc->Add(pText2d);
			//		}
			//	}
			//}
			//////////////////////////////////////////////////////////////////////////
			// Revision Draw
			CRevDataRecordSet* pRevDataRecordSet = pLineDataRecord->GetRevDataRecordSet();
			if(pRevDataRecordSet) DrawRevDataToOutputDwg( pDgnDoc , pRevDataRecordSet , rIniFilePath );
			
			/// draw line data record field
			DrawLineDataToOuputDwg( pDgnDoc , pLineDataRecord , rIniFilePath );

			pDgnDoc->Write();	
		}
		CDgnDocument::DeleteInstance(pDgnDoc);

		/// execute rev. automation module - 2012.01.03 added by humkyung
		if(pDoc->m_bRevAutomation)
		{
			ExecuteSmartISODiff( pLineDataRecord );
		}
		/// up to here
		
		/// quality control check
		try
		{
			CString rCheckMsg;

			CQCCheck qcCheck;
			
			qcCheck.Check(rCheckMsg , rSeedFilePath);
			const bool bSyncLog = false;///(TRUE == pWorkspaceBar->m_IsoEditOptionDlg.GetOptionCheck( LOG_FILE_SYNCHRONIZING )) ? true : false;
			if(NULL != m_pViewLogFileForm) m_pViewLogFileForm->AddLog(rCheckMsg , bSyncLog);
		}
		catch(...)
		{

		}
		/// up to here
	}
}

/**
	@brief	

	@author	humkyung

	@param	p	LogViewFileForm pointer
**/
void CIsoEditForm::SetViewLogFileForm(CIsoEditViewLogFileForm *p)
{
	assert(p && "Null Parameter");

	if(p) m_pViewLogFileForm = p;
}

/**	
	@brief	선택한 DGN 파일을 오픈한다.

	@author	humkyung

	@date	????.??.??

	@param	pNMHDR	a parameter of type NMHDR*
	@param	pResult	a parameter of type LRESULT*

	@return	void	
*/
void CIsoEditForm::OnDblclkListLineData(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	POSITION pos = m_wndLineDataReport.GetFirstSelectedItemPosition();
	int nItem = m_wndLineDataReport.GetNextSelectedItem(pos);
	CString rValue = m_wndLineDataReport.GetItemText(nItem , FILE_NAME_COL);
	if(nItem != -1)
	{
		if( _T("\\") != m_rJobIsoDwgFileFolder.Right(1)) m_rJobIsoDwgFileFolder += _T("\\");
		rValue = m_rJobIsoDwgFileFolder + rValue;
		if(CFileTools::DoesFileExist(rValue))
		{
			TCHAR szBuf[MAX_PATH + 1]={'\0',};
			CString rIniPath = m_rIniFilePath; 
			CString rDWGFilePath = m_rJobIsoDwgFileFolder;
			if(_T("\\") != rDWGFilePath.Right(1)) rDWGFilePath += _T("\\");
			rDWGFilePath += m_wndLineDataReport.GetItemText(nItem , FILE_NAME_COL);
			
			if( GetPrivateProfileString(_T("Iso Edit Setting") , _T("MStation Path") , _T("") ,szBuf,MAX_PATH,rIniPath) )
			{
				CString rMSFilePath = szBuf;
				if( !rMSFilePath.IsEmpty() )
				{
					::ShellExecute(NULL, _T("open") ,rMSFilePath,rDWGFilePath,NULL,SW_SHOW);
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
	@brief	환경 설정 파일에서 데이터를 읽는다.

	@param	rIniFilePath	a parameter of type const CString &

	@return	void	
*/
void CIsoEditForm::LoadData(const CString &rIniFilePath)
{
	CString rApp( "Iso Edit Setting" );
	TCHAR szBuf[MAX_PATH] = {'\0' ,};
	if(GetPrivateProfileString( rApp , _T("Source Iso File Folder") , NULL , szBuf , MAX_PATH , m_rIniFilePath))
	{
		m_rSourceIsoDwgFileFolder = szBuf;
	}
	if(GetPrivateProfileString( rApp , _T("Output Iso Dwg File Folder") , NULL , szBuf , MAX_PATH , m_rIniFilePath))
	{
		m_rEditedIsoDwgFileFolder = szBuf;
	}
	if(GetPrivateProfileString( rApp , _T("Select From") , NULL , szBuf , MAX_PATH , m_rIniFilePath))
	{
		if( CString("Source") == szBuf )
		{
			m_rJobIsoDwgFileFolder = m_rSourceIsoDwgFileFolder;
			m_bEditIsoFolder = FALSE;
		}
		else
		{
			m_rJobIsoDwgFileFolder = m_rEditedIsoDwgFileFolder;
			m_bEditIsoFolder = TRUE;
		}
	}
	else	
	{
		/// 기본 설정은 Source이다.
		m_rJobIsoDwgFileFolder = m_rSourceIsoDwgFileFolder;
		m_bEditIsoFolder = FALSE;
	}

	UpdateData(FALSE);
}

void CIsoEditForm::ShowRecordCount(int nRecordCount)
{
	/*m_nRecordCount = nRecordCount;
	CWnd* pWnd = GetDlgItem(IDC_STATIC_SELECTED_LINE);
	if(pWnd)
	{
		CString rCount;
		rCount.Format( _T("Selected Line(s) : %d") ,m_nRecordCount);
		pWnd->SetWindowText((LPCTSTR)rCount);
	}*/
}

/**	
	@brief	output dwg에 revison data를 적는다.
	@author	HumKyung
	@date	????.??.??
	@param	pRevDataRecordSet	a parameter of type CRevDataRecordSet *
	@return	void	
*/
void CIsoEditForm::DrawRevDataToOutputDwg(CDgnDocument* pDgnDoc , CRevDataRecordSet *pRevDataRecordSet , const CString& rIniFilePath )
{
	assert( pDgnDoc && "pDgnDoc is NULL" );
	assert( pRevDataRecordSet && "pRevDataRecordSet is NULL" );
	CAppDocData*  pDocData  = CAppDocData::GetInstance();

	if( pDgnDoc && pRevDataRecordSet )
	{
		static const CString rApp(_T("Revision Data Field"));
		TCHAR szBuf[MAX_PATH + 1] = {'\0' ,};
		
		///const double dScaleFactor = pDocData->GetLineDataFieldTextSacale();	//! 2011.09.22 added by humkyung

		const int nDisCount = GetPrivateProfileInt(rApp,_T("Display Count"),0,rIniFilePath);
		double nDisTance = 0.f;
		if(GetPrivateProfileString(rApp , _T("Display Distance") , _T("") , szBuf , MAX_PATH , rIniFilePath))
		{
			nDisTance = ATOF_T(szBuf);
		}
		CString rSort;
		if(GetPrivateProfileString(rApp , _T("Sort") , _T("ASCEND") , szBuf , MAX_PATH , rIniFilePath))
		{
			rSort = szBuf;
			if(rSort != _T("ASCEND")) nDisTance*=(-1);
		}
		
		list<STRING_T> oFieldNameEntry;
		const CString rSqlFilePath = CAppDocData::GetExecPath() + _T("\\Setting\\T_ISO_REVISION.XML");
		CSQLFile sqlFile(rSqlFilePath.operator LPCSTR());
		if(ERROR_SUCCESS == sqlFile.ParseXML(rSqlFilePath.operator LPCTSTR()))
		{
			const int nCount = sqlFile.GetColumnCount();
			for(int i = 0;i < nCount;++i)
			{
				const string rFieldName = sqlFile.GetFieldNameAt(i);
				oFieldNameEntry.push_back(rFieldName);
			}
		}

		CProjectData* pProjectData = CProjectData::GetInstance();

		CString rKey;
		vector<string> oResult;
		const int nRecordCount = pRevDataRecordSet->GetRecordCount();
		int nDisIndex = 0;
		for(int i = ((nRecordCount > nDisCount) ? (nRecordCount - nDisCount) : 0) ; i < nRecordCount;++i)
		{
			CRevDataRecord* pRevDataRecord = pRevDataRecordSet->GetRecordAt(i);
			CDataField* pDataField = pRevDataRecord->Find(_T("REV_NO"));
			const int iRevNo   = atoi(pDataField->value());
			const int iPrefixNo = (iRevNo / 100);
			if(false == pDocData->IsThisGradeShow(iPrefixNo)) continue;
			
			for(list<STRING_T>::iterator itr = oFieldNameEntry.begin();itr != oFieldNameEntry.end();++itr)
			{
				if((_T("KEY") == (*itr)) || (_T("GRADE") == (*itr))) continue;

				rKey.Format(_T("%s") , itr->c_str());				
				if(GetPrivateProfileString(rApp , rKey , _T("") , szBuf , MAX_PATH , rIniFilePath))
				{
					CTokenizer<CIsComma>::Tokenize(oResult , szBuf , CIsComma());
					if(oResult.size() >= 2)
					{
						DPoint3d origin;
						origin.x = atof(oResult[0].c_str());
						origin.y = atof(oResult[1].c_str()) + (nDisTance * nDisIndex);
						origin.z = 0.0;

						CString rInputText;
						CDataField* pRevDataField = pRevDataRecord->Find(itr->c_str());
						if(pRevDataField)
						{
							/// get string to be displayed in iso drawing
							rInputText = pRevDataField->value();

							if(_T("REV_NO") == STRING_T(pRevDataField->name()))
							{
								const int nRevNo = atoi(rInputText);
								rInputText = (*pDocData->m_pRevisionStageMap)[nRevNo].rText.c_str();
							}
							/// get display text for revision date - 2011.09.22 added by humkyung
							else if((_T("REV_DATE") == STRING_T(pRevDataField->name())) || (_T("REV_CONTROL_WORKING_DATE") ==STRING_T(pRevDataField->name())))
							{
								rInputText = pProjectData->GetDisplayIssueDateFrom( rInputText );
							}
							/// up to here
							else if(_T("REV_DESC") == STRING_T(pRevDataField->name()))
							{
								CDataField* pRevDataField = pRevDataRecord->Find(_T("REV_NO"));
								if(pRevDataField)
								{
									RevisionStage oRevisionStage;
									const int iRevNo = ATOI_T(pRevDataField->value());
									if(ERROR_SUCCESS == pDocData->GetRevisionStage(oRevisionStage,iRevNo))
									{
										const CString sDefine(oRevisionStage.rDefine.c_str());
										vector<GradeDefine>::const_iterator where = find_if(pDocData->m_pGradeDefineEntry->begin(),pDocData->m_pGradeDefineEntry->end(),[sDefine](GradeDefine param){return (sDefine==param.rPrefix);});
										if(where != pDocData->m_pGradeDefineEntry->end())
										{
											rInputText = where->rDesc;
										}
									}
								}
							}
						}
						if(rInputText.IsEmpty())	continue;
						
						CDgnText* pText2ad = static_cast<CDgnText*>(CDgnText::CreateInstance());
						if(pText2ad)
						{
							pText2ad->SetJustification(CDgnText::JUST_CENTER_CENTER);
							
							pText2ad->SetLineColor( pDocData->m_borderSetting.TextColorNo );
							pText2ad->SetLineWeight( pDocData->m_borderSetting.iTextWeight );
							
							pText2ad->Set(pDocData->m_borderSetting.FontNo , origin , rInputText);
							pText2ad->SetTextWidth(pDocData->m_borderSetting.widthMulti);
							pText2ad->SetTextHeight(pDocData->m_borderSetting.heightMulti);
							
							/// recalculate origin position for JUST_CENTER_CENTER - 2011.07.09 added by humkyung
							{
								CDgnVolume vol = pText2ad->volume();
								origin.x = (origin.x - (vol.maxx() - vol.minx())*0.5);
								origin.y = (origin.y - (vol.maxy() - vol.miny())*0.5);
								pText2ad->Set( pDocData->m_borderSetting.FontNo , origin , rInputText.operator LPCTSTR());
							}
							
							pDgnDoc->Add(pText2ad);
						}
						///}
					}
				}
			}
			nDisIndex++;
		}

		/// write latest revision data
		DrawLatestRevDataToOutputDwg(pDgnDoc , pRevDataRecordSet , rIniFilePath);
	}
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-25
    @class      CIsoEditForm
    @function   DrawLatestRevDataToOutputDwg
    @return     void
    @param      CDgnDocument*          pDgnDoc
    @param      CRevDataRecordSet   *pRevDataRecordSet
    @param      const               CString&
    @param      rIniFilePath
    @brief		write latest revision data
******************************************************************************/
void CIsoEditForm::DrawLatestRevDataToOutputDwg(CDgnDocument* pDgnDoc , CRevDataRecordSet *pRevDataRecordSet , const CString& rIniFilePath )
{
	assert( pDgnDoc && "pDgnDoc is NULL" );
	assert( pRevDataRecordSet && "pRevDataRecordSet is NULL" );
	CAppDocData*  pDocData  = CAppDocData::GetInstance();

	if( pDgnDoc && pRevDataRecordSet )
	{
		//! 맨 나중의 REV NO.를 표기한다.
		CRevDataRecord* pRevDataRecord = pRevDataRecordSet->FindLatestRevDataRecord();
		if(pRevDataRecord)
		{
			const int nLastRevNo = atoi(pRevDataRecord->GetRevNo());
			for(map<CString,RevPos>::iterator itr = pDocData->m_oLatestRevData.begin();itr != pDocData->m_oLatestRevData.end();++itr)
			{
				CString sFieldName(itr->first);
				sFieldName.Replace(_T("LATEST_") , _T(""));

				CString sValue;
				if(_T("REV NO") == sFieldName)
				{
					sValue = (*pDocData->m_pRevisionStageMap)[nLastRevNo].rText.c_str();
				}
				else
				{
					CDataField* pDataField = pRevDataRecord->FindWithName(sFieldName);
					if(NULL != pDataField) sValue = pDataField->value();
				}

				DPoint3d origin;
				origin.x = itr->second.PosX;
				origin.y = itr->second.PosY;
				origin.z = 0.0;
				const double dTextScale = itr->second.dTextScale;

				CDgnText* pText2ad = static_cast<CDgnText*>(CDgnText::CreateInstance());
				if(pText2ad)
				{
					pText2ad->SetJustification(CDgnText::JUST_CENTER_CENTER);

					pText2ad->SetLineColor( pDocData->m_borderSetting.TextColorNo );
					pText2ad->SetLineWeight( pDocData->m_borderSetting.iTextWeight );
					pText2ad->Set(pDocData->m_borderSetting.FontNo , origin , sValue);
					pText2ad->SetTextWidth(pDocData->m_borderSetting.widthMulti * dTextScale);		/// text scale is multiplied - 2011.09.22 added by humkyung
					pText2ad->SetTextHeight(pDocData->m_borderSetting.heightMulti * dTextScale);	/// text scale is multiplied - 2011.09.22 added by humkyung

					//! recalculate origin position for JUST_CENTER_CENTER
					{
						CDgnVolume vol = pText2ad->volume();
						origin.x = (origin.x - (vol.maxx() - vol.minx())*0.5);
						origin.y = (origin.y - (vol.maxy() - vol.miny())*0.5);
						pText2ad->Set( pDocData->m_borderSetting.FontNo , origin , sValue.operator LPCTSTR());
					}

					pDgnDoc->Add(pText2ad);
				}
			}
		}
	}
}

/**	
	@brief	temp  폴더에 있는 파일들을 찾는다.

	@author	HumKyung
**/
void GetTempDataFiles( vector< CString >& workDataFiles , const CString folder , const CString rFilter = "*.")
{
        CFileFind finder;
	BOOL working = finder.FindFile( folder + "\\" + rFilter );
	while ( working )
	{	
		working = finder.FindNextFile();
			
		if ( finder.IsDots() ) continue;
		if ( finder.IsDirectory() )
		{
			/// 하위폴더를 뒤져본다.	
			GetTempDataFiles( workDataFiles , finder.GetFilePath() , rFilter);
		}
		else
		{
			CString curFilePath = finder.GetFilePath();
			workDataFiles.push_back( curFilePath );
		}
	}
}

/**	
	@brief	작업을 수행한후 옵션에 따라 후 처리를 실시한다.
	@remarks
	1. REMOVE TEMP FILE이 체크 되어 있으면 TEMP 폴더에 있는 파일들을 삭제한다.
	@author	HumKyung
	@date	????.??.??
	@param	
	@return	void	
*/
void CIsoEditForm::PostProcess()
{
	CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();

	const BOOL checked = pWorkspaceBar->m_IsoEditOptionDlg.GetOptionCheck(REMOVE_TEMP_FILE);
	if( checked )
	{
		for(vector<IsoOutPath>::const_iterator itr = m_OutPutFileFullPath.begin(); itr != m_OutPutFileFullPath.end(); ++itr)
		{
			::DeleteFile(itr->rTempFilePath);
		}
		
		/// temp 폴더에 있는 *.m 파일을 찾아 삭제한다.
		CAppDocData*  pDocData  = CAppDocData::GetInstance();

		vector< CString > TempDataFiles;
		CString rTempPath = pDocData->GetTempFolder();
		GetTempDataFiles( TempDataFiles , rTempPath , _T("*.m"));
		
		for( vector< CString >::iterator jtr = TempDataFiles.begin();jtr != TempDataFiles.end();++jtr)
		{
			DeleteFile( jtr->operator LPCTSTR() );
		}
	}

	SaveLogFile();
	SaveErrorSumFile();

	/// delete drawing index excel file - 2011.01.31 added by Humkyung
	CAppDocData* pDocData = CAppDocData::GetInstance();
	const CString sPath = pDocData->GetProjectPath().c_str() + CString(_T("\\PRODUCT\\INDEX"));
	CFileTools::DeleteFiles(sPath , _T("*.*") , false);
	/// 
}

/**	
	@brief	The CIsoEditForm::GetOutputFilePathWithLastRevNo function

	@author	HumKyung

	@date	????.??.??

	@param	pLineDataRecord	a parameter of type CLineDataRecord *

	@return	CString	
*/
CString CIsoEditForm::GetOutputFilePathWithLastRevNo(CLineDataRecord *pLineDataRecord)
{
	assert( pLineDataRecord && "pLineDataRecord is NULL" );
	CString rOutputFilePath;

	if( pLineDataRecord )
	{
		CRevDataRecordSet* pRevDataRecordSet = (CRevDataRecordSet*)pLineDataRecord->GetRevDataRecordSet();
		int nRevCount = pRevDataRecordSet->GetRecordCount();
		CString rLastRevNo;
		if(pRevDataRecordSet && (nRevCount!=0))
		{
			for(int i =0; i < nRevCount; ++i)
			{
				CRevDataRecord* pRevDataRecord = (CRevDataRecord*)pRevDataRecordSet->GetRecordAt(i);
				if(i == nRevCount - 1) ///< 맨 마지막
				{
					rLastRevNo = pRevDataRecord->GetRevNo();
					rLastRevNo = "." + rLastRevNo;
				}
			}
		}

		char szBuf[MAX_PATH] = {'\0' ,};
		if(GetPrivateProfileString("Iso Edit Setting","Output Iso Dwg File Folder","",szBuf , MAX_PATH, m_rIniFilePath ))
		{
			rOutputFilePath = szBuf;	
		}
		if('\\' != rOutputFilePath[rOutputFilePath.GetLength() - 1]) rOutputFilePath += "\\";

		CAppDocData*  pDocData  = CAppDocData::GetInstance();
		rOutputFilePath += pDocData->GetProject()->GetOutputIsoFileName(pLineDataRecord);
		rOutputFilePath += rLastRevNo;
	}

	return rOutputFilePath;
}

/**	
	@brief	

	@remarks
**/
void CIsoEditForm::PreProcess()
{
}

/**	
	@brief	Line Data Field 항목을 Iso 파일에 그린다.
	@author	HumKyung.Baek
	@date	????.??.??
	@param	pDgnDoc	a parameter of type CDgnDocument *
	@param	pLineDataRecord	a parameter of type CLineDataRecord *
	@param	rIniFilePath	a parameter of type const CString &
	@return	void	
*/
void CIsoEditForm::DrawLineDataToOuputDwg(CDgnDocument *pDgnDoc, CLineDataRecord *pLineDataRecord, const CString &rIniFilePath)
{
	assert( pDgnDoc && "pDgnDoc is NULL" );
	assert( pLineDataRecord && "pLineDataRecord is NULL" );
	CAppDocData*  pDocData  = CAppDocData::GetInstance();
	
	if( pDgnDoc && pLineDataRecord )
	{
		vector< STRING_T > oResult;
		map<STRING_T,STRING_T>* pDrawingSetup = CProjectData::GetInstance()->GetDrawingSetup();
		for(list<STRING_T>::const_iterator itr = m_oLineDataFieldNameEntry.begin();itr != m_oLineDataFieldNameEntry.end();++itr)
		{
			DPoint3d origin;
			double dTextScale = 1.0;

			map<STRING_T,STRING_T>::const_iterator where = find_if(pDrawingSetup->begin(),pDrawingSetup->end(),
				[=](const map<STRING_T,STRING_T>::value_type& param){return (0 == STRICMP_T(itr->c_str(),param.first.c_str()));});
			if(where != pDrawingSetup->end())
			{
				CTokenizer<CIsComma>::Tokenize(oResult , where->second, CIsComma());
				if((4 == oResult.size()) || (3 == oResult.size()))
				{
					if(!oResult[1].empty() && !oResult[2].empty())
					{
						///rFieldDesc = oResult[0].c_str();
						origin.x = ATOF_T(oResult[1].c_str());
						origin.y = ATOF_T(oResult[2].c_str());
						origin.z = 0.0;
						
						dTextScale = (4 == oResult.size()) ? ATOF_T(oResult[3].c_str()) : 1.0;
					}else	continue;
				}else continue;
			}else continue;

			CString sFieldValue;
			/// get data to draw on iso drawing
			CDataField* pField = pLineDataRecord->FindWithDesc(itr->c_str());
			if( pField )
			{
				sFieldValue = pField->value();
				if(sFieldValue.IsEmpty()) sFieldValue = _T("-");	/// 2011.01.26 - added by HumKyung
			}

			if(!sFieldValue.IsEmpty())
			{
				CDgnText* pText2ad = static_cast<CDgnText*>(CDgnText::CreateInstance());
				if(pText2ad)
				{
					pText2ad->SetJustification(CDgnText::JUST_CENTER_CENTER);

					pText2ad->SetLineColor( pDocData->m_borderSetting.TextColorNo );
					pText2ad->SetLineWeight( pDocData->m_borderSetting.iTextWeight );
					pText2ad->Set(pDocData->m_borderSetting.FontNo , origin , sFieldValue);
					pText2ad->SetTextWidth(pDocData->m_borderSetting.widthMulti * dTextScale);
					pText2ad->SetTextHeight(pDocData->m_borderSetting.heightMulti * dTextScale);
					
					/// recalculate origin position for JUST_CENTER_CENTER - 2011.07.09 added by humkyung
					{
						CDgnVolume vol = pText2ad->volume();
						origin.x = (origin.x - (vol.maxx() - vol.minx())*0.5);
						origin.y = (origin.y - (vol.maxy() - vol.miny())*0.5);
						pText2ad->Set( pDocData->m_borderSetting.FontNo , origin , sFieldValue.operator LPCTSTR());
					}

					pDgnDoc->Add(pText2ad);
				}
			}
		}
	}
}

/**	
	@brief	사용자가 지정한 REMOVE AREA에 속하는 텍스트를 지운다.
	@author	HumKyung
	@date	????.??.??
	@param	pDgnDoc
	@return
**/
void CIsoEditForm::DeleteRemoveArea(CDgnDocument *pDgnDoc)
{
	assert( pDgnDoc && "pDgnDoc is NULL" );
	
	CAppDocData* pDocData = CAppDocData::GetInstance();
	if( pDgnDoc )
	{
		//////////////////////////////////////////////////////////////////////////
		// 선택된 Area를 지우는 부분~
		for(vector<IsoDwgDataArea>::iterator itr =  pDocData->m_RemoveAreaEntry.begin(); itr != pDocData->m_RemoveAreaEntry.end(); ++itr)
		{
			const size_t nCount = pDgnDoc->GetEntityCount();
			for(size_t i = 0;i < nCount;i++)
			{
				CDgnElement* pEnt = pDgnDoc->GetEntityAt(i);
				if(pEnt->IsKindOf(CDgnText::TypeString()))
				{
					CDgnText* pDGNText2d = static_cast<CDgnText*>(pEnt);
					DPoint3d origin = pDGNText2d->origin();
					
					CIsVolume vol;
					vol.Add(CIsPoint3d(origin.x , origin.y , 0));
					
					if(vol.CollideWith((itr)->volume))
					{
						pDGNText2d->Delete();
					}
				}
			}
		}
	}
}

/**	
	@brief	error summary file을 저장한다.
	
	@author	백흠경

	@date	????.??.??

	@param

	@return

**/
void CIsoEditForm::SaveErrorSumFile()
{
	CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();

	//! ERROR SUM 로그 파일을 저장한다.
	BOOL checked = pWorkspaceBar->m_IsoEditOptionDlg.GetOptionCheck( QUALITY_CONTROL_CHECK );
	if( checked )
	{
		CAppDocData*  pDocData  = CAppDocData::GetInstance();
		
		SYSTEMTIME st;
		GetLocalTime( &st );
		
		CString rLogFileName;
		rLogFileName.Format( _T("%d-%02d-%02d-%d-%d.ERR_SUM_ISO") , st.wYear , st.wMonth , st.wDay , st.wHour , st.wMinute);
		/// try to create "ERR" folder - 2011.09.29
		CreateDirectory(pDocData->GetProjectPath().c_str() + CString(_T("\\PRODUCT\\ERR")) , NULL);
		/// up to here
		CString rLogFilePath ( pDocData->GetProjectPath().c_str() + CString(_T("\\PRODUCT\\ERR\\")) + rLogFileName );
		
		if(NULL != m_pViewLogFileForm) m_pViewLogFileForm->SaveErrorSum( rLogFilePath );
	}
}

/**
	@brief	log내용을 파일로 저장한다.

	@author	HumKyung

	@date	????.??.??

	@param

	@return
*/
void CIsoEditForm::SaveLogFile()
{
	CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();

	BOOL checked = pWorkspaceBar->m_IsoEditOptionDlg.GetOptionCheck( SAVE_LOG_FILE );
	if( checked )
	{
		CAppDocData* pDocData = CAppDocData::GetInstance();
		
		SYSTEMTIME st;
		GetLocalTime( &st );
		
		CString rLogFileName;
		rLogFileName.Format( _T("%d-%02d-%02d-%d-%d.LOG") , st.wYear , st.wMonth , st.wDay , st.wHour , st.wMinute);
		CString rLogFilePath ( pDocData->GetProjectPath().c_str() + CString(_T("\\PRODUCT\\ERR\\")) + rLogFileName );
		
		if(NULL != m_pViewLogFileForm) m_pViewLogFileForm->SaveLog( rLogFilePath );
	}
}

/******************************************************************************
    @author     humkyung
    @date       2012-01-03
    @class      CIsoEditForm
    @function   ExecuteSmartISODiff
    @return     void
    @brief
******************************************************************************/
void CIsoEditForm::ExecuteSmartISODiff( CLineDataRecord* pLineDataRecord ) 
{
	assert(pLineDataRecord && "pLineDataRecord is NULL");

	if((NULL != pLineDataRecord) && NULL != SmartISODiff)
	{
		CAppDocData*  pDocData  = CAppDocData::GetInstance();
		CRevDataRecordSet* pRevDataRecordSet = (CRevDataRecordSet*)pLineDataRecord->GetRevDataRecordSet();
		if(NULL == pRevDataRecordSet) return;
		const int iRevCount = pRevDataRecordSet->GetRecordCount();
		if(iRevCount >= 1)
		{
			const CString sCurrIsoFileName = pDocData->GetProject()->GetOutputIsoFileName(pLineDataRecord);
			const CString sCurrIsoFileExt  = pDocData->GetProject()->GetOutputIsoFileExt(pLineDataRecord);
			
			CRevDataRecord* pRevRecord = pRevDataRecordSet->GetRecordAt( iRevCount - ((1 == iRevCount) ? 1 : 2));
			const CString sOldIsoFileExt  = pDocData->GetProject()->GetOutputIsoFileExt(pLineDataRecord , pRevRecord->GetRevNo());

			CString sCurrIsoFilePath = CString(pDocData->GetProject()->path()) + _T("\\Temp\\");
			sCurrIsoFilePath += sCurrIsoFileName;
			sCurrIsoFilePath += _T(".") + sCurrIsoFileExt;
			
			CString sOldIsoFilePath = pDocData->GetIsoFolder4RevAutomation().c_str();
			if(_T("\\") != sOldIsoFilePath.Right(1)) sOldIsoFilePath += _T("\\");
			sOldIsoFilePath  += sCurrIsoFileName + _T(".") + sOldIsoFileExt;	
			
			if(CFileTools::DoesFileExist(sCurrIsoFilePath) && CFileTools::DoesFileExist(sOldIsoFilePath))
			{
				SmartISODiff(sCurrIsoFilePath , sOldIsoFilePath , m_rIniFilePath);
			}
		}
	}
}

/******************************************************************************
    @author     humkyung
    @date       2012-04-06
    @class      CIsoEditForm
    @function   PreTranslateMessage
    @return     BOOL
    @param      MSG*    pMsg
    @brief
******************************************************************************/
BOOL CIsoEditForm::PreTranslateMessage(MSG* pMsg)
{
	UpdateDialogControls(this, TRUE);

	return CFormView::PreTranslateMessage(pMsg);
}

/******************************************************************************
    @author     humkyung
    @date       2012-05-24
    @class      CIsoEditForm
    @function   OnHelpInfo
    @return     BOOL
    @param      HELPINFO*   pHelpInfo
    @brief
******************************************************************************/
BOOL CIsoEditForm::OnHelpInfo(HELPINFO* pHelpInfo)
{
	HWND hHtml = ::HtmlHelp(this->m_hWnd , CAppDocData::GetExecPath() + _T("\\Help\\SmartISO.chm::/Iso Edit.htm") , HH_DISPLAY_TOPIC , 0);

	return CFormView::OnHelpInfo(pHelpInfo);
}

/**
	@brief	force uncheck if iso drawing file is not found
	@author	humkyung
	@date	2015.10.22
*/
void CIsoEditForm::OnLvnItemchangedListLineData(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	if((pNMLV->uOldState ^ pNMLV->uNewState) & LVIS_STATEIMAGEMASK)
	{
		const CString sStatus = m_wndLineDataReport.GetItemText(pNMLV->iItem,3).MakeUpper();
		if(_T("NOT FOUND") == sStatus) m_wndLineDataReport.UnCheckItem(pNMLV->iItem);
	}
	*pResult = 0;
}
