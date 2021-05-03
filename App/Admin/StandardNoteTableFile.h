// StandardNoteTableFile.h: interface for the CStandardNoteTableFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STANDARDNOTETABLEFILE_H__D8DFB788_2E54_4404_BFD5_516A2C8C9A32__INCLUDED_)
#define AFX_STANDARDNOTETABLEFILE_H__D8DFB788_2E54_4404_BFD5_516A2C8C9A32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <map>
using namespace std;

class CStandardNoteTableFile  
{
public:
	class CStandardNoteType
	{
		typedef struct tagNoteNode
		{
			int nNum;
			string rNumString;
		}NoteNode;
	public:
		int GetItemCount() const { return m_noteNodes.size(); }
		NoteNode* GetItemAt(const unsigned int& at)
		{
			if(at < m_noteNodes.size()) return &(m_noteNodes.at(at));

			return NULL;
		}

		int ParseColumnIndex(const CString &rTextString);
		int Read(ifstream& ifile);
	private:
		int m_nNoteType;
		vector<NoteNode> m_noteNodes;
		int m_nFieldPos[4];	///< 필드 위치를 담는 배열.
	};

	CStandardNoteTableFile();
public:
	CStandardNoteType* GetStandardNoteType(const int& nNoteNum);
	int Read(const string& rFilePath);
	static CStandardNoteTableFile& GetInstance();
	virtual ~CStandardNoteTableFile();
private:
	int Clear();
	map<int,CStandardNoteType*> m_StandardNoteTypeMap;
};

#endif // !defined(AFX_STANDARDNOTETABLEFILE_H__D8DFB788_2E54_4404_BFD5_516A2C8C9A32__INCLUDED_)
