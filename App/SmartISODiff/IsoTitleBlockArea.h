#pragma once

#include "IsoArea.h"

class CIsoTitleBlockArea : public CIsoArea
{
public:
	CIsoTitleBlockArea(const CString& sName , const double& minx , const double& miny , const double& maxx , const double& maxy , CIsoModel* pIsoModel);
	~CIsoTitleBlockArea(void);

	void PrepareToCompare();

	int CompareWith(list<CDgnElement*>& oRemovedElemList , list<CDgnElement*>& oAddedElemList , CIsoArea* rhs);
	
private:
	void ParseTitleBlockElements();
	map<CString , vector<DPoint3d>> GetIsoRevisedTitleBlockDataShapePointMap(list<CIsoTitleBlockData> oIsoRevisionTitleBlockDataList);
	vector<DPoint3d> GetIsoRevisedTitleBlockDataShapePoint(const int iStartPos , const int iWidth , CString sElemdntID , DgnModelRefP modelRef);
	int CIsoTitleBlockArea::ExpandEnclosePolygon( vector<DPoint3d>& oEnclosePolygon , const double dWidthOffset , const double dHeightOffset);

private:
	list<CIsoTitleBlockData> m_oIsoTitleBlockDataList;
	
	double m_minx;
	double m_miny;
	double m_maxx;
	double m_maxy;
};
