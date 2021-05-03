// QueryClauseListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UserApp.h"
#include "QueryClauseListDlg.h"
#include "UserAppDocData.h"
#include "MainFrm.h"

// CQueryClauseListDlg dialog

IMPLEMENT_DYNAMIC(CQueryClauseListDlg, CDialog)

CQueryClauseListDlg::CQueryClauseListDlg(CString sQueryClause , CWnd* pParent /*=NULL*/)
	: CDialog(CQueryClauseListDlg::IDD, pParent)
{
	m_sQueryClause = sQueryClause;
}

CQueryClauseListDlg::~CQueryClauseListDlg()
{
}

void CQueryClauseListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX , IDC_LIST_QUERY_CLAUSE , m_wndQueryClauseListCtrl);
}


BEGIN_MESSAGE_MAP(CQueryClauseListDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CQueryClauseListDlg::OnBnClickedOk)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_QUERY_CLAUSE, &CQueryClauseListDlg::OnLvnKeydownListQueryClause)
END_MESSAGE_MAP()


// CQueryClauseListDlg message handlers
BOOL CQueryClauseListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_wndQueryClauseListCtrl.InsertColumn(0 , _T("Name") , 0 , 150);
	m_wndQueryClauseListCtrl.InsertColumn(1 , _T("Query Clause") , 0, 250);

	CUserAppDocData* pDocData = CUserAppDocData::GetInstance();
	for(list<CUserAppDocData::QueryClause>::iterator itr = pDocData->m_oQueryClauseList.begin();itr != pDocData->m_oQueryClauseList.end();++itr)
	{
		const int iItem = m_wndQueryClauseListCtrl.InsertItem(m_wndQueryClauseListCtrl.GetItemCount() , itr->sName);
		m_wndQueryClauseListCtrl.SetItemText(iItem , 1 , itr->sQueryClause);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/******************************************************************************
    @author     humkyung
    @date       2012-08-14
    @function   OnBnClickedOk
    @return     void
    @brief
******************************************************************************/
void CQueryClauseListDlg::OnBnClickedOk()
{
	CString rString;
	GetDlgItemText(IDC_EDIT_QUERY_CLAUSE_NAME , rString);
	if(!rString.IsEmpty())
	{
		for(int i = 0;i < m_wndQueryClauseListCtrl.GetItemCount();++i)
		{
			if(rString == m_wndQueryClauseListCtrl.GetItemText(i , 0))
			{
				AfxMessageBox(_T("Please, there is same clause name") , MB_OK | MB_ICONWARNING);
				return;
			}
		}

		CUserAppDocData* pDocData = CUserAppDocData::GetInstance();
		if(pDocData)
		{
			const CString strFilePath = pDocData->m_ProjectPath;
			const CString strFileName = pDocData->m_ProjectName;
			
			CADODB adoDB;
			STRING_T rConnectionString = pDocData->GetConnectionString();
			if(adoDB.DBConnect(rConnectionString))
			{
				STRING_T sSql = _T("INSERT INTO T_QUERY_CLAUSE_LIST(C_NAME) VALUES('") + rString + _T("')");
				if(TRUE == adoDB.ExecuteQuery(sSql))
				{
					sSql = _T("SELECT C_QUERY_CLAUSE FROM T_QUERY_CLAUSE_LIST WHERE C_NAME='" + rString + "'");
					adoDB.OpenQuery(sSql);
					LONG lRecordCount = 0L;
					adoDB.GetRecordCount(&lRecordCount);
					if(1 == lRecordCount)
					{
						adoDB.AppendChunk(0 , 0 , (LPVOID)(m_sQueryClause.operator LPCTSTR()) , m_sQueryClause.GetLength());
					}
					adoDB.CloseRecordSet();

					const int iItem = m_wndQueryClauseListCtrl.InsertItem(m_wndQueryClauseListCtrl.GetItemCount() , rString);
					m_wndQueryClauseListCtrl.SetItemText(iItem , 1 , m_sQueryClause.operator LPCTSTR());

					{
						CUserAppDocData::QueryClause queryClause;
						queryClause.sName = rString;
						queryClause.sQueryClause = m_sQueryClause;
						pDocData->m_oQueryClauseList.push_back(queryClause);
					}
				}
			}
			{
				GetUserAppMainWnd()->GetWorkspaceBar()->m_wndQuery1.FillQueryClauseList();
				GetUserAppMainWnd()->GetWorkspaceBar()->m_wndQuery2.FillQueryClauseList();
				GetUserAppMainWnd()->GetWorkspaceBar()->m_wndQuery3.FillQueryClauseList();
				GetUserAppMainWnd()->GetWorkspaceBar()->m_wndQuery4.FillQueryClauseList();
			}
		}
	}
	else
	{
		AfxMessageBox(_T("Please, input query clause name") , MB_OK | MB_ICONWARNING);
	}

	///OnOK();
}

struct MatchQueryClauseName : public binary_function<CUserAppDocData::QueryClause , CString , bool>
{
	bool operator()(const CUserAppDocData::QueryClause& lhs , const CString& rhs) const
	{
		return (lhs.sName == rhs);
	}
};

/******************************************************************************
    @author     humkyung
    @date       2012-08-14
    @function   OnLvnKeydownListQueryClause
    @return     void
    @param      NMHDR   *pNMHDR
    @param      LRESULT *pResult
    @brief
******************************************************************************/
void CQueryClauseListDlg::OnLvnKeydownListQueryClause(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	
	if(VK_DELETE == pLVKeyDow->wVKey)
	{
		if(1 == m_wndQueryClauseListCtrl.GetSelectedCount())
		{
			if(IDYES == AfxMessageBox(_T("Do you want to delete selected query clause?") , MB_YESNO | MB_ICONQUESTION))
			{
				CUserAppDocData* pDocData = CUserAppDocData::GetInstance();
				if(pDocData)
				{
					const CString strFilePath = pDocData->m_ProjectPath;
					const CString strFileName = pDocData->m_ProjectName;

					CADODB adoDB;
					STRING_T rConnectionString = pDocData->GetConnectionString();
					if(adoDB.DBConnect(rConnectionString))
					{
						POSITION pos = m_wndQueryClauseListCtrl.GetFirstSelectedItemPosition();
						while(pos)
						{
							const int iItem = m_wndQueryClauseListCtrl.GetNextSelectedItem(pos);
							CString sQueryClauseName = m_wndQueryClauseListCtrl.GetItemText(iItem , 0);
							m_wndQueryClauseListCtrl.DeleteItem(iItem);

							STRING_T sSql = CString(_T("DELETE * FROM T_QUERY_CLAUSE_LIST WHERE C_NAME='") + sQueryClauseName + _T("'")).operator LPCTSTR();
							adoDB.ExecuteQuery(sSql);

							pDocData->m_oQueryClauseList.erase(
								remove_if(pDocData->m_oQueryClauseList.begin() , pDocData->m_oQueryClauseList.end() , bind2nd(MatchQueryClauseName() , sQueryClauseName)) , pDocData->m_oQueryClauseList.end());
						}
					}

					{
						GetUserAppMainWnd()->GetWorkspaceBar()->m_wndQuery1.FillQueryClauseList();
						GetUserAppMainWnd()->GetWorkspaceBar()->m_wndQuery2.FillQueryClauseList();
						GetUserAppMainWnd()->GetWorkspaceBar()->m_wndQuery3.FillQueryClauseList();
						GetUserAppMainWnd()->GetWorkspaceBar()->m_wndQuery4.FillQueryClauseList();
					}
				}
			}
		}
	}
	
	*pResult = 0;
}
