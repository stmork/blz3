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

#include "b3UndoShape.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
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
