/*
**
**      $Filename:      b3ItemRegisterEntry.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Author$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Blizzard file structure loading
**
**      (C) Copyright 2001  Steffen A. Mork
**          All Rights Reserved
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

#include "blz3/b3Config.h"
#include "blz3/base/b3World.h"
#include "b3ItemRegister.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.5  2004/09/23 09:31:33  sm
**      - Changed b3Runtime into a real singleton.
**      - Added functions for OpenGL extension.
**      - Removed b3PrintF()s from singletons.
**
**      Revision 1.4  2003/07/20 09:21:18  sm
**      - Added item register dump.
**
**      Revision 1.3  2002/08/05 16:04:55  sm
**      - Found first texture init bug. This wasn't an OpenGL bug. This
**        couldn't be because every implementation had got the same
**        bug. The static aux image for creating textures wasn't initialized
**        at the right time.
**      - Version handling introduced: The version number is extracted
**        from the version resource now.
**      - The b3Tx::b3AllocTx() method uses b3Realloc() for better
**        memory usage.
**      - Some b3World messages removed.
**      - The 0x7fff class is registered into the b3ItemRegister now. This
**        prevents printing a warning when this class isn't found. Due to
**        the fact that *every* Blizzard data contains this class every
**        data read put out this warning.
**
**      Revision 1.2  2001/08/05 19:51:56  sm
**      - Now having OpenGL software for Windows NT and created
**        new Lines III.
**
**      Revision 1.1  2001/08/05 12:46:06  sm
**      - Splitting b3ItemXXX routines
**
**
*/

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

b3_bool b3ItemRegisterEntry::b3IsClassType(b3_u32 class_type_to_check)
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

b3Item *b3ItemRegisterEntry::b3Init()
{
	return init_func(class_type);
}

b3Item *b3ItemRegisterEntry::b3Load(b3_u32 *buffer)
{
	return load_func(buffer);
}

void b3ItemRegisterEntry::b3Dump()
{
	b3PrintF(B3LOG_FULL,"  class %04lx:%04lx%s.\n",
		class_type >> 16,class_type & 0xffff,is_class ? " (class)" : "");
}
