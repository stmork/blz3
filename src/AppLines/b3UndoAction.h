/*
**
**	$Filename:	b3UndoAction.h $
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

#ifndef B3UNDOACTION_H
#define B3UNDOACTION_H

#include "AppLines.h"
#include "b3Undo.h"

class b3OpObjectAction : public b3Operation
{
	b3Array<b3BBox *>  m_Active;
	b3Scene           *m_Scene;
	b3_matrix          m_UndoAction;
	b3_matrix          m_RedoAction;

public:
	     b3OpObjectAction(b3Scene *scene,b3_matrix *matrix);

protected:
	void b3Do();
	void b3Undo();
	void b3Redo();

	inline int  b3GetId()
	{
		return IDS_OP_ACTION;
	}
};

class b3OpCameraAction : public b3Operation
{
	b3CameraPart      *m_Camera;
	b3_matrix          m_UndoAction;
	b3_matrix          m_RedoAction;

public:
	     b3OpCameraAction(b3CameraPart *camera,b3_matrix *matrix);

protected:
	void b3Do();
	void b3Undo();
	void b3Redo();

	inline int  b3GetId()
	{
		return IDS_OP_ACTION;
	}
};

#endif
