// NoteTableForOra.h: interface for the CNoteTableForOra class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UserDocData.h"

#include <fstream>
#include <vector>
using namespace std;

extern ofstream __STNOTE__;
class USER_EXT_CLASS CNoteTableForOra  
{
public:
	void Read(ifstream &ifile);
	typedef struct _NoteNode
	{
		int nNum;
		string rNumString;

		void OutputDebugString()
		{
			CString rMsg;
			rMsg.Format("%d , %s" , nNum , rNumString.c_str());
			__STNOTE__ << rMsg.operator LPCTSTR() << std::endl;
		}
	}NoteNode;

	bool Find(NoteNode &noteNode, const string &key);
	bool Find(NoteNode &noteNode , const int& key);
	void Add(const NoteNode &noteNode);
	void ParseColumnIndex(const CString &rTextString);

	CNoteTableForOra();
	virtual ~CNoteTableForOra();
private:
	vector<NoteNode> m_noteNodes;
	int m_nFieldPos[4];	///< 필드 위치를 담는 배열.
};
