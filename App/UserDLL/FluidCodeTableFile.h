// FluidCodeTableFile.h: interface for the CFluidCodeTableFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLUIDCODETABLEFILE_H__EFB262E5_778D_4281_A7FC_4AB4201B2F54__INCLUDED_)
#define AFX_FLUIDCODETABLEFILE_H__EFB262E5_778D_4281_A7FC_4AB4201B2F54__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UserDocData.h"

#include <IsTools.h>
#include <map>
#include <string>
using namespace std;

class USER_EXT_CLASS CFluidCodeTableFile  
{
public:
	bool FindFluidCodeString(STRING_T& rFluidCodeString , const int& nFluidCode);
	bool Read(const STRING_T& rFilePath);
	CFluidCodeTableFile();
	virtual ~CFluidCodeTableFile();
private:
	void ClearContents();
	STRING_T* m_pFilePath;
	map<int , STRING_T>* m_pFluidCodeMap;
};

#endif // !defined(AFX_FLUIDCODETABLEFILE_H__EFB262E5_778D_4281_A7FC_4AB4201B2F54__INCLUDED_)
