// DrawingPlottingForm.cpp : implementation file
//

#include "stdafx.h"
#include <Winspool.h>
#include <util/FileTools.h>
#include <SplitPath.h>
#include "UserApp.h"
#include "MainFrm.h"
#include "UserAppView.h"
#include "DrawingPlottingForm.h"
#include "UserAppDocData.h"
#include <tokenizer.h>
#include "WorkStatusDlg.h"
#include "ViewLogFileForm.h"
#include "WorkspaceWnd.h"

#include <ProjectData.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	FILE_NAME_COL	1

/////////////////////////////////////////////////////////////////////////////
// CDrawingPlottingForm

IMPLEMENT_DYNCREATE(CDrawingPlottingForm, CFormView)

CDrawingPlottingForm::CDrawingPlottingForm()
	: CFormView(CDrawingPlottingForm::IDD), m_pBoldFont(NULL), m_pLineDataRecordSet(NULL)
{
	//{{AFX_DATA_INIT(CDrawingPlottingForm)
	m_pViewLogFileForm = NULL;
	m_rOutputDrawingFolder = _T("");
	//}}AFX_DATA_INIT
}

CDrawingPlottingForm::~CDrawingPlottingForm()
{
	try
	{
		if(NULL != m_pLineDataRecordSet) delete m_pLineDataRecordSet;
		if(NULL != m_pBoldFont) delete m_pBoldFont;
		m_pViewLogFileForm = NULL;
	}
	catch (CException* e)
	{
		UNUSED_ALWAYS(e);
	}
}

void CDrawingPlottingForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDrawingPlottingForm)
	DDX_Text(pDX, IDC_STATIC_STATUS, m_rOutputDrawingFolder);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDrawingPlottingForm, CFormView)
	//{{AFX_MSG_MAP(CDrawingPlottingForm)
	ON_WM_SIZE()
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_START, OnUpdatePrint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawingPlottingForm diagnostics

#ifdef _DEBUG
void CDrawingPlottingForm::AssertValid() const
{
	CFormView::AssertValid();
}

void CDrawingPlottingForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDrawingPlottingForm message handlers

BOOL CDrawingPlottingForm::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

/**
	@brief	overrive for calling UpdateDialogControls
	@author	HumKyung	
	@date	2011.01.26
	@param
	@return
*/
BOOL CDrawingPlottingForm::PreTranslateMessage(MSG* pMsg) 
{
	UpdateDialogControls(this, TRUE);
	
	return CFormView::PreTranslateMessage(pMsg);
}

/**
	@brief	
	@author	HumKyung
	@date	????.??.??
	@param
	@return
**/
void CDrawingPlottingForm::InitContents()
{
	CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();
	if(NULL != pWorkspaceBar) pWorkspaceBar->m_wndSelectIssueDate.InitContents( this );
	
	CUserAppView* pView = static_cast<CUserAppView*>(GetParent());
	CUserAppDoc*  pDoc  = pView->GetDocument();
	CAppDocData* pDocData = CAppDocData::GetInstance();
	CString rIniFilePath= pDocData->GetProject()->GetIniFilePath();
	
	TCHAR szBuf[MAX_PATH + 1] = {'\0' ,};
	CString rApp(_T("Iso Edit Setting"));
	if(GetPrivateProfileString(rApp, _T("Source Iso File Folder") , NULL,szBuf,MAX_PATH,rIniFilePath))
	{
		m_rSourceFilePath = szBuf;
	}
	if(GetPrivateProfileString(rApp , _T("Output Iso Dwg File Folder") , NULL , szBuf , MAX_PATH , rIniFilePath))
	{
		m_rOutputDrawingFolder = szBuf;
		if( "\\" != m_rOutputDrawingFolder.Right(1)) m_rOutputDrawingFolder += _T("\\");
		UpdateData(FALSE);
	}
	rApp = _T("Border Setting");
	if(GetPrivateProfileString(rApp , _T("Border File Path") , NULL, szBuf, MAX_PATH, rIniFilePath))
	{
		m_rBorderFilePath = szBuf;
	}
}

