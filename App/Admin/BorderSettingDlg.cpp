// BorderSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include <iostream>
#include <ado/SQLFile.h>
#include <ProjectData.h>
#include "Admin.h"
#include "MainFrm.h"
#include "AdminView.h"
#include "AdminDocData.h"
#include "tokenizer.h"
#include "BorderSettingDlg.h"
#include "enablebuddybutton.h"
#include <winuser.h>

#define QC_CHECK_AREA_LABEL			_T("Q/C Check Area...")
#define	LINK_KEY_DATA_AREA_LABEL	_T("Link Key Data Area")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CBorderSettingDlg* __instance__ = NULL;
/**
	@brief	return instance of CBorderSettingDlg
	@author	humkyung
	@date	2015.11.26
*/
CBorderSettingDlg* CBorderSettingDlg::GetInstance()
{
	return __instance__;
}

/////////////////////////////////////////////////////////////////////////////
// CBorderSettingDlg dialog
CBorderSettingDlg::CBorderSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBorderSettingDlg::IDD, pParent) , m_pAdminDoc(NULL) , m_DgnDoc(CDgnDocument::CreateInstance())
{
	//{{AFX_DATA_INIT(CBorderSettingDlg)
	m_ctrlPropTree = NULL;
	m_pGeneralDataRoot = NULL;
	m_pRevisionDataRoot = NULL;
	m_pTextWidthItem = NULL;
	m_pTextHeightItem= NULL;
	m_pTextWeightItem= NULL;	/// 2011.09.21 added by humkyung
	m_pIsoDwgAreaRoot = NULL;
	m_pLinkKeyDataAreaRoot = NULL;	/// 2012.08.26 added by humkyung
	m_pQCCheckAreaRoot= NULL;
	m_pIsoDwgRemoveAreaRoot = NULL;
	__instance__ = this;
	//}}AFX_DATA_INIT
}

/**	
	@brief	desctuctor
	@author humkyung
	@date	2011.07.08
	@return	
*/
CBorderSettingDlg::~CBorderSettingDlg()
{
	try
	{
		for(map<STRING_T,TreeItemValue*>::const_iterator itr = m_oTreeItemValueMap.begin();itr != m_oTreeItemValueMap.end();++itr)
		{
			free((void*)(itr->second));
		}
	}
	catch(...)
	{
	}
}

void CBorderSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBorderSettingDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX , IDC_BUTTON_BORDER_FILE , m_wndBorderFileButton);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBorderSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CBorderSettingDlg)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_BORDER_FILE, OnButtonBorderFile)
	//}}AFX_MSG_MAP
	ON_WM_HELPINFO()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBorderSettingDlg message handlers

/**	
	@brief	CBorderSettingDlg 초기화 부분.
	@author HumKyung.BAEK
	@return	BOOL	
*/
BOOL CBorderSettingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(m_pAdminDoc)
	{
		m_wndBorderFileButton.SetIcon(IDI_REFRESH2 , 18 , 18);

		HWND hwndEdit  = GetDlgItem(IDC_EDIT_BORDER_FILE_PATH)->GetSafeHwnd();
		HWND hwndButton= GetDlgItem(IDC_BUTTON_BORDER_FILE)->GetSafeHwnd();
		VERIFY( ::EnableBuddyButton( hwndEdit, hwndButton, BBS_RIGHT ) );

		CRect rc;

		/// PTS_NOTIFY - CPropTree will send notification messages to the parent window
		const DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_EX_STATICEDGE | PTS_NOTIFY;

		/// Init the control's size to cover the entire client area
		GetClientRect(rc);

		/// Create CPropTree control
		rc.left  = 10;
		rc.top   = 50;
		rc.right = rc.left + 250;

		m_ctrlPropTree = new CPropTree();
		if(FALSE == m_ctrlPropTree->Create(dwStyle, rc, this, 0x100))
		{
			AfxMessageBox( _T("Can't create proptree window") );
			return FALSE;
		}
		
		static CFont font;
		font.CreateFont(18,0,0,0 ,FW_BOLD,FALSE,FALSE,0,0,0,0,0,0, _T("Arial"));
		m_ctrlPropTree->SetFont(&font);
		GetDlgItem(IDC_EDIT_IGNORE_CHAR)->SetFont(&font);

		if(!m_GLCanvas.m_hWnd) 
		{
			CRect rect(285 , 50 , rc.Width() - 330 , rc.Height() - 58);
			
			m_GLCanvas.Create(TRUE, &rect, this);
			m_GLCanvas.SetScene(&m_GLScene);
						
			CIsVolume vol = m_GLScene.GetVolume();
			m_GLCanvas.m_dTrans[2] = (vol.maxx() - vol.minx())/200.f;
			m_GLCanvas.m_dTrans[0] = -(m_GLScene.m_ptCenter.x());
			m_GLCanvas.m_dTrans[1] = -(m_GLScene.m_ptCenter.y());
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBorderSettingDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CWnd* pWnd = GetDlgItem(IDC_EDIT_BORDER_FILE_PATH);
	if(NULL != pWnd)
	{
		CRect rect;
		pWnd->GetWindowRect(&rect);

		pWnd->SetWindowPos(NULL , 0 , 0 , cx - 94 , rect.Height() , SWP_NOMOVE);
	}
	
	CRect rect;
	pWnd = GetDlgItem(IDC_STATIC_EX);
	if(NULL != pWnd)
	{
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);
	}

	if(m_ctrlPropTree && m_ctrlPropTree->GetSafeHwnd())
	{
		m_ctrlPropTree->SetWindowPos(NULL , 0 , rect.bottom + 10 , 270 , cy - rect.bottom - 10 , SWP_NOZORDER);
	}

	if(m_GLCanvas.GetSafeHwnd())
	{
		m_GLCanvas.SetWindowPos(NULL , 270 , rect.bottom + 10 , cx - 270 , cy - rect.bottom - 10 , SWP_NOZORDER);
	}
}

