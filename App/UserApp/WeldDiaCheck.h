// WeldDiaCheck.h: interface for the CWeldDiaCheck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WELDDIACHECK_H__B7AF15DE_09E2_423F_BD74_DD10A41D2AE5__INCLUDED_)
#define AFX_WELDDIACHECK_H__B7AF15DE_09E2_423F_BD74_DD10A41D2AE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "WeldDetailBMFile.h"
#include <vector>

#include <set>
#include <map>
using namespace std;
typedef struct TotalSpecValue_
{
	double m_nTotalSpec[6][4];

	TotalSpecValue_()
	{
		memset(m_nTotalSpec,'\0',sizeof(double)*6*4);
	}

}TotalSpecValue;

class CWeldDiaCheck  
{
public:
	void Clear();
	
	bool Write(const CString& rOuputFilePath);
	void Add(const CWeldDetailBMFile& weld_detail);
	//void Add(double nSub_Summary_By_Material[][4],const CString rSpec,vector<SpecSizeTotal> m_specSizeTotalEntry[2]);
	CWeldDiaCheck();
	virtual ~CWeldDiaCheck();
	
private:
	void InitInternalVariables();
	double m_nTotal_Sum[6][4];
	CString m_rSpec;	
	map<CString, TotalSpecValue > m_TotalSpecTypeEntry;
	CQCCheck* m_pQCCheck;
	set<double> m_sizeEntry[2];	///< BW,SW,TW - FW
	map<string , vector<SpecSizeTotal> > m_specSizeTotalEntry[2];
};

#endif // !defined(AFX_WELDDIACHECK_H__B7AF15DE_09E2_423F_BD74_DD10A41D2AE5__INCLUDED_)
