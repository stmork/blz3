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

#define B3_NODE_FIRST   1
#define B3_NODE_PREV    2
#define B3_NODE_NULL    4
#define B3_NODE_SUCC    8
#define B3_NODE_LAST   16

class b3Head;

class b3Node
{
public:
	b3Node      *Succ;
	b3Node      *Prev;
protected:
	b3_u32       ClassType;
	b3_size      Size;
	b3_offset    Offset;
public:
	               b3Node(b3_u32 size,b3_u32 new_class = 0);
	              ~b3Node();

	unsigned long  b3ClassType();
	void           b3Append(b3Head *);
	void           b3First(b3Head *);
	void           b3Remove(b3Head *);
	void           b3Insert(b3Head *,b3Node *);
};

typedef int  (*bsortFunc)(b3Node *,b3Node *,void *);

class b3Head
{
protected:
	b3_u32       Class;
public:
	b3Node      *First;
	b3Node      *Last;
public:
	                       b3Head(b3_u32 new_class = 0);
	                      ~b3Head();

	       void            b3InitHead(b3_u32 new_class = 0);
	       long  b3State(b3Node *);
	friend void  b3Node::b3Append(b3Head *);
	friend void  b3Node::b3First(b3Head *);
	friend void  b3Node::b3Remove(b3Head *);
	friend void  b3Node::b3Insert(b3Head *,b3Node *);
	       void            b3Sort(bsortFunc,void *);
};

#define CLASSOF(node) (((b3Node *)(node))->ClassType & 0xffff0000)
#define TYPEOF(node)  (((b3Node *)(node))->ClassType & 0x0000ffff)

template <class T> class b3Link
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
	~b3Link()
	{
	}

	b3_u32 b3GetClass()
	{
		return ClassType & 0xffff0000;
	}

	b3_u32 b3GetType()
	{
		return ClassType & 0x0000ffff;
	}

	b3_u32 b3GetClassType()
	{
		return ClassType;
	}

	b3_size b3GetSize()
	{
		return Size;
	}
};

#define B3_FOR_BASE(b,n) for((n) = (b)->First;(n)!= null;(n) = (n)->Succ)

