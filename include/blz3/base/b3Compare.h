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

#pragma once

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
	 * which override this interface. It returns a negative number if \c a is
	 * smaller than \c b, a positive number if \c a is greater than \c b and
	 * \c 0 otherwise.
	 *
	 * @param left The first element to compare.
	 * @param right The second element to compare.
	 */
	static int b3Sort(const T * left, const T * right)
	{
		return left->b3Cmp(right);
	}

	/**
	 * This virtual function is the real comparing method. It
	 * compares a value of this instance with an external instance.
	 * If the external value is greater than of this instance return
	 * a positive integer. If the external value is smaller return
	 * a negative number. In case of two equal values return zero.
	 *
	 * @note This method may be changed to a spaceship operator when using
	 * C++20.
	 *
	 * @param other The external instance to compare to.
	 * @return The comparison value as documented above.
	 */
	virtual int b3Cmp(const T * other) const = 0;

	inline bool operator<(const T & other) const
	{
		return b3Cmp(&other) < 0;
	}
};

/**
 * This function definition is used for function parameters
 * inside the b3Compare class.
 */
typedef int (*b3QSortFunc)(const void *, const void *);

#endif
