// OracleSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "admin.h"
#include <ProjectData.h>
#include "AdminDocData.h"
#include "OracleSettingDlg.h"
#include "enablebuddybutton.h"
#include "FilterGridCell.h"
#include "FileNameFormatDlg.h"
#include "OracleMappingGridCell.h"

#include <tokenizer.h>
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COracleSettingDlg dialog
IMPLEMENT_DYNCREATE(COracleSettingDlg, CDialog)

COracleSettingDlg::COracleSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COracleSettingDlg::IDD, pParent), m_pAdminDoc(NULL), m_pOraDatabaseDefFile(NULL)
{
	//{{AFX_DATA_INIT(COracleSettingDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
COracleSettingDlg::~COracleSettingDlg()
{
	try
	{
		if(NULL != m_pOraDatabaseDefFile) delete m_pOraDatabaseDefFile;
	}
	catch(...)
	{
	}
}

void COracleSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COracleSettingDlg)
	DDX_Control(pDX , IDC_EDIT_IDMSKEY , m_IDMSKeyFormatEdit);
	DDX_Text( pDX , IDC_EDIT_IDMSKEY , m_rIDMSKeyFormat );
	DDX_Control( pDX , IDC_COMBO_ORAKEY , m_oraKeyComboBox );

	DDX_Control(pDX , IDC_BUTTON_NEW_MAPPING , m_btnNewMapping);
	DDX_Control(pDX , IDC_BUTTON_DELETE_MAPPING , m_btnDeleteMapping);

	DDX_Control(pDX , IDC_CHECK_PDS_DATA_UPDATE , m_wndPDSDataUpdateGroupCheckButton);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COracleSettingDlg, CDialog)
	//{{AFX_MSG_MAP(COracleSettingDlg)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_IDMSKEY, OnButtonIdmskey)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_LOADING_RULE1, &COracleSettingDlg::OnBnClickedCheckLoadingRule1)
	ON_BN_CLICKED(IDC_CHECK_LOADING_RULE2, &COracleSettingDlg::OnBnClickedCheckLoadingRule2)
	ON_BN_CLICKED(IDC_BUTTON_NEW_MAPPING, &COracleSettingDlg::OnBnClickedNewMapping)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_MAPPING, &COracleSettingDlg::OnBnClickedDeleteMapping)
	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDC_CHECK_PDS_DATA_UPDATE, &COracleSettingDlg::OnBnClickedUpdatePDSData)
	ON_BN_CLICKED(IDC_BUTTON_CONNECTION_TEST, &COracleSettingDlg::OnBnClickedButtonConnectionTest)
	ON_BN_CLICKED(IDC_BUTTON_DESIGN_CONNECTION_TEST, &COracleSettingDlg::OnBnClickedButtonDesignConnectionTest)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COracleSettingDlg message handlers

