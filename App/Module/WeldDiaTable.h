// WeldDiaTable.h: interface for the CWeldDiaTable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WELDDIATABLE_H__1EF778C9_C3B7_4817_B055_1C46DD80AE76__INCLUDED_)
#define AFX_WELDDIATABLE_H__1EF778C9_C3B7_4817_B055_1C46DD80AE76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef MODULE_EXPORTS
	#define SMARTISO_MODULE_EXT_CLASS	__declspec(dllexport)
#else
	#define SMARTISO_MODULE_EXT_CLASS	__declspec(dllimport)
#endif

#include <vector>
using namespace std;

struct SMARTISO_MODULE_EXT_CLASS WeldDia
{
	CString* name;
	double   main_size;
	double   sub_size;
	CString* remark;
};

class SMARTISO_MODULE_EXT_CLASS CWeldDiaTable  
{
public:
	WeldDia* GetWeldDiaAt(const size_t& at);
	size_t GetWeldDiaCount() const;
	void Load(const CString &rFilePath);
	CWeldDiaTable();
	virtual ~CWeldDiaTable();
private:
	vector<WeldDia*>* m_pWeldDiaEntry;
};

#endif // !defined(AFX_WELDDIATABLE_H__1EF778C9_C3B7_4817_B055_1C46DD80AE76__INCLUDED_)
