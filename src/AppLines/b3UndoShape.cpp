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

#include "AppLinesInclude.h"

#include "DlgNewObject.h"

#include "b3UndoShape.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.8  2006/03/25 22:11:20  sm
**	- Fix shape/object creation problem in object editor.
**	- Added double click option in hierarchy tree.
**
**	Revision 1.7  2006/03/05 22:12:33  sm
**	- Added precompiled support for faster comiling :-)
**	
**	Revision 1.6  2005/06/12 11:38:51  sm
**	- Fix ticket no.30. Surface property changes are reflected
**	  to document modified changes.
**	
**	Revision 1.5  2005/05/13 15:33:54  sm
**	- Tested surface property operations.
**	
**	Revision 1.4  2005/05/12 20:16:12  sm
**	- Some more undo/redo surface operations.
**	
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

b3OpShapeCreate::b3OpShapeCreate(
	b3BBox        *root,
	CAppObjectDoc *pDoc,
	CDlgHierarchy *hierarchy) :
		b3OpShape(root, pDoc, hierarchy)
{
	b3ItemEditCall  call;
	CDlgNewObject   dlg;
	int             result;

	m_InsertAfter = m_DlgHierarchy->b3GetSelectedShape();
	m_Selected    = ((m_InsertAfter != null) && (m_InsertAfter->b3GetClass() != CLASS_BBOX) ?
		m_BBox->b3FindParentBBox((b3Shape *)m_InsertAfter) :
		m_DlgHierarchy->b3GetSelectedBBox());
	if (m_Selected != null)
	{
		dlg.m_InsertAfter = m_InsertAfter;
		dlg.m_BBox        = m_Selected;

		if ((dlg.DoModal() == IDOK) && (dlg.m_NewItem != null))
		{
			// Open edit dialog if available
			call   = CB3ImageList::b3GetEditCall(dlg.m_NewItem);
			result = (call != null ? call(m_pObjDoc->b3GetSceneType(), dlg.m_NewItem,true) : IDOK);
			if (result == IDOK)
			{
				// Manually insert to prevent uninitialized redraw
				// of new item.
				m_NewItem = dlg.m_NewItem;
				if (m_NewItem->b3GetClass() == CLASS_BBOX)
				{
					m_Base  = m_Selected->b3GetBBoxHead();
				}
				else
				{
					m_Base  = m_Selected->b3GetShapeHead();
				}

				b3Initialize();
				m_PrepareGeometry         = true;
				m_PrepareChangedStructure = true;
				m_UpdateMaterial          = true;
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
	m_Base->b3Remove(m_NewItem);
	m_DlgHierarchy->b3SelectItem(m_InsertAfter);
}

void b3OpShapeCreate::b3Redo()
{
	m_Base->b3Insert(m_InsertAfter, m_NewItem);
	m_DlgHierarchy->b3SelectItem(m_NewItem);
}

void b3OpShapeCreate::b3Delete()
{
	if (!b3IsDone() && (m_NewItem != null))
	{
		delete m_NewItem;
	}
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo shape creation                      **
**                                                                      **
*************************************************************************/

b3OpShapeEdit::b3OpShapeEdit(
	b3BBox        *root,
	CAppObjectDoc *pDoc,
	CDlgHierarchy *hierarchy) :
		b3OpShape(root, pDoc, hierarchy)
{
	b3ItemEditCall  call;

	m_Selected = m_DlgHierarchy->b3GetSelectedItem();
	if (m_Selected != null)
	{
		call = CB3ImageList::b3GetEditCall(m_Selected);
		if (call != null)
		{
			m_Clone = b3World::b3Clone(m_Selected);
			if (call(m_pObjDoc->b3GetSceneType(), m_Clone, false) == IDOK)
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

b3OpShapeDelete::b3OpShapeDelete(
	b3BBox        *root,
	CAppObjectDoc *pDoc,
	CDlgHierarchy *hierarchy) :
		b3OpShape(root, pDoc, hierarchy)
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
