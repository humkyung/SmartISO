#pragma once

#include <vector>
using namespace std;

class CFileTools
{
private:
	CFileTools();
public:
	virtual ~CFileTools();

	static int FindFolders(vector<CString>& findfolders , const CString& path);
	static BOOL DoesFileExist(LPCTSTR _lpFullFileName);
	static BOOL GetExecutableDirectory(CString &_sDirName);
	static int CopyFiles(const CString& path , const CString& newPath , const CString& findname , const bool& bLookSubfolders = false);
	static int DeleteFiles(const CString& path , const CString& findname , const bool& bLookSubfolders = false);
	static int FindFiles(vector<CString>& findfiles , const CString& path , const CString& findname , const bool& bLookSubfolders);
};