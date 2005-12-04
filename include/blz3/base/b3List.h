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

/**
 * This class is one list element. All elements are identified by
 * the same class part of the elements class type. If the class type
 * is zero the b3Link is meant as an anonymous list.
 *
 * We are using a doubly linked list in the following manner:
\verbatim
                                                                    +------------+
   b3Base                 b3Link       b3Link       b3Link ...      |            |
 +---------+                                                        v            |
 |         |              +----+       +----+       +----+       +----+          |
 |  First  |------------->|Succ|------>|Succ|------>|Succ|------>|Succ|--> null  |
 |         |              |    |       |    |       |    |       |    |          |
 |  Last   |--+   null <--|Prev|<------|Prev|<------|Prev|<------|Prev|          |
 |         |  |           +----+       +----+       +----+       +----+          |
 +---------+  |                                                                  |
              +------------------------------------------------------------------+
\endverbatim
 *
 * @see b3Base
 */
template <class T> class B3_PLUGIN b3Link
{
public:
	T         *Succ; //!< The next list element or null if this is the last one.
	T         *Prev; //!< The previous list element or null if this is the first one.

protected:
	b3_u32     ClassType; //!< The class and type of thie element instance.
	b3_size    Size;      //!< The size in bytes of this instance.
	b3_offset  Offset;    //!< The offset to the instance text for serailizing.

public:
	/**
	 * This constructor initializes this list element.
	 *
	 * @param new_size The class instance size.
	 * @param new_class The class type for this list item.
	 */
	b3Link(const b3_size new_size,const b3_u32 new_class = 0)
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

	/**
	 * This method returns the class part of the class type.
	 *
	 * @return The class part of the class type.
	 */
	inline b3_u32 b3GetClass()
	{
		return ClassType & B3_CLASS_MASK;
	}

	/**
	 * This method returns the type part of the class type.
	 *
	 * \return The type part of the class type.
	 */
	inline b3_u32 b3GetType()
	{
		return ClassType & B3_TYPE_MASK;
	}

	/**
	 * This method returns the class type of this list element.
	 *
	 * @return The class type of this list element.
	 */
	inline b3_u32 b3GetClassType()
	{
		return ClassType;
	}

	/**
	 * This method returns the class instance size.
	 *
	 * @return The class instance size.
	 */
	inline b3_size b3GetSize()
	{
		return Size;
	}
};

#define B3_FOR_BASE(b,n)    for((n) = (b)->First;(n)!= null;(n) = (n)->Succ)
#define B3_FOR_BASE_BACK(b,n)    for((n) = (b)->Last;(n)!= null;(n) = (n)->Prev)
#define B3_DELETE_BASE(b,n) ((b)->b3Free())

/**
 * This class is the base container for a doubly linked list. All
 * elements in the list have got the same class specifier.
 *
\verbatim
                                                                    +------------+
   b3Base                 b3Link       b3Link       b3Link ...      |            |
 +---------+                                                        v            |
 |         |              +----+       +----+       +----+       +----+          |
 |  First  |------------->|Succ|------>|Succ|------>|Succ|------>|Succ|--> null  |
 |         |              |    |       |    |       |    |       |    |          |
 |  Last   |--+   null <--|Prev|<------|Prev|<------|Prev|<------|Prev|          |
 |         |  |           +----+       +----+       +----+       +----+          |
 +---------+  |                                                                  |
              +------------------------------------------------------------------+
\endverbatim
 *
 * @see b3Link
 */
template <class T> class B3_PLUGIN b3Base
{
protected:
	b3_u32  Class; //!< The class specifier.

public:
	T      *First; //!< The first list element.
	T      *Last;  //!< The last list element.

public:
	/**
	 * This constructor initializes the list with the specified class.
	 */
	b3Base(const b3_u32 new_class = 0)
	{
		b3InitBase(new_class);
	}

	~b3Base    ()
	{
	}

	/**
	 * This method initializes the list with the specified class.
	 * Note that a previously non empty list will loose all its
	 * elements!
	 */
	inline void b3InitBase(const b3_u32 new_class = 0)
	{
		First = null;
		Last  = null;
		Class = new_class;
	}

	/**
	 * Thie method returns the class specifier.
	 *
	 * @return The class specifier.
	 */
	inline b3_u32 b3GetClass()
	{
		return Class;
	}

	/**
	 * This method returns true if the list is empty.
	 *
	 * \return True if list is empty.
	 */
	inline b3_bool b3IsEmpty()
	{
		return (First == null) && (Last == null);
	}

	/**
	 * This method appends all list elements from the source list
	 * to this list.
	 *
	 * @param from The source list where all list elements are removed
	 *             and appended to this list.
	 */
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

	/**
	 * This method removes and deletes all elements from this list.
	 */
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

	/**
	 * This method only removes all elements from this list.
	 */
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

	/**
	 * This method returns the amount of listed elements.
	 *
	 * \return The amount of listed elements.
	 */
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

	/**
	 * This element appends the specified element to the list.
	 *
	 * \param *ptr The list element to append.
	 */
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

	/**
	 * This element prepends the specified element to the list.
	 *
	 * \param *ptr The list element to prepend.
	 */
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

	/**
	 * This method removes the specified element from the list.
	 *
	 * \param *ptr The element to remove.
	 */
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

	/**
	 * This method removes the first element from the list.
	 *
	 * \return The removed first element.
	 */
	inline T *b3RemoveFirst()
	{
		T *removed = First;

		if (removed != null)
		{
			b3Remove(removed);
		}
		return removed;
	}

	/**
	 * This method removes the last element from the list.
	 *
	 * \return The removed last element.
	 */
	inline T *b3RemoveLast()
	{
		T *removed = Last;

		if (removed != null)
		{
			b3Remove(removed);
		}
		return removed;
	}

	/**
	 * This method inserts an element at a specified position.
	 *
	 * \param *pre The element after that the element is inserted.
	 * \param *ptr The element to insert.
	 */
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

	/**
	 * This method selects another element from the list based on the
	 * given element. This is for reselecting purposes in GUIs if one
	 * list element should be removed. The specified element is not
	 * removed.
	 *
	 * \param *ptr The element from which reselecting is based on.
	 * \return The reselected element.
	 */
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

	/**
	 * This method returns state information for the given element.
	 *
	 * \param *ptr The element
	 * \return The state of the element.
	 */
	inline long b3State(const T *ptr)
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

	/**
	 * This method sorts the elements using a specified sorting method.
	 * This is a nice piece of code! An in situ merge sort on
	 * doubly linked list.
	 *
	 * \param *func The sorting method.
	 * \param *Ptr A pointer to custom information.
	 */
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

	/**
	 * This method moves all elements from another list into this list. This
	 * list must be empty before moving.
	 *
	 * \param *src The source list where the elements moved from into this list.
	 */
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
