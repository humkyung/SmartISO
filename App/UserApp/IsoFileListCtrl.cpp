// IsoFileListCtrl.cpp: implementation of the CIsoFileListCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UserApp.h"
#include "Options.h"
#include "IsoFileListCtrl.h"
#include "IsoEditForm.h"
#include "DrawingReportForm.h"
#include "BMReportForm.h"
#include "UserAppDocData.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIsoFileListCtrl::CIsoFileListCtrl(CWnd *pOwner) : CFileListCtrl(pOwner)
{
	SetColumnSize(5);
	m_rFileFilter = _T("*.*");
}

CIsoFileListCtrl::~CIsoFileListCtrl()
{
	int d = 1;
}

BEGIN_MESSAGE_MAP(CIsoFileListCtrl, CFileListCtrl)
	//{{AFX_MSG_MAP(CFileListCtrl)
	ON_WM_CREATE()
	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetdispinfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CIsoFileListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFileListCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	int widths[6]={150, 66, 100, 99 , 66};
	/* 
	//File List Rev. No
	CString str;
	str.LoadString(IDS_HEADER_REV_NO);
	InsertColumn(4,str,LVCFMT_LEFT, widths[4]);
	*/
	return 0;
}

void CIsoFileListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	int nItem = HitTest(point);
	if(-1 == nItem)
	{
		AfxMessageBox("No Item DblClick");
	}
	else
	{
		if(nItem!=0)
		{
			TCHAR szBuf[MAX_PATH]={'\0',};
			
			CWnd* pParent = GetParent();
			CAppDocData* pDocData = CAppDocData::GetInstance();
			CString rIniPath = pDocData->GetProject()->GetIniFilePath();
			
			GetPrivateProfileString( _T("Iso Edit Setting") , _T("MStation Path") , _T("") , szBuf,MAX_PATH,rIniPath);
			CString rMSFilePath = szBuf;
			GetPrivateProfileString( _T("Iso Edit Setting") , _T("Source Iso File Folder") , _T("") ,szBuf,MAX_PATH,rIniPath);
			CString rDWGFilePath = szBuf;
			
			if(pParent->IsKindOf(RUNTIME_CLASS(CIsoEditForm)))
			{
				POSITION pos = GetFirstSelectedItemPosition();
				int nItem = GetNextSelectedItem(pos);
				if(nItem != 0 )
				{	
					if (rDWGFilePath.Right(1) != _T("\\") ) rDWGFilePath += _T("\\");

					rDWGFilePath += GetItemText(nItem,0);
					::ShellExecute(NULL, _T("open") ,rMSFilePath,rDWGFilePath,NULL,SW_SHOW);
				}
			}
			else if(pParent->IsKindOf(RUNTIME_CLASS(CDrawingReportForm)) || pParent->IsKindOf(RUNTIME_CLASS(CBMReportForm)))
			{
				POSITION pos = GetFirstSelectedItemPosition();
				int nItem = GetNextSelectedItem(pos);
				if(nItem != 0 )
				{	
					GetPrivateProfileString(_T("Iso Edit Setting") , _T("Output Iso Dwg File Folder") , _T("") ,szBuf,MAX_PATH,rIniPath);
					rDWGFilePath = szBuf;

					if (rDWGFilePath.Right(1) != _T("\\") ) rDWGFilePath += _T("\\");
					
					rDWGFilePath += GetItemText(nItem,0);
					::ShellExecute(NULL , _T("open") , rMSFilePath , rDWGFilePath , NULL,SW_SHOW);
				}
			}
		}
	}

	CFileListCtrl::OnLButtonDblClk(nFlags, point);
}


