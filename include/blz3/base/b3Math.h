/*
**
**	$Filename:	b3Math.h $
**	$Release:	Dortmund 2004 $
**	$Revision$
**	$Date$
**	$Author$
**	$Developer:	Steffen A. Mork $
**
**	Blizzard III - some math convenience methods...
**
**	(C) Copyright 2004  Steffen A. Mork
**	    All Rights Reserved
**
**
*/

#ifndef B3_BASE_MATH_H
#define B3_BASE_MATH_H

#include "blz3/b3Config.h"

class b3Math
{
	static b3_f64 epsilon;

public:
	static inline b3_f64 b3Sqr(b3_f64 value)
	{
		return value * value;
	}


	static inline b3_f64 b3GetMu(b3_f64 F0)
	{
		b3_f64 sF0 = sqrt(F0);

		return (1.0 + sF0) / (1.0 - sF0);
	}

	static inline b3_f64 b3GetFresnel(b3_f64 phi,b3_f64 mu)
	{
		b3_f64 theta,a,b,c,d;
	
		if (phi < epsilon)
		{
			phi = epsilon;
		}
		theta = asin(sin(phi) / mu);
		
		a = b3Sqr(sin(phi - theta));
		b = b3Sqr(sin(phi + theta));
		c = b3Sqr(tan(phi - theta));
		d = b3Sqr(tan(phi + theta));
		
		return 0.5 * (a / b + c / d);
	}
};


#endif
