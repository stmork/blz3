/*
**
**	$Filename:	b3UndoAction.cpp
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Undo/Redo for mouse actions
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

#include "b3UndoAction.h"
#include "b3Action.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2003/01/28 15:58:27  sm
**	- Added support for undoing/redoing picking
**
*/

/*************************************************************************
**                                                                      **
**                        Undo/Redo picking                             **
**                                                                      **
*************************************************************************/

b3OpUndoPick::b3OpUndoPick(
	b3_vector *undo,
	b3_vector *redo)
{
	m_UndoVec = *undo;
	m_RedoVec = *redo;
	m_Vector  =  redo;
}

void b3OpUndoPick::b3Do()
{
	// Do nothing!
}

void b3OpUndoPick::b3Redo()
{
	*m_Vector = m_RedoVec;
}

void b3OpUndoPick::b3Undo()
{
	*m_Vector = m_UndoVec;
}
