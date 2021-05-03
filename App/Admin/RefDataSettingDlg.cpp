// RefDataSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include <ProjectData.h>
#include "admin.h"
#include "RefDataSettingDlg.h"
#include "AdminDocData.h"
#include "StandardNoteTableFile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRefDataSettingDlg dialog


CRefDataSettingDlg::CRefDataSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRefDataSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRefDataSettingDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CRefDataSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRefDataSettingDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRefDataSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CRefDataSettingDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_WM_HELPINFO()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRefDataSettingDlg message handlers

/**
	@brief	내용을 저장한다.
	@author	humkyung
**/
void CRefDataSettingDlg::Save()
{
	static const STRING_T rApp("Iso Edit Setting");
	
	CAdminDocData* pDocData = CAdminDocData::GetInstance();
	CString rIniFilePath = pDocData->GetIniFilePath();
	map<STRING_T,STRING_T>* pDrawingSetup = CProjectData::GetInstance()->GetDrawingSetup();
	
	CString rString;
	map<STRING_T , CMFCPropertyGridProperty*>::iterator where = m_oGridPropertyMap.find(_T("Spec File Folder"));
	rString = (m_oGridPropertyMap.end() != where) ? where->second->GetValue() : _T("");
	(*pDrawingSetup)[_T("Spec File Folder")] = rString.operator LPCTSTR();
	
	where = m_oGridPropertyMap.find(_T("Standard Note File"));
	rString = (m_oGridPropertyMap.end() != where) ? where->second->GetValue() : _T("");
	(*pDrawingSetup)[_T("Standard Note File")] = rString.operator LPCTSTR();

	where = m_oGridPropertyMap.find(_T("UNIT"));
	rString = (m_oGridPropertyMap.end() != where) ? where->second->GetValue() : _T("");
	(*pDrawingSetup)[_T("Unit")] = rString.operator LPCTSTR();

	where = m_oGridPropertyMap.find(_T("Drawing Type"));
	rString = (m_oGridPropertyMap.end() != where) ? where->second->GetValue() : _T("");
	CStandardNoteTableFile& stdnote = CStandardNoteTableFile::GetInstance();
	CStandardNoteTableFile::CStandardNoteType* pNoteType = stdnote.GetStandardNoteType(2000);
	if(pNoteType)
	{
		const int nItemCount = pNoteType->GetItemCount();
		for(int i = 0;i < nItemCount;++i)
		{
			if(rString == CString(pNoteType->GetItemAt(i)->rNumString.c_str()))
			{
				rString.Format(_T("%d") , pNoteType->GetItemAt(i)->nNum);
				(*pDrawingSetup)[_T("Drawing Type")] = rString.operator LPCTSTR();
			}
		}
	}

	/// 2009.03.11에 추가.
	CProjectData* pProjectData = CProjectData::GetInstance();
	if(NULL != pProjectData)
	{
		map<CString,CString>::iterator where1 = pProjectData->m_pProjectSettingMap->find(_T("WORKING_UNIT"));
		if(where1 != pProjectData->m_pProjectSettingMap->end())
		{
			where = m_oGridPropertyMap.find(_T("Working Unit"));
			rString = (m_oGridPropertyMap.end() != where) ? where->second->GetValue() : _T("");
			where1->second = rString;
		}

		where1 = pProjectData->m_pProjectSettingMap->find(_T("dvcs_e"));
		if(where1 != pProjectData->m_pProjectSettingMap->end())
		{
			where = m_oGridPropertyMap.find(_T("E"));
			rString = (m_oGridPropertyMap.end() != where) ? where->second->GetValue() : _T("");
			where1->second = rString;
		}

		where1 = pProjectData->m_pProjectSettingMap->find(_T("dvcs_n"));
		if(where1 != pProjectData->m_pProjectSettingMap->end())
		{
			where = m_oGridPropertyMap.find(_T("N"));
			rString = (m_oGridPropertyMap.end() != where) ? where->second->GetValue() : _T("");
			where1->second = rString;
		}

		where1 = pProjectData->m_pProjectSettingMap->find(_T("dvcs_el"));
		if(where1 != pProjectData->m_pProjectSettingMap->end())
		{
			where = m_oGridPropertyMap.find(_T("EL"));
			rString = (m_oGridPropertyMap.end() != where) ? where->second->GetValue() : _T("");
			where1->second = rString;
		}

		/// 2011.09.21 - added by humkyung
		where1 = pProjectData->m_pProjectSettingMap->find(_T("get_plan_dwg"));
		if(where1 != pProjectData->m_pProjectSettingMap->end())
		{
			rString = _T("OFF");
			where = m_oGridPropertyMap.find(_T("Get Plan Dwg"));
			rString = (m_oGridPropertyMap.end() != where) ? where->second->GetValue() : _T("");
			where1->second = rString;
		}
		/// up to here

		/// 2013.06.03 - added by humkyung
		where1 = pProjectData->m_pProjectSettingMap->find(_T("use_plan_dwg_by"));
		if(where1 != pProjectData->m_pProjectSettingMap->end())
		{
			rString = _T("DRAWING_NO");
			where = m_oGridPropertyMap.find(_T("Use Plan Dwg By"));
			rString = (m_oGridPropertyMap.end() != where) ? where->second->GetValue() : _T("");
			where1->second = rString;
		}
		/// up to here
	}
}

