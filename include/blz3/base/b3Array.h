/*
**
**	$Filename:	b3Array.h $
**	$Release:	Dortmund 2002, 2005 $
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

#ifndef B3_BASE_ARRAY_H
#define B3_BASE_ARRAY_H

#include "blz3/system/b3Exception.h"
#include "blz3/base/b3Compare.h"

#define B3_ARRAY_INITIAL            10
#define B3_ARRAY_DEFAULT_INCREMENT 128

enum b3_array_error
{
	B3_ARRAY_ERROR = -1,		//!< General error.
	B3_ARRAY_OK    =  0,		//!< OK.
	B3_ARRAY_NO_MEMORY,			//!< Not enough memory available.
	B3_ARRAY_OUT_OF_BOUNDS,		//!< Array index out of defined bounds.
	B3_ARRAY_INVALID_INCREMENT	//!< Invalid increment. Increments must be greater than zero.
};

typedef b3Exception<b3_array_error,'ARR'> b3ArrayException;

/**
 * This class implements a dynamic sized array of elements. It includes
 * a base amount of elements to avoid dynamic memory allocation.
 */
template <class T> class B3_PLUGIN b3Array
{
	b3_count  m_Increment;
	b3_index  m_Index;
	b3_count  m_Max;
	T        *m_Buffer;
	T         m_Start[B3_ARRAY_INITIAL];

public:
	/**
	 * Constructor with default increment.
	 *
	 * @param increment The incremental reallocations of memory.
	 */
	inline b3Array(b3_count increment = B3_ARRAY_DEFAULT_INCREMENT)
	{
		if (increment <= 0)
		{
			B3_THROW(b3ArrayException,B3_ARRAY_INVALID_INCREMENT);
		}
		m_Increment = increment;
		m_Index     = 0;
		m_Max       = B3_ARRAY_INITIAL;
		m_Buffer    = m_Start;
	}

	/**
	 * The destructor frees the allocated memory.
	 */
	inline virtual ~b3Array()
	{
		if (m_Buffer != m_Start)
		{
			delete [] m_Buffer;
		}
	}

	/**
	 * This method adds one element and increases the buffer if necessary.
	 *
	 * @param element The new element to add.
	 */
	inline void b3Add(T element)
	{
		T        *buffer;
		b3_count  max = m_Max + m_Increment;

		if (m_Index >= m_Max)
		{
			buffer = new T[max];
			if (buffer != null)
			{
				// Copy old memory (should be done by realloc later!
				if (m_Index > 0)
				{
					B3_ASSERT(m_Buffer != null);
					memcpy (buffer,m_Buffer,m_Index * sizeof(T));
					if (m_Buffer != m_Start)
					{
						delete [] m_Buffer;
					}
				}

				// Setup new values
				m_Buffer = buffer;
				m_Max    = max;
			}
			else
			{
				B3_THROW(b3ArrayException,B3_ARRAY_NO_MEMORY);
			}
		}
		m_Buffer[m_Index++] = element;
	}

	/**
	 * This method frees the allocated buffer. If the argument is true the
	 * real memory is freed also. Otherwise only the allocation index is
	 * set to zero.
	 *
	 * @param really_free A flag which indicates if the memory should also be freed.
	 */
	inline void b3Clear(b3_bool really_free = false)
	{
		m_Index = 0;
		if (really_free)
		{
			if (m_Buffer != m_Start)
			{
				delete [] m_Buffer;
			}
			m_Buffer = null;
			m_Max    = 0;
		}
	}

	/**
	 * This method returns the collected amounts of elements.
	 *
	 * @return The allocated amount of elements.
	 */
	inline b3_count b3GetCount()
	{
		return m_Index;
	}

	/**
	 * This method returns the allocated buffer as pointer.
	 *
	 * @return The allocated buffer with the elements.
	 */
	inline T *b3GetBuffer()
	{
		return m_Buffer;
	}

	/**
	 * This method sets another increment value if there is not enough memory
	 * to store new elements.
	 */
	inline void b3SetIncrement(b3_count increment = B3_ARRAY_DEFAULT_INCREMENT)
	{
		m_Increment = increment;
	}

	inline T & operator [](const b3_index index)
	{
#ifdef _DEBUG
		if ((index < 0) || (index >= m_Index))
		{
			B3_THROW(b3ArrayException,B3_ARRAY_OUT_OF_BOUNDS);
		}
#endif
		return m_Buffer[index];
	}

	/**
	 * This method sorts the array with a custom sorting method.
	 *
	 * @param sorter Sorting method.
	 * @param ptr    Custom pointer for custom information retrieval.
	 */
	inline void b3Sort(int (*sorter)(T *a,T *b,const void *ptr),const void *ptr)
	{
		qsort(m_Buffer,b3GetCount(),sizeof(T),(b3QSortFunc)sorter);
	}

	/**
	 * This method sorts the array with a custom sorting method.
	 *
	 * @param sorter Sorting method.
	 */
	inline void b3Sort(int (*sorter)(T *a,T *b))
	{
		qsort(m_Buffer,b3GetCount(),sizeof(T),(b3QSortFunc)sorter);
	}

	/**
	 * This method sorts the array using a comparator class.
	 *
	 * @param comparator A comparator class which can compare elements
	 *                   of this template class.
	 * @see b3Comparator
	 */
	inline void b3Sort(b3Comparator<T> comparator,const void *ptr = null)
	{
		qsort(m_Buffer,b3GetCount(),sizeof(T),comparator.b3Sort,ptr);
	}
};

#endif
