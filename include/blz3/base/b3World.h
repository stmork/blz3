/*
**
**	$Filename:      b3World.h $
**	$Release:       Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:     Steffen A. Mork $
**
**	Blizzard III - world loading
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
**
**
*/

#ifndef B3_BASE_WORLD_H
#define B3_BASE_WORLD_H

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/b3Types.h"
#include "blz3/system/b3Exception.h"
#include "blz3/system/b3Log.h"
#include "blz3/system/b3Mem.h"
#include "blz3/system/b3FileAbstract.h"
#include "blz3/base/b3List.h"
#include "blz3/base/b3Spline.h"
#include "blz3/base/b3Nurbs.h"
#include "blz3/base/b3SearchPath.h"

/*************************************************************************
**                                                                      **
**                        Some head/node defines                        **
**                                                                      **
*************************************************************************/

#define no_DEBUG_VERBOSE

#define B3_BLiZ 'BLiZ'
#define B3_ZiLB 'ZiLB'

#define B3_CLASS_MAX 0x7fff0000

#define B3_PARSE_INDEX_VALID ((b3_size)(b3Item::m_ParseIndex << 2) < b3Item::m_ItemSize)
#define	B3_ASSERT_INDEX      B3_ASSERT(B3_PARSE_INDEX_VALID)

enum b3_node_indices
{
	B3_NODE_IDX_SUCC = 0,
	B3_NODE_IDX_PREV,
	B3_NODE_IDX_CLASSTYPE,
	B3_NODE_IDX_SIZE,
	B3_NODE_IDX_OFFSET,
	B3_NODE_IDX_FIRSTHEAD_CLASS,
	B3_NODE_IDX_MIN // Should be 6 (including end of head marker)!
};

enum b3_head_indices
{
	B3_HEAD_IDX_CLASS = 0,
	B3_HEAD_IDX_FIRST,
	B3_HEAD_IDX_LAST,
	B3_HEAD_SIZE // Should be 3
};

/*************************************************************************
**                                                                      **
**                        Exceptions concerning items                   **
**                                                                      **
*************************************************************************/

enum b3_world_error
{
	B3_WORLD_ERROR = -1,
	B3_WORLD_OK    =  0,
	B3_WORLD_OPEN,
	B3_WORLD_READ,
	B3_WORLD_MEMORY,
	B3_WORLD_PARSE,
	B3_WORLD_WRITE,
	B3_WORLD_STORAGE_NOT_IMPLEMENTED,
	B3_WORLD_OUT_OF_ORDER,
	B3_WORLD_CLASSTYPE_UNKNOWN
};

typedef b3Exception<b3_world_error,'BLZ'> b3WorldException; 

/*************************************************************************
**                                                                      **
**                        The b3World class                             **
**                                                                      **
*************************************************************************/

class b3Item;
class b3FirstItem;

class b3World : public b3Mem, public b3SearchPath
{
	b3_u32         *m_Buffer;
	b3_size         m_BufferSize;
	b3_bool         m_NeedEndianChange;
	b3FirstItem    *m_Start;

public:
	b3_bool         m_AutoDelete;

public:
	                b3World();
	                b3World(const char *world_name);
	virtual        ~b3World(); 

	static b3Item  *b3AllocNode(b3_u32 class_value);
	static b3Item  *b3AllocNode(b3_u32 *buffer);

	b3_bool         b3Read (const char *world_name);
	b3_bool         b3Write(const char *world_name);
	b3_world_error  b3Read (b3FileAbstract *file_handle);
	b3_world_error  b3Write(b3FileAbstract *file_handle);
	b3_bool         b3ReadDump(const char *world_name);
	b3_size         b3Length();
	void            b3Dump();
	b3Item         *b3RemoveFirst();
	b3Item         *b3GetFirst();
	void            b3SetFirst(b3Item *item);
	b3Base<b3Item> *b3GetHead(b3_u32 class_value = 0);

	static  b3Item *b3Clone(b3Item *original);

private:
	b3_world_error  b3EndianSwapWorld();
	b3_world_error  b3Parse();

	static void     b3CloneBase(b3Base<b3Item> *srcBase,b3Base<b3Item> *dstBase);
};

/*************************************************************************
**                                                                      **
**                        The b3Item class itself                       **
**                                                                      **
*************************************************************************/