/**
	@brief	
	@author	HumKyung
	@date	????.??.??
	@param
	@return
*/
void CDrawingPlottingForm::OnInitialUpdate() 
{
	static bool __init = false;
	CFormView::OnInitialUpdate();
	
	if(false == __init)
	{
		m_ImageList.Create(IDB_CHECKBOXES, 16, 3, RGB(255,0,255));
		
		CRect rectDummy;
		rectDummy.SetRectEmpty ();
		GetClientRect(rectDummy);
		
		CUserAppView* pView = (CUserAppView*)(GetParent());
		CUserAppDoc*  pDoc  = pView->GetDocument();
		CAppDocData* pDocData = CAppDocData::GetInstance();
				
		if(m_wndIsoFileList.SubclassWindow(GetDlgItem(IDC_LIST_ISO_FILE)->GetSafeHwnd()))
		{
			m_wndIsoFileList.SetWindowPos(NULL , 0 , 30 , 0 , 0 , SWP_NOZORDER);
			m_wndIsoFileList.SetCheckboxes();

			LONG style = GetWindowLong(m_wndIsoFileList.m_hWnd , GWL_STYLE);
			style |= WS_EX_CLIENTEDGE;
			SetWindowLong(m_wndIsoFileList.m_hWnd , GWL_EXSTYLE , style );
			m_wndIsoFileList.SetImageList(&m_ImageList , LVSIL_SMALL);
			m_wndIsoFileList.SetHeaderImageList(&m_ImageList);
			m_wndIsoFileList.SetHeadings( _T("[],30;") + _TR("File Name") + _T(",300;") + _TR("Ext") + _T(",100;") + _TR("Issue Date") + _T(",200") );
			m_wndIsoFileList.SetGridLines();
			m_wndIsoFileList.SetExtendedStyle(LVS_EX_CHECKBOXES | m_wndIsoFileList.GetExtendedStyle());
			m_wndIsoFileList.EnableHeaderCheckBox(TRUE);
		}

		__init = true;
	}
	
}

/**
	@brief
	@author	HumKyung
	@date	2011.01.28
	@param	nType
	@param	cx
	@param	cy
	@return
*/
void CDrawingPlottingForm::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	static bool __init_size = true;
		
	CRect rect;
	GetWindowRect(&rect);
	DetermineLayout(cx,rect.Height());
}

/**
	@brief	form의 size에 맞게 control들의 위치를 재조정한다.
	@author	humkyung
	@date	????.??.??
	@param	cx
	@param	cy
	@return
*/
void CDrawingPlottingForm::DetermineLayout(int cx, int cy)
{
	const int gap = 3 , LeftX = 2;
	CWnd* pWnd = NULL;
	
	CRect rect;
	pWnd = GetDlgItem(IDC_STATIC_1);
	if(pWnd)
	{
		pWnd->SetWindowPos(NULL , LeftX , 5 , 0, 0, SWP_NOSIZE);

		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);
	}

	pWnd = GetDlgItem(IDC_STATIC_STATUS);
	if(pWnd)
	{
		pWnd->SetWindowPos(NULL , rect.right + 5 , rect.top , 
			cx - rect.right - 10, rect.Height() , SWP_NOZORDER);

		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);

	}

	if(m_wndIsoFileList.GetSafeHwnd())
	{
		m_wndIsoFileList.SetWindowPos(NULL , LeftX , rect.bottom + 5 , cx - LeftX , cy - rect.bottom - 10, SWP_NOZORDER);
		m_wndIsoFileList.GetWindowRect(&rect);
		ScreenToClient(&rect);
	}
}

/**
	@brief
	@author	HumKyung
	@date	2011.01.26
	@param
	@return
**/
void CDrawingPlottingForm::OnUpdatePrint(CCmdUI* pCmdUI)
{
	/*const int iCurSel = m_cboPrinterList.GetCurSel();
	pCmdUI->Enable((-1 != iCurSel) && (m_pIsoFileList->GetItemCount() > 1));*/
}

