// LineListSelectOption1.cpp : implementation file
//


#include "stdafx.h"
#include "UserApp.h"
#include "tokenizer.h"
#include "LineListSelectOption1.h"
#include "UserAppDocData.h"
#include "EditLineDataForm.h"
#include "Profile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int CLineListSelectOption1::WIDTH = 230;
static vector<CLineListSelectOption1*> __SIBLINGS__;
/////////////////////////////////////////////////////////////////////////////
// CLineListSelectOption1 dialog


CLineListSelectOption1::CLineListSelectOption1(CWnd* pParent /*=NULL*/)
	: CDialog(CLineListSelectOption1::IDD, pParent) , /*m_pPropTreeCtrl(NULL) , */m_pTargetWnd(NULL)
{
	//{{AFX_DATA_INIT(CLineListSelectOption1)
	//}}AFX_DATA_INIT
	m_pBoldFont = NULL;
	m_bPlantListBox = FALSE;
	m_bAreaListBox = FALSE;
	m_bUnitListBox = FALSE;
	m_bSectionListBox = FALSE;
	m_bSelectBoutton = FALSE;

	__SIBLINGS__.push_back(this);
}

CLineListSelectOption1::~CLineListSelectOption1()
{
	try
	{
		SAFE_DELETE(m_pBoldFont);
	}
	catch(...)
	{
	}
}

void CLineListSelectOption1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLineListSelectOption1)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_STATIC_TITLE, m_TitleStaticCtrl);

	DDX_Control(pDX , IDC_LIST_ISSUED_DATE , m_wndIssueDateReport);
	DDX_Control(pDX , IDC_BUTTON_SELECT , m_btnSelect);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLineListSelectOption1, CDialog)
	//{{AFX_MSG_MAP(CLineListSelectOption1)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_ALL, OnButtonSelectAll)
	ON_BN_CLICKED(IDC_CHECK_ISSUE_DATE, OnCheckIssueDate)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_SELECT, OnButtonSelect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/// just a helper function to create nicer fade effect.
static COLORREF DarkenColor(COLORREF col,double factor)
{
	if(factor>0.0 && factor<=1.0)
	{
		BYTE red,green,blue,lightred,lightgreen,lightblue;
		red = GetRValue(col);
		green = GetGValue(col);
		blue = GetBValue(col);
		lightred = (BYTE)(red-(factor*red));
		lightgreen = (BYTE)(green-(factor*green));
		lightblue = (BYTE)(blue-(factor*blue));
		col = RGB(lightred,lightgreen,lightblue);
	}
	return(col);
}

/////////////////////////////////////////////////////////////////////////////
// CLineListSelectOption1 message handlers

BOOL CLineListSelectOption1::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CAppDocData* pDocData = CAppDocData::GetInstance();

	m_imgList.Create(IDB_USER_APP , 16 , 16 , RGB(255 , 0 , 255));

	m_pBoldFont = new CFont;
	m_pBoldFont->CreateFont(20,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0, _T("Arial"));
	m_TitleStaticCtrl.SetFont(m_pBoldFont);
	m_TitleStaticCtrl.SetWindowText( _T("WBS Control Query") );
	
	CRect rect;
	m_TitleStaticCtrl.GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.top += rect.bottom + 5;
	rect.bottom += 100;

	DWORD uStyle = TVS_HASBUTTONS|TVS_HASLINES|TVS_FULLROWSELECT|TVS_NONEVENHEIGHT|TVS_CHECKBOXES|TVS_EDITLABELS;
	if(m_cSelectionTreeList.Create(uStyle|WS_CHILD|WS_VISIBLE|WS_BORDER , rect , this , 0x101))
	{
		m_cSelectionTreeList.InsertColumn(0 , _T("Plant/Area/Unit/Section") , LVCFMT_LEFT, 180);
		
		m_cImages.Create (16, 16, ILC_COLOR32|ILC_MASK, 15, 1);
		m_cImages.Add(AfxGetApp()->LoadIcon(IDI_ROOT));
		m_cImages.Add(AfxGetApp()->LoadIcon(IDI_PLANT));
		m_cImages.Add(AfxGetApp()->LoadIcon(IDI_AREA));
		m_cImages.Add(AfxGetApp()->LoadIcon(IDI_UNIT));
		m_cImages.Add(AfxGetApp()->LoadIcon(IDI_SECTION));
		m_cSelectionTreeList.SetImageList(&m_cImages,TVSIL_NORMAL);

		const DWORD uExStyle = TVS_EX_SUBSELECT|TVS_EX_FULLROWMARK|TVS_EX_ITEMLINES | WS_EX_CLIENTEDGE;
		m_cSelectionTreeList.SetExtendedStyle(uExStyle);
	}

	m_wndIssueDateReport.SetHeadings( _T("Date , 150;Count , 50") );
	m_wndIssueDateReport.SetCheckboxes( TRUE );
	m_wndIssueDateReport.EnableWindow(FALSE);
	m_wndIssueDateReport.SetImageList(&m_imgList , LVSIL_SMALL);
	m_wndIssueDateReport.SetHeaderImageList();
	
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_REVISION);
	if( pComboBox )
	{
		pComboBox->EnableWindow( FALSE );
		int nItem = pComboBox->AddString( _T("LATEST") );
		pComboBox->SetItemData(nItem , -1);
		nItem = pComboBox->AddString( _T("ALL") );
		pComboBox->SetItemData(nItem , -1);
		pComboBox->SetCurSel( 0 );
	}
	
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_STATUS);
	if( pComboBox )
	{
		pComboBox->SetCurSel(0);
	}

	CString strTooltip = _T("Load line data with selected condition");
	m_btnSelect.SetTooltip( strTooltip );
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/**
	@brief	when user click SelectAll button

	@author	humkyung
*/
void CLineListSelectOption1::OnButtonSelectAll() 
{
	NMHDR hdr;
	hdr.hwndFrom = GetSafeHwnd();
	hdr.idFrom = GetDlgCtrlID();

	if(NULL != m_pTargetWnd)
	{
		m_pTargetWnd->SendMessage(WM_NOTIFY , (WPARAM)SELECT_ALL , (LPARAM)&hdr);
	}
	
	CEditLineDataForm* pLineListForm = (CEditLineDataForm*)(GetParent());
}

