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

	static b3Mutex  m_Mutex;

public:
	static b3_count m_Enlargement;

public:
	inline b3Mem()
	{
		m_SlotCount = B3_MEM_INITIAL_SLOTS;
		m_SlotMax   = 0;
		m_SlotPtr   = m_Slots;
	}

	virtual ~b3Mem()
	{
		b3Free();
		
		if (m_SlotPtr != m_Slots)
		{
			b3MemAccess::b3Free(m_SlotPtr);
		}
	}
	
	void *b3Alloc(const b3_size size)
	{
		void     *ptr = b3MemAccess::b3Alloc(size);
		b3_index  i;

		m_Mutex.b3Lock();
		i = b3FindIndex(null);
		if (i >= 0)
		{
			m_SlotPtr[i].m_Ptr  = ptr;
			m_SlotPtr[i].m_Size = size;
		}
		else
		{
			if (m_SlotMax < m_SlotCount)
			{
				m_SlotPtr[m_SlotMax].m_Ptr  = ptr;
				m_SlotPtr[m_SlotMax].m_Size = size;
				m_SlotMax++;
			}
			else
			{
				b3_count     max   = m_SlotCount + B3_MEM_ADDITIONAL_SLOTS;
				b3_mem_info *slots = (b3_mem_info*)b3MemAccess::b3Alloc(max * sizeof(b3_mem_info));

				if (slots != null)
				{
					memcpy(slots,m_SlotPtr,m_SlotMax * sizeof(b3_mem_info));
					if (m_SlotPtr != m_Slots)
					{
						b3MemAccess::b3Free(m_SlotPtr);
					}

					m_SlotPtr   = slots;
					m_SlotCount = max;
					m_Enlargement++;

					m_SlotPtr[m_SlotMax].m_Ptr  = ptr;
					m_SlotPtr[m_SlotMax].m_Size = size;
					m_SlotMax++;
				}
			}
		}
		m_Mutex.b3Unlock();
		
		return ptr;
	}

	void *b3Realloc(const void *old_ptr,const b3_size new_size)
	{
		void      *new_ptr  = null;

		B3_ASSERT((old_ptr != null) || (new_size > 0));
		if (old_ptr == null)
		{
			new_ptr = b3Alloc(new_size);
		}
		else
		{
			if (new_size > 0)
			{
				b3_index i;

				m_Mutex.b3Lock();
				i = b3FindIndex(old_ptr);
				if (i >= 0)
				{
					if(m_SlotPtr[i].m_Size >= new_size)
					{
						new_ptr = m_SlotPtr[i].m_Ptr;
					}
					else
					{
						new_ptr = b3MemAccess::b3Alloc(new_size);
						if (new_ptr != null)
						{
							memcpy(new_ptr, m_SlotPtr[i].m_Ptr, m_SlotPtr[i].m_Size);
							b3MemAccess::b3Free(m_SlotPtr[i].m_Ptr);
							m_SlotPtr[i].m_Ptr  = new_ptr;
							m_SlotPtr[i].m_Size = new_size;
						}
						else
						{
							// Error!
						}
					}
				}
				else
				{
					// Error!
				}
				m_Mutex.b3Unlock();
			}
			else
			{
				b3Free(old_ptr);
			}
		}
		return new_ptr;
	}

	b3_bool b3Free(const void *ptr)
	{
		b3_index i;
		b3_bool  found = false;

		if (ptr != null)
		{
			m_Mutex.b3Lock();
			i = b3FindIndex(ptr);
			if (i >= 0)
			{
				m_SlotPtr[i].m_Ptr  = null;
				m_SlotPtr[i].m_Size = 0;
				found = true;
			}
			m_Mutex.b3Unlock();

			if (found)
			{
				b3MemAccess::b3Free(ptr);
			}
		}

		return found;
	}

	inline b3_bool b3Free()
	{
		b3_index i;

		m_Mutex.b3Lock();
		for (i = 0;i < m_SlotMax;i++)
		{
			if (m_SlotPtr[i].m_Ptr != null)
			{
				b3MemAccess::b3Free(m_SlotPtr[i].m_Ptr);
				m_SlotPtr[i].m_Ptr  = null;
				m_SlotPtr[i].m_Size = 0;
			}
		}
		m_SlotMax = 0;
		m_Mutex.b3Unlock();

		return true;
	}

	void b3Dump()
	{
		b3_index i;

		m_Mutex.b3Lock();
		b3PrintF (B3LOG_FULL,"### CLASS: b3Mem  # slot max: %d  slot count: %d\n",m_SlotMax,m_SlotCount);
		b3PrintF (B3LOG_FULL,"### CLASS: b3Mem  # slots: %p   initial slots: %p\n",m_SlotPtr, m_Slots);
		for (i = 0;i < m_SlotMax;i++)
		{
			b3PrintF (B3LOG_FULL,"### CLASS: b3Mem  # %3d: %p\n",i,m_SlotPtr[i]);
		}
		m_Mutex.b3Unlock();
	}

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
