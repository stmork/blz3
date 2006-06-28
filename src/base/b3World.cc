/*
**
**	$Filename:	b3World.cc $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Blizzard file structure loading
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

#define not_VERBOSE

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3BaseInclude.h"
#include "blz3/base/b3Array.h"
#include "blz3/base/b3World.h"
#include "blz3/base/b3Endian.h"
#include "blz3/system/b3File.h"
#include "b3ItemRegister.h"

/*************************************************************************
**                                                                      **
**                        b3World: which constructs whole               **
**                                 data structure                       **
**                                                                      **
*************************************************************************/

b3World::b3World()
{
	m_Buffer     = null;
	m_BufferSize = 0;
	m_Start      = null;
	m_AutoDelete = true;
	m_Missed     = 0;
}

b3World::b3World(const char *world_name)
{
	m_AutoDelete = true;
	b3Read(world_name);
}

b3World::~b3World()
{
	if (m_Start != null)
	{
		if (!m_AutoDelete)
		{
			m_Start->b3GetHead()->b3RemoveAll();
		}
		delete m_Start;
	}
}

void b3World::b3Free()
{
	if (m_Start != null)
	{
		m_Start->b3GetHead()->b3RemoveAll();
		delete m_Start;
		m_Start      = null;
	}
	b3Mem::b3Free();
	m_Buffer     = null;
	m_BufferSize = 0;
	m_Missed     = 0;
}

b3_world_error b3World::b3EndianSwapWorld()
{
	b3_size  i,max_file;
	b3_index k,max_node,max_offset;

	b3PrintF(B3LOG_FULL,"Converting endian type.\n");

	i        = 0;
	max_file = m_BufferSize >> 2;
	while (i < max_file)
	{
		// Convert node itself
		for (k = 0;k < 5;k++) b3Endian::b3ChangeEndian32(&m_Buffer[i+k]);

		// Extract size information
		max_node   = m_Buffer[i + B3_NODE_IDX_SIZE] >> 2;
		max_offset = m_Buffer[i + B3_NODE_IDX_OFFSET] >> 2;
		if (max_offset == 0) max_offset = max_node;

		// Make some consistence checking
		if (max_node < B3_NODE_IDX_MIN) return B3_WORLD_PARSE;

		// Convert header an custom area without appending strings
		for (k = 5;k < max_offset;k++) b3Endian::b3ChangeEndian32(&m_Buffer[i+k]);

		i += max_node;
	}
	return i == max_file ? B3_WORLD_OK : B3_WORLD_PARSE;
}

b3Item *b3World::b3AllocNode(b3_u32 class_value, const b3_bool throw_exception)
{
	b3ItemRegisterEntry *entry;

	entry = b3ItemRegister::b3Find(class_value);
	if (entry != null)
	{
		return entry->b3Init();
	}
	else
	{
		if (throw_exception)
		{
			B3_THROW(b3WorldException, B3_WORLD_NOT_REGISTERED);
		}
	}
	return 0;
}

b3Item *b3World::b3AllocNode(b3_u32 *buffer, const b3_bool throw_exception)
{
	b3ItemRegisterEntry *entry;
	b3Item              *item;
	b3_u32               class_type = buffer[B3_NODE_IDX_CLASSTYPE];

	entry = b3ItemRegister::b3Find(class_type);
	if (entry != null)
	{
		item = entry->b3Load(buffer);
	}
	else
	{
		if (throw_exception)
		{
			B3_THROW(b3WorldException, B3_WORLD_NOT_REGISTERED);
		}
		else
		{
			item = new b3Item(buffer);
		}
	}
	return item;
}

#ifdef _DEBUG
#define b3Space(level) { for (int i = 0;i < level;i++) b3PrintF(B3LOG_FULL,"  "); }
#endif

