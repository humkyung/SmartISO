// DataField.h: interface for the CDataField class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAFIELD_H__EBB42067_4A4C_46A4_B27E_0BE999206640__INCLUDED_)
#define AFX_DATAFIELD_H__EBB42067_4A4C_46A4_B27E_0BE999206640__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef MODULE_EXPORTS
	#define SMARTISO_MODULE_EXT_CLASS	__declspec(dllexport)
#else
	#define SMARTISO_MODULE_EXT_CLASS	__declspec(dllimport)
#endif

#include <string>
using namespace std;

class SMARTISO_MODULE_EXT_CLASS CDataField  
{
	CDataField();
public:
	void Copy(CDataField* rhs);
	bool CoincideWith(CDataField* p);
	const char* GetExcelCol() const;
	void SetExcelCol(const char* pExcelCol);
	CDataField* Clone();
	const char* value() const;
	void Set(const char* name , const char* desc , const char* value);
	static void DeleteInstance(CDataField* pLineDataField);
	static CDataField* CreateInstance();
	const char* desc() const;
	const char* name() const;
	
	virtual ~CDataField();
private:
	string* m_rFieldName;
	string* m_rFieldDesc;
	string* m_rFieldValue;
	
	string* m_pExcelCol;
};

#endif // !defined(AFX_DATAFIELD_H__EBB42067_4A4C_46A4_B27E_0BE999206640__INCLUDED_)
