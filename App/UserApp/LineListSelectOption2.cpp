// LineListSelectOption2.cpp : implementation file
//

#include "stdafx.h"
#include "UserApp.h"
#include "LineListSelectOption2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLineListSelectOption2 dialog


CLineListSelectOption2::CLineListSelectOption2(CWnd* pParent /*=NULL*/)
	: CDialog(CLineListSelectOption2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLineListSelectOption2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CLineListSelectOption2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLineListSelectOption2)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLineListSelectOption2, CDialog)
	//{{AFX_MSG_MAP(CLineListSelectOption2)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLineListSelectOption2 message handlers