/**	
	@brief	ISSUE DATE의 선택에 따라 issue data report와 revision option을 on/off 시킨다.

	@author	humkyung

	@return	void	
*/
void CLineListSelectOption1::OnCheckIssueDate() 
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_ISSUE_DATE);
	if(pButton->GetCheck())
	{
		m_wndIssueDateReport.EnableWindow(TRUE);
	}
	else
	{
		m_wndIssueDateReport.EnableWindow(FALSE);
	}

	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_REVISION);
	(pButton->GetCheck()) ? pComboBox->EnableWindow( TRUE ) : pComboBox->EnableWindow( FALSE );
}

/**	
	@brief	프로젝트가 로딩되고 난뒤 폼을 초기화 시킨다.

	@author	humkyung

	@return	void
*/
void CLineListSelectOption1::InitContents( CWnd* pTargetWnd )
{
	m_pTargetWnd = pTargetWnd;
	
	//////////////////////////////////////////////////////////////////////////
	UpdateSelectionTreeList(false , this);
	UpdateRevNoComboBox(false , this);

	CAppDocData* pDocData = CAppDocData::GetInstance();
	if(pDocData && pDocData->m_pIssueDateRecordSet)
	{
		const int nCount = pDocData->m_pIssueDateRecordSet->GetCount();
		for(int i = 0;i < nCount;i++)
		{
			CIssueDateRecord* pRecord = pDocData->m_pIssueDateRecordSet->GetAt(i);
			if(m_wndIssueDateReport.GetSafeHwnd())
			{
				const int nItem = m_wndIssueDateReport.InsertItem(i , pRecord->dateString() , 8);
			}
		}

		/// display count of drawings of issue date - 2012.08.10 added by humkyung
		const CString rProjectPath = pDocData->GetProjectPath().c_str();
		const CString sIniFilePath= pDocData->GetProject()->GetIniFilePath();
		const CProject::DatabaseProp prop = pDocData->GetProject()->GetDatabaseProp();

		try
		{
			CAppPostgreSQL database(pDocData->GetProject()->name(),prop.Host.c_str(),prop.User.c_str(),prop.Password.c_str());
			STRING_T sSql = _T("SELECT DISTINCT REV_DATE,KEY FROM T_ISO_REVISION UNION SELECT DISTINCT REV_DATE,KEY FROM T_ISSUED_ISO_REVISION");
			soci::rowset<soci::row> rs(database.session()->prepare << sSql);
			map<STRING_T,int> oCountOfDrawingOfIssueDate;
			for(soci::rowset<soci::row>::const_iterator itr = rs.begin();itr != rs.end();++itr)
			{
				STRING_T sValue;
				sValue = itr->get<STRING_T>(_T("rev_date"));
				if(oCountOfDrawingOfIssueDate.end() == oCountOfDrawingOfIssueDate.find(sValue))
				{
					oCountOfDrawingOfIssueDate.insert(make_pair(sValue , 1));
				}
				else
				{
					oCountOfDrawingOfIssueDate[sValue]++;
				}
			}

			for(int i = 0;i < this->m_wndIssueDateReport.GetItemCount();++i)
			{
				CString sName = this->m_wndIssueDateReport.GetItemText(i , 0);
				map<STRING_T,int>::iterator where = oCountOfDrawingOfIssueDate.find(sName.operator LPCTSTR());
				if(oCountOfDrawingOfIssueDate.end() != where)
				{
					CString str;
					str.Format(_T("%d") , where->second);
					this->m_wndIssueDateReport.SetItemText(i , 1 , str);
				}
			}
		}
		catch(const std::exception& ex)
		{
			AfxMessageBox(ex.what(),MB_OK|MB_ICONERROR);
		}
	}

	this->m_cSelectionTreeList.CheckAll();	/// 2012.09.24 added by humkyung
}