b3_world_error b3Item::b3ParseLinkuage(
	b3Item  **array,
	b3_u32    node_count,
	b3_u32    my_class_limit,
	b3_count  level)
{
	b3_u32    pos,i;
	b3_u32    act_class;
	b3_u32    cmp_class;

#ifdef VERBOSE
	b3Space(level);
	b3PrintF (B3LOG_FULL,"---- %08lx %08lx Start\n",b3GetClass(),my_class_limit);
#endif
	for (i = 0;i < m_HeadCount;i++)
	{
		cmp_class = m_Heads[i].b3GetClass();
#ifdef VERBOSE
		b3Space(level);
		b3PrintF (B3LOG_FULL,"       %08lx\n",cmp_class);
#endif
		pos = 1;
		while ((pos < node_count) && ((act_class = array[pos]->b3GetClass()) < my_class_limit))
		{
#ifdef VERBOSE
			b3Space(level);
			b3PrintF (B3LOG_FULL,"         %08lx %08lx %p",
				act_class,
				cmp_class,
				array[pos]);
#endif
			if (act_class == cmp_class)
			{
				m_Heads[i].b3Append(array[pos]);
#ifdef VERBOSE
				b3PrintF (B3LOG_FULL," appended.\n");
#endif
				array[pos]->b3ParseLinkuage(&array[pos],node_count - pos,cmp_class,level + 1);
			}
			else
			{
#ifdef VERBOSE
				b3PrintF (B3LOG_FULL," ignored.\n");
#endif
			}
			pos++;
		}
	}
#ifdef VERBOSE
	b3Space(level);
	b3PrintF (B3LOG_FULL,"---- %08lx %08lx End\n",b3GetClass(),my_class_limit);
#endif
	return B3_WORLD_OK;
}

b3_world_error b3World::b3Parse(const b3_bool throw_exception)
{
	b3_world_error     result;
	b3_index           k;
	b3_u32             i,max_file;
	b3_u32             max_node,max_offset;
	b3Item            *node;
	b3Array<b3Item *>  array;

	result = (m_NeedEndianChange ? b3World::b3EndianSwapWorld() : B3_WORLD_OK);
	if (result != B3_WORLD_OK)
	{
		return result;
	}

	if (b3ItemRegister::b3Find(B3_CLASS_MAX) == null)
	{
		b3PrintF(B3LOG_NORMAL,"Item registry not initialized!\n");
		return B3_WORLD_PARSE;
	}
#ifdef _DEBUG
	b3ItemRegister::b3Dump();
#endif

	b3PrintF(B3LOG_FULL,"Parsing...\n");

	i        = 0;
	max_file = m_BufferSize >> 2;
	while (i < max_file)
	{
		// Extract size information
		max_node   = m_Buffer[i + B3_NODE_IDX_SIZE] >> 2;
		max_offset = m_Buffer[i + B3_NODE_IDX_OFFSET] >> 2;
		if (max_offset == 0) max_offset = max_node;

		// Make some consistence checking
		if (max_node < B3_NODE_IDX_MIN)
		{
			// On error - no chance to proceed
			for(k = 0; k < array.b3GetCount(); k++)
			{
				delete array[k];
			}
			return B3_WORLD_PARSE;
		}

		// Create node
		try
		{
			node = b3AllocNode(&m_Buffer[i], throw_exception);
			if (node == null)
			{
				// On error - no chance to proceed
				for(k = 0; k < array.b3GetCount(); k++)
				{
					delete array[k];
				}
				return B3_WORLD_MEMORY;
			}
			array.b3Add(node);
		}
		catch(b3WorldException &we)
		{
			m_Missed++;
			b3PrintF(B3LOG_DEBUG,"%s\n", we.b3GetErrorMsg());
		}

		i += max_node;
	}

	if (m_Missed > 0)
	{
		for(k = 0; k < array.b3GetCount(); k++)
		{
			delete array[k];
		}
		return B3_WORLD_NOT_REGISTERED;
	}

#ifdef VERBOSE
	if (b3CheckLevel(B3LOG_FULL))
	{
		b3PrintF (B3LOG_FULL,"Counted %d nodes.\n",node_count);
		for(k = 0; k < array.b3GetCount(), k++)
		{
			array[k]->b3DumpSimple();
		}
	}
#endif

	m_Start = (b3FirstItem *)array[0];
	result  = m_Start->b3ParseLinkuage(
		array.b3GetBuffer(),
		array.b3GetCount(),B3_CLASS_MAX);

	return result;
}

b3_bool b3World::b3Read(const char *name, const b3_bool throw_exception) throw(b3WorldException)
{
	b3Path         world_name;
	b3_world_error error;

	if (!b3IsValid(name,world_name))
	{
		error = B3_WORLD_OPEN;
	}
	else
	{
		b3File file;

		if (file.b3Open(world_name,B_READ))
		{
			error = b3Read(&file, throw_exception);
			file.b3Close();
		}
		else
		{
			error = B3_WORLD_OPEN;
		}
	}

	// Cleanup any occured error
	if (error != B3_WORLD_OK)
	{
		m_BufferSize = 0;
		B3_THROW(b3WorldException,error);
	}

	b3Mem::b3Free(m_Buffer);
	m_Buffer = null;

	return true;
}

