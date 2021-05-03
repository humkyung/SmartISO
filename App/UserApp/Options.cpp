//FileZilla - a Windows ftp client

// Copyright (C) 2002-2004 - Tim Kosse <tim.kosse@gmx.de>

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

// Options.cpp: Implementierungsdatei
//

#include "stdafx.h"
#include "options.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

COptions::t_OptionsCache COptions::m_OptionsCache[OPTIONS_NUM];

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld COptions

COptions::COptions()
{
	//{{AFX_DATA_INIT(COptions)
		// HINWEIS: Der Klassen-Assistent f?t hier Elementinitialisierung ein
	//}}AFX_DATA_INIT
}

/////////////////////////////////////////////////////////////////////////////
// Behandlungsroutinen f? Nachrichten COptions

typedef struct
{
	TCHAR name[30];
	int nType;
} t_Option;

static t_Option options[OPTIONS_NUM]={ _T("Logontype"),					1,	_T("Pasv"),								1,
									_T("FWHost"),						0,	_T("FWPort"),							1,
									_T("FWUser"),						0,	_T("FWPass"),							0,
									_T("Timeout Length"),				1,	_T("Use Keep Alive"),					1,
									_T("Interval Low"),					1,	_T("Interval High"),					1,
									_T("Num Retries"),					1,	_T("Retry Delay"),						1,
									_T("Last Server Host"),				0,	_T("Last Server Port"),					1,
									_T("Last Server User"),				0,	_T("Last Server Pass"),					0,
									_T("Proxy Type"),					1,	_T("Proxy Host"),						0,
									_T("Proxy Port"),					1,	_T("Proxy User"),						0,
									_T("Proxy Pass"),					0,	_T("Proxy Use Logon"),					1,
									_T("Last Server Path"),				0,	_T("Last Server Firewall Bypass"),		1,
									_T("Language"),						0,	_T("Default Folder Type"),				1,
									_T("Default Folder"),				0,	_T("Transfer Mode"),					1,
									_T("ASCII files"),					0,	_T("No view labels"),					1,
									_T("No toolbar"),					1,	_T("No Quickconnbar"),					1,
									_T("No Statusbar"),					1,	_T("No Messagelog"),					1,
									_T("No Treeview"),					1,	_T("No Queue"),							1,
									_T("Remember View settings"),		1,	_T("Local List View Style"),			1,
									_T("Hide Local Columns"),			1,	_T("Remote List View Style"),			1,
									_T("Hide Remote Columns"),			1,	_T("Sitemanager on startup"),			1,
									_T("Use GSS"),						1,	_T("GSS enabled servers"),				0,
									_T("Last Server dont save pass"),	1,	_T("Remember Window Size"),				1,
									_T("Last Window Size"),				0,	_T("Debug Trace"),						1,
									_T("Debug Show Listing"),			1,	_T("Debug Log To File"),				1,
									_T("Debug Log File"),				0,	_T("Limit Port Range"),					1,
									_T("Port Range Low"),				1,	_T("Port Range High"),					1,
									_T("Remember Local View"),			1,	_T("Remember Remote View"),				1,
									_T("Last Splitter Pos"),			0,	_T("Use directory cache"),				1,
									_T("Max Dir Cache Time"),			1,	_T("Last Server Type"),					1,
									_T("Minimize to Tray"),				1,	_T("Show Remote Treeview"),				1,
									_T("Remember Local column widths"),	1,	_T("Remember Remote column widths"),	1,
									_T("Local column widths"),			0,	_T("Remote column widths"),				0,
									_T("Local Filesize format"),		1,	_T("Remote Filesize format"),			1,
									_T("Last server name"),				0,	_T("Preserve DL file time"),			1,
									_T("Run in Secure Mode"),			0,	_T("Default View Edit Prog"),			0,
									_T("Custom View Edit Progs"),		0,	_T("Always Show Hidden Files"),			1,
									_T("Primary TransferMax Size"),		1,	_T("Enable debug menu"),				1,
									_T("Transfer Api Count"),			1,	_T("Use Registry"),						1,
									_T("Use multiple connections"),		1,	_T("Use Ident server"),					1,
									_T("Ident when connecting only"),	1,	_T("Ident reply same IP"),				1,
									_T("Ident OS"),						0,	_T("Ident UserID"),						0,
									_T("File exists Action"),			1,	_T("SpeedLimit Download Type"),			1,
									_T("SpeedLimit Upload Type"),		1,	_T("SpeedLimit Download Value"),		1,
									_T("SpeedLimit Upload Value"),		1,	_T("SpeedLimit Download Rules"),		0,
									_T("SpeedLimit Upload Rules"),		0,	_T("SiteManager Folders First"),		1,
									_T("No Expand SiteManager Folders"),1,	_T("Show Local Statusbar"),				1,
									_T("Show Remote Statusbar"),		1,	_T("Local column sort direction"),		1,
									_T("Remote column sort direction"),	1,	_T("SSH Use Compression"),				1,
									_T("SSH Force Protocol"),			1,	_T("Transfer IP"),						0,
									_T("Local doubleclick action"),		1,	_T("Remote doubleclick action"),		1,
									_T("Local treeview location"),		1,	_T("Remote treeview location"),			1,
									_T("Last Server transfer mode"),	1,  _T("Use custom messaglog font"),		1,
									_T("Messagelog font name"),			0,	_T("Messagelog font size"),				1,
									_T("Switch view locations"),		1,	_T("Use MODE Z "),						1,
									_T("MODE Z level"),					1,	_T("Transfer IP v6"),					0,
									_T("Enable IPv6"),					1,	_T("Log Timestamps"),					1,
									_T("VMS display all revisions"),	1,	_T("Last Server UTF8"),					1,
									_T("Anonymous password"),			0,	_T("Last Server account"),				0,
									_T("Last Server Timezone"),			1
								};

