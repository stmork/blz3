/*
**
**	$Filename:	b3Node.h $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - doubly linked list [obsolete, use templates <b3Base>/<b3Link> instead]
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_BASE_NODE_H
#define B3_BASE_NODE_H

#include "blz3/base/b3List.h"

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
	               b3Node(b3_size size,b3_u32 new_class = 0);
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
	       long            b3State(b3Node *);
	       void            b3Sort(bsortFunc,void *);

	// Some friends
	friend void  b3Node::b3Append(b3Head *);
	friend void  b3Node::b3First(b3Head *);
	friend void  b3Node::b3Remove(b3Head *);
	friend void  b3Node::b3Insert(b3Head *,b3Node *);
};

#define CLASSOF(node) (((b3Node *)(node))->ClassType & 0xffff0000)
#define TYPEOF(node)  (((b3Node *)(node))->ClassType & 0x0000ffff)

#endif
