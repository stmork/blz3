/*
**
**      $Filename:      b3Item.cc $
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

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/b3Config.h"
#include "blz3/base/b3World.h"
#include "b3ItemRegister.h"

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**      $Log$
**      Revision 1.25  2002/08/15 13:56:42  sm
**      - Introduced B3_THROW macro which supplies filename
**        and line number of source code.
**      - Fixed b3AllocTx when allocating a zero sized image.
**        This case is definitely an error!
**      - Added row refresh count into Lines
**
**      Revision 1.24  2002/08/09 13:20:18  sm
**      - b3Mem::b3Realloc was a mess! Now fixed to have the same
**        behaviour on all platforms. The Windows method ::GlobalReAlloc
**        seems to be broken:-(
**      - Introduced b3DirAbstract and b3PathAbstract classes
**
**      Revision 1.23  2002/02/17 21:25:06  sm
**      - Introduced CSG
**        o Heavily reorganized shape inheritance
**        o New file b3CSGShape added
**
**      Revision 1.22  2002/01/09 17:47:54  sm
**      - Finished CB3ImageButton implementation.
**      - Finished CDlgObjectCopy
**
**      Revision 1.21  2002/01/07 16:18:51  sm
**      - Added b3Item clone
**      - Added Drag & Drop
**
**      Revision 1.20  2002/01/01 13:50:21  sm
**      - Fixed some memory leaks:
**        o concerning triangle shape and derived spline shapes
**        o concerning image pool handling. Images with windows
**          path weren't found inside the image pool requesting
**          further image load.
**
**      Revision 1.19  2001/12/31 12:15:55  sm
**      - Fixed obsolete b3AnimElement handling
**
**      Revision 1.18  2001/12/30 22:52:35  sm
**      - Made b3Scene::b3SetCamera() compatible to earlier versions.
**
**      Revision 1.17  2001/12/30 18:24:35  sm
**      - Added missing b3AnimControl class
**      - Some minor bug fixes done:
**        o Missed some SetModifiedFlag()
**        o b3Scene::b3SetCamera() calls added which now puts the
**          selected camera in front of the b3Special list so that Lines III
**          select it when reloading.
**
**      Revision 1.16  2001/12/30 16:54:15  sm
**      - Inserted safe b3Write() into Lines III
**      - Fixed b3World saving: b3StoreXXX() methods must ensure
**        buffer before the buffer itself is used.
**      - Extended b3Shape format with shape activation flag. Nice: The
**        new data structures don't confuse the old Lines II/Blizzard II and
**        even stores these new values.
**
**      Revision 1.15  2001/12/30 14:16:57  sm
**      - Abstracted b3File to b3FileAbstract to implement b3FileMem (not done yet).
**      - b3Item writing implemented and updated all raytracing classes
**        to work properly.
**      - Cleaned up spline shapes and CSG shapes.
**      - Added b3Caustic class for compatibility reasons.
**
**      Revision 1.14  2001/12/02 17:38:17  sm
**      - Removing nasty CR/LF
**      - Added b3ExtractExt()
**      - Added stricmp() for Un*x
**      - Fixed some defines
**
**      Revision 1.13  2001/12/02 15:43:49  sm
**      - Creation/Deletion/Editing of lights
**      - Creation/Deletion of cameras
**      - New toolbars introduced.
**
**      Revision 1.12  2001/11/08 19:31:33  sm
**      - Nasty CR/LF removal!
**      - Added TGA/RGB8/PostScript image saving.
**      - Hoping to win Peter H. for powerful MFC programming...
**
**      Revision 1.11  2001/11/05 16:57:39  sm
**      - Creating demo scenes.
**      - Initializing some b3Item derived objects
**
**      Revision 1.10  2001/10/19 14:46:56  sm
**      - Rotation spline shape bug found.
**      - Major optimizations done.
**      - Cleanups
**
**      Revision 1.9  2001/09/30 15:53:19  sm
**      - Removing nasty CR/LF
**
**      Revision 1.8  2001/09/01 15:54:54  sm
**      - Tidy up Size confusion in b3Item/b3World and derived classes
**      - Made (de-)activation of objects
**
**      Revision 1.7  2001/08/14 07:03:28  sm
**      - Made some ASSERT cleanups. New define when _DEBUG is switched on:
**        B3_ASSERT(condition) abort()s when condition is false.
**
**      Revision 1.6  2001/08/11 16:29:07  sm
**      - Nasty UnCR done
**      - Compiling but not running OpenGL under Unix
**
**      Revision 1.5  2001/08/10 18:28:58  sm
**      - Some bug fixing
**      - Update functions per view inserted. Now with redraw when loading.
**
**      Revision 1.4  2001/08/08 20:12:59  sm
**      - Fixing some makefiles
**      - introducing check/BlzDump (BlzDump moved from tools)
**      - Some further line drawing added
**      - b3RenderContext and b3RenderObject introduced. Every b3Shape inherit from
**        b3RenderObject.
**
**      Revision 1.3  2001/08/05 19:53:43  sm
**      - Removing some nasty CR/LF
**
**      Revision 1.2  2001/08/05 19:51:56  sm
**      - Now having OpenGL software for Windows NT and created
**        new Lines III.
**
**      Revision 1.1  2001/08/05 12:46:06  sm
**      - Splitting b3ItemXXX routines
**
**
*/

