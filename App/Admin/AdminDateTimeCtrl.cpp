// AdminDateTimeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "admin.h"
#include "AdminDateTimeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace GridCell;
/////////////////////////////////////////////////////////////////////////////
// CAdminDateTimeCtrl

CAdminDateTimeCtrl::CAdminDateTimeCtrl( const int& row , const int& column , CLineDataSettingGridCell* pParent ) : m_pParent( pParent )
{
	m_nRow = row;
	m_nColumn = column;
}

CAdminDateTimeCtrl::~CAdminDateTimeCtrl()
{
}


BEGIN_MESSAGE_MAP(CAdminDateTimeCtrl, CDateTimeCtrl)
	//{{AFX_MSG_MAP(CAdminDateTimeCtrl)
	ON_NOTIFY_REFLECT(DTN_DATETIMECHANGE, OnDatetimechange)
	ON_NOTIFY_REFLECT(DTN_USERSTRING, OnUserstring)
	ON_NOTIFY_REFLECT(DTN_CLOSEUP, OnCloseup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdminDateTimeCtrl message handlers

void CAdminDateTimeCtrl::OnDatetimechange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	/*
	if( NULL != m_pParent )
	{
		m_pParent->EndEdit();
	}
	*/

	*pResult = 0;
}

void CAdminDateTimeCtrl::OnUserstring(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	*pResult = 0;
}

void CAdminDateTimeCtrl::OnCloseup(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if( NULL != m_pParent )
	{
		m_pParent->EndEdit();
	}

	*pResult = 0;
}
