// UserDocData.cpp: implementation of the CUserDocData class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UserDLL.h"
#include "UserDocData.h"
#include <util\FileTools.h>
#include <util\Markup\Markup.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CUserDocData::CUserDocData()
{
	m_pWeldTypeMap = NULL;
}

CUserDocData::~CUserDocData()
{
	try
	{
		SAFE_DELETE(m_pWeldTypeMap);
	}
	catch(...)
	{
	}
}

/******************************************************************************
    @author     humkyung
    @date       2012-08-20
    @class      CUserDocData
    @function   GetInstance
    @return     CUserDocData&
    @brief
******************************************************************************/
CUserDocData& CUserDocData::GetInstance()
{
	static CUserDocData __instance__;

	return (__instance__);
}

/******************************************************************************
    @author     humkyung
    @date       2012-08-20
    @class      CUserDocData
    @function   LoadWeldTypeMap
    @return     int
    @param      const               STRING_T&
    @param      sWeldTypeFilePath
    @brief		load weld type definition file
******************************************************************************/
int CUserDocData::LoadWeldTypeMap(const STRING_T& sWeldTypeFilePath)
{
	if(NULL == m_pWeldTypeMap)
	{
		m_pWeldTypeMap = new map<STRING_T,STRING_T>;
		if(CFileTools::DoesFileExist(sWeldTypeFilePath.c_str()))
		{
			CMarkup xml;
			if(true == xml.Load(sWeldTypeFilePath.c_str()))
			{
				MCD_STR sLoadResult = xml.GetResult();
				if(true == xml.FindElem(_T("WeldTypeList")))
				{
					xml.IntoElem();
					while(xml.FindElem(MCD_T("WeldType")))
					{
						STRING_T sCode , sType;
						
						int iCount = 0;
						while ( 1 )
						{
							STRING_T sName = xml.GetAttribName(iCount++);
							if ( sName.empty() ) break;
							if(_T("Code") == sName)
							{
								sCode = xml.GetAttrib( sName );
							}
							else if(_T("Type") == sName)
							{
								sType = xml.GetAttrib( sName );
							}
						}
						if(!sCode.empty() && !sType.empty())
						{
							m_pWeldTypeMap->insert(make_pair(sCode , sType));
						}
					}
				}
			}
		}
		else	/// default weld type
		{
			m_pWeldTypeMap->insert(make_pair(_T("301") , _T("BW")));
			
			m_pWeldTypeMap->insert(make_pair(_T("391") , _T("SW")));
			m_pWeldTypeMap->insert(make_pair(_T("421") , _T("SW")));
			
			m_pWeldTypeMap->insert(make_pair(_T("331") , _T("TW")));
			m_pWeldTypeMap->insert(make_pair(_T("441") , _T("TW")));
			
			m_pWeldTypeMap->insert(make_pair(_T("11") , _T("FW")));
			m_pWeldTypeMap->insert(make_pair(_T("21") , _T("FW")));
			m_pWeldTypeMap->insert(make_pair(_T("31") , _T("FW")));
			m_pWeldTypeMap->insert(make_pair(_T("32") , _T("FW")));
			m_pWeldTypeMap->insert(make_pair(_T("51") , _T("FW")));
			m_pWeldTypeMap->insert(make_pair(_T("61") , _T("FW")));
			m_pWeldTypeMap->insert(make_pair(_T("71") , _T("FW")));
			m_pWeldTypeMap->insert(make_pair(_T("81") , _T("FW")));
			m_pWeldTypeMap->insert(make_pair(_T("91") , _T("FW")));

			m_pWeldTypeMap->insert(make_pair(_T("111") , _T("TF")));
			m_pWeldTypeMap->insert(make_pair(_T("121") , _T("TF")));
			m_pWeldTypeMap->insert(make_pair(_T("131") , _T("TF")));
		}
	}

	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2012-08-20
    @class      CUserDocData
    @function   FindWeldTypeOf
    @return     STRING_T
    @param      const   STRING_T&
    @param      code
    @brief
******************************************************************************/
STRING_T CUserDocData::FindWeldTypeOf(const STRING_T& code , const STRING_T& sWeldTypeFilePath)
{
	STRING_T res(_T(""));

	if(NULL == this->m_pWeldTypeMap) this->LoadWeldTypeMap(sWeldTypeFilePath);
	if(NULL != this->m_pWeldTypeMap)
	{
		map<STRING_T,STRING_T>::iterator where = this->m_pWeldTypeMap->find(code);
		if(where != this->m_pWeldTypeMap->end())
		{
			res = where->second;
		}
	}

	return res;
}