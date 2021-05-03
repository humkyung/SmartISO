#include "StdAfx.h"
#include <assert.h>
#include <float.h>
#include <Windows.h>
#include <isstring.h>
#include "BMLine.h"

#include <algorithm>
using namespace std;

CBMLine::CBMLine(CDgnText* pDgnText) : m_rxnode(NULL)
{
	if(NULL != pDgnText) m_oDgnTextList.push_back( pDgnText );
}


CBMLine::~CBMLine(void)
{
	int d = 1;
}

CBMLine::CBMLine(const CBMLine& rhs)
{
	(*this) = rhs;
}

CBMLine& CBMLine::operator=(const CBMLine& rhs)
{
	if(this != &rhs)
	{
		m_oDgnTextList.clear();
		m_oDgnTextList.insert(m_oDgnTextList.begin() , rhs.m_oDgnTextList.begin() , rhs.m_oDgnTextList.end());

		m_sDesc = rhs.m_sDesc;
	}

	return (*this);
}

/**
	@brief	return first dgn text's origin

	@author	humkyung

	@date	2013.02.14
*/
DPoint3d CBMLine::origin() const
{
	DPoint3d res={0.0,0.0,0.0};
	if(!m_oDgnTextList.empty())
	{
		res = m_oDgnTextList[0]->origin();
	}

	return res;
}

/**
	@brief	return volume of bm line(consist of several dgn text)

	@author	humkyung

	@date	2013.02.14
*/
CDgnVolume CBMLine::volume() const
{
	CDgnVolume res;
	
	for(vector<CDgnText*>::const_iterator itr = m_oDgnTextList.begin();itr != m_oDgnTextList.end();++itr)
	{
		res += (*itr)->volume();
	}

	return res;
}

/**
	@brief	return y coord of bm line

	@author	humkyung

	@date	2013.02.13
*/
double CBMLine::yCoord() const
{
	double res = DBL_MAX;

	if(!m_oDgnTextList.empty())
	{
		res = m_oDgnTextList[0]->origin().y;
	}

	return res;
}

bool SortByX(CDgnText* lhs , CDgnText* rhs)
{
	return (lhs->origin().x < rhs->origin().x);
}

int CBMLine::Add(CDgnText* pDgnText)
{
	assert(pDgnText && "pDgnText is NULL");

	if(pDgnText && (this->yCoord() == pDgnText->origin().y))
	{
		m_oDgnTextList.push_back( pDgnText );
		::stable_sort(m_oDgnTextList.begin() , m_oDgnTextList.end() , SortByX);

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

/**
	@brief	return text string combined all dgn text

	@author	humkyung

	@date	2013.02.14
*/
LPCTSTR CBMLine::textString() const
{
	static STRING_T res;

	res = _T("");
	if(!m_oDgnTextList.empty())
	{
		for(vector<CDgnText*>::const_iterator itr = m_oDgnTextList.begin();itr != m_oDgnTextList.end();++itr)
		{
			res += (*itr)->textString();
		}
	}

	return res.c_str();
}

/**
	@brief	return count of text list

	@author	humkyung

	@date	2013.02.14
*/
int CBMLine::GetTextCount() const
{
	return m_oDgnTextList.size();
}

/**
	@brief	return text string at given position

	@author	humkyung

	@date	2013.02.14
*/
LPCTSTR CBMLine::GetTextAt(const int& at)
{
	assert((at >= 0) && (at < int(m_oDgnTextList.size())));
	if((at >= 0) && (at < int(m_oDgnTextList.size())))
	{
		return m_oDgnTextList[at]->textString();
	}

	return NULL;
}

//bool CBMLine::HasPTNo()
//{
//	bool res = false;
//
//	if(!m_oDgnTextList.empty())
//	{
//		int nLastFindWhatLen = 0;
//		STRING_T str(m_oDgnTextList[0]->textString());
//		IsString::TrimWhiteSpace(str);
//		int nPos = ::FindStringHelper(str.c_str() , _T("[0-9]+") , 0, nLastFindWhatLen, m_rxnode, &m_rxmatch);
//		if(0 == nPos)
//		{
//			return true;
//		}
//	}
//
//	return res;
//}

bool CBMLine::SortByY(CBMLine* lhs , CBMLine* rhs)
{
	return (lhs->yCoord() > rhs->yCoord());
}