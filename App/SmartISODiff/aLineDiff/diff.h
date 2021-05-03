#pragma once

#ifdef	FN_REVISO_MODULE
#define ALINEDIFF_EXT_CLASS	__declspec(dllexport)
#else
#define ALINEDIFF_EXT_CLASS	__declspec(dllimport)
#endif

#ifndef FN_REVISO_MODULE
#ifdef _UNICODE
#ifdef _DEBUG
#pragma comment(lib, "aLineDiff_vc80_unicode_debug")
#pragma message("Automatically linking with aLineDiff_vc80_unicode_debug.dll (Debug Unicode)")
#else
#pragma comment(lib, "aLineDiff_vc80_unicode")
#pragma message("Automatically linking with aLineDiff_vc80_unicode.dll (Release Unicode)")
#endif
#else
#ifdef _DEBUG
#pragma comment(lib, "aLineDiff_vc80_debug")
#pragma message("Automatically linking with aLineDiff_vc80_debug.dll (Debug)")
#else
#pragma comment(lib, "aDiff_vc80")
#pragma message("Automatically linking with aLineDiff_vc80.dll (Release)")
#endif
#endif // _UNICODE
#endif // USER_EXPORTS

#include <IsTools.h>
#include "DiffData.h"

///#if (_MSC_VER >= 1500)
///#include <unordered_map>
///#define	MAP_T	std::tr1::unordered_map
///#else
#include <map>
#define	MAP_T	std::map
///#endif

namespace diff
{
	class ALINEDIFF_EXT_CLASS CDiff
	{
		/// <summary>
		/// Shortest Middle Snake Return Data
		/// </summary>
		struct SMSRD
		{
			int x, y;
			// internal int u, v;  // 2002.09.20: no need for 2 points 
		};
	public:
		/// <summary>details of one difference.</summary>
		struct Item 
		{
			/// <summary>Start Line number in Data A.</summary>
			int StartA;
			/// <summary>Start Line number in Data B.</summary>
			int StartB;

			/// <summary>Number of changes in Data A.</summary>
			int deletedA;
			/// <summary>Number of changes in Data B.</summary>
			int insertedB;
		};// Item

		static vector<CDiff::Item*> DiffText(STRING_T TextA, STRING_T TextB, bool trimSpace=false , bool ignoreSpace=false, bool ignoreCase=false);

		/// <summary>
		/// If a sequence of modified lines starts with a line that contains the same content
		/// as the line that appends the changes, the difference sequence is modified so that the
		/// appended line and not the starting line is marked as modified.
		/// This leads to more readable diff sequences when comparing text files.
		/// </summary>
		/// <param name="Data">A Diff data buffer containing the identified changes.</param>
		static void Optimize(CDiffData* Data);

		static int DiffInt(vector<Item*> oResult , vector<int> ArrayA, vector<int> ArrayB);

		static vector<int> DiffCodes(STRING_T aText, MAP_T<STRING_T,int>* h, bool trimSpace, bool ignoreSpace, bool ignoreCase);

	private:
		static SMSRD SMS(CDiffData* DataA, int LowerA, int UpperA, CDiffData* DataB, int LowerB, int UpperB, vector<int>& oDownVector, vector<int>& oUpVector);

		static void LCS(CDiffData* DataA, int LowerA, int UpperA, CDiffData* DataB, int LowerB, int UpperB, vector<int>& oDownVector, vector<int>& oUpVector);

		static int CreateDiffs(vector<Item*>& oResult , CDiffData* DataA, CDiffData* DataB);
	};
};