/**
	@brief	
	@author	humkyung
**/
void COracleSettingDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	CWnd* pWnd = GetDlgItem(IDC_EDIT_DESIGN_DDL_FILE_PATH);
	if(pWnd)
	{
		CRect rect;
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);

		pWnd->SetWindowPos(NULL , rect.left , rect.top , cx - rect.left - 10 , rect.Height() , SWP_NOMOVE);
	}

	pWnd = GetDlgItem(IDC_STATIC_IDMS_KEY);
	if(pWnd)
	{
		CRect rect;
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);

		pWnd->SetWindowPos(NULL , rect.left , rect.top , (cx - rect.left - 10) / 2 , rect.Height() , SWP_NOMOVE);
	}

	pWnd = GetDlgItem(IDC_COMBO_ORAKEY);
	if(pWnd)
	{
		CRect rect;
		CWnd* pWnd2 = GetDlgItem(IDC_STATIC_IDMS_KEY);
		pWnd2->GetWindowRect(&rect);
		ScreenToClient(&rect);

		pWnd->SetWindowPos(NULL , rect.left + 100, rect.top , rect.Width() - 150, rect.Height() , SWP_NOMOVE);
	}
	
	pWnd = GetDlgItem(IDC_EDIT_IDMSKEY);
	if(pWnd)
	{
		CRect rect;
		CWnd* pWnd2 = GetDlgItem(IDC_COMBO_ORAKEY);
		pWnd2->GetWindowRect(&rect);
		ScreenToClient(&rect);

		pWnd->SetWindowPos(NULL, rect.left ,rect.top + 20, rect.Width() ,rect.Height(),SWP_NOMOVE);
	}

	pWnd = GetDlgItem(IDC_STATIC_LOADING_RULE);
	if(pWnd)
	{
		CRect rect , rectThis;
		CWnd* pwn2 = GetDlgItem(IDC_STATIC_IDMS_KEY);
		pwn2->GetWindowRect(&rect);
		ScreenToClient(&rect);

		pWnd->GetWindowRect(&rectThis);
		ScreenToClient(&rectThis);

		pWnd->SetWindowPos(NULL, rect.left , rect.bottom + 20 , rect.Width() , rectThis.Height() , SWP_NOZORDER);
	}
	
	pWnd = GetDlgItem(IDC_GROUP_MAPPING_FOR_INSERT);
	if(pWnd)
	{
		CRect rect;
		CWnd* pWnd2 = GetDlgItem(IDC_STATIC_IDMS_KEY);
		pWnd2->GetWindowRect(&rect);
		ScreenToClient(&rect);

		pWnd->SetWindowPos(NULL, rect.right + 15 , rect.top , rect.Width() - 15 , cy - rect.top - 5 , SWP_NOZORDER);
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);

		CRect self;
		m_btnDeleteMapping.GetWindowRect(&self);
		m_btnDeleteMapping.SetWindowPos(NULL , rect.right - self.Width() - 5 , rect.top + 15 , 0 , 0 , SWP_NOZORDER | SWP_NOSIZE);
		m_btnDeleteMapping.GetWindowRect(&rect);
		ScreenToClient(&rect);

		m_btnNewMapping.GetWindowRect(&self);
		m_btnNewMapping.SetWindowPos(NULL , rect.left - self.Width() - 5 , rect.top , 0 , 0 , SWP_NOZORDER | SWP_NOSIZE);
		m_btnNewMapping.GetWindowRect(&self);
		ScreenToClient(&self);

		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);
		m_ctrlMappingForInsertDataToPDS.SetWindowPos(NULL, rect.left + 10 , self.bottom + 10, rect.Width() - 20 , rect.bottom - self.bottom - 20 , SWP_NOZORDER);
	}
}	

/**	
	@brief	The COracleSettingDlg::OnInitDialog function
	@author	humkyung
	@return	BOOL
*/
BOOL COracleSettingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	HWND hwndEdit  = GetDlgItem(IDC_EDIT_DESIGN_DDL_FILE_PATH)->GetSafeHwnd();
	
	hwndEdit  = GetDlgItem(IDC_EDIT_IDMSKEY)->GetSafeHwnd();
	HWND hwndButton= GetDlgItem(IDC_BUTTON_IDMSKEY)->GetSafeHwnd();
	VERIFY( ::EnableBuddyButton( hwndEdit, hwndButton, BBS_RIGHT ) );
	CAdminDocData* pDocData = CAdminDocData::GetInstance();

	static const TCHAR* __pColumnLabels[]=
	{
		_T("NO") , _T("Line Data Field") , _T("Oracle Field")
	};

	BOOL b = FALSE;
	if(TRUE == (b = m_ctrlMappingForInsertDataToPDS.Create(CRect(0 , 0 , 0 , 0) , this , 0x101)))
	{
		/// 그리드 컨트롤 폰트 코드
		CFont font;
		font.CreateFont(-12,0,0,0,400,FALSE,FALSE,FALSE,HANGUL_CHARSET,3,2,1,VARIABLE_PITCH | FF_MODERN , _T("Arial")); 
		m_ctrlMappingForInsertDataToPDS.SetFont(&font);
		font.DeleteObject();
		// fill it up with stuff
		m_ctrlMappingForInsertDataToPDS.SetEditable( TRUE );
		m_ctrlMappingForInsertDataToPDS.EnableDragAndDrop(FALSE);
		m_ctrlMappingForInsertDataToPDS.SetTextBkColor(RGB(255, 255, 224));
		m_ctrlMappingForInsertDataToPDS.SetRowResize(TRUE);
		m_ctrlMappingForInsertDataToPDS.SetColumnResize(FALSE);
		m_ctrlMappingForInsertDataToPDS.SetListMode(FALSE);
		m_ctrlMappingForInsertDataToPDS.SetSingleRowSelection(TRUE);
		m_ctrlMappingForInsertDataToPDS.SetHeaderSort(FALSE);
		m_ctrlMappingForInsertDataToPDS.SetSingleColSelection(TRUE);
		/////////////////////////////////////////////////////////////////
		const int nColCount = ARRAYSIZE(__pColumnLabels);
		m_ctrlMappingForInsertDataToPDS.SetColumnCount(nColCount);
		m_ctrlMappingForInsertDataToPDS.SetFixedRowCount(1);
		for(int i = 0;i < nColCount;++i)
		{
			m_ctrlMappingForInsertDataToPDS.SetItemFormat(0, i, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_ctrlMappingForInsertDataToPDS.SetItemText(0 , i, __pColumnLabels[i]);
		}
	}

	if(NULL == m_pOraDatabaseDefFile) m_pOraDatabaseDefFile = new COraDatabaseDefFile;

	{
		HICON hIcon = (HICON) LoadImage(AfxGetInstanceHandle(),
									MAKEINTRESOURCE(IDI_ADD),
									IMAGE_ICON, 16, 16, 0);
		HICON hOldIcon = m_btnNewMapping.SetIcon(hIcon);
		if(hOldIcon) DestroyIcon(hOldIcon);
	}
	{
		HICON hIcon = (HICON) LoadImage(AfxGetInstanceHandle(),
									MAKEINTRESOURCE(IDI_REMOVE),
									IMAGE_ICON, 16, 16, 0);
		HICON hOldIcon = m_btnDeleteMapping.SetIcon(hIcon);
		if(hOldIcon) DestroyIcon(hOldIcon);
	}

	return TRUE;  
}

