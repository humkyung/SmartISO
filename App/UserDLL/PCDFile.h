// PCDFile.h: interface for the CPCDFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PCDFILE_H__9B36A7AA_A589_42F7_9DBF_9AA4D2A4DCDA__INCLUDED_)
#define AFX_PCDFILE_H__9B36A7AA_A589_42F7_9DBF_9AA4D2A4DCDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UserDocData.h"

#include <IsTools.h>
#include <vector>
using namespace std;

typedef struct  
{
	STRING_T matl_code;
	STRING_T item;
	STRING_T prp[2];
}PCDContent;

class USER_EXT_CLASS CPCDFile  
{
	CPCDFile();
	CPCDFile(const CPCDFile&){};
public:
	const char* GetFilePath() const;
	const char* GetWeldType(const STRING_T &rMatlCode, const STRING_T &rItem , const STRING_T& sWeldTypeFilePath);
	const char* GetSpec() const;
	bool FindMatlCode(const STRING_T& rMatlCode);
	bool FindItemWithMatlCode(STRING_T& item , const STRING_T& rMatlCode);
	bool Read(const STRING_T& rFilePath);
	
	static CPCDFile* CreateInstance();
	static void DeleteInstance(CPCDFile*);

	virtual ~CPCDFile();
private:
	STRING_T* m_rFilePath;

	STRING_T* m_spec;
	vector<PCDContent>* m_contentEntry;
};

#endif // !defined(AFX_PCDFILE_H__9B36A7AA_A589_42F7_9DBF_9AA4D2A4DCDA__INCLUDED_)
