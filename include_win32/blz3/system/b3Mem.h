/*
**
**	$Filename:	b3Mem.h $
**	$Release:	Dortmund 2001 $
**	$Revision: 1099 $
**	$Date: 2005-05-17 13:05:18 +0200 (Di, 17 Mai 2005) $
**	$Author: smork $
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

class B3_PLUGIN b3MemAccess
{
public:
	static inline void *b3Alloc(b3_size size)
	{
		return ::GlobalAlloc(GPTR,size);
	}

	static inline void b3Free(const void *ptr)
	{
#ifdef REALLY_FREE
		::GlobalFree((void *)ptr);
#endif
	}
};

#include "blz3/system/b3Memory.h"

#endif