/**	
	@brief	프로젝트 로딩후 초기화 루틴.
	@author	HumKyung.BAEK
	@return	void	
*/
void CBorderSettingDlg::UpdateContents()
{
	static bool bInitialized = false;

	if(true == bInitialized) return;
	
	CProject* pProject = m_pAdminDoc->GetCurProject();
	if(NULL != pProject && (NULL != m_DgnDoc.get()))
	{
		CString rIniFilePath = pProject->path() + CString("\\Setting\\") + pProject->name() + CString(".ini");
		TCHAR szBuf[MAX_PATH] = {0, };
		
		if(GetPrivateProfileString(_T("Border Setting") , _T("Border File Path") , NULL , szBuf , MAX_PATH , rIniFilePath))
		{
			SetDlgItemText(IDC_EDIT_BORDER_FILE_PATH , szBuf);
			
			try
			{
				if( 0 == m_DgnDoc->GetEntityCount() )
				{
					m_DgnDoc->Read(szBuf);
					m_GLScene.Set(m_DgnDoc.get());
				}
			}
			catch(...)
			{
				AfxMessageBox( _T("Unhandled Error") , MB_OK|MB_ICONERROR);
			}
		}
		else
		{
			SetDlgItemText(IDC_EDIT_BORDER_FILE_PATH , _T("Select a border file."));
		}
		
		/// display ignore char - 2013.08.22 added by humkyung
		CProjectData* pProjectData = CProjectData::GetInstance();
		map<CString,CString>::const_iterator where = pProjectData->m_pProjectSettingMap->find(_T("ignore_char"));
		if(where != pProjectData->m_pProjectSettingMap->end())
		{
			SetDlgItemText(IDC_EDIT_IGNORE_CHAR , where->second);
		}
		/// up to here

		const CString name = pProject->name();
		const CProject::DatabaseProp prop = pProject->GetDatabaseProp();
		CAppPostgreSQL database(name , prop.Host.c_str() , prop.User.c_str() , prop.Password.c_str());
		try
		{
			m_ctrlPropTree->DeleteAllItems();
			//////////////////////////////////////////////////////////////////////////
			m_pGeneralDataRoot = m_ctrlPropTree->InsertItem(new CPropTreeItem());
			m_pGeneralDataRoot->SetLabelText(_T("General"));

			CPropTreeItemCombo* pPropTreeItemCombo = (CPropTreeItemCombo*)m_ctrlPropTree->InsertItem(new CPropTreeItemCombo(), m_pGeneralDataRoot);
			pPropTreeItemCombo->SetLabelText(_T("Font No"));

			const DWORD dwStyle = WS_BORDER | WS_CHILD | WS_VSCROLL | CBS_UPPERCASE | CBS_SORT | CBS_AUTOHSCROLL | CBS_DROPDOWNLIST;
			
			pPropTreeItemCombo->CreateComboBox(dwStyle);
			int nItem = pPropTreeItemCombo->AddString("1");
			nItem = pPropTreeItemCombo->SetItemData(nItem , LPARAM(1));
			nItem = pPropTreeItemCombo->AddString("2");
			pPropTreeItemCombo->SetItemData(nItem , LPARAM(2));
			nItem = pPropTreeItemCombo->AddString("3");
			pPropTreeItemCombo->SetItemData(nItem , LPARAM(3));
			nItem = pPropTreeItemCombo->AddString("4");
			pPropTreeItemCombo->SetItemData(nItem , LPARAM(4));
			nItem = pPropTreeItemCombo->AddString("5");
			pPropTreeItemCombo->SetItemData(nItem , LPARAM(5));
			
			pPropTreeItemCombo->SetCurSel(0);
			{
				OSTRINGSTREAM_T oss;
				oss << "select value from t_iso_drawing_setup where name='Font No'";
				soci::rowset<soci::row> rs(database.session()->prepare << oss.str().c_str());
				for(soci::rowset<soci::row>::const_iterator itr = rs.begin();itr != rs.end();++itr)
				{
					const STRING_T sValue = itr->get<STRING_T>(_T("value"));
					pPropTreeItemCombo->SetItemValue(LPARAM(ATOI_T(sValue.c_str())));
					m_GLCanvas.m_sTextStyle = sValue;
				}
			}

			CPropTreeItemEdit* pPropTreeItemEdit = static_cast<CPropTreeItemEdit*>(m_ctrlPropTree->InsertItem(new CPropTreeItemEdit(), m_pGeneralDataRoot));
			pPropTreeItemEdit->SetLabelText(_T("Text Color"));
			CString sRegEx (_T("0|[1-9][0-9]*"));
			pPropTreeItemEdit->SetRegEx(sRegEx);
			pPropTreeItemEdit->SetItemValue(LPARAM("10"));
			{
				OSTRINGSTREAM_T oss;
				oss << "select value from t_iso_drawing_setup where name='Text Color'";
				soci::rowset<soci::row> rs(database.session()->prepare << oss.str().c_str());
				for(soci::rowset<soci::row>::const_iterator itr = rs.begin();itr != rs.end();++itr)
				{
					const STRING_T sValue = itr->get<STRING_T>(_T("value"));
					pPropTreeItemCombo->SetItemValue(LPARAM(sValue.c_str()));
				}
			}

			m_pTextWidthItem = static_cast<CPropTreeItemEdit*>(m_ctrlPropTree->InsertItem(new CPropTreeItemEdit(), m_pGeneralDataRoot));
			m_pTextWidthItem->SetLabelText(_T("Text Width"));
			m_pTextWidthItem->SetRegEx(sRegEx);
			m_pTextWidthItem->SetItemValue(LPARAM("30000"));
			{
				OSTRINGSTREAM_T oss;
				oss << "select value from t_iso_drawing_setup where name='Text Width'";
				soci::rowset<soci::row> rs(database.session()->prepare << oss.str().c_str());
				for(soci::rowset<soci::row>::const_iterator itr = rs.begin();itr != rs.end();++itr)
				{
					const STRING_T sValue = itr->get<STRING_T>(_T("value"));
					m_pTextWidthItem->SetItemValue(LPARAM(sValue.c_str()));
					m_GLCanvas.m_dTextWidth = ATOF_T(sValue.c_str());
				}
			}
			
			m_pTextHeightItem = static_cast<CPropTreeItemEdit*>(m_ctrlPropTree->InsertItem(new CPropTreeItemEdit(), m_pGeneralDataRoot));
			m_pTextHeightItem->SetLabelText(_T("Text Height"));
			m_pTextHeightItem->SetRegEx(sRegEx);
			m_pTextHeightItem->SetItemValue(LPARAM("1000"));
			{
				OSTRINGSTREAM_T oss;
				oss << "select value from t_iso_drawing_setup where name='Text Height'";
				soci::rowset<soci::row> rs(database.session()->prepare << oss.str().c_str());
				for(soci::rowset<soci::row>::const_iterator itr = rs.begin();itr != rs.end();++itr)
				{
					const STRING_T sValue = itr->get<STRING_T>(_T("value"));
					m_pTextHeightItem->SetItemValue(LPARAM(sValue.c_str()));
					m_GLCanvas.m_dTextHeight = ATOF_T(sValue.c_str());
				}
			}

			/// create a item for text weight - 2011.09.21 added by humkyung
			m_pTextWeightItem = static_cast<CPropTreeItemEdit*>(m_ctrlPropTree->InsertItem(new CPropTreeItemEdit(), m_pGeneralDataRoot));
			m_pTextWeightItem->SetLabelText(_T("Text Weight"));
			m_pTextWeightItem->SetRegEx(sRegEx);
			m_pTextWeightItem->SetItemValue(LPARAM("1"));
			{
				OSTRINGSTREAM_T oss;
				oss << "select value from t_iso_drawing_setup where name='Text Weight'";
				soci::rowset<soci::row> rs(database.session()->prepare << oss.str().c_str());
				for(soci::rowset<soci::row>::const_iterator itr = rs.begin();itr != rs.end();++itr)
				{
					const STRING_T sValue = itr->get<STRING_T>(_T("value"));
					m_pTextWeightItem->SetItemValue(LPARAM(sValue.c_str()));
				}
			}
			/// up to here

			m_pGeneralDataRoot->Expand(); // have this item expanded by default
			//////////////////////////////////////////////////////////////////////////

			/// Create another root item
			InitLineDataTree(database);
			InitLatestRevDataTree(database);
			InitRevDataTree(database);
			InitAreaDataTree(database);
			InitCheckAreaDataTree(database);
			InitRemoveAreaTree(database);
		}
		catch(const std::exception& ex)
		{
			AfxMessageBox(ex.what(),MB_OK|MB_ICONERROR);
		}
	}

	bInitialized = true;
}

/**	
	@brief	border 파일 열기.
	@author	HumKyung
	@date	????.??.??
	@param
	@return	voi
*/
void CBorderSettingDlg::OnButtonBorderFile() 
{
	CString rBorderFilePath;
	GetDlgItemText(IDC_EDIT_BORDER_FILE_PATH , rBorderFilePath);

	/// 2011.01.26 - added by HumKyung
	const int iPos = rBorderFilePath.ReverseFind('\\');
	if(iPos > 0)
	{
		const CString sBorderFileName = rBorderFilePath.Right(rBorderFilePath.GetLength() - iPos - 1);

		CopyFile(rBorderFilePath , m_pAdminDoc->m_ProjectPath + _T("\\Temp\\") + sBorderFileName , FALSE);
		CopyFile(rBorderFilePath , m_pAdminDoc->m_ProjectPath + _T("\\PRODUCT\\ISO\\") + sBorderFileName , FALSE);
	}
	if(NULL != m_DgnDoc.get())
	{
		m_DgnDoc->Read(rBorderFilePath);
		m_GLScene.Set(m_DgnDoc.get());
		m_GLCanvas.Invalidate();
	}
}

