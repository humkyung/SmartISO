// SupportCellDoc.cpp : implementation of the CSupportCellDoc class
//

#include "stdafx.h"
#include "SupportCell.h"

#include <DgnV7Lib.h>

#include "SupportCellDoc.h"
#include "SupportCellView.h"
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSupportCellDoc

IMPLEMENT_DYNCREATE(CSupportCellDoc, CDocument)

BEGIN_MESSAGE_MAP(CSupportCellDoc, CDocument)
	//{{AFX_MSG_MAP(CSupportCellDoc)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSupportCellDoc construction/destruction

CSupportCellDoc::CSupportCellDoc()
{
	// TODO: add one-time construction code here

}

CSupportCellDoc::~CSupportCellDoc()
{

}

BOOL CSupportCellDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSupportCellDoc serialization

void CSupportCellDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSupportCellDoc diagnostics

#ifdef _DEBUG
void CSupportCellDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSupportCellDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSupportCellDoc commands

void CSupportCellDoc::OnFileOpen() 
{
	// TODO: Add your command handler code here
	CFileDialog dlg(TRUE); dlg.DoModal();
	m_FileName = dlg.GetPathName();
	ExtractCell();
}

void CSupportCellDoc::ExtractCell()
{
	CString rFilePath = m_FileName;
	int at = rFilePath.ReverseFind('.');
	if(at != -1)
	{
		rFilePath = rFilePath.Left(at)+".txt" ;
		CDgnDocument* pDoc = CDgnDocument::CreateInstance();
		if(pDoc)
		{
			//////////////////////////////////////////////////////////////////////////
			/// 파일이름.txt 로 정보를 뽑아내고 리스트에 나타낸다.
			ofstream ofile(rFilePath);
			
			pDoc->Read(m_FileName);
			size_t entities = pDoc->GetEntityCount();
			int nItem=0;
			for(size_t i = 0;i < entities;++i)
			{
				
				CDgnElement* pEnt = pDoc->GetEntityAt(i);
				if(pEnt->IsKindOf(CDgnCellHeader::TypeString()))
				{
					CDgnCellHeader* pCellHeader = static_cast<CDgnCellHeader*>(pEnt);
					DPoint3d pOrigin = pCellHeader->origin();
					ofile << pCellHeader->name() << "=" << pEnt->filePos() << "," << pEnt->GetElementWords() << "," << pOrigin.x << "," << pOrigin.y <<std::endl;
					CString rName = pCellHeader->name(),rPos,rX,rY,rSize;
					rPos.Format("%ld",pEnt->filePos());
					rSize.Format("%ld",pEnt->GetElementWords());
					rX.Format("%ld" ,pOrigin.x);
					rY.Format("%ld",pOrigin.y);
					POSITION pos = GetFirstViewPosition();
					CSupportCellView* pView = (CSupportCellView*)GetNextView( pos );
					
					nItem =	pView->m_cellInfoListCtrl.InsertItem(i,rName,0);
					pView->m_cellInfoListCtrl.SetItemText(nItem,1,_T(rPos));
					pView->m_cellInfoListCtrl.SetItemText(nItem,2,_T(rSize));
					pView->m_cellInfoListCtrl.SetItemText(nItem,3,_T(rX));
					pView->m_cellInfoListCtrl.SetItemText(nItem,4,_T(rY));
				}
			}
			CDgnDocument::DeleteInstance(pDoc);
		}
		AfxMessageBox("Complete.");
	}
}