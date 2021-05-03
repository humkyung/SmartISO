// GLObject.cpp: implementation of the CGLObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include "GLObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGLObject::CGLObject() : m_name(0)
{
	m_red = 255;
	m_green = 255;
	m_blue = 255;
}

CGLObject::CGLObject(GLdouble x , GLdouble y , const char* pTag) : m_name(0) , m_tag(pTag)
{
	m_red = 255;
	m_green = 255;
	m_blue = 255;
}

CGLObject::~CGLObject()
{

}

CGLObject::CGLObject(const CGLObject& rhs)
{
	(*this) = rhs;	
}

/**
	@brief	대입 연산자.

	@author	BHK

	@date	2009.07.07
*/
CGLObject& CGLObject::operator=(const CGLObject& rhs)
{
	if(this != &rhs)
	{
		m_red  = rhs.m_red;
		m_green= rhs.m_green;
		m_blue = rhs.m_blue;
		m_tag = rhs.m_tag;
	}

	return (*this);
}

string CGLObject::TypeString()
{
	static const string __TYPE_STRING__ = _T("GLObject");
	return __TYPE_STRING__;
}

/**
	@brief	

	@author	BHK

	@date
*/
bool CGLObject::IsKindOf(const char* pTypeString) const
{
	assert(pTypeString && "pTypeString is NULL");

	if(pTypeString) return (CGLObject::TypeString() == string(pTypeString));

	return false;
}
