#pragma once
#include "isoelement.h"

namespace IsoElement
{
class CDimExtLineElement : public CIsoElement
{
public:
	CDimExtLineElement();
	~CDimExtLineElement(void);

	void Check(CDgnDocument* pDgnDoc , CString iColor);
};
};