/*
**
**	$Filename:      b3ItemRegister.h $
**	$Release:       Dortmund 2001, 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - a registry for class maintainance
**
**	(C) Copyright 2001, 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

#ifndef B3_ITEMREGISTER_H
#define B3_ITEMREGISTER_H

#include "blz3/b3PluginDef.h"

class B3_PLUGIN b3ItemRegisterEntry : public b3Link<b3ItemRegisterEntry>
{
	b3_item_init_func init_func;
	b3_item_load_func load_func;
	b3_u32            class_type;
	b3_bool           is_class;

public:
	b3ItemRegisterEntry(
		b3_item_init_func new_init_func,
		b3_item_load_func new_load_func,
		b3_u32            new_class_type,
		b3_bool           new_is_class = false);

	b3_bool  b3IsClassType(b3_u32 class_type_to_check);
	b3Item  *b3Init();
	b3Item  *b3Load(b3_u32 *buffer);
	void     b3Dump();
};

class B3_PLUGIN b3ItemRegister : b3Base<b3ItemRegisterEntry>
{
	static b3ItemRegister       m_Register;

	                            b3ItemRegister();

public:
	                           ~b3ItemRegister();
	static b3ItemRegisterEntry *b3Find(b3_u32 class_type);
	static void                 b3Dump();

	static inline void          b3Append(b3ItemRegisterEntry *new_entry)
	{
		m_Register.b3Base<b3ItemRegisterEntry>::b3Append(new_entry);
	}

	static inline b3_bool       b3IsEmpty()
	{
		return m_Register.b3Base<b3ItemRegisterEntry>::b3IsEmpty();
	}
};

#endif