/**	
	@brief	The CBorderSettingDlg::OnNotify function
	@author	HumKyung
	@date	????.??.??
	@param	wParam	a parameter of type WPARAM
	@param	lParam	a parameter of type LPARAM
	@param	pResult	a parameter of type LRESULT*
	@return	BOOL	
*/
BOOL CBorderSettingDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	PNMPROPTREE pNMPropTree=(PNMPROPTREE)lParam;
	if(PTN_ITEMBUTTONCLICK == pNMPropTree->hdr.code)
	{
		if(!m_GLScene.IsEmpty())
		{
			CPropTreeItem* pParent = pNMPropTree->pItem->GetParent();
			if(pParent && 
				(CString(_T("Iso Dwg Data Area...")) == pParent->GetLabelText()) || pParent && (CString(_T("Remove Area...")) == pParent->GetLabelText()))
			{
				float x1=0.0 , y1=0.0 , x2=0.0 , y2=0.0;
				(CString(_T("Remove Area...")) == pParent->GetLabelText()) ? m_GLCanvas.SetGLObjectColor(255,255,0) : m_GLCanvas.SetGLObjectColor(255,0,0);
				m_GLCanvas.Pick(x1 , y1 , x2 , y2 , pNMPropTree->pItem->GetLabelText());

				CString rMsg;
				rMsg.Format(_T("%.0lf,%.0lf,%.0lf,%.0lf") , x1 , y1 , x2 , y2);
				(pNMPropTree->pItem)->SetItemValue((LPARAM)(rMsg.operator LPCTSTR()));
				m_ctrlPropTree->RedrawWindow();
			}
			else if(pParent && ((CString(QC_CHECK_AREA_LABEL) == pParent->GetLabelText()) || (CString(LINK_KEY_DATA_AREA_LABEL) == pParent->GetLabelText())))
			{
				float x1=0.0 , y1=0.0 , x2=0.0 , y2=0.0;
				m_GLCanvas.Pick(x1 , y1 , x2 , y2 , pNMPropTree->pItem->GetLabelText());

				CString rMsg;
				rMsg.Format( _T("%.0lf,%.0lf,%.0lf,%.0lf") , x1 , y1 , x2 , y2);
				(pNMPropTree->pItem)->SetItemValue((LPARAM)(rMsg.operator LPCTSTR()));
				m_ctrlPropTree->RedrawWindow();
			}
			else if(pParent && (CString("Display Distance") == pNMPropTree->pItem->GetLabelText()))
			{
				float distance = 0.;
				m_GLCanvas.Pick(distance , pNMPropTree->pItem->GetLabelText());

				CString rMsg;
				rMsg.Format("%0.lf" , distance);
				(pNMPropTree->pItem)->SetItemValue((LPARAM)(rMsg.operator LPCTSTR()));
				m_ctrlPropTree->RedrawWindow();
			}
			else
			{
				double width = atof((TCHAR*)(m_pTextWidthItem->GetItemValue()));
				double height= atof((TCHAR*)(m_pTextHeightItem->GetItemValue()));

				double dScaleFactor = 1.0;
				CString sDisplayString;
				if((_T("Line Data") == CString(pParent->GetLabelText())) || (_T("Latest Rev Data") == CString(pParent->GetLabelText())))
				{
					/// 2011.07.08/2011.09.22 - added by humkyung
					vector<STRING_T> oResult;
					CTokenizer<CIsComma>::Tokenize(oResult , LPCTSTR(pNMPropTree->pItem->GetItemValue()) , CIsComma());
					if(3 == oResult.size())
					{
						dScaleFactor = ATOF_T(oResult[2].c_str());
					}
					///dScaleFactor = ATOF_T((LPCTSTR)(pParent->GetChild()->GetItemValue()));
					///
					sDisplayString = pNMPropTree->pItem->GetLabelText();
				}
				else
				{
					/// 2011.07.11 - added by humkyung
					sDisplayString = pNMPropTree->pItem->GetLabelText();
					map<string , TreeItemValue*>::iterator where = m_oTreeItemValueMap.find(pNMPropTree->pItem->GetLabelText());
					if(m_oTreeItemValueMap.end() != where)
					{
						sDisplayString = where->second->sUserData.c_str();
					}
					///
				}

				double x = 0.f , y = 0.f;
				m_GLCanvas.Pick(x , y , dScaleFactor , sDisplayString);	/// waiting for user click
			
				CString rMsg;
				if((_T("Line Data") == CString(pParent->GetLabelText())) || (_T("Latest Rev Data") == CString(pParent->GetLabelText())))
				{
					rMsg.Format(_T("%d,%d,%.1lf") , int(x) , int(y) , dScaleFactor);
				}
				else
				{
					rMsg.Format(_T("%d,%d") , int(x) , int(y));
				}
				(pNMPropTree->pItem)->SetItemValue((LPARAM)(rMsg.operator LPCTSTR()));
				///m_ctrlPropTree->RedrawWindow();
			}
		}
		else
		{
			AfxMessageBox( _T("First of all, select border file.") );
		}
	}
	else if(PTN_ITEMCHANGED == pNMPropTree->hdr.code)
	{
		CPropTreeItem* pParent = pNMPropTree->pItem->GetParent();
		if(pParent && (CString("Revision Data") == pParent->GetLabelText()) || pParent && (CString("Reserved Data") == pParent->GetLabelText()))
		{
			vector< string > oResult;
			CString rLabelText = pNMPropTree->pItem->GetLabelText();
			CString rItemValue = (const char*)pNMPropTree->pItem->GetItemValue();
			CTokenizer<CIsComma>::Tokenize( oResult , rItemValue.operator LPCTSTR() , CIsComma() );
			if( 2 == oResult.size() )
			{
				/// 2011.07.08 - added by humkyung
				const double dScaleFactor = 1.0;
				///

				m_GLCanvas.SetGLObject( atof(oResult[0].c_str()) , atof(oResult[1].c_str()) , dScaleFactor , rLabelText );
				m_GLCanvas.RedrawWindow();
			}
		}
		else if(pParent && (CString("Area...") == pParent->GetLabelText()) || pParent && (CString("Remove Area...") == pParent->GetLabelText()))
		{
			vector< string > oResult;
			CString rLabelText = pNMPropTree->pItem->GetLabelText();
			CString rItemValue = (const char*)pNMPropTree->pItem->GetItemValue();
			CTokenizer<CIsComma>::Tokenize( oResult , rItemValue.operator LPCTSTR() , CIsComma() );
			if( 4 == oResult.size() )
			{
				GLfloat x1 = atof(oResult[0].c_str()) , y1 = atof(oResult[1].c_str()) , x2 = atof(oResult[2].c_str()) , y2 = atof(oResult[3].c_str());

				m_GLCanvas.SetGLObject( x1 , y1 , x2 , y2 , rLabelText );
				m_GLCanvas.RedrawWindow();
			}
		}
		else if(pParent && (CString("Line Data") == pParent->GetLabelText()) || (CString("Latest Rev Data") == pParent->GetLabelText()))
		{
			vector< STRING_T > oResult;
			CString rLabelText = pNMPropTree->pItem->GetLabelText();
			CString rItemValue = (const char*)pNMPropTree->pItem->GetItemValue();
			CTokenizer<CIsComma>::Tokenize( oResult , rItemValue.operator LPCTSTR() , CIsComma() );
			if( 3 == oResult.size() )
			{
				//! 2011.07.08/2011.09.22 - added by humkyung
				const double dScaleFactor = ATOF_T( oResult[2].c_str() );;
				//!

				m_GLCanvas.SetGLObject( ATOF_T(oResult[0].c_str()) , ATOF_T(oResult[1].c_str()) , dScaleFactor , rLabelText );
				m_GLCanvas.RedrawWindow();
			}
			else
			{
				m_GLCanvas.DestroyGLObject(rLabelText );
				m_GLCanvas.RedrawWindow();
			}
		}
	}

	return TRUE;
}

