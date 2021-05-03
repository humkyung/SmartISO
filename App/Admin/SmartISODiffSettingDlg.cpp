// SmartISODiffSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include <Tokenizer.h>
#include <gui/enablebuddybutton.h>
#include <gui/FolderDlg/FolderDlg.h>
#include "Admin.h"
#include "AdminDocData.h"
#include "SmartISODiffSettingDlg.h"
#include <ProjectData.h>

// CSmartISODiffSettingDlg dialog

IMPLEMENT_DYNAMIC(CSmartISODiffSettingDlg, CDialog)

CSmartISODiffSettingDlg::CSmartISODiffSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSmartISODiffSettingDlg::IDD, pParent)
{
	m_pAdminDoc = NULL;
}

CSmartISODiffSettingDlg::~CSmartISODiffSettingDlg()
{
}

void CSmartISODiffSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSmartISODiffSettingDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_REV_ISO_FOLDER, &CSmartISODiffSettingDlg::OnBnClickedButtonRevIsoFolder)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CSmartISODiffSettingDlg message handlers
/******************************************************************************
    @author     humkyung
    @date       2012-01-03
    @class      CSmartISODiffSettingDlg
    @function   UpdateContents
    @return     void
    @brief
******************************************************************************/
void CSmartISODiffSettingDlg::UpdateContents()
{
	static bool initialized = false;
	static const CString sApp(_T("Revision Automation"));

	if(false == initialized)
	{
		CAdminDocData* pDocData = CAdminDocData::GetInstance();

		CMFCPropertyGridProperty* pGridProperty=NULL;
		map<STRING_T , CMFCPropertyGridProperty*>::iterator where = m_oGridPropertyMap.find(_T("Drawing Type"));
		/*pGridProperty = (where != m_oGridPropertyMap.end()) ? where->second : NULL;

		UINT uiArrowType = GetPrivateProfileInt(sApp , _T("Arrow Type") , 1 , sIniFilePath);
		if(NULL != pGridProperty) pGridProperty->SetValue((1 == uiArrowType) ? _T("PDS ISO") : _T("PDMS ISO"));*/

		map<STRING_T,STRING_T>* pDrawingSetup = CProjectData::GetInstance()->GetDrawingSetup();
		map<STRING_T,STRING_T>::const_iterator itr = pDrawingSetup->find(_T("PipeLine"));
		if(itr != pDrawingSetup->end())
		{
			vector<STRING_T> oResult;
			CTokenizer<CIsComma>::Tokenize(oResult ,itr->second, CIsComma());
			if(3 == oResult.size())
			{
				where = m_oGridPropertyMap.find(_T("Pipe Line Level"));
				pGridProperty = (where != m_oGridPropertyMap.end()) ? where->second : NULL;
				if(NULL != pGridProperty) pGridProperty->SetValue(oResult[0].c_str());

				where = m_oGridPropertyMap.find(_T("Pipe Line Color"));
				pGridProperty = (where != m_oGridPropertyMap.end()) ? where->second : NULL;
				if(NULL != pGridProperty) pGridProperty->SetValue(oResult[1].c_str());

				where = m_oGridPropertyMap.find(_T("Pipe Line Weight"));
				pGridProperty = (where != m_oGridPropertyMap.end()) ? where->second : NULL;
				if(NULL != pGridProperty) pGridProperty->SetValue(oResult[2].c_str());
			}
		}

		where = m_oGridPropertyMap.find(_T("Cloud Mark Offset"));
		pGridProperty = (where != m_oGridPropertyMap.end()) ? where->second : NULL;
		itr = pDrawingSetup->find(_T("Cloud Mark Offset"));
		if(itr != pDrawingSetup->end())
		{
			if(NULL != pGridProperty) pGridProperty->SetValue(itr->second.c_str());
		}
		else
		{
			if(NULL != pGridProperty) pGridProperty->SetValue(_T("1"));
		}
		
		itr = pDrawingSetup->find(_T("CloudMark"));
		if(itr != pDrawingSetup->end())
		{
			vector<STRING_T> oResult;
			CTokenizer<CIsComma>::Tokenize(oResult ,itr->second, CIsComma());
			if(3 == oResult.size())
			{
				where = m_oGridPropertyMap.find(_T("Cloud Mark Level"));
				pGridProperty = (where != m_oGridPropertyMap.end()) ? where->second : NULL;
				if(NULL != pGridProperty) pGridProperty->SetValue(oResult[0].c_str());

				where = m_oGridPropertyMap.find(_T("Cloud Mark Color"));
				pGridProperty = (where != m_oGridPropertyMap.end()) ? where->second : NULL;
				if(NULL != pGridProperty) pGridProperty->SetValue(oResult[1].c_str());

				where = m_oGridPropertyMap.find(_T("Cloud Mark Weight"));
				pGridProperty = (where != m_oGridPropertyMap.end()) ? where->second : NULL;
				if(NULL != pGridProperty) pGridProperty->SetValue(oResult[2].c_str());
			}
		}

		itr = pDrawingSetup->find(_T("ArcMin"));
		if(itr != pDrawingSetup->end())
		{
			where = m_oGridPropertyMap.find(_T("Cloud Mark ArcMin"));
			pGridProperty = (where != m_oGridPropertyMap.end()) ? where->second : NULL;
			if(NULL != pGridProperty) pGridProperty->SetValue(itr->second.c_str());
		}

		itr = pDrawingSetup->find(_T("ArcMax"));
		if(itr != pDrawingSetup->end())
		{
			where = m_oGridPropertyMap.find(_T("Cloud Mark ArcMax"));
			pGridProperty = (where != m_oGridPropertyMap.end()) ? where->second : NULL;
			if(NULL != pGridProperty) pGridProperty->SetValue(itr->second.c_str());
		}

		itr = pDrawingSetup->find(_T("Diff Rev No Display"));
		if(itr != pDrawingSetup->end())
		{
			where = m_oGridPropertyMap.find(_T("Rev No Display"));
			pGridProperty = (where != m_oGridPropertyMap.end()) ? where->second : NULL;
			if(NULL != pGridProperty) pGridProperty->SetValue(itr->second.c_str());
		}

		itr = pDrawingSetup->find(_T("Rev No Text Style"));
		if(itr != pDrawingSetup->end())
		{
			vector<STRING_T> oResult;
			CTokenizer<CIsComma>::Tokenize(oResult ,itr->second, CIsComma());
			if(3 == oResult.size())
			{
				where = m_oGridPropertyMap.find(_T("Rev No Font No"));
				pGridProperty = (where != m_oGridPropertyMap.end()) ? where->second : NULL;
				if(NULL != pGridProperty) pGridProperty->SetValue(oResult[0].c_str());

				where = m_oGridPropertyMap.find(_T("Rev No Text Level"));
				pGridProperty = (where != m_oGridPropertyMap.end()) ? where->second : NULL;
				if(NULL != pGridProperty) pGridProperty->SetValue(oResult[1].c_str());

				where = m_oGridPropertyMap.find(_T("Rev No Text Color"));
				pGridProperty = (where != m_oGridPropertyMap.end()) ? where->second : NULL;
				if(NULL != pGridProperty) pGridProperty->SetValue(oResult[2].c_str());
			}
		}
				
		initialized = true;
	}
}