/******************************************************************************
    @author     humkyung
    @date       2011-10-29
    @class      CLineListSelectOption1
    @function   OnSize
    @return     void
    @param      UINT    nType
    @param      int     cx
    @param      int     cy
    @brief
******************************************************************************/
void CLineListSelectOption1::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CRect rect , rectTreeList;
	if(m_cSelectionTreeList.GetSafeHwnd())
	{
		m_cSelectionTreeList.GetWindowRect(&rect);
		ScreenToClient(&rect);
		m_cSelectionTreeList.SetWindowPos(NULL , rect.left , rect.top , cx - rect.left - 2 , int(cy*0.5) , SWP_NOACTIVATE);
		m_cSelectionTreeList.GetWindowRect(&rectTreeList);
		ScreenToClient(&rectTreeList);
	}

	CWnd* pWnd = GetDlgItem(IDC_CHECK_ISSUE_DATE);
	if((NULL != pWnd) && pWnd->GetSafeHwnd())
	{
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);
		pWnd->SetWindowPos(NULL , rect.left , rectTreeList.bottom + 5 , 0 , 0 , SWP_NOSIZE | SWP_NOACTIVATE);
	}

	CRect rectBase;
	if(m_btnSelect.GetSafeHwnd())
	{
		CRect rect;
		m_btnSelect.GetWindowRect(&rect);
		ScreenToClient(&rect);

		m_btnSelect.SetWindowPos(NULL , int(cx*0.5) + 2 , cy - rect.Height() - 4 , int(cx*0.5) - 4 , rect.Height() , SWP_NOACTIVATE);
		m_btnSelect.GetWindowRect(&rectBase);
		ScreenToClient(&rectBase);
	}
	
	pWnd = GetDlgItem(IDC_COMBO_STATUS);
	if((NULL != pWnd) && pWnd->GetSafeHwnd())
	{
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);

		pWnd->SetWindowPos(NULL , rect.left , rectBase.top - rect.Height() - 10 , cx - rect.left - 2 , rect.Height() , SWP_NOACTIVATE);
		pWnd->GetWindowRect(&rectBase);
		ScreenToClient(&rectBase);
	}

	pWnd = GetDlgItem(IDC_STATIC_STATUS);
	if((NULL != pWnd) && pWnd->GetSafeHwnd())
	{
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);

		pWnd->SetWindowPos(NULL , rect.left , rectBase.top - rect.Height() - 10 , 0 , 0 , SWP_NOSIZE | SWP_NOACTIVATE);
		pWnd->GetWindowRect(&rectBase);
		ScreenToClient(&rectBase);
	}

	pWnd = GetDlgItem(IDC_COMBO_REVISION);
	if((NULL != pWnd) && pWnd->GetSafeHwnd())
	{
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);

		pWnd->SetWindowPos(NULL , rect.left , rectBase.top - rect.Height() - 10 , cx - rect.left - 2 , rect.Height() , SWP_NOACTIVATE);
		pWnd->GetWindowRect(&rectBase);
		ScreenToClient(&rectBase);
	}

	pWnd = GetDlgItem(IDC_STATIC_REVISION_OPTION);
	if((NULL != pWnd) && pWnd->GetSafeHwnd())
	{
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);

		pWnd->SetWindowPos(NULL , rect.left , rectBase.top - rect.Height() - 10 , 0 , 0 , SWP_NOSIZE | SWP_NOACTIVATE);
		pWnd->GetWindowRect(&rectBase);
		ScreenToClient(&rectBase);
	}

	pWnd = GetDlgItem(IDC_LIST_ISSUED_DATE);
	if((NULL != pWnd) && pWnd->GetSafeHwnd())
	{
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);

		pWnd->SetWindowPos(NULL , rect.left , rectTreeList.bottom + 25 , cx - rect.left - 2 , rectBase.top - rectTreeList.bottom - 30 , SWP_NOACTIVATE);
	}
}

