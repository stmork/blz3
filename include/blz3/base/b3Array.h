/*
**
**	$Filename:	b3Array.h $
**	$Release:	Dortmund 2002, 2005, 2016 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - dynamically growing array
**
**	(C) Copyright 2002, 2005  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#pragma once

#ifndef B3_BASE_ARRAY_H
#define B3_BASE_ARRAY_H

#include "blz3/system/b3Exception.h"
#include "blz3/base/b3Compare.h"
#include <vector>
#include <algorithm>

enum b3_array_error
{
	B3_ARRAY_ERROR = -1,		//!< General error.
	B3_ARRAY_OK    =  0,		//!< OK.
	B3_ARRAY_NO_MEMORY,			//!< Not enough memory available.
	B3_ARRAY_OUT_OF_BOUNDS,		//!< Array index out of defined bounds.
	B3_ARRAY_INVALID_INCREMENT	//!< Invalid increment. Increments must be greater than zero.
};

typedef b3Exception<b3_array_error, 0x415252> b3ArrayException;

/**
 * This class implements a dynamic sized array of elements. It includes
 * a base amount of elements to avoid dynamic memory allocation.
 */
template <class T> class B3_PLUGIN b3Array
{
	static const b3_count B3_ARRAY_DEFAULT_INCREMENT = 128;
	static const b3_count B3_ARRAY_INITIAL           =  16;

	std::vector<T> elements;

public:
	/**
	 * Constructor with default increment.
	 *
	 * @param increment The incremental reallocations of memory.
	 * @throws b3ArrayException
	 */
	inline b3Array(b3_count increment B3_UNUSED = B3_ARRAY_DEFAULT_INCREMENT)
	{
	}

	/**
	 * The destructor frees the allocated memory.
	 */
	virtual ~b3Array()
	{
		elements.clear();
	}

	/**
	 * This method adds one element and increases the buffer if necessary.
	 *
	 * @param element The new element to add.
	 * @throws b3ArrayException
	 */
	inline void b3Add(const T & element)
	{
		elements.push_back(element);
	}

	/**
	 * This method frees the allocated buffer. If the argument is true the
	 * real memory is freed also. Otherwise only the allocation index is
	 * set to zero.
	 *
	 * @param really_free A flag which indicates if the memory should also be freed.
	 */
	inline void b3Clear(const b3_bool really_free B3_UNUSED = false)
	{
		elements.clear();
	}

	/**
	 * This method returns the collected amounts of elements.
	 *
	 * @return The allocated amount of elements.
	 */
	inline b3_count b3GetCount() const
	{
		return elements.size();
	}

	/**
	 * This method returns the allocated buffer as pointer.
	 *
	 * @return The allocated buffer with the elements.
	 */
	inline T * b3GetBuffer() const
	{
		T * ptr = (T *)elements.data();

		return ptr;
	}

	/**
	 * This method sets another increment value if there is not enough memory
	 * to store new elements.
	 */
	[[deprecated]]
	inline void b3SetIncrement(b3_count increment B3_UNUSED = B3_ARRAY_DEFAULT_INCREMENT)
	{
	}

	/**
	 * This index operator provides access to a special indexed array element.
	 *
	 * @param index The array index.
	 * @throw b3ArrayException
	 * @return The reference to the indexed array element.
	 */
	inline T & operator [](const b3_index index)
	{
#ifdef _DEBUG
		if ((index < 0) || (index >= b3Size()))
		{
			B3_THROW(b3ArrayException, B3_ARRAY_OUT_OF_BOUNDS);
		}
#endif
		return elements[index];
	}

	/**
	 * This index operator provides const access to a special indexed array
	 * element.
	 *
	 * @param index The array index.
	 * @throw b3ArrayException
	 * @return The const reference to the indexed array element.
	 */
	inline const T & operator [](const b3_index index) const
	{
#ifdef _DEBUG
		if ((index < 0) || (index >= b3Size()))
		{
			B3_THROW(b3ArrayException, B3_ARRAY_OUT_OF_BOUNDS);
		}
#endif
		return elements[index];
	}

	/**
	 * This method sorts the array with a custom sorting method.
	 *
	 * @param sorter Sorting method.
	 */
	inline void b3Sort(int (*sorter)(T * a, T * b))
	{
		std::sort(elements.begin(), elements.end(), [&](T a, T b)
		{
			return sorter(&a, &b);
		});
	}

	/**
	 * This method sorts the array using a comparator class.
	 *
	 * @param comparator A comparator class which can compare elements
	 *                   of this template class.
	 * @see b3Comparator
	 */
	inline void b3Sort(b3Comparator<T> & comparator)
	{
		std::sort(elements.begin(), elements.end(), [&](T a, T b)
		{
			return comparator.b3Sort(&a, &b);
		});
	}
};

#endif
