#ifndef	__IS_PROFILE_H__
#define	__IS_PROFILE_H__

//typedef unsigned int  UINT;
//typedef unsigned char BYTE;
//typedef unsigned long DWORD;

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <mmsystem.h>
#include <dos.h>

#include <map>
#include <vector>
#include <string>
using namespace std;


class CProfileItem
{
	
public:
	CProfileItem( const CProfileItem& rhs )
	{
		m_nStartTime = rhs.m_nStartTime;
		m_nEndTime = rhs.m_nEndTime;
	}

	CProfileItem( const char* name ) : m_name( name )
	{
		m_nStartTime = m_nEndTime = timeGetTime();
	}
	~CProfileItem()
	{
	}

	CProfileItem& operator=( const CProfileItem& rhs )
	{
		if( this != &rhs )
		{
			m_nStartTime = rhs.m_nStartTime;
			m_nEndTime = rhs.m_nEndTime;
		}

		return (*this);
	}

	void End()
	{
		m_nEndTime = timeGetTime();
		Add();
	}

	DWORD GetUsedTime() const;

	bool operator<( const CProfileItem& rhs ) const
	{
		return (GetUsedTime() < rhs.GetUsedTime());
	}
	bool operator>( const CProfileItem& rhs ) const
	{
		return (GetUsedTime() > rhs.GetUsedTime());
	}

	static void Clear();
	static void Save( const char* pszFilePath );
private:
	void Add();

	string m_name;
	DWORD m_nStartTime;
	DWORD m_nEndTime;

	static map< string , vector< CProfileItem > > m_profileItemMap;
};

/*
#define	BEGIN_PROFILE( unnamed , token )\
	CProfileItem _##unnamed(token);\

#define	END_PROFILE(unnamed)\
	_##unnamed.End();\
*/
#endif
