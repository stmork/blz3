/*
**
**	$Filename:	AppRaytraceDoc.cpp $
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

#include "stdafx.h"
#include "AppLines.h"

#include "AppRaytraceDoc.h"
#include "AppRaytraceView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2001/10/24 14:59:08  sm
**	- Some GIG bug fixes
**	- An image viewing bug fixed in bimg3
**
**	Revision 1.2  2001/10/03 20:17:55  sm
**	- Minor bugfixes
**	
**	Revision 1.1  2001/09/30 15:46:06  sm
**	- Displaying raytracing under Windows
**	- Major cleanups in Lines III with introducing CAppRaytraceDoc/
**	  CAppRaytraceView pair for displaying Raytracing
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CAppRaytraceDoc implementation                **
**                                                                      **
*************************************************************************/

IMPLEMENT_DYNCREATE(CAppRaytraceDoc, CDocument)

BEGIN_MESSAGE_MAP(CAppRaytraceDoc, CDocument)
	//{{AFX_MSG_MAP(CAppRaytraceDoc)
	ON_COMMAND(ID_RAYTRACE, OnRaytrace)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAppRaytraceDoc construction/destruction

CAppRaytraceDoc::CAppRaytraceDoc()
{
	// TODO: add one-time construction code here
	m_Scene = null;
	m_LinesDoc = null;
}

CAppRaytraceDoc::~CAppRaytraceDoc()
{
}

BOOL CAppRaytraceDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

void CAppRaytraceDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_LinesDoc->b3ClearRaytraceDoc();
	CDocument::OnCloseDocument();
}



/////////////////////////////////////////////////////////////////////////////
// CAppRaytraceDoc serialization

void CAppRaytraceDoc::Serialize(CArchive& ar)
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
// CAppRaytraceDoc diagnostics

#ifdef _DEBUG
void CAppRaytraceDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAppRaytraceDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAppRaytraceDoc commands

void CAppRaytraceDoc::b3SetLinesDoc(CAppLinesDoc *LinesDoc)
{
	m_LinesDoc = LinesDoc;
}

b3Display *CAppRaytraceDoc::b3GetDisplay(b3_res xSize,b3_res ySize,const char *title)
{
	CAppRaytraceView *pView;
	POSITION          pos;

	pos   = GetFirstViewPosition();
	pView = (CAppRaytraceView *)GetNextView(pos);
	return new b3Display(pView,xSize,ySize,title);
}

b3Display *CAppRaytraceDoc::b3GetDisplay(const char *title)
{
	CAppRaytraceView *pView;
	POSITION          pos;

	pos   = GetFirstViewPosition();
	pView = (CAppRaytraceView *)GetNextView(pos);
	return new b3Display(pView,title);
}

void CAppRaytraceDoc::OnRaytrace() 
{
	// TODO: Add your command handler code here
	m_LinesDoc->b3Raytrace();
}
