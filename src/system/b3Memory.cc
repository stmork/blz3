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

#include "b3SystemIndInclude.h"
#include "blz3/system/b3Mem.h"

/*************************************************************************
**                                                                      **
**                        Implementation                                **
**                                                                      **
*************************************************************************/

b3_count b3Mem::m_Enlargement = 0;

b3Mem::~b3Mem()
{
	b3Free();

	if (m_SlotPtr != m_Slots)
	{
		b3MemAccess::b3Free(m_SlotPtr);
	}
}

void * b3Mem::b3Alloc(const b3_size size)
{
	void       *       ptr = b3MemAccess::b3Alloc(size);
	b3_index           i;
	b3CriticalSection  lock(*this);

	i = b3FindIndex(nullptr);
	if (i >= 0)
	{
		// Found empty slot
		m_SlotPtr[i].m_Ptr  = ptr;
		m_SlotPtr[i].m_Size = size;
	}
	else
	{
		if (m_SlotMax < m_SlotCount)
		{
			// There are enough unused slots available
			m_SlotPtr[m_SlotMax].m_Ptr  = ptr;
			m_SlotPtr[m_SlotMax].m_Size = size;
			m_SlotMax++;
		}
		else
		{
			// We need more slots...
			b3_count      max   = m_SlotCount + B3_MEM_ADDITIONAL_SLOTS;
			b3_mem_info * slots = b3MemAccess::b3AllocTyped<b3_mem_info>(max);

			if (slots != nullptr)
			{
				memcpy(slots, m_SlotPtr, m_SlotMax * sizeof(b3_mem_info));
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
			else
			{
				// If there is no memory for new slots we should better
				// check really requested memory chunk.
				if (ptr != nullptr)
				{
					b3MemAccess::b3Free(ptr);
				}
				ptr = nullptr;
			}
		}
	}

	return ptr;
}

void * b3Mem::b3Realloc(const void * old_ptr, const b3_size new_size)
{
	void   *   new_ptr  = nullptr;

	B3_ASSERT((old_ptr != nullptr) || (new_size > 0));
	if (old_ptr == nullptr)
	{
		new_ptr = b3Alloc(new_size);
	}
	else
	{
		if (new_size > 0)
		{
			b3CriticalSection lock(*this);
			b3_index i;

			i = b3FindIndex(old_ptr);
			if (i >= 0)
			{
				if (m_SlotPtr[i].m_Size >= new_size)
				{
					new_ptr = m_SlotPtr[i].m_Ptr;
				}
				else
				{
					new_ptr = b3MemAccess::b3Alloc(new_size);
					if (new_ptr != nullptr)
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
		}
		else
		{
			b3Free(old_ptr);
		}
	}
	return new_ptr;
}

b3_bool b3Mem::b3Free(const void * ptr)
{
	b3_index i;
	b3_bool  found = false;

	if (ptr != nullptr)
	{
		// Critical Section
		{
			b3CriticalSection lock(*this);

			i = b3FindIndex(ptr);
			if (i >= 0)
			{
				m_SlotPtr[i].m_Ptr  = nullptr;
				m_SlotPtr[i].m_Size = 0;
				found = true;
			}
		}

		if (found)
		{
			b3MemAccess::b3Free(ptr);
		}
	}

	return found;
}

b3_bool b3Mem::b3Free()
{
	b3CriticalSection lock(*this);
	b3_index          i;

	for (i = 0; i < m_SlotMax; i++)
	{
		if (m_SlotPtr[i].m_Ptr != nullptr)
		{
			b3MemAccess::b3Free(m_SlotPtr[i].m_Ptr);
			m_SlotPtr[i].m_Ptr  = nullptr;
			m_SlotPtr[i].m_Size = 0;
		}
	}
	m_SlotMax = 0;

	return true;
}

void b3Mem::b3Dump()
{
	b3CriticalSection lock(*this);
	b3_index          i;

	b3PrintF(B3LOG_FULL, "### CLASS: b3Mem  # slot max: %d  slot count: %d\n", m_SlotMax, m_SlotCount);
	b3PrintF(B3LOG_FULL, "### CLASS: b3Mem  # slots: %p   initial slots: %p\n", m_SlotPtr, m_Slots);
	for (i = 0; i < m_SlotMax; i++)
	{
		b3PrintF(B3LOG_FULL, "### CLASS: b3Mem  # %3d: %p\n", i, m_SlotPtr[i]);
	}
}