void COptions::SetOption(int nOptionID,int value)
{
	ASSERT(options[nOptionID-1].nType==1);

	m_OptionsCache[nOptionID-1].bCached=TRUE;
	m_OptionsCache[nOptionID-1].createtime=CTime::GetCurrentTime();
	m_OptionsCache[nOptionID-1].value=value;
	m_OptionsCache[nOptionID-1].nType=1;

	unsigned char tmp[4];
	memcpy(tmp,&value,4);
	HKEY key;
	if (RegCreateKeyEx(HKEY_CURRENT_USER, _T("Software\\SmartISO"), 0, 0, REG_OPTION_NON_VOLATILE, KEY_WRITE, 0, &key, NULL)==ERROR_SUCCESS)
	{
		RegSetValueEx(key, options[nOptionID-1].name, 0, REG_DWORD, tmp, 4);
		RegCloseKey(key);
	}
}

void COptions::SetOption(int nOptionID, CString value)
{
	ASSERT(!options[nOptionID-1].nType);

	m_OptionsCache[nOptionID-1].bCached=TRUE;
	m_OptionsCache[nOptionID-1].createtime=CTime::GetCurrentTime();
	m_OptionsCache[nOptionID-1].str=value;
	m_OptionsCache[nOptionID-1].nType=0;

	LPTSTR tmp=new TCHAR[value.GetLength()+1];
	LPCTSTR str=value;
	_tcscpy(tmp,str);
	HKEY key;
	if (RegCreateKeyEx(HKEY_CURRENT_USER, _T("Software\\SmartISO"), 0, 0, REG_OPTION_NON_VOLATILE, KEY_WRITE, 0, &key, NULL)==ERROR_SUCCESS)
	{
		RegSetValueEx(key, options[nOptionID-1].name, 0, REG_SZ, (unsigned char *)tmp, (_tcslen(tmp)+1)*sizeof(TCHAR) );
		RegCloseKey(key);
	}
	delete [] tmp;
}

