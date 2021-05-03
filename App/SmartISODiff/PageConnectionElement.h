#pragma once

#include "AttributeElement.h"

namespace IsoElement
{
class CPageConnectionElement : public CAttributeElement
{
public:
	CPageConnectionElement(void);
	~CPageConnectionElement(void);

	int DrawCloudMark( CDgnDocument* pDgnDoc );

	static STRING_T TypeString();

	bool CompareWith(const CIsoElement* rhs) const;
private:

};
};