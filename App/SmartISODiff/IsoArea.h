#pragma once

#include "IsTools.h"
#include "IsRegExp.h"
#include <DgnText.h>
#include "IsoElement.h"
#include "IsoBMData.h"
#include "IsoTitleBlockData.h"
#include "AttributeElement.h"
#include "PipeRoutine.h"
#include "PipeSupportElement.h"
#include "SmartPtr.h"
#include <string>
#include <vector>

using namespace IsoElement;
class CIsoModel;
class CIsoArea
{
public:
	CIsoArea(const CString& sName , const double& minx , const double& miny , const double& maxx , const double& maxy , CIsoModel* pIsoModel);
	virtual ~CIsoArea(void);

	int Check();

	int CheckEnvironment();	/// 2011.11.11 added by humkyung

	CPipeRoutine* GetPipeRoutineStartWith( CIsoElement* pStartElm );	/// 2011.10.22 added by humkyung
	CPipeElement* ChoosePipeToRoute(vector<CIsoElement*>* pPipeElmList);	/// 2011.10.21 added by humkyung

	virtual void PrepareToCompare();	/// 2011.09.27 added by humkyung
	virtual void Add(CDgnElement* pEle);
	void FindConnectedpartNoDimLine(CAttributeElement* pAttrElm , CDgnVolume volume);
	void Test_DrawBoundary(CDgnVolume volume);
	CPipeSupportElement* GetPipeSupportAt(const DPoint3d& pt);

	virtual int CompareWith(CIsoArea* rhs);

	CDgnVolume volume() const;
	CIsoModel* GetIsoModel() const;	/// 2011.10.24 added by humkyung
private:
	CString m_sName;
	CDgnVolume m_volume;
protected:
	CIsoModel* m_pIsoModel;

	map<CString , vector<CDgnElement*>*> m_oDgnElementMap;
	map<CString , vector<CIsoElement*>*> m_oIsoElementMap;

	IsRegExp::RxNode *m_rxnode;
	IsRegExp::RxMatchRes m_rxmatch;
private:
	int GetElevationAttribute(vector<CDgnElement*>* pDgnElmList);	/// 2011.12.22 added by humkyung
	int CollectLeaderLineForAttr(vector<CIsoElement*>* pAttrElmList , vector<CDgnElement*>* pDgnElmList);	/// 2011.12.20 added by humkyung

	int CleanElementMap();	/// 2011.12.14 added by humkyung
	int SeperateDimTextFrom(vector<CDgnElement*>* pDgnElmList);	/// 2011.11.29 added by humkyung
	bool IsRectangular(CDgnLineString* pLineStringElm);	/// 2011.11.03 added by humkyung
	int SetupElementMap();						/// 2011.10.19 added by humkyung
	int CollectPageConnectionElement();			/// 2011.10.19 added by humkyung
	int GetDisconnectedPipeRoutineCount();		/// 2011.10.17 added by humkyung
	int CollectAttributeElements(void);
	int CreatePipeSupportElementWith(CAttributeElement* pAttrElm);
	int CreateElbowElementWith(CAttributeElement* pAttrElm);
	int CreateTeeElementWith(CAttributeElement* pAttrElm);
	int CreateOletElementWith(CAttributeElement* pAttrElm);
	int CreateReducerElementWith(CAttributeElement* pAttrElm);	/// 2011.10.31 added by humkyung
	int CreateRWeldElementWith(CAttributeElement* pAttrElm);	/// 2011.11.02 added by humkyung
	int CreateConnectedElementWithAttribute(void);
	int CombinePipeElementConnectedToFlowMark(vector<CDgnElement*>* pDgnElmList);
	int CombinePipeElements(vector<CIsoElement*>* pPipeElmList);
	int CreatePipeRoutineForBranchElm(vector<CIsoElement*>* pFittingElmList , vector<CIsoElement*>* pPipeElmList);	/// 2011.11.09 added by humkyung
	int ConnectPipes(void);
	int CollectArrows(const CString& sCategory);
	int CollectDimensions(void);
	int AssignDimensionToPipe(void);

	vector<CPipeRoutine*> m_oPipeRoutineList;
};
