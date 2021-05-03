#pragma once
#include <DgnText.h>

class CBMLine
{
public:
	CBMLine(CDgnText* pDgnText);
	~CBMLine(void);
	CBMLine(const CBMLine& rhs);
	CBMLine& operator=(const CBMLine& rhs);

	double yCoord() const;
	DPoint3d origin() const;
	CDgnVolume volume() const;
	int Add(CDgnText* pDgnText);
	LPCTSTR textString() const;

	int GetTextCount() const;
	LPCTSTR GetTextAt(const int& at);

	static bool SortByY(CBMLine* lhs , CBMLine* rhs);
private:
	vector<CDgnText*> m_oDgnTextList;
	STRING_T m_sDesc;

	IsRegExp::RxNode *m_rxnode;
	IsRegExp::RxMatchRes m_rxmatch;
};

