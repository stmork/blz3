/*
**
**	$Filename:	b3List.h $
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - double linked list routines (proto types)
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_BASE_LIST_H
#define B3_BASE_LIST_H

#define B3_NO_CLASS_CHECK

#include "blz3/b3Config.h"
#include "blz3/base/b3Compare.h"

#define B3_NODE_FIRST        1
#define B3_NODE_PREV         2
#define B3_NODE_NULL         4
#define B3_NODE_SUCC         8
#define B3_NODE_LAST        16
#define B3_NODE_NOT_FIRST   32
#define B3_NODE_NOT_PREV    64
#define B3_NODE_NOT_NULL   128
#define B3_NODE_NOT_SUCC   256
#define B3_NODE_NOT_LAST   512

#define B3_CLASS_MASK 0xffff0000
#define B3_TYPE_MASK  0x0000ffff

template <class T> class B3_PLUGIN b3Link
{
public:
	T         *Succ;
	T         *Prev;
protected:
	b3_u32     ClassType;
	b3_size    Size;
	b3_offset  Offset;
public:
	b3Link(b3_size new_size,b3_u32 new_class = 0)
	{
		Succ      = null;
		Prev      = null;
		Size      = new_size;
		Offset    = 0;
		ClassType = new_class;
	}

	virtual ~b3Link()
	{
	}

	inline b3_u32 b3GetClass()
	{
		return ClassType & B3_CLASS_MASK;
	}

	inline b3_u32 b3GetType()
	{
		return ClassType & B3_TYPE_MASK;
	}

	inline b3_u32 b3GetClassType()
	{
		return ClassType;
	}

	inline b3_size b3GetSize()
	{
		return Size;
	}
};

#define B3_FOR_BASE(b,n)    for((n) = (b)->First;(n)!= null;(n) = (n)->Succ)
#define B3_FOR_BASE_BACK(b,n)    for((n) = (b)->Last;(n)!= null;(n) = (n)->Prev)
#define B3_DELETE_BASE(b,n) ((b)->b3Free())

template <class T> class B3_PLUGIN b3Base
{
protected:
	b3_u32  Class;

public:
	T      *First;
	T      *Last;

public:
	b3Base(b3_u32 new_class = 0)
	{
		b3InitBase(new_class);
	}

	~b3Base    ()
	{
	}

	inline void b3InitBase(b3_u32 new_class = 0)
	{
		First = null;
		Last  = null;
		Class = new_class;
	}

	inline b3_u32 b3GetClass()
	{
		return Class;
	}

	inline b3_bool b3IsEmpty()
	{
		return (First == null) && (Last == null);
	}

	inline void b3Move(b3Base<T> *from)
	{
#ifndef B3_NO_CLASS_CHECK
		if (from->b3GetClass() != Class)
		{
			return;
		}
#endif
		if (!from->b3IsEmpty())
		{
			if(b3IsEmpty())
			{
				// Simple move
				First = from->First;
				Last  = from->Last;
			}
			else
			{
				Last->Succ = from->First;
				from->First->Prev = Last;
				Last = from->Last;
			}
			
			from->b3InitBase(Class);
		}
	}

	inline void b3Free()
	{
		T *node,*succ;

		for(node = First;node != null;node = succ)
		{
			succ = node->Succ;
			delete node;
		}
		First = null;
		Last  = null;
	}

	inline void b3RemoveAll()
	{
		T *node = First,*succ;

		// Empty base
		First = null;
		Last  = null;

		while(node != null)
		{
			// unlink every node
			succ = node->Succ;
			node->Succ = null;
			node->Prev = null;
			node = succ;
		}
	}

	inline b3_count b3GetCount()
	{
		T        *node;
		b3_count  count = 0;

		for (node  = First;node != null;node  = node->Succ)
		{
			count++;
		}
		return count;
	}

	inline void b3Append(T *ptr)
	{
		B3_ASSERT((ptr->Succ == null) && (ptr->Prev == null) && (ptr != First) && (ptr != Last));
#ifndef B3_NO_CLASS_CHECK
		if (ptr->b3GetClass() != Class)
		{
			return;
		}
#endif

		if (First == null)
		{
			First = ptr;
		}
		if (Last != null)
		{
			Last->Succ = ptr;
		}
		ptr->Prev = Last;
		ptr->Succ = null;
		Last      = ptr;
	}

	inline void b3First(T *ptr)
	{
		B3_ASSERT((ptr->Succ == null) && (ptr->Prev == null) && (ptr != First) && (ptr != Last));
#ifndef B3_NO_CLASS_CHECK
		if (ptr->b3GetClass() != Class)
		{
			return;
		}
#endif

		if (Last == null)
		{
			Last = ptr;
		}
		if (First != null)
		{
			First->Prev = ptr;
		}
		ptr->Prev = null;
		ptr->Succ = First;
		First     = ptr;
	}

	inline void b3Remove(T *ptr)
	{
#ifndef B3_NO_CLASS_CHECK
		if (ptr->b3GetClass() != Class)
		{
			return;
		}
#endif

		// Relink backward link
		if (ptr->Prev == null)
		{
			First = ptr->Succ;
		}
		else
		{
			ptr->Prev->Succ = ptr->Succ;
		}

		// Relink forward link
		if (ptr->Succ == null)
		{
			Last = ptr->Prev;
		}
		else
		{
			ptr->Succ->Prev = ptr->Prev;
		}

		// Delete link itself
		ptr->Succ = null;
		ptr->Prev = null;
	}

	inline T *b3RemoveFirst()
	{
		T *removed = First;

		if (removed != null)
		{
			b3Remove(removed);
		}
		return removed;
	}

	inline T *b3RemoveLast()
	{
		T *removed = Last;

		if (removed != null)
		{
			b3Remove(removed);
		}
		return removed;
	}

	inline void b3Insert(T *pre,T *ptr)
	{
		T *succ;

		B3_ASSERT((ptr->Succ == null) && (ptr->Prev == null));
#ifndef B3_NO_CLASS_CHECK
		if (ptr->b3GetClass() != Class)
		{
			return;
		}
#endif

		ptr->Prev = pre;
		if (pre == null)
		{
			// Like b3First(node)
			ptr->Succ = First;
			if (ptr->Succ == null)
			{
				// First element in list
				Last = ptr;
			}
			else
			{
				// First is != null!
				First->Prev = ptr;
			}
			First = ptr;
		}
		else
		{
			succ = pre->Succ;
			ptr->Succ = pre->Succ;
			if (ptr->Succ == null)
			{
				// Found "pre" as last element in list
				// so correct Last
				Last = ptr;
			}
			else
			{
				succ->Prev = ptr;
			}
			pre->Succ = ptr;
		}
	}

	inline T *b3Reselect(T *ptr)
	{
		T *select = null;

		if (ptr != null)
		{
			select = ptr->Prev;
			if (select == null)
			{
				select = ptr->Succ;
			}
		}
		return select;
	}

	inline long b3State(T *ptr)
	{
		b3_u32  flags;

		if (ptr == null)
		{
			flags = B3_NODE_NULL;
		}
		else
		{
			flags = B3_NODE_NOT_NULL;
			if (ptr == First) flags |= B3_NODE_FIRST;
			else              flags |= B3_NODE_NOT_FIRST;
			if (ptr == Last)  flags |= B3_NODE_LAST;
			else              flags |= B3_NODE_NOT_LAST;
			if (ptr->Prev)    flags |= B3_NODE_PREV;
			else              flags |= B3_NODE_NOT_PREV;
			if (ptr->Succ)    flags |= B3_NODE_SUCC;
			else              flags |= B3_NODE_NOT_SUCC;
		}

		return flags;
	}

	// This is a nice piece of code! An in situ merge sort on
	// doubly linked list.
	inline void b3Sort(int (*func)(T *,T *,const void *),const void *Ptr = null)
	{
		b3Base    Right;
		T        *start,*end;
		b3_count  i = 0;

		// We don't need to sort one or zero element.
		if (First == Last)
		{
			return;
		}

		// We must find the middle node for dividing.
		// So we are going step by step from First and Last.
		// If both paths hit in the same node we are in
		// the middle of the list.
		// NOTE:
		// This is the inefficient part of this algorithm
		// because it is O(n).
		start = First;
		end   = Last;
		while (start->Succ != end)
		{
			if (i & 1)
			{
				// One backwards
				end   = end->Prev;
			}
			else
			{
				// One forwards
				start = start->Succ;
			}
			i++;
		}

		// DIVIDE
		// Now we know the middle of the lists: Split It!
		// NOTE:
		// This is very efficient: O(1)
		Right.Class = Class;
		Right.First = end;
		Right.Last  = Last;
		Last        = start;
		end->Prev   = null;
		start->Succ = null;


		// & CONQUER
		// This gives the algorithm the factor O(log n)
			  b3Sort(func,Ptr);
		Right.b3Sort(func,Ptr);

		// Now we have to merge two sorted list into
		// one sorted list.
		start = First;
		end   = Right.First;
		while ((start != null) && (end != null))
		{
			// Here is the comparison function. If the node
			// of the first list is lower we leave the node
			// at its place and use the following node.
			// Else we remove the node from the second list
			// an insert it before the node of the first list.
			if (func((T *)start,(T *)end,Ptr) > 0)
			{
				Right.b3Remove (end);
				b3Insert (start->Prev,end);
				end   = Right.First;
			}
			else 
			{
				start = start->Succ;	/* start <= end */
			}
		}

		// We have to append the rest of the second list
		// to at the end of the first list.
		// NOTE:
		// This is even more efficient: O(1)
		if (end) /* append right list to left list */
		{
			start = Last;
			end->Prev = start;
			if (start)
			{
				start->Succ = end;
			}
			else
			{
				First = end;
			}
			Last = Right.Last;
		}
	}

	inline void b3MoveFrom(b3Base<T> *src)
	{
		B3_ASSERT(b3IsEmpty());

		First = src->First;
		Last  = src->Last;
		src->First = null;
		src->Last  = null;
	}
};

#endif
