#include "StdAfx.h"
#include "IsRegExp.h"
#include "IsoTitleBlockData.h"


CIsoTitleBlockData::CIsoTitleBlockData(DPoint3d origin , const CString& sTextString)
{
	m_origin = origin;
	m_sDataString = sTextString;
}

CIsoTitleBlockData::CIsoTitleBlockData(const CIsoTitleBlockData& rhs)
{
	(*this) = rhs;
}

CIsoTitleBlockData::~CIsoTitleBlockData(void)
{
	
}
/******************************************************************************
    @author     humkyung
    @date       2011-10-04
    @class      CIsoBMData
    @function   operator=
    @return     CIsoBMData&
    @param      const   CIsoBMData&
    @param      rhs
    @brief		대입 연산자
******************************************************************************/
CIsoTitleBlockData& CIsoTitleBlockData::operator=(const CIsoTitleBlockData& rhs)
{
	if(this != &rhs)
	{
		m_origin = rhs.m_origin ;
		m_sDataString = rhs.m_sDataString ;
		m_oDgnElementList.insert(m_oDgnElementList.begin() , rhs.m_oDgnElementList.begin() , rhs.m_oDgnElementList.end());
	}
	return (*this);
}

/******************************************************************************
    @author     humkyung
    @date       2011-09-21
    @class      CIsoBMData
    @function   operator==
    @return     bool
    @param      const   CIsoBMData&
    @param      rhs
    @brief
******************************************************************************/
bool CIsoTitleBlockData::operator==(const CIsoTitleBlockData& rhs)
{
	if((m_origin.x == rhs.m_origin.x) && (m_origin.y == rhs.m_origin.y) && (m_sDataString == rhs.m_sDataString))
	{
		return ((m_origin.x == rhs.m_origin.x) && (m_origin.y == rhs.m_origin.y) && (m_sDataString == rhs.m_sDataString));
	}

	return false;
}