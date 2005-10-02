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

#define not_VERBOSE

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/b3Config.h"
#include "blz3/base/b3World.h"
#include "blz3/base/b3Endian.h"
#include "blz3/system/b3File.h"
#include "b3ItemRegister.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.37  2005/10/02 09:51:12  sm
**      - Added OpenEXR configuration.
**      - Added more excpetion handling.
**
**      Revision 1.36  2005/08/08 14:46:31  smork
**      - Documentation.
**
**      Revision 1.35  2005/06/10 21:31:43  sm
**      - Vectorization fix for Windows
**      - Read binary from registry returns null when no
**        entry is in registry.
**
**      Revision 1.34  2005/01/04 15:13:59  smork
**      - Changed some data types.
**
**      Revision 1.33  2005/01/02 19:15:25  sm
**      - Fixed signed/unsigned warnings
**
**      Revision 1.32  2004/12/30 16:27:39  sm
**      - Removed assertion problem when starting Lines III: The
**        image list were initialized twice due to double calling
**        OnInitDialog() of CDialogBar. The CDialogBar::Create()
**        calls OnInitDialog() automatically sinde MFC 7
**      - Removed many global references from raytrace and base lib
**      - Fixed ticket no. 29: The b3RenderObject::b3Recompute
**        method checks the vertex maintainer against a null pointer.
**
**      Revision 1.31  2004/11/26 16:28:17  smork
**      - Equalizing VERBOSE print outs.
**      - pthread error messages only in _DEBUG version.
**
**      Revision 1.30  2003/07/20 09:21:18  sm
**      - Added item register dump.
**
**      Revision 1.29  2003/06/26 08:15:43  sm
**      - Cleaned up b3ItemRegister
**      - Added writing support for unregistered b3Item instances.
**
**      Revision 1.28  2002/08/23 11:35:23  sm
**      - Added motion blur raytracing. The image creation looks very
**        nice! The algorithm is not as efficient as it could be.
**
**      Revision 1.27  2002/08/15 13:56:43  sm
**      - Introduced B3_THROW macro which supplies filename
**        and line number of source code.
**      - Fixed b3AllocTx when allocating a zero sized image.
**        This case is definitely an error!
**      - Added row refresh count into Lines
**
**      Revision 1.26  2002/08/09 13:20:19  sm
**      - b3Mem::b3Realloc was a mess! Now fixed to have the same
**        behaviour on all platforms. The Windows method ::GlobalReAlloc
**        seems to be broken:-(
**      - Introduced b3DirAbstract and b3PathAbstract classes
**
**      Revision 1.25  2002/08/05 16:04:55  sm
**      - Found first texture init bug. This wasn't an OpenGL bug. This
**        couldn't be because every implementation had got the same
**        bug. The static aux image for creating textures wasn't initialized
**        at the right time.
**      - Version handling introduced: The version number is extracted
**        from the version resource now.
**      - The b3Tx::b3AllocTx() method uses b3Realloc() for better
**        memory usage.
**      - Some b3World messages removed.
**      - The 0x7fff class is registered into the b3ItemRegister now. This
**        prevents printing a warning when this class isn't found. Due to
**        the fact that *every* Blizzard data contains this class every
**        data read put out this warning.
**
**      Revision 1.24  2002/03/03 21:22:22  sm
**      - Added support for creating surfaces using profile curves.
**      - Added simple creating of triangle fields.
**
**      Revision 1.23  2002/01/09 17:47:54  sm
**      - Finished CB3ImageButton implementation.
**      - Finished CDlgObjectCopy
**
**      Revision 1.22  2002/01/08 16:04:08  sm
**      - New copy dialog added
**      - Merge with daily work
**
**      Revision 1.21  2002/01/07 16:18:51  sm
**      - Added b3Item clone
**      - Added Drag & Drop
**
**      Revision 1.20  2002/01/04 17:53:53  sm
**      - Added new/delete object.
**      - Added deactive rest of all scene objects.
**      - Changed icons to reflect object activation.
**      - Sub object insertion added.
**      - Fixed update routines to reflect correct state in hierarchy.
**      - Better hierarchy update coded.
**
**      Revision 1.19  2002/01/03 19:07:27  sm
**      - Cleaned up cut/paste
**
**      Revision 1.18  2002/01/03 15:50:14  sm
**      - Added cut/copy/paste
**
**      Revision 1.17  2001/12/30 14:16:57  sm
**      - Abstracted b3File to b3FileAbstract to implement b3FileMem (not done yet).
**      - b3Item writing implemented and updated all raytracing classes
**        to work properly.
**      - Cleaned up spline shapes and CSG shapes.
**      - Added b3Caustic class for compatibility reasons.
**
**      Revision 1.16  2001/11/12 16:50:29  sm
**      - Scene properties dialog coding
**
**      Revision 1.15  2001/11/05 16:57:39  sm
**      - Creating demo scenes.
**      - Initializing some b3Item derived objects
**
**      Revision 1.14  2001/10/29 19:34:02  sm
**      - Added new define B3_DELETE_BASE.
**      - Added support to abort raytrace processing.
**      - Added search path to world loading.
**      - Fixed super sampling.
**      - Fixed memory leak in raytracing row processing.
**
**      Revision 1.13  2001/10/11 16:06:33  sm
**      - Cleaning up b3BSpline with including isolated methods.
**      - Cleaning up endian conversion routines and collecting into
**        b3Endian
**      - Cleaning up some datatypes for proper display in Together.
**
**      Revision 1.12  2001/10/07 20:17:26  sm
**      - Prepared texture support.
**      - Noise procedures added.
**      - Added bump and material support.
**      - Added soft shadows.
**
**      Revision 1.11  2001/09/01 15:54:54  sm
**      - Tidy up Size confusion in b3Item/b3World and derived classes
**      - Made (de-)activation of objects
**
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
**                        b3World: which constructs whole               **
**                                 data structure                       **
**                                                                      **
*************************************************************************/

b3World::b3World()
{
	b3Free();
	m_Buffer     = null;
	m_BufferSize = 0;
	m_Start      = null;
	m_AutoDelete = true;
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

b3Item *b3World::b3AllocNode(b3_u32 class_value)
{
	b3ItemRegisterEntry *entry;

	entry = b3ItemRegister::b3Find(class_value);
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
	b3_u32               class_type = buffer[B3_NODE_IDX_CLASSTYPE];

	entry = b3ItemRegister::b3Find(class_type);
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

b3_world_error b3World::b3Parse()
{
	b3_world_error  result;
	b3_u32          i,max_file;
	b3_u32          max_node,max_offset;
	b3_u32          node_count = 0;
	b3Base<b3Item>  node_list;
	b3Item         *node;
	b3Item       **array;

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
			return B3_WORLD_PARSE;
		}

		// Create node
		node = b3AllocNode(&m_Buffer[i]);
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

#ifdef VERBOSE
	if (b3CheckLevel(B3LOG_FULL))
	{
		b3PrintF (B3LOG_FULL,"Counted %d nodes.\n",node_count);
		B3_FOR_BASE(&node_list,node)
		{
			node->b3DumpSimple();
		}
	}
#endif

	array = (b3Item **)b3Alloc(node_count * sizeof(b3Item *));
	if (array != null)
	{
#ifdef _DEBUG
		b3_count counted = node_list.b3GetCount();
		B3_ASSERT(counted == node_count);
#endif
		for (i = 0;i < node_count;i++)
		{
			array[i] = node_list.b3RemoveFirst();
		}
		m_Start = (b3FirstItem *)array[0];
		result  = m_Start->b3ParseLinkuage(array,node_count,0x7fff0000);
		b3Free(array);
	}
	else
	{
		result = B3_WORLD_MEMORY;
	}
	return result;
}

b3_bool b3World::b3Read(const char *name) throw(b3WorldException)
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
			error = b3Read(&file);
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

	b3Free(m_Buffer);
	m_Buffer = null;

	return true;
}

b3_world_error b3World::b3Read(b3FileAbstract *file)
{
	b3_u32         header[2];
	b3_world_error error = B3_WORLD_ERROR;

	b3Free();
	m_Buffer     = null;
	m_BufferSize = 0;
	m_Start      = null;

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
					error = b3Parse();
				}
				else
				{
					error = B3_WORLD_READ;
					b3Free(m_Buffer);
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

	b3Free(m_Buffer);
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
	b3Base<b3Item> *dstBase) throw(b3WorldException)
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
			dstBase->b3Append(dstItem);
			for(i = 0;i < srcItem->m_HeadCount;i++)
			{
				b3CloneBase(&srcItem->m_Heads[i],&dstItem->m_Heads[i]);
			}
			srcItem->b3Free(srcItem->m_StoreBuffer);
			srcItem->m_StoreBuffer = null;
			srcItem->m_StoreSize   = 0;
		}
		else
		{
			B3_THROW(b3WorldException,B3_WORLD_CLASSTYPE_UNKNOWN);
		}
	}
}

b3Item *b3World::b3Clone(b3Item *original) throw(b3WorldException)
{
	b3ItemRegisterEntry *entry;
	b3Item              *item;
	b3_u32               i;

	original->b3Store();
	entry = b3ItemRegister::b3Find(original->b3GetClassType());
	if (entry != null)
	{
		item = entry->b3Load(original->m_StoreBuffer);
		for(i = 0;i < original->m_HeadCount;i++)
		{
			b3CloneBase(&original->m_Heads[i],&item->m_Heads[i]);
		}
		original->b3Free(original->m_StoreBuffer);
		original->m_StoreBuffer = null;
		original->m_StoreSize   = 0;
	}
	else
	{
		B3_THROW(b3WorldException,B3_WORLD_CLASSTYPE_UNKNOWN);
	}
	return item;
}

b3_bool b3World::b3Write(const char *filename) throw(b3WorldException)
{
	b3File          file;
	b3_world_error  error;

	if(file.b3Open(filename,B_WRITE))
	{
		error = b3Write(&file);
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

b3_world_error b3World::b3Write(b3FileAbstract *file)
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

