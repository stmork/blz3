/*
**
**	$Filename:	b3Item.cc $
**	$Release:	Dortmund 2001, 2016 $
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

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "b3BaseInclude.h"
#include "blz3/system/b3Locale.h"
#include "blz3/base/b3World.h"
#include "b3ItemRegister.h"

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
	b3ItemRegisterEntry * entry;

	entry = new b3ItemRegisterEntry(init_func, load_func, class_type, is_class);
	if(entry == null)
	{
		B3_THROW(b3WorldException, B3_WORLD_MEMORY);
	}
	b3ItemRegister::b3Append(entry);
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
	b3Link<b3Item>(class_size, class_type)
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

b3Item::b3Item(b3_u32 * src) :
	b3Link<b3Item>(
		src[B3_NODE_IDX_SIZE] + sizeof(b3Item),
		src[B3_NODE_IDX_CLASSTYPE])
{
	b3_u32 i, k;

	m_ItemSize   = src[B3_NODE_IDX_SIZE];
	m_ItemOffset = src[B3_NODE_IDX_OFFSET];
	m_Buffer     = (b3_u32 *)b3Alloc(m_ItemSize);
	m_HeadCount  = 0;
	memcpy(m_Buffer, src, m_ItemSize);

	// Count heads
	for(i = B3_NODE_IDX_FIRSTHEAD_CLASS;
		m_Buffer[i + B3_HEAD_IDX_CLASS] != 0;
		i += B3_HEAD_SIZE)
	{
		m_HeadCount++;
	}

	// Init subclasses' heads
	if(m_HeadCount > 0)
	{
		m_Heads = (b3Base<b3Item> *)b3Alloc(m_HeadCount * sizeof(b3Base<b3Item>));
		if(m_Heads != null)
		{
			k = B3_NODE_IDX_FIRSTHEAD_CLASS + B3_HEAD_IDX_CLASS;
			for(i = 0; i < m_HeadCount; i++)
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
	b3_u32  i;

	for(i = 0; i < m_HeadCount; i++)
	{
		m_Heads[i].b3Free();
	}
}

const b3_bool b3Item::b3IsClass(const b3_u32 class_type, const b3_u32 class_id)
{
	return (class_type & B3_CLASS_MASK) == class_id;
}

const b3_bool b3Item::b3IsClass(const b3_u32 class_id) const
{
	return b3IsClass(b3GetClassType(), class_id);
}

b3_bool b3Item::b3AllocHeads(b3_count new_head_count)
{
	b3_u32  i;

	m_Heads = (b3Base<b3Item> *)b3Alloc(new_head_count * sizeof(b3Base<b3Item>));
	if(m_Heads != null)
	{
		m_HeadCount = new_head_count;
		for(i = 0; i < m_HeadCount; i++)
		{
			m_Heads[i].b3InitBase();
		}
	}
	return m_Heads != null;
}

const char * b3Item::b3GetName() const
{
	return null;
}

b3_bool b3Item::b3Prepare(b3_preparation_info * prep_info)
{
	return true;
}

void b3Item::b3DumpSpace(const b3_count level, const b3_log_level log_level)
{
	for(b3_count i = 0; i < level; i++)
	{
		b3PrintF(log_level, "  ");
	}
}

void b3Item::b3Dump(b3_count level) const
{
	b3Item * node;
	b3_u32  i;

	b3DumpSpace(level);
	b3PrintF(B3LOG_NORMAL, "%08lx %7d\n", ClassType, Size);

	for(i = 0; i < m_HeadCount; i++)
	{
		b3DumpSpace(level);
		b3PrintF(B3LOG_NORMAL, "%08lx -------------\n", m_Heads[i].b3GetClass());
		B3_FOR_BASE(&m_Heads[i], node)
		{
			node->b3Dump(level + 1);
		}
	}
}

void b3Item::b3DumpSimple(b3_count level, b3_log_level log_level) const
{
	b3_u32  i, max = (b3_count)(m_ItemSize >> 2);

	b3DumpSpace(level, log_level);
	b3PrintF(log_level, "%08lx %7d # %7d %7d # %7d", ClassType, Size, m_ItemSize, m_ItemOffset, m_ParseIndex);

	for(i = 0; i < m_HeadCount; i++)
	{
		b3PrintF(log_level, "  %08lx", m_Heads[i].b3GetClass());
	}
	b3PrintF(log_level, " #");

	for(i = 0; i < max; i++)
	{
		b3PrintF(log_level, "  %08lx", m_Buffer[i]);
	}
	b3PrintF(log_level, "\n");
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

b3_index b3Item::b3InitIndex()
{
	B3_ASSERT_INDEX;
	return m_Buffer[m_ParseIndex++];
}

b3_count b3Item::b3InitCount()
{
	B3_ASSERT_INDEX;
	return m_Buffer[m_ParseIndex++];
}

b3_f32 b3Item::b3InitFloat()
{
	B3_ASSERT_INDEX;
	b3_f32 * ptr = (b3_f32 *)&m_Buffer[m_ParseIndex++];

	return *ptr;
}

b3_bool b3Item::b3InitBool()
{
	B3_ASSERT_INDEX;
	return m_Buffer[m_ParseIndex++] != 0;
}

void b3Item::b3InitVector(b3_vector * vec)
{
	B3_ASSERT_INDEX;
	if(vec != null)
	{
		b3_f32 * ptr = (b3_f32 *)&m_Buffer[m_ParseIndex];

		vec->x   = *ptr++;
		vec->y   = *ptr++;
		vec->z   = *ptr++;
	}
	m_ParseIndex += 3;
}

void b3Item::b3InitVector(b3Vector32 & vec)
{
	B3_ASSERT_INDEX;

	b3_f32 * ptr = (b3_f32 *)&m_Buffer[m_ParseIndex];

	vec.b3Init(ptr[0], ptr[1], ptr[2]);
	m_ParseIndex += 3;
}

void b3Item::b3InitVector4D(b3_vector4D * vec)
{
	B3_ASSERT_INDEX;
	if(vec != null)
	{
		b3_f32 * ptr = (b3_f32 *)&m_Buffer[m_ParseIndex];

		vec->x = *ptr++;
		vec->y = *ptr++;
		vec->z = *ptr++;
		vec->w = *ptr++;
	}
	m_ParseIndex += 4;
}

void b3Item::b3InitVector4D(b3Vector32 & vec)
{
	B3_ASSERT_INDEX;

	b3_f32 * ptr = (b3_f32 *)&m_Buffer[m_ParseIndex];

	vec.b3Init(ptr[0], ptr[1], ptr[2], ptr[3]);
	m_ParseIndex += 3;
}

void b3Item::b3InitMatrix(b3_matrix * mat)
{
	b3_f32 * ptr = (b3_f32 *)&m_Buffer[m_ParseIndex];

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
	b3Spline * spline,
	b3_vector * new_controls,
	b3_f32  *  new_knots)
{
	B3_ASSERT_INDEX;

	spline->m_ControlNum = b3InitInt();
	spline->m_KnotNum    = b3InitInt();
	spline->m_Degree     = b3InitInt();
	spline->m_SubDiv     = b3InitInt();
	spline->m_ControlMax = b3InitInt();
	spline->m_KnotMax    = b3InitInt();
	spline->m_Offset     = b3InitInt();
	spline->m_Closed     = b3InitBool();
	spline->m_Controls   = new_controls;
	spline->m_Knots      = new_knots;

	m_ParseIndex += 2;
}

void b3Item::b3InitNurbs(
	b3Nurbs   *  nurbs,
	b3_vector4D * new_controls,
	b3_f32   *   new_knots)
{
	B3_ASSERT_INDEX;

	nurbs->m_ControlNum = b3InitInt();
	nurbs->m_KnotNum    = b3InitInt();
	nurbs->m_Degree     = b3InitInt();
	nurbs->m_SubDiv     = b3InitInt();
	nurbs->m_ControlMax = b3InitInt();
	nurbs->m_KnotMax    = b3InitInt();
	nurbs->m_Offset     = b3InitInt();
	nurbs->m_Closed     = b3InitBool();
	nurbs->m_Controls   = new_controls;
	nurbs->m_Knots      = new_knots;

	m_ParseIndex += 2;
}

void b3Item::b3InitColor(b3_color * col)
{
	b3_f32 * ptr = (b3_f32 *)&m_Buffer[m_ParseIndex];

	B3_ASSERT_INDEX;

	col->a = *ptr++;
	col->r = *ptr++;
	col->g = *ptr++;
	col->b = *ptr++;
	m_ParseIndex += 4;
}

void b3Item::b3InitColor(b3Color & col)
{
	b3_color  aux;

	b3InitColor(&aux);
	col = b3Color(aux);
}

void b3Item::b3InitString(char * name, b3_size len)
{
	b3_u32  pos = m_ParseIndex << 2;
	b3_u32  new_size;

	B3_ASSERT_INDEX;

	new_size = pos + len > m_ItemSize ? m_ItemSize - pos : len;
	b3Locale::b3IsoToLocale((const char *)&m_Buffer[m_ParseIndex], name, new_size);
	if(new_size < len)
	{
		name[new_size] = 0;
	}
	m_ParseIndex += ((b3_u32)(len + 3) >> 2);
}

void * b3Item::b3InitNull()
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

b3_u32 b3Item::b3Store()
{
	b3Item * item;
	b3_u32   size = 0;
	b3_u32   i;

	// Write b3Link data later
	m_StoreIndex  = B3_NODE_IDX_FIRSTHEAD_CLASS;
	m_StoreOffset = 0;

	// Allocate store buffer
	if(m_StoreBuffer == null)
	{
		if((m_ItemSize >> 2) < (b3_size)(B3_NODE_IDX_FIRSTHEAD_CLASS + m_HeadCount * B3_HEAD_SIZE))
		{
			m_StoreSize = 8192;
		}
		else
		{
			m_StoreSize = m_ItemSize;
		}
		m_StoreBuffer = (b3_u32 *)b3Alloc(m_StoreSize);
		if(m_StoreBuffer == null)
		{
			B3_THROW(b3WorldException, B3_WORLD_MEMORY);
		}
	}

	// Prepare heads
	for(i = 0; i < m_HeadCount; i++)
	{
		b3StoreInt(m_Heads[i].b3GetClass());
		b3StorePtr(m_Heads[i].First);
		b3StorePtr(m_Heads[i].Last);
		B3_FOR_BASE(&m_Heads[i], item)
		{
			size += item->b3Store();
		}
	}

	// Head end marker
	b3StoreNull();

	// Store Data
	b3Write();

	// Prepare b3Link
	m_StoreBuffer[B3_NODE_IDX_SUCC]      = Succ != null ? 0xabadcafe : null;
	m_StoreBuffer[B3_NODE_IDX_PREV]      = Prev != null ? 0xdeadbeef : null;
	m_StoreBuffer[B3_NODE_IDX_CLASSTYPE] = b3GetClassType();
	m_StoreBuffer[B3_NODE_IDX_SIZE]      = m_ItemSize   = m_StoreIndex  << 2;
	m_StoreBuffer[B3_NODE_IDX_OFFSET]    = m_ItemOffset = m_StoreOffset << 2;

	return size + m_ItemSize;
}

void b3Item::b3Write()
{
	if(m_ItemOffset == 0)
	{
		m_ItemOffset = m_ItemSize;
	}

	b3EnsureStoreBuffer((m_ItemOffset >> 2) - m_StoreIndex);
	b3EnsureStoreBuffer(((m_ItemSize - m_ItemOffset) >> 2) - m_StoreIndex, false);

	memcpy(&m_StoreBuffer[m_StoreIndex], &m_Buffer[m_StoreIndex], m_ItemSize - (m_StoreIndex << 2));
	m_StoreIndex  = m_ItemSize >> 2;
	m_StoreOffset = m_ItemOffset >> 2;

	b3PrintF(B3LOG_DEBUG, "WARN: b3Item::b3Write() not implemented:\n");
	b3PrintF(B3LOG_DEBUG, "      CLASS TYPE: %08x\n", b3GetClassType());
	b3PrintF(B3LOG_DEBUG, "      Size:       %8d\n", m_ItemSize);
	b3PrintF(B3LOG_DEBUG, "      Offset:     %8d\n", m_ItemOffset);
}

b3_world_error b3Item::b3StoreFile(b3FileAbstract * file)
{
	b3Item     *    item;
	b3_world_error  error = B3_WORLD_WRITE;
	b3_u32          i;

	if(m_StoreBuffer != null)
	{
		if(file->b3Write(m_StoreBuffer, m_ItemSize) == m_ItemSize)
		{
			for(i = 0; i < m_HeadCount; i++)
			{
				B3_FOR_BASE(&m_Heads[i], item)
				{
					error = item->b3StoreFile(file);
					if(error != B3_WORLD_OK)
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

void b3Item::b3EnsureStoreBuffer(b3_u32 needed, b3_bool is_data)
{
	// Clearify some things...
	if((m_StoreOffset != 0) && (is_data))
	{
		B3_THROW(b3WorldException, B3_WORLD_OUT_OF_ORDER);
	}

	if((m_StoreIndex + needed) > (m_StoreSize >> 2))
	{
		b3_u32 new_size = m_StoreSize + (needed << 2) + 16384;
#if 1
		b3_u32 * new_buffer;

		new_buffer = (b3_u32 *)b3Alloc(new_size);
		if(new_buffer != null)
		{
			memcpy(new_buffer, m_StoreBuffer, m_StoreIndex << 2);
			b3Free(m_StoreBuffer);
		}
		m_StoreBuffer = new_buffer;
#else
		m_StoreBuffer = (b3_u32 *)b3Realloc(m_StoreBuffer, new_size);
#endif
		if(m_StoreBuffer == null)
		{
			m_StoreSize   = 0;
			m_StoreIndex  = 0;
			m_StoreOffset = 0;
			B3_THROW(b3WorldException, B3_WORLD_MEMORY);
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

	b3_res * ptr = (b3_res *)&m_StoreBuffer[m_StoreIndex++];
	*ptr = value;
}

void b3Item::b3StoreCount(const b3_count value)
{
	b3EnsureStoreBuffer(1);

	b3_s32 * ptr = (b3_s32 *)&m_StoreBuffer[m_StoreIndex++];
	*ptr = (b3_s32)value;
}

void b3Item::b3StoreIndex(const b3_index value)
{
	b3EnsureStoreBuffer(1);

	b3_index * ptr = (b3_index *)&m_StoreBuffer[m_StoreIndex++];
	*ptr = value;
}

void b3Item::b3StoreFloat(const b3_f32 value)
{
	b3EnsureStoreBuffer(1);

	b3_f32 * ptr = (b3_f32 *)&m_StoreBuffer[m_StoreIndex++];
	*ptr = value;
}

void b3Item::b3StoreBool(const b3_bool value)
{
	b3EnsureStoreBuffer(1);

	m_StoreBuffer[m_StoreIndex++] = (b3_u32)value;
}

void b3Item::b3StorePtr(const void * ptr)
{
	b3EnsureStoreBuffer(1);

	m_StoreBuffer[m_StoreIndex++] = ptr != null ? 0xbadc0ded : null;
}

void b3Item::b3StoreVector(const b3_vector * vec)
{
	b3EnsureStoreBuffer(3);

	b3_f32 * ptr = (b3_f32 *)&m_StoreBuffer[m_StoreIndex];
	if(vec != null)
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

void b3Item::b3StoreVector(b3Vector32 & vec)
{
	b3EnsureStoreBuffer(3);

	b3_f32 * ptr = (b3_f32 *)&m_StoreBuffer[m_StoreIndex];

	*ptr++ = vec[X];
	*ptr++ = vec[Y];
	*ptr++ = vec[Z];

	m_StoreIndex += 3;
}

void b3Item::b3StoreVector4D(const b3_vector4D * vec)
{
	b3EnsureStoreBuffer(4);

	b3_f32 * ptr = (b3_f32 *)&m_StoreBuffer[m_StoreIndex];
	if(vec != null)
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

void b3Item::b3StoreVector4D(b3Vector32 & vec)
{
	b3EnsureStoreBuffer(4);

	b3_f32 * ptr = (b3_f32 *)&m_StoreBuffer[m_StoreIndex];

	*ptr++ = vec[X];
	*ptr++ = vec[Y];
	*ptr++ = vec[Z];
	*ptr++ = vec[W];

	m_StoreIndex += 4;
}

void b3Item::b3StoreMatrix(const b3_matrix * mat)
{
	b3EnsureStoreBuffer(16);

	b3_f32 * ptr = (b3_f32 *)&m_StoreBuffer[m_StoreIndex];

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

void b3Item::b3StoreSpline(const b3Spline * spline)
{
	b3StoreInt((b3_u32)spline->m_ControlNum);
	b3StoreInt((b3_u32)spline->m_KnotNum);
	b3StoreInt((b3_u32)spline->m_Degree);
	b3StoreInt((b3_u32)spline->m_SubDiv);
	b3StoreInt((b3_u32)spline->m_ControlMax);
	b3StoreInt((b3_u32)spline->m_KnotMax);
	b3StoreInt((b3_u32)spline->m_Offset);
	b3StoreBool(spline->m_Closed);
	b3StorePtr(spline->m_Controls);
	b3StorePtr(spline->m_Knots);
}

void b3Item::b3StoreNurbs(const b3Nurbs * nurbs)
{
	b3StoreInt((b3_u32)nurbs->m_ControlNum);
	b3StoreInt((b3_u32)nurbs->m_KnotNum);
	b3StoreInt((b3_u32)nurbs->m_Degree);
	b3StoreInt((b3_u32)nurbs->m_SubDiv);
	b3StoreInt((b3_u32)nurbs->m_ControlMax);
	b3StoreInt((b3_u32)nurbs->m_KnotMax);
	b3StoreInt((b3_u32)nurbs->m_Offset);
	b3StoreBool(nurbs->m_Closed);
	b3StorePtr(nurbs->m_Controls);
	b3StorePtr(nurbs->m_Knots);
}

void b3Item::b3StoreColor(const b3_color * col)
{
	b3EnsureStoreBuffer(4);

	b3_f32 * ptr = (b3_f32 *)&m_StoreBuffer[m_StoreIndex];
	*ptr++ = col->a;
	*ptr++ = col->r;
	*ptr++ = col->g;
	*ptr++ = col->b;

	m_StoreIndex += 4;
}

void b3Item::b3StoreColor(b3Color & col)
{
	b3_color aux = col;

	b3StoreColor(&aux);
}

void b3Item::b3StoreString(const char * name, const b3_size len)
{
	b3EnsureStoreBuffer((b3_u32)len >> 2, false);

	if(m_StoreOffset == 0)
	{
		m_StoreOffset = m_StoreIndex;
	}
	b3Locale::b3LocaleToIso(name, (char *)&m_StoreBuffer[m_StoreIndex], len);
	m_StoreIndex += ((b3_u32)len >> 2);
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

const b3_u32 b3Item::b3Checksum()
{
	b3_u32 result = 0;

	b3Store();

	for(b3_u32 i = 0; i < m_StoreIndex; i++)
	{
		result += m_StoreBuffer[i];
	}
	return result;
}
