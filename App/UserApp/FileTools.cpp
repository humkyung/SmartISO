#include "stdafx.h"
#include "FileTools.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CFileTools::CFileTools()
{}


CFileTools::~CFileTools()
{}


BOOL CFileTools::DoesFileExist(LPCTSTR _lpFullFileName)
{
	if(_lpFullFileName!=NULL)
	{
		HANDLE hFile;

		hFile= CreateFile(_lpFullFileName,
						  0, //GENERIC_READ,           // Open for reading
						  FILE_SHARE_READ,        // Share for reading
						  NULL,                   // No security
						  OPEN_EXISTING,          // Existing file only
						  FILE_ATTRIBUTE_NORMAL,  // Normal file
						  NULL);                  // No template file

		if (hFile != INVALID_HANDLE_VALUE)
		{
			CloseHandle(hFile);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CFileTools::GetExecutableDirectory(CString& _sDirName)
{
	BOOL bRet= FALSE;
	if(&_sDirName!=NULL)
	{
		int		iLength;
		LPTSTR	lpPtr= _sDirName.GetBuffer(512);
		if(lpPtr!=NULL)
		{
			iLength= ::GetModuleFileName(NULL, lpPtr, 512);
			_sDirName.ReleaseBuffer();
			if(iLength!=0)
			{
				iLength= _sDirName.GetLength();
				iLength--;
				while(iLength >0)
				{
					if(_sDirName.GetAt(iLength) == '\\')
						break;
					else
						iLength--;
				}
				_sDirName= _sDirName.Left(iLength);
				_sDirName+= "\\";
				bRet= TRUE;
			}
		}
	}
	return bRet;
}

/**
	@brief	폴더를 뒤져서 findname과 같은 이름을 가지는 파일들을 찾는다.

	@author HumKyung	

	@date 2009-08-28 오전 10:11:44	

	@param	

	@return		
*/
int CFileTools::FindFiles(vector<CString>& findfiles , const CString& path , const CString& findname , const bool& bLookSubfolders)
{
	CFileFind finder;
	bool working = finder.FindFile( path + _T("\\") + findname );
	while ( working )
	{
		working = finder.FindNextFile();

		if ( finder.IsDots() ) continue;

		if ( bLookSubfolders && finder.IsDirectory() )
		{
			/// 하위폴더를 뒤져본다.
			return CFileTools::FindFiles( findfiles , finder.GetFilePath() , findname , bLookSubfolders);
		}
		else
		{
			const CString curfile = finder.GetFileName();
			findfiles.push_back(finder.GetFilePath());
		}
	}

	return ERROR_SUCCESS;
}

/**
	@brief	매치하는 파일을 찾아 복사한다.

	@author HumKyung	

	@date 2011.01.20

	@param	path
	@param	findname
	@param	bLookSubfolders

	@return	int
*/
int CFileTools::CopyFiles(const CString& path , const CString& newPath , const CString& findname , const bool& bLookSubfolders)
{
	vector<CString> findfiles;
	CFileTools::FindFiles(findfiles , path , findname , bLookSubfolders);
	for(vector<CString>::iterator itr = findfiles.begin();itr != findfiles.end();++itr)
	{
		const int pos = itr->ReverseFind('\\');
		const CString sFileName = itr->Right(itr->GetLength() - pos - 1);
		::CopyFile((*itr) , CString(newPath + _T("\\") + sFileName) , FALSE);
	}

	return ERROR_SUCCESS;
}

/**
	@brief	매치하는 파일을 찾아 삭제한다.

	@author BHK	

	@date 2009-08-28 오전 10:07:58	

	@param	

	@return		
*/
int CFileTools::DeleteFiles(const CString& path , const CString& findname , const bool& bLookSubfolders)
{
	vector<CString> findfiles;
	CFileTools::FindFiles(findfiles , path , findname , bLookSubfolders);
	for(vector<CString>::iterator itr = findfiles.begin();itr != findfiles.end();++itr)
	{
		::DeleteFile(*itr);
	}

	return ERROR_SUCCESS;
}

/**
	@brief	폴더를 뒤져서 폴더를 찾는다.

	@author HumKyung	

	@date 2011.01.28

	@param	findfolders
	@param	path

	@return		
*/
int CFileTools::FindFolders(vector<CString>& findfolders , const CString& path)
{
	CFileFind finder;
	bool working = finder.FindFile( path + _T("\\*.*") );
	while ( working )
	{
		working = finder.FindNextFile();

		if ( finder.IsDots() ) continue;

		if ( finder.IsDirectory() )
		{
			const CString sFolderName = finder.GetFileTitle();

			findfolders.push_back( sFolderName );
		}
	}

	return ERROR_SUCCESS;
}