/**
	@brief
	@author	humkyung
	@date	????.??.??
	@param
	@return
**/
int CDrawingPlottingForm::Generate() 
{
	const int nItemCount = m_wndIsoFileList.GetItemCount();
	if( nItemCount )
	{
		/// Get a pointer to the ribbon bar
		CMFCRibbonBar* pRibbon = ((CFrameWndEx*) AfxGetMainWnd())->GetRibbonBar();
		ASSERT_VALID(pRibbon);
		// Get a pointer to the Thin Width combo box
		CMFCRibbonComboBox* pPrinterComboBox = DYNAMIC_DOWNCAST( CMFCRibbonComboBox, pRibbon->FindByID(IDC_COMBO_PRINTER_LIST));
		/// Get the selected value
		const int nCurSel = pPrinterComboBox->GetCurSel();
		if (nCurSel >= 0)
		{
			m_sPrinterName = pPrinterComboBox->GetItem(nCurSel);
			m_sPrinterName.Trim();

			const CString sIniFilePath = CAppDocData::GetExecPath() + _T("\\Setting\\SmartISO.ini");
			WritePrivateProfileString(_T("SmartISO") , _T("Printer") , m_sPrinterName , sIniFilePath);
		}
		else
		{
			m_sPrinterName.Empty();
		}

		CWorkStatusDlg dlg;
		dlg.m_pThread = AfxBeginThread(StatusThreadEntry, this, THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
		if (NULL == dlg.m_pThread)
		{
			AfxMessageBox( _T("Can't create thread!!!") );
		}
		else
		{
			dlg.DoModal();
			return ERROR_SUCCESS;
		}
	}
	else { }

	return ERROR_BAD_ENVIRONMENT;
}

/**
	@brief
	@author	humkyung
	@date	2011.01.28
	@param	wParam
	@param	lParam
	@param	pResult
	@return	BOOL
*/
BOOL CDrawingPlottingForm::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();

	NMHDR* pNMHDR = (NMHDR*)(lParam);
	
	if((SELECT == wParam) && (pWorkspaceBar->m_wndSelectIssueDate.GetDlgCtrlID() == pNMHDR->idFrom))
	{	
		SetDlgItemText(IDC_STATIC_STATUS , pWorkspaceBar->m_wndSelectIssueDate.m_sSelectedFolder);

		if(m_wndIsoFileList.GetSafeHwnd())
		{
			m_wndIsoFileList.SetRedraw(FALSE);
			m_wndIsoFileList.DeleteAllItems();

			vector<CString> oFindfiles;
			CFileTools::FindFiles(oFindfiles , pWorkspaceBar->m_wndSelectIssueDate.m_sSelectedFolder, _T("*.*") , false);
			for(vector<CString>::const_iterator itr = oFindfiles.begin();itr != oFindfiles.end();++itr)
			{
				CSplitPath path(*itr);
				CString sExtension(path.GetExtension());
				sExtension.MakeUpper();
				const CString sIsoFileName = path.GetFileName().MakeUpper();
				{
					const int it = m_wndIsoFileList.InsertItem(m_wndIsoFileList.GetItemCount() , _T("") , 0);
					m_wndIsoFileList.SetItemData(it , (DWORD)NULL);
					m_wndIsoFileList.SetItemText(it , 1 , sIsoFileName);
					m_wndIsoFileList.SetItemText(it , 2 , sExtension);
				}
			}
			m_wndIsoFileList.SetRedraw(TRUE);
		}
	}
	if((SELECT_ISSUED == wParam) && (pWorkspaceBar->m_wndSelectIssueDate.GetDlgCtrlID() == pNMHDR->idFrom))
	{	
		SetDlgItemText(IDC_STATIC_STATUS , _T(""));
		if(m_wndIsoFileList.GetSafeHwnd())
		{
			CString sWhereClause;

			vector<STRING_T> aIssueDateEntry;
			pWorkspaceBar->m_wndSelectIssueDate.GetSelectedIssueDate(aIssueDateEntry);
			if(!aIssueDateEntry.empty())
			{
				sWhereClause = _T("ISSUE_DATE IN (");
				for(vector<STRING_T>::const_iterator itr = aIssueDateEntry.begin();itr != aIssueDateEntry.end();++itr)
				{
					sWhereClause += _T("'") + CString(itr->c_str()) + _T("',");
				}
				sWhereClause = sWhereClause.TrimRight(_T(",")) + _T(")");
			}
		
			LoadLineDataRecordSetFromDatabase(sWhereClause);
		}
	}
	
	return CFormView::OnNotify(wParam, lParam, pResult);
}

