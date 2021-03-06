/* @doc
* @module ScintillaWnd.cpp | Implementation of a Scintilla syntax coloring edit control for MFC
* This program is an example how to use the excellent scintilla edit control of Neil Hodgson.<nl>
* See www.scintilla.org for details<nl>
* Author: Horst Br?kner, hb@ec-logic.com<nl>
* Environment: VisualC++ Version 6, static Build of Scintilla, SciLexer.dll as Lexer<nl>
*/
#include "stdafx.h"
#include <3rdparty/scintilla176/scintilla/include/ScintillaWnd.h>
#include <Tokenizer.h>
#include <3rdparty/scintilla176/scintilla/include/scintilla.h>
#include <3rdparty/scintilla176/scintilla/include/SciLexer.h>
#include <3rdparty/scintilla176/scintilla/include/Accessor.h>
#include <3rdparty/scintilla176/scintilla/include/Propset.h>
#include <3rdparty/scintilla176/scintilla/include/keywords.h>
#include "resource.h"

#include <fstream>
#include <io.h>
using namespace std;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
// the next 2 arrays are used to determine lexer format from file extensions
static TCHAR *szExtensions[] = 
{
	"py",                                                                // SCLEX_PYTHON
	"c|cc|cpp|cxx|cs|h|hh|hpp|hxx|sma|dlg|rc|rc2|idl|odl|mak|js|java",   // SCLEX_CPP
	"htm|html|shtml|htt|cfm|tpl|hta",         // SCLEX_HTML
	"xml|gcl|xsl|svg|xul|xsd|dtd|xslt|axl",   // SCLEX_XML
	"pl|pm|cgi|pod",                          // SCLEX_PERL
	"sql|spec|body|sps|spb|sf|sp",            // SCLEX_SQL
	"vb|bas|frm|cls|ctl|pag|dsr|dob",         // SCLEX_VB
	"properties",                             // SCLEX_PROPERTIES
	"err",                                    // SCLEX_ERRORLIST
	"iface|mak",                              // SCLEX_MAKEFILE
	"bat|cmd|nt",                             // SCLEX_BATCH
	"xcode",                                  // SCLEX_XCODE
	"tex|sty",                                // SCLEX_LATEX
	"lua",                                    // SCLEX_LUA
	"diff",                                   // SCLEX_DIFF
	"conf",                                   // SCLEX_CONF
	"pas|inc|pp",                             // SCLEX_PASCAL
	"ave",                                    // SCLEX_AVE
	"ada|ads|adb",                            // SCLEX_ADA
	"lsp|lisp|scm|smd|ss",                    // SCLEX_LISP
	"rb",                                     // SCLEX_RUBY
	"e",                                      // SCLEX_EIFFEL
	"e",                                      // SCLEX_EIFFELKW
	"tcl",                                    // SCLEX_TCL
	"tab|spf",                                // SCLEX_NNCRONTAB
	"ant",                                    // SCLEX_BULLANT
	"vbs|dsm",                                // SCLEX_VBSCRIPT
	"asp|aspx",                               // SCLEX_ASP
	"php|php3|php4",                          // SCLEX_PHP
	"bc|cln",                                 // SCLEX_BAAN
	"m",                                      // SCLEX_MATLAB
	"sol",                                    // SCLEX_SCRIPTOL
	"asm",                                    // SCLEX_ASM
	"cpp",                                    // SCLEX_CPPNOCASE
	"f|for|f90|f95",                          // SCLEX_FORTRAN
	"f77",                                    // SCLEX_F77
	"css",                                    // SCLEX_CSS
	"pov",                                    // SCLEX_POV
	"lt|lot",                                 // SCLEX_LOUT
	"src|em",                                 // SCLEX_ESCRIPT
	0,
};
static int nFormats[] = {
	SCLEX_PYTHON,
	SCLEX_CPP,
	SCLEX_HTML,
	SCLEX_XML,
	SCLEX_PERL,
	SCLEX_SQL,
	SCLEX_VB,
	SCLEX_PROPERTIES,
	SCLEX_ERRORLIST,
	SCLEX_MAKEFILE,
	SCLEX_BATCH,
	SCLEX_XCODE,
	SCLEX_LATEX,
	SCLEX_LUA,
	SCLEX_DIFF,
	SCLEX_CONF,
	SCLEX_PASCAL,
	SCLEX_AVE,
	SCLEX_ADA,
	SCLEX_LISP,
	SCLEX_RUBY,
	SCLEX_EIFFEL,
	SCLEX_EIFFELKW,
	SCLEX_TCL,
	SCLEX_NNCRONTAB,
	SCLEX_BULLANT,
	SCLEX_VBSCRIPT,
	SCLEX_ASP,
	SCLEX_PHP,
	SCLEX_BAAN,
	SCLEX_MATLAB,
	SCLEX_SCRIPTOL,
	SCLEX_ASM,
	SCLEX_CPPNOCASE,
	SCLEX_FORTRAN,
	SCLEX_F77,
	SCLEX_CSS,
	SCLEX_POV,
	SCLEX_LOUT,
	SCLEX_ESCRIPT,
	0,
};

