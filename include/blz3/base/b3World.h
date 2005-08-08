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
#include "blz3/base/b3Color.h"
#include "blz3/base/b3List.h"
#include "blz3/base/b3Spline.h"
#include "blz3/base/b3Nurbs.h"
#include "blz3/base/b3SearchPath.h"
#include "blz3/base/b3Vector.h"

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
	B3_NODE_IDX_MIN // Should be 6 (including end of node marker)!
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
	B3_WORLD_CLASSTYPE_UNKNOWN,
	B3_WORLD_IMPORT
};

typedef b3Exception<b3_world_error,'BLZ'> b3WorldException; 

/*************************************************************************
**                                                                      **
**                        The b3World class                             **
**                                                                      **
*************************************************************************/

class b3Item;
class b3FirstItem;

/**
 * This class provides serialization methods for hierarchical organized
 * data elements.
 *
 * \see b3Item
 */
class B3_PLUGIN b3World : public b3Mem, public b3SearchPath
{
	b3_u32         *m_Buffer;
	b3_u32          m_BufferSize;
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
	b3_u32          b3Length();
	void            b3Dump();
	b3Item         *b3RemoveFirst();
	b3Item         *b3GetFirst();
	void            b3SetFirst(b3Item *item);
	b3Base<b3Item> *b3GetHead(b3_u32 class_value = 0);

	static b3Item  *b3Clone(b3Item *original);
	static void     b3CloneBase(b3Base<b3Item> *srcBase,b3Base<b3Item> *dstBase);

private:
	b3_world_error  b3EndianSwapWorld();
	b3_world_error  b3Parse();
};

/*************************************************************************
**                                                                      **
**                        The b3Item class itself                       **
**                                                                      **
*************************************************************************/

typedef b3Item * (*b3_item_init_func)(b3_u32  class_type);
typedef b3Item * (*b3_item_load_func)(b3_u32 *src);

#define B3_ITEM_BASE(item_class)  item_class(b3_size class_size,b3_u32 classtype);
#define B3_ITEM_INIT(item_class)  item_class(b3_u32 class_type); static b3Item *b3StaticInit(b3_u32  class_type) { return new item_class(class_type); }
#define B3_ITEM_LOAD(item_class)  item_class(b3_u32 *src);       static b3Item *b3StaticInit(b3_u32 *src)        { return new item_class(src); }

/**
 * This class provides serialization methods for one data item.
 */
class B3_PLUGIN b3Item : public b3Link<b3Item>, public b3Mem
{
protected:
	b3_u32          m_ItemSize;    //!< The stored size of this item in bytes.
	b3_s32          m_ItemOffset;  //!< The offset to the text area in this stored b3Item.
	b3Base<b3Item> *m_Heads;       //!< The list heads.
	b3_u32          m_HeadCount;   //!< The number of list heads.

	// Attributes for parsing
	b3_u32         *m_Buffer;      //!< This is a memory buffer of an archived b3Item.
	b3_u32          m_ParseIndex;  //!< This is an index in the memory buffer for parsing the b3Item.

	// Attributes for writing
	b3_u32          m_StoreIndex;  //!< The index to a 32 bit unsigned integer of the actual store position.
	b3_s32          m_StoreOffset; //!< The index to the text area as an index to a 32 bit wide unsigned integer.
	b3_u32          m_StoreSize;   //!< The number of 32 bit unsigned integers.
	b3_u32         *m_StoreBuffer; //!< A temporary store buffer.

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

	virtual void            b3Write();
	virtual char           *b3GetName();
	virtual b3_bool         b3Prepare();
	virtual void            b3Dump(b3_count level);
	        void            b3DumpSimple(b3_count level = 0,b3_log_level log_level = B3LOG_NORMAL);
			b3_bool         b3AllocHeads(b3_count head_count);

	        b3_u32          b3Store();
	        b3_world_error  b3StoreFile(b3FileAbstract *file);
			b3_u32          b3Checksum();

	static  b3_bool         b3IsClass(b3_u32 class_type,b3_u32 class_id);
	        b3_bool         b3IsClass(b3_u32 class_id);
protected:
	// Parsing routines
	/**
	 * This method initializes the indices for interpreting the b3Item data.
	 */
	void     b3Init();

	/**
	 * This method reads a signed integer value.
	 *
	 * @return The signed integer.
	 */
	b3_s32   b3InitInt();

	/**
	 * This methid reads an index.
	 *
	 * @return The index.
	 */
	b3_index b3InitIndex();

	/**
	 * This method reads a counter.
	 *
	 * @return The counter.
	 */
	b3_count b3InitCount();

	/**
	 * This method reads a single precision floating point value.
	 *
	 * @return The float value.
	 */
	b3_f32   b3InitFloat();

	/**
	 * This method reads a boolean value.
	 *
	 * @return The boolean value.
	 */
	b3_bool  b3InitBool();

	/**
	 * This method reads a three component vector into the given vector pointer.
	 *
	 * @param vec The vector buffer as pointer.
	 */
	void     b3InitVector  (b3_vector   *vec = null);

	/**
	 * This method reads a three component vector into the given vector reference.
	 *
	 * @param vec The vector buffer as reference.
	 */
	void     b3InitVector  (b3Vector32  &vec);

	/**
	 * This method reads a four component vector into the given vector pointer.
	 *
	 * @param vec The vector buffer as pointer.
	 */
	void     b3InitVector4D(b3_vector4D *vec = null);

