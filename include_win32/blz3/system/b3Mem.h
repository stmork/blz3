/*
**
**	$Filename:	b3Mem.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - System dependent memory routines
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_MEM_H
#define B3_SYSTEM_MEM_H

#define REALLY_FREE

#include "stdafx.h"
#include "blz3/b3Types.h"

class b3MemAccess
{
protected:
	static inline void *b3Alloc(b3_size size)
	{
		return ::GlobalAlloc(GPTR,size);
	}

	static inline void b3Free(void *ptr)
	{
#ifdef REALLY_FREE
		::GlobalFree(ptr);
#endif
	}
	
	static inline void *b3Realloc(void *ptr,b3_size new_size)
	{
#if 0
		return ::GlobalReAlloc(ptr,new_size,GPTR);
#else
		void *new_ptr;

		if (new_size == 0)
		{
			::GlobalFree(ptr);
			new_ptr = null;
		}
		else
		{
			new_ptr = ::GlobalAlloc(GPTR,new_size);
			if (new_ptr != null)
			{
				b3_size old_size = ::GlobalSize(ptr);

				memcpy(new_ptr,ptr,B3_MIN(old_size,new_size));
				::GlobalFree(ptr);
			}
		}
		return new_ptr;
#endif
	}
};

#include "blz3/system/b3Memory.h"

#endif
