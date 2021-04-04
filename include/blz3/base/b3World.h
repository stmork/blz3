/*
**
**	$Filename:	b3World.h $
**	$Release:	Dortmund 2001, 2016 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
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

#pragma once

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
#include "blz3/base/b3SearchPath.h"
#include "blz3/base/b3Vector.h"

/*************************************************************************
**                                                                      **
**                        Some head/node defines                        **
**                                                                      **
*************************************************************************/

#define no_DEBUG_VERBOSE

#define B3_BLiZ 0x424c695a
#define B3_ZiLB 0x5a694c42

#define B3_CLASS_MAX 0x7fff0000

#define B3_PARSE_INDEX_VALID ((b3_size)(b3Item::m_ParseIndex << 2) < b3Item::m_ItemSize)
#define	B3_ASSERT_INDEX      B3_ASSERT(B3_PARSE_INDEX_VALID)

/**
 * This enumeration lists the b3Item indices for 32 bit unsigned integer reference.
 */
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

/**
 * This enumeration lists the b3Base<T> indices for 32 bit unsigned integer reference.
 */
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

/**
 * This enumeration lists the possible error codes for b3World/b3Item handling.
 */
enum b3_world_error
{
	B3_WORLD_ERROR = -1,   //!< General serialization error.
	B3_WORLD_OK    =  0,   //!< Serialization OK.
	B3_WORLD_OPEN,         //!< File open error.
	B3_WORLD_READ,         //!< Read access denied.
	B3_WORLD_MEMORY,       //!< Not enough memory for serialization.
	B3_WORLD_PARSE,        //!< Parse error during load (wrong file format).
	B3_WORLD_WRITE,        //!< Write error.
	B3_WORLD_NOT_REGISTERED,     //!< The items classtype is not registered an no implementation is found.
	B3_WORLD_OUT_OF_ORDER,       //!< Parse error because of wrong structure size.
	B3_WORLD_CLASSTYPE_UNKNOWN,  //!< Cannot instanciate because of unknown class type.
	B3_WORLD_IMPORT              //!< General import error during file conversion.
};

typedef b3Exception<b3_world_error, 0x424c5a> b3WorldException;

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
	b3_u32     *    m_Buffer = nullptr;
	b3_u32          m_BufferSize = 0;
	bool            m_NeedEndianChange;
	b3FirstItem  *  m_Start = nullptr;
	b3_count        m_Missed = 0;

public:
	/**
	 * This variable deletes the content on destruction. This is not always
	 * wanted if the hierarchy should survive the life time of this instance.
	 * So you can disable auto deletion.
	 */
	bool            m_AutoDelete = true;