/**
	@brief	현재 설정된 내용을 화면에 표시한다.
	@author	humkyung
**/
void CRefDataSettingDlg::UpdateContents()
{
	static bool initialized = false;
	static const STRING_T rApp( _T("Iso Edit Setting") );

	if(false == initialized)
	{
		CAdminDocData* pDocData = CAdminDocData::GetInstance();
		CString rIniFilePath = pDocData->GetIniFilePath();
		
		CMFCPropertyGridProperty* pGridProperty = NULL;
		map<STRING_T , CMFCPropertyGridProperty*>::iterator where = m_oGridPropertyMap.find(_T("Spec File Folder"));
		pGridProperty = (m_oGridPropertyMap.end() != where) ? where->second : NULL;

		TCHAR szBuf[MAX_PATH + 1] = {'0',};
		map<STRING_T,STRING_T>* pDrawingSetup = CProjectData::GetInstance()->GetDrawingSetup();
		map<STRING_T,STRING_T>::const_iterator ctr = pDrawingSetup->find(_T("Spec File Folder"));
		if(ctr != pDrawingSetup->end())
		{
			if((NULL != pGridProperty)) pGridProperty->SetValue(ctr->second.c_str());
		}

		ctr = pDrawingSetup->find(_T("Standard Note File"));
		if(ctr != pDrawingSetup->end())
		{
			map<STRING_T , CMFCPropertyGridProperty*>::iterator where = m_oGridPropertyMap.find(_T("Standard Note File"));
			pGridProperty = (m_oGridPropertyMap.end() != where) ? where->second : NULL;
			if((NULL != pGridProperty)) pGridProperty->SetValue(ctr->second.c_str());
			
			CStandardNoteTableFile& stdnote = CStandardNoteTableFile::GetInstance();
			stdnote.Read(szBuf);
		}
		
		ctr = pDrawingSetup->find(_T("Unit"));
		if(ctr != pDrawingSetup->end())
		{
			map<STRING_T , CMFCPropertyGridProperty*>::iterator where = m_oGridPropertyMap.find(_T("UNIT"));
			pGridProperty = (m_oGridPropertyMap.end() != where) ? where->second : NULL;
			if((NULL != pGridProperty)) pGridProperty->SetValue(ctr->second.c_str());
		}
		else
		{
			map<STRING_T , CMFCPropertyGridProperty*>::iterator where = m_oGridPropertyMap.find(_T("UNIT"));
			pGridProperty = (m_oGridPropertyMap.end() != where) ? where->second : NULL;
			if((NULL != pGridProperty)) pGridProperty->SetValue(_T("INCH"));
		}
		
		{
			map<STRING_T , CMFCPropertyGridProperty*>::iterator where = m_oGridPropertyMap.find(_T("Drawing Type"));
			pGridProperty = (m_oGridPropertyMap.end() != where) ? where->second : NULL;
			if(NULL != pGridProperty) pGridProperty->RemoveAllOptions();
			
			CStandardNoteTableFile& stdnote = CStandardNoteTableFile::GetInstance();
			CStandardNoteTableFile::CStandardNoteType* pNoteType = stdnote.GetStandardNoteType(2000);
			if(pNoteType)
			{
				vector<STRING_T> oNoteTypeList;
				const int nItemCount = pNoteType->GetItemCount();
				for(int i = 0;i < nItemCount;++i)
				{
					oNoteTypeList.push_back( pNoteType->GetItemAt(i)->rNumString.c_str() );					
				}

				::stable_sort(oNoteTypeList.begin() , oNoteTypeList.end());
				for(vector<STRING_T>::iterator itr = oNoteTypeList.begin();itr != oNoteTypeList.end();++itr)
				{
					if(NULL != pGridProperty) pGridProperty->AddOption( itr->c_str() );
				}
			
				ctr = pDrawingSetup->find(_T("Drawing Type"));
				if(ctr != pDrawingSetup->end())
				{
					int i = 0;
					for(i = 0;i < nItemCount;++i)
					{
						if(ATOI_T(ctr->second.c_str()) == pNoteType->GetItemAt(i)->nNum)
						{
							if(NULL != pGridProperty)
							{
								pGridProperty->SetValue(pNoteType->GetItemAt(i)->rNumString.c_str());
								break;
							}
						}
					}
				}
			}
		}

		/// 2009.03.11에 추가.
		CProjectData* pProjectData = CProjectData::GetInstance();
		if(NULL != pProjectData)
		{
			CString rValue;
			rValue = (*(pProjectData->m_pProjectSettingMap))[_T("working_unit")];
			map<STRING_T , CMFCPropertyGridProperty*>::iterator where = m_oGridPropertyMap.find(_T("Working Unit"));
			pGridProperty = (m_oGridPropertyMap.end() != where) ? where->second : NULL;
			if((NULL != pGridProperty)) pGridProperty->SetValue( rValue );

			rValue = (*(pProjectData->m_pProjectSettingMap))[_T("dvcs_e")];
			where = m_oGridPropertyMap.find(_T("E"));
			pGridProperty = (m_oGridPropertyMap.end() != where) ? where->second : NULL;
			if((NULL != pGridProperty)) pGridProperty->SetValue( rValue );

			rValue = (*(pProjectData->m_pProjectSettingMap))[_T("dvcs_n")];
			where = m_oGridPropertyMap.find(_T("N"));
			pGridProperty = (m_oGridPropertyMap.end() != where) ? where->second : NULL;
			if((NULL != pGridProperty)) pGridProperty->SetValue( rValue );

			rValue = (*(pProjectData->m_pProjectSettingMap))[_T("dvcs_el")];
			where = m_oGridPropertyMap.find(_T("EL"));
			pGridProperty = (m_oGridPropertyMap.end() != where) ? where->second : NULL;
			if((NULL != pGridProperty)) pGridProperty->SetValue( rValue );

			/// 2011.09.21 - added by HumKyung
			rValue = (*(pProjectData->m_pProjectSettingMap))[_T("get_plan_dwg")];
			if(0 == rValue.CompareNoCase(_T("ON")))
			{
				map<STRING_T , CMFCPropertyGridProperty*>::iterator where = m_oGridPropertyMap.find(_T("Get Plan Dwg"));
				pGridProperty = (m_oGridPropertyMap.end() != where) ? where->second : NULL;
				if((NULL != pGridProperty)) pGridProperty->SetValue( _T("ON") );
			}
			else
			{
				map<STRING_T , CMFCPropertyGridProperty*>::iterator where = m_oGridPropertyMap.find(_T("Get Plan Dwg"));
				pGridProperty = (m_oGridPropertyMap.end() != where) ? where->second : NULL;
				if((NULL != pGridProperty)) pGridProperty->SetValue( _T("OFF") );
			}
			/// up to here

			/// 2013.06.03 - added by HumKyung
			if((pProjectData->m_pProjectSettingMap)->end() != (pProjectData->m_pProjectSettingMap)->find(_T("use_plan_dwg_by")))
			{
				rValue = (*(pProjectData->m_pProjectSettingMap))[_T("use_plan_dwg_by")];
				map<STRING_T , CMFCPropertyGridProperty*>::iterator where = m_oGridPropertyMap.find(_T("Use Plan Dwg By"));
				pGridProperty = (m_oGridPropertyMap.end() != where) ? where->second : NULL;
				if((NULL != pGridProperty)) pGridProperty->SetValue( rValue );
			}
			/// up to here
		}

		initialized = true;
	}
}

