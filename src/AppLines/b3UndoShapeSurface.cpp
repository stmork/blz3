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

#include "blz3/raytrace/b3BBox.h"

#include "AppObjectDoc.h"
#include "DlgItemMaintain.h"
#include "DlgCopyProperties.h"

#include "b3UndoShapeSurface.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2005/05/14 19:01:24  sm
**	- Added shape property copy to undo/redo operations
**
**	
*/

/*************************************************************************
**                                                                      **
**                        Undo/Redo surface list edit                   **
**                                                                      **
*************************************************************************/

b3OpShapeSurfaceList::b3OpShapeSurfaceList(b3BBox *root, CDlgHierarchy *hierarchy) :
	b3OpShape(root, hierarchy)
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

b3OpShapeMaterialEditList::b3OpShapeMaterialEditList(CAppObjectDoc *pDoc, b3BBox *root, CDlgHierarchy *hierarchy) :
	b3OpShapeSurfaceList(root, hierarchy)
{
	if (m_Shape != null)
	{
		m_Base = m_Shape->b3GetMaterialHead();
		m_CloneBase.b3InitBase(m_Base->b3GetClass());
		m_Shape->b3Store();
		b3World::b3CloneBase(m_Base,&m_CloneBase);
		m_OrigBase.b3InitBase(m_Base->b3GetClass());

		CDlgItemMaintain dlg(pDoc,&m_CloneBase);

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
}

void b3OpShapeMaterialEditList::b3Redo()
{
	b3OpShapeSurfaceList::b3Redo();
	m_Shape->b3RecomputeMaterial();
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo bump list edit                      **
**                                                                      **
*************************************************************************/

b3OpShapeBumpEditList::b3OpShapeBumpEditList(CAppObjectDoc *pDoc, b3BBox *root, CDlgHierarchy *hierarchy) :
	b3OpShapeSurfaceList(root, hierarchy)
{
	if (m_Shape != null)
	{
		m_Base = m_Shape->b3GetBumpHead();
		m_CloneBase.b3InitBase(m_Base->b3GetClass());
		m_Shape->b3Store();
		b3World::b3CloneBase(m_Base,&m_CloneBase);
		m_OrigBase.b3InitBase(m_Base->b3GetClass());

		CDlgItemMaintain dlg(pDoc,&m_CloneBase);

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

b3OpShapeConditionEditList::b3OpShapeConditionEditList(CAppObjectDoc *pDoc, b3BBox *root, CDlgHierarchy *hierarchy) :
	b3OpShapeSurfaceList(root, hierarchy)
{
	if (m_Shape != null)
	{
		m_Base = m_Shape->b3GetConditionHead();
		m_CloneBase.b3InitBase(m_Base->b3GetClass());
		m_Shape->b3Store();
		b3World::b3CloneBase(m_Base,&m_CloneBase);
		m_OrigBase.b3InitBase(m_Base->b3GetClass());

		CDlgItemMaintain dlg(pDoc,&m_CloneBase);

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
}

void b3OpShapeConditionEditList::b3Redo()
{
	b3OpShapeSurfaceList::b3Redo();
	m_Shape->b3RecomputeIndices();
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo surface item edit                   **
**                                                                      **
*************************************************************************/

b3OpShapeSurfaceItem::b3OpShapeSurfaceItem(b3BBox *root, CDlgHierarchy *hierarchy) :
	b3OpShape(root, hierarchy)
{
	m_InsertAfter     = null;
	m_UpdateMaterial  = true;
	m_PrepareGeometry = false;
}

void b3OpShapeSurfaceItem::b3Undo()
{
	m_Base->b3Remove(m_Clone);
	m_Base->b3Insert(m_InsertAfter, m_Original);
}

void b3OpShapeSurfaceItem::b3Redo()
{
	m_Base->b3Remove(m_Original);
	m_Base->b3Insert(m_InsertAfter, m_Clone);
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
	CAppObjectDoc *pDoc,
	b3BBox        *root,
	b3Shape       *shape,
	CDlgHierarchy *hierarchy) :
		b3OpShapeSurfaceItem(root, hierarchy)
{
	B3_ASSERT(shape != null);
	m_Shape    = shape;
	m_Base     = m_Shape->b3GetMaterialHead();

	B3_ASSERT(m_Base->b3GetCount() == 1);
	m_Original = m_Base->First;
	m_Clone    = b3World::b3Clone(m_Original);
	if (b3Loader::b3GetLoader().b3Edit(m_Clone, pDoc))
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
}

void b3OpShapeMaterialEdit::b3Redo()
{
	b3OpShapeSurfaceItem::b3Redo();
	m_Shape->b3RecomputeMaterial();
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo single bump edit                    **
**                                                                      **
*************************************************************************/

b3OpShapeBumpEdit::b3OpShapeBumpEdit(
	CAppObjectDoc *pDoc,
	b3BBox        *root,
	b3Shape       *shape,
	CDlgHierarchy *hierarchy) :
		b3OpShapeSurfaceItem(root, hierarchy)
{
	B3_ASSERT(shape != null);
	m_Shape    = shape;
	m_Base     = m_Shape->b3GetBumpHead();
	
	B3_ASSERT(m_Base->b3GetCount() == 1);
	m_Original = m_Base->First;
	m_Clone    = b3World::b3Clone(m_Original);
	if (b3Loader::b3GetLoader().b3Edit(m_Clone, pDoc))
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
	CAppObjectDoc *pDoc,
	b3BBox        *root,
	b3Shape       *shape,
	CDlgHierarchy *hierarchy) :
		b3OpShapeSurfaceItem(root, hierarchy)
{
	B3_ASSERT(shape != null);
	m_Shape    = shape;
	m_Base     = m_Shape->b3GetConditionHead();

	B3_ASSERT(m_Base->b3GetCount() == 1);
	m_Original = m_Base->First;
	m_Clone    = b3World::b3Clone(m_Original);
	if (b3Loader::b3GetLoader().b3Edit(m_Clone, pDoc))
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
}

void b3OpShapeConditionEdit::b3Redo()
{
	b3OpShapeSurfaceItem::b3Redo();
	m_Shape->b3RecomputeIndices();
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo surface list edit                   **
**                                                                      **
*************************************************************************/

b3OpShapeCopySurface::b3OpShapeCopySurface(b3BBox *root, CDlgHierarchy *hierarchy) :
	b3OpShape(root, hierarchy)
{
	m_Shape = m_DlgHierarchy->b3GetSelectedShape();
	if (m_Shape != null)
	{
		CDlgCopyProperties dlg;

		if (dlg.DoModal() == IDOK)
		{
			dlg.b3CopyProperties(&m_CopyInfo, m_BBox, m_Shape);

			b3Initialize();
			m_UpdateMaterial = true;
			m_PrepareGeometry = true;
			m_PrepareChangedStructure = false;
		}
	}
}

void b3OpShapeCopySurface::b3Undo()
{
	m_CopyInfo.b3Undo();
}

void b3OpShapeCopySurface::b3Redo()
{
	m_CopyInfo.b3Redo();
}

void b3OpShapeCopySurface::b3Delete()
{
	m_CopyInfo.b3Delete(b3IsDone());
}
