#pragma once


// CMFCButtonEx

class CMFCButtonEx : public CMFCButton
{
	DECLARE_DYNAMIC(CMFCButtonEx)

public:
	CMFCButtonEx();
	virtual ~CMFCButtonEx();

	CMFCButtonEx& SetIcon(UINT nIDResource);
protected:
	DECLARE_MESSAGE_MAP()
};


