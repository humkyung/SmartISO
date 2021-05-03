// PdTable121.h: interface for the CPdTable121 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PDTABLE121_H__3D455F63_AB39_43CD_A360_B3176B2FA6BD__INCLUDED_)
#define AFX_PDTABLE121_H__3D455F63_AB39_43CD_A360_B3176B2FA6BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <fstream>
#include <set>
#include "PdTable122.h"

#include "soci.h"
#include "soci-sqlite3.h"

class CPdTable121Record
{
	CPdTable121Record(const CPdTable121Record& rhs){}
public:
	CPdTable121Record(){}
	virtual ~CPdTable121Record(){}

	int Parse(soci::row&);
private:
	STRING_T m_dwg_index_no;
	STRING_T m_drawing_no;       
	STRING_T m_drawing_title;         
	STRING_T m_drawing_file_spec;
	STRING_T m_path_name;        
	STRING_T m_network_address;
	STRING_T m_drawing_type;
public:
	STRING_T drawing_type() const { return m_drawing_type; }
	STRING_T dwg_index_no(void) const;
	STRING_T drawing_no(void) const;       
	STRING_T drawing_title(void) const;    
	STRING_T drawing_file_spec(void) const;
	STRING_T path_name(void) const;        
	STRING_T network_address(void) const; 
};

class CPdTable121  
{
	CPdTable121(void);
	CPdTable121(const CPdTable121& rhs){}
public:
	static CPdTable121& GetInstance(void);
	virtual ~CPdTable121(void);
private:
	vector<CPdTable121Record*> m_RecordEntry;
public:
	int Read(soci::session&);
	int FindDrawingTitle(set<string>& DrawingTitleEntry, set<string>& DwgIndexNoEntry);
	int FindDrawingNo(set<string>& DrawingNoEntry, set<string>& DwgIndexNoEntry);
	//int GetDisciplineNameFormIndexNo(vector<string>& DisciplineNameEntry , const string& rDisciplineName);
private:
	int Clear();
};

#endif // !defined(AFX_PDTABLE121_H__3D455F63_AB39_43CD_A360_B3176B2FA6BD__INCLUDED_)
