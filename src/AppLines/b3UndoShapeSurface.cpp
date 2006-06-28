/*
**
**	$Filename:	b3UndoShapeSurface.cpp
**	$Release:	Dortmund 2005 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Undo/Redo for shape surface handling
**
**	(C) Copyright 2005  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "AppLinesInclude.h"

#include "DlgItemMaintain.h"
#include "DlgCopyProperties.h"

#include "b3UndoShapeSurface.h"

/*************************************************************************
**                                                                      **
**                        Undo/Redo surface list edit                   **
**                                                                      **
*************************************************************************/

b3OpShapeSurfaceList::b3OpShapeSurfaceList(
	b3BBox        *root,
	CAppObjectDoc *pDoc,
	CDlgHierarchy *hierarchy) :
		b3OpShape(root, pDoc, hierarchy)
{
	m_Shape = m_DlgHierarchy->b3GetSelectedShape();
}

void b3OpShapeSurfaceList::b3Undo()
{
	m_CloneBase.b3MoveFrom(m_Base);
	m_Base->b3MoveFrom(&m_OrigBase);
}

void b3OpShapeSurfaceList::b3Redo()
{
	m_OrigBase.b3MoveFrom(m_Base);
	m_Base->b3MoveFrom(&m_CloneBase);
}

void b3OpShapeSurfaceList::b3Delete()
{
	if (b3IsDone())
	{
		m_OrigBase.b3Free();
	}
	else
	{
		m_CloneBase.b3Free();
	}
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo material list edit                  **
**                                                                      **
*************************************************************************/

b3OpShapeMaterialEditList::b3OpShapeMaterialEditList(
	b3BBox *root,
	CAppObjectDoc *pDoc,
	CDlgHierarchy *hierarchy) :
		b3OpShapeSurfaceList(root, pDoc, hierarchy)
{
	if (m_Shape != null)
	{
		m_Base = m_Shape->b3GetMaterialHead();
		m_CloneBase.b3InitBase(m_Base->b3GetClass());
		m_Shape->b3Store();
		b3World::b3CloneBase(m_Base,&m_CloneBase);
		m_OrigBase.b3InitBase(m_Base->b3GetClass());

		CDlgItemMaintain dlg(m_pObjDoc,&m_CloneBase);

		if (dlg.DoModal() == IDOK)
		{
			b3Initialize();
			m_UpdateMaterial = true;
		}
		else
		{
			m_CloneBase.b3Free();
		}
	}
}

void b3OpShapeMaterialEditList::b3Undo()
{
	b3OpShapeSurfaceList::b3Undo();
	m_Shape->b3RecomputeMaterial();
	m_pObjDoc->SetModifiedFlag(m_Modified);
}

void b3OpShapeMaterialEditList::b3Redo()
{
	b3OpShapeSurfaceList::b3Redo();
	m_Shape->b3RecomputeMaterial();
	m_pObjDoc->SetModifiedFlag();
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo bump list edit                      **
**                                                                      **
*************************************************************************/

b3OpShapeBumpEditList::b3OpShapeBumpEditList(
	b3BBox *root,
	CAppObjectDoc *pDoc,
	CDlgHierarchy *hierarchy) :
		b3OpShapeSurfaceList(root, pDoc, hierarchy)
{
	if (m_Shape != null)
	{
		m_Base = m_Shape->b3GetBumpHead();
		m_CloneBase.b3InitBase(m_Base->b3GetClass());
		m_Shape->b3Store();
		b3World::b3CloneBase(m_Base,&m_CloneBase);
		m_OrigBase.b3InitBase(m_Base->b3GetClass());

		CDlgItemMaintain dlg(m_pObjDoc,&m_CloneBase);

		if (dlg.DoModal() == IDOK)
		{
			b3Initialize();
			m_UpdateMaterial = true;
		}
		else
		{
			m_CloneBase.b3Free();
		}
	}
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo condition list edit                 **
**                                                                      **
*************************************************************************/

b3OpShapeConditionEditList::b3OpShapeConditionEditList(
	b3BBox *root,
	CAppObjectDoc *pDoc,
	CDlgHierarchy *hierarchy) :
		b3OpShapeSurfaceList(root, pDoc, hierarchy)
{
	if (m_Shape != null)
	{
		m_Base = m_Shape->b3GetConditionHead();
		m_CloneBase.b3InitBase(m_Base->b3GetClass());
		m_Shape->b3Store();
		b3World::b3CloneBase(m_Base,&m_CloneBase);
		m_OrigBase.b3InitBase(m_Base->b3GetClass());

		CDlgItemMaintain dlg(m_pObjDoc, &m_CloneBase);

		if (dlg.DoModal() == IDOK)
		{
			b3Initialize();
			m_UpdateMaterial = true;
		}
		else
		{
			m_CloneBase.b3Free();
		}
	}
}

void b3OpShapeConditionEditList::b3Undo()
{
	b3OpShapeSurfaceList::b3Undo();
	m_Shape->b3RecomputeIndices();
	m_pObjDoc->SetModifiedFlag(m_Modified);
}

void b3OpShapeConditionEditList::b3Redo()
{
	b3OpShapeSurfaceList::b3Redo();
	m_Shape->b3RecomputeIndices();
	m_pObjDoc->SetModifiedFlag();
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo surface item edit                   **
**                                                                      **
*************************************************************************/

b3OpShapeSurfaceItem::b3OpShapeSurfaceItem(
	b3BBox        *root,
	CAppObjectDoc *pDoc,
	CDlgHierarchy *hierarchy) :
		b3OpShape(root, pDoc, hierarchy)
{
	m_InsertAfter     = null;
	m_UpdateMaterial  = true;
	m_PrepareGeometry = false;
}

void b3OpShapeSurfaceItem::b3Undo()
{
	m_Base->b3Remove(m_Clone);
	m_Base->b3Insert(m_InsertAfter, m_Original);
	m_pObjDoc->SetModifiedFlag(m_Modified);
}

void b3OpShapeSurfaceItem::b3Redo()
{
	m_Base->b3Remove(m_Original);
	m_Base->b3Insert(m_InsertAfter, m_Clone);
	m_pObjDoc->SetModifiedFlag();
}

void b3OpShapeSurfaceItem::b3Delete()
{
	if (b3IsDone())
	{
		delete m_Original;
	}
	else
	{
		delete m_Clone;
	}
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo single material edit                **
**                                                                      **
*************************************************************************/

b3OpShapeMaterialEdit::b3OpShapeMaterialEdit(
	b3BBox        *root,
	b3Shape       *shape,
	CAppObjectDoc *pDoc,
	CDlgHierarchy *hierarchy) :
		b3OpShapeSurfaceItem(root, pDoc, hierarchy)
{
	B3_ASSERT(shape != null);
	m_Shape    = shape;
	m_Base     = m_Shape->b3GetMaterialHead();

	B3_ASSERT(m_Base->b3GetCount() == 1);
	m_Original = m_Base->First;
	m_Clone    = b3World::b3Clone(m_Original);
	if (b3Loader::b3GetLoader().b3Edit(m_Clone, m_pObjDoc))
	{
		b3Initialize();
	}
	else
	{
		delete m_Clone;
	}
}

void b3OpShapeMaterialEdit::b3Undo()
{
	b3OpShapeSurfaceItem::b3Undo();
	m_Shape->b3RecomputeMaterial();
	m_pObjDoc->SetModifiedFlag(m_Modified);
}

void b3OpShapeMaterialEdit::b3Redo()
{
	b3OpShapeSurfaceItem::b3Redo();
	m_Shape->b3RecomputeMaterial();
	m_pObjDoc->SetModifiedFlag();
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo single bump edit                    **
**                                                                      **
*************************************************************************/

b3OpShapeBumpEdit::b3OpShapeBumpEdit(
	b3BBox        *root,
	b3Shape       *shape,
	CAppObjectDoc *pDoc,
	CDlgHierarchy *hierarchy) :
		b3OpShapeSurfaceItem(root, pDoc, hierarchy)
{
	B3_ASSERT(shape != null);
	m_Shape    = shape;
	m_Base     = m_Shape->b3GetBumpHead();
	
	B3_ASSERT(m_Base->b3GetCount() == 1);
	m_Original = m_Base->First;
	m_Clone    = b3World::b3Clone(m_Original);
	if (b3Loader::b3GetLoader().b3Edit(m_Clone, m_pObjDoc))
	{
		b3Initialize();
	}
	else
	{
		delete m_Clone;
	}
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo single condition edit               **
**                                                                      **
*************************************************************************/

b3OpShapeConditionEdit::b3OpShapeConditionEdit(
	b3BBox        *root,
	b3Shape       *shape,
	CAppObjectDoc *pDoc,
	CDlgHierarchy *hierarchy) :
		b3OpShapeSurfaceItem(root, pDoc, hierarchy)
{
	B3_ASSERT(shape != null);
	m_Shape    = shape;
	m_Base     = m_Shape->b3GetConditionHead();

	B3_ASSERT(m_Base->b3GetCount() == 1);
	m_Original = m_Base->First;
	m_Clone    = b3World::b3Clone(m_Original);
	if (b3Loader::b3GetLoader().b3Edit(m_Clone, m_pObjDoc))
	{
		b3Initialize();
	}
	else
	{
		delete m_Clone;
	}
}

void b3OpShapeConditionEdit::b3Undo()
{
	b3OpShapeSurfaceItem::b3Undo();
	m_Shape->b3RecomputeIndices();
	m_pObjDoc->SetModifiedFlag(m_Modified);
}

void b3OpShapeConditionEdit::b3Redo()
{
	b3OpShapeSurfaceItem::b3Redo();
	m_Shape->b3RecomputeIndices();
	m_pObjDoc->SetModifiedFlag();
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo surface list edit                   **
**                                                                      **
*************************************************************************/

b3OpShapeCopySurface::b3OpShapeCopySurface(
	b3BBox        *root,
	CAppObjectDoc *pDoc,
	CDlgHierarchy *hierarchy) :
		b3OpShape(root, pDoc, hierarchy)
{
	m_Shape = m_DlgHierarchy->b3GetSelectedShape();
	if (m_Shape != null)
	{
		CDlgCopyProperties dlg;

		if (dlg.DoModal() == IDOK)
		{
			CWaitCursor waiting_for_coffee;

			dlg.b3CopyProperties(&m_CopyInfo, m_BBox, m_Shape);

			b3Initialize();
			m_UpdateMaterial          = true;
			m_PrepareGeometry         = true;
			m_PrepareChangedStructure = false;
		}
	}
}

void b3OpShapeCopySurface::b3Undo()
{
	CWaitCursor wait_for_undo;

	m_CopyInfo.b3Undo();
	m_pObjDoc->SetModifiedFlag(m_Modified);
}

void b3OpShapeCopySurface::b3Redo()
{
	CWaitCursor wait_for_redo;

	m_CopyInfo.b3Redo();
	m_pObjDoc->SetModifiedFlag();
}

void b3OpShapeCopySurface::b3Delete()
{
	m_CopyInfo.b3Delete(b3IsDone());
}
