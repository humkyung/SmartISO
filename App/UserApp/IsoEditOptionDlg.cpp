// IsoEditOptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UserApp.h"
#include <DgnDocument.h>
#include <DgnText.h>
#include "IsoEditOptionDlg.h"
#include <Tokenizer.h>
#include "IsoEditForm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString CIsoEditOptionDlg::APP("Iso Edit Setting");
/////////////////////////////////////////////////////////////////////////////
// CIsoEditOptionDlg dialog


CIsoEditOptionDlg::CIsoEditOptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIsoEditOptionDlg::IDD, pParent) , m_pBoldFont(NULL) , m_pTargetWnd(NULL)
{
	//{{AFX_DATA_INIT(CIsoEditOptionDlg)
	//}}AFX_DATA_INIT
}

CIsoEditOptionDlg::~CIsoEditOptionDlg()
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

void CIsoEditOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIsoEditOptionDlg)
	DDX_Check(pDX , IDC_RADIO_SOURCE , m_bSelectSourceIsoDwgFileFolder);
	DDX_Check(pDX , IDC_RADIO_EDITED , m_bSelectEditedIsoDwgFileFolder);
	DDX_Check(pDX , IDC_CHECK_LOG_FILE_SYNCHRONIZING, m_bLOG_FILE_SYNCHRONIZING);
	DDX_Check(pDX , IDC_CHECK_SAVE_LOG_FILE , m_bSAVE_LOG_FILE);
	DDX_Check(pDX , IDC_CHECK_REMOVE_TEMP_FILE , m_bREMOVE_TEMP_FILE);
	DDX_Check(pDX , IDC_CHECK_SHOW_EXISTING_FILE , m_bSHOW_EXISTING_FILE);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_TitleStaticCtrl);
	DDX_Check(pDX , IDC_CHECK_QUALITY_CONTROL_CHECK , m_bQUALITY_CONTROL_CHECK);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIsoEditOptionDlg, CDialog)
	//{{AFX_MSG_MAP(CIsoEditOptionDlg)
	ON_BN_CLICKED(IDC_CHECK_PLOTTING_AFTER_EDITING, OnCheckPlottingAfterEditing)
	ON_BN_CLICKED(IDC_RADIO_SOURCE, OnRadioSource)
	ON_BN_CLICKED(IDC_RADIO_EDITED, OnRadioEdited)
	ON_BN_CLICKED( IDC_CHECK_QUALITY_CONTROL_CHECK , OnCheckQualityControlCheck )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIsoEditOptionDlg message handlers

BOOL CIsoEditOptionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_pBoldFont = new CFont;
	m_pBoldFont->CreateFont(20,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0, _T("Arial"));
	m_TitleStaticCtrl.SetFont(m_pBoldFont);

	m_bSelectSourceIsoDwgFileFolder = TRUE;
	m_bSelectEditedIsoDwgFileFolder = FALSE;

	/// 초기 옵션 설정.
	m_bLOG_FILE_SYNCHRONIZING = TRUE;
	m_bSAVE_LOG_FILE          = TRUE;

	m_bQUALITY_CONTROL_CHECK  = TRUE;
	///m_bSPEC_BREAK_HOLD_CHECK  = TRUE;
	///m_bPIPE_SUPPORT_CHECK     = TRUE;

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CIsoEditOptionDlg::OnCheckPlottingAfterEditing() 
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_PLOTTING_AFTER_EDITING);
	if(pButton)
	{
		if(pButton->GetCheck())
		{
			AfxMessageBox("프린터 설정을 확인");
		}
	}
}

/**
	@brief	Quality control button을 check했을 때 target window으로 notify message를 보낸다.
	
	@author	humkyung
*/
void CIsoEditOptionDlg::OnCheckQualityControlCheck() 
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_QUALITY_CONTROL_CHECK);
	if(pButton)
	{
		if(NULL != m_pTargetWnd)
		{
			NMHDR hdr;
			hdr.hwndFrom = GetSafeHwnd();
			hdr.idFrom = GetDlgCtrlID();
			m_pTargetWnd->SendMessage(WM_NOTIFY , (WPARAM)QUALITY_CONTROL_CHECK , (LPARAM)&hdr);
		}
	}
}

/**	
	@brief	SOURCE 라디오 버튼을 클릭했을 때...

	@author	humkyung
**/
void CIsoEditOptionDlg::OnRadioSource() 
{
	m_bSelectSourceIsoDwgFileFolder = TRUE;
	m_bSelectEditedIsoDwgFileFolder = FALSE;
	SaveData( m_rIniFilePath );

	if(NULL != m_pTargetWnd)
	{
		NMHDR hdr;
		hdr.hwndFrom = GetSafeHwnd();
		hdr.idFrom = GetDlgCtrlID();
		m_pTargetWnd->SendMessage(WM_NOTIFY , (WPARAM)SELECT_SOURCE_ISO_DWG_FOLDER , (LPARAM)&hdr);
	}
}

