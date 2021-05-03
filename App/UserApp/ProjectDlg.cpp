// ProjectFormDlg.cpp : implementation file
//

#include "stdafx.h"

#include <Project.h>
#include <ADO\ADODB.h>
#include <ado/AdoRemoteDB.h>
#include <Util/Registry.h>
#include <util/FileTools.h>
#include <util/Path.h>

#include "SplitPath.h"
#include "ProjectDlg.h"
#include "UserAppDocData.h"

#include "soci.h"
#include "soci-sqlite3.h"
#include "soci-postgresql.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProjectDlg

IMPLEMENT_DYNCREATE(CProjectDlg, CDialog)

CProjectDlg::CProjectDlg()
	: CDialog(CProjectDlg::IDD)
{
	//{{AFX_DATA_INIT(CProjectDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CProjectDlg::~CProjectDlg()
{
}

void CProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProjectDlg)
	DDX_Control(pDX , IDC_EDIT_CLIENT_NAME , m_wndClientName);
	DDX_Control( pDX , IDC_EDIT_FOLDER , m_wndProjectFolder );
	DDX_Control(pDX , IDC_BUTTON_CREATE , m_btnCreateProject);
	DDX_Control(pDX , IDC_BUTTON_DEL , m_btnDeleteProject);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProjectDlg, CDialog)
	//{{AFX_MSG_MAP(CProjectDlg)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_DEL, OnUpdateButtonDel)
	//}}AFX_MSG_MAP
	ON_WM_HELPINFO()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProjectDlg diagnostics

#ifdef _DEBUG
void CProjectDlg::AssertValid() const
{
	CDialog::AssertValid();
}

void CProjectDlg::Dump(CDumpContext& dc) const
{
	CDialog::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CProjectDlg message handlers
BOOL CProjectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(hIcon, TRUE);		/// Set big icon
	SetIcon(hIcon, FALSE);		/// Set small icon

	m_NameEdit.SubclassWindow(GetDlgItem(IDC_EDIT_PROJECT_NAME)->m_hWnd);
	CString rRegEx= _T("([^?:/<>*|])*([a-zA-Z0-9 ])*([^?:/<>*|])*");
	m_NameEdit.SetRegEx(rRegEx);
	m_NameEdit.CreateToolTip(this , _T("프로젝트 이름으로 사용할 수 없는 글자입니다."));
	m_NameEdit.SetBackgroundColourFocus (RGB (150, 200, 255));
	m_NameEdit.SetForegroundColourError (RGB (255, 255, 255));
	m_NameEdit.SetBackgroundColourError (RGB (255, 0, 0));

	m_projectNoEdit.SubclassWindow(GetDlgItem(IDC_EDIT_PROJECT_NO)->m_hWnd);
	rRegEx = _T("(.)*([a-zA-Z0-9 ])*(.)*");
	m_projectNoEdit.SetRegEx(rRegEx);
	m_projectNoEdit.CreateToolTip(this , _T("프로젝트 설명으로 사용할 수 없는 글자입니다."));
	m_projectNoEdit.SetBackgroundColourFocus (RGB (150, 200, 255));
	m_projectNoEdit.SetForegroundColourError (RGB (255, 255, 255));
	m_projectNoEdit.SetBackgroundColourError (RGB (255, 0, 0));

	short shBtnColor = 30;
	m_btnCreateProject.SetIcon(IDI_NEW);

	m_btnDeleteProject.SetIcon(IDI_CANCEL);

	m_ProjectReportCtrl.SubclassWindow(GetDlgItem(IDC_LIST_JOB)->m_hWnd);
	m_ProjectReportCtrl.SetHeadings(_T("Name , 100; Desc,100; PDS Name , 100; Project No , 100;Path , 200; Date , 150"));

	m_ProjectReportCtrl.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_FULLROWSELECT);
	BOOL b = m_ImageList.Create(IDB_PROJECT , 16 , 16 , RGB(255 , 255 , 255));
	if(TRUE == b)
	{
		m_ProjectReportCtrl.SetImageList(&m_ImageList , LVSIL_SMALL);
	}
	else
	{
		AfxMessageBox( _T("Can't create imagelist!!!") );
	}
	m_ProjectReportCtrl.SetHeaderImageList();

	UpdateContents();

	this->CenterWindow();	/// set window position to center of screen - 2012.07.02 added by humkyung

	return TRUE;
}

