#pragma once

#include <DgnElement.h>
#include "DgnLineString.h"
namespace IsoElement
{
class CIsoElement
{
public:
	class SortIsoElmByDistance
	{
	public:
		SortIsoElmByDistance(DPoint3d ptConn) : m_ptConn(ptConn) {}

		bool operator()(const CIsoElement* lhs , const CIsoElement* rhs) const
		{
			const double d1 = lhs->DistanceTo( m_ptConn );
			const double d2 = rhs->DistanceTo( m_ptConn );
			return (d1 < d2);
		}
	private:
		DPoint3d m_ptConn;
	};

	class SortDgnElmByDistance
	{
	public:
		SortDgnElmByDistance(DPoint3d ptConn) : m_ptConn(ptConn) {}

		bool operator()(const CDgnElement* lhs , const CDgnElement* rhs) const
		{
			const double d1 = lhs->DistanceTo( m_ptConn );
			const double d2 = rhs->DistanceTo( m_ptConn );
			return (d1 < d2);
		}
	private:
		DPoint3d m_ptConn;
	};

	class SortDgnElmByDistanceFromDgnElm
	{
	public:
		SortDgnElmByDistanceFromDgnElm(CDgnElement* p) : m_pBase(p) {}

		bool operator()(const CDgnElement* lhs , const CDgnElement* rhs)
		{
			DPoint3d ptBase = m_pBase->origin();
			DPoint3d pts[2] = {'\0',};
			pts[0] = lhs->origin();
			pts[1] = rhs->origin();
			const double d1 = ::DistanceBetween( (ptBase) , (pts[0]) );
			const double d2 = ::DistanceBetween( (ptBase) , (pts[1]) );

			return (d1 < d2);
		}
	private:
		CDgnElement* m_pBase;
	};

	class SortDPoint3dFrom
	{
	public:
		SortDPoint3dFrom(DPoint3d ptFrom) : m_ptFrom(ptFrom) {}

		bool operator()(const DPoint3d lhs , const DPoint3d rhs) const
		{
			const double d1 = ::DistanceBetween(lhs , m_ptFrom);
			const double d2 = ::DistanceBetween(rhs , m_ptFrom );
			return (d1 < d2);
		}
	private:
		DPoint3d m_ptFrom;
	};

	CIsoElement();
	
	virtual ~CIsoElement(void);

	virtual void Check(CDgnDocument* pDgnDoc , CString sColor = _T("2"));

	const bool IsKindOf(const STRING_T& sTypeString) const;	/// 2011.11.25 added by humkyung

	static int GetLeaderLineFrom(vector<CDgnElement*>& oOneArrowList , vector<CDgnElement*>& oTwoArrowList , 
		const vector<CDgnElement*>& oCandidatedElmList , vector<CIsoElement*>* pArrElmList , const DPoint3d& ptCenter);	/// 2011.11.17 added by humkyung

	static bool IsPossibleToBeCircle(DPoint3d& center , double& dRadius , CDgnElement* pDgnElm);	/// 2011.12.01 added by humkyung
	
	static bool Collinear(DPoint3d lhs[2] , DPoint3d rhs[2] , const double& dMinAngle=1.0);
	static CIsoElement* GetArrowElementHas(const DPoint3d& ptEnd , DVector3d* pDir , vector<CIsoElement*>* pArrElmList , double dAngleToler=1.5);	/// 2011.10.24 added by humkyung
	static bool IsInnerPt(vector<DPoint3d> polygon , DPoint3d pt);
	static bool IsSamePoint( DPoint3d pt1 , DPoint3d pt2 , const double dToler=0.001 );
	virtual double DistanceTo( const DPoint3d& pt ) const;

	virtual int Reverse();	/// 2011.10.28 added by humkyung
	virtual DPoint3d center() const;	/// 2011.10.27 added by humkyung
	virtual int DrawCloudMark( DgnModelRefP modelRef );	/// 2011.10.27 added by humkyung
	virtual int GetConnPointList(vector<DPoint3d>& pts);

	bool ShouldHaveOwnDim() const;	/// 2011.11.03 added by humkyung
	bool IsBranchElm() const;		/// 2011.10.22 added by humkyung
	bool IsFittingElm() const;		/// 2011.10.22 added by humkyung
	
	STRING_T typeString() const;

	CDgnVolume volume();
	CDgnElement* DgnElementP( const int& at );
	size_t size() const;
	virtual int Add(CDgnElement* p);
	vector<DPoint3d> GetptConn();

	static DPoint3d GetStartPointForRevNo(vector<DPoint3d> EnclosePolygon);
public:
	vector<CDgnElement*> m_oDgnElementList;
	///TODO: 필요없는 지 확인
	vector<CIsoElement*> m_oIsoElementList;
	
protected:
	STRING_T m_sTypeString;
	vector<DPoint3d> m_ptConn;	/// connection point list
};
};