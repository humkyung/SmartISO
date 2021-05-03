// GLEllipse2dObject.h: interface for the GLArc2dObject class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_GLELLIPSE2DPOLYOBJECT_H__6D854052_92DE_4C53_B757_73728454D20F__INCLUDED_)
#define AFX_GLELLIPSE2DPOLYOBJECT_H__6D854052_92DE_4C53_B757_73728454D20F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <IsPoint2d.h>
#include <IsEllipse2d.h>
#include "GLObject.h"
#include "GLSimplePolyObject.h"
#include <vector>
using namespace std;

class _OGL2D_EXT_CLASS CGLEllipse2dObject : public CGLObject  
{
public:
	CGLEllipse2dObject(const int& nSegments = 16);
	CGLEllipse2dObject(const CGLEllipse2dObject& rhs){ (*this) = rhs; }
	CGLEllipse2dObject& operator =(const CGLEllipse2dObject& rhs);
	virtual ~CGLEllipse2dObject();
	int Set(const CIsEllipse2d& ellipse2d){ m_ellipse2d = ellipse2d; return ERROR_SUCCESS;}
	CIsEllipse2d Get() const { return m_ellipse2d; }
	int Copy(CGLObject* pCopied);
	int Render();
	
	DECLARE_GL_OBJECT(CGLEllipse2dObject)
private:
	int m_nSegments;
	CIsEllipse2d m_ellipse2d;
	CIsPoly2d m_poly2d;	//! ellipse를 digitize한 결과 값을 저장하는 변수.
};

#endif // !defined(AFX_GLELLIPSE2DPOLYOBJECT_H__6D854052_92DE_4C53_B757_73728454D20F__INCLUDED_)