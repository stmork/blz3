/*
**
**	$Filename:	b3UndoCutPaste.cpp
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Undo/Redo
**
**	(C) Copyright 2003  Steffen A. Mork
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
#include "DlgHierarchy.h"

#include "b3UndoCutPaste.h"

#include "blz3/base/b3FileMem.h"

/*************************************************************************
**                                                                      **
**                        Undo/Redo drop                                **
**                                                                      **
*************************************************************************/

b3OpDrop::b3OpDrop(b3Scene *scene,b3BBox *src,b3BBox *dstBBox)
{
	m_Scene   = scene;
	m_SrcBBox = src;
	m_Prev    = src->Prev;

	// Get BBoxes an their bases
	m_SrcBase = m_Scene->b3FindBBoxHead(m_SrcBBox);
	m_DstBase = (dstBBox != null ? dstBBox->b3GetBBoxHead() : m_Scene->b3GetBBoxHead());

	b3Initialize();
	m_PrepareGeometry = false;
	m_PrepareChangedStructure = true;
}

void b3OpDrop::b3Undo()
{
	// Mark every ancestor before relink as changed
	m_Scene->b3BacktraceRecompute(m_SrcBBox);

	// Relink BBox
	m_DstBase->b3Remove(m_SrcBBox);
	m_SrcBase->b3Insert(m_Prev,m_SrcBBox);

	// Mark every ancestor after relink as changed
	m_Scene->b3Recount();
	m_Scene->b3BacktraceRecompute(m_SrcBBox);
}

void b3OpDrop::b3Redo()
{
	// Mark every ancestor before relink as changed
	m_Scene->b3BacktraceRecompute(m_SrcBBox);

	// Relink BBox
	m_SrcBase->b3Remove(m_SrcBBox);
	m_DstBase->b3Append(m_SrcBBox);

	// Mark every ancestor after relink as changed
	m_Scene->b3Recount();
	m_Scene->b3BacktraceRecompute(m_SrcBBox);
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo Cut                                 **
**                                                                      **
*************************************************************************/

b3OpCut::b3OpCut(
	CDlgHierarchy *hierarchy,
	b3Scene       *scene)
{
	CAppLinesApp *app  = CB3GetLinesApp();

	m_DlgHierarchy = hierarchy;
	m_Scene        = scene;
	m_BBox         = m_DlgHierarchy->b3GetSelectedBBox();

	m_DlgHierarchy->b3GetData();
	if (m_BBox != null)
	{
		m_Base   = m_Scene->b3FindBBoxHead(m_BBox);
		m_Prev   = m_BBox->Prev;
		m_Select = (m_BBox->Succ != null ? m_BBox->Succ : m_BBox->Prev);
		m_Base->b3Remove(m_BBox);
		if(app->b3PutClipboard(m_BBox))
		{
			b3Initialize();
			m_PrepareGeometry = false;
			m_PrepareChangedStructure = true;
		}
		m_Base->b3Insert(m_Prev,m_BBox);
	}
}

void b3OpCut::b3Delete()
{
	if (b3IsDone() && (m_BBox != null))
	{
		delete m_BBox;
	}
}

void b3OpCut::b3Undo()
{
	m_Base->b3Insert(m_Prev,m_BBox);
	m_Scene->b3BacktraceRecompute(m_BBox);
	m_DlgHierarchy->b3SelectItem(m_BBox);
}

void b3OpCut::b3Redo()
{
	m_Scene->b3BacktraceRecompute(m_BBox);
	m_Base->b3Remove(m_BBox);
	m_DlgHierarchy->b3SelectItem(m_Select);
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo Paste                               **
**                                                                      **
*************************************************************************/

b3OpPaste::b3OpPaste(
	CDlgHierarchy *hierarchy,
	b3Scene       *scene,
	b3_bool        insert_sub)
{
	CAppLinesApp   *app  = CB3GetLinesApp();
	b3BBox         *selected;

	m_DlgHierarchy = hierarchy;
	m_Scene        = scene;
	selected       = m_DlgHierarchy->b3GetSelectedBBox();
	B3_ASSERT(selected != null);
	if (insert_sub)
	{
		m_Base = selected->b3GetBBoxHead();
		m_InsertAfter = m_Base->Last;
	}
	else
	{
		m_Base = m_Scene->b3FindBBoxHead(selected);
		m_InsertAfter = selected;
	}
	B3_ASSERT(m_Base != null);
	m_BBox = app->b3PasteClipboard(&m_World);
	if (m_BBox != null)
	{
		m_Level = m_BBox->b3GetType();
		b3Initialize();
		m_PrepareChangedStructure = true;
	}
}

void b3OpPaste::b3Delete()
{
	if (!b3IsDone() && (m_BBox != null))
	{
		delete m_BBox;
	}
}

void b3OpPaste::b3Do()
{
	b3BBox::b3Reorg(m_World.b3GetHead(),m_Base,m_Level,1,m_InsertAfter);
	m_Scene->b3Recount();
	m_Scene->b3BacktraceRecompute(m_BBox);
	m_DlgHierarchy->b3SelectItem(m_BBox);
}

void b3OpPaste::b3Undo()
{
	m_Scene->b3BacktraceRecompute(m_BBox);
	m_Base->b3Remove(m_BBox);
	m_DlgHierarchy->b3SelectItem(m_InsertAfter);
}

void b3OpPaste::b3Redo()
{
	m_Base->b3Insert(m_InsertAfter,m_BBox);
	m_Scene->b3BacktraceRecompute(m_BBox);
	m_DlgHierarchy->b3SelectItem(m_BBox);
}