void CLineListSelectOption1::OnDestroy()
{
	CDialog::OnDestroy();
}

/**	
	@brief	쿼리 조건문을 생성한 뒤 타겟 윈도우로 메세지를 보낸다.
	@author	humkyung
	@return	void
*/
void CLineListSelectOption1::OnButtonSelect() 
{
	m_rQueryCondition.Empty();
	
	m_bSelectBoutton = TRUE;
	if( TRUE == m_bSelectBoutton )
	{
		if(false == CheckIssueDateToQuery())
		{
			CString rString;
			rString.LoadString(IDS_NEED_ISSUE_DATE_TO_QUERY);
			AfxMessageBox(rString);
			return;
		}

		if(false == this->AreAllItemsChecked(m_cSelectionTreeList.GetRootItem()))
		{
			vector< STRING_T > oPlantNos , oAreaNos , oUnitNos , oSectionNos;
			GetSelPlantNos( oPlantNos );
			if( !oPlantNos.empty() )
			{
				m_rQueryCondition.Empty();
				for( vector< STRING_T >::const_iterator itr = oPlantNos.begin();itr != oPlantNos.end();++itr)
				{
					GetSelAreaNos( oAreaNos , *itr );
					for(vector<STRING_T>::const_iterator jtr = oAreaNos.begin();jtr != oAreaNos.end();++jtr)
					{
						GetSelUnitNos( oUnitNos , *itr , *jtr);
						for(vector<STRING_T>::const_iterator ktr = oUnitNos.begin();ktr != oUnitNos.end();++ktr)
						{
							CString sQueryCondition;
							if(!itr->empty())
							{
								sQueryCondition += _T("PLANT_NO = '") + CString(itr->c_str()) + _T("'");
							}
							else
							{
								sQueryCondition += _T("(PLANT_NO = '' OR PLANT_NO is null)");
							}
							sQueryCondition += _T(" AND ");
							if(!jtr->empty())
							{
								sQueryCondition += _T("AREA_NO='") + CString(jtr->c_str()) + _T("'");
							}
							else
							{
								sQueryCondition += _T("(AREA_NO='' OR AREA_NO is null)");
							}
							sQueryCondition += _T(" AND ");
							if(!ktr->empty())
							{
								sQueryCondition += _T("UNIT_NO='") + CString(ktr->c_str()) + _T("'");
							}
							else
							{
								sQueryCondition += _T("(UNIT_NO='' OR UNIT_NO is null)");
							}
							GetSelSectionNos( oSectionNos , *itr , *jtr , *ktr );
							if(!oSectionNos.empty())
							{
								bool bFoundNull = false;
								CString sSections;
								for( vector< STRING_T >::iterator ltr = oSectionNos.begin();ltr != oSectionNos.end();++ltr)
								{
									if(ltr->empty()) bFoundNull = true;
									sSections += _T("'") + CString(ltr->c_str()) + _T("',");
								}
								if(_T(",") == sSections.Right(1)) sSections = sSections.Left( sSections.GetLength() - 1 );

								if(false == bFoundNull)
								{
									sQueryCondition += _T(" AND SECTION_NO IN (") + sSections + _T(")");
								}
								else
								{
									sQueryCondition += _T(" AND (SECTION_NO IN (") + sSections + _T(") OR SECTION_NO is null)");
								}

								if(!m_rQueryCondition.IsEmpty()) m_rQueryCondition += _T(" OR ");
								m_rQueryCondition += _T("(") + sQueryCondition + _T(")");
							}
						}
					}
				}
			}
			else
			{
				AfxMessageBox(_T("There is no condition to query!!!") , MB_ICONEXCLAMATION);
				return;
			}
		}
		else
		{
			m_rQueryCondition.Empty();
		}

		/// add status condition to query string - 2012.08.11 added by humkyung
		CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_STATUS);
		if(NULL != pComboBox)
		{
			CString rString;
			pComboBox->GetWindowText(rString);
			if(_T("ALL") != rString)
			{
				if(!m_rQueryCondition.IsEmpty())
				{
					m_rQueryCondition += _T(" AND ");
				}
				m_rQueryCondition += _T("STATUS='") + rString + _T("'");
			}
		}
		/// up to here

		/// add site condition to query string - 2012.08.13 added by humkyung
		CAppDocData* pDocData = CAppDocData::GetInstance();
		if(_T("ALL") != pDocData->m_rSiteName)
		{
			if(!m_rQueryCondition.IsEmpty())
			{
				m_rQueryCondition += _T(" AND ");
			}
			m_rQueryCondition += _T("SITE='") + pDocData->m_rSiteName + _T("'");
		}
		/// up to here

		if(!m_rQueryCondition.IsEmpty()) m_rQueryCondition = _T("WHERE (") + m_rQueryCondition + _T(")");
		
		/// send message to target window
		NMHDR hdr;
		hdr.hwndFrom = GetSafeHwnd();
		hdr.idFrom = GetDlgCtrlID();
		if(NULL != m_pTargetWnd)
		{
			m_pTargetWnd->SendMessage(WM_NOTIFY , (WPARAM)SELECT , (LPARAM)&hdr);
		}
	}
}

