// SmartISODiff.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "SmartISODiff.h"
#include <Tokenizer.h>
#include "IsoModel.h"
#include "IsoArea.h"
#include "IsoModelDocData.h"
#include "SmartISODiffDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CRevISOApp

BEGIN_MESSAGE_MAP(CSmartISODiffApp, CWinApp)
END_MESSAGE_MAP()


// CRevISOApp construction

CSmartISODiffApp::CSmartISODiffApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CRevISOApp object

CSmartISODiffApp theApp;


// CRevISOApp initialization

BOOL CSmartISODiffApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

/******************************************************************************
    @author     humkyung
    @date       2012-01-02
    @return     CDialog*
    @param      CWnd*
    @brief
******************************************************************************/
extern "C" __declspec(dllexport)
HWND CreateSettingDialog(HWND hWnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	static CRuntimeClass* pRuntimeClass = NULL;
	static CSmartISODiffDlg* __instance__ = NULL;

	if(NULL == __instance__)
	{
		CWnd *pParent = CWnd::FromHandle(hWnd);

		__instance__ = new CSmartISODiffDlg(pParent);
		__instance__->Create(CSmartISODiffDlg::IDD , pParent); 
	}
	/*if(NULL == pRuntimeClass)
	{
		pRuntimeClass = RUNTIME_CLASS(CSmartISODiffDlg);
		CObject* pObj = pRuntimeClass->CreateObject();
		__instance__ = dynamic_cast<CSmartISODiffDlg*> ( pObj );
		
		CWnd *pParent = CWnd::FromHandle(hWnd);
		if ( !__instance__->Create( CSmartISODiffDlg::IDD , pParent ) ) return NULL;
	}*/
	
	return __instance__->GetSafeHwnd();
}

/******************************************************************************
    @author     humkyung
    @date       2011-09-01
    @class
    @function   SmartISODiff
    @return     int
    @param      const LPCTSTR pNewFilePath
	@param      const LPCTSTR pOldFilePath
	@param      const LPCTSTR pIniFilePath
    @brief
******************************************************************************/
extern "C" __declspec(dllexport)
int SmartISODiff(const LPCTSTR pNewFilePath , const LPCTSTR pOldFilePath , const LPCTSTR pIniFilePath)
{
	int res = ERROR_SUCCESS;

	CIsoModelDocData& docData = CIsoModelDocData::GetInstance();
	docData.m_oKeyValueMap.clear ();
	docData.ReadSetting(pIniFilePath);

	/// read header information - 2011.09.21 added by humkyung
	CIsoBMData::m_oBMAttrPosMap.clear();
	CString HeaderTextArray[] = {_T("NO") , _T("MATL DESC") , _T("SIZE") , _T("MATL SPEC") , _T("SYM") , _T("MATL CODE") , _T("BOLT LEN") , _T("QTY")};
	TCHAR szBuf[MAX_PATH + 1] = {'\0'};
	for(int l = 0;l < SIZE_OF_ARRAY(HeaderTextArray);++l)
	{
		if(GetPrivateProfileStringA(("Iso Edit Setting") , HeaderTextArray[l] + _T("_HeaderInfo") , ("") , szBuf , MAX_PATH , pIniFilePath))
		{
			vector<STRING_T> oResult;
			CTokenizer<CIsComma>::Tokenize(oResult , CString(szBuf).operator LPCTSTR() , CIsComma());
			if(2 == oResult.size())
			{
				const int iStart = ATOI_T(oResult[0].c_str());
				const int iWidth = ATOI_T(oResult[1].c_str());
				CIsoBMData::m_oBMAttrPosMap.insert(make_pair(HeaderTextArray[l] , make_pair(iStart , iWidth)));
			}
		}
	}

	CIsoModel* pNewModel = new CIsoModel(pNewFilePath);
	if(NULL != pNewModel)
	{
		pNewModel->Read(pIniFilePath , true);
	}

	CIsoModel* pOldModel = new CIsoModel(pOldFilePath);
	if(NULL != pOldModel)
	{
		pOldModel->Read(pIniFilePath , false);
	}
	
	if((NULL != pNewModel) && (NULL != pOldModel))
	{
		res = pNewModel->CompareWith(pOldModel);
	}
	else
	{
		res = ERROR_BAD_ENVIRONMENT;
	}

	/// free allocaed memory
	SAFE_DELETE(pNewModel);
	SAFE_DELETE(pOldModel);

	return res;
}