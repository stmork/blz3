/*
**
**	$Filename:	b3UndoObject.h $
**	$Release:	Dortmund 2003 $
**	$Revision: 1448 $
**	$Date: 2006-06-28 19:07:36 +0200 (Mi, 28 Jun 2006) $
**	$Author: sm $
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

class b3OpObjectFile : public b3OpObject
{
protected:
	b3BBox         *m_Selected;
	b3BBox         *m_BBox;
	b3Base<b3Item> *m_Base;
	b3World         m_World;
	b3_count        m_Level;

protected:
	        b3OpObjectFile(b3Scene *scene,CDlgHierarchy *hierarchy,const char *reg_entry);
};

class b3OpObjectLoad : public b3OpObjectFile
{
public:
	         b3OpObjectLoad(b3Scene *scene,CDlgHierarchy *hierarchy);

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

class b3OpObjectReplace : public b3OpObjectFile
{
public:
	         b3OpObjectReplace(b3Scene *scene,CDlgHierarchy *hierarchy);

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

class b3OpObjectLoadCob : public b3OpObject
{
protected:
	b3BBox         *m_Selected;
	b3BBox         *m_BBox;
	b3Base<b3Item> *m_Base;

public:
	         b3OpObjectLoadCob(b3Scene *scene,CDlgHierarchy *hierarchy);

protected:
	virtual void b3Do();
	virtual void b3Undo();
	virtual void b3Redo();
	virtual void b3Delete();

	inline int  b3GetId()
	{
		return IDS_OP_COB_LOAD;
	}
};

class b3OpObjectLoadTgf : public b3OpObject
{
protected:
	b3BBox         *m_Selected;
	b3BBox         *m_BBox;
	b3Base<b3Item> *m_Base;

public:
	         b3OpObjectLoadTgf(b3Scene *scene,CDlgHierarchy *hierarchy);

protected:
	virtual void b3Do();
	virtual void b3Undo();
	virtual void b3Redo();
	virtual void b3Delete();

	inline int  b3GetId()
	{
		return IDS_OP_COB_LOAD;
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