/////////////////////////////////////
// @mfunc This is an empty constructor
// @rvalue void | not used
//
CScintillaWnd::CScintillaWnd()
{
	m_bLinenumbers = FALSE;
	m_bSelection = TRUE;
	m_bFolding = FALSE;
	m_nSearchflags = 0;
}

/////////////////////////////////////
// @mfunc This is a destructor
// @rvalue void | not used
//
CScintillaWnd::~CScintillaWnd()
{

}
/////////////////////////////////////
// @mfunc Create the window
// @rvalue BOOL | TRUE on success else FALSE on error
//
BOOL CScintillaWnd::Create (LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	if (!CWnd::CreateEx(WS_EX_CLIENTEDGE, STR_SCINTILLAWND,lpszWindowName, dwStyle, rect, pParentWnd,(UINT)nID))
	{   
		LPVOID lpMsgBuf;
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf,0,NULL);
		// Write to stderr
		TRACE (_T("%s\n"), (LPCTSTR)lpMsgBuf);
		// Free the buffer.
		LocalFree( lpMsgBuf );
		return FALSE;
	}
	m_accelerator = ::LoadAccelerators(AfxGetInstanceHandle() , MAKEINTRESOURCE( IDR_SCI_ACCELERATOR ));

	SendMessage(SCI_SETMODEVENTMASK, SC_MOD_INSERTTEXT|SC_MOD_DELETETEXT);
	return TRUE;
}
/////////////////////////////////////
// @mfunc Try to load the Scintilla dll - usually named "SciLexer.dll" or "Scintilla.dll".  We try to locate the dll in 
// the current dirtectory and along the path environment.
// Call this function in your CWinApp derived application in the InitInstance function by calling:<nl>
// CScintillaWnd::LoadScintillaDll()<nl>
// @rvalue BOOL | FALSE on error - TRUE on success
//
HMODULE CScintillaWnd::LoadScintillaDll (
	LPCSTR szDllFile) //@parm filename of the lexer dll - default "SciLexer.dll"
{
	CString strLexer = STR_LEXERDLL;
	if (szDllFile != NULL)
		strLexer = szDllFile;
	// this call to LoadLibrary searches in:
	// 1.) current directory
	// 2.) wint
	// 3.) winnt/system32
	// 4.) path
	HMODULE hModule = ::LoadLibrary(strLexer);
	// if load fails get an extended error message 
	if (hModule == NULL)
	{
		LPVOID lpMsgBuf;
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf,0,NULL);
		// Write to stderr
		TRACE (_T("%s:%s\n"), (LPCSTR)strLexer, (LPCTSTR)lpMsgBuf);
		// Free the buffer.
		LocalFree( lpMsgBuf );
		return NULL;
	}
	return hModule;
}

void CScintillaWnd::AppendText (LPCSTR szText) //@parm pointer to new text
{
	LRESULT lResult = 0;
	if (szText != NULL)
		lResult = SendMessage(SCI_APPENDTEXT, strlen(szText) ,(LPARAM)szText);
}

