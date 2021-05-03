// DrawingPlotting.cpp : implementation file
//

#include "stdafx.h"
#include "UserApp.h"
#include "UserAppDocData.h"
#include "DrawingPlotting.h"
#include "WorkStatusDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDrawingPlotting dialog


CDrawingPlotting::CDrawingPlotting(CWnd* pParent /*=NULL*/)
	: CDialog(CDrawingPlotting::IDD, pParent) ,m_pIsoFileList(NULL), m_pLineDataRecordSet(NULL)
{
	//{{AFX_DATA_INIT(CDrawingPlotting)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CDrawingPlotting::~CDrawingPlotting()
{
	try
	{
		if( m_pIsoFileList ) delete m_pIsoFileList;
	}
	catch(...)
	{
	}
}

void CDrawingPlotting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDrawingPlotting)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDrawingPlotting, CDialog)
	//{{AFX_MSG_MAP(CDrawingPlotting)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDrawingPlotting message handlers

BOOL CDrawingPlotting::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	CUserAppDocData* pDocData = CUserAppDocData::GetInstance();
	
	CString rIniFilePath =pDocData->m_pSelUser->GetProject()->GetIniFilePath();
	LoadData(rIniFilePath);

	m_pIsoFileList = new CIsoFileListCtrl(this);
	m_pIsoFileList->Create(LVS_REPORT | WS_CHILD | WS_VISIBLE | WS_BORDER | WS_EX_STATICEDGE | LVS_SHOWSELALWAYS | LVS_SHAREIMAGELISTS | LVS_EDITLABELS | LVS_OWNERDATA , 
		CRect(0 , 0 , 763 , 430) , this , 0x100);
	LONG style = GetWindowLong(m_pIsoFileList->m_hWnd , GWL_STYLE);
	style |= WS_EX_CLIENTEDGE;
	SetWindowLong(m_pIsoFileList->m_hWnd , GWL_EXSTYLE , style );
	m_pIsoFileList->SetDisplayFolder(FALSE);

	CWorkStatusDlg dlg;
	dlg.m_pThread = AfxBeginThread(StatusThreadEntry, this, THREAD_PRIORITY_NORMAL,0,CREATE_SUSPENDED);
	if (NULL == dlg.m_pThread)
	{
		AfxMessageBox("Can't create thread!!!");
	}
	else
	{
		dlg.DoModal();

		/// display output files
		const int nRecordCount = m_pLineDataRecordSet->GetLineDataRecordCount();
		for(int j=0 ; j < nRecordCount ; j++)
		{
			CLineDataRecord* pLineDataRecord = (CLineDataRecord*)m_pLineDataRecordSet->GetLineDataRecordAt(j);
			CString rIsoFileName = pDocData->m_pSelUser->GetOutputIsoFileName(pLineDataRecord);
			
			CRevDataRecordSet* pRevRecordSet = pLineDataRecord->GetRevDataRecordSet();
			const size_t nRecordCount = pRevRecordSet->GetRecordCount();
			for( int i = 0;i < nRecordCount;++i)
			{
				CRevDataRecord* pRevRecord = pRevRecordSet->GetRecordAt( i );
				CString rRevNo = pRevRecord->GetRevNo();

				m_pIsoFileList->AddToDisplayFileList(rIsoFileName + "." + rRevNo );
			}
		}
		
		m_pIsoFileList->SetFolder(m_rOutputDrawingFolder);
		///AfxMessageBox("Completion");
	}
	
	/*
	vector< string > issueDates;
	m_pLineDataRecordSet = pDocData->m_pSelUser->GetLineDataRecordSetFromDatabase("" , NULL , issueDates);
	
	const int nRecordCount = m_pLineDataRecordSet->GetLineDataRecordCount();
	for(int j=0 ; j < nRecordCount ; j++)
	{
		CLineDataRecord* pLineDataRecord = (CLineDataRecord*)m_pLineDataRecordSet->GetLineDataRecordAt(j);
		CString rIsoFileName = pDocData->m_pSelUser->GetOutputIsoFileName(pLineDataRecord);
		m_pIsoFileList->AddToDisplayFileList(rIsoFileName);
	}

	m_pIsoFileList->SetFolder(m_rOutputDrawingFolder);
	*/
	CenterWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/**
	@brief	

	@author	BHK
*/
void CDrawingPlotting::OnOK() 
{
	// TODO: Add extra validation here
	vector<int> SelItemEntry;
	POSITION pos = m_pIsoFileList->GetFirstSelectedItemPosition();
	while(pos)
	{
		int nItem = m_pIsoFileList->GetNextSelectedItem(pos);
		SelItemEntry.push_back(nItem);
	}
	for(vector<int>::iterator itr = SelItemEntry.begin(); itr != SelItemEntry.end(); ++itr)
	{
		CString rFileName = m_pIsoFileList->GetItemText(*itr , 0);
		if(rFileName == "..") continue;
		Print_IPlot(rFileName);
	}
	
	CDialog::OnOK();
}

