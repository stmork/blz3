/*
**
**	$Filename:	b3Undo.cpp
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

#include "b3Undo.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.9  2006/03/05 22:12:33  sm
**	- Added precompiled support for faster comiling :-)
**
**	Revision 1.8  2005/05/12 12:16:25  sm
**	- Added surface property editing for undo/redo editing.
**	
**	Revision 1.7  2003/02/01 12:57:17  sm
**	- Ready to undo/redo!
**	
**	Revision 1.6  2003/01/30 19:49:55  sm
**	- Further undo/redo history dialog build.
**	
**	Revision 1.5  2003/01/28 15:58:27  sm
**	- Added support for undoing/redoing picking
**	
**	Revision 1.4  2003/01/12 10:39:45  sm
**	- Fixed proper delete for undo/redo.
**	
**	Revision 1.3  2003/01/11 17:16:15  sm
**	- Object handling with undo/redo
**	- Light handling with undo/redo
**	
**	Revision 1.2  2003/01/07 16:14:38  sm
**	- Lines III: object editing didn't prepared any more. Fixed.
**	- Some prepare optimizations.
**	
**	Revision 1.1  2003/01/05 16:13:24  sm
**	- First undo/redo implementations
**	
**
*/

/*************************************************************************
**                                                                      **
**                        Undo/Redo                                     **
**                                                                      **
*************************************************************************/

b3LinesUndoBuffer::b3LinesUndoBuffer(CAppRenderDoc *pDoc)
{
	m_PrepareInfo = new b3LinesUndoPrepareInfo(pDoc);
}

b3LinesUndoBuffer::~b3LinesUndoBuffer()
{
	delete m_PrepareInfo;
}

void b3LinesUndoBuffer::b3FillUndoList(CListBox &listbox)
{
	CString          label;
	b3UndoOperation *op;
	int              index;

	B3_FOR_BASE_BACK(&m_UndoBuffer,op)
	{
		label.LoadString(op->b3GetId());
		index = listbox.AddString(label);
		listbox.SetItemDataPtr(index,op);
	}
}

void b3LinesUndoBuffer::b3FillRedoList(CListBox &listbox)
{
	CString          label;
	b3UndoOperation *op;
	int              index;

	B3_FOR_BASE(&m_RedoBuffer,op)
	{
		label.LoadString(op->b3GetId());
		index = listbox.AddString(label);
		listbox.SetItemDataPtr(index,op);
	}
}

void b3LinesUndoBuffer::b3UndoList(b3UndoOperation *op)
{
	b3_bool end;

	do
	{
		end = m_UndoBuffer.Last == op;
		b3Undo();
	}
	while(!end);
}

void b3LinesUndoBuffer::b3RedoList(b3UndoOperation *op)
{
	b3_bool end;

	do
	{
		end = m_RedoBuffer.First == op;
		b3Redo();
	}
	while(!end);
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo                                     **
**                                                                      **
*************************************************************************/

b3Operation::b3Operation()
{
	m_PrepareGeometry         = true;
	m_PrepareChangedStructure = false;
	m_UpdateMaterial          = false;
}

b3Operation::~b3Operation()
{
}

void b3Operation::b3Undo()
{
	CString desc;

	desc.LoadString(b3GetId());
	b3PrintF(B3LOG_NORMAL,"Undo-Operation for %s not overloaded.\n",desc);
}

void b3Operation::b3Redo()
{
	CString desc;

	desc.LoadString(b3GetId());
	b3PrintF(B3LOG_NORMAL,"Redo-Operation for %s not overloaded.\n",desc);
}

void b3Operation::b3Prepare(b3UndoPrepareInfo *info)
{
	b3LinesUndoPrepareInfo *lupi = (b3LinesUndoPrepareInfo *)info;

	b3Prepare(lupi->m_pDoc);
}

void b3Operation::b3Prepare(CAppRenderDoc *pDoc)
{
	pDoc->b3Prepare(
		m_PrepareGeometry,
		m_PrepareChangedStructure,
		false,
		m_UpdateMaterial);
}
