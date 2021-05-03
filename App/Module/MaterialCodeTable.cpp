// MaterialCodeTable.cpp: implementation of the CMaterialCodeTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <assert.h>
#include "Module.h"
#include "MaterialCodeTable.h"
#include "ado/ADODB.h"

#include <algorithm>
#include <functional>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define	FLANGE_ITEM_GROUP_STRING		"2"
#define	FITTING_ITEM_GROUP_STRING		"4"
#define	PIPE_ITEM_GROUP_STRING			"5"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMaterialCodeTable::CMaterialCodeTable() : m_pMaterialCodeEntry(NULL)
{

}

CMaterialCodeTable::~CMaterialCodeTable()
{
	try
	{
		if(NULL != m_pMaterialCodeEntry)
		{
			for(vector<MaterialCode*>::iterator itr = m_pMaterialCodeEntry->begin();itr != m_pMaterialCodeEntry->end();++itr)
			{
				delete (*itr);
			}
			m_pMaterialCodeEntry->clear();
			delete m_pMaterialCodeEntry;
		}
	}
	catch(...)
	{

	}
}

namespace
{
	struct FindMatlNumWithGroupAndCode8_9Func : public binary_function<CMaterialCodeTable::MaterialCode* , CMaterialCodeTable::MaterialCode* , bool>
	{
		bool operator()(const CMaterialCodeTable::MaterialCode* lhs , const CMaterialCodeTable::MaterialCode* rhs) const
		{
			return ((lhs->code1 == rhs->code1) && (lhs->code8_9 == rhs->code8_9));
		}
	};

	struct FindMatlNumWithGroupAndCode10_11Func : public binary_function<CMaterialCodeTable::MaterialCode* , CMaterialCodeTable::MaterialCode* , bool>
	{
		bool operator()(const CMaterialCodeTable::MaterialCode* lhs , const CMaterialCodeTable::MaterialCode* rhs) const
		{
			return ((lhs->code1 == rhs->code1) && (lhs->code10_11 == rhs->code10_11));
		}
	};

	struct FindMatlNumWithGroupAndCodeLoc4Func : public binary_function<CMaterialCodeTable::MaterialCode* , CMaterialCodeTable::MaterialCode* , bool>
	{
		bool operator()(const CMaterialCodeTable::MaterialCode* lhs , const CMaterialCodeTable::MaterialCode* rhs) const
		{
			return ((lhs->code1 == rhs->code1) && (lhs->code2 == rhs->code2));
		}
	};
};

/**
	@brief	item group과 code(8~9 , 10~11)의 값으로 matl number를 구한다.
	
	@author	백흠경

	@param	pGroupString
	@param	pCodeString

	@return material code num, 찾을 수 없으면 -1을 리턴한다.
*/
int CMaterialCodeTable::FindMatlNumForGALV(const char *pGroupString , const char *pCodeString) const
{
	assert(pGroupString && pCodeString && "pGroupString or pCodeString is NULL");

	if(pGroupString && pCodeString)
	{
		MaterialCode materialCode;
		materialCode.code1 = pGroupString;
		if(PIPE_ITEM_GROUP_STRING == materialCode.code1)
		{
			materialCode.code10_11 = pCodeString;

			vector<MaterialCode*>::iterator where = find_if(m_pMaterialCodeEntry->begin() , m_pMaterialCodeEntry->end() , bind2nd(FindMatlNumWithGroupAndCode10_11Func() , &materialCode));
			if(m_pMaterialCodeEntry->end() != where)
			{
				return (*where)->matl_num;
			}
		}
		else if((FLANGE_ITEM_GROUP_STRING == materialCode.code1) || (FITTING_ITEM_GROUP_STRING == materialCode.code1))
		{
			materialCode.code8_9 = pCodeString;

			vector<MaterialCode*>::iterator where = find_if(m_pMaterialCodeEntry->begin() , m_pMaterialCodeEntry->end() , bind2nd(FindMatlNumWithGroupAndCode8_9Func() , &materialCode));
			if(m_pMaterialCodeEntry->end() != where)
			{
				return (*where)->matl_num;
			}
		}
	}

	return -1;
}

/**
	@brief	item group과 code loc 4의 값으로 matl number를 구한다.
	
	@author	백흠경

	@return material code num, 찾을 수 없으면 -1을 리턴한다.
*/
int CMaterialCodeTable::FindMatlNumWithGroupAndCodeLoc4(const char *pCode1, const char *pCode2) const
{
	assert(pCode1 && pCode2 && "pCode1 or pCode2 is NULL");

	if(pCode1 && pCode2)
	{
		MaterialCode materialCode;
		materialCode.code1 = pCode1;
		materialCode.code2 = pCode2;

		vector<MaterialCode*>::iterator where = find_if(m_pMaterialCodeEntry->begin() , m_pMaterialCodeEntry->end() , bind2nd(FindMatlNumWithGroupAndCodeLoc4Func() , &materialCode));
		if(m_pMaterialCodeEntry->end() != where)
		{
			return (*where)->matl_num;
		}
	}

	return -1;
}

/**	
	@brief	The CMaterialCodeTable::Load function
	
	@author	humkyung

	@param	rFilePath	a parameter of type const CString &

	@return	void
*/
void CMaterialCodeTable::Load(const CString &rFilePath)
{
	if(NULL == m_pMaterialCodeEntry)
	{
		m_pMaterialCodeEntry = new vector<MaterialCode*>;
		if(NULL == m_pMaterialCodeEntry) return;
		
		CADODB adoDB;
		{
			STRING_T rConnectionString = _T("Provider=Microsoft.Jet.OLEDB.4.0;Data Source = ");
			rConnectionString += rFilePath;
			if(TRUE == adoDB.DBConnect(rConnectionString.c_str()))
			{
				STRING_T rQuery(_T("SELECT * FROM T_MATERIAL"));
				
				try
				{
					adoDB.OpenQuery(rQuery.c_str());
					
					LONG lRecordCount = 0L;
					if(adoDB.GetRecordCount(&lRecordCount))
					{
						for(int i = 0;i < lRecordCount;i++)
						{
							MaterialCode* p = (MaterialCode*)calloc(1 , sizeof(MaterialCode));
							if(p)
							{
								STRING_T rCode1, rCode2 , rCode8_9 , rCode10_11 , rMatlNum;
								adoDB.GetFieldValue(i , _T("ITEM_GROUP") , &rCode1);
								adoDB.GetFieldValue(i , _T("MATL_CODE_LOC_4") , &rCode2);
								adoDB.GetFieldValue(i , _T("MATL_CODE_LOC_8_9") , &rCode8_9);
								adoDB.GetFieldValue(i , _T("MATL_CODE_LOC_10_11") , &rCode10_11);
								adoDB.GetFieldValue(i , _T("BASIC_MATL") , &rMatlNum);

								if(p)
								{
									p->code1     = rCode1.c_str();
									p->code2     = rCode2.c_str();
									p->code8_9   = rCode8_9.c_str();
									p->code10_11 = rCode10_11.c_str();
									p->matl_num  = atoi(rMatlNum.c_str());

									m_pMaterialCodeEntry->push_back(p);
								}
							}
						}
					}
				}
				catch(const exception& ex)
				{
					AfxMessageBox(ex.what());
				}
			}
			else
			{
				AfxMessageBox(STRING_T(_T("Fail to connect to ") + rFilePath).c_str());
			}
		}
	}
}
