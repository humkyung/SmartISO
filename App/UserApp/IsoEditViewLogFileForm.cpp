// IsoEditViewLogFileForm.cpp : implementation file
//

#include "stdafx.h"
#include "UserApp.h"
#include <excel9/ExcelAutomation.h>
#include <util/FileTools.h>
#include <ProjectData.h>
#include "IsoEditViewLogFileForm.h"
#include "UserAppDocData.h"

#include <fstream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIsoEditViewLogFileForm

IMPLEMENT_DYNCREATE(CIsoEditViewLogFileForm, CFormView)

CIsoEditViewLogFileForm::CIsoEditViewLogFileForm()
	: CFormView(CIsoEditViewLogFileForm::IDD) , m_pBoldFont(NULL)
{
	//{{AFX_DATA_INIT(CIsoEditViewLogFileForm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CIsoEditViewLogFileForm::~CIsoEditViewLogFileForm()
{
	try
	{
		if(NULL != m_pBoldFont) delete m_pBoldFont;
	}
	catch(...)
	{

	}
}

void CIsoEditViewLogFileForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIsoEditViewLogFileForm)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIsoEditViewLogFileForm, CFormView)
	//{{AFX_MSG_MAP(CIsoEditViewLogFileForm)
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, 0x101, OnDblclkErrorListCtrl)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_EXCEL_EXPORT, &CIsoEditViewLogFileForm::OnBnClickedButtonExcelExport)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_EXCEL_EXPORT, OnUpdateExcelExport)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIsoEditViewLogFileForm diagnostics

#ifdef _DEBUG
void CIsoEditViewLogFileForm::AssertValid() const
{
	CFormView::AssertValid();
}

void CIsoEditViewLogFileForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CIsoEditViewLogFileForm message handlers

BOOL CIsoEditViewLogFileForm::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{	
	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CIsoEditViewLogFileForm::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	const int gap = 3;
	
	if(m_wndTabCtrl.GetSafeHwnd())
	{
		CRect rect;
		/*
		CWnd* pWnd = GetDlgItem(IDC_BUTTON_EXCEL_EXPORT);
		pWnd->GetWindowRect(&rect);
		*/
		
		m_wndTabCtrl.SetWindowPos(NULL , gap , gap + rect.Height() , cx - gap , cy - rect.Height() - gap, SWP_NOZORDER);
	}
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-04
    @class      CIsoEditViewLogFileForm
    @function   CreateScintillaWnd
    @return     int
    @brief
******************************************************************************/
int CIsoEditViewLogFileForm::CreateScintillaWnd()
{
	if(NULL == m_wndScintilla.GetSafeHwnd())
	{
		const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;
		if (!m_wndScintilla.Create(_T("LogWnd"), WS_CHILD | WS_VISIBLE, CRect(0,0,0,0), &(this->m_wndTabCtrl), 10000)) // hb - todo autogenerate id
		{   
			LPVOID lpMsgBuf;
			::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf,0,NULL);
			/// Write to stderr
			TRACE (_T("%s\n"), (LPCTSTR)lpMsgBuf);
			/// Free the buffer.
			LocalFree( lpMsgBuf );

			return -1;
		}
		m_wndTabCtrl.AddTab(&m_wndScintilla , _T("LOG"));

		m_wndScintilla.SetLexer(SCLEX_CPP);
		m_wndScintilla.SendMessage(SCI_SETSTYLEBITS, 5, 0);
		m_wndScintilla.SendMessage(SCE_B_NUMBER, RGB(0, 127, 127));
		m_wndScintilla.SendMessage(SCE_B_KEYWORD, RGB(0, 0, 127));
		m_wndScintilla.SendMessage(SCI_COLOURISE, 0, -1);
		m_wndScintilla.SetDisplayLinenumbers();
		m_wndScintilla.Init();

		/// Set up the global default style. These attributes are used wherever no explicit choices are made.
		m_wndScintilla.SetAStyle( STYLE_DEFAULT, RGB(0 , 0 , 0) , RGB(255 , 255 , 255) , 10, _T("Courier New") );
		m_wndScintilla.SetCurrentLineHiLiting(true , RGB(232 , 232 , 255));

		/// Set syntax colors
		for ( long i = 0; g_rgbSyntaxCpp[ i ].iItem != -1; i++ )
		{
			m_wndScintilla.SetAStyle( g_rgbSyntaxCpp[ i ].iItem, g_rgbSyntaxCpp[ i ].rgb , RGB(255 , 255 , 255) , 12 , _T("Courier New") );
		}
		m_wndScintilla.SendMessage(SCI_STYLESETBOLD, SCE_C_NUMBER , TRUE);
	}

	return ERROR_SUCCESS;
}

