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

void b3UndoBuffer::b3Clear()
{
	m_UndoBuffer.b3Free();
	m_RedoBuffer.b3Free();
}

b3_bool b3UndoBuffer::b3Do(b3Operation *op)
{
	b3_bool result = op->b3IsInitialized();

	if (result)
	{
		op->b3Do();
		op->b3Prepare(m_Doc);
		m_RedoBuffer.b3Free();
		m_UndoBuffer.b3Append(op);
	}
	else
	{
		delete op;
	}
	return result;
}

void b3UndoBuffer::b3Undo()
{
	b3Operation *op = m_UndoBuffer.b3RemoveLast();

	if (op != null)
	{
		op->b3Undo();
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
	m_Initialized = false;
}

b3Operation::~b3Operation()
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
	pDoc->b3Prepare(true,false);
}

b3_bool b3Operation::b3IsInitialized()
{
	return m_Initialized;
}
