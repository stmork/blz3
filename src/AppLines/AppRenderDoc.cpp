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
**	Revision 1.5  2002/01/19 19:57:56  sm
**	- Further clean up of CAppRenderDoc derivates done. Especially:
**	  o Moved tree build from CDlgHierarchy into documents.
**	  o All views react on activating.
**	  o CAppObjectDoc creation cleaned up.
**	  o Fixed some ugly drawing dependencies during initialization.
**	     Note: If you don't need Windows -> You're fine!
**
**	Revision 1.4  2002/01/17 15:46:00  sm
**	- CAppRaytraceDoc.cpp cleaned up for later use from CAppObjectDoc.
**	- Opening a CAppRaytraceDoc for all image extensions.
**	
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
	m_RaytraceDoc  = null;
	m_Raytracer    = new b3Thread("Raytracing master thread");
	m_Display      = null;
}

CAppRenderDoc::~CAppRenderDoc()
{
	delete m_Raytracer;
}

void CAppRenderDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
	CB3GetMainFrame()->b3UpdateModellerInfo();
	m_DlgHierarchy->b3InitTree(null);
	CDocument::OnCloseDocument();
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

void CAppRenderDoc::b3InitTree()
{
}

void CAppRenderDoc::b3DropBBox(b3BBox *srcBBox,b3BBox *dstBBox)
{
}

/*************************************************************************
**                                                                      **
**                        Raytracing methods                            **
**                                                                      **
*************************************************************************/

void CAppRenderDoc::b3ToggleRaytrace()
{
	if (!b3IsRaytracing())
	{
		b3StartRaytrace();
	}
	else
	{
		b3StopRaytrace();
	}
}

b3_bool CAppRenderDoc::b3IsRaytracing()
{
	return m_Raytracer->b3IsRunning();
}

void CAppRenderDoc::b3StartRaytrace()
{
}

void CAppRenderDoc::b3StopRaytrace()
{
}

void CAppRenderDoc::b3ClearRaytraceDoc()
{
	b3StopRaytrace();
	m_RaytraceDoc = null;
}
