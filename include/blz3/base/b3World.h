/*
**
**      $Filename:      b3World.h $
**      $Release:       Dortmund 2001 $
**      $Revision$
**      $Date$
**      $Author$
**      $Developer:     Steffen A. Mork $
**
**      Blizzard III - world loading
**
**      (C) Copyright 2001  Steffen A. Mork
**          All Rights Reserved
**
**
**
**
*/

#ifndef B3_BASE_WORLD_H
#define B3_BASE_WORLD_H

#include "blz3/b3Types.h"
#include "blz3/base/b3List.h"
#include "blz3/base/b3Spline.h"
#include "blz3/base/b3Nurbs.h"
#include "blz3/system/b3Log.h"
#include "blz3/system/b3Mem.h"

#define B3_BLiZ 'BLiZ'
#define B3_ZiLB 'ZiLB'

#define B3_CLASS_MAX 0x7fff0000

typedef enum
{
	B3_NODE_IDX_SUCC = 0,
	B3_NODE_IDX_PREV,
	B3_NODE_IDX_CLASSTYPE,
	B3_NODE_IDX_SIZE,
	B3_NODE_IDX_OFFSET,
	B3_NODE_IDX_FIRSTHEAD_CLASS,
	B3_NODE_IDX_MIN // Should be 6 (including end of head marker)!
} b3_node_indices;

typedef enum
{
	B3_HEAD_IDX_CLASS = 0,
	B3_HEAD_IDX_FIRST,
	B3_HEAD_IDX_LAST,
	B3_HEAD_SIZE // Should be 3
} b3_head_indices;

typedef enum
{
	B3_WORLD_ERROR = -1,
	B3_WORLD_OK    =  0,
	B3_WORLD_OPEN,
	B3_WORLD_READ,
	B3_WORLD_MEMORY,
	B3_WORLD_PARSE
} b3_world_error;

class b3WorldException
{
protected:
	b3_world_error error;

public:
	b3WorldException(b3_world_error error = B3_WORLD_ERROR)
	{
		this->error = error;
	}

	b3_world_error b3GetError()
	{
		return error;
	}
};

class b3Item;

typedef b3Item * (*b3_item_init_func)(b3_u32  class_type);
typedef b3Item * (*b3_item_load_func)(b3_u32 *src);

class b3Item : public b3Link<b3Item>, public b3Mem
{
protected:
	b3_size         size;
	b3_u32         *buffer;
	b3Base<b3Item> *heads;
	b3_index        head_count;

	b3_index        parseIndex;
public:
	                        b3Item();
	                        b3Item(b3_size class_size,b3_u32 class_type);
	                        b3Item(b3_u32 *buffer);
	                       ~b3Item();
	static  void            b3Register(
		b3_item_init_func init_func,
		b3_item_load_func load_func,
		b3_u32            class_type,
		b3_bool           is_class = false);

	virtual void            b3Read();
	virtual void            b3Write();
	virtual void            b3Dump(b3_count level);
	        void            b3DumpSimple(b3_count level = 0,b3_log_level log_level = B3LOG_NORMAL);

	        b3_world_error  b3ParseLinkuage(b3Item **array,b3_count node_count,b3_u32 class_limit,b3_count level = 0);

protected:
	void     b3Init();
	b3_s32   b3InitInt();
	b3_f32   b3InitFloat();
	b3_bool  b3InitBool();
	void     b3InitVector(b3_vector *vec = null);
	void     b3InitVector4D(b3_vector4D *vec = null);
	void     b3InitMatrix(b3_matrix *mat);
	void     b3InitColor(b3_color *col);
	void     b3InitSpline(b3_spline *spline,b3_vector   *controls = null,b3_f32 *knots = null);
	void     b3InitNurbs (b3_nurbs  *nurbs, b3_vector4D *controls = null,b3_f32 *knots = null);
	void     b3InitString(char *name,b3_size len);
	void    *b3InitNull();
	void     b3InitNOP();

protected:
	static void b3DumpSpace(b3_count level,b3_log_level log_level = B3LOG_NORMAL);
};

#define B3_ITEM_INIT(item_class)  item_class(b3_u32 class_type); static b3Item *b3Init(b3_u32  class_type) { return new (item_class)(class_type); }
#define B3_ITEM_LOAD(item_class)  item_class(b3_u32 *src);       static b3Item *b3Init(b3_u32 *src)        { return new (item_class)(src); }

class b3FirstItem : public b3Item
{
public:
	B3_ITEM_INIT(b3FirstItem);
	B3_ITEM_LOAD(b3FirstItem);

	b3Item *b3GetFirst();
};

class b3World : public b3Mem
{
	b3_u32         *buffer;
	b3_size         size;
	b3_bool         need_endian_change;
	b3FirstItem    *start;

public:
	 b3World();
	~b3World(); 
	 b3World(const char *world_name);

	static b3Item  *b3AllocNode(b3_u32 class_value);
	static b3Item  *b3AllocNode(b3_u32 *buffer);

	b3_bool         b3Read(const char *world_name);
	b3_bool         b3ReadDump(const char *world_name);
	b3_size         b3Length();
	void            b3Dump();
	b3Item         *b3GetFirst();
private:
	void            b3EndianSwap32(b3_u32 *ptr);
	b3_world_error  b3EndianSwapWorld();
	b3_world_error  b3ReadInternal(const char *world_name);
	b3_world_error  b3Parse();
};

#endif
