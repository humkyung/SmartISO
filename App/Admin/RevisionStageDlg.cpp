// RevisionStageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "admin.h"
#include "RevisionStageDlg.h"
#include "AdminDoc.h"

#include <ProjectData.h>
#include <transaction.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRevisionStageDlg dialog


CRevisionStageDlg::CRevisionStageDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRevisionStageDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRevisionStageDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CRevisionStageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRevisionStageDlg)
	DDX_Control(pDX , IDC_BUTTON_GRADE_ADD , m_btnAddGrade);
	DDX_Control(pDX , IDC_BUTTON_GRADE_DEL , m_btnDelGrade);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRevisionStageDlg, CDialog)
	//{{AFX_MSG_MAP(CRevisionStageDlg)
	ON_BN_CLICKED(IDC_BUTTON_GRADE_DEL, OnButtonGradeDel)
	ON_BN_CLICKED(IDC_BUTTON_GRADE_ADD, OnButtonGradeAdd)
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRevisionStageDlg message handlers

BOOL CRevisionStageDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CWnd* pWnd = GetDlgItem(IDC_STATIC_SELECT_REV_NO);
	if(NULL != pWnd) pWnd->SetWindowText(_TR("Select Revision No"));

	pWnd = GetDlgItem(IDC_STATIC_REV_NO_DEFINE);
	if(NULL != pWnd)
	{
		pWnd->SetWindowText(_TR("Rev. No Define"));

		CRect rect;
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);
		rect.DeflateRect(5,20,5,5);

		if( m_wndRevNoDefineGridCtrl.Create(rect , this , 0x100) )
		{
			m_wndRevNoDefineGridCtrl.SetEditable(TRUE);
			m_wndRevNoDefineGridCtrl.EnableDragAndDrop(FALSE);
			m_wndRevNoDefineGridCtrl.SetRowResize(TRUE);
			m_wndRevNoDefineGridCtrl.SetColumnResize(TRUE);
			m_wndRevNoDefineGridCtrl.SetListMode(FALSE);
			m_wndRevNoDefineGridCtrl.SetSingleRowSelection(TRUE);
			m_wndRevNoDefineGridCtrl.SetHeaderSort(FALSE);
			m_wndRevNoDefineGridCtrl.SetSingleColSelection(TRUE);
		}
	}

	pWnd = GetDlgItem(IDC_STATIC_REV_USER);
	if(NULL != pWnd)
	{
		pWnd->SetWindowText(_TR("Rev. User"));

		CRect rect;
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);
		rect.DeflateRect(5,20,5,5);

		if(m_wndRevUserGridCtrl.Create(rect , this , 0x101))
		{
			m_wndRevUserGridCtrl.SetEditable(TRUE);
			m_wndRevUserGridCtrl.EnableDragAndDrop(FALSE);
			m_wndRevUserGridCtrl.SetRowResize(TRUE);
			m_wndRevUserGridCtrl.SetColumnResize(TRUE);
			m_wndRevUserGridCtrl.SetListMode(FALSE);
			m_wndRevUserGridCtrl.SetSingleRowSelection(TRUE);
			m_wndRevUserGridCtrl.SetHeaderSort(FALSE);
			m_wndRevUserGridCtrl.SetSingleColSelection(TRUE);

			m_wndRevUserGridCtrl.SetRowCount(2);
			m_wndRevUserGridCtrl.SetColumnCount(4);
			m_wndRevUserGridCtrl.SetFixedRowCount(1);
			m_wndRevUserGridCtrl.SetFixedColumnCount(0);
			m_wndRevUserGridCtrl.SetItemText(0,0,_T("DRWN BY"));
			m_wndRevUserGridCtrl.SetItemText(0,1,_T("CHECKED BY"));
			m_wndRevUserGridCtrl.SetItemText(0,2,_T("REVIEWED BY"));
			m_wndRevUserGridCtrl.SetItemText(0,3,_T("APPROVED BY"));
		}
	}

	{
		HICON hIcon = (HICON) LoadImage(AfxGetInstanceHandle(),
									MAKEINTRESOURCE(IDI_ADD),
									IMAGE_ICON, 16, 16, 0);
		HICON hOldIcon = m_btnAddGrade.SetIcon(hIcon);
		if(hOldIcon) DestroyIcon(hOldIcon);
	}
	{
		HICON hIcon = (HICON) LoadImage(AfxGetInstanceHandle(),
									MAKEINTRESOURCE(IDI_REMOVE),
									IMAGE_ICON, 16, 16, 0);
		HICON hOldIcon = m_btnDelGrade.SetIcon(hIcon);
		if(hOldIcon) DestroyIcon(hOldIcon);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/**	
	@brief	프로젝트가 로드되고 난뒤 폼을 초기화 시킨다.
	@author	HumKyung.BAEK
	@date	2010.03.04
	@return	void	
*/
int CRevisionStageDlg::UpdateContents()
{
	CAdminDoc* pDoc = GetDocument();

	if(pDoc)
	{
		CProject::DatabaseProp prop  = CAdminDoc::GetInstance()->GetCurProject()->GetDatabaseProp();
		try
		{
			CAppPostgreSQL database(CAdminDoc::GetInstance()->GetCurProject()->name() , prop.Host.c_str() , prop.User.c_str() , prop.Password.c_str());
			{
				CString strSQL = _T("SELECT * FROM T_GRADE_DEFINE");
				soci::rowset<soci::row> rs(database.session()->prepare << (strSQL.operator LPCTSTR()));
				
				int row = 0;
				m_wndRevNoDefineGridCtrl.SetColumnCount(3);
				m_wndRevNoDefineGridCtrl.SetFixedRowCount(1);
				m_wndRevNoDefineGridCtrl.SetFixedColumnCount(0);
				for(soci::rowset<soci::row>::const_iterator itr = rs.begin();itr != rs.end();++itr,++row)
				{
					m_wndRevNoDefineGridCtrl.SetRowCount(row + 2);

					STRING_T strValue;
					strValue = (soci::i_ok == itr->get_indicator(_T("grade_prefix"))) ? itr->get<STRING_T>(_T("grade_prefix")) : _T("");
					m_wndRevNoDefineGridCtrl.SetItemText(row + 1,0,strValue.c_str());
					strValue = (soci::i_ok == itr->get_indicator(_T("grade_desc"))) ? itr->get<STRING_T>(_T("grade_desc")) : _T("");
					m_wndRevNoDefineGridCtrl.SetItemText(row + 1,1,strValue.c_str());
				}

				for(row = 1;row < m_wndRevNoDefineGridCtrl.GetRowCount();++row)
				{
					const CString sGrade = m_wndRevNoDefineGridCtrl.GetItemText(row , 0);
					strSQL.Format("SELECT * FROM T_REVISION_STAGE WHERE FROM_GRADE_DEFINE='%s' ORDER BY REVISION_NO ASC" , sGrade);
					soci::rowset<soci::row> rs(database.session()->prepare << (strSQL.operator LPCTSTR()));
					vector<STRING_T> oValueList;
					for(soci::rowset<soci::row>::const_iterator itr = rs.begin();itr != rs.end();++itr)
					{
						STRING_T strValue((soci::i_ok == itr->get_indicator(_T("revision_text"))) ? itr->get<STRING_T>(_T("revision_text")) : _T(""));
						oValueList.push_back(strValue);
					}
					if(oValueList.size() + 3 > m_wndRevNoDefineGridCtrl.GetColumnCount())
					{
						m_wndRevNoDefineGridCtrl.SetColumnCount(oValueList.size() + 3);
					}
					long col = 0;
					for(vector<STRING_T>::const_iterator itr = oValueList.begin();itr != oValueList.end();++itr,++col)
					{
						m_wndRevNoDefineGridCtrl.SetItemText(row,col + 2,itr->c_str());
					}
				}

				for(int col = 0;col < m_wndRevNoDefineGridCtrl.GetColumnCount();++col)
				{
					if(0 == col)
					{
						m_wndRevNoDefineGridCtrl.SetItemText(0,col,_T("NO"));
					}
					else if(1 == col)
					{
						m_wndRevNoDefineGridCtrl.SetItemText(0,col,_T("DESC."));
					}
					else if(m_wndRevNoDefineGridCtrl.GetColumnCount() - 1 == col)
					{
						m_wndRevNoDefineGridCtrl.SetItemText(0,col,_T("*"));
					}
					else
					{
						CString str;
						str.Format(_T("%d") , col - 1);
						m_wndRevNoDefineGridCtrl.SetItemText(0,col,str);
					}
				}
				m_wndRevNoDefineGridCtrl.AutoSizeColumns();
			}
		}
		catch(std::exception& ex)
		{
			AfxMessageBox(ex.what() , MB_OK | MB_ICONERROR);
		}

		/// 2011.08.20 modified by humkyung
		CProjectData* pProjectData = CProjectData::GetInstance();
		if(pProjectData)
		{
			map<CString,CString>::iterator where = pProjectData->m_pProjectSettingMap->find(_T("rev_drwn_by"));
			if(pProjectData->m_pProjectSettingMap->end() != where)
			{
				m_wndRevUserGridCtrl.SetItemText(1,0,where->second);
			}

			where = pProjectData->m_pProjectSettingMap->find(_T("rev_checked_by"));
			if(pProjectData->m_pProjectSettingMap->end() != where)
			{
				m_wndRevUserGridCtrl.SetItemText(1,1,where->second);
			}

			where = pProjectData->m_pProjectSettingMap->find(_T("rev_reviewed_by"));
			if(pProjectData->m_pProjectSettingMap->end() != where)
			{
				m_wndRevUserGridCtrl.SetItemText(1,2,where->second);
			}

			where = pProjectData->m_pProjectSettingMap->find(_T("rev_approved_by"));
			if(pProjectData->m_pProjectSettingMap->end() != where)
			{
				m_wndRevUserGridCtrl.SetItemText(1,3,where->second);
			}

			where = pProjectData->m_pProjectSettingMap->find(_T("select_rev_no"));
			if(pProjectData->m_pProjectSettingMap->end() != where)
			{
				if(_T("MANUAL") == where->second)
				{
					CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_MANUAL);
					if(pButton) pButton->SetCheck(TRUE);
				}
				else
				{
					CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_AUTOMATIC);
					if(pButton) pButton->SetCheck(TRUE);
				}
			}
			else
			{
				CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_AUTOMATIC);
				if(pButton) pButton->SetCheck(TRUE);
			}
		}
		/// up to here
	}

	return ERROR_SUCCESS;
}

