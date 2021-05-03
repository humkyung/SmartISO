// OraDatabaseDefFile.cpp: implementation of the COraDatabaseDefFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UserDLL.h"
#include "OraDatabaseDefFile.h"
#include "tokenizer.h"
#include "isstring.h"

#include <fstream>
#include <string>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COraDatabaseDefFile::COraDatabaseDefFile()
{
	m_pFilePath = new STRING_T;
	m_pFieldEntry = new vector<CFieldDef*>;
}

COraDatabaseDefFile::~COraDatabaseDefFile()
{
	try
	{
		if(NULL != m_pFilePath) delete m_pFilePath;
		if(NULL != m_pFieldEntry)
		{
			ClearContents();
			delete m_pFieldEntry;
		}
	}
	catch(...)
	{
	}
}

/**	
	@brief	oracle database definition file을 읽는다.

	@author	HumKyung

	@param	rFilePath	a parameter of type const string &

	@remarks	우선 12번 테이블만 읽도록 한다.

	@return	bool	
*/
bool COraDatabaseDefFile::Read(const STRING_T &rFilePath)
{
	if((*m_pFilePath) == rFilePath) return true;
	(*m_pFilePath) = rFilePath;

	ClearContents();

	IFSTREAM_T ifile;
	STRING_T line;
	ifile.open(rFilePath.c_str());
	if(ifile.is_open())
	{
		vector<STRING_T> oColNum, oTemp, oResult;
		while(ifile.good() && !ifile.eof())
		{
			getline(ifile,line);
			//! skip empty or start with '#' character line
			if(line.empty() || ('#' == line[0])) continue;

			if(STRING_T::npos != line.find( _T("table number") ))
			{
				int iTableNumber = 0 , iColumnCount = 0;
				if(ERROR_SUCCESS != ParseTableNumber(iTableNumber , iColumnCount , line)) continue;

				//! parse only 12 table
				if(12 == iTableNumber)
				{
					for(int count = 0; count < iColumnCount;)
					{
						getline(ifile,line);
						if(line.empty()) continue;
						
						CTokenizer<CIsComma>::Tokenize(oTemp , line , CIsComma());
						if(oTemp.size() >= 3)
						{
							CFieldDef* p = new CFieldDef;
							p->m_rFieldName = oTemp[1];
							IsString::TrimWhiteSpace(p->m_rFieldName);
							p->m_rFieldType = oTemp[2];
							IsString::TrimWhiteSpace(p->m_rFieldType);

							if(4 == oTemp.size())
							{
								p->m_rNote = oTemp[3];
								IsString::TrimWhiteSpace(p->m_rNote);
							}

							m_pFieldEntry->push_back(p);
						}
						
						++count;
					}
					break;
				}
			}
		}
		ifile.close();
	}

	return true;
}

size_t COraDatabaseDefFile::GetFieldCount() const
{
	return m_pFieldEntry->size();
}

/**	
	@brief	The COraDatabaseDefFile::GetFieldAt function

	@param	def	a parameter of type FieldDef &
	@param	at	a parameter of type const size_t &

	@return	bool	
*/
bool COraDatabaseDefFile::GetFieldAt(COraDatabaseDefFile::CFieldDef &def, const size_t &at)
{
	if(at < m_pFieldEntry->size())
	{
		def = *((*m_pFieldEntry)[at]);
		return true;
	}

	return false;
}

/**	
	@brief	내용을 삭제한다.

	@author	humkyung

	@return	void	
*/
void COraDatabaseDefFile::ClearContents()
{
	if(NULL != m_pFieldEntry)
	{
		for(vector<CFieldDef*>::iterator itr = m_pFieldEntry->begin();itr != m_pFieldEntry->end();++itr)
		{
			delete (*itr);
		}
		m_pFieldEntry->clear();
	}
}

/**	
	@brief	12번 테이블의 필드 이름들을 리턴한다.

	@author	humkyung

	@param	rString	a parameter of type CString&

	@return	bool	
*/
bool COraDatabaseDefFile::GetAllFieldString(CString& rString) const
{
	rString.Empty();

	for(vector<CFieldDef*>::const_iterator itr = m_pFieldEntry->begin();itr != m_pFieldEntry->end();++itr)
	{
		rString += (*itr)->m_rFieldName.c_str() + CString( _T(",") );
	}

	rString.TrimRight( _T(",") );

	return true;
}

