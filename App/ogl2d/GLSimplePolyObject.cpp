// GLSimplePolyObject.cpp: implementation of the CGLSimplePolyObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include "GLSimplePolyObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENTS_GL_OBJECT(CGLSimplePolyObject , CGLObject , "GL_SIMPLE_POLY_OBJECT")
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGLSimplePolyObject::CGLSimplePolyObject()
{
	m_ppts = new vector<CIsPoint3d>;
}

/**
	@brief	대입 연산자

	@author	BHK

	@date	2009.07.08
*/
CGLSimplePolyObject& CGLSimplePolyObject::operator =(const CGLSimplePolyObject& rhs)
{
	if(this != &rhs)
	{
		CGLObject::operator =(rhs);

		m_ppts->clear();
		m_ppts->insert(m_ppts->begin() , rhs.m_ppts->begin() , rhs.m_ppts->end());
	}

	return (*this);
}

CGLSimplePolyObject::~CGLSimplePolyObject()
{
	try
	{
		if(m_ppts) SAFE_DELETE(m_ppts);
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
int CGLSimplePolyObject::Copy(CGLObject* pCopied)
{
	assert(pCopied && "pCopied is NULL");

	if(pCopied && pCopied->IsKindOf(CGLSimplePolyObject::TypeString()))
	{
		CGLSimplePolyObject* pObj = static_cast<CGLSimplePolyObject*>(pCopied);

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
int CGLSimplePolyObject::Render()
{
	glColor3f((float)m_red/255.,(float)m_green/255.,(float)m_blue/255.);
	glBegin(GL_LINE_STRIP);
	for(vector<CIsPoint3d>::const_iterator itr = m_ppts->begin();itr != m_ppts->end();++itr)
	{
		glVertex3f((float)itr->x(),(float)itr->y(),(float)itr->z());
	}
	glEnd();
	
	return ERROR_SUCCESS;
}
