#pragma once

#include "PipeElement.h"

namespace IsoElement
{
class CElbowElement : public CPipeElement
{
public:
	CElbowElement(void);
	~CElbowElement(void);

	DPoint3d center() const;	/// 2011.10.27 added by humkyung
	int CollectElements( vector<CDgnElement*>* pDgnElmList , vector<CIsoElement*>* pWeldMarkElmList );

	void Check(CDgnDocument* pDgnDoc , CString sColor);
	static STRING_T TypeString();
private:
	int CheckElbowType(vector<CIsoElement*>* pWeldMarkElmList);	/// 2011.12.01 added by humkyung

	int ChooseElement4Type2(DPoint3d* pPoint1 , DPoint3d* pPoint2);

	int m_iType;
};
};