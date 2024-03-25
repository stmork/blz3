/*
**
**	$Filename:	b3Matrix.h
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
	 * This method solves a quadratic equation.
	 *
	 * \param *Coeffs The three coefficents.
	 * \param *x The result set.
	 * \return The number of solutions.
	 */
	static b3_count b3SolveOrd2(const b3_f64 * Coeffs, b3_f64 * x);

	/**
	 * This method solves a cubic equation.
	 *
	 * \param *Coeffs The four coefficents.
	 * \param *x The result set.
	 * \return The number of solutions.
	 */
	static b3_count b3SolveOrd3(const b3_f64 * Coeffs, b3_f64 * x);

	/**
	 * This method solves a quartic equation.
	 *
	 * \param *Coeffs The five coefficents.
	 * \param *x The result set.
	 * \return The number of solutions.
	 */
	static b3_count b3SolveOrd4(const b3_f64 * Coeffs, b3_f64 * x);
};

#endif
