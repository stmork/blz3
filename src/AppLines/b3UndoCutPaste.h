/*
**
**	$Filename:	b3UndoCutPaste.h $
**	$Release:	Dortmund 2003 $
**	$Revision$
**	$Date$
**	$Author$
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

class b3OpDrop : public b3Operation
{
	b3Scene    *    m_Scene;
	b3Base<b3Item> * m_SrcBase;
	b3Base<b3Item> * m_DstBase;
	b3BBox     *    m_SrcBBox;
	b3Item     *    m_Prev;

public:
	b3OpDrop(b3Scene * scene, b3BBox * src, b3BBox * dst);

protected:
	void b3Undo();
	void b3Redo();

	inline int  b3GetId()
	{
		return IDS_OP_DROP;
	}
};

class b3OpCut : public b3Operation
{
	CDlgHierarchy * m_DlgHierarchy;
	b3Scene    *    m_Scene;
	b3BBox     *    m_BBox;
	b3Item     *    m_Select;
	b3Item     *    m_Prev;
	b3Base<b3Item> * m_Base;

public:
	b3OpCut(CDlgHierarchy * hierarchy, b3Scene * scene);

protected:
	void b3Undo();
	void b3Redo();
	void b3Delete();

	inline int  b3GetId()
	{
		return IDS_OP_CUT;
	}
};

class b3OpPaste : public b3Operation
{
	CDlgHierarchy * m_DlgHierarchy;
	b3Scene    *    m_Scene;
	b3BBox     *    m_BBox;
	b3Item     *    m_InsertAfter;
	b3Base<b3Item> * m_Base;
	b3World         m_World;
	b3_count        m_Level;

public:
	b3OpPaste(CDlgHierarchy * hierarchy, b3Scene * scene, b3_bool insert_sub);

protected:
	void b3Do();
	void b3Undo();
	void b3Redo();
	void b3Delete();

	inline int  b3GetId()
	{
		return IDS_OP_PASTE;
	}
};

#endif