void CIsoEditViewLogFileForm::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	static bool __init = false;

	if(false == __init)
	{
		CRect rectDummy;
		rectDummy.SetRectEmpty ();
		if (!m_wndTabCtrl.Create (CMFCTabCtrl::STYLE_3D_VS2005 , rectDummy, this, 0x100 , CMFCTabCtrl::LOCATION_BOTTOM))
		{
			TRACE0("Failed to create workspace tab window\n");
			return;      // fail to create
		}

		m_pBoldFont = new CFont;
		m_pBoldFont->CreateFont(18,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0 , _T("Arial"));
		
		if(m_wndErrorListCtrl.Create(WS_CHILD|WS_VISIBLE|WS_BORDER|LVS_REPORT|LVS_SINGLESEL , rectDummy , &m_wndTabCtrl , 0x101))	/// 2013.01.02 modified by humkyung
		{
			/// LVS_EX_GRIDLINES : 리스트에 그리드를 표시 , LVS_EX_FULLROWSELECT: 선택 지정과 표시를 전체 줄 단위 - 2013.01.02 added by humkyung
			m_wndErrorListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
			/// up to here

			m_wndErrorListCtrl.InsertColumn(0 , _TR("File Name") , LVCFMT_LEFT , 150);
			for(int i = 0;i < QC_CHECK_ITEM_COUNT;++i)
			{
				CString str;
				str.Format(_T("%d") , i + 1);
				m_wndErrorListCtrl.InsertColumn(i + 1 , str , LVCFMT_LEFT , 50);
			}

			m_wndTabCtrl.AddTab(&m_wndErrorListCtrl , _T("Error Sum"));
		}

		CreateScintillaWnd();

		__init = true;
	}
}

/**	
	@brief	log 텍스트를 추가한다.

	@param	rLog	a parameter of type const CString &

	@return	void	
*/
void CIsoEditViewLogFileForm::AddLog( const CString &rLog , const bool& bShow )
{
	CString rMsg(rLog);
	rMsg.Replace(_T("\n") , _T("\r\n"));
	if( true == bShow )
	{
		m_wndScintilla.AppendText(rMsg + _T("\r\n"));
	}
	m_LogTextEntry.push_back( rMsg.operator LPCSTR() );
}

void CIsoEditViewLogFileForm::ClearAllLog()
{
	m_LogTextEntry.clear();
}

/**	@brief	log를 파일로 저장한다.
	
	@author	humkyung

	@param	rFilePath	a parameter of type const CString &

	@return	bool	
*/
bool CIsoEditViewLogFileForm::SaveLog(const CString &rFilePath)
{
	OFSTREAM_T ofile( rFilePath.operator LPCSTR() );
	if( ofile.is_open() && ofile.good() )
	{
		for( vector< STRING_T >::iterator itr = m_LogTextEntry.begin();itr != m_LogTextEntry.end();++itr)
		{
			ofile << itr->c_str() << std::endl;
		}

		return true;
	}

	return false;
}

