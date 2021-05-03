// StandardNoteTableFile.cpp: implementation of the CStandardNoteTableFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "admin.h"
#include "StandardNoteTableFile.h"

#include <fstream>
using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/**
	@brief	Standard Note의 Type별 Header를 인식한다.

	@author	백흠경
**/
int CStandardNoteTableFile::CStandardNoteType::ParseColumnIndex(const CString &rTextString)
{
	m_nFieldPos[0] = 0;
	string::size_type at = rTextString.Find("Standard Note Type");
	m_nFieldPos[1] = at;
	at = rTextString.Find("Number");
	m_nFieldPos[2] = at;
	at = rTextString.Find("Text");
	m_nFieldPos[3] = at;

	return ERROR_SUCCESS;
}

int CStandardNoteTableFile::CStandardNoteType::Read(ifstream& ifile)
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

			m_noteNodes.push_back(nNote);
		}
	}

	return ERROR_SUCCESS;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStandardNoteTableFile::CStandardNoteTableFile()
{

}

CStandardNoteTableFile::~CStandardNoteTableFile()
{
	try
	{
		Clear();
	}
	catch(...)
	{
	}
}

CStandardNoteTableFile& CStandardNoteTableFile::GetInstance()
{
	static CStandardNoteTableFile __instance__;

	return __instance__;
}

/**
	@brief	

	@author	BHK
*/
int CStandardNoteTableFile::Read(const string &rFilePath)
{
	Clear();

	string rStdFilePath = rFilePath;
	ifstream ifile;
	ifile.open(rFilePath.c_str());
	if(!ifile.is_open())	return ERROR_FILE_NOT_FOUND;

	string aLine;
	while(ifile.good() && !ifile.eof())
	{
		getline(ifile, aLine);
		if(ifile.eof()) break;

		string rFindName;
		if(-1 != aLine.find("Standard Note Type"))
		{
			CStandardNoteTableFile::CStandardNoteType* pNoteType = NULL;
			
			int at = aLine.find("Standard Note Type");
			const int len = aLine.length();
			int temp = len - ( at + strlen("Standard Note Type"));
			rFindName = aLine.substr((at + strlen("Standard Note Type")) , temp);
			int nNoteType = 0;
			sscanf(rFindName.c_str(), "%d" ,&nNoteType);
			
			map<int , CStandardNoteType*>::iterator where = m_StandardNoteTypeMap.find(nNoteType);
			if(where == m_StandardNoteTypeMap.end())
			{
				if(2000 == nNoteType)
				{
					int d = 1;
				}

				pNoteType = new CStandardNoteType;
				if(pNoteType)
				{
					CString rTextString = aLine.c_str();
					pNoteType->ParseColumnIndex(rTextString);
					pNoteType->Read(ifile);
					m_StandardNoteTypeMap[nNoteType] = pNoteType;
				}
			}
			else
			{
				pNoteType = where->second;
				pNoteType->Read(ifile);
			}
		}
	}
}

CStandardNoteTableFile::CStandardNoteType* CStandardNoteTableFile::GetStandardNoteType(const int& nNoteNum)
{
	map<int,CStandardNoteType*>::iterator where = m_StandardNoteTypeMap.find(nNoteNum);
	if(where != m_StandardNoteTypeMap.end()) return where->second;

	return NULL;
}

int CStandardNoteTableFile::Clear()
{
	for(map<int,CStandardNoteType*>::iterator itr = m_StandardNoteTypeMap.begin();itr != m_StandardNoteTypeMap.end();++itr)
	{
		delete itr->second;
		itr->second = NULL;
	}

	m_StandardNoteTypeMap.clear();

	return ERROR_SUCCESS;
}