/**
	@brief	
	@author	humkyung
**/
void CDrawingPlottingForm::LoadLineDataRecordSetFromDatabase(const CString &sWhereClause)
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	if(NULL != pDocData)
	{
		CWorkspaceWnd* pWorkspaceBar = GetUserAppMainWnd()->GetWorkspaceBar();

		if(NULL != m_pLineDataRecordSet) CLineDataRecordSet::DeleteInstance(m_pLineDataRecordSet);
		
		BeginWaitCursor();
		m_pLineDataRecordSet = pDocData->GetProject()->GetIssuedLineDataRecordSet(sWhereClause);
		if(NULL != m_pLineDataRecordSet)
		{
			const int nRecordCount = m_pLineDataRecordSet->GetLineDataRecordCount();
			if(0 == nRecordCount)
			{
				m_wndIsoFileList.DeleteAllItems();
				return;
			}
			DisplayOutputFileWithDataRecord();
			m_wndIsoFileList.GroupByColumn(2);
		}
		EndWaitCursor();
	}
}

/**
	@brief	
	@author	humkyung
**/
void CDrawingPlottingForm::DisplayOutputFileWithDataRecord()
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	const CString name = pDocData->GetProject()->name();
	const CProject::DatabaseProp prop = pDocData->GetProject()->GetDatabaseProp();
	CString rIniFilePath = pDocData->GetProject()->GetIniFilePath();
	
	m_wndIsoFileList.SetRedraw(FALSE);
	m_wndIsoFileList.DeleteAllItems();
	
	LVITEM lvitem;
	lvitem.mask = LVIF_TEXT;
	lvitem.iImage = 0;
	lvitem.cchTextMax = 32;

	CAppPostgreSQL database(name , prop.Host.c_str() , prop.User.c_str() , prop.Password.c_str());
	try
	{
		int nFileCount = 0;
		const int nRecordCount = m_pLineDataRecordSet->GetLineDataRecordCount();
		for(int i=0 ;i < nRecordCount;++i)
		{
			CLineDataRecord* pLineDataRecord = (CLineDataRecord*)m_pLineDataRecordSet->GetLineDataRecordAt(i);
			CDataField* pDataField = pLineDataRecord->FindWithFieldName(_T("KEY"));
			const STRING_T sKey = pDataField->value();
			pDataField = pLineDataRecord->FindWithFieldName(_T("REV_NO"));
			const STRING_T sRevNo = pDataField->value();
			const STRING_T sSql = STRING_T(_T("SELECT COUNT(*) FROM T_ISO_DWG_FILE WHERE KEY='") + sKey + _T("' AND REV_NO='") + sRevNo + _T("' AND FILE_DESC='DGN V7'"));
			long long lCount=0L;
			(*database.session()) << sSql,soci::into(lCount);
			if(lCount > 0)
			{
				const CString sIsoFileName = pDocData->GetProject()->GetOutputIsoFileName(pLineDataRecord);
				const CString sIsoFileExt  = pDocData->GetProject()->GetOutputIsoFileExt(pLineDataRecord);
				{
					const int it = m_wndIsoFileList.InsertItem(i , _T("") , 0);
					m_wndIsoFileList.SetItemText(it , 1 , sIsoFileName);
					m_wndIsoFileList.SetItemData(it , (DWORD)pLineDataRecord);
					m_wndIsoFileList.SetItemText(it , 2 , _T(".") + sIsoFileExt);
					pDataField = pLineDataRecord->FindWithFieldName(_T("ISSUE_DATE"));
					m_wndIsoFileList.SetItemText(it , 3, (NULL != pDataField) ? pDataField->value() : _T(""));

					++nFileCount;
				}
			}
		}
	}
	catch(const std::exception& ex)
	{
		AfxMessageBox(ex.what(),MB_OK|MB_ICONERROR);
	}
	m_wndIsoFileList.SetRedraw( TRUE );
}

/**
**/
int CDrawingPlottingForm::System_IPlot(CString ip)
{
	int res = system(ip);
	/*
	int i=0;
	CString rtemp;
	
	strcpy(File_Temp, Tip_ED.CString2Char( ip ));
	i = system( File_Temp );

	if ( i == 0 )	ctemp.Format("              %s  : OK ", File_Temp); 
	else		ctemp.Format("              %s  : FAILED ", File_Temp); 
	Make_Log_File( rtemp );
	*/

	return res;
}

#define	IPLOTJ_PATH	"C:\\Program Files\\Common Files\\InterPlot\\IPLOT\\bin\\iplotj.exe"

