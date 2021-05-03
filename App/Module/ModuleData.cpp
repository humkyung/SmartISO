// ModuleData.cpp: implementation of the CModuleData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Module.h"
#include "ModuleData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CModuleData::CModuleData() : m_pOldLineDataRecordSet(NULL) , m_pNewLineDataRecordSet(NULL)
{

}

CModuleData::~CModuleData()
{
	try
	{
		//if(NULL != m_pOldLineDataRecordSet) delete m_pOldLineDataRecordSet;
		if(NULL != m_pNewLineDataRecordSet) delete m_pNewLineDataRecordSet;
	}
	catch(...)
	{

	}
}

CModuleData* CModuleData::GetInstance()
{
	static CModuleData __instance;

	return &(__instance);
}
