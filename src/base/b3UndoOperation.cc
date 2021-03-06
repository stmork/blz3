/*
**
**	$Filename:	b3UndoOperation.cc
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
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

#include "b3BaseInclude.h"
#include "blz3/base/b3UndoOperation.h"

/*************************************************************************
**                                                                      **
**                        Undo/Redo                                     **
**                                                                      **
*************************************************************************/

b3UndoBuffer::b3UndoBuffer()
{
	m_UndoBuffer.b3InitBase();
	m_RedoBuffer.b3InitBase();
	m_PrepareInfo = nullptr;
}

b3UndoBuffer::~b3UndoBuffer()
{
	b3Clear();
}

void b3UndoBuffer::b3Delete(b3UndoOperation * op)
{
	if (op->b3IsInitialized())
	{
		op->b3Delete();
	}
	delete op;
}

void b3UndoBuffer::b3Delete(b3Base<b3UndoOperation> * buffer)
{
	b3UndoOperation * op;

	while ((op = buffer->b3RemoveFirst()) != nullptr)
	{
		b3Delete(op);
	}
}

void b3UndoBuffer::b3Clear()
{
	b3Delete(&m_UndoBuffer);
	b3Delete(&m_RedoBuffer);
}

bool b3UndoBuffer::b3Do(b3UndoOperation * op)
{
	b3_bool result = op->b3IsInitialized();

	if (result)
	{
		op->b3Do();
		op->b3Done(true);
		op->b3Prepare(m_PrepareInfo);
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
	b3UndoOperation * op = m_UndoBuffer.b3RemoveLast();

	if (op != nullptr)
	{
		op->b3Undo();
		op->b3Done(false);
		op->b3Prepare(m_PrepareInfo);
		m_RedoBuffer.b3First(op);
	}
}

void b3UndoBuffer::b3Redo()
{
	b3UndoOperation * op = m_RedoBuffer.b3RemoveFirst();

	if (op != nullptr)
	{
		op->b3Redo();
		op->b3Done(true);
		op->b3Prepare(m_PrepareInfo);
		m_UndoBuffer.b3Append(op);
	}
}

bool b3UndoBuffer::b3HasUndo() const
{
	return !m_UndoBuffer.b3IsEmpty();
}

bool b3UndoBuffer::b3HasRedo() const
{
	return !m_RedoBuffer.b3IsEmpty();
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo                                     **
**                                                                      **
*************************************************************************/

b3UndoOperation::b3UndoOperation() :
	b3Link<b3UndoOperation>(sizeof(b3UndoOperation))
{
}

void b3UndoOperation::b3Delete()
{
}

void b3UndoOperation::b3Do()
{
	b3Redo();
}

void b3UndoOperation::b3Undo()
{
	b3PrintF(B3LOG_NORMAL, "Undo-Operation for ID %d not overloaded.\n",
		b3GetId());
}

void b3UndoOperation::b3Redo()
{
	b3PrintF(B3LOG_NORMAL, "Redo-Operation for ID %d not overloaded.\n",
		b3GetId());
}
