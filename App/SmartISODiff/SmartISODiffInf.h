#pragma once

#ifndef FN_REVISO_MODULE
	#ifdef _UNICODE
		#if defined _DEBUG || defined _DEBUG_BUILD
			#pragma comment(lib, "SmartISODiff")
			#pragma message("Automatically linking with SmartISODiff.dll (Debug Unicode)")
		#else
			#pragma comment(lib, "SmartISODiff")
			#pragma message("Automatically linking with SmartISODiff.dll (Release Unicode)")
		#endif
	#else
		#if defined _DEBUG || defined _DEBUG_BUILD
			#pragma comment(lib, "SmartISODiff")
			#pragma message("Automatically linking with SmartISODiff.dll (Debug)")
		#else
			#pragma comment(lib, "SmartISODiff")
			#pragma message("Automatically linking with SmartISODiff.dll (Release)")
		#endif
	#endif

	extern "C" __declspec(dllexport) int SmartISODiff(const LPCTSTR pNewFilePath , const LPCTSTR pOldFilePath , const LPCTSTR pIniFilePath);
#endif
