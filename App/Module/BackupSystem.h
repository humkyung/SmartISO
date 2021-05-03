// BackupSystem.h: interface for the CBackupSystem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BACKUPSYSTEM_H__2635C846_43A1_4403_BAD4_9001EA63F69B__INCLUDED_)
#define AFX_BACKUPSYSTEM_H__2635C846_43A1_4403_BAD4_9001EA63F69B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef MODULE_EXPORTS
	#define SMARTISO_MODULE_EXT_CLASS	__declspec(dllexport)
#else
	#define SMARTISO_MODULE_EXT_CLASS	__declspec(dllimport)
#endif

class CProject;
class SMARTISO_MODULE_EXT_CLASS CBackupSystem  
{
public:
	BOOL Backup();
	
	CBackupSystem(CProject* pProject);
	virtual ~CBackupSystem();
private:
	const char* GetNewBackupFileName();
	CProject* m_pProject;
};

#endif // !defined(AFX_BACKUPSYSTEM_H__2635C846_43A1_4403_BAD4_9001EA63F69B__INCLUDED_)
