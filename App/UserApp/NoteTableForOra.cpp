// NoteTableForOra.cpp: implementation of the CNoteTableForOra class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UserApp.h"
#include "NoteTableForOra.h"
#include "isstring.h"

#include <functional>
#include <algorithm>
#include <fstream>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace std;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

struct FindNoteNodeUsingNum : public binary_function<CNoteTableForOra::NoteNode , int , bool>
{
	bool operator()(const CNoteTableForOra::NoteNode& lhs , const int& rhs) const
	{
		return (lhs.nNum == rhs);
	}
};

struct FindNoteNodeUsingStr : public binary_function<CNoteTableForOra::NoteNode , string , bool>
{
	bool operator()(const CNoteTableForOra::NoteNode& lhs , const string& rhs) const
	{
		string _lhs = lhs.rNumString;
		string _rhs = rhs;
		IsString::ToUpper(_lhs);
		IsString::ToUpper(_rhs);

		return (_lhs == _rhs);
	}
};

CNoteTableForOra::CNoteTableForOra()
{

}

CNoteTableForOra::~CNoteTableForOra()
{

}

void CNoteTableForOra::Add(const NoteNode &noteNode)
{
	m_noteNodes.push_back(noteNode);
}

/**	\brief	The CNoteTableForOra::Find function\n
	Code Number로 찾는다.

	\param	noteNode	a parameter of type NoteNode &
	\param	key	a parameter of type const int &

	\return	bool	
*/
bool CNoteTableForOra::Find(NoteNode &noteNode, const int &key)
{
	vector<NoteNode>::iterator where = find_if(m_noteNodes.begin() , m_noteNodes.end() , bind2nd(FindNoteNodeUsingNum() , key));
	if(m_noteNodes.end() != where)
	{
		noteNode = (*where);
		return true;
	}

	return false;
}

/**	\brief	The CNoteTableForOra::Find function\n
	Code String로 찾는다.

	\param	noteNode	a parameter of type NoteNode &
	\param	key	a parameter of type const string &

	\return	bool	
*/
bool CNoteTableForOra::Find(NoteNode &noteNode, const string &key)
{
	vector<NoteNode>::iterator where = find_if(m_noteNodes.begin() , m_noteNodes.end() , bind2nd(FindNoteNodeUsingStr() , key));
	if(m_noteNodes.end() != where)
	{
		noteNode = (*where);
		return true;
	}

	return false;
}

/**
	\author	백흠경
**/
void CNoteTableForOra::Read(ifstream &ifile)
{
	static const CString rStandardNoteTypeString("Standard Note Type");

	string aLine;
	while(ifile.good() && !ifile.eof())
	{
		
		int savefpos = ifile.tellg();
		getline(ifile, aLine);
		if(aLine.empty()) continue;
		if(aLine.length() < rStandardNoteTypeString.GetLength()) continue;
		if(-1 != aLine.find("")) continue;
		if(-1 != aLine.find(rStandardNoteTypeString))
		{
			ifile.seekg(savefpos);	
			break;
		}
		
		if(!aLine.empty())
		{
			NoteNode nNote;
			string a = aLine.substr(m_nFieldPos[2], m_nFieldPos[3] - m_nFieldPos[2]).c_str();
			CString rNum = a.c_str();		
			rNum.TrimLeft();
			rNum.TrimRight();

			string b = aLine.substr(m_nFieldPos[3], aLine.length() - m_nFieldPos[3]).c_str();
			string::size_type at = b.find("=");
			/// '=' 이전의 것을 구한다.
			b = (string::npos != at) ? b.substr(0 , at) : b;
			CString rString = b.c_str();
			rString.TrimLeft();
			rString.TrimRight();
			
			nNote.nNum = atoi(rNum);
			nNote.rNumString = rString;
			
			nNote.OutputDebugString();	///< 디버깅

			m_noteNodes.push_back(nNote);
		}
	}
}

/**
	\brief	Standard Note의 Type별 Header를 인식한다.
	\author	백흠경
**/
void CNoteTableForOra::ParseColumnIndex(const CString &rTextString)
{
	m_nFieldPos[0] = 0;
	string::size_type at = rTextString.Find("Standard Note Type");
	m_nFieldPos[1] = at;
	at = rTextString.Find("Number");
	m_nFieldPos[2] = at;
	at = rTextString.Find("Text");
	m_nFieldPos[3] = at;
}