struct OraMapFindDesc : public binary_function<MappingMap, CString, bool>
{
//          typedef T first_argument_type;
//          typedef T second_argument_type;
//          typedef T result_type;

	bool operator ()( const MappingMap& lhs , const CString& rhs ) const
	{
		return (lhs.rFieldName == rhs);
	}
};

/**	
	@brief	The COracleSettingDlg::UpdateContents function
	@author	HumKyung
	@date	????.??.??
	@param	
	@return	void	
*/
void COracleSettingDlg::UpdateContents()
{
	if(NULL != m_pAdminDoc)
	{
		CAdminDocData* pDocData = CAdminDocData::GetInstance();
		CString rIniFilePath = pDocData->GetIniFilePath();
	
		// set ddl file path
		SetDlgItemText(IDC_EDIT_DESIGN_DDL_FILE_PATH , pDocData->m_rDDLFilePath);

		if(m_pOraDatabaseDefFile)
		{	
			m_pOraDatabaseDefFile->Read(pDocData->m_rDDLFilePath.operator LPCTSTR());

			m_oraKeyComboBox.ResetContent();
			const size_t nCount = m_pOraDatabaseDefFile->GetFieldCount();
			for(int i = 0;i < int(nCount);++i)
			{
				COraDatabaseDefFile::CFieldDef def;
				if(m_pOraDatabaseDefFile->GetFieldAt(def , i))
				{
					m_oraKeyComboBox.InsertString(i , def.m_rFieldName.c_str());
				}
			}
		}

		TCHAR szBuf[MAX_PATH + 1] = {'0',};
		CString rApp(_T("Oracle Setting"));
		{
			if(GetPrivateProfileString(rApp , _T("OraKey") , NULL , szBuf , MAX_PATH , rIniFilePath))
			{	
				const int iFound = m_oraKeyComboBox.FindStringExact(-1 , szBuf);
				if(-1 != iFound)
				{
					m_oraKeyComboBox.SetCurSel(iFound);
				}
			}
			if(GetPrivateProfileString(rApp , _T("SmartISOKey") , NULL , szBuf , MAX_PATH , rIniFilePath))
			{	
				CString rTemp = szBuf;
				rTemp.Replace(":" , " ~ ");
				SetDlgItemText(IDC_EDIT_IDMSKEY,rTemp);
			}
			if(GetPrivateProfileString(rApp , _T("Host") , NULL , szBuf , MAX_PATH , rIniFilePath))
			{
				SetDlgItemText(IDC_EDIT_HOST , szBuf);
			}
			if(GetPrivateProfileString(rApp , _T("Service Name") , NULL , szBuf , MAX_PATH , rIniFilePath))
			{
				SetDlgItemText(IDC_EDIT_SERVICE_NAME , szBuf);
			}
			if(GetPrivateProfileString(rApp , _T("User Name") , NULL , szBuf , MAX_PATH , rIniFilePath))
			{
				SetDlgItemText(IDC_EDIT_USER_NAME , szBuf);
			}
			if(GetPrivateProfileString(rApp , _T("Password") , NULL , szBuf , MAX_PATH , rIniFilePath))
			{
				SetDlgItemText(IDC_EDIT_PASSWORD , szBuf);
			}
			if(GetPrivateProfileString(rApp , _T("Design User Name") , NULL , szBuf , MAX_PATH , rIniFilePath))
			{
				SetDlgItemText(IDC_EDIT_DESIGN_USER_NAME , szBuf);
			}
			if(GetPrivateProfileString(rApp , _T("Design Password") , NULL , szBuf , MAX_PATH , rIniFilePath))
			{
				SetDlgItemText(IDC_EDIT_DESIGN_PASSWORD , szBuf);
			}
		}
				
		m_MappingMapData.clear();
		m_MappingMapDataForInsert.clear();
		
		rApp = _T("Oracle Mapping");
		vector<STRING_T> oResult;
		const int nCount = GetPrivateProfileInt(rApp, _T("Count") , 0 ,rIniFilePath);
		
		MappingMap mapData;
		for(int i = 0;i < nCount;++i)
		{
			CString rKey;
			rKey.Format("Name%d",i);		
			if(GetPrivateProfileString(rApp, rKey, NULL, szBuf , MAX_PATH, rIniFilePath))
			{
				CTokenizer<CIsFromString>::Tokenize(oResult , szBuf , CIsFromString(":"));
				if(2 == oResult.size())
				{
					mapData.rFieldName = oResult[0].c_str();
					mapData.rFieldDesc = oResult[1].c_str();
					m_MappingMapData.push_back(mapData);
				}
			}

			rKey.Format("NAME_FOR_INSERT%d",i);		
			if(GetPrivateProfileString(rApp, rKey, NULL, szBuf , MAX_PATH, rIniFilePath))
			{
				CTokenizer<CIsFromString>::Tokenize(oResult , szBuf , CIsFromString(":"));
				if(2 == oResult.size())
				{
					mapData.rFieldName = oResult[0].c_str();
					mapData.rFieldDesc = oResult[1].c_str();
					m_MappingMapDataForInsert.push_back(mapData);
				}
			}
		}
		oResult.clear();

		int iIndex = 0;
		m_ctrlMappingForInsertDataToPDS.SetRowCount(m_MappingMapDataForInsert.size() + 1);
		m_ctrlMappingForInsertDataToPDS.SetColumnCount(3);
		m_ctrlMappingForInsertDataToPDS.SetFixedColumnCount(1);
		for(vector<MappingMap>::iterator itr = m_MappingMapDataForInsert.begin();itr != m_MappingMapDataForInsert.end();++itr,++iIndex)
		{
			CString rKey;
			rKey.Format(_T("NAME%d") , iIndex);
			
			{
				OSTRINGSTREAM_T oss;
				oss << (iIndex + 1);
				m_ctrlMappingForInsertDataToPDS.SetItemText(iIndex + 1  , 0 , oss.str().c_str());
			}
			{
				m_ctrlMappingForInsertDataToPDS.SetItemText(iIndex + 1  , 1 , itr->rFieldName);
				/// 2012.05.08 added by humkyung
				if((_T("INS_TYPE") == itr->rFieldName) || (_T("INS_THICKNESS") == itr->rFieldName))
				{
					CGridCellBase* pCell = m_ctrlMappingForInsertDataToPDS.GetCell(iIndex + 1  , 1);
					pCell->SetBackClr(RGB(217,217,217));
					pCell->SetState(GVIS_READONLY);
					pCell = m_ctrlMappingForInsertDataToPDS.GetCell(iIndex + 1  , 2);
					pCell->SetBackClr(RGB(217,217,217));
					pCell->SetState(GVIS_READONLY);
				}
				/// up to here
				else
				{
					m_ctrlMappingForInsertDataToPDS.SetCellType(iIndex + 1 , 1 , RUNTIME_CLASS(GridCell::CLineDataFieldGridCell));	/// 2012.09.14 added by humkyung
					m_ctrlMappingForInsertDataToPDS.SetCellType(iIndex + 1 , 2 , RUNTIME_CLASS(GridCell::COracleMappingGridCell));					
					m_ctrlMappingForInsertDataToPDS.SetItemText(iIndex + 1  , 2 , itr->rFieldDesc);
				}
			}
		}
		m_ctrlMappingForInsertDataToPDS.AutoSizeRows();
		m_ctrlMappingForInsertDataToPDS.AutoSizeColumn(0);
		m_ctrlMappingForInsertDataToPDS.SetColumnWidth(1 , 200);
		m_ctrlMappingForInsertDataToPDS.SetColumnWidth(2 , 200);

		CProjectData* pProjectData = CProjectData::GetInstance();
		if(NULL != pProjectData)
		{
			//! 2011.01.19 - added by HumKyung
			CString rValue = (*(pProjectData->m_pProjectSettingMap))[_T("update_pds")];
			CWnd* pParent = m_wndPDSDataUpdateGroupCheckButton.GetParent();
			m_wndPDSDataUpdateGroupCheckButton.SetCheck( (0 == rValue.CompareNoCase(_T("ON"))) );
			this->OnBnClickedUpdatePDSData();

			/// 2012.05.08 added by humkyung
			rValue = (*(pProjectData->m_pProjectSettingMap))[_T("check_loading_rule1")];
			CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_LOADING_RULE1);
			pButton->SetCheck(0 == rValue.CompareNoCase(_T("ON")));
			this->OnBnClickedCheckLoadingRule1();
			
			rValue = (*(pProjectData->m_pProjectSettingMap))[_T("edit_loading_rule1")];
			SetDlgItemText(IDC_EDIT_LOADING_RULE1 , rValue);
			
			rValue = (*(pProjectData->m_pProjectSettingMap))[_T("check_loading_rule2")];
			pButton = (CButton*)GetDlgItem(IDC_CHECK_LOADING_RULE2);
			pButton->SetCheck(0 == rValue.CompareNoCase(_T("ON")));
			/// up to here
		}
	}
}

