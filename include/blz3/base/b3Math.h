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

/**
 * This class provides useful math helper methods.
 */
class B3_PLUGIN b3Math
{
	static b3_f64 epsilon;
	static b3_f64 m_CbrtCoeffs[15];

public:
	/**
	 * This method squares a value.
	 *
	 * \param value The value to square.
	 * \return The squared value.
	 */
	static inline b3_f64 b3Sqr(const b3_f64 value)
	{
		return value * value;
	}

	/**
	 * This computes a Fresnel term from an input value.
	 *
	 * \param F0
	 * \return The computed Fresnel term.
	 */
	static inline b3_f64 b3GetMu(const b3_f64 F0)
	{
		b3_f64 sF0 = sqrt(F0);

		return (1.0 + sF0) / (1.0 - sF0);
	}

	/**
	 * This method computes a Fresnel term from two values. This is
	 * known as Schlick's hack.
	 *
	 * \param phi
	 * \param mu
	 * \return The fresnel computed value.
	 */
	static inline b3_f64 b3GetFresnel(b3_f64 phi,const b3_f64 mu)
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

	/**
	 * This method limit a value against the bounds [0..1].
	 *
	 * \param value The value to limit.
	 * \return The bound value.
	 */
	static inline b3_f64 b3Limit(const b3_f64 value)
	{
		if (value < 0)
		{
			return 0;
		}
		else if (value > 1)
		{
			return 1;
		}
		return value;
	}

	/**
	 * This method limits a value against the specified bounds. This is the
	 * floating point version.
	 *
	 * \param value The value to limit.
	 * \param min The lower limit.
	 * \param max The upper limit.
	 * \return The bound value.
	 */
	static inline b3_f64 b3Limit(
		const b3_f64 value,
		const b3_f64 min,
		const b3_f64 max)
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

	/**
	 * This method limits a value against the specified bounds. This is the integer
	 * version.
	 *
	 * \param value The value to limit.
	 * \param min The lower limit.
	 * \param max The upper limit.
	 * \return The bound value.
	 */
	static inline b3_s32 b3Limit(
		const b3_s32 value,
		const b3_s32 min,
		const b3_s32 max)
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

	/**
	 * This method rounds a number by the given precision.
	 *
	 * \param x The value to round.
	 * \param rnd The precision.
	 * \return The rounded value.
	 */
	static inline b3_f32 b3Round(const b3_f32 x,const b3_f32 rnd)
	{
		b3_f32 mul    = floorf(0.5 / rnd + 0.5);
		b3_f32 result = floorf(x * mul + 0.5) / mul;

		return result;
	}

	/**
	 * This method rounds a number by the given precision.
	 *
	 * \param x The value to round.
	 * \param rnd The precision.
	 * \return The rounded value.
	 */
	static inline b3_f64 b3Round(const b3_f64 x,const b3_f64 rnd)
	{
		b3_f64 mul    = floor(0.5 / rnd + 0.5);
		b3_f64 result = floor(x * mul + 0.5) / mul;

		return result;
	}

	/**
	 * This method returns the fractional part of the division <code>a/b</code>.
	 * The result is always positive or zero even the dividend is negative.
	 *
	 * \param a The dividend.
	 * \param b The divisor.
	 * \return The fractional part.
	 */
	static inline b3_f64 b3Frac(b3_f64 a,b3_f64 b)
	{
#if 0
		b3_f64 n;

		if (a < 0)
		{
			a = -a;
			n = floor(a / b);
			return b - a + n * b;
		}
		else
		{
			n = floor(a / b);
			return a - n * b;
		}
#else
		b3_f64 remainder = fmod(a,b);

		return remainder < 0 ? remainder + b : remainder;
#endif
	}

	/**
	 * This returns the fractional part of a floating point number.
	 *
	 * \param a The complete floating point number.
	 * \return The fractional part of a float.
	 */
	static inline b3_f64 b3FracOne(const b3_f64 a)
	{
		return a - floor(a);
	}

	/**
	 * This method mixes two values with a given mixer value. The mixer
	 * must be in the range [0..1]. A mixer value of 0 results in a, a
	 * value of 1 results in b.
	 *
	 * \param a The first value to mix.
	 * \param b The second value to mix.
	 * \param x The mixer in range [0..1].
	 * \return The mixed value.
	 */
	static inline b3_f64 b3Mix(const b3_f64 a,const b3_f64 b,const b3_f64 x)
	{
		B3_ASSERT(x >= 0);
		B3_ASSERT(x <= 1);
		return a + x * (b - a);
	}

	/**
	 * This method returns the lower value of a or b.
	 *
	 * \param a One value.
	 * \param b Another value.
	 * \return The lower value of both given values.
	 */
	static inline b3_f64 b3Min(const b3_f64 a,const b3_f64 b)
	{
		return a < b ? a : b;
	}