/**	
	@brief	설정값을 저장한다.
	@author	humkyung
	@date	????.??.??
	@return	void	
*/
void CBorderSettingDlg::Save()
{
	CProject* pProject = m_pAdminDoc->GetCurProject();
	if(NULL != pProject)
	{
		const CString name = pProject->name();
		const CProject::DatabaseProp prop = pProject->GetDatabaseProp();
		CAppPostgreSQL database(name , prop.Host.c_str() , prop.User.c_str() , prop.Password.c_str());
		try
		{
			CString rIniFilePath = pProject->path() + CString( _T("\\Setting\\") ) + pProject->name() + CString(".ini");

			CString rApp( _T("Border Setting") );
			
			CString rString;
			GetDlgItemText(IDC_EDIT_BORDER_FILE_PATH , rString);
			if(!rString.IsEmpty())
			{
				WritePrivateProfileString(rApp , _T("Border File Path") , rString , rIniFilePath);
			}

			/// save ignore char - 2013.08.22 added by humkyung
			CProjectData* pProjectData = CProjectData::GetInstance();
			map<STRING_T,STRING_T>* pDrawingSetup = pProjectData->GetDrawingSetup();
			map<CString,CString>::iterator where = pProjectData->m_pProjectSettingMap->find(_T("ignore_char"));
			if(where != pProjectData->m_pProjectSettingMap->end())
			{
				GetDlgItemText(IDC_EDIT_IGNORE_CHAR  , rString);
				where->second = rString;
			}
			/// up to here

			/// Iso Dwg Data Area를 저장한다.
			{
				int iIndex = 0;
				CPropTreeItem* pItem = m_pIsoDwgAreaRoot->GetChild();
				while(pItem)
				{
					const STRING_T sLabelValue = (char*)pItem->GetLabelText();
					const STRING_T sItemValue = (char*)pItem->GetItemValue();
					
					(*pDrawingSetup)[sLabelValue + _T("_area")] = sItemValue;

					pItem = pItem->GetSibling();
					iIndex++;
				}
				pItem = m_pLinkKeyDataAreaRoot->GetChild();
				while(pItem)
				{
					const STRING_T sLabelValue = (char*)pItem->GetLabelText();
					const STRING_T sItemValue = (char*)pItem->GetItemValue();
					CString sKeyName;

					(*pDrawingSetup)[sLabelValue + _T("_area")] = sItemValue;

					pItem = pItem->GetSibling();
					iIndex++;
				}
				/// up to here
			}
			
			SaveRemoveAreaData(database);
			SaveCheckAreaData(database,rIniFilePath);
			
			//////////////////////////////////////////////////////////////////////////
			CPropTreeItem* pItem = m_pGeneralDataRoot->GetChild();
			while(pItem)
			{
				CString sItemValue;
				const STRING_T sLabelValue = pItem->GetLabelText();
				if(sLabelValue == _T("Font No"))
				{
					CPropTreeItemCombo* pItemCombo = static_cast<CPropTreeItemCombo*>(pItem);
					pItemCombo->GetWindowText(sItemValue);
				}
				else
				{
					sItemValue = (char*)pItem->GetItemValue();
				}
				
				OSTRINGSTREAM_T oss;
				oss << "insert into t_iso_drawing_setup(name,value) select '" << sLabelValue << "','" << sItemValue.operator LPCTSTR();
				oss << "' where not exists(select 1 from t_iso_drawing_setup where name='" << sLabelValue << "')";
				(*database.session()) << oss.str();
				oss.str(_T(""));
				oss << "update t_iso_drawing_setup set value='" << sItemValue.operator LPCTSTR() << "' where name='" << sLabelValue << "'";
				(*database.session()) << oss.str();

				pItem = pItem->GetSibling();
			}

			int index = 0;
			rApp = _T("Line Data Field");
			pItem = m_pLineDataRoot->GetChild();
			while(pItem)
			{
				const STRING_T sLabelValue = pItem->GetLabelText();
				const STRING_T sItemValue = (char*)pItem->GetItemValue();

				OSTRINGSTREAM_T oss;
				oss << "insert into t_iso_drawing_setup(name,value) select '" << sLabelValue << "_pos','" << sItemValue;
				oss << "' where not exists(select 1 from t_iso_drawing_setup where name='" << sLabelValue << "_pos')";
				(*database.session()) << oss.str();
				oss.str(_T(""));
				oss << "update t_iso_drawing_setup set value='" << sItemValue << "' where name='" << sLabelValue << "_pos'";
				(*database.session()) << oss.str();

				pItem = pItem->GetSibling();
			}

			/// save latest rev data - 2011.10.25 added by humkyung
			rApp = _T("Border Setting");
			pItem = m_oPropTreeItemMap[_T("Latest Rev Data")]->GetChild();
			while(pItem)
			{
				const STRING_T sLabelValue = pItem->GetLabelText();
				const STRING_T sItemValue  = (TCHAR*)pItem->GetItemValue();
				{
					OSTRINGSTREAM_T oss;
					oss << _T("insert into t_iso_drawing_setup(name,value) select '") << sLabelValue << _T("','") << sItemValue;
					oss << _T("' where not exists(select 1 from t_iso_drawing_setup where name='") << sLabelValue << _T("')");
					(*database.session()) << oss.str();
					oss.str(_T(""));
					oss << _T("update t_iso_drawing_setup set value='") << sItemValue << _T("' where name='") << sLabelValue << _T("'");
					(*database.session()) << oss.str();
				}
				pItem = pItem->GetSibling();
			}
			/// up to here

			index = 0;
			rApp = _T("Revision Data Field");
			pItem = m_pRevisionDataRoot->GetChild();
			while(pItem)
			{
				STRING_T sItemValue;
				const STRING_T sLabelValue = pItem->GetLabelText();
				/// 2012.09.21 added by humkyung
				if(sLabelValue == _T("Sort"))
				{
					CString rString;
					CPropTreeItemCombo* pItemCombo = static_cast<CPropTreeItemCombo*>(pItem);
					pItemCombo->GetWindowText(rString);
					sItemValue = rString.operator LPCTSTR();
				}
				else
				{
					sItemValue = (TCHAR*)pItem->GetItemValue();
				}
				/// up to here
				
				{
					OSTRINGSTREAM_T oss;
					oss << _T("insert into t_iso_drawing_setup(name,value) select '") << sLabelValue << _T("','") << sItemValue;
					oss << _T("' where not exists(select 1 from t_iso_drawing_setup where name='") << sLabelValue << _T("')");
					(*database.session()) << oss.str();
					oss.str(_T(""));
					oss << _T("update t_iso_drawing_setup set value='") << sItemValue << _T("' where name='") << sLabelValue << _T("'");
					(*database.session()) << oss.str();
				}

				pItem = pItem->GetSibling();
			}
		}
		catch(const std::exception& ex)
		{
			AfxMessageBox(ex.what(),MB_OK|MB_ICONERROR);
		}
	}
}

/**
	@brief	reutrn CDgnDocument
	@author	humkyung
**/
CDgnDocument* CBorderSettingDlg::GetDgnDocument()
{
	return m_DgnDoc.get();
}

/**
	@brief	return BOM Area
	@author	humkyung
	@date	2015.11.26
*/
CDgnVolume CBorderSettingDlg::GetBOMArea() const
{
	CDgnVolume res(0,0,0,0,0,0);

	CPropTreeItem* pItem = m_pQCCheckAreaRoot->GetChild();
	while(pItem)
	{
		const STRING_T sLabelValue = (TCHAR*)pItem->GetLabelText();
		const STRING_T sItemValue = (TCHAR*)pItem->GetItemValue();
		if(_T("BOM AREA") == sLabelValue)
		{
			vector<STRING_T> oResult;
			CTokenizer<CIsComma>::Tokenize(oResult,sItemValue,CIsComma());
			if(4 == oResult.size())
			{
				res.Set
				(
					ATOF_T(oResult[0].c_str()),ATOF_T(oResult[1].c_str()),0,
					ATOF_T(oResult[2].c_str()),ATOF_T(oResult[3].c_str()),0
				);
				break;
			}
		}
		
		pItem = pItem->GetSibling();
	}

	return res;
}

