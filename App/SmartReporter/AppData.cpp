// CAppData.cpp: implementation of the CAppData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SmartReporter.h"
#include "AppData.h"
#include <fstream>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static const char MASK[]=
{
	'B' , 'Z' , 'T' , 'O' , 'O' , 'L' , 'K' , 'I' , 'T'
};
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAppData::CAppData() : m_rAdminPassword( _T("gopds") )
{
}

CAppData::~CAppData()
{

}

/**
**/
bool CAppData::LoadData(const STRING_T &rDataFilePath)
{
	IFSTREAM_T ifile( rDataFilePath.c_str() );
	if( ifile.is_open() )
	{
		m_rAdminPassword.assign( _T("") );

		const int nMaskCount = sizeof(MASK)/sizeof(MASK[0]);

		long nLen = 0L;
		ifile >> nLen;
		for( int i = 0;i < nLen;++i)
		{
			CHAR_T ch = '\0';
			ifile >> ch;
			ch = (ch ^ MASK[i % nMaskCount]);
			m_rAdminPassword += ch;
		}
		return true;
	}

	return false;
}

/**
**/
bool CAppData::SaveData(const STRING_T &rDataFilePath)
{
	const long nLen = m_rAdminPassword.length();

	ofstream ofile( rDataFilePath.c_str() );
	if( ofile.is_open() )
	{
		const int nMaskCount = sizeof(MASK)/sizeof(MASK[0]);
		ofile << nLen;
		for( int i = 0;i < nLen;++i)
		{
			char ch = (m_rAdminPassword[i] ^ MASK[i % nMaskCount]);
			ofile <<  (ch);
		}

		return true;
	}

	return false;
}

/**
**/
STRING_T CAppData::GetAdminPassword() const
{
	return m_rAdminPassword;
}

/**
**/
void CAppData::SetAdminPassword(const STRING_T &rAdminPassword)
{
	m_rAdminPassword = rAdminPassword;
}