CString COptions::GetOption(int nOptionID)
{
	ASSERT(!options[nOptionID-1].nType);

	CString res = "";
	if (m_OptionsCache[nOptionID-1].bCached)
	{
		ASSERT(!m_OptionsCache[nOptionID-1].nType);
		CTimeSpan span=CTime::GetCurrentTime()-m_OptionsCache[nOptionID-1].createtime;
		if (span.GetTotalSeconds() < 120)
			res=m_OptionsCache[nOptionID-1].str;
		else
			m_OptionsCache[nOptionID-1].bCached=FALSE;
	}
	if (!m_OptionsCache[nOptionID-1].bCached)
	{
		HKEY key;
		unsigned long tmp=2000;
		unsigned char *buffer=new unsigned char[2000];
		BOOL exists = FALSE;

		if (RegOpenKey(HKEY_CURRENT_USER, _T("Software\\SmartISO"), &key)==ERROR_SUCCESS)
		{
			memset(buffer, 0, 2000);

			if (RegQueryValueEx(key, options[nOptionID-1].name, NULL, NULL, buffer, &tmp)==ERROR_SUCCESS)
			{
				exists=TRUE;
				res=(LPTSTR)buffer;
			}
			RegCloseKey(key);
		}

		if (!exists || (!_ttoi(res) && nOptionID==OPTION_RUNINSECUREMODE))
		{
			tmp = 2000;
			memset(buffer, 0, 2000);
			if (RegOpenKey(HKEY_LOCAL_MACHINE, _T("Software\\SmartISO") ,&key)==ERROR_SUCCESS)
			{
				if (RegQueryValueEx(key, options[nOptionID-1].name, NULL, NULL, buffer, &tmp)==ERROR_SUCCESS)
					res = (LPTSTR)buffer;
				RegCloseKey(key);
			}
		}
		delete [] buffer;
	}
	if (res == "")
	{
		if (nOptionID == OPTION_ASCIIFILES)
			res = "ASP;BAT;C;CFM;CGI;CONF;CPP;CSS;DHTML;DIZ;H;HPP;HTM;HTML;INC;JS;MAK;NFO;PAS;PATCH;PHP;PHTML;PINERC;PL;QMAIL;SH;SHTML;SQL;TCL;TPL;TXT;VBS;";
		else if (nOptionID == OPTION_GSSSERVERS)
			res = _T("mit.edu;");
		else if (nOptionID == OPTION_IDENTSYSTEM)
			res = _T("UNIX");
		else if (nOptionID == OPTION_ANONPWD)
			res = _T("anon@");
	}
	if (!m_OptionsCache[nOptionID-1].bCached)
	{
		m_OptionsCache[nOptionID-1].bCached=TRUE;
		m_OptionsCache[nOptionID-1].createtime=CTime::GetCurrentTime();
		m_OptionsCache[nOptionID-1].str=res;
		m_OptionsCache[nOptionID-1].nType=0;
	}
	return res;
}

