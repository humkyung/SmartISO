#pragma once

#include "IsoElement.h"

namespace IsoElement
{
class CArrowElement : public CIsoElement
{
public:
	CArrowElement(void);
public:
	~CArrowElement(void);

	bool IsSameAngle( const double& angle ) const;	/// 2011.10.26 added by humkyung
	double DistanceTo( const DPoint3d& pt ) const;	/// 2011.10.26 added by humkyung
	bool IsLocatedInArrow(CDgnLineString* pLineStringElm , vector<DPoint3d>& oEnclosePolygon );	/// 2011.10.24 added by humkyung
	int Add(CDgnElement* p);						/// 2011.10.24 added by humkyung
	vector<DPoint3d> GetEnclosePolygon();
	int ExpandEnclosePolygon( vector<DPoint3d>& oEnclosePolygon/* , const double dOffset */);

	void Check(CDgnDocument* pDgnDoc , CString sColor);

	double angle() const;
private:
	bool DoesLineLocateInArrow(DPoint3d pts[2] , vector<DPoint3d>& oEnclosePolygon);	/// 2011.11.16 added by humkyung
public:
	vector<DPoint3d> m_oShapeList;
private:
	double m_angle[2];
	double m_angleRaw[2];
};
};