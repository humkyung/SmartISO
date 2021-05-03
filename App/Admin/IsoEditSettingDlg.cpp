// IsoEditSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include <Tokenizer.h>
#include "Admin.h"
#include "IsoEditSettingDlg.h"
#include "FileNameFormatDlg.h"
#include "enablebuddybutton.h"
#include "AdminDocData.h"
#include "BMFieldTypeGridCell.h"
#include "BorderSettingDlg.h"
#include <ProjectData.h>
#include <DgnText.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIsoEditSettingDlg dialog


CIsoEditSettingDlg::CIsoEditSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIsoEditSettingDlg::IDD, pParent) , m_pAdminDoc(NULL)  , m_pFont(NULL)
{
	//{{AFX_DATA_INIT(CIsoEditSettingDlg)
	//}}AFX_DATA_INIT
}

CIsoEditSettingDlg::~CIsoEditSettingDlg()
{
	try
	{
		SAFE_DELETE(m_pFont);
	}
	catch(...)
	{

	}
}

void CIsoEditSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIsoEditSettingDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX , IDC_BUTTON_INPUT_FILE_NAME , m_inputFileNameButton);
	DDX_Control(pDX , IDC_EDIT_INPUT_FILE_NAME , m_InputFileNameFormatEdit);
	DDX_Control(pDX , IDC_BUTTON_OUTPUT_FILE_NAME , m_outputFileNameButton);
	DDX_Text(pDX , IDC_EDIT_INPUT_FILE_NAME , m_rInputFileNameFormat);
	DDX_Control(pDX , IDC_EDIT_OUTPUT_FILE_NAME , m_OutputFileNameFormatEdit);
	DDX_Text(pDX , IDC_EDIT_OUTPUT_FILE_NAME , m_rOutputFileNameFormat);
	/// 2011.08.18 added by humkyung
	DDX_Control(pDX , IDC_BUTTON_OUTPUT_FILE_EXT , m_wndOutputFileExtButton);
	DDX_Control(pDX , IDC_EDIT_OUTPUT_FILE_EXT , m_OutputFileExtFormatEdit);
	DDX_Text(pDX , IDC_EDIT_OUTPUT_FILE_EXT , m_rOutputFileExtFormat);
	/// up to here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIsoEditSettingDlg, CDialog)
	//{{AFX_MSG_MAP(CIsoEditSettingDlg)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_INPUT_FILE_NAME, OnButtonInputFileName)
	ON_BN_CLICKED(IDC_BUTTON_OUTPUT_FILE_NAME, OnButtonOutputFileName)
	ON_BN_CLICKED(IDC_BUTTON_OUTPUT_FILE_EXT, OnButtonOutputFileExt)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_LEVEL , &CIsoEditSettingDlg::OnBnClickedCheckLevel)
	ON_BN_CLICKED(IDC_CHECK_COLOR , &CIsoEditSettingDlg::OnBnClickedCheckColor)
	ON_BN_CLICKED(IDC_CHECK_STYLE , &CIsoEditSettingDlg::OnBnClickedCheckStyle)
	ON_BN_CLICKED(IDC_CHECK_WEIGHT, &CIsoEditSettingDlg::OnBnClickedCheckWeight)
	ON_WM_HELPINFO()
	ON_BN_CLICKED(IDC_RADIO_COLUMN_DATA, &CIsoEditSettingDlg::OnBnClickedRadioColumnData)
	ON_BN_CLICKED(IDC_RADIO_MATL_DESC, &CIsoEditSettingDlg::OnBnClickedRadioMatlDesc)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CIsoEditSettingDlg::OnBnClickedButtonRefresh)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIsoEditSettingDlg message handlers