/**
**/
void CDrawingPlotting::LoadData(const CString &rIniFilePath)
{
	char szBuf[MAX_PATH] = {'\0' ,};
	CString rApp ="Iso Edit Setting";
	if(GetPrivateProfileString(rApp , "Output Iso Dwg File Folder" , NULL , szBuf , MAX_PATH , rIniFilePath))
	{
		m_rOutputDrawingFolder = szBuf;
	}
	if(GetPrivateProfileString(rApp , "Printer" , NULL , szBuf , MAX_PATH , rIniFilePath))
	{
		m_rPrintName = szBuf;
	}
	if(GetPrivateProfileString(rApp,"Source Iso File Folder",NULL,szBuf,MAX_PATH,rIniFilePath))
	{
		m_rSourceFilePath = szBuf;
	}
}

ofstream __LOG_FILE__;

/**	
	@brief	선택한 iso file을 plotting한다.

	@author	BHK

	@param	rIsoFile	a parameter of type const CString &

	@return	void	
*/
void CDrawingPlotting::Print_IPlot(const CString &rIsoFile)
{
	CUserAppDocData* pDocData = CUserAppDocData::GetInstance();
	CString rPath = m_rSourceFilePath;
	if(rPath.Right(1) != "\\") rPath += "\\";
	CString rPname = pDocData->m_ProjectName;
	
	CString rTemp, rBorder, rPen_Table, rQueue, rProjectName;
	UpdateData(TRUE);
	rQueue = m_rPrintName;
	rQueue.TrimLeft();
	rQueue.TrimRight();

	CString rPaperSize = "-PAPER_SIZE=";
	
	if ( m_rPrintName.Find("a4") != -1 || m_rPrintName.Find("A4") != -1 )
	{
		rPen_Table = rPath + "ISO_BM_DATA\\PLOT_DATA\\dec_a4.i";
		rPaperSize +="A4";
	}
	else
	{
		rPen_Table = rPath + "ISO_BM_DATA\\PLOT_DATA\\dec_a3.i";
		rPaperSize +="A3";
	}

	CString rNewPenTable;
	int at = rPen_Table.ReverseFind('\\');
	rNewPenTable = rPen_Table.Left(at);
	CString rIsoFileName;
	at = rIsoFile.ReverseFind('\\');
	rIsoFileName = rIsoFile.Mid(at);
	rNewPenTable += "\\" + rIsoFile;
	CopyFile(rPen_Table, rNewPenTable,FALSE);
	rPen_Table = rNewPenTable;

	if(rPname.Find("_") != -1)
	{
		rProjectName = rPname.Mid(0,rPname.Find("_"));
	}
	else
	{
		rProjectName = rPname;
	}

	rBorder = rPath + "ISO_BM_DATA\\PLOT\\ISOC_" + rProjectName +".BRD";

	rTemp = "iplot mod -queue=" + rQueue + " -attach_ref[border]=" + rBorder + "  " + rPen_Table +" " + rPaperSize;
	System_IPlot( rTemp );

	if(!__LOG_FILE__.is_opened())
	{
		__LOG_FILE__.open("c:\\temp\\UserApp.log");
	}

	if(__LOG_FILE__.is_opened())
	{
		__LOG_FILE__ << rTemp.operator LPCSTR() << std::end;
	}
}

/**
	@brief	

	@author	BHK
*/
void CDrawingPlotting::System_IPlot(CString ip)
{
	system(ip);
	/*
	int i=0;
	CString rtemp;
	
	strcpy(File_Temp, Tip_ED.CString2Char( ip ));
	i = system( File_Temp );

	if ( i == 0 )	ctemp.Format("              %s  : OK ", File_Temp); 
	else			ctemp.Format("              %s  : FAILED ", File_Temp); 
	Make_Log_File( rtemp );
	*/
}

UINT CDrawingPlotting::StatusThreadEntry(LPVOID pVoid)
{
	return ((CDrawingPlotting*)pVoid)->StatusThread();
}

UINT CDrawingPlotting::StatusThread()
{
	CWorkStatusDlg* pWorkStatusDlg = CWorkStatusDlg::GetInstance();
	try
	{
		InterlockedExchange((LONG*)(&(pWorkStatusDlg->m_bThreadRunning)) , TRUE);// so the main thread knows that this thread is still running
		
		CUserAppDocData* pDocData = CUserAppDocData::GetInstance();
		vector< string > issueDates;
		m_pLineDataRecordSet = pDocData->m_pSelUser->GetLineDataRecordSetFromDatabase("" , NULL , issueDates);
		
		InterlockedExchange((LONG*)(&(pWorkStatusDlg->m_bThreadRunning)) , FALSE);
		//////////////////////////////////////////////////////////////////////////
		/// close work status dialog
		pWorkStatusDlg->SendMessage(WM_COMMAND , IDOK );
		
		return IDOK;
	}
	catch(...)
	{
		AfxMessageBox("Unknown error");
		
		InterlockedExchange((LONG*)(&(pWorkStatusDlg->m_bThreadRunning)) , FALSE);
		
		//////////////////////////////////////////////////////////////////////////
		/// close work status dialog
		pWorkStatusDlg->SendMessage(WM_COMMAND , IDOK );
	}
	
	return IDCANCEL;
}
