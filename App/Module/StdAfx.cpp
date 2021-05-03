// stdafx.cpp : source file that includes just the standard includes
//	Module.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"


/*
	@brief	return executive path

	@author	HumKyung.BAEK

	@return	CString
*/
CString GetExecPath()
{
	TCHAR szPathName[MAX_PATH] = {0,};
	GetModuleFileName(AfxGetInstanceHandle() , szPathName , MAX_PATH);
	
	CString rExecPathName(szPathName);
	int it = 0;
	it = rExecPathName.ReverseFind('\\');
	if(it)
	{
		rExecPathName = rExecPathName.Left(it);
	}

	return rExecPathName;
}