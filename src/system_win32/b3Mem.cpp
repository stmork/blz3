/*
**
**	$Filename:	b3Mem.c $
**	$Release:	Dortmund 2001 $
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

#include "stdafx.h"
#include "blz3/system/b3Mem.h"
#include "blz3/system/b3Log.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.1  2001/07/01 16:31:52  sm
**	- Creating MSVC Projects
**	- Welcome to Windows 32
**
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
	act = (struct b3MemNode *)GlobalAlloc(GPTR,size + sizeof(struct b3MemNode));
	if (act == null)
	{
#ifdef no_DEBUG
		throw new b3MemException(B§_MEM_MEMORY);
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
		GlobalFree (act);
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
			GlobalFree (act);
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

// If we need debugging output...
void b3Mem::b3Dump()
{
	register struct b3MemNode *act,*next;

	for (act  = ChunkNext;
	     act != null;
	     act  = next)
	{
		next = act->ChunkNext;
		b3PrintF (B3LOG_FULL,"### CLASS: b3Mem  # %08lx: %9ld Bytes.\n",act->Chunk,act->ChunkSize);
	}
}