	/**
	 * This method returns the greater value of a or b.
	 *
	 * \param a One value.
	 * \param b Another value.
	 * \return The greater value of both given values.
	 */
	static inline b3_f64 b3Max(const b3_f64 a,const b3_f64 b)
	{
		return a > b ? a : b;
	}

	static inline b3_f64 b3Hermite(
		const b3_f64 t1,
		const b3_f64 s0,
		const b3_f64 s1)
	{
		b3_f64 t2 = t1 * t1;
		b3_f64 t3 = t2 * t1;

		b3_f64 P4 = -2 * t3 + 3 * t2;
		b3_f64 R1 =      t3 - 2 * t2 + t1;
		b3_f64 R4 =      t3 -     t2;

		return P4 + s0 * R1 + s1 * R4;
	}

	/**
	 * This method is a smooth hermite ramp inside the range [s..e]
	 *
	 * \param s The start value of the ramp.
	 * \param e The end value of the ramp.
	 * \param x The value to interpolate.
	 * \return The hermite interpolation.
	 */
	static inline b3_f64 b3Smoothstep(
		const b3_f64 s,
		const b3_f64 e,
		const b3_f64 x)
	{
		if (x < s)
		{
			return 0;
		}
		else if (x > e)
		{
			return 1;
		}

		b3_f64 t = (x - s) / (e - s);

		return (3 - 2 * t) * t * t;
	}


	/**
	 * This method is a smooth hermite ramp inside the range [0..1]
	 *
	 * \param t The input value.
	 * \return The hermite interpolation.
	 */
	static inline b3_f64 b3Smoothstep(const b3_f64 t)
	{
		if (t < 0)
		{
			return 0;
		}
		else if (t > 1)
		{
			return 1;
		}

		return (3 - 2 * t) * t * t;
	}

	static inline b3_f64 b3SmoothPulse(
		const b3_f64 e0,
		const b3_f64 e1,
		const b3_f64 e2,
		const b3_f64 e3,
		const b3_f64 x)
	{
		return b3Smoothstep(e0,e1,x) - b3Smoothstep(e2,e3,x);
	}

	static inline b3_f64 b3SmoothPulseTrain(
		const b3_f64 e0,
		const b3_f64 e1,
		const b3_f64 e2,
		const b3_f64 e3,
		const b3_f64 x,
		const b3_f64 period)
	{
		return b3SmoothPulse(e0,e1,e2,e3,b3Math::b3Frac(x,period));
	}

	/**
	 *
	 * \param t
	 * \return
	 */
	static inline b3_f64 b3Fade(const b3_f64 t)
	{
		return t * t * t * (t * (t * 6 - 15) + 10);
	}

	/**
	 * This funtion is a fast pow() version for high integer exponents.
	 *
	 * \param x The base.
	 * \param exponent The exponent.
	 * \return The power of x with integer exponent exponent.
	 */
	static inline b3_f64 b3FastPow(const b3_f64 x,b3_u32 exponent)
	{
		b3_f64 result = 1;
		b3_f64 factor = x;

		while (exponent != 0)
		{
			if (exponent & 1)
			{
				result *= factor;
			}
			exponent = exponent >> 1;
			factor *= factor;
		}
		return result;
	}

	/**
	 * This method is the clamped version of the standard asin function.
	 *
	 * \param x The sine value.
	 * \return The arc sine value.
	 */
	static inline b3_f64 b3Asin(const b3_f64 x)
	{
		if (x <= -1)
		{
			return -M_PI * 0.5;
		}
		else if (x >= 1)
		{
			return M_PI * 0.5;
		}
		return asin(x);
	}

	/**
	 * This method is the clamped version of the standard acos function.
	 *
	 * \param x The cosine value.
	 * \return The arc cosine value.
	 */
	static inline b3_f64 b3Acos(const b3_f64 x)
	{
		if (x <= -1)
		{
			return M_PI;
		}
		else if (x >= 1)
		{
			return 0;
		}
		return acos(x);
	}

	static inline b3_f64 b3ArcAngleOfScalars(const b3_f64 u, const b3_f64 v)
	{
		return atan2(v,u) + (v < 0 ? (M_PI * 2.0) : 0);
	}

	static inline b3_f64 b3RelAngleOfScalars(const b3_f64 u, const b3_f64 v)
	{
		return atan2(v,u) * 0.5 / M_PI + (v < 0 ?   1.0 : 0);
	}

	static inline b3_f64 b3AngleOfScalars(const b3_f64 u, const b3_f64 v)
	{
		return atan2(v, u) * 180.0 / M_PI + (v < 0 ? 360.0 : 0);
	}


#ifndef CBRT_SYS
	/**
	 * This method computes the cubic root from a value.
	 *
	 * \param x The value to compute the cubic root from.
	 * \return The cubic root.
	 */
#ifdef  CBRT_SLOW
	static inline b3_f64 b3Cbrt(const b3_f64 x)
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