struct MatchProject : binary_function<CProjectDlg::ProjectInfo , CString , bool>
{
	bool operator()(const CProjectDlg::ProjectInfo& lhs , const CString& rhs) const
	{
		return (lhs.Name == rhs);
	}
};

/******************************************************************************
    @author     humkyung
    @date       2012-08-12
    @function   SyncTableToTableDefFile
    @return     int
    @param      CAppPosgreSQL&
    @brief
******************************************************************************/
int CProjectDlg::SyncTableToTableDefFile(CAppPostgreSQL& database)
{
	const STRING_T SqlFiles[]=
	{
		_T("T_PROJECT_SETTING.SQL") , _T("T_ISO_LINE_DATA.XML") , _T("T_ISSUED_ISO_LINE_DATA.XML") , _T("T_ISO_BOM_DATA.XML") , _T("T_ISSUED_ISO_BOM_DATA.XML"),
		_T("T_ISO_REVISION.XML") , _T("T_ISSUED_ISO_REVISION.XML") , _T("T_ISSUE_DATE.XML") , _T("T_ISO_DWG_FILE.XML") , _T("T_GRADE_DEFINE.XML") ,
		_T("T_REVISION_STAGE.XML") , _T("T_LINE_LIST_DATA.XML") , _T("T_LINE_LIST_DATA_MAPPING.XML") , _T("T_ISO_DRAWING_SETUP.XML")
	};

	for(int i = 0;i < SIZE_OF_ARRAY(SqlFiles);++i)
	{
		const STRING_T sSqlFilePath = STRING_T(CAppDocData::GetExecPath().operator LPCTSTR()) + _T("\\Setting\\") + SqlFiles[i];

		CSQLFile sqlFile(sSqlFilePath);
		sqlFile.m_eDataSourceType = CSQLFile::AAAPIDB_POSTGRESQL;
		STRING_T::size_type at = sSqlFilePath.find(_T(".XML"));
		if(STRING_T::npos != at)
		{
			if(ERROR_SUCCESS != sqlFile.ParseXML(sSqlFilePath)) return ERROR_BAD_ENVIRONMENT;
		}
		else
		{
			if(ERROR_SUCCESS != sqlFile.Parse()) return ERROR_BAD_ENVIRONMENT;
		}
		
		const STRING_T sTableName = SqlFiles[i].substr(0 , SqlFiles[i].find(_T(".")));
		if(ERROR_SUCCESS == database.SyncTable(sqlFile , sTableName.c_str()))
		{
			/// 2012.08.15 added by humkyung
			if(_T("T_PROJECT_SETTING") == sTableName)
			{
				try
				{
					long long lRecordCount = 0L;
					(*database.session()) << _T("select count(*) from ") + sTableName , soci::into(lRecordCount);
					if(0 == lRecordCount)
					{
						STRING_T sql = _T("INSERT INTO ") + sTableName + _T(" DEFAULT VALUES");
						(*database.session()) << sql;

						sql = _T("UPDATE ") + sTableName + _T(" SET dwg_report_excel_file_data=lo_creat(-1)");
						(*database.session()) << sql;
					}
				}
				catch(...){}
				/// up to here
			}
			else if(_T("T_LINE_LIST_DATA_MAPPING") == sTableName)
			{
				long long lRecordCount = 0L;
				(*database.session()) << _T("select count(*) from ") + sTableName , soci::into(lRecordCount);
				if(0 == lRecordCount)
				{
					const STRING_T rSql = _T("INSERT INTO ") + sTableName + _T(" DEFAULT VALUES");
					(*database.session()) << rSql;
				}
			}
		}
	}

	return ERROR_SUCCESS;
}

