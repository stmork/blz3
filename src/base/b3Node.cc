/*
**
**	$Filename:	b3Node.cc $
**	$Release:	Dortmund 1998 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - doubly linked list [obsolete, use templates <b3Base>/<b3Link> instead]
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III includes                         **
**                                                                      **
*************************************************************************/

#include "blz3/base/b3Node.h"
#include "blz3/system/b3Log.h"

/*
We are using a doubly linked list in the following manner:
                                                                   +------------+
  b3Head                 b3Node       b3Node       b3Node ...      |            |
+---------+                                                        v            |
|         |              +----+       +----+       +----+       +----+          |
|  First  |------------->|Succ|------>|Succ|------>|Succ|------>|Succ|--> NULL  |
|         |              |    |       |    |       |    |       |    |          |
|  Last   |--+   NULL <--|Prev|<------|Prev|<------|Prev|<------|Prev|          |
|         |  |           +----+       +----+       +----+       +----+          |
+---------+  |                                                                  |
             +------------------------------------------------------------------+
*/

/*************************************************************************
**                                                                      **
**                        Blizzard III development log                  **
**                                                                      **
*************************************************************************/

/*
**	$Log$
**	Revision 1.2  2002/02/14 16:51:49  sm
**	- Done some cleanups concernig doubly linked lists. Moved
**	  obsolete b3Node/b3Head into their own include.
**
**	Revision 1.1.1.1  2001/07/01 12:24:58  sm
**	Blizzard III is born
**	
*/

/*************************************************************************
**                                                                      **
**                        b3Head                                        **
**                                                                      **
*************************************************************************/

b3Head::b3Head(b3_u32 newClass)
{
	First = null;
	Last  = null;
	Class = newClass;
}

b3Head::~b3Head()
{
}

void b3Head::b3InitHead(b3_u32 newClass)
{
	First = null;
	Last  = null;
	Class = newClass;
}

// This is a nice piece of code. It sorts a doubly linked list using
// heap sort in the same manner as "qsort()". This algorithm
// works "in situ".
void b3Head::b3Sort(
	bsortFunc  CmpFunc,
	void      *Ptr)
{
	         b3Head  Right;
	register b3Node *start,*end;
	register long      i=0;

	// We don't need to sort one element.
	if (First == Last) return;

	// We must find the middle node for dividing.
	// So we are going step by step from First and Last.
	// If both paths hit in the same node we are in
	// the middle of the list.
	// NOTE:
	// This is the unefficient part of this algorithm
	// because it is O(n).
	start = First;
	end   = Last;
	while (start->Succ != end)
	{
		if (i & 1) end   = end->Prev;
		else       start = start->Succ;
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
	      b3Sort(CmpFunc,Ptr);
	Right.b3Sort(CmpFunc,Ptr);

	// Now we have to merge two sorted list into
	// one sorted list.
	start = First;
	end   = Right.First;
	while (start && end)
	{
		// Here is the comparison function. If the node
		// of the first list is lower we leave the node
		// at its place and use the following node.
		// Else we remove the node from the second list
		// an insert it before the node of the first list.
		if (CmpFunc(start,end,Ptr) > 0)
		{
			end->b3Remove (&Right);
			end->b3Insert (this,start->Prev);
			end   = Right.First;
		}
		else start = start->Succ;	/* start <= end */
	}

	// We have to append the rest of the second list
	// to at the end of the first list.
	// NOTE:
	// This is even more efficient: O(1)
	if (end) /* append right list to left list */
	{
		start = Last;
		end->Prev = start;
		if (start) start->Succ = end;
		else       First = end;
		Last = Right.Last;
	}
}

long b3Head::b3State(b3Node *node)
{
	long flags = 0;

	if (node == First)  flags |= B3_NODE_FIRST;
	if (node == Last)   flags |= B3_NODE_LAST;
	if (node == null)   flags |= B3_NODE_NULL;
	else
	{
		if (node->Prev) flags |= B3_NODE_PREV;
		if (node->Succ) flags |= B3_NODE_SUCC;
	}
	return flags;
}

/*************************************************************************
**                                                                      **
**                        includes                                      **
**                                                                      **
*************************************************************************/

b3Node::b3Node(
	b3_size size,
	b3_u32  new_class)
{
	Succ      = null;
	Prev      = null;
	Size      = size;
	Offset    = 0;
	ClassType = new_class;
}

b3Node::~b3Node()
{
}

void b3Node::b3First(b3Head *head)
{
	if (CLASSOF(this) != head->Class) return;
	Prev = null;
	if ((Succ = head->First)==null)	head->Last = this;
	else head->First->Prev = this;
	head->First = this;
}

unsigned long b3Node::b3ClassType()
{
	return ClassType;
}

void b3Node::b3Append(b3Head *head)
{
	if (CLASSOF(this) != head->Class) return;
	Succ = null;
	if (head->Last==null)
	{
		head->First = head->Last = this;
		Prev  = null;
	}
	else
	{
		Prev             = head->Last;
		head->Last->Succ = this;
		head->Last       = this;
	}
}

void b3Node::b3Remove(b3Head *head)
{
	if (CLASSOF(this) != head->Class) return;
	if (Prev==null)	head->First	= Succ;
	else			Prev->Succ	= Succ;
	if (Succ==null)	head->Last	= Prev;
	else			Succ->Prev	= Prev;
	Succ = Prev = null;
}

void b3Node::b3Insert(
	b3Head *head,
	b3Node *node)
{
	if (CLASSOF(this) != head->Class) return;
	if (node == this) return;

	Prev = node;
	if (node==null)
	{
		if ((Succ = head->First)==null)	head->Last = this;
		else head->First->Prev = this;
		head->First	= this;
	}
	else
	{
		if ((Succ = node->Succ) == null) head->Last = this;
		else node->Succ->Prev = this;
		node->Succ = this;
	}
}

 
