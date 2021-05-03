// PDSArea.cpp: implementation of the CPDSArea class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainFrm.h"
#include "SmartReporterDoc.h"
#include "SmartReporter.h"
#include "PDSArea.h"

#include "LineListToOraDocData.h"
#include "IsString.h"

#include <fstream>
using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPDSArea::CPDSArea()
{

}

CPDSArea::~CPDSArea()
{

}

CPDSArea::CPDSArea(const CPDSArea& rhs)
{
	(*this) = rhs;
}


/**
**/
CPDSArea& CPDSArea::operator=(const CPDSArea& rhs)
{
	if(this != &rhs)
	{
		m_rName = rhs.m_rName;

		m_ModelEntry.clear();
		if(!rhs.m_ModelEntry.empty())
		{
			for(vector<CPDSModel>::const_iterator itr = rhs.m_ModelEntry.begin();itr != rhs.m_ModelEntry.end();++itr)
			{
				m_ModelEntry.push_back((*itr));
			}
		}
	}

	return (*this);
}

/**	
	@brief	get area from oracle

	@author	humkyung

	@date	????.??.??

	@param	

	@return
**/
void CPDSArea::QueryAreaName(vector<CPDSArea>& AreaNameEntry , CAdoRemoteDB& RemoteDB , const CString& rProjectName , const CString& rDiscipline)
{
	CMainFrame* pFrame = static_cast<CMainFrame*>(AfxGetMainWnd());

	AreaNameEntry.clear();

#ifdef _DEBUG
	CPDSArea area;

	area.m_rName = _T("A");
	AreaNameEntry.push_back(area);

	area.m_rName = _T("B");
	AreaNameEntry.push_back(area);

	area.m_rName = _T("C");
	AreaNameEntry.push_back(area);
#else
	//! 2011.05.02 added by humkyung
	STRING_T sSqlFilePath = STRING_T(GetAppDataPath().operator LPCTSTR()) + _T("\\SmartISO\\SmartReporter\\Area.sql");
	STRING_T sSql = _T("");
	IFSTREAM_T ifile(sSqlFilePath.c_str());
	if( ifile.is_open() )
	{
		STRING_T aLine;
		while(!ifile.eof())
		{
			getline(ifile , aLine);
			sSql += aLine;
		}
		
		ifile.close();

		IsString::ReplaceOf(sSql , _T("{PROJECTNAME}") , rProjectName.operator LPCTSTR());
		IsString::ReplaceOf(sSql , _T("{DISCIPLINE}") , rDiscipline.operator LPCTSTR());
	}
	
	/*
	CString rSQL = "SELECT ";
	rSQL += "DISTINCT ";
	rSQL += "P112.AREA_NAME ";
	rSQL += "FROM ";
	rSQL += "PD_" + rProjectName + ".PDTABLE_113 P113,";
	rSQL += "PD_" + rProjectName + ".PDTABLE_112 P112 ";
	rSQL += "WHERE ";
	rSQL += "P113.AREA_INDEX_NO=P112.AREA_INDEX_NO AND ";
	rSQL += "P112.DISCIPLINE_INDX_NO=" + rDiscipline;
	*/

	try
	{
		if(RemoteDB.OpenQuery(sSql.c_str()))
		{
			LONG lRecordCount = 0;
			RemoteDB.GetRecordCount(&lRecordCount);
			for(int i = 0;i < lRecordCount;++i)
			{			
				CPDSArea area;

				STRING_T rFieldName = RemoteDB.GetFieldNameAt(0);	///< FIELD 이름을 구한다.
				STRING_T rValue;
				RemoteDB.GetFieldValue(i , rFieldName , &rValue);	///< FIELD 값을 구한다.
				area.m_rName = rValue.c_str();
				
				AreaNameEntry.push_back(area);
			}
		}
		else
		{
			AfxMessageBox( _T("OpenQuery Error") );
		}
	}
	catch(const exception& ex)
	{
		AfxMessageBox( CString(ex.what()) );
	}
#endif
}


/**	
	@brief	get model from oracle

	@author	humkyung

	@date	????.??.??

	@param

	@return
**/
void CPDSArea::QueryModelName(CAdoRemoteDB &RemoteDB, const CString &rProjectName, const CString &rDiscipline)
{
	CMainFrame* pFrame = static_cast<CMainFrame*>(AfxGetMainWnd());
	m_ModelEntry.clear();

	//! 2011.05.02 added by humkyung
	STRING_T sSqlFilePath = STRING_T(GetAppDataPath().operator LPCTSTR()) + _T("\\SmartISO\\SmartReporter\\Model.sql");
	STRING_T sSql = _T("");
	IFSTREAM_T ifile(sSqlFilePath.c_str());
	if( ifile.is_open() )
	{
		STRING_T aLine;
		while(!ifile.eof())
		{
			getline(ifile , aLine);
			sSql += aLine;
		}
		
		ifile.close();

		IsString::ReplaceOf(sSql , _T("{PROJECTNAME}") , rProjectName.operator LPCTSTR());
		IsString::ReplaceOf(sSql , _T("{AREANAME}") , m_rName.operator LPCTSTR());
		IsString::ReplaceOf(sSql , _T("{DISCIPLINE}") , rDiscipline.operator LPCTSTR());
	}

	/*
	CString rSQL="SELECT ";
	rSQL += "DISTINCT ";
	rSQL += "P112.AREA_NAME,";
	rSQL += "P113.MODEL_NO ";
	rSQL += "FROM ";
	rSQL += "PD_" + rProjectName + ".PDTABLE_113 P113,";
	rSQL += "PD_" + rProjectName + ".PDTABLE_112 P112 ";
	rSQL += "WHERE ";
	rSQL += "P113.AREA_INDEX_NO=P112.AREA_INDEX_NO AND ";
	rSQL += "P112.AREA_NAME='" + m_rName +"' AND ";
	rSQL += "P112.DISCIPLINE_INDX_NO=" + rDiscipline;
	*/

	try
	{
		if(RemoteDB.OpenQuery(sSql.c_str()))
		{
			m_ModelEntry.clear();
			
			LONG lRecordCount = 0;
			RemoteDB.GetRecordCount(&lRecordCount);
			for(int i = 0;i < lRecordCount;++i)
			{	
				CPDSModel model;

				STRING_T rFieldName = RemoteDB.GetFieldNameAt(1);	///< FIELD 이름을 구한다.
				STRING_T rValue;
				RemoteDB.GetFieldValue(i , rFieldName , &rValue);	///< FIELD 값을 구한다.
				model.m_rName = rValue.c_str();
				
				m_ModelEntry.push_back(model);
			}
		}
		else
		{
			AfxMessageBox( _T("OpenQuery Error.") );
		}
	}
	catch(const exception& ex)
	{
		AfxMessageBox( CString(ex.what()) );
	}
}

size_t CPDSArea::GetModelCount() const
{
	return m_ModelEntry.size();
}

CPDSModel* CPDSArea::GetModelAt(const size_t &at)
{
	if(at < m_ModelEntry.size()) return (&m_ModelEntry.at(at));

	return NULL;
}

const CHAR_T* CPDSArea::name() const
{
	return m_rName.operator LPCTSTR();
}

/**
**/
BOOL CPDSArea::IsModelSelected(const CString &rModel)
{
	for(vector<CPDSModel>::iterator itr = m_ModelEntry.begin();itr != m_ModelEntry.end();++itr)
	{
		if(rModel == itr->name())
		{
			return (itr->m_bSelected);
		}
	}

	return FALSE;
}