/**	
	@brief	쿼리 조건을 구한다.
	@author	humkyung
	@return	CString	
*/
CString CLineListSelectOption1::GetQueryCondition() const
{
	return m_rQueryCondition;
}

/**	
	@brief	사용자가 선택한 issue date를 구한다.

	@author	HumKyung.BAEK

	@return	const char*	
*/
void CLineListSelectOption1::GetSelectedIssueDate( vector< STRING_T >& issueDates )
{
	issueDates.clear();

	CString rString;
	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_ISSUE_DATE);
	if(pButton->GetCheck())
	{
		const int nItemCount = m_wndIssueDateReport.GetItemCount();
		for( int i = 0;i < nItemCount;++i)
		{
			if( m_wndIssueDateReport.GetCheck( i ) )
			{
				rString = m_wndIssueDateReport.GetItemText(i , 0);
				issueDates.push_back( rString.operator LPCTSTR() );
			}
		}
	}
}

/**	
	@brief	사용자가 선택한 rev no를 구한다.
	
	@author	humkyung
	
	@return	"" - 선택한 REV NO가 없다.
*/
CString CLineListSelectOption1::GetSelectedRevNo() const
{
	CString rString("");
	
	CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_ISSUE_DATE);
	if(pButton->GetCheck())
	{
		CComboBox* pComboBox = (CComboBox*)GetDlgItem( IDC_COMBO_REVISION );
		if( pComboBox )
		{
			const int nItem = pComboBox->GetCurSel();
			if(-1 != nItem)
			{
				DWORD nItemData = pComboBox->GetItemData(nItem);
				if(-1 == nItemData)
				{
					/// LATEST or ALL
					pComboBox->GetWindowText( rString );
				}
				else
				{
					rString.Format(_T("%d") , nItemData);
				}
			}
		}

		return rString;
	}

	return rString;
}

