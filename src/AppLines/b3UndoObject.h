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

class b3OpObject : public b3Operation
{
protected:
	b3Scene       *m_Scene;
	CDlgHierarchy *m_DlgHierarchy;

protected:
	inline b3OpObject(b3Scene *scene,CDlgHierarchy *hierarchy)
	{
		m_Scene        = scene;
		m_DlgHierarchy = hierarchy;
	}
};

class b3OpObjectCreate : public b3OpObject
{
	b3BBox         *m_Selected;
	b3BBox         *m_BBox;
	b3Item         *m_InsertAfter;
	b3Base<b3Item> *m_Base;

public:
	         b3OpObjectCreate(b3Scene *scene,CDlgHierarchy *hierarchy,b3_bool insert_sub);

protected:
	void b3Undo();
	void b3Redo();
	void b3Delete();

	inline int  b3GetId()
	{
		return IDS_OP_OBJECT_CREATE;
	}
};

class b3OpObjectDelete : public b3OpObject
{
	b3Base<b3Item> *m_Base;
	b3Item         *m_Prev;
	b3Item         *m_Select;
	b3BBox         *m_Selected;

public:
	         b3OpObjectDelete(b3Scene *scene,CDlgHierarchy *hierarchy);

protected:
	void b3Undo();
	void b3Redo();
	void b3Delete();

	inline int  b3GetId()
	{
		return IDS_OP_OBJECT_DELETE;
	}
};

#define B3_OBJECT_LOAD_REGITEM "Loaded object filename"

class b3OpObjectLoad : public b3OpObject
{
protected:
	b3BBox         *bbox;
	b3Base<b3Item> *base;
	b3World         world;
	b3_count        level;
	b3BBox         *selected;

public:
	         b3OpObjectLoad(b3Scene *scene,CDlgHierarchy *hierarchy,const char *regitem = B3_OBJECT_LOAD_REGITEM);

protected:
	virtual void b3Do();
	virtual void b3Undo();
	virtual void b3Redo();
	virtual void b3Delete();

	inline int  b3GetId()
	{
		return IDS_OP_OBJECT_LOAD;
	}
};

#define B3_OBJECT_REPLACE_REGITEM "Replaced object filename"

class b3OpObjectReplace : public b3OpObjectLoad
{
public:
	         b3OpObjectReplace(b3Scene *scene,CDlgHierarchy *hierarchy,const char *regitem = B3_OBJECT_REPLACE_REGITEM);

protected:
	void b3Do();
	void b3Undo();
	void b3Redo();
	void b3Delete();

	inline int  b3GetId()
	{
		return IDS_OP_OBJECT_REPLACE;
	}
};

class b3OpObjectCopy : public b3OpObject
{
	b3BBox            *m_Selected;
	b3Base<b3Item>    *m_Base;
	b3Array<b3BBox *>  m_ClonedBBoxes;

public:
	         b3OpObjectCopy(b3Scene *scene,CDlgHierarchy *hierarchy,b3_vector *fulcrum);

protected:
	void b3Undo();
	void b3Redo();
	void b3Delete();

	inline int  b3GetId()
	{
		return IDS_OP_OBJECT_COPY;
	}
};

#endif
