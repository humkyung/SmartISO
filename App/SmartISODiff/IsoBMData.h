/******************************************************************************
    @author    humkyung
    @date      2011-09-20
    @brief	class to store iso bm data
******************************************************************************/
#pragma once

#include <DgnText.h>

#include "IsoElement.h"
#include "IsTools.h"

#include <vector>
#include <map>

using namespace std;

class CIsoBMData 
{
	CString ParseSizeNum( const CString& rString );
public:
	typedef enum
	{
		NONE = 0,
		MODIFIED = 1
	}CompareResult;

	CIsoBMData(DPoint3d origin , const CString& sPartNo , const CString& sTextString);
	CIsoBMData(const CIsoBMData& rhs);
	CIsoBMData& operator=(const CIsoBMData& rhs);

	~CIsoBMData(void);

	void AddSubTextElm(CDgnText* pTextElm);
	bool Parse();
	CString GetAllSubString() const;

	bool operator==(const CIsoBMData& rhs);

	static map<STRING_T , pair<int,int> > m_oBMAttrPosMap;
									  
	CString m_sType;

	CString m_sDrawingNo;
	CString m_sMatlDesc;
	CString m_sMatlSpec;
	CString m_sBMCSSYM;
	CString m_sMatlCode;	///< Mat'l code
	CString m_sPaintCode;
	CString m_sInsType , m_sInsThickness;
	CString m_sNPD[2];
	CString m_sQTY;
	
	CString matlDesc() const
	{
		return m_sMatlDesc;
	}

	CString GetMainSize() const
	{
		return m_sNPD[0];
	}

	CString GetSubSize() const
	{		
		return m_sNPD[1];
	}

	CString matlSpec() const
	{
		return m_sMatlSpec;
	}
	
	CString bmcsSYM() const
	{
		return m_sBMCSSYM;
	}

	CString matlCode() const
	{
		return m_sMatlCode;
	}

	CString QTY() const
	{
		double nQTY = 0.f;
		SSCANF_T(m_sQTY , _T("%lf") , &nQTY);
		STRINGSTREAM_T oss;
		oss << nQTY;
		
		return oss.str().c_str();
	}

	CString plantNo() const
	{
		return m_sPlantNo;
	}
	CString areaNo() const
	{
		return m_sAreaNo;
	}
	CString unitNo() const
	{
		return m_sUnitNo;
	}
	CString sectionNo() const
	{
		return m_sSectionNo;
	}
	
	CString PartNo() const;

	CString DataString() const
	{
		return m_sDataString;
	}

	int SetCompareResult( const STRING_T& sItem , const int& iValue );	/// 2011.12.19 added by humkyung
	int GetCompareResultOf( const STRING_T& sItem );					/// 2011.12.19 added by humkyung

	vector<CString> m_oSubTextList;

	vector<CDgnElement*> m_oDgnElementList;
private:
	map<STRING_T , int> m_oCompareResultList;

	CString m_sPartNo;
	CString m_sPlantNo , m_sAreaNo , m_sUnitNo , m_sSectionNo;
	
	CString m_sDataString;
};
