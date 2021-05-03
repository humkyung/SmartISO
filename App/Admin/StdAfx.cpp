// stdafx.cpp : source file that includes just the standard includes
//	Admin.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include "AdminDoc.h"
#include "StringTable.h"

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

/**
**/
void PrintError(const DWORD& dwErrorNo)
{
	LPVOID lpMsgBuf;
	if (!FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwErrorNo,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),// Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL ))
	{
		// Handle the error.
		return;
	}
	
	// Process any inserts in lpMsgBuf.
	// ...
	// Display the string.
	
	// 이곳을 바꾸면 오류를 적당히 처리 가능하다
	::MessageBox(NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
	// 이런 식으로...
	//printf("%s ", (LPCTSTR)lpMsgBuf);
	
	
	// Free the buffer.
	LocalFree( lpMsgBuf ); 
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-27
    @class
    @function   GetSDIActiveDocument
    @return     CAdminDoc*
    @brief
******************************************************************************/
CAdminDoc* GetSDIActiveDocument()
{
	CAdminDoc* pDoc = NULL;

	CWnd* pWndMain = AfxGetMainWnd();
	ASSERT(pWndMain);
	ASSERT(pWndMain->IsKindOf(RUNTIME_CLASS(CFrameWnd)) && 
		!pWndMain->IsKindOf(RUNTIME_CLASS(CMDIFrameWnd))); // Not an SDI app.

	pDoc = (CAdminDoc*)(((CFrameWnd*)pWndMain)->GetActiveDocument());

	return pDoc;
}

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