/**
	@brief	create a project folder with given name
	@author	humkyung
	@date	2015.08.19
*/
int CProjectDlg::CreateProjectIfNeed(const CString& sPrjName)
{
	using namespace soci;

	int res = ERROR_SUCCESS;

	const CString sAppPath = CFileTools::GetAppDataPath();
	const CString sDatabaseFilePath = IsTools::CPath::Combine(sAppPath , PRODUCT_NAME , _T("Account.db") , 0);	/// 2015.08.19 added by humkyung

	try
	{
		map<CString,CString> sValueMap;

		soci::session aSession(*soci::factory_sqlite3() , sDatabaseFilePath.operator LPCTSTR());
		soci::rowset<soci::row> rs(aSession.prepare << "select * from Account where PrjName='" << sPrjName.operator LPCSTR() << "'");
		for(soci::rowset<row>::const_iterator itr = rs.begin();itr != rs.end();++itr)
		{
			for(std::size_t i = 0;i < itr->size();++i)
			{
				const column_properties& props = itr->get_properties(i);
				string sColName = props.get_name();
				IsString::ToUpper(sColName);
				const std::string sValue = itr->get<std::string>(i);
				sValueMap.insert(make_pair(sColName.c_str() , sValue.c_str()));
			}
		}

		CAppDocData* pDocData = CAppDocData::GetInstance();
		if(!sValueMap[_T("PRJNAME")].IsEmpty() && !sValueMap[_T("PRJCODE")].IsEmpty() && !sValueMap[_T("FOLDER")].IsEmpty())
		{
			try
			{
				CString sPrjIniFilePath = sValueMap[_T("FOLDER")];
				if(_T("\\") != sPrjIniFilePath.Right(1)) sPrjIniFilePath += _T("\\");
				sPrjIniFilePath += CString(_T("Setting\\")) + sValueMap[_T("PRJNAME")] + CString(_T(".ini"));
				/// read MSTN J path name from registry - 2012.05.12 added by humkyung
				CString sRegistry(_T("SOFTWARE\\Bentley\\MicroStation\\07.01")) , sMSTNPathName;
				CRegistry reg;
				if(reg.VerifyKey(HKEY_LOCAL_MACHINE , sRegistry))
				{
					/// Open key
					if (reg.Open(HKEY_LOCAL_MACHINE , sRegistry))
					{
						/// try to read pathname
						reg.Read(_T("PathName"), sMSTNPathName);

						/// Close registry
						reg.Close();
					}
				}
				/// up to here

				/// create a new project
				CProject* pProject = CProject::CreateInstance();
				if(NULL != pProject)
				{
					pDocData->SetProject(pProject);

					pProject->SetName(sValueMap[_T("PRJNAME")].operator LPCTSTR());
					pProject->SetDesc(sValueMap[_T("PRJDESC")].operator LPCTSTR());	/// 2013.01.05 added by humkyung
					pProject->SetProjectNo(sValueMap[_T("PROJECT_NO")].operator LPCTSTR());
					pProject->SetComment(_T(""));
					pProject->SetClientName(sValueMap[_T("CLIENT_NAME")]);
					pProject->SetPath(sValueMap[_T("FOLDER")]);
					
					CProject::DatabaseProp prop;
					{
						prop.Host = sValueMap[_T("HOST")];
						prop.User = sValueMap[_T("ID")];
						prop.Password = sValueMap[_T("PASSWORD")];
						prop.Port = _T("5432");
					}
					pProject->SetDatabaseProp(prop);

					CString rMsg;
					CTime t = CTime::GetCurrentTime();
					rMsg.LoadString( IDS_JOB_CREATED_TIME );
					pProject->SetDate(t.Format( rMsg ));

					if( pProject->CreateFolder(sValueMap[_T("FOLDER")]) ) /// 폴더 생성과 JOB에 필요한 파일들을 복사한다.
					{
						ProjectInfo projectInfo;
						projectInfo.Name = sValueMap[_T("PRJNAME")];
						projectInfo.path = sValueMap[_T("FOLDER")];

						CAppPostgreSQL database(sValueMap[_T("PRJNAME")] , prop.Host.c_str() , prop.User.c_str() , prop.Password.c_str());
						{
							CProjectDlg::SyncTableToTableDefFile(database);	/// 2012.08.12 added by humkyung

							/*CString rFormat = _T("UPDATE T_PROJECT_SETTING SET PROJECT_DESC='%s',PDS_NAME='%s',PROJECT_NO='%s',JOB_NO='%s',CLIENT_NAME='%s',PROJECT_FOLDER='%s',COMMENT='%s',DATE_='%s'");
							rFormat += _T(" WHERE PROJECT='%s'");

							projectInfo.desc = sValueMap[_T("DESC")];
							projectInfo.pds_name = sValueMap[_T("PDS_NAME")];
							projectInfo.project_no = sValueMap[_T("PROJECT_NO")];
							projectInfo.job_no = sValueMap[_T("JOB_NO")];
							projectInfo.client_name = sValueMap[_T("CLIENT_NAME")];
							CString rString;
							rString.Format(rFormat , projectInfo.desc.c_str() , projectInfo.pds_name.c_str() , projectInfo.project_no.c_str() , projectInfo.job_no.c_str() , projectInfo.client_name.c_str() , 
								projectInfo.path , projectInfo.comment.c_str() , pProject->date() , projectInfo.Name);
							adoDB.ExecuteQuery(rString.operator LPCTSTR());*/

							/// update ini file
							/*TCHAR szBuf[MAX_PATH] = {'\0',};
							GetPrivateProfileString(PRODUCT_NAME , _T("Project_Count") , NULL , szBuf , MAX_PATH , pDocData->GetPrjIniFilePath());
							rString.Format(_T("%d") , atoi(szBuf) + 1);
							WritePrivateProfileString(PRODUCT_NAME , _T("Project_Count") , rString , pDocData->GetPrjIniFilePath());
							rString.Format(_T("Project_Name%d") , atoi(szBuf));
							WritePrivateProfileString(PRODUCT_NAME , rString , projectInfo.Name , pDocData->GetPrjIniFilePath());
							WritePrivateProfileString(projectInfo.Name , _T("Path") , projectInfo.path , pDocData->GetPrjIniFilePath());
							WritePrivateProfileString(projectInfo.Name , _T("Date") , pProject->date() , pDocData->GetPrjIniFilePath());	/// 2012.08.22 added by humkyung*/
						}
						/*else
						{
							AfxMessageBox(_T("Can't connect to database") , MB_OK|MB_ICONEXCLAMATION);
							res = ERROR_BAD_ENVIRONMENT;
						}*/
					}
				}
				else
				{
					AfxMessageBox(_T("Fail to create project instance") , MB_OK|MB_ICONEXCLAMATION);
					res = ERROR_BAD_ENVIRONMENT;
				}
			}
			catch(const exception& ex)
			{
				AfxMessageBox(ex.what());
				res = ERROR_BAD_ENVIRONMENT;
			}
			catch(...)
			{	
				AfxMessageBox( _T("unexpected error") );
				res = ERROR_BAD_ENVIRONMENT;
			}
		}
		else
		{
			res = ERROR_BAD_ENVIRONMENT;
		}
	}
	catch(std::exception& ex)
	{
	}

	return res;
}