void CIsoEditSettingDlg::OnSize(UINT nType, int cx, int cy) 
{
	const int gap = 10;
		
	CWnd* pWnd = GetDlgItem(IDC_STATIC_DATABASE_REFERENCE_KEY);
	pWnd = GetDlgItem(IDC_STATIC_GROUP_BOX1);
	if(pWnd)
	{
		CRect rect;
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);

		pWnd->SetWindowPos(NULL , rect.left , rect.top , cx - rect.left - gap, rect.Height(), SWP_NOZORDER);
	}

	pWnd = GetDlgItem(IDC_EDIT_INPUT_FILE_NAME);
	if(pWnd)
	{
		CRect rect;
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);

		pWnd->SetWindowPos(NULL , rect.left , rect.top , cx - rect.left - gap * 2, rect.Height(), SWP_NOZORDER);
	}
	
	pWnd = GetDlgItem(IDC_EDIT_OUTPUT_FILE_NAME);
	if(pWnd)
	{
		CRect rect[2];
		pWnd->GetWindowRect(&rect[0]);
		ScreenToClient(&rect[0]);

		pWnd->SetWindowPos(NULL , rect[0].left , rect[0].top , int((cx - rect[0].left - gap * 2) * 0.5) , rect[0].Height(), SWP_NOZORDER);
		pWnd->GetWindowRect(&rect[0]);
		ScreenToClient(&rect[0]);

		/// 2011.08.18 added by humkyung
		pWnd = GetDlgItem(IDC_STATIC_DOT);
		if(pWnd)
		{
			pWnd->SetFont(m_pFont);
			pWnd->SetWindowPos(NULL , rect[0].right , int((rect[0].top + rect[0].bottom) * 0.5) , 0 , 0 , SWP_NOZORDER | SWP_NOSIZE);
			
			pWnd->GetWindowRect(&rect[1]);
			ScreenToClient(&rect[1]);
		}

		pWnd = GetDlgItem(IDC_EDIT_OUTPUT_FILE_EXT);
		if(pWnd) pWnd->SetWindowPos(NULL , rect[1].right , rect[0].top , rect[0].Width() - gap , rect[0].Height(), SWP_NOZORDER);
		/// up to here
	}
	
	/// 2011.08.31 added by humkyung
	pWnd = GetDlgItem(IDC_GROUPBOX_BM_FORMAT);
	if(pWnd)
	{
		CRect rect;

		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);

		pWnd->SetWindowPos(NULL,0,0,int((cx - rect.left*2)),rect.Height(),SWP_NOMOVE|SWP_NOZORDER);
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);

		m_wndBMFormatGridCtrl.SetWindowPos(NULL , rect.left+gap , rect.top+gap*2 , rect.Width() - gap*2 , rect.Height() - gap*3 , SWP_NOZORDER);
	}
	/// up to here

	/// 2011.08.29 added by humkyung
	pWnd = GetDlgItem(IDC_GROUPBOX_HEADER_INFO);
	if(pWnd)
	{
		CRect rect , self;
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);

		pWnd->SetWindowPos(NULL ,0,0,int((cx - rect.left*2)),cy-rect.top,SWP_NOZORDER|SWP_NOMOVE);
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);

		pWnd = GetDlgItem(IDC_STATIC_HEADER_TEXT_GRID_CTRL);
		pWnd->GetWindowRect(&self);
		ScreenToClient(&self);
		m_wndHeaderInfoGridCtrl.SetWindowPos(NULL,self.left,self.top,self.Width(),rect.Height() - (self.top - rect.top) - gap,SWP_NOZORDER);
		m_wndHeaderInfoGridCtrl.GetWindowRect(&rect);
		ScreenToClient(&rect);

		pWnd = GetDlgItem(IDC_BUTTON_REFRESH);
		pWnd->SetWindowPos(NULL,rect.right+gap,rect.top+(rect.Height()-16)*0.5,0,0,SWP_NOZORDER|SWP_NOSIZE);

		if(m_wndBOMTexts.GetSafeHwnd())
		{
			m_wndHeaderInfoGridCtrl.GetWindowRect(&rect);
			ScreenToClient(&rect);
			m_wndBOMTexts.SetWindowPos(NULL,rect.right+16+gap*3,rect.top,cx-rect.right-16-gap*5,rect.Height(),SWP_NOZORDER);
		}
	}
	/// up to here

	CDialog::OnSize(nType, cx, cy);
}