/*************************************************************************
**                                                                      **
**                        b3Item: one node inside file (base class)     **
**                                                                      **
*************************************************************************/

void b3Item::b3Register(
	b3_item_init_func init_func,
	b3_item_load_func load_func,
	b3_u32            class_type,
	b3_bool           is_class)
{
	b3ItemRegisterEntry *entry;

	entry = new b3ItemRegisterEntry(init_func,load_func,class_type,is_class);
	if (entry == null)
	{
		B3_THROW(b3WorldException,B3_WORLD_MEMORY);
	}
	b3_item_register.b3Append(entry);
}

b3Item::b3Item() : b3Link<b3Item>(sizeof(b3Item))
{
	m_ItemSize   = 0;
	m_ItemOffset = 0;
	m_Buffer     = null;
	m_HeadCount  = 0;
	m_Heads      = null;
	m_ParseIndex = 0;

	m_StoreIndex  = 0;
	m_StoreOffset = 0;
	m_StoreBuffer = null;
	m_StoreSize   = 0;
}

b3Item::b3Item(
	b3_size class_size,
	b3_u32 class_type) :
		b3Link<b3Item>(class_size,class_type)
{
	m_ItemSize   = 0;
	m_ItemOffset = 0;
	m_Buffer     = null;
	m_HeadCount  = 0;
	m_Heads      = null;
	m_ParseIndex = 0;

	m_StoreIndex  = 0;
	m_StoreOffset = 0;
	m_StoreBuffer = null;
	m_StoreSize   = 0;
}

b3Item::b3Item(b3_u32 *src) :
	b3Link<b3Item>(
		src[B3_NODE_IDX_SIZE] + sizeof(b3Item),
		src[B3_NODE_IDX_CLASSTYPE])
{
	b3_index i,k;

	m_ItemSize   = src[B3_NODE_IDX_SIZE];
	m_ItemOffset = src[B3_NODE_IDX_OFFSET];
	m_Buffer     = (b3_u32 *)b3Alloc(m_ItemSize);
	m_HeadCount  = 0;
	memcpy (m_Buffer,src,m_ItemSize);

	// Count heads
	for (i = B3_NODE_IDX_FIRSTHEAD_CLASS;
	     m_Buffer[i + B3_HEAD_IDX_CLASS] != 0;
		 i += B3_HEAD_SIZE)
	{
		m_HeadCount++;
	}

	// Init subclasses' heads
	if (m_HeadCount > 0)
	{
		m_Heads = (b3Base<b3Item> *)b3Alloc(m_HeadCount * sizeof(b3Base<b3Item>));
		if (m_Heads != null)
		{
			k = B3_NODE_IDX_FIRSTHEAD_CLASS + B3_HEAD_IDX_CLASS;
			for (i = 0;i < m_HeadCount;i++)
			{
				m_Heads[i].b3InitBase(m_Buffer[k]);
				k += B3_HEAD_SIZE;
			}
		}
	}
	else
	{
		m_Heads = null;
	}
	b3Init();

	m_StoreIndex  = 0;
	m_StoreOffset = 0;
	m_StoreBuffer = null;
	m_StoreSize   = 0;
}

b3Item::~b3Item()
{
	b3_index  i;

	for (i = 0;i < m_HeadCount;i++)
	{
		m_Heads[i].b3Free();
	}
}

