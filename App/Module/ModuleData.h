// ModuleData.h: interface for the CModuleData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODULEDATA_H__0F0A2959_A553_41A7_9FE0_3549CF2010DE__INCLUDED_)
#define AFX_MODULEDATA_H__0F0A2959_A553_41A7_9FE0_3549CF2010DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LineDataRecordSet.h"

class CModuleData  
{
	CModuleData();
public:
	static CModuleData* GetInstance();
	
	virtual ~CModuleData();

	CLineDataRecordSet* m_pOldLineDataRecordSet;
	CLineDataRecordSet* m_pNewLineDataRecordSet;
};

#endif // !defined(AFX_MODULEDATA_H__0F0A2959_A553_41A7_9FE0_3549CF2010DE__INCLUDED_)
