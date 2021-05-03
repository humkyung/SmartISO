// Minball2dImpl.h: interface for the CMinball2dImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MINBALL2DIMPL_H__0D67545E_322E_47B9_8C02_41D909D495F4__INCLUDED_)
#define AFX_MINBALL2DIMPL_H__0D67545E_322E_47B9_8C02_41D909D495F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMinball2dImpl  
{
	CMinball2dImpl(const CMinball2dImpl& rhs){}	//! prevent copy constructure
public:
	bool IsEmpty() const;
	void Clear();
	double radius() const {	return m_dRadius; }
	DPoint3d center() const { return m_ptOrigin; }
	int CalculateFrom(const vector<DPoint3d>& pts);
	CMinball2dImpl();
	virtual ~CMinball2dImpl();
private:
	DPoint3d m_ptOrigin;
	double m_dRadius;
};

#endif // !defined(AFX_ISBOUNDINGBOX2D_H__0D67545E_322E_47B9_8C02_41D909D495F4__INCLUDED_)
