/*
**
**	$Filename:	b3Mem.c $
**	$Release:	Dortmund 1998 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - memory routines
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#define REALLY_FREE

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/system/b3Mem.h"
#include "blz3/system/b3Log.h"
#include <stdlib.h>

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.7  2001/12/31 11:05:18  sm
**	- Added TestData for testing Blizzard data structures for reading
**	  and writing.
**	- Fixed bugs found with previous mentioned tool:
**	  o Some b3AnimElement errors found to be fixed under Windows.
**	  o b3TriangleShape destructor handled unchecked m_GridList pointer
**	- Changed some output levels in b3Light, b3Scene and b3ShadeXXX from
**	  B3LOG_NORMAL to B3LOG_DEBUG.
**
**	Revision 1.6  2001/12/30 14:16:58  sm
**	- Abstracted b3File to b3FileAbstract to implement b3FileMem (not done yet).
**	- b3Item writing implemented and updated all raytracing classes
**	  to work properly.
**	- Cleaned up spline shapes and CSG shapes.
**	- Added b3Caustic class for compatibility reasons.
**	
**	Revision 1.5  2001/08/08 20:12:59  sm
**	- Fixing some makefiles
**	- introducing check/BlzDump (BlzDump moved from tools)
**	- Some further line drawing added
**	- b3RenderContext and b3RenderObject introduced. Every b3Shape inherit from
**	  b3RenderObject.
**	
**	Revision 1.4  2001/07/08 12:30:06  sm
**	- New tool to remove nasty CR/LF from Windoze.
**	- Removing some nasty CR/LF with that new tool.
**	
**	Revision 1.3  2001/07/02 19:52:03  sm
**	- Cleaning up comments
**	
**	Revision 1.2  2001/07/01 16:48:00  sm
**	- FILESTRINGLEN -> B3_FILESTRINGLEN
**	- Cleaned up some makefiles
**	- Cleaned up some CVS conflicts
**	
**	Revision 1.1.1.1  2001/07/01 12:24:59  sm
**	Blizzard III is born
**	
*/

/*************************************************************************
**                                                                      **
**                        b3Mem routines                                **
**                                                                      **
*************************************************************************/

/*
We are using own file descriptors named "KeyMem". If we use this, we can
safely free memory without doing it actively by inheriting this "b3Mem"-class
to those classes which need memory. We can use this class by using it in
malloc()/free() manner - or by deleting this class itself. Further we can
free the whole "Key". The routines are thread safe.
*/

b3Mem::b3Mem()
{
	ChunkLast = null;
	ChunkNext = null;
	ChunkSize = 0;
	Chunk     = null;
}

// Free all memory in list...
b3Mem::~b3Mem()
{
	b3Free();
}

// This routine is like "malloc()"
void *b3Mem::b3Alloc(b3_size size)
{
	register struct b3MemNode *act;

	// allocate memory block
	act = (struct b3MemNode *)calloc(size + sizeof(struct b3MemNode),1);
	if (act == null)
	{
#ifdef no_DEBUG
		throw new b3MemException(B3_MEM_MEMORY);
#else
		return null;
#endif
	}

	// Set values
	act->ChunkSize = size;
	act->Chunk     = (void *)&act[1];

	// link into MemNode chain
	mutex.b3Lock();
	if (ChunkNext)
	{
		ChunkNext->ChunkLast = act;
	}
	act->ChunkNext = ChunkNext;
	act->ChunkLast = this;
	ChunkNext      = act;
	mutex.b3Unlock();
	return act->Chunk;
}

// This frees all memory known to this "Key".
b3_bool b3Mem::b3Free()
{
	register struct b3MemNode *next,*act;

	mutex.b3Lock();
	for (act  = ChunkNext;
	     act != null;
	     act  = next)
	{
		next = act->ChunkNext;
#ifdef REALLY_FREE
		free (act);
#endif
	}
	ChunkNext = null;
	ChunkLast = null;
	ChunkSize = 0;
	Chunk     = null;
	mutex.b3Unlock();
	return true;
}

// This routine is like "free()"
// Return true if ptr was found!
b3_bool b3Mem::b3Free(const void *ptr)
{
	register struct b3MemNode *act,*next;

	mutex.b3Lock();
	for (act  = ChunkNext;
	     act != null;
	     act  = next)
	{
		next = act->ChunkNext;
		if (act->Chunk == ptr)
		{
			if (next)
			{
				next->ChunkLast = act->ChunkLast;
			}
			act->ChunkLast->ChunkNext  = next;
#ifdef REALLY_FREE
			free (act);
#endif
			mutex.b3Unlock();
			return true;
		}
	}
	mutex.b3Unlock();
#ifdef _DEBUG
	if (ptr != null)
	{
		throw new b3MemException(B3_MEM_UNKNOWN_PTR);
	}
#endif
	return false;
}

void *b3Mem::b3Realloc(const void *old_ptr,const b3_size new_size)
{
	struct b3MemNode *act,*next,*new_act;

	if (old_ptr == null)
	{
		return b3Alloc(new_size);
	}

	mutex.b3Lock();
	for (act  = ChunkNext;
	     act != null;
	     act  = next)
	{
		next = act->ChunkNext;
		if ((act->Chunk == old_ptr) && (act->ChunkSize < new_size))
		{
			new_act = (struct b3MemNode *)realloc(act,new_size);
			if (new_act == null)
			{
				mutex.b3Unlock();
#ifdef no_DEBUG
				throw new b3MemException(B3_MEM_MEMORY);
#endif
			}
			else
			{
				// Set values
				new_act->ChunkSize = new_size;
				new_act->Chunk     = (void *)&new_act[1];
				if (next != null)
				{
					next->ChunkLast = new_act;
				}
				new_act->ChunkLast->ChunkNext  = new_act;
			}

			mutex.b3Unlock();
			return new_act;
		}
	}
	mutex.b3Unlock();
#ifdef _DEBUG
	if (old_ptr != null)
	{
		throw new b3MemException(B3_MEM_UNKNOWN_PTR);
	}
#endif
	return null;
}

// If we need debugging output...
void b3Mem::b3Dump()
{
	register struct b3MemNode *act,*next;

	for (act  = ChunkNext;
	     act != null;
	     act  = next)
	{
		next = act->ChunkNext;
		b3PrintF (B3LOG_FULL,"### CLASS: b3Mem  # %08lx: %9ld Bytes.\n",
			act->Chunk,
			act->ChunkSize);
	}
}