/**
	@brief	ENTER KEY나 ESCAPE KEY를 방지한다.

	@author	humkyung
**/
BOOL CRefDataSettingDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN ) // Enter Key Prevent.. so good.. ^^		
	{
		if(pMsg->wParam == VK_RETURN)		
		{
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
    @date       2012-05-05
    @function   OnInitDialog
    @return     BOOL
    @brief
******************************************************************************/
BOOL CRefDataSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect rectDummy;
	rectDummy.SetRectEmpty();
	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD | WS_BORDER, rectDummy, this, 2))
	{
		TRACE0("Failed to create Properties Grid \n");
		return -1;      // fail to create
	}
	this->InitPropList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

/******************************************************************************
    @author     humkyung
    @date       2012-05-05
    @function   SetPropListFont
    @return     void
    @brief
******************************************************************************/
void CRefDataSettingDlg::SetPropListFont()
{
	::DeleteObject(m_fntPropList.Detach());

	LOGFONT lf;
	afxGlobalData.fontRegular.GetLogFont(&lf);

	NONCLIENTMETRICS info;
	info.cbSize = sizeof(info);

	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_fntPropList.CreateFontIndirect(&lf);

	m_wndPropList.SetFont(&m_fntPropList);
}

/******************************************************************************
    @author     humkyung
    @date       2012-05-05
    @function   InitPropList
    @return     void
    @brief
******************************************************************************/
void CRefDataSettingDlg::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	m_oGridPropertyMap.clear();

	CMFCPropertyGridProperty* pGroupProperty = new CMFCPropertyGridProperty(_T("PCD Spec File Folder"));
	m_wndPropList.AddProperty(pGroupProperty);
	{
		CMFCPropertyGridProperty* pSubItem = new CMFCPropertyGridProperty(_T("Spec File Folder") , (_variant_t) _T("") , _T("(Ex) \\server01\\train\\spec\\"));
		pGroupProperty->AddSubItem( pSubItem );

		m_oGridPropertyMap.insert(make_pair(_T("Spec File Folder") , pSubItem));
	}

	pGroupProperty = new CMFCPropertyGridProperty(_T("Standard Note File"));
	m_wndPropList.AddProperty(pGroupProperty);
	{
		CMFCPropertyGridProperty* pSubItem = new CMFCPropertyGridProperty(_T("Standard Note File") , (_variant_t) _T("") , _T("(Ex) \\server01\\project\\stnotelib.rpt"));
		pGroupProperty->AddSubItem( pSubItem );

		m_oGridPropertyMap.insert(make_pair(_T("Standard Note File") , pSubItem));
	}

	pGroupProperty = new CMFCPropertyGridProperty(_T("Unit"));
	m_wndPropList.AddProperty(pGroupProperty);
	{
		CMFCPropertyGridProperty* pSubItem = new CMFCPropertyGridProperty(_T("Unit") , (_variant_t) _T("") , _T("Select unit"));
		pSubItem->AddOption(_T("INCH"));
		pSubItem->AddOption(_T("MM"));
		pSubItem->AllowEdit(FALSE);
		pGroupProperty->AddSubItem( pSubItem );

		m_oGridPropertyMap.insert(make_pair(_T("UNIT") , pSubItem));
	}

	pGroupProperty = new CMFCPropertyGridProperty(_T("Get Plan Dwg"));
	m_wndPropList.AddProperty(pGroupProperty);
	{
		CMFCPropertyGridProperty* pSubItem = new CMFCPropertyGridProperty(_T("Get Plan Dwg") , (_variant_t) _T("") , _T("Get Plan Dwg On/Off"));
		pSubItem->AddOption(_T("ON"));
		pSubItem->AddOption(_T("OFF"));
		pSubItem->AllowEdit(FALSE);
		pGroupProperty->AddSubItem( pSubItem );
		m_oGridPropertyMap.insert(make_pair(_T("Get Plan Dwg") , pSubItem));

		/// 2013.06.03 added by humkyung
		pSubItem = new CMFCPropertyGridProperty(_T("Use Plan Dwg By") , (_variant_t) _T("") , _T("Use Plan Dwg By"));
		pSubItem->AddOption(_T("DRAWING_NO"));
		pSubItem->AddOption(_T("DRAWING_TITLE"));
		pSubItem->AllowEdit(FALSE);
		pGroupProperty->AddSubItem( pSubItem );
		m_oGridPropertyMap.insert(make_pair(_T("Use Plan Dwg By") , pSubItem));
		/// up to here

		pSubItem = new CMFCPropertyGridProperty(_T("Drawing Type") , (_variant_t) _T("") , _T("Select Drawing Type"));
		pGroupProperty->AddSubItem( pSubItem );
		m_oGridPropertyMap.insert(make_pair(_T("Drawing Type") , pSubItem));

		pSubItem = new CMFCPropertyGridProperty(_T("Working Unit") , (_variant_t) _T("") , _T("Working Unit"));
		pGroupProperty->AddSubItem( pSubItem );
		m_oGridPropertyMap.insert(make_pair(_T("Working Unit") , pSubItem));

		CMFCPropertyGridProperty* pSubGroupProperty = new CMFCPropertyGridProperty(_T("DVCS"));
		pGroupProperty->AddSubItem( pSubGroupProperty );
		{
			pSubItem = new CMFCPropertyGridProperty(_T("E") , (_variant_t) _T("") , _T("East"));
			pSubGroupProperty->AddSubItem( pSubItem );
			m_oGridPropertyMap.insert(make_pair(_T("E") , pSubItem));

			pSubItem = new CMFCPropertyGridProperty(_T("N") , (_variant_t) _T("") , _T("North"));
			pSubGroupProperty->AddSubItem( pSubItem );
			m_oGridPropertyMap.insert(make_pair(_T("N") , pSubItem));

			pSubItem = new CMFCPropertyGridProperty(_T("EL") , (_variant_t) _T("") , _T("Elevation"));
			pSubGroupProperty->AddSubItem( pSubItem );
			m_oGridPropertyMap.insert(make_pair(_T("EL") , pSubItem));
		}
	}
}

/******************************************************************************
    @author     humkyung
    @date       2012-05-05
    @function   OnSize
    @return     void
    @param      UINT    nType
    @param      int     cx
    @param      int     cy
    @brief
******************************************************************************/
void CRefDataSettingDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(m_wndPropList.GetSafeHwnd())
	{
		m_wndPropList.SetWindowPos(NULL , 0 , 0 , cx , cy , SWP_NOZORDER);
	}
}

/******************************************************************************
    @author     humkyung
    @date       2012-06-05
    @class      CRefDataSettingDlg
    @function   OnHelpInfo
    @return     BOOL
    @brief
******************************************************************************/
BOOL CRefDataSettingDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	HWND hHtml = ::HtmlHelp(this->m_hWnd , CAdminDocData::GetExecPath() + _T("\\Help\\SmartISO.chm::/Ref Data Setting.htm") , HH_DISPLAY_TOPIC , 0);

	return TRUE;///CDialog::OnHelpInfo(pHelpInfo);
}