/******************************************************************************
    @author     humkyung
    @date       2012-08-12
    @function   OnSize
    @return     void
    @param      UINT    nType
    @param      int     cx
    @param      int     cy
    @brief
******************************************************************************/
void CProjectDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	/*CWnd* pWnd = (CWnd*)GetDlgItem(IDC_STATIC_PROJECT);
	if(pWnd && pWnd->m_hWnd)
	{
		pWnd->SetWindowPos(this , 0 , 0 , cx , 30 , 0x0000);
	}*/
}

/**	
	@brief	선택한 프로젝트를 삭제한다. ini 파일을 갱신한다.


	@return	void	
*/
void CProjectDlg::OnButtonDel() 
{
	if(m_ProjectReportCtrl.GetSelectedItemCount() > 0)
	{
		int res = AfxMessageBox(_T("Do you want to delete selected project?") , MB_YESNO|MB_ICONQUESTION);
		if(IDYES == res)
		{
			CAppDocData* pDocData = CAppDocData::GetInstance();

			POSITION pos = m_ProjectReportCtrl.GetFirstSelectedItemPosition();
			while(NULL != pos)
			{
				const int nItem = m_ProjectReportCtrl.GetNextSelectedItem(pos);
				if(-1 != nItem)
				{
					CString rProjectName = m_ProjectReportCtrl.GetItemText(nItem , 0);
					m_ProjectReportCtrl.DeleteItem(nItem);

					list<ProjectInfo>::iterator where = find_if(m_ProjectInfoEntry.begin() , m_ProjectInfoEntry.end() , bind2nd(MatchProject() , rProjectName.operator LPCSTR()));
					if(where != m_ProjectInfoEntry.end())
					{
						m_ProjectInfoEntry.erase(where);
					}
					
					//! update ini file
					TCHAR szBuf[MAX_PATH] = {'\0',};
					CString rString;
					rString.Format(_T("%d") , m_ProjectInfoEntry.size());
					WritePrivateProfileString(PRODUCT_NAME , _T("Project_Count") , rString , pDocData->GetPrjIniFilePath());
					
					int nCount = 0;
					for(list<ProjectInfo>::iterator itr = m_ProjectInfoEntry.begin();itr != m_ProjectInfoEntry.end();++itr,++nCount)
					{
						rString.Format(_T("Project_Name%d") , nCount);
						WritePrivateProfileString(PRODUCT_NAME , rString , itr->Name , pDocData->GetPrjIniFilePath());
						WritePrivateProfileString(itr->Name , _T("Path") , itr->path , pDocData->GetPrjIniFilePath());
					}
					
					break;
				}
			}
		}
	}
}

