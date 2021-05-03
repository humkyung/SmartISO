// PCDFile.cpp: implementation of the CPCDFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include "UserDLL.h"
#include "PCDFile.h"
#include <isstring.h>
#include <tokenizer.h>

#include <string>
#include <fstream>
#include <functional>
using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPCDFile::CPCDFile() : m_contentEntry(new vector<PCDContent>) , m_spec(new STRING_T)
{
	m_rFilePath = new STRING_T;
}

CPCDFile::~CPCDFile()
{
	///NOTICE: do not delete m_stringEntry
	try
	{
		delete m_rFilePath;
		delete (m_contentEntry);
		delete (m_spec);
	}
	catch(...)
	{
	}
}

/**
	@brief	PCD 파일의 내용을 읽는다.
	@author	humkyung
*/
bool CPCDFile::Read(const STRING_T& rFilePath)
{
	m_rFilePath->assign(rFilePath);

	STRING_T line;

	IFSTREAM_T ifile(m_rFilePath->c_str());
	while(ifile.good())
	{
		getline(ifile , line);
		if(line.size() < 76) continue;
		if('!' == line[0]) continue;	///< skip comment

		//IsString::TrimWhiteSpace(line);
		string::size_type at = line.find( _T("PIPING_MATERIALS_CLASS") );
		if(string::npos != at)
		{
			vector<STRING_T> oResult;
			CTokenizer<CIsFromString>::Tokenize(oResult , line.c_str() , CIsFromString( _T("=") ));
			if(2 == oResult.size())
			{
				(*m_spec) = oResult[1];
				IsString::TrimWhiteSpace(*m_spec);
			}
			continue;
		}
		at = line.find( _T("Single_Spacing") );
		if(string::npos != at) continue;
		at = line.find( _T("Order") );
		if(string::npos != at) continue;
		
		if(line.size() > 129)
		{
			PCDContent content;

			content.prp[0].assign(line.substr(26 , 4));
			IsString::TrimWhiteSpace(content.prp[0]);
			content.prp[1].assign(line.substr(58 , 4));
			IsString::TrimWhiteSpace(content.prp[1]);

			content.matl_code.assign(line.substr(78 , 15));
			IsString::TrimWhiteSpace(content.matl_code);
			if(line.length() > 132) content.item.assign(line.substr(132 , 7));///< pcd File의 자릿수가 늘어났다. 128->132
			IsString::TrimWhiteSpace(content.item);
			
			m_contentEntry->push_back(content);
		}
	}

	return true;
}

namespace
{
	struct IsMatlCode : public binary_function<PCDContent , STRING_T , bool>
	{
		bool operator()(const PCDContent& lhs , const STRING_T& rhs) const
		{
			string rMatlCode = lhs.matl_code;
			if('*' == rMatlCode[0])
			{
				rMatlCode = rMatlCode.substr(1 , rMatlCode.length() - 1);
			}
			return (rMatlCode == rhs);
		}
	};
};

bool CPCDFile::FindMatlCode(const STRING_T &rMatlCode)
{
	vector<PCDContent>::iterator where = find_if(m_contentEntry->begin() , m_contentEntry->end() , bind2nd(IsMatlCode() , rMatlCode));
	return (m_contentEntry->end() != where);
}

/**
	@brief	material code를 가지는 item 항목을 구한다.

	@author	humkyung
**/
bool CPCDFile::FindItemWithMatlCode(STRING_T& item , const STRING_T& rMatlCode)
{
	vector<PCDContent>::iterator where = find_if(m_contentEntry->begin() , m_contentEntry->end() , bind2nd(IsMatlCode() , rMatlCode));
	if(m_contentEntry->end() != where)
	{
		item = where->item;
		return true;
	}

	return false;
}

const CHAR_T* CPCDFile::GetSpec() const
{
	return m_spec->c_str();
}

CPCDFile* CPCDFile::CreateInstance()
{
	return new CPCDFile;
}

void CPCDFile::DeleteInstance(CPCDFile* p)
{
	assert(p && "p is NULL");
	
	if(p) delete p;
}

/**	
	@brief	먼저 rMatlCode , rItem과 일치하는 항목을 구한다.
	항목의 property값으로 Welding Type을 구한다.

	@author	humkyung

	@param	rSpec	a parameter of type const string &
	@param	rMatlCode	a parameter of type const string &
	@param	rItem	a parameter of type const string &

	@return	Welding Type
*/
const CHAR_T* CPCDFile::GetWeldType(const STRING_T &rMatlCode, const STRING_T &rItem , const STRING_T& sWeldTypeFilePath)
{
	static STRING_T _rWeldType;
	//static const CString TW_CODES[] = { _T("111") , _T("121") , _T("131")};
	CUserDocData& docData = CUserDocData::GetInstance();

	_rWeldType = _T("");
	for (vector<PCDContent>::iterator itr = m_contentEntry->begin();itr != m_contentEntry->end();++itr)
	{
		if ( (*itr).matl_code.c_str() == rMatlCode )
		{
			if ( (*itr).item.c_str() == rItem )
			{ 
				_rWeldType = docData.FindWeldTypeOf((*itr).prp[0] , sWeldTypeFilePath);
				_rWeldType+= _T(",") + docData.FindWeldTypeOf((*itr).prp[1] , sWeldTypeFilePath);				
				
				return _rWeldType.c_str();
			}
		}
	}

	return _rWeldType.c_str();
}

/**
	@breif	

	@author	humkyung
**/
const char* CPCDFile::GetFilePath() const
{
	return (*m_rFilePath).c_str();
}
