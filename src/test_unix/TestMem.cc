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
  
#include "blz3/b3Config.h"
#include "blz3/system/b3Mem.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.5  2002/08/11 06:38:54  sm
**	- Started some library reorganizations: Moved folowing classes into
**	  system lib. Introduced new system library which is platform
**	  independend but part of platform dependend library.
**	  o b3FileAbstract
**	  o b3DirAbstract
**	  o b3Exception
**	- The following classes should be reorganized with abstract
**	  base classes to:
**	  o b3Date
**	  o b3Time
**	  o b3Log
**
**	Revision 1.4  2002/08/09 13:20:20  sm
**	- b3Mem::b3Realloc was a mess! Now fixed to have the same
**	  behaviour on all platforms. The Windows method ::GlobalReAlloc
**	  seems to be broken:-(
**	- Introduced b3DirAbstract and b3PathAbstract classes
**	
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
									   
#define MEM_MIN       1000
#define MEM_LOW_MULT     2
#define MEM_HIGH_MULT  200

void b3TestMem()
{
	void     *ptr1,*ptr2;
	b3Mem     mem;
	b3_count  count = 0;
	char      buffer[MEM_MIN];
	int       i;

	// Put some stuff into realloc buffer
	for (i = 0;i < MEM_MIN;i++)
	{
		buffer[i] = B3_IRAN(255);
	}
	ptr1 = mem.b3Alloc (MEM_MIN);
	ptr2 = mem.b3Alloc (MEM_MIN);
	b3PrintF (B3LOG_NORMAL,"ptr1: %p\n",ptr1);
	b3PrintF (B3LOG_NORMAL,"ptr2: %p\n",ptr2);
	mem.b3Free(ptr1);
	b3PrintF (B3LOG_NORMAL,"ptr1 freed...\n");
	mem.b3Free(NULL);
	b3PrintF (B3LOG_NORMAL,"NULL-Pointer freed...\n");
	mem.b3Free();
	b3PrintF (B3LOG_NORMAL,"whole node freed...\n");

	ptr1 = mem.b3Realloc(null,  MEM_MIN * 2);
	memcpy(ptr1,buffer,MEM_MIN);
	b3PrintF (B3LOG_NORMAL,"ptr1 = %p after b3Realloc() (%s)\n",
		ptr1,
		ptr1 != null ? "OK" : "wrong");

	ptr2 = mem.b3Realloc(ptr1,  MEM_MIN);
	b3PrintF (B3LOG_NORMAL,"ptr2 = %p, ptr1 = %p after b3Realloc() with size reduction (%s)\n",
		ptr2,ptr1,
		(ptr1 == ptr2) && (ptr2 != null) ? "OK" : "wrong");

	ptr1 = mem.b3Realloc(ptr2,MEM_MIN * MEM_HIGH_MULT);
	b3PrintF (B3LOG_NORMAL,"ptr1 = %p, ptr2 = %p after b3Realloc() with size enlargement (%s)\n",
		ptr1,ptr2,
		(ptr1 != ptr2) && (ptr1 != null) ? "OK" : "wrong");
	b3PrintF (B3LOG_NORMAL,"   Memory buffer is %s\n",
		memcmp(buffer,ptr1,MEM_MIN) == 0 ? "preserved" : "corrupted");
	for (i = MEM_MIN;i < (MEM_MIN * MEM_HIGH_MULT);i++)
	{
		count += ((char *)ptr1)[i];
	}
	b3PrintF (B3LOG_NORMAL,"   Rest memory buffer is %s\n",
		count == 0 ? "zero initialized" : "garbled");

	ptr2 = mem.b3Realloc(ptr1,     0);
	b3PrintF (B3LOG_NORMAL,"ptr2 = %p, ptr1 = %p after b3Realloc() with zero size allocation (%s)\n",
		ptr2,ptr1,
		(ptr1 != ptr2) && (ptr2 == null) ? "OK" : "wrong");
}
