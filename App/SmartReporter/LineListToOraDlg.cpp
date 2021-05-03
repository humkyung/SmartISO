// LineListToOraDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SmartReporter.h"
#include "enablebuddybutton.h"
#include "LineListToOraDlg.h"
#include "ExcelMapingGridCell.h"

#include "Tokenizer.h"

#include "LineListToOraDocData.h"
#include <sstream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const STRING_T CLineListToOraDlg::APP( _T("MAPPING") );
/////////////////////////////////////////////////////////////////////////////
// CLineListToOraDlg dialog


CLineListToOraDlg::CLineListToOraDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLineListToOraDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLineListToOraDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CLineListToOraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLineListToOraDlg)
	DDX_Control( pDX , IDC_EDIT_STD_NOTE_FILE_PATH , m_wndStdNoteFilePath );
	DDX_Control( pDX , IDC_BUTTON_STD_NOTE , m_wndSelectStdNoteFile );
	DDX_Control( pDX , IDC_EDIT_DESIGN_DLL_FILE_PATH , m_wndDesignDDLFilePath );
	DDX_Control( pDX , IDC_BUTTON_DESIGN_DDL , m_wndSelectDesignDDLFile );
	DDX_Control(pDX , IDC_LIST_KEY , m_wndKeyListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLineListToOraDlg, CDialog)
	//{{AFX_MSG_MAP(CLineListToOraDlg)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_DESIGN_DDL, OnButtonDesignDdl)
	///ON_CBN_SELENDOK(IDC_COMBO_KEY, OnSelendokComboKey)
	///ON_CBN_SELCHANGE(IDC_COMBO_KEY, OnSelchangeComboKey)
	ON_BN_CLICKED(IDC_BUTTON_STD_NOTE, OnButtonStdNote)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLineListToOraDlg message handlers

