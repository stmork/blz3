/*
**
**	$Filename:	AppRenderDoc.cpp $
**	$Release:	Dortmund 2002 $
**	$Revision: 476 $
**	$Date: 2003-02-08 15:04:18 +0100 (Sa, 08 Feb 2003) $
**	$Author: sm $
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

#ifndef B3_LINES_DOCUMENT_H
#define B3_LINES_DOCUMENT_H

#include "blz3/b3Config.h"
#include "blz3/system/b3Document.h"

class CB3LinesDocument : public CDocument, public b3Document
{
public:
	virtual const char       *b3GetDocumentName() = 0;
	static  CB3LinesDocument *b3GetLinesDocument(CView *pView);
	static  void              b3HandleBarState(CView *pView,b3_bool activation);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CB3LinesDocument)
	public:
	virtual void OnCloseDocument();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CB3LinesDocument)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	DECLARE_DYNAMIC(CB3LinesDocument)
	// Generated message map functions
};

#endif
