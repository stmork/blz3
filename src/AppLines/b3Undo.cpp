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

#include "b3Undo.h"
#include "blz3/b3Config.h"
#include "AppRenderDoc.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
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

b3UndoBuffer::b3UndoBuffer(CAppRenderDoc *pDoc)
{
	m_UndoBuffer.b3InitBase();
	m_RedoBuffer.b3InitBase();
	m_Doc = pDoc;
}

b3UndoBuffer::~b3UndoBuffer()
{
	b3Clear();
}

void b3UndoBuffer::b3Delete(b3Operation *op)
{
	if (op->b3IsInitialized())
	{
		op->b3Delete();
	}
	delete op;
}

void b3UndoBuffer::b3Delete(b3Base<b3Operation> *buffer)
{
	b3Operation *op;

	while((op = buffer->b3RemoveFirst()) != null)
	{
		b3Delete(op);
	}
}

void b3UndoBuffer::b3Clear()
{
	b3Delete(&m_UndoBuffer);
	b3Delete(&m_RedoBuffer);
}

b3_bool b3UndoBuffer::b3Do(b3Operation *op)
{
	b3_bool result = op->b3IsInitialized();

	if (result)
	{
		op->b3Do();
		op->b3Done(true);
		op->b3Prepare(m_Doc);
		b3Delete(&m_RedoBuffer);
		m_UndoBuffer.b3Append(op);
	}
	else
	{
		b3Delete(op);
	}
	return result;
}

void b3UndoBuffer::b3Undo()
{
	b3Operation *op = m_UndoBuffer.b3RemoveLast();

	if (op != null)
	{
		op->b3Undo();
		op->b3Done(false);
		op->b3Prepare(m_Doc);
		m_RedoBuffer.b3First(op);
	}
}

void b3UndoBuffer::b3Redo()
{
	b3Operation *op = m_RedoBuffer.b3RemoveFirst();

	if (op != null)
	{
		op->b3Redo();
		op->b3Done(true);
		op->b3Prepare(m_Doc);
		m_UndoBuffer.b3Append(op);
	}
}

b3_bool b3UndoBuffer::b3HasUndo()
{
	return !m_UndoBuffer.b3IsEmpty();
}

b3_bool b3UndoBuffer::b3HasRedo()
{
	return !m_RedoBuffer.b3IsEmpty();
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo                                     **
**                                                                      **
*************************************************************************/

b3Operation::b3Operation() : b3Link<b3Operation>(sizeof(b3Operation))
{
	m_Initialized             = false;
	m_Done                    = false;
	m_PrepareGeometry         = true;
	m_PrepareChangedStructure = false;
}

b3Operation::~b3Operation()
{
}

void b3Operation::b3Delete()
{
}

void b3Operation::b3Do()
{
	b3Redo();
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

void b3Operation::b3Prepare(CAppRenderDoc *pDoc)
{
	pDoc->b3Prepare(
		m_PrepareGeometry,
		m_PrepareChangedStructure);
}
