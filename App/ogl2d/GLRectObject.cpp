// GLRectObject.cpp: implementation of the CGLRectObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include "GLRectObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENTS_GL_OBJECT(CGLRectObject , CGLObject , "GL_RECT_OBJECT")
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGLRectObject::CGLRectObject() : CGLObject()
{
	m_x1 = 0;
	m_y1 = 0;
	m_x2 = 0;
	m_y2 = 0;
}


CGLRectObject::CGLRectObject(GLdouble x1 , GLdouble y1 , GLdouble x2 , GLdouble y2 ,  const char* pTag)
{
	m_tag = pTag;

	m_x1 = x1;
	m_y1 = y1;
	m_x2 = x2;
	m_y2 = y2;
}

/**
	@brief	대입 연산자

	@author	BHK

	@date	2009.07.07
*/
CGLRectObject& CGLRectObject::operator =(const CGLRectObject& rhs)
{
	if(this != &rhs)
	{
		CGLObject::operator =(rhs);

		m_x1 = rhs.m_x1;
		m_y1 = rhs.m_y1;
		m_x2 = rhs.m_x2;
		m_y2 = rhs.m_y2;
	}

	return (*this);
}

CGLRectObject::~CGLRectObject()
{

}

void CGLRectObject::Get(GLdouble &x1, GLdouble &y1 , GLdouble &x2, GLdouble &y2)
{
	x1 = m_x1;
	y1 = m_y1;
	x2 = m_x2;
	y2 = m_y2;
}

void CGLRectObject::Set(GLdouble x1, GLdouble y1, GLdouble x2, GLdouble y2, const char *pTag)
{
	assert(pTag && "pTag is NULL");

	if(pTag)
	{
		m_x1 = x1;
		m_y1 = y1;
		m_x2 = x2;
		m_y2 = y2;
	}
}

/**
	@brief	GLObject의 내용을 복사한다.

	@author	BHK

	@date	2009.07.06
*/
int CGLRectObject::Copy(CGLObject* pCopied)
{
	assert(pCopied && "pCopied is NULL");

	if(pCopied && pCopied->IsKindOf(CGLRectObject::TypeString()))
	{
		CGLRectObject* pObj = static_cast<CGLRectObject*>(pCopied);

		(*this) = (*pObj);

		return ERROR_SUCCESS;
	}

	return ERROR_BAD_ENVIRONMENT;
}

/**
	@brief	GLRectObject를 그린다.

	@author	BHK

	@date	2009.07.06
*/
int CGLRectObject::Render()
{
	glBegin(GL_LINE_STRIP);
	glVertex3f((float)m_x1,(float)m_y1,(float)0.f);
	glVertex3f((float)m_x2,(float)m_y1,(float)0.f);
	glVertex3f((float)m_x2,(float)m_y2,(float)0.f);
	glVertex3f((float)m_x1,(float)m_y2,(float)0.f);
	glVertex3f((float)m_x1,(float)m_y1,(float)0.f);
	glEnd();

	return ERROR_SUCCESS;
}