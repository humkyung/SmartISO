#pragma once

#include "IsoElement.h"
#include "PipeRoutine.h"

namespace IsoElement
{
class CRWeldElement : public CIsoElement
{
public:
	CRWeldElement(void);
	~CRWeldElement(void);

	DPoint3d GetStartPointOfBranch();		/// 2011.11.10 added by humkyung
	CIsoElement* GetStartElmOfBranchFrom(vector<CIsoElement*>* pPipeElmList);			/// 2011.11.02 added by humkyung

	int SetBranch( CPipeRoutine* pBranch );	/// 2011.11.02 added by humkyung
	CPipeRoutine* GetBranch() const;		/// 2011.11.02 added by humkyung

	int CollectElements( vector<CDgnElement*>* pDgnElmList , vector<CIsoElement*>* pWeldMarkElmList , const DPoint3d& ptConn );	/// 2011.11.02 added by humkyung
	
	void Check(CDgnDocument* pDgnDoc , CString sColor);

	static STRING_T TypeString();
private:
	CDgnElement* m_pStartDgnElmOfBranch;

	CPipeRoutine* m_pBranch;
};
};