void CIsoFileListCtrl::OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM* pItem= &(pDispInfo)->item;

	if (static_cast<int>(m_IndexMapping.size())<=pItem->iItem)
		return;
	if (static_cast<int>(m_FileData.size())<=m_IndexMapping[pItem->iItem])
		return;
	
	if (pItem->mask & LVIF_IMAGE && !pItem->iSubItem)
	{
		if (m_FileData[m_IndexMapping[pItem->iItem]].iIcon!=-1)
			pItem->iImage=m_FileData[m_IndexMapping[pItem->iItem]].iIcon;
		else
		{
			CString path;
			if (m_FileData[m_IndexMapping[pItem->iItem]].lName=="..")
				path="alkjhgfdfghjjhgfdghuztxvbhzt";
			else
			{
				path=m_Fullpath+m_FileData[m_IndexMapping[pItem->iItem]].Name;
				if (m_Fullpath=="")
					path+="\\";			
			}
			
			int iIcon=-1;
			SHFILEINFO shFinfo;
			memset(&shFinfo,0,sizeof(SHFILEINFO));
			if (SHGetFileInfo( path,
				m_FileData[m_IndexMapping[pItem->iItem]].bIsDir?FILE_ATTRIBUTE_DIRECTORY:FILE_ATTRIBUTE_NORMAL,
					&shFinfo,
					sizeof( SHFILEINFO ),
					SHGFI_ICON | ((m_FileData[m_IndexMapping[pItem->iItem]].lName=="..")?SHGFI_USEFILEATTRIBUTES:0) ) )
			{
				iIcon = shFinfo.iIcon;
				// we only need the index from the system image ctrl
				DestroyIcon( shFinfo.hIcon );
				m_FileData[m_IndexMapping[pItem->iItem]].iIcon=iIcon;
			}
			pItem->iImage=iIcon;
		}
	}
	if (pItem->mask & LVIF_TEXT)
	{
		if (!pItem->pszText)
			return;
		if (!m_pColumns[pItem->iSubItem])
		{
			_tcsncpy(pItem->pszText, m_FileData[m_IndexMapping[pItem->iItem]].Name, pItem->cchTextMax);
		}
		else if (m_pColumns[pItem->iSubItem]==2 && (pItem->iItem||(m_Fullpath=="")))
		{
			CString type=GetType(m_FileData[m_IndexMapping[pItem->iItem]].lName, m_FileData[m_IndexMapping[pItem->iItem]].bIsDir);
			_tcsncpy(pItem->pszText, type, pItem->cchTextMax);
		}
		else if (m_pColumns[pItem->iSubItem]==1)
		{
			__int64 size=m_FileData[m_IndexMapping[pItem->iItem]].nSize;
			if (size!=-1)
			{
				int nFormat = COptions::GetOptionVal(OPTION_LOCALFILESIZEFORMAT);
				if (!nFormat)
					if (size<1024)
						nFormat=1;
					else if (size<(1024*1024))
						nFormat=2;
					else
						nFormat=3;
				
				CString tmp, sizestr;
				switch (nFormat)
				{
				case 1:
					sizestr.Format(_T("%I64d"), size);
					break;
				case 2:
					tmp.LoadString(IDS_SIZE_KBS);
					sizestr.Format(_T("%I64d %s"), (size + 1023) / 1024, tmp);
					break;
				case 3:
					tmp.LoadString(IDS_SIZE_MBS);
					sizestr.Format(_T("%I64d %s"), (size + 1024 * 1024 - 1) / 1024 / 1024, tmp);
					break;
				default:
					ASSERT(FALSE);
				}
				_tcsncpy(pItem->pszText, sizestr, pItem->cchTextMax);
			}
		}
		else if (m_pColumns[pItem->iSubItem]==3 && m_Fullpath!=_T("") && pItem->iItem && m_FileData[m_IndexMapping[pItem->iItem]].hasTime)
		{
			CTime time=m_FileData[m_IndexMapping[pItem->iItem]].Time;
			SYSTEMTIME timeDest;
			time.GetAsSystemTime( timeDest ) ;
		
			TCHAR text[512];
			if (!GetDateFormat(
				LOCALE_USER_DEFAULT,               // locale for which date is to be formatted
				DATE_SHORTDATE,             // flags specifying function options
				&timeDest,  // date to be formatted
				0,          // date format string
				text,          // buffer for storing formatted string
				512                // size of buffer
				))
				return;
			CString text2=text;
			
			if (!GetTimeFormat(
				LOCALE_USER_DEFAULT,               // locale for which date is to be formatted
				TIME_NOSECONDS|TIME_FORCE24HOURFORMAT,             // flags specifying function options
				&timeDest,  // date to be formatted
				0,          // date format string
				text,          // buffer for storing formatted string
				512                // size of buffer
				))
				return;
			text2+=" ";
			text2+=text;
			_tcsncpy(pItem->pszText, text2, pItem->cchTextMax);
		}
		else if (m_pColumns[pItem->iSubItem] == 4 && m_Fullpath != _T("") && pItem->iItem)
		{
			_tcsncpy(pItem->pszText, m_RevNoData[m_IndexMapping[pItem->iItem]] , pItem->cchTextMax);
		}
		pItem->pszText[pItem->cchTextMax - 1] = 0;
	}
	*pResult = 0;
}