public:
	/**
	 * This constructor initializes this instance.
	 */
	b3World();

	/**
	 * This constructor deserialize the content of the named file.
	 *
	 * @param worldname The file to deserialize.
	 */
	b3World(const char * worldname);

	/**
	 * This destructor deinitializes this instance. If the auto deletion is activated
	 * the b3Item hierarchy is deleted also.
	 */
	virtual        ~b3World();

	/**
	 * This method frees all memory inside this world instance.
	 */
	void b3Free();

	/**
	 * This method allocates a b3Item instance from the item registry. If the
	 * needed class type is not found a null pointer is returned. The new
	 * instance get the default values via the initfunc call registerd in the
	 * registry.
	 *
	 * @param classtype The needed class type.
	 * @param throw_exception If true throw exception if the needed class type is
	 *                        not registered.
	 * @return The initialized b3Item instance.
	 */
	static b3Item * b3AllocNode(b3_u32  classtype, const bool throw_exception = false);

	/**
	 * This method allocates a b3Item instance from the item registry. If the
	 * needed class type is not found a null pointer is returned. The class type
	 * is determined from the given serialization buffer. The new
	 * instance reads back the values from the given serialization buffer
	 * via the loadfunc call registerd in the registry.
	 *
	 * @param buffer The serialization buffer.
	 * @param throw_exception If true throw exception if the needed class type is
	 *                        not registered.
	 * @return The initialized b3Item instance.
	 */
	static b3Item * b3AllocNode(b3_u32 * buffer, const bool throw_exception = false);

	/**
	 * This method deserialize the content of the named file.
	 *
	 * @param worldname The file to deserialize.
	 * @param throw_exception If true throw exception if the needed class type is
	 *                        not registered.
	 * @return True on success.
	 * @throws b3WorldException
	 */
	bool            b3Read(const char * worldname, const bool throw_exception = true);

	/**
	 * This method serialize the content of the named file.
	 *
	 * @param worldname The file to serialize.
	 * @param throw_exception If true throw exception if the needed class type is
	 *                        not registered.
	 * @return True on success.
	 * @throws b3WorldException
	 */
	bool            b3Write(const char * worldname, const bool throw_exception = true);

	/**
	 * This method deserialize the content from the given file handle.
	 *
	 * @param filehandle The file to deserialize.
	 * @param throw_exception If true throw exception if the needed class type is
	 *                        not registered.
	 * @return True on success.
	 */
	b3_world_error  b3Read(b3FileAbstract * filehandle, const bool throw_exception = true);

	/**
	 * This method serialize the content into the given file handle.
	 *
	 * @param filehandle The file for the serialized data.
	 * @param throw_exception If true throw exception if the needed class type is
	 *                        not registered.
	 * @return The error code.
	 */
	b3_world_error  b3Write(b3FileAbstract * filehandle, const bool throw_exception = true);

	/**
	 * this method dumps the content of the given file name to show the b3Item
	 * structure for debugging purposes.
	 *
	 * @param worldname The file to dump.
	 * @return True if the content is correct.
	 * @throws b3WorldException
	 */
	bool            b3ReadDump(const char * worldname);

	/**
	 * This method dumps the b3Item hierarchy for debugging purposes.
	 */
	void            b3Dump() const;

	/**
	 * This method removes the first b3Item from the internal b3FirstItem class
	 * and returns it.
	 *
	 * @return The removed b3Item.
	 */
	b3Item     *    b3RemoveFirst();

	/**
	 * This method returns the first b3Item listed in this instance. This is the
	 * access to the content.
	 *
	 * @return The first b3Item.
	 */
	b3Item     *    b3GetFirst() const;

	/**
	 * This method sets the given b3Item as the first b3Item.
	 *
	 * @param item The first item to set.
	 */
	void            b3SetFirst(b3Item * item);

	/**
	 * This method returns the base of the first b3Item. The given
	 * class id must match the initialized class base in the case
	 * the class id is 0.
	 *
	 * @param classid The matching class id.
	 * @return The base of the first b3Item.
	 */
	b3Base<b3Item> * b3GetHead(b3_u32 classid = 0) const;

	/**
	 * This method clones the given b3Item. First b3Store is called to serialize
	 * the content. Then the b3Item is initialized via the b3AllocNode() method
	 * in the buffer deserialization variant. So the class type of the original
	 * b3Item must be registered
	 * in the item registry. The b3Item is cloned completely with his sub items
	 * using the b3CloneBase() method.
	 *
	 * @param original The b3Item to clone.
	 * @param throw_exception If true throw exception if the needed class type is
	 *                        not registered.
	 * @return The cloned b3Item.
	 * @throws b3WorldException
	 */
	static b3Item * b3Clone(b3Item * original, const bool throw_exception = true);

	/**
	 * This method clones a complete b3Base list. The items must be stored
	 * via the b3Store() method call.
	 *
	 * @param srcBase The source base to clone.
	 * @param dstBase The destination base which gets the cloned b3Items.
	 * @param throw_exception If true throw exception if the needed class type is
	 *                        not registered.
	 * @throws b3WorldException
	 */
	static void     b3CloneBase(
		b3Base<b3Item> * srcBase,
		b3Base<b3Item> * dstBase, const bool throw_exception = true);

private:
	b3_world_error  b3EndianSwapWorld();
	b3_world_error  b3Parse(const bool throw_exception);
};

/*************************************************************************
**                                                                      **
**                        The b3Item class itself                       **
**                                                                      **
*************************************************************************/

/**
 * This defines an initialization function for b3Item initialization with default values.
 */
typedef b3Item * (*b3_item_init_func)(b3_u32  class_type);

