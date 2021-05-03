// BMReportOptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UserApp.h"
#include "BMReportOptionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBMReportOptionDlg dialog


CBMReportOptionDlg::CBMReportOptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBMReportOptionDlg::IDD, pParent) , m_pBoldFont(NULL)
{
	//{{AFX_DATA_INIT(CBMReportOptionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBMReportOptionDlg::~CBMReportOptionDlg()
{
	try
	{
		delete m_pBoldFont;
		m_pBoldFont = NULL;
	}
	catch(...)
	{

	}
}

void CBMReportOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBMReportOptionDlg)
	DDX_Control(pDX, IDC_STATIC_TITLE, m_TitleStaticCtrl);
	//DDX_Control(pDX, IDC_BUTTON_START, m_StartButton);
	//}}AFX_DATA_MAP
}



BEGIN_MESSAGE_MAP(CBMReportOptionDlg, CDialog)
	//{{AFX_MSG_MAP(CBMReportOptionDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBMReportOptionDlg message handlers
/**	
	@brief	다이얼로그 초기화 부분.
**/
BOOL CBMReportOptionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_pBoldFont = new CFont;
	m_pBoldFont->CreateFont(20,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0, _T("Arial"));
	m_TitleStaticCtrl.SetFont(m_pBoldFont);

	CButton* pButton = (CButton*)GetDlgItem( IDC_CHECK_GENERATE_INF_FILE );
	if( pButton ) pButton->SetCheck( TRUE );

	pButton = (CButton*)GetDlgItem(IDC_CHECK_MAKE_MERGE_FILE);
	if(pButton) pButton->SetCheck(TRUE);
		
	pButton = (CButton*)GetDlgItem(IDC_CHECK_WELDING_DIA_CHECK);
	if(pButton) pButton->SetCheck(TRUE);
	
	pButton = (CButton*)GetDlgItem(IDC_CHECK_MAKE_NTR_FILE);
	if(pButton) pButton->SetCheck(TRUE);

	pButton = (CButton*)GetDlgItem(IDC_CHECK_MAKE_PRT_FILE);
	if(pButton) pButton->SetCheck(TRUE);

	pButton = (CButton*)GetDlgItem(IDC_CHECK_LOG_FILE_SYNCHORNIZING);
	if(pButton) pButton->SetCheck(TRUE);

	pButton = (CButton*)GetDlgItem(IDC_CHECK_SAVE_LOG_FILE);
	if(pButton) pButton->SetCheck(TRUE);

	pButton = (CButton*)GetDlgItem(IDC_CHECK_WELDING_DETAIL_CHECK);
	if(pButton) pButton->SetCheck(TRUE);

	pButton = (CButton*)GetDlgItem(IDC_CHECK_MAKE_PAINT_CODE);
	if(pButton) pButton->SetCheck(TRUE);

	pButton = (CButton*)GetDlgItem(IDC_CHECK_MAKE_INS_BM);
	if(pButton) pButton->SetCheck(TRUE);

	pButton = (CButton*)GetDlgItem(IDC_CHECK_PIPE_SUPPORT);
	if(pButton) pButton->SetCheck(TRUE);

	pButton = (CButton*)GetDlgItem( IDC_CHECK_SAVE_ERR_SUM );
	if( pButton ) pButton->SetCheck( TRUE );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

bool CBMReportOptionDlg::GetMakeNtrFileCheck() const
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_MAKE_NTR_FILE);
	if(pButton)
	{
		return (TRUE == pButton->GetCheck()) ? true : false;
	}

	return false;
}

bool CBMReportOptionDlg::GetMakePrtFileCheck() const
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_MAKE_PRT_FILE);
	if(pButton)
	{
		return (TRUE == pButton->GetCheck()) ? true : false;
	}

	return false;
}

bool CBMReportOptionDlg::GetLogFileSynchronizingCheck() const
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_LOG_FILE_SYNCHORNIZING);
	if(pButton)
	{
		return (TRUE == pButton->GetCheck()) ? true : false;
	}

	return false;
}

bool CBMReportOptionDlg::GetSaveLogFileCheck() const
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_SAVE_LOG_FILE);
	if(pButton)
	{
		return (TRUE == pButton->GetCheck()) ? true : false;
	}

	return false;
}

bool CBMReportOptionDlg::GetMakePaintCodeCheck() const
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_MAKE_PAINT_CODE);
	if(pButton)
	{
		return (TRUE == pButton->GetCheck()) ? true : false;
	}

	return false;
}

bool CBMReportOptionDlg::GetMakeInsBMCheck() const
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_MAKE_INS_BM);
	if(pButton)
	{
		return (TRUE == pButton->GetCheck()) ? true : false;
	}

	return false;
}

bool CBMReportOptionDlg::GetMakeWeldDiaCheck() const
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_WELDING_DIA_CHECK);
	if(pButton)	
	{
		return (TRUE == pButton->GetCheck()) ? true : false;
	}
	return false;
}

/**	\brief	The CBMReportOptionDlg::GetGenerateINFFileCheck function


	\return	bool	
*/
bool CBMReportOptionDlg::GetGenerateINFFileCheck() const
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_GENERATE_INF_FILE);
	if(pButton)	
	{
		return (TRUE == pButton->GetCheck()) ? true : false;
	}
	return false;
}

/**	\brief	The CBMReportOptionDlg::GetGenerateINFSumFileCheck function


	\return	bool	
*/
bool CBMReportOptionDlg::GetGenerateINFSumFileCheck() const
{
	CButton* pButton = (CButton*)GetDlgItem( IDC_CHECK_GENERATE_INF_SUM_FILE );
	if(pButton)	
	{
		return (TRUE == pButton->GetCheck()) ? true : false;
	}
	return false;
}

/**
**/
bool CBMReportOptionDlg::GetPipeSupportCheck() const
{
	CButton* pButton = (CButton*)GetDlgItem( IDC_CHECK_PIPE_SUPPORT );
	if(pButton)	
	{
		return (TRUE == pButton->GetCheck()) ? true : false;
	}
	return false;
}

/**
**/
bool CBMReportOptionDlg::GetMakeWeldDetailCheck() const
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_WELDING_DETAIL_CHECK);
	if(pButton)	
	{
		return (TRUE == pButton->GetCheck()) ? true : false;
	}
	return false;
}

/**
	\brief	
	\author	백흠경
**/
BOOL CBMReportOptionDlg::GetSaveErrorSumFile() const
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_SAVE_ERR_SUM);
	if(pButton) return pButton->GetCheck();
	
	return FALSE;
}