/**	
	@brief	selection tree list를 갱신한다.
	@author	humkyung
	@remarks	area no , unit no , section no 내용들을 초기화 시킨다.
	@return	void	
*/
void CLineListSelectOption1::OnUpdateSelectionTreeList(bool bReload)
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	CProject* pPrj = pDocData->GetProject();
	CProject::DatabaseProp prop = pPrj->GetDatabaseProp();

	try
	{
		CAppPostgreSQL database(pPrj->name() , prop.Host.c_str() , prop.User.c_str() , prop.Password.c_str());
		m_cSelectionTreeList.DeleteAllItems();
		m_cSelectionTreeList.InsertItem(_T("All"));
		HTREEITEM hRoot = m_cSelectionTreeList.GetRootItem();
		map<STRING_T , HTREEITEM> oPlantTreeItemMap;

		CString sSql = _T("SELECT DISTINCT PLANT_NO,AREA_NO,UNIT_NO,SECTION_NO FROM T_ISO_LINE_DATA");
		soci::rowset<soci::row> rs(database.session()->prepare << (sSql.operator LPCTSTR()));
		for(soci::rowset<soci::row>::const_iterator itr = rs.begin();itr != rs.end();++itr)
		{
			STRING_T rValue;
			HTREEITEM hPlant = NULL;
			rValue = (soci::indicator::i_ok == itr->get_indicator(_T("plant_no"))) ? rValue = itr->get<STRING_T>(_T("plant_no")) : _T("");
			{
				if(oPlantTreeItemMap.end() == oPlantTreeItemMap.find(rValue))
				{
					hPlant = m_cSelectionTreeList.InsertItem(rValue.c_str() , 1 , 1 , hRoot , TVI_LAST);
					oPlantTreeItemMap.insert(make_pair(rValue , hPlant));
					m_cSelectionTreeList.Expand(hPlant , TVE_EXPAND);
				}
				else
				{
					hPlant = oPlantTreeItemMap[rValue];
				}
			}

			HTREEITEM hArea = NULL;
			rValue = (soci::indicator::i_ok == itr->get_indicator(_T("area_no"))) ? itr->get<STRING_T>(_T("area_no")) : _T("");
			{
				HTREEITEM hChild = m_cSelectionTreeList.GetChildItem(hPlant);
				while(NULL != hChild)
				{
					if(rValue == STRING_T(m_cSelectionTreeList.GetItemText(hChild)))
					{
						hArea = hChild;
						break;
					}
					hChild = m_cSelectionTreeList.GetNextSiblingItem(hChild);
				}
				if(NULL == hArea)
				{
					hArea = m_cSelectionTreeList.InsertItem(rValue.c_str() , 2 , 2 , hPlant , TVI_LAST);
					m_cSelectionTreeList.Expand(hArea , TVE_EXPAND);
				}
			}

			HTREEITEM hUnit = NULL;
			rValue = (soci::indicator::i_ok == itr->get_indicator(_T("unit_no"))) ? itr->get<STRING_T>(_T("unit_no")) : _T("");
			{
				HTREEITEM hChild = m_cSelectionTreeList.GetChildItem(hArea);
				while(NULL != hChild)
				{
					if(rValue == STRING_T(m_cSelectionTreeList.GetItemText(hChild)))
					{
						hUnit = hChild;
						break;
					}
					hChild = m_cSelectionTreeList.GetNextSiblingItem(hChild);
				}
				if(NULL == hUnit)
				{
					hUnit = m_cSelectionTreeList.InsertItem(rValue.c_str() , 3 , 3 , hArea , TVI_LAST);
					m_cSelectionTreeList.Expand(hUnit , TVE_EXPAND);
				}
			}

			HTREEITEM hSection = NULL;
			rValue = (soci::indicator::i_ok == itr->get_indicator(_T("section_no"))) ? itr->get<STRING_T>(_T("section_no")) : _T("");
			{
				HTREEITEM hChild = m_cSelectionTreeList.GetChildItem(hUnit);
				while(NULL != hChild)
				{
					if(rValue == STRING_T(m_cSelectionTreeList.GetItemText(hChild)))
					{
						hSection = hChild;
						break;
					}
					hChild = m_cSelectionTreeList.GetNextSiblingItem(hChild);
				}
				if(NULL == hSection)
				{
					hSection = m_cSelectionTreeList.InsertItem(rValue.c_str() , 4 , 4 , hUnit , TVI_LAST);
					m_cSelectionTreeList.Expand(hSection , TVE_EXPAND);
				}
			}
		}

		m_cSelectionTreeList.Expand(hRoot , TVE_EXPAND);
	}
	catch(const std::exception& ex)
	{
		UNUSED_ALWAYS(ex);
	}
}

/******************************************************************************
    @author     humkyung
    @date       2012-04-05
    @class      CLineListSelectOption1
    @function   AreAllItemsChecked
    @return     bool
    @param      HTREEITEM   hItem
    @brief
******************************************************************************/
bool CLineListSelectOption1::AreAllItemsChecked(HTREEITEM hItem)
{
	if(FALSE == m_cSelectionTreeList.GetCheck(hItem)) return false;
	HTREEITEM hChild = m_cSelectionTreeList.GetChildItem(hItem);
	while(hChild)
	{
		if(FALSE == m_cSelectionTreeList.GetCheck(hChild)) return false;
		if(false == this->AreAllItemsChecked(hChild)) return false;
		hChild = m_cSelectionTreeList.GetNextSiblingItem(hChild);
	}

	return true;
}