/******************************************************************************
    @author     humkyung
    @date       2012-01-03
    @class      CSmartISODiffSettingDlg
    @function   Save
    @return     void
    @brief
******************************************************************************/
void CSmartISODiffSettingDlg::Save()
{
	static const CString sApp(_T("Revision Automation"));
	
	CAdminDocData* pDocData = CAdminDocData::GetInstance();

	CMFCPropertyGridProperty* pGridProperty=NULL;
	map<STRING_T , CMFCPropertyGridProperty*>::iterator where = m_oGridPropertyMap.find(_T("Drawing Type"));
	pGridProperty = (where != m_oGridPropertyMap.end()) ? where->second : NULL;

	CString sValue;
	map<STRING_T,STRING_T>* pDrawingSetup = CProjectData::GetInstance()->GetDrawingSetup();
	{
		where = m_oGridPropertyMap.find(_T("Pipe Line Level"));
		pGridProperty = (where != m_oGridPropertyMap.end()) ? where->second : NULL;
		sValue = (NULL != pGridProperty) ? pGridProperty->GetValue() : _T("");
		sValue+= _T(",");

		where = m_oGridPropertyMap.find(_T("Pipe Line Color"));
		pGridProperty = (where != m_oGridPropertyMap.end()) ? where->second : NULL;
		sValue += (NULL != pGridProperty) ? pGridProperty->GetValue() : _T(",");
		sValue+= _T(",");

		where = m_oGridPropertyMap.find(_T("Pipe Line Weight"));
		pGridProperty = (where != m_oGridPropertyMap.end()) ? where->second : NULL;
		sValue += (NULL != pGridProperty) ? pGridProperty->GetValue() : _T("");

		(*pDrawingSetup)[_T("PipeLine")] = sValue;
	}

	where = m_oGridPropertyMap.find(_T("Cloud Mark Offset"));
	pGridProperty = (where != m_oGridPropertyMap.end()) ? where->second : NULL;
	sValue = (NULL != pGridProperty) ? pGridProperty->GetValue() : _T("");
	(*pDrawingSetup)[_T("Cloud Mark Offset")] = sValue;

	{
		where = m_oGridPropertyMap.find(_T("Cloud Mark Level"));
		pGridProperty = (where != m_oGridPropertyMap.end()) ? where->second : NULL;
		sValue = (NULL != pGridProperty) ? pGridProperty->GetValue() : _T("");
		sValue+= _T(",");

		where = m_oGridPropertyMap.find(_T("Cloud Mark Color"));
		pGridProperty = (where != m_oGridPropertyMap.end()) ? where->second : NULL;
		sValue += (NULL != pGridProperty) ? pGridProperty->GetValue() : _T("");
		sValue+= _T(",");

		where = m_oGridPropertyMap.find(_T("Cloud Mark Weight"));
		pGridProperty = (where != m_oGridPropertyMap.end()) ? where->second : NULL;
		sValue += (NULL != pGridProperty) ? pGridProperty->GetValue() : _T("");

		(*pDrawingSetup)[_T("CloudMark")] = sValue;
	}

	where = m_oGridPropertyMap.find(_T("Cloud Mark ArcMin"));
	pGridProperty = (where != m_oGridPropertyMap.end()) ? where->second : NULL;
	sValue = (NULL != pGridProperty) ? pGridProperty->GetValue() : _T("");
	(*pDrawingSetup)[_T("ArcMin")] = sValue;

	where = m_oGridPropertyMap.find(_T("Cloud Mark ArcMax"));
	pGridProperty = (where != m_oGridPropertyMap.end()) ? where->second : NULL;
	sValue = (NULL != pGridProperty) ? pGridProperty->GetValue() : _T("");
	(*pDrawingSetup)[_T("ArcMax")] = sValue;

	{
		where = m_oGridPropertyMap.find(_T("Rev No Display"));
		pGridProperty = (where != m_oGridPropertyMap.end()) ? where->second : NULL;
		sValue = (NULL != pGridProperty) ? pGridProperty->GetValue() : _T("");
		(*pDrawingSetup)[_T("Diff Rev No Display")] = sValue;

		where = m_oGridPropertyMap.find(_T("Rev No Font No"));
		pGridProperty = (where != m_oGridPropertyMap.end()) ? where->second : NULL;
		sValue = (NULL != pGridProperty) ? pGridProperty->GetValue() : _T("");
		sValue+= _T(",");

		where = m_oGridPropertyMap.find(_T("Rev No Text Level"));
		pGridProperty = (where != m_oGridPropertyMap.end()) ? where->second : NULL;
		sValue += (NULL != pGridProperty) ? pGridProperty->GetValue() : _T("");
		sValue+= _T(",");

		where = m_oGridPropertyMap.find(_T("Rev No Text Color"));
		pGridProperty = (where != m_oGridPropertyMap.end()) ? where->second : NULL;
		sValue += (NULL != pGridProperty) ? pGridProperty->GetValue() : _T("");

		(*pDrawingSetup)[_T("Rev No Text Style")] = sValue;
	}
}

