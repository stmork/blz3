/*
**
**	$Filename:	b3UndoCamera.h $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Undo/Redo for camera actions
**
**	(C) Copyright 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#ifndef B3UNDOCAMERA_H
#define B3UNDOCAMERA_H

#include "AppLines.h"
#include "b3Undo.h"

class b3OpCameraCreate : public b3Operation
{
protected:
	inline int  b3GetId()
	{
		return IDS_OP_CAMERA_CREATE;
	}
};

class b3OpCameraDelete : public b3Operation
{
protected:
	inline int  b3GetId()
	{
		return IDS_OP_CAMERA_DELETE;
	}
};

class b3OpCameraEnable : public b3Operation
{
protected:
	inline int  b3GetId()
	{
		return IDS_OP_CAMERA_ENABLE;
	}
};

#endif
