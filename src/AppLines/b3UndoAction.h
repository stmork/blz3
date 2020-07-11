/*
**
**	$Filename:	b3UndoAction.h $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
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
#include "blz3/base/b3Array.h"

class b3Scene;
class b3CameraPart;

class b3OpObjectTransformation : public b3Operation
{
	b3Array<b3BBox *>  m_Active;

protected:
	b3Scene      *     m_Scene;
	b3_matrix          m_UndoAction;
	b3_matrix          m_RedoAction;

protected:
	b3OpObjectTransformation(b3Scene * scene);
	void b3Undo();
	void b3Redo();

private:
	void b3Reactivate();
};

class b3OpObjectAction : public b3OpObjectTransformation
{
public:
	b3OpObjectAction(b3Scene * scene, b3_matrix * matrix);

protected:
	inline void b3Do()
	{
		// Do nothing since action has already done it!
	}

	inline int  b3GetId()
	{
		return IDS_OP_ACTION;
	}
};

class b3OpMove : public b3OpObjectTransformation
{
public:
	b3OpMove(b3Scene * scene, b3_vector * stepper);

protected:
	inline int  b3GetId()
	{
		return IDS_OP_MOVE;
	}
};

class b3OpRotate : public b3OpObjectTransformation
{
public:
	b3OpRotate(b3Scene * scene, b3_line * axis, b3_f64 angle);

protected:
	inline int  b3GetId()
	{
		return IDS_OP_ROTATE;
	}
};

class b3OpCameraAction : public b3Operation
{
	b3CameraPart   *   m_Camera;
	b3_matrix          m_UndoAction;
	b3_matrix          m_RedoAction;

public:
	b3OpCameraAction(b3CameraPart * camera, b3_matrix * matrix);

protected:
	void b3Do();
	void b3Undo();
	void b3Redo();
	void b3Prepare(CAppRenderDoc * pDoc);

	inline int  b3GetId()
	{
		return IDS_OP_ACTION;
	}
};

#endif