template <class T> class b3Base
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

	void b3InitBase(b3_u32 new_class = 0)
	{
		First = null;
		Last  = null;
		Class = new_class;
	}

	b3_u32 b3GetClass()
	{
		return Class;
	}

	void b3Free()
	{
		T *node,*succ;

		node = First;
		while (node != null)
		{
			succ = node->Succ;
			delete node;
			node = succ;
		}
		First = null;
		Last  = null;
	}

	b3_count b3Count()
	{
		b3Link<T>  *node;
		b3_count    count = 0;

		for (node  = (b3Link<T> *)First;
		     node !=  null;
		     node  = (b3Link<T> *)node->Succ)
		{
			count++;
		}
		return count;
	}

	void b3Append  (T *ptr)
	{
		b3Link<T> *node = (b3Link<T> *)ptr;
		b3Link<T> *last = (b3Link<T> *)Last;

#ifndef B3_NO_CLASS_CHECK
		if (node->b3GetClass() != Class)
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
			last->Succ = ptr;
		}
		node->Prev = Last;
		node->Succ = null;
		Last       = ptr;
	}

	void b3First   (T *ptr)
	{
		b3Link<T> *node  = (b3Link<T> *)ptr;
		b3Link<T> *first = (b3Link<T> *)First;

#ifndef B3_NO_CLASS_CHECK
		if (node->b3GetClass() != Class)
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
			first->Prev = ptr;
		}
		node->Prev = null;
		node->Succ = First;
		First      = ptr;
	}

	void b3Remove  (T *ptr)
	{
		b3Link<T> *node = (b3Link<T> *)ptr;
		b3Link<T> *prev;
		b3Link<T> *succ;

#ifndef B3_NO_CLASS_CHECK
		if (node->b3GetClass() != Class)
		{
			return;
		}
#endif

		// Relink backward link
		if (node->Prev == null)
		{
			First = node->Succ;
		}
		else
		{
			prev = (b3Link<T> *)node->Prev;
			prev->Succ = node->Succ;
		}

		// Relink forward link
		if (node->Succ==null)
		{
			Last = node->Prev;
		}
		else
		{
			succ = (b3Link<T> *)node->Succ;
			succ->Prev	= node->Prev;
		}

		// Delete link itself
		node->Succ = null;
		node->Prev = null;
	}

	void b3Insert  (T *pre,T *ptr)
	{
		b3Link<T> *ins   = (b3Link<T> *)pre;
		b3Link<T> *node  = (b3Link<T> *)ptr;
		b3Link<T> *first = (b3Link<T> *)First;
		b3Link<T> *succ;

#ifndef B3_NO_CLASS_CHECK
		if (node->b3GetClass() != Class)
		{
			return;
		}
#endif

		node->Prev = pre;
		if (ins == null)
		{
			// Like b3First(node)
			node->Succ = First;
			if (node->Succ == null)
			{
				// First element in list
				Last = ptr;
			}
			else
			{
				first->Prev = ptr;
			}
			First = ptr;
		}
		else
		{
			succ = (b3Link<T> *)ins->Succ;
			node->Succ = ins->Succ;
			if (node->Succ == null)
			{
				// Found ins as last element in list
				// so correct Last
				Last = ptr;
			}
			else
			{
				succ->Prev = ptr;
			}
			ins->Succ = ptr;
		}
	}

	long b3State   (T *ptr)
	{
		b3Link<T> *node = (b3Link<T> *)ptr;
		b3_u32     flags = 0;

		if (ptr == First)   flags |= B3_NODE_FIRST;
		if (ptr == Last)    flags |= B3_NODE_LAST;
		if (ptr == null)    flags |= B3_NODE_NULL;
		else
		{
			if (node->Prev) flags |= B3_NODE_PREV;
			if (node->Succ) flags |= B3_NODE_SUCC;
		}
		return flags;
	}

	void b3Sort    (int (*func)(T *,T *,void *),void *Ptr)
	{
		b3Base     Right;
		b3Link<T> *start,*end;
		b3_count   i = 0;

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
		start = (b3Link<T> *)First;
		end   = (b3Link<T> *)Last;
		while (start->Succ != (T *)end)
		{
			if (i & 1)
			{
				// One backwards
				end   = (b3Link<T> *)end->Prev;
			}
			else
			{
				// One forwards
				start = (b3Link<T> *)start->Succ;
			}
			i++;
		}

		// DIVIDE
		// Now we know the middle of the lists: Split It!
		// NOTE:
		// This is very efficient: O(1)
		Right.Class = Class;
		Right.First = (T *)end;
		Right.Last  = Last;
		Last        = (T *)start;
		end->Prev   = null;
		start->Succ = null;


		// & CONQUER
		// This gives the algorithm the factor O(log n)
			  b3Sort(func,Ptr);
		Right.b3Sort(func,Ptr);

		// Now we have to merge two sorted list into
		// one sorted list.
		start = (b3Link<T> *)First;
		end   = (b3Link<T> *)Right.First;
		while (start && end)
		{
			// Here is the comparison function. If the node
			// of the first list is lower we leave the node
			// at its place and use the following node.
			// Else we remove the node from the second list
			// an insert it before the node of the first list.
			if (func((T *)start,(T *)end,Ptr) > 0)
			{
				Right.b3Remove ((T *)end);
				b3Insert (start->Prev,(T *)end);
				end   = (b3Link<T> *)Right.First;
			}
			else 
			{
				start = (b3Link<T> *)start->Succ;	/* start <= end */
			}
		}

		// We have to append the rest of the second list
		// to at the end of the first list.
		// NOTE:
		// This is even more efficient: O(1)
		if (end) /* append right list to left list */
		{
			start = (b3Link<T> *)Last;
			end->Prev = (T *)start;
			if (start)
			{
				start->Succ = (T *)end;
			}
			else
			{
				First = (T *)end;
			}
			Last = Right.Last;
		}
	}
};

#endif
