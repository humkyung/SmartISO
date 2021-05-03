// PdTable126.cpp: implementation of the CPdTable126 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PdTable126.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPdTable126::CPdTable126()
{

}

CPdTable126::~CPdTable126()
{

}
/**
	@brief	

	@author	BHK
*/
CPdTable126& CPdTable126::GetInstance(void)
{
	static CPdTable126 __instance__;

	return __instance__;
}