/******************************************************************************
    @author     humkyung
    @date       2012-04-05
    @class      CLineListSelectOption1
    @function   GetCheckedItem
    @return     int
    @param      vector<     STRING_T
    @param      >&          oCheckedList
    @param      HTREEITEM   hItem
    @brief
******************************************************************************/
int CLineListSelectOption1::GetCheckedItemList( vector< STRING_T >& oCheckedList , HTREEITEM hItem)
{
	assert(hItem && "hItem is NULL");

	if(hItem)
	{
		while(hItem)
		{
			if(m_cSelectionTreeList.GetCheck(hItem))
			{
				STRING_T sItemText(m_cSelectionTreeList.GetItemText(hItem));
				oCheckedList.push_back(sItemText);
			}

			hItem = m_cSelectionTreeList.GetNextSiblingItem(hItem);
		}

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

/**	
	@brief	사용자가 선택한 plant no들을 구한다.

	@author	HumKyung.BAEK

	@param	plantNos	a parameter of type vector< string >

	@return	void	
*/
void CLineListSelectOption1::GetSelPlantNos( vector< STRING_T >& oPlantNos )
{
	oPlantNos.clear();
	
	HTREEITEM hRoot = m_cSelectionTreeList.GetRootItem();
	HTREEITEM hItem = m_cSelectionTreeList.GetChildItem(hRoot);
	this->GetCheckedItemList( oPlantNos , hItem );
}

/**	
	@brief	사용자가 선택한 area no들을 구한다.

	@author	humkyung

	@param	areaNos	a parameter of type vector< string >&

	@return	void	
*/
void CLineListSelectOption1::GetSelAreaNos( vector< STRING_T >& oAreaNos , const STRING_T& sPlantNo)
{
	oAreaNos.clear();

	HTREEITEM hRoot = m_cSelectionTreeList.GetRootItem();
	HTREEITEM hItem = m_cSelectionTreeList.GetChildItem(hRoot);
	while(hItem)
	{
		if((sPlantNo == m_cSelectionTreeList.GetItemText(hItem)) && m_cSelectionTreeList.GetCheck(hItem))
		{
			HTREEITEM hChild = m_cSelectionTreeList.GetChildItem(hItem);
			this->GetCheckedItemList( oAreaNos , hChild );
		}
		hItem = m_cSelectionTreeList.GetNextSiblingItem(hItem);
	}
}

/**	
	@brief	사용자가 선택한 unit no들을 구한다.

	@author	humkyung

	@param	areaNos	a parameter of type vector< string >&

	@return	void	
*/
void CLineListSelectOption1::GetSelUnitNos( vector< STRING_T >& oUnitNos , const STRING_T& sPlantNo , const STRING_T& sAreaNo )
{
	oUnitNos.clear();

	HTREEITEM hRoot = m_cSelectionTreeList.GetRootItem();
	HTREEITEM hItem = m_cSelectionTreeList.GetChildItem(hRoot);
	while(hItem)
	{
		if((sPlantNo == m_cSelectionTreeList.GetItemText(hItem)) && m_cSelectionTreeList.GetCheck(hItem))
		{
			HTREEITEM hChild = m_cSelectionTreeList.GetChildItem(hItem);
			while(hChild)
			{
				if((sAreaNo == m_cSelectionTreeList.GetItemText(hChild)) && m_cSelectionTreeList.GetCheck(hChild))
				{
					HTREEITEM hUnitItem = m_cSelectionTreeList.GetChildItem(hChild);
					this->GetCheckedItemList(oUnitNos , hUnitItem);
				}
				hChild = m_cSelectionTreeList.GetNextSiblingItem(hChild);
			}

		}
		hItem = m_cSelectionTreeList.GetNextSiblingItem(hItem);
	}
}

/**	
	@brief	사용자가 선택한 section no들을 구한다.

	@author	humkyung

	@param	areaNos	a parameter of type vector< string >&

	@return	void	
*/
void CLineListSelectOption1::GetSelSectionNos( vector< STRING_T >& oSectionNos , const STRING_T& sPlantNo , const STRING_T& sAreaNo , const STRING_T& sUnitNo )
{
	oSectionNos.clear();
	
	HTREEITEM hRoot = m_cSelectionTreeList.GetRootItem();
	HTREEITEM hItem = m_cSelectionTreeList.GetChildItem(hRoot);
	while(hItem)
	{
		if((sPlantNo == m_cSelectionTreeList.GetItemText(hItem)) && m_cSelectionTreeList.GetCheck(hItem))
		{
			HTREEITEM hChild = m_cSelectionTreeList.GetChildItem(hItem);
			while(hChild)
			{
				if((sAreaNo == m_cSelectionTreeList.GetItemText(hChild)) && m_cSelectionTreeList.GetCheck(hChild))
				{
					HTREEITEM hUnitItem = m_cSelectionTreeList.GetChildItem(hChild);
					while(hUnitItem)
					{
						if((sUnitNo == m_cSelectionTreeList.GetItemText(hUnitItem)) && m_cSelectionTreeList.GetCheck(hUnitItem))
						{
							HTREEITEM hSectionItem = m_cSelectionTreeList.GetChildItem(hUnitItem);
							this->GetCheckedItemList(oSectionNos , hSectionItem);
						}
						
						hUnitItem = m_cSelectionTreeList.GetNextSiblingItem(hUnitItem);
					}
				}
				hChild = m_cSelectionTreeList.GetNextSiblingItem(hChild);
			}

		}
		hItem = m_cSelectionTreeList.GetNextSiblingItem(hItem);
	}
}

/**	
	@brief	The CLineListSelectOption1::UpdateRevNoComboBox function
	@author	humkyung
	@return	void	
*/
void CLineListSelectOption1::OnUpdateRevNoComboBox(bool bReload)
{
	static list< STRING_T > oRevNoList;

	CAppDocData* pDocData = CAppDocData::GetInstance();
	if(oRevNoList.empty() || (true == bReload))
	{
		pDocData->GetProject()->CollectAllRevNo( oRevNoList );
	}
	
	CComboBox* pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_REVISION);
	if( pComboBox )
	{
		int nCount = pComboBox->GetCount();
		for( int i = nCount - 1;i >= 1;--i)
		{
			pComboBox->DeleteString( i );
		}

		for( list< STRING_T >::iterator itr = oRevNoList.begin();itr != oRevNoList.end();++itr)
		{
			RevisionStage revisionStage;
			if(ERROR_SUCCESS == pDocData->GetRevisionStage(revisionStage , ATOI_T(itr->c_str())))
			{
				CString rRevNo = STRING_T(revisionStage.rDefine + revisionStage.rText).c_str();
				const int nItem = pComboBox->AddString( rRevNo );
				if(-1 != nItem)
				{
					pComboBox->SetItemData(nItem , DWORD(atoi(itr->c_str())));
				}
			}
		}
	}
}

/**
	@brief	

	@author	humkyung
**/
void CLineListSelectOption1::SetEnableSelectButton()
{
	if( m_bPlantListBox == TRUE && m_bAreaListBox == TRUE && m_bUnitListBox == TRUE && m_bSectionListBox == TRUE )
	{
		m_btnSelect.EnableWindow(TRUE);
		m_bSelectBoutton = TRUE;
		m_btnSelect.RedrawWindow();
	}
	else
	{
		m_btnSelect.EnableWindow(FALSE);
		m_bSelectBoutton = TRUE;
		m_btnSelect.RedrawWindow();
	}
}

/**
	@brief	

	@author	HumKyung.BAEK
**/
CString CLineListSelectOption1::GetInAreaQueryCondition() const
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	CString rArea = pDocData->m_ProjectArea;
	CString rQueryCondition = _T("WHERE (AREA_NO IN ('") + rArea + _T("'))");
	rArea.MakeUpper();
	if(rArea.IsEmpty() || (rArea == _T("ALL"))) rQueryCondition.Empty();;

	return rQueryCondition;
}