/**
 * This defines an initialization function for b3Item initialization from a serialization buffer.
 */
typedef b3Item * (*b3_item_load_func)(b3_u32 * src);

#define B3_ITEM_BASE(item_class)  item_class(b3_size class_size,b3_u32 classtype);
#define B3_ITEM_INIT(item_class)  item_class(b3_u32 class_type); static b3Item *b3StaticInit(b3_u32  class_type) { return new item_class(class_type); }
#define B3_ITEM_LOAD(item_class)  item_class(b3_u32 *src);       static b3Item *b3StaticInit(b3_u32 *src)        { return new item_class(src); }

struct b3_preparation_info
{
};

/**
 * This class provides serialization methods for one data item.
 */
class B3_PLUGIN b3Item : public b3Link<b3Item>, public b3Mem
{
protected:
	b3_u32           m_ItemSize;    //!< The stored size of this item in bytes.
	b3_s32           m_ItemOffset;  //!< The offset to the text area in this stored b3Item.
	b3Base<b3Item> * m_Heads;       //!< The list heads.
	b3_u32           m_HeadCount;   //!< The number of list heads.

	// Attributes for parsing
	b3_u32     *     m_Buffer;      //!< This is a memory buffer of an archived b3Item.
	b3_u32           m_ParseIndex;  //!< This is an index in the memory buffer for parsing the b3Item.

	// Attributes for writing
	b3_u32           m_StoreIndex;  //!< The index to a 32 bit unsigned integer of the actual store position.
	b3_s32           m_StoreOffset; //!< The index to the text area as an index to a 32 bit wide unsigned integer.
	b3_u32           m_StoreSize;   //!< The number of 32 bit unsigned integers.
	b3_u32     *     m_StoreBuffer; //!< A temporary store buffer.

public:
	/**
	 * This constructor initializes the instance with default values and no content.
	 */
	b3Item();

	/**
	 * This constructor is meant for initializing the instance with default values. The
	 * use is creating a new data item.
	 *
	 * @param classsize The instance size.
	 * @param classtype The class type of the new instance.
	 */
	b3Item(b3_size classsize, b3_u32 classtype);

	/**
	 * This constructor initializes the instance content from a serialization buffer. The
	 * use is for loading serialized data.
	 *
	 * @param buffer The serialization buffer.
	 */
	b3Item(b3_u32 * buffer);

	/**
	 * This destructor deinitializes this instance.
	 */
	virtual                ~b3Item();

	/**
	 * This method registers a class type for the b3ItemRegistry object factory.
	 *
	 * @param initfunc The initialization method to fill the b3Item with default values.
	 * @param loadfunc The load method to fill the b3Item with serialized data.
	 * @param classtype The class type as key.
	 * @param isclass The flag wether a class initialization should be used.
	 * @throws b3WorldException
	 */
	static  void            b3Register(
		b3_item_init_func initfunc,
		b3_item_load_func loadfunc,
		b3_u32            classtype,
		bool              isclass = false);

	/**
	 * This method serializes the content of this instance. The header is written
	 * automatically through the b3Store(9 method. The content is written in the overloaded
	 * version of this method and
	 * uses the b3Store* methods described at another position.
	 */
	virtual void            b3Write();

	/**
	 * This method returns an instance individual description.
	 *
	 * @return The individual instance name.
	 */
	virtual const char   *  b3GetName() const;

	/**
	 * This virtual function initializes this class in an implementation specific manner.
	 *
	 * @param info Information for preparation job.
	 *
	 * @return True on success.
	 */
	virtual bool            b3Prepare(b3_preparation_info * info);

	/**
	 * This method dumps the content of this instance. This method can be overloaded to
	 * dump implementation specific information of the overloaded class.
	 *
	 * @param level The actual recursion depth.
	 */
	virtual void            b3Dump(b3_count level) const;

	/**
	 * This method dumps the maintainance information of this class for debugging purposes.
	 *
	 * @param level The recursion level.
	 * @param loglevel The log level.
	 */
	void b3DumpSimple(b3_count level = 0, b3_log_level loglevel = B3LOG_NORMAL) const;

