/*
**
**	$Filename:	b3Polynom.h
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

#ifndef B3_BASE_POLYNOM_H
#define B3_BASE_POLYNOM_H

#include "blz3/b3Types.h"

/**
 * This class provides some methods for solving equations.
 */
class B3_PLUGIN b3Polynom
{
public:
	/**
	 * This method solves a quadratic equation. The coefficents are sorted
	 * in ascending order. So there is:
	 * x^2 + Coeffs[1] * x^1 + Coeffs[0] = 0
	 *
	 * @note Use the b3Cubic::Ord2 enumeration for correct indexing!
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
	 * @note Use the b3Cubic::Ord3 enumeration for correct indexing!
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
	 * @note Use the b3Cubic::Ord4 enumeration for correct indexing!
	 *
	 * @param *Coeffs The four coefficents.
	 * @param *x The result set up to four elements.
	 * @return The number of solutions in the range of [0..4].
	 */
	static b3_count b3SolveOrd4(const b3_f64 * Coeffs, b3_f64 * x);

	/**
	 * Compute a polynom using Horner's scheme.
	 *
	 * @note The highest power does have the coefficient 1.0. So there are
	 * not ord + 1 coefficients but only ord. The order is like the other
	 * methods of this class.
	 *
	 * @param Coeffs The coefficients.
	 * @param x The input value of the polynom.
	 * @param ord The degree of the polynom.
	 * @return The result.
	 */
	static b3_f64 b3Horner(
		const b3_f64 * Coeffs,
		const b3_f64   x,
		const b3_count ord);
};

#endif
