// NtrData.cpp: implementation of the CNtrData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UserApp.h"
#include "NtrData.h"
#include "UserAppDocData.h"

#include <Tokenizer.h>

#include <sstream>
#include <IsString.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNtrData::CNtrData()
{
	ParseColumnIndex();
}

CNtrData::CNtrData(const CNtrData& rhs)
{
	(*this) = rhs;
}

CNtrData::~CNtrData()
{

}

/**
**/
CNtrData& CNtrData::operator=(const CNtrData& rhs)
{
	if(this != &rhs)
	{
		m_rTextString = rhs.m_rTextString;
		m_rLineNo = rhs.m_rLineNo;
		m_rSerialNo = rhs.m_rSerialNo;
		m_rFieldShop = rhs.m_rFieldShop;
		m_rFluidCode = rhs.m_rFluidCode;
		m_rMainSize = rhs.m_rMainSize;
		m_rSubSize = rhs.m_rSubSize;
		m_rMatlCode = rhs.m_rMatlCode;
	}
	
	return (*this);
}

/**
**/
const char* CNtrData::GetTextString() const
{
	return m_rTextString.c_str();
}

CString CNtrData::GetLineNo() const
{
	return m_rLineNo;
}

CString CNtrData::GetSerialNo() const
{
	return m_rSerialNo;
}

CString CNtrData::GetFieldShop() const
{
	return m_rFieldShop;
}

CString CNtrData::GetFluidCode() const
{
	return m_rFluidCode;
}

CString CNtrData::GetMainSize() const
{
	return m_rMainSize;
}

CString CNtrData::GetSubSize() const
{
	return m_rSubSize;
}

CString CNtrData::GetMatlCode() const
{
	return m_rMatlCode;
}

void CNtrData::ParseColumnIndex()
{
	/*
	rec   mat     1st     2nd     qty             commodity                                   [ wbs ]                                          insul   option
	type  class   size    size    (pipe,bolt)     code (bmcs code)      line id               p    a    u    s    sht seq no            f/s fluid   type    code
	----  ------  ------  ------  --------------  --------------------  --------------------  ---  ---  ---  ---  --  ----------------  --  ------  ------  ------
	*/
	
	m_nFieldPos[0] = 0;
	m_nFieldPos[1] = 14;	/// 1st size
	m_nFieldPos[2] = 22;	/// 2nd size
	m_nFieldPos[3] = 30;
	m_nFieldPos[4] = 45;	/// commodity code
	m_nFieldPos[5] = 68;	/// line id
	m_nFieldPos[6] = 90;
	m_nFieldPos[7] = 114;	/// seq no
	m_nFieldPos[8] = 132;	/// F/S
	m_nFieldPos[9] = 136;	/// fluid
	m_nFieldPos[10] = 144;
}

/**
	@brief	NTR TEXT를 분석한다.

	@author	HumKyung.BAEK
*/
bool CNtrData::ParseText(string rTextString)
{
	if(!rTextString.empty())
	{
		try
		{
			CString rTemp;
			double nTemp = 0.f;
			sscanf(rTextString.substr(m_nFieldPos[1], m_nFieldPos[2] - m_nFieldPos[1]).c_str(), _T("%lf") ,&nTemp);
			{
				stringstream oss;
				oss << nTemp;
				m_rMainSize = oss.str().c_str();
				m_rMainSize.TrimLeft(); m_rMainSize.TrimRight();
			}

			string aToken = rTextString.substr(m_nFieldPos[2], m_nFieldPos[3] - m_nFieldPos[2]).c_str();
			IsString::TrimWhiteSpace(aToken);
			if(!aToken.empty())
			{
				sscanf(aToken.c_str() , "%lf" ,&nTemp);
				{
					stringstream oss;
					oss << nTemp;
					m_rSubSize = oss.str().c_str();
					m_rSubSize.TrimLeft(); m_rSubSize.TrimRight();
				}
			}
						
			rTemp = rTextString.substr(m_nFieldPos[4], m_nFieldPos[5] - m_nFieldPos[4]).c_str();
			rTemp.TrimLeft(); rTemp.TrimRight();		
			m_rMatlCode = rTemp;
			
			rTemp = rTextString.substr(m_nFieldPos[5], m_nFieldPos[6] - m_nFieldPos[5]).c_str();
			rTemp.TrimLeft(); rTemp.TrimRight();		
			m_rLineNo = rTemp;

			rTemp = rTextString.substr(m_nFieldPos[7], m_nFieldPos[8] - m_nFieldPos[7]).c_str();
			rTemp.TrimLeft(); rTemp.TrimRight();
			m_rSerialNo = rTemp;

			rTemp = rTextString.substr(m_nFieldPos[8], m_nFieldPos[9] - m_nFieldPos[8]).c_str();
			rTemp.TrimLeft(); rTemp.TrimRight();
			m_rFieldShop = rTemp;
			
			rTemp = rTextString.substr(m_nFieldPos[9], m_nFieldPos[10] - m_nFieldPos[9]).c_str();
			rTemp.TrimLeft(); rTemp.TrimRight();
			m_rFluidCode = rTemp;

			return true;
		}
		catch(exception& ex)
		{
			UNUSED_ALWAYS(ex);
		}
	}

	return false;
}


