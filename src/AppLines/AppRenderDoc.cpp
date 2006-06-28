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

#include "AppLinesInclude.h"
#include "DlgUndoRedo.h"

/*************************************************************************
**                                                                      **
**                        CAppRenderDoc implementation                  **
**                                                                      **
*************************************************************************/

IMPLEMENT_DYNAMIC(CAppRenderDoc, CB3LinesDocument)

BEGIN_MESSAGE_MAP(CAppRenderDoc, CB3LinesDocument)
	//{{AFX_MSG_MAP(CAppRenderDoc)
	ON_COMMAND(ID_RENAME, OnRename)
	ON_UPDATE_COMMAND_UI(ID_RENAME, OnUpdateRename)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

b3Color CAppRenderDoc::m_BgColor(0.9f,0.9f,0.9f);

/////////////////////////////////////////////////////////////////////////////
// CAppRenderDoc construction/destruction

CAppRenderDoc::CAppRenderDoc()
{
	// TODO: add one-time construction code here
	m_DlgHierarchy = &CB3GetMainFrame()->m_dlgHierarchy;
	m_RaytraceDoc  = null;
	m_Raytracer    = new b3Thread("Raytracing master thread");
	m_Display      = null;
	m_FirstVisible = null;
	m_Selected     = null;
	m_UndoBuffer   = new b3LinesUndoBuffer(this);
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
	CB3LinesDocument::OnCloseDocument();
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
	b3_bool reorg,
	b3_bool material_changed)
{
}

void CAppRenderDoc::b3ComputeBounds()
{
}

void CAppRenderDoc::b3InitTree()
{
}

void CAppRenderDoc::b3HierarchySelectionChanged()
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

/*************************************************************************
**                                                                      **
**                        Undo/Redo methods                             **
**                                                                      **
*************************************************************************/

void CAppRenderDoc::b3AddUndoAction(CB3Action *action)
{
}

void CAppRenderDoc::b3AddOp(b3UndoOperation *op)
{
	m_UndoBuffer->b3Do(op);
}

void CAppRenderDoc::b3ClearOp()
{
	m_UndoBuffer->b3Clear();
}

void CAppRenderDoc::b3UndoList(CRect *rect)
{
	CDlgUndoRedo dlg(m_UndoBuffer,B3_LIST_UNDO,rect);

	dlg.DoModal();
}

void CAppRenderDoc::b3RedoList(CRect *rect)
{
	CDlgUndoRedo dlg(m_UndoBuffer,B3_LIST_REDO,rect);

	dlg.DoModal();
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