	/**
	 * This method reads a four component vector into the given vector reference.
	 *
	 * @param vec The vector buffer as reference.
	 */
	void     b3InitVector4D(b3Vector32  &vec);

	/**
	 * This method reads a 4x4 matrix into the given matrix pointer.
	 *
	 * @param mat The matrix buffer as pointer.
	 */
	void     b3InitMatrix  (b3_matrix   *mat);

	/**
	 * This method reads a four component color value into the given b3_color pointer.
	 *
	 * @param col The color buffer as pointer.
	 */
	void     b3InitColor   (b3_color    *col);

	/**
	 * This method reads a four component color value into the given b3Color pointer.
	 *
	 * @param color The color instance as pointer.
	 */
	void     b3InitColor   (b3Color     &color);

	/**
	 * This method reads back a spline instance.
	 *
	 * @param spline The b3_spline structure pointer which defines the spline.
	 * @param controls The pointer to the spline control pointer.
	 * @param knots The knot vector of the spline.
	 */
	void     b3InitSpline  (b3_spline   *spline,b3_vector   *controls = null,b3_f32 *knots = null);

	/**
	 * This method reads back a NURBS instance.
	 *
	 * @param spline The b3_nurbs structure pointer which defines the NURBS.
	 * @param controls The pointer to the NURBS control pointer.
	 * @param knots The knot vector of the NURBS.
	 */
	void     b3InitNurbs   (b3_nurbs    *nurbs, b3_vector4D *controls = null,b3_f32 *knots = null);

	/**
	 * This method reads back a string of the given size and corrects the read index.
	 *
	 * @param name The text buffer.
	 * @param len  The size of the text buffer.
	 */
	void     b3InitString  (char        *name,b3_size len);

	/**
	 * This method bumps the read index and returns a null pointer.
	 *
	 * @return Null pointer.
	 */
	void    *b3InitNull();
	
	/**
	 * This method simply bumps the read index.
	 */
	void     b3InitNOP();


	////////////////////////////////////////// Storing routines
	/**
	 * This method stores one signed integer value.
	 *
	 * @param value The signed integer to store.
	 */
	void     b3StoreInt     (const b3_s32       value);

	/**
	 * This method stores one unsigned integer value.
	 *
	 * @param value The unsigned integer to store.
	 */
	void     b3StoreInt     (const b3_u32       value);

	/**
	 * This method stores one index value.
	 *
	 * @param value The index to store.
	 */
	void     b3StoreIndex   (const b3_index     value);

	/**
	 * This method stores one resolution value.
	 *
	 * @param value The resolution value to store.
	 */
	void     b3StoreRes     (const b3_res       value);

	/**
	 * This method stores one counter value.
	 *
	 * @param value The counter to store.
	 */
	void     b3StoreCount   (const b3_count     value);

	/**
	 * This method stores one single precision floating point value.
	 *
	 * @param value The float value to store.
	 */
	void     b3StoreFloat   (const b3_f32       value);
	/**
	 * This method stores one boolean value. Note that this value
	 * is 32 bits wide!
	 *
	 * @param value The boolean value to store.
	 */
	void     b3StoreBool    (const b3_bool      value);

	/**
	 * This method stores one pointer place holder. Note that this
	 * value is a 32 bit value even on 64 bit machines! This value
	 * 
	 */
	void     b3StorePtr     (const void        *ptr);
	void     b3StoreVector  (const b3_vector   *vec = null);
	void     b3StoreVector  (      b3Vector32  &vec);
	void     b3StoreVector4D(const b3_vector4D *vec = null);
	void     b3StoreVector4D(      b3Vector32  &vec);
	void     b3StoreMatrix  (const b3_matrix   *mat);
	void     b3StoreColor   (const b3_color    *col);
	void     b3StoreColor   (      b3Color     &col);
	void     b3StoreSpline  (const b3_spline   *spline);
	void     b3StoreNurbs   (const b3_nurbs    *nurbs);
	void     b3StoreString  (const char        *name,const b3_size len);
	void     b3StoreNull();
	void     b3StoreNOP();

protected:
	static void b3DumpSpace(b3_count level,b3_log_level log_level = B3LOG_NORMAL);

private:
	void            b3EnsureStoreBuffer(b3_u32 needed,b3_bool is_data=true);
	b3_world_error  b3ParseLinkuage(b3Item **array,b3_u32 node_count,b3_u32 class_limit,b3_count level = 0);

	friend class b3World;
};

/*************************************************************************
**                                                                      **
**                        Entry point for item file                     **
**                                                                      **
*************************************************************************/

/**
 * This class is a special root class for data item serialization.
 */
class B3_PLUGIN b3FirstItem : public b3Item
{
public:
	/**
	 * This constructor initializes this instance with default values.
	 */
	B3_ITEM_INIT(b3FirstItem);

	/**
	 * This constructor initializes this instance from a memory buffer.
	 */
	B3_ITEM_LOAD(b3FirstItem);

	/**
	 * This method write recursively the content of this instance.
	 */
	void    b3Write();

protected:
	/**
	 * This method returns the list head of the content.
	 *
	 * @return The content list header.
	 */
	b3Base<b3Item> *b3GetHead();

	/**
	 * This method initializes the internal list head with the type
	 * of use.
	 *
	 * @param classtype The items class type for the root element.
	 */
	void            b3InitBase(b3_u32 classtype = 0);

	friend class b3World;
};

#endif
