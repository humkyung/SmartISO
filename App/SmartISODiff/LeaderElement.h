#pragma once
#include "isoelement.h"
#include "ArrowElement.h"

namespace IsoElement
{
class CLeaderElement : public CIsoElement
{
public:
	CLeaderElement();
	~CLeaderElement(void);

	void Check(CDgnDocument* pDgnDoc , CString sColor);
	
	int ClearConnPointList();
	int AddConnPoint( DPoint3d pt );
	double GetAngle(DVec3d* vec1 , DVec3d* vec2);
	int Add(CDgnElement* p , DPoint3d ptConn);
	int Add(CDgnElement* p){ return CIsoElement::Add( p ); }

	int CollectPartOfLeaderLine(vector<CDgnElement*>* pDgnElmList , vector<CIsoElement*>* pArrElmList );
	
	CArrowElement* GetArrowElement();
private:
	int GetNextConnPointFrom( DPoint3d& ptAdjacent , DPoint3d& ptNextConn , CDgnLineString* pLineStringElm , const DPoint3d& ptConn );	/// 2011.10.26 added by humkyung
private:
	CArrowElement* m_pArrowElement;
};
};
