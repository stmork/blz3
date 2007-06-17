/*
**
**	$Filename:	b3UndoPick.h $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Undo/Redo for light handling
**
**	(C) Copyright 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#ifndef B3UNDOPICK_H
#define B3UNDOPICK_H

#include "AppLines.h"
#include "b3Undo.h"

class b3RenderObject;

class b3OpUndoPick : public b3Operation
{
	b3_vector       m_UndoVec;
	b3_vector       m_RedoVec;
	b3_vector      *m_Vector;
	b3RenderObject *m_RenderObject;

protected:
	     b3OpUndoPick(b3_vector *undo,b3_vector *redo, b3RenderObject *object);
	void b3Do();
	void b3Undo();
	void b3Redo();
};

class b3OpPickPoint : public b3OpUndoPick
{
public:
	inline b3OpPickPoint(b3_vector *undo,b3_vector *redo, b3RenderObject *object) : b3OpUndoPick(undo, redo, object)
	{
	}

	inline int b3GetId()
	{
		return IDS_OP_PICK_POINT;
	}
};

class b3OpPickDir : public b3OpUndoPick
{
public:
	inline b3OpPickDir(b3_vector *undo,b3_vector *redo, b3RenderObject *object) : b3OpUndoPick(undo, redo, object)
	{
	}

	inline int b3GetId()
	{
		return IDS_OP_PICK_DIR;
	}
};

#endif