/**	
	@brief	revision data tree를 초기화 시킨다.
	@author	humkyung
	@date	????.??.??
	@return	void	
*/
void CBorderSettingDlg::InitRevDataTree(CAppPostgreSQL& database)
{
	CProject* pProject = m_pAdminDoc->GetCurProject();
	if(NULL != pProject)
	{
		CString rIniFilePath = pProject->GetIniFilePath();

		TCHAR szBuf[MAX_PATH] = {'\0' ,};
		m_pRevisionDataRoot = m_ctrlPropTree->InsertItem(new CPropTreeItem());
		if(NULL == m_pRevisionDataRoot) return;
		CString rApp = _T("Revision Data Field");

		CPropTreeItemCombo* pPropTreeItemCombo = (CPropTreeItemCombo*)m_ctrlPropTree->InsertItem(new CPropTreeItemCombo(), m_pRevisionDataRoot);
		pPropTreeItemCombo->SetLabelText(_T("Sort"));

		const DWORD dwStyle = WS_BORDER | WS_CHILD | WS_VSCROLL | CBS_UPPERCASE | CBS_SORT | CBS_AUTOHSCROLL | CBS_DROPDOWNLIST;	// for grid style only
		
		pPropTreeItemCombo->CreateComboBox(dwStyle);
		int nItem = pPropTreeItemCombo->AddString("ASCEND");
		nItem = pPropTreeItemCombo->SetItemData(nItem , LPARAM(1));
		nItem = pPropTreeItemCombo->AddString("DESCEND");
		pPropTreeItemCombo->SetItemData(nItem , LPARAM(2));
		
		pPropTreeItemCombo->SetCurSel(0);
		{
			OSTRINGSTREAM_T oss;
			oss << _T("select value from t_iso_drawing_setup where name='Sort'");
			soci::rowset<soci::row> rs(database.session()->prepare << oss.str().c_str());
			for(soci::rowset<soci::row>::const_iterator itr = rs.begin();itr != rs.end();++itr)
			{
				if(soci::i_ok != itr->get_indicator(_T("value"))) continue;
				const STRING_T sValue = itr->get<STRING_T>(_T("value"));
				pPropTreeItemCombo->SetItemValue(LPARAM(sValue.c_str()));
				(STRING_T(_T("ASCEND")) == sValue) ? pPropTreeItemCombo->SetCurSel(0) : nItem = pPropTreeItemCombo->SetCurSel(1);
			}
		}

		CPropTreeItemEdit* pPropTreeItemEdit = static_cast<CPropTreeItemEdit*>(m_ctrlPropTree->InsertItem(new CPropTreeItemEdit(), m_pRevisionDataRoot));
		pPropTreeItemEdit->SetLabelText(_T("Display Count"));
		const CString sRegEx (_T("0|[1-9][0-9]*"));
		pPropTreeItemEdit->SetRegEx(sRegEx);
		{
			OSTRINGSTREAM_T oss;
			oss << _T("select value from t_iso_drawing_setup where name='Display Count'");
			soci::rowset<soci::row> rs(database.session()->prepare << oss.str().c_str());
			for(soci::rowset<soci::row>::const_iterator itr = rs.begin();itr != rs.end();++itr)
			{
				if(soci::i_ok != itr->get_indicator(_T("value"))) continue;
				const STRING_T sValue = itr->get<STRING_T>(_T("value"));
				pPropTreeItemEdit->SetItemValue(LPARAM(sValue.c_str()));
			}
		}
		
		pPropTreeItemEdit = static_cast<CPropTreeItemEditButton*>(m_ctrlPropTree->InsertItem(new CPropTreeItemEditButton("p"), m_pRevisionDataRoot));
		pPropTreeItemEdit->SetLabelText(_T("Display Distance"));
		pPropTreeItemEdit->SetRegEx(sRegEx);
		{
			OSTRINGSTREAM_T oss;
			oss << _T("select value from t_iso_drawing_setup where name='Display Distance'");
			soci::rowset<soci::row> rs(database.session()->prepare << oss.str().c_str());
			for(soci::rowset<soci::row>::const_iterator itr = rs.begin();itr != rs.end();++itr)
			{
				if(soci::i_ok != itr->get_indicator(_T("value"))) continue;
				const STRING_T sValue = itr->get<STRING_T>(_T("value"));
				pPropTreeItemEdit->SetItemValue(LPARAM(sValue.c_str()));
			}
		}

		/// revision data항목을 표기
		const CString rSqlFilePath = CAdminDocData::GetExecPath() + _T("\\Setting\\T_REVISION.XML");
		CSQLFile sqlFile(rSqlFilePath.operator LPCSTR());
		if(ERROR_SUCCESS == sqlFile.ParseXML(rSqlFilePath.operator LPCTSTR()))
		{
			const int nCount = sqlFile.GetColumnCount();
			list<STRING_T> oFieldNameEntry;
			{
				for(int i = 0;i < nCount;++i)
				{
					const string rFieldName = sqlFile.GetFieldNameAt(i);
					oFieldNameEntry.push_back(rFieldName);
				}
			}
			
			const CString sCoordRegEx(_T("[0-9]+,[0-9]+"));
			vector<STRING_T> oResult;
			for(list<STRING_T>::const_iterator itr = oFieldNameEntry.begin();itr != oFieldNameEntry.end();++itr)
			{
				if((_T("KEY") == (*itr)) || (_T("GRADE") == (*itr))) continue;

				CPropTreeItemEditButton* pPropTreeItem = static_cast<CPropTreeItemEditButton*>(m_ctrlPropTree->InsertItem(new CPropTreeItemEditButton("p"), m_pRevisionDataRoot));	
				pPropTreeItem->SetLabelText(itr->c_str());
				pPropTreeItem->SetRegEx(sCoordRegEx);
				{
					OSTRINGSTREAM_T oss;
					oss << _T("select value from t_iso_drawing_setup where name='") << *itr << _T("'");
					soci::rowset<soci::row> rs(database.session()->prepare << oss.str().c_str());
					for(soci::rowset<soci::row>::const_iterator jtr = rs.begin();jtr != rs.end();++jtr)
					{
						if(soci::i_ok != jtr->get_indicator(_T("value"))) continue;
						const STRING_T sValue = jtr->get<STRING_T>(_T("value"));
						CTokenizer<CIsComma>::Tokenize(oResult , sValue , CIsComma());
						if(oResult.size() >= 2)
						{
							pPropTreeItem->SetItemValue(LPARAM(sValue.c_str()));
							m_GLCanvas.SetGLObjectColor(255,0,0);
							m_GLCanvas.SetGLObject(ATOF_T(oResult[0].c_str()) , ATOF_T(oResult[1].c_str()) , 1.0 , itr->c_str());

							/// set tree item map
							map<STRING_T, TreeItemValue*>::iterator where = m_oTreeItemValueMap.find(*itr);
							if(m_oTreeItemValueMap.end() == where)
							{
								TreeItemValue *pTreeItemValue = (TreeItemValue*)calloc(1 , sizeof(TreeItemValue)); 
								pTreeItemValue->pTreeItem = pPropTreeItem;
								pTreeItemValue->sUserData = *itr;
								m_oTreeItemValueMap.insert(make_pair(*itr , pTreeItemValue));
							}
							else
							{
								where->second->pTreeItem = pPropTreeItem;
							}
							///
						}
					}
				}
			}
		}
		m_pRevisionDataRoot->Expand(); // have this item expanded by default
		
		m_pRevisionDataRoot->SetLabelText(_T("Revision Data"));
	}
}

CString CBorderSettingDlg::GetLineDataPos( const CString& rLineDataName , const CString& rIniFilePath)
{
	CString res;

	CString rApp = "Line Data Field";
	char szBuf[MAX_PATH] = {0,};
	
	int nCount = GetPrivateProfileInt(rApp , _T("Count") , 0 , rIniFilePath);	
	vector<string> oResult;
	for(int i = 0;i < nCount;++i)
	{
		CString rKey;
		rKey.Format("Pos%d" , i);
		if(GetPrivateProfileString(rApp , rKey , NULL , szBuf , MAX_PATH , rIniFilePath))
		{
			CTokenizer<CIsComma>::Tokenize( oResult , szBuf , CIsComma());
			if( rLineDataName == oResult[0].c_str() )
			{
				res = oResult[1].c_str() + CString(",") + oResult[2].c_str();
				return res;
			}
		}
	}

	return res;

}

