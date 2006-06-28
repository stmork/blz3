/*
**
**	$Filename:	b3LinesDocument.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - CDocument part for OpenGL Documents (scenes/objects)
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Lines III includes                            **
**                                                                      **
*************************************************************************/

#include "AppLinesInclude.h"

#define NO_STATE

/*************************************************************************
**                                                                      **
**                        CB3LinesDocument implementation               **
**                                                                      **
*************************************************************************/

IMPLEMENT_DYNAMIC(CB3LinesDocument, CDocument)

BEGIN_MESSAGE_MAP(CB3LinesDocument, CDocument)
	//{{AFX_MSG_MAP(CB3LinesDocument)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CB3LinesDocument *CB3LinesDocument::b3GetLinesDocument(CView *pView)
{
	CDocument *pDoc = pView->GetDocument();

	return (CB3LinesDocument *)pDoc;
}

void CB3LinesDocument::b3HandleBarState(CView *pView,b3_bool activation)
{
#ifndef NO_STATE
	CB3LinesDocument *pDoc = b3GetLinesDocument(pView);
	CAppLinesApp *app = CB3GetLinesApp();
	const char       *name = pDoc->b3GetDocumentName();

	if (activation)
	{
		app->b3LoadState(name);
	}
	else
	{
		app->b3SaveState(name);
	}
#endif
}

void CB3LinesDocument::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
#ifndef NO_STATE
	CAppLinesApp *app = CB3GetLinesApp();
	const char   *name = b3GetDocumentName();

	app->b3SaveState(name);
#endif
	CDocument::OnCloseDocument();
}
