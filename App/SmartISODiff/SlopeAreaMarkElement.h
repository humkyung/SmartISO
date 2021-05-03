#pragma once

#include "IsoElement.h"
#include "PipeRoutine.h"

namespace IsoElement
{
class CSlopeAreaMarkElement : public CIsoElement
{
public:
	CSlopeAreaMarkElement();
	~CSlopeAreaMarkElement(void);

	static STRING_T TypeString();

	int GetPipeRunList(vector<CPipeRoutine::CPipeRun*>& oPipeRunList);	/// 2011.12.13 added by humkyung
	int CreatePipeRun();												/// 2011.12.13 added by humkyung

	static bool IsSlopeAreaMark(CDgnLineString* pLineStringElm);	/// 2011.12.13 added by humkyung

	void Check(CString sColor);
private:
	int CleanUpPipeRunList();			/// 2011.12.13 added by humkyung

	vector<CPipeRoutine::CPipeRun*> m_oPipeRunList;	/// 2011.12.13 added by humkyung
};
};