b3_world_error b3World::b3Read(b3FileAbstract *file, const b3_bool throw_exception)
{
	b3_u32         header[2];
	b3_world_error error = B3_WORLD_ERROR;

	b3Free();

	// Read specified file into buffer
	if (file->b3Read(header,sizeof(header)) == sizeof(header))
	{
		switch(header[0])
		{
		case B3_BLiZ:
			m_NeedEndianChange = false;
			break;

		case B3_ZiLB:
			m_NeedEndianChange = true;
			b3Endian::b3ChangeEndian32(&header[0]);
			b3Endian::b3ChangeEndian32(&header[1]);
			break;

		default:
			error = B3_WORLD_PARSE;
			break;
		}

		if (error != B3_WORLD_PARSE)
		{
			m_BufferSize = header[1];
			m_Buffer     = (b3_u32 *)b3Alloc(m_BufferSize);
			if (m_Buffer != null)
			{
				if (file->b3Read(m_Buffer,m_BufferSize) == m_BufferSize)
				{
					error = b3Parse(throw_exception);
				}
				else
				{
					error = B3_WORLD_READ;
					b3Mem::b3Free(m_Buffer);
					m_Buffer = null;
				}
			}
			else
			{
				error = B3_WORLD_MEMORY;
			}
		}
	}
	else
	{
		error = B3_WORLD_READ;
	}

	return error;
}

b3_bool b3World::b3ReadDump(const char *world_name) throw(b3WorldException)
{
	b3File         file;
	b3_world_error error;
	b3_size        i,max_file;
	b3_index       max_node,max_offset;

	if (file.b3Open(world_name,B_READ))
	{
		error = b3Read(&file);
		if (error == B3_WORLD_OK)
		{
			error = (m_NeedEndianChange ? b3World::b3EndianSwapWorld() : B3_WORLD_OK);
		}
		file.b3Close();

		// Cleanup any occured error
		if (error == B3_WORLD_OK)
		{
			i        = 0;
			max_file = m_BufferSize >> 2;
			while (i < max_file)
			{
				// Extract size information
				max_node   = m_Buffer[i + B3_NODE_IDX_SIZE] >> 2;
				max_offset = m_Buffer[i + B3_NODE_IDX_OFFSET] >> 2;
				if (max_offset == 0) max_offset = max_node;

				// Print node class/type
				b3PrintF(B3LOG_NORMAL,"%04lx:%04lx s:%6lu o:%6lu # ",
					m_Buffer[i + B3_NODE_IDX_CLASSTYPE] >> 16,
					m_Buffer[i + B3_NODE_IDX_CLASSTYPE] & 0xffff,
					m_Buffer[i + B3_NODE_IDX_SIZE],
					m_Buffer[i + B3_NODE_IDX_OFFSET]);

				// Print heads
				for (int k = B3_NODE_IDX_MIN;m_Buffer[i+k] != null;k += 3)
				{
					b3PrintF(B3LOG_NORMAL,"%08lx ",m_Buffer[i+k]);
				}
				b3PrintF(B3LOG_NORMAL,"\n");

				// Print custom area (longs)
				// Print strings at node end
				i += max_node;
			}
		}
	}
	else
	{
		error = B3_WORLD_OPEN;
	}

	b3Mem::b3Free(m_Buffer);
	m_Buffer     = null;
	m_BufferSize = 0;
	if (error != B3_WORLD_OK)
	{
		B3_THROW(b3WorldException,error);
	}
	return error == B3_WORLD_OK;
}

