#include "StdAfx.h"
#include "SmartISOLog.h"
#include <IsTools.h>
#include "UserAppDocData.h"
#include <util/FileTools.h>

static OFSTREAM_T g_logfile;
static long g_logcnt = 0L;

/******************************************************************************
    @author     humkyung
    @date       2012-01-04
    @class
    @function   SATRT_LOG
    @return     void
    @brief
******************************************************************************/
void START_LOG()
{
	if(g_logfile.is_open()) g_logfile.close();
	
	g_logcnt = 0L;

	CString sLogFilePath = CAppDocData::GetExecPath() + _T("\\log.html");
	if(CFileTools::DoesFileExist(sLogFilePath)) ::DeleteFile(sLogFilePath);
	g_logfile.open(sLogFilePath.operator LPCTSTR());
	if(g_logfile.is_open())
	{
		CAppDocData* pDocData = CAppDocData::GetInstance();
		pDocData->appendHeader( g_logfile );
	}
}

/******************************************************************************
    @author     humkyung
    @date       2012-01-04
    @class
    @function   WRITE_LOG
    @return     void
    @param      const   STRING_T&
    @param      log
    @brief
******************************************************************************/
void WRITE_LOG(const STRING_T& log)
{
	if(g_logfile.is_open())
	{
		g_logfile << std::endl << _T("<tr>") << std::endl;
		g_logfile << _T("<td title=\"NO\">") << (g_logcnt++) << _T("</td>");
		g_logfile << _T("<td title=\"desc.\"><font color=red>") << log << _T("</font></td>");
		g_logfile << _T("</tr>") << std::endl;
	}
}

/******************************************************************************
    @author     humkyung
    @date       2012-01-04
    @class
    @function   CLOSE_LOG
    @return     void
    @brief
******************************************************************************/
void CLOSE_LOG()
{
	if(g_logfile.is_open())
	{
		g_logfile.flush();
		g_logfile.close();
	}
}

/******************************************************************************
    @author     humkyung
    @date       2012-01-04
    @class
    @function   LOG_COUNT
    @return     long
    @brief
******************************************************************************/
long LOG_COUNT()
{
	return g_logcnt;
}

/******************************************************************************
    @author     humkyung
    @date       2012-01-04
    @class
    @function   OPEN_LOG
    @return     void
    @brief
******************************************************************************/
void OPEN_LOG()
{
	if(g_logcnt > 0)
	{
		CString sLogFilePath = CAppDocData::GetExecPath() + _T("\\log.html");
		::ShellExecute(NULL , _T("open") , sLogFilePath , _T("log.html") , NULL , SW_SHOWNORMAL);
	}
}