// IMPFile.h: interface for the CIMPFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMPFILE_H__9B36A7AA_A589_42F7_9DBF_9AA4D2A4DCDA__INCLUDED_)
#define AFX_IMPFILE_H__9B36A7AA_A589_42F7_9DBF_9AA4D2A4DCDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
using namespace std;

typedef struct  
{
	vector<string> contents;
}IMPContent;

class CIMPFile  
{
	CIMPFile();
	CIMPFile(const CIMPFile&){};
public:
	const char* GetFilePath() const;
	bool FindDependtionCode(const string& rMatlCode);
	bool FindBMCSCodeWithDependtionCode(string& item , const string& rDependtionCode);
	bool Read(const string& rFilePath);
	
	static CIMPFile& GetInstance();

	virtual ~CIMPFile();
private:
	string* m_rFilePath;

	vector<IMPContent>* m_contentEntry;
};

#endif // !defined(AFX_IMPFILE_H__9B36A7AA_A589_42F7_9DBF_9AA4D2A4DCDA__INCLUDED_)
