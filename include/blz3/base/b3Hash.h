/*
**
**	$Filename:	b3Hash.h $
**	$Release:	Dortmund 2002, 2016 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - Some hashing template classes
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#pragma once

#ifndef B3_BASE_HASH_H
#define B3_BASE_HASH_H

#include "blz3/b3Config.h"
#include "blz3/system/b3Exception.h"
#include "blz3/base/b3Array.h"
#include "blz3/base/b3List.h"

/*************************************************************************
**                                                                      **
**                        b3HashException implementation                **
**                                                                      **
*************************************************************************/

enum b3_hash_error
{
	B3_HASH_ERROR = -1,    //!< General error.
	B3_HASH_OK    =  0,    //!< Everything is OK.
	B3_HASH_DUPLICATE_KEY, //!< The used key exists already in the hash map.
	B3_HASH_INVALID        //!< The computed hash is invalid.
};

typedef b3Exception<b3_hash_error,0x485348> b3HashException;

/*************************************************************************
**                                                                      **
**                        b3HashPair<key,object> implementation         **
**                                                                      **
*************************************************************************/

template <class Key,class Object> class b3HashMap;

/**
 * This structure represents one single key to object mapping.
 */
template <class Key,class Object> struct b3HashContainer
{
	Key    m_Key;    //!< The key object.
	Object m_Object; //!< The resulting object.
};

/**
 * This class handles the hash container pair consisting from a
 * key object pair.
 */
template <class Key,class Object> class b3HashPair :
			public b3Link<b3HashPair<Key,Object> >,
			protected b3HashContainer<Key,Object>
{
	friend class b3HashMap<Key,Object>;

	using b3HashContainer<Key,Object>::m_Key;
	using b3HashContainer<Key,Object>::m_Object;

	/**
	 * This constructor constructs one key object pair from the given value.
	 *
	 * @param key The object's key.
	 * @param key The object itself.
	 */
	b3HashPair(const Key &key,const Object &object) :
			b3Link<b3HashPair<Key,Object> >(sizeof(b3HashPair<Key,Object>))
	{
		m_Key    = key;
		m_Object = object;
	}

	/**
	 * This copy constructor constructs a hash pair copy from the given
	 * pair.
	 *
	 * @param constainer The given source b3HashContainer.
	 */
	b3HashPair(const b3HashContainer<Key,Object> &container) :
			b3Link<b3HashPair<Key,Object> >(sizeof(b3HashPair<Key,Object>))
	{
		m_Key    = container.m_Key;
		m_Object = container.m_Object;
	}

	/**
	 * This Method resets the object hold in this class instance.
	 *
	 * @param object The new object reference.
	 */
	inline void b3SetObject(const Object &object)
	{
		m_Object = object;
	}
};

/*************************************************************************
**                                                                      **
**                        b3HashMap<key,object> implementation          **
**                                                                      **
*************************************************************************/

#define B3_MAX_HASH_INDEX 173

typedef b3_u32 b3_hash;

/**
 * This class represents a key to object mapping. It is used
 * to find an object from a key in an average time of O(1).
 */
