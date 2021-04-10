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
}

void * b3Mem::b3Alloc(const b3_size size)
{
	b3_mem_info info;

	info.m_Ptr  = b3MemAccess::b3Alloc(size);
	info.m_Size = size;

	b3CriticalSection  lock(*this);

	m_Slots.push_back(info);

	return info.m_Ptr;
}

void * b3Mem::b3Realloc(void * old_ptr, const b3_size new_size)
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
			for (b3_mem_info & info : m_Slots)
			{
				if (info.m_Ptr == old_ptr)
				{
					if (info.m_Size >= new_size)
					{
						new_ptr = info.m_Ptr;
					}
					else
					{
						b3CriticalSection lock(*this);

						new_ptr = b3MemAccess::b3Alloc(new_size);

						memcpy(new_ptr, info.m_Ptr, info.m_Size);
						b3MemAccess::b3Free(info.m_Ptr);
						info.m_Ptr  = new_ptr;
						info.m_Size = new_size;
					}
					break;
				}
			}
		}
		else
		{
			b3Free(old_ptr);
		}
	}
	return new_ptr;
}

b3_bool b3Mem::b3Free(void * ptr)
{
	for (b3_mem_info & info : m_Slots)
	{
		if (info.m_Ptr == ptr)
		{
			b3CriticalSection lock(*this);

			const b3_mem_info & last = *m_Slots.rbegin();

			info = last;
			m_Slots.pop_back();

			b3MemAccess::b3Free(ptr);

			return true;
		}
	}

	return false;
}

b3_bool b3Mem::b3Free()
{
	b3CriticalSection lock(*this);

	for (const b3_mem_info & info : m_Slots)
	{
		b3MemAccess::b3Free(info.m_Ptr);
	}
	m_Slots.clear();

	return true;
}

b3_index b3Mem::b3Count() const
{
	return m_Slots.size();
}

void b3Mem::b3Dump() const
{
	const std::string & text(*this);

	b3PrintF(B3LOG_FULL, "%s", text.c_str());
}

b3Mem::operator std::string() const
{
	std::string       result;
	char              buffer[256];

	snprintf(buffer, sizeof(buffer),
		"### CLASS: b3Mem  # slot count: %ld\n", m_Slots.size());

	result = buffer;
	for (const b3_mem_info & info : m_Slots)
	{
		snprintf(buffer, sizeof(buffer),
			"### CLASS: b3Mem  # %3zd: %p\n", info.m_Size, info.m_Ptr);

		result += buffer;
	}
	return result;
}
