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

#ifdef _DEBUG
#include <assert.h>

#define	ASSERT_INDEX	assert((parseIndex << 2) < (b3_index)size)
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
		throw new b3WorldException(B3_WORLD_MEMORY);
	}
	b3_item_register.b3Append(entry);
}

b3Item::b3Item() : b3Link<b3Item>(sizeof(b3Item))
{
	size       = 0;
	buffer     = null;
	head_count = 0;
	heads      = null;
}

b3Item::b3Item(
	b3_size class_size,
	b3_u32 class_type) :
		b3Link<b3Item>(class_size,class_type)
{
	size       = 0;
	buffer     = null;
	head_count = 0;
	heads      = null;
}

b3Item::b3Item(b3_u32 *src) :
	b3Link<b3Item>(
		src[B3_NODE_IDX_SIZE] + sizeof(b3Item),
		src[B3_NODE_IDX_CLASSTYPE])
{
	b3_index i,k;

	size       = src[B3_NODE_IDX_SIZE];
	buffer     = (b3_u32 *)b3Alloc(size);
	head_count = 0;
	memcpy (buffer,src,size);

	// Count heads
	for (i = B3_NODE_IDX_FIRSTHEAD_CLASS;buffer[i + B3_HEAD_IDX_CLASS] != 0;i += B3_HEAD_SIZE)
	{
		head_count++;
	}

	// Init subclasses' heads
	if (head_count > 0)
	{
		heads = (b3Base<b3Item> *)b3Alloc(head_count * sizeof(b3Base<b3Item>));
		if (heads != null)
		{
			k = B3_NODE_IDX_FIRSTHEAD_CLASS + B3_HEAD_IDX_CLASS;
			for (i = 0;i < head_count;i++)
			{
				heads[i].b3InitBase(buffer[k]);
				k += B3_HEAD_SIZE;
			}
		}
	}
	else
	{
		heads = null;
	}
	b3Init();
}

b3Item::~b3Item()
{
	b3_index  i;
	b3Item   *item;

	for (i = 0;i < head_count;i++)
	{
		while (item = heads[i].First)
		{
			heads[i].b3Remove(item);
			delete item;
		}
	}
}

void b3Item::b3Read()
{
}

void b3Item::b3Write()
{
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

	for (i = 0;i < head_count;i++)
	{
		b3DumpSpace(level);
		b3PrintF (B3LOG_NORMAL,"%08lx -------------\n",heads[i].b3GetClass());
		B3_FOR_BASE(&heads[i],node)
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

	for (i = 0;i < head_count;i++)
	{
		b3PrintF (log_level,"  %08lx",heads[i].b3GetClass());
	}
	b3PrintF (log_level,"\n");
}


void b3Item::b3Init()
{
	parseIndex = B3_NODE_IDX_MIN + head_count * B3_HEAD_SIZE;
}

b3_s32 b3Item::b3InitInt()
{
	ASSERT_INDEX;
	return buffer[parseIndex++];
}

b3_f32 b3Item::b3InitFloat()
{
	ASSERT_INDEX;
	b3_f32 *ptr = (b3_f32 *)&buffer[parseIndex++];

	return *ptr;
}

b3_bool b3Item::b3InitBool()
{
	ASSERT_INDEX;
	return buffer[parseIndex++] != 0;
}

void b3Item::b3InitVector(b3_vector *vec)
{
	ASSERT_INDEX;
	if (vec != null)
	{
		b3_f32 *ptr = (b3_f32 *)&buffer[parseIndex];

		vec->x = *ptr++;
		vec->y = *ptr++;
		vec->z = *ptr++;
	}
	parseIndex += 3;
}

void b3Item::b3InitVector4D(b3_vector4D *vec)
{
	ASSERT_INDEX;
	if (vec != null)
	{
		b3_f32 *ptr = (b3_f32 *)&buffer[parseIndex];

		vec->x = *ptr++;
		vec->y = *ptr++;
		vec->z = *ptr++;
		vec->w = *ptr++;
	}
	parseIndex += 4;
}

void b3Item::b3InitMatrix(b3_matrix *mat)
{
	b3_f32 *ptr = (b3_f32 *)&buffer[parseIndex];

	ASSERT_INDEX;

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

	parseIndex += 16;
}

void b3Item::b3InitSpline(
	b3_spline *spline,
	b3_vector *new_controls,
	b3_f32    *new_knots)
{
	ASSERT_INDEX;

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

	parseIndex += 2;
}

void b3Item::b3InitNurbs(
	b3_nurbs    *nurbs,
	b3_vector4D *new_controls,
	b3_f32      *new_knots)
{
	ASSERT_INDEX;

	nurbs->control_num = b3InitInt();
	nurbs->knot_num    = b3InitInt();
	nurbs->degree      = b3InitInt();
	nurbs->subdiv      = b3InitInt();
	nurbs->control_max = b3InitInt();
	nurbs->knot_max    = b3InitInt();
	nurbs->closed      = b3InitBool();
	nurbs->controls    = new_controls;
	nurbs->knots       = new_knots;

	parseIndex += 2;
}

void b3Item::b3InitColor(b3_color *col)
{
	b3_f32 *ptr = (b3_f32 *)&buffer[parseIndex];

	ASSERT_INDEX;

	col->a = *ptr++;
	col->r = *ptr++;
	col->g = *ptr++;
	col->b = *ptr++;
	parseIndex += 4;
}

void b3Item::b3InitString(char *name,b3_size len)
{
	ASSERT_INDEX;
	memcpy(name,&buffer[parseIndex],len);
	parseIndex += ((len + 3) >> 2);
}

void *b3Item::b3InitNull()
{
	ASSERT_INDEX;
	parseIndex++;

	return null;
}

void b3Item::b3InitNOP()
{
	ASSERT_INDEX;
	parseIndex++;
}


