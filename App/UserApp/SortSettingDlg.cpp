// SortSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "UserApp.h"
#include "SortSettingDlg.h"
#include "tokenizer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSortSettingDlg dialog


CSortSettingDlg::CSortSettingDlg(CString sTitle , CWnd* pParent /*=NULL*/)
	: CDialog(CSortSettingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSortSettingDlg)
		m_sTitle = sTitle;
	//}}AFX_DATA_INIT
}

CSortSettingDlg::~CSortSettingDlg()
{
	try
	{
		delete m_pFont;
		m_pFont = NULL;
	}
	catch(...)
	{
		
	}
}
void CSortSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSortSettingDlg)
	DDX_Control(pDX , IDC_LIST_SORT_NAME , m_FileNameEditListBox);
	DDX_Control(pDX , IDOK , m_btnOK);
	DDX_Control(pDX , IDCANCEL , m_btnCancel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSortSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CSortSettingDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSortSettingDlg message handlers

BOOL CSortSettingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	this->SetWindowText(m_sTitle);

	CAppDocData*  pDocData  = CAppDocData::GetInstance();
	vector<STRING_T> oResultSort;
	if(NULL != pDocData)
	{
		TCHAR szBuf[MAX_PATH] = {'\0' ,};
		CString rIniFilePath = pDocData->GetProject()->GetIniFilePath();
		CString rApp = _T("Line Data Field");
		
		int nCount = 0;
		if(GetPrivateProfileString(rApp , _T("Count") , NULL , szBuf , MAX_PATH , rIniFilePath))
		{
			nCount = atoi(szBuf);
		}

		m_FieldDescEntry.clear();
		vector<string> oResult;
		CString rKey;
		for(int i = 0;i < nCount;i++)
		{
			rKey.Format("NAME%d" , i);
			if(GetPrivateProfileString(rApp , rKey , NULL , szBuf , MAX_PATH , rIniFilePath))
			{
				CTokenizer<CIsFromString>::Tokenize(oResult , szBuf , CIsFromString(":"));
				m_FieldDescEntry.push_back(oResult[1].c_str());
			}
		}
		rApp = _T("Sorting Set");
		if(GetPrivateProfileString(rApp , _T("Sort Key") , NULL,szBuf,MAX_PATH,rIniFilePath))
		{
			CTokenizer<CIsFromString>::Tokenize(oResultSort,szBuf,CIsFromString(":"));
		}
	}

	m_pFont = new CFont;
	m_pFont->CreateFont(16,0,0,0,600,0,0,0,0,0,0,ANTIALIASED_QUALITY,0, _T("Arial"));

	m_FileNameEditListBox.SetThemeHelper(&m_themeHelperST);
	m_EditListBoxBuddy.SubclassDlgItem( IDC_LISTSORTBUDDY, this );
	m_EditListBoxBuddy.SetListbox( &m_FileNameEditListBox );
	
	vector<STRING_T> oResult;
	m_FileNameEditListBox.SetFont(m_pFont);
	m_rFileNameFormat.Replace(_T(" ~ ") , _T(":"));
	CTokenizer<CIsFromString>::Tokenize(oResult , m_rFileNameFormat.operator const char*() , CIsFromString(":"));
	vector<string>::iterator itr = oResult.begin();
	for(itr = oResult.begin();itr != oResult.end();++itr)
	{
		m_FileNameEditListBox.AddString(itr->c_str());
	}
	m_FileNameEditListBox.DeleteString(0);
	for(itr = oResultSort.begin();itr != oResultSort.end();++itr)
	{
		m_FileNameEditListBox.AddString(itr->c_str());
	}

	HICON hIcon = (HICON) LoadImage(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_OK),IMAGE_ICON,0,0,0);
	HICON hOldIcon = m_btnOK.SetIcon(hIcon);
	if(hOldIcon) DestroyIcon(hOldIcon);

	hIcon = (HICON) LoadImage(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_CANCEL),IMAGE_ICON,0,0,0);
	hOldIcon = m_btnCancel.SetIcon(hIcon);
	if(hOldIcon) DestroyIcon(hOldIcon);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSortSettingDlg::OnOK() 
{
	// TODO: Add extra validation here
	CAppDocData* pDocData = CAppDocData::GetInstance();
	CString rIniFilePath = pDocData->GetProject()->GetIniFilePath();
	m_rFileNameFormat.Empty();
	
	CString rString;
	const int nCount = m_FileNameEditListBox.GetCount();
	for(int i = 0;i < nCount;i++)
	{
		m_FileNameEditListBox.GetText(i , rString);
		m_rFileNameFormat += rString;
		if(i < nCount - 1) m_rFileNameFormat += ":";
		m_rFileNameFormat.MakeUpper();
	}
	
	CDialog::OnOK();
}
