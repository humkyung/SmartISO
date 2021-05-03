// GLScene.cpp: implementation of the CGLScene class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include "GLCanvas.h"
#include "GLScene.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//	parameter	:
//	description	:
//	remarks		:
//	returns		:
CGLScene::CGLScene() : m_nUpperBound(0) , m_pCanvasRef(NULL)
{
	m_pGLObjectMap = new map<long , CGLObject*>;
	CGLScene::Clear();
}

//	parameter	:
//	description	:
//	remarks		:
//	returns		:
CGLScene::~CGLScene()
{
	try
	{
		CGLScene::Clear();
		if(m_pGLObjectMap) SAFE_DELETE(m_pGLObjectMap);
	}
	catch(...)
	{
	}
}

//	parameter	:
//	description	:
//	remarks		:
//	returns		:
CIsVolume CGLScene::GetVolume() const
{
	return m_vol;
}

/**
	@brief	GLObject를 추가한다.

	@author	BHK

	@date	2009.07.08
*/
int CGLScene::Add(CGLObject* pObj)
{
	assert(pObj && "pObj is NULL");

	if(pObj)
	{
		m_pGLObjectMap->insert(make_pair(m_nUpperBound++ , pObj));
		return ERROR_SUCCESS;
	}

	return ERROR_BAD_ENVIRONMENT;
}


/**	
	@brief	The CGLScene::Clear function
	
	@author	BHK

	@return	void	
*/
void CGLScene::Clear()
{
	m_nUpperBound = 0L;
	if(m_pGLObjectMap)
	{
		for(map<long , CGLObject*>::iterator itr = m_pGLObjectMap->begin();itr != m_pGLObjectMap->end();++itr)
		{
			SAFE_DELETE(itr->second);
		}
		m_pGLObjectMap->clear();
	}

	m_vol.Set(0.f , 0.f , 0.f , 0.f , 0.f, 0.f);

	m_ptCenter.Set(0.f , 0.f , 0.f);
}
