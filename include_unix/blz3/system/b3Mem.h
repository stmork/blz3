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

#include "blz3/b3Types.h"
#include "blz3/system/b3Thread.h"

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
	b3_MEM_UNKNOWN_PTR
} b3_mem_error;

class b3MemException
{
protected:
	b3_mem_error error;
	
public:
	b3MemException(b3_mem_error error = B3_MEM_ERROR)
	{
		this->error = error;
	}

	b3_mem_error b3GetError()
	{
		return error;
	}
};

class b3Mem : public b3MemNode
{
protected:
	b3Mutex    mutex;

public:
	           b3Mem();
	          ~b3Mem();
	void      *b3Alloc(const b3_size);
	b3_bool    b3Free(const void *);
	b3_bool    b3Free();
	void       b3Dump();
};

#endif