	/**
	 * This method allocates memory for the specified amount of list heads.
	 *
	 * @param headcount The number of heads needed.
	 * @return True on success.
	 */
	bool    b3AllocHeads(b3_count headcount);

	/**
	 * This method writes recursively the content of this b3Item instance. It writes the header
	 * the list heads and then calls b3Write(). After that the b3Items found in the list heads
	 * are called for storing. The storing occurs into an internal memory buffer.
	 *
	 * @throws b3WorldException
	 * @return The overall size in bytes written yet.
	 */
	b3_u32  b3Store();

	/**
	 * This method writes recursively the content of this b3Item instance. It writes the header
	 * the list heads and then calls b3Write(). After that the b3Items found in the list heads
	 * are called for storing. The storing occurs into a given file.
	 *
	 * @param file The file to store into.
	 * @return The error code for storing.
	 * @see b3FileAbstract.
	 */
	b3_world_error  b3StoreFile(b3FileAbstract * file);

	/**
	 * This method computes a checksum of the written content. The checksum is used to determine
	 * value changes of values that could be stored. You must call b3Store() before checksumming.
	 *
	 * @return The computed checksum.
	 */
	b3_u32    b3Checksum();

	/**
	 * This method returns true if the specified class type is in the same class
	 * of the given class id.
	 *
	 * @param classtype The class type to check.
	 * @param classid   The class ID.
	 * @return True if the class type is in the given class.
	 */
	static bool     b3IsClass(const b3_u32 classtype, const b3_u32 classid);

	/**
	 * This method returns true if the class type of this instance is in the same class
	 * of the given class id.
	 *
	 * @param classid   The class ID.
	 * @return True if the class type of this instance  is in the given class.
	 */
	bool    b3IsClass(const b3_u32 classid) const;

protected:
	////////////////////////////////////////////// Parsing routines
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
	void     b3InitVector(b3_vector  * vec = nullptr);

	/**
	 * This method reads a three component vector into the given vector reference.
	 *
	 * @param vec The vector buffer as reference.
	 */
	void     b3InitVector(b3Vector32 & vec);

	/**
	 * This method reads a four component vector into the given vector pointer.
	 *
	 * @param vec The vector buffer as pointer.
	 */
	void     b3InitVector4D(b3_vector4D * vec = nullptr);

	/**
	 * This method reads a four component vector into the given vector reference.
	 *
	 * @param vec The vector buffer as reference.
	 */
	void     b3InitVector4D(b3Vector32 & vec);

	/**
	 * This method reads a 4x4 matrix into the given matrix pointer.
	 *
	 * @param mat The matrix buffer as pointer.
	 */
	void     b3InitMatrix(b3_matrix  * mat);

	/**
	 * This method reads a four component color value into the given b3_color pointer.
	 *
	 * @param col The color buffer as pointer.
	 */
	void     b3InitColor(b3_color  *  col);

	/**
	 * This method reads a four component color value into the given b3Color pointer.
	 *
	 * @param color The color instance as pointer.
	 */
	void     b3InitColor(b3Color   &  color);

	/**
	 * This method reads back a spline instance.
	 *
	 * @param spline The b3Spline class pointer which defines the spline.
	 * @param controls The pointer to the spline control pointer.
	 * @param knots The knot vector of the spline.
	 */
	void     b3InitSpline(b3Spline  * spline, b3_vector  * controls = nullptr, b3_f32 * knots = nullptr);

	/**
	 * This method reads back a NURBS instance.
	 *
	 * @param nurbs The b3Nurbs class pointer which defines the NURBS.
	 * @param controls The pointer to the NURBS control pointer.
	 * @param knots The knot vector of the NURBS.
	 */
	void     b3InitNurbs(b3Nurbs * nurbs, b3_vector4D * controls = nullptr, b3_f32 * knots = nullptr);

	/**
	 * This method reads back a string of the given size and corrects the read index.
	 *
	 * @param name The text buffer.
	 * @param len  The size of the text buffer.
	 */
	void     b3InitString(char * name, b3_size len);

	/**
	 * This method bumps the read index and returns a null pointer.
	 *
	 * @return Null pointer.
	 */
	void  *  b3InitNull();

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
	void     b3StoreInt(const b3_s32       value);

