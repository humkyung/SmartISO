// TestSuit.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <tchar.h>
#include <gtest/gtest.h>

#include "../SmartISODiffInf.h"

#if defined (_DEBUG)
#pragma comment (lib, "gtestd.lib")
#else
#pragma comment (lib, "gtest.lib")
#endif

TEST(SmartISO , Diff_Test)
{
	char* pNewFilePath = "e:\\PROJECT\\9SQ\\Projects\\HANWHA\\IsoManPlus\\DATA\\BUTENE1\\isoout\\CVDCSD21351.i02";
	char* pOldFilePath = "e:\\PROJECT\\9SQ\\Projects\\HANWHA\\IsoManPlus\\DATA\\BUTENE1\\isoout\\CVDCSD21351.i01";
	char* pIniFilePath = "e:\\PROJECT\\9SQ\\Projects\\HANWHA\\IsoManPlus\\DATA\\BUTENE1\\ISO_BM_DATA\\Setting\\BUTENE1.ini";

	SmartISODiff(pNewFilePath , pOldFilePath , pIniFilePath);
}

int _tmain(int argc, _TCHAR* argv[])
{
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}

