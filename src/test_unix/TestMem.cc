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
	b3PrintF (B3LOG_NORMAL,"ptr1: %8lx\n",ptr1);
	b3PrintF (B3LOG_NORMAL,"ptr2: %8lx\n",ptr2);
	mem.b3Free(ptr1);
	b3PrintF (B3LOG_NORMAL,"ptr1 freed...\n");
	mem.b3Free(NULL);
	b3PrintF (B3LOG_NORMAL,"NULL-Pointer freed...\n");
	mem.b3Free();
	b3PrintF (B3LOG_NORMAL,"whole node freed...\n");
}
