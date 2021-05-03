// GLSimplePolyObject.h: interface for the GLSimplePolyObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLSIMPLEPOLYOBJECT_H__6D854052_92DE_4C53_B757_73728454D20F__INCLUDED_)
#define AFX_GLSIMPLEPOLYOBJECT_H__6D854052_92DE_4C53_B757_73728454D20F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <IsPoint3d.h>
#include "GLObject.h"
#include <vector>
using namespace std;

class _OGL2D_EXT_CLASS CGLSimplePolyObject : public CGLObject  
{
public:
	CGLSimplePolyObject();
	CGLSimplePolyObject(const CGLSimplePolyObject& rhs){ (*this) = rhs; }
	CGLSimplePolyObject& operator =(const CGLSimplePolyObject& rhs);
	virtual ~CGLSimplePolyObject();
	int Copy(CGLObject* pCopied);
	int Length() const { return m_ppts->size(); }
	CIsPoint3d At(const int& at)
	{
		if(at < m_ppts->size()) return (*m_ppts)[at];
		throw exception("range error");
	}
	int Add(const CIsPoint3d& pt)
	{
		m_ppts->push_back(pt);
		return ERROR_SUCCESS;
	}
	int Render();
	
	DECLARE_GL_OBJECT(CGLSimplePolyObject)
private:
	vector<CIsPoint3d>* m_ppts;
};

#endif // !defined(AFX_GLSIMPLEPOLYOBJECT_H__6D854052_92DE_4C53_B757_73728454D20F__INCLUDED_)
