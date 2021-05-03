#pragma once

#include <DgnDocument.h>
#include "IsoElement.h"
#include "IsoArea.h"
#include "IsoBMArea.h"
#include "IsoTitleBlockArea.h"
#include "SmartPtr.h"

class CIsoModel
{
public:
	CIsoModel(CString sFilePath);
	~CIsoModel(void);
	int Read(const CString& sIniFilePath , const bool& bOpenForWrite = false);

	int Check();

	CDgnDocument* GetDgnDocument();	/// 2011.10.18 added by humkyung
	bool GetBMDataCorrespondingToPartNo(CIsoBMData& oIsoBMData , const CString& sPartNo);	/// 2011.10.04 added by humkyung
	int CompareWith(CIsoModel* pOldModel);
private:
	int ReadVolume(const CString& sIniFilePath);

	bool m_bOpenForWrite;
	CString m_sFilePath;
	CDgnDocument* m_pDgnDocument;

	map<CString , SmartPtr<CIsoArea> > m_oIsoAreaMap;

	list<CDgnElement*> m_oDgnElementList;
	
	list<CDgnElement*> m_oDummyDgnElementList;	/// 2011.11.15 added by humkyung
};