/////////////////////////////////////
// @mfunc Reset the Scintiall control and add new Text
// @rvalue void | not used
//
void CScintillaWnd::SetText (
					    LPCSTR szText) //@parm pointer to new text
{
	LRESULT lResult = 0;
	if (szText != NULL)
		lResult = SendMessage(SCI_SETTEXT,0,(LPARAM)szText);
	GotoPosition(0);
	///SetFocus();
}
/////////////////////////////////////
// @mfunc Get the text from the control
// @rvalue void | not used
//
void CScintillaWnd::GetText (CString &strText , const long& lStartPos , const long& lEndPos) //@parm handle to receive text
{
	LPSTR szText = GetText();
	if (szText != NULL)
	{
		strText = szText;
		delete [] szText;
	}
}
/////////////////////////////////////
// @mfunc Get the text from the control
// @rvalue LPSTR | a character string with text from the control - NULL on error - the caller has to free pointer
//
LPSTR CScintillaWnd::GetText ()
{
	long lLen = SendMessage(SCI_GETLENGTH, 0 , 0) + 1;
	if (lLen > 0)
	{
		TCHAR *pReturn = new TCHAR[lLen];
		if (pReturn != NULL)
		{
			*pReturn = '\0';
			SendMessage(SCI_GETTEXT, lLen, (long)pReturn);
			return pReturn;
		}
	}
	return NULL;
}
/////////////////////////////////////
// @mfunc Try to load a new file
// @rvalue BOOL | FALSE on error - TRUE on success
//
BOOL CScintillaWnd::LoadFile (
						LPCSTR szPath) //@parm filename of to load
{
	// if pathname is empty do nothing
	if (szPath == NULL || *szPath == '\0')
		return TRUE;
	// try to get extension and figure out what lexer to use
	CString strFile(szPath);
	int nIndex = strFile.ReverseFind('.');
	CString strExtension = strFile.Right(strFile.GetLength()-nIndex-1);
	SendMessage(SCI_SETLEXER, GetFormatFromExtension(strExtension), 0);

	BOOL bReturn = TRUE;
	// ty to open file in sharing mode
	ifstream file;
	long len = 0L;
	long nTotal;
	TCHAR *szBuffer = NULL;
	file.open(szPath, ios::in);///, filebuf::sh_read);
	// ok success - try to get length of file
	if (file.is_open())
	{
		///len = _filelength(file.fd());
		if (len > 0)
		{
			//       alloc new buffer of sie = filesize+1 for termination NULL
			szBuffer = new TCHAR[len+1];
			if (szBuffer != NULL)
			{
				file.read(szBuffer, len);
				nTotal = file.gcount();
				if (nTotal > 0 && nTotal <= len)
					szBuffer[nTotal] = '\0';
				//          read error
				if (file.bad())
				{
					file.close();
					bReturn = FALSE;
				}
			}
			else
			{
				file.close();
				bReturn = FALSE;
			}
		}
		file.close();
		//    set text to control
		SetText (szBuffer);
		//    tell scintilla that we have an unmodified document
		SendMessage(SCI_SETSAVEPOINT,0 , 0);
		GotoPosition(0);

	}
	// file open error - return
	else
		bReturn = FALSE;
	// clean up
	if (szBuffer != NULL)
		delete [] szBuffer;
	return bReturn;
}
/////////////////////////////////////
// @mfunc Try to save the file
// @rvalue BOOL | FALSE on error - TRUE on success
//
BOOL CScintillaWnd::SaveFile (
						LPCSTR szPath) //@parm filename to save to
{
	ofstream file;
	file.open(szPath);
	if (file.fail())
	{
		return FALSE;
	}
	int buflen = SendMessage(SCI_GETLENGTH)+1; //last NULL
	TCHAR *pBuffer = new TCHAR[buflen];
	if (pBuffer != NULL)
	{
		SendMessage(SCI_GETTEXT, buflen,(long)pBuffer);
		file.write(pBuffer, buflen-1);
		delete [] pBuffer;
	}
	file << ends;
	file.close();
	return TRUE;
}
/////////////////////////////////////
// @mfunc Try to find format for lexer by looking at the file extension.<nl>
// See global arrays at top of file.
// @rvalue int | Scintilla integer format for lexer
//
int CScintillaWnd::GetFormatFromExtension (
	LPCSTR szExtension) //@parm filename extension without dot e.g. "cpp"
{
	int i = 0;
	TCHAR *pExtension = szExtensions[i];
	while (pExtension != NULL)
	{
		vector<STRING_T> oResult;
		CTokenizer<CIsFromString>::Tokenize(oResult , szExtensions[i],  CIsFromString(_T("|")));
		for(vector<STRING_T>::iterator itr = oResult.begin();itr != oResult.end();++itr)
		{
			if(0 == STRICMP_T(szExtension , itr->c_str())) return nFormats[i];
		}
		i++;
		pExtension = szExtensions[i];
	}
	return SCLEX_NULL;
}
/////////////////////////////////////
// @mfunc Try to calculate the number of characters needed for the display of the linenumbers.
// This function returns 3 for 10-99 lines in file, 4 for 100-999 lines and so on.
// @rvalue int | number of characters needed to display linenumbers
//
int CScintillaWnd::GetLinenumberChars ()
{
	// try to get number of lines in control
	LRESULT lLines = SendMessage(SCI_GETLINECOUNT, 0, 0);

	int nChars = 1;
	while (lLines > 0)
	{
		lLines = lLines / 10;
		nChars++;
	}
	return nChars; // allow an extra char for safety
}
/////////////////////////////////////
// @mfunc Calculate a reasonable width for Linenumberdisplay
// @rvalue int | number of pixels for the margin width of margin (0) 
//
int CScintillaWnd::GetLinenumberWidth ()
{
	// get number of chars needed to display highest linenumber
	int nChars = GetLinenumberChars ()+1;
	// get width of character '9' in pixels
	LRESULT lWidth = SendMessage(SCI_TEXTWIDTH, STYLE_LINENUMBER, (long)_T("9"));
	return nChars * lWidth;
}
/////////////////////////////////////
// @mfunc Set the display of line numbers on or off.
// Scintilla uses 3 "Margin" at the left of the edit window. The Margin (0)
// is used to display the linenumbers. If set to 0 no numbers are displayed. If
// set to a width > 0 - linenumbers are displayed. We use a helper function to 
// calculate the size of this margin.
// @rvalue int | number of pixels for the margin width of margin (0) 
//
void CScintillaWnd::SetDisplayLinenumbers(
	BOOL bFlag) //@parm flag if we shuld display line numbers
{
	// if nothing changes just return
	if (GetDisplayLinenumbers() == bFlag)
		return;
	// if display is turned off we set margin 0 to 0
	if (!bFlag)
	{
		SendMessage(SCI_SETMARGINWIDTHN, 0, 0);
	}
	// if display is turned o we set margin 0 to the calculated width
	else
	{
		int nWidth = GetLinenumberWidth() + 4;
		SendMessage(SCI_SETMARGINWIDTHN, 0, nWidth);
	}
	m_bLinenumbers = bFlag;
}
/////////////////////////////////////
// @mfunc Cut the selected text to the clipboard
// @rvalue void | not used
//
void CScintillaWnd::Cut()
{
	SendMessage(SCI_CUT, 0, 0);
}
/////////////////////////////////////
// @mfunc Copy the selected text to the clipboard
// @rvalue void | not used
//
void CScintillaWnd::Copy()
{
	SendMessage(SCI_COPY, 0, 0);
}
/////////////////////////////////////
// @mfunc Paste the text from the clipboard to the control
// @rvalue void | not used
//
void CScintillaWnd::Paste()
{
	SendMessage(SCI_PASTE, 0, 0);
}
/////////////////////////////////////
// @mfunc Delete the selected text
// @rvalue void | not used
//
void CScintillaWnd::Clear()
{
	SendMessage(SCI_CLEAR, 0, 0);
}
/////////////////////////////////////
// @mfunc Select the complete text
// @rvalue void | not used
//
void CScintillaWnd::SelectAll()
{
	SendMessage(SCI_SELECTALL, 0, 0);
}
/////////////////////////////////////
// @mfunc Undo the last action
// @rvalue void | not used
//
void CScintillaWnd::Undo()
{
	SendMessage(SCI_UNDO, 0, 0);
}
/////////////////////////////////////
// @mfunc Redo the last undone action
// @rvalue void | not used
//
void CScintillaWnd::Redo()
{
	SendMessage(SCI_REDO, 0, 0);
}
/////////////////////////////////////
// @mfunc Returns a flag if we can undo the last action
// @rvalue BOOL | TRUE if we can undo - else FALSE
//
BOOL CScintillaWnd::CanUndo()
{
	return SendMessage(SCI_CANUNDO, 0, 0) != 0;
}
/////////////////////////////////////
// @mfunc Returns a flag if we can redo the last undone action
// @rvalue BOOL | TRUE if we can redo - else FALSE
//
BOOL CScintillaWnd::CanRedo()
{
	return SendMessage(SCI_CANREDO, 0, 0) != 0;
}
/////////////////////////////////////
// @mfunc Returns a flag if there is text in the clipboard which we can paste
// @rvalue BOOL | TRUE if the clipboard contains text to paste - else FALSE
//
BOOL CScintillaWnd::CanPaste()
{
	return SendMessage(SCI_CANPASTE, 0, 0) != 0;
}
/////////////////////////////////////
// @mfunc Get the current line number - this the with the caret in it
// @rvalue long | line number with the caret in it - starts with 1
//
long CScintillaWnd::GetCurrentLine()
{
	long lPos = SendMessage(SCI_GETCURRENTPOS, 0, 0);
	return SendMessage(SCI_LINEFROMPOSITION, lPos, 0) + 1;
}
/////////////////////////////////////
// @mfunc Get the current column number = position of the caret within the line.
// This return value my be affected by the TAB setting! Starts with 1
// @rvalue long | current column number
//
long CScintillaWnd::GetCurrentColumn()
{
	long lPos = SendMessage(SCI_GETCURRENTPOS, 0, 0);
	return SendMessage(SCI_GETCOLUMN, lPos, 0) + 1;
}
/////////////////////////////////////
// @mfunc Return the current character position within the file.
// @rvalue long | current character position
//
long CScintillaWnd::GetCurrentPosition()
{
	return SendMessage(SCI_GETCURRENTPOS, 0, 0);
}
/////////////////////////////////////
// @mfunc Return the current style at the caret
// @rvalue int | the current style index (0...127)
//
int CScintillaWnd::GetCurrentStyle()
{
	long lPos = SendMessage(SCI_GETCURRENTPOS, 0, 0);
	return SendMessage(SCI_GETSTYLEAT, lPos, 0);
}
/////////////////////////////////////
// @mfunc Return the current folding level at the caret line
// @rvalue int | the current folding level (0...n)
//
int CScintillaWnd::GetCurrentFoldinglevel()
{
	long lLine = GetCurrentLine();
	int level = (SendMessage(SCI_GETFOLDLEVEL, lLine, 0)) & SC_FOLDLEVELNUMBERMASK;
	return level-1024;
}
/////////////////////////////////////
// @mfunc set the fontname e.g. "Arial"
// @rvalue void | not used
//
void CScintillaWnd::SetFontname(int nStyle, LPCSTR szFontname)
{
	SendMessage(SCI_STYLESETFONT, nStyle, (long)szFontname);
}
/////////////////////////////////////
// @mfunc Set the font height in points
// @rvalue void | not used
//
void CScintillaWnd::SetFontheight(int nStyle, int nHeight)
{
	SendMessage(SCI_STYLESETSIZE, nStyle, (long)nHeight);
}
/////////////////////////////////////
// @mfunc Set the foreground color
// @rvalue void | not used
//
void CScintillaWnd::SetForeground(int nStyle, COLORREF crForeground)
{
	SendMessage(SCI_STYLESETFORE, nStyle, (long)crForeground);
}
/////////////////////////////////////
// @mfunc set the backgroundcolor
// @rvalue void | not used
//
void CScintillaWnd::SetBackground(int nStyle, COLORREF crBackground)
{
	SendMessage(SCI_STYLESETBACK, nStyle, (long)crBackground);
}
/////////////////////////////////////
// @mfunc set given style to bold
// @rvalue void | not used
//
void CScintillaWnd::SetBold(int nStyle, BOOL bBold)
{
	SendMessage(SCI_STYLESETBOLD, nStyle, (long)bBold);
}
/////////////////////////////////////
// @mfunc set given style to bold
// @rvalue void | not used
//
void CScintillaWnd::SetItalic(int nStyle, BOOL bItalic)
{
	SendMessage(SCI_STYLESETITALIC, nStyle, (long)bItalic);
}
/////////////////////////////////////
// @mfunc set given style to bold
// @rvalue void | not used
//
void CScintillaWnd::SetUnderline(int nStyle, BOOL bUnderline)
{
	SendMessage(SCI_STYLESETUNDERLINE, nStyle, (long)bUnderline);
}
/////////////////////////////////////
// @mfunc Return flag if we are in overstrike mode
// @rvalue BOOL | TRUE if we are in overstrike mode - else FALSE
//
BOOL CScintillaWnd::GetOverstrike()
{
	return SendMessage(SCI_GETOVERTYPE , 0, 0);
}
/////////////////////////////////////
// @mfunc set set overstrike mode (TRUE) or insert mode (FALSE)
// @rvalue void | not used
//
void CScintillaWnd::SetOverstrike(BOOL bOverstrike)
{
	SendMessage(SCI_SETOVERTYPE, bOverstrike, 0);
}
/////////////////////////////////////
// @mfunc Toggle the display of the selection bookmark margin
// @rvalue void | not used
//
void CScintillaWnd::SetDisplaySelection(BOOL bFlag)
{
	m_bSelection = bFlag;
	if (bFlag)
		SendMessage(SCI_SETMARGINWIDTHN, 1, 16);
	else
		SendMessage(SCI_SETMARGINWIDTHN, 1, 0);
}
/////////////////////////////////////
// @mfunc Toggle the display of the folding margin
// @rvalue void | not used
//
void CScintillaWnd::SetDisplayFolding(BOOL bFlag)
{
	m_bFolding = bFlag;
	if (bFlag)
		SendMessage(SCI_SETMARGINWIDTHN, 2, 16);
	else
		SendMessage(SCI_SETMARGINWIDTHN, 2, 0);
}
/////////////////////////////////////
// @mfunc init the view with reasonable defaults
// @rvalue void | not used
//
void CScintillaWnd::Init()
{
	// clear all text styles
	SendMessage(SCI_CLEARDOCUMENTSTYLE, 0, 0);
	// set the number of styling bits to 7 - the asp/html views need a lot of styling - default is 5
	// If you leave the default you will see twiggle lines instead of ASP code
	SendMessage(SCI_SETSTYLEBITS, 7, 0);
	// set the display for indetation guides to on - this displays virtical dotted lines from the beginning of 
	// a code block to the end of the block
	SendMessage(SCI_SETINDENTATIONGUIDES, TRUE, 0);
	// set tabwidth to 3
	SendMessage(SCI_SETTABWIDTH,3,0);
	// set indention to 3
	SendMessage(SCI_SETINDENT,3,0);
	// set the caret blinking time to 400 milliseconds
	SendMessage(SCI_SETCARETPERIOD,400,0);
	// source folding section
	// tell the lexer that we want folding information - the lexer supplies "folding levels"
	SendMessage(SCI_SETPROPERTY, (WPARAM)_T("fold"), (LPARAM)_T("1"));
	SendMessage(SCI_SETPROPERTY, (WPARAM)_T("fold.html"), (LPARAM)_T("1"));
	SendMessage(SCI_SETPROPERTY, (WPARAM)_T("fold.html.preprocessor"), (LPARAM)_T("1"));
	SendMessage(SCI_SETPROPERTY, (WPARAM)_T("fold.comment"), (LPARAM)_T("1"));
	SendMessage(SCI_SETPROPERTY, (WPARAM)_T("fold.at.else"), (LPARAM)_T("1"));
	SendMessage(SCI_SETPROPERTY, (WPARAM)_T("fold.flags"), (LPARAM)_T("1"));
	SendMessage(SCI_SETPROPERTY, (WPARAM)_T("fold.preprocessor"), (LPARAM)_T("1"));
	SendMessage(SCI_SETPROPERTY, (WPARAM)_T("styling.within.preprocessor"), (LPARAM)_T("1"));
	SendMessage(SCI_SETPROPERTY, (WPARAM)_T("asp.default.language"), (LPARAM)_T("1"));
	// Tell scintilla to draw folding lines UNDER the folded line
	SendMessage(SCI_SETFOLDFLAGS, 16,0);
	// Set margin 2 = folding margin to display folding symbols
	SendMessage(SCI_SETMARGINMASKN, 2, SC_MASK_FOLDERS);
	// allow notifications for folding actions
	SendMessage(SCI_SETMODEVENTMASK, SC_MOD_INSERTTEXT|SC_MOD_DELETETEXT, 0);
	//   SendMessage(SCI_SETMODEVENTMASK, SC_MOD_CHANGEFOLD|SC_MOD_INSERTTEXT|SC_MOD_DELETETEXT, 0);
	// make the folding margin sensitive to folding events = if you click into the margin you get a notification event
	SendMessage(SCI_SETMARGINSENSITIVEN, 2, TRUE);
	// define a set of markers to displa folding symbols
	SendMessage(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPEN, SC_MARK_MINUS);
	SendMessage(SCI_MARKERDEFINE, SC_MARKNUM_FOLDER, SC_MARK_PLUS);
	SendMessage(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERSUB, SC_MARK_EMPTY);
	SendMessage(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERTAIL, SC_MARK_EMPTY);
	SendMessage(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEREND, SC_MARK_EMPTY);
	SendMessage(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY);
	SendMessage(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_EMPTY);
	// set the forground color for some styles
	SendMessage(SCI_STYLESETFORE, 0, RGB(0,0,0));
	SendMessage(SCI_STYLESETFORE, 2, RGB(0,64,0));
	SendMessage(SCI_STYLESETFORE, 5, RGB(0,0,255));
	SendMessage(SCI_STYLESETFORE, 6, RGB(200,20,0));
	SendMessage(SCI_STYLESETFORE, 9, RGB(0,0,255));
	SendMessage(SCI_STYLESETFORE, 10, RGB(255,0,64));
	SendMessage(SCI_STYLESETFORE, 11, RGB(0,0,0));
	// set the backgroundcolor of brace highlights
	SendMessage(SCI_STYLESETBACK, STYLE_BRACELIGHT, RGB(0,255,0));
	// set end of line mode to CRLF
	SendMessage(SCI_CONVERTEOLS, 2, 0);
	SendMessage(SCI_SETEOLMODE, 2, 0);
	//   SendMessage(SCI_SETVIEWEOL, TRUE, 0);
	// set markersymbol for marker type 0 - bookmark
	SendMessage(SCI_MARKERDEFINE, 0, SC_MARK_CIRCLE);
	// display all margins
	SetDisplayLinenumbers(TRUE);
	SetDisplayFolding(TRUE);
	SetDisplaySelection(TRUE);

}
/////////////////////////////////////
// @mfunc Update UI and do brace matching
// @rvalue void | not used
//
void CScintillaWnd::UpdateUI()
{
	// do brace matching
	long lStart = SendMessage(SCI_GETCURRENTPOS, 0, 0);
	long lEnd = SendMessage(SCI_BRACEMATCH, lStart-1, 0);
	// if there is a matching brace highlight it
	if (lEnd >= 0)
		SendMessage(SCI_BRACEHIGHLIGHT, lStart-1, lEnd);
	// if there is NO matching brace erase old highlight
	else
		SendMessage(SCI_BRACEHIGHLIGHT, -1, -1);
}
/////////////////////////////////////
// @mfunc Do default folding for given margin and position - usually called from notification handler
// @rvalue void | not used
//
void CScintillaWnd::DoDefaultFolding(
							  int nMargin,      //@parm maring we want to handle - normally (2)
							  long lPosition)   //@parm character position where user clicked margin
{
	// simply toggle fold
	if (nMargin == 2)
	{
		long lLine = SendMessage(SCI_LINEFROMPOSITION, lPosition, 0);
		SendMessage(SCI_TOGGLEFOLD, lLine, 0);
	}
}
/////////////////////////////////////
// @mfunc Do default folding for given margin and position - usually called from notification handler
// @rvalue void | not used
//
void CScintillaWnd::Refresh()
{
	SendMessage(SCI_COLOURISE, 0, -1);
}
/////////////////////////////////////
// @mfunc Add a bookmark at given line
// @rvalue void | not used
//
void CScintillaWnd::AddBookmark(
						  long lLine) //@parm line where to add bookmark - lines start at 1
{
	SendMessage(SCI_MARKERADD, lLine-1, 0);
}
/////////////////////////////////////
// @mfunc Delete a bookmark at given line
// @rvalue void | not used
//
void CScintillaWnd::DeleteBookmark(
							long lLine) //@parm line where to delete bookmark - lines start at 1
{
	if (HasBookmark(lLine))
	{
		SendMessage(SCI_MARKERDELETE, lLine-1, 0);
	}
}
/////////////////////////////////////
// @mfunc Check if given line has a bookmark
// @rvalue BOOL | TRUE if given line has a bookmark - else FALSE
//
BOOL CScintillaWnd::HasBookmark(
						  long lLine) //@parm line where to add bookmark - lines start at 1
{
	int n = SendMessage(SCI_MARKERGET, lLine-1, 0);
	// check mask for markerbit 0
	if (n & 0x1)
		return TRUE;
	return FALSE;
}
/////////////////////////////////////
// @mfunc Find next bookmark from current line
// @rvalue void | not used
//
void CScintillaWnd::FindNextBookmark()
{
	long lLine = SendMessage(SCI_MARKERNEXT, GetCurrentLine(), 1);
	if (lLine >= 0)
		SendMessage(SCI_GOTOLINE, lLine,0);
}
/////////////////////////////////////
// @mfunc Find previous bookmark from current line
// @rvalue void | not used
//
void CScintillaWnd::FindPreviousBookmark()
{
	long lLine = SendMessage(SCI_MARKERPREVIOUS, GetCurrentLine()-2, 1);
	if (lLine >= 0)
		SendMessage(SCI_GOTOLINE, lLine,0);
}
/////////////////////////////////////
// @mfunc Goto givven character position
// @rvalue void | not used
//
void CScintillaWnd::GotoPosition(
						   long lPos) //@parm new character position
{
	SendMessage(SCI_GOTOPOS, lPos, 0);
}
/////////////////////////////////////
// @mfunc Goto givven line position
// @rvalue void | not used
//
void CScintillaWnd::GotoLine(
					    long lLine) //@parm new line - lines start at 1
{
	SendMessage(SCI_GOTOLINE, lLine-1, 0);
}
/////////////////////////////////////
// @mfunc Search forward for a given string and select it if found. You may use regular expressions on the text.
// @rvalue BOOL | TRUE if text is ffound else FALSE
//
BOOL CScintillaWnd::SearchForward(
						    LPSTR szText) //@parm text to search
{
	if (szText == NULL)
		return FALSE;
	long lPos = GetCurrentPosition();
	TextToFind tf;
	tf.lpstrText = szText;
	tf.chrg.cpMin = lPos+1;
	tf.chrg.cpMax = SendMessage(SCI_GETLENGTH, 0, 0);
	lPos = SendMessage(SCI_FINDTEXT, m_nSearchflags, (long)&tf);
	if (lPos >= 0)
	{
		SetFocus();
		GotoPosition(lPos);
		SendMessage(SCI_SETSEL, tf.chrgText.cpMin, tf.chrgText.cpMax);
		SendMessage(SCI_FINDTEXT, m_nSearchflags, (long)&tf);
		return TRUE;
	}
	return FALSE;
}
/////////////////////////////////////
// @mfunc Search backward for a given string and select it if found. You may use regular expressions on the text.
// @rvalue BOOL | TRUE if text is ffound else FALSE
//
BOOL CScintillaWnd::SearchBackward(
							LPSTR szText) //@parm text to search
{
	if (szText == NULL)
		return FALSE;
	long lPos = GetCurrentPosition();
	long lMinSel = GetSelectionStart();
	TextToFind tf;
	tf.lpstrText = szText;
	if (lMinSel >= 0)
		tf.chrg.cpMin = lMinSel-1;
	else
		tf.chrg.cpMin = lPos-1;
	tf.chrg.cpMax = 0;
	lPos = SendMessage(SCI_FINDTEXT, m_nSearchflags, (long)&tf);
	if (lPos >= 0)
	{
		SetFocus();
		GotoPosition(lPos);
		SendMessage(SCI_SETSEL, tf.chrgText.cpMin, tf.chrgText.cpMax);
		SendMessage(SCI_FINDTEXT, m_nSearchflags, (long)&tf);
		return TRUE;
	}
	return FALSE;
}
/////////////////////////////////////
// @mfunc Replace a text with a new text. You can use regular expression with tagging on the replacement text.
// @rvalue void | not used
//
void CScintillaWnd::ReplaceSearchedText(
								LPCSTR szText) //@parm new text
{
	if (szText == NULL)
		return;
	SendMessage(SCI_TARGETFROMSELECTION, 0, 0);
	if (m_nSearchflags & SCFIND_REGEXP)
		SendMessage(SCI_REPLACETARGETRE, strlen(szText), (long)szText);
	else
		SendMessage(SCI_REPLACETARGET, strlen(szText), (long)szText);
}
/////////////////////////////////////
// @mfunc Set lexer format - see scintilla.h
// @rvalue void | not used
//
void CScintillaWnd::SetLexer(
					    int nLexer) //@parm lexer code e.g. 
{
	SendMessage(SCI_SETLEXER, nLexer, 0);
}
/////////////////////////////////////
// @mfunc Get start of selection (anchor) as character position
// @rvalue long | character position of selection start (anchor) - -1 on error
//
long CScintillaWnd::GetSelectionStart()
{
	return SendMessage(SCI_GETSELECTIONSTART, 0, 0);
}
/////////////////////////////////////
// @mfunc Get end of selection as character position
// @rvalue long | character position of selection end - -1 on error
//
long CScintillaWnd::GetSelectionEnd()
{
	return SendMessage(SCI_GETSELECTIONEND, 0, 0);
}
/////////////////////////////////////
// @mfunc Get selected string
// @rvalue CString | string with currentliy selected text
//
CString CScintillaWnd::GetSelectedText()
{
	long lLen = (GetSelectionEnd() - GetSelectionStart()) + 1;
	if (lLen > 0)
	{
		TCHAR *p = new TCHAR[lLen+1];
		if (p != NULL)
		{
			*p = '\0';
			SendMessage(SCI_GETSELTEXT, 0, (long)p);
			CString strReturn = p;
			delete [] p;
			return strReturn;
		}
	}
	return _T("");
}
/////////////////////////////////////
// @mfunc Replace a text in a selection or in the complete file multiple times
// @rvalue int | number of replacements
//
int CScintillaWnd::ReplaceAll(
						LPCSTR szFind, 
						LPCSTR szReplace, 
						BOOL bUseSelection)
{
	int nCount = 0;
	// different branches for replace in selection or total file
	if (bUseSelection)
	{
		//    get starting selection range
		long lLen = 0;
		long lStart = GetSelectionStart();
		long lEnd = GetSelectionEnd();
		//    set target to selection
		SendMessage(SCI_SETTARGETSTART, lStart);
		SendMessage(SCI_SETTARGETEND, lEnd);
		//    try to find text in target for the first time
		long lPos = SendMessage(SCI_SEARCHINTARGET, strlen(szFind), (long)szFind);
		//    loop over selection until end of selection reached - moving the target start each time
		while (lPos < lEnd && lPos >= 0)
		{
			if (m_nSearchflags & SCFIND_REGEXP) // check for regular expression flag
				lLen = SendMessage(SCI_REPLACETARGETRE, strlen(szReplace), (long)szReplace);
			else
				lLen = SendMessage(SCI_REPLACETARGET, strlen(szReplace), (long)szReplace);
			//       the end of the selection was changed - recalc the end
			lEnd = GetSelectionEnd();
			//       move start of target behind last change and end of target to new end of selection
			SendMessage(SCI_SETTARGETSTART, lPos+lLen);
			SendMessage(SCI_SETTARGETEND, lEnd);
			//       find again - if nothing found loop exits
			lPos = SendMessage(SCI_SEARCHINTARGET, strlen(szFind), (long)szFind);
			nCount++;
		}
	}
	else
	{
		//    start with first and last char in buffer
		long lLen = 0;
		long lStart = 0;
		long lEnd = SendMessage(SCI_GETTEXTLENGTH, 0, 0);
		//    set target to selection
		SendMessage(SCI_SETTARGETSTART, lStart, 0);
		SendMessage(SCI_SETTARGETEND, lEnd, 0);
		//    try to find text in target for the first time
		long lPos = SendMessage(SCI_SEARCHINTARGET, strlen(szFind), (long)szFind);
		//    loop over selection until end of selection reached - moving the target start each time
		while (lPos < lEnd && lPos >= 0)
		{
			if (m_nSearchflags & SCFIND_REGEXP) // check for regular expression flag
				lLen = SendMessage(SCI_REPLACETARGETRE, strlen(szReplace), (long)szReplace);
			else
				lLen = SendMessage(SCI_REPLACETARGET, strlen(szReplace), (long)szReplace);
			//       the end of the selection was changed - recalc the end
			lEnd = SendMessage(SCI_GETTEXTLENGTH, 0, 0);;
			//       move start of target behind last change and end of target to new end of buffer
			SendMessage(SCI_SETTARGETSTART, lPos+lLen);
			SendMessage(SCI_SETTARGETEND, lEnd);
			//       find again - if nothing found loop exits
			lPos = SendMessage(SCI_SEARCHINTARGET, strlen(szFind), (long)szFind);
			nCount++;
		}
	}
	return nCount;
}


