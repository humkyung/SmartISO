// GLObject.h: interface for the CGLTextObject class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLTEXTOBJECT_H__A3FFC7D8_C598_431D_B71F_6F66FAC21E59__INCLUDED_)
#define AFX_GLTEXTOBJECT_H__A3FFC7D8_C598_431D_B71F_6F66FAC21E59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <gl/gl.h>
#include "GLObject.h"
#include "GLFont.h"

#include <string>
using namespace std;

class _OGL2D_EXT_CLASS CGLTextObject : public CGLObject  
{
public:
	CGLTextObject(GLdouble x , GLdouble y , const char* pTag);
	CGLTextObject();
	CGLTextObject(const CGLTextObject& rhs){ (*this) = rhs; }
	CGLTextObject& operator=(const CGLTextObject& rhs);
	virtual ~CGLTextObject();
	void Set(GLdouble x, GLdouble y, const char *pTag);
	void Get(GLdouble &x, GLdouble &y);
	const char* GetTextString() const;
	int Copy(CGLObject* pCopied);
	int Render();

	int SetTextStyle( const string& sTextStyle);	/// 2011.11.13 added by humkyung

	GLdouble GetTextLength() const;	//! 2011.07.08 - added by humkyung
	GLdouble GetWidthFactor() const{ return m_nWidthFactor;}
	int SetWidthFactor(const GLdouble& nWidthFactor)
	{ 
		m_nWidthFactor = nWidthFactor;
		return ERROR_SUCCESS;
	}
	GLdouble GetTextHeight() const{ return m_nTextHeight;}
	int SetTextHeight(const GLdouble& nTextHeight)
	{ 
		m_nTextHeight = nTextHeight;
		return ERROR_SUCCESS;
	}

	DECLARE_GL_OBJECT(CGLTextObject)
private:
	GLdouble m_x;
	GLdouble m_y;
	GLdouble m_nWidthFactor , m_nTextHeight , m_nRotate;
	string m_rTextStyle;
	string m_rTextString;

///	CGLFont* m_pGLFontRef;
};

#endif // !defined(AFX_GLTEXTOBJECT_H__A3FFC7D8_C598_431D_B71F_6F66FAC21E59__INCLUDED_)
