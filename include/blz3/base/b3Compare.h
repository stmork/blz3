/*
**
**	$Filename:	b3Compare.h $
**	$Release:	Dortmund 2002 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - some comparator definitions
**
**	(C) Copyright 2002  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_BASE_COMPARE_H
#define B3_BASE_COMPARE_H

template<class T> class b3Comparator
{
public:
	static int b3Sort(T *a,T *b)
	{
		return a->b3Cmp(b);
	}

	static int b3Sort(T *a,T *b,const void *dummy)
	{
		return a->b3Cmp(b);
	}

	virtual int b3Cmp(T *b)
	{
		// This is left side
		// b is right side.
		return 0;
	}
};

typedef int (*b3QSortFunc)(const void *,const void *);
//typedef int b3SortFunc(const <class T> *a,const <class T> *b,const void *ptr);

#endif
