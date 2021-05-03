// GLObject.h: interface for the CGLObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLOBJECT_H__A3FFC7D8_C598_431D_B71F_6F66FAC21E59__INCLUDED_)
#define AFX_GLOBJECT_H__A3FFC7D8_C598_431D_B71F_6F66FAC21E59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <gl/gl.h>

#include <string>
using namespace std;

#ifdef	_OGL2D
#define	_OGL2D_EXT_CLASS	__declspec(dllexport)
#else
#define	_OGL2D_EXT_CLASS	__declspec(dllimport)
#endif

#ifndef SAFE_DELETE 
#define SAFE_DELETE(p)  { if(p) { delete (p); (p)=NULL; } } 
#endif // SAFE_DELETE 

#define	DECLARE_GL_OBJECT(classname)\
public:\
	virtual bool IsKindOf(const char* typeString) const;\
	static CGLObject* CreateInstance();\
	static int DeleteInstance(classname* p);\
	CGLObject* Clone();\
	string GetTypeString();\
	static const char* TypeString();\

#define	IMPLEMENTS_GL_OBJECT(classname,baseclassname,typestr)\
CGLObject* classname::CreateInstance()\
{\
	return (new classname);\
}\
int classname::DeleteInstance(classname* p)\
{\
	assert(p && "p is NULL");\
	if(p)\
	{\
		delete p;\
		p = NULL;\
	}\
	return ERROR_SUCCESS;\
}\
CGLObject* classname::Clone()\
{\
	classname* p = static_cast<classname*>(classname::CreateInstance());\
	if(p) p->Copy(this);\
	return p;\
}\
bool classname::IsKindOf(const char* typeString) const\
{\
	if(0 == strcmp(classname::TypeString() , typeString)) return true;\
	return baseclassname::IsKindOf(typeString);\
}\
string classname::GetTypeString()\
{\
	return classname::TypeString();\
}\
const char* classname::TypeString()\
{\
	static const char* typeString=typestr;\
	return typestr;\
}\

class _OGL2D_EXT_CLASS CGLObject
{
public:
	virtual bool IsKindOf(const char* typeString) const;
	static string TypeString();
	
	virtual int Render(){ return ERROR_SUCCESS; }
	
	CGLObject(GLdouble x , GLdouble y , const char* pTag);
	CGLObject();
	CGLObject(const CGLObject& rhs);
	CGLObject& operator=(const CGLObject& rhs);
	virtual ~CGLObject();

	long GetName() const { return m_name; }
	int SetName(const long& name) { m_name = name; return ERROR_SUCCESS;}

	const char* tag() const{ return m_tag.c_str(); }

	int GetRGB(GLdouble& red , GLdouble& green , GLdouble& blue)
	{
		red = m_red;
		green = m_green;
		blue = m_blue;
		return ERROR_SUCCESS;
	}
	int SetRGB(const GLdouble& red , const GLdouble& green , const GLdouble& blue)
	{
		m_red = red;
		m_green = green;
		m_blue = blue;
		return ERROR_SUCCESS;
	}
protected:
	GLdouble m_red , m_green , m_blue;
	long m_name;
	string m_tag;
};

#endif // !defined(AFX_GLOBJECT_H__A3FFC7D8_C598_431D_B71F_6F66FAC21E59__INCLUDED_)
