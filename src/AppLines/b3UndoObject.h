/*
**
**	$Filename:	b3UndoObject.h $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Undo/Redo for object handling
**
**	(C) Copyright 2003  Steffen A. Mork
**	    All Rights Reserved
**
**
**
*/

#ifndef B3UNDOOBJECT_H
#define B3UNDOOBJECT_H

#include "AppLines.h"
#include "b3Undo.h"

class b3OpObjectCreate : public b3Operation
{
public:
	b3OpObjectCreate();

protected:
	void b3Do();

	inline int  b3GetId()
	{
		return IDS_OP_OBJECT_CREATE;
	}
};

class b3OpObjectDelete : public b3Operation
{
public:
	b3OpObjectDelete();

protected:
	void b3Do();

	inline int  b3GetId()
	{
		return IDS_OP_OBJECT_DELETE;
	}
};

class b3OpObjectLoad : public b3Operation
{
public:
	b3OpObjectLoad();

protected:
	void b3Do();

	inline int  b3GetId()
	{
		return IDS_OP_OBJECT_LOAD;
	}
};

class b3OpObjectReplace : public b3Operation
{
public:
	b3OpObjectReplace();

protected:
	void b3Do();

	inline int  b3GetId()
	{
		return IDS_OP_OBJECT_REPLACE;
	}
};

class b3OpObjectCopy : public b3Operation
{
public:
	b3OpObjectCopy();

protected:
	void b3Do();

	inline int  b3GetId()
	{
		return IDS_OP_OBJECT_COPY;
	}
};

#endif
