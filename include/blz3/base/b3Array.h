/*
**
**	$Filename:	b3Array.h $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - dynamically growing array
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_BASE_ARRAY_H
#define B3_BASE_ARRAY_H

#include "blz3/system/b3Mem.h"
#include "blz3/system/b3Exception.h"
#include "blz3/base/b3Compare.h"

#define B3_ARRAY_DEFAULT_INCREMENT 128

enum b3_array_error
{
	B3_ARRAY_ERROR = -1,
	B3_ARRAY_OK    =  0,
	B3_ARRAY_NO_MEMORY,
	B3_ARRAY_OUT_OF_BOUNDS
};

typedef b3Exception<b3_array_error,'ARR'> b3ArrayException;

template <class T> class b3Array : protected b3Mem
{
	b3_count  m_Increment;
	b3_count  m_Index;
	b3_count  m_Max;
	T        *m_Buffer;

public:
	inline b3Array(b3_count increment = B3_ARRAY_DEFAULT_INCREMENT)
	{
		m_Increment = increment;
		m_Index     = 0;
		m_Max       = 0;
		m_Buffer    = null;
	}

	inline void b3Add(T element)
	{
		T        *buffer;
		b3_count  max = m_Max + m_Increment;

		if (m_Index >= m_Max)
		{
			buffer = (T *)b3Alloc(max * sizeof(T));
			if (buffer != null)
			{
				// Copy old memory (should be done by realloc later!
				if (m_Index > 0)
				{
					B3_ASSERT(m_Buffer != null);
					memcpy (buffer,m_Buffer,m_Index * sizeof(T));
					b3Free(m_Buffer);
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

	inline void b3Clear(b3_bool really_free = false)
	{
		m_Index = 0;
		if (really_free)
		{
			b3Free(m_Buffer);
			m_Buffer = null;
			m_Max    = 0;
		}
	}

	inline b3_count b3GetCount()
	{
		return m_Index;
	}

	inline T *b3GetBuffer()
	{
		return m_Buffer;
	}

	inline T & operator [](const int index)
	{
#ifdef _DEBUG
		if ((index < 0) || (index >= m_Index))
		{
			B3_THROW(b3ArrayException,B3_ARRAY_OUT_OF_BOUNDS);
		}
#endif
		return m_Buffer[index];
	}

	inline void b3Sort(int (*sorter)(T *a,T *b,const void *ptr),const void *ptr)
	{
		qsort(m_Buffer,b3GetCount(),sizeof(T),(b3QSortFunc)sorter);
	}

	inline void b3Sort(int (*sorter)(T *a,T *b))
	{
		qsort(m_Buffer,b3GetCount(),sizeof(T),(b3QSortFunc)sorter);
	}

	inline void b3Sort(b3Comparator<T> comparator,const void *ptr = null)
	{
		qsort(m_Buffer,b3GetCount(),sizeof(T),comparator.b3Sort,ptr);
	}
};

#endif