/**	
	@brief	iso 도면을 plotting한다.
	@author	HumKyung
	@dates	2009.01.21
	@param	nIndex
	@param	nCount
	@param	rIsoFile
	@return
**/
void CDrawingPlottingForm::Print_IPlot(const int& nIndex , const int& nCount , const CString &rIsoFile)
{
	//! iplotj가 설치되어 있으면 iplotj로 그렇지 않으면 iplot을 사용하도록 한다.
	const CString rIplotCommand = (TRUE == CFileTools::DoesFileExist(IPLOTJ_PATH)) ? _T("iplotj") : _T("iplot");

	CAppDocData* pDocData = CAppDocData::GetInstance();
	if(NULL == pDocData) return;

	CString rPath = pDocData->GetProjectPath().c_str();
	rPath.MakeUpper();			 /// 대문자로 변환시킨다.
	if(_T("\\") != rPath.Right(1)) rPath += _T("\\");
	CString rPname = pDocData->GetProject()->name();
	
	CString rTemp, rBorder, rProjectName;
	UpdateData(TRUE);

	CString rPaperSize(_T("-PAPER_SIZE="));
	CString sPenTable;
	if ( m_sPrinterName.Find(_T("a4")) != -1 || m_sPrinterName.Find(_T("A4")) != -1 )
	{
		sPenTable = rPath + _T("PLOT_DATA\\SmartISO_a4.i");
		rPaperSize+= _T("A4");
	}
	else
	{
		sPenTable = rPath + _T("PLOT_DATA\\SmartISO_a3.i");
		rPaperSize+= _T("A3");
	}
	if( m_sPrinterName.Find("\\WHT_KIP") > 0 || m_sPrinterName.Find("\\TRC_KIP") > 0) /// 2003.6.25 페이퍼 사이즈 옵션 추가 
	{
		rPaperSize +=_T("J0"); 
	}

	CString sNewPenTable;
	int at = sPenTable.ReverseFind('\\');
	sNewPenTable = sPenTable.Left(at);

	CString rProductFile = m_rOutputDrawingFolder;
	if(_T("\\") != rProductFile.Right(1)) rProductFile += _T("\\");
	rProductFile += rIsoFile;
	const CString rCopiedFile = sNewPenTable + _T("\\") + rIsoFile;
	BOOL b = CopyFile(rProductFile, rCopiedFile , FALSE);	//! PRODUCT 파일을 PLOT_DATA 폴더로 복사한다.
	if(FALSE == b)
	{
		AfxMessageBox( _T("Product 파일을 복사할수 없습니다.") );
	}

	CString rIsoFileName = rIsoFile;
	at = rIsoFileName.ReverseFind('.');
	rIsoFileName = rIsoFileName.Left(at+1);
	rIsoFileName += CString("i");
	sNewPenTable += _T("\\") + rIsoFileName;
	b = CopyFile(sPenTable, sNewPenTable , FALSE);		///< iplot 파일을 'PRODUCT 파일 이름.i' 파일로 복사한다.
	if(FALSE == b)
	{
		AfxMessageBox(rIplotCommand + _T("(") + sPenTable + _T(")을 복사할 수 없습니다."));
	}
	sPenTable = sNewPenTable;

	if(rPname.Find("_") != -1)
	{
		rProjectName = rPname.Mid(0,rPname.Find("_"));
	}
	else
	{
		rProjectName = rPname;
	}
	
	rBorder = m_rBorderFilePath;

	CString rMsg;
	rMsg.Format("[%d / %d] %s" , nIndex , nCount , rCopiedFile.operator LPCSTR());
	//m_pViewLogFileForm->AddLog(rMsg);

	//! CAHNGE PRINTER NAME IN IPARM FILE PLOT REF FILE WITH DGN FIEL : CHANGE BORDER NAME WITH FD
	//! rTemp = rIplotCommand + " mod -queue=\"" + rQueue + "\" -ref_filename[border]=" + rBorder + "  " + rPen_Table + " " + rPaperSize;
	rTemp = rIplotCommand + _T(" mod -queue=\"") + m_sPrinterName + _T("\"") + _T(" mod -ref_filename[border]=") + rBorder + _T("  ") + 
		sPenTable + _T(" ") + rPaperSize;
	int res = System_IPlot( rTemp );
	rMsg.Format(_T("        %s"), rTemp);
	if(0 == res)
	{
		//m_pViewLogFileForm->AddLog(rMsg + _T(" : OK"));
	}/*else	m_pViewLogFileForm->AddLog(rMsg + _T(" : FAILED"));*/

	rTemp = rIplotCommand + _T(" mod -design=") + rCopiedFile + _T("  ") + sPenTable;
	res = System_IPlot( rTemp );
	rMsg.Format(_T("        %s") , rTemp);
	if(0 == res)
	{
		//m_pViewLogFileForm->AddLog(rMsg + _T(" : OK"));
	}/*else	m_pViewLogFileForm->AddLog(rMsg + _T(" : FAILED"));*/

	/// CHANGE DESIGN FILE NAME IN IPARM FILE
	rTemp = rIplotCommand + _T(" gen ") + sPenTable ;
	res = System_IPlot( rTemp );
	rMsg.Format(_T("        %s"),rTemp);
	if(0 == res)
	{
		//m_pViewLogFileForm->AddLog(rMsg + _T(" : OK"));
	}/*else	m_pViewLogFileForm->AddLog(rMsg + _T(" : FAILED"));*/

	/// 
	rTemp = rIplotCommand + _T(" sub -copies=1  ") + sPenTable;
	res = System_IPlot( rTemp );
	rMsg.Format(_T("        %s"),rTemp);
	if(0 == res)
	{
		//m_pViewLogFileForm->AddLog(rMsg + _T(" : OK"));
	}/*else	m_pViewLogFileForm->AddLog(rMsg + _T(" : FAILED"));*/
	rMsg.Format(_T("%s") , _T(""));
	/*m_pViewLogFileForm->AddLog(rMsg);
	m_pViewLogFileForm->SaveLog(CAppDocData::GetExecPath() + _T("\\DrawingPlotting.log"));*/
	
	/// delete pen table file
	::DeleteFile(sPenTable);
	::DeleteFile(rCopiedFile);
	
	/// *.m 파일을 삭제한다.
	CString rTempFilePath(rCopiedFile);
	at = rTempFilePath.ReverseFind('.');
	if(-1 != at)
	{
		rTempFilePath = rTempFilePath.Left(at+1);
		rTempFilePath += CString(_T("m"));
		::DeleteFile(rTempFilePath);
	}
}