BOOL CSmartISODiffSettingDlg::OnInitDialog()
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
    @date       2012-05-09
    @class      CSmartISODiffSettingDlg
    @function   SetPropListFont
    @return     void
    @brief
******************************************************************************/
void CSmartISODiffSettingDlg::SetPropListFont()
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
    @date       2012-05-09
    @class      CSmartISODiffSettingDlg
    @function   InitPropList
    @return     void
    @brief
******************************************************************************/
void CSmartISODiffSettingDlg::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	m_oGridPropertyMap.clear();

	CMFCPropertyGridProperty* pGroupProperty = new CMFCPropertyGridProperty(_T("Pipe Line"));
	m_wndPropList.AddProperty(pGroupProperty);
	{
		CMFCPropertyGridProperty* pSubItem = new CMFCPropertyGridProperty(_T("Level") , (_variant_t) _T("") , _T("Level"));
		CString rString;
		for(int i = 1;i <= 63;++i)
		{
			rString.Format(_T("%d") , i);
			pSubItem->AddOption(rString);
		}
		pGroupProperty->AddSubItem( pSubItem );
		m_oGridPropertyMap.insert(make_pair(_T("Pipe Line Level") , pSubItem));

		pSubItem = new CMFCPropertyGridProperty(_T("Color") , (_variant_t) _T("") , _T("Color"));
		for(int i = 0;i < 256;++i)
		{
			rString.Format(_T("%d") , i);
			pSubItem->AddOption(rString);
		}
		pGroupProperty->AddSubItem( pSubItem );
		m_oGridPropertyMap.insert(make_pair(_T("Pipe Line Color") , pSubItem));

		pSubItem = new CMFCPropertyGridProperty(_T("Weight") , (_variant_t) _T("") , _T("Weight"));
		for(int i = 0;i < 16;++i)
		{
			rString.Format(_T("%d") , i);
			pSubItem->AddOption(rString);
		}
		pGroupProperty->AddSubItem( pSubItem );
		m_oGridPropertyMap.insert(make_pair(_T("Pipe Line Weight") , pSubItem));
	}

	pGroupProperty = new CMFCPropertyGridProperty(_T("Cloud Mark"));
	m_wndPropList.AddProperty(pGroupProperty);
	{
		CMFCPropertyGridProperty* pSubItem = new CMFCPropertyGridProperty(_T("Offset") , (_variant_t) _T("") , _T("Offset"));
		pGroupProperty->AddSubItem( pSubItem );
		m_oGridPropertyMap.insert(make_pair(_T("Cloud Mark Offset") , pSubItem));

		pSubItem = new CMFCPropertyGridProperty(_T("Level") , (_variant_t) _T("") , _T("Level"));
		CString rString;
		for(int i = 1;i <= 63;++i)
		{
			rString.Format(_T("%d") , i);
			pSubItem->AddOption(rString);
		}
		pGroupProperty->AddSubItem( pSubItem );
		m_oGridPropertyMap.insert(make_pair(_T("Cloud Mark Level") , pSubItem));

		pSubItem = new CMFCPropertyGridProperty(_T("Color") , (_variant_t) _T("") , _T("Color"));
		for(int i = 0;i < 256;++i)
		{
			rString.Format(_T("%d") , i);
			pSubItem->AddOption(rString);
		}
		pGroupProperty->AddSubItem( pSubItem );
		m_oGridPropertyMap.insert(make_pair(_T("Cloud Mark Color") , pSubItem));

		pSubItem = new CMFCPropertyGridProperty(_T("Weight") , (_variant_t) _T("") , _T("Weight"));
		for(int i = 0;i < 16;++i)
		{
			rString.Format(_T("%d") , i);
			pSubItem->AddOption(rString);
		}
		pGroupProperty->AddSubItem( pSubItem );
		m_oGridPropertyMap.insert(make_pair(_T("Cloud Mark Weight") , pSubItem));

		pSubItem = new CMFCPropertyGridProperty(_T("ArcMin") , (_variant_t) _T("") , _T("ArcMin"));
		pGroupProperty->AddSubItem( pSubItem );
		m_oGridPropertyMap.insert(make_pair(_T("Cloud Mark ArcMin") , pSubItem));

		pSubItem = new CMFCPropertyGridProperty(_T("ArcMax") , (_variant_t) _T("") , _T("ArcMax"));
		pGroupProperty->AddSubItem( pSubItem );
		m_oGridPropertyMap.insert(make_pair(_T("Cloud Mark ArcMax") , pSubItem));
	}

	pGroupProperty = new CMFCPropertyGridProperty(_T("Rev No"));
	m_wndPropList.AddProperty(pGroupProperty);
	{
		CMFCPropertyGridProperty* pSubItem = new CMFCPropertyGridProperty(_T("Display"),(_variant_t) _T("") , _T("Display"));
		{
			pSubItem->AddOption(_T("True"));
			pSubItem->AddOption(_T("False"));
		}
		pGroupProperty->AddSubItem(pSubItem);
		m_oGridPropertyMap.insert(make_pair(_T("Rev No Display") , pSubItem));

		pSubItem = new CMFCPropertyGridProperty(_T("Font No") , (_variant_t) _T("") , _T("Font No"));
		pGroupProperty->AddSubItem( pSubItem );
		m_oGridPropertyMap.insert(make_pair(_T("Rev No Font No") , pSubItem));

		pSubItem = new CMFCPropertyGridProperty(_T("Level") , (_variant_t) _T("") , _T("Level"));
		CString rString;
		for(int i = 1;i <= 63;++i)
		{
			rString.Format(_T("%d") , i);
			pSubItem->AddOption(rString);
		}
		pGroupProperty->AddSubItem( pSubItem );
		m_oGridPropertyMap.insert(make_pair(_T("Rev No Text Level") , pSubItem));

		pSubItem = new CMFCPropertyGridProperty(_T("Color") , (_variant_t) _T("") , _T("Color"));
		for(int i = 1;i <= 256;++i)
		{
			rString.Format(_T("%d") , i);
			pSubItem->AddOption(rString);
		}
		pGroupProperty->AddSubItem( pSubItem );
		m_oGridPropertyMap.insert(make_pair(_T("Rev No Text Color") , pSubItem));
	}
}

