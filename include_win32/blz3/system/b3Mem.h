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
	
	static inline void *b3Realloc(void *ptr,b3_size size)
	{
		return ::GlobalReAlloc(ptr,size,GPTR);
	}
};

#include "blz3/system/b3Memory.h"

#endif
