#pragma once
#include <DgnLineString.h>
#include <DgnText.h>
#include "isoelement.h"
#include "LeaderElement.h"
#include "DimLineElement.h"
#include "DimExtLineElement.h"
#include "PipeRoutine.h"

#include <vector>
using namespace std;

namespace IsoElement
{
class CDimElement : public CIsoElement
{
public:
	class SortByLognerLength
	{
	public:
		SortByLognerLength(){}

		bool operator()(const CIsoElement* lhs , const CIsoElement* rhs) const
		{
			CDimElement* p1 = (CDimElement*)(lhs);
			CDimElement* p2 = (CDimElement*)(rhs);

			const double l1 = p1->GetDimValue();
			const double l2 = p2->GetDimValue();
			return (l1 > l2);
		}
	};

	class SortByDistance
	{
	public:
		SortByDistance(DPoint3d ptOrigin) : m_ptOrigin(ptOrigin){}

		bool operator()(const CDimElement* lhs , const CDimElement* rhs) const
		{
			vector<DPoint3d> oExtPtList;
			lhs->GetExtPointList( oExtPtList );
			double d1 = ::DistanceBetween(m_ptOrigin , oExtPtList[0]);
			double d2 = ::DistanceBetween(m_ptOrigin , oExtPtList[1]);
			const double dMinDist1 = (d1 < d2) ? d1 : d2;

			rhs->GetExtPointList( oExtPtList );
			::DistanceBetween(m_ptOrigin , oExtPtList[0]);
			::DistanceBetween(m_ptOrigin , oExtPtList[1]);
			d1 = ::DistanceBetween(m_ptOrigin , oExtPtList[0]);
			d2 = ::DistanceBetween(m_ptOrigin , oExtPtList[1]);
			const double dMinDist2 = (d1 < d2) ? d1 : d2;

			return (dMinDist1 < dMinDist2);
		}
	private:
		DPoint3d m_ptOrigin;
	};

	CDimElement();
	~CDimElement(void);

	void Check(CDgnDocument* pDgnDoc , CString sColor);

	CDimExtLineElement* GetExtLineElement();	/// 2011.12.22 added by humkyung

	double DistanceTo( const CDimElement& oDimElm );	/// 2011.120.5 added by humkyung

	int GetDimType( ) const;									/// 2011.11.09 added by humkyung
	int SetDimType( const int& iType );							/// 2011.11.09 added by humkyung
	int GetShapeForCloudMark(vector<DPoint3d>& oEnclosePolygon);/// 2011.11.09 added by humkyung

	int CompareWith( CDimElement* pDimElm );	/// 2011.10.20 added by humkyung
	
	CPipeRoutine::CPipeRun* GetConnectedPipeRun() const;
	int FindConnectedPipeRunFrom(vector<CPipeRoutine::CPipeRun*>& oPipeRunList);	/// 2011.10.22 added by humkyung
	int FindConnectedPipeRunFrom(vector<CIsoElement*>* oDimElmList);	/// 2011.12.05 added by humkyung

	int DrawCloudMark(CDgnDocument* pDgnDoc);	/// 2011.10.27 added by humkyung
	CDgnText* GetDimTextElement() const;	/// 2011.10.20 added by humkyung
	int Select(bool bIncludeDimLine , DgnModelRefP modelRef);	/// 2011.10.18 added by humkyung
	int CollectSubDimension(vector<CIsoElement*>* pDimElmList);	/// 2011.10.18 added by humkyung
	int GetExtPointList(vector<DPoint3d>& oExtPtList) const;	/// 2011.10.18 added by humkyung
	int GetEnclosePolygon(vector<DPoint3d>& oEnclosePolygon , const double dOffset=-1.0);	/// 2011.10.18 added by humkyung
	DVector3d GetExtendLineDirection();	/// 2011.10.17 added by humkyung
	double GetDimValue() const;			/// 2011.10.17 added by humkyung
	double angle() const;				/// 2011.10.17 added by humkyung

	vector<CDgnElement*> GetLineListConnectedToDimText(vector<CDgnElement*>* pDgnElmList , const double& dWOffset , const double& dHOffset);
	int CollectDimLines( vector<CDgnElement*>* pDgnElmList , vector<CIsoElement*>* pArrElmList );
	int CollectExtendLines( vector<CDgnElement*>* pDgnElmList , vector<CIsoElement*>* pArrElmList );

	static STRING_T TypeString();
private:
	CDgnElement* GetNearestElmWithSameAngleFrom(vector<CDgnElement*>* pDgnElmList , DPoint3d ptOrg , DVector3d l , const double& dDimTextAngle);	/// 2011.10.17 added by humkyung
	DPoint3d GetCenterOfDimText();	/// 2011.10.17 added by humkyung
	int ExpandEnclosePolygon( vector<DPoint3d>& oEnclosePolygon , const double dWidthOffset , const double dHeightOffset);	/// 2011.10.13 added by humkyung
	vector<DPoint3d> GetDimTextEnclosePolygon();	/// 2011.10.13 added by humkyung
	
	CLeaderElement* m_pLeaderLineElm;
	CDimLineElement* m_pDimLineElm;
	CDimExtLineElement* m_pExtLineElm;

	CPipeRoutine::CPipeRun* m_pConnectedPipeRun;	/// don't destry this. it's just reference - 2011.10.22 added by humkyung
	vector<CIsoElement*> m_oSubDimElmList;			/// objects to create dimension - 2011.10.12 added by humkyung

	int m_iDimType;	/// 2011.11.09 added by humkyung


};
};