/**	
	@brief	사용자가 설정한 값들을 ini 파일로 저장한다.
	@author	humkyung
	@date	????.??.??
	@return	void	
*/
void COracleSettingDlg::Save()
{
	CAdminDocData* pAdminDocData = CAdminDocData::GetInstance();
	CString rIniFilePath = pAdminDocData->GetIniFilePath();
	CProject* pProject = m_pAdminDoc->GetCurProject();

	CString rIdmsKey,rOraKey,rResult;
	
	CEdit *edit = (CEdit*)GetDlgItem(IDC_EDIT_IDMSKEY);
	edit->GetWindowText(rIdmsKey);
		
	CComboBox * pCombo = (CComboBox*)GetDlgItem(IDC_COMBO_ORAKEY);
	pCombo->GetWindowText(rOraKey);

	if(!rIdmsKey.IsEmpty())
	{
		rIdmsKey.Replace( _T(" ~ ") , _T(":"));
	}
	
	CString rDDLFilePath;
	GetDlgItemText( IDC_EDIT_DESIGN_DDL_FILE_PATH , rDDLFilePath );
	
	///
	CString rApp("Oracle Setting");
	{
		WritePrivateProfileString( rApp , _T("DDL FILE PATH") , rDDLFilePath , rIniFilePath );
		WritePrivateProfileString( rApp , _T("SmartISOKey") , rIdmsKey, rIniFilePath);
		WritePrivateProfileString( rApp , _T("OraKey") , rOraKey, rIniFilePath);

		/// write oracle setting - 2012.09.09 added by humkyung
		CString rString;
		GetDlgItemText(IDC_EDIT_HOST , rString);
		WritePrivateProfileString(rApp , _T("Host") , rString, rIniFilePath);
		GetDlgItemText(IDC_EDIT_SERVICE_NAME , rString);
		WritePrivateProfileString(rApp , _T("Service Name") , rString , rIniFilePath);
		GetDlgItemText(IDC_EDIT_USER_NAME , rString);
		WritePrivateProfileString(rApp , _T("User Name") , rString , rIniFilePath);
		GetDlgItemText(IDC_EDIT_PASSWORD , rString);
		WritePrivateProfileString(rApp , _T("Password") , rString , rIniFilePath);
		GetDlgItemText(IDC_EDIT_DESIGN_USER_NAME , rString);
		WritePrivateProfileString(rApp , _T("Design User Name") , rString , rIniFilePath);
		GetDlgItemText(IDC_EDIT_DESIGN_PASSWORD , rString);
		WritePrivateProfileString(rApp , _T("Design Password") , rString , rIniFilePath);
		/// up to here
	}

	rApp = _T("Oracle Mapping");
	int nRow = 0 , i = 0;

	//////
	nRow = m_ctrlMappingForInsertDataToPDS.GetRowCount();
	for(i = 0; i < nRow; ++i)
	{
		CString rLineData = m_ctrlMappingForInsertDataToPDS.GetItemText(i + 1 , 1);
		CString rOracle   = m_ctrlMappingForInsertDataToPDS.GetItemText(i + 1, 2 );
		
		CString rMapping = rLineData+ _T(":") +rOracle;
		CString rTemp;
		rTemp.Format( _T("NAME_FOR_INSERT%d") ,i);
		WritePrivateProfileString(rApp,rTemp, rMapping,rIniFilePath);
	}

	CProjectData* pProjectData = CProjectData::GetInstance();
	if(NULL != pProjectData)
	{
		map<CString,CString>::iterator where = pProjectData->m_pProjectSettingMap->find(_T("update_pds"));
		if(where != pProjectData->m_pProjectSettingMap->end())
		{
			//! 2011.01.19 - added by HumKyung
			CString rString = _T("OFF");
			CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_PDS_DATA_UPDATE);
			rString = (pButton->GetCheck()) ? _T("ON") : _T("OFF");
			/// up to here
			where->second = rString;
		}

		/// 2012.05.08 added by humkyung
		where = pProjectData->m_pProjectSettingMap->find(_T("check_loading_rule1"));
		if(where != pProjectData->m_pProjectSettingMap->end())
		{
			CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_LOADING_RULE1);
			CString rString = (TRUE == pButton->GetCheck()) ? _T("ON") : _T("OFF");
			where->second = rString;
		}
		where = pProjectData->m_pProjectSettingMap->find(_T("edit_loading_rule1"));
		if(where != pProjectData->m_pProjectSettingMap->end())
		{
			CString rString;
			(CButton*)GetDlgItemText(IDC_EDIT_LOADING_RULE1 , rString);
			where->second = rString;
		}
		where = pProjectData->m_pProjectSettingMap->find(_T("check_loading_rule2"));
		if(where != pProjectData->m_pProjectSettingMap->end())
		{
			CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_LOADING_RULE2);
			CString rString = (TRUE == pButton->GetCheck()) ? _T("ON") : _T("OFF");
			where->second = rString;
		}
		/// up to here
	}
}

