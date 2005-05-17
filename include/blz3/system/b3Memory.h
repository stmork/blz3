/*
**
**	$Filename:	b3Memory.h $
**	$Release:	Dortmund 2001, 2002, 2005 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - memory routines (optimized handling)
**
**	(C) Copyright 2001, 2002, 2005  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_MEMORY_H
#define B3_SYSTEM_MEMORY_H

#include "blz3/b3Types.h"
#include "blz3/system/b3Thread.h"
#include "blz3/system/b3Exception.h"

#define LOOP_B      (b3_ptr(4))
#define LOOP_INSIDE (b3_ptr(1 << LOOP_B))
#define LOOP_MASK   (LOOP_INSIDE - 1)

#define B3_MEM_INITIAL_SLOTS       8
#define B3_MEM_ADDITIONAL_SLOTS  256

enum b3_mem_error
{
	B3_MEM_ERROR = -1,
	B3_MEM_OK    =  0,
	B3_MEM_MEMORY,
	B3_MEM_UNKNOWN_PTR
};

typedef b3Exception<b3_mem_error,'MEM'> b3MemException;

struct b3_mem_info
{
	void    *m_Ptr;
	b3_size  m_Size;
};

class B3_PLUGIN b3Mem : protected b3MemAccess
{
	b3_index        m_SlotCount;
	b3_index        m_SlotMax;
	b3_mem_info    *m_SlotPtr;
	b3_mem_info     m_Slots[B3_MEM_INITIAL_SLOTS];

	b3Mutex         m_Mutex;

public:
	static b3_count m_Enlargement;

public:
	inline b3Mem()
	{
		m_SlotCount = B3_MEM_INITIAL_SLOTS;
		m_SlotMax   = 0;
		m_SlotPtr   = m_Slots;
	}

	virtual ~b3Mem();
	
	void    *b3Alloc(const b3_size size);
	void    *b3Realloc(const void *old_ptr,const b3_size new_size);
	b3_bool  b3Free(const void *ptr);
	b3_bool  b3Free();
	void     b3Dump();

	inline static void b3SetString(
		char *buffer,size_t size,const char *src)
	{
		strncpy(buffer,src,size);
		buffer[size-1] = 0;
	}

	inline static void b3LongMemSet(
		      b3_u32   *data,
		const b3_count  max,
		const b3_u32    value)
	{
		b3_index  i;
		b3_count  long_max,short_max;

		// Compute loop sizes
		long_max  = max >> LOOP_B;
		short_max = max &  LOOP_MASK;

		// Long copy
		for (i = 0;i < long_max;i++)
		{
			*data++ = value;
			*data++ = value;
			*data++ = value;
			*data++ = value;
			*data++ = value;
			*data++ = value;
			*data++ = value;
			*data++ = value;
			*data++ = value;
			*data++ = value;
			*data++ = value;
			*data++ = value;
			*data++ = value;
			*data++ = value;
			*data++ = value;
			*data++ = value;
		}

		// Copy rest
		for (i = 0;i < short_max;i++)
		{
			*data++ = value;
		}
	}
	
	inline static void b3LongMemCopy(
		      b3_u32   *dst,
		const b3_u32   *src,
		const b3_count  max)
	{
		b3_index  i;
		b3_count  long_max,short_max;

		// Compute loop sizes
		long_max  = max >> LOOP_B;
		short_max = max &  LOOP_MASK;

		// Long copy
		for (i = 0;i < long_max;i++)
		{
			*dst++ = *src++;
			*dst++ = *src++;
			*dst++ = *src++;
			*dst++ = *src++;
			*dst++ = *src++;
			*dst++ = *src++;
			*dst++ = *src++;
			*dst++ = *src++;
			*dst++ = *src++;
			*dst++ = *src++;
			*dst++ = *src++;
			*dst++ = *src++;
			*dst++ = *src++;
			*dst++ = *src++;
			*dst++ = *src++;
			*dst++ = *src++;
		}

		// Copy rest
		for (i = 0;i < short_max;i++)
		{
			*dst++ = *src++;
		}
	}

private:
	inline b3_index b3FindIndex(const void *ptr)
	{
		b3_index i;

		for (i = 0;i < m_SlotMax;i++)
		{
			if (m_SlotPtr[i].m_Ptr == ptr)
			{
				return i;
			}
		}
		return -1;
	}
};

#endif
