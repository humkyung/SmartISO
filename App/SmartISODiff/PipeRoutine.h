#pragma once

#include "PipeElement.h"
#include "SmartPtr.h"

class CIsoArea;
namespace IsoElement
{
class CDimElement;
class CPipeRoutine
{
public:
	class CPipeRun
	{
		CPipeRun(const CPipeRun& rhs){}
	public:
		CPipeRun(CIsoElement* pStartElm , DPoint3d ptStart , DPoint3d ptEnd , STRING_T sTypeString=_T("AUTO"));	
		~CPipeRun();

		int Check( CDgnDocument* pDgnDoc , CString sColor );

		int DrawCloudMarkForAttr(CDgnDocument* pDgnDoc);	/// 2011.12.22 added by humkyung

		double length() const;	/// 2011.12.13 added by humkyung
		int GetShapeForCloudMark( vector<DPoint3d>& oShapeForCloudMark );/// 2011.11.09 added by humkyung

		int AppendElement(CIsoElement* pIsoElm);	/// 2011.11.03 added by humkyung
		int SetAngle( const double& angle );		/// 2011.11.03 added by humkyung

		int Separate( vector<CPipeRoutine::CPipeRun*>& oPipeRunList );	/// 2011.10.28 added by humkyung
		int AssignDimension(vector<CIsoElement*>& oDimElmList);			/// 2011.10.28 added by humkyung
		double angle() const;											/// 2011.10.28 added by humkyung
		STRING_T typeString() const;		/// 2011.10.25 added by humkyung
		bool IsStartElmBranch() const;		/// 2011.10.25 added by humkyung
		CIsoElement* GetStartElm() const;	///	2011.10.25 added by humkyung
		int DrawCloudMark(bool bIncludeDimLine , CDgnDocument* pDgnDoc);/// 2011.10.22 added by humkyung
		int CompareWith( CPipeRun* pPipeRun);							/// 2011.10.22 added by humkyung
		int IntersectWith(DPoint3d& intersect , DVector3d* l);			/// 2011.10.22 added by humkyung
	public:
		DPoint3d m_ptStart , m_ptEnd;
		vector<CDimElement*> m_oDimElmList;
	private:
		double Length( );					/// 2011.10.28 added by humkyung
		double DistanceTo( DPoint3d pt );	/// 2011.10.28 added by humkyung

		STRING_T m_sTypeString;				/// 2011.11.03 added by humkyung
		double m_dAngle;					/// 2011.11.03 added by humkyung
		vector<CIsoElement*> m_oElmList;	/// 2011.10.25 added by humkyung
	};
private:
	CPipeRoutine(const CPipeRoutine& rhs){}

public:
	CPipeRoutine(CIsoArea* pIsoArea , CIsoElement* pStartElm=NULL);
	~CPipeRoutine(void);

	int GetPipeElmCount() const;/// 2011.11.10 added by humkyung

	int AlignToCompare();		/// 2011.11.04 added by humkyung
	
	CIsoArea* GetIsoArea() const;										/// 2011.10.25 added by humkyung
	int CompareWith(CPipeRoutine* pPipeRoutine , CIsoArea* pIsoArea );	/// 2011.10.24 added by humkyung
	int ComparePipeRunWith( const int iStartIndex1 , int iCount , CPipeRoutine* pPipeRoutine , const int iStartIndex2 , CIsoArea* pIsoArea );
	int GetPipeRunList(vector<CPipeRoutine::CPipeRun*>& oPipeRunList);	/// 2011.10.22 added by humkyung
	int CreatePipeRun();												/// 2011.10.22 added by humkyung
	int DrawCloudMark(int iStart , int iCount ,  bool bIncludeDimLine , CDgnDocument* pDgnDoc);	/// 2011.10.18 added by humkyung
	int AssignDimension(vector<CIsoElement*>* pDimElmList);				/// 2011.10.17 added by humkyung

	CIsoElement* GetStartElm();	/// 2011.10.17 added by humkyung
	int Join( CPipeRoutine* pPipeRoutine , vector<CIsoElement*>* pWeldMarkElmList );
	double DistanceToIfCollinear( CPipeRoutine* pTo );
	int Add( DPoint3d pt , CIsoElement* pIsoElm );
	int CollectConnectedPipe(vector<DPoint3d> oConnPtList , CIsoElement* pFrom , 
		vector<CIsoElement*>* pPipeElmList , vector<CIsoElement*>* pFittingElmList);
	
	int Check( CDgnDocument* pDgnDoc , CString sColor );
	
	vector<DPoint3d> m_ptConnList;		/// connection point list : don't access directly

private:
	bool ExistInConnPtList( const DPoint3d& pt ) const;	/// 2011.11.11 added by humkyung
	int CleanUpPipeRunList();	/// 2011.11.09 added by humkyung
	int Reverse();	/// 2011.10.28 added by humkyung
	int FindConnectedWeldMarkTo(DPoint3d pt1 , DPoint3d pt2 , vector<CIsoElement*>* pWeldMarkElmList);	/// 2011.10.28 added by humkyung
	CIsoElement* FindDimensionWith(DPoint3d pt1 , DPoint3d pt2);
	CIsoElement* GetFittingElmConnectedTo(vector<DPoint3d>& oNextConnPtList , const DPoint3d& ptConn , vector<CIsoElement*>* pFittingElmList);	/// 2011.10.17 added by humkyung

	CIsoElement* m_pStartElm;
		
	vector<CIsoElement*> m_oPipeElmList;
	vector<CPipeRun*> m_oPipeRunList;	/// 2011.10.22 added by humkyung

	CIsoArea* m_pIsoArea;	/// 2011.10.25 added by humkyung
};
};