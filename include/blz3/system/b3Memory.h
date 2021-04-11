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

#pragma once

#ifndef B3_SYSTEM_MEMORY_H
#define B3_SYSTEM_MEMORY_H

#include "blz3/b3Types.h"
#include "blz3/system/b3Thread.h"
#include "blz3/system/b3Exception.h"

#include <vector>

/**
 * This class handles an amount of memory chunk. Each chunk can be
 * managed seperately. It can be allocated, freed or reallocated.
 * On destruction all memory chunks handled here will be freed
 * automatically.
 *
 * @note This class is thread safe.
 */
class B3_PLUGIN b3Mem : protected b3Mutex
{

	/**
	 * This structure holds information about one memory chunk.
	 */
	struct b3_mem_info
	{
		void  *  m_Ptr;  //!< The memory chunk.
		b3_size  m_Size; //!< The chunk size.
	};

	std::vector<b3_mem_info> m_Slots; //!< The memory chunk information.

public:
	/**
	 * This constructor initializes the chunk handler.
	 */
	b3Mem() = default;

	/**
	 * This method frees all memory chunks registered with this chunk handler. It calls
	 * b3Free() to do this.
	 */
	virtual ~b3Mem();

	/**
	 * This method allocates a memory buffer of the requested size. It returns
	 * null if the requested size lower or equal to zero. If the requested size
	 * is too large to allocate memory a std::bad_alloc exception is thrown.
	 *
	 * @param size The requested memory size. This value can be @c 0 resulting
	 * in a @c nullptr pointer.
	 * @return The allocated memory pointer. @c nullptr means an error so
	 * please check!
	 */
	[[nodiscard]]
	void  *  b3Alloc(const b3_size size);

	template <class T> inline T * b3TypedAlloc(const b3_count count = 1)
	{
		return static_cast<T *>(b3Alloc(sizeof(T) * count));
	}

	/**
	 * This pointer reallocates a memory buffer. There are four cases:
	 * -# oldptr is @c nullptr: b3Realloc() behaves like b3Alloc().
	 * -# oldptr is not @c nullptr:
	 *    -   newsize is 0: b3Realloc() behaves like b3Free().
	 *    -   newsize is less or equal than the previous memory size: oldptr is returned.
	 *    -   newsize is greater than the previous memory size: It returns a resized memory chunk with the old memory content at the beginning.
	 *
	 * @param oldptr   The old memory pointer to resize.
	 * @param newsize  The requested memory size.
	 * @return The new memory pointer.
	 */
	[[nodiscard]]
	void  *  b3Realloc(void * oldptr, const b3_size newsize);

	/**
	 * This method frees the specified memory chunk.
	 *
	 * @param ptr The memory pointer to free.
	 * @return True if the memory chunk is handled and freed by this chunk handler.
	 */
	b3_bool  b3Free(void * ptr) noexcept;

	/**
	 * This method frees all memory chunks handled by this instance.
	 *
	 * @return True if there were any memory chunks.
	 */
	b3_bool  b3Free() noexcept;

	/**
	 * This method returns the number of managed memory chunks.
	 *
	 * @return The number of managed memory chunks.
	 */
	b3_index b3Count() const noexcept;

	/**
	 * This method simply dumps all memory chunks for debugging purposes.
	 */
	void     b3Dump() const noexcept;

	/**
	 * This method returns a short info string about this memory pool.
	 *
	 * @return An info string.
	 */
	operator std::string() const noexcept;

	/**
	 * This static method sets a specified text string into the specified
	 * memory buffer and cuts the string that it fits into this buffer.
	 *
	 * @param buffer The buffer to copy to (destination).
	 * @param size   The buffer size. The source text will be cut to fit into this buffer.
	 * @param src    The text string (source).
	 */
	inline static void b3SetString(
		char * buffer, size_t size, const char * src)
	{
		strncpy(buffer, src, size);
		buffer[size - 1] = 0;
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
		b3_u32     *    data,
		const b3_count  max,
		const b3_u32    value)
	{
		std::fill(data, data + max, value);
	}

	/**
	 * This method copies a complete memory buffer of color quadrupel values to another.
	 * The data pointer must be on a 32 bit boundary. The max value is meant as the number
	 * of 32 bit float quadrupel color values not as bytes. The source and destination buffers must not
	 * be overlapping!
	 *
	 * @param dst  The destination pointer to the color quadrupel buffer.
	 * @param src  The source pointer to the color quadrupel buffer.
	 * @param max  The number of color quadrupel values to copy.
	 */
	inline static void b3ColorMemCopy(
		b3_color    *    dst,
		const b3_color * src,
		const b3_count   max)
	{
		std::copy(src, src + max, dst);
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
		b3_u32     *    dst,
		const b3_u32  * src,
		const b3_count  max)
	{
		std::copy(src, src + max, dst);
	}
};

#endif