/**	
	@brief	쿼리할 이슈 날짜를 선택했는지 확인한다.

	@author	humkyung
**/
bool CLineListSelectOption1::CheckIssueDateToQuery() const
{
	CButton* pCheckIssueDate = (CButton*)GetDlgItem(IDC_CHECK_ISSUE_DATE);
	if(pCheckIssueDate)
	{
		const BOOL bCheck = pCheckIssueDate->GetCheck(); 
		if(TRUE == bCheck)
		{
			UINT nCheckItemCount = 0;
			const int nItemCount = m_wndIssueDateReport.GetItemCount();
			for( int i = 0;i < nItemCount;++i)
			{
				if( m_wndIssueDateReport.GetCheck( i ) ) ++nCheckItemCount;
			}

			int nSelItem = -1;
			CComboBox* pRevNoComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_REVISION);
			if(pRevNoComboBox) nSelItem = pRevNoComboBox->GetCurSel();

			return ((0 != nCheckItemCount) && (-1 != nSelItem));
		}else	return true;
	}

	return false;
}

/**
	@brief	

	@author	humkyung
**/
void CLineListSelectOption1::UpdateSelectionTreeList(bool bReload , CLineListSelectOption1* pTarget)
{
	if(NULL == pTarget)
	{
		for(vector<CLineListSelectOption1*>::iterator itr = __SIBLINGS__.begin();itr != __SIBLINGS__.end();++itr)
		{
			(*itr)->OnUpdateSelectionTreeList(bReload);
			(*itr)->m_cSelectionTreeList.CheckAll();	/// 2013.01.02 added by humkyung
		}
	}
	else
	{
		pTarget->OnUpdateSelectionTreeList(bReload);
		pTarget->m_cSelectionTreeList.CheckAll();	/// 2013.01.02 added by humkyung
	}
}

/**
	@brief	call OnUpdateRevNoComboBox

	@author	humkyung
**/
void CLineListSelectOption1::UpdateRevNoComboBox(bool bReload , CLineListSelectOption1* pTarget)
{
	if(NULL == pTarget)
	{
		for(vector<CLineListSelectOption1*>::iterator itr = __SIBLINGS__.begin();itr != __SIBLINGS__.end();++itr)
		{
			(*itr)->OnUpdateRevNoComboBox(bReload);
		}
	}
	else
	{
		pTarget->OnUpdateRevNoComboBox(bReload);
	}
}
