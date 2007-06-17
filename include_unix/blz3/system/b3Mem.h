/*
**
**	$Filename:	b3Mem.h $
**	$Release:	Dortmund 2001 $
**	$Revision: 1475 $
**	$Date: 2007-01-01 14:04:37 +0100 (Mo, 01 Jan 2007) $
**	$Author: sm $
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - memory routines (proto types)
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_MEM_H
#define B3_SYSTEM_MEM_H

#define REALLY_FREE

#include <stdlib.h>
#include <stdio.h>
#ifdef HAVE_MEMALIGN
#include <string.h>
#include <malloc.h>
#endif

#include "blz3/b3Types.h"

/**
 * This class provides system dependend methods for providing
 * memory access.
 */
class B3_PLUGIN b3MemAccess
{
public:
	/**
	 * This method allocates a new memory block.
	 *
	 * @param size The size of the new memory block.
	 * @return The new memory block or null if there is no memory available.
	 */
	static inline void *b3Alloc(b3_size size)
	{
#ifdef HAVE_MEMALIGN
		void *ptr = memalign(16,size);
		if (ptr != null)
		{
			memset (ptr, 0, size);
		}
		return ptr;
#else
		return calloc(size,1);
#endif
	}

	/**
	 * This method frees the specified memory block.
	 *
	 * @param ptr The pointer to the memory block.
	 */
	static inline void b3Free(const void *ptr)
	{
#ifdef REALLY_FREE
		free((void *)ptr);
#endif
	}
};

#include "blz3/system/b3Memory.h"

#endif
