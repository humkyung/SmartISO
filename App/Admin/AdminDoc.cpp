// AdminDoc.cpp : implementation of the CAdminDoc class
//

#include "stdafx.h"
#include "Admin.h"
#include <util/FileTools.h>
#include <util/Path.h>
#include <Splash/SplashScreenFx.h>

#include "tokenizer.h"
#include "FieldTypeGridCell.h"
#include "ExcelMapingGridCell.h"
#include "AdminView.h"
#include "AdminDoc.h"
#include "AdminDocData.h"
#include <ado/SqlFile.h>

#include "soci.h"
#include "soci-sqlite3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdminDoc

IMPLEMENT_DYNCREATE(CAdminDoc, CDocument)

BEGIN_MESSAGE_MAP(CAdminDoc, CDocument)
	//{{AFX_MSG_MAP(CAdminDoc)
	ON_COMMAND(ID_APP_CLOSE, OnAppClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdminDoc construction/destruction
static CAdminDoc* __instance__ = NULL;
CAdminDoc::CAdminDoc() : m_modifyFlag(CLEAR_MODIFIED)
{
	__instance__ = this;
	m_pCurProject = NULL;
}

CAdminDoc::~CAdminDoc()
{
	try
	{
		if(NULL != m_pCurProject) delete m_pCurProject;
		__instance__ = NULL;
	}
	catch(...)
	{
		
	}
}

BOOL CAdminDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	/// exit app if no parameter - 2012.06.05 added by humkyung
	CSplashScreenFx* pSplash = CSplashScreenFx::GetInstance();
	if(pSplash)
	{
		pSplash->SetTextColor(RGB(255,0,0));
		pSplash->SetText( CString(_T("Run SmartAdmin through SmartISO")) + _T(" - click to continue") );
		MessageBeep(MB_ICONHAND);
		pSplash->WaitUntilUserConfirm();
	}
	/// up to here

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// CAdminDoc serialization

void CAdminDoc::Serialize(CArchive& ar)
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
// CAdminDoc diagnostics

#ifdef _DEBUG
void CAdminDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAdminDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAdminDoc commands

/******************************************************************************
    @author     humkyung
    @date       2012-01-27
    @class      CAdminDoc
    @function   GetInstance
    @return     CAdminDoc*
    @brief
******************************************************************************/
CAdminDoc* CAdminDoc::GetInstance()
{
	return __instance__;
}

/**
	@brief	
	@author	humkyung
**/
BOOL CAdminDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{	
	using namespace soci;
	CAdminDocData* pDocData = CAdminDocData::GetInstance();
	CSplashScreenFx* pSplash = CSplashScreenFx::GetInstance();

	vector<STRING_T> oResult;
	CTokenizer<CIsFromString>::Tokenize(oResult , lpszPathName , CIsFromString(_T("?")));
	if(2 != oResult.size()) return FALSE;
	
	const STRING_T sParam = oResult[1];
	CTokenizer<CIsFromString>::Tokenize(oResult , sParam , CIsFromString(_T(";")));
	if(2 != oResult.size()) return FALSE;
	const STRING_T sPrjName = oResult[0];
	const STRING_T sId = oResult[1];

	const CString sAppPath = CFileTools::GetAppDataPath();
	const CString sDatabaseFilePath = IsTools::CPath::Combine(sAppPath , PRODUCT_NAME , _T("Account.db") , 0);	/// 2015.08.19 added by humkyung
	try
	{
		map<CString,CString> sValueMap;

		soci::session aSession(*soci::factory_sqlite3() , sDatabaseFilePath.operator LPCTSTR());
		soci::rowset<soci::row> rs(aSession.prepare << "select * from Account where PrjName='" << sPrjName << "'");
		for(soci::rowset<row>::const_iterator itr = rs.begin();itr != rs.end();++itr)
		{
			for(std::size_t i = 0;i < itr->size();++i)
			{
				const column_properties& props = itr->get_properties(i);
				STRING_T sColName = props.get_name();
				IsString::ToUpper(sColName);
				const STRING_T sValue = itr->get<STRING_T>(i);
				sValueMap.insert(make_pair(sColName.c_str() , sValue.c_str()));
			}
		}

		if(!sValueMap[_T("PRJNAME")].IsEmpty() && !sValueMap[_T("FOLDER")].IsEmpty())
		{
			try
			{
				CString sPrjFolderPath = sValueMap[_T("FOLDER")];
				if(_T("\\") != sPrjFolderPath.Right(1)) sPrjFolderPath += _T("\\");
				m_rIniFilePath = sPrjFolderPath + CString(_T("\\Setting\\")) + sValueMap[_T("PRJNAME")] + CString(_T(".ini"));
				pDocData->SetIniFilePath(m_rIniFilePath);
		
				/*CString rPara[2];
				rPara[0] = oResult[1].c_str();
				CTokenizer<CIsComma>::Tokenize(oResult , rPara[0].operator LPCTSTR() , CIsComma());
				rPara[0] = oResult[0].c_str();
				rPara[1] = oResult[1].c_str();
				CTokenizer<CIsFromString>::Tokenize(oResult , rPara[0].operator LPCTSTR() , CIsFromString("="));*/
				const CString rId = sId.c_str();
				/*CTokenizer<CIsFromString>::Tokenize(oResult , rPara[1].operator LPCTSTR() , CIsFromString("="));
				CString rPass = oResult[1].c_str();*/

				/// check ini file exist - 2012.12.30 added by humkyung
				if(FALSE == CFileTools::DoesFileExist(m_rIniFilePath))
				{
					if(pSplash)
					{
						pSplash->SetTextColor(RGB(255,0,0));
						CString sMsg;	
						sMsg.Format(_T("Can't find %s - click to continue") , m_rIniFilePath);
						pSplash->SetText( sMsg );
						MessageBeep(MB_ICONHAND);
						pSplash->WaitUntilUserConfirm();
					}

					return FALSE;
				}
				/// up to here

				if (!CDocument::OnOpenDocument(m_rIniFilePath)) return FALSE;

				TCHAR szBuf[MAX_PATH] = {0,};
				m_ProjectName = sPrjName.c_str();
				pDocData->m_ProjectName = sPrjName.c_str();

				m_ProjectPath = sPrjFolderPath;
				pDocData->m_ProjectPath = sPrjFolderPath;
					
				BeginWaitCursor();
				//////////////////////////////////////////////////////////////////////////
				if(NULL != m_pCurProject) SAFE_DELETE(m_pCurProject);
				//////////////////////////////////////////////////////////////////////////
					
				if(NULL == m_pCurProject)
				{
					m_pCurProject = CProject::LoadFromIniFile(m_rIniFilePath);
					if(NULL != m_pCurProject)
					{
						CProject::DatabaseProp prop;
						{
							prop.Host = sValueMap[_T("HOST")];
							prop.User = sValueMap[_T("ID")];
							prop.Password = sValueMap[_T("PASSWORD")];
							prop.Port = _T("5432");
							m_pCurProject->SetDatabaseProp(prop);
						}

						pDocData->LoadData();
						
						POSITION pos = GetFirstViewPosition();
						while(NULL != pos)
						{
							CView* pView = GetNextView(pos);
							if(pView->IsKindOf(RUNTIME_CLASS(CAdminView)))
							{
								CAdminView* pAdminView = static_cast<CAdminView*>(pView);
								pAdminView->UpdateContents();
							}
						}
					}
					else
					{
						if(pSplash)
						{
							pSplash->SetTextColor(RGB(255,0,0));
							pSplash->SetText( _T("Fail to load project!!! - click to continue") );
							MessageBeep(MB_ICONHAND);
							pSplash->WaitUntilUserConfirm();
						}
						return FALSE;
					}
				}
				if(pSplash) pSplash->Hide();
				EndWaitCursor();
			}
			catch(const exception& ex)
			{
				if(pSplash)
				{
					pSplash->SetTextColor(RGB(255,0,0));
					pSplash->SetText(ex.what());
					MessageBeep(MB_ICONHAND);
					pSplash->WaitUntilUserConfirm();
				}
				return FALSE;
			}
		}
		else
		{
			if(pSplash)
			{
				pSplash->SetTextColor(RGB(255,0,0));
				pSplash->SetText( _T("wrong parameter - click to continue") );
				MessageBeep(MB_ICONHAND);
				pSplash->WaitUntilUserConfirm();
			}
			return FALSE;
		}
	}
	catch(const exception& ex)
	{
		if(pSplash)
		{
			pSplash->SetTextColor(RGB(255,0,0));
			pSplash->SetText( _T("wrong parameter - click to continue") );
			MessageBeep(MB_ICONHAND);
			pSplash->WaitUntilUserConfirm();
		}
		return FALSE;
	}

	return TRUE;
}

/**
	@brief	return current project
	@author	humkyung
	@date	????.??.??
*/
CProject* CAdminDoc::GetCurProject()
{
	return m_pCurProject;
}

void CAdminDoc::OnAppClose() 
{
	TCHAR szPathName[MAX_PATH] = {0,};
	GetModuleFileName(AfxGetInstanceHandle() , szPathName , MAX_PATH);
	
	CString rPathName(szPathName);
	int it = 0;
	it = rPathName.ReverseFind('\\');
	if(it)
	{
		rPathName = rPathName.Left(it);
		::ShellExecute(NULL, _T("open") , rPathName + _T("\\SmartISO.exe") , _T("") , rPathName , SW_SHOWNORMAL);
		
		AfxGetMainWnd()->PostMessage(WM_CLOSE); 
	}	
}

void CAdminDoc::SetTitle(LPCTSTR lpszTitle)
{
	m_strTitle = m_ProjectName;
	UpdateFrameCounts();        // will cause name change in views
}
