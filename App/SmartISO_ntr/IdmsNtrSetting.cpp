// IdmsNtrSetting.cpp: implementation of the CIdmsNtrSetting class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <windows.h>
#include "IdmsNtrSetting.h"
#include "tokenizer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIdmsNtrSetting::CIdmsNtrSetting() : m_x(0) , m_y(0) , m_font(1) , m_txtColor(0)
{
	m_rUnit = "INCH";
	m_WeldNoXPos[0] = m_WeldNoXPos[1] = m_WeldNoXPos[2] = m_WeldNoXPos[3] = 0;
	m_WeldNoFont = 50;
}

CIdmsNtrSetting::~CIdmsNtrSetting()
{

}

CIdmsNtrSetting& CIdmsNtrSetting::GetInstance()
{
	static CIdmsNtrSetting __instance;

	return (__instance);
}


/**	
	@brief	SmartISO_NTR용 세팅을 읽는다.

	@author	BHK

	@date	?

	@param	rIniFilePath	a parameter of type const string&

	@return	void	
*/
void CIdmsNtrSetting::Read(const string& rIniFilePath)
{
	TCHAR szBuf[MAX_PATH + 1]={'\0',};
	static const string rApp = "SmartISO_ntr";

	vector<string> oResult;
	if(GetPrivateProfileString(rApp.c_str(),"pos",NULL,szBuf,MAX_PATH , rIniFilePath.c_str()))
	{
		CTokenizer<CIsComma>::Tokenize(oResult,szBuf , CIsComma());
		m_x = atoi(oResult[0].c_str());
		m_y = atoi(oResult[1].c_str());
	}
	else
	{
		printf("can't get pos");
		///getchar();
	}

	if(GetPrivateProfileString(rApp.c_str(),"Weld No Area",NULL,szBuf,MAX_PATH , rIniFilePath.c_str()))
	{
		CTokenizer<CIsComma>::Tokenize(oResult,szBuf , CIsComma());
		if(4 == oResult.size())
		{
			DPoint3d pt;
			pt.x = atof(oResult[0].c_str());
			pt.y = atof(oResult[1].c_str());
			pt.z = 0.0;

			m_WeldNoVolume.Add(pt);
			
			pt.x = atof(oResult[2].c_str());
			pt.y = atof(oResult[3].c_str());
			pt.z = 0.0;
			m_WeldNoVolume.Add(pt);
		}
	}
	else
	{
		printf("can't get Weld No Area");
		///getchar();
	}

	if(GetPrivateProfileString(rApp.c_str(),"Weld No XPos",NULL,szBuf,MAX_PATH , rIniFilePath.c_str()))
	{
		CTokenizer<CIsComma>::Tokenize(oResult,szBuf , CIsComma());
		if(4 == oResult.size())
		{
			m_WeldNoXPos[0] = atoi(oResult[0].c_str());
			m_WeldNoXPos[1] = atoi(oResult[1].c_str());
			m_WeldNoXPos[2] = atoi(oResult[2].c_str());
			m_WeldNoXPos[3] = atoi(oResult[3].c_str());
		}
	}
	else
	{
		printf("can't get Weld No XPos");
		///getchar();
	}

	if(GetPrivateProfileString(rApp.c_str(),"Cell Pos",NULL,szBuf,MAX_PATH , rIniFilePath.c_str()))
	{
		CTokenizer<CIsComma>::Tokenize(oResult,szBuf , CIsComma());
		m_CellX = atoi(oResult[0].c_str());
		m_CellY = atoi(oResult[1].c_str());
		m_Celldistance = atoi(oResult[2].c_str());
	}
	else
	{
		printf("can't get Cell pos");
		///getchar();
	}

	if(GetPrivateProfileString(rApp.c_str(),"Font No",NULL,szBuf,MAX_PATH,rIniFilePath.c_str()))
	{
		m_font = atoi(szBuf);	
	}

	if(GetPrivateProfileString(rApp.c_str(),"Weld No Font No",NULL,szBuf,MAX_PATH,rIniFilePath.c_str()))
	{
		m_WeldNoFont = atoi(szBuf);	
	}

	if(GetPrivateProfileString(rApp.c_str(),"TextColor",NULL,szBuf,MAX_PATH,rIniFilePath.c_str()))
	{
		m_txtColor = atoi(szBuf);	
	}
	if(GetPrivateProfileString(rApp.c_str(),"SpecFileFolder",NULL,szBuf,MAX_PATH,rIniFilePath.c_str()))
	{
		m_rPCDFilePath = szBuf;	
	}
	if(GetPrivateProfileString(rApp.c_str(),"Cell Info Path",NULL,szBuf,MAX_PATH,rIniFilePath.c_str()))
	{
		m_rCellInfoPath = szBuf;	
	}

	if(GetPrivateProfileString(rApp.c_str(),"Cell direction",NULL,szBuf,MAX_PATH,rIniFilePath.c_str()))
	{
		m_Celldirection = szBuf;	
	}

	if(GetPrivateProfileString(rApp.c_str(),"DGN File Path",NULL,szBuf,MAX_PATH,rIniFilePath.c_str()))
	{
		m_rDGNFilePath = szBuf;	
	}
	if(GetPrivateProfileString(rApp.c_str(),"Cell Map Table",NULL,szBuf,MAX_PATH,rIniFilePath.c_str()))
	{
		m_rMapTablePath = szBuf;	
	}

	if(GetPrivateProfileString("SmartISO_ntr" , "BM Area" , NULL , szBuf , MAX_PATH , rIniFilePath.c_str()))
	{
		CTokenizer<CIsComma>::Tokenize(oResult , szBuf , CIsComma());
		if(4 == oResult.size())
		{
			DPoint3d pt; //8142,8184,11793,1749
			pt.x = atof(oResult[0].c_str());
			pt.y = atof(oResult[1].c_str());
			pt.z = 0.0;
			m_bm_volume.Add(pt);

			pt.x = atof(oResult[2].c_str());
			pt.y = atof(oResult[3].c_str());
			pt.z = 0.0;
			m_bm_volume.Add(pt);
		}
	}

	if(GetPrivateProfileString("SmartISO_ntr" , "Graphic Area" , NULL , szBuf , MAX_PATH , rIniFilePath.c_str()))
	{
		CTokenizer<CIsComma>::Tokenize(oResult , szBuf , CIsComma());
		if(4 == oResult.size())
		{
			DPoint3d pt; //8142,8184,11793,1749
			pt.x = atof(oResult[0].c_str());
			pt.y = atof(oResult[1].c_str());
			pt.z = 0.0;
			m_graphic_volume.Add(pt);

			pt.x = atof(oResult[2].c_str());
			pt.y = atof(oResult[3].c_str());
			pt.z = 0.0;
			m_graphic_volume.Add(pt);
		}
	}

	//! INCH or MM. 기본값은 INCH이다.
	if(GetPrivateProfileString(rApp.c_str() , "UNIT" , NULL , szBuf , MAX_PATH , rIniFilePath.c_str()))
	{
		m_rUnit = szBuf;
	}
	else
	{
		m_rUnit = "INCH";
	}

	//! 2009.10.28
	if(GetPrivateProfileString(rApp.c_str() , "SizeOnOff" , NULL , szBuf , MAX_PATH , rIniFilePath.c_str()))
	{
		m_bUseDependentFile = (0 == strcmp(szBuf , "ON")) ? true : false;
	}
	else
	{
		m_bUseDependentFile = false;
	}

	if(GetPrivateProfileString(rApp.c_str() , "SizeDependent" , NULL , szBuf , MAX_PATH , rIniFilePath.c_str()))
	{
		m_rDependentFilePath = szBuf;
	}

	//! 2010.06.07
	if(GetPrivateProfileString(rApp.c_str() , "ImpliedOnOff" , NULL , szBuf , MAX_PATH , rIniFilePath.c_str()))
	{
		m_bUseImpliedDataFile = (0 == strcmp(szBuf , "ON")) ? true : false;
	}
	else
	{
		m_bUseImpliedDataFile = false;
	}

	if(GetPrivateProfileString(rApp.c_str() , "ImpliedData" , NULL , szBuf , MAX_PATH , rIniFilePath.c_str()))
	{
		m_rImpliedDataFilePath = szBuf;
	}
}

int CIdmsNtrSetting::GetCellDistance() const
{
	return m_Celldistance;
}
