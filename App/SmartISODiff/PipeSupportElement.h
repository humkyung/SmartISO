#pragma once

#include "PipeElement.h"
#include "AttributeElement.h"

namespace IsoElement
{
class CPipeSupportElement : public CPipeElement
{
public:
	CPipeSupportElement(DPoint3d pt , CAttributeElement* pAttrElm);
	~CPipeSupportElement(void);

	int AddAttributeElement(CAttributeElement* pAttrElm);			/// 2011.12.22 added by humkyung
	int GetAttrElement(vector<CAttributeElement*>& oAttrElmList);	/// 2011.12.22 added by humkyung

	int DrawCloudMark( CDgnDocument* pDgnDoc );	/// 2011.11.18 added by humkyung

	void Check(CDgnDocument* pDgnDoc , CString sColor);
	DPoint3d GetOrigin(void);
	
	static STRING_T TypeString();
private:
	DPoint3d m_ptOrigin;
	vector<CAttributeElement*> m_oAttrElmList;
};
};