/**
	@brief	

	@author	humkyung

	@date	????.??.??
**/
void COracleSettingDlg::OnButtonIdmskey() 
{
	CAdminApp* pApp = (CAdminApp*)AfxGetApp();
	CResourceSwitch resSwitch(pApp->m_hMultiLangRes);

	UpdateData( TRUE );
	CFileNameFormatDlg dlg;
	dlg.m_rFileNameFormat = m_rIDMSKeyFormat;
	dlg.m_pAdminDoc = m_pAdminDoc;
	if(IDOK == dlg.DoModal())
	{
		m_rIDMSKeyFormat = dlg.m_rFileNameFormat;
		CString rFileNameFormat(m_rIDMSKeyFormat);
		rFileNameFormat.Replace( _T(":") , _T(" ~ "));
		m_IDMSKeyFormatEdit.SetWindowText(rFileNameFormat);
	}
}

/**
	@brief	

	@author	humkyung

	@date	????.??.??
**/
BOOL COracleSettingDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( pMsg->message == WM_KEYDOWN ) // Enter Key Prevent.. so good.. ^^		
	{
		if(pMsg->wParam == VK_RETURN)		
		{
			CWnd* pWnd = GetDlgItem(IDC_EDIT_DESIGN_DDL_FILE_PATH);
			if(pMsg->hwnd == pWnd->GetSafeHwnd())
			{
				CString rDDLFilePath;
				
				GetDlgItemText(IDC_EDIT_DESIGN_DDL_FILE_PATH , rDDLFilePath);

				CProject* pProject = m_pAdminDoc->GetCurProject();
				CString rNewDDLFilePath = pProject->path() + CString( _T("\\Setting\\design.ddl") );
				if(0 == CopyFile(rDDLFilePath , rNewDDLFilePath , FALSE)) //! Setting 폴더에 있는 design.ddl 파일을 무시한다.
				{
					DWORD dwErrorNo = GetLastError();
					if(32 != dwErrorNo) PrintError(dwErrorNo);	//! 파일을 같은 이름으로 복사할려 할때 ERROR CODE = 32
				}
				if(m_pOraDatabaseDefFile)
				{	
					m_pOraDatabaseDefFile->Read(rDDLFilePath.operator LPCTSTR());
					
					m_oraKeyComboBox.ResetContent();
					const size_t nCount = m_pOraDatabaseDefFile->GetFieldCount();
					for(int i = 0;i < int(nCount);++i)
					{
						COraDatabaseDefFile::CFieldDef def;
						if(m_pOraDatabaseDefFile->GetFieldAt(def , i))
						{
							m_oraKeyComboBox.InsertString(i , def.m_rFieldName.c_str());
						}
					}
				}
			}
			::TranslateMessage(pMsg);
			
			::DispatchMessage(pMsg);
			
			return TRUE;                    // DO NOT process further	
		}
		else if(pMsg->wParam == VK_ESCAPE)
		{
			::TranslateMessage(pMsg);
			
			::DispatchMessage(pMsg);
			
			return TRUE; 
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

/******************************************************************************
    @author     humkyung
    @date       2012-05-08
    @class      COracleSettingDlg
    @function   OnBnClickedCheckLoadingRule1
    @return     void
    @brief
******************************************************************************/
void COracleSettingDlg::OnBnClickedCheckLoadingRule1()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_LOADING_RULE1);
	GetDlgItem(IDC_EDIT_LOADING_RULE1)->EnableWindow(pButton->GetCheck());
}

