// BackupSystem.cpp: implementation of the CBackupSystem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Module.h"
#include "Project.h"
#include "BackupSystem.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBackupSystem::CBackupSystem(CProject* pProject) : m_pProject(pProject)
{

}

CBackupSystem::~CBackupSystem()
{

}

/**
	\brief	새로운 BACKUP FILE NAME을 구한다.
	\author	백흠경
**/
const char* CBackupSystem::GetNewBackupFileName()
{
	static CString rRevFileName;

	CString rRevHistoryFileName = m_pProject->path() + CString("\\Database\\Backup\\Revision History.dat");
	
	char szBuf[MAX_PATH] = {'\0' ,};
	CString rApp("Revision History");
	if(GetPrivateProfileString(rApp , "Rev No" , "" , szBuf , MAX_PATH , rRevHistoryFileName))
	{
		rRevFileName.Format("%s.r%d" , m_pProject->name() , atoi(szBuf));
	}
	else
	{
		rRevFileName.Format("%s.r1" , m_pProject->name());
		WritePrivateProfileString(rApp , "Rev No" , "1" , rRevHistoryFileName);
	}

	return rRevFileName;
}

/**
	\author	백흠경
**/
BOOL CBackupSystem::Backup()
{
	CString rRevHistoryFileName = m_pProject->path() + CString("\\Database\\Backup\\Revision History.dat");

	CString rDatabaseFileName(m_pProject->GetDatabaseFilePath());
	CString rBackupFileName   = m_pProject->path() + CString("\\Database\\Backup\\") + GetNewBackupFileName();
	if(CopyFile(rDatabaseFileName , rBackupFileName , FALSE))
	{
		char szBuf[MAX_PATH + 1] = {'\0' ,};
		CString rApp("Revision History");
		if(GetPrivateProfileString(rApp , "Rev No" , "" , szBuf , MAX_PATH , rRevHistoryFileName))
		{
			CString rRevNo;
			rRevNo.Format("%d" , atoi(szBuf) + 1);
			WritePrivateProfileString(rApp , "Rev No" , rRevNo , rRevHistoryFileName);
		}
		return TRUE;
	}

	return FALSE;
}
