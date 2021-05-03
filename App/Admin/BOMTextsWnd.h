#pragma once

#include <map>
#include <list>

// CBMOTextsWnd

class CBMOTextsWnd : public CWnd
{
	DECLARE_DYNAMIC(CBMOTextsWnd)

public:
	CBMOTextsWnd();
	virtual ~CBMOTextsWnd();

	std::map<double,CString> m_oBmTexts;
	std::list<int> m_oBmHeaderIndices;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


