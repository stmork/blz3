/*
**
**	$Filename:	TestMem.cc $
**	$Release:	Dortmund 2001, 2002 $
**	$Revision$
**	$Date$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - Testing memory routines
**
**      (C) Copyright 2001, 2002  Steffen A. Mork
**          All Rights Reserved
**
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/
  
#include "blz3/system/b3Log.h"
#include "blz3/system/b3Mem.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.3  2002/08/09 08:00:32  sm
**	- Reimplemented b3Mem::b3Realloc and made checks.
**
**	Revision 1.2  2002/08/06 15:54:12  sm
**	- Introduced b3HashMap<key,object> class incl. test prog.
**	- Found some source files without header.
**	
**
*/

/*************************************************************************
**                                                                      **
**                        implementation                                **
**                                                                      **
*************************************************************************/

void b3TestMem()
{
	void  *ptr1,*ptr2;
	b3Mem  mem;

	ptr1 = mem.b3Alloc (1000);
	ptr2 = mem.b3Alloc (1000);
	b3PrintF (B3LOG_NORMAL,"ptr1: %p\n",ptr1);
	b3PrintF (B3LOG_NORMAL,"ptr2: %p\n",ptr2);
	mem.b3Free(ptr1);
	b3PrintF (B3LOG_NORMAL,"ptr1 freed...\n");
	mem.b3Free(NULL);
	b3PrintF (B3LOG_NORMAL,"NULL-Pointer freed...\n");
	mem.b3Free();
	b3PrintF (B3LOG_NORMAL,"whole node freed...\n");

	ptr1 = mem.b3Realloc(null,  2000);
	b3PrintF (B3LOG_NORMAL,"ptr1 = %p after b3Realloc()\n",ptr1);

	ptr2 = mem.b3Realloc(ptr1,  1000);
	b3PrintF (B3LOG_NORMAL,"ptr2 = %p, ptr1 = %p (%s)\n",ptr2,ptr1,ptr1 == ptr2 ? "OK" : "wrong");

	ptr1 = mem.b3Realloc(ptr2,200000);
	b3PrintF (B3LOG_NORMAL,"ptr1 = %p, ptr2 = %p (%s)\n",ptr1,ptr2,ptr1 != ptr2 ? "OK" : "wrong");

	ptr2 = mem.b3Realloc(ptr1,     0);
	b3PrintF (B3LOG_NORMAL,"ptr2 = %p, ptr1 = %p (%s)\n",ptr2,ptr1,ptr1 == ptr2 ? "OK" : "wrong");
}