namespace
{
	struct IsInDisplayFileList : public binary_function<CString, CString, bool>
	{
	//          typedef T first_argument_type;
	//          typedef T second_argument_type;
	//          typedef T result_type;

		bool operator ()( const CString& a, const CString& b ) const
		{
			/*
			string str = b.operator const char*();
			string::size_type at = str.find_last_of(".");
			if((0 != at) && (string::npos != at))
			{
				str.assign(str.substr(0 , at));
			}
			CString _a(a);
			CString _b(str.c_str());
			_a.MakeUpper();
			_b.MakeUpper();
	
			return (_a == _b);
			*/
			return (a == b);
		}
	};
}

/**
	@brief	

	@author	humkyung
**/
void CIsoFileListCtrl::SetFolder(CString folder)
{
	if (m_bUpdating)
	{
		m_NewDir = folder;
		return;
	}
	else if (GetEditControl() && folder == m_Fullpath)
	{
		m_NewDir = folder;
		return;
	}
	else
		m_NewDir = _T("");
	
	m_NewDir = folder;
	
	BOOL bDidHaveFocus = GetFocus() == this;
	
	folder.TrimRight(_T("\\")); 
	folder += _T("\\");
	
	if (folder == m_Fullpath || (folder == _T("\\") && m_Fullpath == _T("")))
	{
		for (int i = 1; i < GetItemCount(); i++)
		{
			if (GetItemState( i, LVIS_SELECTED))
			{
				m_selectionBuffer.insert(m_FileData[m_IndexMapping[i]].lName);
				SetItemState(i, 0, LVIS_SELECTED);
			}
		}
	}
	else	m_selectionBuffer.clear();
	
	m_FileData.clear();
	m_RevNoData.clear();

	m_IndexMapping.clear();
	m_Fullpath = folder;
	if (m_Fullpath == _T("\\"))
	{
		m_Fullpath="";
		DisplayDrives();
		return;
	}
	m_listOnly = false;
	m_bUpdating=TRUE;
	
	SetRedraw(FALSE);
	EnableWindow(FALSE);
	
	
	CString strPathFiles = m_Fullpath;
	
	if ( strPathFiles.Right(1) != _T("\\") )
		strPathFiles += _T("\\");
	strPathFiles += m_rFileFilter;
	
	WIN32_FIND_DATA find;
	HANDLE hFind = FindFirstFile( strPathFiles, &find);
	
	CString folder2=m_Fullpath;
	folder2.TrimRight('\\');
	
	int pos=folder2.ReverseFind('\\');
	folder2=folder2.Left(pos+1);
	
	t_FileData FileData;
	FileData.Name = _T("..");
	FileData.lName= _T("..");
	FileData.bIsDir=TRUE;
	m_IndexMapping.push_back(0);
	FileData.nSize=-1;
	FileData.hasTime = false;
	m_FileData.push_back(FileData);
	m_RevNoData.push_back( _T("") );
	
	int nItemCount = 1;

	CFileStatus fileStatus;
	for(map<CString , int>::iterator itr = m_DisplayFileMap.begin(); itr != m_DisplayFileMap.end();++itr)
	{
		CString rFileName = itr->first;
		//read information
		if(CFile::GetStatus((folder + rFileName),fileStatus))
		{
			t_FileData FileData;
			CString rRevNo;
			FileData.Name = rFileName;
			FileData.lName= rFileName;
			FileData.lName.MakeLower();
			FileData.Time = fileStatus.m_mtime;
			FileData.hasTime = true;
			FileData.bIsDir  = FALSE;
			FileData.nSize   = fileStatus.m_size;
			rRevNo = _T("i01->1");

			m_FileData.push_back(FileData);
			m_RevNoData.push_back(rRevNo);
			m_IndexMapping.push_back(nItemCount);
			
			nItemCount++;
		}
	}

/*
	while (hFind != INVALID_HANDLE_VALUE)
	{
		if (!(find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY && (!_tcscmp(find.cFileName, _T("..")) || !_tcscmp(find.cFileName, _T(".")))))
		{
			t_FileData FileData;
			CString rRevNo;
			FileData.Name=find.cFileName;
			FileData.lName=find.cFileName;
			FileData.lName.MakeLower();
			
			TRY
			{
				FileData.Time = find.ftLastWriteTime;
				FileData.hasTime = true;
			}
			CATCH_ALL(e)
			{
				FileData.hasTime = false;
			}
			END_CATCH_ALL;
			
			if( find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				if(m_bDisplayFolder)
				{
					FileData.bIsDir=TRUE;
					FileData.nSize=-1;
					
					m_FileData.push_back(FileData);
					m_IndexMapping.push_back(nItemCount);
					
					nItemCount++;
				}
			}
			else
			{
				FileData.bIsDir=FALSE;
				FileData.nSize=(_int64)find.nFileSizeLow + ((_int64)find.nFileSizeHigh<<32);
				rRevNo = "i01->1";
				
				//////////////////////////////////////////////////////////////////////////
				vector<CString>::iterator where = find_if(m_DisplayFileMap.begin() , m_DisplayFileMap.end() , 
					bind2nd(IsInDisplayFileList() , FileData.Name));
				if(m_DisplayFileMap.end() != where)
				{
					m_FileData.push_back(FileData);
					m_RevNoData.push_back(rRevNo);
					m_IndexMapping.push_back(nItemCount);
					
					nItemCount++;
				}
			}
		}
		//#define SPEEDTEST 20000
#ifndef SPEEDTEST
		if (nItemCount==100)
		{
			VERIFY(PostMessage(WM_APP + 1, bDidHaveFocus, (LPARAM)hFind));
			return;
		}
#endif //SPEEDTEST
		if (!FindNextFile(hFind, &find))
		{
			FindClose(hFind);
			hFind=0;
#ifdef SPEEDTEST
			//Speed test code with SPEEDTEST files
			if (nItemCount > 1 && nItemCount<SPEEDTEST) 
				hFind = FindFirstFile( strPathFiles, &find);
			else
				break;
#else
			break;
#endif //SPEEDTEST
		}
	}
*/
	m_bUpdating = FALSE;
	
	SetItemCount(nItemCount);
	
	SortList(m_sortcolumn , m_sortdir);
	
	for (int i = 1; i < GetItemCount(); i++)
	{
		if (m_selectionBuffer.empty())
			break;
		const CString& name = m_FileData[m_IndexMapping[i]].lName;
		std::set<CString>::iterator iter = m_selectionBuffer.find(name);
		if (iter != m_selectionBuffer.end())
		{
			SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
			m_selectionBuffer.erase(iter);
		}
	}
	
	EnableWindow(TRUE);
	
	SetRedraw( TRUE );
	
	if (bDidHaveFocus)
		SetFocus();
	
	//	UpdateStatusBar();
}

