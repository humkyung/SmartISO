#pragma once
#include "PipeElement.h"
#include "PipeRoutine.h"

#include "SmartPtr.h"

namespace IsoElement
{
class CTeeElement : public CPipeElement
{
public:
	CTeeElement();
	~CTeeElement(void);

	void Check(CDgnDocument* pDgnDoc , CString sColor);

	int Add(CDgnElement* p);	/// 2011.11.14 added by humkyung

	int SetMainRoutine( CPipeRoutine* pMainRoutine );	/// 2011.11.09 added by humkyung
	CPipeRoutine* GetMainRoutine() const;				/// 2011.11.09 added by humkyung

	int Reverse();				/// 2011.11.03 added by humkyung

	DPoint3d center() const;	/// 2011.10.19 added by humkyung
	void BuildConnPointList( const DPoint3d& ptCenter );

	static STRING_T TypeString();
	DPoint3d GetStartPointOfBranch();
	int GetConnPointList(vector<DPoint3d>& pts);

	int SetBranchRoutine( CPipeRoutine* pBranch );
	CPipeRoutine* GetBranchRoutine() const;
private:
	DPoint3d m_ptCenter;
	
	CPipeRoutine *m_pMainRoutineRef , *m_pBranchRoutineRef;
};
};