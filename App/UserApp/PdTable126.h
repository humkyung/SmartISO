// PdTable126.h: interface for the CPdTable126 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PDTABLE126_H__AB554898_D76F_402A_B9D9_ABF39E491F03__INCLUDED_)
#define AFX_PDTABLE126_H__AB554898_D76F_402A_B9D9_ABF39E491F03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CPdTable126  
{
	CPdTable126();
public:
	static CPdTable126& GetInstance();
	virtual ~CPdTable126();
public:
private:
	vector<string> m_DrawingTypeEntry;
};

#endif // !defined(AFX_PDTABLE126_H__AB554898_D76F_402A_B9D9_ABF39E491F03__INCLUDED_)
