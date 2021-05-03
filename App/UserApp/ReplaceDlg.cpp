// ReplaceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UserApp.h"
#include "ReplaceDlg.h"


// CReplaceDlg dialog

IMPLEMENT_DYNAMIC(CReplaceDlg, CDialog)

CReplaceDlg::CReplaceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReplaceDlg::IDD, pParent)
{

}

CReplaceDlg::~CReplaceDlg()
{
}

void CReplaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX , IDC_EDIT_FIND , m_sFind);
	DDX_Text(pDX , IDC_EDIT_REPLACE , m_sReplace);
}


BEGIN_MESSAGE_MAP(CReplaceDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CReplaceDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CReplaceDlg message handlers

void CReplaceDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	OnOK();
}
