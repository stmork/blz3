/*
**
**	$Filename:	b3Compare.h $
**	$Release:	Dortmund 2002, 2016 $
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

/**
 * This template class gives an interface to
 * implement comparing two instances.
 */
template<class T> class b3Comparator
{
public:
	/**
	 * This static methods compares two instances
	 * which override this interface.
	 *
	 * @param a The first element to compare.
	 * @param b The second element to compare.
	 */
	static int b3Sort(const T *a, const T *b)
	{
		return a->b3Cmp(b);
	}

	/**
	 * This static methods compares two instances
	 * which override this interface.
	 *
	 * @param a The first element to compare.
	 * @param b The second element to compare.
	 * @param dummy A pointer to provide additional information into the
	 *              comparing process.
	 */
	static const int b3Sort(const T *a, const T *b,const void *dummy)
	{
		return a->b3Cmp(b);
	}

	/**
	 * This virtual function is the real comparing method. It
	 * compares a value of this instance with an external instance.
	 * If the external value is greater than of this instance return
	 * a positive integer. If the external value is smaller return
	 * a negative number. In case of two equal values return zero.
	 *
	 * @param compare The external instance to compare to.
	 * @return The comparison value as documented above.
	 */
	virtual const int b3Cmp(const T *compare) const = 0;
};

/**
 * This function definition is used for function parameters
 * inside the b3Compare class.
 */
typedef int (*b3QSortFunc)(const void *,const void *);

#endif
