/*
**
**	$Filename:	b3CopyProperty.h $
**	$Release:	Dortmund 2005 $
**	$Revision: 1092 $
**	$Date: 2005-05-15 08:53:23 +0200 (So, 15 Mai 2005) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Select shape property copy modes
**
**	(C) Copyright 2005  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3COPYPROPERTY_H
#define B3COPYPROPERTY_H

#include "blz3/base/b3Array.h"
#include "blz3/base/b3World.h"

class b3Shape;

struct b3CopyPropertyItem
{
	b3Shape        *m_Shape;
	b3Base<b3Item> *m_DstBase;
	b3Base<b3Item>  m_Original;
	b3Base<b3Item>  m_Cloned;
};

class b3CopyPropertyInfo
{
	b3Array<b3CopyPropertyItem> m_Shapes;

public:
	void b3Add(
		b3Shape        *srcShape,
		b3Shape        *dstShape,
		b3Base<b3Item> *srcBase,
		b3Base<b3Item> *dstBase);

	void b3Undo();
	void b3Redo();
	void b3Delete(b3_bool done);

private:
	static void b3RecomputeShape(b3Shape *shape,b3_u32 surface_class);
};

#endif
