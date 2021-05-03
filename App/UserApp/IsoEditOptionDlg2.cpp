// IsoEditOptionDlg2.cpp : implementation file
//

#include "stdafx.h"
#include "UserApp.h"
#include "IsoEditOptionDlg2.h"
#include "tokenizer.h"
#include "QCCheck.h"
#include "UserAppDocData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const int _ITEM_IDS_[]=
{	
	IDC_CHECK_DRWNO , IDC_CHECK_ERRREAD , IDC_CHECK_FGBERR , IDC_CHECK_PIPECUTERR , IDC_CHECK_NOTFOUNERR ,
	IDC_CHECK_FAILISOERR , IDC_CHECK_PCDSYMERR , IDC_CHECK_HOLDITEMERR , IDC_CHECK_SPECBREAKERR , IDC_CHECK_SYMCODEERR , 
	IDC_CHECK_SUPPORTBMERR , IDC_CHECK_TEXTMODIFY/* , IDC_CHECK_SPEC_BREAK_HOLD , IDC_CHECK_PIPE_SUPPORT_CHECK*/
};

CString CIsoEditOptionDlg2::m_sAppName = _T("Iso Edit Setting");
/////////////////////////////////////////////////////////////////////////////
// CIsoEditOptionDlg2 dialog
    
CIsoEditOptionDlg2::CIsoEditOptionDlg2(CWnd* pParent /*=NULL*/)
	: CDialog(CIsoEditOptionDlg2::IDD, pParent) , m_pBoldFont(NULL) , m_pTargetWnd(NULL)
{
	//{{AFX_DATA_INIT(CIsoEditOptionDlg2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CIsoEditOptionDlg2::~CIsoEditOptionDlg2()
{
	try
	{
		if(NULL != m_pBoldFont) delete m_pBoldFont;
		m_pBoldFont = NULL;
	}
	catch(...)
	{

	}
}
void CIsoEditOptionDlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIsoEditOptionDlg2)
	DDX_Control(pDX, IDC_STATIC_TITLE2, m_TitleStaticCtrl);
	DDX_Control(pDX , IDC_CHECK_DRWNO , m_chkDrwNo);
	DDX_Control(pDX , IDC_CHECK_ERRREAD , m_chkErrorRead);
	DDX_Control(pDX , IDC_CHECK_PIPECUTERR , m_chkPipeCutErr);
	DDX_Control(pDX , IDC_CHECK_NOTFOUNERR , m_chkNotFoundErr);
	DDX_Control(pDX , IDC_CHECK_FAILISOERR , m_chkFailIsoErr);
	DDX_Control(pDX , IDC_CHECK_PCDSYMERR , m_chkPcdSymErr);
	DDX_Control(pDX , IDC_CHECK_PIPECUTERR ,  m_chkPipeCutErr);
	DDX_Control(pDX , IDC_CHECK_HOLDITEMERR , m_chkHoldItemErr);
	DDX_Control(pDX , IDC_CHECK_SYMCODEERR , m_chkSymCodeErr);
	DDX_Control(pDX , IDC_CHECK_SUPPORTBMERR , m_chkSupportBMErr);
	DDX_Control(pDX , IDC_CHECK_TEXTMODIFY , m_chkTextModifiedErr);
	DDX_Control(pDX , IDC_CHECK_FGBERR , m_chkFGBErr);
	DDX_Control(pDX , IDC_CHECK_SPECBREAKERR , m_chkSpecBreakErr);
	DDX_Control(pDX , IDC_CHECK_BOM_OVER , m_chkBOMOverCheck);
	DDX_Control(pDX , IDC_CHECK_SPEC_BREAK_HOLD , m_chkSpecBreakHoldCheck);
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIsoEditOptionDlg2, CDialog)
	//{{AFX_MSG_MAP(CIsoEditOptionDlg2)
	ON_BN_CLICKED(IDC_CHECK_DRWNO, OnCheckDrwNo)
	ON_BN_CLICKED(IDC_CHECK_ERRREAD, OnCheckErrRead)
	ON_BN_CLICKED(IDC_CHECK_FGBERR, OnCheckFGBErr)
	ON_BN_CLICKED(IDC_CHECK_PIPECUTERR, OnCheckPipeCutErr)
	ON_BN_CLICKED(IDC_CHECK_NOTFOUNERR, OnCheckNotFoundErr)
	ON_BN_CLICKED(IDC_CHECK_FAILISOERR, OnCheckFailIsoErr)
	ON_BN_CLICKED(IDC_CHECK_PCDSYMERR, OnCheckPcdSymBolErr)
	ON_BN_CLICKED(IDC_CHECK_HOLDITEMERR, OnCheckHoldItemErr)
	ON_BN_CLICKED(IDC_CHECK_SPECBREAKERR, OnCheckSpecBreakErr)
	ON_BN_CLICKED(IDC_CHECK_SYMCODEERR, OnCheckSymBolCodeErr)
	ON_BN_CLICKED(IDC_CHECK_SUPPORTBMERR, OnCheckSupportBMErr)
	ON_BN_CLICKED(IDC_CHECK_TEXTMODIFY,OnCheckTextModify)
	ON_BN_CLICKED(IDC_CHECK_BOM_OVER,OnCheckBOMOver)
	//ON_BN_CLICKED(IDC_CHECK_QUALITY_CONTROL_CHECK, OnCheckQualityControlCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CIsoEditOptionDlg2::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_pBoldFont = new CFont;
	m_pBoldFont->CreateFont(20,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0, _T("Arial") );
	m_TitleStaticCtrl.SetFont(m_pBoldFont);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/**
	@brief	initialize control's state

	@author	HumKyung

	@date	2011.01.19

	@param

	@return
*/
int CIsoEditOptionDlg2::InitContents()
{
	TCHAR szBuf[MAX_PATH + 1] = {'\0',};
	CAppDocData* pDocData = CAppDocData::GetInstance();
	static const CString sApp(_T("Iso Edit Setting"));

	/*CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_QUALITY_CONTROL_CHECK);
	if(pButton)
	{
		if(GetPrivateProfileString(sApp , _T("CHECK_QUALITY_CONTROL_CHECK") , _T("") , szBuf , MAX_PATH , pDocData->m_rIniFilePath))
		{
			pButton->SetCheck((0 == _stricmp(szBuf , _T("ON"))));
		}else	pButton->SetCheck(FALSE);
	}*/

	if(m_chkDrwNo.GetSafeHwnd())
	{
		if(GetPrivateProfileString(sApp , _T("CHECK_DRWNO") , _T("") , szBuf , MAX_PATH , pDocData->GetPrjIniFilePath()))
		{
			const BOOL bCheck = (0 == _stricmp(szBuf , _T("ON")));
			m_chkDrwNo.SetCheck(bCheck);
			m_DRWNO = bCheck;
			CQCCheck::m_DRWNO = bCheck;
		}else	CQCCheck::m_DRWNO = FALSE;
	}

	if(m_chkErrorRead.GetSafeHwnd())
	{
		if(GetPrivateProfileString(sApp , _T("CHECK_ERRREAD") , _T("") , szBuf , MAX_PATH , pDocData->GetPrjIniFilePath()))
		{
			const BOOL bCheck = (0 == STRICMP_T(szBuf , _T("ON")));
			m_chkErrorRead.SetCheck(bCheck);
			m_ERRREAD = bCheck;
			CQCCheck::m_ERRREAD = bCheck;
		}else	CQCCheck::m_ERRREAD = FALSE;
	}

	if(m_chkFGBErr.GetSafeHwnd())
	{
		if(GetPrivateProfileString(sApp , _T("CHECK_FGBERR") , _T("") , szBuf , MAX_PATH , pDocData->GetPrjIniFilePath()))
		{
			const BOOL bCheck = (0 == stricmp(szBuf , _T("ON")));
			m_chkFGBErr.SetCheck(bCheck);
			m_FGBERR = bCheck;
			CQCCheck::m_FGBERR = bCheck;
		}else	CQCCheck::m_FGBERR = FALSE;
	}

	if(m_chkPipeCutErr.GetSafeHwnd())
	{
		if(GetPrivateProfileString(sApp , _T("CHECK_PIPECUTERR") , _T("") , szBuf , MAX_PATH , pDocData->GetPrjIniFilePath()))
		{
			const BOOL bCheck = (0 == stricmp(szBuf , _T("ON")));
			m_chkPipeCutErr.SetCheck(bCheck);
			m_PIPECUTERR = bCheck;
			CQCCheck::m_PIPECUTERR = bCheck;
		}else	CQCCheck::m_PIPECUTERR = FALSE;
	}

	if(m_chkNotFoundErr.GetSafeHwnd())
	{
		if(GetPrivateProfileString(sApp , _T("CHECK_NOTFOUNERR") , _T("") , szBuf , MAX_PATH , pDocData->GetPrjIniFilePath()))
		{
			const BOOL bCheck = (0 == stricmp(szBuf , _T("ON")));
			m_chkNotFoundErr.SetCheck(bCheck);
			m_NOTFOUNERR = bCheck;
			CQCCheck::m_NOTFOUNERR = bCheck;
		}else	CQCCheck::m_NOTFOUNERR = FALSE;
	}

	if(m_chkFailIsoErr.GetSafeHwnd())
	{
		if(GetPrivateProfileString(sApp , _T("CHECK_FAILISOERR") , _T("") , szBuf , MAX_PATH , pDocData->GetPrjIniFilePath()))
		{
			const BOOL bCheck = (0 == stricmp(szBuf , _T("ON")));
			m_chkFailIsoErr.SetCheck(bCheck);
			m_FAILISOERR = bCheck;
			CQCCheck::m_FAILISOERR = bCheck;
		}else	CQCCheck::m_FAILISOERR = FALSE;
	}

	if(m_chkPcdSymErr.GetSafeHwnd())
	{
		if(GetPrivateProfileString(sApp , _T("CHECK_PCDSYMERR") , _T("") , szBuf , MAX_PATH , pDocData->GetPrjIniFilePath()))
		{
			const BOOL bCheck = (0 == stricmp(szBuf , _T("ON")));
			m_chkPcdSymErr.SetCheck(bCheck);
			m_PCDSYMERR = bCheck;
			CQCCheck::m_PCDSYMERR = bCheck;
		}else	CQCCheck::m_PCDSYMERR = FALSE;
	}

	if(m_chkHoldItemErr.GetSafeHwnd())
	{
		if(GetPrivateProfileString(sApp , _T("CHECK_HOLDITEMERR") , _T("") , szBuf , MAX_PATH , pDocData->GetPrjIniFilePath()))
		{
			const BOOL bCheck = (0 == stricmp(szBuf , _T("ON")));
			m_chkHoldItemErr.SetCheck(bCheck);
			m_HOLDITEMERR = bCheck;
			CQCCheck::m_HOLDITEMERR = bCheck;
		}else	CQCCheck::m_HOLDITEMERR = FALSE;
	}

	if(m_chkSpecBreakErr.GetSafeHwnd())
	{
		if(GetPrivateProfileString(sApp , _T("CHECK_SPECBREAKERR") , _T("") , szBuf , MAX_PATH , pDocData->GetPrjIniFilePath()))
		{
			const BOOL bCheck = (0 == stricmp(szBuf , _T("ON")));
			m_chkSpecBreakErr.SetCheck(bCheck);
			m_SPECBREAKERR = bCheck;
			CQCCheck::m_SPECBREAKERR = bCheck;
		}else	CQCCheck::m_SPECBREAKERR = FALSE;
	}

	if(m_chkSymCodeErr.GetSafeHwnd())
	{
		if(GetPrivateProfileString(sApp , _T("CHECK_SYMCODEERR") , _T("") , szBuf , MAX_PATH , pDocData->GetPrjIniFilePath()))
		{
			const BOOL bCheck = (0 == stricmp(szBuf , _T("ON")));
			m_chkSymCodeErr.SetCheck(bCheck);
			m_SYMCODEERR = bCheck;
			CQCCheck::m_SYMCODEERR = bCheck;
		}else	CQCCheck::m_SYMCODEERR = FALSE;
	}

	if(m_chkSupportBMErr.GetSafeHwnd())
	{
		if(GetPrivateProfileString(sApp , _T("CHECK_SUPPORTBMERR") , _T("") , szBuf , MAX_PATH , pDocData->GetPrjIniFilePath()))
		{
			const BOOL bCheck = (0 == stricmp(szBuf , _T("ON")));
			m_chkSupportBMErr.SetCheck(bCheck);
			m_SUPPORTBMERR = bCheck;
			CQCCheck::m_bCheckSupportBMErr = bCheck;
		}else	CQCCheck::m_bCheckSupportBMErr = FALSE;
	}

	if(m_chkTextModifiedErr.GetSafeHwnd())
	{
		if(GetPrivateProfileString(sApp , _T("CHECK_TEXTMODIFY") , _T("") , szBuf , MAX_PATH , pDocData->GetPrjIniFilePath()))
		{
			const BOOL bCheck = (0 == stricmp(szBuf , _T("ON")));
			m_chkTextModifiedErr.SetCheck(bCheck);
			m_TextModify = bCheck;
			CQCCheck::m_bCheckTextModify = bCheck;
		}else	CQCCheck::m_bCheckTextModify = FALSE;
	}

	/// 2013.06.12 added by humkyung
	if(m_chkBOMOverCheck.GetSafeHwnd())
	{
		if(GetPrivateProfileString(sApp , _T("CHECK_BOMOVER") , _T("") , szBuf , MAX_PATH , pDocData->GetPrjIniFilePath()))
		{
			m_BOMOVER = (0 == STRICMP_T(szBuf , _T("ON")));
			m_chkBOMOverCheck.SetCheck(m_BOMOVER);
			CQCCheck::m_BOMOVER = m_BOMOVER;
		}else	CQCCheck::m_BOMOVER = FALSE;
	}
	/// up to here

	m_nSelectFrom = 0;
	UpdateData(FALSE);

	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       ????-??-??
    @function   OnCheckDrwNo
    @return     void
    @brief
******************************************************************************/
void CIsoEditOptionDlg2::OnCheckDrwNo() 
{
	if(m_chkDrwNo.GetSafeHwnd())
	{
		if(m_chkDrwNo.GetCheck())
		{	
			m_DRWNO = TRUE;
		}
		else
		{
			m_DRWNO = FALSE;
		}
		CQCCheck::m_DRWNO = m_DRWNO;

		//! 2011.01.19 - added by HumKyung
		CAppDocData* pDocData = CAppDocData::GetInstance();
		if(pDocData)
		{
			CString sValue((TRUE == m_DRWNO) ? _T("ON") : _T("OFF"));
			WritePrivateProfileString(CIsoEditOptionDlg2::m_sAppName , _T("CHECK_DRWNO") , sValue , pDocData->GetPrjIniFilePath());
		}
	}
}

void CIsoEditOptionDlg2::OnCheckErrRead()
{
	if(m_chkErrorRead.GetSafeHwnd())
	{
		if(m_chkErrorRead.GetCheck())
		{	
			m_ERRREAD = TRUE;
		}
		else
		{
			m_ERRREAD = FALSE;
		}
		CQCCheck::m_ERRREAD = m_ERRREAD;

		//! 2011.01.19 - added by HumKyung
		CAppDocData* pDocData = CAppDocData::GetInstance();
		if(pDocData)
		{
			CString sValue((TRUE == m_ERRREAD) ? _T("ON") : _T("OFF"));
			WritePrivateProfileString(CIsoEditOptionDlg2::m_sAppName , _T("CHECK_ERRREAD") , sValue , pDocData->GetPrjIniFilePath());
		}
	}
}

void CIsoEditOptionDlg2::OnCheckFailIsoErr()
{
	if(m_chkFailIsoErr.GetSafeHwnd())
	{
		if(m_chkFailIsoErr.GetCheck())
		{	
			m_FAILISOERR = TRUE;
		}
		else
		{
			m_FAILISOERR = FALSE;
		}
		CQCCheck::m_FAILISOERR = m_FAILISOERR;

		//! 2011.01.19 - added by HumKyung
		CAppDocData* pDocData = CAppDocData::GetInstance();
		if(pDocData)
		{
			CString sValue((TRUE == m_FAILISOERR) ? _T("ON") : _T("OFF"));
			WritePrivateProfileString(CIsoEditOptionDlg2::m_sAppName , _T("CHECK_FAILISOERR") , sValue , pDocData->GetPrjIniFilePath());
		}
	}
}

void CIsoEditOptionDlg2::OnCheckFGBErr()
{
	if(m_chkFGBErr.GetSafeHwnd())
	{
		if(m_chkFGBErr.GetCheck())
		{	
			m_FGBERR = TRUE;
		}
		else
		{
			m_FGBERR = FALSE;
		}
		CQCCheck::m_FGBERR = m_FGBERR;

		//! 2011.01.19 - added by HumKyung
		CAppDocData* pDocData = CAppDocData::GetInstance();
		if(pDocData)
		{
			CString sValue((TRUE == m_FGBERR) ? _T("ON") : _T("OFF"));
			WritePrivateProfileString(CIsoEditOptionDlg2::m_sAppName , _T("CHECK_FGBERR") , sValue , pDocData->GetPrjIniFilePath());
		}
	}
}

void CIsoEditOptionDlg2::OnCheckHoldItemErr()
{
	if(m_chkHoldItemErr.GetSafeHwnd())
	{
		if(m_chkHoldItemErr.GetCheck())
		{	
			m_HOLDITEMERR = TRUE;
		}
		else
		{
			m_HOLDITEMERR = FALSE;
		}
		CQCCheck::m_HOLDITEMERR = m_HOLDITEMERR;

		//! 2011.01.19 - added by HumKyung
		CAppDocData* pDocData = CAppDocData::GetInstance();
		if(pDocData)
		{
			CString sValue((TRUE == m_HOLDITEMERR) ? _T("ON") : _T("OFF"));
			WritePrivateProfileString(CIsoEditOptionDlg2::m_sAppName , _T("CHECK_HOLDITEMERR") , sValue , pDocData->GetPrjIniFilePath());
		}
	}
}

void CIsoEditOptionDlg2::OnCheckNotFoundErr()
{
	if(m_chkNotFoundErr.GetSafeHwnd())
	{
		if(m_chkNotFoundErr.GetCheck())
		{	
			m_NOTFOUNERR = TRUE;
		}
		else
		{
			m_NOTFOUNERR = FALSE;
		}
		CQCCheck::m_NOTFOUNERR = m_NOTFOUNERR;

		//! 2011.01.19 - added by HumKyung
		CAppDocData* pDocData = CAppDocData::GetInstance();
		if(pDocData)
		{
			CString sValue((TRUE == m_NOTFOUNERR) ? _T("ON") : _T("OFF"));
			WritePrivateProfileString(CIsoEditOptionDlg2::m_sAppName , _T("CHECK_NOTFOUNERR") , sValue , pDocData->GetPrjIniFilePath());
		}
	}
}

void CIsoEditOptionDlg2::OnCheckPcdSymBolErr()
{
	if(m_chkPcdSymErr.GetSafeHwnd())
	{
		if(m_chkPcdSymErr.GetCheck())
		{	
			m_PCDSYMERR = TRUE;
		}
		else
		{
			m_PCDSYMERR = FALSE;
		}
		CQCCheck::m_PCDSYMERR = m_PCDSYMERR;

		//! 2011.01.19 - added by HumKyung
		CAppDocData* pDocData = CAppDocData::GetInstance();
		if(pDocData)
		{
			CString sValue((TRUE == m_PCDSYMERR) ? _T("ON") : _T("OFF"));
			WritePrivateProfileString(CIsoEditOptionDlg2::m_sAppName , _T("CHECK_PCDSYMERR") , sValue , pDocData->GetPrjIniFilePath());
		}
	}
}

void CIsoEditOptionDlg2::OnCheckPipeCutErr()
{
	if(m_chkPipeCutErr.GetSafeHwnd())
	{
		if(m_chkPipeCutErr.GetCheck())
		{	
			m_PIPECUTERR = TRUE;
		}
		else
		{
			m_PIPECUTERR = FALSE;
		}
		CQCCheck::m_PIPECUTERR = m_PIPECUTERR;

		//! 2011.01.19 - added by HumKyung
		CAppDocData* pDocData = CAppDocData::GetInstance();
		if(pDocData)
		{
			CString sValue((TRUE == m_PIPECUTERR) ? _T("ON") : _T("OFF"));
			WritePrivateProfileString(CIsoEditOptionDlg2::m_sAppName , _T("CHECK_PIPECUTERR") , sValue , pDocData->GetPrjIniFilePath());
		}
	}
}

/**
**/
void CIsoEditOptionDlg2::OnCheckSupportBMErr()
{
	if(m_chkSupportBMErr.GetSafeHwnd())
	{
		if(m_chkSupportBMErr.GetCheck())
		{	
			m_SUPPORTBMERR = TRUE;
		}
		else
		{
			m_SUPPORTBMERR = FALSE;
		}
		CQCCheck::m_bCheckSupportBMErr = m_SUPPORTBMERR;

		//! 2011.01.19 - added by HumKyung
		CAppDocData* pDocData = CAppDocData::GetInstance();
		if(pDocData)
		{
			CString sValue((TRUE == m_SUPPORTBMERR) ? _T("ON") : _T("OFF"));
			WritePrivateProfileString(CIsoEditOptionDlg2::m_sAppName , _T("CHECK_SUPPORTBMERR") , sValue , pDocData->GetPrjIniFilePath());
		}
	}
}

void CIsoEditOptionDlg2::OnCheckSymBolCodeErr()
{
	if(m_chkSymCodeErr.GetSafeHwnd())
	{
		if(m_chkSymCodeErr.GetCheck())
		{	
			m_SYMCODEERR = TRUE;
		}
		else
		{
			m_SYMCODEERR = FALSE;
		}
		CQCCheck::m_SYMCODEERR = m_SYMCODEERR;

		//! 2011.01.19 - added by HumKyung
		CAppDocData* pDocData = CAppDocData::GetInstance();
		if(pDocData)
		{
			CString sValue((TRUE == m_SYMCODEERR) ? _T("ON") : _T("OFF"));
			WritePrivateProfileString(CIsoEditOptionDlg2::m_sAppName , _T("CHECK_SYMCODEERR") , sValue , pDocData->GetPrjIniFilePath());	
		}
	}
}

void CIsoEditOptionDlg2::OnCheckSpecBreakErr()
{
	if(m_chkSpecBreakErr.GetSafeHwnd())
	{
		if(m_chkSpecBreakErr.GetCheck())
		{	
			m_SPECBREAKERR = TRUE;
		}
		else
		{
			m_SPECBREAKERR = FALSE;
		}
		CQCCheck::m_SPECBREAKERR = m_SPECBREAKERR;

		//! 2011.01.19 - added by HumKyung
		CAppDocData* pDocData = CAppDocData::GetInstance();
		if(pDocData)
		{
			CString sValue((TRUE == m_SPECBREAKERR) ? _T("ON") : _T("OFF"));
			WritePrivateProfileString(CIsoEditOptionDlg2::m_sAppName , _T("CHECK_SPECBREAKERR") , sValue , pDocData->GetPrjIniFilePath());
		}
	}
}

void CIsoEditOptionDlg2::OnCheckTextModify()
{
	if(m_chkTextModifiedErr.GetSafeHwnd())
	{
		if(m_chkTextModifiedErr.GetCheck())
		{	
			m_TextModify = TRUE;
		}
		else
		{
			m_TextModify = FALSE;
		}
		CQCCheck::m_bCheckTextModify = m_TextModify;

		//! 2011.01.19 - added by HumKyung
		CAppDocData* pDocData = CAppDocData::GetInstance();
		if(pDocData)
		{
			CString sValue((TRUE == m_TextModify) ? _T("ON") : _T("OFF"));
			WritePrivateProfileString(CIsoEditOptionDlg2::m_sAppName , _T("CHECK_TEXTMODIFY") , sValue , pDocData->GetPrjIniFilePath());
		}
	}
}

/**
	@brief	check BOMOver

	@author	humkyung

	@date	2013.06.12
*/
void CIsoEditOptionDlg2::OnCheckBOMOver()
{
	if(m_chkBOMOverCheck.GetSafeHwnd())
	{
		m_BOMOVER = (m_chkBOMOverCheck.GetCheck()) ? TRUE : FALSE;
		CQCCheck::m_BOMOVER = m_BOMOVER;

		CAppDocData* pDocData = CAppDocData::GetInstance();
		if(pDocData)
		{
			CString sValue((TRUE == m_BOMOVER) ? _T("ON") : _T("OFF"));
			WritePrivateProfileString(CIsoEditOptionDlg2::m_sAppName , _T("CHECK_BOMOVER") , sValue , pDocData->GetPrjIniFilePath());
		}
	}
}

/**	@brief	Quality control check의 하위 항목 모두를 OFF 시킨다.

	@author	HumKyung

	@date	????.??.??

	@param

	@return
**/
void CIsoEditOptionDlg2::DisableAllOptions()
{
	int nCount = sizeof( _ITEM_IDS_ ) / sizeof( _ITEM_IDS_[0] );
	for( int i = 0;i < nCount;++i)
	{
		CButton* pButton = (CButton*)(GetDlgItem( _ITEM_IDS_[i] ));
		if( pButton )
		{
			pButton->SetCheck(FALSE);
			pButton->EnableWindow( FALSE );
		}
	}

	///
	CQCCheck::m_DRWNO = FALSE;
	CQCCheck::m_ERRREAD = FALSE;
	CQCCheck::m_FGBERR = FALSE;
	CQCCheck::m_PIPECUTERR = FALSE;
	CQCCheck::m_NOTFOUNERR = FALSE;
	CQCCheck::m_FAILISOERR = FALSE;
	CQCCheck::m_PCDSYMERR = FALSE;
	CQCCheck::m_HOLDITEMERR = FALSE;
	CQCCheck::m_SPECBREAKERR = FALSE;
	CQCCheck::m_SYMCODEERR = FALSE;
	CQCCheck::m_bCheckSupportBMErr = FALSE;
	CQCCheck::m_bCheckTextModify = FALSE;
	CQCCheck::m_BOMOVER = FALSE;	/// 2013.06.12 added by humkyung
}

/**	@brief	Quality control check의 하위 항목 모두를 ON 시킨다.

	@author	HumKyung

	@date	????.??.??

	@param

	@return
**/
void CIsoEditOptionDlg2::EnableAllOptions()
{
	int nCount = sizeof( _ITEM_IDS_ ) / sizeof( _ITEM_IDS_[0] );
	for( int i = 0;i < nCount;++i)
	{
		CButton* pButton = (CButton*)(GetDlgItem( _ITEM_IDS_[i] ));
		if( pButton )
		{
			pButton->SetCheck(TRUE);
			pButton->EnableWindow( TRUE );
		}
	}

	///
	CQCCheck::m_DRWNO = TRUE;
	CQCCheck::m_ERRREAD = TRUE;
	CQCCheck::m_FGBERR = TRUE;
	CQCCheck::m_PIPECUTERR = TRUE;
	CQCCheck::m_NOTFOUNERR = TRUE;
	CQCCheck::m_FAILISOERR = TRUE;
	CQCCheck::m_PCDSYMERR = TRUE;
	CQCCheck::m_HOLDITEMERR = TRUE;
	CQCCheck::m_SPECBREAKERR = TRUE;
	CQCCheck::m_SYMCODEERR = TRUE;
	CQCCheck::m_bCheckSupportBMErr = TRUE;
	CQCCheck::m_bCheckTextModify = TRUE;
	CQCCheck::m_BOMOVER = TRUE;	/// 2013.06.12 added by humkyung
}

/**
	@brief	QUALITY_CONTROL_CHECK flag 여부에 따라 check item을 on/off 시킨다.

	@author	humkyung
*/
//void CIsoEditOptionDlg2::OnCheckQualityControlCheck() 
//{
//	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_QUALITY_CONTROL_CHECK);
//	if( pButton )
//	{
//		BOOL res = pButton->GetCheck();
//		( TRUE == res ) ? EnableAllOptions() : DisableAllOptions();
//
//		//! 2011.01.19 - added by HumKyung
//		CAppDocData* pDocData = CAppDocData::GetInstance();
//		if(pDocData)
//		{
//			CString sValue((TRUE == res) ? _T("ON") : _T("OFF"));
//			WritePrivateProfileString(_T("Iso Edit Setting") , _T("CHECK_QUALITY_CONTROL_CHECK") , sValue , pDocData->m_rIniFilePath);
//		}
//	}
//}