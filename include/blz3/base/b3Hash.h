/*
**
**	$Filename:	b3Hash.h $
**	$Release:	Dortmund 2002 $
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
	B3_HASH_ERROR = -1,
	B3_HASH_OK    =  0,
	B3_HASH_DUPLICATE_KEY,
	B3_HASH_INVALID
};

typedef b3Exception<b3_hash_error,'HSH'> b3HashException;

/*************************************************************************
**                                                                      **
**                        b3HashPair<key,object> implementation         **
**                                                                      **
*************************************************************************/

template <class Key,class Object> class b3HashMap;

template <class Key,class Object> struct b3HashContainer
{
	Key    m_Key;
	Object m_Object;
};

template <class Key,class Object> class b3HashPair :
	public b3Link<b3HashPair<Key,Object> >,
	protected b3HashContainer<Key,Object>
{
	friend class b3HashMap<Key,Object>;

	b3HashPair(const Key &key,const Object &object) :
		b3Link<b3HashPair<Key,Object> >(sizeof(b3HashPair<Key,Object>))
	{
		m_Key    = key;
		m_Object = object;
	}

	b3HashPair(const b3HashContainer<Key,Object> &container) :
		b3Link<b3HashPair<Key,Object> >(sizeof(b3HashPair<Key,Object>))
	{
		m_Key    = container.m_Key;
		m_Object = container.m_Object;
	}

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

template <class Key,class Object> class b3HashMap
{
	b3Base<b3HashPair<Key,Object > >   m_HashMap[B3_MAX_HASH_INDEX];
	b3_hash                          (*m_HashFunc)(const Key &key);
	
public:
	b3HashMap()
	{
		b3SetHashFunc(&b3HashFunc);
	}

	virtual ~b3HashMap()
	{
		b3Clear();
	}

	inline void b3SetHashFunc(b3_hash (*func)(const Key &key))
	{
		m_HashFunc = func;
	}

	inline void b3Init(b3HashContainer<Key,Object> *container,b3_count num)
	{
		for (b3_count i = 0;i < num;i++)
		{
			b3Add(container[i].m_Key,container[i].m_Object);
		}
	}

	inline void b3Add(const Key &key,const Object &object)
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

	inline b3_bool b3Replace(const Key &key,const Object &object)
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

	inline b3_bool b3IsEmpty()
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

	inline b3_count b3GetCount()
	{
		b3_count count = 0;

		for (b3_loop i = 0;i < B3_MAX_HASH_INDEX;i++)
		{
			count += m_HashMap[i].b3GetCount();
		}
		return count;
	}

	inline Object *b3Find(Key key)
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

	inline b3_bool b3HasKey(Key key)
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

	b3_bool b3Remove(const Key &key)
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

	inline void b3Clear()
	{
		b3HashPair<Key,Object> *pair;

		for(b3_loop i = 0;i < B3_MAX_HASH_INDEX;i++)
		{
			while(pair = m_HashMap[i].b3RemoveFirst())
			{
				delete pair;
			}
		}
	}

	inline b3Array<Key> *b3GetKeys()
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
	inline b3_hash b3Hash(const Key &key)
	{
		b3_hash hash = m_HashFunc(key);

		if ((hash < 0) || (hash >= B3_MAX_HASH_INDEX))
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
