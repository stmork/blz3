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
**	Revision 1.15  2003/01/14 19:07:35  sm
**	- Added some camera undo/redo actions.
**
**	Revision 1.14  2003/01/12 19:21:37  sm
**	- Some other undo/redo actions added (camera etc.)
**	
**	Revision 1.13  2003/01/07 16:14:38  sm
**	- Lines III: object editing didn't prepared any more. Fixed.
**	- Some prepare optimizations.
**	
**	Revision 1.12  2003/01/05 16:13:24  sm
**	- First undo/redo implementations
**	
**	Revision 1.11  2002/08/01 15:02:56  sm
**	- Found texture missing bug when printing. There weren't any
**	  selected textures inside an other OpenGL rendering context.
**	  Now fixed!
**	
**	Revision 1.10  2002/02/22 20:18:09  sm
**	- Added shape/bbox creation in object editor. So bigger
**	  icons (64x64) for shape selection are created.
**	- Created new class for image list maintainance.
**	
**	Revision 1.9  2002/02/12 18:39:02  sm
**	- Some b3ModellerInfo cleanups concerning measurement.
**	- Added raster drawing via OpenGL. Nice!
**	- Added pick points for light sources.
**	- Added support for post OpenGL rendering for Win DC. This
**	  is needed for drawing pick points. Note that there is a
**	  slight offset when drawing pick points into a printer DC.
**	
**	Revision 1.8  2002/02/01 15:04:09  sm
**	- Prepared shapes for icon conversion
**	- Added to save selected/first visible item in
**	  hierarchy dialog.
**	- Some print cleanups done.
**	- Fixed activation of b3SuperSample.
**	
**	Revision 1.7  2002/01/31 19:30:31  sm
**	- Some OpenGL print optimizations done.
**	- Object renaming inside hierarchy tree added.
**	
**	Revision 1.6  2002/01/24 15:55:57  sm
**	- Fixed key handling on TreeCtrl (hierarchy dialog bar)
**	- Added support for conext menu depending on scene/object edit.
**	
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
	ON_COMMAND(ID_RENAME, OnRename)
	ON_UPDATE_COMMAND_UI(ID_RENAME, OnUpdateRename)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
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
	m_FirstVisible = null;
	m_Selected     = null;
	m_UndoBuffer   = new b3UndoBuffer(this);
}

CAppRenderDoc::~CAppRenderDoc()
{
	delete m_Raytracer;
	delete m_UndoBuffer;
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
	m_Fulcrum.b3Draw(&m_Context);
}

void CAppRenderDoc::b3Prepare(
	b3_bool geometry_changed,
	b3_bool structure_changed,
	b3_bool reorg)
{
}

void CAppRenderDoc::b3ComputeBounds()
{
}

void CAppRenderDoc::b3InitTree()
{
}

void CAppRenderDoc::b3SelectItem(b3Item *item)
{
	m_DlgHierarchy->b3SelectItem(item);
}

void CAppRenderDoc::b3DropBBox(b3BBox *srcBBox,b3BBox *dstBBox)
{
}

void CAppRenderDoc::b3ContextMenu(HTREEITEM item)
{
}

void CAppRenderDoc::OnRename() 
{
	// TODO: Add your command handler code here
	HTREEITEM  item = m_DlgHierarchy->m_Hierarchy.GetSelectedItem();

	if (item != null)
	{
		m_DlgHierarchy->m_Hierarchy.EditLabel(item);
	}
}

void CAppRenderDoc::OnUpdateRename(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_DlgHierarchy->b3GetSelectedBBox() != null);
}

void CAppRenderDoc::b3DragBegin()
{
}

HTREEITEM CAppRenderDoc::b3Dragging(HTREEITEM dragitem,HTREEITEM dropitem)
{
	return null;
}

void CAppRenderDoc::b3Drop(HTREEITEM dragitem,HTREEITEM dropitem)
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

void CAppRenderDoc::b3AddUndoAction(CB3Action *action)
{
}

void CAppRenderDoc::b3AddOp(b3Operation *op)
{
	m_UndoBuffer->b3Do(op);
}

void CAppRenderDoc::b3ClearOp()
{
	m_UndoBuffer->b3Clear();
}

void CAppRenderDoc::OnEditUndo() 
{
	// TODO: Add your command handler code here
	m_UndoBuffer->b3Undo();
}

void CAppRenderDoc::OnEditRedo() 
{
	// TODO: Add your command handler code here
	m_UndoBuffer->b3Redo();
}

void CAppRenderDoc::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_UndoBuffer->b3HasUndo());
}

void CAppRenderDoc::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_UndoBuffer->b3HasRedo());
}
