// DgnScene.cpp: implementation of the CDgnScene class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include <DgnTCB.h>
#include <DgnText.h>
#include <DgnSharedCellDef2d.h>
#include <DgnSharedCellDef3d.h>
#include <DgnRefData.h>
#include <DgnUnknownElement.h>
#include "DgnCanvas.h"
#include "DgnScene.h"
#include <Tokenizer.h>

#include <fstream>
#include <algorithm>
using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDgnScene* CDgnScene::m_pInst=NULL;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/**	\brief
*/
CDgnScene::CDgnScene() : m_DgnDoc(CDgnDocument::CreateInstance()) , m_pGLFontMap(NULL)
{
	m_vol.Set(-60 , -60 , -1 , 60 , 60 , 1);
	m_ptCenter.Set(0 , 0 , 0);
}

/**	\brief
*/
CDgnScene::~CDgnScene()
{
	try
	{
		if(NULL != m_pGLFontMap)
		{
			for(map<int,CGLFont*>::iterator itr=m_pGLFontMap->begin();itr != m_pGLFontMap->end();itr++)
			{
				delete (*itr).second;
			}
			delete m_pGLFontMap;
		}
	}catch(...)
	{

	}
}

//	parameter	:
//	description	:
//	remarks		:
//	returns		:
void CDgnScene::ParseLine()
{
}

/**	\brief	The CDgnScene::GetInstance function


	\return	CDgnScene*	
*/
CDgnScene* CDgnScene::GetInstance()
{
	if(NULL == CDgnScene::m_pInst) CDgnScene::m_pInst = new CDgnScene;
	return CDgnScene::m_pInst;
}

void CDgnScene::AddScene(const TCHAR* pFilePath)
{
	assert(pFilePath && "pFilePath is NULL");

	if(pFilePath && NULL != m_DgnDoc.get())
	{
		m_DgnDoc->Read(pFilePath);
		this->Set(m_DgnDoc.get());
	}
}

/**	
	@brief	DGN 파일의 내용을 SCENE에 담는다.

	@param	pDGNDoc	a parameter of type CDgnDocument *

	@return	void	
*/
void CDgnScene::Set(CDgnDocument *pDGNDoc)
{
	assert(pDGNDoc && "pDGNDoc is NULL");

	if(pDGNDoc)
	{		
		m_vol.Reset();

		const size_t nCount = m_DgnDoc->GetEntityCount();
		for(int i = 0;i < int(nCount);++i)
		{
			CDgnElement* pEnt = m_DgnDoc->GetEntityAt(i);
			if(pEnt->IsKindOf(CDgnTCB::TypeString()))		continue;
			if(pEnt->IsKindOf(CDgnSharedCellDef2d::TypeString())) continue;
			if(pEnt->IsKindOf(CDgnSharedCellDef3d::TypeString())) continue;
			if(pEnt->IsKindOf(CDgnRefData::TypeString()))		continue;
			if(pEnt->IsKindOf(CDgnUnknownElement::TypeString()))		continue;

			CDgnVolume vol = pEnt->volume();
			m_vol += CIsVolume(vol.minx() , vol.miny() , vol.minz() , vol.maxx() , vol.maxy() , vol.maxz());
		}

		double x = (m_vol.maxx() + m_vol.minx()) * 0.5;
		double y = (m_vol.maxy() + m_vol.miny()) * 0.5;
		m_ptCenter.Set(x , y , 0);
		
		CDgnCanvas* pCanvas = static_cast<CDgnCanvas*>(m_pCanvasRef);
		if(NULL != pCanvas)
		{
			pCanvas->SetScene(this);
		}
	}
}

CGLFont* CDgnScene::GetGLFont(const int &font)
{
	if(NULL != m_pGLFontMap)
	{
		return (*m_pGLFontMap)[font];
	}

	return NULL;
}

bool CDgnScene::LoadGLFont(const TCHAR *pFilePath)
{
	assert(pFilePath && "pFilePath is NULL");
	
	if(pFilePath)
	{
		//if(NULL == FindGLFont(pFilePath))
		//{
			CGLFont* pGLFont = new CGLFont;
			if(NULL != pGLFont) pGLFont->Read(pFilePath);
			
			//////////////////////////////////////////////////////////////////////////
			if(NULL == m_pGLFontMap) m_pGLFontMap = new map<int,CGLFont*>;
			if(NULL != m_pGLFontMap) (*m_pGLFontMap)[0] = pGLFont;
			//////////////////////////////////////////////////////////////////////////
		//}
		
		return true;
	}

	return false;
}

bool CDgnScene::IsEmpty()
{
	return (NULL == m_DgnDoc.get()) ? true : false;
}
