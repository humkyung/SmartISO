// MaterialCodeTable.h: interface for the CMaterialCodeTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MATERIALCODETABLE_H__BC7AAF1A_4166_43E0_AD3B_D3C1DC90DA4C__INCLUDED_)
#define AFX_MATERIALCODETABLE_H__BC7AAF1A_4166_43E0_AD3B_D3C1DC90DA4C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef MODULE_EXPORTS
	#define SMARTISO_MODULE_EXT_CLASS	__declspec(dllexport)
#else
	#define SMARTISO_MODULE_EXT_CLASS	__declspec(dllimport)
#endif

#include <vector>
#include <string>
using namespace std;



class SMARTISO_MODULE_EXT_CLASS CMaterialCodeTable  
{
public:
	void Load(const CString& rFilePath);
	struct MaterialCode
	{
		string code1;
		string code2;
		string code8_9;
		string code10_11;
		int    matl_num;
	};
	
	int FindMatlNumForGALV(const char* pCode1 , const char* pCode2) const;
	int FindMatlNumWithGroupAndCodeLoc4(const char* pCode1 , const char* pCode2) const;
	CMaterialCodeTable();
	virtual ~CMaterialCodeTable();
private:
	vector<MaterialCode*>* m_pMaterialCodeEntry;
};

#endif // !defined(AFX_MATERIALCODETABLE_H__BC7AAF1A_4166_43E0_AD3B_D3C1DC90DA4C__INCLUDED_)
