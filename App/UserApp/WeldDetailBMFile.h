// WeldDetailBMFile.h: interface for the CWeldDetailBMFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WELDDETAILBMFILE_H__6C8ED4BA_E351_4750_A5F3_2654FDCB9D09__INCLUDED_)
#define AFX_WELDDETAILBMFILE_H__6C8ED4BA_E351_4750_A5F3_2654FDCB9D09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <WeldDiaTable.h>
#include "QCCheck.h"

typedef struct SpecSizeTotal_
{
	CString rSpec;
	double	size;
	double	total;

	SpecSizeTotal_()
	{
		size  = 0;
		total = 0;
	}
	SpecSizeTotal_(const struct SpecSizeTotal_& unnamed)
	{
		rSpec = unnamed.rSpec;
		size  = unnamed.size;
		total = unnamed.total;
	}
}SpecSizeTotal;

class CWeldDetailBMFile  
{
public:
	void Clear();
	bool Write(const CString& rOuputFilePath);
	void Add(const CString& rHeader , CQCCheck& qcCheck);
	CWeldDetailBMFile();
	virtual ~CWeldDetailBMFile();
private:
	CString GetMatlItemGroup( const CString& rMatlCode ) const;
	void Add_Welding_SpecSize_Total(const int& weld_type , const double& f , const double& qty , const double& size);
	void InitInternalVariables();
	void SUB_SUMMARY_BY_MAT_Func();
	long GetWeldingLength(const double& length) const;
	void PIPE_CUTTING_LENGTH_CALCULATION_Func(CQCCheck& qcCheck);
	WeldDia* FindWeldDia(const CString& rMatlCode);
	CString GetMatlType(const int& matlCodeNum) const;
	int CheckMatlCodeNumForGALV(CString& rCode1 , CString& rCode2 , const CString& rMatlCode) const;
	int CheckMatlCodeNum(CString& rCode1 , CString& rCode2 , const CString& rMatlCode) const;
	CString m_rFileName;
	vector<CString> m_weldDetailEntry;
	
	double m_nSub_Summary_By_Material[6][4];
	double m_nTotal_Sum[6][4];
	vector<SpecSizeTotal> m_specSizeTotalEntry[2];
	CString m_rSpec;

	friend class CWeldDiaCheck;
};

#endif // !defined(AFX_WELDDETAILBMFILE_H__6C8ED4BA_E351_4750_A5F3_2654FDCB9D09__INCLUDED_)
