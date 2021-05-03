#pragma once

#include "IsoElement.h"

namespace IsoElement
{
class CWeldMarkElement : public CIsoElement
{
public:
	CWeldMarkElement(const DPoint3d& center , const double& dRadius);
	~CWeldMarkElement(void);

	static bool IsWeldMark( DPoint3d& center , double& dRadius , 
		CDgnLineString* pLineStringElm , vector<CDgnElement*>* pDgnElmList);	/// 2011.12.01 added by humkyung

	DPoint3d center() const;						/// 2011.10.31 added by humkyung
	double DistanceTo( const DPoint3d& pt ) const;	/// 2011.10.31 added by humkyung
	int Add(CDgnElement* p);						/// 2011.10.31 added by humkyung

	void Check(CDgnDocument* pDgnDoc , CString sColor);
private:
	DPoint3d m_center;
	double m_dRadius;
};
};