/**	
	@brief	delete selected grade
	@author	HumKyung.BAEK
	@date	2010.03.04
	@return	void	
*/
void CRevisionStageDlg::OnButtonGradeDel() 
{
	CCellRange cRange = m_wndRevNoDefineGridCtrl.GetSelectedCellRange();
	for(int row = cRange.GetMaxRow();row >= cRange.GetMinRow();--row)
	{
		m_wndRevNoDefineGridCtrl.DeleteRow(row);
	}
}

/**	
	@brief	
	@author	HumKyung.BAEK
	@date	2010.03.04
	@return	void	
*/
CAdminDoc* CRevisionStageDlg::GetDocument()
{
	POSITION pos = AfxGetApp()->GetFirstDocTemplatePosition();
	CDocTemplate* pDocTemplate = AfxGetApp()->GetNextDocTemplate(pos);
	pos = pDocTemplate->GetFirstDocPosition();
	CAdminDoc* pDoc = (CAdminDoc*)pDocTemplate->GetNextDoc(pos);

	return pDoc;
}

/**	
	@brief	add a new grade
	@author	humkyung
	@date	2010.03.04
	@return	void	
*/
void CRevisionStageDlg::OnButtonGradeAdd() 
{
	m_wndRevNoDefineGridCtrl.SetRowCount(m_wndRevNoDefineGridCtrl.GetRowCount() + 1);
}

