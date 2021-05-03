#pragma once

#include <map>
using namespace std;

class CIsoModelDocData
{
	CIsoModelDocData(void);
public:
	static CIsoModelDocData& GetInstance();
	~CIsoModelDocData(void);

	double GetCloudOffset();	/// 2011.12.22 added by humkyung

	int GetArcRadiusForCloudMark(double& dArcMin , double& dArcMax);	/// 2011.12.20 added by humkyung

	UInt32 GetCloudLevel() const;	/// 2011.11.18 added by humkyung
	UInt32 GetCloudColor() const;	/// 2011.11.18 added by humkyung
	double GetCloudHeight() const;   /// 2011.11.22 added by kyj
	double GetCloudWidth() const;    /// 2011.11.22 added by kyj
	CString GetRevisionNo(CDgnDocument* pDgnDoc) const; /// 2011.11.22 added by kyj 
	CString GetCloudDisplay() const;
	int GetArrowType() const;	/// 2011.11.16 added by humkyung
	
	double GetAngleToler() const;	/// 2011.11.02 added by humkyung
	
	map<CString , CString> m_oKeyValueMap;	/// 2011.10.25 added by humkyung

	bool IsPipeLineLevel( const UInt32& uiLevel ) const;
	bool IsPipeLineColor( const CString& sColor ) const;
	bool IsPipeLineWeight( const UInt32& uiWeight) const;

	bool IsPageConnectionString( const CString& sString ) const;	/// 2011.10.26 added by humkyung

	double GetOffset() const;
	/// up to here
	
	void ReadSetting(const CString& sIniFilePath);
public:
	double m_dScale;				/// 2011.11.17 added by humkyung
	CString m_sIniFilePath;			/// 2011.11.16 added by humkyung
	static double m_dDrawingToler;	/// 2011.11.16 added by humkyung
private:
	double m_dArcMinScale , m_dArcMaxScale;
	double m_dCloudOffsetScale;
};
