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
#include <stdio.h>
#include "blz3/b3Types.h"

class B3_PLUGIN b3MemAccess
{
public:
	static inline void *b3Alloc(b3_size size)
	{
		return calloc(size,1);
	}

	static inline void b3Free(const void *ptr)
	{
#ifdef REALLY_FREE
		free((void *)ptr);
#endif
	}
};

#include "blz3/system/b3Memory.h"

#endif
