/*
**
**      $Filename:      b3World.cc $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Author$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - Blizzard file structure loading
**
**      (C) Copyright 2001  Steffen A. Mork
**          All Rights Reserved
**
**
**
**
*/

#define no_DEBUG_VERBOSE

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/b3Config.h"
#include "blz3/base/b3World.h"
#include "blz3/system/b3File.h"
#include "b3ItemRegister.h"

#ifdef _DEBUG
#define	ASSERT_INDEX	B3_ASSERT((parseIndex << 2) < (b3_index)size)
#else
#define ASSERT_INDEX
#endif

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.10  2001/08/14 07:03:28  sm
**      - Made some ASSERT cleanups. New define when _DEBUG is switched on:
**        B3_ASSERT(condition) abort()s when condition is false.
**
**      Revision 1.9  2001/08/05 19:53:43  sm
**      - Removing some nasty CR/LF
**
**      Revision 1.8  2001/08/05 19:51:56  sm
**      - Now having OpenGL software for Windows NT and created
**        new Lines III.
**
**      Revision 1.7  2001/08/05 12:46:06  sm
**      - Splitting b3ItemXXX routines
**
**      Revision 1.6  2001/08/02 15:37:17  sm
**      - Now we are able to draw Blizzard Scenes with OpenGL.
**
**      Revision 1.5  2001/07/08 12:30:06  sm
**      - New tool to remove nasty CR/LF from Windoze.
**      - Removing some nasty CR/LF with that new tool.
**
**      Revision 1.4  2001/07/02 19:28:25  sm
**      - Applying console application on Windows 32
**      - Added further Windows environment
**
**      Revision 1.3  2001/07/02 16:09:46  sm
**      - Added bounding box reorganization.
**
**      Revision 1.2  2001/07/01 16:31:52  sm
**      - Creating MSVC Projects
**      - Welcome to Windows 32
**
**      Revision 1.1.1.1  2001/07/01 12:24:58  sm
**      Blizzard III is born
**
*/

/*************************************************************************
**                                                                      **
**                        b3FirstItem: which contains Blizzard data     **
**                                                                      **
*************************************************************************/

b3FirstItem::b3FirstItem(b3_u32  class_type) : b3Item(sizeof(b3FirstItem),class_type)
{
}

b3FirstItem::b3FirstItem(b3_u32 *src) : b3Item(src)
{
}

b3Item *b3FirstItem::b3GetFirst()
{
	return heads[0].First;
}

/*************************************************************************
**                                                                      **
**                        b3World: which constructs whole               **
**                                 data structure                       **
**                                                                      **
*************************************************************************/

b3World::b3World()
{
	b3Free();
	buffer = null;
	size   = 0;
	start  = null;
}

b3World::b3World(const char *world_name)
{
	b3Read(world_name);
}

b3World::~b3World()
{
	if (start != null)
	{
		delete start;
	}
}

void b3World::b3EndianSwap32(b3_u32 *uPtr)
{
	register b3_u08 *ptr = (b3_u08 *)uPtr;

	B3_SWAP(ptr[0],ptr[3]);
	B3_SWAP(ptr[1],ptr[2]);
}

b3_world_error b3World::b3EndianSwapWorld()
{
	b3_index i,max_file;
	b3_index k,max_node,max_offset;

	b3PrintF(B3LOG_FULL,"Converting endian type.\n");

	i        = 0;
	max_file = size >> 2;
	while (i < max_file)
	{
		// Convert node itself
		for (k = 0;k < 5;k++) b3World::b3EndianSwap32(&buffer[i+k]);

		// Extract size information
		max_node   = buffer[i + B3_NODE_IDX_SIZE] >> 2;
		max_offset = buffer[i + B3_NODE_IDX_OFFSET] >> 2;
		if (max_offset == 0) max_offset = max_node;

		// Make some consistence checking
		if (max_node < B3_NODE_IDX_MIN) return B3_WORLD_PARSE;

		// Convert header an custom area without appending strings
		for (k = 5;k < max_offset;k++) b3World::b3EndianSwap32(&buffer[i+k]);

		i += max_node;
	}
	return i == max_file ? B3_WORLD_OK : B3_WORLD_PARSE;
}

b3Item *b3World::b3AllocNode(b3_u32 class_value)
{
	b3ItemRegisterEntry *entry;

	entry = b3_item_register.b3Find(class_value);
	if (entry != null)
	{
		return entry->b3Init();
	}
	return 0;
}

