/*
**
**	$Filename:	b3ItemRegister.cc $
**	$Release:	Dortmund 2001, 2016 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Blizzard file structure loading
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

#define not_VERBOSE

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3BaseInclude.h"
#include "blz3/base/b3World.h"
#include "blz3/system/b3File.h"
#include "b3ItemRegister.h"

/*************************************************************************
**                                                                      **
**                        b3FirstItem: which contains Blizzard data     **
**                                                                      **
*************************************************************************/

b3FirstItem::b3FirstItem(const b3_u32  class_type) : b3Item(sizeof(b3FirstItem), class_type)
{
	b3AllocHeads(1);
	m_Heads[0].b3InitBase();
}

b3FirstItem::b3FirstItem(const b3_u32 * src) : b3Item(src)
{
}

void b3FirstItem::b3Write()
{
}

b3Base<b3Item> * b3FirstItem::b3GetHead() const
{
	return &m_Heads[0];
}

/*************************************************************************
**                                                                      **
**                        b3Item: one node inside file (base class)     **
**                                                                      **
*************************************************************************/

b3ItemRegister::b3ItemRegister()
{
	b3ItemRegisterEntry * entry = new b3ItemRegisterEntry(
		&b3FirstItem::b3StaticInit,
		&b3FirstItem::b3StaticInit, B3_CLASS_MAX, true);
	b3Append(entry);
}

b3ItemRegister::~b3ItemRegister()
{
	b3Free();
}

b3ItemRegisterEntry * b3ItemRegister::b3FindCached(b3_u32 class_type)
{
	b3ItemRegisterEntry * entry;

	B3_FOR_BASE(this, entry)
	{
		if (entry->b3IsClassType(class_type))
		{
#ifdef VERBOSE
			b3PrintF(B3LOG_FULL, "%08lx found,\n", class_type);
#endif
			// Some kind of LRU
			b3Remove(entry);
			b3First(entry);
			return entry;
		}
	}
	b3PrintF(B3LOG_DEBUG, "b3ItemRegister::b3Find(%08x) not found.\n", class_type);
	return nullptr;
}

void b3ItemRegister::b3Dump() const
{
	b3ItemRegisterEntry * entry;

	b3PrintF(B3LOG_FULL, "Item register contains following classes:\n");
	B3_FOR_BASE(this, entry)
	{
		entry->b3Dump();
	}
}