/******************************************************************************
    @author     humkyung
    @date       2012-05-08
    @class      COracleSettingDlg
    @function   OnBnClickedCheckLoadingRule2
    @return     void
    @brief
******************************************************************************/
void COracleSettingDlg::OnBnClickedCheckLoadingRule2()
{
}

/******************************************************************************
    @author     humkyung
    @date       2012-09-05
    @function   OnBnClickedNewMapping
    @return     void
    @brief
******************************************************************************/
void COracleSettingDlg::OnBnClickedNewMapping()
{
	m_ctrlMappingForInsertDataToPDS.SetRowCount(m_ctrlMappingForInsertDataToPDS.GetRowCount() + 1);
	{
		OSTRINGSTREAM_T oss;
		const int iRowCount = m_ctrlMappingForInsertDataToPDS.GetRowCount();
		oss << iRowCount - 1;
		m_ctrlMappingForInsertDataToPDS.SetItemText(iRowCount - 1 , 0 , oss.str().c_str());
		m_ctrlMappingForInsertDataToPDS.SetCellType(iRowCount - 1 , 1 , RUNTIME_CLASS(GridCell::CLineDataFieldGridCell));	/// 2012.09.20 added by humkyung
		m_ctrlMappingForInsertDataToPDS.SetCellType(iRowCount - 1 , 2 , RUNTIME_CLASS(GridCell::COracleMappingGridCell));

		m_ctrlMappingForInsertDataToPDS.AutoSizeRows();
		m_ctrlMappingForInsertDataToPDS.AutoSizeColumn(0);
		m_ctrlMappingForInsertDataToPDS.SetColumnWidth(1 , 200);
	}
}

