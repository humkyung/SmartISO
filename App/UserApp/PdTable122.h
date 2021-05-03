// PdTable122.h: interface for the CPdTable122 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PDTABLE122_H__78896E0D_8F1A_49B7_B790_FD02182BFF15__INCLUDED_)
#define AFX_PDTABLE122_H__78896E0D_8F1A_49B7_B790_FD02182BFF15__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <set>
#include <fstream>
using namespace std;

#include "soci.h"
#include "soci-sqlite3.h"

class CPdTable122Record
{
	CPdTable122Record( const CPdTable122Record& rhs){ UNUSED_ALWAYS(rhs); }
public:
	CPdTable122Record(){}
	virtual ~CPdTable122Record(){}
	
	int Parse(soci::row&);
private:
	STRING_T m_dwg_view_index_no; 
	STRING_T m_dwg_view_no;
	STRING_T m_dwg_index_no; 
	int m_dwg_view_scale;
	double m_dwg_view_x_low;    
	double m_dwg_view_y_low;    
	double m_dwg_view_z_low;    
	double m_dwg_view_x_high;   
	double m_dwg_view_y_high;   
	double m_dwg_view_z_high; 

public:
	double dwg_view_z_high(void) const;
	double dwg_view_y_high(void) const;
	double dwg_view_x_high(void) const;
	double dwg_view_z_low(void) const;
	double dwg_view_y_low(void) const;
	double dwg_view_x_low(void) const;
	STRING_T dwg_index_no(void) const;
	STRING_T dwg_view_no(void) const;
	STRING_T dwg_view_index_no(void) const;
	int dwg_view_scale(void) const;

};

class CPdTable122  
{
	CPdTable122(void);
	CPdTable122(const CPdTable122& rhs){}
public:
	static CPdTable122& GetInstance(void);
	virtual ~CPdTable122(void);
private:
	vector<CPdTable122Record*> m_RecordEntry;
public:
	int FindDwgIndexNo(set<STRING_T>& DwgIndexNoEntry , const double& x, const double& y, const double& z);
	int Read(soci::session&);
private:
	int Clear();
};

#endif // !defined(AFX_PDTABLE122_H__78896E0D_8F1A_49B7_B790_FD02182BFF15__INCLUDED_)