/**	
	@brief	save
	@author	humkyung
	@date	2010.03.04
	@return	void	
*/
int CRevisionStageDlg::SaveData()
{
	CAdminDoc* pDoc = GetDocument();
	if(pDoc)
	{
		CProject::DatabaseProp prop  = CAdminDoc::GetInstance()->GetCurProject()->GetDatabaseProp();
		try
		{
			CAppPostgreSQL database(CAdminDoc::GetInstance()->GetCurProject()->name() , prop.Host.c_str() , prop.User.c_str() , prop.Password.c_str());
			try
			{
				soci::transaction txn(*database.session());
				(*database.session()) << _T("DELETE FROM T_GRADE_DEFINE");
				for(int row = 1;row < m_wndRevNoDefineGridCtrl.GetRowCount();++row)
				{
					const CString sGradePrefix = m_wndRevNoDefineGridCtrl.GetItemText(row , 0);
					const CString sGradeDesc   = m_wndRevNoDefineGridCtrl.GetItemText(row , 1);
					CString sql;
					sql.Format(_T("INSERT INTO T_GRADE_DEFINE(GRADE_PREFIX,GRADE_DESC,SHOW_ON_REVISION_TABLE) VALUES('%s','%s',1)") , sGradePrefix , sGradeDesc);
					(*database.session()) << sql.operator LPCTSTR();
				}
				txn.commit();
			}
			catch(std::exception ex)
			{
				AfxMessageBox(ex.what() , MB_OK|MB_ICONERROR);
			}

			try
			{
				soci::transaction txn(*database.session());
				(*database.session()) << _T("DELETE FROM T_REVISION_STAGE");
				for(int row = 1;row < m_wndRevNoDefineGridCtrl.GetRowCount();++row)
				{
					const CString sGradePrefix = m_wndRevNoDefineGridCtrl.GetItemText(row , 0);
					for(int col = 2;col < m_wndRevNoDefineGridCtrl.GetColumnCount() - 1;++col)
					{
						int iRevNo = row*100 + col-2;
						const CString sRevText = m_wndRevNoDefineGridCtrl.GetItemText(row , col);
						if(sRevText.IsEmpty()) break;
						CString sql;
						sql.Format(_T("INSERT INTO T_REVISION_STAGE(FROM_GRADE_DEFINE,REVISION_NO,REVISION_TEXT) VALUES('%s',%d,'%s')") , sGradePrefix , iRevNo , sRevText);
						(*database.session()) << sql.operator LPCTSTR();
					}
				}
				txn.commit();
			}
			catch(std::exception& ex)
			{
				AfxMessageBox(ex.what() , MB_OK|MB_ICONERROR);
			}
		}
		catch(std::exception ex)
		{
		}
		
		CProjectData* pProjectData = CProjectData::GetInstance();
		if(NULL != pProjectData)
		{
			CString sDrwnBy = m_wndRevUserGridCtrl.GetItemText(1,0);
			CString sCheckedBy = m_wndRevUserGridCtrl.GetItemText(1,1);
			CString sReviewedBy = m_wndRevUserGridCtrl.GetItemText(1,2);
			CString sApprovedBy = m_wndRevUserGridCtrl.GetItemText(1,3);

			(*(pProjectData->m_pProjectSettingMap))[_T("rev_drwn_by")] = sDrwnBy;
			(*(pProjectData->m_pProjectSettingMap))[_T("rev_checked_by")] = sCheckedBy;
			(*(pProjectData->m_pProjectSettingMap))[_T("rev_reviewed_by")] = sReviewedBy;
			(*(pProjectData->m_pProjectSettingMap))[_T("rev_approved_by")] = sApprovedBy;

			CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_MANUAL);
			CString sSelectRevNo = (TRUE == pButton->GetCheck()) ? _T("MANUAL") : _T("AUTOMATIC");
			/// up to here
			(*(pProjectData->m_pProjectSettingMap))[_T("select_rev_no")] = sSelectRevNo;
		}
	}

	return ERROR_SUCCESS;
}