	/**
	 * This method stores one unsigned integer value.
	 *
	 * @param value The unsigned integer to store.
	 */
	void     b3StoreInt(const b3_u32       value);

	/**
	 * This method stores one index value.
	 *
	 * @param value The index to store.
	 */
	void     b3StoreIndex(const b3_index     value);

	/**
	 * This method stores one resolution value.
	 *
	 * @param value The resolution value to store.
	 */
	void     b3StoreRes(const b3_res       value);

	/**
	 * This method stores one counter value.
	 *
	 * @param value The counter to store.
	 */
	void     b3StoreCount(const b3_count     value);

	/**
	 * This method stores one single precision floating point value.
	 *
	 * @param value The float value to store.
	 */
	void     b3StoreFloat(const b3_f32       value);
	/**
	 * This method stores one boolean value. Note that this value
	 * is 32 bits wide!
	 *
	 * @param value The boolean value to store.
	 */
	void     b3StoreBool(const b3_bool      value);

	/**
	 * This method stores one pointer place holder. Note that this
	 * value is a 32 bit value even on 64 bit machines! This value
	 * signals only a non null pointer.
	 *
	 * @param ptr The pointer.
	 */
	void     b3StorePtr(const void    *    ptr);

	/**
	 * This method stores a three component vector.
	 *
	 * @param vec The vector to store.
	 */
	void     b3StoreVector(const b3_vector  * vec = nullptr);

	/**
	 * This method stores a three component vector.
	 *
	 * @param vec The vector to store.
	 */
	void     b3StoreVector(b3Vector32 & vec);

	/**
	 * This method stores a four component vector.
	 *
	 * @param vec The vector to store.
	 */
	void     b3StoreVector4D(const b3_vector4D * vec = nullptr);

	/**
	 * This method stores a four component vector.
	 *
	 * @param vec The vector to store.
	 */
	void     b3StoreVector4D(b3Vector32 & vec);

	/**
	 * This method stores a 4x4 matrix.
	 *
	 * @param matrix The matrix to store.
	 */
	void     b3StoreMatrix(const b3_matrix  * matrix);

	/**
	 * This method stores a four component color.
	 *
	 * @param col The color.
	 */
	void     b3StoreColor(const b3_color  *  col);

	/**
	 * This method stores a four component color.
	 *
	 * @param col The color.
	 */
	void     b3StoreColor(b3Color   &  col);

	/**
	 * This method stores the contents of a b3Spline class.
	 *
	 * @param spline The spline structure.
	 */
	void     b3StoreSpline(const b3Spline  * spline);

	/**
	 * This method stores the contents of a b3Nurbs class.
	 *
	 * @param nurbs The NURBS structure.
	 */
	void     b3StoreNurbs(const b3Nurbs * nurbs);

	/**
	 * This method stores a text string. The offset is adjusted automatically
	 * if no previous string storage occured. Note that only more strings
	 * may be stored.
	 *
	 * @param name The text buffer.
	 * @param len  The size of the text buffer.
	 */
	void     b3StoreString(const char    *    name, const b3_size len);

	/**
	 * This method stores a null pointer. The size of the stored pointer
	 * is 32 bit even on a 64 bit machine. It is only an indicator to
	 * a null reference.
	 */
	void     b3StoreNull();

	/**
	 * This method bumps the store index.
	 */
	void     b3StoreNOP();

protected:
	/**
	 * This method dumps spaces depending on the nesting dump level.
	 *
	 * @param level The nesting level.
	 * @param loglevel The log level used for logging.
	 */
	static void     b3DumpSpace(const b3_count level, const b3_log_level loglevel = B3LOG_NORMAL);

private:
	void            b3EnsureStoreBuffer(b3_u32 needed, bool    is_data = true);
	b3_world_error  b3ParseLinkuage(b3Item ** array, b3_u32 node_count, b3_u32 class_limit, b3_count level = 0);

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
	void    b3Write() override;

protected:
	/**
	 * This method returns the list head of the content.
	 *
	 * @return The content list header.
	 */
	b3Base<b3Item> * b3GetHead() const;

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
