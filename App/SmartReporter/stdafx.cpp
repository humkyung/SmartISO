
// stdafx.cpp : source file that includes just the standard includes
// LineControlManager.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include <Util/FileTools.h>
/*
	@brief	return exection path

	@author	humkyung

	@date	2011.04.30

	@param

	@return	string
*/
CString GetExecPath()
{
	CString sExecPath = _T("");

	TCHAR szBuf[MAX_PATH]={'\0' ,};
	::GetModuleFileName( NULL , szBuf , MAX_PATH );
	sExecPath = szBuf;
	const int at = sExecPath.ReverseFind('\\');
	if( -1 != at )
	{
		sExecPath = sExecPath.Left(at);
	}

	return sExecPath;
}

/*
	@brief	return common application data path

	@author	humkyung

	@date	2011.08.13

	@param
*/
CString GetAppDataPath()
{
	TCHAR szTemp[_MAX_PATH] = {0};

	// Application data path
	SHGetSpecialFolderPath(NULL, szTemp, CSIDL_COMMON_APPDATA, FALSE);
	CString sAppDataPath = szTemp;

	return sAppDataPath;
}

BOOL FileExist(const CString& rFilePath)
{
	return CFileTools::DoesFileExist(rFilePath);
}

COLORREF DarkenColor(COLORREF col,double factor)
{
	if(factor>0.0&&factor<=1.0)
	{
		BYTE red,green,blue,lightred,lightgreen,lightblue;
		red = GetRValue(col);
		green = GetGValue(col);
		blue = GetBValue(col);
		lightred = (BYTE)(red-(factor*red));
		lightgreen = (BYTE)(green-(factor*green));
		lightblue = (BYTE)(blue-(factor*blue));
		col = RGB(lightred,lightgreen,lightblue);
	}
	return(col);
}

//std::wstring StringToWString(const std::string& s)
//{
//	std::wstring temp(s.length(),L' ');
//	std::copy(s.begin(), s.end(), temp.begin());
//	return temp;
//}
//
//std::string WStringToString(const std::wstring& s)
//{
//	std::string temp(s.length(), ' ');
//	std::copy(s.begin(), s.end(), temp.begin());
//	return temp;
//}