/******************************************************************************
    @author     humkyung
    @date       2012-09-05
    @function   OnBnClickedDeleteMapping
    @return     void
    @brief
******************************************************************************/
void COracleSettingDlg::OnBnClickedDeleteMapping()
{
	CCellRange range = m_ctrlMappingForInsertDataToPDS.GetSelectedCellRange();
	const int row = range.GetMaxRow();
	if((row >= 1) && (row <= m_ctrlMappingForInsertDataToPDS.GetRowCount()-1))
	{
		m_ctrlMappingForInsertDataToPDS.DeleteRow(row);
		m_ctrlMappingForInsertDataToPDS.InvalidateRect(NULL , TRUE);

		OSTRINGSTREAM_T oss;
		const int iRowCount = m_ctrlMappingForInsertDataToPDS.GetRowCount();
		for(int i = 0;i < iRowCount;++i)
		{
			oss << i + 1;
			m_ctrlMappingForInsertDataToPDS.SetItemText(i + 1 , 0 , oss.str().c_str());
			oss.str(_T(""));
		}

		m_ctrlMappingForInsertDataToPDS.SetSelectedRange((row <= iRowCount - 1) ? row : row - 1, 1 , (row <= iRowCount - 1) ? row : row - 1, 2);
		m_ctrlMappingForInsertDataToPDS.SetFocusCell((row <= iRowCount - 1) ? row : row - 1, 1);
	}
}

/******************************************************************************
    @author     humkyung
    @date       2012-06-05
    @class      COracleSettingDlg
    @function   OnHelpInfo
    @return     BOOL
    @brief
******************************************************************************/
BOOL COracleSettingDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	HWND hHtml = ::HtmlHelp(this->m_hWnd , CAdminDocData::GetExecPath() + _T("\\Help\\SmartISO.chm::/Oracle Loading Setting.htm") , HH_DISPLAY_TOPIC , 0);

	return TRUE;///CDialog::OnHelpInfo(pHelpInfo);
}

