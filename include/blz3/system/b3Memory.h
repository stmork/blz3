/*
**
**	$Filename:	b3Mem.h $
**	$Release:	Dortmund 2001, 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - memory routines
**
**	(C) Copyright 2001, 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_SYSTEM_MEMORY_H
#define B3_SYSTEM_MEMORY_H

#include "blz3/b3Types.h"
#include "blz3/system/b3Thread.h"
#include "blz3/base/b3Exception.h"

#define LOOP_B       4
#define LOOP_INSIDE (1 << LOOP_B)
#define LOOP_MASK   (LOOP_INSIDE - 1)

struct b3MemNode
{
	b3MemNode *m_Succ;
	b3MemNode *m_Prev;
	void      *m_Chunk;
	b3_size    m_ChunkSize;
};

enum b3_mem_error
{
	B3_MEM_ERROR = -1,
	B3_MEM_OK    =  0,
	B3_MEM_MEMORY,
	B3_MEM_UNKNOWN_PTR
};

typedef b3Exception<b3_mem_error,'MEM'> b3MemException;

class b3Mem : protected b3MemNode, protected b3MemAccess
{
	b3Mutex m_Mutex;

public:
	inline b3Mem()
	{
		m_Prev = null;
		m_Succ = null;
		m_ChunkSize = 0;
		m_Chunk     = null;
	}

	virtual ~b3Mem()
	{
		b3Free();
	}
	
	void *b3Alloc(const b3_size size)
	{
		b3MemNode *node;

		// allocate memory block
		node = (b3MemNode *)b3MemAccess::b3Alloc(size + sizeof(b3MemNode));
		if (node == null)
		{
#ifndef _DEBUG
			throw b3MemException(B3_MEM_MEMORY);
#else
			return null;
#endif
		}

		// Set values
		node->m_ChunkSize = size;
		node->m_Chunk     = (void *)&node[1];

		// link into MemNode chain
		m_Mutex.b3Lock();
		b3LinkChunk(node);
		m_Mutex.b3Unlock();

		// Done...
		return node->m_Chunk;
	}

	void *b3Realloc(const void *old_ptr,const b3_size new_size)
	{
		b3MemNode *new_node,*node;
		void      *new_ptr  = null;

		B3_ASSERT((old_ptr != null) || (new_size > 0));
		if (old_ptr == null)
		{
			// Allocate on the standard way...
			return b3Alloc(new_size);
		}
		if (new_size == 0)
		{
			// Free on the standard way
			m_Mutex.b3Lock();
			node = b3FindNode(old_ptr);
			if (node != null)
			{
				b3UnlinkChunk(node);
				b3MemAccess::b3Free(node);
			}
			m_Mutex.b3Unlock();
			return null;
		}

		m_Mutex.b3Lock();
		node = b3FindNode(old_ptr);
		if (node != null)
		{
			if (node->m_ChunkSize < new_size)
			{
				// After realloc node may be invalid
				b3UnlinkChunk(node);
				new_node = (b3MemNode *)b3MemAccess::b3Realloc(node,new_size + sizeof(b3MemNode));
				if (new_node != null)
				{
					new_node->m_ChunkSize = new_size;
					new_node->m_Chunk     = (void *)&new_node[1];
					b3LinkChunk(new_node);
				}
				else
				{
					// node is still valid -> relink
					b3LinkChunk(node);
					m_Mutex.b3Unlock();
#ifndef _DEBUG
					throw b3MemException(B3_MEM_MEMORY);
#else
					return null;
#endif
				}
				node = new_node;
			}
			new_ptr = node->m_Chunk;
		}
		m_Mutex.b3Unlock();

		// Pointer not found. This shouldn't be but it's failsafe!
		return node != null ? new_ptr : b3Alloc(new_size);
	}

	b3_bool b3Free(const void *ptr)
	{
		b3MemNode *node;

		m_Mutex.b3Lock();
		node = b3FindNode(ptr);
		if (node != null)
		{
			// Found! Unlink and free!
			b3UnlinkChunk(node);
			b3MemAccess::b3Free(node);
		}
		m_Mutex.b3Unlock();

		return node != null;
	}

	b3_bool b3Free()
	{
		b3MemNode *next,*node;

		m_Mutex.b3Lock();
		for (node  = m_Succ;
		     node != null;
		     node  = next)
		{
			next = node->m_Succ;
			b3MemAccess::b3Free (node);
		}
		
		// Clear head entries
		m_Succ      = null;
		m_Prev      = null;
		m_ChunkSize = 0;
		m_Chunk     = null;
		m_Mutex.b3Unlock();

		// Done...
		return true;
	}

	void b3Dump()
	{
		b3MemNode *node;

		m_Mutex.b3Lock();
		for (node  = m_Succ;
		     node != null;
		     node  = node->m_Succ)
		{
			b3PrintF (B3LOG_FULL,"### CLASS: b3Mem  # %p: %9ld Bytes.\n",
				node->m_Chunk,
				node->m_ChunkSize);
		}
		m_Mutex.b3Unlock();
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
	inline b3MemNode *b3FindNode(const void *ptr)
	{
		b3MemNode *node;

		for (node = m_Succ;node != null;node = node->m_Succ)
		{
			if (node->m_Chunk == ptr)
			{
				return node;
			}
		}
		return null;
	}

	inline void b3LinkChunk(b3MemNode *node)
	{
		B3_ASSERT(node != null);

		// node pointer
		node->m_Succ = m_Succ;
		node->m_Prev = this;

		// pointer to node
		if (m_Succ != null) m_Succ->m_Prev = node;
		m_Succ = node;
	}

	inline void b3UnlinkChunk(b3MemNode *node)
	{
		b3MemNode *prev,*succ;

		B3_ASSERT(node != null);

		// Get node pointer
		prev = node->m_Prev;
		succ = node->m_Succ;

		// Cross connect those pointer
		if (prev != null) prev->m_Succ = succ;
		if (succ != null) succ->m_Prev = prev;
	}
};

#endif
