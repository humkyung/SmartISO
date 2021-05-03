// PdTable121.cpp: implementation of the CPdTable121 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "userapp.h"
#include "PdTable121.h"
#include <string>
#include <tokenizer.h>
#include <IsString.h>
#include "UserAppDocData.h"

using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/******************************************************************************
    @author     humkyung
    @date       2012-08-29
    @class      CPdTable121Record
    @function   Parse
    @return     int
    @param      CADODB&     adoDB
    @param      const       int&
    @param      iRecordNo
    @brief
******************************************************************************/
int CPdTable121Record::Parse(soci::row& row)
{
	const static STRING_T aryFieldNames[]=
	{ 
		_T("DWG_INDEX_NO") , _T("DRAWING_NO") , _T("DRAWING_TITLE") , _T("DRAWING_FILE_SPEC") , _T("PATH_NAME") , _T("NETWORK_ADDRESS") , 
		_T("DRAWING_TYPE")
	};

	for(int i = 0;i < ARRAYSIZE(aryFieldNames);++i)
	{
		STRING_T sValue = row.get<std::string>(aryFieldNames[i]);
		switch(i)
		{
			case 0:
				m_dwg_index_no = sValue;
				break;
			case 1:
				m_drawing_no = sValue;
				break;
			case 2:
				m_drawing_title = sValue;
				break;
			case 3:
				m_drawing_file_spec = sValue;
				break;
			case 4:
				m_path_name = sValue;
				break;
			case 5:
				m_network_address = sValue;
				break;
			case 6:
				m_drawing_type = sValue;
			break;
			default:break;
		}
	}

	return ERROR_SUCCESS;
}

CPdTable121::CPdTable121()
{

}

CPdTable121& CPdTable121::GetInstance(void)
{
	static CPdTable121 __instance__;

	return __instance__;
}

/**
	@brief	destructure
	@author	BAEK HUM KYUNG
*/
CPdTable121::~CPdTable121()
{
	try
	{
		Clear();
	}
	catch(...)
	{
	}
}

/******************************************************************************
    @author     humkyung
    @date       2012-08-29
    @class      CPdTable121
    @function   Clear
    @return     int
    @brief
******************************************************************************/
int CPdTable121::Clear()
{
	for(vector<CPdTable121Record*>::iterator itr = m_RecordEntry.begin();itr != m_RecordEntry.end();++itr)
	{
		SAFE_DELETE(*itr);
	}
	m_RecordEntry.clear();

	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2012-08-29
    @class      CPdTable121
    @function   Read
    @return     int
    @param      CADODB& adoDB
    @brief
******************************************************************************/
int CPdTable121::Read(soci::session& aSession)
{
	using namespace soci;

	this->Clear();

	soci::rowset<soci::row> rs(aSession.prepare << "select * from pdtable_121");
	for(soci::rowset<row>::const_iterator itr = rs.begin();itr != rs.end();++itr)
	{
		CPdTable121Record* pRecord = new CPdTable121Record;
		if(pRecord)
		{
			pRecord->Parse(*itr);
			m_RecordEntry.push_back( pRecord );
		}
	}

	return ERROR_SUCCESS;
}

STRING_T CPdTable121Record::dwg_index_no() const
{
	return m_dwg_index_no;

}

STRING_T CPdTable121Record::drawing_no() const
{
	return m_drawing_no;
}

STRING_T CPdTable121Record::drawing_title() const
{
	return m_drawing_title;
}

STRING_T CPdTable121Record::drawing_file_spec() const
{
	return m_drawing_file_spec;
}

STRING_T CPdTable121Record::path_name() const
{
	return m_path_name;
}

STRING_T CPdTable121Record::network_address() const
{
	return m_network_address;
}

/*
	@brief	dwg_index_no로 drawing_no를 찾아온다.

	@author	humkyung
*/
int CPdTable121::FindDrawingNo(set<STRING_T>& DrawingNoEntry, set<STRING_T>& DwgIndexNoEntry)
{
	DrawingNoEntry.clear();

	for(set<STRING_T>::iterator itr = DwgIndexNoEntry.begin(); itr != DwgIndexNoEntry.end(); ++itr)
	{
		for(vector<CPdTable121Record*>::iterator jtr = m_RecordEntry.begin(); jtr != m_RecordEntry.end();++jtr)
		{
			if((*jtr)->dwg_index_no() == (*itr).data())
			{
				DrawingNoEntry.insert((*jtr)->drawing_no());
			}
		}
	}
	return DrawingNoEntry.size();
}

/*
	@brief	dwg_index_no로 drawing_title를 찾아온다.

	@author	BHK
*/
int CPdTable121::FindDrawingTitle(set<STRING_T>& DrawingTitleEntry, set<STRING_T>& DwgIndexNoEntry)
{
	DrawingTitleEntry.clear();

	CAppDocData* pDocData = CAppDocData::GetInstance();

	for(set<string>::iterator itr = DwgIndexNoEntry.begin(); itr != DwgIndexNoEntry.end(); ++itr)
	{
		for(vector<CPdTable121Record*>::iterator jtr = m_RecordEntry.begin(); jtr != m_RecordEntry.end();++jtr)
		{
			if(pDocData->drawing_type() != (*jtr)->drawing_type()) continue;

			if((*jtr)->dwg_index_no() == (*itr).data())
			{
				DrawingTitleEntry.insert((*jtr)->drawing_title());
			}
		}
	}
	return DrawingTitleEntry.size();
}