/**	
	@brief	필드 이름으로 필드를 찾는다.

	@author	humkyung

	@param	defField	a parameter of type COraDatabaseDefFile::CFieldDef&
	@param	rFieldName	a parameter of type const string&

	@return	bool	
*/
bool COraDatabaseDefFile::GetOraDefField(COraDatabaseDefFile::CFieldDef& defField , const STRING_T& rFieldName)
{
	STRING_T rhs = rFieldName;
	IsString::ToUpper(rhs);

	if( m_pFieldEntry && !m_pFieldEntry->empty() )
	{
		for( vector<CFieldDef*>::iterator itr = m_pFieldEntry->begin();itr != m_pFieldEntry->end();++itr )
		{
			STRING_T lhs = (*itr)->m_rFieldName;
			IsString::ToUpper(lhs);
			
			if(lhs == rhs)
			{
				defField = *(*itr);
				return true;
			}
		}
	}

	return false;
}

/**	
	@brief	parse table number

	@author	HumKyung

	@date	2011.01.04

	@param	line	- a line from file

	@return	bool	

	@TODO : try to use regular expression
*/
int COraDatabaseDefFile::ParseTableNumber(int& iTableNumber , int& iColumnCount , const STRING_T& line) const
{
	iTableNumber = iColumnCount = 0;

	vector<STRING_T> oTemp , oResult;
	CTokenizer<CIsComma>::Tokenize(oTemp , line , CIsComma());
	if(2 == oTemp.size())
	{
		IsString::TrimWhiteSpace(oTemp[0]);
		IsString::TrimWhiteSpace(oTemp[1]);

		CTokenizer<CIsFromString>::Tokenize(oResult , oTemp[0] , CIsFromString(_T("=")));
		if(2 == oResult.size())
		{
			IsString::TrimWhiteSpace(oResult[0]);
			IsString::TrimWhiteSpace(oResult[1]);

			if(_T("table number") == oResult[0])
			{
				iTableNumber = atoi(oResult[1].c_str());
			}
		}

		CTokenizer<CIsFromString>::Tokenize(oResult , oTemp[1] , CIsFromString(_T("=")));
		if(2 == oResult.size())
		{
			IsString::TrimWhiteSpace(oResult[0]);
			IsString::TrimWhiteSpace(oResult[1]);

			if(_T("number of columns") == oResult[0])
			{
				iColumnCount = atoi(oResult[1].c_str());
			}
		}

		return ERROR_SUCCESS;
	}

	return ERROR_BAD_ENVIRONMENT;
}

/**
	@brief	check if str has alphabet

	@author	humkyung

	@date	2013.06.11
*/
bool COraDatabaseDefFile::CFieldDef::HasAlphaCharacter(const STRING_T& str) const
{
	const int count = str.length();
	for(int i=0; i < count; ++i)
	{
		if(isalpha(str[i])) return true;
	}
	return false;
}

/**
	@brief	convert value based on data type

	@author	humkyung

	@date	2013.06.11
*/
STRING_T COraDatabaseDefFile::CFieldDef::ConvertValue(const STRING_T& sValue)
{
	STRING_T res = sValue;

	/// string type field
	if((STRING_T::npos != m_rFieldType.find( _T("character") )) || (STRING_T::npos != m_rFieldType.find( _T("upper_case") )))
	{
		res = _T("''") + sValue + _T("''");									/// 문자열 형식은 앞뒤로 '를 붙인다.
	}
	else if(STRING_T::npos != m_rFieldType.find( _T("double") ))	/// 숫자 형식의 field
	{
		/// 숫자 형식만 받아들이도록 한다.
		/// 비어있거나 '-'이거나 알파벳일 경우에는 0으로 처리한다.
		if( sValue.empty() || (_T("-") == sValue) || HasAlphaCharacter(sValue)) res = _T("0");

		STRING_T::size_type at = sValue.find( _T("/") );
		if(-1 != at)
		{
			res = sValue.substr(0 , at);
		}
	}

	return res;
}