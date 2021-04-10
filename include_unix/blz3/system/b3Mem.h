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

#pragma once

#ifndef B3_SYSTEM_MEM_H
#define B3_SYSTEM_MEM_H

#define REALLY_FREE

#include <strings.h>
#include <cstdlib>

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
	 * @note You don't need to compare against @c nullptr since the method
	 * throws the std::bad_alloc exception!
	 *
	 * @param size The size of the new memory block.
	 * @return The new memory block.
	 * @throws std::bad_alloc exception if no memory is available.
	 */
	[[nodiscard]]
	static inline void * b3Alloc(const b3_size size)
	{
		void * ptr = aligned_alloc(16, size);

		if (ptr == nullptr)
		{
			throw std::bad_alloc();
		}
		bzero(ptr, size);
		return ptr;
	}

	template<class T> static inline T * b3TypedAlloc(const b3_size elements = 1)
	{
		const b3_size size = elements * sizeof(T);

		return static_cast<T *>(b3Alloc(size));
	}

	/**
	 * This method frees the specified memory block.
	 *
	 * @param ptr The pointer to the memory block.
	 */
	static inline void b3Free(void * ptr)
	{
#ifdef REALLY_FREE
		std::free(ptr);
#endif
	}
};

#include "blz3/system/b3Memory.h"

#endif
