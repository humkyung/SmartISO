#include "StdAfx.h"
#include "InlineElement.h"

using namespace IsoElement;

CInlineElement::CInlineElement(DPoint3d pt1 , DPoint3d pt2)
{
	m_ptConn.push_back(pt1);
	m_ptConn.push_back(pt2);

	m_sTypeString = CInlineElement::TypeString();
}

CInlineElement::~CInlineElement(void)
{
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-20
    @class      CInlineElement
    @function   TypeString
    @return     CString
    @brief		return type string of inline element
******************************************************************************/
STRING_T CInlineElement::TypeString()
{
	return _T("inline");
}

void CInlineElement::Check(CDgnDocument* pDgnDoc , CString sColor)
{
	::DrawPolyline( pDgnDoc , m_ptConn , sColor);
}