/**	
	@brief	Line data tree를 초기화 시킨다.
	@author	HumKyung
	@date	????.??.??
	@return	void	
*/
void CBorderSettingDlg::InitLineDataTree(CAppPostgreSQL& database)
{
	CProject* pProject = m_pAdminDoc->GetCurProject();
	if(NULL != pProject)
	{
		CString rIniFilePath = pProject->GetIniFilePath();
 
		TCHAR szBuf[MAX_PATH] = {'\0' ,};
		m_pLineDataRoot = m_ctrlPropTree->InsertItem(new CPropTreeItem());
		m_pLineDataRoot->SetLabelText(_T("Line Data"));
		if(NULL == m_pLineDataRoot) return;
		
		CString strRegEx (_T("(-)?[0-9]*,(-)?[0-9]*,([0-9])*((\\.)?[0-9]?)"));
		CString rApp = _T("Line Data Field");
		vector<STRING_T> oResult;

		double dScaleFactor = 1.0;

		try
		{
			const CString rSqlFilePath = CAdminDocData::GetExecPath() + _T("\\Setting\\T_ISO_LINE_DATA.XML");
			CSQLFile sqlFile(rSqlFilePath.operator LPCSTR());
			if(ERROR_SUCCESS == sqlFile.ParseXML(rSqlFilePath.operator LPCTSTR()))
			{
				const int nCount = sqlFile.GetColumnCount();
				list<STRING_T> oFieldNameEntry;
				{
					for(int i = 0;i < nCount;++i)
					{
						const string rFieldName = sqlFile.GetFieldNameAt(i);
						oFieldNameEntry.push_back(rFieldName);
					}
				}

				for(list<STRING_T>::const_iterator itr = oFieldNameEntry.begin();itr != oFieldNameEntry.end();++itr)
				{
					CString rKey;
					{
						CString rLabelText(itr->c_str());
						CPropTreeItemEditButton* pPropTreeItem = static_cast<CPropTreeItemEditButton*>(m_ctrlPropTree->InsertItem(new CPropTreeItemEditButton("p"), m_pLineDataRoot));
						pPropTreeItem->SetLabelText(rLabelText);
						pPropTreeItem->SetRegEx( strRegEx );

						map<string , TreeItemValue*>::iterator where = m_oTreeItemValueMap.find(*itr);
						if(m_oTreeItemValueMap.end() == where)
						{
							TreeItemValue *pTreeItemValue = (TreeItemValue*)calloc(1 , sizeof(TreeItemValue)); 
							pTreeItemValue->pTreeItem = pPropTreeItem;
							pTreeItemValue->sUserData = _T("1");
							m_oTreeItemValueMap.insert(make_pair(*itr , pTreeItemValue));
						}
						else
						{
							where->second->pTreeItem = pPropTreeItem;
						}
					}
				}

				int i = 0;
				{
					for(list<STRING_T>::const_iterator itr = oFieldNameEntry.begin();itr != oFieldNameEntry.end();++itr,++i)
					{
						OSTRINGSTREAM_T oss;
						oss << "select value from t_iso_drawing_setup where name='" << *itr << "_pos'";
						soci::rowset<soci::row> rs(database.session()->prepare << oss.str().c_str());
						for(soci::rowset<soci::row>::const_iterator jtr = rs.begin();jtr != rs.end();++jtr)
						{
							const STRING_T sValue = jtr->get<STRING_T>("value");
							CTokenizer<CIsComma>::Tokenize(oResult , sValue , CIsComma());
							if( oResult.size() >= 3 )
							{
								map<string , TreeItemValue*>::iterator where = m_oTreeItemValueMap.find(oResult[0]);
								if(m_oTreeItemValueMap.end() != where)
								{
									STRING_T sTextScale = (4 == oResult.size()) ? oResult[3] : _T("1");
									if(!oResult[1].empty() && !oResult[2].empty())
									{
										CString rValue = STRING_T(oResult[1] + _T(",") + oResult[2] + _T(",") + sTextScale).c_str();
										
										where->second->pTreeItem->SetItemValue(LPARAM(rValue.operator LPCSTR()));

										/// 2011.07.08 - added by humkyung
										oss.str(_T(""));
										oss << dScaleFactor;
										where->second->sUserData = oss.str();

										m_GLCanvas.SetGLObjectColor(255,0,0);
										m_GLCanvas.SetGLObject(ATOF_T(oResult[1].c_str()) , ATOF_T(oResult[2].c_str()) , ATOF_T(sTextScale.c_str()) , itr->c_str());
									}
								}
							}
						}
					}
				}
			}
			m_pLineDataRoot->Expand(); // have this item expanded by default
			m_pLineDataRoot->SetLabelText(_T("Line Data"));
		}
		catch(const std::exception& ex)
		{
			AfxMessageBox(ex.what(),MB_OK|MB_ICONERROR);
		}
	}
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-24
    @class      CBorderSettingDlg
    @function   InitLatestRevDataTree
    @return     void
    @brief		init latest revision data tree
******************************************************************************/
void CBorderSettingDlg::InitLatestRevDataTree(CAppPostgreSQL& database)
{
	TCHAR szBuf[MAX_PATH] = {'\0' ,};

	CProject* pProject = m_pAdminDoc->GetCurProject();
	if(NULL != pProject)
	{
		CString rIniFilePath = pProject->GetIniFilePath();
		vector<STRING_T> oResult;

		const CString sRegEx (_T("(-)?[0-9]*,(-)?[0-9]*,([0-9])*((\\.)?[0-9]?)"));

		CString sLabelText = _T("Latest Rev Data");
		m_oPropTreeItemMap.insert(make_pair(sLabelText , m_ctrlPropTree->InsertItem(new CPropTreeItem())));
		m_oPropTreeItemMap[sLabelText]->SetLabelText(sLabelText);

		double dScaleFactor = 1.0;
		const CString rApp = _T("Border Setting");
		STRING_T sKeyNames[]={_T("LATEST_REV_NO") , _T("LATEST_REV_DATE") , _T("LATEST_REV_DRWN_BY") , _T("LATEST_REV_CHECKED_BY") , _T("LATEST_REV_REVIEWED_BY") , _T("LATEST_REV_APPROVED_BY")};
		for(int i = 0;i < SIZE_OF_ARRAY(sKeyNames);++i)
		{
			CPropTreeItemEditButton* pPropTreeItem = static_cast<CPropTreeItemEditButton*>(m_ctrlPropTree->InsertItem(new CPropTreeItemEditButton("p"), m_oPropTreeItemMap[sLabelText]));
			pPropTreeItem->SetLabelText(sKeyNames[i].c_str());
			pPropTreeItem->SetRegEx(sRegEx);
			{
				OSTRINGSTREAM_T oss;
				oss << _T("select value from t_iso_drawing_setup where name='") << sKeyNames[i] << _T("'");
				soci::rowset<soci::row> rs(database.session()->prepare << oss.str().c_str());
				for(soci::rowset<soci::row>::const_iterator itr = rs.begin();itr != rs.end();++itr)
				{
					if(soci::i_ok != itr->get_indicator(_T("value"))) continue;
					const STRING_T sValue = itr->get<STRING_T>(_T("value"));
					CTokenizer<CIsComma>::Tokenize(oResult , sValue , CIsComma());
					if( oResult.size() >= 3)
					{
						STRING_T sTextScale = (3 == oResult.size()) ? oResult[2] : _T("1");
						CString rValue = STRING_T(oResult[0] + _T(",") + oResult[1] + _T(",") + sTextScale).c_str();

						pPropTreeItem->SetItemValue(LPARAM(rValue.operator LPCSTR()));
						m_GLCanvas.SetGLObjectColor(255,0,0);
						m_GLCanvas.SetGLObject(ATOF_T(oResult[0].c_str()) , ATOF_T(oResult[1].c_str()) , ATOF_T(sTextScale.c_str()) , sKeyNames[i].c_str() );
					}
				}
			}
		}

		m_oPropTreeItemMap[sLabelText]->Expand();
	}
}

/**	
	@brief	The CBorderSettingDlg::InitCheckAreaDataTree function
	@author	humkyung
	@return	void	
*/
void CBorderSettingDlg::InitCheckAreaDataTree(CAppPostgreSQL& database)
{
	CProject* pProject = m_pAdminDoc->GetCurProject();
	if(NULL != pProject)
	{
		static const CString sRegEx (_T("-?[0-9]*,-?[0-9]*,-?[0-9]*,-?[0-9]*"));
		CString rIniFilePath = pProject->GetIniFilePath();
		CString rApp( _T("Border Setting") );
		TCHAR szBuf[MAX_PATH] = {'\0' ,};
		
		vector<STRING_T> oResult;
		//////////////////////////////////////////////////////////////////////////
		m_pQCCheckAreaRoot = m_ctrlPropTree->InsertItem(new CPropTreeItem());
		m_pQCCheckAreaRoot->SetLabelText(_T(QC_CHECK_AREA_LABEL));

		CPropTreeItemEditButton* pPropTreeItem = static_cast<CPropTreeItemEditButton*>(m_ctrlPropTree->InsertItem(new CPropTreeItemEditButton(_T("p")), m_pQCCheckAreaRoot));
		pPropTreeItem->SetLabelText(_T("BOM AREA"));
		pPropTreeItem->SetInfoText( _T("BOM AREA") );
		pPropTreeItem->SetRegEx(sRegEx);
		{
			OSTRINGSTREAM_T oss;
			oss << "select value from t_iso_drawing_setup where name='BOM AREA'";
			soci::rowset<soci::row> rs(database.session()->prepare << oss.str());
			for(soci::rowset<soci::row>::const_iterator jtr = rs.begin();jtr != rs.end();++jtr)
			{
				const STRING_T sValue = jtr->get<STRING_T>("value");
				pPropTreeItem->SetItemValue(LPARAM(sValue.c_str()));
				CTokenizer<CIsComma>::Tokenize(oResult , sValue , CIsComma());
				if(4 == oResult.size())
				{
					m_GLCanvas.SetGLObjectColor(255,0,0);
					m_GLCanvas.SetGLObject(ATOF_T(oResult[0].c_str()) , ATOF_T(oResult[1].c_str()) , ATOF_T(oResult[2].c_str()) , ATOF_T(oResult[3].c_str()) , _T("BOM AREA"));
				}
			}
		}

		pPropTreeItem = static_cast<CPropTreeItemEditButton*>(m_ctrlPropTree->InsertItem(new CPropTreeItemEditButton( _T("p") ), m_pQCCheckAreaRoot));
		pPropTreeItem->SetLabelText(_T("GRAPHIC AREA"));
		pPropTreeItem->SetInfoText(_T("GRAPHIC AREA"));
		pPropTreeItem->SetRegEx(sRegEx);
		{
			OSTRINGSTREAM_T oss;
			oss << "select value from t_iso_drawing_setup where name='GRAPHIC AREA'";
			soci::rowset<soci::row> rs(database.session()->prepare << oss.str());
			for(soci::rowset<soci::row>::const_iterator jtr = rs.begin();jtr != rs.end();++jtr)
			{
				const STRING_T sValue = jtr->get<STRING_T>("value");
				pPropTreeItem->SetItemValue(LPARAM(sValue.c_str()));
				CTokenizer<CIsComma>::Tokenize(oResult , sValue , CIsComma());
				if(4 == oResult.size())
				{
					m_GLCanvas.SetGLObjectColor(255,0,0);
					m_GLCanvas.SetGLObject(ATOF_T(oResult[0].c_str()) , ATOF_T(oResult[1].c_str()) , ATOF_T(oResult[2].c_str()) , ATOF_T(oResult[3].c_str()) , _T("GRAPHIC AREA"));
				}
			}
		}

		/// 2012.09.19 added by humkyung
		pPropTreeItem = static_cast<CPropTreeItemEditButton*>(m_ctrlPropTree->InsertItem(new CPropTreeItemEditButton( _T("p") ), m_pQCCheckAreaRoot));
		pPropTreeItem->SetLabelText(_T("PLOTTING AREA"));
		pPropTreeItem->SetInfoText(_T("PLOTTING AREA"));
		pPropTreeItem->SetRegEx(sRegEx);
		{
			OSTRINGSTREAM_T oss;
			oss << "select value from t_iso_drawing_setup where name='PLOTTING AREA'";
			soci::rowset<soci::row> rs(database.session()->prepare << oss.str());
			for(soci::rowset<soci::row>::const_iterator jtr = rs.begin();jtr != rs.end();++jtr)
			{
				const STRING_T sValue = jtr->get<STRING_T>("value");
				pPropTreeItem->SetItemValue(LPARAM(sValue.c_str()));
				CTokenizer<CIsComma>::Tokenize(oResult , sValue , CIsComma());
				if(4 == oResult.size())
				{
					m_GLCanvas.SetGLObjectColor(255,0,0);
					m_GLCanvas.SetGLObject(ATOF_T(oResult[0].c_str()) , ATOF_T(oResult[1].c_str()) , ATOF_T(oResult[2].c_str()) , ATOF_T(oResult[3].c_str()) , _T("PLOTTING AREA"));
				}
			}
		}
		/// up to here

		m_pQCCheckAreaRoot->Expand(); // have this item expanded by default
	}
}

/**
	@brief	save q/c check area
	@author	humkyung
**/
void CBorderSettingDlg::SaveCheckAreaData(CAppPostgreSQL& database,const CString &rIniFilePath)
{
	CString rApp("Border Setting");
	
	/// 2012.09.19 added by humkyung
	{
		const long dScale = m_DgnDoc->uor_per_subunit()*m_DgnDoc->subunits_per_master();
		CString str;
		str.Format(_T("%d") , dScale);
		WritePrivateProfileString(rApp , _T("uor per master") , str , rIniFilePath);
	}
	/// up to here

	map<STRING_T,STRING_T>* pDrawingSetup = CProjectData::GetInstance()->GetDrawingSetup();

	CPropTreeItem* pItem = m_pQCCheckAreaRoot->GetChild();
	while(pItem)
	{
		const STRING_T sLabelValue = (TCHAR*)pItem->GetLabelText();
		const STRING_T sItemValue = (TCHAR*)pItem->GetItemValue();

		(*pDrawingSetup)[sLabelValue] = sItemValue;
		
		pItem = pItem->GetSibling();
	}
}

/**	
	@brief	area data tree를 초기화한다.
	@author	HumKyung
	@author	????.??.??
	@return	void	
*/
void CBorderSettingDlg::InitAreaDataTree(CAppPostgreSQL& database)
{
	CProject* pProject = m_pAdminDoc->GetCurProject();
	if(NULL != pProject)
	{
		static const CString sRegEx (_T("-?[0-9]*,-?[0-9]*,-?[0-9]*,-?[0-9]*"));
		CString rIniFilePath = pProject->GetIniFilePath();
		CString rApp( _T("Border Setting") );
		TCHAR szBuf[MAX_PATH] = {'\0' ,};
	
		vector<STRING_T> oResult;
		//////////////////////////////////////////////////////////////////////////
		m_pIsoDwgAreaRoot = m_ctrlPropTree->InsertItem(new CPropTreeItem());
		m_pIsoDwgAreaRoot->SetLabelText(_T("Iso Dwg Data Area..."));

		const CString rSqlFilePath = CAdminDocData::GetExecPath() + _T("\\Setting\\T_ISO_LINE_DATA.XML");
		CSQLFile sqlFile(rSqlFilePath.operator LPCSTR());
		if(ERROR_SUCCESS == sqlFile.ParseXML(rSqlFilePath.operator LPCTSTR()))
		{
			const int nCount = sqlFile.GetColumnCount();
			list<STRING_T> oFieldNameEntry;
			{
				for(int i = 0;i < nCount;++i)
				{
					const string rFieldName = sqlFile.GetFieldNameAt(i);
					oFieldNameEntry.push_back(rFieldName);
				}
			}

			/// 2012.08.26 added by humkyung
			CString aryLinkKeyDataArea[]=
			{
				_T("PLANT_NO") , _T("AREA_NO") , _T("UNIT_NO") , _T("SECTION_NO") , _T("CLASS") , 
				_T("SEQUENCE") , _T("FLUID")   , _T("SHEET_NO"), _T("LINE_ID")
			};
			const int iArraySize = sizeof(aryLinkKeyDataArea)/sizeof(aryLinkKeyDataArea[0]);
			m_pLinkKeyDataAreaRoot = m_ctrlPropTree->InsertItem(new CPropTreeItem());
			m_pLinkKeyDataAreaRoot->SetLabelText(LINK_KEY_DATA_AREA_LABEL);
			/// up to here

			for(list<STRING_T>::const_iterator itr = oFieldNameEntry.begin();itr != oFieldNameEntry.end();++itr)
			{
				CPropTreeItemEditButton* pPropTreeItem = NULL;
				const int at = find(aryLinkKeyDataArea , aryLinkKeyDataArea + iArraySize , itr->c_str()) - aryLinkKeyDataArea;
				if((at >=0) && (at < iArraySize))
				{
					pPropTreeItem = static_cast<CPropTreeItemEditButton*>(m_ctrlPropTree->InsertItem(new CPropTreeItemEditButton(_T("p")), m_pLinkKeyDataAreaRoot));
				}
				else
				{
					pPropTreeItem = static_cast<CPropTreeItemEditButton*>(m_ctrlPropTree->InsertItem(new CPropTreeItemEditButton(_T("p")), m_pIsoDwgAreaRoot));
				}

				CString sTreeItemName(itr->c_str());
				pPropTreeItem->SetLabelText(sTreeItemName);
				pPropTreeItem->SetRegEx(sRegEx);

				OSTRINGSTREAM_T oss;
				oss << "select value from t_iso_drawing_setup where name='" << *itr << "_area'";
				soci::rowset<soci::row> rs(database.session()->prepare << oss.str());
				for(soci::rowset<soci::row>::const_iterator jtr = rs.begin();jtr != rs.end();++jtr)
				{
					const STRING_T sValue = jtr->get<STRING_T>("value");
					pPropTreeItem->SetItemValue(LPARAM(sValue.c_str()));
					CTokenizer<CIsComma>::Tokenize(oResult , sValue , CIsComma());
					if(4 == oResult.size())
					{
						m_GLCanvas.SetGLObjectColor(255,0,0);
						m_GLCanvas.SetGLObject(atof(oResult[0].c_str()) , atof(oResult[1].c_str()) , atof(oResult[2].c_str()) , atof(oResult[3].c_str()) , sTreeItemName );
					}
				}
			}
			m_pIsoDwgAreaRoot->Expand(); /// have this item expanded by default
			m_pLinkKeyDataAreaRoot->Expand();
		}
	}
}

/**	
	@brief	remove area data tree를 초기화한다.
	@author	humkyung
	@return	void	
*/
void CBorderSettingDlg::InitRemoveAreaTree(CAppPostgreSQL& database)
{
	CProject* pProject = m_pAdminDoc->GetCurProject();
	if(NULL != pProject)
	{
		static const CString sRegEx (_T("-?[0-9]*,-?[0-9]*,-?[0-9]*,-?[0-9]*"));
		static const STRING_T __rTreeDataString__[] = 
		{
			_T("Remove Area1") , _T("Remove Area1") , _T("Remove Area1") ,
			_T("Remove Area2") , _T("Remove Area2") , _T("Remove Area2") ,
			_T("Remove Area3") , _T("Remove Area3") , _T("Remove Area3") ,
			_T("Remove Area4") , _T("Remove Area4") , _T("Remove Area4") ,
			_T("Remove Area5") , _T("Remove Area5") , _T("Remove Area5")
		};
		CString rIniFilePath = pProject->GetIniFilePath();
		CString rApp("Border Setting");
		TCHAR szBuf[MAX_PATH] = {'\0' ,};
		vector<string> oResult;

		m_pIsoDwgRemoveAreaRoot = m_ctrlPropTree->InsertItem(new CPropTreeItem());
		m_pIsoDwgRemoveAreaRoot->SetLabelText(_T("Remove Area..."));
		const size_t nCount = sizeof( __rTreeDataString__ ) / sizeof( __rTreeDataString__[0] );
		for( int i = 0;i < nCount;i += 3 )
		{
			CPropTreeItemEditButton* pPropTreeItem = static_cast<CPropTreeItemEditButton*>(m_ctrlPropTree->InsertItem(new CPropTreeItemEditButton("p"), m_pIsoDwgRemoveAreaRoot));
			pPropTreeItem->SetLabelText(__rTreeDataString__[i].c_str());
			pPropTreeItem->SetInfoText(__rTreeDataString__[i + 1].c_str());
			pPropTreeItem->SetRegEx(sRegEx);
			{
				OSTRINGSTREAM_T oss;
				oss << _T("select value from t_iso_drawing_setup where name='") << __rTreeDataString__[i] << _T("'");
				soci::rowset<soci::row> rs(database.session()->prepare << oss.str());
				for(soci::rowset<soci::row>::const_iterator itr = rs.begin();itr != rs.end();++itr)
				{
					if(soci::i_ok != itr->get_indicator(_T("value"))) continue;
					const STRING_T sValue=itr->get<STRING_T>(_T("value"));
					pPropTreeItem->SetItemValue(LPARAM(sValue.c_str()));
					CTokenizer<CIsComma>::Tokenize(oResult , sValue , CIsComma());
					if(4 == oResult.size())
					{
						m_GLCanvas.SetGLObjectColor(255,255,0);
						m_GLCanvas.SetGLObject(ATOF_T(oResult[0].c_str()) , ATOF_T(oResult[1].c_str()) , ATOF_T(oResult[2].c_str()) , ATOF_T(oResult[3].c_str()) ,__rTreeDataString__[i].c_str());
					}
				}
			}
		}
		
		m_pIsoDwgRemoveAreaRoot->Expand(); // have this item expanded by default
	}
}

/**	
	@brief	remove area 설정값을 저장한다.
	@author	HumKyung.BAEK
	@param	rIniFilePath	a parameter of type const CString &
	@return	void	
*/
void CBorderSettingDlg::SaveRemoveAreaData(CAppPostgreSQL& database) const
{
	CString rApp("Border Setting");

	CPropTreeItem *pItem = m_pIsoDwgRemoveAreaRoot->GetChild();
	while(pItem)
	{
		const STRING_T sLabelValue = (char*)pItem->GetLabelText();
		const STRING_T sItemValue = (char*)pItem->GetItemValue();
		{
			OSTRINGSTREAM_T oss;
			oss << _T("insert into t_iso_drawing_setup(name,value) select '") << sLabelValue << _T("','") << sItemValue;
			oss << _T("' where not exists(select 1 from t_iso_drawing_setup where name='") << sLabelValue << _T("')");
			(*database.session()) << oss.str();
			oss.str(_T(""));
			oss << _T("update t_iso_drawing_setup set value='") << sItemValue << _T("' where name='") << sLabelValue << _T("'");
			(*database.session()) << oss.str();
		}
		pItem = pItem->GetSibling();
	}
}

BOOL CBorderSettingDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN ) // Enter Key Prevent.. 
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE )		
		{
			CString rBorderFilePath;
			GetDlgItemText(IDC_EDIT_BORDER_FILE_PATH,rBorderFilePath);
			
			if(NULL != m_DgnDoc.get())
			{
				m_DgnDoc->Read(rBorderFilePath);
				m_GLScene.Set(m_DgnDoc.get());
			}
			::TranslateMessage(pMsg);
			
			::DispatchMessage(pMsg);
			
			return TRUE;                    // DO NOT process further	
		}
	}	
	return CDialog::PreTranslateMessage(pMsg);
}

/******************************************************************************
    @author     humkyung
    @date       2012-06-05
    @class      CBorderSettingDlg
    @function   OnHelpInfo
    @return     BOOL
    @brief
******************************************************************************/
BOOL CBorderSettingDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	HWND hHtml = ::HtmlHelp(this->m_hWnd , CAdminDocData::GetExecPath() + _T("\\Help\\SmartISO.chm::/Border Setting.htm") , HH_DISPLAY_TOPIC , 0);

	return TRUE;///CDialog::OnHelpInfo(pHelpInfo);
}
