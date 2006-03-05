/*
**
**	$Filename:	b3CopyProperty.cpp $
**	$Release:	Dortmund 2005 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Select shape property copy modes
**
**	(C) Copyright 2005  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Lines III includes                            **
**                                                                      **
*************************************************************************/

#include "AppLinesInclude.h"

#include "b3CopyProperty.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2006/03/05 22:12:32  sm
**	- Added precompiled support for faster comiling :-)
**
**	Revision 1.1  2005/05/15 06:53:23  sm
**	- Tested shape surface property copy operation.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        DlgCopyProperties implementation              **
**                                                                      **
*************************************************************************/

void b3CopyPropertyInfo::b3Add(
	b3Shape        *srcShape,
	b3Shape        *dstShape,
	b3Base<b3Item> *srcBase,
	b3Base<b3Item> *dstBase)
{
	b3CopyPropertyItem item;

	srcShape->b3Store();
	item.m_Original.b3InitBase(srcBase->b3GetClass());
	item.m_Cloned.b3InitBase(srcBase->b3GetClass());
	item.m_DstBase = dstBase;
	item.m_Shape   = dstShape;
	b3World::b3CloneBase(srcBase,&item.m_Cloned);
	m_Shapes.b3Add(item);
}

void b3CopyPropertyInfo::b3Undo()
{
	b3_count i,max = m_Shapes.b3GetCount();

	for (i = 0;i < max;i++)
	{
		b3CopyPropertyItem &item = m_Shapes[i];

		item.m_Cloned.b3MoveFrom(item.m_DstBase);
		item.m_DstBase->b3MoveFrom(&item.m_Original);
		b3RecomputeShape(item.m_Shape, item.m_DstBase->b3GetClass());
	}
}

void b3CopyPropertyInfo::b3Redo()
{
	b3_count i,max = m_Shapes.b3GetCount();

	for (i = 0;i < max;i++)
	{
		b3CopyPropertyItem &item = m_Shapes[i];

		item.m_Original.b3MoveFrom(item.m_DstBase);
		item.m_DstBase->b3MoveFrom(&item.m_Cloned);
		b3RecomputeShape(item.m_Shape, item.m_DstBase->b3GetClass());
	}
}

void b3CopyPropertyInfo::b3Delete(b3_bool done)
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

void b3CopyPropertyInfo::b3RecomputeShape(b3Shape *shape,b3_u32 surface_class)
{
	switch(surface_class)
	{
	case CLASS_MATERIAL:
		shape->b3RecomputeMaterial();
		break;

	case CLASS_CONDITION:
		shape->b3RecomputeIndices();
		break;
	}
}