typedef b3Item * (*b3_item_init_func)(b3_u32  class_type);
typedef b3Item * (*b3_item_load_func)(b3_u32 *src);

#define B3_ITEM_INIT(item_class)  item_class(b3_u32 class_type); static b3Item *b3StaticInit(b3_u32  class_type) { return new item_class(class_type); }
#define B3_ITEM_LOAD(item_class)  item_class(b3_u32 *src);       static b3Item *b3StaticInit(b3_u32 *src)        { return new item_class(src); }

class b3Item : public b3Link<b3Item>, public b3Mem
{
protected:
	b3_size         m_ItemSize;
	b3_size         m_ItemOffset;
	b3Base<b3Item> *m_Heads;
	b3_index        m_HeadCount;

	// Attributes for parsing
	b3_u32         *m_Buffer;
	b3_index        m_ParseIndex;

	// Attributes for writing
	b3_index        m_StoreIndex;
	b3_index        m_StoreOffset;
	b3_size         m_StoreSize;
	b3_u32         *m_StoreBuffer;

public:
	                        b3Item();
	                        b3Item(b3_size class_size,b3_u32 class_type);
	                        b3Item(b3_u32 *buffer);
	virtual                ~b3Item();
	static  void            b3Register(
		b3_item_init_func init_func,
		b3_item_load_func load_func,
		b3_u32            class_type,
		b3_bool           is_class = false);

	virtual void            b3Read();
	virtual void            b3Write();
	virtual char           *b3GetName();
	virtual void            b3Dump(b3_count level);
	        void            b3DumpSimple(b3_count level = 0,b3_log_level log_level = B3LOG_NORMAL);
			b3_bool         b3AllocHeads(b3_count head_count);

	        b3_world_error  b3ParseLinkuage(b3Item **array,b3_count node_count,b3_u32 class_limit,b3_count level = 0);
	        b3_size         b3Store();
	        b3_world_error  b3StoreFile(b3FileAbstract *file);

protected:
	// Parsing routines
	void     b3Init();
	b3_s32   b3InitInt();
	b3_f32   b3InitFloat();
	b3_bool  b3InitBool();
	void     b3InitVector  (b3_vector   *vec = null);
	void     b3InitVector4D(b3_vector4D *vec = null);
	void     b3InitMatrix  (b3_matrix   *mat);
	void     b3InitColor   (b3_color    *col);
	void     b3InitSpline  (b3_spline   *spline,b3_vector   *controls = null,b3_f32 *knots = null);
	void     b3InitNurbs   (b3_nurbs    *nurbs, b3_vector4D *controls = null,b3_f32 *knots = null);
	void     b3InitString  (char        *name,b3_size len);
	void    *b3InitNull();
	void     b3InitNOP();

	// Storing routines
	void     b3StoreInt     (const b3_s32       value);
	void     b3StoreInt     (const b3_u32       value);
	void     b3StoreIndex   (const b3_index     value);
	void     b3StoreRes     (const b3_res       value);
	void     b3StoreCount   (const b3_count     value);
	void     b3StoreFloat   (const b3_f32       value);
	void     b3StoreBool    (const b3_bool      value);
	void     b3StorePtr     (const void        *ptr);
	void     b3StoreVector  (const b3_vector   *vec = null);
	void     b3StoreVector4D(const b3_vector4D *vec = null);
	void     b3StoreMatrix  (const b3_matrix   *mat);
	void     b3StoreColor   (const b3_color    *col);
	void     b3StoreSpline  (const b3_spline   *spline);
	void     b3StoreNurbs   (const b3_nurbs    *nurbs);
	void     b3StoreString  (const char        *name,const b3_size len);
	void     b3StoreNull();
	void     b3StoreNOP();

protected:
	static void b3DumpSpace(b3_count level,b3_log_level log_level = B3LOG_NORMAL);

private:
	void     b3EnsureStoreBuffer(b3_index needed,b3_bool is_data=true);

	friend class b3World;
};

/*************************************************************************
**                                                                      **
**                        Entry point for item file                     **
**                                                                      **
*************************************************************************/

class b3FirstItem : public b3Item
{
public:
	B3_ITEM_INIT(b3FirstItem);
	B3_ITEM_LOAD(b3FirstItem);

	void    b3Write();

protected:
	b3Base<b3Item> *b3GetHead();
	void            b3InitBase(b3_u32 class_value = 0);

	friend class b3World;
};

#endif
