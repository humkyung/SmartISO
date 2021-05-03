// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://msdn.microsoft.com/officeui.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

#include "stdafx.h"
#include <assert.h>

#include <Tokenizer.h>
#include "PropertiesWnd.h"
#include "Resource.h"
#include "MainFrm.h"
#include "UserApp.h"
#include "UserAppDoc.h"
#include "UserAppDocData.h"
#include "UserAppView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CMFCPropEx, CMFCPropertyGridProperty)
CMFCPropEx::CMFCPropEx(const CString& strName, const COleVariant& varValue, LPCTSTR lpszDescr, DWORD_PTR dwData,
		LPCTSTR lpszEditMask, LPCTSTR lpszEditTemplate, LPCTSTR lpszValidChars) : CMFCPropertyGridProperty(strName, varValue, lpszDescr, dwData,
		lpszEditMask, lpszEditTemplate, lpszValidChars)
{
}

static CPropertiesWnd* __instance__=NULL;
/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar

CPropertiesWnd::CPropertiesWnd()
{
	__instance__ = this;
}

CPropertiesWnd::~CPropertiesWnd()
{
}

BEGIN_MESSAGE_MAP(CPropertiesWnd, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	/*ON_COMMAND(ID_EXPAND_ALL, OnExpandAllProperties)
	ON_UPDATE_COMMAND_UI(ID_EXPAND_ALL, OnUpdateExpandAllProperties)
	ON_COMMAND(ID_SORTPROPERTIES, OnSortProperties)
	ON_UPDATE_COMMAND_UI(ID_SORTPROPERTIES, OnUpdateSortProperties)
	ON_COMMAND(ID_PROPERTIES1, OnProperties1)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES1, OnUpdateProperties1)
	ON_COMMAND(ID_PROPERTIES2, OnProperties2)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES2, OnUpdateProperties2)*/
	ON_WM_SETFOCUS()
	ON_WM_SETTINGCHANGE()
	ON_REGISTERED_MESSAGE(AFX_WM_PROPERTY_CHANGED, OnPropertyChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceViewBar message handlers
CPropertiesWnd* CPropertiesWnd::GetInstance()
{
	return __instance__;
}

void CPropertiesWnd::AdjustLayout()
{
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	CRect rectClient;
	GetClientRect(rectClient);

	int cyCmb = 0;
	int cyTlb = 0;///m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;

	if(m_wndToolBar.GetSafeHwnd())
	{
		m_wndToolBar.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb, rectClient.Width(), cyTlb, SWP_NOACTIVATE | SWP_NOZORDER);
	}
	m_wndPropList.SetWindowPos(NULL, rectClient.left, rectClient.top + cyCmb + cyTlb, rectClient.Width(), rectClient.Height() -(cyCmb+cyTlb), SWP_NOACTIVATE | SWP_NOZORDER);
}

int CPropertiesWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// Create combo:
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_wndPropList.Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
	{
		TRACE0("Failed to create Properties Grid \n");
		return -1;      // fail to create
	}

	InitPropList();

	// All commands will be routed via this control , not via the parent frame:
	m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

	AdjustLayout();
	return 0;
}

void CPropertiesWnd::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);
	AdjustLayout();
}

void CPropertiesWnd::OnExpandAllProperties()
{
	m_wndPropList.ExpandAll();
}

void CPropertiesWnd::OnUpdateExpandAllProperties(CCmdUI* pCmdUI)
{
}

void CPropertiesWnd::OnSortProperties()
{
	m_wndPropList.SetAlphabeticMode(!m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnUpdateSortProperties(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_wndPropList.IsAlphabeticMode());
}

void CPropertiesWnd::OnProperties1()
{
	// TODO: Add your command handler code here
}

void CPropertiesWnd::OnUpdateProperties1(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}

void CPropertiesWnd::OnProperties2()
{
	// TODO: Add your command handler code here
}

void CPropertiesWnd::OnUpdateProperties2(CCmdUI* /*pCmdUI*/)
{
	// TODO: Add your command update UI handler code here
}

/******************************************************************************
    @author     humkyung
    @date       2012-03-31
    @function   InitPropList
    @return     void
    @brief
******************************************************************************/
void CPropertiesWnd::InitPropList()
{
	SetPropListFont();

	m_wndPropList.EnableHeaderCtrl(FALSE);
	m_wndPropList.EnableDescriptionArea();
	m_wndPropList.SetVSDotNetLook();
	m_wndPropList.MarkModifiedProperties();

	CMFCPropertyGridProperty* pGroup1 = new CMFCPropertyGridProperty(_T("Line Data"));
	m_wndPropList.AddProperty(pGroup1);
}

void CPropertiesWnd::OnSetFocus(CWnd* pOldWnd)
{
	CDockablePane::OnSetFocus(pOldWnd);
	m_wndPropList.SetFocus();
}

void CPropertiesWnd::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CDockablePane::OnSettingChange(uFlags, lpszSection);
	SetPropListFont();
}

void CPropertiesWnd::SetPropListFont()
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
    @date       2012-03-31
    @function   Reset
    @return     int
    @brief
******************************************************************************/
int CPropertiesWnd::Reset()
{
	CMFCPropertyGridProperty* pGroupProperty = m_wndPropList.GetProperty(0);
	if(NULL != pGroupProperty)
	{
		const int iSubItems = pGroupProperty->GetSubItemsCount();
		for(int i = 0;i < iSubItems;++i)
		{
			pGroupProperty->GetSubItem(i)->SetValue(_T(""));
		}
	}

	pGroupProperty = m_wndPropList.GetProperty(1);
	if(NULL != pGroupProperty)
	{
		const int iSubItems = pGroupProperty->GetSubItemsCount();
		for(int i = 0;i < iSubItems;++i)
		{
			pGroupProperty->GetSubItem(i)->SetValue(_T(""));
		}
	}

	return ERROR_SUCCESS;
}

