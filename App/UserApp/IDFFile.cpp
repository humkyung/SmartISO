// IDFFile.cpp: implementation of the CIDFFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "userapp.h"
#include "IDFFile.h"
#include <Tokenizer.h>
#include <IsString.h>
#include <iostream>

using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIDFFile& CIDFFile::GetInstance(void)
{
	static CIDFFile __instance__;

	return __instance__;
}

CIDFFile::CIDFFile()
{

}

CIDFFile::~CIDFFile()
{
	try
	{
		Clear();
	}
	catch(...)
	{
	}

}

/*
	@brief	������ �о index�� 100�� �������� �߰��ϸ� x, y, z ��ǥ�� �Ľ��Ѵ�.

	@author	������
*/
int CIDFFileRecord::Parse(const string &aLine)
{
	static const int _FILE_POS_[]=
	{
		7,20,33
	};

	int nSCALE = 26;
	if(!aLine.empty() && (aLine.size() > 33))
	{
		string aToken;
	
		aToken = aLine.substr(0, 4);
		IsString::TrimWhiteSpace(aToken);
		m_nRecordIndex = atoi(aToken.c_str());
		
		aToken = aLine.substr(_FILE_POS_[0] , 11);
		IsString::TrimWhiteSpace(aToken);
		m_start_X = atof(aToken.c_str());
		
		aToken = aLine.substr(_FILE_POS_[1] , 11);
		IsString::TrimWhiteSpace(aToken);
		m_start_Y = atof(aToken.c_str());
		
		aToken = aLine.substr(_FILE_POS_[2] , 11);
		IsString::TrimWhiteSpace(aToken);
		m_start_Z = atof(aToken.c_str());
	}
	
	return ERROR_SUCCESS;
	
}

/*
	@brief	.IDF ������ �о�´�.

	@author	humkyung
*/
int CIDFFile::Read(const string &rFilePath)
{
	Clear();

	ifstream ifile(rFilePath.c_str());
	
	if(ifile.is_open())
	{
		STRING_T aLine, aToken;
		vector<string> oResult;
		CIDFFileRecord *pRecord = NULL;
		while(ifile.good() && !ifile.eof())
		{
			getline(ifile, aLine);
			if(aLine.empty()) continue;
			
			aToken = aLine.substr(0, 4);
			IsString::TrimWhiteSpace(aToken);
			const int nItemCode = atoi(aToken.c_str());
			if(100 == nItemCode)				/// PIPE
			{
				pRecord = new CIDFFileRecord;
				if(pRecord)
				{
					pRecord->Parse( aLine );
					m_RecordEntry.push_back( pRecord );
				}	
			}
			else if((35 == nItemCode) || (36 == nItemCode))	//! ELBO
			{
				pRecord = new CIDFFileRecord;
				if(pRecord)
				{
					pRecord->Parse( aLine );
					m_RecordEntry.push_back( pRecord );
				}
			}
			else if((45 == nItemCode) || (46 == nItemCode) || (47 == nItemCode))	//! TERF
			{
				pRecord = new CIDFFileRecord;
				if(pRecord)
				{
					pRecord->Parse( aLine );
					m_RecordEntry.push_back( pRecord );
				}
			}
			else if(105 == nItemCode)	//! FLWN
			{
				pRecord = new CIDFFileRecord;
				if(pRecord)
				{
					pRecord->Parse( aLine );
					m_RecordEntry.push_back( pRecord );
				}
			}
			else if(107 == nItemCode)	//! FLBL
			{
				pRecord = new CIDFFileRecord;
				if(pRecord)
				{
					pRecord->Parse( aLine );
					m_RecordEntry.push_back( pRecord );
				}
			}
		}
	}

	return ERROR_BAD_ENVIRONMENT;
	
}

int CIDFFileRecord::RecordIndex(void)
{
	return m_nRecordIndex;
}

double CIDFFileRecord::start_X(void)
{
	return m_start_X;
}

double CIDFFileRecord::start_Y(void)
{
	return m_start_Y;
}

double CIDFFileRecord::start_Z(void)
{
	return m_start_Z;
}

/*
	@brief	IDF ���Ͽ��� x,y,z��ǥ�� �Ѱ��ְ� dwg_index_no�� ã�´�. 

	@author	������
*/
int CIDFFile::GetCoodinateList(vector<CIsPoint3d>& Point3dList)
{
	Point3dList.clear();
	if(!m_RecordEntry.empty())
	{
		for(vector<CIDFFileRecord*>::iterator itr = m_RecordEntry.begin(); itr != m_RecordEntry.end(); ++itr)
		{
			const double x = (*itr)->start_X();
			const double y = (*itr)->start_Y();
			const double z = (*itr)->start_Z();

			CIsPoint3d pt3d(x,y,z);
			Point3dList.push_back(pt3d);
		}
		return ERROR_SUCCESS;
	}

	return ERROR_BAD_ENVIRONMENT;
}

int CIDFFile::GetIDFFileTableRecordSize()
{
	return m_RecordEntry.size();

}

/**
	@brief	������ �о��� ������ Ŭ�����Ѵ�.

	@author	BHK
*/
int CIDFFile::Clear()
{
	for(vector<CIDFFileRecord*>::iterator itr = m_RecordEntry.begin();itr != m_RecordEntry.end();++itr)
	{
		delete (*itr);
	}
	m_RecordEntry.clear();

	return ERROR_SUCCESS;
}