/**	
	@brief	The CIsoEditSettingDlg::Save function
	@author	HumKyung.BAEK
	@return	void	
*/
void CIsoEditSettingDlg::Save()
{
	CProject* pProject = m_pAdminDoc->GetCurProject();
	if(NULL != pProject)
	{
		CString rIniFilePath = pProject->path() + CString( _T("\\Setting\\") ) + pProject->name() + CString( _T(".ini") );
		
		CAdminDoc* pDoc = GetDocument();
		if(pDoc)
		{
			CString rString;
			CProjectData* pProjectData = CProjectData::GetInstance();
			if(NULL != pProjectData)
			{
				/// save bolt length configuraion - 2012.08.27 added by humkyung
				if(pProjectData->m_pProjectSettingMap->end() != pProjectData->m_pProjectSettingMap->find(_T("bolt_length_location")))
				{
					CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_COLUMN_DATA);
					if(pButton->GetCheck()) rString = _T("1");
					pButton = (CButton*)GetDlgItem(IDC_RADIO_MATL_DESC);
					if(pButton->GetCheck()) rString = _T("2");
					(*(pProjectData->m_pProjectSettingMap))[_T("bolt_length_location")] = rString;
				}
				if(pProjectData->m_pProjectSettingMap->end() != pProjectData->m_pProjectSettingMap->find(_T("bolt_length_keyword")))
				{
					GetDlgItemText(IDC_EDIT_KEYWORD_FOR_BOLT_LENGTH , rString);
					(*(pProjectData->m_pProjectSettingMap))[_T("bolt_length_keyword")] = rString;
				}
				/// up to here
			}
		}

		const CString rApp( _T("Iso Edit Setting") );
		
		CString rString;
		GetDlgItemText(IDC_EDIT_ISO_DWG_FOLDER , rString);
		//CheckFolder(rString);
		if(!rString.IsEmpty())
		{
			WritePrivateProfileString(rApp , "Source Iso File Folder" , rString , rIniFilePath);
		}
		
		GetDlgItemText(IDC_EDIT_OUTPUT_ISO_DWG_FOLDER , rString);
		if(!rString.IsEmpty())
		{
			WritePrivateProfileString(rApp , "Output Iso Dwg File Folder" , rString , rIniFilePath);
		}

		GetDlgItemText(IDC_EDIT_INPUT_FILE_NAME , rString);
		if(!rString.IsEmpty())
		{
			rString.Replace(" ~ " , ":");
		}
		WritePrivateProfileString(rApp , "Input File Name Format" , rString , rIniFilePath);

		GetDlgItemText(IDC_EDIT_OUTPUT_FILE_NAME , rString);
		if(!rString.IsEmpty())
		{
			rString.Replace(" ~ " , ":");
		}
		WritePrivateProfileString(rApp , "Output File Name Format" , rString , rIniFilePath);

		/// 2011.08.19 added by humkyung
		GetDlgItemText(IDC_EDIT_OUTPUT_FILE_EXT , rString);
		if(!rString.IsEmpty())
		{
			rString.Replace(_T(" ~ ") , _T(":"));
		}
		WritePrivateProfileString(rApp , _T("Output File Ext Format") , rString , rIniFilePath);
		/// up to here

		/// 2011.08.29 added by humkyung
		map<STRING_T,STRING_T>* pDrawingSetup = CProjectData::GetInstance()->GetDrawingSetup();
		for(int i = 1;i < m_wndHeaderInfoGridCtrl.GetRowCount();++i)
		{
			const CString sItemText = m_wndHeaderInfoGridCtrl.GetItemText(i,0).MakeUpper();
			const CString sIndex = m_wndHeaderInfoGridCtrl.GetItemText(i,1);
			if(!sIndex.IsEmpty())
			{
				const int iIndex = ATOI_T(sIndex);
				int iWidth = INT_MAX;
				for(int j = 1;j < m_wndHeaderInfoGridCtrl.GetColumnCount();++j)
				{
					if(i == j) continue;
					const CString sItemText = m_wndHeaderInfoGridCtrl.GetItemText(1 , j);
					if(sItemText.IsEmpty()) continue;
					const int iNextStart = ATOI_T(sItemText);
					if(iIndex > iNextStart) continue;
					const int d = iNextStart - iIndex;
					iWidth = (d < iWidth) ? d : iWidth;
				}
				
				CString sValue;
				sValue.Format(_T("%d,%d") , iIndex, (INT_MAX == iWidth) ? -1 : iWidth - 1);
				(*pDrawingSetup)[(sItemText + _T(" COL DEF")).operator LPCTSTR()] = sValue.operator LPCTSTR();
			}
			else
			{
				(*pDrawingSetup)[(sItemText + _T(" COL DEF")).operator LPCTSTR()] = _T("");
			}
		}
		/// up to here

		/// 2011.08.31 added by humkyung
		CString sBOMFieldFormat;
		for(int i = 0;i < m_wndBMFormatGridCtrl.GetColumnCount();++i)
		{
			const CString sItemText = m_wndBMFormatGridCtrl.GetItemText(1 , i);
			sBOMFieldFormat += sItemText + _T(",");
		}
		if(_T(",") == sBOMFieldFormat.Right(1)) sBOMFieldFormat = sBOMFieldFormat.Left(sBOMFieldFormat.GetLength() - 1);
		(*pDrawingSetup)[_T("BOM Field Format")] = sBOMFieldFormat;
		/// up to here

		/// 2011.11.02 added by humkyung
		{
			CString sValue;
			CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK_LEVEL);
			if(pButton->GetCheck())
			{
				GetDlgItemText(IDC_EDIT_LEVEL , rString);
				sValue += _T("LEVEL:") + rString + _T(",");
			}
			pButton = (CButton*)GetDlgItem(IDC_CHECK_COLOR);
			if(pButton->GetCheck())
			{
				GetDlgItemText(IDC_EDIT_COLOR , rString);
				sValue += _T("COLOR:") + rString + _T(",");
			}
			pButton = (CButton*)GetDlgItem(IDC_CHECK_STYLE);
			if(pButton->GetCheck())
			{
				GetDlgItemText(IDC_EDIT_STYLE , rString);
				sValue += _T("STYLE:") + rString + _T(",");
			}
			pButton = (CButton*)GetDlgItem(IDC_CHECK_WEIGHT);
			if(pButton->GetCheck())
			{
				GetDlgItemText(IDC_EDIT_WEIGHT , rString);
				sValue += _T("WEIGHT:") + rString;
			}
			WritePrivateProfileString(rApp , _T("Text Modified Error(Exception)") , sValue , rIniFilePath);
		}
		/// up to here
	}
}

