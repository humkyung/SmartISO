#include "StdAfx.h"
#include <Tokenizer.h>
#include "IsoModelDocData.h"

double CIsoModelDocData::m_dDrawingToler = 0.0;

CIsoModelDocData::CIsoModelDocData(void) : m_dScale(1.0)
{
	m_dArcMinScale = 1.0;
	m_dArcMaxScale = 5.0;
	
	m_dCloudOffsetScale = 1.0;
}

CIsoModelDocData::~CIsoModelDocData(void)
{
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-02
    @class      CIsoModelDocData
    @function   GetAngleToler
    @return     double
    @brief		return the angle tolerance
******************************************************************************/
double CIsoModelDocData::GetAngleToler() const
{
	return (5.0);
}

/******************************************************************************
    @author     humkyung
    @date       2011-09-01
    @class      CIsoModelDocData
    @function   GetInstance
    @return     CIsoModelDocData&
    @brief
******************************************************************************/
CIsoModelDocData& CIsoModelDocData::GetInstance()
{
	static CIsoModelDocData __instance__;

	return __instance__;
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-16
    @class      CIsoModelDocData
    @function   IsPipeLineLevel
    @return     bool
    @param      const   CString&
    @param      sLevel
    @brief
******************************************************************************/
bool CIsoModelDocData::IsPipeLineLevel( const UInt32& uiLevel ) const
{
	map<CString,CString>::const_iterator where = m_oKeyValueMap.find(_T("PipeLine Level"));
	if(m_oKeyValueMap.end() != where)
	{
		STRING_T sLevel = where->second.operator LPCTSTR();		
		vector<STRING_T> oResult;
		CTokenizer<CIsFromString>::Tokenize(oResult , sLevel , CIsFromString(_T("|")));
		
		if(oResult.end() != find(oResult.begin() , oResult.end() , _T("?")))
		{
			return true;
		}

		STRINGSTREAM_T oss;
		oss << uiLevel;
		return (oResult.end() != find(oResult.begin() , oResult.end() , oss.str()));
	}

	return false;
}

bool CIsoModelDocData::IsPipeLineColor( const CString& sColor ) const
{
	map<CString,CString>::const_iterator where = m_oKeyValueMap.find(_T("PipeLine Color"));
	if(m_oKeyValueMap.end() != where)
	{
		STRING_T rostr = where->second.operator LPCTSTR();		
		vector<STRING_T> oResult;
		CTokenizer<CIsFromString>::Tokenize(oResult , rostr , CIsFromString(_T("|")));
		
		if(oResult.end() != find(oResult.begin() , oResult.end() , _T("?")))
		{
			return true;
		}

		return (oResult.end() != find(oResult.begin() , oResult.end() , sColor.operator LPCTSTR()));
	}

	return false;
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-16
    @class      CIsoModelDocData
    @function   IsPipeLineWeight
    @return     bool
    @param      const       UInt32&
    @param      uiWeight
    @brief
******************************************************************************/
bool CIsoModelDocData::IsPipeLineWeight( const UInt32& uiWeight ) const
{
	map<CString,CString>::const_iterator where = m_oKeyValueMap.find(_T("PipeLine Weight"));
	if(m_oKeyValueMap.end() != where)
	{
		STRING_T sWeight = where->second.operator LPCTSTR();		
		vector<STRING_T> oResult;
		CTokenizer<CIsFromString>::Tokenize(oResult , sWeight , CIsFromString(_T("|")));
		
		if(oResult.end() != find(oResult.begin() , oResult.end() , _T("?")))
		{
			return true;
		}

		STRINGSTREAM_T oss;
		oss << uiWeight;
		return (oResult.end() != find(oResult.begin() , oResult.end() , oss.str()));
	}

	return false;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-06
    @class      CIsoModelDocData
    @function   GetOffset
    @return     double
    @brief		return the offset
******************************************************************************/
double CIsoModelDocData::GetOffset() const
{
	return (CIsoModelDocData::m_dDrawingToler*10);
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-26
    @class      CIsoModelDocData
    @function   IsPageConnectionString
    @return     bool
    @param      const   CString&
    @param      sString
    @brief		check it is page connection string or not
******************************************************************************/
bool CIsoModelDocData::IsPageConnectionString( const CString& sString ) const
{
	return (
		(0 == sString.CompareNoCase(_T("COI NO"))) || 
		(0 == sString.CompareNoCase(_T("CONN TO"))) ||
		(0 == sString.CompareNoCase(_T("CONT. NO"))) ||
		(0 == sString.CompareNoCase(_T("CONN. TO")))
		);
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-16
    @class      CIsoModelDocData
    @function   GetArrowType
    @return     int
    @brief		return arrow type
******************************************************************************/
int CIsoModelDocData::GetArrowType() const
{
	map<CString , CString>::const_iterator where = m_oKeyValueMap.find(_T("Arrow Type"));
	if(where != m_oKeyValueMap.end())
	{
		return atoi(CStringA(where->second));
	}

	return 1;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-26
    @class      CIsoModelDocData
    @function   SetIniFilePathF    
    @param      
    @brief		Revision Cloud Mark Level , Color ∏¶ ¿˙¿Â
******************************************************************************/
void CIsoModelDocData::ReadSetting(const CString& sIniFilePath)
{
	if(_T("") != sIniFilePath)
	{
		CIsoModelDocData& docData = CIsoModelDocData::GetInstance ();

		docData.m_oKeyValueMap.clear();
		/// read setting from ini file - 2011.10.25 added by humkyung
		TCHAR szBuf[MAX_PATH + 1]={0,};
		
		CString sApp = _T("REVISION MARK");

		m_sIniFilePath = sIniFilePath;
		
		if(GetPrivateProfileString(sApp , _T("RevNo") , NULL , szBuf , MAX_PATH , sIniFilePath))
		{
			docData.m_oKeyValueMap.insert(make_pair(_T("RevNo") , szBuf));
		}
		else
		{
			docData.m_oKeyValueMap.insert(make_pair(_T("RevNo") , _T("?")));
		}
		if(GetPrivateProfileString(sApp , _T("Height") , NULL , szBuf , MAX_PATH , sIniFilePath))
		{
			docData.m_oKeyValueMap.insert(make_pair(_T("Height") , szBuf));
		}
		else
		{
			docData.m_oKeyValueMap.insert(make_pair(_T("Height") , _T("0.15")));
		}
		if(GetPrivateProfileString(sApp , _T("Width") , NULL , szBuf , MAX_PATH , sIniFilePath))
		{
			docData.m_oKeyValueMap.insert(make_pair(_T("Width") , szBuf));
		}
		else
		{
			docData.m_oKeyValueMap.insert(make_pair(_T("Width") , _T("0.15")));
		}

		if(GetPrivateProfileString(sApp , _T("Display") , NULL , szBuf , MAX_PATH , sIniFilePath))
		{
			docData.m_oKeyValueMap.insert(make_pair(_T("Display") , szBuf));
		}
		else
		{
			docData.m_oKeyValueMap.insert(make_pair(_T("Display") , _T("Cloud&RevNo")));
		}

		sApp = _T("Revision Automation");
		if(GetPrivateProfileString(sApp , _T("Arrow Type") , NULL , szBuf , MAX_PATH , sIniFilePath))
		{
			docData.m_oKeyValueMap.insert(make_pair(_T("Arrow Type") , szBuf));
		}

		if(GetPrivateProfileString(sApp , _T("PipeLine") , NULL , szBuf , MAX_PATH , sIniFilePath))
		{
			vector<STRING_T> oResult;
			CTokenizer<CIsComma>::Tokenize(oResult , szBuf , CIsComma());
			if(3 == oResult.size())
			{
				docData.m_oKeyValueMap.insert(make_pair(_T("PipeLine Level") , oResult[0].c_str()));
				docData.m_oKeyValueMap.insert(make_pair(_T("PipeLine Color") , oResult[1].c_str()));
				docData.m_oKeyValueMap.insert(make_pair(_T("PipeLine Weight") , oResult[2].c_str()));
			}
		}

		/*if(GetPrivateProfileString(sApp , _T("CloudMark") , NULL , szBuf , MAX_PATH , sIniFilePath))
		{
			vector<STRING_T> oResult;
			CTokenizer<CIsComma>::Tokenize(oResult , szBuf , CIsComma());
			if(3 == oResult.size())
			{
				docData.m_oKeyValueMap.insert(make_pair(_T("Level") , oResult[0].c_str()));
				docData.m_oKeyValueMap.insert(make_pair(_T("Color") , oResult[1].c_str()));
				docData.m_oKeyValueMap.insert(make_pair(_T("Weight"), oResult[2].c_str()));
			}
			else
			{
				docData.m_oKeyValueMap.insert(make_pair(_T("Level") , _T("1")));
				docData.m_oKeyValueMap.insert(make_pair(_T("Color") , _T("3")));
				docData.m_oKeyValueMap.insert(make_pair(_T("Weight"), _T("2")));
			}
		}
		else
		{
			docData.m_oKeyValueMap.insert(make_pair(_T("Level") , _T("1")));
			docData.m_oKeyValueMap.insert(make_pair(_T("Color") , _T("3")));
			docData.m_oKeyValueMap.insert(make_pair(_T("Weight"), _T("2")));
		}
		
		if(GetPrivateProfileString(sApp , _T("Offset") , _T("") , szBuf , MAX_PATH , sIniFilePath))
		{
			m_dCloudOffsetScale = ATOF_T( szBuf );
		}
		else
		{
			m_dCloudOffsetScale = 1.0;
		}

		if(GetPrivateProfileString(sApp , _T("ArcMin") , _T("") , szBuf , MAX_PATH , sIniFilePath))
		{
			m_dArcMinScale = ATOF_T( szBuf );
		}
		else
		{
			m_dArcMinScale = 1.0;
		}

		if(GetPrivateProfileString(sApp , _T("ArcMax") , _T("") , szBuf , MAX_PATH , sIniFilePath))
		{
			m_dArcMaxScale = ATOF_T( szBuf );
		}
		else
		{
			m_dArcMaxScale = 5.0;
		}*/
		/// up to here
	}
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-18
    @class      CIsoModelDocData
    @function   GetCloudLevel
    @return     UInt32
    @brief
******************************************************************************/
UInt32 CIsoModelDocData::GetCloudLevel() const
{
	map<CString,CString>::const_iterator where = m_oKeyValueMap.find(_T("Level"));
	if(where != m_oKeyValueMap.end())
	{
		return ATOI_T(where->second);
	}

	return 1;
}

/******************************************************************************
    @author     humkyung
    @date       2011-11-18
    @class      CIsoModelDocData
    @function   GetCloudColor
    @return     UInt32
    @brief
******************************************************************************/
UInt32 CIsoModelDocData::GetCloudColor() const
{
	map<CString,CString>::const_iterator where = m_oKeyValueMap.find(_T("Color"));
	if(where != m_oKeyValueMap.end())
	{
		return ATOI_T(where->second);
	}

	return 0;
}

double CIsoModelDocData::GetCloudHeight() const
{
	map<CString,CString>::const_iterator where = m_oKeyValueMap.find(_T("Height"));
	if(where != m_oKeyValueMap.end())
	{
		return ATOF_T(where->second);
	}

	return 0.15;
}
double CIsoModelDocData::GetCloudWidth() const
{
	map<CString,CString>::const_iterator where = m_oKeyValueMap.find(_T("Width"));
	if(where != m_oKeyValueMap.end())
	{
		return ATOF_T(where->second);
	}

	return 0.15;
}

CString CIsoModelDocData::GetCloudDisplay() const
{
	map<CString,CString>::const_iterator where = m_oKeyValueMap.find(_T("Display"));
	if(where != m_oKeyValueMap.end())
	{
		return where->second;
	}

	return _T("Cloud&RevNo");
}



CString CIsoModelDocData::GetRevisionNo( CDgnDocument* pDgnDoc ) const
{
	CString RevisionNo = _T("");
	/*ModelRefIteratorP   mrIterator = NULL ;
	mdlModelRefIterator_create (&mrIterator,modelRef,MRITERATE_Root|MRITERATE_PrimaryChildRefs,0);
	char         fileName[MAX_MODEL_DISPLAY_LENGTH];
	DgnModelRefP modelP = mdlModelRefIterator_getFirst (mrIterator);

	mdlModelRef_getFileName(modelP, fileName, sizeof(fileName));

	CString FullFilePath = CString(fileName);

	CArray<CString,CString> v;

	int index = 0;
	while(AfxExtractSubString(RevisionNo,FullFilePath,index,_T('.')))
	{
		v.Add(RevisionNo);
		++index;
	}
	
	return v[index - 1];;*/
	return RevisionNo;
}

/******************************************************************************
    @author     humkyung
    @date       2011-12-20
    @class      CIsoModelDocData
    @function   GetArcRadiusForCloudMark
    @return     int
    @param      double& dArcMin
    @param      double& dArcMax
    @brief		get arc min,max for cloud mark
******************************************************************************/
int CIsoModelDocData::GetArcRadiusForCloudMark(double& dArcMin , double& dArcMax)
{
	const double scale = this->m_dScale;
	dArcMin = (CIsoModelDocData::m_dDrawingToler*10.0 * m_dArcMinScale) / scale;	/// in uor
	dArcMax = (CIsoModelDocData::m_dDrawingToler*10.0 * m_dArcMaxScale) / scale;

	return ERROR_SUCCESS;
}

/******************************************************************************
    @author     humkyung
    @date       2011-12-22
    @class      CIsoModelDocData
    @function   GetCloudOffset
    @return     double
    @brief
******************************************************************************/
double CIsoModelDocData::GetCloudOffset()
{
	return ((CIsoModelDocData::m_dDrawingToler*10.0) * m_dCloudOffsetScale);
}