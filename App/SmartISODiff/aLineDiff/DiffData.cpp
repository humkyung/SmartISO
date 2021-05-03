#include "StdAfx.h"
#include <IsTools.h>
#include "DiffData.h"

using namespace diff;

CDiffData::CDiffData()
{
}

CDiffData::CDiffData(vector<int> oInitData)
{
	m_oData.insert( m_oData.begin() , oInitData.begin() , oInitData.end() );
	m_iLength = oInitData.size();
	m_oModified.resize( m_iLength + 2 );
}

CDiffData::~CDiffData()
{
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-09
    @class      CDiffData
    @function   DeleteInstance
    @return     void
    @param      CDiffData*  p
    @brief
******************************************************************************/
void CDiffData::DeleteInstance(CDiffData* p)
{
	SAFE_DELETE( p );
}

/******************************************************************************
    @author     humkyung
    @date       ????.??.??
    @class      CDiffData
    @function   GetLength
    @return     int
    @brief
******************************************************************************/
int CDiffData::GetLength() const
{
	return m_iLength;
}