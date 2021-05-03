// IssueDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IssueDlg.h"
#include "afxdialogex.h"
#include "UserAppDocData.h"
#include <extend/libcsv/inc/csv_writer.hpp>

// CIssueDlg dialog

IMPLEMENT_DYNAMIC(CIssueDlg, CDialogEx)

CIssueDlg::CIssueDlg(const CString& sOutputDrawingFolder,vector<LineDataRecordMap>& aLineDataRecordEntry,CWnd* pParent /*=NULL*/) : CDialogEx(CIssueDlg::IDD, pParent)
{
	m_sOutputDrawingFolder = sOutputDrawingFolder;
	m_oLineDataRecordEntry.insert(m_oLineDataRecordEntry.begin(),aLineDataRecordEntry.begin(),aLineDataRecordEntry.end());
}

CIssueDlg::~CIssueDlg()
{
}

void CIssueDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX,IDC_LIST_LINE_DATA,m_wndLineDataReport);
	DDX_Control(pDX,IDC_BUTTON_EXPORT,m_btnExport);
	DDX_Control(pDX,IDOK,m_btnOK);
	DDX_Control(pDX,IDCANCEL,m_btnCancel);
}


BEGIN_MESSAGE_MAP(CIssueDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CIssueDlg::OnBnClickedOk)
	ON_UPDATE_COMMAND_UI(IDC_BUTTON_EXPORT, OnUpdateButtonExport)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, &CIssueDlg::OnBnClickedButtonExport)
END_MESSAGE_MAP()


// CIssueDlg message handlers

