#pragma once
#include "isoelement.h"
#include "ArrowElement.h"
#include "LeaderElement.h"

#include <vector>
using namespace std;

namespace IsoElement
{
class CDimLineElement : public CIsoElement
{
	CDimLineElement(const CDimLineElement& rhs){}
public:
	CDimLineElement();
	~CDimLineElement(void);

	int CollectElements( vector<CDgnElement*>* pDgnElmList , vector<CIsoElement*>* pArrElmList , CLeaderElement* pLeaderElm , const int& iDimType );	/// 2011.11.29 added by humkyung

	int GetArrowCount() const;	/// 2011.11.29 added by humkyung

	int CollectArrows( vector<CIsoElement*>* pArrElmList );	/// 2011.10.26 added by humkyung
	void Check(CDgnDocument* pDgnDoc , CString sColor);
private:
	int CollectElements4Type1( vector<CDgnElement*>* pDgnElmList , vector<CIsoElement*>* pArrElmList , CLeaderElement* pLeaderElm);

	vector<CArrowElement*> m_oArrElmList;
};
};