/******************************************************************************
    @author     humkyung
    @date       2012-01-04
    @class      CSmartISODiffSettingDlg
    @function   OnBnClickedButtonRevIsoFolder
    @return     void
    @brief
******************************************************************************/
void CSmartISODiffSettingDlg::OnBnClickedButtonRevIsoFolder()
{
	CString sRevIsoFolderPath;
	GetDlgItemText(IDC_EDIT_REV_ISO_FOLDER, sRevIsoFolderPath);
	const UINT uFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
	CFolderDialog oFolderDlg(_T("Select Rev. Iso Folder") , sRevIsoFolderPath , this , uFlags);
	if(IDOK == oFolderDlg.DoModal())
	{
		sRevIsoFolderPath = oFolderDlg.GetFolderPath();
		SetDlgItemText(IDC_EDIT_REV_ISO_FOLDER , sRevIsoFolderPath);
	}
}

/******************************************************************************
    @author     humkyung
    @date       2012-05-09
    @class      CSmartISODiffSettingDlg
    @function   OnSize
    @return     void
    @param      UINT    nType
    @param      int     cx
    @param      int     cy
    @brief
******************************************************************************/
void CSmartISODiffSettingDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if(m_wndPropList.GetSafeHwnd())
	{
		CRect rect;
		GetDlgItem(IDC_STATIC_CLOUD_MARK)->GetWindowRect(&rect);
		ScreenToClient(&rect);

		m_wndPropList.SetWindowPos(NULL , rect.left , rect.bottom + 5 , rect.Width() , cy - rect.bottom - 10 , SWP_NOZORDER);
	}
}