/**
	@brief 디스플레이할 파일 이름을 추가한다.

	@author	HumKyung.BAEK

	@remarks
	기존이 파일 이름과 중복이 되는지 검사한다.
*/
void CIsoFileListCtrl::AddToDisplayFileList(const CString &rFileName , const int& nIndex)
{
	CString rString( rFileName );
	rString.MakeUpper();

	map<CString,int>::iterator where = m_DisplayFileMap.find(rString);
	///vector< CString >::iterator where = find( m_DisplayFileMap.begin() , m_DisplayFileMap.end() , rFileName );
	if( where == m_DisplayFileMap.end() )
	{	
		m_DisplayFileMap.insert(make_pair(rString , nIndex));
	}
}

/**
	\brief	
	\author	백흠경
**/
void CIsoFileListCtrl::ClearDisplayFileList()
{
	m_DisplayFileMap.clear();
}

void CIsoFileListCtrl::RemoveDisplayItem(const CString &rItem)
{
	CString rString( rItem );
	rString.MakeUpper();
	map< CString , int>::iterator where = m_DisplayFileMap.find(rItem);
	if( where != m_DisplayFileMap.end() ) m_DisplayFileMap.erase( where );
}

int CIsoFileListCtrl::GetDisplayItemCount() const
{
	return m_DisplayFileMap.size();
}
