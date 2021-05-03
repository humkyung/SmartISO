#ifndef __ISO_BM_DATA_H__
#define __ISO_BM_DATA_H__

#include <DgnV7Lib.h>

class CIsoBMData
{
	STRING_T ParseSizeNum( const STRING_T& rString );
public:
	
	int SetDGNTextString(const STRING_T& str);
	void SetDGNText2D(CDgnText* p);
	CDgnText* GetDGNText2D()
	{
		return m_pDGNText2d;
	}
	void UpdateBMCSForDGNText2d();
	bool Parse();
	void AddSubText(CDgnText* pDgnText2d , const string& rString);
	explicit CIsoBMData(const CIsPoint2d& origin , const char* p) : m_pDGNText2d(NULL)
	{
		m_origin = origin;
		m_rDataString = p;
	}

	CIsoBMData(const CIsoBMData& data) : m_subTextEntry(data.m_subTextEntry)
	{
		m_origin = data.m_origin;
		m_rMatlDesc = data.m_rMatlDesc;
		m_rNPD[0] = data.m_rNPD[0];
		m_rNPD[1] = data.m_rNPD[1];
		m_rMatlSpec = data.m_rMatlSpec;
		m_rBMCSSYM = data.m_rBMCSSYM;
		m_rMatlCode = data.m_rMatlCode;
		m_rQTY = data.m_rQTY;
		m_rDataString = data.m_rDataString;
		m_pDGNText2d = data.m_pDGNText2d;
	}
	
	void operator =(const char* p)
	{
		m_rDataString = p;
	}

	string dataString() const
	{
		return m_rDataString;
	}

	/// NO          MATERIALS DESCRIPTION            (IN) or (MM)         SPEC    SYM       CODE              QTY 
	static int m_nFieldPos[7];
	string m_rMatlDesc;
	string m_rMatlSpec;
	string m_rBMCSSYM;
	string m_rMatlCode;
	string m_rQTY;

	string matlDesc() const
	{
		return m_rMatlDesc;
	}

	string NPD1() const
	{
		return m_rNPD[0];
	}

	string NPD2() const
	{
		return m_rNPD[1];
	}

	string matlSpec() const
	{
		return m_rMatlSpec;
	}
	
	string bmcsSYM() const
	{
		return m_rBMCSSYM;
	}

	string matlCode() const
	{
		return m_rMatlCode;
	}

	string QTY() const
	{
		return m_rQTY;
	}

	CIsPoint2d origin() const
	{
		return m_origin;
	}
	map<CDgnText*,string> m_subTextEntry;
private:
	string m_rNPD[2];
	CDgnText* m_pDGNText2d;
	
	CIsPoint2d m_origin;
	string m_rDataString;
};

#endif