#include "StdAfx.h"
#include <assert.h>
#include "Profile.h"
#include <fstream>
#include <algorithm>

map< string , vector< CProfileItem > > CProfileItem::m_profileItemMap;
void CProfileItem::Clear()
{
	m_profileItemMap.clear();
}

DWORD CProfileItem::GetUsedTime() const
{
	return (m_nEndTime - m_nStartTime);
}

void CProfileItem::Add()
{
	CProfileItem::m_profileItemMap[m_name].push_back( *this );
}

typedef struct
{
	string name;
	long   size;
	long   total_time;
}PROFILE_OUTPUT;
bool AscendSort( const PROFILE_OUTPUT& lhs , const PROFILE_OUTPUT& rhs )
{
	return (lhs.total_time > rhs.total_time);
}

/**	\brief	프로파일 내용을 파일로 저장한다.

	\param	pszFilePath	a parameter of type const char*

	\return	void	
*/
void CProfileItem::Save( const char* pszFilePath )
{
	assert( pszFilePath && "pszFilePath is NULL");

	if( pszFilePath )
	{
		PROFILE_OUTPUT profileOutput;
		vector< PROFILE_OUTPUT > ProfileOutputEntry;

		ofstream ofile( pszFilePath );
		for( map< string , vector< CProfileItem > >::iterator itr = m_profileItemMap.begin();itr != m_profileItemMap.end();++itr)
		{
			DWORD nTotalTime = 0;
			for( vector< CProfileItem >::iterator jtr = itr->second.begin();jtr != itr->second.end();++jtr)
			{
				nTotalTime += jtr->GetUsedTime();
			}
			
			profileOutput.name = itr->first;
			profileOutput.size = itr->second.size();
			profileOutput.total_time = nTotalTime;
			ProfileOutputEntry.push_back( profileOutput );
		}

		stable_sort( ProfileOutputEntry.begin() , ProfileOutputEntry.end() , AscendSort );
		for( vector< PROFILE_OUTPUT >::iterator jtr = ProfileOutputEntry.begin();jtr != ProfileOutputEntry.end();++jtr)
		{
			ofile << "[" << jtr->name << "]" << std::endl;
			ofile << "size=" << jtr->size << std::endl;
			ofile << "time=" << jtr->total_time << std::endl;
			ofile << "[END]" << std::endl << std::endl;
		}
	}
}