b3_bool b3Item::b3AllocHeads(b3_count new_head_count)
{
	b3_index  i;

	m_Heads = (b3Base<b3Item> *)b3Alloc(new_head_count * sizeof(b3Base<b3Item>));
	if (m_Heads != null)
	{
		m_HeadCount = new_head_count;
		for (i = 0;i < m_HeadCount;i++)
		{
			m_Heads[i].b3InitBase();
		}
	}
	return m_Heads != null;
}

void b3Item::b3Read()
{
}

void b3Item::b3Write()
{
	b3PrintF(B3LOG_NORMAL,"ERROR: b3Item::b3Write() not implemented:\n");
	b3PrintF(B3LOG_NORMAL,"       CLASS TYPE: %08x\n",b3GetClassType());
	b3PrintF(B3LOG_NORMAL,"       Size:       %8d\n",m_ItemSize);
	b3PrintF(B3LOG_NORMAL,"       Offset:     %8d\n",m_ItemOffset);

	B3_THROW(b3WorldException,B3_WORLD_STORAGE_NOT_IMPLEMENTED);
}

char *b3Item::b3GetName()
{
	return null;
}

void b3Item::b3DumpSpace(b3_count level,b3_log_level log_level)
{
	for (b3_count i = 0;i < level;i++)
	{
		b3PrintF(log_level,"  ");
	}
}

void b3Item::b3Dump(b3_count level)
{
	b3Item   *node;
	b3_count  i;

	b3DumpSpace(level);
	b3PrintF (B3LOG_NORMAL,"%08lx %7d\n",ClassType,Size);

	for (i = 0;i < m_HeadCount;i++)
	{
		b3DumpSpace(level);
		b3PrintF (B3LOG_NORMAL,"%08lx -------------\n",m_Heads[i].b3GetClass());
		B3_FOR_BASE(&m_Heads[i],node)
		{
			node->b3Dump(level + 1);
		}
	}
}

void b3Item::b3DumpSimple(b3_count level,b3_log_level log_level)
{
	b3_count  i;

	b3DumpSpace(level,log_level);
	b3PrintF (log_level,"%08lx %7d #",ClassType,Size);

	for (i = 0;i < m_HeadCount;i++)
	{
		b3PrintF (log_level,"  %08lx",m_Heads[i].b3GetClass());
	}
	b3PrintF (log_level,"\n");
}


/*************************************************************************
**                                                                      **
**                        Read initialization routines                  **
**                                                                      **
*************************************************************************/

void b3Item::b3Init()
{
	m_ParseIndex = B3_NODE_IDX_MIN + m_HeadCount * B3_HEAD_SIZE;
}

b3_s32 b3Item::b3InitInt()
{
	B3_ASSERT_INDEX;
	return m_Buffer[m_ParseIndex++];
}

b3_f32 b3Item::b3InitFloat()
{
	B3_ASSERT_INDEX;
	b3_f32 *ptr = (b3_f32 *)&m_Buffer[m_ParseIndex++];

	return *ptr;
}

b3_bool b3Item::b3InitBool()
{
	B3_ASSERT_INDEX;
	return m_Buffer[m_ParseIndex++] != 0;
}

void b3Item::b3InitVector(b3_vector *vec)
{
	B3_ASSERT_INDEX;
	if (vec != null)
	{
		b3_f32 *ptr = (b3_f32 *)&m_Buffer[m_ParseIndex];

		vec->x = *ptr++;
		vec->y = *ptr++;
		vec->z = *ptr++;
	}
	m_ParseIndex += 3;
}

void b3Item::b3InitVector4D(b3_vector4D *vec)
{
	B3_ASSERT_INDEX;
	if (vec != null)
	{
		b3_f32 *ptr = (b3_f32 *)&m_Buffer[m_ParseIndex];

		vec->x = *ptr++;
		vec->y = *ptr++;
		vec->z = *ptr++;
		vec->w = *ptr++;
	}
	m_ParseIndex += 4;
}

void b3Item::b3InitMatrix(b3_matrix *mat)
{
	b3_f32 *ptr = (b3_f32 *)&m_Buffer[m_ParseIndex];

	B3_ASSERT_INDEX;

	mat->m11 = *ptr++;
	mat->m12 = *ptr++;
	mat->m13 = *ptr++;
	mat->m14 = *ptr++;

	mat->m21 = *ptr++;
	mat->m22 = *ptr++;
	mat->m23 = *ptr++;
	mat->m24 = *ptr++;

	mat->m31 = *ptr++;
	mat->m32 = *ptr++;
	mat->m33 = *ptr++;
	mat->m34 = *ptr++;

	mat->m41 = *ptr++;
	mat->m42 = *ptr++;
	mat->m43 = *ptr++;
	mat->m44 = *ptr++;

	m_ParseIndex += 16;
}

