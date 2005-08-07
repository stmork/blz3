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

#define B3_MEM_INITIAL_SLOTS       5
#define B3_MEM_ADDITIONAL_SLOTS  256

/**
 * This enumeration lists all possible memory management errors.
 */
enum b3_mem_error
{
	B3_MEM_ERROR = -1, //!< General error.
	B3_MEM_OK    =  0, //!< Memory handling OK.
	B3_MEM_MEMORY,     //!< ???
	B3_MEM_UNKNOWN_PTR //!< Memory pointer unknown in this chunk handler.
};

typedef b3Exception<b3_mem_error,'MEM'> b3MemException;

/**
 * This structure holds information about one memory chunk.
 */
struct b3_mem_info
{
	void    *m_Ptr;  //!< The memory chunk.
	b3_size  m_Size; //!< The chunk size.
};

/**
 * This class handles an amount of memory chunk. Each chunk can be
 * managed seperately. It can be allocated, freed or reallocated.
 * On destruction all memory chunks handled here will be freed
 * automatically.
 */
class B3_PLUGIN b3Mem : protected b3Mutex
{
	b3_mem_info    *m_SlotPtr; //!< A pointer to the chunk pointer array. This array is initially inside this chunk handler.
	b3_index        m_SlotMax; //!< The size of the chunk pointer array.
	b3_index        m_SlotCount; //!< The used number of chunk pointer. This value is less or equal to the slot max value.
	b3_mem_info     m_Slots[B3_MEM_INITIAL_SLOTS]; //!< This array is the initial chunk pointer array. It does not need any extra memory handling when creating this instance.

public:
	static b3_count m_Enlargement; //!< The amount of enlargement used for the chunk pointer array.

public:
	/**
	 * This constructor initializes the chunk handler.
	 */
	inline b3Mem()
	{
		m_SlotCount = B3_MEM_INITIAL_SLOTS;
		m_SlotMax   = 0;
		m_SlotPtr   = m_Slots;
	}

	/**
	 * This method frees all memory chunks registered with this chunk handler. It calls
	 * b3Free() to do this.
	 */
	virtual ~b3Mem();
	
	/**
	 * This method allocates a memory buffer of the requested size. It returns null if
	 * the requested size is too large to allocate.
	 *
	 * @param size The requested memory size. This value can be 0 resulting in a null pointer.
	 * @return The allocated memory pointer. Null means an error so please check!
	 */
	void    *b3Alloc(const b3_size size);

	/**
	 * This pointer reallocates a memory buffer. There are four cases:
	 * -# oldptr is null: b3Realloc() behaves like b3Alloc().
	 * -# oldptr is not null:
	 * -   newsize is 0: b3Realloc() behaves like b3Free().
	 * -   newsize is less or equal than the previous memory size: oldptr is returned.
	 * -   newsize is greater than the previous memory size: It returns a resized memory chunk with the old memory content at the beginning.
	 *
	 * @param oldptr   The old memory pointer to resize.
	 * @param newsize  The requested memory size.
	 * @return The new memory pointer.
	 */
	void    *b3Realloc(const void *oldptr,const b3_size newsize);

	/**
	 * This method frees the specified memory chunk.
	 *
	 * @param ptr The memory pointer to free.
	 * @return True if the memory chunk is handled and freed by this chunk handler.
	 */
	b3_bool  b3Free(const void *ptr);

	/**
	 * This method frees all memory chunks handled by this instance.
	 *
	 * @return True if there were any memory chunks.
	 */#
	b3_bool  b3Free();

	/**
	 * This method simply dumps all memory chunks for debugging purposes.
	 */
	void     b3Dump();

	/**
	 * This static method sets a specified text string into the specified
	 * memory buffer and cuts the string that it fits into this buffer.
	 *
	 * @param buffer The buffer to copy to (destination).
	 * @param size   The buffer size. The source text will be cut to fit into this buffer.
	 * @param src    The text string (source).
	 */
	inline static void b3SetString(
		char *buffer,size_t size,const char *src)
	{
		strncpy(buffer,src,size);
		buffer[size-1] = 0;
	}

	/**
	 * This method fills a complete memory buffer to an unsigned 32 bit integer value.
	 * The data pointer must be on a 32 bit boundary. The max value is meant as the number
	 * of 32 unsigned integer values not as bytes.
	 *
	 * @param data The pointer to the unsigned 32 bit integer buffer to fill.
	 * @param max  The number of unsigned 32 bit values to fill.
	 * @param value The unsigned 32 bit integer itself.
	 */
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
	
	/**
	 * This method copies a complete memory buffer of unsigned 32 bit integer values to another.
	 * The data pointer must be on a 32 bit boundary. The max value is meant as the number
	 * of 32 unsigned integer values not as bytes. The source and destination buffers must not
	 * be overlapping!
	 *
	 * @param dst  The destination pointer to the unsigned 32 bit integer buffer.
	 * @param src  The source pointer to the unsigned 32 bit integer buffer.
	 * @param max  The number of unsigned 32 bit values to copy.
	 */
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