b3Item *b3World::b3AllocNode(b3_u32 *buffer)
{
	b3ItemRegisterEntry *entry;
	b3Item              *item;

	entry = b3_item_register.b3Find(buffer[B3_NODE_IDX_CLASSTYPE]);
	if (entry != null)
	{
		item = entry->b3Load(buffer);
	}
	else
	{
		item = new b3Item(buffer);
	}
	return item;
}

#ifdef _DEBUG
static void b3Space(b3_count level)
{
	for (int i = 0;i < level;i++)
	{
		b3PrintF(B3LOG_FULL,"  ");
	}
}
#endif

b3_world_error b3Item::b3ParseLinkuage(
	b3Item  **array,
	b3_count  node_count,
	b3_u32    my_class_limit,
	b3_count  level)
{
	b3_index  pos,i;
	b3_u32    act_class;
	b3_u32    cmp_class;

#ifdef _DEBUG_VERBOSE
	b3Space(level);
	b3PrintF (B3LOG_FULL,"---- %08lx %08lx Start\n",b3GetClass(),my_class_limit);
#endif
	for (i = 0;i < head_count;i++)
	{
		cmp_class = heads[i].b3GetClass();
#ifdef _DEBUG_VERBOSE
		b3Space(level);
		b3PrintF (B3LOG_FULL,"       %08lx\n",cmp_class);
#endif
		pos = 1;
		while ((pos < node_count) && ((act_class = array[pos]->b3GetClass()) < my_class_limit))
		{
#ifdef _DEBUG_VERBOSE
			b3Space(level);
			b3PrintF (B3LOG_FULL,"         %08lx %08lx %p",
				act_class,
				cmp_class,
				array[pos]);
#endif
			if (act_class == cmp_class)
			{
				heads[i].b3Append(array[pos]);
#ifdef _DEBUG_VERBOSE
				b3PrintF (B3LOG_FULL," appended.\n");
#endif
				array[pos]->b3ParseLinkuage(&array[pos],node_count - pos,cmp_class,level + 1);
			}
			else
			{
#ifdef _DEBUG_VERBOSE
				b3PrintF (B3LOG_FULL," ignored.\n");
#endif
			}
			pos++;
		}
	}
#ifdef _DEBUG_VERBOSE
	b3Space(level);
	b3PrintF (B3LOG_FULL,"---- %08lx %08lx End\n",b3GetClass(),my_class_limit);
#endif
	return B3_WORLD_OK;
}

b3_world_error b3World::b3Parse()
{
	b3_world_error  result;
	b3_index        i,max_file;
	b3_index        max_node,max_offset;
	b3_count        node_count = 0;
	b3Base<b3Item>  node_list;
	b3Item         *node;
	b3Item       **array;

	result = (need_endian_change ? b3World::b3EndianSwapWorld() : B3_WORLD_OK);
	if (result != B3_WORLD_OK)
	{
		return result;
	}

	if (b3_item_register.b3Find(B3_CLASS_MAX) == null)
	{
		b3PrintF(B3LOG_FULL,"Initializing item registry...\n");
		b3Item::b3Register(&b3FirstItem::b3Init,&b3FirstItem::b3Init,B3_CLASS_MAX,true);
	}

	b3PrintF(B3LOG_FULL,"Parsing...\n");

	i        = 0;
	max_file = size >> 2;
	while (i < max_file)
	{
		// Extract size information
		max_node   = buffer[i + B3_NODE_IDX_SIZE] >> 2;
		max_offset = buffer[i + B3_NODE_IDX_OFFSET] >> 2;
		if (max_offset == 0) max_offset = max_node;

		// Make some consistence checking
		if (max_node < B3_NODE_IDX_MIN)
		{
			// On error - no chance to proceed
			return B3_WORLD_PARSE;
		}

		// Create node
		node = b3AllocNode(&buffer[i]);
		if (node != null)
		{
			node_list.b3Append(node);
			node_count++;
		}
		else
		{
			// On error - no chance to proceed
			return B3_WORLD_MEMORY;
		}

		i += max_node;
	}

	if (b3CheckLevel(B3LOG_FULL))
	{
		b3PrintF (B3LOG_FULL,"Counted %d nodes.\n",node_count);
		B3_FOR_BASE(&node_list,node)
		{
			node->b3DumpSimple();
		}
	}

	array = (b3Item **)b3Alloc(node_count * sizeof(b3Item *));
	if (array != null)
	{
#ifdef _DEBUG
		b3_count counted = node_list.b3Count();
		B3_ASSERT(counted == node_count);
#endif
		for (i = 0;i < node_count;i++)
		{
			node = node_list.First;
			array[i] = node;
			node_list.b3Remove(node);
		}
		start  = (b3FirstItem *)array[0];
		result = start->b3ParseLinkuage(array,node_count,0x7fff0000);
		b3Free(array);
	}
	else
	{
		result = B3_WORLD_MEMORY;
	}
	return result;
}