/**
	@brief	log를 edit ctrl에 표시한다.

	@author	humkyung
**/
void CIsoEditViewLogFileForm::DisplayLog()
{
	m_wndScintilla.SetText(_T(""));

	CString rMsg;
	long nEndLine = 0, nEndChar = 0;
	for( vector< STRING_T >::iterator itr = m_LogTextEntry.begin();itr != m_LogTextEntry.end();++itr)
	{
		rMsg = itr->c_str();
		rMsg.Replace(_T("\n") , _T("\r\n"));

		m_wndScintilla.AppendText(rMsg + _T("\r\n"));
	}
}

/**
	@brief	display error sum

	@author	humkyung

	@date	2012.08.10
**/
int CIsoEditViewLogFileForm::DisplayErrorSum()
{
	CAppDocData*  pDocData  = CAppDocData::GetInstance();
	if(NULL != pDocData)
	{
		int nErrors[CHECK_ERROR_COUNT] = {0,};

		this->m_wndErrorListCtrl.DeleteAllItems();
		this->m_wndErrorListCtrl.ClearAllTextColor();	/// 2012.10.28 added by humkyung
		for(vector<CIsoEditError>::iterator itr = pDocData->m_IsoEditErrorEntry.begin();itr != pDocData->m_IsoEditErrorEntry.end();++itr)
		{
			const int iItem = this->m_wndErrorListCtrl.InsertItem(this->m_wndErrorListCtrl.GetItemCount() , itr->m_rFileName);
			for(int i = 0;i < CHECK_ERROR_COUNT;++i)
			{
				if(i > m_wndErrorListCtrl.GetHeaderCtrl().GetItemCount()) break;	/// 2012.11.10 added by humkyung

				CString str;
				str.Format(_T("%d") , itr->m_nError[i]);
				this->m_wndErrorListCtrl.SetItemText(iItem , i + 1, str);
				/// set text color 2012.10.28 added by humkyung
				if(itr->m_nError[i] > 0)
				{
					this->m_wndErrorListCtrl.SetItemTextColor(iItem , i + 1 , RGB(255,0,0));
				}
				/// up to here

				nErrors[i] += itr->m_nError[i];
			}
		}
		
		return ERROR_SUCCESS;
	}

	return ERROR_BAD_ENVIRONMENT;
}

/**	@brief	Error summary를 파일로 저장한다.
	
	@author	humkyung

	@date	2012.08.10

	@param	rFilePath	a parameter of type const CString &

	@return	int	
*/
int CIsoEditViewLogFileForm::SaveErrorSum(const CString &rFilePath)
{
	int nIndex = 1;
	OFSTREAM_T ofile(rFilePath);
	if(ofile.is_open())
	{
		CAppDocData*  pDocData  = CAppDocData::GetInstance();

		for(vector<CIsoEditError>::iterator itr = pDocData->m_IsoEditErrorEntry.begin();itr != pDocData->m_IsoEditErrorEntry.end();++itr , ++nIndex)
		{
			CString rLog;
			rLog.Format(_T("[ %3d / %3d ] : ") , nIndex , pDocData->m_IsoEditErrorEntry.size());
			rLog += itr->OutputErrorString();
			ofile << rLog.operator LPCSTR() << std::endl;
		}

		ofile << std::endl;
		ofile << _T("ERROR SUMMARY OF ") << pDocData->m_IsoEditErrorEntry.size() << _T(" DGN FILE(S)") << std::endl;
		ofile << _T("=============================================") << std::endl;
		int nTotalErrorCount = 0;
		for(int i = 0;i < CHECK_ERROR_COUNT ;++i)
		{
			CString rTemp;
			rTemp.Format(_T("%30s : %d") , CAppDocData::m_pCheckMessages[i] , CAppDocData::m_nIsoEditErrorCount[i]);
			ofile << rTemp.operator LPCSTR() << std::endl;
			nTotalErrorCount += CAppDocData::m_nIsoEditErrorCount[i];
		}
		ofile << _T("=============================================") << std::endl;
		CString rString;
		rString.Format(_T("%30s  : %d") , _T("TOTAL ERROR COUNT ") , nTotalErrorCount);
		ofile << rString.operator LPCSTR() << std::endl;

		ofile.close();

		return ERROR_SUCCESS;
	}

	return ERROR_FILE_NOT_FOUND;
}

