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
	static b3_f64 m_CbrtCoeffs[15];

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

	static inline b3_f64 b3Limit(const b3_f64 value,const b3_f64 min,const b3_f64 max)
	{
		if (value < min)
		{
			return min;
		}
		else if (value > max)
		{
			return max;
		}
		return value;
	}

	static inline b3_f64 b3Hermite(b3_f64 t1,b3_f64 s0,b3_f64 s1)
	{
		b3_f64 t2 = t1 * t1;
		b3_f64 t3 = t2 * t1;

		b3_f64 P4 = -2 * t3 + 3 * t2;
		b3_f64 R1 =      t3 - 2 * t2 + t1;
		b3_f64 R4 =      t3 -     t2;
		
		return P4 + s0 * R1 + s1 * R4;
	}
	
	static inline b3_f64 b3Smoothstep(b3_f64 t)
	{
		if (t < 0)
		{
			return 0;
		}
		else if (t > 1)
		{
			return 1;
		}
//		return b3Hermite(t,0.0,0.0);
		return (3 - 2 * t) * t * t;
	}

#ifndef CBRT_SYS
#ifdef  CBRT_SLOW

	static inline b3_f64 b3Cbrt(b3_f64 x)
	{
		if (x > 0) return ( exp(log( x) / 3));
		if (x < 0) return (-exp(log(-x) / 3));
		return 0;
	}
#else
	static inline b3_f64 b3Cbrt(b3_f64 x)
	{
		b3_f64   xx,y;
		b3_count Negative;
		int      expon,n;

		if (x <= 0)
		{
			if (x==0) return 0;
			x = -x;
			Negative = true;
		}
		else
		{
			Negative = false;
		}

		xx = frexp (x,&expon);
		n = expon % 3;
		y = ((((((((((((((
			m_CbrtCoeffs[14]) * xx +
			m_CbrtCoeffs[13]) * xx +
			m_CbrtCoeffs[12]) * xx +
			m_CbrtCoeffs[11]) * xx +
			m_CbrtCoeffs[10]) * xx +
			m_CbrtCoeffs[ 9]) * xx +
			m_CbrtCoeffs[ 8]) * xx +
			m_CbrtCoeffs[ 7]) * xx +
			m_CbrtCoeffs[ 6]) * xx +
			m_CbrtCoeffs[ 5]) * xx +
			m_CbrtCoeffs[ 4]) * xx +
			m_CbrtCoeffs[ 3]) * xx +
			m_CbrtCoeffs[ 2]) * xx +
			m_CbrtCoeffs[ 1]) * xx +
			m_CbrtCoeffs[ 0];
		y *= (4.0 / 3 - y * y * y * xx * (1.0/3));
		y *= (y * xx);
		if (n < 0) n += 3;
		if (n == 1)    y *= 1.25992104989;	/* 2 ^ (1/3) */
		else if (n==2) y *= 1.58740105196;	/* 4 ^ (1/3) */

		if ((n=expon) < 0) n -= 2;
		n /= 3;

		b3_f64 result = ldexp(y,n);

		return Negative ? -result : result;
	}
#endif
#endif
};

#endif