b3_bool b3World::b3Read(const char *world_name)
{
	b3_world_error error;

	error = b3ReadInternal(world_name);
	if (error == B3_WORLD_OK)
	{
		error = b3Parse();
	}

	// Cleanup any occured error
	if (error != B3_WORLD_OK)
	{
		size = 0;
		throw new b3WorldException(error);
	}

	b3Free(buffer);
	buffer = null;

	return true;
}

b3_bool b3World::b3ReadDump(const char *world_name)
{
	b3_world_error error;
	b3_index       i,max_file;
	b3_index       k,max_node,max_offset;

	error = b3ReadInternal(world_name);
	if (error == B3_WORLD_OK)
	{
		error = (need_endian_change ? b3World::b3EndianSwapWorld() : B3_WORLD_OK);
	}

	// Cleanup any occured error
	if (error != B3_WORLD_OK)
	{
		size = 0;
		throw new b3WorldException(error);
	}

	i        = 0;
	max_file = size >> 2;
	while (i < max_file)
	{
		// Extract size information
		max_node   = buffer[i + B3_NODE_IDX_SIZE] >> 2;
		max_offset = buffer[i + B3_NODE_IDX_OFFSET] >> 2;
		if (max_offset == 0) max_offset = max_node;

		// Print node class/type
		b3PrintF(B3LOG_NORMAL,"%04lx:%04lx s:%6lu o:%6lu # ",
			buffer[i + B3_NODE_IDX_CLASSTYPE] >> 16,
			buffer[i + B3_NODE_IDX_CLASSTYPE] & 0xffff,
			buffer[i + B3_NODE_IDX_SIZE],
			buffer[i + B3_NODE_IDX_OFFSET]);

		// Print heads
		for (k = B3_NODE_IDX_MIN;buffer[i+k] != null;k += 3)
		{
			b3PrintF(B3LOG_NORMAL,"%08lx ",buffer[i+k]);
		}
		k++;
		b3PrintF(B3LOG_NORMAL,"\n");

		// Print custom area (longs)
		// Print strings at node end
		i += max_node;
	}
	
	b3Free(buffer);
	buffer = null;

	return true;
}

b3_world_error b3World::b3ReadInternal(const char *world_name)
{
	b3File         file;
	b3_u32         header[2];
	b3_world_error error = B3_WORLD_ERROR;

	b3Free();
	buffer = null;
	size   = 0;
	start  = null;

	// Read specified file into buffer
	if (file.b3Open(world_name,B_READ))
	{
		if (file.b3Read(header,sizeof(header)) == sizeof(header))
		{
			switch(header[0])
			{
			case B3_BLiZ:
				need_endian_change = false;
				break;

			case B3_ZiLB:
				need_endian_change = true;
				b3EndianSwap32(&header[0]);
				b3EndianSwap32(&header[1]);
				break;

			default:
				error = B3_WORLD_PARSE;
				break;
			}

			if (error != B3_WORLD_PARSE)
			{
				size   = header[1];
				buffer = (b3_u32 *)b3Alloc(size);
				if (buffer != null)
				{
					if (file.b3Read(buffer,size) == size)
					{
						error = B3_WORLD_OK;
					}
					else
					{
						error = B3_WORLD_READ;
						b3Free(buffer);
						buffer = null;
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
		file.b3Close();
	}
	else
	{
		error = B3_WORLD_OPEN;
	}

	return error;
}

b3_size b3World::b3Length()
{
	return 0;
}

void b3World::b3Dump()
{
	if (start != null)
	{
		b3PrintF(B3LOG_FULL,"--- World dump:\n");
		start->b3Dump(0);
		b3PrintF(B3LOG_FULL,"--- World dump complete.\n");
	}
}

b3Item *b3World::b3GetFirst()
{
	return start->b3GetFirst();
}
