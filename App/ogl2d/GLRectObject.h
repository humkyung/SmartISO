// GLRectObject.h: interface for the CGLRectObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLRECTOBJECT_H__6D854052_92DE_4C53_B757_73728454D20F__INCLUDED_)
#define AFX_GLRECTOBJECT_H__6D854052_92DE_4C53_B757_73728454D20F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GLObject.h"

class _OGL2D_EXT_CLASS CGLRectObject : public CGLObject  
{
public:
	CGLRectObject();
	CGLRectObject(const CGLRectObject& rhs){ (*this) = rhs; }
	CGLRectObject& operator =(const CGLRectObject& rhs);
	void Set(GLdouble x1 , GLdouble y1 , GLdouble x2 , GLdouble y2 , const char* pTag);
	void Get(GLdouble &x1, GLdouble &y1 , GLdouble &x2, GLdouble &y2);
	CGLRectObject(GLdouble x1 , GLdouble y1 , GLdouble x2 , GLdouble y2 ,  const char* pTag);
	virtual ~CGLRectObject();
	int Copy(CGLObject* pCopied);
	int Render();
	
	DECLARE_GL_OBJECT(CGLRectObject)
private:
	GLdouble m_x1;
	GLdouble m_y1;
	GLdouble m_x2;
	GLdouble m_y2;
};

#endif // !defined(AFX_GLRECTOBJECT_H__6D854052_92DE_4C53_B757_73728454D20F__INCLUDED_)
