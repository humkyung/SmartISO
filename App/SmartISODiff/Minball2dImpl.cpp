// Minball2dImpl.cpp: implementation of the CMinball2dImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Minball2dImpl.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMinball2dImpl::CMinball2dImpl()
{
}

CMinball2dImpl::~CMinball2dImpl()
{
	try
	{
	}
	catch(...)
	{
	}
}

/******************************************************************************
    @author     humkyung
    @date       2011-12-01
    @class      CMinball2dImpl
    @function   IsEmpty
    @return     bool
    @brief
******************************************************************************/
bool CMinball2dImpl::IsEmpty() const
{
	return ((0.0 == m_ptOrigin.x) && (0.0 == m_ptOrigin.y) && (0.f == m_dRadius));
}

/******************************************************************************
    @author     humkyung
    @date       2011-12-01
    @class      CMinball2dImpl
    @function   Clear
    @return     void
    @brief
******************************************************************************/
void CMinball2dImpl::Clear()
{
	m_ptOrigin.x = m_ptOrigin.y = m_ptOrigin.z = 0.0;
	m_dRadius = 0.0;
}

/**
	@brief	

	@author	BAEK HUM KYUNG
*/
int CMinball2dImpl::CalculateFrom(const vector<DPoint3d>& pts)
{
	double minx = pts[0].x , miny = pts[0].y;
	double maxx = minx , maxy = miny;
	for(vector<DPoint3d>::const_iterator itr = pts.begin();itr != pts.end();++itr)
	{
		if(minx > itr->x) minx = itr->x;
		if(miny > itr->y) miny = itr->y;
		if(maxx < itr->x) maxx = itr->x;
		if(maxy < itr->y) maxy = itr->y;
	}
	
	{
		DPoint3d center;
		center.x = (minx + maxx)*0.5;
		center.y = (miny + maxy)*0.5;
		double radius = max((maxx - minx) , (maxy - miny)) * 0.5;
		double dist_power_2 = radius * radius;
		for(vector<DPoint3d>::const_iterator itr = pts.begin();itr != pts.end();++itr)
		{
			const double dx = itr->x - center.x;
			const double dy = itr->y - center.y;
			const double new_dist_power_2 = dx*dx + dy*dy;
			if(new_dist_power_2 > dist_power_2)
			{
				const double dist = sqrt(new_dist_power_2);
				radius = (radius + dist) * 0.5;
				dist_power_2 = radius * radius;
				center.x = (radius*center.x + (dist - radius)*itr->x) / dist;
				center.y = (radius*center.y + (dist - radius)*itr->y) / dist;
			}
		}

		m_ptOrigin = center;
		m_dRadius =  radius;
	}

	return ERROR_SUCCESS;
}