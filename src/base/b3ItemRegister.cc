/*
**
**      $Filename:      b3ItemRegister.cc $
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
#include "blz3/system/b3File.h"
#include "b3ItemRegister.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.4  2001/08/14 07:03:28  sm
**      - Made some ASSERT cleanups. New define when _DEBUG is switched on:
**        B3_ASSERT(condition) abort()s when condition is false.
**
**      Revision 1.3  2001/08/05 19:53:43  sm
**      - Removing some nasty CR/LF
**
**      Revision 1.2  2001/08/05 19:51:56  sm
**      - Now having OpenGL software for Windows NT and created
**        new Lines III.
**
**      Revision 1.1  2001/08/05 12:46:06  sm
**      - Splitting b3ItemXXX routines
**
**      Revision 1.6  2001/08/02 15:37:17  sm
**      - Now we are able to draw Blizzard Scenes with OpenGL.
**
**      Revision 1.5  2001/07/08 12:30:06  sm
**      - New tool to remove nasty CR/LF from Windoze.
**      - Removing some nasty CR/LF with that new tool.
**
**      Revision 1.4  2001/07/02 19:28:25  sm
**      - Applying console application on Windows 32
**      - Added further Windows environment
**
**      Revision 1.3  2001/07/02 16:09:46  sm
**      - Added bounding box reorganization.
**
**      Revision 1.2  2001/07/01 16:31:52  sm
**      - Creating MSVC Projects
**      - Welcome to Windows 32
**
**      Revision 1.1.1.1  2001/07/01 12:24:58  sm
**      Blizzard III is born
**
*/

/*************************************************************************
**                                                                      **
**                        b3Item: one node inside file (base class)     **
**                                                                      **
*************************************************************************/

b3ItemRegister::~b3ItemRegister()
{
	b3ItemRegisterEntry *item;

	while(item = classes.First)
	{
		classes.b3Remove(item);
		delete item;
	}
}

void b3ItemRegister::b3Append(b3ItemRegisterEntry *new_entry)
{
	classes.b3Append(new_entry);
}

b3_bool b3ItemRegister::b3IsEmpty()
{
	return classes.First == null;
}

b3ItemRegisterEntry *b3ItemRegister::b3Find(b3_u32 class_type)
{
	b3ItemRegisterEntry *entry;

	B3_FOR_BASE(&classes,entry)
	{
		if (entry->b3IsClassType(class_type))
		{
			b3PrintF (B3LOG_FULL,"%08lx found,\n",class_type);
			return entry;
		}
	}
	b3PrintF (B3LOG_NORMAL,"%08lx not found.\n",class_type);
	return null;
}

b3ItemRegister b3_item_register;
