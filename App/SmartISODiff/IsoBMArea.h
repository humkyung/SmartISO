#pragma once

#include "IsoArea.h"

class CIsoBMArea : public CIsoArea
{
public:
	CIsoBMArea(const CString& sName , const double& minx , const double& miny , const double& maxx , const double& maxy , CIsoModel* pIsoModel);
	~CIsoBMArea(void);

	void Add(CDgnElement* pEle);	/// 2011.11.16 added by humkyung

	bool GetBMDataCorrespondingToPartNo(CIsoBMData& oIsoBMData , const CString& sPartNo);
	void PrepareToCompare();
	vector<DPoint3d> GetIsoRevisedBMDataShapePoint(const int iStartPos , const int iWidth , CDgnText* pDgnText , CDgnDocument* pDgnDoc);
	vector<DPoint3d> GetSubTextShapePoint(CDgnText* pDgnText , CDgnDocument* pDgnDoc);
	int CompareWith(CIsoArea* rhs);
	int GetBMAttrTextHeaderStartPos(CString sTextHeader);
	int GetBMAttrTextHeaderWidth(CString sTextHeader);
	list<CIsoBMData> GetIsoRevisedBMDataListCompareWith(list<CIsoBMData> CurrentBMDataList , list<CIsoBMData> ReferenceBMDataList );
	map<pair<int , CString>, vector<DPoint3d>> GetIsoRevisedBMDataShapePointMap(list<CIsoBMData> oIsoRevisionBMDataList);
private:
	bool IsPartOfMatlDesc(const CString& sTextString);	/// 2011.12.19 added by humkyung
	void ParseBMElements();
	static bool SortByElemPosY(const CDgnElement* lhs , const CDgnElement* rhs);
	static bool SortByElemPosX(const CDgnElement* lhs , const CDgnElement* rhs);

	int ExpandEnclosePolygon( vector<DPoint3d>& oEnclosePolygon , const double dWidthOffset , const double dHeightOffset);	
private:
	list<CIsoBMData> m_oIsoBMDataList;

};