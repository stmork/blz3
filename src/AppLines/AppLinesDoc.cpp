/*
**
**	$Filename:	AppLinesDoc.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - CDocument part
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Lines III includes                            **
**                                                                      **
*************************************************************************/

#include "AppLines.h"
#include "AppLinesDoc.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2001/08/06 19:58:58  sm
**	- Drawing area - the first shape we can see with OpenGL
**
**	Revision 1.1  2001/08/05 19:51:56  sm
**	- Now having OpenGL software for Windows NT and created
**	  new Lines III.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CAppLinesDoc implementation                   **
**                                                                      **
*************************************************************************/

IMPLEMENT_DYNCREATE(CAppLinesDoc, CDocument)

BEGIN_MESSAGE_MAP(CAppLinesDoc, CDocument)
	//{{AFX_MSG_MAP(CAppLinesDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CAppLinesDoc, CDocument)
	//{{AFX_DISPATCH_MAP(CAppLinesDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//      DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IAppLines to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {72D69519-8984-11D5-A54F-0050BF4EB3F3}
static const IID IID_IAppLines =
{ 0x72d69519, 0x8984, 0x11d5, { 0xa5, 0x4f, 0x0, 0x50, 0xbf, 0x4e, 0xb3, 0xf3 } };

BEGIN_INTERFACE_MAP(CAppLinesDoc, CDocument)
	INTERFACE_PART(CAppLinesDoc, IID_IAppLines, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAppLinesDoc construction/destruction

CAppLinesDoc::CAppLinesDoc()
{
	// TODO: add one-time construction code here
	m_Scene = null;
	EnableAutomation();

	AfxOleLockApp();
}

CAppLinesDoc::~CAppLinesDoc()
{
	AfxOleUnlockApp();
}

BOOL CAppLinesDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	m_Scene = null;//new b3Scene(TRACEPHOTO_MORK);
	return TRUE;
}

BOOL CAppLinesDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	m_World.b3Read(lpszPathName);
	m_Scene = (b3Scene *)m_World.b3GetFirst();
	m_Scene->b3AllocVertices();
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CAppLinesDoc serialization

void CAppLinesDoc::Serialize(CArchive& ar)
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
// CAppLinesDoc diagnostics

#ifdef _DEBUG
void CAppLinesDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAppLinesDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAppLinesDoc commands

b3Scene * CAppLinesDoc::b3GetScene()
{
	return m_Scene;
}