static bool SortByLen( const STRING_T& lhs , const STRING_T& rhs )
{
	return (lhs.length() > rhs.length());
}

/******************************************************************************
    @author     humkyung
    @date       2013-02-06
    @function   GetValueOf
    @return     STRING_T
    @param      
    @brief		return value of name(serise of * if there are different values)
******************************************************************************/
static STRING_T GetValueOf(const STRING_T& sName , list<CLineDataRecord*>& oLineDataRecordList)
{
	STRING_T res;

	vector<STRING_T> oResult;
	for(list<CLineDataRecord*>::const_iterator itr = oLineDataRecordList.begin();itr != oLineDataRecordList.end();++itr)
	{
		CDataField* pField = (*itr)->FindWithFieldName(sName.c_str());
		if(NULL != pField)
		{
			vector<STRING_T>::const_iterator where = find(oResult.begin() , oResult.end() , STRING_T(pField->value()));
			if(where == oResult.end())
			{
				oResult.push_back(pField->value());
			}
		}
	}

	if(1 == oResult.size())
	{
		res = oResult[0];
	}
	else if(oResult.size() > 1)
	{
		::stable_sort(oResult.begin() , oResult.end() , SortByLen);
		for(int i = 0;i < int(oResult[0].length());++i)
		{
			res += _T("*");
		}
	}

	return res;
}

/******************************************************************************
    @author     humkyung
    @date       2013-02-05
    @function   DisplayDataOf
    @return     int
    @param      CLineDataRecord* pLineDataRecord
    @brief
******************************************************************************/
int CPropertiesWnd::DisplayDataOf(list<CLineDataRecord*>& oLineDataRecordList,const BOOL& isEditable)
{
	CMFCPropertyGridProperty* pGroupProperty = m_wndPropList.GetProperty(0);

	if(!oLineDataRecordList.empty())
	{
		m_oLineDataRecordList.clear();
		m_oLineDataRecordList.insert(m_oLineDataRecordList.begin() , oLineDataRecordList.begin() , oLineDataRecordList.end());

		const int iFieldCount = oLineDataRecordList.front()->GetFieldCount();
		if(0 == pGroupProperty->GetSubItemsCount())
		{
			for(int i = 0;i < iFieldCount;++i)
			{
				CDataField* pField = oLineDataRecordList.front()->GetFieldAt(i);
				const STRING_T sValue = GetValueOf(pField->name() , oLineDataRecordList);
				CMFCPropertyGridProperty* pSubItem = NULL;
				if(STRING_T("STATUS") == pField->name())
				{
					pSubItem = new CMFCPropEx(pField->name(), (_variant_t) sValue.c_str() , pField->desc() , (DWORD_PTR)(pField->name()));
					{
						pSubItem->AddOption(_T("IN PROGRESS"));
						pSubItem->AddOption(_T("COMPLETE"));
						pSubItem->AddOption(_T("HOLD"));
						pSubItem->AddOption(_T("DELETED"));
					}
				}
				else
				{
					pSubItem = new CMFCPropEx(pField->name(), (_variant_t) sValue.c_str() , sValue.c_str() , (DWORD_PTR)(pField->name()));
				}
				if(NULL != pSubItem)
				{
					pGroupProperty->AddSubItem( pSubItem );
					(STRING_T(_T("KEY")) == pField->name()) ? pSubItem->AllowEdit(FALSE) : pSubItem->AllowEdit(isEditable);
				}
			}
		}
		else
		{
			for(int i = 0;i < iFieldCount;++i)
			{
				CDataField* pField = oLineDataRecordList.front()->GetFieldAt(i);
				int j = 0;
				for(j = 0;j < pGroupProperty->GetSubItemsCount();++j)
				{
					CMFCPropertyGridProperty* pSubItem = pGroupProperty->GetSubItem(j);
					if(STRING_T(pField->name()) == STRING_T(pSubItem->GetName()))
					{
						break;
					}
				}
				if(j != pGroupProperty->GetSubItemsCount())
				{
					const STRING_T sValue = GetValueOf(pField->name() , oLineDataRecordList);
					pGroupProperty->GetSubItem(j)->SetOriginalValue(sValue.c_str());
					pGroupProperty->GetSubItem(j)->SetValue(sValue.c_str());
					pGroupProperty->AllowEdit(isEditable);
				}
			}
		}
		m_wndPropList.ExpandAll();

		return ERROR_SUCCESS;
	}

	return ERROR_INVALID_PARAMETER;
}

/******************************************************************************
    @author     humkyung
    @date       2012-03-31
    @function   OnPropertyChanged
    @return     LRESULT
    @param      WPARAM  wp
    @param      LPARAM  lp
    @brief
******************************************************************************/
LRESULT CPropertiesWnd::OnPropertyChanged(WPARAM wp,  LPARAM lp)
{
	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*)(lp);
	if(!m_oLineDataRecordList.empty())
	{
		CUserAppView* pView = CUserAppView::GetInstance();
		if(NULL != pView)
		{
			pView->m_pLineDataForm->SavePartDataOf(pProp->GetName() , CString(pProp->GetValue()).operator LPCTSTR() , m_oLineDataRecordList);
		}
	}

	return 0;
}