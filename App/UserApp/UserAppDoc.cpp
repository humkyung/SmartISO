// UserAppDoc.cpp : implementation of the CUserAppDoc class
//

#include "stdafx.h"
#include "UserApp.h"
#include "MainFrm.h"

#include "UserAppDocData.h"
#include "tokenizer.h"
#include "UserAppDoc.h"
#include "UserAppView.h"
#include "UserAppSettingSheet.h"
#include "BackupDataForm.h"
#include "ResourceSwitch.h"
#include <SplitPath.h>
#include "Profile.h"
#include "WorkStatusDlg.h"
#include <Splash/SplashScreenFx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUserAppDoc

IMPLEMENT_DYNCREATE(CUserAppDoc, CDocument)

BEGIN_MESSAGE_MAP(CUserAppDoc, CDocument)
	//{{AFX_MSG_MAP(CUserAppDoc)
	ON_COMMAND(ID_APP_CLOSE, OnAppClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUserAppDoc construction/destruction
static CUserAppDoc* __instance__ = NULL;
CUserAppDoc::CUserAppDoc()
{
	__instance__ = this;
	m_bRevAutomation = FALSE;
	m_bIssueAutomation = FALSE;
	m_bUseIPLOT = FALSE;
}

CUserAppDoc::~CUserAppDoc()
{
	try
	{
	}
	catch(...)
	{
		
	}
}

/******************************************************************************
    @author     humkyung
    @date       ????-??-??
    @class      CUserAppDoc
    @function   OnNewDocument
    @return     BOOL
    @brief
******************************************************************************/
BOOL CUserAppDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	CSplashScreenFx* pSplash = CSplashScreenFx::GetInstance();
	if(pSplash)
	{
		pSplash->SetTextColor(RGB(255,0,0));
		pSplash->SetText( _T("Run SmartUserApp through") + CString(PRODUCT_NAME) + _T(" - click to continue") );
		MessageBeep(MB_ICONHAND);
		pSplash->WaitUntilUserConfirm();
	}

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CUserAppDoc serialization

void CUserAppDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CUserAppDoc diagnostics

#ifdef _DEBUG
void CUserAppDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CUserAppDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CUserAppDoc commands

/**	
	@brief	프로젝트를 로드한다.
	@author	humkyung
	@date	????.??.??
	@param	lpszPathName	a parameter of type LPCTSTR
	@return	BOOL	
*/
BOOL CUserAppDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	CRect rect;
	CMainFrame* pMainFrame = GetUserAppMainWnd();
	{
		pMainFrame->GetWindowRect(&rect);
		pMainFrame->MoveWindow(0 , 0 , 1 , 1);
		pMainFrame->CenterWindow();
	}

	CSplashScreenFx* pSplash = CSplashScreenFx::GetInstance();

	vector<STRING_T> oResult;
	CTokenizer<CIsFromString>::Tokenize(oResult , lpszPathName , CIsFromString( _T("?") ));
	if(2 == oResult.size())
	{
		CString rPath = oResult[0].c_str();
		CString rPara[3];
		rPara[0] = oResult[1].c_str();

		CTokenizer<CIsComma>::Tokenize(oResult , rPara[0].operator LPCTSTR() , CIsComma());
		
		rPara[0].Empty(); rPara[1].Empty(); rPara[2].Empty();
		int iIndex = 0;
		for(vector<STRING_T>::const_iterator itr = oResult.begin();(itr != oResult.end()) && (iIndex < 3);++itr,++iIndex)
		{
			rPara[iIndex] = itr->c_str();
		}

		CTokenizer<CIsFromString>::Tokenize(oResult , rPara[0].operator LPCTSTR() , CIsFromString(_T("=")));
		CString rId = oResult[1].c_str();
		CTokenizer<CIsFromString>::Tokenize(oResult , rPara[1].operator LPCTSTR() , CIsFromString(_T("=")));
		CString rPass = oResult[1].c_str();
		CString rSite(_T("ALL"));
		
		if (!CDocument::OnOpenDocument(rPath))
		return FALSE;

		CAppDocData* pDocData = CAppDocData::GetInstance();
		pDocData->Initialize(rPath);
		pDocData->m_sUserId = rId;
		pDocData->m_rSiteName = rSite;
		pDocData->GetProject()->LoadSetting(pDocData->GetPrjIniFilePath());

		/*static const CString rApp( _T("General") );
		TCHAR szBuf[MAX_PATH + 1] = {0,};
		if(GetPrivateProfileString( rApp , _T("Name") , NULL , szBuf , MAX_PATH , rPath))
		{
			pDocData->m_ProjectName = szBuf;
		}
		else
		{
			if(NULL != pSplash)
			{
				pSplash->SetTextColor(RGB(255,0,0));
				pSplash->SetText( _T("Project Name이 없습니다. - click to continue") );
				MessageBeep(MB_ICONHAND);
				pSplash->WaitUntilUserConfirm();
			}
			else
			{
				AfxMessageBox(_T("Project Name이 없습니다."));
			}
		}

		if(GetPrivateProfileString( rApp , _T("Path") , NULL , szBuf , MAX_PATH , rPath))
		{
			pDocData->m_ProjectPath = szBuf;
			if('\\' == pDocData->m_ProjectPath.Right(1)) pDocData->m_ProjectPath = pDocData->m_ProjectPath.Left(pDocData->m_ProjectPath.GetLength() - 1);
		}
		else
		{
			if(NULL != pSplash)
			{
				pSplash->SetTextColor(RGB(255,0,0));
				pSplash->SetText( _T("Can't find project path. - click to continue") );
				MessageBeep(MB_ICONHAND);
				pSplash->WaitUntilUserConfirm();
			}
			else
			{
				AfxMessageBox(_T("There is no project path."));
			}
		}*/

		BeginWaitCursor();

		//if(NULL == m_pSelProject)
		{
			//if(NULL != pDocData->m_pSelUser)
			{
				pDocData->InitContents();
				AddNewIsoDwgDataToDatabase(pDocData->GetPrjIniFilePath());
				
				//////////////////////////////////////////////////////////////////////////
				CUserAppView* pUserAppView = (CUserAppView*)(CUserAppView::GetInstance());
				if(pUserAppView) pUserAppView->InitContents();
				
				pMainFrame->GetWorkspaceBar()->InitContents();

				CUserAppOutputWnd* pUserAppOutputWnd = (CUserAppOutputWnd*)(pMainFrame->GetUserAppOutputBar());
				if(pUserAppOutputWnd)
				{
					pUserAppOutputWnd->InitContents();
					pUserAppOutputWnd->Invalidate();
				}

				//! 2011.01.20 - added by HumKyung
				if(pMainFrame)
				{
					pMainFrame->SendMessage(WM_COMMAND , ID_LINE_DATA);
				}
			}
		}
		EndWaitCursor();

		if(pSplash) pSplash->Hide();
	}
	else
	{
		if(NULL != pSplash)
		{
			pSplash->SetTextColor(RGB(255,0,0));
			pSplash->SetText( _T("잘못된 인자입니다. - click to continue") );
			MessageBeep(MB_ICONHAND);
			pSplash->WaitUntilUserConfirm();
		}
		return FALSE;
	}

	{
		CMainFrame* pMainFrame = GetUserAppMainWnd();
		pMainFrame->MoveWindow(&rect);
		pMainFrame->MoveWindow(&rect);
	}

	CPropertiesWnd* pPropertiesWnd = CPropertiesWnd::GetInstance();
	pPropertiesWnd->SetWindowPos(NULL , 0 , 0 , 300 , 200 , SWP_NOZORDER | SWP_NOMOVE);
	pPropertiesWnd->ShowPane(FALSE , FALSE , FALSE);

	return TRUE;
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-27
    @class      CUserAppDoc
    @function   GetInstance
    @return     CUserAppDoc*
    @brief
******************************************************************************/
CUserAppDoc* CUserAppDoc::GetInstance()
{
	return __instance__;
}

/**	
	@brief	프로그램을 종료하고 SmartISO.exe로 돌아간다.

	@author	HumKyung

	@date	????.??.??

	@param

	@return	void	
*/
void CUserAppDoc::OnAppClose() 
{
	TCHAR szPathName[MAX_PATH] = {0,};
	GetModuleFileName(AfxGetInstanceHandle() , szPathName , MAX_PATH);
	
	CString rPathName(szPathName);
	const int it = rPathName.ReverseFind('\\');
	if(it)
	{
		rPathName = rPathName.Left(it);
		::ShellExecute(NULL, _T("open") , rPathName + _T("\\SmartISO.exe") , _T("") , rPathName , SW_SHOWNORMAL);
		
		GetUserAppMainWnd()->PostMessage(WM_CLOSE); 
	}
}

/**	
	@brief	The CUserAppDoc::SetTitle function
	@author	humkyung
	@param	lpszTitle	a parameter of type LPCTSTR
	@return	void	
*/
void CUserAppDoc::SetTitle(LPCTSTR lpszTitle)
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	m_strTitle = pDocData->GetProject()->name();
	UpdateFrameCounts();        // will cause name change in views
}

/**	
	@brief	데이터베이스에 들어가 있지 않은 Iso Dwg의 내용을 데이터베이스에 넣는다.

	@author	HumKyung.BAEK

	@date	2010.06.21

	@param	rIniFilePath

	@return	int	
*/
int CUserAppDoc::AddNewIsoDwgDataToDatabase(const CString& rIniFilePath)
{
	return ERROR_SUCCESS;
}
