#pragma once

#include "IsoElement.h"

namespace IsoElement
{
class CReducerElement :	public CIsoElement
{
public:
	CReducerElement(void);
	~CReducerElement(void);

	static STRING_T TypeString();

	int CollectElements( vector<CDgnElement*>* pDgnElmList , vector<CIsoElement*>* pWeldMarkElmList , const DPoint3d ptConn[2] );

	void Check(CDgnDocument* pDgnDoc , CString sColor);
};
};