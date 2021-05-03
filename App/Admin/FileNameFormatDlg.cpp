// FileNameFormatDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Admin.h"

#include "tokenizer.h"
#include "FileNameFormatDlg.h"
#include "AdminDocData.h"
#include <ado/SQLFile.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileNameFormatDlg dialog


CFileNameFormatDlg::CFileNameFormatDlg(CWnd* pParent /*=NULL*/ , const CString& sFieldDescEntry)
	: CDialog(CFileNameFormatDlg::IDD, pParent) , m_pAdminDoc(NULL) , m_pFont(NULL)
{
	//{{AFX_DATA_INIT(CFileNameFormatDlg)
	/// 2011.08.20 added by humkyung
	if(!sFieldDescEntry.IsEmpty())
	{
		vector<STRING_T> oResult;
		CTokenizer<CIsComma>::Tokenize(oResult , sFieldDescEntry.operator LPCTSTR() , CIsComma());
		for(vector<STRING_T>::iterator itr = oResult.begin();itr != oResult.end();++itr)
		{
			m_FieldDescEntry.push_back(itr->c_str());
		}
	}
	/// up to here
	//}}AFX_DATA_INIT
}

CFileNameFormatDlg::~CFileNameFormatDlg()
{
	try
	{
		SAFE_DELETE(m_pFont);
	}
	catch(...)
	{
		
	}
}

void CFileNameFormatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileNameFormatDlg)
	DDX_Control(pDX , IDC_EDIT_DELIMITER , m_DelimiterEdit);
	DDX_Control(pDX , IDC_BUTTON_ADD , m_AddButton);
	DDX_Control(pDX , IDC_LIST_FILE_NAME_ITEM , m_FileNameEditListBox);
	DDX_Control(pDX , IDOK , m_okButton);
	DDX_Control(pDX , IDCANCEL , m_cancelButton);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileNameFormatDlg, CDialog)
	//{{AFX_MSG_MAP(CFileNameFormatDlg)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileNameFormatDlg message handlers

BOOL CFileNameFormatDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if((NULL != m_pAdminDoc) && m_FieldDescEntry.empty() )
	{
		const CString rSqlFilePath = CAdminDocData::GetExecPath() + _T("\\Setting\\T_ISO_LINE_DATA.XML");
		CSQLFile sqlFile(rSqlFilePath.operator LPCSTR());
		if(ERROR_SUCCESS == sqlFile.ParseXML(rSqlFilePath.operator LPCTSTR()))
		{
			int nCount = sqlFile.GetColumnCount();

			m_FieldDescEntry.clear();
			for(int i = 0;i < nCount;++i)
			{
				const string rFieldName = sqlFile.GetFieldNameAt(i);
				m_FieldDescEntry.push_back(rFieldName.c_str());
			}
		}
	}
	
	m_pFont = new CFont;
	m_pFont->CreateFont(16,0,0,0,600,0,0,0,0,0,0,ANTIALIASED_QUALITY,0, _T("Arial") );

	m_DelimiterEdit.SetRegEx( _T("[A-Za-z-_*^]+") );
	m_DelimiterEdit.SetBackgroundColourFocus (RGB (150, 200, 255));
	m_DelimiterEdit.SetForegroundColourError (RGB (255, 255, 255));
	m_DelimiterEdit.SetBackgroundColourError (RGB (255, 0, 0));
	m_DelimiterEdit.SetFont(m_pFont);

	m_AddButton.SetIcon(IDI_ADD , 16 , 16);
	
	m_FileNameEditListBox.SetThemeHelper(&m_themeHelperST);
	m_EditListBoxBuddy.SubclassDlgItem( IDC_LISTBUDDY, this );
	m_EditListBoxBuddy.SetListbox( &m_FileNameEditListBox );
	int nCount = m_FileNameEditListBox.GetCount();
	for( int i = nCount - 1;i >= 0;--i) m_FileNameEditListBox.DeleteString( i );
	
	vector<string> oResult;
	m_FileNameEditListBox.SetFont(m_pFont);
	m_rFileNameFormat.Replace( _T(" ~ ") , _T(":") );
	if( !m_rFileNameFormat.IsEmpty() )
	{
		CTokenizer<CIsFromString>::Tokenize(oResult , m_rFileNameFormat.operator const char*() , CIsFromString( _T(":") ));
		for(vector<string>::iterator itr = oResult.begin();itr != oResult.end();++itr)
		{
			m_FileNameEditListBox.AddString(itr->c_str());
		}
	}

	m_okButton.SetIcon(IDI_OK/* , 16 , 16*/);
	///m_okButton.SetBtnCursor(IDC_HAND_CUR);
	///m_okButton.SetFlat(FALSE);

	m_cancelButton.SetIcon(IDI_DEL/* , 32 , 32*/);
	///m_cancelButton.SetBtnCursor(IDC_HAND_CUR);
	///m_cancelButton.SetFlat(FALSE);

	m_ctrlTT.Create(this);
	m_ctrlTT.AddTool(&m_DelimiterEdit,_T("파일이름 구분자 입니다."));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFileNameFormatDlg::OnButtonAdd() 
{
	int nSelItem = m_FileNameEditListBox.GetCurSel();
	if(-1 != nSelItem)
	{
		CString rString;
		GetDlgItemText(IDC_EDIT_DELIMITER , rString);
		if(!rString.IsEmpty())
		{
			nSelItem = m_FileNameEditListBox.InsertString(nSelItem + 1, rString);
			m_FileNameEditListBox.SetCurSel(nSelItem);
		}
	}
}

void CFileNameFormatDlg::OnOK() 
{
	// TODO: Add extra validation here
	m_rFileNameFormat.Empty();
	
	CString rString;
	const int nCount = m_FileNameEditListBox.GetCount();
	for(int i = 0;i < nCount;i++)
	{
		m_FileNameEditListBox.GetText(i , rString);
		m_rFileNameFormat += rString;
		if(i < nCount - 1) m_rFileNameFormat += ":";
	}

	CDialog::OnOK();
}

BOOL CFileNameFormatDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_ctrlTT.RelayEvent(pMsg);
	return CDialog::PreTranslateMessage(pMsg);
}