void CIsoEditViewLogFileForm::OnDblclkErrorListCtrl(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	POSITION pos = m_wndErrorListCtrl.GetFirstSelectedItemPosition();
	int nItem = m_wndErrorListCtrl.GetNextSelectedItem(pos);
	if(nItem != -1)
	{
		CAppDocData* pDocData = CAppDocData::GetInstance();
		const CString sIniFilePath = pDocData->GetProject()->GetIniFilePath();
		CString rApp( "Iso Edit Setting" ) , sJobIsoDwgFileFolder;
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

		CString rValue = m_wndErrorListCtrl.GetItemText(nItem , 0);

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
	@brief	

	@author	humkyung

	@date	2012.10.28
*/
BOOL CIsoEditViewLogFileForm::PreTranslateMessage(MSG* pMsg)
{
	UpdateDialogControls(this, TRUE);

	return CFormView::PreTranslateMessage(pMsg);
}

/**
	@brief	Error Count Excel Export
	@author	humkyung
	@date	2012.10.28
*/
void CIsoEditViewLogFileForm::OnBnClickedButtonExcelExport()
{
	CExcelAutomation xlApp;
	CString sExecPath;
	CFileTools::GetExecutableDirectory( sExecPath );
	if(true == xlApp.Open(sExecPath + + _T("Setting\\IsoProductTemplate.xls")))
	{
		const int iItemCount = m_wndErrorListCtrl.GetItemCount();
		const int iColumnCount = m_wndErrorListCtrl.GetHeaderCtrl().GetItemCount();
		for(int i = 0;i < iItemCount;++i)
		{
			for(int j = 0;j < iColumnCount;++j)
			{
				const CString sValue = m_wndErrorListCtrl.GetItemText(i , j);
				xlApp.SetCellValue(COleVariant(xlApp.MakeExcelNoString(i + 6 , j + 1)) , COleVariant(sValue));
			}
		}

		CTime t = CTime::GetCurrentTime();
		CString rCurrentDate = t.Format( _T("%Y-%m-%d-%H-%M") );
		
		/// 2012.11.10 added by humkyung
		CProjectData* pPrjData = CProjectData::GetInstance();	
		xlApp.SetCellValue(COleVariant(_T("A2")) , COleVariant(_T("PROJECT NAME : ") + (*(pPrjData->m_pProjectSettingMap))[_T("project_no")]));
		xlApp.SetCellValue(COleVariant(_T("L2")) , COleVariant(_T("DATE : ") + t.Format(_T("%Y-%m-%d"))));
		/// up to here

		/// 2013.01.05 added by humkyung
		CAppDocData* pDocData = CAppDocData::GetInstance();
		CString sOutputFilePath = CString(pDocData->GetProjectPath().c_str()) + CString(_T("\\PRODUCT\\ERR\\")) + rCurrentDate + _T(".xls");
		/// up to here
		xlApp.SaveAs(sOutputFilePath);
		xlApp.Close();

		::ShellExecute(NULL, _T("open") , sOutputFilePath , sOutputFilePath , NULL , SW_SHOW);
	}
}

void CIsoEditViewLogFileForm::OnUpdateExcelExport(CCmdUI* pCmdUI) 
{
	if(m_wndTabCtrl.GetSafeHwnd())
	{
		CWnd* pWnd = GetDlgItem(IDC_BUTTON_EXCEL_EXPORT);
		pWnd->EnableWindow(0 == m_wndTabCtrl.GetActiveTab());
	}
}