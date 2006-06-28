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

#include "AppLinesInclude.h"
#include "b3UndoPick.h"
#include "blz3/base/b3Render.h"

/*************************************************************************
**                                                                      **
**                        Undo/Redo picking                             **
**                                                                      **
*************************************************************************/

b3OpUndoPick::b3OpUndoPick(
	b3_vector      *undo,
	b3_vector      *redo,
	b3RenderObject *object)
{
	m_UndoVec      = *undo;
	m_RedoVec      = *redo;
	m_Vector       =  redo;
	m_RenderObject = object;
	b3Initialize();
}

void b3OpUndoPick::b3Do()
{
	// Do nothing!
}

void b3OpUndoPick::b3Redo()
{
	*m_Vector = m_RedoVec;
	if (m_RenderObject != null)
	{
		m_RenderObject->b3Recompute();
	}
}

void b3OpUndoPick::b3Undo()
{
	*m_Vector = m_UndoVec;
	if (m_RenderObject != null)
	{
		m_RenderObject->b3Recompute();
	}
}
