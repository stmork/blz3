/*
**
**	$Filename:	DlgCopyProperties.cpp $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Select shape property copy modes
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3COPYPROPERTY_H
#define B3COPYPROPERTY_H

#include "blz3/base/b3Array.h"
#include "blz3/base/b3World.h"

struct b3CopyPropertyItem
{
	b3Base<b3Item> *m_DstBase;
	b3Base<b3Item>  m_Original;
	b3Base<b3Item>  m_Cloned;
};

class b3CopyPropertyInfo
{
	b3Array<b3CopyPropertyItem> m_Shapes;

public:
	inline void b3Add(b3Item *shape,b3Base<b3Item> *srcBase,b3Base<b3Item> *dstBase)
	{
		b3CopyPropertyItem item;

		shape->b3Store();
		item.m_Original.b3InitBase(srcBase->b3GetClass());
		item.m_Cloned.b3InitBase(srcBase->b3GetClass());
		item.m_DstBase = dstBase;
		b3World::b3CloneBase(srcBase,&item.m_Cloned);
		m_Shapes.b3Add(item);
	}

	inline void b3Undo()
	{
		b3_count i,max = m_Shapes.b3GetCount();

		for (i = 0;i < max;i++)
		{
			b3CopyPropertyItem &item = m_Shapes[i];

			item.m_Cloned.b3MoveFrom(item.m_DstBase);
			item.m_DstBase->b3MoveFrom(&item.m_Original);
		}
	}

	inline void b3Redo()
	{
		b3_count i,max = m_Shapes.b3GetCount();

		for (i = 0;i < max;i++)
		{
			b3CopyPropertyItem &item = m_Shapes[i];

			item.m_Original.b3MoveFrom(item.m_DstBase);
			item.m_DstBase->b3MoveFrom(&item.m_Cloned);
		}
	}

	inline void b3Delete(b3_bool done)
	{
		b3_count i,max = m_Shapes.b3GetCount();

		if (done)
		{
			for (i = 0;i < max;i++)
			{
				m_Shapes[i].m_Original.b3Free();
			}
		}
		else
		{
			for (i = 0;i < max;i++)
			{
				m_Shapes[i].m_Cloned.b3Free();
			}
		}
	}
};

#endif
