/*
**
**	$Filename:	b3Memory.h $
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
#include "blz3/system/b3Exception.h"

#define LOOP_B       4
#define LOOP_INSIDE (1 << LOOP_B)
#define LOOP_MASK   (LOOP_INSIDE - 1)

#define B3_MEM_ALIGN 64
#define B3_MEM_MASK  (-B3_MEM_ALIGN)

#define B3_MEM_ALIGN_CHUNK(p)  (void *)(((size_t)(p) + (B3_MEM_ALIGN - 1)) & B3_MEM_MASK)
#define B3_MEM_ASSERT(p)       B3_ASSERT((((size_t)(p)) % B3_MEM_ALIGN) == 0)

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

class B3_PLUGIN b3Mem : protected b3MemNode, protected b3MemAccess
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
		node = (b3MemNode *)b3MemAccess::b3Alloc(size + sizeof(b3MemNode) + B3_MEM_ALIGN);
		if (node == null)
		{
#ifndef _DEBUG
			b3PrintF(B3LOG_NORMAL,"Tried to allocate %d bytes\n",size);
			B3_THROW(b3MemException,B3_MEM_MEMORY);
#else
			return null;
#endif
		}

		// Set values
		node->m_ChunkSize = size;
		node->m_Chunk     = B3_MEM_ALIGN_CHUNK(&node[1]);
		B3_MEM_ASSERT(node->m_Chunk);

		// link into MemNode chain
		m_Mutex.b3Lock();
		b3LinkChunk(node);
		m_Mutex.b3Unlock();

		// Done...
		return node->m_Chunk;
	}

	void *b3Realloc(const void *old_ptr,const b3_size new_size)
	{
		b3MemNode *new_node,*old_node;
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
			old_node = b3FindNode(old_ptr);
			if (old_node != null)
			{
				b3UnlinkChunk(old_node);
				b3MemAccess::b3Free(old_node);
			}
			m_Mutex.b3Unlock();
			return null;
		}

		m_Mutex.b3Lock();
		old_node = b3FindNode(old_ptr);
		if (old_node != null)
		{
			if (old_node->m_ChunkSize < new_size)
			{
				// After realloc node may be invalid
				b3UnlinkChunk(old_node);
				new_node = b3Realloc(
					old_node,
					old_node->m_ChunkSize + sizeof(b3MemNode) + B3_MEM_ALIGN,
					new_size          + sizeof(b3MemNode) + B3_MEM_ALIGN);
				if (new_node != null)
				{
					new_node->m_ChunkSize = new_size;
					new_node->m_Chunk     = B3_MEM_ALIGN_CHUNK(&new_node[1]);

					memcpy(
						new_node->m_Chunk,old_node->m_Chunk,
						B3_MIN(new_node->m_ChunkSize,old_node->m_ChunkSize));
					b3MemAccess::b3Free(old_node);
					
					B3_MEM_ASSERT(new_node->m_Chunk);
					b3LinkChunk(new_node);
				}
				else
				{
					// node is still valid -> relink
					b3LinkChunk(old_node);
					m_Mutex.b3Unlock();
#ifndef _DEBUG
					b3PrintF(B3LOG_NORMAL,"Tried to reallocate %d bytes\n",new_size);
					B3_THROW(b3MemException,B3_MEM_MEMORY);
#else
					return null;
#endif
				}
				old_node = new_node;
			}
			new_ptr = old_node->m_Chunk;
		}
		m_Mutex.b3Unlock();

		// Pointer not found. This shouldn't be but it's failsafe!
		return old_node != null ? new_ptr : b3Alloc(new_size);
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
	static inline b3MemNode *b3Realloc(
		b3MemNode *old_ptr,
		b3_size    old_size,
		b3_size    new_size)
	{
		b3MemNode *new_ptr;

		if (new_size == 0)
		{
			b3MemAccess::b3Free(old_ptr);
			new_ptr = null;
		}
		else
		{
			new_ptr = (b3MemNode *)b3MemAccess::b3Alloc(new_size);
		}
		return new_ptr;
	}

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
