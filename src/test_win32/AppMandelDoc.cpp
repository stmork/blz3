// AppMandelDoc.cpp : implementation of the CAppMandelDoc class
//

#include "stdafx.h"
#include "AppMandel.h"

#include "AppMandelDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAppMandelDoc

IMPLEMENT_DYNCREATE(CAppMandelDoc, CDocument)

BEGIN_MESSAGE_MAP(CAppMandelDoc, CDocument)
	//{{AFX_MSG_MAP(CAppMandelDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAppMandelDoc construction/destruction

CAppMandelDoc::CAppMandelDoc()
{
	// TODO: add one-time construction code here

}

CAppMandelDoc::~CAppMandelDoc()
{
}

BOOL CAppMandelDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CAppMandelDoc serialization

void CAppMandelDoc::Serialize(CArchive& ar)
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
// CAppMandelDoc diagnostics

#ifdef _DEBUG
void CAppMandelDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAppMandelDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAppMandelDoc commands
