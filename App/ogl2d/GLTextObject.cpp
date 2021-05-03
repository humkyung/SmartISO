// GLObject.cpp: implementation of the CGLTextObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ogl2d.h"
#include <assert.h>
#include "GLTextObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENTS_GL_OBJECT(CGLTextObject , CGLObject , "GL_TEXT_OBJECT")
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGLTextObject::CGLTextObject() : m_x(0) , m_y(0) , m_nRotate(0)/// , m_pGLFontRef(NULL)
{
	m_nWidthFactor = 1.;
	m_nTextHeight  = 1.;
}

CGLTextObject::CGLTextObject(GLdouble x , GLdouble y , const char* pTag) : m_nRotate(0)/// , m_pGLFontRef(NULL)
{
	m_nWidthFactor = 1.;
	m_nTextHeight  = 1.;
	Set(x , y , pTag);
}

/**
	@brief	대입연산자.

	@author	BHK

	@date	2009.07.07
*/
CGLTextObject& CGLTextObject::operator=(const CGLTextObject& rhs)
{
	if(this != &rhs)
	{
		CGLObject::operator =(rhs);

		m_x = rhs.m_x;
		m_y = rhs.m_y;
		m_nWidthFactor = rhs.m_nWidthFactor;
		m_nTextHeight  = rhs.m_nTextHeight;
		m_nRotate      = rhs.m_nRotate;
		m_rTextStyle   = rhs.m_rTextStyle;
		m_rTextString  = rhs.m_rTextString;
	}

	return (*this);
}

CGLTextObject::~CGLTextObject()
{

}

void CGLTextObject::Set(GLdouble x, GLdouble y, const char *pTag)
{
	assert(pTag && "pTag is NULL");

	if(pTag)
	{
		m_x = x;
		m_y = y;
		m_rTextString = pTag;
	}
}

void CGLTextObject::Get(GLdouble &x, GLdouble &y)
{
	x = m_x;
	y = m_y;
}

const char* CGLTextObject::GetTextString() const
{
	return m_rTextString.c_str();
}

/**
	@brief	GLTextObject의 내용을 복사한다.

	@author	BHK

	@date	2009.07.06
*/
int CGLTextObject::Copy(CGLObject* pCopied)
{
	assert(pCopied && "pCopied is NULL");

	if(pCopied && pCopied->IsKindOf(CGLTextObject::TypeString()))
	{
		CGLTextObject* pObj = static_cast<CGLTextObject*>(pCopied);

		(*this) = (*pObj);

		return ERROR_SUCCESS;
	}

	return ERROR_BAD_ENVIRONMENT;
}

/**
	@brief	get length of text

	@author	humkyung

	@date	2011.07.08
*/
GLdouble CGLTextObject::GetTextLength() const
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));
	
	COgl2dApp* pApp = (COgl2dApp*)AfxGetApp();
	if(pApp)
	{
		CGLFont* pGLFont = pApp->GetGLFont(m_rTextStyle);
		if(pGLFont)
		{
			pGLFont->SetXScale(m_nWidthFactor);
			pGLFont->SetYScale(m_nTextHeight);
			pGLFont->SetZScale(1.f);
			pGLFont->SetZRotate(m_nRotate);

			return pGLFont->TextLength(m_rTextString.c_str());
		}
		
	}
	
	return 0.0f;
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-13
    @class      CGLTextObject
    @function   SetTextStyle
    @return     int
    @param      const       string&
    @param      sTextStyle
    @brief
******************************************************************************/
int CGLTextObject::SetTextStyle(const string& sTextStyle)
{
	m_rTextStyle = sTextStyle;

	return ERROR_SUCCESS;
}

/**
	@brief	GLTextObject를 그린다. 먼저 TextStyle에 맞는 Font를 구한다.

	@author	humkyung

	@date	2009.07.06
*/
int CGLTextObject::Render()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState( ));

	COgl2dApp* pApp = (COgl2dApp*)AfxGetApp();
	if(pApp)
	{
		CGLFont* pGLFont = pApp->GetGLFont(m_rTextStyle);
		if(pGLFont)
		{
			glColor3f((float)m_red/255.,(float)m_green/255.,(float)m_blue/255.);

			pGLFont->SetXScale(m_nWidthFactor);
			pGLFont->SetYScale(m_nTextHeight);
			pGLFont->SetZScale(1.f);
			pGLFont->SetZRotate(m_nRotate);
			pGLFont->GLDrawText(m_x , m_y , 0.f , m_rTextString.c_str());

			return ERROR_SUCCESS;
		}
	}

	return ERROR_BAD_ENVIRONMENT;
}