int COptions::GetOptionVal(int nOptionID)
{
	ASSERT(options[nOptionID-1].nType==1);

	int val=0;
	if (m_OptionsCache[nOptionID-1].bCached)
	{
		ASSERT(m_OptionsCache[nOptionID-1].nType==1);
		CTimeSpan span=CTime::GetCurrentTime()-m_OptionsCache[nOptionID-1].createtime;
		if (span.GetTotalSeconds() < 120)
			return m_OptionsCache[nOptionID-1].value;
		else
			m_OptionsCache[nOptionID-1].bCached=FALSE;
	}
	int error=1;
	if (!m_OptionsCache[nOptionID-1].bCached)
	{
		error=1;
		HKEY key;
		unsigned char *buffer = new unsigned char[200];
		unsigned long tmp=100;

		if (RegOpenKey(HKEY_CURRENT_USER, _T("Software\\SmartISO") ,&key)==ERROR_SUCCESS)
		{
			DWORD type;
			if (RegQueryValueEx(key, options[nOptionID-1].name, NULL, &type, buffer, &tmp)==ERROR_SUCCESS)
			{
				if (type == REG_DWORD)
				{
					if (tmp<=4)
					{
						memcpy(&val,buffer,tmp);
						error=0;
					}
				}
				else
				{
					int i;
					LPTSTR str = (LPTSTR)buffer;
					for (i = 0; i < 100; i++)
					{
						if (!str[i])
							break;
					}
					if (i == 100)
						str[99] = 0;

					val = _ttoi(str);
					error = 0;
				}
			}
			RegCloseKey(key);
		}

		if (error)
		{
			tmp = 100;
			memset(buffer,0,100);
			if (RegOpenKey(HKEY_LOCAL_MACHINE, _T("Software\\SmartISO") ,&key)==ERROR_SUCCESS)
			{
				DWORD type;
				if (RegQueryValueEx(key, options[nOptionID-1].name, NULL, &type, buffer, &tmp)==ERROR_SUCCESS)
				{
					if (type == REG_DWORD)
					{
						if (tmp<=4)
						{
							memcpy(&val,buffer,tmp);
							error=0;
						}
					}
					else
					{
						int i;
						LPTSTR str = (LPTSTR)buffer;
						for (i = 0; i < 100; i++)
						{
							if (!str[i])
								break;
						}
						if (i == 100)
							str[99] = 0;

						val = _ttoi(str);
						error = 0;
					}
				}
				RegCloseKey(key);
			}
		}
		delete [] buffer;
	}
	switch (nOptionID)
	{
		case OPTION_PASV:
			if (error)
				val = 1;
			break;
		case OPTION_KEEPALIVE:
			if (error)
				val=1;
			break;
		case OPTION_INTERVALLOW:
			if (val>500)
				val=15;
			else if (val<15)
				val=15;
			break;
		case OPTION_INTERVALHIGH:
			if (val>500)
				val=30;
			else if (val<15)
				val=30;
			break;
		case OPTION_TIMEOUTLENGTH:
			if (val<30)
				val=30;
			else if (val>999)
				val=30;
			break;
		case OPTION_NUMRETRIES:
			if (val<0 || error)
				val=5;
			else if (val>999)
				val=5;
			break;
		case OPTION_RETRYDELAY:
			if (val<0 || error)
				val=5;
			else if (val>999)
				val=5;
			break;
		case OPTION_DEFAULTFOLDERTYPE:
			if (val!=1)
				val=0;
			break;
		case OPTION_TRANSFERMODE:
			if (val<0 || val>2)
				val=0;
			break;
		case OPTION_PORTRANGELOW:
			if (val<1 || val>65535)
				val=1;
			break;
		case OPTION_PORTRANGEHIGH:
			if (val<1 || val>65535)
				val=65535;
			break;
		case OPTION_MAXCACHETIME:
			if (val<=0)
				val=60*30;
			break;
		case OPTION_LOCALFILESIZEFORMAT:
			if (error || val<0 || val >3)
				val=0;
			break;
		case OPTION_REMOTEFILESIZEFORMAT:
			if (error || val<0 || val >3)
				val=1;
			break;
		case OPTION_USECACHE:
			if (error)
				val=1;
			break;
		case OPTION_TRANSFERPRIMARYMAXSIZE:
			if (error || val < 0 || val > 999999)
				val=1024;
			break;
		case OPTION_TRANSFERAPICOUNT:
			if (error || val < 1 || val > 10)
				val=2;
			break;
		case OPTION_TRANSFERUSEMULTIPLE:
			if (error)
				val=1;
			break;
		case OPTION_FILEEXISTSACTION:
			if (val<0 || val>5)
				val=0;
			break;
		case OPTION_SPEEDLIMIT_DOWNLOAD_VALUE:
		case OPTION_SPEEDLIMIT_UPLOAD_VALUE:
			if (error || val<=0)
				val=10;
			break;
		case OPTION_SHOWLOCALSTATUSBAR:
		case OPTION_SHOWREMOTESTATUSBAR:
			if (error)
				val = 1;
			break;
		case OPTION_HIDEREMOTECOLUMNS:
			if (error)
				val = 0x20;
			break;
		case OPTION_SSHUSECOMPRESSION:
		case OPTION_SSHPROTOCOL:
			if (val < 0 || val > 2)
				val = 0;
			break;
		case OPTION_MINIMIZETOTRAY:
			if (val < 0 || val > 2)
				val = 0;
			break;
		case OPTION_LOCAL_DOUBLECLICK_ACTION:
		case OPTION_REMOTE_DOUBLECLICK_ACTION:
			if (val < 0 || val > 2)
				val = 0;
			break;
		case OPTION_LOCALTREEVIEWLOCATION:
		case OPTION_REMOTETREEVIEWLOCATION:
			if (val < 0 || val > 1)
				val = 0;
			break;
		case OPTION_USEREGISTRY:
			if (error || val < 0 || val > 2)
				val = -1;
			break;
		case OPTION_LASTSERVERTRANSFERMODE:
			if (error || val < 0 || val > 2)
				val = 0;
			break;
		case OPTION_MESSAGELOG_FONTSIZE:
			if (error || val < 1 || val > 72)
				val = 0;
			break;
		case OPTION_MODEZ_USE:
			if (error || val < 0 || val > 2)
				val = 2;
			break;
		case OPTION_MODEZ_LEVEL:
			if (error || val < 1 || val > 9)
				val = 8;
			break;
	}
	if (!m_OptionsCache[nOptionID-1].bCached)
	{
		m_OptionsCache[nOptionID-1].bCached = TRUE;
		m_OptionsCache[nOptionID-1].createtime = CTime::GetCurrentTime();
		m_OptionsCache[nOptionID-1].value = val;
		m_OptionsCache[nOptionID-1].nType = 1;
	}

	return val;
}

