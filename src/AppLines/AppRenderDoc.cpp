/*
**
**	$Filename:	AppRenderDoc.cpp $
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

#include "AppLines.h"
#include "AppRenderDoc.h"
#include "AppRenderView.h"
#include "MainFrm.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2002/01/14 16:13:02  sm
**	- Some further cleanups done.
**	- Icon reordering done.
**
**	Revision 1.2  2002/01/13 20:50:51  sm
**	- Done more CAppRenderDoc/View cleanups
**	
**	Revision 1.1  2002/01/13 19:24:11  sm
**	- Introduced CAppRenderDoc/View (puuh!)
**	
**
*/

/*************************************************************************
**                                                                      **
**                        CAppRenderDoc implementation                  **
**                                                                      **
*************************************************************************/

IMPLEMENT_DYNCREATE(CAppRenderDoc, CDocument)

BEGIN_MESSAGE_MAP(CAppRenderDoc, CDocument)
	//{{AFX_MSG_MAP(CAppRenderDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAppRenderDoc construction/destruction

CAppRenderDoc::CAppRenderDoc()
{
	// TODO: add one-time construction code here
	m_DlgHierarchy = &CB3GetMainFrame()->m_dlgHierarchy;
	m_Fulcrum.b3AllocVertices(&m_Context);
}

CAppRenderDoc::~CAppRenderDoc()
{
}



/////////////////////////////////////////////////////////////////////////////
// CAppRenderDoc serialization

void CAppRenderDoc::Serialize(CArchive& ar)
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
// CAppRenderDoc diagnostics

#ifdef _DEBUG
void CAppRenderDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAppRenderDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/*************************************************************************
**                                                                      **
**                        General scene commands                        **
**                                                                      **
*************************************************************************/

b3_vector *CAppRenderDoc::b3GetFulcrum()
{
	return &m_Info->m_Center;
}

b3_vector *CAppRenderDoc::b3GetStepMove()
{
	return &m_Info->m_StepMove;
}

b3_vector *CAppRenderDoc::b3GetStepRotate()
{
	return &m_Info->m_StepRotate;
}

void CAppRenderDoc::b3DrawFulcrum()
{
	m_Fulcrum.b3Draw();
}

void CAppRenderDoc::b3ComputeBounds()
{
}