/******************************************************************************
	@brief		return plotting area as string
    @author     humkyung
    @date       2012-09-19
    @class      CDrawingPlottingForm
    @return		CString 
******************************************************************************/
CString CDrawingPlottingForm::GetPlottingAreaString() const
{
	CString res;

	CAppDocData* pDocData = CAppDocData::GetInstance();
	const CString sIniFilePath = pDocData->GetPrjIniFilePath();
	map<STRING_T,STRING_T>* pDrawingSetup = CProjectData::GetInstance()->GetDrawingSetup();
	for(map<STRING_T,STRING_T>::const_iterator itr = pDrawingSetup->begin();itr != pDrawingSetup->end();++itr)
	{
		if(0 == STRICMP_T(_T("plotting area"),itr->first.c_str()))
		{
			const int uor_per_master = GetPrivateProfileInt(_T("Border Setting") , _T("uor per master") , 1 , sIniFilePath);

			vector<STRING_T> oResult;
			CTokenizer<CIsComma>::Tokenize(oResult , itr->second , CIsComma());
			if(4 == oResult.size())
			{
				res.Format(_T("%.0lf %.0lf %.0lf %.0lf") , 
					(ATOF_T(oResult[0].c_str()) / double(uor_per_master)) , (ATOF_T(oResult[1].c_str()) / double(uor_per_master)) ,
					(ATOF_T(oResult[2].c_str()) / double(uor_per_master)) , (ATOF_T(oResult[3].c_str()) / double(uor_per_master)));
			}
		}
	}
	
	return res;
}

/**
	@brief	선택한 LINE LIST RECORD들을 구한다.
	@author	HumKyung
	@date	????.??.??
	@param	LineDataRecordEntry - container for LineDataRecordMap
	@return
**/
void CDrawingPlottingForm::GetSelectedLineDataRecord(vector<LineDataRecordMap>& aLineDataRecordEntry)
{
	aLineDataRecordEntry.clear();
	LineDataRecordMap aLineDataRecordMap;

	const int iItemCount = m_wndIsoFileList.GetItemCount();
	for(int i = 0;i < iItemCount;++i)
	{
		if(TRUE == m_wndIsoFileList.GetCheck(i))
		{
			aLineDataRecordMap.nItem = i;
			aLineDataRecordMap.pLineDataRecord = NULL;
			aLineDataRecordEntry.push_back(aLineDataRecordMap);		
		}
	}
}

