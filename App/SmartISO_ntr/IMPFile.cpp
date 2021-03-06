// IMPFile.cpp: implementation of the CIMPFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include "IMPFile.h"
#include "isstring.h"
#include <tokenizer.h>

#include <string>
#include <fstream>
#include <functional>
using namespace std;

/*
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
*/
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIMPFile::CIMPFile() : m_contentEntry(new vector<IMPContent>)
{
	m_rFilePath = new string;
}

CIMPFile::~CIMPFile()
{
	/// do not delete m_stringEntry
	try
	{
		delete m_rFilePath;
		delete (m_contentEntry);
		
	}
	catch(...)
	{
	}
}

/**
	@brief	DEF 파일의 내용을 읽는다.
	
	@author	BHK

	@date	2009.10.28

	@param

	@return
*/
bool CIMPFile::Read(const string& rFilePath)
{
	static bool bAlreadyRead = false;

	if(false == bAlreadyRead)
	{
		m_rFilePath->assign(rFilePath);

		string line;

		ifstream ifile(m_rFilePath->c_str());
		while(ifile.good())
		{
			getline(ifile , line);
			if(line.empty() || ('!' == line[0])) continue;	///< skip comment
			
			IMPContent content;
			string substring;
			for(int i = 0;i < int(line.length());++i)
			{
				if(' ' == line[i])
				{
					if(!substring.empty()) content.contents.push_back(substring);
					substring = "";
					while(' ' == line[++i]);
				}
				substring += line[i];
			}
			if(!substring.empty()) content.contents.push_back(substring);

			m_contentEntry->push_back(content);
			bAlreadyRead = true;
		}
	}

	return true;
}

namespace
{
	struct IsDependtionCode : public binary_function<IMPContent , string , bool>
	{
		bool operator()(const IMPContent& lhs , const string& rhs) const
		{
			string rDependtionCode = (lhs.contents.size() > 5) ? lhs.contents[5] : "";
			return (rDependtionCode == rhs);
		}
	};
};

bool CIMPFile::FindDependtionCode(const string &rDependtionCode)
{
	vector<IMPContent>::iterator where = find_if(m_contentEntry->begin() , m_contentEntry->end() , bind2nd(IsDependtionCode() , rDependtionCode));
	return (m_contentEntry->end() != where);
}

/**
	@brief	dependtion code를 가지는 item 항목을 구한다.

	@author	BHK

	@date	2009.10.28

	@param

	@date	
**/
bool CIMPFile::FindBMCSCodeWithDependtionCode(string& item , const string& rDependtionCode)
{
	vector<IMPContent>::iterator where = find_if(m_contentEntry->begin() , m_contentEntry->end() , bind2nd(IsDependtionCode() , rDependtionCode));
	if(m_contentEntry->end() != where)
	{
		item = where->contents[8];
		return true;
	}

	return false;
}

CIMPFile& CIMPFile::GetInstance()
{
	static CIMPFile __INSTANCE__;

	return __INSTANCE__;
}

/**
	@breif	

	@author	백흠경
**/
const char* CIMPFile::GetFilePath() const
{
	return (*m_rFilePath).c_str();
}