void b3Item::b3InitSpline(
	b3_spline *spline,
	b3_vector *new_controls,
	b3_f32    *new_knots)
{
	B3_ASSERT_INDEX;

	spline->control_num = b3InitInt();
	spline->knot_num    = b3InitInt();
	spline->degree      = b3InitInt();
	spline->subdiv      = b3InitInt();
	spline->control_max = b3InitInt();
	spline->knot_max    = b3InitInt();
	spline->offset      = b3InitInt();
	spline->closed      = b3InitBool();
	spline->controls    = new_controls;
	spline->knots       = new_knots;

	m_ParseIndex += 2;
}

void b3Item::b3InitNurbs(
	b3_nurbs    *nurbs,
	b3_vector4D *new_controls,
	b3_f32      *new_knots)
{
	B3_ASSERT_INDEX;

	nurbs->control_num = b3InitInt();
	nurbs->knot_num    = b3InitInt();
	nurbs->degree      = b3InitInt();
	nurbs->subdiv      = b3InitInt();
	nurbs->control_max = b3InitInt();
	nurbs->knot_max    = b3InitInt();
	nurbs->offset      = b3InitInt();
	nurbs->closed      = b3InitBool();
	nurbs->controls    = new_controls;
	nurbs->knots       = new_knots;

	m_ParseIndex += 2;
}

void b3Item::b3InitColor(b3_color *col)
{
	b3_f32 *ptr = (b3_f32 *)&m_Buffer[m_ParseIndex];

	B3_ASSERT_INDEX;

	col->a = *ptr++;
	col->r = *ptr++;
	col->g = *ptr++;
	col->b = *ptr++;
	m_ParseIndex += 4;
}

void b3Item::b3InitString(char *name,b3_size len)
{
	b3_index pos = m_ParseIndex << 2;
	b3_size  new_size;

	B3_ASSERT_INDEX;

	new_size = pos + len > m_ItemSize ? m_ItemSize - pos : len;
	memcpy(name,&m_Buffer[m_ParseIndex],new_size);
	if (new_size < len)
	{
		name[new_size] = 0;
	}
	m_ParseIndex += ((len + 3) >> 2);
}

void *b3Item::b3InitNull()
{
	B3_ASSERT_INDEX;
	m_ParseIndex++;

	return null;
}

void b3Item::b3InitNOP()
{
	B3_ASSERT_INDEX;
	m_ParseIndex++;
}


/*************************************************************************
**                                                                      **
**                        Storage routines                              **
**                                                                      **
*************************************************************************/

b3_size b3Item::b3Store()
{
	b3Item   *item;
	b3_size   size = 0;
	b3_count  i;

	// Write b3Link data later
	m_StoreIndex  = B3_NODE_IDX_FIRSTHEAD_CLASS;
	m_StoreOffset = 0;

	// Allocate store buffer
	if (m_StoreBuffer == null)
	{
		if ((m_ItemSize >> 2) < (b3_size)(B3_NODE_IDX_FIRSTHEAD_CLASS + m_HeadCount * B3_HEAD_SIZE))
		{
			m_StoreSize = 8192;
		}
		else
		{
			m_StoreSize = m_ItemSize;
		}
		m_StoreBuffer = (b3_u32 *)b3Alloc(m_StoreSize);
		if (m_StoreBuffer == null)
		{
			B3_THROW(b3WorldException,B3_WORLD_MEMORY);
		}
	}

	// Prepare heads
	for (i = 0;i < m_HeadCount;i++)
	{
		b3StoreInt(m_Heads[i].b3GetClass());
		b3StorePtr(m_Heads[i].First);
		b3StorePtr(m_Heads[i].Last);
		B3_FOR_BASE(&m_Heads[i],item)
		{
			size += item->b3Store();
		}
	}

	// Head end marker
	b3StoreNull();

	// Store Data
	b3Write();

	// Prepare b3Link
	m_StoreBuffer[B3_NODE_IDX_SUCC]      = (b3_u32)Succ;
	m_StoreBuffer[B3_NODE_IDX_PREV]      = (b3_u32)Prev;
	m_StoreBuffer[B3_NODE_IDX_CLASSTYPE] = b3GetClassType();
	m_StoreBuffer[B3_NODE_IDX_SIZE]      = m_ItemSize   = m_StoreIndex  << 2;
	m_StoreBuffer[B3_NODE_IDX_OFFSET]    = m_ItemOffset = m_StoreOffset << 2;

	return size + m_ItemSize;
}

