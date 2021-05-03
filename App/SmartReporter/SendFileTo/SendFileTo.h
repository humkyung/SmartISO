#ifndef __SENDFILETO_H__
#define __SENDFILETO_H__

#include "StdAfx.h"
#include <mapi.h>

class CSendFileTo
{
public:
	bool SendMail(HWND hWndParent, CStringA &strRecipient , CStringA &strAttachmentFileName, CStringA &strSubject = CStringA(_T("")))
	{
		if (strAttachmentFileName.IsEmpty())
			return false;

		if (!hWndParent || !::IsWindow(hWndParent))
			return false;

		HINSTANCE hMAPI = ::LoadLibraryA(LPCSTR("MAPI32.DLL"));
		if (!hMAPI)
			return false;

		ULONG (PASCAL *SendMail)(ULONG, ULONG_PTR, MapiMessage*, FLAGS, ULONG);
		(FARPROC&)SendMail = GetProcAddress(hMAPI, LPCSTR("MAPISendMail"));

		if (!SendMail)
			return false;

		char szFileName[_MAX_PATH];
		char szPath[_MAX_PATH];
		char szSubject[_MAX_PATH];
		::strcpy_s(szFileName, _MAX_PATH , LPSTR(strAttachmentFileName.GetString()));
		::strcpy_s(szPath, _MAX_PATH , LPSTR(strAttachmentFileName.GetString()));
		::strcpy_s(szSubject, _MAX_PATH , LPSTR(strSubject.GetString()));

		MapiFileDesc fileDesc;
		::ZeroMemory(&fileDesc, sizeof(fileDesc));
		fileDesc.nPosition = (ULONG)-1;
		fileDesc.lpszPathName = (LPSTR)(strAttachmentFileName.GetBuffer());///(szPath);
		fileDesc.lpszFileName = LPSTR(szFileName);

		MapiRecipDesc recipient;
		recipient.ulRecipClass = MAPI_TO;
		recipient.lpszName = (LPSTR)(strRecipient.GetBuffer());
		recipient.lpszAddress = "";
		recipient.ulEIDSize = 0;
		recipient.lpEntryID = NULL;

		MapiMessage message;
		::ZeroMemory(&message, sizeof(message));
		message.lpszSubject = LPSTR(szSubject);
		message.nFileCount = 1;
		message.lpFiles = &fileDesc;
		message.lpRecips = &recipient;
		message.nRecipCount = 1;

		int nError = SendMail(0, (ULONG_PTR)hWndParent, &message, MAPI_LOGON_UI|MAPI_DIALOG, 0);

		if (nError != SUCCESS_SUCCESS && nError != MAPI_USER_ABORT && nError != MAPI_E_LOGIN_FAILURE)
			return false;

		return true;
	}
};

#endif