template <class Key,class Object> class b3HashMap
{
	b3Base<b3HashPair<Key,Object > >   m_HashMap[B3_MAX_HASH_INDEX];
	b3_hash                          (*m_HashFunc)(const Key &key);

public:
	/**
	 * This constructor initializes the hash map using the default
	 * hash function.
	 */
	b3HashMap()
	{
		b3SetHashFunc(&b3HashFunc);
	}

	virtual ~b3HashMap()
	{
		b3Clear();
	}

	/**
	 * This method sets a new hash computation function.
	 *
	 * @param func The new hash function.
	 */
	inline void b3SetHashFunc(b3_hash (*func)(const Key &key))
	{
		m_HashFunc = func;
	}

	/**
	 * This method adds a number of key object pairs into the hash map.
	 *
	 * @param container A pointer to several key object pairs.
	 * @param num The number of key object pairs.
	 */
	inline void b3Init(b3HashContainer<Key,Object> *container,b3_count num)
	{
		for (b3_count i = 0;i < num;i++)
		{
			b3Add(container[i].m_Key,container[i].m_Object);
		}
	}

	/**
	 * This method adds a key object pair thze the hash map.
	 *
	 * @param key The object's key.
	 * @param object The object itself.
	 * @throws b3HashException
	 */
	inline void b3Add(const Key &key, const Object &object)
	{
		b3HashPair<Key,Object> *pair;
		b3_hash                 idx = b3Hash(key);

		B3_FOR_BASE(&m_HashMap[idx],pair)
		{
			if (pair->m_Key == key)
			{
				B3_THROW(b3HashException,B3_HASH_DUPLICATE_KEY);
			}
		}
		pair = new b3HashPair<Key,Object>(key,object);
		m_HashMap[idx].b3First(pair);
	}

	/**
	 * This method replaces an object for the given key.
	 *
	 * @param key The object's key.
	 * @param object The object to replace.
	 * @return True if an key object pair already existed.
	 */
	inline b3_bool b3Replace(const Key &key, const Object &object)
	{
		b3HashPair<Key,Object> *pair;
		b3_hash                 idx = b3Hash(key);

		B3_FOR_BASE(&m_HashMap[idx],pair)
		{
			if (pair->m_Key == key)
			{
				pair->b3SetObject(object);
				return true;
			}
		}
		pair = new b3HashPair<Key,Object>(key,object);
		m_HashMap[idx].b3First(pair);
		return false;
	}

	/**
	 * This method returns true if the hash map is empty.
	 *
	 * @return True if the map is empty.
	 */
	inline const b3_bool b3IsEmpty() const
	{
		for (b3_loop i = 0;i < B3_MAX_HASH_INDEX;i++)
		{
			if (m_HashMap[i].First != null)
			{
				return false;
			}
		}
		return true;
	}

	/**
	 * This method returns the mapped amount of objects.
	 *
	 * @return The amount of mapped objects.
	 */
	inline const b3_count b3GetCount() const
	{
		b3_count count = 0;

		for (b3_loop i = 0;i < B3_MAX_HASH_INDEX;i++)
		{
			count += m_HashMap[i].b3GetCount();
		}
		return count;
	}

	/**
	 * This method returns an object which is mapped through
	 * a given key.
	 *
	 * @param key The search key.
	 * @return The resulting object.
	 */
	inline const Object *b3Find(const Key &key) const
	{
		b3HashPair<Key,Object> *pair;
		b3_hash                 idx = b3Hash(key);

		B3_FOR_BASE(&m_HashMap[idx],pair)
		{
			if (pair->m_Key == key)
			{
				return &pair->m_Object;
			}
		}
		return null;
	}

	/**
	 * This method checks whether a given key is mapped inside this hash map.
	 *
	 * @param key The key to check.
	 * @return A flag if the given key is inside the hash map.
	 */
	inline const b3_bool b3HasKey(const Key &key) const
	{
		b3HashPair<Key,Object> *pair;
		b3_hash                 idx = b3Hash(key);

		B3_FOR_BASE(&m_HashMap[idx],pair)
		{
			if (pair->m_Key == key)
			{
				return true;
			}
		}
		return false;
	}

	/**
	 * This method removes a given key object mapping from the hash map.
	 *
	 * @param key The key to remove.
	 * @return A flag if the key object pair was found.
	 */
	inline b3_bool b3Remove(const Key &key)
	{
		b3HashPair<Key,Object> *pair;
		b3_hash                 idx = b3Hash(key);

		B3_FOR_BASE(&m_HashMap[idx],pair)
		{
			if (pair->m_Key == key)
			{
				m_HashMap[idx].b3Remove(pair);
				delete pair;
				return true;
			}
		}
		return false;
	}

	/**
	 * This method removes all key object pairs in the hash map.
	 */
	inline void b3Clear()
	{
		b3HashPair<Key,Object> *pair;

		for (b3_loop i = 0;i < B3_MAX_HASH_INDEX;i++)
		{
			while ((pair = m_HashMap[i].b3RemoveFirst()) != null)
			{
				delete pair;
			}
		}
	}

	/**
	 * This method returns an array with all keys inside this hash map.
	 *
	 * @return An array with all mapping keys.
	 */
	inline b3Array<Key> *b3GetKeys() const
	{
		b3Array<Key>           *keys = new b3Array<Key>;
		b3HashPair<Key,Object> *pair;

		for (b3_loop i = 0;i < B3_MAX_HASH_INDEX;i++)
		{
			B3_FOR_BASE (&m_HashMap[i],pair)
			{
				keys->b3Add(pair->m_Key);
			}
		}
		return keys;
	}


	/**
	 * This method returns an array with all objects inside this hash map.
	 *
	 * @return An array with all mapped objects.
	 */
	inline b3Array<Object> *b3GetObjects()
	{
		b3Array<Object>        *objects = new b3Array<Object>;
		b3HashPair<Key,Object> *pair;

		for (b3_loop i = 0;i < B3_MAX_HASH_INDEX;i++)
		{
			B3_FOR_BASE (&m_HashMap[i],pair)
			{
				objects->b3Add(pair->m_Object);
			}
		}
		return objects;
	}

private:
	inline const b3_hash b3Hash(const Key &key) const
	{
		b3_hash hash = m_HashFunc(key);

		if (hash >= B3_MAX_HASH_INDEX)
		{
			B3_THROW(b3HashException,B3_HASH_INVALID);
		}
		return hash;
	}

	inline static b3_hash b3HashFunc(const Key &key)
	{
		b3_u08  *ptr  = (b3_u08 *)&key;
		b3_size  size = sizeof(Key),i;
		b3_hash  hash = 0;

		for (i = 0;i < size;i++)
		{
			hash += ptr[i];
		}
		return (hash >> 2) % B3_MAX_HASH_INDEX;
	}
};

#endif
