// GLLineObject.h: interface for the CGLLineObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLLINEOBJECT_H__6D854052_92DE_4C53_B757_73728454D20F__INCLUDED_)
#define AFX_GLLINEOBJECT_H__6D854052_92DE_4C53_B757_73728454D20F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <IsLine2d.h>
#include "GLObject.h"

class _OGL2D_EXT_CLASS CGLLineObject : public CGLObject  
{
public:
	CGLLineObject();
	CGLLineObject(const CGLLineObject& rhs){ (*this) = rhs; }
	CGLLineObject& operator =(const CGLLineObject& rhs);
	void Set(GLdouble x1 , GLdouble y1 , GLdouble x2 , GLdouble y2 , const char* pTag);
	void Get(GLdouble &x1, GLdouble &y1 , GLdouble &x2, GLdouble &y2);
	CGLLineObject(GLdouble x1 , GLdouble y1 , GLdouble x2 , GLdouble y2 ,  const char* pTag);
	virtual ~CGLLineObject();
	int Copy(CGLObject* pCopied);
	int Render();
	
	DECLARE_GL_OBJECT(CGLLineObject)
private:
	CIsLine2d m_line2d;
};

#endif // !defined(AFX_GLLINEOBJECT_H__6D854052_92DE_4C53_B757_73728454D20F__INCLUDED_)