BOOL CIssueDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	CAppDocData* pDocData = CAppDocData::GetInstance();

	m_wndLineDataReport.SetHeadings(_T("[],30;") + _TR("File Name") + _T(",200;Rev No,30;Status,50"));
	if(TRUE == m_imgList.Create(IDB_CHECKBOXES, 16, 3, RGB(255,0,255)))
	{
		m_wndLineDataReport.SetHeaderImageList(&m_imgList);
		m_wndLineDataReport.SetExtendedStyle(LVS_EX_CHECKBOXES | m_wndLineDataReport.GetExtendedStyle());
		m_wndLineDataReport.EnableHeaderCheckBox(TRUE);
	}

	LVITEM lvitem;
	lvitem.mask = LVIF_TEXT | LVIF_IMAGE;
	lvitem.iImage = 0;
	lvitem.cchTextMax = 32;
	for(vector<LineDataRecordMap>::const_iterator itr = m_oLineDataRecordEntry.begin();itr != m_oLineDataRecordEntry.end();++itr)
	{
		lvitem.iItem = m_wndLineDataReport.GetItemCount();
		lvitem.iSubItem = 0;
		lvitem.pszText = _T("");
		const int at = m_wndLineDataReport.InsertItem(&lvitem);
		if((-1 != at) && (NULL != itr->pLineDataRecord))
		{
			const CString sIsoFileName = pDocData->GetProject()->GetOutputIsoFileName(itr->pLineDataRecord);
			const CString sIsoFileExt  = pDocData->GetProject()->GetOutputIsoFileExt(itr->pLineDataRecord);
			m_wndLineDataReport.SetItemText(at,1,sIsoFileName + _T(".") + sIsoFileExt);
			m_wndLineDataReport.SetItemData(at,(DWORD)(itr->pLineDataRecord));
			CDataField* pField = itr->pLineDataRecord->FindWithFieldName(_T("REV_NO"));
			if( pField ) m_wndLineDataReport.SetItemText(at,2,pField->value());
		}
	}
	m_wndLineDataReport.GroupByColumn(2);

	HICON hIcon = (HICON)::LoadImage( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_SHEET),IMAGE_ICON,16,16, 0 );
	HICON hOldIcon = m_btnExport.SetIcon(hIcon);
	if(NULL != hOldIcon) DestroyIcon(hOldIcon);
	hIcon = (HICON)::LoadImage( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_COMMIT),IMAGE_ICON,16,16, 0 );
	this->SetIcon(hIcon,FALSE);
	hOldIcon = m_btnOK.SetIcon(hIcon);
	if(NULL != hOldIcon) DestroyIcon(hOldIcon);
	hIcon = (HICON)::LoadImage( AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_CANCEL),IMAGE_ICON,	16,16, 0 );
	m_btnCancel.SetIcon(hIcon);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/**
	@brief	issue selected files
	@author	humkyung
	@date	2015.10.16
*/
void CIssueDlg::OnBnClickedOk()
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	const CProject::DatabaseProp prop = pDocData->GetProject()->GetDatabaseProp();

	try
	{
		CAppPostgreSQL database(pDocData->GetProject()->name(),prop.Host.c_str(),prop.User.c_str(),prop.Password.c_str());

		vector<LineDataRecordMap> aLineDataRecordEntry;
		this->GetSelectedLineDataRecord(aLineDataRecordEntry);
		for(vector<LineDataRecordMap>::const_iterator itr = aLineDataRecordEntry.begin();itr != aLineDataRecordEntry.end();++itr)
		{
			STRING_T sKey,sRevNo;
			CDataField* pField = itr->pLineDataRecord->FindWithFieldName( _T("KEY") );
			if( pField ) sKey = pField->value();
			pField = itr->pLineDataRecord->FindWithFieldName( _T("REV_NO") );
			if( pField ) sRevNo = pField->value();
			const CString sIsoFileName = pDocData->GetProject()->GetOutputIsoFileName(itr->pLineDataRecord);
			const CString sIsoFileExt  = pDocData->GetProject()->GetOutputIsoFileExt(itr->pLineDataRecord);

			soci::transaction txn(*database.session());
			try
			{
				OSTRINGSTREAM_T oss;
				oss << _T("DELETE FROM T_ISSUED_ISO_LINE_DATA WHERE KEY='") << sKey << _T("' AND REV_NO='") << sRevNo << _T("'");
				(*database.session()) << oss.str();
				oss.str(_T(""));
				oss << _T("INSERT INTO T_ISSUED_ISO_LINE_DATA SELECT * FROM T_ISO_LINE_DATA WHERE KEY='") + sKey + _T("' AND REV_NO='") << sRevNo << _T("'");
				(*database.session()) << oss.str();
				oss.str(_T(""));
				oss << _T("UPDATE T_ISSUED_ISO_LINE_DATA SET FILE_NAME='") << (sIsoFileName + _T(".") + sIsoFileExt).operator LPCTSTR();
				oss << _T("' WHERE KEY='") << sKey << _T("' AND REV_NO='") << sRevNo << _T("'");
				(*database.session()) << oss.str();

				oss.str(_T(""));
				oss << _T("DELETE FROM T_ISSUED_ISO_REVISION WHERE KEY='") << sKey << _T("' AND REV_NO='") << sRevNo << _T("'");
				(*database.session()) << oss.str();
				oss.str(_T(""));
				oss << _T("INSERT INTO T_ISSUED_ISO_REVISION SELECT * FROM T_ISO_REVISION WHERE KEY='") + sKey + _T("' AND REV_NO='") << sRevNo << _T("'");
				(*database.session()) << oss.str();

				oss.str(_T(""));
				oss << _T("DELETE FROM T_ISSUED_ISO_BOM_DATA WHERE KEY='") << sKey << _T("' AND REV_NO='") << sRevNo << _T("'");
				(*database.session()) << oss.str();
				oss.str(_T(""));
				oss << _T("INSERT INTO T_ISSUED_ISO_BOM_DATA SELECT * FROM T_ISO_BOM_DATA WHERE KEY='") + sKey + _T("' AND REV_NO='") << sRevNo << _T("'");
				(*database.session()) << oss.str();

				oss.str(_T(""));
				oss << _T("DELETE FROM T_ISO_DWG_FILE WHERE KEY='") << sKey << _T("' AND REV_NO='") << sRevNo << _T("'");
				(*database.session()) << oss.str();
				oss.str(_T(""));
				oss << _T("INSERT INTO T_ISO_DWG_FILE(KEY,REV_NO,FILE_DESC,FILE) VALUES('") << sKey << _T("','") << sRevNo << _T("','DGN V7',lo_creat(-1))"); 
				(*database.session()) << oss.str();
				soci::blob blob(*database.session());
				oss.str(_T(""));
				oss << _T("SELECT FILE FROM T_ISO_DWG_FILE WHERE KEY='") << sKey << _T("' AND REV_NO='") << sRevNo << _T("'");
				(*database.session()) << oss.str() , soci::into(blob);

				CString sFilePath = m_sOutputDrawingFolder;
				if(_T("\\") != sFilePath.Right(1)) sFilePath += _T("\\");
				sFilePath += m_wndLineDataReport.GetItemText(itr->nItem , 1);
				CFile f(sFilePath , CFile::modeRead);
				const ULONG fSize = f.GetLength();
				char* pData = (char*)calloc(1 , sizeof(char)*fSize);
				if(pData)
				{
					f.Read(pData,fSize);
					f.Close();
					
					blob.write(0,pData,sizeof(char)*fSize);
					free((void*)pData);
				}

				txn.commit();
				m_wndLineDataReport.SetItemText(itr->nItem,3,_T("OK"));
			}
			catch(const std::exception& ex)
			{
				m_wndLineDataReport.SetItemText(itr->nItem,3,ex.what());
			}
		}
	}
	catch(const std::exception& ex)
	{
		AfxMessageBox(ex.what(),MB_OK|MB_ICONERROR);
	}
}

