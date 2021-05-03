// DEPFile.h: interface for the CDEPFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEPFILE_H__9B36A7AA_A589_42F7_9DBF_9AA4D2A4DCDA__INCLUDED_)
#define AFX_DEPFILE_H__9B36A7AA_A589_42F7_9DBF_9AA4D2A4DCDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
using namespace std;

typedef struct  
{
	vector<string> contents;
}DEPContent;

class CDEPFile  
{
	CDEPFile();
	CDEPFile(const CDEPFile&){};
public:
	const char* GetFilePath() const;
	bool FindDependtionCode(const string& rMatlCode);
	bool FindSysCommodityCodeWithDependtionCode(string& item , const string& rDependtionCode);
	bool Read(const string& rFilePath);
	
	static CDEPFile& GetInstance();

	virtual ~CDEPFile();
private:
	string* m_rFilePath;

	vector<DEPContent>* m_contentEntry;
};

#endif // !defined(AFX_DEPFILE_H__9B36A7AA_A589_42F7_9DBF_9AA4D2A4DCDA__INCLUDED_)