/**	
	@brief	EDITED 라디오 버튼을 클릭했을 때...

	@author	humkyung
**/
void CIsoEditOptionDlg::OnRadioEdited() 
{
	m_bSelectSourceIsoDwgFileFolder = FALSE;
	m_bSelectEditedIsoDwgFileFolder = TRUE;
	SaveData( m_rIniFilePath );

	if(NULL != m_pTargetWnd)
	{
		NMHDR hdr;
		hdr.hwndFrom = GetSafeHwnd();
		hdr.idFrom = GetDlgCtrlID();
		m_pTargetWnd->SendMessage(WM_NOTIFY , (WPARAM)SELECT_EDITED_ISO_DWG_FOLDER , (LPARAM)&hdr);
	}
}

/**	\brief	데이터를 환경 파일로 저장한다.

	\param	rIniFilePath	a parameter of type const CString &

	\return	void	
*/
void CIsoEditOptionDlg::SaveData(const CString &rIniFilePath)
{	
	CString rValue = ( TRUE == m_bSelectSourceIsoDwgFileFolder ) ? "Source" : "Edited";
	WritePrivateProfileString( CIsoEditOptionDlg::APP , "Select From" , rValue , rIniFilePath );
}

/**	\brief	환경 설정 파일에서 데이터를 읽는다.

	\param	rIniFilePath	a parameter of type const CString &

	\return	void	
*/
void CIsoEditOptionDlg::LoadData(const CString &rIniFilePath)
{
	TCHAR szBuf[MAX_PATH + 1] = {'\0' ,};
	if( GetPrivateProfileString( CIsoEditOptionDlg::APP , "Select From" , NULL , szBuf , MAX_PATH , rIniFilePath ))
	{
		if( CString(szBuf) == "Source" )
		{
			m_bSelectSourceIsoDwgFileFolder = TRUE;
			m_bSelectEditedIsoDwgFileFolder = FALSE;
		}
		else
		{
			m_bSelectSourceIsoDwgFileFolder = FALSE;
			m_bSelectEditedIsoDwgFileFolder = TRUE;
		}
	}
	UpdateData( FALSE );
}

/**	
	@brief	The CIsoEditOptionDlg::GetOptionCheck function

	@author	백흠경

	@param	option	a parameter of type const IsoEditOptionMsg &

	@return	BOOL	
*/
BOOL CIsoEditOptionDlg::GetOptionCheck(const IsoEditOptionMsg &option)
{
	int res = FALSE;

	switch( option )
	{
		case LOG_FILE_SYNCHRONIZING:
		{
			///res = m_bLOG_FILE_SYNCHRONIZING;
			CButton* pButton = (CButton*)(GetDlgItem( IDC_CHECK_LOG_FILE_SYNCHRONIZING ));
			if(pButton) res = pButton->GetCheck();
		}
			break;
		case SAVE_LOG_FILE:
		{
			///res = m_bSAVE_LOG_FILE;
			CButton* pButton = (CButton*)(GetDlgItem( IDC_CHECK_SAVE_LOG_FILE ));
			if(pButton) res = pButton->GetCheck();
		}
			break;
		case REMOVE_TEMP_FILE:
		{
			///res = m_bREMOVE_TEMP_FILE;
			CButton* pButton = (CButton*)(GetDlgItem( IDC_CHECK_REMOVE_TEMP_FILE ));
			if(pButton) res = pButton->GetCheck();
		}
			break;
		case SHOW_EXISTING_FILE:
		{
			///res = m_bSHOW_EXISTING_FILE;
			CButton* pButton = (CButton*)(GetDlgItem( IDC_CHECK_SHOW_EXISTING_FILE ));
			if(pButton) res = pButton->GetCheck();
		}
			break;
		case SPEC_BREAK_HOLD_CHECK:
		{
			///res = m_bSPEC_BREAK_HOLD_CHECK;
			CButton* pButton = (CButton*)(GetDlgItem( IDC_CHECK_SPEC_BREAK_HOLD ));
			if(pButton) res = pButton->GetCheck();
		}
			break;
		case QUALITY_CONTROL_CHECK:
		{
			///res = m_bQUALITY_CONTROL_CHECK;
			CButton* pButton = (CButton*)(GetDlgItem( IDC_CHECK_QUALITY_CONTROL_CHECK ));
			if(pButton) res = pButton->GetCheck();
		}
			break;
		case BOM_OVER_CHECK:
		{
			///res = m_bPIPE_SUPPORT_CHECK;
			CButton* pButton = (CButton*)(GetDlgItem( IDC_CHECK_BOM_OVER ));
			if(pButton) res = pButton->GetCheck();
		}
			break;
	}

	return res;
}