b3_world_error b3Item::b3StoreFile(b3FileAbstract *file)
{
	b3Item         *item;
	b3_world_error  error = B3_WORLD_WRITE;
	b3_index        i;

	if (m_StoreBuffer != null)
	{
		if (file->b3Write(m_StoreBuffer,m_ItemSize) == m_ItemSize)
		{
			for (i = 0;i < m_HeadCount;i++)
			{
				B3_FOR_BASE(&m_Heads[i],item)
				{
					error = item->b3StoreFile(file);
					if (error != B3_WORLD_OK)
					{
						b3Free(m_StoreBuffer);
						m_StoreBuffer = null;
						m_StoreSize   = 0;
						return error;
					}
				}
			}
			error = B3_WORLD_OK;
		}
	}
	else
	{
		error = B3_WORLD_MEMORY;
	}

	b3Free(m_StoreBuffer);
	m_StoreBuffer = null;
	m_StoreSize   = 0;
	return error;
}

void b3Item::b3EnsureStoreBuffer(b3_index needed,b3_bool is_data)
{
	// Clearify some things...
	if ((m_StoreOffset != 0) && (is_data))
	{
		B3_THROW(b3WorldException,B3_WORLD_OUT_OF_ORDER);
	}

	if ((m_StoreIndex + needed) > (b3_index)(m_StoreSize >> 2))
	{
		b3_size new_size = m_StoreSize + (needed << 2) + 16384;
#if 1
		b3_u32  *new_buffer;

		new_buffer = (b3_u32 *)b3Alloc(new_size);
		if (new_buffer != null)
		{
			memcpy (new_buffer,m_StoreBuffer,m_StoreIndex << 2);
			b3Free(m_StoreBuffer);
		}
		m_StoreBuffer = new_buffer;
#else
		m_StoreBuffer = (b3_u32 *)b3Realloc(m_StoreBuffer,new_size);
#endif
		if (m_StoreBuffer == null)
		{
			m_StoreSize   = 0;
			m_StoreIndex  = 0;
			m_StoreOffset = 0;
			B3_THROW(b3WorldException,B3_WORLD_MEMORY);
		}
		m_StoreSize = new_size;
	}
}

void b3Item::b3StoreInt(const b3_u32 value)
{
	b3EnsureStoreBuffer(1);

	m_StoreBuffer[m_StoreIndex++] = value;
}

void b3Item::b3StoreInt(const b3_s32 value)
{
	b3EnsureStoreBuffer(1);

	m_StoreBuffer[m_StoreIndex++] = (b3_u32)value;
}

void b3Item::b3StoreRes(const b3_res value)
{
	b3EnsureStoreBuffer(1);

	b3_res *ptr = (b3_res *)&m_StoreBuffer[m_StoreIndex++];
	*ptr = value;
}

void b3Item::b3StoreCount(const b3_count value)
{
	b3EnsureStoreBuffer(1);

	b3_count *ptr = (b3_count *)&m_StoreBuffer[m_StoreIndex++];
	*ptr = value;
}

void b3Item::b3StoreIndex(const b3_index value)
{
	b3EnsureStoreBuffer(1);

	b3_index *ptr = (b3_index *)&m_StoreBuffer[m_StoreIndex++];
	*ptr = value;
}

void b3Item::b3StoreFloat(const b3_f32 value)
{
	b3EnsureStoreBuffer(1);

	b3_f32 *ptr = (b3_f32 *)&m_StoreBuffer[m_StoreIndex++];
	*ptr = value;
}

void b3Item::b3StoreBool(const b3_bool value)
{
	b3EnsureStoreBuffer(1);

	m_StoreBuffer[m_StoreIndex++] = (b3_u32)value;
}

void b3Item::b3StorePtr(const void *ptr)
{
	b3EnsureStoreBuffer(1);

	m_StoreBuffer[m_StoreIndex++] = (b3_u32)ptr;
}

