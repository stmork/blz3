/*
**
**	$Filename:	b3UndoObject.cpp
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

#include "b3UndoObject.h"
#include "b3Action.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2003/01/11 12:30:30  sm
**	- Some additional undo/redo actions
**
*/

/*************************************************************************
**                                                                      **
**                        Undo/Redo object creation                     **
**                                                                      **
*************************************************************************/

b3OpObjectCreate::b3OpObjectCreate()
{
}

void b3OpObjectCreate::b3Do()
{
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo object deletion                     **
**                                                                      **
*************************************************************************/

b3OpObjectDelete::b3OpObjectDelete()
{
}

void b3OpObjectDelete::b3Do()
{
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo object load                         **
**                                                                      **
*************************************************************************/

b3OpObjectLoad::b3OpObjectLoad()
{
}

void b3OpObjectLoad::b3Do()
{
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo object replacement                  **
**                                                                      **
*************************************************************************/

b3OpObjectReplace::b3OpObjectReplace()
{
}

void b3OpObjectReplace::b3Do()
{
}

/*************************************************************************
**                                                                      **
**                        Undo/Redo object copy                         **
**                                                                      **
*************************************************************************/

b3OpObjectCopy::b3OpObjectCopy()
{
}

void b3OpObjectCopy::b3Do()
{
}
