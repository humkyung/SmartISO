// QCCheck.h: interface for the CQCCheck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QCCHECK_H__B2422C9F_12AB_4EA4_94E7_F5C5DC5F48BF__INCLUDED_)
#define AFX_QCCHECK_H__B2422C9F_12AB_4EA4_94E7_F5C5DC5F48BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"

#include <boost/regex.hpp>
//#if(1200 == _MSC_VER)
//#pragma comment(lib, "boost_regex-vc6-mt.lib")			// VC++ 6
//#elif(1500 == _MSC_VER)
//	#pragma comment(lib, "boost_regex-vc90-mt-1_37.lib")	// VC++ 2008
//#endif

#include <DgnV7Lib.h>
#include <PCDFile.h>
#include "IsoBMData.h"

#include <vector>
#include <map>
using namespace std;

class CBoxTextString
{
public:
	CBoxTextString(){}
	~CBoxTextString(){}
	vector<CDgnText*> m_textStringEntry;
};

typedef struct _mapping
{
	STRING_T rCellName;
	double low,high;
	STRING_T rSupportName;
}mapping;

struct cellInfo
{
	STRING_T rCellName;
	double pos;
};

typedef struct
{
	CIsPoint2d pts[4];
	STRING_T WeldNo , SF , NPD , WeldType;
}WeldNoInfo;

class CQCCheck  
{
public:
	void CellMapping(CDgnDocument* pDgnDoc, const string &rCellName, const int &oriX, const int &oriY);
	CPCDFile* GetPCDFile(const string &matl_spec,const string &rIniFilePath);
	void UpdateIsoBMCSCode(const string &rIniFilePath);
	CQCCheck();
	virtual ~CQCCheck();
	vector<STRING_T> m_ntrTextEntry;
	vector<STRING_T> m_prtTextEntry;
	
	void ParseBMTexts(vector<CDgnText*>& systemBMTextEntry);
	void ExtractTextData(CDgnDocument* pDgnDoc , const string &rIniFilePath);
	void ReplaceSupport(CDgnDocument* pDgnDoc);
	int WriteWeldNo(CDgnDocument* pDgnDoc);
private:
	void GetNextCellPos( long& oriX , long& oriY );
	int FindSupportDetailNoAndReplaceSpecialDetailNo(vector<CIsoBMData>& graphicAreaTextEntry , const string& rSupportNo );
	void StoreBMDataText(CIsoBMData& bmData , vector<CDgnText*>::iterator& itr , vector<CDgnText*>& systemBMTextEntry);
	inline void InitInternalVariables();
	CDgnFile m_dgnFile;
	bool m_DgnOpen;
	vector<CIsoBMData> m_generalDatas;
	vector<CIsoBMData> m_pipeDatas;
	vector<CIsoBMData> m_supportDatas;
	vector<CIsoBMData> m_miscellDatas;
	vector<CIsoBMData> m_graphicAreaLineStringEntry;
	vector<CBoxTextString*> m_boxTextStringEntry;
	
	vector<CIsoBMData> m_WeldNoAreaDatas;
	vector<CIsoBMData> m_graphicAreaDatas;
	map<STRING_T , CPCDFile*> m_pcdFileMap;
};

#endif // !defined(AFX_QCCHECK_H__B2422C9F_12AB_4EA4_94E7_F5C5DC5F48BF__INCLUDED_)