/**	
	@brief	다이얼로그 초기화 부분.
	@author	HumKyung.BAEK
**/
BOOL CIsoEditSettingDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_pFont = new CFont;
	m_pFont->CreateFont(16,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY , 0 , _T("Arial"));

	short shBtnColor = 30;
	m_inputFileNameButton.SetGradientColor(DarkenColor(GetSysColor(COLOR_BTNFACE),0.15) , GetSysColor(COLOR_BTNFACE) , CButtonST::VertBar);
	m_inputFileNameButton.SetBtnCursor(IDC_HAND_CUR);
	m_inputFileNameButton.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_inputFileNameButton.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(0, 128, 0));
	m_inputFileNameButton.SetFlat(FALSE);

	m_outputFileNameButton.SetGradientColor(DarkenColor(GetSysColor(COLOR_BTNFACE),0.15) , GetSysColor(COLOR_BTNFACE) , CButtonST::VertBar);
	m_outputFileNameButton.SetBtnCursor(IDC_HAND_CUR);
	m_outputFileNameButton.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_outputFileNameButton.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(0, 128, 0));
	m_outputFileNameButton.SetFlat(FALSE);
	
	HWND hwndEdit  = GetDlgItem(IDC_EDIT_INPUT_FILE_NAME)->GetSafeHwnd();
	HWND hwndButton= GetDlgItem(IDC_BUTTON_INPUT_FILE_NAME)->GetSafeHwnd();
	VERIFY( ::EnableBuddyButton( hwndEdit, hwndButton, BBS_LEFT ) );
	m_InputFileNameFormatEdit.SetFont(m_pFont);

	hwndEdit  = GetDlgItem(IDC_EDIT_OUTPUT_FILE_NAME)->GetSafeHwnd();
	hwndButton= GetDlgItem(IDC_BUTTON_OUTPUT_FILE_NAME)->GetSafeHwnd();
	VERIFY( ::EnableBuddyButton( hwndEdit, hwndButton, BBS_LEFT ) );
	m_OutputFileNameFormatEdit.SetFont(m_pFont);

	/// 2011.08.18 added by humkyung
	m_wndOutputFileExtButton.SetGradientColor(DarkenColor(GetSysColor(COLOR_BTNFACE),0.15) , GetSysColor(COLOR_BTNFACE) , CButtonST::VertBar);
	m_wndOutputFileExtButton.SetBtnCursor(IDC_HAND_CUR);
	m_wndOutputFileExtButton.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	m_wndOutputFileExtButton.SetColor(CButtonST::BTNST_COLOR_FG_IN, RGB(0, 128, 0));
	m_wndOutputFileExtButton.SetFlat(FALSE);

	hwndEdit  = GetDlgItem(IDC_EDIT_OUTPUT_FILE_EXT)->GetSafeHwnd();
	hwndButton= GetDlgItem(IDC_BUTTON_OUTPUT_FILE_EXT)->GetSafeHwnd();
	VERIFY( ::EnableBuddyButton( hwndEdit, hwndButton, BBS_LEFT ) );
	m_OutputFileExtFormatEdit.SetFont(m_pFont);
	/// up to here

	/// 2011.08.18 added by humkyung
	BOOL b = m_wndHeaderInfoGridCtrl.Create(CRect(0,0,0,0),this,0x100);
	if(TRUE == b)
	{
		m_wndHeaderInfoGridCtrl.SetFont(m_pFont);

		/// fill it up with stuff
		m_wndHeaderInfoGridCtrl.SetEditable(TRUE);
		m_wndHeaderInfoGridCtrl.EnableDragAndDrop(FALSE);
		m_wndHeaderInfoGridCtrl.SetTextBkColor(RGB(255, 255, 224));
		m_wndHeaderInfoGridCtrl.SetRowResize(FALSE);
		m_wndHeaderInfoGridCtrl.SetColumnResize(TRUE);
		m_wndHeaderInfoGridCtrl.SetListMode(FALSE);
		m_wndHeaderInfoGridCtrl.SetSingleRowSelection(TRUE);
		m_wndHeaderInfoGridCtrl.SetHeaderSort(FALSE);
		m_wndHeaderInfoGridCtrl.SetSingleColSelection(TRUE);

		const CString HeaderTextArray[] = 
		{
			_T("NO") , _T("MATL DESC") , _T("SIZE") , _T("MATL SPEC") , _T("SYM") , _T("MATL CODE") , _T("BOLT LEN") , _T("QTY")
		};

		m_wndHeaderInfoGridCtrl.SetRowCount(SIZE_OF_ARRAY(HeaderTextArray)+1);
		m_wndHeaderInfoGridCtrl.SetColumnCount(2);
		m_wndHeaderInfoGridCtrl.SetFixedRowCount(1);
		m_wndHeaderInfoGridCtrl.SetFixedColumnCount(1);
		for(int i = 0;i < SIZE_OF_ARRAY(HeaderTextArray);++i)
		{
			m_wndHeaderInfoGridCtrl.SetItemFormat(i+1,0,DT_LEFT|DT_VCENTER|DT_SINGLELINE);
			m_wndHeaderInfoGridCtrl.SetItemText(i+1,0,HeaderTextArray[i]);
		}
		m_wndHeaderInfoGridCtrl.SetItemFormat(0,1,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
		m_wndHeaderInfoGridCtrl.SetItemText(0,1,_T("Index"));

		m_wndHeaderInfoGridCtrl.AutoSizeRows();	
		m_wndHeaderInfoGridCtrl.AutoSizeColumns();
	}
	/// up to here
	if(TRUE == m_wndBOMTexts.Create(NULL,_T(""),WS_CHILD|WS_VISIBLE|WS_BORDER,CRect(0,0,0,0),this,WM_USER+0x100))
	{
	}

	/// 2011.08.31 added by humkyung
	if(TRUE == m_wndBMFormatGridCtrl.Create(CRect(0,0,0,0),this,0x100))
	{
		m_wndBMFormatGridCtrl.SetFont(m_pFont);

		/// fill it up with stuff
		m_wndBMFormatGridCtrl.SetEditable(TRUE);
		m_wndBMFormatGridCtrl.EnableDragAndDrop(FALSE);
		m_wndBMFormatGridCtrl.SetTextBkColor(RGB(255, 255, 224));
		m_wndBMFormatGridCtrl.SetRowResize(FALSE);
		m_wndBMFormatGridCtrl.SetColumnResize(TRUE);
		m_wndBMFormatGridCtrl.SetListMode(FALSE);
		m_wndBMFormatGridCtrl.SetSingleRowSelection(TRUE);
		m_wndBMFormatGridCtrl.SetHeaderSort(FALSE);
		m_wndBMFormatGridCtrl.SetSingleColSelection(TRUE);

		m_wndBMFormatGridCtrl.SetRowCount(2);
		m_wndBMFormatGridCtrl.SetColumnCount(10);
		m_wndBMFormatGridCtrl.SetFixedRowCount(1);

		const CString HeaderTextArray[] = {_T("A") , _T("B") , _T("C") , _T("D") , _T("E") , _T("F") , _T("G") , _T("H") , _T("I") , _T("J")};
		for(int i = 0;i < SIZE_OF_ARRAY(HeaderTextArray);++i)
		{
			m_wndBMFormatGridCtrl.SetItemFormat(0, i , DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_wndBMFormatGridCtrl.SetItemText(0 , i , HeaderTextArray[i]);
			
			m_wndBMFormatGridCtrl.SetCellType(1 , i , RUNTIME_CLASS(GridCell::CBMFieldTypeGridCell));
			m_wndBMFormatGridCtrl.SetColumnWidth(i , 80);
		}
		m_wndBMFormatGridCtrl.AutoSizeRows();	
		m_wndBMFormatGridCtrl.SetRowHeight(1 , 25);
	}
	/// up to here

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CIsoEditSettingDlg::OnDestroy() 
{
	CDialog::OnDestroy();
}

void CIsoEditSettingDlg::SetLocalFolderOut(const int& nId , CString folder)
{
	UpdateData(FALSE);
}

/**
	@brief	display bom column definition
	@author	humkyung
	@date	2015.11.26
*/
int CIsoEditSettingDlg::DisplayBOMCOLDef()
{
	m_wndBOMTexts.m_oBmTexts.clear();
	if(CBorderSettingDlg::GetInstance()->GetSafeHwnd())
	{
		CDgnVolume oBOMArea = CBorderSettingDlg::GetInstance()->GetBOMArea();

		CDgnDocument* pDgnDoc = CBorderSettingDlg::GetInstance()->GetDgnDocument();
		for(int i = 0;i < pDgnDoc->GetEntityCount();++i)
		{
			CDgnElement* pDgnEnt = pDgnDoc->GetEntityAt(i);
			if(pDgnEnt->IsKindOf(CDgnText::TypeString()))
			{
				CDgnText* pDgnText = static_cast<CDgnText*>(pDgnEnt);
				if(oBOMArea.Contains(pDgnText->origin()))
				{
					m_wndBOMTexts.m_oBmTexts.insert(make_pair(pDgnText->origin().y,pDgnText->textString()));
				}
			}
		}

		m_wndBOMTexts.m_oBmHeaderIndices.clear();
		for(int i = 1;i < m_wndHeaderInfoGridCtrl.GetRowCount();++i)
		{
			const CString sItemText = m_wndHeaderInfoGridCtrl.GetItemText(i,1).MakeUpper();
			m_wndBOMTexts.m_oBmHeaderIndices.push_back(ATOI_T(sItemText));
		}
		m_wndBOMTexts.Invalidate();

		return ERROR_SUCCESS;
	}

	return ERROR_BAD_ENVIRONMENT;
}

/**	
	@brief	프로젝트가 로드되고 난뒤에 초기화한다.
	@author	humkyung
	@date	????.??.??
	@return	void	
*/
void CIsoEditSettingDlg::UpdateContents()
{
	CProject* pProject = m_pAdminDoc->GetCurProject();
	if(NULL != pProject)
	{
		CString rIniFilePath = pProject->GetIniFilePath();
		
		TCHAR szBuf[MAX_PATH + 1] = {'\0' ,};
		CString rApp( _T("Iso Edit Setting") );

		if(GetPrivateProfileString(rApp , _T("Input File Name Format") , NULL , szBuf , MAX_PATH , rIniFilePath))
		{
			m_rInputFileNameFormat = szBuf;
			m_rInputFileNameFormat.Replace(":" , " ~ ");
		}

		if(GetPrivateProfileString(rApp , _T("Output File Name Format") , NULL , szBuf , MAX_PATH , rIniFilePath))
		{
			m_rOutputFileNameFormat = szBuf;
			m_rOutputFileNameFormat.Replace(":" , " ~ ");
		}

		/// 2011.08.19 added by humkyung
		if(GetPrivateProfileString(rApp , _T("Output File Ext Format") , NULL , szBuf , MAX_PATH , rIniFilePath))
		{
			m_rOutputFileExtFormat = szBuf;
			m_rOutputFileExtFormat.Replace(_T(":") , _T(" ~ "));
		}
		/// up to here

		/// 2011.08.29 added by humkyung
		map<STRING_T,STRING_T>* pDrawingSetup = CProjectData::GetInstance()->GetDrawingSetup();
		for(int i = 1;i < m_wndHeaderInfoGridCtrl.GetRowCount();++i)
		{
			CString sItemText = m_wndHeaderInfoGridCtrl.GetItemText(i,0).MakeUpper();
			map<STRING_T,STRING_T>::const_iterator where = pDrawingSetup->find((sItemText + _T(" COL DEF")).operator LPCTSTR());
			if(where != pDrawingSetup->end())
			{
				vector<STRING_T> oResult;
				CTokenizer<CIsComma>::Tokenize(oResult ,where->second.c_str(), CIsComma());
				if(2 == oResult.size())
				{
					m_wndHeaderInfoGridCtrl.SetItemFormat(i,1,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
					m_wndHeaderInfoGridCtrl.SetItemText(i,1,oResult[0].c_str());
				}
			}
		}
		DisplayBOMCOLDef();
		/// up to here

		/// 2011.08.31 added by humkyung
		map<STRING_T,STRING_T>::const_iterator where = pDrawingSetup->find(_T("BOM Field Format"));
		if(where != pDrawingSetup->end())
		{
			int col = 0;
			vector<STRING_T> oResult;
			CTokenizer<CIsComma>::Tokenize(oResult,where->second,CIsComma());
			for(vector<STRING_T>::const_iterator itr = oResult.begin();(itr != oResult.end()) && (col < m_wndBMFormatGridCtrl.GetColumnCount());++itr,++col)
			{
				m_wndBMFormatGridCtrl.SetItemFormat(1,col,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
				m_wndBMFormatGridCtrl.SetItemText(1,col,itr->c_str());
			}
			m_wndBMFormatGridCtrl.AutoSizeColumns();
		}
		/// up to here

		/// 2011.11.02 added by humkyung
		if(GetPrivateProfileString(rApp , _T("Text Modified Error(Exception)") , NULL , szBuf , MAX_PATH , rIniFilePath))
		{
			vector<STRING_T> oResult;
			CTokenizer<CIsComma>::Tokenize(oResult , szBuf , CIsComma());
			for(vector<STRING_T>::iterator itr = oResult.begin();itr != oResult.end();++itr)
			{
				vector<STRING_T> oTokens;
				CTokenizer<CIsFromString>::Tokenize(oTokens , (*itr) , CIsFromString(_T(":")));
				if(2 == oTokens.size())
				{
					if(_T("LEVEL") == oTokens[0])
					{
						((CButton*)GetDlgItem(IDC_CHECK_LEVEL))->SetCheck(TRUE);
						GetDlgItem(IDC_EDIT_LEVEL)->EnableWindow(TRUE);
						SetDlgItemText(IDC_EDIT_LEVEL , oTokens[1].c_str());
					}
					else if(_T("COLOR") == oTokens[0])
					{
						((CButton*)GetDlgItem(IDC_CHECK_COLOR))->SetCheck(TRUE);
						GetDlgItem(IDC_EDIT_COLOR)->EnableWindow(TRUE);
						SetDlgItemText(IDC_EDIT_COLOR , oTokens[1].c_str());
					}
					else if(_T("STYLE") == oTokens[0])
					{
						((CButton*)GetDlgItem(IDC_CHECK_STYLE))->SetCheck(TRUE);
						GetDlgItem(IDC_EDIT_STYLE)->EnableWindow(TRUE);
						SetDlgItemText(IDC_EDIT_STYLE , oTokens[1].c_str());
					}
					else if(_T("WEIGHT") == oTokens[0])
					{
						((CButton*)GetDlgItem(IDC_CHECK_WEIGHT))->SetCheck(TRUE);
						GetDlgItem(IDC_EDIT_WEIGHT)->EnableWindow(TRUE);
						SetDlgItemText(IDC_EDIT_WEIGHT , oTokens[1].c_str());
					}
				}
			}
		}
		/// up to here

		/// display bolt length configuration - 2012.08.27 added by humkyung
		CProjectData* pProjectData = CProjectData::GetInstance();
		if(pProjectData->m_pProjectSettingMap->end() != pProjectData->m_pProjectSettingMap->find(_T("BOLT_LENGTH_LOCATION")))
		{
			CString rString = (*pProjectData->m_pProjectSettingMap)[_T("BOLT_LENGTH_LOCATION")];
			if(_T("1") == rString)
			{
				CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_COLUMN_DATA);
				pButton->SetCheck(TRUE);
			}
			else if(_T("2") == rString)
			{
				CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_MATL_DESC);
				pButton->SetCheck(TRUE);
			}
		}
		if(pProjectData->m_pProjectSettingMap->end() != pProjectData->m_pProjectSettingMap->find(_T("BOLT_LENGTH_KEYWORD")))
		{
			CString rString = (*pProjectData->m_pProjectSettingMap)[_T("BOLT_LENGTH_KEYWORD")];
			SetDlgItemText(IDC_EDIT_KEYWORD_FOR_BOLT_LENGTH , rString);
		}
		/// up to here

		UpdateData( FALSE );
	}
	m_inputFileNameButton.SetFocus();
}

void CIsoEditSettingDlg::OnButtonInputFileName()
{
	CAdminApp* pApp = (CAdminApp*)AfxGetApp();
	///CResourceSwitch resSwitch(pApp->m_hMultiLangRes);

	CFileNameFormatDlg dlg;
	dlg.m_rFileNameFormat = m_rInputFileNameFormat;
	dlg.m_pAdminDoc = m_pAdminDoc;
	if(IDOK == dlg.DoModal())
	{
		m_rInputFileNameFormat = dlg.m_rFileNameFormat;
		CString rFileNameFormat(m_rInputFileNameFormat);
		rFileNameFormat.Replace(":" , " ~ ");
		m_InputFileNameFormatEdit.SetWindowText(rFileNameFormat);
	}
}

void CIsoEditSettingDlg::OnButtonOutputFileName() 
{
	CAdminApp* pApp = (CAdminApp*)AfxGetApp();
	///CResourceSwitch resSwitch(pApp->m_hMultiLangRes);

	CFileNameFormatDlg dlg;
	dlg.m_rFileNameFormat = m_rOutputFileNameFormat;
	dlg.m_pAdminDoc = m_pAdminDoc;
	if(IDOK == dlg.DoModal())
	{
		m_rOutputFileNameFormat = dlg.m_rFileNameFormat;
		CString rFileNameFormat(m_rOutputFileNameFormat);
		rFileNameFormat.Replace(":" , " ~ ");
		m_OutputFileNameFormatEdit.SetWindowText(rFileNameFormat);
	}
}

/******************************************************************************
    @author     humkyung
    @date       2011-08-18
    @class      CIsoEditSettingDlg
    @function   OnButtonOutputFileExt
    @return     void
    @brief
******************************************************************************/
void CIsoEditSettingDlg::OnButtonOutputFileExt() 
{
	CAdminApp* pApp = (CAdminApp*)AfxGetApp();

	CFileNameFormatDlg dlg(this , _T("GRADE,REV_NO"));
	dlg.m_rFileNameFormat = m_rOutputFileExtFormat;
	dlg.m_pAdminDoc = m_pAdminDoc;
	if(IDOK == dlg.DoModal())
	{
		m_rOutputFileExtFormat = dlg.m_rFileNameFormat;
		CString rFileExtFormat(m_rOutputFileExtFormat);
		rFileExtFormat.Replace(_T(":") , _T(" ~ "));
		m_OutputFileExtFormatEdit.SetWindowText(rFileExtFormat);
	}
}

void CIsoEditSettingDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if( VK_F5 == nChar )
	{


	}

	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CIsoEditSettingDlg::CheckFolder(const CString &rString)
{
	CFileStatus status;
	CString rIso = rString + "Iso";
	CString rIsoOut = rString + "Iso\\out";
	CString rSpec = rString + "spec";
	CString rIsoBmData = rString + "SmartISO";
	CString rIsoBmDataPlot = rIsoBmData +"\\Plot";
	CString rIsoBmDataPlotData = rIsoBmData +"\\Plot_data";
	
	if(!CFile::GetStatus( rIso , status))
	{
		AfxMessageBox("Iso 폴더가 없습니다.");
	}
	if(!CFile::GetStatus( rIsoOut , status))
	{
		AfxMessageBox("Iso\\Out 폴더가 없습니다.");
	}
	if(!CFile::GetStatus( rSpec , status))
	{
		AfxMessageBox("Spec 폴더가 없습니다.");
	}
	if(!CFile::GetStatus( rIsoBmData , status))
	{
		AfxMessageBox("SmartISO폴더가 없습니다.");
	}
	if(!CFile::GetStatus( rIsoBmDataPlot , status))
	{
		AfxMessageBox("SmartISO\\plot 폴더가 없습니다.");
	}
	if(!CFile::GetStatus( rIsoBmDataPlotData , status))
	{
		AfxMessageBox("SmartISO\\plot_data 폴더가 없습니다.");
	}
}

BOOL CIsoEditSettingDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN ) // Enter Key Prevent.. so good.. ^^		
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE )		
		{
			::TranslateMessage(pMsg);
			
			::DispatchMessage(pMsg);
			
			return TRUE;                    // DO NOT process further	
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

/**	
	@brief	
	
	@author	HumKyung.BAEK

	@date	2010.03.04

	@return	void	
*/
CAdminDoc* CIsoEditSettingDlg::GetDocument()
{
	POSITION pos = AfxGetApp()->GetFirstDocTemplatePosition();
	CDocTemplate* pDocTemplate = AfxGetApp()->GetNextDocTemplate(pos);
	pos = pDocTemplate->GetFirstDocPosition();
	CAdminDoc* pDoc = (CAdminDoc*)pDocTemplate->GetNextDoc(pos);

	return pDoc;
}

void CIsoEditSettingDlg::OnBnClickedCheckLevel()
{
	GetDlgItem(IDC_EDIT_LEVEL)->EnableWindow( ((CButton*)GetDlgItem(IDC_CHECK_LEVEL))->GetCheck() );
}

void CIsoEditSettingDlg::OnBnClickedCheckColor()
{
	GetDlgItem(IDC_EDIT_COLOR)->EnableWindow( ((CButton*)GetDlgItem(IDC_CHECK_COLOR))->GetCheck() );
}

void CIsoEditSettingDlg::OnBnClickedCheckStyle()
{
	GetDlgItem(IDC_EDIT_STYLE)->EnableWindow( ((CButton*)GetDlgItem(IDC_CHECK_STYLE))->GetCheck() );
}

void CIsoEditSettingDlg::OnBnClickedCheckWeight()
{
	GetDlgItem(IDC_EDIT_WEIGHT)->EnableWindow( ((CButton*)GetDlgItem(IDC_CHECK_WEIGHT))->GetCheck() );
}

/******************************************************************************
    @author     humkyung
    @date       2012-06-05
    @class      CIsoEditSettingDlg
    @function   OnHelpInfo
    @return     BOOL
    @brief
******************************************************************************/
BOOL CIsoEditSettingDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	HWND hHtml = ::HtmlHelp(this->m_hWnd , CAdminDocData::GetExecPath() + _T("\\Help\\SmartISO.chm::/ISO Product Setting.htm") , HH_DISPLAY_TOPIC , 0);

	return TRUE;///CDialog::OnHelpInfo(pHelpInfo);
}

