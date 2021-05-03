// GetDataFromIDFStartUpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "userapp.h"
#include "UserAppDocData.h"
#include "GetDataFromIDFStartUpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGetDataFromIDFStartUpDlg dialog


CGetDataFromIDFStartUpDlg::CGetDataFromIDFStartUpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGetDataFromIDFStartUpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGetDataFromIDFStartUpDlg)
		CAppDocData* pDocData = CAppDocData::GetInstance();
		m_rUNITX = pDocData->GetWorkingUnit().c_str();
		m_rUNITY = pDocData->GetWorkingUnit().c_str();
		m_rUNITZ = pDocData->GetWorkingUnit().c_str();
		m_rDVCSX = pDocData->GetDVCSE().c_str();
		m_rDVCSY = pDocData->GetDVCSN().c_str();
		m_rDVCSZ = pDocData->GetDVCSEL().c_str();
	//}}AFX_DATA_INIT
}


void CGetDataFromIDFStartUpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGetDataFromIDFStartUpDlg)
		DDX_Text(pDX , IDC_EDIT_UNIT_X , m_rUNITX);
		DDX_Text(pDX , IDC_EDIT_UNIT_Y , m_rUNITY);
		DDX_Text(pDX , IDC_EDIT_UNIT_Z , m_rUNITZ);
		DDX_Text(pDX , IDC_EDIT_DVCS_X , m_rDVCSX);
		DDX_Text(pDX , IDC_EDIT_DVCS_Y , m_rDVCSY);
		DDX_Text(pDX , IDC_EDIT_DVCS_Z , m_rDVCSZ);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGetDataFromIDFStartUpDlg, CDialog)
	//{{AFX_MSG_MAP(CGetDataFromIDFStartUpDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGetDataFromIDFStartUpDlg message handlers
