/*
**
**	$Filename:	b3Cubic.h
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork
**
**	Blizzard III - cubic calculations
**
**	(C) Copyright 2001  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#pragma once

#ifndef B3_BASE_CUBIC_H
#define B3_BASE_CUBIC_H

#include "blz3/b3Types.h"

/**
 * This class provides some methods for solving equations.
 */
class B3_PLUGIN b3Cubic
{
public:
	enum Ord2 : int
	{
		O2_X0 = 0,
		O2_X1
	};


	enum Ord3 : int
	{
		O3_X0 = 0,
		O3_X1,
		O3_X2
	};

	enum Ord4 : int
	{
		O4_X0 = 0,
		O4_X1,
		O4_X2,
		O4_X3
	};

	/**
	 * This method checks a real number if it is near zero.
	 *
	 * @param x The real number to check against zero.
	 * @return True if the real number can be assumed as zero.
	 */
	template<typename T>
	static inline bool b3IsZero(const T x)
	{
		return std::fabs(x) < 4.656613e-10;
	}

	/**
	 * This method solves a quadratic equation. The coefficents are sorted
	 * in ascending order. So there is:
	 * x^2 + Coeffs[1] * x^1 + Coeffs[0] = 0
	 *
	 * @note Use the Ord2 enumeration for correct indexing!
	 *
	 * @param *Coeffs The two coefficents.
	 * @param *x The result set up to two elements.
	 * @return The number of solutions in the range of [0..2].
	 */
	static b3_count b3SolveOrd2(const b3_f64 * Coeffs, b3_f64 * x);

	/**
	 * This method solves a cubic equation. The coefficents are sorted
	 * in ascending order. So there is:
	 * x^3 + Coeffs[2] * x^2 + Coeffs[1] * x^1 + Coeffs[0] = 0
	 *
	 * @note Use the Ord3 enumeration for correct indexing!
	 *
	 * @param *Coeffs The three coefficents.
	 * @param *x The result set up to three elements.
	 * @return The number of solutions in the range of [0..3].
	 */
	static b3_count b3SolveOrd3(const b3_f64 * Coeffs, b3_f64 * x);

	/**
	 * This method solves a quartic equation. The coefficents are sorted
	 * in ascending order. So there is:
	 * x^4 + Coeffs[3] * x^3 + Coeffs[2] * x^2 + Coeffs[1] * x^1 + Coeffs[0] = 0
	 *
	 * @note Use the Ord4 enumeration for correct indexing!
	 *
	 * @param *Coeffs The four coefficents.
	 * @param *x The result set up to four elements.
	 * @return The number of solutions in the range of [0..4].
	 */
	static b3_count b3SolveOrd4(const b3_f64 * Coeffs, b3_f64 * x);
};

#endif
