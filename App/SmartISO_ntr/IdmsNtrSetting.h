// IdmsNtrSetting.h: interface for the CIdmsNtrSetting class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IDMSNTRSETTING_H__3064ED1A_CE97_4FA8_88D3_DC7A0D0691B0__INCLUDED_)
#define AFX_IDMSNTRSETTING_H__3064ED1A_CE97_4FA8_88D3_DC7A0D0691B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <IsVolume.h>
#include <DgnV7Lib.h>
#include <string>
using namespace std;

class CIdmsNtrSetting  
{
	CIdmsNtrSetting();
public:
	void Read(const string& rIniFilePath);
	static CIdmsNtrSetting& GetInstance();
	virtual ~CIdmsNtrSetting();

	bool UseDependentFile() const
	{
		return m_bUseDependentFile;
	}
	string GetDependentFilePath() const
	{
		return m_rDependentFilePath;
	}
	bool UseImpliedDataFile() const
	{
		return m_bUseImpliedDataFile;
	}
	string GetImpliedDataFilePath() const
	{
		return m_rImpliedDataFilePath;
	}
public:
	int GetCellDistance() const;
	int m_x , m_y , m_CellX,m_CellY,m_Celldistance;
	int m_WeldNoXPos[4];
	int m_font , m_txtColor;
	int m_WeldNoFont;
	string m_rPCDFilePath;
	string m_rMapTablePath;
	string m_rDGNFilePath;
	string m_rCellInfoPath;
	string m_Celldirection;
	string m_rWFilePath;	//! W 파일 이름.
	CDgnVolume m_bm_volume , m_WeldNoVolume; 
	CDgnVolume m_graphic_volume;
	string m_rUnit;	//! 
private:
	bool m_bUseDependentFile;
	string m_rDependentFilePath;
	bool m_bUseImpliedDataFile;
	string m_rImpliedDataFilePath;
};

#endif // !defined(AFX_IDMSNTRSETTING_H__3064ED1A_CE97_4FA8_88D3_DC7A0D0691B0__INCLUDED_)