BOOL CLineListToOraDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	VERIFY( ::EnableBuddyButton( m_wndStdNoteFilePath.m_hWnd , m_wndSelectStdNoteFile.m_hWnd , BBS_RIGHT ) );
	VERIFY( ::EnableBuddyButton( m_wndDesignDDLFilePath.m_hWnd , m_wndSelectDesignDDLFile.m_hWnd , BBS_RIGHT ) );

	BOOL b = m_wndGridCtrl.Create(CRect(0,0,0,0),this,0x100);
	if(TRUE == b)
	{
		//그리드 컨트롤 폰트 코드
		CFont font;
		font.CreateFont(12,0,0,0,400,FALSE,FALSE,FALSE,HANGUL_CHARSET,3,2,1,VARIABLE_PITCH | FF_MODERN, _T("굴림") );
		m_wndGridCtrl.SetFont(&font);
		font.DeleteObject();
		
		///m_ImageList.Create(IDB_ADMIN_TREE , 16 , 16 , RGB(255 , 255 , 255));
		///m_wndGridCtrl.SetImageList(&m_ImageList);

		// fill it up with stuff
		m_wndGridCtrl.SetEditable(TRUE);
		m_wndGridCtrl.EnableDragAndDrop(FALSE);
		m_wndGridCtrl.SetTextBkColor(RGB(255, 255, 224));
		m_wndGridCtrl.SetRowResize(FALSE);
		m_wndGridCtrl.SetColumnResize(TRUE);
		m_wndGridCtrl.SetListMode(FALSE);
		m_wndGridCtrl.SetSingleRowSelection(TRUE);
		m_wndGridCtrl.SetHeaderSort(FALSE);
		m_wndGridCtrl.SetSingleColSelection(TRUE);

		m_wndGridCtrl.SetFixedRowCount( 1 );
		m_wndGridCtrl.SetFixedColumnCount( 1 );
		/////////////////////////////////////////////////////////////////

		InitKeyListCtrl();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/**
**/
void CLineListToOraDlg::OnSize(UINT nType, int cx, int cy) 
{
	const int GAP = 8;

	CDialog::OnSize(nType, cx, cy);
	
	if(m_wndGridCtrl.GetSafeHwnd())
	{
		CRect rect;
		/*
		CWnd* pWnd = GetDlgItem(IDC_STATIC_LINE_DATA);
		if(pWnd)
		{
			CRect rect;
			pWnd->SetWindowPos(NULL , 0 , gap , cx , (cy * 0.5) - 73 - 10 , SWP_NOZORDER);
			pWnd->GetWindowRect(&rect);
			ScreenToClient(&rect);
			
			m_ButtonDel.SetWindowPos(NULL , rect.left + gap , rect.top + 16 , 0 , 0 , SWP_NOSIZE);
			m_ButtonDel.GetWindowRect(&rect);
			ScreenToClient(&rect);
			
			m_ButtonSave.SetWindowPos(NULL , rect.right + 1, rect.top , 0 , 0 , SWP_NOSIZE);
		}
		*/
		if( m_wndStdNoteFilePath.GetSafeHwnd() )
		{
			m_wndStdNoteFilePath.GetWindowRect( &rect );
			ScreenToClient( &rect );

			m_wndStdNoteFilePath.SetWindowPos( NULL , 0 , 0 , cx - rect.left , rect.Height() , SWP_NOMOVE | SWP_NOZORDER );
		}

		if( m_wndDesignDDLFilePath.GetSafeHwnd() )
		{
			m_wndDesignDDLFilePath.GetWindowRect( &rect );
			ScreenToClient( &rect );

			m_wndDesignDDLFilePath.SetWindowPos( NULL , 0 , 0 , cx - rect.left , rect.Height() , SWP_NOMOVE | SWP_NOZORDER );
		}

		if( m_wndKeyListCtrl.GetSafeHwnd() )
		{
			m_wndKeyListCtrl.GetWindowRect( &rect );
			ScreenToClient( &rect );

			m_wndKeyListCtrl.SetWindowPos( NULL , 0 , 0 , cx - rect.left , rect.Height() , SWP_NOMOVE | SWP_NOZORDER );
		}

		m_wndGridCtrl.SetWindowPos(NULL , 0 , rect.bottom + GAP , cx , cy - rect.bottom - GAP , SWP_NOZORDER);
	}	
}

/**	
	@brief	design ddl 파일을 선택한다.

	@author	humkyung

	@date	????.??.??
**/
void CLineListToOraDlg::OnButtonDesignDdl()
{
	CFileDialog dlg(TRUE,_T(""),_T("*.ddl"),OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,
		_T("Design DDL document(*.ddl)||All documents(*.*)|*.*||")); 
	if( IDOK == dlg.DoModal() )
	{
		CString rDefFilePath = dlg.GetPathName();
		m_wndDesignDDLFilePath.SetWindowText( rDefFilePath );

		m_OracleDefFile.Read( rDefFilePath.operator LPCTSTR() );
		
		FillKeyListCtrl(true);
		InitGridCtrl();
	}
}

/**
**/
void CLineListToOraDlg::InitGridCtrl()
{
	COraDatabaseDefFile::CFieldDef FieldDef;

	size_t nCount = m_OracleDefFile.GetFieldCount();
	m_wndGridCtrl.SetRowCount( 5 );
	m_wndGridCtrl.SetColumnCount( nCount + 1 );
	for( int i = 0;i < (int)nCount;++i)
	{
		m_OracleDefFile.GetFieldAt( FieldDef , i );
		m_wndGridCtrl.SetItemText( 0 , i + 1 , CString(FieldDef.m_rFieldName.c_str()) );
		CGridCellBase* pCell = m_wndGridCtrl.GetCell(0 , i + 1);
		if(pCell) pCell->SetTextClr(RGB(0 , 0 ,0));
		
		for( int j = 1;j < 5;++j)
		{
			m_wndGridCtrl.SetCellType( j , i + 1 , RUNTIME_CLASS(CExcelMapingGridCell));
		}
	}

	m_wndGridCtrl.SetItemText( 1 , 0 , _T("1") );
	m_wndGridCtrl.SetItemText( 2 , 0 , _T("2") );
	m_wndGridCtrl.SetItemText( 3 , 0 , _T("3") );
	m_wndGridCtrl.SetItemText( 4 , 0 , _T("4") );

	m_wndGridCtrl.AutoSizeRows();
	m_wndGridCtrl.AutoSizeColumns();

	UpdateGridCell();
}

void CLineListToOraDlg::OnSelendokComboKey() 
{
	UpdateGridCell();
}

/**	
	@brief	현재 설정된 키값를 표현한다.
	
	@remarks
	키값 앞에 *를 붙인다.
**/
void CLineListToOraDlg::UpdateGridCell()
{
	const int nColCount = m_wndGridCtrl.GetColumnCount();

	/// 모든 라벨에서 *을 제거한다. 텍스트 색깔을 검은색으로 한다.
	for( int i = 1;i < nColCount;++i)
	{
		CString rCellText = m_wndGridCtrl.GetItemText( 0 , i );
		if("*" == rCellText.Left(1))
		{
			rCellText = rCellText.Right(rCellText.GetLength() - 1);
			CGridCell* pCell = (CGridCell*)(m_wndGridCtrl.GetCell( 0 , i ));
			if(pCell)
			{
				pCell->SetText(rCellText);
				pCell->SetTextClr(RGB(0 , 0 , 0));
			}
		}
	}

	/// 키값과 같은 라벨에 *을 앞에 넣고 텍스트 색깔을 붉은색으로 한다.
	/*
	for( i = 1;i < nColCount;++i)
	{
		CString rCellText = m_wndGridCtrl.GetItemText( 0 , i );
		if( rString == rCellText )
		{
			CGridCell* pCell = (CGridCell*)(m_wndGridCtrl.GetCell( 0 , i ));
			if(pCell)
			{
				pCell->SetText("*" + rCellText);
				pCell->SetTextClr(RGB(255 , 0 , 0));
			}
			break;
		}
	}
	*/

	m_wndGridCtrl.AutoSizeColumns();
	m_wndGridCtrl.RedrawWindow();
}

void CLineListToOraDlg::OnOK() 
{
	///CDialog::OnOK();
}

void CLineListToOraDlg::OnCancel() 
{
	///CDialog::OnOK();
}

/**
**/
void CLineListToOraDlg::SaveData()
{
	CLineListToOraDocData& docData = CLineListToOraDocData::GetInstance();
	const STRING_T rIniFilePath = docData.GetIniFilePath();

	CString rString;
	m_wndStdNoteFilePath.GetWindowText(rString);
	WritePrivateProfileString(_T("Iso Edit Setting") , _T("Standard Note File") , rString , rIniFilePath.c_str());

	m_wndDesignDDLFilePath.GetWindowText(rString);
	WritePrivateProfileString(_T("Oracle Setting") , _T("DDL FILE PATH") , rString , rIniFilePath.c_str());

	CString rKeyString;
	const int nCount = m_wndKeyListCtrl.GetItemCount();
	for(int i = 0;i < nCount;++i)
	{
		if(TRUE == m_wndKeyListCtrl.IsItemChecked(i))
		{
			rKeyString += m_wndKeyListCtrl.GetItemText(i , 0) + _T(":");
		}
	}
	if(":" == rKeyString.Right(1)) rKeyString = rKeyString.Left(rKeyString.GetLength() - 1);
	///m_wndKeyComboBox.GetWindowText( rString );
	WritePrivateProfileString( APP.c_str() , _T("Key") , rKeyString , rIniFilePath.c_str() );

	const int nColCount = m_wndGridCtrl.GetColumnCount();
	STRINGSTREAM_T oss;
	oss << (nColCount - 1);
	WritePrivateProfileString( APP.c_str() , _T("Col Count") , oss.str().c_str() , rIniFilePath.c_str() );

	for(int i = 1;i < nColCount;++i)
	{
		CString rName = m_wndGridCtrl.GetItemText( 0 , i );
		if(_T("*") == rName.Left(1)) rName = rName.Right(rName.GetLength() - 1);

		CString rValue;
		for(int j = 1;j < 5;++j)
		{
			rString = m_wndGridCtrl.GetItemText( j , i );
			if( !rString.IsEmpty() ) rValue += rString + _T("+");
		}
		if( _T("+") == rValue.Right(1) ) rValue = rValue.Left( rValue.GetLength() - 1);

		oss.str(_T(""));
		oss << _T("Col ") << i;
		WritePrivateProfileString( APP.c_str() , oss.str().c_str() , CString(rName + _T("=") + rValue).operator LPCTSTR() , rIniFilePath.c_str() );
	}
}

/**
	@brief	load data for initializing dialog

	@author	humkyung
**/
void CLineListToOraDlg::LoadData()
{
	TCHAR szBuf[MAX_PATH]={'\0' ,};

	CLineListToOraDocData& docData = CLineListToOraDocData::GetInstance();
	const STRING_T rIniFilePath = docData.GetIniFilePath();

	if(GetPrivateProfileString( _T("Iso Edit Setting") , _T("Standard Note File") , NULL , szBuf , MAX_PATH , rIniFilePath.c_str() ))
	{
		m_wndStdNoteFilePath.SetWindowText( szBuf );
	}

	if(GetPrivateProfileString( _T("Oracle Setting") , _T("DDL FILE PATH") , NULL , szBuf , MAX_PATH , rIniFilePath.c_str() ))
	{
		m_wndDesignDDLFilePath.SetWindowText( szBuf );

		m_OracleDefFile.Read( szBuf );

		FillKeyListCtrl(true);
		InitGridCtrl();
	}

	if(GetPrivateProfileString( CLineListToOraDlg::APP.c_str() , _T("Key") , NULL , szBuf , MAX_PATH , rIniFilePath.c_str() ))
	{
		vector<STRING_T> oResult;
		CTokenizer<CIsFromString>::Tokenize(oResult , szBuf , CIsFromString( _T(":") ));
		for(vector<STRING_T>::iterator itr = oResult.begin();itr != oResult.end();++itr)
		{
			LVFINDINFO info;
			info.flags =LVFI_PARTIAL | LVFI_STRING;
			info.psz = itr->c_str();
			
			int nItem = m_wndKeyListCtrl.FindItem(&info);
			if(-1 != nItem) m_wndKeyListCtrl.SetCheck(nItem);
		}		
	}

	if(GetPrivateProfileString( CLineListToOraDlg::APP.c_str() , _T("Col Count") , NULL , szBuf , MAX_PATH , rIniFilePath.c_str() ))
	{
		STRINGSTREAM_T oss;
		vector<STRING_T> oResult;
		const int nColCount = ATOI_T( szBuf );

		///m_wndGridCtrl.SetRowCount( 5 );
		///m_wndGridCtrl.SetColumnCount( nColCount + 1 );
		for( int i = 1;i <= nColCount;++i)
		{
			oss.str( _T("") );
			oss << _T("Col ") << i;
			if(GetPrivateProfileString(CLineListToOraDlg::APP.c_str() , oss.str().c_str() , NULL , szBuf , MAX_PATH , rIniFilePath.c_str() ))
			{
				CTokenizer<CIsFromString>::Tokenize(oResult , szBuf , CIsFromString( _T("=") ));
				m_wndGridCtrl.SetItemText(0 , i , oResult[0].c_str());
				
				if(oResult.size() >= 2)
				{
					int j = 1;
					CTokenizer<CIsFromString>::Tokenize(oResult , oResult[1].c_str() , CIsFromString( _T("+") ));
					for( vector<STRING_T>::iterator itr = oResult.begin();itr != oResult.end();++itr,++j)
					{
						m_wndGridCtrl.SetItemText(j , i , itr->c_str());
					}
				}
				for(int j = 1;j < 5;++j)
				{
					m_wndGridCtrl.SetCellType( j , i , RUNTIME_CLASS(CExcelMapingGridCell));
				}
			}
			
		}
	}

	m_wndGridCtrl.SetItemText( 1 , 0 , _T("1") );
	m_wndGridCtrl.SetItemText( 2 , 0 , _T("2") );
	m_wndGridCtrl.SetItemText( 3 , 0 , _T("3") );
	m_wndGridCtrl.SetItemText( 4 , 0 , _T("4") );
	
	UpdateGridCell();

	m_wndGridCtrl.AutoSizeRows();
	m_wndGridCtrl.AutoSizeColumns();
}

void CLineListToOraDlg::OnSelchangeComboKey() 
{
	UpdateGridCell();
}

/**
	@author	humkyung

	@date	????.??.??
**/
void CLineListToOraDlg::OnButtonStdNote() 
{
	CFileDialog dlg(TRUE,_T(""),_T("*.rpt"),OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,
		_T("Standard Note document(*.rpt)||All documents(*.*)|*.*||")); 
	if( IDOK == dlg.DoModal() )
	{
		CString rStdNoteFilePath = dlg.GetPathName();
		m_wndStdNoteFilePath.SetWindowText( rStdNoteFilePath );
	}	
}

/**
**/
void CLineListToOraDlg::InitKeyListCtrl()
{
	CString rHeadings = _T("Field Name , 256;");

	m_wndKeyListCtrl.SetHeadings(rHeadings);
	m_wndKeyListCtrl.SetGridLines();
	m_wndKeyListCtrl.SetCheckboxes();
}

/**
	@brief	

	@author	humkyung
**/
void CLineListToOraDlg::FillKeyListCtrl(const bool& NewData)
{
	static const STRING_T DEFAULT_KEYS[] =
	{
		_T("fluid_code") , _T("line_sequence_no")
	};
	int nSize = sizeof(DEFAULT_KEYS) / sizeof(DEFAULT_KEYS[0]);

	m_wndKeyListCtrl.DeleteAllItems();

	COraDatabaseDefFile::CFieldDef FieldDef;
	size_t nCount = m_OracleDefFile.GetFieldCount();
	for( int i = 0;i < (int)nCount;++i)
	{
		m_OracleDefFile.GetFieldAt( FieldDef , i );
		int nItem = m_wndKeyListCtrl.InsertItem(i , CString(FieldDef.m_rFieldName.c_str()));
		
		if(NewData)
		{
			const STRING_T* p = find(DEFAULT_KEYS , DEFAULT_KEYS + nSize , FieldDef.m_rFieldName);
			if((p - DEFAULT_KEYS) < nSize) m_wndKeyListCtrl.SetCheck(nItem , TRUE);
		}
	}
}
