#pragma once

#include "IsoElement.h"
#include "PipeRoutine.h"

namespace IsoElement
{
class COletElement : public CIsoElement
{
public:
	COletElement(void);
	~COletElement(void);

	int SetBranch( CPipeRoutine* pBranch );
	CPipeRoutine* GetBranch() const;

	void Check(CDgnDocument* pDgnDoc , CString sColor);

	DPoint3d center() const;	/// 2011.10.27 added by humkyung
	int CollectElements( vector<CDgnElement*>* pDgnElmList , vector<CIsoElement*>* pWeldMarkElmList , const DPoint3d& ptConn );

	static STRING_T TypeString();
	int GetConnPointList(vector<DPoint3d>& pts);
	DPoint3d GetStartPointOfBranch();
private:
	CPipeRoutine* m_pBranch;
};
};