/******************************************************************************
    @author     humkyung
    @date       2012-09-09
    @class      COracleSettingDlg
    @function   OnBnClickedUpdatePDSData
    @return     void
    @brief
******************************************************************************/
void COracleSettingDlg::OnBnClickedUpdatePDSData()
{
	const BOOL bCheck = this->m_wndPDSDataUpdateGroupCheckButton.GetCheck();
	GetDlgItem(IDC_EDIT_HOST)->EnableWindow(TRUE == bCheck);
	GetDlgItem(IDC_EDIT_SERVICE_NAME)->EnableWindow(TRUE == bCheck);
	GetDlgItem(IDC_EDIT_USER_NAME)->EnableWindow(TRUE == bCheck);
	GetDlgItem(IDC_EDIT_PASSWORD)->EnableWindow(TRUE == bCheck);
	GetDlgItem(IDC_BUTTON_CONNECTION_TEST)->EnableWindow(TRUE == bCheck);
	GetDlgItem(IDC_EDIT_DESIGN_USER_NAME)->EnableWindow(TRUE == bCheck);
	GetDlgItem(IDC_EDIT_DESIGN_PASSWORD)->EnableWindow(TRUE == bCheck);
	GetDlgItem(IDC_BUTTON_DESIGN_CONNECTION_TEST)->EnableWindow(TRUE == bCheck);
}

/******************************************************************************
    @author     humkyung
    @date       2012-09-09
    @class      COracleSettingDlg
    @function   OnBnClickedButtonConnectionTest
    @return     void
    @brief
******************************************************************************/
void COracleSettingDlg::OnBnClickedButtonConnectionTest()
{
	CString Host , UserName, Password,ServiceName;
	GetDlgItemText(IDC_EDIT_HOST , Host);
	GetDlgItemText(IDC_EDIT_SERVICE_NAME , ServiceName);
	GetDlgItemText(IDC_EDIT_USER_NAME , UserName);
	GetDlgItemText(IDC_EDIT_PASSWORD , Password);

	try
	{
		CoInitialize(NULL);
		ISmartISOLayerPtr ptr(__uuidof(SmartISOLayer));
		VARIANT_BOOL varRet;
		BSTR bstrHost = Host.AllocSysString();
		BSTR bstrService = ServiceName.AllocSysString();
		BSTR bstrUserName= UserName.AllocSysString();
		BSTR bstrPassword= Password.AllocSysString();
        ptr->ConnectOracle(bstrHost,bstrService,bstrUserName,bstrPassword,&varRet);
		if(VARIANT_TRUE == varRet)
		{
			AfxMessageBox(_T("Connection is OK"),MB_OK|MB_ICONINFORMATION);
		}
		ptr->DisconnectOracle(&varRet);
		::SysFreeString(bstrHost);
		::SysFreeString(bstrService);
		::SysFreeString(bstrUserName);
		::SysFreeString(bstrPassword);
        ptr->Release();
        CoUninitialize();
	}
	catch(const std::exception& ex)
	{
		AfxMessageBox(ex.what(),MB_OK|MB_ICONERROR);
	}
}

/******************************************************************************
    @author     humkyung
    @date       2012-09-09
    @class      COracleSettingDlg
    @function   OnBnClickedButtonConnectionTest
    @return     void
    @brief
******************************************************************************/
void COracleSettingDlg::OnBnClickedButtonDesignConnectionTest()
{
	CString Host , UserName, Password,ServiceName;
	GetDlgItemText(IDC_EDIT_HOST , Host);
	GetDlgItemText(IDC_EDIT_SERVICE_NAME , ServiceName);
	GetDlgItemText(IDC_EDIT_DESIGN_USER_NAME , UserName);
	GetDlgItemText(IDC_EDIT_DESIGN_PASSWORD , Password);

	try
	{
		CoInitialize(NULL);
		ISmartISOLayerPtr ptr(__uuidof(SmartISOLayer));
		VARIANT_BOOL varRet;
		BSTR bstrHost = Host.AllocSysString();
		BSTR bstrService = ServiceName.AllocSysString();
		BSTR bstrUserName= UserName.AllocSysString();
		BSTR bstrPassword= Password.AllocSysString();
        ptr->ConnectOracle(bstrHost,bstrService,bstrUserName,bstrPassword,&varRet);
		if(VARIANT_TRUE == varRet)
		{
			AfxMessageBox(_T("Connection is OK"),MB_OK|MB_ICONINFORMATION);
		}
		ptr->DisconnectOracle(&varRet);
		::SysFreeString(bstrHost);
		::SysFreeString(bstrService);
		::SysFreeString(bstrUserName);
		::SysFreeString(bstrPassword);
        ptr->Release();
        CoUninitialize();
	}
	catch(const std::exception& ex)
	{
		AfxMessageBox(ex.what(),MB_OK|MB_ICONERROR);
	}
}