void b3World::b3CloneBase(
	b3Base<b3Item> *srcBase,
	b3Base<b3Item> *dstBase, const b3_bool throw_exception) throw(b3WorldException)
{
	b3ItemRegisterEntry *entry;
	b3Item              *srcItem;
	b3Item              *dstItem;
	b3_u32               i;

	B3_FOR_BASE(srcBase,srcItem)
	{
		entry = b3ItemRegister::b3Find(srcItem->b3GetClassType());
		if (entry != null)
		{
			dstItem = entry->b3Load(srcItem->m_StoreBuffer);
		}
		else
		{
			if (throw_exception)
			{
				B3_THROW(b3WorldException,B3_WORLD_CLASSTYPE_UNKNOWN);
			}
			else
			{
				dstItem = new b3Item(srcItem->m_StoreBuffer);
			}
		}

		dstBase->b3Append(dstItem);
		for(i = 0;i < srcItem->m_HeadCount;i++)
		{
			b3CloneBase(&srcItem->m_Heads[i],&dstItem->m_Heads[i], throw_exception);
		}
		srcItem->b3Free(srcItem->m_StoreBuffer);
		srcItem->m_StoreBuffer = null;
		srcItem->m_StoreSize   = 0;
	}
}

b3Item *b3World::b3Clone(b3Item *original, const b3_bool throw_exception) throw(b3WorldException)
{
	b3ItemRegisterEntry *entry;
	b3Item              *item;
	b3_u32               i;

	original->b3Store();
	entry = b3ItemRegister::b3Find(original->b3GetClassType());
	if (entry != null)
	{
		item = entry->b3Load(original->m_StoreBuffer);
	}
	else
	{
		if (throw_exception)
		{
			B3_THROW(b3WorldException,B3_WORLD_CLASSTYPE_UNKNOWN);
		}
		else
		{
			item = new b3Item(original->m_StoreBuffer);
		}
	}

	for(i = 0;i < original->m_HeadCount;i++)
	{
		b3CloneBase(&original->m_Heads[i],&item->m_Heads[i], throw_exception);
	}
	original->b3Free(original->m_StoreBuffer);
	original->m_StoreBuffer = null;
	original->m_StoreSize   = 0;

	return item;
}

b3_bool b3World::b3Write(
	const char    *filename,
	const b3_bool  throw_exception) throw(b3WorldException)
{
	b3File          file;
	b3_world_error  error;

	if(file.b3Open(filename,B_WRITE))
	{
		error = b3Write(&file, throw_exception);
		file.b3Close();
	}
	else
	{
		error = B3_WORLD_WRITE;
	}

	if (error != B3_WORLD_OK)
	{
		B3_THROW(b3WorldException,error);
	}
	return true;
}

b3_world_error b3World::b3Write(b3FileAbstract *file, const b3_bool throw_exception)
{
	b3Item         *item;
	b3_u32          size;
	b3_u32          header[2];
	b3_world_error  error = B3_WORLD_OK;

	header[0] = B3_BLiZ;
	header[1] = 0;

	// Prepare each item's buffer
	for (item = m_Start;item != null;item = item->Succ)
	{
		size = item->b3Store();
		if (size == 0)
		{
			error = B3_WORLD_MEMORY;
			break;
		}
		header[1] += size;
	}

	// Write buffers to file
	if ((error == B3_WORLD_OK) &&
	    (file->b3Write(header,sizeof(header)) == sizeof(header)))
	{
		for (item = m_Start;item != null;item = item->Succ)
		{
			error = item->b3StoreFile(file);
			if (error != B3_WORLD_OK)
			{
				break;
			}
		}
	}
	else
	{
		error = B3_WORLD_WRITE;
	}
	return error;
}

void b3World::b3Dump()
{
	if (m_Start != null)
	{
		b3PrintF(B3LOG_FULL,"--- World dump:\n");
		m_Start->b3Dump(0);
		b3PrintF(B3LOG_FULL,"--- World dump complete.\n");
	}
}

b3Item *b3World::b3RemoveFirst()
{
	return m_Start != null ? m_Start->b3GetHead()->b3RemoveFirst() : null;
}

b3Item *b3World::b3GetFirst()
{
	return m_Start != null ? m_Start->b3GetHead()->First : null;
}

void b3World::b3SetFirst(b3Item *item)
{
	if (m_Start == null)
	{
		m_Start = new b3FirstItem(B3_CLASS_MAX);
		m_Start->b3GetHead()->b3InitBase(item->b3GetClass());
	}
	m_Start->b3GetHead()->b3First(item);
}

b3Base<b3Item> *b3World::b3GetHead(b3_u32 class_value)
{
	b3Base<b3Item> *base = null;

	if (m_Start != null)
	{
		base = m_Start->b3GetHead();
		if ((class_value != 0) && (base->b3GetClass() != class_value))
		{
			base = null;
		}
	}
	return base;
}