BOOL CRevisionStageDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	if(wParam == m_wndRevNoDefineGridCtrl.GetDlgCtrlID())
	{
		NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)(lParam);
		if(GVN_ENDLABELEDIT == pGridView->hdr.code)
		{
			if((pGridView->iColumn == m_wndRevNoDefineGridCtrl.GetColumnCount() - 1 ))
			{
				CString rItemText = m_wndRevNoDefineGridCtrl.GetItemText(pGridView->iRow , pGridView->iColumn);
				const int nColumnCount = m_wndRevNoDefineGridCtrl.GetColumnCount();
				m_wndRevNoDefineGridCtrl.SetColumnCount(nColumnCount + 1);
					
				CString rStr;
				rStr.Format(_T("%d") , nColumnCount-2);
				m_wndRevNoDefineGridCtrl.SetItemFormat(0 , nColumnCount-1, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
				m_wndRevNoDefineGridCtrl.SetItemText(0 , nColumnCount-1 , rStr);
				
				m_wndRevNoDefineGridCtrl.SetItemFormat(0 , nColumnCount , DT_CENTER|DT_VCENTER|DT_SINGLELINE);
				m_wndRevNoDefineGridCtrl.SetItemText(0 , nColumnCount , _T("*"));
				
				m_wndRevNoDefineGridCtrl.AutoSizeColumn(nColumnCount-1);
				m_wndRevNoDefineGridCtrl.AutoSizeColumn(nColumnCount);
			}
		}
	}

	return CDialog::OnNotify(wParam, lParam, pResult);
}