/******************************************************************************
    @author     humkyung
    @date       2012-05-26
    @function   OnUpdateButtonDel
    @return     void
    @param      CCmdUI* pCmdUI
    @brief
******************************************************************************/
void CProjectDlg::OnUpdateButtonDel(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_ProjectReportCtrl.GetSelectedItemCount());
}

/**	
	@brief	필드를 지운다.

	@author	HumKyung

	@date	????.??.??

	@param

	@return	void	
*/
void CProjectDlg::ClearPropertyField()
{
	m_NameEdit.SetWindowText(_T(""));
	m_projectNoEdit.SetWindowText( _T("") );
	SetDlgItemText(IDC_EDIT_FOLDER , _T(""));

	//! 2011.01.24 - added by HumKyung
	SetDlgItemText(IDC_EDIT_PDS_NAME , _T(""));

	/// 2012.05.11 added by humkyung
	//SetDlgItemText(IDC_EDIT_HOST , _T(""));
	//SetDlgItemText(IDC_EDIT_SERVICE_NAME , _T(""));
	//SetDlgItemText(IDC_EDIT_USER_NAME , _T(""));
	SetDlgItemText(IDC_EDIT_PASSWORD , _T(""));
	//SetDlgItemText(IDC_EDIT_PDS_PRJ_NAME , _T(""));
	/// up to here
}

/**	
	@brief	프로젝트의 내용을 list control에 채운다.

	@author	humkyung

	@return	void	
*/
void CProjectDlg::UpdateContents()
{
}

/******************************************************************************
    @author     humkyung
    @date       2012-05-26
    @function   PreTranslateMessage
    @return     BOOL
    @param      MSG*    pMsg
    @brief
******************************************************************************/
BOOL CProjectDlg::PreTranslateMessage(MSG* pMsg)
{
	UpdateDialogControls(this, TRUE);

	return CDialog::PreTranslateMessage(pMsg);
}

/******************************************************************************
    @author     humkyung
    @date       2012-05-26
    @function   OnHelpInfo
    @return     BOOL
    @param      HELPINFO*   pHelpInfo
    @brief
******************************************************************************/
BOOL CProjectDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	CString sHelpPage(CAppDocData::GetExecPath() + _T("\\Help\\SmartISO.chm::/프로젝트_생성.htm"));
	HWND hHtml = ::HtmlHelp(this->m_hWnd, sHelpPage , HH_DISPLAY_TOPIC, 0);

	return TRUE;///CDialog::OnHelpInfo(pHelpInfo);
}

/******************************************************************************
    @author     humkyung
    @date       2012-08-29
    @class      CProjectDlg
    @function   OnClose
    @return     void
    @brief
******************************************************************************/
void CProjectDlg::OnClose()
{
	//m_pRuntimeHost->Stop();

	CDialog::OnClose();
}
