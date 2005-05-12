/*
**
**	$Filename:	b3UndoShape.cpp
**	$Release:	Dortmund 2005 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Undo/Redo for shape handling
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
#include "DlgNewObject.h"
#include "DlgItemMaintain.h"

#include "b3UndoShape.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2005/05/12 12:16:25  sm
**	- Added surface property editing for undo/redo editing.
**
**	Revision 1.2  2005/05/11 16:13:19  sm
**	- Added following undo/redo operations for object editor:
**	  o shape edit
**	  o shape delete
**	
**	Revision 1.1  2005/05/11 14:19:29  sm
**	- Added first Undo operation for object editor.
**	
**	
*/

/*************************************************************************
**                                                                      **
**                        Undo/Redo shape creation                      **
**                                                                      **
*************************************************************************/

b3OpShapeCreate::b3OpShapeCreate(CAppObjectDoc *doc, b3BBox *root, CDlgHierarchy *hierarchy) :
	b3OpShape(root, hierarchy)
{
	b3ItemEditCall  call;
	CDlgNewObject   dlg;
	int             result;

	m_InsertAfter = m_DlgHierarchy->b3GetSelectedShape();
	m_Selected    = (m_InsertAfter != null ?
		m_BBox->b3FindParentBBox(m_InsertAfter) :
		m_DlgHierarchy->b3GetSelectedBBox());
	if (m_Selected != null)
	{
		dlg.m_InsertAfter = m_InsertAfter;
		dlg.m_BBox        = m_Selected;

		if ((dlg.DoModal() == IDOK) && (dlg.m_NewItem != null))
		{
			// Open edit dialog if available
			call   = CB3ImageList::b3GetEditCall(dlg.m_NewItem);
			result = (call != null ? call(doc->b3GetSceneType(), dlg.m_NewItem,true) : IDOK);
			if (result == IDOK)
			{
				// Manually insert to prevent uninitialized redraw
				// of new item.
				m_Shape = (b3Shape *)dlg.m_NewItem;
				m_Base  = m_Selected->b3GetShapeHead();

				b3Initialize();
				m_PrepareGeometry         = true;
				m_PrepareChangedStructure = true;
				m_BBox->b3BacktraceRecompute(m_Selected);
			}
			else
			{
				delete dlg.m_NewItem;
			}
		}
	}
}

void b3OpShapeCreate::b3Undo()
{
	m_Base->b3Remove(m_Shape);
	m_DlgHierarchy->b3SelectItem(m_Selected);
}

void b3OpShapeCreate::b3Redo()
{
	m_Base->b3Insert(m_InsertAfter, m_Shape);
	m_DlgHierarchy->b3SelectItem(m_Shape);
}

