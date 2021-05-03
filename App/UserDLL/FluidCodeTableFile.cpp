// FluidCodeTableFile.cpp: implementation of the CFluidCodeTableFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UserDLL.h"
#include "FluidCodeTableFile.h"
#include "tokenizer.h"
#include "isstring.h"

#include <fstream>
#include <vector>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFluidCodeTableFile::CFluidCodeTableFile()
{
	m_pFilePath = new STRING_T;
	m_pFluidCodeMap = new map<int , STRING_T>;
}

CFluidCodeTableFile::~CFluidCodeTableFile()
{
	try
	{
		if(NULL != m_pFilePath) delete m_pFilePath;

		if(NULL != m_pFluidCodeMap)
		{
			ClearContents();
			delete m_pFluidCodeMap;
		}
	}
	catch(...)
	{
	}
}

/**	\brief	The CFluidCodeTableFile::Read function

	\param	rFilePath	a parameter of type const string &

	\return	bool	
*/
bool CFluidCodeTableFile::Read(const STRING_T &rFilePath)
{
	if((*m_pFilePath) == rFilePath) return true;
	(*m_pFilePath) = rFilePath;
	
	ClearContents();

	bool isFirstLine = true;
	STRING_T line;
	
	IFSTREAM_T ifile;
	ifile.open(rFilePath.c_str());
	vector<STRING_T> oResult;
	while(ifile.good() && !ifile.eof())
	{
		getline(ifile,line);
		if(true == isFirstLine)
		{
			isFirstLine = false;
			continue; ///< skip first line
		}
		if(';' == line[0]) continue; ///< skip comment line

		STRING_T rFluidCode , rFluidCodeString;
		CTokenizer<CIsFromString>::Tokenize(oResult , line , CIsFromString( _T("=") ));
		if(2 == oResult.size())
		{
			rFluidCode       = oResult[0];
			rFluidCodeString = oResult[1];
			
			IsString::TrimWhiteSpace(rFluidCode);
			IsString::TrimWhiteSpace(rFluidCodeString);
			const int nFluidCode = ATOI_T(rFluidCode.c_str());
			(*m_pFluidCodeMap)[nFluidCode] = rFluidCodeString;
		}
	}
	ifile.close();
	
	return true;
}

/**	\brief	The CFluidCodeTableFile::ClearContents function\n
	내용을 삭제한다.


	\return	void	
*/
void CFluidCodeTableFile::ClearContents()
{
	if(m_pFluidCodeMap) m_pFluidCodeMap->clear();
}

/**	\brief	The CFluidCodeTableFile::FindFluidCodeString function\n
	Fluid Code Number에 해당하는 Fluid Code String을 찾는다.

	\param	rFluidCodeString	a parameter of type string&
	\param	nFluidCode	a parameter of type const int &

	\return	true - 찾았음, false - 해당 Fluid Code String이 존재하지 않음.	
*/
bool CFluidCodeTableFile::FindFluidCodeString(STRING_T& rFluidCodeString , const int &nFluidCode)
{
	map<int , STRING_T>::iterator where = m_pFluidCodeMap->find(nFluidCode);
	if(m_pFluidCodeMap->end() != where)
	{
		rFluidCodeString = (where->second);
		return true;
	}

	return false;
}
