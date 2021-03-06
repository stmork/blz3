/*
**
**	$Filename:	b3ItemRegisterEntry.cc $
**	$Release:	Dortmund 2001 $
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

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3BaseInclude.h"
#include "blz3/base/b3World.h"
#include "b3ItemRegister.h"

/*************************************************************************
**                                                                      **
**                        b3ItemRegisterEntry: handling one b3Item      **
**                                                                      **
*************************************************************************/

b3ItemRegisterEntry::b3ItemRegisterEntry(
	b3_item_init_func new_init_func,
	b3_item_load_func new_load_func,
	b3_u32            new_class_type,
	b3_bool           new_is_class) :
	b3Link<b3ItemRegisterEntry>(sizeof(b3ItemRegisterEntry))
{
	if (new_is_class)
	{
		new_class_type &= 0xffff0000;
	}

	init_func  = new_init_func;
	load_func  = new_load_func;
	class_type = new_class_type;
	is_class   = new_is_class;
	/*
		b3PrintF (B3LOG_FULL," Registered class %04lx:%04lx%s.\n",
			class_type >> 16,
			class_type & 0xffff,
			is_class ? " (class)" : "");
	*/
}

b3_u32 b3ItemRegisterEntry::b3GetItemClassType() const
{
	return class_type;
}

bool b3ItemRegisterEntry::b3IsClass() const
{
	return is_class;
}

bool b3ItemRegisterEntry::b3IsClassType(b3_u32 class_type_to_check) const
{
	if (is_class)
	{
		return (class_type_to_check & 0xffff0000) == class_type;
	}
	else
	{
		return class_type_to_check == class_type;
	}

	return false;
}

b3Item * b3ItemRegisterEntry::b3Init()
{
	b3Item * item = init_func(class_type);

	item->Succ = nullptr;
	item->Prev = nullptr;

	return item;
}

b3Item * b3ItemRegisterEntry::b3Load(b3_u32 * buffer)
{
	b3Item * item = load_func(buffer);

	item->Succ = nullptr;
	item->Prev = nullptr;

	return item;
}

void b3ItemRegisterEntry::b3Dump() const
{
	b3PrintF(B3LOG_FULL, "  class %04x:%04x%s.\n",
		class_type >> 16, class_type & 0xffff, is_class ? " (class)" : "");
}