void b3Item::b3StoreVector(const b3_vector *vec)
{
	b3EnsureStoreBuffer(3);

	b3_f32 *ptr = (b3_f32 *)&m_StoreBuffer[m_StoreIndex];
	if (vec != null)
	{
		*ptr++ = vec->x;
		*ptr++ = vec->y;
		*ptr++ = vec->z;
	}
	else
	{
		*ptr++ = 0;
		*ptr++ = 0;
		*ptr++ = 0;
	}
	m_StoreIndex += 3;
}

void b3Item::b3StoreVector4D(const b3_vector4D *vec)
{
	b3EnsureStoreBuffer(4);

	b3_f32 *ptr = (b3_f32 *)&m_StoreBuffer[m_StoreIndex];
	if (vec != null)
	{
		*ptr++ = vec->x;
		*ptr++ = vec->y;
		*ptr++ = vec->z;
		*ptr++ = vec->w;
	}
	else
	{
		*ptr++ = 0;
		*ptr++ = 0;
		*ptr++ = 0;
		*ptr++ = 0;
	}
	m_StoreIndex += 4;
}

void b3Item::b3StoreMatrix(const b3_matrix *mat)
{
	b3EnsureStoreBuffer(16);

	b3_f32 *ptr = (b3_f32 *)&m_StoreBuffer[m_StoreIndex];

	*ptr++ = mat->m11;
	*ptr++ = mat->m12;
	*ptr++ = mat->m13;
	*ptr++ = mat->m14;

	*ptr++ = mat->m21;
	*ptr++ = mat->m22;
	*ptr++ = mat->m23;
	*ptr++ = mat->m24;

	*ptr++ = mat->m31;
	*ptr++ = mat->m32;
	*ptr++ = mat->m33;
	*ptr++ = mat->m34;

	*ptr++ = mat->m41;
	*ptr++ = mat->m42;
	*ptr++ = mat->m43;
	*ptr++ = mat->m44;

	m_StoreIndex += 16;
}

void b3Item::b3StoreSpline(const b3_spline *spline)
{
	b3StoreInt((b3_u32)spline->control_num);
	b3StoreInt((b3_u32)spline->knot_num);
	b3StoreInt((b3_u32)spline->degree);
	b3StoreInt((b3_u32)spline->subdiv);
	b3StoreInt((b3_u32)spline->control_max);
	b3StoreInt((b3_u32)spline->knot_max);
	b3StoreInt((b3_u32)spline->offset);
	b3StoreBool(spline->closed);
	b3StorePtr( spline->controls);
	b3StorePtr( spline->knots);
}

void b3Item::b3StoreNurbs(const b3_nurbs *nurbs)
{
	b3StoreInt((b3_u32)nurbs->control_num);
	b3StoreInt((b3_u32)nurbs->knot_num);
	b3StoreInt((b3_u32)nurbs->degree);
	b3StoreInt((b3_u32)nurbs->subdiv);
	b3StoreInt((b3_u32)nurbs->control_max);
	b3StoreInt((b3_u32)nurbs->knot_max);
	b3StoreInt((b3_u32)nurbs->offset);
	b3StoreBool(nurbs->closed);
	b3StorePtr( nurbs->controls);
	b3StorePtr( nurbs->knots);
}

void b3Item::b3StoreColor(const b3_color *col)
{
	b3EnsureStoreBuffer(4);

	b3_f32 *ptr = (b3_f32 *)&m_StoreBuffer[m_StoreIndex];
	*ptr++ = col->a;
	*ptr++ = col->r;
	*ptr++ = col->g;
	*ptr++ = col->b;

	m_StoreIndex += 4;
}

void b3Item::b3StoreString(const char *name,const b3_size len)
{
	b3EnsureStoreBuffer(len >> 2,false);

	if(m_StoreOffset == 0)
	{
		m_StoreOffset = m_StoreIndex;
	}
	memcpy(&m_StoreBuffer[m_StoreIndex],name,len);
	m_StoreIndex += (len >> 2);
}

void b3Item::b3StoreNull()
{
	b3EnsureStoreBuffer(1);

	m_StoreBuffer[m_StoreIndex++] = 0;
}

void b3Item::b3StoreNOP()
{
	b3EnsureStoreBuffer(1);

	m_StoreIndex++;
}
