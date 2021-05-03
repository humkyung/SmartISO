// stdafx.cpp : source file that includes just the standard includes
//	UserApp.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include "tokenizer.h"
#include <IsRegExp.h>
#include "StringTable.h"

int (* SmartISODiff)(const LPCTSTR ,const LPCTSTR , const LPCTSTR) = NULL;

bool __load_user_defined_table = false;

// A few basic colors
const COLORREF black = RGB( 0, 0, 0 );
const COLORREF white = RGB( 255, 255, 255 );
const COLORREF green = RGB( 0, 255, 0 );
const COLORREF red = RGB( 255, 0, 0 );
const COLORREF blue = RGB( 0, 0, 255 );
const COLORREF yellow = RGB( 255, 255, 0 );
const COLORREF magenta = RGB( 255, 0, 255 );
const COLORREF cyan = RGB( 0, 255, 255 );

/// Default color scheme
SScintillaColors g_rgbSyntaxCpp[] = 
{
	{	SCE_C_COMMENT,		green },
	{	SCE_C_COMMENTLINE,	green },
	{	SCE_C_COMMENTDOC,	green },
	{	SCE_C_NUMBER,		RGB(0 , 128 , 0) },
	{	SCE_C_STRING,		magenta },
	{	SCE_C_CHARACTER,	yellow },
	{	SCE_C_UUID,			cyan },
	{	SCE_C_OPERATOR,		red },
	{	SCE_C_PREPROCESSOR,	cyan },
	{	SCE_C_WORD,			cyan },
	{	-1,				0 }
};

/******************************************************************************
    @author     humkyung
    @date       2011-10-27
    @class
    @function   GetSDIActiveDocument
    @return     CUserAppDoc*
    @brief
******************************************************************************/
CUserAppDoc* GetSDIActiveDocument()
{
	CUserAppDoc* pDoc = NULL;

	CWnd* pWndMain = AfxGetMainWnd();
	ASSERT(pWndMain);
	ASSERT(pWndMain->IsKindOf(RUNTIME_CLASS(CFrameWndEx)) && 
		!pWndMain->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd))); // Not an SDI app.

	pDoc = (CUserAppDoc*)(((CFrameWndEx*)pWndMain)->GetActiveDocument());

	return pDoc;
}

