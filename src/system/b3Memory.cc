/*
**
**	$Filename:	b3Date.cpp $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - date routines
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/system/b3Mem.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2005/05/17 13:56:52  smork
**	- Moved some code into library.
**
**	Revision 1.1  2005/05/17 11:05:18  smork
**	- New memory management based on array rather than doubly linked lists.
**	
**
**
*/

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3Mutex  b3Mem::m_Mutex;
b3_count b3Mem::m_Enlargement = 0;

b3Mem::~b3Mem()
{
	b3Free();
		
	if (m_SlotPtr != m_Slots)
	{
		b3MemAccess::b3Free(m_SlotPtr);
	}
}

void *b3Mem::b3Alloc(const b3_size size)
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

void *b3Mem::b3Realloc(const void *old_ptr,const b3_size new_size)
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

b3_bool b3Mem::b3Free(const void *ptr)
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

b3_bool b3Mem::b3Free()
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

void b3Mem::b3Dump()
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
