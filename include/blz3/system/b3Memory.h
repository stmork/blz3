/*
**
**	$Filename:	b3Mem.h $
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

#ifndef B3_SYSTEM_MEMORY_H
#define B3_SYSTEM_MEMORY_H

#include "blz3/b3Types.h"
#include "blz3/system/b3Thread.h"
#include "blz3/base/b3Exception.h"

struct b3MemNode
{
	b3MemNode *ChunkNext;
	b3MemNode *ChunkLast;
	void      *Chunk;
	b3_size    ChunkSize;
};

typedef enum
{
	B3_MEM_ERROR = -1,
	B3_MEM_OK    =  0,
	B3_MEM_MEMORY,
	B3_MEM_UNKNOWN_PTR
} b3_mem_error;

typedef b3Exception<b3_mem_error,'MEM'> b3MemException;

class b3Mem : protected b3MemNode, protected b3MemAccess
{
private:
	b3Mutex    mutex;

public:
	inline b3Mem()
	{
		ChunkLast = null;
		ChunkNext = null;
		ChunkSize = 0;
		Chunk     = null;
	}

	virtual ~b3Mem()
	{
		b3Free();
	}
	
	void *b3Alloc(const b3_size size)
	{
		struct b3MemNode *act;

		// allocate memory block
		act = (struct b3MemNode *)b3MemAccess::b3Alloc(size + sizeof(struct b3MemNode));
		if (act == null)
		{
#ifndef _DEBUG
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

		// Done...
		return act->Chunk;
	}

	void *b3Realloc(const void *old_ptr,const b3_size new_size)
	{
		struct b3MemNode *act,*next,*new_act;

		if (old_ptr == null)
		{
			// Allocate on the standard way...
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
				new_act = (struct b3MemNode *)b3MemAccess::b3Realloc(act,new_size);
				if (new_act == null)
				{
					mutex.b3Unlock();
#ifndef _DEBUG
					throw new b3MemException(B3_MEM_MEMORY);
#endif
				}
				else
				{
					// Relink new chunk
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

		// Pointer not found. This shouldn't be but it's failsafe!
		return b3Alloc(new_size);
	}

	b3_bool b3Free(const void *ptr)
	{
		struct b3MemNode *act,*next;

		mutex.b3Lock();
		for (act  = ChunkNext;
		     act != null;
		     act  = next)
		{
			next = act->ChunkNext;
			if (act->Chunk == ptr)
			{
				// Found! Unlink and free!
				if (next)
				{
					next->ChunkLast = act->ChunkLast;
				}
				act->ChunkLast->ChunkNext  = next;
				b3MemAccess::b3Free(act);
				mutex.b3Unlock();
				return true;
			}
		}
		mutex.b3Unlock();

		// Pointer not found
		return false;
	}

	b3_bool b3Free()
	{
		struct b3MemNode *next,*act;

		mutex.b3Lock();
		for (act  = ChunkNext;
		     act != null;
		     act  = next)
		{
			next = act->ChunkNext;
			b3MemAccess::b3Free (act);
		}
		
		// Clear head entries
		ChunkNext = null;
		ChunkLast = null;
		ChunkSize = 0;
		Chunk     = null;
		mutex.b3Unlock();

		// Done...
		return true;
	}

	void b3Dump()
	{
		struct b3MemNode *act,*next;

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
};

#endif
