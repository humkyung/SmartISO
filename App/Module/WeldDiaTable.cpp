// WeldDiaTable.cpp: implementation of the CWeldDiaTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Module.h"
#include "ado/ADODB.h"
#include "WeldDiaTable.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWeldDiaTable::CWeldDiaTable() : m_pWeldDiaEntry(NULL)
{

}

CWeldDiaTable::~CWeldDiaTable()
{
	try
	{
		if(NULL != m_pWeldDiaEntry)
		{
			for(vector<WeldDia*>::iterator itr = m_pWeldDiaEntry->begin();itr != m_pWeldDiaEntry->end();++itr)
			{
				delete (*itr)->name;
				delete (*itr)->remark;

				delete (*itr);
			}
			m_pWeldDiaEntry->clear();
			delete m_pWeldDiaEntry;
		}
	}
	catch(...)
	{

	}
}

/**
	@brief	T_WELD_DIA 테이블을 로드한다.

	@author	humkyung
*/
void CWeldDiaTable::Load(const CString &rFilePath)
{
	if(NULL == m_pWeldDiaEntry)
	{
		m_pWeldDiaEntry = new vector<WeldDia*>;
		if(NULL == m_pWeldDiaEntry) return;
		
		CADODB adoDB;
		{
			STRING_T rConnectionString = _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source = ");
			rConnectionString += rFilePath;
			if(TRUE == adoDB.DBConnect(rConnectionString.c_str()))
			{
				string rQuery( _T("SELECT * FROM T_WELD_DIA") );
				
				try
				{
					adoDB.OpenQuery(rQuery.c_str());
					
					LONG lRecordCount = 0L;
					if(adoDB.GetRecordCount(&lRecordCount))
					{
						for(int i = 0;i < lRecordCount;i++)
						{
							WeldDia* p = (WeldDia*)calloc(1 , sizeof(WeldDia));
							if(p)
							{
								STRING_T rName, rRange, rMainSize , rSubSize , rRemark;
								adoDB.GetFieldValue(i , _T("MATL_CODE") , &rName);
								adoDB.GetFieldValue(i , _T("MAIN_WELD_POINT") , &rMainSize);
								adoDB.GetFieldValue(i , _T("SUB_WELD_POINT") , &rSubSize);
								adoDB.GetFieldValue(i , _T("REMARK") , &rRemark);

								if(p)
								{
									p->name = new CString(rName.c_str());
									p->main_size = atof(rMainSize.c_str());
									p->sub_size= atof(rSubSize.c_str());
									p->remark= new CString(rRemark.c_str());

									m_pWeldDiaEntry->push_back(p);
								}
							}
						}
					}
				}
				catch(const exception& ex)
				{
					AfxMessageBox(ex.what());
				}
				
				adoDB.DBDisConnect();
			}
			else
			{
				AfxMessageBox(STRING_T(_T("Fail to connect to ") + rFilePath).c_str());
			}
		}
	}
}

size_t CWeldDiaTable::GetWeldDiaCount() const
{
	if(NULL != m_pWeldDiaEntry) return m_pWeldDiaEntry->size();

	return 0;
}

/**
	@brief	

	@author	백흠경
*/
WeldDia* CWeldDiaTable::GetWeldDiaAt(const size_t &at)
{
	if((NULL != m_pWeldDiaEntry) && (at < m_pWeldDiaEntry->size()))
	{
		return (*m_pWeldDiaEntry)[at];
	}

	throw exception("range error");
}
