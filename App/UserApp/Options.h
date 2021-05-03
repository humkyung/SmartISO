// FileZilla - a Windows ftp client

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

#if !defined(AFX_OPTIONS_H__3E60F2D3_99F3_4271_92A3_2CF71AF62731__INCLUDED_)
#define AFX_OPTIONS_H__3E60F2D3_99F3_4271_92A3_2CF71AF62731__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define OPTIONS_NUM 119

/////////////////////////////////////////////////////////////////////////////
// Dialogfeld COptions
class COptions
{
// Konstruktion
public:
	static void SetOption(int OptionID,CString value);
	static void SetOption(int OptionID,int value);
	static CString GetOption(int OptionID);
	static int GetOptionVal(int OptionID);

	static void SetKey(HKEY key, CString keyname, LPCTSTR value);
	static void SetKey(HKEY key, CString keyname, __int64 value);
	static void GetKey(HKEY key, CString keyname, CString &value);
	static void GetKey(HKEY key, CString keyname, __int64 &value);
	static void GetKey(HKEY key, CString keyname, int &value);


	COptions();   // Standardkonstruktor
protected:
	typedef struct
	{
		BOOL bCached;
		CTime createtime;
		int nType;
		CString str;
		int value;
	} t_OptionsCache;
	static t_OptionsCache m_OptionsCache[OPTIONS_NUM];
	// add a page (page, page title, optional parent)

	static CString m_sConfigFile;

// Dialogfelddaten

// ?erschreibungen
	// Vom Klassen-Assistenten generierte virtuelle Funktions?erschreibungen
	//{{AFX_VIRTUAL(COptions)
	protected:
	//}}AFX_VIRTUAL

// Implementierung
};

