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
	 * This method solves a quadratic equation.
	 *
	 * \param *Coeffs The three coefficents.
	 * \param *x The result set.
	 * \return The number of solutions.
	 */
	static b3_count b3SolveOrd2(b3_f64 *Coeffs,b3_f64 *x);

	/**
	 * This method solves a cubic equation.
	 *
	 * \param *Coeffs The four coefficents.
	 * \param *x The result set.
	 * \return The number of solutions.
	 */
	static b3_count b3SolveOrd3(b3_f64 *Coeffs,b3_f64 *x);

	/**
	 * This method solves a quartic equation.
	 *
	 * \param *Coeffs The five coefficents.
	 * \param *x The result set.
	 * \return The number of solutions.
	 */
	static b3_count b3SolveOrd4(b3_f64 *Coeffs,b3_f64 *x);
};

#endif