/**	\brief	lLineNo?? ???????? ?????? ?????? ??????.
**/
long CScintillaWnd::GetLineLength(const long& lLineNo)
{
	long res = 0;
	if(-1 == lLineNo)
	{
		const long lPos = SendMessage(SCI_GETCURRENTPOS, 0, 0);
		res = SendMessage(SCI_LINELENGTH , SendMessage(SCI_LINEFROMPOSITION, lPos , 0) , 0);
	}
	else
	{
		res = SendMessage(SCI_LINELENGTH , lLineNo , 0);
	}

	return res;
}

/**	\brief	lLineNo?? ???????? ?????? ???????? ??????.
**/
bool CScintillaWnd::GetLineText(CString& rText , long lLineNo)
{
	if(-1 == lLineNo)
	{
		const long lPos = SendMessage(SCI_GETCURRENTPOS, 0, 0);
		lLineNo = SendMessage(SCI_LINEFROMPOSITION, lPos, 0);
	}

	const long nLineLength = GetLineLength(lLineNo);
	if(nLineLength > 0)
	{
		TCHAR *pReturn = new TCHAR[nLineLength];
		if (NULL != pReturn)
		{
			pReturn[nLineLength - 1] = '\0';
			SendMessage(SCI_GETLINE , lLineNo , (long)pReturn);
			if (pReturn != NULL)
			{
				rText = pReturn;
				rText = rText.Left(nLineLength);
				if((rText.GetLength() >= 2) && ("\r\n" == rText.Right(2))) rText = rText.Mid(0 , nLineLength - 2);
				delete [] pReturn;

				return true;
			}
		}
	}
	else
	{
		rText = "";
		return true;
	}

	return false;
}