#define OPTION_LOGONTYPE 1
#define OPTION_PASV 2
#define OPTION_FWHOST 3
#define OPTION_FWPORT 4
#define OPTION_FWUSER 5
#define OPTION_FWPASS 6
#define OPTION_TIMEOUTLENGTH 7
#define OPTION_KEEPALIVE 8
#define OPTION_INTERVALLOW 9
#define OPTION_INTERVALHIGH 10
#define OPTION_NUMRETRIES 11
#define OPTION_RETRYDELAY 12
#define OPTION_LASTSERVERHOST 13
#define OPTION_LASTSERVERPORT 14
#define OPTION_LASTSERVERUSER 15
#define OPTION_LASTSERVERPASS 16
#define OPTION_PROXYTYPE 17
#define OPTION_PROXYHOST 18
#define OPTION_PROXYPORT 19
#define OPTION_PROXYUSER 20
#define OPTION_PROXYPASS 21
#define OPTION_PROXYUSELOGON 22
#define OPTION_LASTSERVERPATH 23
#define OPTION_LASTSERVERFWBYPASS 24
#define OPTION_LANGUAGE 25
#define OPTION_DEFAULTFOLDERTYPE 26
#define OPTION_DEFAULTFOLDER 27
#define OPTION_TRANSFERMODE 28
#define OPTION_ASCIIFILES 29
#define OPTION_SHOWNOLABEL 30
#define OPTION_SHOWNOTOOLBAR 31
#define OPTION_SHOWNOQUICKCONNECTBAR 32
#define OPTION_SHOWNOSTATUSBAR 33
#define OPTION_SHOWNOMESSAGELOG 34
#define OPTION_SHOWNOTREEVIEW 35
#define OPTION_SHOWNOQUEUE 36
#define OPTION_REMEMBERVIEWS 37
#define OPTION_LOCALLISTVIEWSTYLE 38
#define OPTION_HIDELOCALCOLUMNS 39
#define OPTION_REMOTELISTVIEWSTYLE 40
#define OPTION_HIDEREMOTECOLUMNS 41
#define OPTION_SHOWSITEMANAGERONSTARTUP 42
#define OPTION_USEGSS 43
#define OPTION_GSSSERVERS 44
#define OPTION_LASTSERVERDONTREMEMBERPASS 45
#define OPTION_REMEMBERLASTWINDOWPOS 46
#define OPTION_LASTWINDOWPOS 47
#define OPTION_DEBUGTRACE 48
#define OPTION_DEBUGSHOWLISTING 49
#define OPTION_DEBUGLOGTOFILE 50
#define OPTION_DEBUGLOGFILE 51
#define OPTION_LIMITPORTRANGE 52
#define OPTION_PORTRANGELOW 53
#define OPTION_PORTRANGEHIGH 54
#define OPTION_REMEMBERLOCALVIEW 55
#define OPTION_REMEMBERREMOTEVIEW 56
#define OPTION_LASTSPLITTERSIZE 57
#define OPTION_USECACHE	58
#define OPTION_MAXCACHETIME	59
#define OPTION_LASTSERVERTYPE 60
#define OPTION_MINIMIZETOTRAY 61
#define OPTION_SHOWREMOTETREEVIEW 62
#define OPTION_REMEMBERLOCALCOLUMNWIDTHS 63
#define OPTION_REMEMBERREMOTECOLUMNWIDTHS 64
#define OPTION_LOCALCOLUMNWIDTHS 65
#define OPTION_REMOTECOLUMNWIDTHS 66
#define OPTION_LOCALFILESIZEFORMAT 67
#define OPTION_REMOTEFILESIZEFORMAT 68
#define OPTION_LASTSERVERNAME 69
#define OPTION_PRESERVEDOWNLOADFILETIME 70
#define OPTION_RUNINSECUREMODE 71
#define OPTION_VIEWEDITDEFAULT 72
#define OPTION_VIEWEDITCUSTOM 73
#define OPTION_ALWAYSSHOWHIDDEN 74
#define OPTION_TRANSFERPRIMARYMAXSIZE 75
#define OPTION_ENABLEDEBUGMENU 76
#define OPTION_TRANSFERAPICOUNT	77
#define OPTION_USEREGISTRY 78
#define OPTION_TRANSFERUSEMULTIPLE 79
#define OPTION_IDENT 80
#define OPTION_IDENTCONNECT 81
#define OPTION_IDENTSAMEIP 82
#define OPTION_IDENTSYSTEM 83
#define OPTION_IDENTUSER 84
#define OPTION_FILEEXISTSACTION 85
#define OPTION_SPEEDLIMIT_DOWNLOAD_TYPE 86
#define OPTION_SPEEDLIMIT_UPLOAD_TYPE 87
#define OPTION_SPEEDLIMIT_DOWNLOAD_VALUE 88
#define OPTION_SPEEDLIMIT_UPLOAD_VALUE 89
#define OPTION_SPEEDLIMIT_DOWNLOAD_RULES 90
#define OPTION_SPEEDLIMIT_UPLOAD_RULES 91
#define OPTION_SORTSITEMANAGERFOLDERSFIRST 92
#define OPTION_SITEMANAGERNOEXPANDFOLDERS 93
#define OPTION_SHOWLOCALSTATUSBAR 94
#define OPTION_SHOWREMOTESTATUSBAR 95
#define OPTION_LOCALCOLUMNSORT 96
#define OPTION_REMOTECOLUMNSORT 97
#define OPTION_SSHUSECOMPRESSION 98
#define OPTION_SSHPROTOCOL	99
#define OPTION_TRANSFERIP 100
#define OPTION_LOCAL_DOUBLECLICK_ACTION 101
#define OPTION_REMOTE_DOUBLECLICK_ACTION 102
#define OPTION_LOCALTREEVIEWLOCATION 103
#define OPTION_REMOTETREEVIEWLOCATION 104
#define OPTION_LASTSERVERTRANSFERMODE 105
#define OPTION_MESSAGELOG_USECUSTOMFONT 106
#define OPTION_MESSAGELOG_FONTNAME 107
#define OPTION_MESSAGELOG_FONTSIZE 108
#define OPTION_PANELAYOUT_SWITCHLAYOUT 109
#define OPTION_MODEZ_USE 110
#define OPTION_MODEZ_LEVEL 111
#define OPTION_TRANSFERIP6 112
#define OPTION_ENABLE_IPV6 113
#define OPTION_LOGTIMESTAMPS 114
#define OPTION_VMSALLREVISIONS 115
#define OPTION_LASTSERVERUTF8 116
#define OPTION_ANONPWD 117
#define OPTION_LASTSERVERACCOUNT 118
#define OPTION_LASTSERVERTIMEZONE 119

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ f?t unmittelbar vor der vorhergehenden Zeile zus?zliche Deklarationen ein.

#endif // AFX_OPTIONS_H__3E60F2D3_99F3_4271_92A3_2CF71AF62731__INCLUDED_