/******************************************************************************
    @author     humkyung
    @date       2012-08-27
    @function   OnBnClickedRadioColumnData
    @return     void
    @brief
******************************************************************************/
void CIsoEditSettingDlg::OnBnClickedRadioColumnData()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_EDIT_KEYWORD_FOR_BOLT_LENGTH);
	if(pButton) pButton->EnableWindow(FALSE);
	for(int i = 0;i < m_wndHeaderInfoGridCtrl.GetColumnCount();++i)
	{
		CString rString = m_wndHeaderInfoGridCtrl.GetItemText(0 , i);
		if(_T("BOLT LEN") == rString)
		{
			m_wndHeaderInfoGridCtrl.SetItemState(1 , i , m_wndHeaderInfoGridCtrl.GetItemState(1 , i) & ~GVNI_READONLY);
		}
	}
}

/******************************************************************************
    @author     humkyung
    @date       2012-08-27
    @function   OnBnClickedRadioMatlDesc
    @return     void
    @brief
******************************************************************************/
void CIsoEditSettingDlg::OnBnClickedRadioMatlDesc()
{
	CButton* pButton = (CButton*)GetDlgItem(IDC_EDIT_KEYWORD_FOR_BOLT_LENGTH);
	if(pButton) pButton->EnableWindow(TRUE);
	for(int i = 0;i < m_wndHeaderInfoGridCtrl.GetColumnCount();++i)
	{
		CString rString = m_wndHeaderInfoGridCtrl.GetItemText(0 , i);
		if(_T("BOLT LEN") == rString)
		{
			m_wndHeaderInfoGridCtrl.SetItemState(1 , i , GVNI_READONLY);
		}
	}
}

