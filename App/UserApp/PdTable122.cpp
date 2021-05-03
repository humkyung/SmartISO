// PdTable122.cpp: implementation of the CPdTable122 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "userapp.h"
#include "PdTable122.h"
#include <Tokenizer.h>
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
    @class      CPdTable122Record
    @function   Parse
    @return     int
    @param      soci::row&     
    @brief
******************************************************************************/
int CPdTable122Record::Parse(soci::row& row)
{
	const static STRING_T aryFieldNames[]=
	{ 
		_T("DWG_VIEW_NO") , _T("DWG_VIEW_SCALE") , _T("DWG_INDEX_NO") , 
		_T("DWG_VIEW_X_LOW") , _T("DWG_VIEW_Y_LOW") , _T("DWG_VIEW_Z_LOW") , 
		_T("DWG_VIEW_X_HIGH") , _T("DWG_VIEW_Y_HIGH") , _T("DWG_VIEW_Z_HIGH")
	};

	for(int i = 0;i < ARRAYSIZE(aryFieldNames);++i)
	{
		STRING_T sValue = row.get<STRING_T>(aryFieldNames[i]);
		switch(i)
		{
			case 0:
				m_dwg_view_no = sValue;
				break;
			case 1:
			{
				vector<STRING_T> oResult;
				CTokenizer<CIsFromString>::Tokenize(oResult, sValue, CIsFromString( _T(":") ));
				if(2 == oResult.size())
				{
					m_dwg_view_scale = ATOI_T(oResult[1].c_str());
				}
				else
				{
					m_dwg_view_scale = ATOI_T(sValue.c_str());
				}
			}
				break;
			case 2:
				m_dwg_index_no = sValue;
				break;
			case 3:
				m_dwg_view_x_low = ATOF_T(sValue.c_str());
				break;
			case 4:
				m_dwg_view_y_low = ATOF_T(sValue.c_str());
				break;
			case 5:
				m_dwg_view_z_low = ATOF_T(sValue.c_str());
			break;
			case 6:
				m_dwg_view_x_high = ATOF_T(sValue.c_str());
				break;
			case 7:
				m_dwg_view_y_high = ATOF_T(sValue.c_str());
				break;
			case 8:
				m_dwg_view_z_high = ATOF_T(sValue.c_str());
			break;
			default:break;
		}
	}

	return ERROR_SUCCESS;
}

CPdTable122::CPdTable122()
{

}

/**
	@brief	destructure

	@author	BAEK HUM KYUNG
*/
CPdTable122::~CPdTable122()
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
    @class      CPdTable122
    @function   Clear
    @return     int
    @brief
******************************************************************************/
int CPdTable122::Clear()
{
	for(vector<CPdTable122Record*>::iterator itr = m_RecordEntry.begin();itr != m_RecordEntry.end();++itr)
	{
		delete (*itr);
	}
	m_RecordEntry.clear();

	return ERROR_SUCCESS;
}

CPdTable122& CPdTable122::GetInstance(void)
{
	static CPdTable122 __instance__;

	return __instance__;
}

/******************************************************************************
    @author     humkyung
    @date       2012-08-29
    @class      CPdTable122
    @function   Read
    @return     int
    @param      CADODB& adoDB
    @brief
******************************************************************************/
int CPdTable122::Read(soci::session& aSession)
{
	using namespace soci;

	this->Clear();

	soci::rowset<soci::row> rs(aSession.prepare << "select * from pdtable_122");
	for(soci::rowset<row>::const_iterator itr = rs.begin();itr != rs.end();++itr)
	{
		CPdTable122Record* pRecord = new CPdTable122Record;
		if(pRecord)
		{
			pRecord->Parse(*itr);
			m_RecordEntry.push_back( pRecord );
		}
	}

	return ERROR_SUCCESS;
}

STRING_T CPdTable122Record::dwg_view_index_no() const
{
	return m_dwg_view_index_no;
}

STRING_T CPdTable122Record::dwg_view_no() const
{
	return m_dwg_view_no;
}

STRING_T CPdTable122Record::dwg_index_no() const
{
	return m_dwg_index_no;
}

