/*
**
**	$Filename:	b3Matrix.h            
**	$Release:	Dortmund 2001 $
**	$Revision$
**	$Date$
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

class B3_PLUGIN b3Cubic
{
public:
	static b3_count b3SolveOrd2(b3_f64 *Coeffs,b3_f64 *x);
	static b3_count b3SolveOrd3(b3_f64 *Coeffs,b3_f64 *x);
	static b3_count b3SolveOrd4(b3_f64 *Coeffs,b3_f64 *x);
};

#endif
