/*
**
**	$Filename:	b3Mem.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
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
#include "blz3/b3Types.h"

class b3MemAccess
{
protected:
	static inline void *b3Alloc(b3_size size)
	{
		return calloc(size,1);
	}

	static inline void b3Free(void *ptr)
	{
#ifdef REALLY_FREE
		free(ptr);
#endif
	}
	
	static inline void *b3Realloc(
		void    *ptr,
		b3_size  old_size,
		b3_size  new_size)
	{
#if 0
		return realloc(ptr,new_size);
#else
		void *new_ptr;

		if (new_size == 0)
		{
			free(ptr);
			new_ptr = null;
		}
		else
		{
			new_ptr = calloc(new_size,1);
			if (new_ptr != null)
			{
				memcpy(new_ptr,ptr,B3_MIN(old_size,new_size));
				free(ptr);
			}
		}
		return new_ptr;
#endif
	}
};

#include "blz3/system/b3Memory.h"

#endif
