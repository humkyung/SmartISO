// stdafx.cpp : source file that includes just the standard includes
//	UserDLL.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"
#include <IsEntity.h>
#include <IsRegExp.h>
using namespace IsRegExp;

enum
{
	FIND_MATCH_CASE		= 0x0001,
	FIND_WHOLE_WORD		= 0x0002,
	FIND_DIRECTION_UP	= 0x0010,
	REPLACE_SELECTION	= 0x0100
};

//IsRegExp::RxNode __declspec(dllimport) *IsRxCompile(LPCTSTR Regexp);
//int __declspec(dllimport) IsRxExec(IsRegExp::RxNode *Regexp, LPCTSTR Data, int Len, LPCTSTR Start, IsRegExp::RxMatchRes *Match, unsigned int RxOpt);
//int __declspec(dllimport) IsRxReplace(LPCTSTR rep, LPCTSTR Src, int len, IsRegExp::RxMatchRes match, LPTSTR *Dest, int *Dlen);
//void __declspec(dllimport) IsRxFree(IsRegExp::RxNode *Node);
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