#pragma once

#include <DgnText.h>
#include "isoelement.h"
#include "IsoBMData.h"
#include "LeaderElement.h"
#include "ArrowElement.h"
class CIsoArea;
namespace IsoElement
{
class CAttributeElement : public CIsoElement
{
public:
	CAttributeElement(const CString& sPartNo);
	virtual ~CAttributeElement(void);

	int GetShapeForCloudMark(vector<DPoint3d>& oShapeForCloudMark);	/// 2011.12.22 added by humkyung

	int CreateVolumeList(vector<CDgnVolume>& oVolumeList);	/// 2011.12.02 added by humkyung

	void CollectPartOfAttribute(vector<CDgnElement*>* pDgnElmList , CDgnText* pTextElm , DPoint3d PartNo_Origin);

	int DrawCloudMark( CDgnDocument* pDgnDoc );	/// 2011.11.18 added by humkyung

	int Add(CDgnElement* p);				/// 2011.11.16 added by humkyung
	int ExpandVolume( CDgnVolume& volume );	/// 2011.11.16 added by humkyung
	CString GetElementType() const;

	void CollectLeaderLine(vector<CDgnElement*>* pDimElmList , vector<CDgnVolume> oVolumeList , vector<CIsoElement*>* pArrElmList);
	vector<CDgnElement*> GetConnectedFirstLeaderLine(vector<CDgnElement*>* pDgnElmList , vector<CDgnVolume> oVolumeList , vector<CIsoElement*>* pArrElmList);

	CString GetAttrType();
	CDgnText* GetPartNoTextElement();
	int AddLeaderlineElement( CDgnElement* pElm );
	CLeaderElement* GetLeaderElement();
	
	int SetIsoBMData(const CIsoBMData& oIsoBMData);
	CIsoBMData GetIsoBMData() const;
	CString GetPartNo() const;
	CString GetPartNoExceptS() const;

	void Check(CDgnDocument* pDgnDoc , CString sColor);
public:
	double m_dUsedToler;
private:

	CString m_sPartNo;
	CLeaderElement* m_pLeaderLine;

	CIsoBMData m_oIsoBMData;
	
};
};