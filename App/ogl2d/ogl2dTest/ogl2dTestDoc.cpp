// ogl2dTestDoc.cpp : implementation of the COgl2dTestDoc class
//

#include "stdafx.h"
#include "ogl2dTest.h"

#include "ogl2dTestDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COgl2dTestDoc

IMPLEMENT_DYNCREATE(COgl2dTestDoc, CDocument)

BEGIN_MESSAGE_MAP(COgl2dTestDoc, CDocument)
	//{{AFX_MSG_MAP(COgl2dTestDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COgl2dTestDoc construction/destruction

COgl2dTestDoc::COgl2dTestDoc()
{
	// TODO: add one-time construction code here

}

COgl2dTestDoc::~COgl2dTestDoc()
{
}

BOOL COgl2dTestDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// COgl2dTestDoc serialization

void COgl2dTestDoc::Serialize(CArchive& ar)
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
// COgl2dTestDoc diagnostics

#ifdef _DEBUG
void COgl2dTestDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void COgl2dTestDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COgl2dTestDoc commands
