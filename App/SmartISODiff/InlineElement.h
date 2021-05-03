#pragma once

#include "PipeElement.h"

namespace IsoElement
{
class CInlineElement : CPipeElement
{
public:
	CInlineElement(DPoint3d pt1 , DPoint3d pt2);
	~CInlineElement(void);

	void Check(CDgnDocument* pDgnDoc , CString iColor);

	static STRING_T TypeString();
};
};