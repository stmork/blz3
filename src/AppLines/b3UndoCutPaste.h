/*
**
**	$Filename:	b3UndoCutPaste.h $
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

#ifndef B3UNDOCUTPASTE_H
#define B3UNDOCUTPASTE_H

#include "AppLines.h"
#include "b3Undo.h"

class b3OpCut : public b3Operation
{
public:
	     b3OpCut();

protected:
	void b3Undo();
	void b3Redo();

	inline int  b3GetId()
	{
		return IDS_OP_CUT;
	}
};

class b3OpPaste : public b3Operation
{
	CDlgHierarchy *m_DlgHierarchy;
	b3Scene *m_Scene;
				b3World   world;
	b3BBox         *selected;
	b3BBox         *bbox;
	b3Item         *insert_after;
	b3Base<b3Item> *base;

public:
	     b3OpPaste(CDlgHierarchy *hierarchy,b3Scene *scene,b3_bool insert_sub);

protected:
	void b3Undo();
	void b3Redo();

	inline int  b3GetId()
	{
		return IDS_OP_PASTE;
	}
};

#endif