/**
	@brief	download iso dwg file from database
	@author	humkyung
	@date	2015.11.24
*/
CString CDrawingPlottingForm::DownloadIsoDwgFile(const CString& sFileName,CLineDataRecord* pLineDataRecord) const
{
	CString sFilePath;

	CAppDocData* pDocData = CAppDocData::GetInstance();
	const CProject::DatabaseProp prop = pDocData->GetProject()->GetDatabaseProp();
	CAppPostgreSQL database(pDocData->GetProject()->name(),prop.Host.c_str(),prop.User.c_str(),prop.Password.c_str());
	try
	{
		soci::transaction txn(*database.session());
		soci::blob b(*database.session());

		CDataField* pDataField = pLineDataRecord->FindWithFieldName(_T("KEY"));
		const CString sKey = pDataField->value();
		pDataField = pLineDataRecord->FindWithFieldName(_T("REV_NO"));
		const CString sRevNo = pDataField->value();

		CString sSql; 
		sSql.Format(_T("SELECT FILE FROM T_ISO_DWG_FILE WHERE KEY='%s' AND REV_NO='%s' AND FILE_DESC='DGN V7'"),sKey,sRevNo);
		(*database.session()) << sSql.operator LPCTSTR(),into(b);
		const size_t length = b.get_len();
		if(length > 0)
		{
			TCHAR *pData = NULL;
			if(pData = (TCHAR*)calloc(1 , sizeof(TCHAR)*(length + 1)))
			{
				b.read(0,pData,sizeof(TCHAR)*length);
				sFilePath = CString(pDocData->GetProjectPath().c_str()) + _T("\\Temp\\") + sFileName;
				CFile f(sFilePath, CFile::modeCreate|CFile:: modeReadWrite);
				f.Write(pData , length);
				f.Close();

				free((void*)pData);
				pData = NULL;
			}
		}
		txn.commit();
	}
	catch(const std::exception& ex)
	{
		AfxMessageBox(ex.what(),MB_OK|MB_ICONERROR);
	}

	return sFilePath;
}

