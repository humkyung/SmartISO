
// SmartReporterDoc.cpp : implementation of the CSmartReporterDoc class
//

#include "stdafx.h"
#include "SmartReporter.h"
#include "SmartReporterView.h"
#include "SmartReporterDoc.h"
#include <IsString.h>
#include <Util/SplitPath.h>

#include "LineListToOraDocData.h"
#include <Splash/SplashScreenFx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSmartReporterDoc

IMPLEMENT_DYNCREATE(CSmartReporterDoc, CDocument)

BEGIN_MESSAGE_MAP(CSmartReporterDoc, CDocument)
ON_COMMAND(ID_VIEW_ORA_DATA_REPORT, OnViewOraDataReport)
	ON_COMMAND(ID_FILE_SAVE, &CSmartReporterDoc::OnFileSave)
END_MESSAGE_MAP()


// CSmartReporterDoc construction/destruction

CSmartReporterDoc::CSmartReporterDoc()
{
	// TODO: add one-time construction code here

}

CSmartReporterDoc::~CSmartReporterDoc()
{
}

/**	
	@brief	새로운 문서를 연다.

	@brief	humkyung

	@date	????.??.??

	@param

	@return
**/
BOOL CSmartReporterDoc::OnNewDocument()
{
	///if (!CDocument::OnNewDocument())
	///	return FALSE;

	CSplashScreenFx* pSplash = CSplashScreenFx::GetInstance();
	if(NULL != pSplash)
	{
		pSplash->SetTextColor(RGB(255,0,0));
		pSplash->SetText( _T("Run this program through SmartISO - click to continue") );
		MessageBeep(MB_ICONHAND);
		pSplash->WaitUntilUserConfirm();

		if(pSplash) pSplash->Hide();
	}

	return FALSE;
}

// CSmartReporterDoc serialization

void CSmartReporterDoc::Serialize(CArchive& ar)
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


// CSmartReporterDoc diagnostics

#ifdef _DEBUG
void CSmartReporterDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSmartReporterDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CSmartReporterDoc commands
/**
	@brief

	@author	humkyung

	@date	????.??.??

	@param

	@return
**/
void CSmartReporterDoc::OnViewOraDataReport() 
{
	POSITION pos = GetFirstViewPosition();
	while( NULL != pos )
	{
		CView* pView = GetNextView( pos );
		if( pView->IsKindOf( RUNTIME_CLASS( CSmartReporterView )))
		{
			CSmartReporterView* pSmartReporterView = static_cast<CSmartReporterView*>( pView );
			if( pSmartReporterView )
			{
				///pSmartReporterView->m_pWndOraDataReportView->ShowWindow( SW_SHOW );
				///pSmartReporterView->m_pWndOraDataReportView->UpdateWindow();
			}
		}
	}
}

/**
	@brief

	@author	humkyung

	@date	????.??.??

	@param

	@return
**/
void CSmartReporterDoc::OnFileSave()
{
	CSmartReporterView* pSmartReporterView = NULL;

	POSITION pos = GetFirstViewPosition();	
	while( NULL != pos )
	{
		CView* pView = GetNextView( pos );
		if(pView->IsKindOf(RUNTIME_CLASS( CSmartReporterView )))
		{
			pSmartReporterView = static_cast<CSmartReporterView*>(pView);
			break;
		}
	}
}

/******************************************************************************
    @author     humkyung
    @date       2012-05-03
    @class      CSmartReporterDoc
    @function   OnOpenDocument
    @return     BOOL
    @param      LPCTSTR lpszPathName
    @brief
******************************************************************************/
BOOL CSmartReporterDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	CSplashScreenFx* pSplash = CSplashScreenFx::GetInstance();
	if(FALSE == FileExist(lpszPathName))
	{
		pSplash->SetTextColor(RGB(255,0,0));
		pSplash->SetText( _T("Can't find ") + CString(lpszPathName) + _T(" - click to continue") );
		MessageBeep(MB_ICONHAND);
		pSplash->WaitUntilUserConfirm();

		if(pSplash) pSplash->Hide();
		return FALSE;
	}
	/*if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;*/

	m_rExecPath = GetExecPath().operator LPCTSTR();
	{	
		CLineListToOraDocData& docData = CLineListToOraDocData::GetInstance();
		m_rProjectFilePath = lpszPathName;///STRING_T(GetAppDataPath().operator LPCTSTR()) + _T("\\") + STRING_T(APPNAME) + _T("\\") + STRING_T(docData.m_sProjectName.operator LPCTSTR()) + _T(".ini");

		docData.LoadIni( m_rProjectFilePath );
		SetTitle( docData.m_sProjectName );

		///
		POSITION pos = GetFirstViewPosition();
		while(NULL != pos)
		{
			CView* pView = GetNextView(pos);
			if(pView->IsKindOf(RUNTIME_CLASS(CSmartReporterView)))
			{
				CSmartReporterView* pSmartReporterView = static_cast<CSmartReporterView*>(pView);
				if(pSmartReporterView)
				{
					pSmartReporterView->LoadData();
				}
				break;
			}
		}
	}
	if(pSplash) pSplash->Hide();

	CSplitPath path(m_rProjectFilePath.c_str());
	CWnd* pMainWnd = AfxGetMainWnd();
	if(NULL != pMainWnd) pMainWnd->SetWindowText(_T("SmartReporter - ") + path.GetFileName());

	return TRUE;
}

/******************************************************************************
    @author     humkyung
    @date       2012-05-05
    @function   GetProjectFilePath
    @return     CString
    @brief
******************************************************************************/
CString CSmartReporterDoc::GetProjectFilePath() const
{
	return m_rProjectFilePath.c_str();
}