/**	\brief	두 개의 시간을 비교한다.

	\return 
	rCurrentTime이 최근의 시간이면 TRUE, 그렇지 않으면 FALSE
**/
BOOL CompareTwoTimes(const CString& rCurrentTime , const CString rScheduleTime)
{
	vector<string> oResult;
	CTokenizer<CIsFromString>::Tokenize(oResult , rCurrentTime.operator const char*() , CIsFromString("-"));
	
	int nYers[2] , nMonths[2] , nDates[2] , nHours[2] , nMinutes[2];
	if(5 == oResult.size())
	{
		nYers[0] = atoi(oResult[0].c_str());
		nMonths[0] = atoi(oResult[1].c_str());
		nDates[0] = atoi(oResult[2].c_str());
		nHours[0] = atoi(oResult[3].c_str());
		nMinutes[0] = atoi(oResult[4].c_str());
	}
	
	CTokenizer<CIsFromString>::Tokenize(oResult , rScheduleTime.operator const char*() , CIsFromString("-"));
	if(5 == oResult.size())
	{
		nYers[1] = atoi(oResult[0].c_str());
		nMonths[1] = atoi(oResult[1].c_str());
		nDates[1] = atoi(oResult[2].c_str());
		nHours[1] = atoi(oResult[3].c_str());
		nMinutes[1] = atoi(oResult[4].c_str());
	}
	
	if( nYers[0] > nYers[1] )
	{
		return TRUE;
	}
	else if( nYers[0] == nYers[1])
	{
		if( nMonths[0] > nMonths[1] )
		{
			return TRUE;
		}
		else if( nMonths[0] == nMonths[1])
		{
			if( nDates[0] > nDates[1] )
			{
				return TRUE;
			}
			else if( nDates[0] == nDates[1])
			{
				if( nHours[0] > nHours[1])
				{
					return TRUE;
				}
				else if( nHours[0] == nHours[1])
				{
					if(nMinutes[0] >= nMinutes[1])
					{
						return TRUE;
					}
				}
			}
		}
	}
	return FALSE;
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

void TrimRightZeroString( CString& rString )
{
	int at = rString.Find(".");
	if( -1 != at ) rString.TrimRight("0");
	if("." == rString.Right(1)) rString.TrimRight(".");
}

enum
{
	FIND_MATCH_CASE		= 0x0001,
	FIND_WHOLE_WORD		= 0x0002,
	FIND_DIRECTION_UP	= 0x0010,
	REPLACE_SELECTION	= 0x0100
};

IsRegExp::RxNode __declspec(dllimport) *IsRxCompile(LPCTSTR Regexp);
int __declspec(dllimport) IsRxExec(IsRegExp::RxNode *Regexp, LPCTSTR Data, int Len, LPCTSTR Start, IsRegExp::RxMatchRes *Match, unsigned int RxOpt);
int __declspec(dllimport) IsRxReplace(LPCTSTR rep, LPCTSTR Src, int len, IsRegExp::RxMatchRes match, LPTSTR *Dest, int *Dlen);
void __declspec(dllimport) IsRxFree(IsRegExp::RxNode *Node);
int FindStringHelper (LPCTSTR pszFindWhere, LPCTSTR pszFindWhat, DWORD dwFlags, int &nLen, IsRegExp::RxNode *&rxnode, IsRegExp::RxMatchRes *rxmatch)
{
	int pos;
	
	if (rxnode)
		IsRxFree (rxnode);
	rxnode = IsRxCompile (pszFindWhat);
	if (rxnode && IsRxExec (rxnode, pszFindWhere, _tcslen (pszFindWhere), pszFindWhere, rxmatch, (dwFlags & FIND_MATCH_CASE) != 0 ? RX_CASE : 0))
	{
		pos = rxmatch->Open[0];
		nLen = rxmatch->Close[0] - rxmatch->Open[0];
	}
	else
	{
		pos = -1;
	}
	return pos;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Executes the given command using CreateProcess() and WaitForSingleObject().
// Returns FALSE if the command could not be executed or if the exit code could not be determined.
BOOL ExecuteCommandLine(CString cmdLine, DWORD & exitCode)
{
	PROCESS_INFORMATION processInformation = {0};
	STARTUPINFO startupInfo                = {0};
	startupInfo.cb                         = sizeof(startupInfo);
	int nStrBuffer                         = cmdLine.GetLength() + 50;

	// Create the process
	BOOL result = CreateProcess(NULL, cmdLine.GetBuffer(nStrBuffer), 
		NULL, NULL, FALSE, 
		NORMAL_PRIORITY_CLASS | CREATE_NO_WINDOW, 
		NULL, NULL, &startupInfo, &processInformation);
	cmdLine.ReleaseBuffer();

	if (!result)
	{
		// CreateProcess() failed
		// Get the error from the system
		LPVOID lpMsgBuf;
		DWORD dw = GetLastError();
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, 
			NULL, dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL);

		// Display the error
		/*CString strError = (LPTSTR)lpMsgBuf;
		CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->GetMainWnd());
		if(pFrame) pFrame->DisplayMessage(strError);*/

		//TRACE(_T("::ExecuteCommandLine() failed at CreateProcess()\nCommand=%s\nMessage=%s\n\n"), cmdLine, strError);

		// Free resources created by the system
		LocalFree(lpMsgBuf);

		// We failed.
		return FALSE;
	}
	else
	{
		// Successfully created the process.  Wait for it to finish.
		WaitForSingleObject( processInformation.hProcess, INFINITE );

		// Get the exit code.
		result = GetExitCodeProcess(processInformation.hProcess, &exitCode);

		// Close the handles.
		CloseHandle( processInformation.hProcess );
		CloseHandle( processInformation.hThread );

		if (!result)
		{
			// Could not get exit code.
			TRACE(_T("Executed command but couldn't get exit code.\nCommand=%s\n"), cmdLine);
			return FALSE;
		}

		// We succeeded.
		return TRUE;
	}
}

/**
	@brief	translate given str for locale
	@author	humkyung
	@date	2015.08.29
	@return	CString
*/
CString _TR(const CString& str)
{
	CStringTable& table = CStringTable::GetInstance();
	return table.GetString(str.operator LPCTSTR()).c_str();
}