/******************************************************************************
    @author     humkyung
    @date       ????-??-??
    @function   dwg_view_x_low
    @return     double
    @brief
******************************************************************************/
double CPdTable122Record::dwg_view_x_low() const
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	const double unit_x = 1.0;///atof(pDocData->GetWorkingUnit().c_str());
	const double dvcs_x = atof(pDocData->GetDVCSE().c_str());

	return ((m_dwg_view_x_low / unit_x) - dvcs_x)/* * 100.f*/;
}

/******************************************************************************
    @author     humkyung
    @date       ????-??-??
    @function   dwg_view_y_low
    @return     double
    @brief
******************************************************************************/
double CPdTable122Record::dwg_view_y_low() const
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	const double unit_y = 1.0;///atof(pDocData->GetWorkingUnit().c_str());
	const double dvcs_y = atof(pDocData->GetDVCSN().c_str());

	return ((m_dwg_view_y_low / unit_y) - dvcs_y)/* * 100.f*/;
}

/******************************************************************************
    @author     humkyung
    @date       ????-??-??
    @function   dwg_view_z_low
    @return     double
    @brief
******************************************************************************/
double CPdTable122Record::dwg_view_z_low() const
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	const double unit_z = 1.0;///atof(pDocData->GetWorkingUnit().c_str());
	const double dvcs_z = atof(pDocData->GetDVCSEL().c_str());

	return ((m_dwg_view_z_low / unit_z) - dvcs_z)/* * 100.f*/;
}

/******************************************************************************
    @author     humkyung
    @date       ????-??-??
    @function   dwg_view_x_high
    @return     double
    @brief
******************************************************************************/
double CPdTable122Record::dwg_view_x_high() const
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	const double unit_x = 1.0;///atof(pDocData->GetWorkingUnit().c_str());
	const double dvcs_x = atof(pDocData->GetDVCSE().c_str());

	return ((m_dwg_view_x_high / unit_x) - dvcs_x)/* * 100.f*/;
}

/******************************************************************************
    @author     humkyung
    @date       ????-??-??
    @function   dwg_view_y_high
    @return     double
    @brief
******************************************************************************/
double CPdTable122Record::dwg_view_y_high() const
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	const double unit_y = 1.0;///atof(pDocData->GetWorkingUnit().c_str());
	const double dvcs_y = atof(pDocData->GetDVCSN().c_str());

	return ((m_dwg_view_y_high / unit_y) - dvcs_y)/* * 100.f*/;
}

/******************************************************************************
    @author     humkyung
    @date       ????-??-??
    @function   dwg_view_z_high
    @return     double
    @brief
******************************************************************************/
double CPdTable122Record::dwg_view_z_high() const
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	const double unit_z = 1.0;///atof(pDocData->GetWorkingUnit().c_str());
	const double dvcs_z = atof(pDocData->GetDVCSEL().c_str());

	return ((m_dwg_view_z_high / unit_z) - dvcs_z)/* * 100.f*/;
}

int CPdTable122Record::dwg_view_scale() const
{
	return m_dwg_view_scale;
}

/*
	@brief	122 Table에서 dwg_index_no를 찾는다.

	@author	humkyung
*/
int CPdTable122::FindDwgIndexNo(set<STRING_T>& DwgIndexNoEntry , const double &x, const double &y, const double &z)
{
	DwgIndexNoEntry.clear();

	bool bInter = false;

	for(vector<CPdTable122Record*>::iterator itr = m_RecordEntry.begin(); itr != m_RecordEntry.end();++itr)
	{
		double xlow = (*itr)->dwg_view_x_low();
		double xhigh= (*itr)->dwg_view_x_high();
		if(xlow > xhigh) swap(xlow , xhigh);
		double ylow = (*itr)->dwg_view_y_low();
		double yhigh= (*itr)->dwg_view_y_high();
		if(ylow > yhigh) swap(ylow , yhigh);
		double zlow = (*itr)->dwg_view_z_low();
		double zhigh= (*itr)->dwg_view_z_high();
		if(zlow > zhigh) swap(zlow , zhigh);

		if((x  < xlow) || (x > xhigh))
		{
			bInter = false;
		}
		else if((y < ylow) || (y > yhigh))
		{
			bInter = false;
		}
		else if((z < zlow) || (z > zhigh))
		{
			bInter = false;
		}
		else 
		{
			bInter = true;
		}

		if( true == bInter)
		{
			DwgIndexNoEntry.insert((*itr)->dwg_index_no());
		}
	}

	return DwgIndexNoEntry.size();
}