/**
**/
void CScintillaWnd::SetCurrentLineHiLiting(bool isHiliting, COLORREF bgColor)
{
	SendMessage(SCI_SETCARETLINEVISIBLE, isHiliting);
	if (!isHiliting) return;
	SendMessage(SCI_SETCARETLINEBACK, bgColor);
}

/// Sets a Scintilla style
void CScintillaWnd::SetAStyle(int style, COLORREF fore, COLORREF back , int size, const char *face)
{	
	SendMessage(SCI_STYLESETFORE, style, fore);
	SendMessage(SCI_STYLESETBACK, style, back);
	if (size >= 1)
		SendMessage(SCI_STYLESETSIZE, style, size);
	if (face) 
		SendMessage(SCI_STYLESETFONT, style, reinterpret_cast<LPARAM>(face));
}

BEGIN_MESSAGE_MAP(CScintillaWnd, CWnd)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
END_MESSAGE_MAP()

/**
**/
void CScintillaWnd::OnEditCopy()
{
	Copy();
}

/**
**/
void CScintillaWnd::OnEditPaste()
{
	Paste();
}

/**	
	@brief	ACCELERATOR?? ???????? ???? ?????? ???? ?????????? ????.

	@author	humkyung
**/
BOOL CScintillaWnd::PreTranslateMessage(MSG* pMsg)
{
	if(::TranslateAccelerator( GetSafeHwnd() , m_accelerator , pMsg ))
		return TRUE;

	return CWnd::PreTranslateMessage(pMsg);
}