void COptions::GetKey(HKEY key, CString keyname, CString &value)
{
	unsigned char *buffer=new unsigned char[1000];

	unsigned long tmp=1000;
	if (RegQueryValueEx(key, keyname, NULL, NULL, buffer, &tmp)!=ERROR_SUCCESS)
		value="";
	else
		value=(LPTSTR)buffer;
	delete [] buffer;
	return;
}

void COptions::GetKey(HKEY key, CString keyname, int &value)
{
	__int64 val2;
	GetKey(key, keyname, val2);
	value=static_cast<int>(val2);
}

void COptions::GetKey(HKEY key, CString keyname, __int64 &value)
{
	unsigned char *buffer=new unsigned char[1000];

	unsigned long tmp=1000;
	if (RegQueryValueEx(key,keyname,NULL,NULL,buffer,&tmp)!=ERROR_SUCCESS)
		value=0;
	else
	{
		if (_tcslen((LPTSTR)buffer)>20)
			value=0;
		else
			value=_ttoi64((LPTSTR)buffer);
	}
	delete [] buffer;
	return;
}

void COptions::SetKey(HKEY key, CString keyname, LPCTSTR value)
{
	RegSetValueEx(key,keyname,0,REG_SZ, (unsigned char *)value, (_tcslen(value)+1)*sizeof(TCHAR) );
}

void COptions::SetKey(HKEY key, CString keyname, __int64 value)
{
	TCHAR tmp[100];
	_stprintf(tmp, _T("%I64d"), value);
	RegSetValueEx(key, keyname, 0, REG_SZ, (unsigned char *)tmp, (_tcslen(tmp)+1)*sizeof(TCHAR) );
}