void b3OpShapeCreate::b3Delete()
{
	if (!b3IsDone() && (m_Shape != null))
	{
		delete m_Shape;
	}
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo shape creation                      **
**                                                                      **
*************************************************************************/

b3OpShapeEdit::b3OpShapeEdit(CAppObjectDoc *doc, b3BBox *root, CDlgHierarchy *hierarchy) :
	b3OpShape(root, hierarchy)
{
	b3ItemEditCall  call;

	m_Selected = m_DlgHierarchy->b3GetSelectedItem();
	if (m_Selected != null)
	{
		call = CB3ImageList::b3GetEditCall(m_Selected);
		if (call != null)
		{
			m_Clone = b3World::b3Clone(m_Selected);
			if (call(doc->b3GetSceneType(), m_Clone, false) == IDOK)
			{
				b3BBox *parent;

				m_InsertAfter = m_Selected->Prev;
				switch(m_Selected->b3GetClass())
				{
				case CLASS_BBOX:
					m_Base = m_BBox->b3FindBBoxHead((b3BBox *)m_Selected);
					break;

				case CLASS_SHAPE:
				case CLASS_CSG:
					parent = m_BBox->b3FindParentBBox((b3Shape *)m_Selected);
					m_Base = parent->b3GetShapeHead();
					break;

				default:
					m_Base = null;
					break;
				}

				b3Initialize();
				m_PrepareGeometry         = true;
				m_PrepareChangedStructure = true;
			}
			else
			{
				delete m_Clone;
			}
		}
	}
}

void b3OpShapeEdit::b3Undo()
{
	m_Base->b3Remove(m_Clone);
	m_Base->b3Insert(m_InsertAfter, m_Selected);
	m_DlgHierarchy->b3SelectItem(m_Selected);
}

void b3OpShapeEdit::b3Redo()
{
	m_Base->b3Remove(m_Selected);
	m_Base->b3Insert(m_InsertAfter, m_Clone);
	m_DlgHierarchy->b3SelectItem(m_Clone);
}

void b3OpShapeEdit::b3Delete()
{
	if (!b3IsDone())
	{
		if (m_Clone != null)
		{
			delete m_Clone;
		}
	}
	else
	{
		if (m_Selected != null)
		{
			delete m_Selected;
		}
	}
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo shape creation                      **
**                                                                      **
*************************************************************************/

b3OpShapeDelete::b3OpShapeDelete(b3BBox *root, CDlgHierarchy *hierarchy) :
	b3OpShape(root, hierarchy)
{
	b3Base<b3Item> *base;
	b3BBox         *parent;
	b3BBox         *backtrace;
	int             answere;

	m_Selected = m_DlgHierarchy->b3GetSelectedItem();
	if (m_Selected != null)
	{
		// Security question depending on item type
		switch(m_Selected->b3GetClass())
		{
		case CLASS_BBOX:
			answere   = AfxMessageBox(IDS_ASK_DELETE_OBJECT,MB_ICONQUESTION|MB_YESNO);
			m_Base    = m_BBox->b3FindBBoxHead((b3BBox *)m_Selected);
			backtrace = (b3BBox *)m_Selected;
			break;

		case CLASS_SHAPE:
		case CLASS_CSG:
			answere   = AfxMessageBox(IDS_ASK_DELETE_SHAPE,MB_ICONQUESTION|MB_YESNO);
			parent    = m_BBox->b3FindParentBBox((b3Shape *)m_Selected);
			m_Base    = parent->b3GetShapeHead();
			backtrace = parent;
			break;

		default:
			answere   = IDCANCEL;
			base      = null;
			backtrace = null;
			break;
		}

		// If the user really wants to delete item...
		if(answere == IDYES)
		{
			if (backtrace != null)
			{
				m_BBox->b3BacktraceRecompute(backtrace);
			}

			B3_ASSERT(m_Base != null);
			m_Select = (m_Selected->Succ != null ? m_Selected->Succ : m_Selected->Prev);
			m_InsertAfter = m_Selected->Prev;

			b3Initialize();
			m_PrepareGeometry         = false;
			m_PrepareChangedStructure = true;
		}
	}
}

void b3OpShapeDelete::b3Undo()
{
	m_Base->b3Insert(m_InsertAfter, m_Selected);
	m_DlgHierarchy->b3SelectItem(m_Selected);
}

void b3OpShapeDelete::b3Redo()
{
	m_Base->b3Remove(m_Selected);
	m_DlgHierarchy->b3SelectItem(m_Select);
}

void b3OpShapeDelete::b3Delete()
{
	if (b3IsDone() && (m_Selected != null))
	{
		delete m_Selected;
	}
}

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
	m_Shape = m_DlgHierarchy->b3GetSelectedShape();

	if (m_Shape != null)
	{
		m_Base = m_Shape->b3GetMaterialHead();
		b3World::b3CloneBase(m_Base,&m_CloneBase);
		m_OrigBase.b3InitBase(m_Base->b3GetClass());

		CDlgItemMaintain dlg(pDoc,&m_CloneBase);

		if (dlg.DoModal() == IDOK)
		{
			b3Initialize();
			m_UpdateMaterial = true;
		}
	}
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
		b3World::b3CloneBase(m_Base,&m_CloneBase);
		m_OrigBase.b3InitBase(m_Base->b3GetClass());

		CDlgItemMaintain dlg(pDoc,&m_CloneBase);

		if (dlg.DoModal() == IDOK)
		{
			b3Initialize();
			m_UpdateMaterial = true;
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
		b3World::b3CloneBase(m_Base,&m_CloneBase);
		m_OrigBase.b3InitBase(m_Base->b3GetClass());

		CDlgItemMaintain dlg(pDoc,&m_CloneBase);

		if (dlg.DoModal() == IDOK)
		{
			b3Initialize();
			m_UpdateMaterial = true;
		}
	}
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo surface item edit                   **
**                                                                      **
*************************************************************************/

b3OpShapeSurfaceItem::b3OpShapeSurfaceItem(b3BBox *root, CDlgHierarchy *hierarchy) :
	b3OpShape(root, hierarchy)
{
	m_Shape       = m_DlgHierarchy->b3GetSelectedShape();
	m_InsertAfter = null;
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
		m_UpdateMaterial = true;
	}
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
		m_UpdateMaterial = true;
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
		m_UpdateMaterial = true;
	}
}
