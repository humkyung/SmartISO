// NtrData.h: interface for the CNtrData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NTRDATA_H__8969D557_9DB0_4C5B_BD65_C1D3C373C80A__INCLUDED_)
#define AFX_NTRDATA_H__8969D557_9DB0_4C5B_BD65_C1D3C373C80A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNtrData  
{
public:
	CString m_rLineNo, m_rSerialNo, m_rFieldShop, m_rFluidCode ,m_rMainSize , m_rSubSize, m_rMatlCode;
public:
	CString GetMatlCode() const;
	CString GetSubSize() const;
	CString GetMainSize() const;
	CString GetFluidCode() const;
	CString GetFieldShop() const;
	CString GetSerialNo() const;
	CString GetLineNo() const;
	CNtrData();
	CNtrData(const string& rNtrTextString) : m_rTextString(rNtrTextString)
	{
		ParseColumnIndex();
		///ParserText(m_rTextString);
	}
	CNtrData(const CNtrData& rhs);

	CNtrData& operator=(const CNtrData& rhs);
	virtual ~CNtrData();
	bool ParseText(string rTextString);
	const char* GetTextString() const;
private:
	void ParseColumnIndex();

	string m_rTextString;
	int m_nFieldPos[11];
};

#endif // !defined(AFX_NTRDATA_H__8969D557_9DB0_4C5B_BD65_C1D3C373C80A__INCLUDED_)
