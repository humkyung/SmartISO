// GLLineObject.cpp: implementation of the CGLLineObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include "GLLineObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENTS_GL_OBJECT(CGLLineObject , CGLObject , "GL_LINE_OBJECT")
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGLLineObject::CGLLineObject() : CGLObject()
{
}


CGLLineObject::CGLLineObject(GLdouble x1 , GLdouble y1 , GLdouble x2 , GLdouble y2 ,  const char* pTag)
{
	m_tag = pTag;
	m_line2d.Set(CIsPoint2d(x1 , y1) , CIsPoint2d(x2 , y2));
}

/**
	@brief	대입 연산자

	@author	BHK

	@date	2009.07.07
*/
CGLLineObject& CGLLineObject::operator =(const CGLLineObject& rhs)
{
	if(this != &rhs)
	{
		CGLObject::operator =(rhs);

		m_line2d = rhs.m_line2d;
	}

	return (*this);
}

CGLLineObject::~CGLLineObject()
{

}

void CGLLineObject::Get(GLdouble &x1, GLdouble &y1 , GLdouble &x2, GLdouble &y2)
{
	x1 = m_line2d.start().x();
	y1 = m_line2d.start().y();
	x2 = m_line2d.end().x();
	y2 = m_line2d.end().y();
}

void CGLLineObject::Set(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2, const char *pTag)
{
	assert(pTag && "pTag is NULL");

	if(pTag)
	{
		m_line2d.Set(CIsPoint2d(x1 , y1) , CIsPoint2d(x2 , y2));
	}
}

/**
	@brief	GLObject의 내용을 복사한다.

	@author	BHK

	@date	2009.07.06
*/
int CGLLineObject::Copy(CGLObject* pCopied)
{
	assert(pCopied && "pCopied is NULL");

	if(pCopied && pCopied->IsKindOf(CGLLineObject::TypeString()))
	{
		CGLLineObject* pObj = static_cast<CGLLineObject*>(pCopied);

		(*this) = (*pObj);

		return ERROR_SUCCESS;
	}

	return ERROR_BAD_ENVIRONMENT;
}

/**
	@brief	GLLineObject를 그린다.

	@author	BHK

	@date	2009.07.06
*/
int CGLLineObject::Render()
{
	glBegin(GL_LINES);
	glVertex3f((float)m_line2d.start().x(),(float)m_line2d.start().y(),(float)0.f);
	glVertex3f((float)m_line2d.end().x(),(float)m_line2d.end().y(),(float)0.f);
	glEnd();

	return ERROR_SUCCESS;
}