void CIssueDlg::OnUpdateButtonExport(CCmdUI* pCmdUI)
{
	vector<LineDataRecordMap> aLineDataRecordEntry;
	this->GetSelectedLineDataRecord(aLineDataRecordEntry);
	pCmdUI->Enable(aLineDataRecordEntry.size() > 0);
}

/**
	@breif	export selected line data to excel
	@author	humkyung
	@date	2015.11.13
*/
void CIssueDlg::OnBnClickedButtonExport()
{
	CAppDocData* pDocData = CAppDocData::GetInstance();
	const CString name = pDocData->GetProject()->name();
	const CProject::DatabaseProp prop = pDocData->GetProject()->GetDatabaseProp();
	const CString sIniFilePath = pDocData->GetProject()->GetIniFilePath();

	CAppPostgreSQL database(name , prop.Host.c_str() , prop.User.c_str() , prop.Password.c_str());
	{
		csv_writer csv;

		vector<LineDataRecordMap> aLineDataRecordEntry;
		this->GetSelectedLineDataRecord(aLineDataRecordEntry);
		for(vector<LineDataRecordMap>::const_iterator itr = aLineDataRecordEntry.begin();itr != aLineDataRecordEntry.end();++itr)
		{
			STRING_T sKey , sRevNo;
			CDataField* pField = itr->pLineDataRecord->FindWithFieldName( _T("KEY") );
			if( pField ) sKey = pField->value();
			pField = itr->pLineDataRecord->FindWithFieldName(_T("REV_NO"));
			if(pField) sRevNo = pField->value();

			try
			{
				OSTRINGSTREAM_T oss;
				oss << _T("select * from t_iso_bom_data where key='") << sKey << _T("' and rev_no='") << sRevNo << _T("'");
				soci::rowset<soci::row> rs(database.session()->prepare << oss.str());
				for(soci::rowset<soci::row>::const_iterator itr = rs.begin();itr != rs.end();++itr)
				{
					vector<STRING_T> oCSVRow;
					for(vector<STRING_T>::const_iterator jtr = pDocData->m_oBMOutputFormat.m_oFieldFormatList.begin();jtr != pDocData->m_oBMOutputFormat.m_oFieldFormatList.end();++jtr)
					{
						if(_T("DRAWING NO") == (*jtr))
						{
							oCSVRow.push_back(itr->get<STRING_T>(_T("draw_no")));
						}
						else if(_T("SHT NO") == (*jtr))
						{
							oCSVRow.push_back(_T(""));
						}
						else if(_T("PTNO") == (*jtr))
						{
							oCSVRow.push_back(_T(""));
						}
						else if(_T("MATL SPEC") == (*jtr))
						{
							oCSVRow.push_back(itr->get<STRING_T>(_T("matl_spec")));
						}
						else if(_T("MATL CODE") == (*jtr))
						{
							oCSVRow.push_back(itr->get<STRING_T>(_T("matl_code")));
						}
						else if(_T("SIZE1") == (*jtr))
						{
							oCSVRow.push_back(itr->get<STRING_T>(_T("main_size")));
						}
						else if(_T("SIZE2") == (*jtr))
						{
							oCSVRow.push_back(itr->get<STRING_T>(_T("sub_size")));
						}
						else if(_T("QTY") == (*jtr))
						{
							oCSVRow.push_back(itr->get<STRING_T>(_T("qty")));
						}
						else if(_T("BOLT LEN") == (*jtr))
						{
							oCSVRow.push_back(itr->get<STRING_T>(_T("bolt_length")));
						}
						else if(_T("MATL DESC") == (*jtr))
						{
							oCSVRow.push_back(itr->get<STRING_T>(_T("matl_desc")));
						}
						else if(_T("SYM") == (*jtr))
						{
							oCSVRow.push_back(itr->get<STRING_T>(_T("symbol")));
						}
					}
					//STRING_T aRow = csv.write_line(ofile , oCSVRow);
				}
			}
			catch(const std::exception& ex)
			{
				AfxMessageBox(ex.what() , MB_OK|MB_ICONERROR);
			}
		}
	}
}

/**
	@brief	선택한 LINE LIST RECORD들을 구한다.
	@author	humkyung
	@date	2015.11.13
	@param	LineDataRecordEntry - container for LineDataRecordMap
	@return
**/
void CIssueDlg::GetSelectedLineDataRecord(vector<LineDataRecordMap>& aLineDataRecordEntry)
{
	aLineDataRecordEntry.clear();
	LineDataRecordMap aLineDataRecordMap;

	const int iItemCount = m_wndLineDataReport.GetItemCount();
	for(int i = 0;i < iItemCount;++i)
	{
		if(TRUE == m_wndLineDataReport.GetCheck(i))
		{
			aLineDataRecordMap.nItem = i;
			aLineDataRecordMap.pLineDataRecord = (CLineDataRecord*)m_wndLineDataReport.GetItemData(i);
			aLineDataRecordEntry.push_back(aLineDataRecordMap);		
		}
	}
}