// StringHelper.cpp: implementation of the CStringHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Admin.h"
#include "StringHelper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStringHelper::CStringHelper()
{

}

CStringHelper::~CStringHelper()
{

}

CStringHelper& CStringHelper::GetInstance()
{
	static CStringHelper __instance;

	return __instance;
}

/**	\brief	The CStringHelper::GetHelpString function

	\param	index	a parameter of type const UINT &

	\return	const char*	
*/
const char* CStringHelper::GetHelpString(const UINT &nID)
{
	map<UINT , CString>::iterator where = m_stringMap.find( nID );
	if( m_stringMap.end() != where) return where->second.operator LPCTSTR();

	CString rString;
	if( rString.LoadString( nID ) )
	{
		m_stringMap[ nID ] = rString;
	}

	return rString;
}
