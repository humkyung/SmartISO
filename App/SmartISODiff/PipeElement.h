#pragma once
#include "isoelement.h"

namespace IsoElement
{
class CPipeElement : public CIsoElement
{
	CPipeElement(const CPipeElement& rhs){}
public:
	CPipeElement(const bool bHasFlowMark=false);
	virtual ~CPipeElement(void);

	void Check(CDgnDocument* pDgnDoc , CString sColor);

	double angle() const;		/// 2011.11.02 added by humkyung

	bool HasFlowMark() const;	/// 2011.10.21 added by humkyung
	int GetConnPointList(vector<DPoint3d>& pts);
	void BuildConnPointList(CDgnElement* pElm1 , CDgnElement* pElm2);

	static STRING_T TypeString();
	double DistanceWith( CPipeElement* pTo );
	int Add(CDgnElement* p);
	int CollectConnectedPipe(vector<CIsoElement*>* pPipeElmList , vector<CIsoElement*>* pFittingElmList);
private:
	bool m_bHasFlowMark;
	DPoint3d m_ptFlowMarkArrowPoint;	/// 2011.10.21 added by humkyung
	
	DPoint3d GetFlowMarkArrowPoint();	/// 2011.10.21 added by humkyung
};
};