/**	
	@brief	The CDrawingPlottingForm::StatusThread function
	@author	HumKyung
	@date	????.??.??
	@param
	@return	UINT	
*/
UINT CDrawingPlottingForm::StatusThread()
{
	CWorkStatusDlg* pWorkStatusDlg = CWorkStatusDlg::GetInstance();
	try
	{
		InterlockedExchange((LONG*)(&(pWorkStatusDlg->m_bThreadRunning)) , TRUE);// so the main thread knows that this thread is still running
		
		vector<LineDataRecordMap> aLineDataRecordEntry;
		GetSelectedLineDataRecord(aLineDataRecordEntry);

		CString rTitle( _T("Drawing Ploting...") ) , rLog;
		double dProgress = 0.f;

		CUserAppDoc* pDoc = CUserAppDoc::GetInstance();
		int nIndex = 0 , nCount = aLineDataRecordEntry.size();
		if(TRUE == pDoc->m_bUseIPLOT)
		{
			CString sProductFolder = m_rOutputDrawingFolder;
			if(_T("\\") != sProductFolder.Right(1)) sProductFolder += _T("\\");

			CString sIsoFilePath;
			for(vector<LineDataRecordMap>::const_iterator itr = aLineDataRecordEntry.begin(); itr != aLineDataRecordEntry.end(); ++itr,++nIndex)
			{
				const CString sIsoFileName = m_wndIsoFileList.GetItemText(itr->nItem , 1) + m_wndIsoFileList.GetItemText(itr->nItem , 2);
				CLineDataRecord* pLineDataRecord = (CLineDataRecord*)m_wndIsoFileList.GetItemData(itr->nItem);
				if(NULL != pLineDataRecord)
				{
					sIsoFilePath = DownloadIsoDwgFile(sIsoFileName,pLineDataRecord);
				}
				else
				{
					sIsoFilePath = sProductFolder + sIsoFileName;
				}
				
				Print_IPlot(nIndex , nCount , sIsoFilePath);
				
				int progress = (int)((++dProgress) / nCount * 100);
				CWorkStatusDlg::UpdateWorkStatus(rTitle , progress);
			}
		}
		else
		{
			CAppDocData* pDocData = CAppDocData::GetInstance();

			/// print iso drawings by using system printer - 2012.09.18 added by humkyung
			const CString sPlottingArea = GetPlottingAreaString();
			if(!sPlottingArea.IsEmpty())
			{
				BOOL res = ::SetDefaultPrinter(m_sPrinterName.operator LPCTSTR());
				
				CString sTempFolder = pDocData->GetTempFolder();
				OFSTREAM_T ofile(sTempFolder + _T("\\SmartISO.cmd"));
				{
					CString sProductFolder = m_rOutputDrawingFolder,sIsoFilePath;
					if(_T("\\") != sProductFolder.Right(1)) sProductFolder += _T("\\");
					for(vector<LineDataRecordMap>::const_iterator itr = aLineDataRecordEntry.begin(); itr != aLineDataRecordEntry.end(); ++itr,++nIndex)
					{
						const CString sIsoFileName = m_wndIsoFileList.GetItemText(itr->nItem , 1) + m_wndIsoFileList.GetItemText(itr->nItem , 2);

						CLineDataRecord* pLineDataRecord = (CLineDataRecord*)m_wndIsoFileList.GetItemData(itr->nItem);
						if(NULL != pLineDataRecord)
						{
							sIsoFilePath = DownloadIsoDwgFile(sIsoFileName,pLineDataRecord);
						}
						else
						{
							sIsoFilePath = sProductFolder + sIsoFileName;
						}
						
						ofile << _T("NEWFILE ") << sIsoFilePath.operator LPCTSTR() << std::endl;
						ofile << _T("MACRO SmartISO ") << sPlottingArea.operator LPCTSTR() << std::endl;
					}
					ofile.close();

					/// write keyin text file - 2015.06.11 added by humkyung
					const CString sKeyinFilePath = CFileTools::GetTempPath() + _T("keyin.txt");
					OFSTREAM_T ofile(sKeyinFilePath.operator LPCTSTR());
					if(ofile.is_open())
					{
						ofile << _T("mdl load aDgnWriter \"") << sTempFolder.operator LPCTSTR() << _T("\\SmartISO.cmd") << _T("\"") << std::endl;
						ofile << _T("QUIT");
						ofile.close();
					}
					/// up to here

					const long length = GetShortPathName(CFileTools::GetTempPath(), NULL, 0);
					if (length > 0)
					{
						TCHAR* buffer = new TCHAR[length];
						if(GetShortPathName(CFileTools::GetTempPath(), buffer, length) > 0)
						{
							const CString sSeedFilePath(_T("C:\\Bentley\\Workspace\\system\\seed\\seed3d.dgn"));

							const CString sIniFilePath = pDocData->GetProject()->GetIniFilePath();
							TCHAR szBuf[MAX_PATH + 1] = {'\0' ,};
							if(GetPrivateProfileString(_T("Iso Edit Setting") , _T("MStation Path") , _T("") ,szBuf,MAX_PATH,sIniFilePath))
							{
								const CString sArguments(_T("\"") + sSeedFilePath + _T("\" -S") + CString(buffer) + _T("keyin.txt"));
								::ShellExecute(NULL , _T("open") , szBuf , sArguments , NULL , SW_SHOW);
								delete []buffer;
							}
						}
					}
				}
				/// up to here
			}
			else
			{
				AfxMessageBox(_T("Can't get plotting area") , MB_OK | MB_ICONERROR); 
			}
		}
		
		InterlockedExchange((LONG*)(&(pWorkStatusDlg->m_bThreadRunning)) , FALSE);
		//////////////////////////////////////////////////////////////////////////
		/// close work status dialog
		pWorkStatusDlg->SendMessage(WM_COMMAND , IDOK );
		
		return IDOK;
	}
	catch(...)
	{
		AfxMessageBox( _T("Unknown error") );
		
		InterlockedExchange((LONG*)(&(pWorkStatusDlg->m_bThreadRunning)) , FALSE);
		
		//////////////////////////////////////////////////////////////////////////
		/// close work status dialog
		pWorkStatusDlg->SendMessage(WM_COMMAND , IDOK );
	}
	
	return IDCANCEL;
}

UINT CDrawingPlottingForm::StatusThreadEntry(LPVOID pVoid)
{
	return ((CDrawingPlottingForm*)pVoid)->StatusThread();
}

/**
	@brief	
	@author	humkyung
**/
void CDrawingPlottingForm::SetViewLogFileForm(CViewLogFileForm *p)
{
	assert(p && "Null Parameter");

	if(p) m_pViewLogFileForm = p;
}