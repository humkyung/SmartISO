// IDFFile.h: interface for the CIDFFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IDFFILE_H__A3D5ED3C_72C7_4A76_81CF_1385E5C7918A__INCLUDED_)
#define AFX_IDFFILE_H__A3D5ED3C_72C7_4A76_81CF_1385E5C7918A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <IsPoint3d.h>
#include <vector>
#include <fstream>
#include "pdTable122.h"
using namespace std;

class CIDFFileRecord
{
	CIDFFileRecord(const CIDFFileRecord& rhs){ UNUSED_ALWAYS(rhs); }
public:
	CIDFFileRecord(){}
	virtual ~CIDFFileRecord(){}

	//int Parse( const string& rdiscipline_indx_no , ifstream& ifile );
private:
	int m_nRecordIndex;
	double m_start_X;
	double m_start_Y;
	double m_start_Z;
public:
	int Parse(const string& aLine);
	int RecordIndex(void);
	double start_X(void);
	double start_Y(void);
	double start_Z(void);
	
};

class CIDFFile  
{
	CIDFFile(void);
public:
	int GetIDFFileTableRecordSize();
	int GetCoodinateList(vector<CIsPoint3d>& Point3dList);
	int Read(const string& rFilePath);
	static CIDFFile& GetInstance(void);
	virtual ~CIDFFile();
private:
	int Clear();
	vector<CIDFFileRecord*> m_RecordEntry;

};

#endif // !defined(AFX_IDFFILE_H__A3D5ED3C_72C7_4A76_81CF_1385E5C7918A__INCLUDED_)