/**
	@brief	display bom column index when bom column index is changed
	@author	humkyung
	@date	2015.11.26
**/
BOOL CIsoEditSettingDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	if(wParam == m_wndHeaderInfoGridCtrl.GetDlgCtrlID())
	{
		NM_GRIDVIEW* pGridView = (NM_GRIDVIEW*)(lParam);
		if(GVN_ENDLABELEDIT == pGridView->hdr.code)
		{
			/// update T_LINE_LIST_DATA_MAPPING table - 2012.05.09 added by humkyung
			if(1 == pGridView->iColumn)
			{
				m_wndBOMTexts.m_oBmHeaderIndices.clear();
				for(int i = 1;i < m_wndHeaderInfoGridCtrl.GetRowCount();++i)
				{
					const CString sItemText = m_wndHeaderInfoGridCtrl.GetItemText(i,1).MakeUpper();
					m_wndBOMTexts.m_oBmHeaderIndices.push_back(ATOI_T(sItemText));
				}
				m_wndBOMTexts.Invalidate();
			}
		}
	}
	
	return CDialog::OnNotify(wParam, lParam, pResult);
 }

/**
	@brief	display bom texts and column index
	@author	humkyung
	@date	2015.11.26
**/
void CIsoEditSettingDlg::OnBnClickedButtonRefresh()
{
	DisplayBOMCOLDef();
}
