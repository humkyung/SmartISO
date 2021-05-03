// AppData.h: interface for the AppData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_APPDATA_H__3BFC0B8D_16A0_42C7_9CAE_8114665BB877__INCLUDED_)
#define AFX_APPDATA_H__3BFC0B8D_16A0_42C7_9CAE_8114665BB877__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"
#include <string>
using namespace std;

class CAppData  
{
public:
	void SetAdminPassword( const STRING_T& rAdminPassword );
	STRING_T GetAdminPassword() const;
	bool SaveData( const STRING_T& rDataFilePath );
	bool LoadData( const STRING_T& rDataFilePath );
	CAppData( );
	virtual ~CAppData();
private:
	STRING_T m_rAdminPassword;	///< 관리자 암호.
};

#endif // !defined(AFX_APPDATA_H__3BFC0B8D_16A0_42C7_9CAE_8114665BB877__INCLUDED_)
