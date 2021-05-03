// BMLogFileForm.cpp : implementation file
//

#include "stdafx.h"
#include "UserApp.h"
#include <Tokenizer.h>
#include "UserAppDocData.h"
#include "BMLogFileForm.h"

#include <fstream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBMLogFileForm

IMPLEMENT_DYNCREATE(CBMLogFileForm, CFormView)

CBMLogFileForm::CBMLogFileForm()
	: CFormView(CBMLogFileForm::IDD)
{
	//{{AFX_DATA_INIT(CBMLogFileForm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBMLogFileForm::~CBMLogFileForm()
{
	try
	{
		///if(NULL != m_pBoldFont) delete m_pBoldFont;
	}
	catch(...)
	{

	}
}

void CBMLogFileForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBMLogFileForm)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBMLogFileForm, CFormView)
	//{{AFX_MSG_MAP(CBMLogFileForm)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBMLogFileForm diagnostics

#ifdef _DEBUG
void CBMLogFileForm::AssertValid() const
{
	CFormView::AssertValid();
}

void CBMLogFileForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBMLogFileForm message handlers

BOOL CBMLogFileForm::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{	
	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CBMLogFileForm::OnSize(UINT nType, int cx, int cy) 
{
	const int gap = 3;
	CFormView::OnSize(nType, cx, cy);
	
	if(m_wndTabCtrl.GetSafeHwnd())
	{
		m_wndTabCtrl.SetWindowPos(NULL , gap , gap , cx - gap, cy - gap, SWP_NOZORDER);
	}
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-04
    @class      CBMLogFileForm
    @function   CreateScintillaWnd
    @return     int
    @brief
******************************************************************************/
int CBMLogFileForm::CreateScintillaWnd()
{
	if(NULL == m_wndScintilla.GetSafeHwnd())
	{
		const DWORD dwStyle = LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL;
		if (!m_wndScintilla.Create(_T("LogWnd"), WS_CHILD | WS_VISIBLE, CRect(0,0,0,0), &(this->m_wndTabCtrl) , 10000)) // hb - todo autogenerate id
		{   
			LPVOID lpMsgBuf;
			::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf,0,NULL);
			// Write to stderr
			TRACE (_T("%s\n"), (LPCTSTR)lpMsgBuf);
			// Free the buffer.
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

		// Set up the global default style. These attributes are used wherever no explicit choices are made.
		m_wndScintilla.SetAStyle( STYLE_DEFAULT, RGB(0 , 0 , 0) , RGB(255 , 255 , 255) , 10, _T("Courier New") );
		m_wndScintilla.SetCurrentLineHiLiting(true , RGB(232 , 232 , 255));

		// Set syntax colors
		for (long i = 0; g_rgbSyntaxCpp[ i ].iItem != -1; i++)
		{
			m_wndScintilla.SetAStyle( g_rgbSyntaxCpp[ i ].iItem, g_rgbSyntaxCpp[ i ].rgb , RGB(255 , 255 , 255) , 12 , _T("Courier New") );
		}
		m_wndScintilla.SendMessage(SCI_STYLESETBOLD, SCE_C_NUMBER , TRUE);
	}

	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       ????-??-??
    @class      CBMLogFileForm
    @function   OnInitialUpdate
    @return     void
    @brief
******************************************************************************/
void CBMLogFileForm::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	static bool __init__ = false;
	static const STRING_T FIELDS[]=
	{
		_T("NO,30") , _T("Drawing No,100") , _T("Rev No,50") , _T("Symbol,100") , _T("Matl Spec,100") , _T("Matl Code,100") , 
		_T("Main Size,100") , _T("Sub Size,100") , _T("Bolt Length,100") , _T("Quantity,100") , _T("Matl Desc,600")
	};

	if(false == __init__)
	{
		CRect rectDummy;
		rectDummy.SetRectEmpty ();
		if (!m_wndTabCtrl.Create (CMFCTabCtrl::STYLE_3D_VS2005 , rectDummy, this, 0x100 , CMFCTabCtrl::LOCATION_BOTTOM))
		{
			TRACE0("Failed to create workspace tab window\n");
			return;      // fail to create
		}

		if(m_wndBMDataListCtrl.Create(WS_CHILD|WS_VISIBLE|WS_BORDER|LVS_REPORT , rectDummy , &m_wndTabCtrl , 0x101))
		{
			DWORD dwStyleEx = m_wndBMDataListCtrl.GetExtendedStyle(); 
			dwStyleEx |= LVS_EX_FULLROWSELECT; 
			m_wndBMDataListCtrl.SetExtendedStyle(dwStyleEx);

			vector<STRING_T> oResult;
			m_wndBMDataListCtrl.InsertColumn(0 , _T("NO") , LVCFMT_LEFT , 30);
			for(int i = 1;i < ARRAYSIZE(FIELDS);++i)
			{
				CTokenizer<CIsComma>::Tokenize(oResult , FIELDS[i] , CIsComma());
				if(2 == oResult.size())
				{
					m_wndBMDataListCtrl.InsertColumn(i , oResult[0].c_str() , LVCFMT_LEFT , ATOI_T(oResult[1].c_str()));
				}
			}

			m_wndTabCtrl.AddTab(&m_wndBMDataListCtrl , _T("B/M Data"));
		}

		CreateScintillaWnd();

		__init__ = true;
	}
}

/**
**/
void CBMLogFileForm::AddLog(const CString &rLog)
{	
	CString rMsg(rLog);
	rMsg.Replace(_T("\n") , _T("\r\n"));
	m_wndScintilla.AppendText(rMsg + _T("\r\n"));
	
	m_LogEntry.push_back(rLog);
}

/**
	@breif	clear log text
	@author	humkyung
**/
void CBMLogFileForm::ClearAllLog()
{
	if(m_wndScintilla.GetSafeHwnd()) m_wndScintilla.SetText(_T(""));
	m_LogEntry.clear();
}

/**	
	@brief	LOG 내용을 파일로 저장한다.
	@author	humkyung
**/
bool CBMLogFileForm::Save(const CString &rFilePath)
{
	OFSTREAM_T ofile(rFilePath);
	if(ofile.is_open())
	{
		for(vector<CString>::iterator itr = m_LogEntry.begin();itr != m_LogEntry.end();++itr)
		{
			ofile << (*itr).operator LPCTSTR() << std::endl;
		}
		ofile.close();

		return true;
	}

	return false;
}

/******************************************************************************
	@brief		display bom data of iso dwg which has key and rev no
    @author     humkyung
    @date       2012-08-31
    @class      CBMLogFileForm
    @return     int
    @param      const   CString&
******************************************************************************/
int CBMLogFileForm::DisplayBOMData(const CString& sKey,const CString& sRevNo)
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	if(pDocData)
	{
		const CString rProjectPath = pDocData->GetProjectPath().c_str();
		const CProject::DatabaseProp prop = pDocData->GetProject()->GetDatabaseProp();
		const CString sIniFilePath= pDocData->GetProject()->GetIniFilePath();

		try
		{
			CAppPostgreSQL database(pDocData->GetProject()->name(),prop.Host.c_str(),prop.User.c_str(),prop.Password.c_str());

			m_wndBMDataListCtrl.DeleteAllItems();

			STRING_T sSql = _T("SELECT * FROM T_ISO_BOM_DATA WHERE KEY='") + STRING_T(sKey.operator LPCTSTR()) + _T("' AND REV_NO='") + STRING_T(sRevNo.operator LPCTSTR()) + _T("'");
			soci::rowset<soci::row> rs(database.session()->prepare << sSql);
			int iCount = 0;
			for(soci::rowset<soci::row>::const_iterator itr = rs.begin();itr != rs.end();++itr,++iCount)
			{
				STRING_T sValue;
				
				OSTRINGSTREAM_T oss;
				oss << (iCount + 1);
				sValue = oss.str();

				const int iItem = m_wndBMDataListCtrl.InsertItem(m_wndBMDataListCtrl.GetItemCount() , sValue.c_str());
				{
					try
					{
						sValue = itr->get<STRING_T>(_T("draw_no"));
						m_wndBMDataListCtrl.SetItemText(iItem , 1 , sValue.c_str());
					}catch(...){}

					try
					{
						sValue = itr->get<STRING_T>(_T("rev_no"));
						m_wndBMDataListCtrl.SetItemText(iItem , 2 , sValue.c_str());
					}catch(...){}
					
					try
					{
						sValue = itr->get<STRING_T>(_T("symbol"));
						m_wndBMDataListCtrl.SetItemText(iItem , 3 , sValue.c_str());
					}catch(...){}

					try
					{
						sValue = itr->get<STRING_T>(_T("matl_spec"));
						m_wndBMDataListCtrl.SetItemText(iItem , 4 , sValue.c_str());
					}catch(...){}

					try
					{
						sValue = itr->get<STRING_T>(_T("matl_code"));
						m_wndBMDataListCtrl.SetItemText(iItem , 5 , sValue.c_str());
					}catch(...){}

					try
					{
						sValue = itr->get<STRING_T>(_T("main_size"));
						m_wndBMDataListCtrl.SetItemText(iItem , 6 , sValue.c_str());
					}catch(...){}

					try
					{
						sValue = itr->get<STRING_T>(_T("sub_size"));
						m_wndBMDataListCtrl.SetItemText(iItem , 7 , sValue.c_str());
					}catch(...){}

					try
					{
						sValue = itr->get<STRING_T>(_T("bolt_length"));
						m_wndBMDataListCtrl.SetItemText(iItem , 8 , sValue.c_str());
					}catch(...){}

					try
					{
						sValue = itr->get<STRING_T>(_T("qty"));
						m_wndBMDataListCtrl.SetItemText(iItem , 9 , sValue.c_str());
					}catch(...){}

					try
					{
						sValue = itr->get<STRING_T>(_T("matl_desc"));
						m_wndBMDataListCtrl.SetItemText(iItem , 10 , sValue.c_str());
					}catch(...){}
				}
			}
		}
		catch(const std::exception& ex)
		{
			AfxMessageBox(ex.what(),MB_OK|MB_ICONERROR);
		}

		return ERROR_SUCCESS;
	}

	return ERROR_BAD_ENVIRONMENT;
}