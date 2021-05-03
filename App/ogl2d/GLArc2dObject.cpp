// GLArc2dObject.cpp: implementation of the CGLArc2dObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include "GLArc2dObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENTS_GL_OBJECT(CGLArc2dObject , CGLObject , "GL_ARC2D_OBJECT")
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGLArc2dObject::CGLArc2dObject(const int& nSegments) : m_nSegments(nSegments)
{
}

/**
	@brief	대입 연산자

	@author	BHK

	@date	2009.07.08
*/
CGLArc2dObject& CGLArc2dObject::operator =(const CGLArc2dObject& rhs)
{
	if(this != &rhs)
	{
		CGLObject::operator =(rhs);
		m_arc2d = rhs.m_arc2d;

	}

	return (*this);
}

CGLArc2dObject::~CGLArc2dObject()
{
	try
	{
	}
	catch(...)
	{
	}
}

/**
	@brief	GLObject의 내용을 복사한다.

	@author	BHK

	@date	2009.07.08
*/
int CGLArc2dObject::Copy(CGLObject* pCopied)
{
	assert(pCopied && "pCopied is NULL");

	if(pCopied && pCopied->IsKindOf(CGLArc2dObject::TypeString()))
	{
		CGLArc2dObject* pObj = static_cast<CGLArc2dObject*>(pCopied);

		(*this) = (*pObj);

		return ERROR_SUCCESS;
	}

	return ERROR_BAD_ENVIRONMENT;
}

/**
	@brief	GLLineObject를 그린다.

	@author	BHK

	@date	2009.07.08
*/
int CGLArc2dObject::Render()
{
	if(0 == m_poly2d.GetVertexCount()) m_arc2d.CreateSegments(m_poly2d , m_nSegments);
	glBegin(GL_LINE_STRIP);
	for(int i = 0;i < m_poly2d.GetVertexCount();++i)
	{
		const CIsPoint2d pt = m_poly2d.GetVertexAt(i);
		glVertex3f((float)pt.x(),(float)pt.y(),(float)0.f);
	}
	glEnd();
	
	return ERROR_SUCCESS;
}
