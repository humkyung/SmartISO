
#pragma once

#include <DgnText.h>

#include "IsoElement.h"
#include "IsTools.h"

#include <vector>
#include <map>

using namespace std;

class CIsoTitleBlockData
{
public:
	CIsoTitleBlockData(DPoint3d origin , const CString& sTextString);
	CIsoTitleBlockData(const CIsoTitleBlockData& rhs);
	CIsoTitleBlockData& operator=(const CIsoTitleBlockData& rhs);

	~CIsoTitleBlockData(void);

	bool operator==(const CIsoTitleBlockData& rhs);

	vector<CDgnElement*> m_oDgnElementList;
	DPoint3d m_origin;
	CString m_sDataString;

};
