// GLArc2dObject.h: interface for the GLArc2dObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLARC2DPOLYOBJECT_H__6D854052_92DE_4C53_B757_73728454D20F__INCLUDED_)
#define AFX_GLARC2DPOLYOBJECT_H__6D854052_92DE_4C53_B757_73728454D20F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <IsPoint3d.h>
#include <IsArc2d.h>
#include "GLObject.h"
#include "GLSimplePolyObject.h"
#include <vector>
using namespace std;

class _OGL2D_EXT_CLASS CGLArc2dObject : public CGLObject  
{
public:
	CGLArc2dObject(const int& nSegments = 16);
	CGLArc2dObject(const CGLArc2dObject& rhs){ (*this) = rhs; }
	CGLArc2dObject& operator =(const CGLArc2dObject& rhs);
	virtual ~CGLArc2dObject();
	int Set(const CIsArc2d& arc2d){ m_arc2d = arc2d; return ERROR_SUCCESS;}
	CIsArc2d Get() const { return m_arc2d; }
	int Copy(CGLObject* pCopied);
	int Render();
	
	DECLARE_GL_OBJECT(CGLArc2dObject)
private:
	int m_nSegments;
	CIsArc2d m_arc2d;
	CIsPoly2d m_poly2d;	//! arc를 digitize한 결과 값을 저장하는 변수.
};

#endif // !defined(AFX_GLARC2DPOLYOBJECT_H__6D854052_92DE_4C53_B757_73728454D20F__INCLUDED_)