/**
	@brief	update layout for controls
	@author	humkyung
	@date	2015.11.28
*/
void CRevisionStageDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	CWnd* pWnd = GetDlgItem(IDC_STATIC_REV_NO_DEFINE);
	if(NULL != pWnd)
	{
		CRect rect;
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);
		pWnd->SetWindowPos(NULL,0,0,cx-rect.left*2,rect.Height(),SWP_NOMOVE|SWP_NOZORDER);
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);

		CRect rcButton;
		m_btnAddGrade.GetWindowRect(&rcButton);
		m_btnAddGrade.SetWindowPos(NULL,rect.right-rcButton.Width()*2-2,rect.top-rcButton.Height(),0,0,SWP_NOSIZE|SWP_NOZORDER);
		m_btnDelGrade.SetWindowPos(NULL,rect.right-rcButton.Width(),rect.top-rcButton.Height(),0,0,SWP_NOSIZE|SWP_NOZORDER);

		rect.DeflateRect(5,20,5,5);
		m_wndRevNoDefineGridCtrl.SetWindowPos(NULL,rect.left,rect.top,rect.Width(),rect.Height(),SWP_NOZORDER);
		m_wndRevNoDefineGridCtrl.GetWindowRect(&rect);
		ScreenToClient(&rect);
	}

	pWnd = GetDlgItem(IDC_STATIC_REV_USER);
	if(NULL != pWnd)
	{
		CRect rect;
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);
		pWnd->SetWindowPos(NULL,0,0,cx-rect.left*2,rect.Height(),SWP_NOMOVE|SWP_NOZORDER);
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);

		rect.DeflateRect(5,20,5,5);
		m_wndRevUserGridCtrl.SetWindowPos(NULL,rect.left,rect.top,rect.Width(),rect.Height(),SWP_NOZORDER);
	}
}
